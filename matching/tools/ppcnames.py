"""Pair the original's functions with the named functions of the 2004 Mac PowerPC build.

Aspyr's Mac port (v1.03, PowerPC, 2004) compiled the same game source with
GCC and shipped its symbol table: about 9,500 functions with their C++ names
and parameter types. This tool pairs those functions with addresses in the
GOG Windows 1.03 swkotor.exe, so functions that the reconstruction only knows
by address can get their original names.

Signals, strongest first:

1. name     a name the reconstruction already proved (status.csv, symbols.csv)
            that the Mac build defines exactly once
2. vtable   a vtable both builds name (symbols.csv ??_7 entries, the Mac
            build's _ZTV symbols), paired slot by slot
3. strings  the exact set of string literals a function references, when
            only one unpaired function on each side has that set
4. calls    a paired function's single remaining unpaired callee (or caller)
            on each side, proposed by at least two paired functions, repeated
            until nothing changes; then calls1, which takes a single proposal

--holdout hides a share of the name pairs, runs the other signals and reports
how many hidden pairs they recover correctly: the precision of each signal.

Inputs are the user's own files; outputs go to a directory outside the
checkout, since the names they hold come from the Mac binary:

    python matching/tools/ppcnames.py --exe swkotor.exe --ppc "Knights of the Old Republic" --out DIR
    python matching/tools/ppcnames.py ... --holdout 0.2

Writes DIR/pairs.csv (every Windows game function, paired or not),
DIR/ppc_unpaired.csv (Mac functions with no Windows partner, by category)
and prints a summary.
"""
import argparse
import csv
import os
import random
import re
import shutil
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

import pefile

sys.path.insert(0, str(Path(__file__).resolve().parent))
import matchcmp  # noqa: E402
import todo  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
MATCHING = ROOT / "matching"
MIN_STRING = 4


def printable(raw):
    return len(raw) >= MIN_STRING and all(32 <= c < 127 or c in (9, 10, 13) for c in raw)


def strings_in(data, base):
    """{address: text} of each NUL-terminated printable string in data."""
    found = {}
    start = 0
    for end in range(len(data)):
        if data[end] == 0:
            if printable(data[start:end]):
                found[base + start] = data[start:end].decode("latin1")
            start = end + 1
    return found


# ---------------------------------------------------------------------------
# Windows: swkotor.exe, its function starts and the reconstruction's names

class Windows:
    def __init__(self, path):
        self.pe = pefile.PE(path, fast_load=True)
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        text = self.section(b".text")
        self.text_start = self.base + text.VirtualAddress
        self.text = text.get_data()
        self.strings = {}
        for name in (b".rdata", b".data"):
            s = self.section(name)
            self.strings.update(strings_in(s.get_data(), self.base + s.VirtualAddress))

    def section(self, name):
        return next(s for s in self.pe.sections if s.Name.rstrip(b"\0") == name)

    def dword(self, va):
        return struct.unpack("<I", self.pe.get_data(va - self.base, 4))[0]

    def body(self, start, end):
        return self.text[start - self.text_start:end - self.text_start]


def read_csv(name):
    with open(MATCHING / name, newline="") as f:
        return list(csv.DictReader(f))


def windows_functions(win):
    """(game starts, every start, {start: end}, stock LIBCMT starts, starts
    functions.csv lacks). Starts are functions.csv, the addresses status.csv
    and symbols.csv name, and the exe's direct call targets; without the
    latter a function missing from functions.csv runs on into its neighbour."""
    rows = read_csv("functions.csv")
    crt = {int(r["address"], 16) for r in read_csv("crt_libcmt.csv")}
    eh = {int(r["address"], 16) for r in rows if r["kind"] == "eh"}
    listed = {int(r["address"], 16) for r in rows}
    named = {int(r["address"], 16) for r in read_csv("status.csv")}
    named |= {int(r["address"], 16) for r in read_csv("symbols.csv") if r["role"] in ("defines", "calls")}
    text_end = win.text_start + len(win.text)
    found = {a for a in named | matchcmp.call_targets(win.pe) if win.text_start <= a < text_end}
    starts = sorted(listed | found)
    game = set(starts) - crt - eh
    ends = {a: b for a, b in zip(starts, starts[1:] + [text_end])}
    return game, set(starts), ends, crt, found - listed


