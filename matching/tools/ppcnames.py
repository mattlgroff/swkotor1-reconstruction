"""Pair the original's functions with the named functions of Aspyr's Mac PowerPC builds.

Aspyr's Mac ports compiled the same game source with GCC and kept their
symbol tables: C++ names with parameter types. Two builds are known:

- v1.03 (2004), PowerPC: about 10,300 functions
- v1.03e (2009), universal PowerPC/i386: about 13,300 functions, plus a
  debug map that names each function's object file (BioWare's source file)

This tool pairs their functions with addresses in the GOG Windows 1.03
swkotor.exe, so functions that the reconstruction only knows by address can
get their original names. A universal binary is read through its ppc slice.

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

With several builds, each is paired on its own; a function the builds name
differently is left unnamed and reported as a conflict. With a debug map,
paired functions get their object file, and runs of one object's functions
in address order give the original link order and place unnamed functions
between them.

--holdout hides a share of the name pairs, runs the other signals and reports
how many hidden pairs they recover correctly: the precision of each signal.

A name pair also needs compatible parameters where both sides have them
(see compatible): the same count, and per parameter the same kind
(integer, floating point, pointer or reference, or the same class by value).
Builds agree only on the same full signature; a function the builds pair
with different methods or different parameters is left unnamed and its
candidates are listed as a conflict.

Inputs are the user's own files. The working set goes to a directory outside
the checkout; --repo writes the hint files this repository keeps, which stay
separate from the proved names in symbols.csv:

    python matching/tools/ppcnames.py --exe swkotor.exe --ppc "Knights of the Old Republic" XKOTOR --out DIR
    python matching/tools/ppcnames.py ... --holdout 0.2

Writes DIR/pairs.csv (every Windows game function, paired or not),
DIR/link_order.csv (with a debug map), DIR/mac_unpaired.csv (Mac functions
with no Windows partner, by category), DIR/signature_rejects.csv (name pairs
refused for their parameters) and prints a summary. --repo also refreshes
matching/mac_names.csv and matching/link_order.csv; run it with both builds.

Tests: python -m unittest discover -s matching/tools -p "test_*.py"
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

sys.path.insert(0, str(Path(__file__).resolve().parent))

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
        import pefile
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
    with open(MATCHING / name, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def windows_functions(win):
    """(game starts, every start, {start: end}, stock LIBCMT starts, starts
    functions.csv lacks). Starts are functions.csv, the addresses status.csv
    and symbols.csv name, and the exe's direct call targets; without the
    latter a function missing from functions.csv runs on into its neighbour."""
    import matchcmp
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


# x86 instruction lengths, enough for MSVC 7.0 code: the one-byte and 0F
# opcode maps with whether a ModRM byte follows and the immediate's size
# (b 1, w 2, wb 3, z 4 or 2 with 66, a moffs, p far pointer, grp3 only for
# F6/F7 /0 and /1; 0F 0F, 3DNow!, ends in its opcode byte). Later
# extensions (SSE4a's 0F 78/79 with immediates, VEX-encoded AVX) are not
# decoded: MSVC 7.0 does not emit them, and on this exe the lengths agree
# with Capstone. scan_windows walks a body one instruction at a time so an
# E8 or E9 byte inside an operand is not taken for a call.

X86_PREFIXES = frozenset((0xF0, 0xF2, 0xF3, 0x2E, 0x36, 0x3E, 0x26, 0x64, 0x65, 0x66, 0x67))
X86_ONE = {op: (False, None) for op in range(256)}
for _base in range(0x00, 0x40, 0x08):
    X86_ONE.update({_base + k: (True, None) for k in range(4)})
    X86_ONE.update({_base + 4: (False, "b"), _base + 5: (False, "z")})
X86_ONE.update({0x62: (True, None), 0x63: (True, None), 0x68: (False, "z"), 0x69: (True, "z"),
                0x6A: (False, "b"), 0x6B: (True, "b"), 0x80: (True, "b"), 0x81: (True, "z"),
                0x82: (True, "b"), 0x83: (True, "b"), 0x9A: (False, "p"), 0xA8: (False, "b"),
                0xA9: (False, "z"), 0xC0: (True, "b"), 0xC1: (True, "b"), 0xC2: (False, "w"),
                0xC4: (True, None), 0xC5: (True, None), 0xC6: (True, "b"), 0xC7: (True, "z"),
                0xC8: (False, "wb"), 0xCA: (False, "w"), 0xCD: (False, "b"), 0xD4: (False, "b"),
                0xD5: (False, "b"), 0xE8: (False, "z"), 0xE9: (False, "z"), 0xEA: (False, "p"),
                0xEB: (False, "b"), 0xF6: (True, "grp3"), 0xF7: (True, "grp3"),
                0xFE: (True, None), 0xFF: (True, None)})
X86_ONE.update({op: (True, None) for op in [*range(0x84, 0x90), *range(0xD0, 0xD4), *range(0xD8, 0xE0)]})
X86_ONE.update({op: (False, "b") for op in [*range(0x70, 0x80), *range(0xB0, 0xB8), *range(0xE0, 0xE8)]})
X86_ONE.update({op: (False, "z") for op in range(0xB8, 0xC0)})
X86_ONE.update({op: (False, "a") for op in range(0xA0, 0xA4)})
X86_TWO = {op: (True, None) for op in range(256)}
X86_TWO.update({op: (False, None) for op in [0x05, 0x06, 0x07, 0x08, 0x09, 0x0B, 0x0E, *range(0x30, 0x38),
                                             0x77, 0xA0, 0xA1, 0xA2, 0xA8, 0xA9, 0xAA, *range(0xC8, 0xD0)]})
X86_TWO.update({op: (False, "z") for op in range(0x80, 0x90)})
X86_TWO.update({op: (True, "b") for op in (0x0F, 0x70, 0x71, 0x72, 0x73, 0xA4, 0xAC, 0xBA, 0xC2, 0xC4, 0xC5, 0xC6)})


def x86_modrm(code, i, addr16):
    """(bytes of ModRM, SIB and displacement at i, offset of a 4-byte displacement or None)."""
    mod, rm = code[i] >> 6, code[i] & 7
    if mod == 3:
        return 1, None
    if addr16:
        return (3 if rm == 6 else 1, None) if mod == 0 else (2 if mod == 1 else 3, None)
    n = 1
    if rm == 4:
        n = 2
        if mod == 0 and code[i + 1] & 7 == 5:
            return 6, i + 2
    elif mod == 0 and rm == 5:
        return 5, i + 1
    if mod == 1:
        return n + 1, None
    if mod == 2:
        return n + 4, i + n
    return n, None


def x86_instruction(code, i):
    """(length, opcode, offsets of 4-byte displacements and immediates,
    (offset, size) of the immediate or None) of the instruction at code[i];
    0F opcodes are 0x0F00 | second byte. Raises IndexError when the
    instruction runs past the end of code."""
    start, opsize, addr16 = i, False, False
    while code[i] in X86_PREFIXES:
        opsize |= code[i] == 0x66
        addr16 |= code[i] == 0x67
        i += 1
    op = code[i]
    i += 1
    if op == 0x0F:
        op = 0x0F00 | code[i]
        i += 1
        if op in (0x0F38, 0x0F3A):
            i += 1
            modrm, imm = True, "b" if op == 0x0F3A else None
        else:
            modrm, imm = X86_TWO[op & 0xFF]
    else:
        modrm, imm = X86_ONE[op]
    fields = []
    if modrm:
        reg = (code[i] >> 3) & 7
        n, disp = x86_modrm(code, i, addr16)
        if disp is not None:
            fields.append(disp)
        i += n
        if imm == "grp3":
            imm = ("b" if op == 0xF6 else "z") if reg < 2 else None
    size = {None: 0, "b": 1, "w": 2, "wb": 3, "z": 2 if opsize else 4,
            "a": 2 if addr16 else 4, "p": 4 if opsize else 6}[imm]
    if imm in ("z", "a", "p") and size >= 4:
        fields.append(i)
    immediate = (i, size) if size else None
    i += size
    if i > len(code):
        raise IndexError(start)
    return i - start, op, fields, immediate


def scan_windows(win, starts, ends, functions):
    """{function: (callees in order, string set)}, from the rel32 targets of
    E8/E9 instructions (prefixed or not; a 16-bit rel16 form is skipped)
    that are function starts and the 4-byte displacements and immediates
    that are string addresses, decoded instruction by instruction."""
    out = {}
    for fn in functions:
        code = win.body(fn, ends[fn])
        calls, texts = [], set()
        i = 0
        while i < len(code):
            try:
                n, op, fields, immediate = x86_instruction(code, i)
            except IndexError:
                break
            for f in fields:
                value = struct.unpack_from("<I", code, f)[0]
                if value in win.strings:
                    texts.add(win.strings[value])
            if op in (0xE8, 0xE9) and immediate[1] == 4:
                target = (fn + i + n + struct.unpack_from("<i", code, immediate[0])[0]) & 0xFFFFFFFF
                if target in starts and target != fn and not (fn <= target < ends[fn]):
                    calls.append(target)
            i += n
        out[fn] = (list(dict.fromkeys(calls)), frozenset(texts))
    return out


SPECIAL = {"??0": "ctor", "??1": "dtor", "??_G": "deleting", "??_E": "deleting"}


CALLING = re.compile(r"\b__(thiscall|cdecl|stdcall|fastcall)\b")


def normalize_params(params):
    """MSVC and GCC spellings of a parameter list made comparable: no
    class/struct/enum/union keywords or calling conventions, single spaces
    between words only, "" for (void)."""
    params = re.sub(r"\b(class|struct|enum|union)\s+", "", params)
    params = params.replace("std::_FILE", "_iobuf").replace("__ptr64", "")
    params = CALLING.sub("", params)
    params = re.sub(r"\s+", " ", params).strip()
    params = re.sub(r"\s*([*&,()<>])\s*", r"\1", params)
    return "" if params == "void" else params


def split_name(demangled):
    """(qualified name, normalized parameters or None) of a demangled name.
    A const method's parameters end in "|const"."""
    text = re.sub(r"\b(public|private|protected): |\b(virtual|static) |\b(class|struct|enum|union) ", "", demangled)
    text = CALLING.sub("", text)
    m = None
    if "operator" in text:  # operator(), operator+, operator int, operator new[]
        m = re.search(r"^(.*?\boperator\b(?:\(\)|[^(]*?))\((.*)\)\s*(const)?\s*$", text)
    m = m or re.search(r"([A-Za-z_~][\w:~<>, `'*&]*?)\((.*)\)\s*(const)?\s*$", text)
    if not m:
        return text.strip(), None
    # the name is the last word outside template arguments, before it the
    # return type; an operator's name runs from the word with "operator"
    head, depth, cut = m.group(1).strip(), 0, 0
    operator = re.search(r"(?:^|\s)(\S*\boperator\b.*)$", head)
    if operator:
        head, cut = operator.group(1), 0
    for i, c in enumerate(head if not operator else ""):
        depth += (c == "<") - (c == ">")
        if c == " " and depth == 0:
            cut = i + 1
    qual = re.sub(r"\s*([*&,<>])\s*", r"\1", head[cut:]).replace(",", ", ")
    return qual, normalize_params(m.group(2)) + ("|const" if m.group(3) else "")


