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
#include "4ac9e30747-emitter.h"

// LOCAL-DECLS merged into emitter.h (lane header).

namespace {
using namespace kotor_emitter;

// .rdata constants (values read from the pinned image)
constexpr U kHundred_00741838 = 0x42C80000;        // 100.0f
constexpr double kHundredth_00743310 = 0.01;       // qword
constexpr U kHundredth_0073dfec = 0x3C23D70A;      // 0.01f
constexpr U kHalf_0073e9ac = 0x3F000000;           // 0.5f
constexpr U kTenThousandth_00741870 = 0x38D1B717;  // 0.0001f
constexpr U kMinusHundred_0074186c = 0xC2C80000;   // -100.0f
constexpr U kHalfPi_00741bb8 = 0x3FC90FDB;         // 1.5707964f
// lane x87-trig (2026-10-24): the former kHalfPi_007432f8 = 1.5707963267948966 (bits 3FF921FB54442D18) was not
// the image's qword. [007432F8] = 3FF921FB52713C02 (1.5707963259...). The former x87_fcos also rounded the
// 64-bit-significand fcos result to double before the 0048E01C fcompp. Both are replaced by x87trig_0048e01a.
const unsigned long long kX87HalfPi_007432f8 = 0x3FF921FB52713C02ull;   // qword [007432F8]
constexpr U kOne_0073d764 = 0x3F800000;            // 1.0f
constexpr U kMinusTwo_007432f0 = 0xC0000000;       // -2.0f
constexpr U kGravity_007432f4 = 0x411CF5C3;        // 9.81f
constexpr U kMinusOne_00740cbc = 0xBF800000;       // -1.0f
constexpr U kBounceStep_00743330 = 0x3D072B02;     // 0.033f
constexpr U kThreeQuarters_0073e31c = 0x3F400000;  // 0.75f
constexpr U kOffset_00743320 = 0x3CA3D70A;         // 0.02f
constexpr U kStickRate_00743324 = 0x41D00000;      // 26.0f
constexpr double kQuarterPi_00743328 = 0.7853981633974483; // qword
constexpr double kHalf_0073f3d0 = 0.5;             // qword

inline float& fl(void* object, U offset) { return field<float>(object, offset); }
inline void copy_words(void* to, const void* from, U count) { std::memcpy(to, from, count * 4u); }   // raw DWORD moves
inline const kotor_scene3d::SceneVTable* scene_table(void* scene) {
    return *static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene);
}
inline void* active_scene() { return *kotor_scene3d::cells.active_scene_007b92a0; }
// rand()%2 > 0 as compiled: and eax,80000001h / jns / dec / or -2 / inc / test / jle
inline bool rand_odd() { return crt_rand_006fc447() % 2 > 0; }
} // namespace

