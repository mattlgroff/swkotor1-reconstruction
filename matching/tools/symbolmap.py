"""Map the symbols used by matched code to addresses in the original exe.

Every byte-identical function pins down more than itself: each relocation in
its object file (a call, a global, a vtable, a string) sits where the
original has the real address. Reading the original at that spot gives the
address of the symbol the relocation names. This is the bridge between the
two builds:

- a matched function defines a symbol at a known address, so the playable
  build's kotor_<cc>_<address> definition can be replaced by it;
- the symbols it calls resolve to addresses, so in the playable build they
  can forward to that build's existing kotor_<cc>_<address> definitions.

Writes matching/symbols.csv: symbol, demangled, address, role (defines /
calls / data), uses. Only MATCH rows of matching/status.csv are used, so
every address comes from bytes proven identical. A symbol seen at two
different addresses is reported and left out.

usage: python matching/tools/symbolmap.py --msvc DIR --exe swkotor.exe
"""
import argparse
import csv
import os
import struct
import sys
import tempfile
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import pefile

sys.path.insert(0, str(Path(__file__).resolve().parent))
import matchcmp  # noqa: E402

ROOT = matchcmp.ROOT
OUTPUT = ROOT / "matching" / "symbols.csv"
REL32, DIR32 = 0x14, 0x06


def relocated_functions(obj):
    """{function symbol: [(offset, type, target symbol, target is code)]}."""
    data = obj.read_bytes()
    _, nsect, _, symptr, nsyms, opt, _ = struct.unpack_from("<HHIIIHH", data, 0)
    sections = []
    for i in range(nsect):
        off = 20 + opt + i * 40
        _, _, rel_ptr, _, nrel, _, flags = struct.unpack_from("<IIIIHHI", data, off + 16)
        sections.append((rel_ptr, nrel, flags))
    strtab = symptr + nsyms * 18
    names, kinds, defined = {}, {}, []
    i = 0
    while i < nsyms:
        entry = data[symptr + i * 18:symptr + i * 18 + 18]
        if entry[:4] == b"\0\0\0\0":
            start = strtab + struct.unpack_from("<I", entry, 4)[0]
            name = data[start:data.index(b"\0", start)].decode("latin-1")
        else:
            name = entry[:8].rstrip(b"\0").decode("latin-1")
        value, secnum, typ, klass, naux = struct.unpack_from("<IhHBB", entry, 8)
        names[i] = name
        kinds[i] = typ == 0x20
        if typ == 0x20 and secnum > 0 and klass in (2, 3):
            defined.append((name, secnum, value))
        i += 1 + naux
    result = {}
    for name, secnum, value in defined:
        rel_ptr, nrel, _ = sections[secnum - 1]
        relocs = []
        for r in range(nrel):
            vaddr, index, rtype = struct.unpack_from("<IIH", data, rel_ptr + r * 10)
            if rtype in (REL32, DIR32) and vaddr >= value:
                relocs.append((vaddr - value, rtype, names[index], kinds[index]))
        result[name] = relocs
    return result


def scan(src, flags, matched, pe):
    """[(symbol, address, role)] for the matched functions in one source."""
    wanted = [(a, q) for a, q in matchcmp.annotated_functions(src) if a in matched]
    if not wanted:
        return []
    with tempfile.TemporaryDirectory() as tmp:
        funcs = relocated_functions(matchcmp.compile_source(src, flags, tmp))
    readable = matchcmp.demangle(list(funcs))
    base = pe.OPTIONAL_HEADER.ImageBase
    out = []
    for address, qualname in wanted:
        hits = [s for s in funcs if f"{qualname}(" in readable[s]
                or readable[s].endswith(qualname) or s == qualname]
        if len(hits) != 1:
            continue
        symbol = hits[0]
        out.append((symbol, address, "defines"))
        for offset, rtype, target, is_code in funcs[symbol]:
            if target.startswith("$") or target == "__except_list":
                continue  # file-local labels and the fs:[0] SEH head
            value = struct.unpack("<I", pe.get_data(address - base + offset, 4))[0]
            if rtype == REL32:
                out.append((target, (address + offset + 4 + value) & 0xFFFFFFFF, "calls"))
            else:
                out.append((target, value, "calls" if is_code else "data"))
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--msvc", default=os.environ.get("KOTOR_MSVC7"))
    parser.add_argument("--exe", default=os.environ.get("KOTOR_ORIGINAL_EXE"))
    parser.add_argument("--status", default=ROOT / "matching" / "status.csv")
    parser.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4)
    args = parser.parse_args()
    if not args.msvc or not args.exe:
        sys.exit("pass --msvc and --exe (or set KOTOR_MSVC7 and KOTOR_ORIGINAL_EXE)")
    matchcmp.MSVC, matchcmp.EXE = Path(args.msvc), Path(args.exe)
    with open(args.status, encoding="utf-8") as source:
        rows = [r for r in csv.DictReader(source) if r["status"] == "MATCH"]
    matched = {int(r["address"], 16) for r in rows}
    sources = sorted({ROOT / r["file"] for r in rows})
    pe = pefile.PE(str(args.exe), fast_load=True)
    matchcmp.start_wineserver()
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        found = [x for part in pool.map(
            lambda s: scan(s, matchcmp.DEFAULT_FLAGS, matched, pe), sources) for x in part]

    addresses, roles, uses = defaultdict(Counter), defaultdict(set), Counter()
    for symbol, address, role in found:
        addresses[symbol][address] += 1
        roles[symbol].add(role)
        uses[symbol] += 1
    clean = {s: next(iter(c)) for s, c in addresses.items() if len(c) == 1}
    conflicts = sorted(s for s, c in addresses.items() if len(c) > 1)
    readable = matchcmp.demangle(sorted(clean))
    with OUTPUT.open("w", newline="", encoding="utf-8") as out:
        writer = csv.writer(out, lineterminator="\n")
        writer.writerow(["symbol", "demangled", "address", "role", "uses"])
        for symbol in sorted(clean, key=lambda s: (clean[s], s)):
            role = "defines" if "defines" in roles[symbol] else sorted(roles[symbol])[0]
            writer.writerow([symbol, readable.get(symbol, symbol),
                             f"0x{clean[symbol]:08X}", role, uses[symbol]])
    by_role = Counter("defines" if "defines" in roles[s] else sorted(roles[s])[0] for s in clean)
    print(f"{len(clean):,} symbols mapped ({dict(by_role)}); "
          f"{len(conflicts)} left out for mapping to several addresses -> "
          f"{OUTPUT.relative_to(ROOT)}")
    for symbol in conflicts[:10]:
        print("  conflict:", symbol, [f"0x{a:08X}" for a in addresses[symbol]])


if __name__ == "__main__":
    main()