def split_params(params):
    """The top-level parameters of a normalized parameter list."""
    out, depth, current = [], 0, ""
    for c in params:
        if c == "," and depth == 0:
            out.append(current)
            current = ""
            continue
        depth += c in "(<"
        depth -= c in ")>"
        current += c
    return out + [current] if current or out else out


# The differences a name pair may have between the repo's parameter types
# (the Windows side, partly guessed) and the Mac's, each one explicit:
# - another spelling or signedness of the same integer width (int and
#   unsigned long are both 32 bits on both platforms);
# - a 32-bit integer in the repo for a narrower integer or a bool on the Mac
#   (MSVC pushes every integer argument as 32 bits and the repo often
#   writes int; GCC's PowerPC bool is 4 bytes, MSVC's 1);
# - a pointer for a reference, at the same depth, and const or volatile on
#   what is pointed to;
# - void* for a pointer of the same depth, on either side;
# - char*, signed char* and unsigned char* for one another (byte buffers).
# Anything else has to be equal: another class, a callback's type, a
# floating-point or 64-bit value, and a method's own const.
INT32 = {"int", "unsigned int", "unsigned", "signed", "signed int", "long", "unsigned long",
         "long int", "unsigned long int", "signed long"}
INT16 = {"short", "unsigned short", "short int", "unsigned short int", "signed short",
         "wchar_t", "__wchar_t"}
INT8 = {"char", "signed char", "unsigned char"}
BOOL = {"bool", "__bool8"}


