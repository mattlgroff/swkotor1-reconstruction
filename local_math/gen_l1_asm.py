"""gen_l1_asm.py <outdir> : L1 MATH-CRT transcription generator (read-only on the install).

Reads swkotor.exe (GOG 1.03) and writes, in instruction order, the MASM (ml 14.44) transcription of the statically
linked VC7 CRT math routines that L1 owns, plus the data they read:
  <outdir>/kotor_vc7_math.asm        code (one line per original instruction, original address in the comment) and the
                                     small constants, name strings and flags (bit patterns as in the image);
  <outdir>/kotor_vc7_pow_tables.inc  the __pow_pentium4 lookup tables 0075A9C0..0075E2E8 (14,632 bytes of numeric data).
The code set is the recursive-descent closure of the L1 entry points, cut at the boundaries the coordinator ruled on
(errno -> UCRT _errno, the VC7 /GS check -> the toolchain's __security_check_cookie, the OS-SSE probe -> the C++
__try/__except replacement, RaiseException -> the import). Every instruction is translated from capstone's decoding;
tools/l1_bytegate.py proves the assembled object equal to the original bytes, relocation-normalized.
Nothing from the install is stored by this tool except into <outdir>."""
import json, re, struct, sys
from pathlib import Path
import capstone, pefile
if capstone.__version__ != "5.0.7" or pefile.__version__ != "2024.8.26":
    raise SystemExit("Requires capstone 5.0.7 and pefile 2024.8.26")
from capstone import x86

import hashlib
EXE = sys.argv.pop(1)
if hashlib.sha256(Path(EXE).read_bytes()).hexdigest() != "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435":
    raise SystemExit("Original executable hash mismatch")
pe = pefile.PE(EXE, fast_load=True)
BASE = pe.OPTIONAL_HEADER.ImageBase
IMG = pe.get_memory_mapped_image()
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
md.detail = True

