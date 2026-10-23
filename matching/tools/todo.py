#!/usr/bin/env python3
"""List functions that still need byte-identical source, to pick work from.

Reads matching/functions.csv, matching/status.csv and matching/crt_libcmt.csv,
and the playable build's src/ for a reference definition of each function.
Compiler-generated EH funclets and stock LIBCMT functions are never listed.

    python matching/tools/todo.py                      30 smallest unclaimed functions (16 B+)
    python matching/tools/todo.py --near               NONMATCH attempts, closest first
    python matching/tools/todo.py --range 0x5E0000 0x620000 -n 50
    python matching/tools/todo.py --address 0x005B2EF0 print the playable definition
    python matching/tools/todo.py --names none       hide the Mac names and source files

Columns: address, size in bytes, status (todo or NONMATCH with diff count),
and the src/ file that defines kotor_<cc>_<address>, if any. That file is
behaviour-equivalent C++, play-tested, with original instruction addresses
in its comments: read it for semantics and offsets, then write 2002-style
classes. Its raw-offset style never compiles to the original bytes.

Each function is shown with its name in Aspyr's Mac builds, from
matching/mac_names.csv (or --names / KOTOR_PPC_NAMES: another file
ppcnames.py wrote, or "none"), with the signal that
paired it and how many builds agree (2/2 is the most reliable), then its
original source file. A function with no name may still have a source file
from its neighbours. Confirm a name against the code before using it.
"""

from __future__ import annotations

import argparse
import csv
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SUFFIXES = {".cpp", ".h", ".hpp", ".inc"}
DEFINITION = re.compile(
    r"\b[A-Za-z_]\w*?_(00[4-7][0-9a-fA-F]{5})\s*\(([^;{}()]|\([^()]*\))*\)\s*"
    r"(?:const\s*)?(?:noexcept\s*)?(?:->\s*[\w:<>*& ]+)?\{")


def playable_index() -> dict[int, tuple[Path, int]]:
    """address -> (file, line) of the first src/ definition."""
    found: dict[int, tuple[Path, int]] = {}
    for path in sorted(p for p in (ROOT / "src").rglob("*") if p.suffix in SUFFIXES):
        text = path.read_text(encoding="utf-8", errors="replace")
        for m in DEFINITION.finditer(text):
            found.setdefault(int(m.group(1), 16), (path, text.count("\n", 0, m.start()) + 1))
    return found


def body(path: Path, line: int) -> str:
    """The definition starting at line, with the comment block above it."""
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    top = line - 1
    while top > 0 and lines[top - 1].lstrip().startswith("//"):
        top -= 1
    depth, end = 0, line - 1
    for end in range(line - 1, len(lines)):
        depth += lines[end].count("{") - lines[end].count("}")
        if depth <= 0 and "{" in "".join(lines[line - 1:end + 1]):
            break
    return "\n".join(lines[top:end + 1])


def mac_names(path: str | None) -> dict[int, str]:
    """address -> "name [signal, builds] (source file)" from a ppcnames.py pairs.csv."""
    if not path or path == "none" or not Path(path).exists():
        return {}
    found = {}
    with open(path, encoding="utf-8") as source:
        for r in csv.DictReader(source):
            name = f"{r['mac_name']} [{r['method']}, {r['builds']}]" if r["mac_name"] else ""
            where = f"({r['source']})" if r["source"] else ""
            if name or where:
                found[int(r["address"], 16)] = f"{name} {where}".strip()
    return found


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--range", nargs=2, metavar=("LOW", "HIGH"))
    parser.add_argument("--near", action="store_true", help="list NONMATCH attempts instead")
    parser.add_argument("--largest", action="store_true", help="largest first")
    parser.add_argument("-n", type=int, default=30)
    parser.add_argument("--min-size", type=int, default=16,
                        help="skip functions smaller than this many bytes (default 16)")
    parser.add_argument("--address", help="print the playable definition of one function")
    parser.add_argument("--names", default=os.environ.get("KOTOR_PPC_NAMES",
                                                          str(ROOT / "matching" / "mac_names.csv")),
                        help='Mac names from ppcnames.py (default matching/mac_names.csv; "none" to hide)')
    args = parser.parse_args()
    names = mac_names(args.names)

    index = playable_index()
    if args.address:
        hit = index.get(int(args.address, 16))
        if not hit:
            sys.exit(f"src/ has no definition named for {args.address}")
        print(f"== {hit[0].relative_to(ROOT).as_posix()}:{hit[1]}")
        if int(args.address, 16) in names:
            print(f"// Mac name: {names[int(args.address, 16)]}")
        print(body(*hit))
        return

    with (ROOT / "matching" / "functions.csv").open(encoding="utf-8") as source:
        kinds = {int(r["address"], 16): r["kind"] for r in csv.DictReader(source)}
    with (ROOT / "matching" / "status.csv").open(encoding="utf-8") as source:
        status = {int(r["address"], 16): r for r in csv.DictReader(source)}
    with (ROOT / "matching" / "crt_libcmt.csv").open(encoding="utf-8") as source:
        library = {int(r["address"], 16) for r in csv.DictReader(source)}
    for address in status:
        kinds.setdefault(address, "game")
    starts = sorted(kinds)
    low, high = (int(args.range[0], 16), int(args.range[1], 16)) if args.range else (0, 1 << 32)
    rows = []
    for i, address in enumerate(starts):
        size = starts[i + 1] - address if i + 1 < len(starts) else 0
        row = status.get(address)
        if (kinds[address] == "eh" or address in library or not low <= address < high
                or (row and row["status"] == "MATCH")):
            continue
        if args.near != bool(row) or size < args.min_size:
            continue
        state = f"NONMATCH {row['diff_insns']}" if row else "todo"
        where = index.get(address)
        rows.append((int(row["diff_insns"]) if row else 0, size, address, state,
                     where[0].relative_to(ROOT).as_posix() if where else "-"))
    rows.sort(key=lambda r: (r[0], -r[1] if args.largest else r[1], r[2]))
    print(f"{len(rows):,} functions; showing {min(args.n, len(rows))}")
    for _, size, address, state, where in rows[:args.n]:
        name = f"  {names[address]}" if address in names else ""
        print(f"0x{address:08X}  {size:6,} B  {state:<14} {where}{name}")


if __name__ == "__main__":
    main()
