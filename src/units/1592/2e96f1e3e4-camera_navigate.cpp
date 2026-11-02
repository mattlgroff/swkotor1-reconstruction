// CSWCameraNavigate table 007515F4 and the five R1 bodies.
// Source-only reconstruction from the pinned original disassembly. The table and
// all five bodies are the only new definitions here; every helper is an existing
// selected provider (see ../PROVIDERS.md). No original address is dereferenced and
// no original byte payload is embedded.
#include "39f0b22b2a-camera_navigate.h"
// L3 MATH-TRIG-CPP t1 (2026-10-24): 006387E0 Control is written in the PLAN-v3 writing rule and this unit is built
// with the clang LD recipe (cl /arch:IA32 /fp:strict -> clang-cl + -mno-sse -ffp-model=strict -mlong-double-80;
// coordinator ruling 2026-10-24). kotor_math.h is the math declaration set of an LD unit (no <cmath>).
// L3 t1 a2 (2026-10-26): Control's helpers rewritten (the four other bodies and the table are unchanged):
//   * the original copies every quaternion / vector it passes by value, and every float it does not compute (the
//     stored yaw / pitch words pushed to YawPitchRoll), with integer moves. clang's i386 ABI expands a by-value struct
//     of floats into float parameters copied with fld/fstp dword (that quiets a signalling NaN); here those arguments
//     are the word structs Q4W / V3W or U32 words, through an explicitly typed call of the same callee (the vtable
//     slots; the direct callee 004A9D60 / 004ACAC0 through a volatile code address, so the optimizer cannot re-type the
//     call to the callee's own float prototype);
//   * a float the original stores to a stack slot and reloads after calls is a volatile StoredFloat (one store, read
//     back as its word): exactly one (float) conversion per original store, no x87 value spilled narrower than 80 bits;
//   * every helper is __forceinline (the recipe is /Ob0).
#include "../../math/kotor_math.h"

#include <cstddef>
#include <cstring>