# ---------------------------------------------------------------- names (original address -> transcription label)
# public: game-visible C names (ruling: standard names), project names for _finite / __CI* / internal state.
ENTRY = {
    0x006FC0E0: ('_kotor_vc7_CIacos', True, '_CIacos: ST0 argument (80-bit), qword copy for classification'),
    0x006FC0F4: ('_acos', True, 'C acos(double): __fload_withFB, then falls into the shared body'),
    0x006FC0FD: ('kv_acos_body', False, 'shared acos body (expects ST0, EAX and flags set up)'),
    0x006FC1B0: ('_pow', True, 'C pow(double, double): SSE2 dispatch on [00834314]'),
    0x006FC1F0: ('_kotor_vc7_CIpow', True, '_CIpow: ST1 = x, ST0 = y; SSE2 dispatch on [00834314]'),
    0x006FC24D: ('kv_pow_internal', False, '_pow_internal (x87)'),
    0x006FC412: ('kv_test_whether_TOS_is_int', False, '_test_whether_TOS_is_int'),
    0x006FC750: ('_floor', True, 'C floor(double): SSE2 path inline, x87 path 00705672'),
    0x006FC880: ('_ceil', True, 'C ceil(double): SSE2 path inline, x87 path 00705743'),
    0x006FCFF0: ('_kotor_vc7_CIasin', True, '_CIasin: ST0 argument (80-bit)'),
    0x006FD004: ('_asin', True, 'C asin(double)'),
    0x006FD00D: ('kv_asin_body', False, 'shared asin body'),
    0x006FE517: ('_kotor_vc7_finite', True, '_finite(double) (project name, ruling 2)'),
    0x006FE52C: ('kv_fpclass', False, '_fpclass'),
    0x007041A0: ('kv_startTwoArgErrorHandling', False, '_startTwoArgErrorHandling'),
    0x007041B7: ('kv_startOneArgErrorHandling', False, '_startOneArgErrorHandling'),
    0x00704200: ('kv_twoToTOS', False, '_twoToTOS: 2^ST0'),
    0x00704215: ('kv_load_CW', False, '_load_CW: CW = (caller CW & 0300h) | 007Fh: keeps the precision control, RC nearest, all exceptions masked'),
    0x0070422C: ('kv_convertTOStoQNaN', False, '_convertTOStoQNaN'),
    0x00704245: ('kv_fload_withFB', False, '_fload_withFB: fld qword [edx] (NaN/inf through a tbyte)'),
    0x00704288: ('kv_checkTOS_withFB', False, '_checkTOS_withFB'),
    0x0070429E: ('kv_fast_exit', False, '_fast_exit: restore CW, pop, ret'),
    0x007042AB: ('kv_math_exit', False, '_math_exit: precision-exception check'),
    0x007042E9: ('kv_check_range_exit', False, '_check_range_exit'),
    0x007043C1: ('_kotor_vc7_sse2_available_init', True, '__sse2_available_init (registered in .CRT$XIC)'),
    0x00704420: ('kv_CIpow_pentium4', False, '_CIpow SSE2 entry'),
    0x00704439: ('kv_pow_pentium4', False, '__pow_pentium4'),
    0x00705177: ('kv_pow_y_is_int', False, 'pow helper: classify y (0 non-integer, 1 odd, 2 even)'),
    0x007051E1: ('kv_powhlp', False, '_powhlp'),
    0x007053DE: ('kv_sse2_math_error', False, 'SSE2 path error handler (VC7 libm error support)'),
    0x00705672: ('kv_floor_x87', False, 'floor x87 path'),
    0x00705743: ('kv_ceil_x87', False, 'ceil x87 path'),
    0x00706A22: ('kv_matherr', False, 'default _matherr: returns 0 (ruling: reconstruction of the original default)'),
    0x00707600: ('kv_set_exp', False, '_set_exp'),
    0x0070762A: ('kv_sptype', False, '_sptype'),
    0x00707685: ('kv_decomp', False, '_decomp'),
    0x0070773F: ('kv_raise_exc', False, '_raise_exc (RaiseException for unmasked exceptions)'),
    0x007079E4: ('kv_handle_exc', False, '_handle_exc'),
    0x00707C06: ('kv_set_errno_from_matherr', False, '_set_errno_from_matherr'),
    0x00707C2E: ('kv_errcode', False, '_errcode'),
    0x00707C5B: ('kv_umatherr', False, '_umatherr'),
    0x00707CF9: ('kv_handle_qnan1', False, '_handle_qnan1'),
    0x00707D4C: ('kv_except1', False, '_except1'),
    0x00707E0A: ('kv_statfp', False, '_statfp'),
    0x00707E17: ('kv_clrfp', False, '_clrfp'),
    0x00707E25: ('kv_ctrlfp', False, '_ctrlfp'),
    0x00707E49: ('kv_set_statfp', False, '_set_statfp'),
    0x007096B2: ('kv_87except', False, '_87except'),
    0x0070A53B: ('kv_frnd', False, '_frnd'),
}
ROOTS = [0x006FC0E0, 0x006FC0F4, 0x006FCFF0, 0x006FD004, 0x006FC1B0, 0x006FC1F0, 0x006FC750, 0x006FC880, 0x006FE517, 0x007043C1]
# Boundary rebinding (coordinator rulings): call target -> external symbol.
STOP_CALL = {
    0x006FFE78: '__errno',                           # VC7 per-thread errno -> UCRT _errno() (no VC7 TLS)
    0x00705D37: '@__security_check_cookie@4',     # VC7 /GS check -> toolchain check (fastcall ECX)
    0x0070438C: '_kotor_vc7_has_osfxsr_set',       # OS SSE probe (VC7 SEH frame) -> C++ __try/__except, same result
}
EXT_MEM = {0x0073D148: '__imp__RaiseException@16', 0x007A3180: '___security_cookie'}

