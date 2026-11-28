"""Read an original vtable: each slot's function and the names the evidence gives it.

usage: python matching/tools/vtread.py VTABLE_ADDR NSLOTS [FROM_SLOT]   (KOTOR_ORIGINAL_EXE set)

Names come from the Mac debug map (mac_names.csv), then symbols.csv, then the
annotation in matching/src.
"""
import csv
import os
import re
import struct
import sys
from pathlib import Path

import pefile

vt, n = int(sys.argv[1], 16), int(sys.argv[2])
first = int(sys.argv[3]) if len(sys.argv) > 3 else 0
pe = pefile.PE(os.environ["KOTOR_ORIGINAL_EXE"], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
mac = {}
for r in csv.DictReader(open("matching/mac_names.csv")):
    if r["mac_name"]:
        mac.setdefault(int(r["address"], 16), (r["mac_name"], r["source"]))
sym = {}
for r in csv.DictReader(open("matching/symbols.csv")):
    if r["role"] == "defines":
        sym.setdefault(int(r["address"], 16), r["demangled"])
ann = {}
for f in Path("matching/src").rglob("*.cpp"):
    lines = f.read_text(errors="replace").split("\n")
    for i, l in enumerate(lines):
        m = re.search(r"FUNCTION:\s*SWKOTOR\s+(0x[0-9A-Fa-f]+)", l)
        if m:
            nxt = next((x for x in lines[i + 1:i + 4] if x.strip() and not x.startswith("//")), "")
            ann.setdefault(int(m.group(1), 16), nxt.strip()[:70])
for s in range(first, n):
    a = struct.unpack("<I", pe.get_data(vt - base + 4 * s, 4))[0]
    who = mac.get(a, ("", ""))
    print(f"{s:3} +0x{4 * s:03x} 0x{a:08X}  {who[0][:60]:60} {who[1]:18} {sym.get(a, '')[:60] or ann.get(a, '')}")
