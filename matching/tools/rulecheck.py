"""Check annotated functions against the [match] code rules.

A function whose bytes match is counted as matched only when it also passes
every [match] rule (matching/CODE_RULES.md). This tool applies the
mechanical checks for those rules to the source of every annotated
function, without a compiler:

  R1   one declaration per type: a class/struct name defined in two files
       with different bodies, or forward-declared with the other keyword
  R2   one declaration per global: an `extern` data declaration in a .cpp,
       or one global declared with different types in different files
  R4   no offset casts: (T*)((char*)p + N), or `this` in byte arithmetic
  R13  real vtables: virtuals named pad/dummy
  R17  no fake code: volatile, unused scalar locals, empty if/else blocks,
       bare `;` statements, do { } while (0), self-assignment, `+ 0`, `* 1`,
       pad/dummy members
  R18  no assembly: __asm/_asm/__emit outside the helpers listed in
       CODE_RULES.md

Each row is deliberately narrow (CODE_RULES.md, "Checker rows"). It also
reports advisory counts that never block a match: placeholder names (R7),
local function prototypes (R25) and `#if 0` blocks (R37).

    python matching/tools/rulecheck.py                    # summary
    python matching/tools/rulecheck.py --list R4 R18      # findings by rule
    python matching/tools/rulecheck.py --report matching/rules.csv
    python matching/tools/rulecheck.py matching/src/l     # one folder

`--report` writes one row per annotated function with its byte status from
status.csv and its rule status: MATCH (bytes and rules), MATCH-PENDING
(bytes only) or NONMATCH. The headline is "matched under the rules".
"""
import argparse
import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MATCHING = ROOT / "matching"
INCLUDE = MATCHING / "include"

ANNOTATION = re.compile(r"//\s*FUNCTION:\s*SWKOTOR\s+(0x[0-9A-Fa-f]+)")
# Inline-assembly helpers the original had (CODE_RULES.md, R18). A function
# may call them; their own bodies are not annotated functions.
ASM_HELPERS = {"FloatToInt"}
KEYWORDS = {
    "return", "delete", "goto", "else", "case", "throw", "new", "sizeof",
    "typedef", "using", "do", "if", "while", "for", "switch", "break",
    "continue", "default", "operator", "template", "typename", "public",
    "private", "protected", "struct", "class", "enum", "union", "static",
    "const", "unsigned", "signed", "volatile", "register", "inline",
}
SCALARS = (r"(?:unsigned\s+|signed\s+)?(?:char|short|int|long|float|double|"
           r"bool|BOOL|BYTE|WORD|DWORD|__int64)")


