// kotor_vc7_math_init.cpp - L1 MATH-CRT: run-time initialisation of the transcribed VC7 math routines.
//
// 1. kotor_vc7_has_osfxsr_set (replaces 0070438C _has_osfxsr_set; PLAN-v2 8.4 proposed ruling).
//    The original probes whether the OS saves SSE state by executing `movapd xmm0, xmm1` (0070439F) inside a VC7
//    _SEH_prolog frame: result 0 (0070439C), set to 1 after the instruction (007043A4), 0 kept if the instruction faults
//    (filter 007043AD returns 1 = EXCEPTION_EXECUTE_HANDLER, handler 007043B1 resumes). The rebuild has no VC7 SEH
//    machinery, so only the probe mechanism is replaced: the same instruction inside __try/__except gives the same
//    observable result (1 when it executes, 0 when it faults). Like the original it clobbers XMM0.
// 2. The .CRT$XIC registration of kotor_vc7_sse2_available_init (the transcription of 007043C1, the fourth entry of the
//    original C initialiser table). .CRT$XIC runs among the C initialisers (__xi_a..__xi_z), before every C++ dynamic
//    initialiser (.CRT$XC*), so before any game static initialiser can call pow/floor/ceil (PLAN-v2 8.4 ordering gate).
//    The initialiser returns 0 (00704411 xor eax, eax), which the UCRT _initterm_e treats as success.
#include <excpt.h>

extern "C" int __cdecl kotor_vc7_sse2_available_init(void);   // kotor_vc7_math.asm (007043C1)
// The probe instruction itself lives in kotor_math_x87.asm (`movapd xmm0, xmm1 ; ret`). It is called, not inlined:
// clang treats inline asm as non-faulting and deletes a __try around it (observed: no SEH frame emitted), which would
// turn the 0 result of a faulting probe into a crash. A call to an opaque C++-linkage function keeps the SEH frame.
void __cdecl kv_osfxsr_probe_instruction();

extern "C" __declspec(noinline) int __cdecl kotor_vc7_has_osfxsr_set(void) {
    int result = 0;                                             // 0070439C and dword ptr [ebp-1Ch], 0
    __try {
        kv_osfxsr_probe_instruction();                          // 007043A0 movapd xmm0, xmm1
        result = 1;                                             // 007043A4 mov dword ptr [ebp-1Ch], 1
    } __except (EXCEPTION_EXECUTE_HANDLER) {                    // 007043AD filter: return 1
        result = 0;                                             // 007043B1 handler: [ebp-1Ch] still 0
    }
    return result;                                              // 007043B8 mov eax, [ebp-1Ch]
}

typedef int(__cdecl* kotor_vc7_pifv)(void);
#pragma section(".CRT$XIC", long, read)
extern "C" __declspec(allocate(".CRT$XIC")) const kotor_vc7_pifv kotor_vc7_xic_sse2_init = kotor_vc7_sse2_available_init;
#pragma comment(linker, "/include:_kotor_vc7_xic_sse2_init")
