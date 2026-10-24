#pragma once
// Lane W4 shared native helpers. x86 only (32-bit object pointers are stored in DWORD fields of original objects).
// Memory policy: no allocation here; only extern "C" live_operator_new / live_free (original 006FA7E6 / 006FA390)
// may be used by W4 units. Unimplemented callees call kotor_unreachable_trap with the ORIGINAL address.
//
// x87 policy (see D140): the original sets PC = 53-bit at static init (006FAE75 staticInitFloatingPointSystem ->
// 0070179E -> _controlfp(0x10000 _PC_53, 0x30000 _MCW_PC)); RC = nearest. Every original FPU-stack intermediate is carried here as `double`; every original `fstp dword`
// is an explicit `(float)` store. For finite values in float/double range this is bit-identical to the x87
// PC53 sequence (double rounding at the same points). Comparisons reproduce the exact fnstsw/test/jcc
// predicate including the unordered (NaN) outcome; see the jcc_* helpers.
#include <cstdint>
#include <cstring>
static_assert(sizeof(void*) == 4, "W4 native units are x86 only");

namespace w4 {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
using U64 = std::uint64_t;
using I64 = std::int64_t;

inline U32 ad(const void* p) { return static_cast<U32>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* pt(U32 a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U32 lw(const void* o, U32 off) { U32 v; std::memcpy(&v, static_cast<const U8*>(o) + off, 4); return v; }
inline I32 li(const void* o, U32 off) { I32 v; std::memcpy(&v, static_cast<const U8*>(o) + off, 4); return v; }
inline U16 lh(const void* o, U32 off) { U16 v; std::memcpy(&v, static_cast<const U8*>(o) + off, 2); return v; }
inline U8 lb(const void* o, U32 off) { return static_cast<const U8*>(o)[off]; }
inline float lf(const void* o, U32 off) { float v; std::memcpy(&v, static_cast<const U8*>(o) + off, 4); return v; }
inline void* lp(const void* o, U32 off) { return pt(lw(o, off)); }
inline void sw(void* o, U32 off, U32 v) { std::memcpy(static_cast<U8*>(o) + off, &v, 4); }
inline void sb(void* o, U32 off, U8 v) { static_cast<U8*>(o)[off] = v; }
inline void sf(void* o, U32 off, float v) { std::memcpy(static_cast<U8*>(o) + off, &v, 4); }
inline float bits_f(U32 b) { float f; std::memcpy(&f, &b, 4); return f; }
inline U32 f_bits(float f) { U32 b; std::memcpy(&b, &f, 4); return b; }
inline void* vslot(const void* object, U32 slot) { return lp(lp(object, 0), slot); }

// x87 `fld a; fcomp b; fnstsw ax; test ah,MASK; jcc`. Status bits: a>b -> 0, a<b -> C0(01h), a==b -> C3(40h),
// unordered -> C3|C2|C0 (45h). Each helper returns TRUE WHEN THE JUMP IS TAKEN.
inline U8 x87_status(double a, double b) {
    if (a > b) return 0x00;
    if (a < b) return 0x01;
    if (a == b) return 0x40;
    return 0x45;
}
inline bool parity_even(U8 v) { v ^= v >> 4; v ^= v >> 2; v ^= v >> 1; return (v & 1) == 0; }
inline bool jcc_jne(double a, double b, U8 mask) { return (x87_status(a, b) & mask) != 0; }
inline bool jcc_je(double a, double b, U8 mask) { return (x87_status(a, b) & mask) == 0; }
inline bool jcc_jp(double a, double b, U8 mask) { return parity_even(static_cast<U8>(x87_status(a, b) & mask)); }
inline bool jcc_jnp(double a, double b, U8 mask) { return !jcc_jp(a, b, mask); }

// Original x87 transcendental instructions (static init 006FAE75 -> _controlfp(_PC_53, _MCW_PC), so results are
// 53-bit rounded on the FPU stack and storing them to `double` is exact). Executed as the same instruction;
// never substituted by CRT sin/cos/atan2/tan.
inline double x87_fsin(double a) { double r; __asm { fld a } __asm { fsin } __asm { fstp r } return r; }
inline double x87_fcos(double a) { double r; __asm { fld a } __asm { fcos } __asm { fstp r } return r; }
inline double x87_fsqrt(double a) { double r; __asm { fld a } __asm { fsqrt } __asm { fstp r } return r; }
// fpatan: ST1 = atan2(ST1, ST0), pop. Original operand order: fld y; fld x; fpatan.
inline double x87_fpatan(double y, double x) { double r; __asm { fld y } __asm { fld x } __asm { fpatan } __asm { fstp r } return r; }
// fptan: ST0 = tan(ST0), then pushes 1.0; the 1.0 is discarded here.
inline double x87_fptan(double a) { double r; __asm { fld a } __asm { fptan } __asm { fstp st(0) } __asm { fstp r } return r; }
}  // namespace w4

extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long original_address, const char* symbol);
extern "C" void* __cdecl live_operator_new(unsigned size);   // original 006FA7E6
extern "C" void __cdecl live_free(void* block);               // original 006FA390