extern "C" {

// ================================================================ 0048D870 Particle::randomPosition(rx, ry), RET 8
void __fastcall particle_random_position_0048d870(void* self, void*, I range_x, I range_y) {
    I x = 0;                                                                   // 0048D8A9
    if (range_x > 0) {                                                         // 0048D877..0048D87E
        const I a = crt_rand_006fc447();                                       // 0048D880
        const I b = crt_rand_006fc447();                                       // 0048D887
        const I n = ftol2(static_cast<double>(range_x) * W(F(kHalf_0073e9ac))); // 0048D88C..0048D899 fild/fmul/_ftol2
        x = (a * b) % n;                                                       // 0048D890 imul; 0048D8A3 idiv (n==0 traps as in the original)
    }
    I y = 0;                                                                   // 0048D8E0
    if (range_y > 0) {                                                         // 0048D8AB..0048D8B5
        const I a = crt_rand_006fc447();                                       // 0048D8B7
        const I b = crt_rand_006fc447();                                       // 0048D8BE
        const I n = ftol2(static_cast<double>(range_y) * W(F(kHalf_0073e9ac))); // 0048D8C3..0048D8D0
        y = (a * b) % n;                                                       // 0048D8C7 / 0048D8DA
    }
    if (rand_odd()) x = -x;                                                    // 0048D8E6..0048D8FD
    if (rand_odd()) y = -y;                                                    // 0048D901..0048D918
    void* const emitter = field<void*>(self, 4);                               // 0048D91C
    const double sy = static_cast<double>(y) * W(F(kTenThousandth_00741870));  // 0048D91F..0048D927
    const double ay = sy * W(fl(emitter, 0x134));                              // 0048D930 (stays on the stack)
    const float by = S(sy * W(fl(emitter, 0x138)));                            // 0048D938..0048D93E
    const float cy = S(sy * W(fl(emitter, 0x13C)));                            // 0048D944..0048D94A
    const double sx = static_cast<double>(x) * W(F(kTenThousandth_00741870));  // 0048D94E..0048D952
    const double px = sx * W(fl(emitter, 0x128));                              // 0048D95A
    const float qx = S(sx * W(fl(emitter, 0x12C)));                            // 0048D962..0048D968
    const float rx = S(sx * W(fl(emitter, 0x130)));                            // 0048D96E..0048D974
    const double wx = px + W(fl(emitter, 0x164));                              // 0048D978
    const float wy = S(W(qx) + W(fl(emitter, 0x168)));                         // 0048D97E..0048D988
    const float wz = S(W(rx) + W(fl(emitter, 0x16C)));                         // 0048D98C..0048D996
    fl(self, 0x08) = S(wx + ay);                                               // 0048D99A..0048D9A4
    fl(self, 0x0C) = S(W(wy) + W(by));                                         // 0048D9A8..0048D9C0
    fl(self, 0x10) = S(W(wz) + W(cy));                                         // 0048D9B4..0048D9CB
}

// ================================================================ 0048D9E0 Particle::inheritVelocity(dt), RET 4
void __fastcall particle_inherit_velocity_0048d9e0(void* self, void*, float dt) {
    void* const e = field<void*>(self, 4);                                     // 0048D9E3
    const double q0 = fl(e, 0x1B4), q1 = fl(e, 0x1B8), q2 = fl(e, 0x1BC), q3 = fl(e, 0x1C0);
    const double a = (q2 * q0 + q1 * q3) * 2.0;                                // 0048D9E6..0048DA00 (stack)
    const double b = (q2 * q3 - q0 * q1) * 2.0;                                // 0048DA02..0048DA1C (stack)
    const float c = S(1.0 - (q2 * q2 + q1 * q1) * 2.0);                        // 0048DA1E..0048DA3C
    const float dx = S(W(fl(e, 0x140)) - a);                                   // 0048DA44..0048DA4C
    const float dy = S(W(fl(e, 0x144)) - b);                                   // 0048DA50..0048DA58
    const float dz = S(W(fl(e, 0x148)) - W(c));                                // 0048DA60..0048DA6A
    const float lx = S(W(fl(e, 0x164)) - W(fl(self, 0x08)));                   // 0048DA6E..0048DA77
    const float ly = S(W(fl(e, 0x168)) - W(fl(self, 0x0C)));                   // 0048DA7B..0048DA84
    const float lz = S(W(fl(e, 0x16C)) - W(fl(self, 0x10)));                   // 0048DA88..0048DA91
    double k;
    if (W(dt) > 0.0) k = W(F(kOne_0073d764)) / W(dt);                          // 0048DA95..0048DAAC (jne on <= / unordered)
    else k = W(F(kHundredth_0073dfec));                                        // 0048DAB2
    const double len = std::sqrt((W(lz) * W(lz) + W(ly) * W(ly)) + W(lx) * W(lx)); // 0048DAB8..0048DAD5
    const double xv = W(dx) * len * k;                                         // 0048DAD7/0048DAF1 (stack)
    const float ty = S(W(dy) * len);                                           // 0048DADD..0048DAE3
    const float tz = S(W(dz) * len);                                           // 0048DAE7..0048DAED
    const float uy = S(W(ty) * k);                                             // 0048DAF5..0048DAFB
    const float uz = S(W(tz) * k);                                             // 0048DAFF..0048DB05
    const double ex = W(fl(e, 0x164)) - W(fl(e, 0x170));                       // 0048DB09..0048DB0F
    const double ey = W(fl(e, 0x168)) - W(fl(e, 0x174));                       // 0048DB15..0048DB1B
    const float ez = S(W(fl(e, 0x16C)) - W(fl(e, 0x178)));                     // 0048DB21..0048DB32
    const float ax = S(ex * k);                                                // 0048DB36..0048DB3A
    const float ay = S(ey * k);                                                // 0048DB3E..0048DB40
    const float az = S(W(ez) * k);                                             // 0048DB44..0048DB4A
    fl(self, 0x1C) = S(W(ax) + xv);                                            // 0048DB4E..0048DB5C
    fl(self, 0x20) = S(W(ay) + W(uy));                                         // 0048DB62..0048DB7A
    fl(self, 0x24) = S(W(az) + W(uz));                                         // 0048DB6E..0048DB85
    const double x = fl(self, 0x1C), y = fl(self, 0x20), z = fl(self, 0x24);   // 0048DB88..0048DB8E
    const double m = std::sqrt((x * x + y * y) + z * z);                       // 0048DB90..0048DBA0
    fl(self, 0x18) = S(m);                                                     // 0048DBAF fst
    if (m == 0.0) {                                                            // 0048DBA9..0048DBB7 (C3 only)
        field<U>(self, 0x1C) = 0; field<U>(self, 0x20) = 0; field<U>(self, 0x24) = 0; // 0048DBF1..0048DC1C
        return;
    }
    const double inv = W(F(kOne_0073d764)) / m;                                // 0048DBB9 fdivr
    const float nx = S(inv * W(fl(self, 0x1C)));                               // 0048DBBF..0048DBC3
    const float ny = S(inv * W(fl(self, 0x20)));                               // 0048DBC7..0048DBCC
    const float nz = S(inv * W(fl(self, 0x24)));                               // 0048DBD4..0048DBE0
    fl(self, 0x1C) = nx; fl(self, 0x20) = ny; fl(self, 0x24) = nz;             // 0048DBDB..0048DBE8
}

// ---- lane x87-trig a2 (2026-10-24): 00492C10..00492C24 / 00492C40..00492C54 (moveParticleStandard stick test) ----
// The former code called std::acos (the modern CRT) where the original calls 006FC0E0 (_CIacos) and compares its
// 64-bit-significand ST0 result with fcomp qword [00743328]. Now: fld dword -> 80-bit operand image -> the accepted
// t11b replica crt_acos_x87_006fc0e0 (80-bit in/out) -> fcomp qword / fnstsw ax. No CALL inside the asm.
}  // extern "C"
namespace x87trig {
#pragma pack(push, 1)
struct X80 { unsigned char bytes[10]; };   // FSTP TBYTE image (same layout as crt_x87::Extended)
#pragma pack(pop)
const unsigned long long kQuarterPi_00743328 = 0x3FE921FB54442D18ull;   // qword [00743328]
}
extern "C" {
void __cdecl crt_acos_x87_006fc0e0(const x87trig::X80* st0_in, x87trig::X80* st0_out);   // t11b provider (006FC0E0)
// 00492C10 / 00492C40: fld dword ptr [esp+14h] (the operand image handed to 006FC0E0)
__declspec(noinline) void __cdecl x87trig_00492c10(const float* operand, x87trig::X80* st0) {
    __asm {
        mov eax, operand
        fld dword ptr [eax]                    // 00492C10 / 00492C40
        mov eax, st0
        fstp tbyte ptr [eax]
    }
}
// 00492C19..00492C1F / 00492C49..00492C4F: fcomp qword ptr [00743328]; fnstsw ax -> AX (the caller tests 41h; jne).
__declspec(noinline) unsigned __cdecl x87trig_00492c19(const x87trig::X80* acos_result) {
    unsigned short status;
    __asm {
        mov eax, acos_result
        fld tbyte ptr [eax]
        fcomp qword ptr x87trig::kQuarterPi_00743328   // 00492C19 / 00492C49
        fnstsw ax                              // 00492C1F / 00492C4F
        mov status, ax
    }
    return status;
}

// ---- lane x87-trig (2026-10-24): 0048E00D..0048E01E kept whole ----
// Precondition as in the original: PC = 53 bits, RC = nearest. ST1 at 0048E00D is the |dot| copy made by
// 0048DFFC fld st(0) / 0048E005 fchs; it is a sum of float x float products rounded under PC 53, which the double
// `magnitude` carries exactly. fcos keeps its 64-bit significand into the fcompp. Returns the fnstsw AX image.
// (Inside this file's extern "C" block.)
__declspec(noinline) unsigned __cdecl x87trig_0048e01a(double magnitude, const void* node) {
    unsigned short status;
    __asm {
        fld qword ptr magnitude                // ST1 (0048DFFC / 0048E005)
        mov eax, node
        fld dword ptr [eax + 50h]              // 0048E00D
        fsubr qword ptr kX87HalfPi_007432f8    // 0048E014
        fcos                                   // 0048E01A
        fcompp                                 // 0048E01C
        fnstsw ax                              // 0048E01E
        mov status, ax
    }
    return status;
}

// ================================================================ 0048DE60 Particle::randomDirection(a, b), RET 8
// Neither stack argument is read (frame 5Ch + 4 saves; [esp+70h]/[esp+74h] never referenced).
void __fastcall particle_random_direction_0048de60(void* self, void*, I, I) {
    void* const e = field<void*>(self, 4);                                     // 0048DE66
    const float half_spread = S(W(fl(e, 0xA0)) * W(F(kHalf_0073e9ac)));        // 0048DE69..0048DE7D
    void* const camera = *kotor_scene3d::cells.current_camera_007bb4e8;        // 0048DE6F
    Vector to_camera;
    to_camera.x = S(W(fl(self, 0x08)) - W(fl(camera, 0x7C)));                  // 0048DE9D..0048DEA3
    to_camera.y = S(W(fl(self, 0x0C)) - W(fl(camera, 0x80)));                  // 0048DE93..0048DEA7
    to_camera.z = S(W(fl(self, 0x10)) - W(fl(camera, 0x84)));                  // 0048DE85..0048DEAB
    Vector n;
    normalize_004ab180(&n, &to_camera);                                        // 0048DEAF
    Quaternion* const q = &field<Quaternion>(self, 0x2C);                      // 0048DED9
    {
        const I r = crt_rand_006fc447();                                       // 0048DEB7
        const I m = ftol2(W(half_spread) * W(F(kMinusHundred_0074186c)));     // 0048DEBC..0048DEC8
        const I rem = r % (1 - m);                                             // 0048DECD..0048DED7
        Vector axis; copy_words(&axis, at(e, 0x128), 3);                        // 0048DEEA..0048DF0E (emitter row 0)
        const float angle = S(static_cast<double>(rem) * W(F(kHundredth_0073dfec))); // 0048DEE6..0048DEFB
        Quaternion spin, product;
        quaternion_construct_004ac960(&spin, nullptr, axis, angle);             // 0048DF11
        const Quaternion* const result = quaternion_multiply_004a9ac0(&product, &spin, q); // 0048DF16..0048DF1C
        copy_words(q, result, 4);                                              // 0048DF21..0048DF39
    }
    U held[4];                                                                 // ecx/edx/ebp/edi at 0048DFDA..0048DFE9
    {
        const I rem = crt_rand_006fc447() % 0x274;                             // 0048DF3C..0048DF47
        Vector axis; copy_words(&axis, at(e, 0x140), 3);                        // 0048DF57..0048DF7B (emitter row 2)
        const float angle = S(static_cast<double>(rem) * W(F(kHundredth_0073dfec))); // 0048DF53..0048DF68
        Quaternion spin, product;
        quaternion_construct_004ac960(&spin, nullptr, axis, angle);             // 0048DF7E
        const Quaternion* const result = quaternion_multiply_004a9ac0(&product, &spin, q); // 0048DF83..0048DF89
        copy_words(q, result, 4);                                              // 0048DF8E..0048DFA7
        copy_words(held, result, 4);                                           // 0048DFDA..0048DFE9 (reloaded from the result)
    }
    if (fl(field<void*>(field<void*>(self, 4), 4), 0x50) == 0.0f) return;     // 0048DFAA..0048DFC1 (node +50)
    Vector y_axis;
    const Vector* const y = quaternion_ydir_0044f420(q, nullptr, &y_axis);     // 0048DFC7..0048DFCE
    double dot = (W(n.y) * W(y->y) + W(n.z) * W(y->z)) + W(n.x) * W(y->x);     // 0048DFD3..0048DFF4
    for (;;) {
        double magnitude = dot;                                                // 0048DFF6..0048E005 fcom/fld st0/fchs
        if (dot < 0.0) magnitude = -dot;
        void* const node = field<void*>(field<void*>(self, 4), 4);             // 0048E007..0048E010
        const unsigned status = x87trig_0048e01a(magnitude, node);            // 0048E00D..0048E01E fsubr qword / fcos / fcompp
        if ((status & 0x4100u) != 0) {                                         // 0048E020..0048E023 test ah,41h; jne (<= / unordered)
            copy_words(q, held, 4);                                            // 0048E178..0048E182
            return;
        }
        float angle;
        if (!(dot < 0.0)) angle = S((dot - W(F(kHalfPi_00741bb8))) + W(fl(node, 0x50))); // 0048E04D..0048E05A
        else angle = S((W(fl(node, 0x50)) + W(F(kHalfPi_00741bb8))) - dot);  // 0048E036..0048E045
        const double m0 = q->w, m1 = q->x, m2 = q->y, m3 = q->z;               // memory quaternion (unchanged in the loop)
        Vector z_axis;
        z_axis.x = S((m1 * m2 - m3 * m0) * 2.0);                               // 0048E05E..0048E07C
        z_axis.y = S(1.0 - (m3 * m3 + m1 * m1) * 2.0);                         // 0048E080..0048E098
        z_axis.z = S((m2 * m3 + m1 * m0) * 2.0);                               // 0048E0A0..0048E0AF
        Vector cross;
        const Vector* const c = vector_cross_004ab210(&cross, &n, &z_axis);    // 0048E0B3
        Vector axis; copy_words(&axis, c, 3);                                  // 0048E0C1..0048E0D7
        Quaternion spin, product;
        quaternion_construct_004ac960(&spin, nullptr, axis, angle);             // 0048E0DA
        const Quaternion* const r = quaternion_multiply_004a9ac0(&product, &spin, q); // 0048E0DF..0048E0E5
        copy_words(held, r, 4);                                                // 0048E0EA..0048E10D
        Quaternion p; copy_words(&p, held, 4);
        const double p0 = p.w, p1 = p.x, p2 = p.y, p3 = p.z;
        const double ax = (p1 * p2 - p3 * p0) * 2.0;                           // 0048E0F9..0048E11A
        const double ay = 1.0 - (p1 * p1 + p3 * p3) * 2.0;                     // 0048E11C..0048E130
        const double az = (p1 * p0 + p2 * p3) * 2.0;                           // 0048E136..0048E148
        double d = (ax * W(n.x) + ay * W(n.y)) + az * W(n.z);                  // 0048E14A..0048E15E
        if (d < 0.0) d = -d;                                                   // 0048E160..0048E171
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
    field<I>(self, 0x3C) = ftol2(W(fl(e, 0x70)));                              // 0049221A..00492226
    const float spread = fl(e, 0x8C);                                          // 00492229..0049222F (fst dword)
    if (W(spread) > kHundredth_00743310) {                                     // 00492233..0049223E (fcomp qword; jne on <= / unordered)
        const I r = crt_rand_006fc447();                                       // 00492240
        const I n = ftol2(W(spread) * W(F(kHundred_00741838)));                // 00492245..00492251
        random_speed = r % n;                                                  // 0049225A..0049225D
        if (rand_odd()) random_speed = -random_speed;                          // 00492263..0049227A
    }
    void* const e2 = field<void*>(self, 4);                                    // 0049227E
    const double scale = W(F(scale_bits));
    const I range_x = ftol2(scale * W(fl(e2, 0xAC)) * W(F(kHundred_00741838))); // 00492281..00492291
    const I range_y = ftol2(scale * W(fl(e2, 0xB0)) * W(F(kHundred_00741838))); // 00492296..004922AA
    if (range_x == 0 && range_y == 0) copy_words(at(self, 0x08), at(e2, 0x164), 3); // 004922B5..004922D8
    else particle_random_position_0048d870(self, nullptr, range_x, range_y);   // 004922DD..004922E1
    copy_words(at(self, 0x60), at(self, 0x08), 3);                             // 004922EA..004922FD
    void* const node = field<void*>(field<void*>(self, 4), 4);                 // 00492300..00492303
    if (((field<U>(node, 0x12C) >> 7) & 1u) != 0)                              // 00492306..00492311 (InheritingVelocity)
        particle_inherit_velocity_0048d9e0(self, nullptr, F(dt_bits));          // 00492313..0049231A
    if (!(fl(field<void*>(self, 4), 0xA0) == 0.0f))                            // 00492323..00492337 (jnp skips on equal)
        particle_random_direction_0048de60(self, nullptr, range_x, range_y);   // 00492339..00492341
    const double q0 = fl(self, 0x2C), q1 = fl(self, 0x30), q2 = fl(self, 0x34), q3 = fl(self, 0x38);
    fl(self, 0x48) = S((q2 * q0 + q1 * q3) * 2.0);                             // 00492346..0049237E
    fl(self, 0x4C) = S((q3 * q2 - q1 * q0) * 2.0);                             // 00492365..00492388
    fl(self, 0x50) = S(1.0 - (q1 * q1 + q2 * q2) * 2.0);                       // 00492378..004923A4
    void* const e3 = field<void*>(self, 4);                                    // 004923A7 ([esi-50h])
    const double speed = static_cast<double>(random_speed) * W(F(kHundredth_0073dfec)) + W(fl(e3, 0xA8)); // 004923AC..004923B6
    const float vx = S(speed * W(fl(self, 0x48)));                             // 004923BC..004923C0
    const float vy = S(speed * W(fl(self, 0x4C)));                             // 004923C8..004923CD
    const float vz = S(speed * W(fl(self, 0x50)));                             // 004923D1..004923DD
    fl(self, 0x54) = vx; fl(self, 0x58) = vy; fl(self, 0x5C) = vz;             // 004923D8..004923E5
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
    float bounce_metric = 0.0f;
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                         // 004924B1..004924C1 / 00492DC3..00492DD4
        void* const p = as_object(reinterpret_cast<const volatile U*>(
            static_cast<std::uintptr_t>(field<U>(emitter, 0x1C8)))[i]);       // 004924D0..004924D6
        if (field<U8>(p, 0x6C) != 0) continue;                                 // 004924D9..004924DE
        Vector* const pos = &field<Vector>(p, 0x08);                           // 004924E6 (ebx)
        Vector old; copy_words(&old, pos, 3);                                  // 004924E9..004924FB ([esp+5Ch])
        if (inheriting != 0) {                                                 // 004924E4/004924FF
            Vector local;
            local.x = S(W(pos->x) - W(fl(emitter, 0x170)));                    // 00492505..0049251C
            local.y = S(W(pos->y) - W(fl(emitter, 0x174)));                    // 0049252A..0049253A
            local.z = S(W(pos->z) - W(fl(emitter, 0x178)));                    // 00492541..0049254A
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
            delta.x = S(W(fl(gob, 0x78)) - W(fl(emitter, 0x158)));             // 0049264A..00492653
            delta.y = S(W(fl(gob, 0x7C)) - W(fl(emitter, 0x15C)));             // 0049263D..0049265A
            delta.z = S(W(fl(gob, 0x80)) - W(fl(emitter, 0x160)));             // 00492629..00492661
            vector_add_assign_004a9740(&base, nullptr, &delta);                // 00492668
            vector_add_assign_004a9740(&field<Vector>(p, 0x60), nullptr, &delta); // 0049266D..00492678
        }
        if (((field<U>(node, 0x12C) >> 10) & 1u) != 0) {                       // 0049267D..00492689 (InheritingPart)
            Vector delta;
            delta.x = S(W(fl(emitter, 0x164)) - W(fl(emitter, 0x158)));        // 0049268B..004926A3
            delta.y = S(W(fl(emitter, 0x168)) - W(fl(emitter, 0x15C)));        // 004926AA..004926B6
            delta.z = S(W(fl(emitter, 0x16C)) - W(fl(emitter, 0x160)));        // 004926BD..004926C9
            vector_add_assign_004a9740(&base, nullptr, &delta);                // 004926D0
            vector_add_assign_004a9740(&field<Vector>(p, 0x60), nullptr, &delta); // 004926D5..004926E0
        }
        // integrate position (004926E5..004927B5)
        const double t = W(dt);
        const Vector& v = field<Vector>(p, 0x54);                              // ebp = p+54
        const double ax = t * W(v.x);                                          // 004926E5..004926EF
        const double ay = t * W(v.y);                                          // 004926F4..004926FB
        const float az = S(t * W(v.z));                                        // 004926FE..00492708
        const float bx = S(ax + W(base.x));                                    // 00492711..00492715
        const double by = ay + W(base.y);                                      // 0049271C
        const double bz = W(az) + W(base.z);                                   // 00492720..00492727
        const double s = fl(p, 0x18);                                          // 0049272B
        const double sx = s * W(fl(p, 0x1C));                                  // 0049272E..00492730
        const float sy = S(s * W(fl(p, 0x20)));                                // 00492733..00492738
        const float sz = S(s * W(fl(p, 0x24)));                                // 00492741..00492744
        const double dx = sx * t;                                              // 0049274B
        const double dy = W(sy) * t;                                           // 00492752..00492759
        const float dz = S(W(sz) * t);                                         // 00492760..0049276E
        pos->x = S(dx + W(bx));                                                // 00492777..00492786
        pos->y = S(dy + by);                                                   // 00492788..0049279B
        pos->z = S(W(dz) + bz);                                                // 0049278E..004927AA
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
        fl(p, 0x5C) = S(W(fl(p, 0x5C)) - t * W(fl(emitter, 0x7C)) * W(F(kGravity_007432f4))); // 00492835..00492848
        const double decayed = (W(F(kOne_0073d764)) - t) * W(fl(p, 0x18));    // 0049284B..00492858
        fl(p, 0x18) = S(decayed);                                              // 00492861 fst
        if (decayed < 0.0) fl(p, 0x18) = S(decayed * W(F(kMinusOne_00740cbc))); // 0049285B..00492871
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
            if (fl(emitter, 0x5C) == 0.0f) {                                   // 0049297D..0049298B (jp on != / unordered)
                field<U>(velocity, 0) = 0; field<U>(velocity, 4) = 0; field<U>(velocity, 8) = 0; // 0049298D..004929C9
                field<U>(&bounce_metric, 0) = 0;                               // 004929CC
            } else {
                double k = W(F(kBounceStep_00743330)) / t;                     // 004929D9..004929DF
                if (k > W(F(kOne_0073d764))) k = W(F(kOne_0073d764));          // 004929E6..004929F5
                else if (k < W(F(kHalf_0073e9ac))) k = W(F(kHalf_0073e9ac));   // 004929FD..00492A0C
                void* const info = field<void*>(emitter, 0x1E8);               // 00492A12
                const double nx2 = W(fl(info, 0x40)) * W(F(kMinusTwo_007432f0)); // 00492A18..00492A1D
                const double ny2 = W(fl(info, 0x44)) * W(F(kMinusTwo_007432f0)); // 00492A23..00492A26
                const float nz2 = S(W(fl(info, 0x48)) * W(F(kMinusTwo_007432f0))); // 00492A2C..00492A35
                const float dot = S((W(fl(info, 0x48)) * W(velocity->z) + W(fl(info, 0x44)) * W(velocity->y)) +
                                    W(fl(info, 0x40)) * W(velocity->x));        // 00492A3C..00492A52
                const float tx = S(nx2 * W(dot));                              // 00492A56..00492A5C
                const double ty = ny2 * W(dot);                                // 00492A63
                const double tz = W(nz2) * W(dot);                             // 00492A67..00492A6E
                const float rx = S(W(tx) + W(velocity->x));                    // 00492A72..00492A7C
                const float ry = S(ty + W(velocity->y));                       // 00492A83..00492A88
                const double rz = tz + W(velocity->z);                         // 00492A8F
                velocity->x = S(W(rx) * W(F(kThreeQuarters_0073e31c)));        // 00492A92..00492AAE
                velocity->y = S(W(ry) * W(F(kThreeQuarters_0073e31c)));        // 00492AA7..00492AC4
                velocity->z = S(rz * W(F(kThreeQuarters_0073e31c)));           // 00492ABE..00492ACF
                const double damped = k * W(velocity->z) * W(fl(emitter, 0x5C)); // 00492AD2..00492AD5
                fl(p, 0x5C) = S(damped);                                       // 00492AD8 fst
                bounce_metric = S((W(velocity->x) * W(velocity->x) + W(fl(p, 0x58)) * W(fl(p, 0x58))) +
                                  damped * damped);                            // 00492ADB..00492AF1
            }
        } else if (mode == 2) {                                                // 00492B00..00492B03
            Vector* const velocity = &field<Vector>(p, 0x54);
            void* const info = field<void*>(emitter, 0x1E8);                   // 00492B09
            const double nx2 = W(fl(info, 0x40)) * W(F(kMinusTwo_007432f0));   // 00492B0F..00492B13
            const double ny2 = W(fl(info, 0x44)) * W(F(kMinusTwo_007432f0));   // 00492B19..00492B1C
            const float nz2 = S(W(fl(info, 0x48)) * W(F(kMinusTwo_007432f0))); // 00492B22..00492B2B
            const float dot = S((W(velocity->z) * W(fl(info, 0x48)) + W(velocity->y) * W(fl(info, 0x44))) +
                                W(fl(info, 0x40)) * W(velocity->x));            // 00492B32..00492B4A
            const float tx = S(nx2 * W(dot));                                  // 00492B4E..00492B54
            const double ty = ny2 * W(dot);                                    // 00492B5B
            const double tz = W(nz2) * W(dot);                                 // 00492B5F..00492B66
            const float rx = S(W(tx) + W(velocity->x));                        // 00492B6A..00492B74
            const float ry = S(ty + W(velocity->y));                           // 00492B7B..00492B80
            const double rz = tz + W(velocity->z);                             // 00492B87
            const double e = fl(emitter, 0x5C);                                // 00492B8A
            velocity->x = S(W(rx) * e);                                        // 00492B8D..00492BA5
            velocity->y = S(W(ry) * e);                                        // 00492B9E..00492BB5
            velocity->z = S(rz * e);                                           // 00492BB1..00492BC2
            bounce_metric = S(vector_magnitude_004ab100(velocity));            // 00492BC5..00492BCA
        }
        bool stick;
        if (emitter_cell_00798ba0 == 1) {                                      // 00492BD1..00492BD8
            stick = W(bounce_metric) < kHalf_0073f3d0;                         // 00492BDA..00492BE9 (C0 only)
        } else {
            void* const info = field<void*>(emitter, 0x1E8);                   // 00492BF4
            double ax_ = fl(info, 0x40);                                       // 00492BFA..00492C0A
            if (ax_ < 0.0) ax_ = -ax_;
            const float abs_x = S(ax_);                                        // 00492C0C
            // lane x87-trig a2: the original CRT acos 006FC0E0 (t11b replica, 80-bit in/out) and fcomp qword [00743328].
            x87trig::X80 operand, arc;
            x87trig_00492c10(&abs_x, &operand);                                // 00492C10
            crt_acos_x87_006fc0e0(&operand, &arc);                             // 00492C14 call 006FC0E0
            stick = (x87trig_00492c19(&arc) & 0x4100u) == 0;                   // 00492C19..00492C24 (jne on <=, unordered)
            if (stick) {
                double ay_ = fl(info, 0x44);                                   // 00492C2A..00492C3A
                if (ay_ < 0.0) ay_ = -ay_;
                const float abs_y = S(ay_);                                    // 00492C3C
                x87trig_00492c10(&abs_y, &operand);                            // 00492C40
                crt_acos_x87_006fc0e0(&operand, &arc);                         // 00492C44 call 006FC0E0
                stick = (x87trig_00492c19(&arc) & 0x4100u) == 0;               // 00492C49..00492C54
            }
            if (stick) stick = t * W(F(kStickRate_00743324)) > W(bounce_metric); // 00492C5A..00492C70
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
        offset.z = S(W(fl(info, 0x48)) * W(F(kOffset_00743320)));              // 00492D07..00492D0D
        offset.y = S(W(fl(info, 0x44)) * W(F(kOffset_00743320)));              // 00492D16..00492D20
        offset.x = S(W(fl(info, 0x40)) * W(F(kOffset_00743320)));              // 00492D27..00492D2C
        vector_add_assign_004a9740(pos, nullptr, &offset);                     // 00492D34..00492D49
        copy_words(at(p, 0x48), at(field<void*>(emitter, 0x1E8), 0x40), 3);    // 00492D4E..00492D6B
        if (field<U8>(node, 0xF0) != 0) {                                      // 00492D61..00492D76
            const float lift = fl(emitter, 0x90);                              // 00492D78..00492D84
            info = field<void*>(emitter, 0x1E8);                               // 00492D7E
            Vector push;
            push.z = S(W(lift) * W(fl(info, 0x48)));                           // 00492D88..00492D8F
            push.y = S(W(lift) * W(fl(info, 0x44)));                           // 00492D99..00492D9E
            push.x = S(W(lift) * W(fl(info, 0x40)));                           // 00492DA3..00492DA7
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
