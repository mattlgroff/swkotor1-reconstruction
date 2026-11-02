// particle_motion.cpp - lane-3d-emitter fork F6: Particle initialisation and the standard (non point-to-point)
// particle mover of swkotor.exe (GOG K1, image base 00400000):
//   004921D0 Particle::initialize        (Particle vtable 007432D8 slot +08, RET 8)
//   00492400 ChunkyParticle::initialize  (ChunkyParticle vtable 007432E4 slot +08, RET 8)
//   0048D870 Particle::randomPosition    (RET 8)
//   0048DE60 Particle::randomDirection   (RET 8; both stack arguments are never read)
//   0048D9E0 Particle::inheritVelocity   (RET 4)
//   00492450 PartEmitter::moveParticleStandard (RET 4; selected by MoveFunc 00494D40 when node flag bit0 is clear)
// Evidence: dumpbin /disasm of the pinned swkotor.exe; instruction addresses cited per behaviour.
// Float discipline (D30-FINAL): x87 stack values are double; float only at the original `fstp dword` points.
// Particle offsets used (observed): +04 emitter, +08 position, +18 speed, +1C direction, +2C orientation
// quaternion (w,x,y,z), +3C frame, +40/+44, +48 vector, +54 velocity, +60 vector, +6C stuck byte, +70, +74 chunk Gob.
#include "4ac9e30747-emitter.h"   // L3: the HELD copy next to this file (<cmath> -> kotor_math.h)

// LOCAL-DECLS merged into emitter.h (lane header).

namespace {
using namespace kotor_emitter;

// .rdata constants (values read from the pinned image)
constexpr U kHundred_00741838 = 0x42C80000;        // 100.0f
constexpr double kHundredth_00743310 = 0.01;       // qword
constexpr U kHundredth_0073dfec = 0x3C23D70A;      // 0.01f
constexpr U kHalf_0073e9ac = 0x3F000000;           // 0.5f
constexpr U kTenThousandth_00741870 = 0x38D1B717;  // 0.0001f
// lane x87-trig (2026-10-24): the former kHalfPi_007432f8 = 1.5707963267948966 (bits 3FF921FB54442D18) was not
// the image's qword. [007432F8] = 3FF921FB52713C02 (1.5707963259...). The former x87_fcos also rounded the
// 64-bit-significand fcos result to double before the 0048E01C fcompp. Both are replaced by x87trig_0048e01a.
constexpr double kHalfPi_007432f8 = 0x1.921fb52713c02p+0;   // qword [007432F8] = 3FF921FB52713C02 (L3: a plain double constant)
constexpr U kOne_0073d764 = 0x3F800000;            // 1.0f
constexpr U kMinusTwo_007432f0 = 0xC0000000;       // -2.0f
constexpr U kGravity_007432f4 = 0x411CF5C3;        // 9.81f
constexpr U kMinusOne_00740cbc = 0xBF800000;       // -1.0f
constexpr U kBounceStep_00743330 = 0x3D072B02;     // 0.033f
constexpr U kThreeQuarters_0073e31c = 0x3F400000;  // 0.75f
constexpr U kOffset_00743320 = 0x3CA3D70A;         // 0.02f
constexpr U kStickRate_00743324 = 0x41D00000;      // 26.0f
constexpr double kQuarterPi_00743328 = 0x1.921fb54442d18p-1;   // qword [00743328] = 3FE921FB54442D18
constexpr double kHalf_0073f3d0 = 0.5;             // qword

inline void copy_words(void* to, const void* from, U count) { std::memcpy(to, from, count * 4u); }   // raw DWORD moves
inline const kotor_scene3d::SceneVTable* scene_table(void* scene) {
    return *static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene);
}
inline void* active_scene() { return *kotor_scene3d::cells.active_scene_007b92a0; }
// rand()%2 > 0 as compiled: and eax,80000001h / jns / dec / or -2 / inc / test / jle
inline bool rand_odd() { return crt_rand_006fc447() % 2 > 0; }
// L3 t1 a2: the D30-FINAL helpers W/S/F/fl/ftol2 (emitter.h) are calls under the /Ob0 recipe and carry x87 values as
// double (8-byte spills, a qword-rounded _ftol2 argument). The four D30 functions below use these force-inlined
// writing-rule equivalents instead: L = the float on the x87 stack (long double), R = an original fstp dword point,
// Fk = an image constant by its bit pattern, ff = a float field, ftol_ld = __ftol2 006FAE8C (int64 chop, low dword).
__forceinline long double L(float v) { return v; }
__forceinline float R(long double v) { return static_cast<float>(v); }
__forceinline float Fk(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }
__forceinline float& ff(void* object, U offset) { return *reinterpret_cast<float*>(static_cast<unsigned char*>(object) + offset); }
__forceinline I ftol_ld(long double v) { return static_cast<I>(static_cast<long long>(v)); }
} // namespace

