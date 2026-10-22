"""Find functions from a COFF static library (.lib) inside swkotor.exe.

For every code section in every member object, split it at its function
symbols, mask bytes covered by relocations, and search the exe's .text for
an exact masked match. Prints a CSV of (exe address, symbol, size, object).

usage: python matching/tools/libmatch.py <msvc7>/LIB/LIBCMT.LIB <swkotor.exe> out.csv

matching/crt_libcmt.csv keeps the hits at 0x006F0000 and up with a single
placement (LIBCMT, LIBCPMT, LIBCIMT). Functions under 6 bytes are skipped.
"""
import bisect
import csv
import struct
import sys
from collections import defaultdict

import pefile

MIN_SIZE = 6


def ar_members(data):
    assert data[:8] == b"!<arch>\n"
    pos, longnames = 8, b""
    while pos + 60 <= len(data):
        header = data[pos:pos + 60]
        name = header[:16].decode("latin-1").strip()
        size = int(header[48:58].decode().strip())
        body = data[pos + 60:pos + 60 + size]
        if name == "//":
            longnames = body
        elif name not in ("/", ""):
            if name.startswith("/") and name[1:].isdigit():
                off = int(name[1:])
                end = longnames.find(b"\0", off)
                name = longnames[off:end if end >= 0 else None].decode("latin-1").rstrip("/\n")
            yield name.rstrip("/"), body
        pos += 60 + size + (size & 1)


def coff_functions(obj):
    """[(symbol, code bytes, masked offset set)] for functions in code sections."""
    machine, nsec, _, symptr, nsym, optsize, _ = struct.unpack_from("<HHIIIHH", obj, 0)
    if machine != 0x14C or nsym == 0:
        return []
    sections = []
    for i in range(nsec):
        off = 20 + optsize + i * 40
        name, vsize, vaddr, rawsize, rawptr, relptr, _, nrel, _, flags = struct.unpack_from(
            "<8sIIIIIIHHI", obj, off)
        sections.append((rawsize, rawptr, relptr, nrel, flags))
    strtab = symptr + nsym * 18

    def symname(raw):
        if raw[:4] == b"\0\0\0\0":
            o = struct.unpack_from("<I", raw, 4)[0]
            return obj[strtab + o:obj.index(b"\0", strtab + o)].decode("latin-1")
        return raw.rstrip(b"\0").decode("latin-1")

    per_section = defaultdict(list)
    i = 0
    while i < nsym:
        raw, value, secnum, typ, cls, naux = struct.unpack_from("<8sIhHBB", obj, symptr + i * 18)
        if secnum > 0 and cls in (2, 3) and (typ >> 4) == 2 or (secnum > 0 and cls == 2):
            per_section[secnum].append((value, symname(raw)))
        i += 1 + naux
    out = []
    for secnum, syms in per_section.items():
        rawsize, rawptr, relptr, nrel, flags = sections[secnum - 1]
        if not flags & 0x20 or rawptr == 0:  # code only
            continue
        code = obj[rawptr:rawptr + rawsize]
        masked = set()
        for r in range(nrel):
            vaddr, _, rtype = struct.unpack_from("<IIH", obj, relptr + r * 10)
            if rtype in (0x06, 0x07, 0x14):  # DIR32, DIR32NB, REL32
                masked.update(range(vaddr, vaddr + 4))
        starts = sorted(set(syms))
        for k, (value, name) in enumerate(starts):
            end = starts[k + 1][0] if k + 1 < len(starts) else rawsize
            if end - value >= MIN_SIZE:
                out.append((name, code[value:end],
                            {m - value for m in masked if value <= m < end}))
    return out


def main():
    lib, exe, out = sys.argv[1:4]
    pe = pefile.PE(exe, fast_load=True)
    text = next(s for s in pe.sections if s.Name.rstrip(b"\0") == b".text")
    base = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress
    image = text.get_data()
    data = open(lib, "rb").read()
    rows, total, objs = [], 0, 0
    for member, body in ar_members(data):
        objs += 1
        for name, code, masked in coff_functions(body):
            total += 1
            # anchor: longest unmasked run
            best, run_start = (0, 0), None
            for j in range(len(code) + 1):
                if j < len(code) and j not in masked:
                    run_start = j if run_start is None else run_start
                else:
                    if run_start is not None and j - run_start > best[0]:
                        best = (j - run_start, run_start)
                    run_start = None
            length, anchor = best
            if length < 4:
                continue
            needle = code[anchor:anchor + min(length, 32)]
            hits = []
            pos = image.find(needle)
            while pos >= 0 and len(hits) < 4:
                start = pos - anchor
                if start >= 0 and all(image[start + b] == code[b] for b in range(len(code))
                                      if b not in masked):
                    hits.append(start)
                pos = image.find(needle, pos + 1)
            for h in hits:
                rows.append((f"0x{base + h:08X}", name, len(code), member.split("\\")[-1],
                             len(hits)))
    rows.sort()
    with open(out, "w", newline="") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(["address", "symbol", "size", "object", "hits"])
        w.writerows(rows)
    print(f"{lib}: {objs} objects, {total} functions, {len(rows)} placements, "
          f"{len({r[0] for r in rows})} distinct exe addresses")


if __name__ == "__main__":
    main()