# ---------------------------------------------------------------- data blocks (start, end, segment, comment)
BLOCKS = [
    (0x0073EDE8, 0x0073EDF0, 'R', 'double 1.0'),
    (0x0073EDF8, 0x0073EE00, 'R', 'double 0.0'),
    (0x00741DF8, 0x00741E00, 'R', 'double 2.0'),
    (0x0074DC58, 0x0074DC5C, 'R', '"log"'),
    (0x0075A0B0, 0x0075A148, 'R', 'floor/ceil SSE2 constants (16-byte aligned operands)'),
    (0x0075A960, 0x0075A9A4, 'R', 'x87 range-check constants'),
    (0x0075A9C0, 0x0075E2E8, 'T', '__pow_pentium4 tables'),
    (0x0075E358, 0x0075E3A0, 'R', 'SSE2 error-handler function names'),
    (0x0075F764, 0x0075F7F8, 'R', '_umatherr opcode-table names'),
    (0x007A2770, 0x007A2790, 'D', '"acos", double 0.5, "pow"'),
    (0x007A27B0, 0x007A27B8, 'D', '"asin"'),
    (0x007A30E0, 0x007A30F4, 'D', 'tbyte real indefinite, tbyte pi/2'),
    (0x007A3110, 0x007A311A, 'D', 'tbyte +inf'),
    (0x007A316C, 0x007A3178, 'D', '_matherr pointer, floor CW 173F, ceil CW 1B3F'),
    (0x007A32B0, 0x007A33D6, 'D', 'doubles, _umatherr opcode/name table, _set_statfp tbytes'),
    (0x007A36D0, 0x007A36D4, 'D', '_matherr_flag = 9876 (link-time constant; no writer in the image)'),
    (0x00833BDC, 0x00833BE0, 'B', 'flag 00833BDC: constant 0 (no writer; reads kept, PLAN-v3 2.1)'),
    (0x00834314, 0x00834318, 'B', '__use_sse2_mathfcns'),
    (0x00834318, 0x0083431C, 'B', '__sse2_available'),
]
DATA_NAME = {0x00834314: ('_kotor_vc7_use_sse2_mathfcns', True), 0x00834318: ('_kotor_vc7_sse2_available', True),
             0x00833BDC: ('kv_flag_833BDC', False), 0x007A36D0: ('kv_matherr_flag', False), 0x007A316C: ('kv_pmatherr', False)}
# dwords inside data blocks that are pointers in the image (relocated as OFFSET <label>)
PTR_SLOTS = [0x007A316C] + [0x007A32DC + 8 * i for i in range(29)]


def u32(a): return struct.unpack_from('<I', IMG, a - BASE)[0]


def dis1(va): return next(md.disasm(IMG[va - BASE:va - BASE + 16], va))


def closure():
    seen = {}; work = list(ROOTS)
    while work:
        va = work.pop()
        while va not in seen:
            ins = dis1(va); seen[va] = ins
            m = ins.mnemonic
            if m == 'call':
                op = ins.operands[0]
                if op.type == x86.X86_OP_IMM and op.imm not in STOP_CALL: work.append(op.imm)
            elif m == 'jmp':
                op = ins.operands[0]
                if op.type == x86.X86_OP_IMM: work.append(op.imm)
                break
            elif m.startswith('j') or m in ('loop', 'jecxz'):
                work.append(ins.operands[0].imm)
            if m in ('ret', 'int3', 'hlt'): break
            va += ins.size
    return seen


def hexs(v):
    return '0%Xh' % v


def block_of(a):
    for s, e, seg, c in BLOCKS:
        if s <= a < e: return s
    return None


def data_label(a):
    if a in DATA_NAME: return DATA_NAME[a][0]
    return 'D_%08X' % a