def scan_windows(win, starts, ends, functions):
    """{function: (callees in order, string set)}, from E8/E9 rel32 targets that
    are function starts and 4-byte values that are string addresses."""
    out = {}
    for fn in functions:
        code = win.body(fn, ends[fn])
        calls, texts = [], set()
        for i in range(len(code) - 4):
            value = struct.unpack_from("<I", code, i)[0]
            if value in win.strings:
                texts.add(win.strings[value])
            if code[i] in (0xE8, 0xE9) and i + 5 <= len(code):
                target = (fn + i + 5 + struct.unpack_from("<i", code, i + 1)[0]) & 0xFFFFFFFF
                if target in starts and target != fn and not (fn <= target < ends[fn]):
                    calls.append(target)
        out[fn] = (list(dict.fromkeys(calls)), frozenset(texts))
    return out


SPECIAL = {"??0": "ctor", "??1": "dtor", "??_G": "deleting", "??_E": "deleting"}


def normalize_params(params):
    params = re.sub(r"\b(class|struct|enum|union)\s+", "", params)
    params = params.replace("std::_FILE", "_iobuf").replace("__ptr64", "")
    params = re.sub(r"\s+", "", params)
    return "" if params == "void" else params


def split_name(demangled):
    """(qualified name, normalized parameters or None) of a demangled name."""
    text = re.sub(r"\b(public|private|protected): |\b(virtual|static) |__thiscall |__cdecl |__stdcall |__fastcall ", "", demangled)
    m = re.search(r"([A-Za-z_~][\w:~<>, `'*&]*?)\((.*)\)\s*(const)?\s*$", text)
    if not m:
        return text.strip(), None
    qual = m.group(1).split(" ")[-1]
    return qual, normalize_params(m.group(2)) + ("const" if m.group(3) else "")


def windows_names():
    """{address: (qual, params or None, special kind or None, shown name)}."""
    names = {}
    for r in read_csv("symbols.csv"):
        if r["role"] not in ("defines", "calls"):
            continue
        address = int(r["address"], 16)
        qual, params = split_name(r["demangled"])
        kind = next((k for p, k in SPECIAL.items() if r["symbol"].startswith(p)), None)
        names.setdefault(address, (qual, params, kind, r["demangled"]))
    for r in read_csv("status.csv"):
        address = int(r["address"], 16)
        if address not in names and r["name"]:
            parts = r["name"].split("::")
            kind = "ctor" if len(parts) > 1 and parts[-1] == parts[-2] else \
                "dtor" if parts[-1].startswith("~") else None
            names[address] = (r["name"], None, kind, r["name"])
    return names


def windows_vtables(win):
    """{class: [slot targets]} for the ??_7 vtables symbols.csv knows. Slots run
    while they point into .text and stop at the next known vtable; a slot can
    point to a body that /OPT:ICF folded with another class's."""
    known = {}
    for r in read_csv("symbols.csv"):
        m = re.fullmatch(r"\?\?_7(\w+)@@6B@", r["symbol"])
        if m and r["role"] == "data":
            known[int(r["address"], 16)] = m.group(1)
    text_end = win.text_start + len(win.text)
    tables = {}
    for va, cls in known.items():
        slots = []
        while (not slots or va + 4 * len(slots) not in known) and \
                win.text_start <= (value := win.dword(va + 4 * len(slots))) < text_end:
            slots.append(value)
        tables[cls] = slots
    return tables


# ---------------------------------------------------------------------------
# Mac PowerPC: a 32-bit big-endian Mach-O with its symbol table