namespace {

using kotor_scene3d::bits_to_float;
using kotor_scene3d::field;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Vector;

// Exact float constants from the pinned .rdata reads.
constexpr float kFive = 5.0f;                                          // 00743264; bits 0x40A00000
constexpr float kThree = 3.0f;                                         // 00740C84; bits 0x40400000
constexpr float kThreePointThreeThree = 3.3299999237060546875f;        // 00751624; bits 0x40551EB8
constexpr float kTwoPointSixSeven = 2.6700000762939453125f;            // 00751620; bits 0x402AE148
constexpr float kNinety = 90.0f;                                       // 0073E40C; bits 0x42B40000
constexpr float kMinusNinety = -90.0f;                                 // 0074FC44; bits 0xC2B40000
constexpr float kLowpassC1 = 0.96700000762939453125f;                  // 0075161C; bits 0x3F778D50
constexpr float kLowpassC2 = 0.032999999821186065673828125f;           // 00743330; bits 0x3D072B02
constexpr float kZero = 0.0f;                                          // 0073D700
constexpr float kOne = 1.0f;                                           // 0073D764

using U32 = std::uint32_t;
struct V3W { U32 x, y, z; };          // a Vector as three raw words
struct Q4W { U32 w, x, y, z; };       // a Quaternion as four raw words
static_assert(sizeof(V3W) == sizeof(Vector) && sizeof(Q4W) == sizeof(Quaternion), "word forms");
__forceinline V3W words3(const void* p) { V3W v; std::memcpy(&v, p, 12); return v; }
__forceinline Q4W words4(const void* p) { Q4W q; std::memcpy(&q, p, 16); return q; }

// A float the original stores (fstp dword) to a stack slot and uses again after calls: one volatile store, read back
// as its word (pushed as an argument) or loaded from that word (fld).
struct StoredFloat {
    volatile float f;
    __forceinline U32 word() const { return *reinterpret_cast<const volatile U32*>(&f); }
    __forceinline void set_word(U32 w) { *reinterpret_cast<volatile U32*>(&f) = w; }
    __forceinline long double value() const { const U32 w = word(); float g; std::memcpy(&g, &w, 4); return g; }
};
// A float this function stored into a buffer that the original reloads (fld / fmul dword) after the copy or a call.
__forceinline long double reload(const float& f) {
    const U32 w = *reinterpret_cast<const volatile U32*>(&f);
    float g; std::memcpy(&g, &w, 4); return g;
}
__forceinline long double ld(const float& f) { return f; }                       // fld dword

// L3 t1 (2026-10-24): field access for Control. The recipe is /Ob0, so kotor_scene3d::field (plain `inline`) is a
// call; the original addresses fields directly ([esi+1Ch] ...). `ref` is __forceinline (honoured under /Ob0).
template<class T> __forceinline T& ref(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}

// The camera Gob view at this+10h (0063880C / 00638B55 / ... reload [esi+10h] for each call) and its virtual slots
// (vtable layout 00741988): +5C SetPosition(ret*, Vector) RET 10h, +60 SetOrientation(ret*, Quaternion) RET 14h,
// +64 GetPosition(ret*) RET 4, +68 GetOrientation(ret*) RET 4 (both return the buffer; the callers read through EAX).
__forceinline void* owner_ptr(void* self) { return *reinterpret_cast<void* volatile*>(static_cast<unsigned char*>(self) + 0x10); }
__forceinline void* vfn(void* object, unsigned slot) { return *reinterpret_cast<void**>(*static_cast<unsigned char**>(object) + slot); }
__forceinline const Quaternion* owner_get_orientation(void* self, Quaternion* out) {
    void* const o = owner_ptr(self);
    return reinterpret_cast<Quaternion*(__thiscall*)(void*, Quaternion*)>(vfn(o, 0x68))(o, out);
}
__forceinline const Vector* owner_get_position(void* self, Vector* out) {
    void* const o = owner_ptr(self);
    return reinterpret_cast<Vector*(__thiscall*)(void*, Vector*)>(vfn(o, 0x64))(o, out);
}
__forceinline void owner_set_orientation(void* self, Quaternion* ret, Q4W q) {
    void* const o = owner_ptr(self);
    reinterpret_cast<void(__thiscall*)(void*, Quaternion*, Q4W)>(vfn(o, 0x60))(o, ret, q);
}
__forceinline void owner_set_position(void* self, void* ret, V3W v) {
    void* const o = owner_ptr(self);
    reinterpret_cast<void(__thiscall*)(void*, void*, V3W)>(vfn(o, 0x5C))(o, ret, v);
}
// A direct callee through its word-typed form (the same stack bytes, so the same ABI and cleanup), through a volatile
// code address so the optimizer cannot turn the words back into the callee's float parameters.
template <class F, class G> __forceinline F word_form(G* callee) {
    void* volatile address = reinterpret_cast<void*>(callee);
    return reinterpret_cast<F>(address);
}
// 004ACAC0 YawPitchRoll(out, yaw, pitch, roll) cdecl: a computed float argument is a float (fstp dword into its slot),
// a copied one a word.
__forceinline void ypr(Quaternion* out, float yaw, float pitch, float roll) { yaw_pitch_roll_004acac0(out, yaw, pitch, roll); }
__forceinline void ypr_wff(Quaternion* out, U32 yaw, float pitch, float roll) {
    word_form<void*(__cdecl*)(Quaternion*, U32, float, float)>(&yaw_pitch_roll_004acac0)(out, yaw, pitch, roll);
}
__forceinline void ypr_wwf(Quaternion* out, U32 yaw, U32 pitch, float roll) {
    word_form<void*(__cdecl*)(Quaternion*, U32, U32, float)>(&yaw_pitch_roll_004acac0)(out, yaw, pitch, roll);
}
__forceinline void qmul(Quaternion* out, const Quaternion* a, const Quaternion* b) { quaternion_multiply_004a9ac0(out, a, b); }
// 004A9D60 normalize(Quaternion* out, Quaternion q) cdecl, q by value (the words of 00638A60..00638A7D).
__forceinline const Quaternion* qnormalize(Quaternion* out, const Quaternion* q) {
    return word_form<Quaternion*(__cdecl*)(Quaternion*, Q4W)>(&quaternion_normalize_004a9d60)(out, words4(q));
}

} // namespace