def base_type(param):
    """(the type without the outer const, volatile, * and &, the outer pointer
    and reference depth). Template arguments stay as they are."""
    close = param.rfind(">")
    head, tail = param[:close + 1], param[close + 1:]
    head = re.sub(r"^(const|volatile)\s+", "", head)
    depth = tail.count("*") + tail.count("&")
    return " ".join((head + re.sub(r"\b(const|volatile)\b|[*&]", " ", tail)).split()), depth


def param_match(windows, mac):
    """Whether a repo parameter type may stand for a Mac one (see above)."""
    if windows == mac:
        return True
    if "(" in windows or "(" in mac:
        return False
    (wb, wd), (mb, md) = base_type(windows), base_type(mac)
    if wd != md:
        return False
    if wd:
        return wb == mb or "void" in (wb, mb) or (wb in INT8 and mb in INT8)
    if wb == mb or any(wb in c and mb in c for c in (INT32, INT16, INT8, BOOL)):
        return True
    return wb in INT32 and (mb in INT16 or mb in INT8 or mb in BOOL)


def signature_match(windows, mac):
    """How a repo parameter list agrees with a Mac one: "exact", "compatible"
    (the same count, each pair allowed by param_match, the same method
    const), "unknown" when the repo has none (a status.csv name), else None.
    A repo function the CSVs give several parameter lists is a tuple of
    them; the best of them counts."""
    if windows is None or mac is None:
        return "unknown"
    if isinstance(windows, tuple):
        found = {signature_match(w, mac) for w in windows}
        return next((m for m in ("exact", "compatible") if m in found), None)
    if windows == mac:
        return "exact"
    (wp, _, wconst), (mp, _, mconst) = windows.partition("|"), mac.partition("|")
    wl, ml = split_params(wp), split_params(mp)
    if wconst == mconst and len(wl) == len(ml) and all(param_match(w, m) for w, m in zip(wl, ml)):
        return "compatible"
    return None


def windows_names():
    """({address: (qual, params or None, special kind or None, shown name)},
    addresses the CSVs give different names). symbols.csv names one body
    several times when /OPT:ICF folded it, two status.csv rows can name one
    address, and status.csv can disagree with symbols.csv (by name, or by a
    mangled name that is not that address's symbol); such an address keeps
    its first name for display but is not paired by name. One name with
    several parameter lists keeps them all as a tuple."""
    names, seen, symbols = {}, defaultdict(set), defaultdict(set)
    for r in read_csv("symbols.csv"):
        if r["role"] not in ("defines", "calls"):
            continue
        address = int(r["address"], 16)
        qual, params = split_name(r["demangled"])
        kind = next((k for p, k in SPECIAL.items() if r["symbol"].startswith(p)), None)
        names.setdefault(address, (qual, params, kind, r["demangled"]))
        seen[address].add((qual, params))
        symbols[address].add(r["symbol"])
    for r in read_csv("status.csv"):
        address, name = int(r["address"], 16), r["name"]
        if not name:
            continue
        if address in symbols:
            if name.startswith("?") and name not in symbols[address]:
                seen[address].add((name, None))
            elif not name.startswith("?") and name not in {q for q, _ in seen[address]}:
                seen[address].add((name, None))
            continue
        parts = name.split("::")
        kind = "ctor" if len(parts) > 1 and parts[-1] == parts[-2] else \
            "dtor" if parts[-1].startswith("~") else None
        names.setdefault(address, (name, None, kind, name))
        seen[address].add((name, None))
    ambiguous = {a for a, found in seen.items() if len({q for q, _ in found}) > 1}
    for address, found in seen.items():
        params = sorted({p for _, p in found if p is not None})
        if len(params) > 1 and address not in ambiguous:
            qual, _, kind, shown = names[address]
            names[address] = (qual, tuple(params), kind, shown)
    return names, ambiguous


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
# Mac PowerPC: a 32-bit big-endian Mach-O with its symbol table, on its own
# or as the ppc slice of a universal binary

CPU_PPC = 18
N_FUN, N_SO, N_OSO = 0x24, 0x64, 0x66


