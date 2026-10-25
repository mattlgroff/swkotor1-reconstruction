"""l1_bytegate.py <obj> <lst> <l1_layout.json> [--mutant] : relocation-normalized byte gate for the L1 transcription.

Proves the assembled kotor_vc7_math.obj equal to the original swkotor.exe bytes (read-only from the install):
  code  every transcribed instruction, in emission order, has the original length and bytes; the only differences
        allowed are (a) internal branch displacements, which must reach the transcription of the ORIGINAL target,
        (b) relocated fields, whose relocation must name the transcription of the ORIGINAL address (data label
        D_<addr> / named flag, code label, or the ruled external: _errno, @__security_check_cookie@4,
        kotor_vc7_has_osfxsr_set, __imp__RaiseException@16, ___security_cookie);
  data  every data block equals the image bytes (.bss blocks: zero), pointer slots relocate to the ORIGINAL target;
  size  the .text section holds exactly the transcribed instructions and nothing else.
Exit 0 = PASS. A mutant object (--mutant) is expected to FAIL; the exit code is inverted in that mode."""
import json, re, struct, sys
from pathlib import Path
import capstone, pefile

import hashlib
EXE = sys.argv.pop(1)
if hashlib.sha256(Path(EXE).read_bytes()).hexdigest() != "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435":
    raise SystemExit("Original executable hash mismatch")


