; kotor_math_x87.asm - L1 MATH-CRT intrinsic layer (PLAN-v3 1.3, 6.11; coordinator ruling 2: standard names).
;
; The original game was built with VC7 /Oi: sin, cos, tan, atan, atan2, log, log10, sqrt and fabs never call the CRT,
; they are the inline x87 instructions below, applied to the value on the x87 stack, at the current CW (027F: PC 53,
; RC nearest, all exceptions masked), with no argument reduction and no error handling. Observed inline shapes, one
; game site each (these site bytes are the oracles of tests/l1_test.cpp):
;   sin    fsin                          00423C9F      cos    fcos                       0045091A
;   tan    fptan ; fstp st(0)            0045C72B      atan   fld1 ; fpatan              0053F6F3
;   atan2  fld y ; fld x ; fpatan        0053F543      sqrt   fsqrt                      0041FF85
;   log    fldln2 ; (x) ; fyl2x          00522533      log10  fldlg2 ; (x) ; fyl2x       004EEB2B
;   fabs   fabs                          004507FF
; Two forms of each name:
;   C double   _sin(double) ...   : the qword argument loaded exactly, then the inline sequence; result in ST0.
;   long double ?sin@@YAOO@Z ...  : C++ overloads for units built with -mlong-double-80 (PLAN-v3 1.3). The argument is
;                                   the 80-bit register value, passed in a 16-byte slot ([esp+4], second at [esp+20]);
;                                   the result is returned unrounded in ST0, exactly as the inline sequence leaves it.
; acos / asin / pow long double overloads have _CI semantics (80-bit argument): they load the tbyte(s) and enter the
; transcribed _CIacos / _CIasin / _CIpow (kotor_vc7_math.asm), which store the qword copy for classification and
; compute on ST0, as the game's _CI call sites do. floor / ceil have no long double overload: the original passes them a
; qword (fstp qword ptr [esp] ; call 006FC750).
; Also here: kv_osfxsr_probe_instruction, the probe instruction of 0070439F, called from kotor_vc7_math_init.cpp.
.686P
.XMM
.MODEL FLAT
OPTION CASEMAP:NONE

PUBLIC _sin, _cos, _tan, _atan, _atan2, _log, _log10, _sqrt, _fabs
PUBLIC ?sin@@YAOO@Z, ?cos@@YAOO@Z, ?tan@@YAOO@Z, ?atan@@YAOO@Z, ?atan2@@YAOOO@Z, ?log@@YAOO@Z, ?log10@@YAOO@Z
PUBLIC ?sqrt@@YAOO@Z, ?fabs@@YAOO@Z, ?acos@@YAOO@Z, ?asin@@YAOO@Z, ?pow@@YAOOO@Z
PUBLIC ?kv_osfxsr_probe_instruction@@YAXXZ
EXTERN _kotor_vc7_CIacos:NEAR, _kotor_vc7_CIasin:NEAR, _kotor_vc7_CIpow:NEAR

.CODE

; ---------------------------------------------------------------- C double forms (cdecl, result in ST0)
_sin:                                   ; double sin(double)
    fld     QWORD PTR [esp+4]
    fsin                                ; 00423C9F
    ret
_cos:                                   ; double cos(double)
    fld     QWORD PTR [esp+4]
    fcos                                ; 0045091A
    ret
_tan:                                   ; double tan(double)
    fld     QWORD PTR [esp+4]
    fptan                               ; 0045C72B
    fstp    st(0)                       ; 0045C72D (|x| >= 2^63: fptan pushes nothing and this pops x, as the original)
    ret
_atan:                                  ; double atan(double)
    fld     QWORD PTR [esp+4]
    fld1                                ; 0053F6F3
    fpatan                              ; 0053F6F5
    ret
_atan2:                                 ; double atan2(double y, double x)
    fld     QWORD PTR [esp+4]           ; y
    fld     QWORD PTR [esp+12]          ; x
    fpatan                              ; 0053F543: atan(ST1 / ST0)
    ret
_log:                                   ; double log(double)
    fldln2                              ; 00522533
    fld     QWORD PTR [esp+4]
    fyl2x                               ; 00522537
    ret
_log10:                                 ; double log10(double)
    fldlg2                              ; 004EEB2B
    fld     QWORD PTR [esp+4]
    fyl2x                               ; 004EEB31
    ret
_sqrt:                                  ; double sqrt(double)
    fld     QWORD PTR [esp+4]
    fsqrt                               ; 0041FF85
    ret
_fabs:                                  ; double fabs(double): the original's inline D9 E1 (a1 review fix 1)
    fld     QWORD PTR [esp+4]
    fabs                                ; 004507FF
    ret

; ---------------------------------------------------------------- long double overloads (-mlong-double-80 units)
?sin@@YAOO@Z:                           ; long double sin(long double)
    fld     TBYTE PTR [esp+4]
    fsin
    ret
?cos@@YAOO@Z:                           ; long double cos(long double)
    fld     TBYTE PTR [esp+4]
    fcos
    ret
?tan@@YAOO@Z:                           ; long double tan(long double)
    fld     TBYTE PTR [esp+4]
    fptan
    fstp    st(0)
    ret
?atan@@YAOO@Z:                          ; long double atan(long double)
    fld     TBYTE PTR [esp+4]
    fld1
    fpatan
    ret
?atan2@@YAOOO@Z:                        ; long double atan2(long double y, long double x)
    fld     TBYTE PTR [esp+4]           ; y
    fld     TBYTE PTR [esp+20]          ; x
    fpatan
    ret
?log@@YAOO@Z:                           ; long double log(long double)
    fldln2
    fld     TBYTE PTR [esp+4]
    fyl2x
    ret
?log10@@YAOO@Z:                         ; long double log10(long double)
    fldlg2
    fld     TBYTE PTR [esp+4]
    fyl2x
    ret
?sqrt@@YAOO@Z:                          ; long double sqrt(long double): raw fsqrt on the tbyte (review item B)
    fld     TBYTE PTR [esp+4]
    fsqrt
    ret
?fabs@@YAOO@Z:                          ; long double fabs(long double)
    fld     TBYTE PTR [esp+4]
    fabs
    ret
?acos@@YAOO@Z:                          ; long double acos(long double): _CIacos (006FC0E0) on the 80-bit value
    fld     TBYTE PTR [esp+4]
    jmp     _kotor_vc7_CIacos
?asin@@YAOO@Z:                          ; long double asin(long double): _CIasin (006FCFF0)
    fld     TBYTE PTR [esp+4]
    jmp     _kotor_vc7_CIasin
?pow@@YAOOO@Z:                          ; long double pow(long double x, long double y): _CIpow (006FC1F0), ST1 = x, ST0 = y
    fld     TBYTE PTR [esp+4]           ; x
    fld     TBYTE PTR [esp+20]          ; y
    jmp     _kotor_vc7_CIpow

; ---------------------------------------------------------------- OS SSE-state probe instruction (0070439F)
?kv_osfxsr_probe_instruction@@YAXXZ:    ; void kv_osfxsr_probe_instruction(): faults if the OS does not save SSE state
    movapd  xmm0, xmm1                  ; 007043A0
    ret

END