class MachO:
    def __init__(self, path):
        self.path = path
        self.data = Path(path).read_bytes()
        if len(self.data) >= 8 and struct.unpack_from(">I", self.data, 0)[0] == 0xCAFEBABE:  # universal
            count = struct.unpack_from(">I", self.data, 4)[0]
            self.need(8 + 20 * count <= len(self.data), "universal header runs past the file")
            for i in range(count):
                cpu, _, offset, size, _ = struct.unpack_from(">5I", self.data, 8 + 20 * i)
                if cpu == CPU_PPC:
                    self.need(offset + size <= len(self.data), "ppc slice runs past the file")
                    self.data = self.data[offset:offset + size]
                    break
        self.need(len(self.data) >= 28, "too short for a Mach-O header")
        magic, cpu, _, _, ncmds, sizeofcmds, _ = struct.unpack_from(">7I", self.data, 0)
        self.need(magic == 0xFEEDFACE and cpu == CPU_PPC,
                  "not a 32-bit PowerPC Mach-O or a universal binary with one")
        commands_end = 28 + sizeofcmds
        self.need(commands_end <= len(self.data), "load commands run past the file")
        self.sections = []  # (segment, section, address, size, file offset)
        self.symtab = None
        offset = 28
        for _ in range(ncmds):
            self.need(offset + 8 <= commands_end, "load commands run past sizeofcmds")
            cmd, size = struct.unpack_from(">2I", self.data, offset)
            self.need(size >= 8 and offset + size <= commands_end, f"bad load command size {size}")
            if cmd == 1:  # LC_SEGMENT
                self.need(size >= 56, "LC_SEGMENT too short")
                nsects = struct.unpack_from(">I", self.data, offset + 48)[0]
                self.need(56 + 68 * nsects <= size, "LC_SEGMENT sections run past the command")
                for i in range(nsects):
                    s = offset + 56 + 68 * i
                    sect = self.data[s:s + 16].rstrip(b"\0").decode("latin1")
                    seg = self.data[s + 16:s + 32].rstrip(b"\0").decode("latin1")
                    addr, sz, off = struct.unpack_from(">3I", self.data, s + 32)
                    self.need(not off or off + sz <= len(self.data), f"section {seg},{sect} runs past the file")
                    self.sections.append((seg, sect, addr, sz, off))
            elif cmd == 2:  # LC_SYMTAB
                self.need(size >= 24, "LC_SYMTAB too short")
                self.symtab = struct.unpack_from(">4I", self.data, offset + 8)
                symoff, nsyms, stroff, strsize = self.symtab
                self.need(symoff + 12 * nsyms <= len(self.data) and stroff + strsize <= len(self.data),
                          "symbol or string table runs past the file")
            offset += size
        self.need(offset == commands_end, "load commands do not fill sizeofcmds")
        self.need(self.symtab is not None, "no symbol table (LC_SYMTAB): a stripped build has no names")

    def need(self, ok, problem):
        if not ok:
            raise SystemExit(f"{self.path}: {problem}")

    def section(self, seg, sect):
        found = next((s for s in self.sections if s[0] == seg and s[1] == sect), None)
        self.need(found is not None, f"no {seg},{sect} section")
        return found

    def bytes_at(self, addr, n):
        for _, _, start, size, off in self.sections:
            if start <= addr < start + size and off:
                return self.data[off + addr - start:off + addr - start + n]
        return b""

    def word(self, addr):
        raw = self.bytes_at(addr, 4)
        return struct.unpack(">I", raw)[0] if len(raw) == 4 else None

    def name(self, strx):
        symoff, nsyms, stroff, strsize = self.symtab
        self.need(strx < strsize, f"symbol name index {strx} outside the string table")
        end = self.data.find(b"\0", stroff + strx, stroff + strsize)
        self.need(end >= 0, f"symbol name at string index {strx} is not NUL-terminated")
        return self.data[stroff + strx:end].decode("latin1")

    def symbols(self):
        """[(address, section number, name)] of defined, non-debug symbols."""
        symoff, nsyms, stroff, _ = self.symtab
        out = []
        for i in range(nsyms):
            strx, ntype, sect, _, value = struct.unpack_from(">IBBhI", self.data, symoff + 12 * i)
            if ntype & 0xE0 or ntype & 0x0E != 0x0E:
                continue
            out.append((value, sect, self.name(strx)))
        return out

    def objects(self):
        """{function start: object file} from the debug map: an N_OSO names
        the object, its N_FUN entries follow, and an N_SO opens or closes a
        compile unit, so an N_FUN after it belongs to no object until the next
        N_OSO. {} for a build without a debug map."""
        symoff, nsyms, stroff, _ = self.symtab
        found, current = {}, None
        for i in range(nsyms):
            strx, ntype, _, _, value = struct.unpack_from(">IBBhI", self.data, symoff + 12 * i)
            if ntype not in (N_FUN, N_SO, N_OSO):
                continue
            name = self.name(strx)
            if ntype == N_OSO:
                current = name.rsplit("/", 1)[-1]
            elif ntype == N_SO:
                current = None
            elif name and current:
                found[value] = current
        return found


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
    tables = [(a, n) for a, _, n in syms if n.startswith("__ZTV")]
    vtables = {d.replace("vtable for ", ""): a
               for (a, _), d in zip(tables, demangle([n for _, n in tables], tool))}
    stubs = [(s[2], s[2] + s[3]) for s in mac.sections if "stub" in s[1]]
    return ends, names, vtables, stubs


def sign16(v):
    return v - 0x10000 if v & 0x8000 else v


LOADS = {32, 33, 34, 35, 40, 41, 42, 43}
FLOAT_LOADS = {48, 49, 50, 51}
STORES = {36, 37, 38, 39, 44, 45, 52, 53, 54, 55}
UPDATES = {33, 35, 37, 39, 41, 43, 45, 49, 51, 53, 55}  # rA becomes the effective address
VOLATILE = (0, *range(3, 13))  # what a call may change
NONVOLATILE = range(13, 32)  # what a call keeps
WRITE_RA = {20, 21, 23, 25, 26, 27, 28, 29}  # rlwimi, rlwinm, rlwnm, oris, xori, xoris, andi., andis.: rA from rS
WRITE_RD = {7, 8, 12, 13}  # mulli, subfic, addic, addic.
# opcode 31 by extended opcode: logical and shift forms write rA from rS;
# compares, plain stores, cache and sync operations write no register;
# update-form stores and float loads write rA, update-form loads rD and rA.
# Any other opcode 31 instruction is taken to write both rD and rA.
X31_WRITE_RA = {28, 60, 124, 284, 316, 412, 444, 476, 24, 536, 792, 824, 26, 922, 954}
X31_WRITE_NONE = {0, 32, 4, 144, 146, 150, 151, 215, 407, 661, 662, 663, 725, 727, 918, 983,
                  54, 86, 246, 278, 470, 982, 1014, 598, 854, 535, 599}
X31_WRITE_RA_ONLY_UPDATE = {183, 247, 439, 567, 631, 695, 759}
X31_OR = 444