class MachO:
    def __init__(self, path):
        self.data = Path(path).read_bytes()
        magic, cpu, _, _, ncmds, _, _ = struct.unpack_from(">7I", self.data, 0)
        if magic != 0xFEEDFACE or cpu != 18:
            raise SystemExit(f"{path}: not a 32-bit PowerPC Mach-O")
        self.sections = []  # (segment, section, address, size, file offset)
        offset = 28
        for _ in range(ncmds):
            cmd, size = struct.unpack_from(">2I", self.data, offset)
            if cmd == 1:  # LC_SEGMENT
                nsects = struct.unpack_from(">I", self.data, offset + 48)[0]
                for i in range(nsects):
                    s = offset + 56 + 68 * i
                    sect = self.data[s:s + 16].rstrip(b"\0").decode()
                    seg = self.data[s + 16:s + 32].rstrip(b"\0").decode()
                    addr, sz, off = struct.unpack_from(">3I", self.data, s + 32)
                    self.sections.append((seg, sect, addr, sz, off))
            elif cmd == 2:  # LC_SYMTAB
                self.symtab = struct.unpack_from(">4I", self.data, offset + 8)
            offset += size

    def section(self, seg, sect):
        return next(s for s in self.sections if s[0] == seg and s[1] == sect)

    def bytes_at(self, addr, n):
        for _, _, start, size, off in self.sections:
            if start <= addr < start + size and off:
                return self.data[off + addr - start:off + addr - start + n]
        return b""

    def word(self, addr):
        raw = self.bytes_at(addr, 4)
        return struct.unpack(">I", raw)[0] if len(raw) == 4 else None

    def symbols(self):
        """[(address, section number, name)] of defined, non-debug symbols."""
        symoff, nsyms, stroff, _ = self.symtab
        out = []
        for i in range(nsyms):
            strx, ntype, sect, _, value = struct.unpack_from(">IBBhI", self.data, symoff + 12 * i)
            if ntype & 0xE0 or ntype & 0x0E != 0x0E:
                continue
            end = self.data.index(b"\0", stroff + strx)
            out.append((value, sect, self.data[stroff + strx:end].decode("latin1")))
        return out


def demangle(names, tool):
    """Demangled names in order; Mach-O symbols carry one extra leading underscore."""
    stripped = [n[1:] if n.startswith("_") else n for n in names]
    run = subprocess.run([tool, "-n"], input="\n".join(stripped) + "\n", capture_output=True, text=True, check=True)
    out = run.stdout.split("\n")[:len(names)]
    return out if len(out) == len(names) else stripped


def ppc_functions(mac, tool):
    """({start: end}, {start: (mangled, demangled)}, {vtable class: address}, stub ranges)."""
    text = mac.section("__TEXT", "__text")
    text_index = mac.sections.index(text) + 1
    syms = mac.symbols()
    funcs = sorted({(a, n) for a, s, n in syms if s == text_index})
    starts, seen = [], set()
    for a, n in funcs:
        if a not in seen:
            seen.add(a)
            starts.append((a, n))
    demangled = demangle([n for _, n in starts], tool)
    names = {a: (n, d) for (a, n), d in zip(starts, demangled)}
    addresses = [a for a, _ in starts]
    ends = dict(zip(addresses, addresses[1:] + [text[2] + text[3]]))
    vtables = {}
    for a, s, n in syms:
        if n.startswith("__ZTV"):
            vtables[demangle([n], tool)[0].replace("vtable for ", "")] = a
    stubs = [(s[2], s[2] + s[3]) for s in mac.sections if "stub" in s[1]]
    return ends, names, vtables, stubs


def sign16(v):
    return v - 0x10000 if v & 0x8000 else v


LOADS = {32, 33, 34, 35, 40, 41, 42, 43}
FLOAT_LOADS = {48, 49, 50, 51}
STORES = {36, 37, 38, 39, 44, 45, 52, 53, 54, 55}


