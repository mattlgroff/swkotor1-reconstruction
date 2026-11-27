"""Check annotated matching sources against the [match] code rules.

Identical bytes alone are not a match (matching/CODE_RULES.md). A function
counts as matched under the rules only when status.csv says MATCH *and* its
source passes every [match] rule this checker tests:

    R1   one declaration per type (no differing copies of a class)
    R2   globals declared once, as extern in a header
    R4   typed members, never fixed offsets
    R13  no padding virtuals
    R17  no fake code (volatile carriers, empty blocks, padding members, ...)
    R18  no assembly bodies (unless the function is marked ORIGINAL-ASM)

A byte-identical function that fails one of them is MATCH-PENDING. The
checker also reports advisory findings that never block a match: open naming
work (R7), missing function header comments (R33) and addresses or scores in
comments (R21).

The checker reads source only. It needs neither the compiler nor the
original executable, so it runs anywhere and changes no bytes.

usage: python matching/tools/rulecheck.py [--report rules.csv] [-v] [PATH...]
"""
import argparse
import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MATCHING = ROOT / "matching"
ANNOTATION = re.compile(r"//\s*FUNCTION:\s*SWKOTOR\s+(0x[0-9A-Fa-f]+)")
MATCH_RULES = ("R1", "R2", "R4", "R13", "R17", "R18")

# ----------------------------------------------------------------------------
# Source views
# ----------------------------------------------------------------------------