def ppc_step(w, pc, regs, lr, ref):
    """The register values (dict) and link register after one instruction,
    given those before it; ref(address) is called for each address it forms.
    A call (bl, bctrl, blrl, a linked bc) forgets the volatile registers and
    the link register, an update-form load or store moves rA to its
    effective address, and lmw, lswi and lswx forget what they load."""
    regs = dict(regs)
    op, rd, ra, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
    xo = (w >> 1) & 0x3FF

    def called():
        for r in VOLATILE:
            regs.pop(r, None)

    if op == 18:  # b / bl
        if w & 1:
            called()
            lr = None
    elif w == 0x429F0005:  # bcl 20,31,$+4
        lr = pc + 4
    elif op == 16 and w & 1:  # other linked bc
        called()
        lr = None
    elif op == 19 and xo in (16, 528) and w & 1:  # bclrl / bcctrl (blrl, bctrl)
        called()
        lr = None
    elif op == 31 and xo == 339 and ((w >> 11) & 0x3FF) == 0x100:  # mflr
        regs.pop(rd, None)
        if lr is not None:
            regs[rd] = lr
    elif op == 31 and xo == 467 and ((w >> 11) & 0x3FF) == 0x100:  # mtlr
        lr = regs.get(rd)
    elif op == 31 and xo == 597:  # lswi: ceil(NB / 4) registers from rD, wrapping
        nb = (w >> 11) & 31 or 32
        for k in range((nb + 3) // 4):
            regs.pop((rd + k) % 32, None)
    elif op == 31 and xo == 533:  # lswx: a count from XER
        regs.clear()
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
        address = (regs[ra] + sign16(imm)) & 0xFFFFFFFF if ra and ra in regs else None
        if address is not None:
            ref(address)
        if op in UPDATES:
            regs.pop(ra, None)
            if address is not None:
                regs[ra] = address
        if op in LOADS:
            regs.pop(rd, None)
    elif op == 46:  # lmw rD..r31
        for r in range(rd, 32):
            regs.pop(r, None)
    elif op == 31 and xo == X31_OR and rd == (w >> 11) & 31:  # mr rA,rS
        regs.pop(ra, None)
        if rd in regs:
            regs[ra] = regs[rd]
    elif op == 31 and (xo in X31_WRITE_RA or xo in X31_WRITE_RA_ONLY_UPDATE):
        regs.pop(ra, None)
    elif op == 31 and xo in X31_WRITE_NONE:
        pass
    elif op in WRITE_RA:
        regs.pop(ra, None)
    elif op in WRITE_RD:
        regs.pop(rd, None)
    elif op == 31:
        regs.pop(rd, None)
        regs.pop(ra, None)
    return regs, lr


def ppc_branch_target(w, pc):
    """The target of a b or bc instruction."""
    if w >> 26 == 18:
        li = w & 0x03FFFFFC
        return (li - 0x04000000 if li & 0x02000000 else li) + (0 if w & 2 else pc)
    bd = w & 0xFFFC
    return (bd - 0x10000 if bd & 0x8000 else bd) + (0 if w & 2 else pc)


def ppc_flow(words, fn):
    """(successors of each instruction by its branches, block starts, bctr
    indices). A bctr has no successors yet: see ppc_jump_table. Block starts
    are the branch targets and the instructions after a branch that does not
    fall through, but not the one after bcl 20,31,$+4, which only reads the
    PC."""
    n = len(words)

    def index(target):
        i, rest = divmod(target - fn, 4)
        return i if not rest and 0 <= i < n else None

    succ, leaders, jumps = [], set(), []
    for i, w in enumerate(words):
        op, link = w >> 26, w & 1
        always = (w >> 21) & 0x14 == 0x14  # BO: branch whatever the condition
        out = [i + 1]
        if w == 0x429F0005:
            pass
        elif op in (16, 18):
            target = index(ppc_branch_target(w, fn + 4 * i))
            if target is not None:
                leaders.add(target)
            if op == 18 and not link:
                out = [target]  # outside the function: a tail call
            elif op == 16 and (not always or link):
                out = [i + 1, target]
            elif op == 16:
                out = [target]
        elif op == 19 and (w >> 1) & 0x3FF in (16, 528) and always and not link:  # blr / bctr
            out = []
            if (w >> 1) & 0x3FF == 528:
                jumps.append(i)
        if i + 1 not in out and i + 1 < n:
            leaders.add(i + 1)
        succ.append([s for s in out if s is not None and s < n])
    return succ, sorted(leaders), jumps


MAX_CASES = 1024


def ppc_jump_table(words, fn, before, j, read):
    """The case indices a bctr at j goes to, read from its jump table, or
    None. GCC's switch loads the case from a table whose address a register
    holds, either as an address (lwzx rX,rT,rI; mtctr rX) or as an offset
    from the table (lwzx; add rX,rX,rT; mtctr rX). Entries are read while
    they land in the function."""
    n = len(words)
    mtctr = next((k for k in range(j - 1, max(j - 8, -1), -1)
                  if words[k] >> 26 == 31 and (words[k] >> 1) & 0x3FF == 467
                  and (words[k] >> 11) & 0x3FF == 0x120), None)
    if mtctr is None or before[mtctr] is None:
        return None
    target_reg = (words[mtctr] >> 21) & 31
    relative, base = False, None
    for k in range(mtctr - 1, max(mtctr - 8, -1), -1):
        w = words[k]
        if w >> 26 != 31 or (w >> 21) & 31 != target_reg or before[k] is None:
            continue
        ra, rb, xo = (w >> 16) & 31, (w >> 11) & 31, (w >> 1) & 0x3FF
        regs = before[k][0]
        if xo == 266 and not relative:  # add rX,rA,rB: one of them is the table
            known = [regs[r] for r in (ra, rb) if r in regs]
            if len(known) == 1:
                relative, base = True, known[0]
                target_reg = rb if ra in regs else ra
                continue
            return None
        if xo == 23:  # lwzx rX,rA,rB: one of them is the table
            known = [regs[r] for r in (ra, rb) if r in regs]
            if len(known) != 1 or (relative and known[0] != base):
                return None
            base = known[0]
            break
        return None
    else:
        return None
    cases = []
    for t in range(MAX_CASES):
        entry = read(base + 4 * t)
        if entry is None:
            break
        target = (base + (entry - 0x100000000 if entry & 0x80000000 else entry)) & 0xFFFFFFFF \
            if relative else entry
        i, rest = divmod(target - fn, 4)
        if rest or not 0 <= i < n:
            break
        cases.append(i)
    return sorted(set(cases)) or None


def ppc_meet(a, b):
    """What two paths agree on: the registers with one value on both, the
    link register if it is the same."""
    if a is None:
        return b
    return {r: v for r, v in a[0].items() if b[0].get(r) == v}, a[1] if a[1] == b[1] else None


def ppc_states(words, fn, succ):
    """The register values and link register known before each instruction.
    From the entry they follow the branches, keeping where paths meet only
    what they agree on. Code no path from the entry reaches (an exception
    landing pad, or dead code) is worked out apart, never merged into the
    reached code: it starts from the non-volatile registers every reached
    call leaves intact, since a landing pad is entered from a call."""
    n = len(words)
    before = [None] * n

    def spread(start, state, allowed):
        before[start] = state
        work = [start]
        while work:
            i = work.pop()
            after = ppc_step(words[i], fn + 4 * i, *before[i], lambda address: None)
            for s in succ[i]:
                if s not in allowed:
                    continue
                merged = ppc_meet(before[s], after)
                if merged != before[s]:
                    before[s] = merged
                    work.append(s)

    if not n:
        return before
    spread(0, ({}, None), range(n))
    reached = {i for i in range(n) if before[i] is not None}
    kept = None
    for i in sorted(reached):
        w = words[i]
        if (w >> 26 == 18 or w >> 26 == 16 or (w >> 26 == 19 and (w >> 1) & 0x3FF in (16, 528))) and w & 1:
            regs = {r: v for r, v in before[i][0].items() if r in NONVOLATILE}
            kept = ppc_meet(kept, (regs, None))
    unreached = set(range(n)) - reached
    for start in sorted(unreached):
        if before[start] is None:
            spread(start, kept or ({}, None), unreached)
    return before


def ppc_analyse(words, fn, read):
    """(the states before each instruction, successors). Each bctr gets its
    jump table's cases once the states show the table's address; a bctr
    whose table is not found may go to any block start."""
    succ, leaders, jumps = ppc_flow(words, fn)
    pending = list(jumps)
    while True:
        before = ppc_states(words, fn, succ)
        found = False
        for j in list(pending):
            cases = ppc_jump_table(words, fn, before, j, read)
            if cases:
                succ[j] = cases
                pending.remove(j)
                found = True
        if not found:
            break
    if pending:
        for j in pending:
            succ[j] = leaders
        before = ppc_states(words, fn, succ)
    return before, succ


def scan_ppc(mac, ends, strings):
    """{function: (callees in order, string set)}. Tracks lis/addis/addi/ori
    register values (and the bcl/mflr PIC base) to find string addresses,
    along the function's branches and jump tables (see ppc_step,
    ppc_analyse).
    Callees are the targets of b, bl and bc outside the function."""
    out = {}
    for fn, end in ends.items():
        code = mac.bytes_at(fn, end - fn)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code) - 3, 4)]
        before, _ = ppc_analyse(words, fn, mac.word)
        calls, texts = [], set()

        def ref(value):
            if value in strings:
                texts.add(strings[value])

        for i, w in enumerate(words):
            pc = fn + 4 * i
            if w >> 26 in (16, 18):
                target = ppc_branch_target(w, pc)
                if target in ends and target != fn and not (fn <= target < end):
                    calls.append(target)
            ppc_step(w, pc, *before[i], ref)
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
        self.p, self.w, self.method, self.signature, self.conflicts = {}, {}, {}, {}, Counter()

    def add(self, w, p, method, signature=""):
        if p is None or w is None:
            return False
        if self.p.get(w) == p:
            return False
        if w in self.p or p in self.w:
            self.conflicts[method] += 1
            return False
        self.p[w], self.w[p], self.method[w], self.signature[w] = p, w, method, signature
        return True