// L3 t1: 004ACB90 Roll is defined in the camera_stick LD unit as C++ `long double w4cam::roll_004acb90(RollQuaternion)`
// (PLAN-v3 rule 6.9: this unit's 0063891B uses its ST0 result unrounded). Declared here, in this LD unit's own source,
// because long double may not appear in a project header (kmlint LD6). RollQuaternion is the quaternion {w,x,y,z} as
// four raw words (camera_stick.cpp), repeated here token for token.
namespace w4cam {
struct RollQuaternion { std::uint32_t w, x, y, z; };
long double roll_004acb90(RollQuaternion q);
}
namespace {
__forceinline w4cam::RollQuaternion roll_words(const Quaternion* q) { w4cam::RollQuaternion r; std::memcpy(&r, q, 16); return r; }
} // namespace

extern "C" {

// 006386A0 GetType.
std::uint32_t __fastcall kotor_fastcall_006386a0(void*, void*) { return 0x1071u; }

// 006386D0 non-deleting derived destructor: write the derived table, then the base dtor.
void __fastcall kotor_fastcall_006386d0(void* self, void*) {
    using Slots = kotor_scene3d_construct::BehaviorBaseSlots;
    *reinterpret_cast<const Slots**>(self) = &w4cam::camera_navigate_vtable_007515f4;
    behavior_destruct_0049fa70(self, nullptr);
}

// 006386B0 scalar deleting destructor: derived/base destructor, then free only for flags&1.
void* __fastcall kotor_fastcall_006386b0(void* self, void*, std::uint32_t flags) {
    kotor_fastcall_006386d0(self, nullptr);
    if ((flags & 1u) != 0u) live_free(self);
    return self;
}

// 006386E0 Message: six independent keyword/parse-float pairs, exact order and offsets.
void __fastcall kotor_fastcall_006386e0(void* self, void*, char* line) {
    static const char kTurn[] = "turn";
    static const char kTilt[] = "tilt";
    static const char kSpeed[] = "speed";
    static const char kRaise[] = "raise";
    static const char kStrafe[] = "strafe";
    static const char kRoll[] = "roll";

    if (kotor_stricmp_0070acaf(kTurn, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 4, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x14));
    if (kotor_stricmp_0070acaf(kTilt, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 4, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x18));
    if (kotor_stricmp_0070acaf(kSpeed, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 5, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x1c));
    if (kotor_stricmp_0070acaf(kRaise, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 5, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x20));
    if (kotor_stricmp_0070acaf(kStrafe, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 6, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x24));
    if (kotor_stricmp_0070acaf(kRoll, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 4, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x28));
}

// 006387E0 Control (thiscall, RET 4).
// L3 t1 (2026-10-24): PLAN-v3 writing rule - a value the original keeps on the x87 stack is a long double; each
// original fstp/fst dword is an explicit (float); no float-typed arithmetic; comparisons spelled from the original's
// fnstsw masks (ST0 against the operand: test 41h;jne == !(a > b), test 5;jp == !(a < b), test 44h;jp == !(a == b),
// test 1;jne == !(a >= b)). Migrated from cl /arch:IA32 /fp:strict to the clang LD recipe (coordinator ruling
// 2026-10-24). a2: the stack-slot floats are StoredFloat / reload (see the helpers), the copies are words.
// Frame of the original: F = esp after the four pushes; delta = [F+94h]; P = F+60h, Q = F+50h, R = F+40h,
// Pcopy = F+70h, v = F+1Ch (branch A) / F+28h (branch B), moveQ = F+80h.
std::uint32_t __fastcall kotor_fastcall_006387e0(void* self, void*, std::uint32_t raw_delta) {
    if (behavior_control_0049f270(self, nullptr, raw_delta) == 0) return 0;                  // 006387EE..006387F8
    float delta_word;                                                                       // the argument [esp+94h]
    std::memcpy(&delta_word, &raw_delta, sizeof delta_word);
    const long double delta = delta_word;                                                   // every use is fld/fmul [esp+94h]
    const std::int32_t toggle = kotor_input_cells_required::camera_toggle_008338e8;        // 00638807
    (void)owner_ptr(self);                                                                  // 0063880C (every vcall reloads it)

    if (toggle != 0) {
        // Branch A. 00638819..0063884B: fld [esi+N] ; fmul dword [k] ; fstp dword [esi+N], four times.
        ref<float>(self, 0x1c) = static_cast<float>(ld(ref<float>(self, 0x1c)) * kFive);
        ref<float>(self, 0x24) = static_cast<float>(ld(ref<float>(self, 0x24)) * kThree);
        ref<float>(self, 0x18) = static_cast<float>(ld(ref<float>(self, 0x18)) * kThreePointThreeThree);
        ref<float>(self, 0x14) = static_cast<float>(ld(ref<float>(self, 0x14)) * kTwoPointSixSeven);

        Quaternion P, Q, R;
        owner_get_orientation(self, &P);                                                    // 0063884E..00638850 (out P)
        ypr(&Q, static_cast<float>(delta * ref<float>(self, 0x14)), 0.0f, 0.0f);           // 00638853..0063886D
        qmul(&R, &Q, &P);                                                                   // 00638875..0063887B
        std::memcpy(&P, &R, 16);                                                            // 00638880..006388A3 (mov)
        ref<std::uint32_t>(self, 0x14) = 0;                                                 // 006388AA
        ypr(&Q, 0.0f, kMinusNinety, 0.0f);                                                  // 00638890..006388AD (push imm)
        qmul(&R, &P, &Q);                                                                   // 006388B2..006388C0

        std::memcpy(&P, &R, 16);                                                            // 006388CC..0063890C (mov)
        // 006388C5..00638918: fld delta ; fadd st,st ; fld [0075161C] ; fsub st,st(1) ; fmul [esi+2Ch] ; fxch ;
        // fadd [00743330] ; fmul [esi+28h] ; faddp st(1) ; fst dword [esi+28h] ; fstp dword [esi+2Ch].
        const long double two_delta = delta + delta;
        const long double s = (kLowpassC1 - two_delta) * ref<float>(self, 0x2c) + (two_delta + kLowpassC2) * ref<float>(self, 0x28);
        const float lowpass = static_cast<float>(s);
        ref<float>(self, 0x28) = lowpass;
        ref<float>(self, 0x2c) = lowpass;

        // 0063891B..0063892A: call Roll (R by value, ST0 unrounded) ; fsubr dword [esi+28h] ; fstp dword [esp] (roll argument).
        const long double roll = w4cam::roll_004acb90(roll_words(&R));
        ypr(&Q, 0.0f, 0.0f, static_cast<float>(ref<float>(self, 0x28) - roll));            // 0063892D..00638930
        qmul(&R, &P, &Q);                                                                   // 00638935..00638940

        std::memcpy(&P, &R, 16);                                                            // 0063894C..00638975 (mov)
        // 00638945..0063895F: fld delta ; fmul [esi+18h] ; fstp dword [esp+34h] (pb; reloaded at 00638981 / 006389A2).
        StoredFloat pb;
        pb.f = static_cast<float>(delta * ref<float>(self, 0x18));
        // 0063897C..006389A0: Pitch(&P) (ST0, unrounded) ; fsub pb ; fcom [-90] ; test 41h ; jne ; fcomp [90] ; test 5 ; jp.
        const long double diff = static_cast<long double>(quaternion_pitch_004aa020(&P)) - pb.value();
        if (diff > kMinusNinety && diff < kNinety) {
            ypr(&Q, 0.0f, static_cast<float>(-pb.value()), 0.0f);                          // 006389A2..006389B3 fld ; fchs ; fstp dword
            qmul(&R, &P, &Q);                                                               // 006389B8..006389C3
            std::memcpy(&P, &R, 16);                                                        // 006389C8..006389E8 (mov)
        }

        Quaternion Pcopy;
        std::memcpy(&Pcopy, &P, 16);                                                        // 006389F0..00638A1E (mov)
        ref<std::uint32_t>(self, 0x18) = 0;                                                 // 00638A14
        ypr(&Q, 0.0f, kNinety, 0.0f);                                                       // 006389FC..00638A25 (push imm)
        qmul(&R, &P, &Q);                                                                   // 00638A2A..00638A35
        std::memcpy(&P, &R, 16);                                                            // 00638A3A..00638A59 (mov)
        Quaternion N;
        const Quaternion* n = qnormalize(&N, &R);                                           // 00638A60..00638A80 (R by value)
        owner_set_orientation(self, &Q, words4(n));                                         // 00638A85..00638ACA (words through EAX)

        // 00638ACD..00638B07: fld [esi+24h] ; fst dword [esp+1Ch] (v.x) ; v.z = 0 ; fld [esi+1Ch] ; fchs ;
        // fstp dword [esp+20h] (v.y) ; [24h] = [1Ch] = 0 ; fcomp 0 on the loaded strafe (test 44h ; jp) ;
        // fld v.y ; fcomp 0 (test 44h ; jnp -> return 1).
        const long double strafe = ld(ref<float>(self, 0x24));
        Vector v;
        v.x = static_cast<float>(strafe);
        v.z = 0.0f;
        v.y = static_cast<float>(-ld(ref<float>(self, 0x1c)));
        ref<std::uint32_t>(self, 0x24) = 0;
        ref<std::uint32_t>(self, 0x1c) = 0;
        if (strafe == kZero && reload(v.y) == kZero) return 1;

        Vector rot;
        const Vector* r = quaternion_rotate_004a9bb0(&rot, &Pcopy, &v);                     // 00638B0D..00638B1C (read through EAX)
        // 00638B21..00638B64: fld delta ; fmul [rot.N] ; fstp dword [esp+34h/38h/3Ch] (dv, copied into v, reloaded at 00638B72..00638B93).
        StoredFloat dvx, dvy, dvz;
        dvx.f = static_cast<float>(delta * r->x);
        dvy.f = static_cast<float>(delta * r->y);
        dvz.f = static_cast<float>(delta * r->z);
        Vector pb_pos;
        const Vector* pos = owner_get_position(self, &pb_pos);                              // 00638B6A..00638B6F (EAX)
        // 00638B72..00638BAD: fld dv ; fadd [pos.N] ; fstp dword (the SetPosition argument words).
        Vector np;
        np.x = static_cast<float>(dvx.value() + pos->x);
        np.y = static_cast<float>(dvy.value() + pos->y);
        np.z = static_cast<float>(dvz.value() + pos->z);
        owner_set_position(self, &Q, words3(&np));                                          // 00638BB0..00638BB5
        return 1;
    }

    // Branch B.
    Quaternion O;
    StoredFloat yaw, pitch;
    yaw.f = static_cast<float>(quaternion_yaw_004a9f40(owner_get_orientation(self, &O)));   // 00638BCA..00638BDA fstp dword [esp+14h]
    pitch.f = static_cast<float>(quaternion_pitch_004aa020(owner_get_orientation(self, &O))); // 00638BDE..00638BF4 fstp dword [esp+10h]

    long double sign = kOne;                                                                // 00638C05 mov dword [esp+18h], 3F800000
    {
        const Quaternion* o = owner_get_orientation(self, &O);                              // 00638C00..00638C0D (EAX)
        // 00638C10..00638C67: the inline up-axis test on the x87 stack. a = 2(zx + wy), b = 2(zy - wx),
        // zdir = 1 - 2(y*y + x*x); fcomp 0 on ((b + a) * 0.0f) + zdir ; test 1 ; jne keeps +1, else sign = -1.
        const long double zx_wy = ld(o->z) * o->x + ld(o->w) * o->y;
        const long double a = zx_wy + zx_wy;
        const long double zy_wx = ld(o->z) * o->y - ld(o->w) * o->x;
        const long double b = zy_wx + zy_wx;
        const long double yy_xx = ld(o->y) * o->y + ld(o->x) * o->x;
        const long double zdir = kOne - (yy_xx + yy_xx);
        const long double cmp = (b + a) * kZero + zdir;
        if (cmp >= kZero) sign = -kOne;                                                     // 00638C69 mov dword [esp+18h], BF800000
    }

    // 00638C71..00638C82: fld delta ; fmul [esi+14h] ; [14h] = 0 ; fadd [esp+14h] (yaw) ; fstp dword [esp+14h].
    const long double dy = delta * ref<float>(self, 0x14);
    ref<std::uint32_t>(self, 0x14) = 0;
    StoredFloat yaw2;
    yaw2.f = static_cast<float>(dy + yaw.value());

    // 00638C86..00638CB3: fld sign ; fmul [esi+18h] ; fmul delta ; fadd [esp+10h] (pitch) ; fld 0 ; fcomp st(1) ;
    // test 41h ; jne -> fstp dword [esp+10h] (p), else fstp st(0) ; mov dword [esp+10h], 0.
    const long double p_x87 = (sign * ref<float>(self, 0x18)) * delta + pitch.value();
    StoredFloat p;
    if (kZero > p_x87) p.set_word(0);
    else p.f = static_cast<float>(p_x87);
    ref<std::uint32_t>(self, 0x18) = 0;                                                     // 00638CBE

    // 00638CB7..00638D08: GetOrientation ; call Roll (by value) ; fstp dword [esp+0Ch] (roll) ; fld p ; fsub [0073E40C] ;
    // fstp dword [esp] ; push yaw2 (the stored word) -> YawPitchRoll(yaw2, p - 90, roll).
    const long double roll = w4cam::roll_004acb90(roll_words(owner_get_orientation(self, &O)));
    Quaternion moveQ;
    ypr_wff(&moveQ, yaw2.word(), static_cast<float>(p.value() - kNinety), static_cast<float>(roll));

    // 00638D0D..00638D46: fld [esi+24h] ; fst dword [esp+28h] (v.x) ; v.z = 0 ; fld sign ; fmul [esi+1Ch] ;
    // fstp dword [esp+2Ch] (speed = v.y) ; strafe fcomp 0 (test 44h ; jp) ; fld speed ; fcomp 0 (test 44h ; jnp -> skip).
    const long double strafe = ld(ref<float>(self, 0x24));
    Vector v;
    v.x = static_cast<float>(strafe);
    v.z = 0.0f;
    v.y = static_cast<float>(sign * ref<float>(self, 0x1c));
    if (!(strafe == kZero && reload(v.y) == kZero)) {
        Vector rot;
        const Vector* r = quaternion_rotate_004a9bb0(&rot, &moveQ, &v);                     // 00638D4C..00638D5E (EAX)
        // 00638D63..00638DA3: dv = delta * rot (fstp dword x3, copied into v) ; 00638DB4..00638DEF: np = dv + position.
        StoredFloat dvx, dvy, dvz;
        dvx.f = static_cast<float>(delta * r->x);
        dvy.f = static_cast<float>(delta * r->y);
        dvz.f = static_cast<float>(delta * r->z);
        Vector pb_pos;
        const Vector* pos = owner_get_position(self, &pb_pos);                              // 00638DA7..00638DB1 (EAX)
        Vector np;
        np.x = static_cast<float>(dvx.value() + pos->x);
        np.y = static_cast<float>(dvy.value() + pos->y);
        np.z = static_cast<float>(dvz.value() + pos->z);
        Quaternion ret;
        owner_set_position(self, &ret, words3(&np));                                        // 00638DF2..00638DF7
    }

    // 00638DFA..00638E39: GetOrientation ; call Roll ; fstp dword [esp+0Ch] ; push p ; push yaw2 (the stored words)
    // -> YawPitchRoll(yaw2, p, roll2) ; SetOrientation with its words.
    const long double roll2 = w4cam::roll_004acb90(roll_words(owner_get_orientation(self, &O)));
    Quaternion finalQ;
    ypr_wwf(&finalQ, yaw2.word(), p.word(), static_cast<float>(roll2));
    Quaternion ret;
    owner_set_orientation(self, &ret, words4(&finalQ));                                     // 00638E3E..00638E5E
    return 1;
}

} // extern "C"

// The one external typed table definition. Relocations point to the five bodies and
// the three existing 0471 helpers; the table is retained by the Navigate ctor import.
namespace w4cam {
const kotor_scene3d_construct::BehaviorBaseSlots camera_navigate_vtable_007515f4 = {
    kotor_fastcall_006386b0,
    behavior_void_return_004015a0,
    kotor_fastcall_006387e0,
    kotor_fastcall_006386e0,
    kotor_fastcall_006386a0,
    behavior_null_return_0063e7f0,
    behavior_return_zero_0063e7f0,
    behavior_return_zero_0063e7f0,
};
} // namespace w4cam