def scan_ppc(mac, ends, strings):
    """{function: (callees in order, string set)}. Tracks lis/addis/addi/ori
    register values (and the bcl/mflr PIC base) to find string addresses."""
    out = {}
    for fn, end in ends.items():
        code = mac.bytes_at(fn, end - fn)
        regs, lr, calls, texts = {}, None, [], set()

        def ref(value):
            if value in strings:
                texts.add(strings[value])

        for i in range(0, len(code) - 3, 4):
            pc, w = fn + i, struct.unpack_from(">I", code, i)[0]
            op, rd, ra, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
            if op == 18:  # b / bl
                li = w & 0x03FFFFFC
                target = (li - 0x04000000 if li & 0x02000000 else li) + (0 if w & 2 else pc)
                if target in ends and target != fn and not (fn <= target < end):
                    calls.append(target)
                if w & 1:
                    for r in (0, *range(3, 13)):
                        regs.pop(r, None)
            elif w == 0x429F0005:  # bcl 20,31,$+4
                lr = pc + 4
            elif op == 31 and (w >> 1) & 0x3FF == 339 and ((w >> 11) & 0x3FF) == 0x100:  # mflr
                regs.pop(rd, None)
                if lr is not None:
                    regs[rd] = lr
            elif op == 15:  # addis / lis
                base = 0 if ra == 0 else regs.get(ra)
                regs.pop(rd, None)
                if base is not None:
                    regs[rd] = (base + (imm << 16)) & 0xFFFFFFFF
            elif op == 14:  # addi / li
                if ra and ra in regs:
                    value = (regs[ra] + sign16(imm)) & 0xFFFFFFFF
                    ref(value)
                    regs[rd] = value
                else:
                    regs.pop(rd, None)
            elif op == 24:  # ori rA,rS,imm
                if rd in regs:
                    regs[ra] = regs[rd] | imm
                    ref(regs[ra])
                else:
                    regs.pop(ra, None)
            elif op in LOADS or op in FLOAT_LOADS or op in STORES:
                if ra and ra in regs:
                    ref((regs[ra] + sign16(imm)) & 0xFFFFFFFF)
                if op in LOADS:
                    regs.pop(rd, None)
            elif op in (31, 20, 21, 23, 25, 26, 27, 28, 29, 7, 8, 12, 13):
                regs.pop(rd, None)
                regs.pop(ra, None)
        out[fn] = (list(dict.fromkeys(calls)), frozenset(texts))
    return out


VARIANT = re.compile(r"(C[123]|D[012])E")


def ppc_index(names):
    """{qual: [(start, params, variant)]} with ctor/dtor variants from the mangling."""
    index = defaultdict(list)
    for start, (mangled, demangled) in names.items():
        qual, params = split_name(demangled)
        m = VARIANT.search(mangled) if mangled.startswith("__ZN") else None
        index[qual].append((start, params, m.group(1) if m else None))
    return index


# GCC emits complete (C1/D1) and base (C2/D2) copies of a ctor or dtor, or only
# one of them; MSVC has one body. The deleting dtor (D0) is MSVC's ??_G.
VARIANTS = {"ctor": (("C1", None), ("C2",)), "dtor": (("D1", None), ("D2",)), "deleting": (("D0",),)}


def ppc_vtable(mac, address, ends, names, stubs):
    """Slots of the primary vtable, one deleting-destructor slot per D1/D0 pair."""
    slots, at = [], address + 8  # skip offset-to-top and typeinfo
    while True:
        value = mac.word(at)
        if value is None:
            break
        if value in ends:
            slots.append(value)
        elif any(lo <= value < hi for lo, hi in stubs):
            slots.append(None)  # __cxa_pure_virtual
        else:
            break
        at += 4
    out = []
    for i, value in enumerate(slots):
        mangled = names.get(value, ("", ""))[0]
        nxt = names.get(slots[i + 1], ("", ""))[0] if i + 1 < len(slots) and slots[i + 1] else ""
        if "D1E" in mangled and "D0E" in nxt:
            continue
        out.append(value)
    return out


# ---------------------------------------------------------------------------
# Pairing