def pair_names(pairs, wnames, pindex, allowed, rejects=None):
    """Pair each allowed Windows name with the one Mac function of that name
    whose parameters are compatible (see signature_match), preferring exact
    parameters, and only then choose among GCC's ctor/dtor copies. A name
    whose Mac candidates all fail on their parameters goes to rejects as
    (address, [Mac starts])."""
    keys = Counter((q, a) for w, (q, a, k, _) in wnames.items() if w in allowed)
    for w, (qual, params, kind, _) in wnames.items():
        if w not in allowed or keys[(qual, params)] > 1:
            continue
        named = pindex.get(qual, [])
        cands = [c for c in named if signature_match(params, c[1])]
        cands = [c for c in cands if signature_match(params, c[1]) == "exact"] or cands
        if named and not cands and rejects is not None:
            rejects.append((w, [c[0] for c in named]))
        if kind in VARIANTS:
            cands = next((found for wanted in VARIANTS[kind]
                          if (found := [c for c in cands if c[2] in wanted])), [])
        if len(cands) == 1:
            pairs.add(w, cands[0][0], "name", signature_match(params, cands[0][1]))


MIN_FOLD_SAFE = 16  # /OPT:ICF folds tiny bodies across classes


def pair_vtables(pairs, wtables, ptables, pnames, wsize, stats):
    """Slot-by-slot pairs. The Mac table's length is exact; the Windows read
    may run on into the next table, so only a Windows table at least as long
    is used, and only when no slot contradicts a pair already made. Skipped:
    overloaded virtuals (MSVC orders an overload set in reverse, GCC in
    declaration order), tiny bodies that may be folded, and a slot body seen
    with two different partners."""
    proposals = defaultdict(set)
    for cls, wslots in wtables.items():
        pslots = ptables.get(cls)
        if pslots is None:
            continue
        if len(wslots) < len(pslots):
            stats["vtable size differs"] += 1
            continue
        method = [pnames[p][1].split("(")[0].split("::")[-1] if p else None for p in pslots]
        overloaded = {n for n, c in Counter(method).items() if c > 1}
        usable = [(w, p) for w, p, name in zip(wslots, pslots, method)
                  if p is not None and name not in overloaded and wsize.get(w, 0) >= MIN_FOLD_SAFE]
        if any(pairs.p.get(w, p) != p or pairs.w.get(p, w) != w for w, p in usable):
            stats["vtables contradicted"] += 1  # a known pair says the slots are misaligned
            continue
        stats["vtables paired"] += 1
        for w, p in usable:
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
    when min_votes distinct paired functions make it (one that is both caller
    and callee counts once) and nothing competes with it."""
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
        anchors = defaultdict(set)  # proposal: the paired functions that make it
        for w, p in pairs.p.items():
            for wl, pl in ((wscan.get(w, ([],))[0], pscan.get(p, ([],))[0]),
                           (wcallers.get(w, []), pcallers.get(p, []))):
                wl = [x for x in wl if x in wgame and x not in pairs.p]
                pl = [x for x in pl if x not in pairs.w]
                if len(wl) == 1 and len(pl) == 1 and plausible(wl[0], pl[0]):
                    anchors[(wl[0], pl[0])].add(w)
        wseen = Counter(w for w, _ in anchors)
        pseen = Counter(p for _, p in anchors)
        changed = False
        for (w, p), by in anchors.items():
            if len(by) >= min_votes and wseen[w] == 1 and pseen[p] == 1:
                changed |= pairs.add(w, p, method)


METHODS = ("name", "vtable", "strings", "calls", "calls1")


class Original:
    """What the pairing needs from the Windows side."""
    def __init__(self, path):
        self.win = Windows(path)
        self.game, self.starts, self.ends, self.crt, self.unlisted = windows_functions(self.win)
        self.scan = scan_windows(self.win, self.starts, self.ends, self.game)
        self.names, self.ambiguous = windows_names()
        self.tables = windows_vtables(self.win)
        self.size = {w: self.ends[w] - w for w in self.ends}
        self.named = {w for w in self.names if w in self.game and w not in self.ambiguous}


class MacBuild:
    """One Mac executable: its functions, names, scans and debug-map objects."""
    def __init__(self, path, tool):
        self.label = Path(path).name
        mac = MachO(path)
        cstring = mac.section("__TEXT", "__cstring")
        strings = strings_in(mac.bytes_at(cstring[2], cstring[3]), cstring[2])
        self.ends, self.names, vtables, stubs = ppc_functions(mac, tool)
        self.scan = scan_ppc(mac, self.ends, strings)
        self.index = ppc_index(self.names)
        self.tables = {c: ppc_vtable(mac, a, self.ends, self.names, stubs) for c, a in vtables.items()}
        self.size = {p: self.ends[p] - p for p in self.ends}
        self.objects = mac.objects()


def run(w, mac, allowed, stats, rejects=None, seeds=None):
    """Pair one build. seeds, a Pairs of name pairs made beforehand, stands
    in for the name signal; the others start from the allowed ones."""
    pairs = Pairs()
    if seeds is None:
        pair_names(pairs, w.names, mac.index, allowed, rejects)
    else:
        for x, p in seeds.p.items():
            if x in allowed:
                pairs.add(x, p, "name", seeds.signature[x])
    pair_vtables(pairs, w.tables, mac.tables, mac.names, w.size, stats)
    pair_strings(pairs, w.scan, mac.scan, w.game)
    pair_calls(pairs, w.scan, mac.scan, w.game, w.size, mac.size, 2)
    pair_strings(pairs, w.scan, mac.scan, w.game)
    pair_calls(pairs, w.scan, mac.scan, w.game, w.size, mac.size, 1)
    return pairs


def holdout(w, builds, share, seed):
    """Precision and coverage of signals 2 to 5. The name pairs are frozen
    first, with every name known: the hidden ones are the reference, the
    rest seed the other signals as they are, without pairing names again. One
    seeded sample is hidden from every build, so each build and the
    combination are measured on the same functions."""
    references = []
    for mac in builds:
        reference = Pairs()
        pair_names(reference, w.names, mac.index, w.named)
        references.append(reference)
    targets = sorted(set().union(*(r.p for r in references)))
    hidden = set(random.Random(seed).sample(targets, int(len(targets) * share)))
    print(f"{len(hidden)} of {len(targets)} name pairs hidden (seed {seed})")
    results = []
    for mac, reference in zip(builds, references):
        pairs = run(w, mac, w.named - hidden, Counter(), seeds=reference)
        results.append(pairs)
        eligible = hidden & reference.p.keys()
        print(f"{mac.label}: {len(eligible)} of them paired by name in this build")
        print(f"  {'method':8} {'recovered':>9} {'correct':>8} {'precision':>9}")
        total = 0
        for method in METHODS[1:]:
            got = [x for x in eligible if pairs.method.get(x) == method]
            ok = sum(pairs.p[x] == reference.p[x] for x in got)
            total += len(got)
            print(f"  {method:8} {len(got):9} {ok:8} {ok / len(got) if got else 0:9.1%}")
        print(f"  coverage {total} of {len(eligible)} ({total / len(eligible) if eligible else 0:.1%})")
    if len(builds) < 2:
        return
    truth = defaultdict(set)  # hidden function: the signatures its name pairs give it
    for mac, reference in zip(builds, references):
        for x in hidden & reference.p.keys():
            truth[x].add(split_name(mac.names[reference.p[x]][1]))
    every = f"{len(builds)}/{len(builds)}"
    got, right = Counter(), Counter()
    for x, (name, _, agree, _) in combine(hidden, builds, results).items():
        group = "conflict" if agree == "conflict" else "all builds" if agree == every else "some builds"
        got[group] += 1
        right[group] += bool(name) and split_name(name) in truth[x]
    print("combined:")
    print(f"  {'builds':11} {'recovered':>9} {'correct':>8} {'precision':>9}")
    for group in ("all builds", "some builds"):
        print(f"  {group:11} {got[group]:9} {right[group]:8} {right[group] / got[group] if got[group] else 0:9.1%}")
    print(f"  {'conflict':11} {got['conflict']:9}        - (left unnamed)")
    named = got["all builds"] + got["some builds"]
    print(f"  coverage {named} of {len(hidden)} named ({named / len(hidden) if hidden else 0:.1%})")


def combine(game, builds, results):
    """{function: (name, best method, agreement, conflicting names)}. Builds
    agree when they pair a function with the same full signature, name and
    parameters; otherwise it stays unnamed and every candidate is listed."""
    out = {}
    for w in game:
        hits = [(mac.names[pairs.p[w]][1], pairs.method[w])
                for mac, pairs in zip(builds, results) if w in pairs.p]
        if not hits:
            continue
        names = sorted({name for name, _ in hits})
        if len({split_name(name) for name in names}) > 1:
            out[w] = ("", "", "conflict", " | ".join(names))
            continue
        method = min((m for _, m in hits), key=METHODS.index)
        out[w] = (hits[0][0], method, f"{len(hits)}/{len(builds)}", "")
    return out


MIN_RUN = 3  # functions in a row from one object before it counts as that object's place


def link_order(sources):
    """[(first, last, object, functions)] in address order: runs of at least
    MIN_RUN functions from one object file, consecutive runs of one object merged."""
    runs = []
    for address, obj in sorted(sources.items()):
        if runs and runs[-1][2] == obj:
            runs[-1][1], runs[-1][3] = address, runs[-1][3] + 1
        else:
            runs.append([address, address, obj, 1])
    order = []
    for run_ in (r for r in runs if r[3] >= MIN_RUN):
        if order and order[-1][2] == run_[2]:
            order[-1][1], order[-1][3] = run_[1], order[-1][3] + run_[3]
        else:
            order.append(run_)
    return order


def write_link_order(path, order):
    with open(path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["order", "first", "last", "object", "functions"])
        for i, (first, last, obj, n) in enumerate(order, 1):
            writer.writerow([i, f"0x{first:08X}", f"0x{last:08X}", obj, n])


def source_how(x, sources, inferred):
    """How a function's source file is known: "debug map" when the function
    itself is paired with a function the debug map places, "neighbours" when
    it only sits inside a run of that file's functions."""
    return "debug map" if x in sources else "neighbours" if x in inferred else ""


