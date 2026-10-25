// kotor_math.h - L1 MATH-CRT: the math declarations a converted game unit uses instead of <math.h>/<cmath>.
// Tier l1-a2. Supersedes l1-a1 (adds the C double fabs(double) provider required by the a1 review, fix 1;
// a1's long double fabs is unchanged). Supersedes the prototype lanes\math-cpp\proto\kotor_math.h (PLAN-v3 1.3);
// same names and signatures, plus the project-named entries of ruling 2. Every name below is defined by the L1 objects
// only:
//   kotor_vc7_math.obj  _acos _asin _pow _floor _ceil (C entries), kotor_vc7_CIacos/_CIasin/_CIpow, kotor_vc7_finite
//   kotor_math_x87.obj  _sin _cos _tan _atan _atan2 _log _log10 _sqrt _fabs and every long double overload
// Standard names (coordinator ruling 2):
//   double sin(double) ...      the C names with VC7 semantics: sin..sqrt and fabs are the game's /Oi inline x87
//                               instructions (no argument reduction, no errno); acos/asin/pow/floor/ceil are the
//                               transcribed VC7 CRT C entries (006FC0F4, 006FD004, 006FC1B0, 006FC750, 006FC880),
//                               with the SSE2 dispatch. fabs is the inline D9 E1 of the original's 59 sites, the
//                               oracle row being 004507FF.
//   long double sin(long double) ...  C++ overloads, only in units built with -mlong-double-80 (ruling 1). They take and
//                               return the 80-bit x87 value; acos/asin/pow have _CI semantics (80-bit argument, as the
//                               game's 15 + 5 + 28 _CI call sites). floor/ceil have no long double overload on purpose:
//                               the original passes them a qword (fstp qword ptr [esp] ; call 006FC750).
//   An LD unit must not call the C double sqrt/fabs (ldgate R6): use the long double overloads on register values.
// Project names (ruling 2; L2 gate G5 forbids the CRT spellings):
//   kotor_vc7_finite(double)    006FE517 _finite (3 game call sites).
//   kotor_vc7_CIacos / kotor_vc7_CIasin / kotor_vc7_CIpow: the _CI register entries (argument(s) in ST0 / ST1:ST0,
//                               result in ST0, no stack arguments). Not declarable as C functions; for assembly / L6.
//   __ftol2 is NOT provided by L1: 006FAE8C stays with the Task25 provider (_unresolved_ftol2_006fae8c, ruling 3).
// <math.h>/<cmath> must not be included in a game math unit: their inline long double overloads call sinl ->
// sin(double) (the qword clamp). tools/ldgate.py fails any object that defines a long-double-signature symbol.
#pragma once
#if defined(_INC_MATH) || defined(_CMATH_)
#error "kotor_math.h: <math.h>/<cmath> already included in a game math unit"
#endif
extern "C" {
double __cdecl sin(double);
double __cdecl cos(double);
double __cdecl tan(double);
double __cdecl atan(double);
double __cdecl atan2(double, double);
double __cdecl log(double);
double __cdecl log10(double);
double __cdecl sqrt(double);
double __cdecl fabs(double);
double __cdecl acos(double);
double __cdecl asin(double);
double __cdecl pow(double, double);
double __cdecl floor(double);
double __cdecl ceil(double);
int __cdecl kotor_vc7_finite(double);
}
#if defined(__LDBL_MANT_DIG__) && __LDBL_MANT_DIG__ == 64
long double __cdecl sin(long double);
long double __cdecl cos(long double);
long double __cdecl tan(long double);
long double __cdecl atan(long double);
long double __cdecl atan2(long double, long double);
long double __cdecl log(long double);
long double __cdecl log10(long double);
long double __cdecl acos(long double);   // _CIacos semantics: computes on the 80-bit ST0 value
long double __cdecl asin(long double);   // _CIasin semantics
long double __cdecl pow(long double, long double);   // _CIpow semantics (stores both to qwords, as the original)
long double __cdecl sqrt(long double);   // raw fsqrt on the 80-bit register value, as the 73 inline sites
long double __cdecl fabs(long double);   // raw fabs on the 80-bit value
#endif