def main():
    out = Path(sys.argv[1]).resolve()
    source = Path(__file__).resolve().parents[1]
    installation = Path(EXE).resolve().parent
    if any(out.is_relative_to(p) or p.is_relative_to(out) for p in (source, installation)):
        raise SystemExit("Generated output must be separate from source and installation")
    out.mkdir(parents=True, exist_ok=False)
    (out / '.gitignore').write_text('*\n')
    seen = closure()
    addrs = sorted(seen)
    targets = set(ENTRY)
    refs = set()          # data addresses referenced
    for a in addrs:
        ins = seen[a]
        if ins.mnemonic.startswith('j') or ins.mnemonic == 'call':
            op = ins.operands[0]
            if op.type == x86.X86_OP_IMM: targets.add(op.imm)
        for op in ins.operands:
            if op.type == x86.X86_OP_MEM and op.mem.disp >= 0x400000 and op.mem.disp not in EXT_MEM: refs.add(op.mem.disp)
            if op.type == x86.X86_OP_IMM and 0x0073D000 <= op.imm < 0x00840000 and ins.mnemonic != 'call' and not ins.mnemonic.startswith('j'):
                refs.add(op.imm)
    for a in PTR_SLOTS:
        if u32(a) not in ENTRY: refs.add(u32(a))
    for r in sorted(refs):
        if block_of(r) is None: raise SystemExit('data reference %08X outside every block' % r)

    def lab(t):
        if t in STOP_CALL: return STOP_CALL[t]
        if t in ENTRY: return ENTRY[t][0]
        if t not in seen: raise SystemExit('branch target %08X not transcribed' % t)
        return 'L%08X' % t

    def sym(v):
        if v in EXT_MEM: return EXT_MEM[v]
        return data_label(v)

    def tr(ins):
        m = ins.mnemonic; ops = ins.op_str
        if m == 'wait': return 'fwait', ''
        # [esp+0] written with an explicit zero disp8 (the VC7 CRT's own inline asm): MASM always drops a zero
        # displacement, so these are emitted as their exact bytes; the comment carries the instruction.
        if any(op.type == x86.X86_OP_MEM and op.mem.disp == 0 for op in ins.operands) and ins.disp_size == 1:
            return 'db', '%s ; %s %s (zero disp8 kept)' % (', '.join(hexs(x) for x in ins.bytes), m, ops)
        if m == 'call' or m.startswith('j') or m in ('loop', 'jecxz'):
            op = ins.operands[0]
            if op.type == x86.X86_OP_IMM:
                if m == 'call': return 'call', lab(op.imm)
                return m, ('SHORT ' if ins.size == 2 else 'NEAR PTR ') + lab(op.imm)
        # x87 register forms: spell both operands the way MASM encodes them (D8: st, st(i); DC/DE: st(i), st)
        b0 = ins.bytes[0]
        if m in ('fadd', 'fsub', 'fsubr', 'fmul', 'fdiv', 'fdivr', 'faddp', 'fsubp', 'fsubrp', 'fmulp', 'fdivp', 'fdivrp') \
                and ins.bytes[1] >= 0xC0 and b0 in (0xD8, 0xDC, 0xDE):
            regs = re.findall(r'st\((\d)\)', ops)
            i = [r for r in regs if r != '0'] or ['0']
            return m, ('st, st(%s)' % i[0]) if b0 == 0xD8 else ('st(%s), st' % i[0])
        # TEST r/m8|r/m32, reg: MASM puts the FIRST register operand in ModRM.reg, the original has it in ModRM.rm;
        # TEST is commutative, so the operands are written in reverse to keep the original encoding (e.g. 84 CD).
        if m == 'test' and len(ins.operands) == 2 and all(op.type == x86.X86_OP_REG for op in ins.operands):
            a_, b_ = [x.strip() for x in ops.split(',')]
            return m, '%s, %s' % (b_, a_)
        if m in ('fnsave', 'frstor', 'fnstenv', 'fldenv'):
            ops = re.sub(r'^\w+ ptr ', '', ops)
        ops = ops.replace('xword ptr', 'tbyte ptr')
        # memory displacements and address immediates -> symbols
        mem_disp = {op.mem.disp for op in ins.operands if op.type == x86.X86_OP_MEM}
        def rep(mo):
            v = int(mo.group(0), 16)
            if v >= 0x400000 and (v in mem_disp or v in EXT_MEM):
                return sym(v)
            if 0x0073D000 <= v < 0x00840000:
                return 'OFFSET ' + sym(v)
            return hexs(v)
        ops = re.sub(r'0x[0-9a-fA-F]+', rep, ops)
        ops = re.sub(r'(?<![\w$@?])(\d+)(?![\w])', lambda mo: mo.group(1), ops)
        return m, ops

    # ---------------------------------------------------------------- code
    runs = []
    for a in addrs:
        if runs and runs[-1][-1] + seen[runs[-1][-1]].size == a: runs[-1].append(a)
        else: runs.append([a])
    for r in runs:
        last = seen[r[-1]]
        if last.mnemonic not in ('ret', 'jmp'):
            raise SystemExit('run %08X..%08X ends in %s (fall-through would be lost)' % (r[0], r[-1], last.mnemonic))
    L = []
    w = L.append
    w('; kotor_vc7_math.asm - L1 MATH-CRT: transcription of the VC7 CRT math routines statically linked into')
    w(';   swkotor.exe (GOG 1.03): acos / asin (C entries and _CI entries, shared bodies), pow (C, _CI, x87 _pow_internal,')
    w(';   SSE2 __pow_pentium4), floor / ceil (SSE2 and x87 paths), _finite, __sse2_available_init, and the x87 error')
    w(';   machinery they reach (_startOneArgErrorHandling .. _87except, _except1, _handle_exc, _raise_exc).')
    w('; Generated by tools/gen_l1_asm.py from the install, one line per original instruction in original order;')
    w(';   the comment is the original address (no original bytes are written into this file except the constants and the')
    w(';   9 instructions ML cannot encode with their zero disp8). tools/l1_bytegate.py proves the assembled bytes equal')
    w(';   to the original, relocation-normalized, reading the install at run time.')
    w(';   Rebound boundaries (coordinator rulings, PLAN-v2 8.4 / PLAN-v3 2.1):')
    w(';   006FFE78 _errno -> UCRT _errno (__errno); 00705D37 __security_check_cookie and [007A3180] -> toolchain /GS;')
    w(';   0070438C OS-SSE probe -> _kotor_vc7_has_osfxsr_set (C++ __try/__except, same result);')
    w(';   [0073D148] RaiseException -> __imp__RaiseException@16. Flags 00833BDC (0) and 007A36D0 (9876) keep their reads.')
    w('; Not exported here: __ftol2 (006FAE8C, Task25 provider). __CI* and _finite carry project names (ruling 2):')
    w(';   C symbols _kotor_vc7_CIacos/_CIasin/_CIpow (register ABI), _kotor_vc7_finite, _kotor_vc7_sse2_available_init,')
    w(';   data _kotor_vc7_use_sse2_mathfcns / _kotor_vc7_sse2_available (C names without the leading underscore).')
    w('.686P')
    w('.XMM')
    w('.MODEL FLAT')
    w('OPTION CASEMAP:NONE')
    w('')
    for a, (n, pub, c) in sorted(ENTRY.items()):
        if pub: w('PUBLIC %s' % n)
    for a, (n, pub) in sorted(DATA_NAME.items()):
        if pub: w('PUBLIC %s' % n)
    for n in sorted(set(STOP_CALL.values())):
        w('EXTERN %s:NEAR' % n)
    w('EXTERN __imp__RaiseException@16:DWORD')
    w('EXTERN ___security_cookie:DWORD')
    w('')
    # data
    def emit_block(s, e, comment, ptrs):
        w('; ---- %08X..%08X %s' % (s, e, comment))
        a = s
        while a < e:
            if a in refs or a == s:
                if a in DATA_NAME or a in refs or a == s: w('%s LABEL BYTE' % data_label(a))
            if a in ptrs:
                w('    dd OFFSET %s ; %08X -> %08X' % (lab(u32(a)) if u32(a) in ENTRY else data_label(u32(a)), a, u32(a)))
                a += 4; continue
            nxt = min([x for x in list(refs) + ptrs + [e] if a < x <= e] + [a + 16])
            nxt = min(nxt, (a // 16 + 1) * 16, e)
            bs = IMG[a - BASE:nxt - BASE] if a < 0x007A4000 else b'\0' * (nxt - a)
            w('    db %s ; %08X' % (', '.join(hexs(x) for x in bs), a))
            a = nxt
    w('KV_RDATA SEGMENT READONLY ALIGN(64) PUBLIC FLAT ALIAS(".rdata") \'CONST\'')
    for s, e, seg, c in BLOCKS:
        if seg == 'R':
            w('    ALIGN 16'); emit_block(s, e, c, [])
        if seg == 'T':
            w('    ALIGN 64'); w('    INCLUDE kotor_vc7_pow_tables.inc')
    w('KV_RDATA ENDS')
    w('')
    w('KV_DATA SEGMENT ALIGN(16) PUBLIC FLAT ALIAS(".data") \'DATA\'')
    for s, e, seg, c in BLOCKS:
        if seg in 'DB':
            w('    ALIGN 4'); emit_block(s, e, c, [p for p in PTR_SLOTS if s <= p < e])
    w('KV_DATA ENDS')
    w('')
    # tables include
    T = []
    for s, e, seg, c in BLOCKS:
        if seg == 'T':
            T.append('; kotor_vc7_pow_tables.inc - __pow_pentium4 lookup tables %08X..%08X (numeric data from the image,' % (s, e))
            T.append(';   generated by tools/gen_l1_asm.py; relative layout and 64-byte alignment as in the original).')
            a = s
            while a < e:
                if a in refs: T.append('%s LABEL BYTE' % data_label(a))
                nxt = min([x for x in refs if a < x < e] + [(a // 16 + 1) * 16, e])
                T.append('    db %s ; %08X' % (', '.join(hexs(x) for x in IMG[a - BASE:nxt - BASE]), a))
                a = nxt
    (out / 'kotor_vc7_pow_tables.inc').write_text('\n'.join(T) + '\n', newline='\r\n')
    w('.CODE')
    insmap = []
    for r in runs:
        w('')
        w('; ======== %08X..%08X' % (r[0], r[-1] + seen[r[-1]].size))
        for a in r:
            ins = seen[a]
            if a in ENTRY:
                w(''); w('; %08X %s' % (a, ENTRY[a][2]))
                w('%s:' % ENTRY[a][0])
            elif a in targets:
                w('L%08X:' % a)
            m, ops = tr(ins)
            w('    %-9s %-44s ; %08X' % (m, ops, a))
            insmap.append(a)
    w('')
    w('END')
    (out / 'kotor_vc7_math.asm').write_text('\n'.join(L) + '\n', newline='\r\n')
    json.dump({'instructions': insmap, 'runs': [[r[0], r[-1] + seen[r[-1]].size] for r in runs],
               'entries': {'%08X' % a: v[0] for a, v in ENTRY.items()}, 'stops': {'%08X' % a: v for a, v in STOP_CALL.items()},
               'ext_mem': {'%08X' % a: v for a, v in EXT_MEM.items()},
               'blocks': [['%08X' % s, '%08X' % e, seg] for s, e, seg, c in BLOCKS],
               'data_names': {'%08X' % a: v[0] for a, v in DATA_NAME.items()}, 'ptr_slots': ['%08X' % p for p in PTR_SLOTS]},
              open(out / 'l1_layout.json', 'w'), indent=1)
    print('instructions %d in %d runs; data refs %d; wrote %s' % (len(insmap), len(runs), len(refs), out))


if __name__ == '__main__':
    main()