def signature_of(x, results):
    """exact, compatible or unknown for a name pair: how its Windows and Mac
    parameters agree (see signature_match); "" for the other signals."""
    return next((pairs.signature[x] for pairs in results if pairs.signature.get(x)), "")


def write_mac_names(path, game, combined, sources, inferred, results):
    """The repository's hint file: each game function with a Mac name or a source file."""
    with open(path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["address", "mac_name", "method", "builds", "conflict", "signature", "source", "source_how"])
        for x in sorted(game):
            name, method, agree, conflict = combined.get(x, ("", "", "", ""))
            source = sources.get(x) or inferred.get(x, "")
            if agree or source:
                writer.writerow([f"0x{x:08X}", name, method, agree, conflict,
                                 signature_of(x, results) if name else "", source,
                                 source_how(x, sources, inferred)])


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
    parser.add_argument("--ppc", nargs="+",
                        default=[p for p in os.environ.get("KOTOR_PPC_EXE", "").split(os.pathsep) if p],
                        help="one or more Mac PowerPC executables or universal binaries")
    parser.add_argument("--out", required=True, help="new output directory, outside the checkout")
    parser.add_argument("--cxxfilt", default=shutil.which("llvm-cxxfilt") or shutil.which("c++filt"))
    parser.add_argument("--holdout", type=float, default=0.0,
                        help="share of name pairs to hide to measure the other signals")
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--repo", action="store_true",
                        help="also write matching/mac_names.csv and matching/link_order.csv")
    args = parser.parse_args()
    if not args.exe or not args.ppc or not args.cxxfilt:
        parser.error("--exe, --ppc and a c++filt (--cxxfilt) are required")
    out = Path(args.out).resolve()
    if out == ROOT or ROOT in out.parents:
        parser.error("--out must be outside the checkout")

    w = Original(args.exe)
    builds = [MacBuild(path, args.cxxfilt) for path in args.ppc]
    if args.holdout:
        holdout(w, builds, args.holdout, args.seed)
        return

    stats = [Counter() for _ in builds]
    rejects = [[] for _ in builds]
    results = [run(w, mac, w.named, s, r) for mac, s, r in zip(builds, stats, rejects)]
    combined = combine(w.game, builds, results)
    sources = {}
    for mac, pairs in zip(builds, results):
        for x, p in pairs.p.items():
            if x in w.game and p in mac.objects:
                sources.setdefault(x, mac.objects[p])
    order = link_order(sources)
    inferred = {}
    for first, last, obj, _ in order:
        for x in w.game:
            if first < x < last and x not in sources:
                inferred[x] = obj
    import todo
    status = {int(r["address"], 16): r["status"] for r in read_csv("status.csv")}
    playable = todo.playable_index()

    out.mkdir(parents=True, exist_ok=False)
    (out / ".gitignore").write_text("*\n")
    with open(out / "pairs.csv", "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["address", "size", "listed", "status", "playable", "repo_name", "repo_name_conflict",
                         "mac_name", "method", "builds", "conflict", "signature", "source", "source_how",
                         "mac_addresses"])
        for x in sorted(w.game):
            name, method, agree, conflict = combined.get(x, ("", "", "", ""))
            where = playable.get(x)
            source = sources.get(x) or inferred.get(x, "")
            how = source_how(x, sources, inferred)
            macs = ";".join(f"{mac.label}:0x{pairs.p[x]:08X}" for mac, pairs in zip(builds, results) if x in pairs.p)
            writer.writerow([f"0x{x:08X}", w.size[x], "no" if x in w.unlisted else "yes",
                             status.get(x, "todo"), where[0].relative_to(ROOT).as_posix() if where else "",
                             w.names.get(x, (None, None, None, ""))[3], "yes" if x in w.ambiguous else "",
                             name, method, agree, conflict, signature_of(x, results) if name else "",
                             source, how, macs])
    for where in [out] + ([MATCHING] if args.repo else []):
        if order:
            write_link_order(where / "link_order.csv", order)
    if args.repo:
        write_mac_names(MATCHING / "mac_names.csv", w.game, combined, sources, inferred, results)
    with open(out / "signature_rejects.csv", "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["build", "address", "repo_name", "mac_candidates"])
        for mac, refused in zip(builds, rejects):
            for x, starts in refused:
                writer.writerow([mac.label, f"0x{x:08X}", w.names[x][3],
                                 " | ".join(mac.names[p][1] for p in starts)])
    unpaired = Counter()
    with open(out / "mac_unpaired.csv", "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["build", "mac_address", "size", "category", "mac_name"])
        for mac, pairs in zip(builds, results):
            for p in sorted(mac.ends):
                if p not in pairs.w:
                    cat = category(mac.names[p][1])
                    unpaired[(mac.label, cat)] += 1
                    writer.writerow([mac.label, f"0x{p:08X}", mac.size[p], cat, mac.names[p][1]])

    def count(pred):
        return sum(1 for x in w.game if pred(x))

    print(f"Windows game functions: {len(w.game)} ({len(w.game & w.unlisted)} missing from functions.csv; "
          f"stock LIBCMT excluded: {len(w.crt)})")
    print(f"repo names left out of name pairing: {len(w.ambiguous & w.game)} addresses the CSVs name differently")
    for mac, pairs, s, refused in zip(builds, results, stats, rejects):
        print(f"{mac.label}: {len(mac.ends)} functions, {len(mac.objects)} with an object file; "
              f"{s['vtables paired']} vtables paired, {s['vtables contradicted']} contradicted; "
              f"{len(pairs.p)} Windows functions paired "
              + "(" + ", ".join(f"{m} {sum(1 for v in pairs.method.values() if v == m)}" for m in METHODS) + "); "
              + "name pairs by parameters: " + ", ".join(f"{k} {n}" for k, n in sorted(Counter(pairs.signature[x] for x, m in pairs.method.items() if m == "name").items()))
              + f"; refused for parameters {len(refused)}")
    print()
    print(f"{'':42} {'functions':>9}")
    new = lambda x: x in combined and combined[x][0] and x not in w.names
    for label, pred in (
            ("byte-identical (MATCH)", lambda x: status.get(x) == "MATCH"),
            ("attempted (NONMATCH)", lambda x: status.get(x) == "NONMATCH"),
            ("not attempted", lambda x: x not in status),
            ("  of which in the playable src/", lambda x: x not in status and x in playable),
            ("named by the reconstruction", lambda x: x in w.names),
            ("paired with a Mac build", lambda x: x in combined and combined[x][0]),
            ("  paired by several builds, same name", lambda x: x in combined and combined[x][2][0] not in "1c"),
            ("  builds disagree (left unnamed)", lambda x: combined.get(x, ("",) * 3)[2] == "conflict"),
            ("new names (unnamed in the repo)", new),
            (f"  confirmed by all {len(builds)} builds", lambda x: new(x) and combined[x][2] == f"{len(builds)}/{len(builds)}"),
            ("still unnamed", lambda x: x not in w.names and not (x in combined and combined[x][0])),
            ("with an original source file (paired)", lambda x: x in sources),
            ("  placed inside a source-file run", lambda x: x in inferred),
            ("    of them still unnamed", lambda x: x in inferred and x not in w.names
                                                 and not (x in combined and combined[x][0]))):
        print(f"{label:42} {count(pred):9}")
    if order:
        print(f"\ninferred link order: {len(order)} runs over {len({o for _, _, o, _ in order})} object files")
    print("\nMac functions without a Windows partner: "
          + "; ".join(f"{b} {c} {n}" for (b, c), n in sorted(unpaired.items())))
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