class Pairs:
    def __init__(self):
        self.p, self.w, self.method, self.conflicts = {}, {}, {}, Counter()

    def add(self, w, p, method):
        if p is None or w is None:
            return False
        if self.p.get(w) == p:
            return False
        if w in self.p or p in self.w:
            self.conflicts[method] += 1
            return False
        self.p[w], self.w[p], self.method[w] = p, w, method
        return True


def pair_names(pairs, wnames, pindex, allowed):
    keys = Counter((q, a) for w, (q, a, k, _) in wnames.items() if w in allowed)
    for w, (qual, params, kind, _) in wnames.items():
        if w not in allowed or keys[(qual, params)] > 1:
            continue
        cands = pindex.get(qual, [])
        if kind in VARIANTS:
            cands = next((found for wanted in VARIANTS[kind]
                          if (found := [c for c in cands if c[2] in wanted])), [])
        if params is not None and len(cands) > 1:
            cands = [c for c in cands if c[1] == params]
        if len(cands) == 1:
            pairs.add(w, cands[0][0], "name")


MIN_FOLD_SAFE = 16  # /OPT:ICF folds tiny bodies across classes


def pair_vtables(pairs, wtables, ptables, pnames, wsize, stats):
    """Slot-by-slot pairs. The Mac table's length is exact; the Windows read
    may run on into the next table, so only a Windows table at least as long
    is used. Skipped: overloaded virtuals (MSVC orders an overload set in
    reverse, GCC in declaration order), tiny bodies that may be folded, and
    a slot body seen with two different partners."""
    proposals = defaultdict(set)
    for cls, wslots in wtables.items():
        pslots = ptables.get(cls)
        if pslots is None:
            continue
        if len(wslots) < len(pslots):
            stats["vtable size differs"] += 1
            continue
        stats["vtables paired"] += 1
        method = [pnames[p][1].split("(")[0].split("::")[-1] if p else None for p in pslots]
        overloaded = {n for n, c in Counter(method).items() if c > 1}
        for w, p, name in zip(wslots, pslots, method):
            if p is not None and name not in overloaded and wsize.get(w, 0) >= MIN_FOLD_SAFE:
                proposals[w].add(p)
    partners = Counter(p for ps in proposals.values() for p in ps)
    for w, ps in proposals.items():
        if len(ps) == 1 and partners[next(iter(ps))] == 1:
            pairs.add(w, next(iter(ps)), "vtable")


def pair_strings(pairs, wscan, pscan, wgame):
    wkeys, pkeys = defaultdict(list), defaultdict(list)
    for w, (_, texts) in wscan.items():
        if texts and w in wgame and w not in pairs.p:
            wkeys[texts].append(w)
    for p, (_, texts) in pscan.items():
        if texts and p not in pairs.w:
            pkeys[texts].append(p)
    for key, ws in wkeys.items():
        if len(ws) == 1 and len(pkeys.get(key, [])) == 1:
            pairs.add(ws[0], pkeys[key][0], "strings")


SIZE_RATIO = (0.25, 4.0)  # Mac / Windows body size, about the 1st to 99th percentile


def pair_calls(pairs, wscan, pscan, wgame, wsize, psize, min_votes):
    """Each paired function whose callees (or callers) leave exactly one
    unpaired function on each side proposes that pair. A proposal is taken
    when min_votes paired functions make it and nothing competes with it."""
    wcallers, pcallers = defaultdict(list), defaultdict(list)
    for w, (calls, _) in wscan.items():
        for c in calls:
            wcallers[c].append(w)
    for p, (calls, _) in pscan.items():
        for c in calls:
            pcallers[c].append(p)

    def plausible(w, p):
        a, b = wscan.get(w, ((), frozenset()))[1], pscan.get(p, ((), frozenset()))[1]
        ratio = psize[p] / max(wsize[w], 1)
        return not (a and b and not a & b) and SIZE_RATIO[0] <= ratio <= SIZE_RATIO[1]

    method = "calls" if min_votes > 1 else "calls1"
    changed = True
    while changed:
        votes = Counter()
        for w, p in pairs.p.items():
            for wl, pl in ((wscan.get(w, ([],))[0], pscan.get(p, ([],))[0]),
                           (wcallers.get(w, []), pcallers.get(p, []))):
                wl = [x for x in wl if x in wgame and x not in pairs.p]
                pl = [x for x in pl if x not in pairs.w]
                if len(wl) == 1 and len(pl) == 1 and plausible(wl[0], pl[0]):
                    votes[(wl[0], pl[0])] += 1
        wseen = Counter(w for w, _ in votes)
        pseen = Counter(p for _, p in votes)
        changed = False
        for (w, p), n in votes.items():
            if n >= min_votes and wseen[w] == 1 and pseen[p] == 1:
                changed |= pairs.add(w, p, method)