def code_view(text):
    """The text with comments, string and character literals blanked out.

    Offsets and line breaks are preserved, so an offset in the view is the
    same offset in the original text. #define lines (with continuations) are
    blanked as well: a macro body is not a function body (R17's do/while(0)
    is allowed inside a macro)."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = " "
            i = j
        elif c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if text[k] != "\n":
                    out[k] = " "
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            for k in range(i + 1, min(j, n)):
                out[k] = " "
            i = j + 1
        else:
            i += 1
    view = "".join(out)
    # blank preprocessor #define lines, including backslash continuations
    lines = view.split("\n")
    in_define = False
    for idx, line in enumerate(lines):
        if in_define or re.match(r"\s*#\s*define\b", line):
            in_define = line.rstrip().endswith("\\")
            lines[idx] = " " * len(line)
    return "\n".join(lines)


def match_close(view, i, open_ch="{", close_ch="}"):
    """Offset of the bracket that closes the one at view[i], or -1."""
    depth = 0
    for j in range(i, len(view)):
        if view[j] == open_ch:
            depth += 1
        elif view[j] == close_ch:
            depth -= 1
            if depth == 0:
                return j
    return -1


class LineMap:
    def __init__(self, text):
        self.starts = [0]
        for m in re.finditer("\n", text):
            self.starts.append(m.end())
        self.lines = text.split("\n")

    def line(self, offset):
        """1-based line number of an offset."""
        lo, hi = 0, len(self.starts) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if self.starts[mid] <= offset:
                lo = mid
            else:
                hi = mid - 1
        return lo + 1

    def raw(self, lineno):
        return self.lines[lineno - 1] if 0 < lineno <= len(self.lines) else ""


IDENT = re.compile(r"\b[A-Za-z_]\w*\b")

# ----------------------------------------------------------------------------
# Declarations: types and globals
# ----------------------------------------------------------------------------

TYPE_DEF = re.compile(
    r"\b(class|struct|union|enum)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)\s*"
    r"(?::(?![:])[^;{}()]*)?\{")
FORWARD = re.compile(r"(?:^|[;{}])\s*(class|struct)\s+([A-Za-z_]\w*)\s*;")
PAD_MEMBER = re.compile(r"\b(pad\d*|_pad\w*|\w+_pad\d*)\s*(?:\[[^\]]*\]\s*)*;")
PAD_VIRTUAL = re.compile(r"\bvirtual\b[^;{}]*?\b((?:pad|dummy|Pad|Dummy|PAD|DUMMY)\w*)\s*\(")
VOLATILE = re.compile(r"\bvolatile\b")
VOLATILE_NOTE = re.compile(r"//\s*volatile:")


def normalize(s):
    return re.sub(r"\s+", " ", s).strip()


class Unit:
    """One source or header file."""

    def __init__(self, path, text):
        self.path = path
        self.text = text
        self.view = code_view(text)
        self.lines = LineMap(text)
        self.includes = [m.group(1) for m in
                         re.finditer(r'^\s*#\s*include\s*"([^"]+)"', text, re.M)]
        self.types = []      # (keyword, name, normalized body, line, type findings)
        self.forwards = []   # (keyword, name, line)
        self.externs = []    # (name, normalized type, line, is_function)
        self._scan_types()
        self._scan_externs()

    def _scan_types(self):
        v = self.view
        for m in TYPE_DEF.finditer(v):
            keyword, name = m.group(1), m.group(2)
            brace = m.end() - 1
            end = match_close(v, brace)
            if end < 0:
                continue
            body = v[brace + 1:end]
            findings = []
            if keyword != "enum":
                for pm in PAD_MEMBER.finditer(body):
                    findings.append(("R17", f"padding member {pm.group(1)} in {name}"))
                for pv in PAD_VIRTUAL.finditer(body):
                    findings.append(("R13", f"padding virtual {pv.group(1)} in {name}"))
                for vm in VOLATILE.finditer(body):
                    line = self.lines.line(brace + 1 + vm.start())
                    if not VOLATILE_NOTE.search(self.lines.raw(line)):
                        findings.append(("R17", f"volatile member in {name} without a "
                                                f"// volatile: <evidence> note"))
            self.types.append((keyword, name, normalize(body), self.lines.line(m.start()), findings))
        for m in FORWARD.finditer(v):
            self.forwards.append((m.group(1), m.group(2), self.lines.line(m.start(1))))

    def _scan_externs(self):
        v = self.view
        for m in re.finditer(r"\bextern\b", v):
            # extern "C" { ... } blocks: the declarations inside are seen on
            # their own when they carry extern themselves; skip the block.
            rest = v[m.end():m.end() + 400]
            semi = rest.find(";")
            brace = rest.find("{")
            if semi < 0 or (0 <= brace < semi):
                continue
            stmt = rest[:semi]
            stmt = re.sub(r'^\s*"\s*"\s*', "", stmt)  # extern "C" (blanked literal)
            stmt = re.sub(r"__declspec\s*\([^)]*\)", "", stmt)
            line = self.lines.line(m.start())
            fptr = re.search(r"\(\s*(?:__\w+\s+)?\*\s*([A-Za-z_]\w*)\s*\)", stmt)
            if fptr:
                self.externs.append((fptr.group(1), normalize(stmt.replace(fptr.group(1), "")),
                                     line, False))
            elif "(" in stmt:
                head = stmt.split("(")[0].split()
                if head:
                    self.externs.append((head[-1].lstrip("*&"), "", line, True))
            else:
                for decl in stmt.split(","):
                    dm = re.search(r"([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)$", decl.strip())
                    if dm:
                        typ = decl.strip()[:dm.start()] + dm.group(2)
                        self.externs.append((dm.group(1), normalize(typ), line, False))
                        break  # later declarators share the base type; one is enough


# ----------------------------------------------------------------------------
# Functions
# ----------------------------------------------------------------------------

BYTE_CAST = (r"\(\s*(?:const\s+)?(?:unsigned\s+)?(?:char|BYTE|byte|int|long|DWORD|"
             r"unsigned)\s*(?:const\s*)?\*?\s*\)")
OPERAND = r"\s*\(?\s*(this|[A-Za-z_]\w*(?:\s*(?:->|\.)\s*\w+)*)\s*\)?"
OFFSET_ARITH = re.compile(BYTE_CAST + OPERAND + r"\s*[+-]\s*(0x[0-9A-Fa-f]+|\d+)\b")
DEREF_CAST_BEFORE = re.compile(r"\(\s*(?:const\s+)?[A-Za-z_][\w:\s<>,]*\*+\s*\)\s*\(\s*$")
THIS_AS_ARRAY = re.compile(r"\(\s*\(\s*[A-Za-z_][\w\s]*\*\s*\)\s*this\s*\)\s*\[")
ASM = re.compile(r"\b(__asm|_asm|__emit|_emit)\b")
NAKED = re.compile(r"__declspec\s*\(\s*naked\s*\)")
SELF_ASSIGN = re.compile(
    r"(?<![\w.>\]])([A-Za-z_]\w*(?:\s*(?:->|\.)\s*\w+)*)\s*=(?!=)\s*\1\s*;")
PLUS_ZERO = re.compile(r"(?<![eE])(?<=[\w)\]])\s*\+\s*0(?![\w.])")
TIMES_ONE = re.compile(r"(?<=[\w)\]])\s*\*\s*1(?![\w.])")
PROBE = re.compile(r"//\s*(?:PROBE|probe)\b")

PLACEHOLDER = re.compile(
    r"\b(?:FUN_|DAT_|sub_|unk_|local_|param_|field_0x|m_Unk|UnkType_)\w*"
    r"|\b[iupbcfs]Var\d+\b|\buStack\w*|\bvar\d+\b"
    r"|\bLeaf[0-9A-Fa-f]{4,}\b"                    # CExoResMan::Leaf9460
    r"|\bv[0-9a-f]{2,3}\b"                         # vtable slot v0c()
    r"|\bm_[a-z]{1,3}[0-9a-f]{2,4}\b"              # offset-named member m_n04
    r"|\bpad_0x[0-9A-Fa-f]+\b"
    r"|\b\w*Unknown[0-9A-Fa-f]+\w*\b"
    r"|\b[A-Za-z_]*?_?(?=[0-9a-fA-F]*[0-9])[0-9a-fA-F]{6,8}\b")
COMMENT_ADDRESS = re.compile(r"\b0x00[4-7][0-9A-Fa-f]{5}\b")
SCORE_NOTE = re.compile(r"\bNONMATCH\b|\bdiff insns\b|\b\d+\s+insns?\b|\b\d+%")


class Function:
    def __init__(self, unit, address, line, sig_start, brace, end):
        self.unit = unit
        self.address = address
        self.line = line
        self.sig_start = sig_start
        self.brace = brace
        self.end = end
        self.blocking = []
        self.advisory = []
        self.original_asm = False
        self.symbol_text = None  # set when the function has no body of its own

    @property
    def has_body(self):
        return self.brace >= 0

    @property
    def text(self):
        if not self.has_body:
            return self.symbol_text or ""
        return self.unit.view[self.sig_start:self.end + 1]

    @property
    def signature(self):
        return self.unit.view[self.sig_start:self.brace]

    @property
    def body(self):
        return self.unit.view[self.brace + 1:self.end]


def find_functions(unit):
    """Every annotated function in a unit.

    An annotation owns the definition that starts on the next code line. An
    annotation followed by a blank line, another annotation or a declaration
    (`template void X<T>::f();`) has no body of its own: the function is
    compiler-generated (deleting destructors, vtordisp thunks) or an explicit
    instantiation. Such a function is judged by the types its SYMBOL and
    declaration name, with an empty body."""
    found = []
    v, lines = unit.view, unit.lines
    for m in ANNOTATION.finditer(unit.text):
        address = int(m.group(1), 16)
        line = lines.line(m.start())
        n = line + 1
        while n <= len(lines.lines) and lines.raw(n).strip().startswith("//") \
                and not ANNOTATION.search(lines.raw(n)):
            n += 1
        nxt = lines.raw(n).strip()
        start = lines.starts[n - 1] if n - 1 < len(lines.starts) else len(v)
        fn = None
        if nxt and not ANNOTATION.search(nxt):
            fn = definition_at(unit, address, line, start)
        if fn is None:
            stop = lines.starts[n] if n < len(lines.starts) else len(v)
            fn = Function(unit, address, line, lines.starts[line - 1], -1, -1)
            fn.symbol_text = re.sub(r"[^\w]+", " ", m.string[m.start():m.string.find("\n", m.start())])
            if nxt and not ANNOTATION.search(nxt) and start < stop:
                fn.symbol_text += " " + v[start:stop]
        found.append(fn)
    return found


def definition_at(unit, address, line, start):
    v = unit.view
    paren = v.find("(", start)
    if paren < 0:
        return None
    close = match_close(v, paren, "(", ")")
    if close < 0:
        return None
    j, depth = close + 1, 0
    while j < len(v):
        ch = v[j]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif depth == 0 and ch == ";":
            return None  # a declaration, not a definition
        elif depth == 0 and ch == "{":
            end = match_close(v, j)
            return Function(unit, address, line, start, j, end) if end >= 0 else None
        j += 1
    return None


def check_body(fn):
    """[match] and advisory findings that live inside one function."""
    u, body, base = fn.unit, fn.body, fn.brace + 1
    text = fn.text

    def line_of(off):
        return u.lines.line(base + off)

    # R18: assembly bodies
    above = "\n".join(u.lines.raw(n) for n in range(max(1, fn.line - 8), fn.line + 1))
    if ASM.search(body) or NAKED.search(fn.signature):
        if "ORIGINAL-ASM" in above:
            fn.original_asm = True
        else:
            asm_lines = [line_of(m.start()) for m in ASM.finditer(body)]
            helper = asm_lines and all("ASM-HELPER:" in u.lines.raw(n) for n in asm_lines)
            if NAKED.search(fn.signature) or not helper:
                fn.blocking.append(("R18", "assembly body"))

    # R4: fixed offsets
    for m in OFFSET_ARITH.finditer(body):
        before = body[max(0, m.start() - 120):m.start()]
        if m.group(1) == "this" or DEREF_CAST_BEFORE.search(before):
            fn.blocking.append(("R4", f"offset cast at line {line_of(m.start())}: "
                                      f"{normalize(m.group(0))}"))
    for m in THIS_AS_ARRAY.finditer(body):
        fn.blocking.append(("R4", f"this indexed as an array at line {line_of(m.start())}"))

    # R17: fake code
    for m in VOLATILE.finditer(body):
        n = line_of(m.start())
        if not VOLATILE_NOTE.search(u.lines.raw(n)):
            fn.blocking.append(("R17", f"volatile local at line {n}"))
    for m in re.finditer(r"\bif\s*\(", body):
        close = match_close(body, m.end() - 1, "(", ")")
        if close < 0:
            continue
        after = body[close + 1:close + 40]
        if re.match(r"\s*(\{\s*\}|;)", after):
            fn.blocking.append(("R17", f"empty if at line {line_of(m.start())}"))
    for m in re.finditer(r"\belse\s*(\{\s*\}|;)", body):
        fn.blocking.append(("R17", f"empty else at line {line_of(m.start())}"))
    for m in re.finditer(r"\}\s*while\s*\(\s*0\s*\)", body):
        fn.blocking.append(("R17", f"do {{ }} while (0) outside a macro at line "
                                   f"{line_of(m.start())}"))
    for m in SELF_ASSIGN.finditer(body):
        fn.blocking.append(("R17", f"self-assignment at line {line_of(m.start())}"))
    for rx, what in ((PLUS_ZERO, "+ 0"), (TIMES_ONE, "* 1")):
        for m in rx.finditer(body):
            fn.blocking.append(("R17", f"'{what}' at line {line_of(m.start())}"))
    # bare ';' statements: after ';' or '{', or after a block's closing '}'
    paren, stack, prev, prev_block = 0, [], "", False
    for i, ch in enumerate(body):
        if ch.isspace():
            continue
        if ch == "(":
            paren += 1
        elif ch == ")":
            paren -= 1
        elif ch == "{":
            p = body[:i].rstrip()
            stack.append(p.endswith((")", ";", "{", "}")) or p.endswith("else")
                         or p.endswith("do") or p == "")
        elif ch == "}":
            prev_block = stack.pop() if stack else False
            prev = ch
            continue
        elif ch == ";" and paren == 0:
            if prev in (";", "{") or (prev == "}" and prev_block):
                fn.blocking.append(("R17", f"bare ';' statement at line {line_of(i)}"))
        prev = ch
    for n in range(u.lines.line(fn.brace), u.lines.line(fn.end) + 1):
        if PROBE.search(u.lines.raw(n)):
            fn.blocking.append(("R17", f"statement marked as a probe at line {n}"))

    # advisory: R7 open naming
    names = sorted(set(m.group(0) for m in PLACEHOLDER.finditer(text)))
    if names:
        shown = ", ".join(names[:6]) + (", ..." if len(names) > 6 else "")
        fn.advisory.append(("R7", f"{len(names)} placeholder name(s): {shown}"))

    # advisory: R33 header comment, R21 comment content
    prev_line = u.lines.raw(fn.line - 1).strip() if fn.line > 1 else ""
    if not (prev_line.startswith("//") or prev_line.endswith("*/")) \
            or ANNOTATION.search(prev_line):
        fn.advisory.append(("R33", "no header comment"))
    comment_lines = []
    n = fn.line - 1
    while n > 0 and u.lines.raw(n).strip().startswith("//"):
        comment_lines.append(u.lines.raw(n))
        n -= 1
    for k in range(u.lines.line(fn.brace), u.lines.line(fn.end) + 1):
        raw = u.lines.raw(k)
        if "//" in raw:
            comment_lines.append(raw[raw.index("//"):])
    comments = "\n".join(c for c in comment_lines if not ANNOTATION.search(c))
    if COMMENT_ADDRESS.search(comments):
        fn.advisory.append(("R21", "address in a comment"))
    if SCORE_NOTE.search(comments):
        fn.advisory.append(("R21", "score or campaign note in a comment"))


# ----------------------------------------------------------------------------
# Whole-tree analysis
# ----------------------------------------------------------------------------


class Tree:
    def __init__(self, root=MATCHING):
        self.root = root = Path(root).resolve()
        self.units = {}
        for sub in ("include", "src"):
            for path in sorted((root / sub).rglob("*")):
                if path.suffix.lower() in (".h", ".hpp", ".inl", ".c", ".cpp"):
                    text = path.read_text(encoding="utf-8", errors="replace")
                    self.units[path] = Unit(path, text)
        self._closure = {}
        self._analyse_types()
        self._analyse_externs()

    def resolve(self, unit, inc):
        for base in (unit.path.parent, self.root / "include"):
            p = (base / inc).resolve()
            if p in self.units:
                return p
        return None

    def closure(self, path):
        """The file plus every project header it includes, transitively."""
        if path in self._closure:
            return self._closure[path]
        seen, todo = {path}, [path]
        while todo:
            u = self.units[todo.pop()]
            for inc in u.includes:
                p = self.resolve(u, inc)
                if p and p not in seen:
                    seen.add(p)
                    todo.append(p)
        self._closure[path] = seen
        return seen

    def _analyse_types(self):
        defs = defaultdict(list)       # name -> [(path, keyword, body, line)]
        for path, u in self.units.items():
            for keyword, name, body, line, _ in u.types:
                defs[name].append((path, keyword, body, line))
        self.type_defs = defs
        self.conflicts = {}             # name -> description, for R1
        self.copies = {}                # name -> number of files (identical copies)
        for name, ds in defs.items():
            files = {d[0] for d in ds}
            if len(files) < 2:
                continue
            bodies = {d[2] for d in ds}
            keywords = {d[1] for d in ds}
            if len(bodies) > 1:
                self.conflicts[name] = (f"{name} has {len(bodies)} different definitions "
                                        f"in {len(files)} files")
            elif len(keywords) > 1:
                self.conflicts[name] = f"{name} is defined as both class and struct"
            else:
                self.copies[name] = len(files)
        for path, u in self.units.items():
            for keyword, name, line in u.forwards:
                kws = {d[1] for d in defs.get(name, ()) if d[1] in ("class", "struct")}
                if kws and keyword not in kws and name not in self.conflicts:
                    self.conflicts[name] = (f"{name} is forward-declared as {keyword} but "
                                            f"defined as {'/'.join(sorted(kws))}")

    def _analyse_externs(self):
        types = defaultdict(set)
        for path, u in self.units.items():
            for name, typ, line, is_fn in u.externs:
                if not is_fn:
                    types[name].add(typ)
        self.extern_conflicts = {n for n, t in types.items() if len(t) > 1}

    def check(self, fn):
        path = fn.unit.path
        idents = set(IDENT.findall(fn.text))
        vis = self.closure(path)
        is_header = path.suffix.lower() in (".h", ".hpp", ".inl")
        # R1, and type-level R13/R17 findings, for every type the function names
        for p in vis:
            for keyword, name, body, line, findings in self.units[p].types:
                if name not in idents:
                    continue
                if name in self.conflicts:
                    where = "local copy" if p == path else f"via {p.relative_to(self.root)}"
                    fn.blocking.append(("R1", f"{self.conflicts[name]} ({where})"))
                for rule, what in findings:
                    fn.blocking.append((rule, what))
            for keyword, name, line in self.units[p].forwards:
                if name in idents and name in self.conflicts:
                    fn.blocking.append(("R1", self.conflicts[name]))
        # R2: externs of globals outside headers, and globals with two types
        for name, typ, line, is_fn in fn.unit.externs:
            if is_fn or name not in idents:
                continue
            if not is_header:
                fn.blocking.append(("R2", f"extern {name} declared in the unit, not a header"))
            if name in self.extern_conflicts:
                fn.blocking.append(("R2", f"{name} is declared with different types"))
        if fn.has_body:
            check_body(fn)
        fn.blocking = list(dict.fromkeys(fn.blocking))


def read_status(path):
    status = {}
    if path.exists():
        with path.open(newline="", encoding="utf-8") as f:
            for row in csv.DictReader(f):
                status[int(row["address"], 16)] = (row["name"], row["status"])
    return status


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("paths", nargs="*", help="files or folders to report on (default: matching/src)")
    ap.add_argument("--status", default=str(MATCHING / "status.csv"))
    ap.add_argument("--report", help="write one CSV row per annotated function")
    ap.add_argument("-v", "--verbose", action="store_true",
                    help="print every MATCH-PENDING function with its findings")
    ap.add_argument("--top", type=int, default=15,
                    help="list the N types with the most conflicting definitions")
    args = ap.parse_args(argv)

    tree = Tree()
    status = read_status(Path(args.status))
    targets = [Path(p).resolve() for p in args.paths] or [(MATCHING / "src").resolve()]

    def selected(path):
        return any(path == t or t in path.parents for t in targets)

    functions = []
    for path, unit in tree.units.items():
        if selected(path.resolve()):
            for fn in find_functions(unit):
                tree.check(fn)
                functions.append(fn)
    functions.sort(key=lambda f: f.address)

    rows, counts, by_rule, advisory = [], Counter(), Counter(), Counter()
    for fn in functions:
        name, bytes_status = status.get(fn.address, ("?", "UNKNOWN"))
        rules = sorted({r for r, _ in fn.blocking})
        if fn.original_asm:
            verdict = "ORIGINAL-ASM"
        elif bytes_status != "MATCH":
            verdict = bytes_status
        elif rules:
            verdict = "MATCH-PENDING"
            by_rule.update(rules)
        else:
            verdict = "MATCH"
        counts[verdict] += 1
        if bytes_status == "MATCH":
            advisory.update({r for r, _ in fn.advisory})
        rel = fn.unit.path.relative_to(ROOT).as_posix()
        rows.append({
            "address": f"0x{fn.address:08X}", "name": name, "bytes": bytes_status,
            "rules": verdict, "blocking": " | ".join(f"{r}: {w}" for r, w in fn.blocking),
            "advisory": " | ".join(f"{r}: {w}" for r, w in fn.advisory),
            "file": rel, "line": fn.line})
        if args.verbose and verdict == "MATCH-PENDING":
            print(f"{rel}:{fn.line}: 0x{fn.address:08x} {name}: MATCH-PENDING")
            for r, w in fn.blocking:
                print(f"    {r}: {w}")

    if args.report:
        with open(args.report, "w", newline="", encoding="utf-8") as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0]) if rows else ["address"])
            w.writeheader()
            w.writerows(rows)

    bytes_matched = sum(1 for r in rows if r["bytes"] == "MATCH")
    print(f"annotated functions:        {len(rows)}")
    print(f"bytes matched (status.csv): {bytes_matched}")
    print(f"matched under the rules:    {counts['MATCH']}")
    print(f"MATCH-PENDING:              {counts['MATCH-PENDING']}")
    for rule in MATCH_RULES:
        if by_rule[rule]:
            print(f"    {rule:4} blocks {by_rule[rule]}")
    print(f"ORIGINAL-ASM:               {counts['ORIGINAL-ASM']}")
    print("advisory, in byte-matched functions:")
    for rule, label in (("R7", "open naming work"), ("R33", "no header comment"),
                        ("R21", "address or score in comments")):
        print(f"    {rule:4} {label}: {advisory[rule]}")
    if args.top and tree.conflicts:
        print(f"types with the most conflicting definitions (R1), top {args.top}:")
        ranked = sorted(tree.conflicts,
                        key=lambda n: -len({d[0] for d in tree.type_defs.get(n, ())}))
        for name in ranked[:args.top]:
            print(f"    {tree.conflicts[name]}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