def coff(path):
    b = open(path, 'rb').read()
    mach, nsec, ts, symptr, nsym, opt, ch = struct.unpack_from('<HHIIIHH', b, 0)
    assert mach == 0x14C, 'not an i386 COFF object'
    strtab = symptr + nsym * 18
    def name8(raw):
        if raw[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', raw, 4)[0]; e = b.index(b'\0', strtab + off); return b[strtab + off:e].decode()
        return raw.rstrip(b'\0').decode()
    secs = []
    for i in range(nsec):
        o = 20 + opt + i * 40
        raw = b[o:o + 8]; nm = raw.rstrip(b'\0').decode()
        if nm.startswith('/'): off = int(nm[1:]); e = b.index(b'\0', strtab + off); nm = b[strtab + off:e].decode()
        vsz, va, rsz, rptr, relptr, lnptr, nrel, nln, chars = struct.unpack_from('<IIIIIIHHI', b, o + 8)
        rels = [struct.unpack_from('<IIH', b, relptr + k * 10) for k in range(nrel)]
        secs.append(dict(name=nm, data=b[rptr:rptr + rsz] if rptr else b'\0' * rsz, rels=rels, chars=chars))
    syms = {}
    i = 0
    while i < nsym:
        o = symptr + i * 18
        nm = name8(b[o:o + 8]); val, secn, typ, cls, aux = struct.unpack_from('<IhHBB', b, o + 8)
        syms[i] = dict(name=nm, value=val, sec=secn, cls=cls)
        i += 1 + aux
    return secs, syms


def lst_symbols(path):
    out = {}
    for l in open(path, errors='replace'):
        m = re.match(r'^(\S+)\s+[. ]*\.\s+(?:Byte|L Near|DWord|Number|Text)\s+([0-9A-F]{8})\s+(\w+)', l.rstrip())
        if m: out[m.group(1)] = (m.group(3), int(m.group(2), 16))
    return out


def main():
    obj, lst, lay = sys.argv[1:4]; mutant = '--mutant' in sys.argv
    L = json.load(open(lay))
    pe = pefile.PE(EXE, fast_load=True); BASE = pe.OPTIONAL_HEADER.ImageBase; IMG = pe.get_memory_mapped_image()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    secs, syms = coff(obj)
    S = lst_symbols(lst)
    segsec = {'_TEXT': '.text$mn', 'KV_RDATA': '.rdata', 'KV_DATA': '.data'}
    sec_by_name = {}
    for i, s in enumerate(secs):   # MASM also emits an empty default .data; keep the non-empty section of each name
        if s['name'] not in sec_by_name or len(s['data']) > len(sec_by_name[s['name']][1]['data']): sec_by_name[s['name']] = (i + 1, s)
    fails = []
    # label -> original address
    entries = {v: int(k, 16) for k, v in L['entries'].items()}
    dnames = {v: int(k, 16) for k, v in L['data_names'].items()}
    def label_orig(name):
        if re.fullmatch(r'D_[0-9A-F]{8}', name): return int(name[2:], 16)
        if re.fullmatch(r'L[0-9A-F]{8}', name): return int(name[1:], 16)
        if name in entries: return entries[name]
        if name in dnames: return dnames[name]
        return None
    # (section name, offset) -> original address, from the listing
    loc2orig = {}
    for n, (seg, off) in S.items():
        a = label_orig(n)
        if a is not None and seg in segsec: loc2orig[(segsec[seg], off)] = a
    ext = {v: int(k, 16) for k, v in L['ext_mem'].items()}
    stops = {int(k, 16): v for k, v in L['stops'].items()}
    # ---------------------------------------------------------------- code
    tsec_i, tsec = sec_by_name['.text$mn']
    text = tsec['data']
    relmap = {}
    for (va, si, typ) in tsec['rels']: relmap[va] = (syms[si], typ)
    ins_list = L['instructions']
    off = 0; off_of = {}
    for a in ins_list:
        ins = next(md.disasm(IMG[a - BASE:a - BASE + 16], a)); off_of[a] = off; off += ins.size
    if off != len(text): fails.append('size: .text is %d bytes, transcription %d' % (len(text), off))
    orig_of_off = {o: a for a, o in off_of.items()}
    def reloc_target(o, size_check=4):
        """(kind, value) of the relocation at text/data offset o: ('ext', name) or ('orig', address)."""
        return None
    def resolve(sym, addend, secname):
        if sym['sec'] > 0 and sym['cls'] == 3:      # static: section symbol (+ addend) or local label
            sname = secs[sym['sec'] - 1]['name']; o = sym['value'] + addend
            if sname == '.text$mn': return ('orig', orig_of_off.get(o))
            return ('orig', loc2orig.get((sname, o)))
        if sym['sec'] > 0:                           # public symbol defined here
            sname = secs[sym['sec'] - 1]['name']; o = sym['value'] + addend
            if sname == '.text$mn': return ('orig', orig_of_off.get(o))
            return ('orig', loc2orig.get((sname, o)))
        return ('ext', sym['name'])
    nins = nrel = nbr = 0
    for a in ins_list:
        o = off_of[a]
        oi = next(md.disasm(IMG[a - BASE:a - BASE + 16], a))
        n = oi.size; mine = text[o:o + n]; orig = bytes(oi.bytes)
        mi = next(md.disasm(mine, o), None)
        if mi is None or mi.size != n: fails.append('%08X: length/decoding differs (%s)' % (a, mine.hex())); continue
        mask = bytearray(n)
        if any(ro in relmap and ro + 4 > o + n for ro in range(o, o + n)):
            fails.append('%08X: relocation straddles the instruction end (instruction lengths drifted)' % a); break
        for ro in range(o, o + n):
            if ro in relmap:
                sym, typ = relmap[ro]; nrel += 1
                for k in range(4): mask[ro - o + k] = 1
                field = struct.unpack_from('<I', mine, ro - o)[0]
                ofield = struct.unpack_from('<I', orig, ro - o)[0]
                if typ == 0x14:   # REL32 (call/jmp to an external or another section)
                    kind, v = resolve(sym, 0, '.text')
                    otarget = (a + n + ofield) & 0xFFFFFFFF
                    want = stops.get(otarget)
                    if kind == 'ext' and want == v: pass
                    elif kind == 'orig' and v == otarget: pass
                    else: fails.append('%08X: REL32 to %s %s, original target %08X' % (a, kind, v, otarget))
                elif typ == 0x06:  # DIR32
                    kind, v = resolve(sym, field, '.text')
                    if kind == 'ext':
                        if ext.get(v) != ofield: fails.append('%08X: DIR32 external %s, original operand %08X' % (a, v, ofield))
                    elif v != ofield: fails.append('%08X: DIR32 -> %s, original operand %08X' % (a, ('%08X' % v) if v is not None else 'unmapped', ofield))
                else: fails.append('%08X: unexpected relocation type %#x' % (a, typ))
        is_br = (oi.mnemonic.startswith('j') or oi.mnemonic == 'call') and oi.operands and oi.operands[0].type == capstone.x86.X86_OP_IMM
        if is_br and not any(mask):
            nbr += 1
            dsz = 1 if n == 2 else 4
            if mine[:n - dsz] != orig[:n - dsz]: fails.append('%08X: branch opcode differs %s vs %s' % (a, mine.hex(), orig.hex()))
            mt = mi.operands[0].imm; ot = oi.operands[0].imm
            if orig_of_off.get(mt) != ot: fails.append('%08X: branch reaches %s, original target %08X' % (a, orig_of_off.get(mt), ot))
        else:
            for k in range(n):
                if not mask[k] and mine[k] != orig[k]:
                    fails.append('%08X: byte %d differs: %s vs original %s' % (a, k, mine.hex(), orig.hex())); break
        nins += 1
    # ---------------------------------------------------------------- data blocks
    ndata = 0
    for s, e, seg in L['blocks']:
        s = int(s, 16); e = int(e, 16)
        sname = '.data' if seg in 'DB' else '.rdata'
        hits = [o for (sn, o), v in loc2orig.items() if sn == sname and v == s]
        if not hits: fails.append('data block %08X not found in %s' % (s, sname)); continue
        o = hits[0]; i, sec = sec_by_name[sname]
        data = sec['data']; rels = {va: (syms[si], t) for (va, si, t) in sec['rels']}
        want = IMG[s - BASE:e - BASE] if s < 0x007A4000 else b'\0' * (e - s)
        k = 0
        while k < e - s:
            a = s + k
            if (o + k) in rels:
                sym, t = rels[o + k]; field = struct.unpack_from('<I', data, o + k)[0]
                kind, v = resolve(sym, field, sname)
                if v != struct.unpack_from('<I', want, k)[0]: fails.append('data %08X: pointer -> %s, original %08X' % (a, v, struct.unpack_from('<I', want, k)[0]))
                k += 4; continue
            if data[o + k] != want[k]: fails.append('data %08X differs: %02X vs %02X' % (a, data[o + k], want[k])); break
            k += 1; ndata += 1
    for f in fails[:40]: print('FAIL', f)
    ok = not fails
    print('L1 BYTEGATE %s: %d instructions (%d bytes), %d relocated fields, %d internal branches, %d data bytes; %d failures'
          % ('PASS' if ok else 'FAIL', nins, len(text), nrel, nbr, ndata, len(fails)))
    if mutant:
        print('MUTANT %s' % ('CAUGHT' if not ok else 'MISSED'))
        sys.exit(0 if not ok else 1)
    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()