extern "C" {

// ================================================================ 0048D870 Particle::randomPosition(rx, ry), RET 8
void __fastcall particle_random_position_0048d870(void* self, void*, I range_x, I range_y) {
    I x = 0;                                                                   // 0048D8A9
    if (range_x > 0) {                                                         // 0048D877..0048D87E
        const I a = crt_rand_006fc447();                                       // 0048D880
        const I b = crt_rand_006fc447();                                       // 0048D887
        const I n = ftol_ld(static_cast<long double>(range_x) * L(Fk(kHalf_0073e9ac))); // 0048D88C..0048D899 fild/fmul/_ftol2
        x = (a * b) % n;                                                       // 0048D890 imul; 0048D8A3 idiv (n==0 traps as in the original)
    }
    I y = 0;                                                                   // 0048D8E0
    if (range_y > 0) {                                                         // 0048D8AB..0048D8B5
        const I a = crt_rand_006fc447();                                       // 0048D8B7
        const I b = crt_rand_006fc447();                                       // 0048D8BE
        const I n = ftol_ld(static_cast<long double>(range_y) * L(Fk(kHalf_0073e9ac))); // 0048D8C3..0048D8D0
        y = (a * b) % n;                                                       // 0048D8C7 / 0048D8DA
    }
    if (rand_odd()) x = -x;                                                    // 0048D8E6..0048D8FD
    if (rand_odd()) y = -y;                                                    // 0048D901..0048D918
    void* const emitter = field<void*>(self, 4);                               // 0048D91C
    const long double sy = static_cast<long double>(y) * L(Fk(kTenThousandth_00741870));  // 0048D91F..0048D927
    const long double ay = sy * L(ff(emitter, 0x134));                              // 0048D930 (stays on the stack)
    const float by = R(sy * L(ff(emitter, 0x138)));                            // 0048D938..0048D93E
    const float cy = R(sy * L(ff(emitter, 0x13C)));                            // 0048D944..0048D94A
    const long double sx = static_cast<long double>(x) * L(Fk(kTenThousandth_00741870));  // 0048D94E..0048D952
    const long double px = sx * L(ff(emitter, 0x128));                              // 0048D95A
    const float qx = R(sx * L(ff(emitter, 0x12C)));                            // 0048D962..0048D968
    const float rx = R(sx * L(ff(emitter, 0x130)));                            // 0048D96E..0048D974
    const long double wx = px + L(ff(emitter, 0x164));                              // 0048D978
    const float wy = R(L(qx) + L(ff(emitter, 0x168)));                         // 0048D97E..0048D988
    const float wz = R(L(rx) + L(ff(emitter, 0x16C)));                         // 0048D98C..0048D996
    ff(self, 0x08) = R(wx + ay);                                               // 0048D99A..0048D9A4
    ff(self, 0x0C) = R(L(wy) + L(by));                                         // 0048D9A8..0048D9C0
    ff(self, 0x10) = R(L(wz) + L(cy));                                         // 0048D9B4..0048D9CB
}

// ================================================================ 0048D9E0 Particle::inheritVelocity(dt), RET 4
void __fastcall particle_inherit_velocity_0048d9e0(void* self, void*, float dt) {
    void* const e = field<void*>(self, 4);                                     // 0048D9E3
    const long double q0 = ff(e, 0x1B4), q1 = ff(e, 0x1B8), q2 = ff(e, 0x1BC), q3 = ff(e, 0x1C0);
    const long double a = (q2 * q0 + q1 * q3) * 2.0;                                // 0048D9E6..0048DA00 (stack)
    const long double b = (q2 * q3 - q0 * q1) * 2.0;                                // 0048DA02..0048DA1C (stack)
    const float c = R(1.0 - (q2 * q2 + q1 * q1) * 2.0);                        // 0048DA1E..0048DA3C
    const float dx = R(L(ff(e, 0x140)) - a);                                   // 0048DA44..0048DA4C
    const float dy = R(L(ff(e, 0x144)) - b);                                   // 0048DA50..0048DA58
    const float dz = R(L(ff(e, 0x148)) - L(c));                                // 0048DA60..0048DA6A
    const float lx = R(L(ff(e, 0x164)) - L(ff(self, 0x08)));                   // 0048DA6E..0048DA77
    const float ly = R(L(ff(e, 0x168)) - L(ff(self, 0x0C)));                   // 0048DA7B..0048DA84
    const float lz = R(L(ff(e, 0x16C)) - L(ff(self, 0x10)));                   // 0048DA88..0048DA91
    long double k;
    if (L(dt) > 0.0) k = L(Fk(kOne_0073d764)) / L(dt);                          // 0048DA95..0048DAAC (jne on <= / unordered)
    else k = L(Fk(kHundredth_0073dfec));                                        // 0048DAB2
    // L3 t1 a2: fsqrt on the 80-bit register sum (kotor_math.h sqrt(long double)); the root stays on the stack (0048DAD5).
    const long double len = sqrt((L(lz) * L(lz) + L(ly) * L(ly)) + L(lx) * L(lx)); // 0048DAB8..0048DAD5
    const long double xv = L(dx) * len * k;                                         // 0048DAD7/0048DAF1 (stack)
    const float ty = R(L(dy) * len);                                           // 0048DADD..0048DAE3
    const float tz = R(L(dz) * len);                                           // 0048DAE7..0048DAED
    const float uy = R(L(ty) * k);                                             // 0048DAF5..0048DAFB
    const float uz = R(L(tz) * k);                                             // 0048DAFF..0048DB05
    const long double ex = L(ff(e, 0x164)) - L(ff(e, 0x170));                       // 0048DB09..0048DB0F
    const long double ey = L(ff(e, 0x168)) - L(ff(e, 0x174));                       // 0048DB15..0048DB1B
    const float ez = R(L(ff(e, 0x16C)) - L(ff(e, 0x178)));                     // 0048DB21..0048DB32
    const float ax = R(ex * k);                                                // 0048DB36..0048DB3A
    const float ay = R(ey * k);                                                // 0048DB3E..0048DB40
    const float az = R(L(ez) * k);                                             // 0048DB44..0048DB4A
    ff(self, 0x1C) = R(L(ax) + xv);                                            // 0048DB4E..0048DB5C
    ff(self, 0x20) = R(L(ay) + L(uy));                                         // 0048DB62..0048DB7A
    ff(self, 0x24) = R(L(az) + L(uz));                                         // 0048DB6E..0048DB85
    const long double x = ff(self, 0x1C), y = ff(self, 0x20), z = ff(self, 0x24);   // 0048DB88..0048DB8E
    const long double m = sqrt((x * x + y * y) + z * z);                       // 0048DB90..0048DBA0 fsqrt (stays on the stack)
    // L3 t1 a2: in the original's order: 0048DBA9 fcom dword [0073D700] on the unrounded root, THEN 0048DBAF fst dword
    // [ecx+18h], then 0048DBB2..0048DBB7 fnstsw ; test ah,44h ; jnp (taken only when m == 0, not when unordered).
    const bool zero = (m == 0.0);                                              // 0048DBA9
    ff(self, 0x18) = R(m);                                                     // 0048DBAF fst
    if (zero) {                                                                // 0048DBB2..0048DBB7
        field<U>(self, 0x1C) = 0; field<U>(self, 0x20) = 0; field<U>(self, 0x24) = 0; // 0048DBF1..0048DC1C
        return;
    }
    const long double inv = L(Fk(kOne_0073d764)) / m;                                // 0048DBB9 fdivr
    const float nx = R(inv * L(ff(self, 0x1C)));                               // 0048DBBF..0048DBC3
    const float ny = R(inv * L(ff(self, 0x20)));                               // 0048DBC7..0048DBCC
    const float nz = R(inv * L(ff(self, 0x24)));                               // 0048DBD4..0048DBE0
    ff(self, 0x1C) = nx; ff(self, 0x20) = ny; ff(self, 0x24) = nz;             // 0048DBDB..0048DBE8
}

// L3 t1 a2: the stick test's two acos sites (00492C10..00492C24 / 00492C40..00492C54) use the real L1 acos(long double)
// (_CIacos 006FC0E0 semantics: the 80-bit operand loaded from the float slot, the 80-bit result compared unrounded with
// fcomp qword [00743328]). The x87-trig a2 transport helpers x87trig_00492c10 / x87trig_00492c19 and the t11b replica
// crt_acos_x87_006fc0e0 they fed are no longer needed.
}  // extern "C"
extern "C" {
// L3 t1: direct field access and float constants for 0048DE60 (the recipe is /Ob0: fl()/F() are calls; the original
// uses memory operands). Constants are the image's bit patterns. (C++ linkage inside this file's extern "C" block.)
extern "C++" {
namespace {
__forceinline float& fr(void* object, U offset) { return *reinterpret_cast<float*>(static_cast<unsigned char*>(object) + offset); }
constexpr float kHalfF_0073e9ac = 0x1p-1f;              // 3F000000
constexpr float kMinusHundredF_0074186c = -0x1.9p+6f;   // C2C80000
constexpr float kHundredthF_0073dfec = 0x1.47ae14p-7f;  // 3C23D70A
constexpr float kHalfPiF_00741bb8 = 0x1.921fb6p+0f;     // 3FC90FDB
constexpr float kOneF_0073d764 = 1.0f;                  // 3F800000
constexpr float kZeroF_0073d700 = 0.0f;                 // 00000000
}
}
// ================================================================ 0048DE60 Particle::randomDirection(a, b), RET 8
// Neither stack argument is read (frame 5Ch + 4 saves; [esp+70h]/[esp+74h] never referenced).
// L3 MATH-TRIG-CPP t1 (2026-10-24): PLAN-v3 writing rule (LD unit; HELD with the unit's cl -> clang migration). The
// x87-trig a1 helper x87trig_0048e01a is replaced by C++: the fcos result stays 80-bit into the 0048E01C compare.
// Comparisons from the fnstsw masks: test 44h;jnp == (a == b); test 5;jp == !(a < b); test 41h;jne == !(a > b).
// The function returns nothing: the x87 stack is empty at both RETs (the ret80 census entry "RET at 00492341" is a
// linear-sweep false positive).
void __fastcall particle_random_direction_0048de60(void* self, void*, I, I) {
    void* const e = field<void*>(self, 4);                                     // 0048DE66
    // 0048DE69..0048DE7D: fld [e+A0h] ; fmul [0073E9AC] ; fstp dword [esp+10h] (reloaded at 0048DEBC).
    const long double half_spread = static_cast<float>(static_cast<long double>(fr(e, 0xA0)) * kHalfF_0073e9ac);
    void* const camera = *kotor_scene3d::cells.current_camera_007bb4e8;        // 0048DE6F
    Vector to_camera;                                                          // 0048DE85..0048DEAB fsub ; fstp dword x3
    to_camera.x = static_cast<float>(static_cast<long double>(fr(self, 0x08)) - fr(camera, 0x7C));
    to_camera.y = static_cast<float>(static_cast<long double>(fr(self, 0x0C)) - fr(camera, 0x80));
    to_camera.z = static_cast<float>(static_cast<long double>(fr(self, 0x10)) - fr(camera, 0x84));
    Vector n;
    normalize_004ab180(&n, &to_camera);                                        // 0048DEAF
    Quaternion* const q = &field<Quaternion>(self, 0x2C);                      // 0048DED9
    {
        const I r = crt_rand_006fc447();                                       // 0048DEB7
        // 0048DEBC..0048DEC8: fld [esp+10h] ; fmul [0074186C] ; call __ftol2 (int64 chop, low dword).
        const I m = static_cast<I>(static_cast<long long>(half_spread * kMinusHundredF_0074186c));
        const I rem = r % (1 - m);                                             // 0048DECD..0048DED7 idiv
        Vector axis; copy_words(&axis, at(e, 0x128), 3);                        // 0048DEEA..0048DF0E (emitter row 0)
        // 0048DEE6..0048DEFB: fild ; fmul [0073DFEC] ; fstp dword (the angle argument).
        const float angle = static_cast<float>(static_cast<long double>(rem) * kHundredthF_0073dfec);
        Quaternion spin, product;
        quaternion_construct_004ac960(&spin, nullptr, axis, angle);             // 0048DF11
        const Quaternion* const result = quaternion_multiply_004a9ac0(&product, &spin, q); // 0048DF16..0048DF1C
        copy_words(q, result, 4);                                              // 0048DF21..0048DF39
    }
    U held[4];                                                                 // ecx/edx/ebp/edi at 0048DFDA..0048DFE9
    {
        const I rem = crt_rand_006fc447() % 0x274;                             // 0048DF3C..0048DF47
        Vector axis; copy_words(&axis, at(e, 0x140), 3);                        // 0048DF57..0048DF7B (emitter row 2)
        const float angle = static_cast<float>(static_cast<long double>(rem) * kHundredthF_0073dfec); // 0048DF53..0048DF68
        Quaternion spin, product;
        quaternion_construct_004ac960(&spin, nullptr, axis, angle);             // 0048DF7E
        const Quaternion* const result = quaternion_multiply_004a9ac0(&product, &spin, q); // 0048DF83..0048DF89
        copy_words(q, result, 4);                                              // 0048DF8E..0048DFA7
        copy_words(held, result, 4);                                           // 0048DFDA..0048DFE9 (reloaded from the result)
    }
    if (fr(field<void*>(field<void*>(self, 4), 4), 0x50) == kZeroF_0073d700) return;     // 0048DFAA..0048DFC1 (test 44h ; jnp)
    Vector y_axis;
    const Vector* const y = quaternion_ydir_0044f420(q, nullptr, &y_axis);     // 0048DFC7..0048DFCE
    // 0048DFD3..0048DFF4: the dot product stays on the x87 stack.
    long double dot = (static_cast<long double>(n.y) * y->y + static_cast<long double>(n.z) * y->z)
                      + static_cast<long double>(n.x) * y->x;
    for (;;) {
        // 0048DFF6..0048E005: fcom 0 ; fld st(0) ; test 5 ; jp ; fchs  (the |dot| copy).
        const long double magnitude = (dot < kZeroF_0073d700) ? -dot : dot;
        void* const node = field<void*>(field<void*>(self, 4), 4);             // 0048E007..0048E010
        // 0048E00D..0048E023: fld [node+50h] ; fsubr qword [007432F8] ; fcos ; fcompp ; test 41h ; jne -> 0048E178.
        if (!(cos(static_cast<long double>(kHalfPi_007432f8) - static_cast<long double>(fr(node, 0x50))) > magnitude)) {
            copy_words(q, held, 4);                                            // 0048E178..0048E182
            return;
        }
        // 0048E029..0048E05A: angle -> fstp dword [esp+10h] (passed to the Quaternion ctor after the cross product).
        long double angle;
        if (!(dot < kZeroF_0073d700)) angle = static_cast<float>((dot - kHalfPiF_00741bb8) + fr(node, 0x50));   // 0048E04D..0048E05A
        else angle = static_cast<float>((static_cast<long double>(fr(node, 0x50)) + kHalfPiF_00741bb8) - dot);                 // 0048E036..0048E045
        // 0048E05E..0048E0AF: the quaternion's z axis from the memory quaternion (unchanged inside the loop).
        Vector z_axis;
        z_axis.x = static_cast<float>(((static_cast<long double>(q->x) * q->y - static_cast<long double>(q->z) * q->w)) * 2.0f);
        z_axis.y = static_cast<float>(kOneF_0073d764 - (static_cast<long double>(q->z) * q->z + static_cast<long double>(q->x) * q->x) * 2.0f);
        z_axis.z = static_cast<float>((static_cast<long double>(q->y) * q->z + static_cast<long double>(q->x) * q->w) * 2.0f);
        Vector cross;
        const Vector* const c = vector_cross_004ab210(&cross, &n, &z_axis);    // 0048E0B3
        Vector axis; copy_words(&axis, c, 3);                                  // 0048E0C1..0048E0D7
        Quaternion spin, product;
        quaternion_construct_004ac960(&spin, nullptr, axis, static_cast<float>(angle));   // 0048E0DA (push [esp+10h])
        const Quaternion* const r = quaternion_multiply_004a9ac0(&product, &spin, q); // 0048E0DF..0048E0E5
        copy_words(held, r, 4);                                                // 0048E0EA..0048E10D
        Quaternion p; copy_words(&p, held, 4);
        // 0048E0F9..0048E15E: the rotated quaternion's z axis dotted with n, on the x87 stack.
        const long double ax = (static_cast<long double>(p.x) * p.y - static_cast<long double>(p.z) * p.w) * 2.0f;
        const long double ay = kOneF_0073d764 - (static_cast<long double>(p.x) * p.x + static_cast<long double>(p.z) * p.z) * 2.0f;
        const long double az = (static_cast<long double>(p.x) * p.w + static_cast<long double>(p.y) * p.z) * 2.0f;
        long double d = (ax * n.x + ay * n.y) + az * n.z;
        if (d < kZeroF_0073d700) d = -d;                                                  // 0048E160..0048E171 fcom ; test 5 ; jp ; fchs
        dot = d;                                                               // back to 0048DFF6
    }
}

// ================================================================ 004921D0 Particle::initialize(dt, scale), RET 8
// EAX left = bits of the stored velocity y (0049239E..004923D4 last eax load).
U __fastcall particle_initialize_004921d0(void* self, void*, U dt_bits, U scale_bits) {
    void* const e = field<void*>(self, 4);                                     // 004921DA
    field<U8>(self, 0x6C) = 0;                                                 // 004921DD
    field<U>(self, 0x14) = 0; field<U>(self, 0x40) = 0;                        // 004921E0..004921E3
    field<U>(self, 0x70) = 0; field<U>(self, 0x44) = 0;                        // 004921E6..004921E9
    copy_words(at(self, 0x2C), at(e, 0x1A4), 4);                               // 004921ED..0049220B
    field<U>(self, 0x18) = 0; field<U>(self, 0x24) = 0;                        // 0049220E..00492211
    field<U>(self, 0x20) = 0; field<U>(self, 0x1C) = 0;                        // 00492214..00492217
    I random_speed = 0;                                                        // 0049221D
    field<I>(self, 0x3C) = ftol_ld(L(ff(e, 0x70)));                              // 0049221A..00492226
    const float spread = ff(e, 0x8C);                                          // 00492229..0049222F (fst dword)
    if (L(spread) > kHundredth_00743310) {                                     // 00492233..0049223E (fcomp qword; jne on <= / unordered)
        const I r = crt_rand_006fc447();                                       // 00492240
        const I n = ftol_ld(L(spread) * L(Fk(kHundred_00741838)));                // 00492245..00492251
        random_speed = r % n;                                                  // 0049225A..0049225D
        if (rand_odd()) random_speed = -random_speed;                          // 00492263..0049227A
    }
    void* const e2 = field<void*>(self, 4);                                    // 0049227E
    const long double scale = L(Fk(scale_bits));
    const I range_x = ftol_ld(scale * L(ff(e2, 0xAC)) * L(Fk(kHundred_00741838))); // 00492281..00492291
    const I range_y = ftol_ld(scale * L(ff(e2, 0xB0)) * L(Fk(kHundred_00741838))); // 00492296..004922AA
    if (range_x == 0 && range_y == 0) copy_words(at(self, 0x08), at(e2, 0x164), 3); // 004922B5..004922D8
    else particle_random_position_0048d870(self, nullptr, range_x, range_y);   // 004922DD..004922E1
    copy_words(at(self, 0x60), at(self, 0x08), 3);                             // 004922EA..004922FD
    void* const node = field<void*>(field<void*>(self, 4), 4);                 // 00492300..00492303
    if (((field<U>(node, 0x12C) >> 7) & 1u) != 0)                              // 00492306..00492311 (InheritingVelocity)
        particle_inherit_velocity_0048d9e0(self, nullptr, Fk(dt_bits));          // 00492313..0049231A
    if (!(ff(field<void*>(self, 4), 0xA0) == 0.0f))                            // 00492323..00492337 (jnp skips on equal)
        particle_random_direction_0048de60(self, nullptr, range_x, range_y);   // 00492339..00492341
    const long double q0 = ff(self, 0x2C), q1 = ff(self, 0x30), q2 = ff(self, 0x34), q3 = ff(self, 0x38);
    ff(self, 0x48) = R((q2 * q0 + q1 * q3) * 2.0);                             // 00492346..0049237E
    ff(self, 0x4C) = R((q3 * q2 - q1 * q0) * 2.0);                             // 00492365..00492388
    ff(self, 0x50) = R(1.0 - (q1 * q1 + q2 * q2) * 2.0);                       // 00492378..004923A4
    void* const e3 = field<void*>(self, 4);                                    // 004923A7 ([esi-50h])
    const long double speed = static_cast<long double>(random_speed) * L(Fk(kHundredth_0073dfec)) + L(ff(e3, 0xA8)); // 004923AC..004923B6
    const float vx = R(speed * L(ff(self, 0x48)));                             // 004923BC..004923C0
    const float vy = R(speed * L(ff(self, 0x4C)));                             // 004923C8..004923CD
    const float vz = R(speed * L(ff(self, 0x50)));                             // 004923D1..004923DD
    ff(self, 0x54) = vx; ff(self, 0x58) = vy; ff(self, 0x5C) = vz;             // 004923D8..004923E5
    return float_to_bits(vy);                                                  // EAX = [esp+18h] (004923D4)
}

// ================================================================ 00492400 ChunkyParticle::initialize(dt, scale), RET 8
// Base initialize always runs with scale 1.0f (00492405 push 3F800000h); the caller's second argument goes to
// the chunk Gob's SetObjectScale. EAX: base result when no chunk Gob, else PlayAnimation's EAX.
U __fastcall chunky_particle_initialize_00492400(void* self, void*, U dt_bits, U scale_bits) {
    const U result = particle_initialize_004921d0(self, nullptr, dt_bits, kOne_0073d764); // 00492400..0049240D
    if (field<void*>(self, 0x74) == nullptr) return result;                    // 00492412..00492417
    void* gob = field<void*>(self, 0x74);
    gob_table(gob)->attach_to_scene(as_gob(gob), nullptr,
        static_cast<aurora_model::Scene*>(active_scene()));                    // 00492419..00492421 (Gob vtbl+4C)
    gob = field<void*>(self, 0x74);                                            // 00492424
    gob_table(gob)->s0cc_gob_set_object_scale(as_gob(gob), nullptr, scale_bits, 1); // 00492427..00492430 (vtbl+CC)
    gob = field<void*>(self, 0x74);                                            // 00492436
    return gob_table(gob)->s018_gob_play_animation(as_gob(gob), nullptr,
        word_of("impact"), kOne_0073d764, 1, 0);                               // 0049243B..00492449 (743318 "impact", vtbl+18)
}

// ================================================================ 00492450 PartEmitter::moveParticleStandard(dt), RET 4
void __fastcall part_emitter_move_particle_standard_00492450(void* emitter, void*, float dt) {
    void* const node = field<void*>(emitter, 4);                               // 0049245B ([esp+28h])
    Vector base;  field<U>(&base, 0) = 0; field<U>(&base, 4) = 0; field<U>(&base, 8) = 0;   // 0049245E..0049246E
    Vector wind;  field<U>(&wind, 0) = 0; field<U>(&wind, 4) = 0; field<U>(&wind, 8) = 0;   // 0049247C..0049248C
    const bool bouncing = ((field<U>(node, 0x12C) >> 4) & 1u) != 0;           // 00492494..004924AD ([esp+1Bh])
    const U inheriting = (field<U>(node, 0x12C) >> 6) & 1u;                    // 004924A0..004924B9 ([esp+4Ch])
    // D2: [esp+10h] is only written on the bounce paths (mode 1 / mode 2) and read by the stick tests; for a
    // bounce mode other than 1/2 on the first bouncing particle the original reads an uninitialised stack slot
    // (00492C67). Value-initialised to 0.0f (D2 label, original address 00492C67).
    // L3 t1 a2: the metric is the float the original stores at [esp+10h] (00492AF1 / 00492BCA fstp dword, 004929CC
    // mov 0); it is held here as that rounded value in a long double (no second float store / reload of its own).
    long double bounce_metric = 0.0;
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                         // 004924B1..004924C1 / 00492DC3..00492DD4
        void* const p = as_object(reinterpret_cast<const volatile U*>(
            static_cast<std::uintptr_t>(field<U>(emitter, 0x1C8)))[i]);       // 004924D0..004924D6
        if (field<U8>(p, 0x6C) != 0) continue;                                 // 004924D9..004924DE
        Vector* const pos = &field<Vector>(p, 0x08);                           // 004924E6 (ebx)
        Vector old; copy_words(&old, pos, 3);                                  // 004924E9..004924FB ([esp+5Ch])
        if (inheriting != 0) {                                                 // 004924E4/004924FF
            Vector local;
            local.x = R(L(pos->x) - L(ff(emitter, 0x170)));                    // 00492505..0049251C
            local.y = R(L(pos->y) - L(ff(emitter, 0x174)));                    // 0049252A..0049253A
            local.z = R(L(pos->z) - L(ff(emitter, 0x178)));                    // 00492541..0049254A
            Quaternion inverse, relative; Vector rotated;
            quaternion_negate_w_004a9b50(&inverse, &field<Quaternion>(emitter, 0x1B4));   // 00492551
            quaternion_multiply_004a9ac0(&relative, &field<Quaternion>(emitter, 0x1A4), &inverse); // 00492556..00492560
            const Vector* r = quaternion_rotate_004a9bb0(&rotated, &relative, &local);   // 00492568..00492571
            copy_words(&base, r, 3);                                           // 00492576..00492589
            Vector* const velocity = &field<Vector>(p, 0x54);                  // 0049258D (ebp)
            Quaternion inverse2, relative2; Vector rotated2;
            quaternion_negate_w_004a9b50(&inverse2, &field<Quaternion>(emitter, 0x1B4)); // 00492590..004925A0
            quaternion_multiply_004a9ac0(&relative2, &field<Quaternion>(emitter, 0x1A4), &inverse2); // 004925A5..004925B5
            r = quaternion_rotate_004a9bb0(&rotated2, &relative2, velocity);   // 004925BD..004925C6
            copy_words(velocity, r, 3);                                        // 004925CB..004925E7
            vector_add_assign_004a9740(&base, nullptr, &field<Vector>(emitter, 0x164)); // 004925DC..004925EA
        } else {
            copy_words(&base, pos, 3);                                         // 004925F1..00492603
        }
        const U flags = field<U>(node, 0x12C);                                 // 00492607..0049260B
        if (((flags >> 8) & 1u) != 0 && ((flags >> 6) & 1u) == 0 && field<void*>(emitter, 0x40) != nullptr) { // 00492611..00492627
            void* const gob = field<void*>(emitter, 0x40);
            Vector delta;
            delta.x = R(L(ff(gob, 0x78)) - L(ff(emitter, 0x158)));             // 0049264A..00492653
            delta.y = R(L(ff(gob, 0x7C)) - L(ff(emitter, 0x15C)));             // 0049263D..0049265A
            delta.z = R(L(ff(gob, 0x80)) - L(ff(emitter, 0x160)));             // 00492629..00492661
            vector_add_assign_004a9740(&base, nullptr, &delta);                // 00492668
            vector_add_assign_004a9740(&field<Vector>(p, 0x60), nullptr, &delta); // 0049266D..00492678
        }
        if (((field<U>(node, 0x12C) >> 10) & 1u) != 0) {                       // 0049267D..00492689 (InheritingPart)
            Vector delta;
            delta.x = R(L(ff(emitter, 0x164)) - L(ff(emitter, 0x158)));        // 0049268B..004926A3
            delta.y = R(L(ff(emitter, 0x168)) - L(ff(emitter, 0x15C)));        // 004926AA..004926B6
            delta.z = R(L(ff(emitter, 0x16C)) - L(ff(emitter, 0x160)));        // 004926BD..004926C9
            vector_add_assign_004a9740(&base, nullptr, &delta);                // 004926D0
            vector_add_assign_004a9740(&field<Vector>(p, 0x60), nullptr, &delta); // 004926D5..004926E0
        }
        // integrate position (004926E5..004927B5)
        const long double t = L(dt);
        const Vector& v = field<Vector>(p, 0x54);                              // ebp = p+54
        const long double ax = t * L(v.x);                                          // 004926E5..004926EF
        const long double ay = t * L(v.y);                                          // 004926F4..004926FB
        const float az = R(t * L(v.z));                                        // 004926FE..00492708
        const float bx = R(ax + L(base.x));                                    // 00492711..00492715
        const long double by = ay + L(base.y);                                      // 0049271C
        const long double bz = L(az) + L(base.z);                                   // 00492720..00492727
        const long double s = ff(p, 0x18);                                          // 0049272B
        const long double sx = s * L(ff(p, 0x1C));                                  // 0049272E..00492730
        const float sy = R(s * L(ff(p, 0x20)));                                // 00492733..00492738
        const float sz = R(s * L(ff(p, 0x24)));                                // 00492741..00492744
        const long double dx = sx * t;                                              // 0049274B
        const long double dy = L(sy) * t;                                           // 00492752..00492759
        const float dz = R(L(sz) * t);                                         // 00492760..0049276E
        pos->x = R(dx + L(bx));                                                // 00492777..00492786
        pos->y = R(dy + by);                                                   // 00492788..0049279B
        pos->z = R(L(dz) + bz);                                                // 0049278E..004927AA
        if (((field<U>(node, 0x12C) >> 2) & 1u) != 0) {                        // 004927AF..004927BD (AffectedByWind)
            if (field<I>(emitter, 0x1CC) < 0x32 || i % 6 == 0) {               // 004927BF..004927D6
                void* const scene = active_scene();                            // 004927DF / 00492801
                Vector out;
                const U r = scene_table(scene)->s0d0_scene_get_wind_vector(scene, nullptr, word_of(&out),
                    field<U>(&base, 0), field<U>(&base, 4), field<U>(&base, 8), float_to_bits(dt)); // 004927D8..0049280F
                copy_words(&wind, as_object(r), 3);                            // 00492815..00492825
            }
            vector_add_assign_004a9740(pos, nullptr, &wind);                   // 00492829..00492830
        }
        ff(p, 0x5C) = R(L(ff(p, 0x5C)) - t * L(ff(emitter, 0x7C)) * L(Fk(kGravity_007432f4))); // 00492835..00492848
        const long double decayed = (L(Fk(kOne_0073d764)) - t) * L(ff(p, 0x18));    // 0049284B..00492858
        ff(p, 0x18) = R(decayed);                                              // 00492861 fst
        if (decayed < 0.0) ff(p, 0x18) = R(decayed * L(Fk(kMinusOne_00740cbc))); // 0049285B..00492871
        if (!bouncing) continue;                                               // 00492878..0049287E
        // ---- bounce (00492884..00492DBE)
        aurora_hit_info_clear_output_00456a60(field<void*>(emitter, 0x1E8), nullptr); // 00492884..0049288A
        copy_words(&base, pos, 3);                                             // 0049288F..004928A3
        if (field<U8>(node, 0xF0) != 0) {                                      // 004928A7..004928AF (chunk name)
            PointerList& ignore = field<PointerList>(field<void*>(emitter, 0x1E8), 8); // 004928B1..004928BA
            if (ignore.data != 0) {                                            // 004928B7..004928BF
                live_free(as_object(ignore.data));                             // 004928C1..004928C2
                ignore.data = 0;                                               // 004928CA
            }
            ignore.capacity = 0; ignore.size = 0;                              // 004928D0..004928D5
            void* const chunk = field<void*>(p, 0x74);                         // 004928D8..004928DD
            if (chunk != nullptr) {
                PointerList& list = field<PointerList>(field<void*>(emitter, 0x1E8), 8); // 004928E3..004928EC
                if (list.size == list.capacity) {                              // 004928E9..004928F2
                    const I capacity = list.capacity;
                    menu_list_growth::allocate_005e03c0(&list, nullptr,
                        static_cast<menu_list_growth::Word>(capacity != 0 ? capacity + capacity : 8)); // 004928F4..00492904
                }
                reinterpret_cast<volatile U*>(static_cast<std::uintptr_t>(list.data))[list.size] = word_of(chunk); // 0049290D..00492912
                list.size = list.size + 1;                                     // 00492915
            }
        }
        void* const scene = active_scene();                                    // 00492918 / 0049295E
        Vector current; copy_words(&current, pos, 3);                          // 00492926..0049293C (by value)
        const I hit = scene_table(scene)->s038_scene_hit_check(scene, nullptr, old, current, pos,
                                                               field<void*>(emitter, 0x1E8)); // 0049291F..00492964
        if (hit == 0) continue;                                                // 00492967..00492969
        const U mode = emitter_cell_00798ba0;                                  // 0049296F
        if (mode == 1) {                                                       // 00492974..00492977
            Vector* const velocity = &field<Vector>(p, 0x54);                  // ebp
            if (ff(emitter, 0x5C) == 0.0f) {                                   // 0049297D..0049298B (jp on != / unordered)
                field<U>(velocity, 0) = 0; field<U>(velocity, 4) = 0; field<U>(velocity, 8) = 0; // 0049298D..004929C9
                bounce_metric = 0.0;                                           // 004929CC mov dword [esp+10h], 0 (+0.0f)
            } else {
                long double k = L(Fk(kBounceStep_00743330)) / t;                     // 004929D9..004929DF
                if (k > L(Fk(kOne_0073d764))) k = L(Fk(kOne_0073d764));          // 004929E6..004929F5
                else if (k < L(Fk(kHalf_0073e9ac))) k = L(Fk(kHalf_0073e9ac));   // 004929FD..00492A0C
                void* const info = field<void*>(emitter, 0x1E8);               // 00492A12
                const long double nx2 = L(ff(info, 0x40)) * L(Fk(kMinusTwo_007432f0)); // 00492A18..00492A1D
                const long double ny2 = L(ff(info, 0x44)) * L(Fk(kMinusTwo_007432f0)); // 00492A23..00492A26
                const float nz2 = R(L(ff(info, 0x48)) * L(Fk(kMinusTwo_007432f0))); // 00492A2C..00492A35
                const float dot = R((L(ff(info, 0x48)) * L(velocity->z) + L(ff(info, 0x44)) * L(velocity->y)) +
                                    L(ff(info, 0x40)) * L(velocity->x));        // 00492A3C..00492A52
                const float tx = R(nx2 * L(dot));                              // 00492A56..00492A5C
                const long double ty = ny2 * L(dot);                                // 00492A63
                const long double tz = L(nz2) * L(dot);                             // 00492A67..00492A6E
                const float rx = R(L(tx) + L(velocity->x));                    // 00492A72..00492A7C
                const float ry = R(ty + L(velocity->y));                       // 00492A83..00492A88
                const long double rz = tz + L(velocity->z);                         // 00492A8F
                velocity->x = R(L(rx) * L(Fk(kThreeQuarters_0073e31c)));        // 00492A92..00492AAE
                velocity->y = R(L(ry) * L(Fk(kThreeQuarters_0073e31c)));        // 00492AA7..00492AC4
                velocity->z = R(rz * L(Fk(kThreeQuarters_0073e31c)));           // 00492ABE..00492ACF
                const long double damped = k * L(velocity->z) * L(ff(emitter, 0x5C)); // 00492AD2..00492AD5
                ff(p, 0x5C) = R(damped);                                       // 00492AD8 fst
                bounce_metric = R((L(velocity->x) * L(velocity->x) + L(ff(p, 0x58)) * L(ff(p, 0x58))) +
                                  damped * damped);                            // 00492ADB..00492AF1
            }
        } else if (mode == 2) {                                                // 00492B00..00492B03
            Vector* const velocity = &field<Vector>(p, 0x54);
            void* const info = field<void*>(emitter, 0x1E8);                   // 00492B09
            const long double nx2 = L(ff(info, 0x40)) * L(Fk(kMinusTwo_007432f0));   // 00492B0F..00492B13
            const long double ny2 = L(ff(info, 0x44)) * L(Fk(kMinusTwo_007432f0));   // 00492B19..00492B1C
            const float nz2 = R(L(ff(info, 0x48)) * L(Fk(kMinusTwo_007432f0))); // 00492B22..00492B2B
            const float dot = R((L(velocity->z) * L(ff(info, 0x48)) + L(velocity->y) * L(ff(info, 0x44))) +
                                L(ff(info, 0x40)) * L(velocity->x));            // 00492B32..00492B4A
            const float tx = R(nx2 * L(dot));                                  // 00492B4E..00492B54
            const long double ty = ny2 * L(dot);                                    // 00492B5B
            const long double tz = L(nz2) * L(dot);                                 // 00492B5F..00492B66
            const float rx = R(L(tx) + L(velocity->x));                        // 00492B6A..00492B74
            const float ry = R(ty + L(velocity->y));                           // 00492B7B..00492B80
            const long double rz = tz + L(velocity->z);                             // 00492B87
            const long double e = ff(emitter, 0x5C);                                // 00492B8A
            velocity->x = R(L(rx) * e);                                        // 00492B8D..00492BA5
            velocity->y = R(L(ry) * e);                                        // 00492B9E..00492BB5
            velocity->z = R(rz * e);                                           // 00492BB1..00492BC2
            bounce_metric = R(vector_magnitude_004ab100(velocity));            // 00492BC5..00492BCA
        }
        bool stick;
        if (emitter_cell_00798ba0 == 1) {                                      // 00492BD1..00492BD8
            stick = bounce_metric < kHalf_0073f3d0;                         // 00492BDA..00492BE9 (C0 only)
        } else {
            void* const info = field<void*>(emitter, 0x1E8);                   // 00492BF4
            long double ax_ = ff(info, 0x40);                                       // 00492BFA..00492C0A
            if (ax_ < 0.0) ax_ = -ax_;
            const float abs_x = R(ax_);                                        // 00492C0C fstp dword [esp+14h]
            // 00492C10 fld dword [esp+14h] ; 00492C14 call 006FC0E0 ; 00492C19 fcomp qword [00743328] ; test ah,41h ; jne:
            // continues only when acos > pi/4 (ordered).
            stick = acos(L(abs_x)) > kQuarterPi_00743328;
            if (stick) {
                long double ay_ = ff(info, 0x44);                                   // 00492C2A..00492C3A
                if (ay_ < 0.0) ay_ = -ay_;
                const float abs_y = R(ay_);                                    // 00492C3C fstp dword [esp+14h]
                stick = acos(L(abs_y)) > kQuarterPi_00743328;                  // 00492C40..00492C54 (as above)
            }
            if (stick) stick = t * L(Fk(kStickRate_00743324)) > bounce_metric; // 00492C5A..00492C70
        }
        if (!stick) continue;
        // ---- stick (00492C76..00492DBE)
        field<U8>(p, 0x6C) = 1;                                                // 00492C76
        Vector to; copy_words(&to, at(field<void*>(emitter, 0x1E8), 0x40), 3); // 00492C7A..00492C95
        Vector from; field<U>(&from, 0) = 0; field<U>(&from, 4) = 0; field<U>(&from, 8) = kOne_0073d764; // 00492C9B..00492CE0
        Quaternion arc;
        const Quaternion* const r = rotation_arc_004ab630(&arc, from, to);     // 00492CDF..00492CE3
        copy_words(at(p, 0x2C), r, 4);                                         // 00492CE8..00492CFE
        void* info = field<void*>(emitter, 0x1E8);                             // 00492D01
        Vector offset;
        offset.z = R(L(ff(info, 0x48)) * L(Fk(kOffset_00743320)));              // 00492D07..00492D0D
        offset.y = R(L(ff(info, 0x44)) * L(Fk(kOffset_00743320)));              // 00492D16..00492D20
        offset.x = R(L(ff(info, 0x40)) * L(Fk(kOffset_00743320)));              // 00492D27..00492D2C
        vector_add_assign_004a9740(pos, nullptr, &offset);                     // 00492D34..00492D49
        copy_words(at(p, 0x48), at(field<void*>(emitter, 0x1E8), 0x40), 3);    // 00492D4E..00492D6B
        if (field<U8>(node, 0xF0) != 0) {                                      // 00492D61..00492D76
            const float lift = ff(emitter, 0x90);                              // 00492D78..00492D84
            info = field<void*>(emitter, 0x1E8);                               // 00492D7E
            Vector push;
            push.z = R(L(lift) * L(ff(info, 0x48)));                           // 00492D88..00492D8F
            push.y = R(L(lift) * L(ff(info, 0x44)));                           // 00492D99..00492D9E
            push.x = R(L(lift) * L(ff(info, 0x40)));                           // 00492DA3..00492DA7
            vector_add_assign_004a9740(pos, nullptr, &push);                   // 00492DA9..00492DBE
        }
    }
    void* const gob = field<void*>(emitter, 0x40);                             // 00492DDE
    if (gob != nullptr && ((field<U>(node, 0x12C) >> 8) & 1u) != 0) {         // 00492DE1..00492DF1
        copy_words(at(emitter, 0x158), at(gob, 0x78), 3);                      // 00492DF3..00492E0E
        copy_words(at(emitter, 0x194), at(gob, 0x84), 4);                      // 00492E09..00492E2A
        return;
    }
    if (((field<U>(node, 0x12C) >> 10) & 1u) != 0) {                           // 00492E3A..00492E45
        copy_words(at(emitter, 0x158), at(emitter, 0x164), 3);                 // 00492E47..00492E60
        copy_words(at(emitter, 0x194), at(emitter, 0x1A4), 4);                 // 00492E63..00492E82
    }
}

} // extern "C"