def blank(text):
    """Text with comments and string/char literals replaced by spaces.

    Offsets and newlines are kept, so positions map back to the source."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            j = n if j < 0 else j
            out[i:j] = " " * (j - i)
            i = j
        elif c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out[i:j] = [ch if ch == "\n" else " " for ch in text[i:j]]
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out[i + 1:j - 1] = " " * max(0, j - 1 - (i + 1))
            i = j
        else:
            i += 1
    return "".join(out)


def match_close(text, start, open_ch, close_ch):
    """Index of the bracket closing text[start] (== open_ch), or -1."""
    depth = 0
    for i in range(start, len(text)):
        if text[i] == open_ch:
            depth += 1
        elif text[i] == close_ch:
            depth -= 1
            if depth == 0:
                return i
    return -1


def line_of(text, pos):
    return text.count("\n", 0, pos) + 1


class Unit:
    """One source or header file, blanked, with its includes."""

    def __init__(self, path):
        self.path = path
        self.raw = path.read_text(encoding="utf-8", errors="replace")
        self.text = blank(self.raw)
        self.includes = re.findall(r'^\s*#\s*include\s*"([^"]+)"', self.raw, re.M)


def resolve_include(name, here):
    for base in (here.parent, INCLUDE):
        p = (base / name).resolve()
        if p.exists():
            return p
    return None


# ---------------------------------------------------------------- types (R1)

TYPE_DEF = re.compile(r"(?<![\w:])(class|struct)\s+([A-Za-z_]\w*)\s*(?::[^{;()]*)?\{")
TYPE_FWD = re.compile(r"(?<![\w:<,])(class|struct)\s+([A-Za-z_]\w*)\s*;")


def normalise(body):
    return re.sub(r"\s+", "", body)


def collect_types(units):
    """{name: {normalised body: [files]}}, {name: {keyword: [files]}}."""
    bodies = defaultdict(lambda: defaultdict(list))
    keywords = defaultdict(lambda: defaultdict(list))
    for u in units:
        t = u.text
        for m in TYPE_DEF.finditer(t):
            close = match_close(t, m.end() - 1, "{", "}")
            if close < 0:
                continue
            bodies[m.group(2)][normalise(t[m.end():close])].append(u.path)
            keywords[m.group(2)][m.group(1)].append(u.path)
        for m in TYPE_FWD.finditer(t):
            if re.search(r"\bfriend\s*$", t[:m.start()]):
                continue
            keywords[m.group(2)][m.group(1)].append(u.path)
    return bodies, keywords


# -------------------------------------------------------------- globals (R2)

def extern_data(text):
    """[(name, normalised type, offset)] for top-level extern data declarations."""
    found = []
    for m in re.finditer(r"(?m)^[ \t]*extern\b(?!\s*\"C\"\s*\{)", text):
        end = text.find(";", m.end())
        if end < 0:
            continue
        decl = text[m.end():end]
        decl = re.sub(r'^\s*"C(?:\+\+)?"', "", decl).strip()
        if "{" in decl or "}" in decl:
            continue
        # function prototypes are R25 business, function pointers are data
        if "(" in decl and not re.search(r"\(\s*(?:__\w+\s+)?\*", decl):
            continue
        name = None
        fp = re.search(r"\(\s*(?:__\w+\s+)?\*\s*(?:const\s+)?([A-Za-z_]\w*)\s*\)", decl)
        if fp:
            name = fp.group(1)
        else:
            nm = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*$", decl)
            if nm:
                name = nm.group(1)
        if not name or name in KEYWORDS:
            continue
        typ = normalise(decl.replace(name, "@", 1))
        typ = re.sub(r"\[[^\]]*\]", "[]", typ)  # extern T a[]; and a[N]; agree
        found.append((name, typ, m.start()))
    return found


# ------------------------------------------------------------ functions

def function_bodies(unit):
    """[(address, name, body start, body end)] for each annotated definition."""
    out = []
    raw, text = unit.raw, unit.text
    for m in ANNOTATION.finditer(raw):
        pos = raw.find("\n", m.end())
        if pos < 0:
            continue
        paren = 0
        body = None
        i = pos
        while i < len(text):
            c = text[i]
            if c == "(":
                paren += 1
            elif c == ")":
                paren -= 1
            elif c == ";" and paren == 0:
                break  # a declaration, not a definition
            elif c == "{" and paren == 0:
                close = match_close(text, i, "{", "}")
                if close > 0:
                    body = (i, close + 1)
                break
            elif c == "#":
                nl = text.find("\n", i)
                i = len(text) if nl < 0 else nl
                continue
            i += 1
        head = re.sub(r"\s+", " ", text[pos:body[0]]).strip() if body else ""
        head = re.sub(r"__declspec\s*\([^)]*\)", "", head).strip()
        if body is None or "(" not in head or \
                re.match(r"(?:template\s*<[^>]*>\s*)?(?:class|struct|union|enum|namespace)\b", head):
            # explicit instantiation, compiler-generated member or a
            # declaration: the body is elsewhere
            out.append((int(m.group(1), 16), "?", "", None, None))
            continue
        name = head.split("(")[0].split()[-1].lstrip("*&") if "(" in head else "?"
        out.append((int(m.group(1), 16), name, head, body[0], body[1]))
    return out


IF_HEAD = re.compile(r"\b(if|else\s+if)\s*\(")
BYTE_PTR = r"\(\s*(?:const\s+)?(?:unsigned\s+)?(?:char|BYTE|byte|uint8)\s*\*\s*\)"
OFFSET_CAST = re.compile(
    r"\(\s*(?:const\s+)?[A-Za-z_][\w:<>\s]*?\*+\s*(?:const\s*)?\)\s*\(\s*" + BYTE_PTR +
    r"\s*\(?\s*[\w.\->]+\s*\)?\s*[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\)")
THIS_ARITH = re.compile(BYTE_PTR + r"\s*\(?\s*this\s*\)?\s*[+-]")
ASM = re.compile(r"\b(?:__asm|_asm|__emit)\b")
# A pointer cast of an image address: a global with no declaration (project
# tightening of R2; the image is based at 0x00400000).
ABS_ADDRESS = re.compile(
    r"\(\s*(?:const\s+)?[A-Za-z_][\w\s:<>]*?\*+\s*\)\s*(0x00[4-9A-Fa-f][0-9A-Fa-f]{5})\b")
SELF_ASSIGN = re.compile(r"(?:^|[;{}])\s*([A-Za-z_][\w.\->\[\]]*)\s*=\s*\1\s*;")
PLUS_ZERO = re.compile(r"(?<=[\w)\]])\s*\+\s*0(?![\w.])")
TIMES_ONE = re.compile(r"(?<=[\w)\]])\s*\*\s*1(?![\w.])")
SCALAR_DECL = re.compile(
    r"(?:^|[;{}])\s*(?:const\s+)?(" + SCALARS + r"|[A-Za-z_]\w*\s*\*+)\s*\*?\s*"
    r"([A-Za-z_]\w*)\s*(=[^;]*)?;")


def body_findings(body, head, raw_body):
    """[(rule, detail)] for one function body (blanked text and raw text)."""
    f = []
    lines = [ln if not ln.lstrip().startswith("#") else "" for ln in body.split("\n")]
    b = "\n".join(lines)
    if OFFSET_CAST.search(b):
        f.append(("R4", "typed cast of byte-pointer offset arithmetic"))
    if THIS_ARITH.search(b):
        f.append(("R4", "`this` in byte-pointer arithmetic"))
    m = ABS_ADDRESS.search(b)
    if m:
        f.append(("R2", f"global read through its address {m.group(1)}"))
    if re.search(r"\bvolatile\b", b):
        f.append(("R17", "volatile"))
    if ASM.search(b):
        f.append(("R18", "inline assembly in the function body"))
    if re.search(r"\}\s*while\s*\(\s*0\s*\)", b):
        f.append(("R17", "do { } while (0) in a function body"))
    m = SELF_ASSIGN.search(b)
    if m:
        f.append(("R17", f"self-assignment of {m.group(1)}"))
    if PLUS_ZERO.search(b):
        f.append(("R17", "`+ 0`"))
    if TIMES_ONE.search(b):
        f.append(("R17", "`* 1`"))
    # empty if / else blocks, unless the block labels an original quirk (R36)
    def labelled(start, end):
        return re.search(r"//\s*(NOTE|BUG|R36)\b", raw_body[start:end + 1]) is not None

    for m in IF_HEAD.finditer(b):
        close = match_close(b, m.end() - 1, "(", ")")
        if close < 0:
            continue
        e = re.match(r"\s*(\{\s*\}|;)", b[close + 1:])
        if e and not labelled(close + 1, close + 1 + e.end()):
            f.append(("R17", "empty if block"))
            break
    for m in re.finditer(r"\belse\s*(\{\s*\}|;)", b):
        if not labelled(m.start(), m.end()):
            f.append(("R17", "empty else block"))
            break
    # bare `;` statements: after `{`, `;` or a block's `}`, at paren depth 0
    depth = 0
    kinds = []
    prev = ""
    for i, c in enumerate(b):
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        elif c == "{":
            kinds.append("init" if prev in "=,(" or prev == "{" and kinds and kinds[-1] == "init" else "block")
        elif c == "}":
            kind = kinds.pop() if kinds else "block"
            prev = "}" if kind == "block" else "i}"
            continue
        elif c == ";" and depth == 0 and prev in ("{", ";", "}") and prev != "":
            f.append(("R17", "bare `;` statement"))
            break
        if not c.isspace():
            prev = c
    # unused scalar/pointer locals: declared, never named again
    params = head[head.find("("):] if "(" in head else ""
    inner = b[1:]
    raw_lines = raw_body[1:].split("\n")
    for m in SCALAR_DECL.finditer(inner):
        name = m.group(2)
        if name in KEYWORDS or name == "this" or name in params:
            continue
        ln = inner.count("\n", 0, m.end())
        label = " ".join(raw_lines[max(0, ln - 1):ln + 1])
        if re.search(r"//\s*(NOTE|BUG|R36)\b", label):
            continue  # an original quirk, labelled (R36)
        uses = len(re.findall(r"\b" + re.escape(name) + r"\b", inner))
        if uses == 1:
            f.append(("R17", f"local {name} is never used"))
            break
    return f


# Bare `pad`/`_pad` members only: members named by their offset (`pad_0x1c`,
# `pad28`, `pad0`) are placeholders for an unknown range (R27, open R7 work).
MEMBER_PAD = re.compile(r"\b(_?pad)\s*(?:\[[^\]]*\]\s*)?;")
VIRTUAL_PAD = re.compile(r"\bvirtual\b[^;{}]*\b(_?[Pp]ad\w*|_?[Dd]ummy\w*)\s*\(")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="*", default=[str(MATCHING / "src")])
    ap.add_argument("--status", default=str(MATCHING / "status.csv"))
    ap.add_argument("--report", help="write a CSV with one row per annotated function")
    ap.add_argument("--list", nargs="*", metavar="RULE",
                    help="print each finding (all rules, or the ones named)")
    args = ap.parse_args()

    status = {}
    with open(args.status, newline="") as fh:
        for row in csv.DictReader(fh):
            status[int(row["address"], 16)] = row["status"]

    every = sorted(set((MATCHING / "src").rglob("*.cpp")) | set(INCLUDE.rglob("*.h")))
    units = {p.resolve(): Unit(p) for p in every}
    wanted = []
    for p in args.paths:
        p = Path(p).resolve()
        wanted += sorted(p.rglob("*.cpp")) if p.is_dir() else [p]

    # R1: types defined twice with different bodies, or both keywords
    bodies, keywords = collect_types(units.values())
    dup_types = {n for n, v in bodies.items() if len(v) > 1}
    kw_types = {n for n, v in keywords.items() if len(v) > 1}

    # R2: every extern data declaration, by name
    decls = defaultdict(list)  # name -> [(type, path)]
    for u in units.values():
        for name, typ, _ in extern_data(u.text):
            decls[name].append((typ, u.path))
    conflicting = {n for n, v in decls.items() if len({t for t, _ in v}) > 1}
    # globals and members declared volatile without a `// volatile: <evidence>` note
    volatile_names = defaultdict(set)  # file -> names
    for u in units.values():
        raw_lines = u.raw.split("\n")
        for m in re.finditer(r"\bvolatile\b[^;(){}=]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*;", u.text):
            ln = line_of(u.text, m.start()) - 1
            if "volatile:" not in " ".join(raw_lines[max(0, ln - 1):ln + 1]):
                volatile_names[u.path].add(m.group(1))

    # types with pad/dummy members, virtual pad slots (R17, R13)
    pad_types, vpad_types = set(), set()
    for u in units.values():
        t = u.text
        for m in TYPE_DEF.finditer(t):
            close = match_close(t, m.end() - 1, "{", "}")
            if close < 0:
                continue
            body = t[m.end():close]
            if MEMBER_PAD.search(body):
                pad_types.add(m.group(2))
            if VIRTUAL_PAD.search(body):
                vpad_types.add(m.group(2))

    def visible(path, seen=None):
        """Files a translation unit sees: itself and its transitive includes."""
        seen = set() if seen is None else seen
        if path in seen or path not in units:
            return seen
        seen.add(path)
        for inc in units[path].includes:
            r = resolve_include(inc, path)
            if r:
                visible(r, seen)
        return seen

    rows, findings = [], []
    advisory = Counter()
    placeholder = re.compile(r"\b(?:FUN|DAT|sub|unk|field|local|param|LAB)_[0-9A-Fa-f]\w*|\bm_?[Uu]nk\w*|"
                             r"\b[A-Za-z_]*?_?(?=[0-9a-fA-F]*[0-9])[0-9a-fA-F]{6,8}\b")
    for path in wanted:
        path = path.resolve()
        u = units.get(path) or Unit(path)
        seen = visible(path)
        seen_text = "\n".join(units[p].text for p in seen if p in units)
        local_externs = {n for n, _, _ in extern_data(u.text)} if path.suffix == ".cpp" else set()
        # a type this unit sees whose definitions elsewhere differ from it
        tu_dups = {n for n in dup_types
                   if any(p in seen for ps in bodies[n].values() for p in ps)}
        if re.search(r"(?m)^\s*#\s*if\s+0\b", u.raw):
            advisory["R37 #if 0 block"] += 1
        rel = path.relative_to(ROOT).as_posix()
        for addr, name, head, start, end in function_bodies(u):
            if start is None:
                rows.append((addr, name, status.get(addr, "?"), "UNCHECKED", "", rel))
                continue
            body = u.text[start:end]
            words = set(re.findall(r"[A-Za-z_]\w*", head + body))
            f = body_findings(body, head, u.raw[start:end])
            tu_volatile = set().union(*(volatile_names[p] for p in seen))
            for n in sorted((words - set(re.findall(r"[A-Za-z_]\w*", head))) & tu_volatile):
                if not re.search(r"\bvolatile\b", body):
                    f.append(("R17", f"{n} is declared volatile"))
            for n in sorted(words & local_externs):
                f.append(("R2", f"extern {n} declared in this .cpp"))
            for n in sorted(words & conflicting):
                f.append(("R2", f"{n} declared with different types"))
            for n in sorted(words & tu_dups):
                f.append(("R1", f"{n} defined differently in {len(bodies[n])} places"))
            for n in sorted(words & kw_types):
                f.append(("R1", f"{n} declared both class and struct"))
            for n in sorted(words & pad_types):
                if re.search(r"\b" + n + r"\b", seen_text):
                    f.append(("R17", f"{n} has a pad/dummy member"))
            for n in sorted(words & vpad_types):
                f.append(("R13", f"{n} has a pad/dummy virtual"))
            if re.search(r"(?m)^\s*extern\b[^;]*\(", body):
                advisory["R25 local prototype"] += 1
            if placeholder.search(head) or placeholder.search(body):
                advisory["R7 placeholder name"] += 1
            byte = status.get(addr, "?")
            rules = sorted({r for r, _ in f})
            if byte == "MATCH":
                state = "MATCH-PENDING" if rules else "MATCH"
            else:
                state = byte
            rows.append((addr, name, byte, state, " ".join(rules), rel))
            for r, d in f:
                findings.append((r, rel, line_of(u.text, start), addr, name, byte, d))

    if args.list is not None:
        want = set(args.list)
        for r, rel, ln, addr, name, byte, d in sorted(findings):
            if not want or r in want:
                print(f"{r}\t{rel}:{ln}\t0x{addr:08X}\t{byte}\t{name}\t{d}")

    byte_match = sum(1 for r in rows if r[2] == "MATCH")
    clean = sum(1 for r in rows if r[3] == "MATCH")
    pending = sum(1 for r in rows if r[3] == "MATCH-PENDING")
    unchecked = sum(1 for r in rows if r[3] == "UNCHECKED" and r[2] == "MATCH")
    per_rule = Counter()
    for r in rows:
        if r[3] == "MATCH-PENDING":
            for rule in r[4].split():
                per_rule[rule] += 1
    print(f"{len(rows)} annotated functions; {byte_match} byte-identical: "
          f"{clean} matched under the rules, {pending} MATCH-PENDING, "
          f"{unchecked} unchecked (no body at the annotation: explicit template "
          f"instantiations and compiler-generated members)")
    for rule in sorted(per_rule, key=lambda s: int(s[1:])):
        print(f"  {rule}: {per_rule[rule]} byte-identical functions blocked")
    copies = sum(len(ps) for n in dup_types for ps in bodies[n].values())
    print(f"R1 backlog: {len(dup_types)} types defined differently in {copies} places; "
          f"{len(kw_types)} declared both class and struct")
    if args.list is not None and "R1" in args.list:
        for n in sorted(dup_types, key=lambda n: -sum(len(p) for p in bodies[n].values())):
            print(f"  {n}: {sum(len(p) for p in bodies[n].values())} definitions, "
                  f"{len(bodies[n])} distinct")
    if advisory:
        print("advisory (does not block):")
        for k, v in sorted(advisory.items()):
            print(f"  {k}: {v}")

    if args.report:
        with open(args.report, "w", newline="") as fh:
            w = csv.writer(fh, lineterminator="\n")
            w.writerow(["address", "name", "bytes", "status", "rules", "file"])
            for addr, name, byte, state, rules, rel in sorted(rows):
                w.writerow([f"0x{addr:08X}", name, byte, state, rules, rel])


if __name__ == "__main__":
    sys.exit(main())