METHODS = ("name", "vtable", "strings", "calls", "calls1")


def run(wnames, allowed, pindex, pnames, wtables, ptables, wscan, pscan, wgame, wsize, psize, stats):
    pairs = Pairs()
    pair_names(pairs, wnames, pindex, allowed)
    pair_vtables(pairs, wtables, ptables, pnames, wsize, stats)
    pair_strings(pairs, wscan, pscan, wgame)
    pair_calls(pairs, wscan, pscan, wgame, wsize, psize, 2)
    pair_strings(pairs, wscan, pscan, wgame)
    pair_calls(pairs, wscan, pscan, wgame, wsize, psize, 1)
    return pairs


def category(demangled):
    if re.search(r"_Mac\b|Aspyr|^agl|^DSp|Miles|Carbon|Mac[A-Z]", demangled):
        return "mac platform layer"
    if demangled.startswith(("std::", "__gnu_cxx", "__cxa", "operator ")) or "std::" in demangled.split("(")[0]:
        return "C++ library"
    if re.match(r"C[A-Z]\w*::", demangled):
        return "game class"
    return "other"


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--exe", default=os.environ.get("KOTOR_ORIGINAL_EXE"), help="GOG 1.03 swkotor.exe")
    parser.add_argument("--ppc", default=os.environ.get("KOTOR_PPC_EXE"),
                        help="the Mac PowerPC executable (Contents/MacOS/Knights of the Old Republic)")
    parser.add_argument("--out", required=True, help="new output directory, outside the checkout")
    parser.add_argument("--cxxfilt", default=shutil.which("llvm-cxxfilt") or shutil.which("c++filt"))
    parser.add_argument("--holdout", type=float, default=0.0,
                        help="share of name pairs to hide to measure the other signals")
    parser.add_argument("--seed", type=int, default=1)
    args = parser.parse_args()
    if not args.exe or not args.ppc or not args.cxxfilt:
        parser.error("--exe, --ppc and a c++filt (--cxxfilt) are required")
    out = Path(args.out).resolve()
    if out == ROOT or ROOT in out.parents:
        parser.error("--out must be outside the checkout")

    win = Windows(args.exe)
    wgame, wstarts, wends, crt, unlisted = windows_functions(win)
    wscan = scan_windows(win, wstarts, wends, wgame)
    wnames = windows_names()
    wtables = windows_vtables(win)
    wsize = {w: wends[w] - w for w in wends}

    mac = MachO(args.ppc)
    cstring = mac.section("__TEXT", "__cstring")
    pstrings = strings_in(mac.bytes_at(cstring[2], cstring[3]), cstring[2])
    pends, pnames, pvtables, stubs = ppc_functions(mac, args.cxxfilt)
    pscan = scan_ppc(mac, pends, pstrings)
    pindex = ppc_index(pnames)
    ptables = {c: ppc_vtable(mac, a, pends, pnames, stubs) for c, a in pvtables.items()}
    psize = {p: pends[p] - p for p in pends}

    named = {w for w in wnames if w in wgame}
    if args.holdout:
        reference = Pairs()
        pair_names(reference, wnames, pindex, named)
        rng = random.Random(args.seed)
        hidden = set(rng.sample(sorted(reference.p), int(len(reference.p) * args.holdout)))
        stats = Counter()
        pairs = run(wnames, named - hidden, pindex, pnames, wtables, ptables, wscan, pscan, wgame, wsize, psize, stats)
        print(f"holdout: {len(hidden)} of {len(reference.p)} name pairs hidden")
        print(f"{'method':8} {'recovered':>9} {'correct':>8} {'precision':>9}")
        for method in METHODS[1:]:
            got = [w for w in hidden if pairs.method.get(w) == method]
            ok = sum(pairs.p[w] == reference.p[w] for w in got)
            print(f"{method:8} {len(got):9} {ok:8} {ok / len(got) if got else 0:9.1%}")
        return

    stats = Counter()
    pairs = run(wnames, named, pindex, pnames, wtables, ptables, wscan, pscan, wgame, wsize, psize, stats)
    status = {int(r["address"], 16): r["status"] for r in read_csv("status.csv")}
    playable = todo.playable_index()

    out.mkdir(parents=True, exist_ok=False)
    (out / ".gitignore").write_text("*\n")
    with open(out / "pairs.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["address", "size", "listed", "status", "playable", "repo_name",
                         "ppc_address", "ppc_name", "method"])
        for w in sorted(wgame):
            p = pairs.p.get(w)
            where = playable.get(w)
            writer.writerow([f"0x{w:08X}", wends[w] - w, "no" if w in unlisted else "yes",
                             status.get(w, "todo"),
                             where[0].relative_to(ROOT).as_posix() if where else "",
                             wnames.get(w, (None, None, None, ""))[3],
                             f"0x{p:08X}" if p else "", pnames[p][1] if p else "", pairs.method.get(w, "")])
    unpaired = Counter()
    with open(out / "ppc_unpaired.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["ppc_address", "size", "category", "ppc_name"])
        for p in sorted(pends):
            if p not in pairs.w:
                cat = category(pnames[p][1])
                unpaired[cat] += 1
                writer.writerow([f"0x{p:08X}", pends[p] - p, cat, pnames[p][1]])

    def count(pred):
        return sum(1 for w in wgame if pred(w))

    print(f"Windows game functions: {len(wgame)} ({len(wgame & unlisted)} missing from functions.csv; "
          f"stock LIBCMT excluded: {len(crt)})")
    print(f"Mac PowerPC functions: {len(pends)}; vtables named on both sides: "
          f"{stats['vtables paired']} paired, {stats['vtable size differs']} with different slot counts")
    print()
    print(f"{'':34} {'functions':>9}")
    for label, pred in (
            ("byte-identical (MATCH)", lambda w: status.get(w) == "MATCH"),
            ("attempted (NONMATCH)", lambda w: status.get(w) == "NONMATCH"),
            ("not attempted", lambda w: w not in status),
            ("  of which in the playable src/", lambda w: w not in status and w in playable),
            ("named by the reconstruction", lambda w: w in wnames),
            ("paired with the Mac build", lambda w: w in pairs.p)):
        print(f"{label:34} {count(pred):9}")
    print()
    print("paired, by signal:")
    for method in METHODS:
        print(f"  {method:8} {count(lambda w: pairs.method.get(w) == method):6}"
              f"   conflicts skipped: {pairs.conflicts[method]}")
    new = lambda w: w in pairs.p and w not in wnames
    print()
    print(f"new names (paired, unnamed in the repo): {count(new)}")
    print(f"  of which not yet attempted:            {count(lambda w: new(w) and w not in status)}")
    print(f"still unnamed:                           {count(lambda w: w not in wnames and w not in pairs.p)}")
    print()
    print("Mac functions without a Windows partner: " + ", ".join(f"{k} {v}" for k, v in unpaired.most_common()))
    print(f"wrote {out / 'pairs.csv'} and {out / 'ppc_unpaired.csv'}")


if __name__ == "__main__":
    main()
