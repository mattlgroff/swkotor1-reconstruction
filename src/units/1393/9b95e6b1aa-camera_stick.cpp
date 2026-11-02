// Lane W4 G14: CSWCameraOnAStick (original vtable 0075165C, size 138h). Transcribed from dis/<ADDR>.asm.
// Field map (from the ctor 00639FC0 and the Control_* bodies):
//   +00 vtable  +04/+08/+0C CAurBehavior dependency list  +10 owner (camera Gob view)  +14 SafePointer target part
//   +18 SafePointer combat target  +1C camera AurObject  +28..+34 desired orientation {w,x,y,z}
//   +38..+40 / +44..+4C / +50..+58 last/target/reset positions (+44 = FLT_MAX until initialised)
//   +5C style row  +60/+64 snap flags  +68 following body  +6C head height  +80 look-at object id
//   +84 view-angle-lock flag  +88/+8C style distance/view angle  +90/+94 shake yaw/pitch  +98  +9C interp speed
//   +A0 interp max  +AC  +B0 free-look mouse state  +B4..+103 CSWRK4SplitAcceleration  +100 mouse turn  +104 tilt
//   +108 pitch  +10C keyboard turn  +110 DISTANCE  +114 SPEED  +118  +11C PITCH  +120 HEIGHT  +124
//   +128..+130  +134 TILTSPEED
#include "4644e3d982-camera_stick.hpp"
// L3 MATH-TRIG-CPP t1 a2 (2026-10-26): the whole unit is built as a long double (LD) unit - recipe + /clang:-mno-sse
// /clang:-ffp-model=strict /clang:-mlong-double-80 (PLAN-v3 1.1) - so every FP function of it is written in the PLAN-v3
// writing rule (section 6), not only the t1 functions:
//   * a value the original keeps on the x87 stack is a long double; every original fstp/fst dword is an explicit (float)
//     at that point, and a value used again after such a store is the long double initialised from the (float)
//     conversion (exact), so the conversion is the only dword round trip and no x87 value is spilled narrower than
//     80 bits (spill gate S1);
//   * no float arithmetic; comparisons spelled from the original's fnstsw mask (ST0 against the operand: status
//     > 00, < 01, = 40, unordered 45):  test 44h;jp taken unless equal; test 05h;jp taken unless less;
//     test 41h;jne taken unless greater; test 01h;jne taken if less or unordered;
//   * math through the standard names of kotor_math.h (bound to L1); <cmath> is not included;
//   * a2: the original copies vectors, quaternions and float arguments it does not compute with integer moves (raw
//     bits, a signalling NaN is kept). clang's i386 ABI expands a by-value struct of floats into float parameters,
//     which it loads and stores with fld/fstp dword (that quiets a signalling NaN and spilled 4-byte x87 values across
//     calls). Here every such copy is a word copy: by-value vectors / quaternions are passed as the word structs V3W /
//     Q4W (same 12 / 16 stack bytes) through an explicitly typed call of the same callee, and by-value struct
//     arguments of this unit's own functions are read as words from their stack slots (KOTOR_ARGS: the first stack
//     argument is the word above the return address), as the a2 reference anim_contested 00438AF0 does;
//   * the recipe is /Ob0: plain `inline` helpers are calls there, so every helper here is __forceinline (field access
//     is a direct memory operand, as in the original, and no float travels through a helper call).
#include "../../math/kotor_math.h"
#include <intrin.h>   // _AddressOfReturnAddress (the by-value struct arguments, a2)

namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }   // (0,0,1) (0532 world-los cells)
extern "C" float x_axis_007a1998[3];                                     // (1,0,0) (0542 core46 math)

using namespace w4;
using namespace w4cam;

// The stack arguments of the function this expands in: [esp+4] at its entry (a2; see the header comment).
#define KOTOR_ARGS() (static_cast<const U8*>(_AddressOfReturnAddress()) + 4)

namespace w4cam {
float tilt_up_007a2420 = 30.0f, tilt_down_007a2424 = 30.0f, rotation_007a2428 = 90.0f;
// x87 `_pow` for the non-LD callers of other units (camera_collision 006402F0 .. : `double x87_pow(double, double)`,
// camera.hpp). a2: the checkpoint body held `call genuine_x87_pow_006fc1f0` inside MS inline asm (clang-cl does not
// treat EAX/ECX/EDX as clobbered by such a call) and bound the project wrapper of the original _CIpow. It is now the
// L1 C pow (the transcribed VC7 C entry 006FC1B0, kotor_math.h): the callers pass doubles, which the original _CIpow
// entry stores to qwords at once (006FC230 fstp qword / 006FC233 fst qword) before the shared _pow_internal 006FC24D,
// exactly the qwords the C entry receives on its stack, and the C entry returns that same ST0 with no conversion, so
// no long double -> double round trip exists in this unit (spill gate S3). Value-gated against the original C pow
// 006FC1B0 (specgen/camera_stick.py).
double x87_pow(double base, double exponent) { return pow(base, exponent); }
// 004ACB90 Roll's argument, a quaternion {w,x,y,z} as four raw words (a2: copied with integer moves, as the original's
// callers do; C++ linkage, declared again in camera_navigate.cpp and module_camera.cpp, the LD callers).
struct RollQuaternion { U32 w, x, y, z; };
long double roll_004acb90(RollQuaternion q);
}

namespace {
const float k_zero = 0.0f, k_one = 1.0f;
const float k_one_and_half_0073f414 = 1.5f;
const float k_half_deg_to_rad_00741bb4 = 0.008726646192371845f;   // bits 3C0EFA35
const float k_deg_to_rad_007415f8 = 0.01745329238474369f;          // bits 3C8EFA35
const float k_minus_deg_to_rad_00744100 = -0.01745329238474369f;   // bits BC8EFA35
const float k_rad_to_deg_00742ef8 = 57.2957801818847656f;          // bits 42652EE1
const float k_flt_max_0074c468 = 3.40282346638528859812e+38f;     // bits 7F7FFFFF
const float k_360_00741868 = 360.0f, k_270_0073e404 = 270.0f, k_90_0073e40c = 90.0f, k_180_0073e408 = 180.0f;
const float k_minus_180_0074f4f0 = -180.0f;
const float k_dist_scale_0074ea10 = 0.013333333656191826f;        // bits 3C5A740E
const float k_three_00740c84 = 3.0f, k_five_00743264 = 5.0f, k_half_0073e9ac = 0.5f;
const double k_one_double_0073ede8 = 1.0, k_zero_double_0073edf8 = 0.0, k_cos30_00751628 = 0.8659999966621399;
inline float c_bits(U32 b) { return bits_f(b); }
const float k_wrap_hi_lo_0075163c = c_bits(0x4333FFF9u);           // 179.99999
const float k_wrap_hi_hi_00751638 = c_bits(0x43340007u);           // 180.00002
const float k_wrap_lo_hi_00751634 = c_bits(0xC333FFF9u);
const float k_wrap_lo_lo_00751630 = c_bits(0xC3340007u);

// a2: raw-word forms of a Vector / Quaternion (the original's mov copies).
struct V3W { U32 x, y, z; };
struct Q4W { U32 w, x, y, z; };
static_assert(sizeof(V3W) == sizeof(W4Vector) && sizeof(Q4W) == sizeof(W4CamQuaternion), "word forms");
__forceinline V3W words3(const void* p) { V3W v; std::memcpy(&v, p, 12); return v; }
__forceinline Q4W words4(const void* p) { Q4W q; std::memcpy(&q, p, 16); return q; }
__forceinline U32 word(const void* p) { U32 w; std::memcpy(&w, p, 4); return w; }
// An original `fstp dword` into an argument slot written before further calls: the float is stored to a volatile slot
// and later pushed as that slot's word, so no x87 register lives across those calls (a plain float local, or its word
// through memcpy, is folded back into a float that clang spills as 4 bytes: measured in the a2 listing).
struct StoredFloat {
    volatile float f;
    __forceinline U32 word() const { return *reinterpret_cast<const volatile U32*>(&f); }
};
__forceinline V3W up_axis() { return words3(&kotor_world_los::up_007a19b0); }   // [007A19B0..B8] mov
__forceinline V3W x_axis() { return words3(x_axis_007a1998); }                   // [007A1998..A0] mov
// Field access: a direct memory operand ([esi+8Ch] ...), never a call under /Ob0.
template <class T> __forceinline T& at(void* o, U32 off) { return *reinterpret_cast<T*>(static_cast<U8*>(o) + off); }
__forceinline float* fp(void* o, U32 off) { return reinterpret_cast<float*>(static_cast<U8*>(o) + off); }
__forceinline long double ld(const float& f) { return f; }                       // fld dword
// A float this function stored (fstp dword) into a buffer that a call reads or rewrites before the original reloads
// it: read back as its word (volatile, so it is a mov) and only then loaded as a float. The value is the same bits the
// original's fld / fmul dword reads; the listing then shows the store and the word read, not a second same-slot
// fstp/fld dword pair besides the one (float) conversion of that original store (spill gate S3).
__forceinline long double reload(const float& f) {
    const U32 w = *reinterpret_cast<const volatile U32*>(&f);
    float g; std::memcpy(&g, &w, 4); return g;
}
__forceinline void* owner(void* s) { return at<void*>(s, 0x10); }
__forceinline void* target(void* s) { return at<void*>(s, 0x14); }
__forceinline void* vfn(void* object, U32 slot) { return *reinterpret_cast<void**>(*static_cast<U8**>(object) + slot); }
// a2: a direct callee called through its word-typed form (V3W / Q4W / U32 in place of a by-value Vector / Quaternion /
// copied float: the same stack bytes, so the same callee ABI and cleanup), like a vtable slot: through the code address.
// The address goes through a volatile local, so the optimizer cannot re-type the call to the callee's own prototype
// (it would turn the word arguments back into floats copied by fld/fstp dword, measured in the a2 listing).
template <class F, class G> __forceinline F word_form(G* callee) {
    void* volatile address = reinterpret_cast<void*>(callee);
    return reinterpret_cast<F>(address);
}
// Gob-view virtuals on owner/target (original vtable 00741988 layout): +5C SetPosition(ret*, Vector) RET 10h,
// +60 SetOrientation(ret*, Quaternion) RET 14h, +64 GetPosition(ret*) RET 4, +68 GetOrientation(ret*) RET 4,
// +58 SetBehavior RET 4.
__forceinline W4Vector* gob_get_position(void* gob, W4Vector* out) {
    return reinterpret_cast<W4Vector*(__thiscall*)(void*, W4Vector*)>(vfn(gob, 0x64))(gob, out);
}
__forceinline W4CamQuaternion* gob_get_orientation(void* gob, W4CamQuaternion* out) {
    return reinterpret_cast<W4CamQuaternion*(__thiscall*)(void*, W4CamQuaternion*)>(vfn(gob, 0x68))(gob, out);
}
__forceinline void gob_set_position(void* gob, V3W v) {
    W4Vector r; reinterpret_cast<void(__thiscall*)(void*, W4Vector*, V3W)>(vfn(gob, 0x5C))(gob, &r, v);
}
__forceinline void gob_set_orientation(void* gob, Q4W q) {
    W4CamQuaternion r; reinterpret_cast<void(__thiscall*)(void*, W4CamQuaternion*, Q4W)>(vfn(gob, 0x60))(gob, &r, q);
}
// 004ACAC0 YawPitchRoll(out, yaw, pitch, roll) cdecl: a float the original computes is stored by fstp dword into its
// argument slot (the float parameter); one it copies is pushed as a word (ypr_words).
__forceinline void ypr(W4CamQuaternion* out, float yaw, float pitch, float roll) { yaw_pitch_roll_004acac0(out, yaw, pitch, roll); }
__forceinline void ypr_words(W4CamQuaternion* out, U32 yaw, U32 pitch, U32 roll) {
    word_form<void*(__cdecl*)(void*, U32, U32, U32)>(&yaw_pitch_roll_004acac0)(out, yaw, pitch, roll);
}
// 004AC960 Quaternion::Quaternion(Vector axis, float angle) (thiscall, RET 10h): the axis words copied (mov), the angle
// stored by fstp dword.
__forceinline void axis_angle(W4CamQuaternion* out, V3W axis, float angle) {
    word_form<void*(__fastcall*)(W4CamQuaternion*, void*, V3W, float)>(&quaternion_from_axis_angle_004ac960)(out, nullptr, axis, angle);
}
__forceinline void qmul(W4CamQuaternion* out, const W4CamQuaternion* a, const W4CamQuaternion* b) { quaternion_multiply_004a9ac0(out, a, b); }
// The quaternion "z axis" expression inlined at 0063A124.. / 00638FF5..: {2(zx+yw), 2(zy-xw), 1-2(y^2+x^2)}; each
// sum is doubled by fadd st,st and stored by fstp dword.
__forceinline void quat_zdir_inline(W4Vector* v, const W4CamQuaternion& q) {
    const long double a = ld(q.z) * q.x + ld(q.y) * q.w;
    v->x = static_cast<float>(a + a);
    const long double b = ld(q.z) * q.y - ld(q.x) * q.w;
    v->y = static_cast<float>(b + b);
    const long double c = ld(q.y) * q.y + ld(q.x) * q.x;
    v->z = static_cast<float>(k_one - (c + c));
}
// The quaternion "y axis" expression inlined at 0063A205.. / 006390E4..: {2(yx-zw), 1-2(z^2+x^2), 2(zy+xw)}.
__forceinline void quat_ydir_inline(W4Vector* v, const W4CamQuaternion& q) {
    const long double a = ld(q.y) * q.x - ld(q.z) * q.w;
    v->x = static_cast<float>(a + a);
    const long double b = ld(q.z) * q.z + ld(q.x) * q.x;
    v->y = static_cast<float>(k_one - (b + b));
    const long double c = ld(q.z) * q.y + ld(q.x) * q.w;
    v->z = static_cast<float>(c + c);
}
void safe_pointer_clear(void* slot) {                                                  // 0063A07A..0063A08A / 0063A360
    void* object = lp(slot, 0);
    if (object != nullptr) exo_array_list_remove_00445080(static_cast<U8*>(object) + 4, nullptr, slot);
    sw(slot, 0, 0);
}
}  // namespace

namespace w4cam {
// Original vtable 0075165C (8 slots; slots 14h..1Ch return zero, 04h is void_return).
extern "C" void __fastcall behavior_void_return_004015a0(void*, void*);              // 0471 behaviour tables
extern "C" std::uint32_t __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);   // 0832 m15e tables (return_zero)
void* const camera_stick_vtable_0075165c[8] = {
    reinterpret_cast<void*>(&kotor_fastcall_0063ad40), reinterpret_cast<void*>(&behavior_void_return_004015a0),
    reinterpret_cast<void*>(&kotor_fastcall_0063bcb0), reinterpret_cast<void*>(&kotor_fastcall_00637a30),
    reinterpret_cast<void*>(&kotor_fastcall_0063a310), reinterpret_cast<void*>(&kotor_m15e_slot010_zero_0063e7f0),
    reinterpret_cast<void*>(&kotor_m15e_slot010_zero_0063e7f0), reinterpret_cast<void*>(&kotor_m15e_slot010_zero_0063e7f0)};
}

// 0049F1F0 CAurBehavior::RemoveDependency(CAurObject*) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_0049f1f0(void* behavior, void*, void* object) {
    const I32 count = li(behavior, 8);                                                     // 0049F1F4
    I32 i = 0;
    for (; i < count; ++i) {                                                               // 0049F1FB..0049F215
        void* record = lp(lp(behavior, 4), static_cast<U32>(i) * 4u);
        if (lp(record, 0) == object) break;                                                // 0049F208..0049F20D
    }
    if (i >= count) return;
    void* record = lp(lp(behavior, 4), static_cast<U32>(i) * 4u);                          // 0049F21E
    if (record != nullptr) {                                                               // 0049F221..0049F223
        if (void* target_object = lp(record, 0))                                           // 0049F225..0049F229
            exo_array_list_remove_00445080(static_cast<U8*>(target_object) + 4, nullptr, record); // 0049F22B..0049F22F
        live_free(record);                                                                 // 0049F234..0049F23A
    }
    const I32 last = li(behavior, 8) - 1;                                                  // 0049F23D..0049F240
    sw(behavior, 8, static_cast<U32>(last));                                               // 0049F247
    for (I32 j = i; j < last; ++j) {                                                       // 0049F243..0049F262
        U8* array = static_cast<U8*>(lp(behavior, 4));
        sw(array, static_cast<U32>(j) * 4u, lw(array, static_cast<U32>(j + 1) * 4u));
    }
}

namespace w4cam {
// 004ACB90 Roll(Quaternion) (cdecl; the quaternion is passed by value, 16 bytes, and the original overwrites that
// argument in place). Returns the unrounded ST0 value: PLAN-v3 rule 6.9 (ret80 census: caller 0063891B
// CSWCameraNavigate::Control uses it unrounded, `call 0x4acb90 ; fsubr dword ptr [esi+28h]`). C++ linkage on purpose
// (long double may not appear in an extern "C" signature, kmlint LD1); the LD callers declare it in their own .cpp.
long double roll_004acb90(RollQuaternion arg) {
    W4CamQuaternion* const q = reinterpret_cast<W4CamQuaternion*>(&arg);                     // the argument itself
    // 004ACB9D..004ACBB3: Yaw(&q) (ST0) * [00744100] -> fstp dword [esp] (the angle argument).
    const float yaw = static_cast<float>(static_cast<long double>(quaternion_yaw_004a9f40(q)) * k_minus_deg_to_rad_00744100);
    W4CamQuaternion a, p;
    axis_angle(&a, up_axis(), yaw);                                                        // 004ACBA8..004ACBCC
    qmul(&p, &a, q);                                                                       // 004ACBD1..004ACBD7
    std::memcpy(q, &p, 16);                                                                // 004ACBDC..004ACC00 (mov)
    // 004ACC04..004ACC1B: Pitch(&q) (ST0) * [00744100] -> fstp dword [esp].
    const float pitch = static_cast<float>(static_cast<long double>(quaternion_pitch_004aa020(q)) * k_minus_deg_to_rad_00744100);
    W4CamQuaternion b, r;
    axis_angle(&b, x_axis(), pitch);                                                       // 004ACC0F..004ACC35
    qmul(&r, &b, q);                                                                       // 004ACC3A..004ACC40
    // 004ACC55..004ACC96 on the product {w,x,y,z} = [0],[4],[8],[C]:
    //   a = (z*x - y*w) doubled                fld [C]; fmul [4]; fld [8]; fmul [0]; fsubp st(1); fadd st,st
    //   b = 1.0f - (y*y + z*z) doubled         fld [8]; fmul [8]; fld [C]; fmul [C]; faddp st(1); fadd st,st; fsubr [0073D764]
    //   atan2(-a, b) * 57.29578f              fxch; fchs; fxch; fpatan; fmul dword [00742EF8]
    const long double t = static_cast<long double>(r.z) * r.x - static_cast<long double>(r.y) * r.w;
    const long double ta = t + t;
    const long double u = static_cast<long double>(r.y) * r.y + static_cast<long double>(r.z) * r.z;
    const long double tb = k_one - (u + u);
    return atan2(-ta, tb) * k_rad_to_deg_00742ef8;
}
}  // namespace w4cam

// The project's existing extern "C" name for 004ACB90 (`double kotor_cdecl_004acb90(W4CamQuaternion)`, camera_stick.hpp),
// kept for the non-LD callers: w5_player_control-changed 00679940 (`fstps` at once), claude_dialogue_camera 0062C030,
// claude_dialogue_camera2 006306E0 and 006BCF50, claude_vm_actions_game 0062E900 (each spills the ST0 return as a tbyte
// and reloads it only into `fstps` before any arithmetic), minigame_t4_track 006DB180 (`fstps` at once); the LD units
// (camera_stick, camera_navigate, module_camera) call w4cam::roll_004acb90 directly. Every original caller of those
// sites stores Roll's result with fstp dword at once (ret80 census: STORE32). a2: the value is rounded to float here,
// once, and returned as that float's (exact) double: for every such caller (float)(double)(float)v == (float)v, so the
// stored value is unchanged, and the only round trip is the function's own float conversion right before `ret`
// (spill gate S3 exemption), with no qword conversion. The argument words are copied as words (the original's mov).
// Integration constraint: the a2 camera_navigate object (Roll's arithmetic caller 0063891B, LD) replaces the c68 cl
// object, which used this adapter's double unrounded (fsubl).
extern "C" double __cdecl kotor_cdecl_004acb90(W4CamQuaternion) {
    w4cam::RollQuaternion q;
    std::memcpy(&q, KOTOR_ARGS(), 16);                                                     // [esp+4]..[esp+10h]
    return static_cast<float>(w4cam::roll_004acb90(q));
}

// 00639FC0 CSWCameraOnAStick::CSWCameraOnAStick(CAurObject* owner, CAurObject* part) (thiscall, RET 8).
extern "C" void* __fastcall kotor_fastcall_00639fc0(void* s, void*, void* owner_object, void* part) {
    sw(s, 8, 0); sw(s, 0x0C, 0); sw(s, 4, 0); sw(s, 0x10, ad(owner_object));             // 0049F110 CAurBehavior::CAurBehavior
    sw(s, 0, ad(camera_stick_vtable_0075165c));                                            // 00639FEE
    sw(s, 0x14, 0); sw(s, 0x18, 0);                                                        // 00639FF8..00639FFE
    sw(s, 0x34, 0); sw(s, 0x30, 0); sw(s, 0x2C, 0); sw(s, 0x28, 0x3F800000u);              // 0063A001..0063A00A
    for (U32 off = 0x38; off <= 0x58; off += 4) sw(s, off, 0);                             // 0063A011..0063A02C
    sw(s, 0x74, 0); sw(s, 0x78, 0); sw(s, 0x7C, 0);                                        // 0063A03A..0063A040
    kotor_fastcall_006d10b0(static_cast<U8*>(s) + 0xB4, nullptr);                          // 0063A02F..0063A043
    sw(s, 0x128, 0); sw(s, 0x12C, 0); sw(s, 0x130, 0);                                     // 0063A050..0063A05C
    sw(s, 0x1C, ad(owner_object));                                                         // 0063A062
    behavior_add_dependency_0049f130(s, nullptr, part);                                    // 0063A065..0063A068
    safe_pointer_assign_00447fa0(static_cast<U8*>(s) + 0x14, nullptr, part);               // 0063A06D..0063A075
    safe_pointer_clear(static_cast<U8*>(s) + 0x18);                                        // 0063A07A..0063A08A
    sw(s, 0x5C, 0xFFFFFFFFu);                                                              // 0063A08D
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                         // 0063A094..0063A09D
    kotor_fastcall_00638070(s, nullptr, li(lp(module, 0x48), 0xB8));                       // 0063A0A2..0063A0AE
    sw(s, 0x128, 0); sw(s, 0x12C, 0); sw(s, 0x130, 0);                                     // 0063A0B3..0063A0E9
    sw(s, 0x108, lw(s, 0x11C));                                                            // 0063A0C5/0063A0EF
    sw(s, 0x124, 0); sw(s, 0x118, 0x42C80000u); sw(s, 0x104, 0); sw(s, 0x100, 0); sw(s, 0x10C, 0); // 0063A0FC..0063A118
    W4CamQuaternion q;
    gob_get_orientation(target(s), &q);                                                    // 0063A0F5..0063A121
    {
        W4Vector v, n;
        quat_zdir_inline(&v, q);                                                           // 0063A124..0063A180
        vector_normalized_004ab180(&n, &v);                                                // 0063A184
        const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&n));                 // 0063A189..0063A18F fstp dword
        W4CamQuaternion o;
        ypr_words(&o, word(&yaw), at<U32>(s, 0x11C), 0);                                   // 0063A12C..0063A19B (pitch: mov ecx,[esi+11Ch] ; push ecx)
        std::memcpy(fp(s, 0x28), &o, 16);                                                  // 0063A1A0..0063A1B9
        gob_set_orientation(owner(s), words4(&o));                                         // 0063A1B6..0063A1DE
    }
    W4Vector p;
    gob_get_position(target(s), &p);                                                       // 0063A1E1..0063A1EB
    p.z = static_cast<float>(ld(p.z) + k_one_and_half_0073f414);                           // 0063A1EE..0063A201
    W4Vector w;
    quat_ydir_inline(&w, q);                                                               // 0063A205..0063A24F
    vector_subtract_assign_004a9770(&p, nullptr, &w);                                      // 0063A253
    gob_set_position(owner(s), words3(&p));                                                // 0063A258..0063A27B
    std::memcpy(fp(s, 0x44), &p, 12);                                                      // 0063A27E..0063A2A4
    std::memcpy(fp(s, 0x50), &p, 12);
    std::memcpy(fp(s, 0x38), &p, 12);
    sw(s, 0x64, 0); sw(s, 0x84, 0); sw(s, 0x90, 0); sw(s, 0x94, 0); sw(s, 0xA4, 0); sw(s, 0xAC, 0); // 0063A2A7..0063A2C2
    sw(s, 0x60, 1);                                                                        // 0063A2C8
    sw(s, 0x44, 0x7F7FFFFFu);                                                              // 0063A2CF
    sw(s, 0x80, 0x7F000000u);                                                              // 0063A2D5
    sw(s, 0x98, 0xBF800000u);                                                              // 0063A2DF
    sw(s, 0xB0, static_cast<U32>(kotor_input_cells_required::word_008338f0));              // 0063A2E9..0063A2F0
    return s;
}

// 0063A320 CSWCameraOnAStick::~CSWCameraOnAStick (thiscall).
extern "C" void __fastcall kotor_fastcall_0063a320(void* s, void*) {
    sw(s, 0, ad(camera_stick_vtable_0075165c));                                            // 0063A326
    if (void* o = lp(s, 0x18)) exo_array_list_remove_00445080(static_cast<U8*>(o) + 4, nullptr, static_cast<U8*>(s) + 0x18); // 0063A32C..0063A336
    if (void* o = lp(s, 0x14)) exo_array_list_remove_00445080(static_cast<U8*>(o) + 4, nullptr, static_cast<U8*>(s) + 0x14); // 0063A33B..0063A349
    behavior_destruct_0049fa70(s, nullptr);                                                // 0063A351 jmp
}
// 0063AD40 deleting destructor (slot 00h, RET 4).
extern "C" void* __fastcall kotor_fastcall_0063ad40(void* s, void*, U32 flags) {
    kotor_fastcall_0063a320(s, nullptr);                                                   // 0063AD43
    if (flags & 1) live_free(s);                                                           // 0063AD48..0063AD50
    return s;
}
// 0063A310 GetType (slot 10h).
extern "C" U32 __fastcall kotor_fastcall_0063a310(void*, void*) { return 0x106A; }
// 0063A360 ClearCombatCamera.
extern "C" void __fastcall kotor_fastcall_0063a360(void* s, void*) { safe_pointer_clear(static_cast<U8*>(s) + 0x18); }
// 0063F600 GetPosition(Vector* out) (RET 4): camera AurObject (+1Ch) GetPosition or zero.
extern "C" W4Vector* __fastcall kotor_fastcall_0063f600(void* s, void*, W4Vector* out) {
    void* camera = lp(s, 0x1C);                                                            // 0063F600
    if (camera == nullptr) { sw(out, 0, 0); sw(out, 4, 0); sw(out, 8, 0); return out; }   // 0063F61A..0063F626
    gob_get_position(camera, out);                                                         // 0063F609..0063F611
    return out;
}
// 0063F630 SetPosition(Vector) (RET 0Ch): the vector words copied from the argument (0063F638..0063F650 mov).
extern "C" void __fastcall kotor_fastcall_0063f630(void* s, void*, W4Vector) {
    if (void* camera = at<void*>(s, 0x1C)) gob_set_position(camera, words3(KOTOR_ARGS())); // 0063F630..0063F658
}
namespace w4cam { const std::uint32_t camera_settings_0075182c = 0; }   // .rdata 0075182C (DWORD 0)
// 0063BA80 SaveCameraSettings(settings*) (RET 4).
extern "C" void __fastcall kotor_fastcall_0063ba80(void* s, void*, void* settings) {
    sw(settings, 0, 1);                                                                    // 0063BA85
    sw(settings, 4, camera_settings_0075182c);                                             // 0063BA8B..0063BA93 (.rdata 0)
    sw(settings, 8, lw(s, 0x1C));                                                          // 0063BA96..0063BA99
    safe_pointer_assign_00447fa0(static_cast<U8*>(settings) + 0x0C, nullptr, lp(s, 0x14)); // 0063BA9C..0063BAA3
    sw(settings, 0x14, lw(s, 0x68));                                                       // 0063BAA8..0063BAAB
    sw(settings, 0x10, lw(s, 0x5C));                                                       // 0063BAAE..0063BAB1
    sw(settings, 0x18, lw(s, 0x6C));                                                       // 0063BAB4..0063BAB8
    sw(settings, 0x1C, 0xFFFFFFFFu);                                                       // 0063BABB
}

// 00637A30 Message(const char*) (slot 0Ch, RET 4).
extern "C" void __fastcall kotor_fastcall_00637a30(void* s, void*, const char* message) {
    static const char m_fDistance_007437dc[] = "m_fDist";
    static const char m_fSpeed_007437d0[] = "m_fSpeed";
    static const char m_fPitch_007514f8[] = "m_fPresetPitch";
    static const char m_fHeight_007437c4[] = "m_fHeight";
    if (kotor_stricmp_0070acaf(m_fDistance_007437dc, firstword_00463530(message)) == 0)    // 00637A36..00637A4E
        parse_float_004635c0(message + 7, fp(s, 0x110));                                   // 00637A50..00637A5B
    if (kotor_stricmp_0070acaf(m_fSpeed_007437d0, firstword_00463530(message)) == 0)       // 00637A63..00637A79
        parse_float_004635c0(message + 8, fp(s, 0x114));                                   // 00637A7B..00637A86
    if (kotor_stricmp_0070acaf(m_fPitch_007514f8, firstword_00463530(message)) == 0)       // 00637A8E..00637AA4
        parse_float_004635c0(message + 0x0E, fp(s, 0x11C));                                // 00637AA6..00637AB1
    if (kotor_stricmp_0070acaf(m_fHeight_007437c4, firstword_00463530(message)) == 0)      // 00637AB9..00637ACF
        parse_float_004635c0(message + 9, fp(s, 0x120));                                   // 00637AD1..00637ADC
}

// 00638070 SetCameraStyle(int style) (RET 4).
extern "C" void __fastcall kotor_fastcall_00638070(void* s, void*, I32 style) {
    if (li(s, 0x5C) == style) return;                                                      // 0063808E..00638099
    void* table = lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x74);                           // 0063809F..006380AB camerastyle 2DA
    if (table == nullptr) {                                                                // 006380AE..006380B0
        sw(s, 0x5C, 0xFFFFFFFFu);                                                          // 006380B2
        sw(s, 0x110, 0x40C80000u); sw(s, 0x114, 0x41A00000u);                              // 006380B9..006380C3
        sw(s, 0x11C, 0x42A00000u); sw(s, 0x120, 0x40600000u);                              // 006380CD..006380D7
        return;
    }
    if (style > li(table, 0x24)) return;                                                   // 006380F5..006380F8 (jg)
    sw(s, 0x5C, static_cast<U32>(style));                                                  // 00638107
    struct Column { const char* name; float* out; };
    float view_angle = 0.0f;   // [esp+0Ch]; NOTE: uninitialised in the original if the VIEWANGLE lookup fails
    const Column columns[] = {
        {"DISTANCE", fp(s, 0x110)}, {"SPEED", fp(s, 0x114)}, {"PITCH", fp(s, 0x11C)}, {"HEIGHT", fp(s, 0x120)},
        {"TILTUP", &tilt_up_007a2420}, {"TILTDOWN", &tilt_down_007a2424}, {"TILTSPEED", fp(s, 0x134)},
        {"ROTATION", &rotation_007a2428}, {"VIEWANGLE", &view_angle}};                     // 006380FE..006382F4
    for (const Column& c : columns) {
        U32 name[2];
        frame_LogStringCtor_005e5a90(name, nullptr, c.name);
        menu_tables_required::C2daFloatEntry_00413430(static_cast<options_video::Table*>(table), nullptr, li(s, 0x5C),
                                                      reinterpret_cast<options_video::String*>(name), c.out);
        frame_LogStringDtor_005e5c20(name, nullptr);
    }
    // 00638306..00638350: the three option getters (ST0) -> fstp dword [esi+F4h/F8h/FCh].
    at<float>(s, 0xF4) = static_cast<float>(kotor_fastcall_0061d5e0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
    at<float>(s, 0xF8) = static_cast<float>(kotor_fastcall_0061d610(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
    at<float>(s, 0xFC) = static_cast<float>(kotor_fastcall_0061d640(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
    if (view_angle == k_zero) at<U32>(&view_angle, 0) = 0x425C0000u;                      // 00638356..00638367 (test 44h ; jp)
    void* camera = reinterpret_cast<void*(__thiscall*)(void*)>(vfn(at<void*>(s, 0x1C), 0x04))(at<void*>(s, 0x1C)); // 0063836F..00638374
    reinterpret_cast<void(__thiscall*)(void*, U32)>(vfn(camera, 0x44))(camera, word(&view_angle)); // 00638377..00638380 SetViewAngle (push word)
    at<U32>(s, 0x88) = at<U32>(s, 0x110);                                                  // 00638383..0063838D
    at<U32>(s, 0x8C) = word(&view_angle);                                                  // 00638389..00638393
}

// 0063A380 ChangeTarget(CAurObject* part, int) (RET 8).
extern "C" void __fastcall kotor_fastcall_0063a380(void* s, void*, void* part, U32) {
    sw(s, 0x100, 0);                                                                       // 0063A394
    if (part != target(s)) {                                                               // 0063A38B..0063A39E
        kotor_fastcall_0049f1f0(s, nullptr, target(s));                                    // 0063A3A0..0063A3A1
        behavior_add_dependency_0049f130(s, nullptr, part);                                // 0063A3A6..0063A3A9
    }
    safe_pointer_assign_00447fa0(static_cast<U8*>(s) + 0x14, nullptr, part);               // 0063A3AE..0063A3B1
    W4CamQuaternion qa, qb, tmp;
    std::memcpy(&qa, gob_get_orientation(target(s), &tmp), 16);                            // 0063A3B6..0063A434
    std::memcpy(&qb, gob_get_orientation(owner(s), &tmp), 16);                             // 0063A438..0063A45B
    w4cam::RollQuaternion rq;
    std::memcpy(&rq, &qb, 16);                                                             // 0063A45F..0063A477 (mov)
    StoredFloat roll, pitch;
    roll.f = static_cast<float>(w4cam::roll_004acb90(rq));                                 // 0063A47A..0063A47F fstp dword
    pitch.f = static_cast<float>(quaternion_pitch_004aa020(&qb));                          // 0063A486..0063A490 fstp dword
    const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&qa));                    // 0063A493..0063A49D fstp dword
    W4CamQuaternion o;
    ypr_words(&o, word(&yaw), pitch.word(), roll.word());                                              // 0063A4A0..0063A4A5 (stored in the argument slots before the calls)
    gob_set_orientation(owner(s), words4(&o));                                             // 0063A4AA..0063A4CF
    // 0063A4D2..0063A54C: a = 2(xy - zw); b = 1 - 2(x^2 + z^2); c = 2(yz + xw) (stored at 0063A514, its negation at
    // 0063A53C, then overwritten by 0063A54C); v = (-a, -b, 0).
    const long double ta = ld(qa.x) * qa.y - ld(qa.z) * qa.w;
    const long double a = ta + ta;
    const long double tb = ld(qa.x) * qa.x + ld(qa.z) * qa.z;
    const long double b = k_one - (tb + tb);
    W4Vector v;
    v.x = static_cast<float>(-a);                                                          // 0063A51A..0063A524
    v.y = static_cast<float>(-b);                                                          // 0063A528..0063A538
    v.z = 0.0f;                                                                            // 0063A54C
    vector_normalize_004ab130(&v, nullptr);                                                // 0063A554
    const long double distance = ld(at<float>(s, 0x110));                                  // 0063A559
    const long double dx = static_cast<float>(reload(v.x) * distance);                     // 0063A55F..0063A56E fstp dword (v normalized in place)
    const long double dy = static_cast<float>(reload(v.y) * distance);                     // 0063A572..0063A578 fstp dword
    W4Vector pb;
    const W4Vector* p = gob_get_position(target(s), &pb);                                  // 0063A563..0063A57E (read through EAX)
    W4Vector r;
    r.x = static_cast<float>(dx + p->x);                                                   // 0063A581..0063A59A
    r.y = static_cast<float>(dy + p->y);                                                   // 0063A59E..0063A5B0
    at<U32>(&r, 8) = at<U32>(s, 0x120);                                                    // 0063A585..0063A596 (mov)
    std::memcpy(fp(s, 0x44), &r, 12);                                                      // 0063A5AC..0063A5C2
    std::memcpy(fp(s, 0x50), &r, 12);
}

// 005F5180 GetTargetHeadPosition: REUSED from runtime1115 (@kotor_fastcall_005f5180@12, 0715 m48b client sound engine).

// 00637AF0 Control_ComputeDesiredPosition(Vector* out, Vector head) (RET 10h).
extern "C" W4Vector* __fastcall kotor_fastcall_00637af0(void* s, void*, W4Vector* out, W4Vector) {
    const float* head = reinterpret_cast<const float*>(KOTOR_ARGS() + 4);                  // [esp+8]..[esp+10h]
    const long double distance = ld(at<float>(s, 0x110));                                  // 00637AF3
    const long double zero_times = k_zero * distance;                                      // 00637AFD..00637B04 (fld 0 ; fmul st(1))
    W4Vector offset;
    offset.x = static_cast<float>(zero_times);                                             // 00637B0E fst dword
    offset.y = static_cast<float>(distance);                                               // 00637B15 fstp dword
    offset.z = static_cast<float>(zero_times);                                             // 00637B19 fstp dword
    W4Vector r;
    quaternion_rotate_004a9bb0(&r, fp(s, 0x28), &offset);                                  // 00637B1D
    W4Vector o;
    o.x = static_cast<float>(ld(head[0]) - r.x);                                           // 00637B24..00637B3A
    o.y = static_cast<float>(ld(head[1]) - r.y);                                           // 00637B3E..00637B54
    o.z = static_cast<float>(ld(head[2]) - r.z);                                           // 00637B58..00637B69
    std::memcpy(out, &o, 12);                                                              // 00637B5C..00637B71 (mov)
    return out;
}

// 00637B80 Control_AccelerateInterp(cur, min, max, Vector a, Vector b, delta) (RET 28h; ST0).
// The return type is the header's `double` (camera_stick.hpp); the result is computed in double-typed x87 operations
// (no call in the body, so nothing is spilled), so it is returned in ST0 with no long double -> double conversion,
// exactly the original's ST0 (the caller 0063BE4E stores it with fstp dword at once).
extern "C" double __fastcall kotor_fastcall_00637b80(void*, void*, float cur, float lo, float hi, W4Vector, W4Vector, float delta) {
    const float* a = reinterpret_cast<const float*>(KOTOR_ARGS() + 0x0C);                  // [esp+10h]..[esp+18h]
    const float* b = reinterpret_cast<const float*>(KOTOR_ARGS() + 0x18);                  // [esp+1Ch]..[esp+24h]
    // 00637B84..00637BAC: dx, dy, dz on the stack; (dz*dz + dy*dy) + dx*dx -> fstp dword (into the b.x slot).
    const long double dx = ld(b[0]) - a[0], dy = ld(b[1]) - a[1], dz = ld(b[2]) - a[2];
    const long double d2 = static_cast<float>((dz * dz + dy * dy) + dx * dx);
    double v = cur;                                                                        // 00637B80 fld cur
    if (d2 > k_one) v = static_cast<double>(delta) * k_three_00740c84 + cur;               // 00637BB6..00637BD3 (test 41h ; jne)
    if (d2 < k_one) v = v - static_cast<double>(delta) * k_five_00743264;                  // 00637BD7..00637BF2 (test 5 ; jp)
    if (!(v > lo)) v = lo;                                                                 // 00637BF4..00637C01 (test 41h ; je keeps v > lo)
    if (!(v < hi)) v = hi;                                                                 // 00637C05..00637C12 (test 5 ; jnp keeps v < hi)
    return v;
}

// 00639760 Control_SetCameraOrientation(Quaternion* out, delta) (RET 8).
extern "C" W4CamQuaternion* __fastcall kotor_fastcall_00639760(void* s, void*, W4CamQuaternion* out, float delta) {
    W4Vector cb, tb;
    const W4Vector* cam = gob_get_position(owner(s), &cb);                                 // 00639767..00639775 (EBX)
    const W4Vector* tgt = gob_get_position(target(s), &tb);                                // 00639778..00639783 (EAX)
    W4Vector v;
    v.y = static_cast<float>(ld(tgt->y) - cam->y);                                         // 00639786..00639789 / 006397A4
    v.x = static_cast<float>(ld(tgt->x) - cam->x);                                         // 0063978F..00639795 / 006397A0
    v.z = 0.0f;                                                                            // 00639797
    W4CamQuaternion qb;
    const W4CamQuaternion* current = gob_get_orientation(owner(s), &qb);                   // 006397A8..006397AA
    const long double pitch = quaternion_pitch_004aa020(current);                          // 006397AD..006397AE (kept on ST)
    if (fabs(ld(at<float>(s, 0x104))) > k_one_double_0073ede8)                             // 006397B3..006397C9 (fabs ; fcomp qword ; test 41h ; jne)
        at<float>(s, 0x104) = static_cast<float>(ld(at<float>(s, 0x104)) * k_dist_scale_0074ea10); // 006397CB..006397D7
    if (ld(at<float>(s, 0x104)) > k_zero) {                                                // 006397DD..006397EE (test 41h ; jne)
        const long double limit = ld(tilt_up_007a2420) + at<float>(s, 0x11C);              // 006397F0..006397F6
        const long double cand = static_cast<float>((ld(at<float>(s, 0x134)) * at<float>(s, 0x104)) * delta + at<float>(s, 0x108)); // 006397FC..00639812 fstp dword
        at<float>(s, 0x108) = static_cast<float>(limit < cand ? limit : cand);             // 00639816..00639827 (test 5 ; jnp)
    }
    if (ld(at<float>(s, 0x104)) < k_zero) {                                                // 0063982D..0063983E (test 5 ; jp)
        const long double limit = ld(at<float>(s, 0x11C)) - tilt_down_007a2424;            // 00639840..00639846
        const long double cand = static_cast<float>((ld(at<float>(s, 0x134)) * at<float>(s, 0x104)) * delta + at<float>(s, 0x108)); // 0063984C..00639862 fstp dword
        at<float>(s, 0x108) = static_cast<float>(limit > cand ? limit : cand);             // 00639866..00639877 (test 41h ; je)
    }
    const long double d = ((ld(at<float>(s, 0x108)) - pitch) * delta) * k_half_0073e9ac;  // 0063987D..0063989C (fsubr [esi+108h])
    at<U32>(s, 0x104) = 0;                                                                 // 00639892
    StoredFloat new_pitch;
    new_pitch.f = static_cast<float>((ld(at<float>(s, 0x108)) - d) + at<float>(s, 0x94)); // 006398A2..006398B0 fstp dword
    W4Vector n;
    vector_normalized_004ab180(&n, &v);                                                    // 006398B3..006398B7
    const float yaw = static_cast<float>(static_cast<long double>(kotor_m15a_yaw_004aa0f0(&n)) + at<float>(s, 0x90)); // 006398BC..006398CF
    ypr_words(out, word(&yaw), new_pitch.word(), 0);                                       // 006398D2..006398D3 (pitch stored at 006398B0 before the calls)
    return out;
}

// 006398F0 Control_ComputeNewCameraPosition(Vector* out, Vector a, Vector b, Vector c, delta) (RET 2Ch).
// Arguments: [esp+4] out, [esp+8] a, [esp+14h] b, [esp+20h] c, [esp+2Ch] delta.
extern "C" W4Vector* __fastcall kotor_fastcall_006398f0(void* s, void*, W4Vector* out, W4Vector, W4Vector, W4Vector, float delta) {
    const float* b = reinterpret_cast<const float*>(KOTOR_ARGS() + 0x10);
    const float* c = reinterpret_cast<const float*>(KOTOR_ARGS() + 0x1C);
    W4Vector t;                                                                            // [esp+4]..[esp+0Ch] of the frame
    t.x = static_cast<float>(ld(b[0]) - c[0]);                                             // 006398F3..00639902 fstp dword
    t.y = static_cast<float>(ld(b[1]) - c[1]);                                             // 00639908..00639920 fstp dword
    t.z = 0.0f;                                                                            // 0063990C
    const long double tx = t.x, ty = t.y;                                                  // 00639924 / 0063992E fld dword
    W4Vector neg;                                                                          // the argument b, rewritten in place
    neg.x = static_cast<float>(-tx);                                                       // 00639924..0063992A
    neg.y = static_cast<float>(-ty);                                                       // 0063992E..00639934
    neg.z = 0.0f;                                                                          // 00639918
    const float qa_angle = static_cast<float>(static_cast<long double>(kotor_m15a_yaw_004aa0f0(&neg)) * k_deg_to_rad_007415f8); // 00639938..0063994E
    W4CamQuaternion qa;
    axis_angle(&qa, up_axis(), qa_angle);                                                  // 00639943..00639968
    // 0063996D..00639995: Yaw(&qa) ; fst dword y1 ; fcomp 0 on the UNROUNDED value (test 5 ; jp) ; y1 + 360 -> fstp.
    const long double y1r = quaternion_yaw_004a9f40(&qa);
    long double y1 = static_cast<float>(y1r);
    if (y1r < k_zero) y1 = static_cast<float>(y1 + k_360_00741868);
    // 00639999..006399C0: the same for the stored orientation +28h (y2).
    const long double y2r = quaternion_yaw_004a9f40(reinterpret_cast<const W4CamQuaternion*>(fp(s, 0x28)));
    long double y2 = static_cast<float>(y2r);
    if (y2r < k_zero) y2 = static_cast<float>(y2 + k_360_00741868);
    long double diff;
    if (y1 >= k_270_0073e404 && y2 <= k_90_0073e40c) {                                     // 006399C4..006399E4 (test 1 ; jne / test 41h ; jp)
        diff = (ld(k_360_00741868) - y1) + y2;                                             // 006399E6..006399F0
    } else if (y1 <= k_90_0073e40c && y2 >= k_270_0073e404) {                              // 006399F6..00639A16
        diff = -((ld(k_360_00741868) - y2) + y1);                                          // 00639A18..00639A26
    } else {
        diff = y2 - y1;                                                                    // 00639A2A..00639A2E
    }
    // 00639A32..00639A64: (hi_lo < diff < hi_hi) or (lo_lo < diff < lo_hi) -> 180.
    const bool to_180 = (diff > k_wrap_hi_lo_0075163c && diff < k_wrap_hi_hi_00751638) ||
                        (diff < k_wrap_lo_hi_00751634 && diff > k_wrap_lo_lo_00751630);
    if (to_180) diff = k_180_0073e408;                                                     // 00639A66..00639A68
    else if (diff > k_180_0073e408) diff = diff - k_360_00741868;                          // 00639A70..00639A7D (test 41h ; jne)
    else if (diff < k_minus_180_0074f4f0) diff = diff + k_360_00741868;                    // 00639A85..00639A92 (test 5 ; jp)
    long double a = (diff * at<float>(s, 0x9C)) * delta;                                   // 00639A98..00639AA3
    if (at<U32>(s, 0x60) != 0) {                                                           // 00639A9E..00639AA7
        at<U32>(s, 0x60) = 0;                                                              // 00639AB2
        W4CamQuaternion qt;
        std::memcpy(fp(s, 0x28), gob_get_orientation(target(s), &qt), 16);                // 00639AA9..00639AD5
        a = quaternion_yaw_004a9f40(reinterpret_cast<const W4CamQuaternion*>(fp(s, 0x28))); // 00639AD4..00639AD8
    }
    if (a > k_360_00741868) a = a - k_360_00741868;                                        // 00639AE0..00639AED (test 41h ; jne)
    if (a < k_zero) a = a + k_360_00741868;                                                // 00639AF3..00639B00 (test 5 ; jp)
    if (at<U32>(s, 0x64) != 0) { at<U32>(s, 0x64) = 0; a = y2; }                           // 00639B06..00639B16
    else a = a + y1;                                                                       // 00639B1C
    W4CamQuaternion qb;
    axis_angle(&qb, up_axis(), static_cast<float>(a * k_deg_to_rad_007415f8));             // 00639B20..00639B4C
    const long double m = vector_magnitude_004ab100(&t);                                   // 00639B51..00639B56
    const long double e = m - at<float>(s, 0x110);                                         // 00639B5B
    const long double r = (e - ld(new_position_scale_007a243c) * e) + at<float>(s, 0x110); // 00639B61..00639B6B
    const long double zr = k_zero * r;                                                     // 00639B71..00639B77 (fld 0 ; fmul st(1))
    W4Vector offset;
    offset.x = static_cast<float>(zr);                                                     // 00639B79 fst dword
    offset.y = static_cast<float>(r);                                                      // 00639B87 fstp dword
    offset.z = static_cast<float>(zr);                                                     // 00639B8F fstp dword
    W4Vector rotated;
    quaternion_rotate_004a9bb0(&rotated, &qb, &offset);                                    // 00639BAE
    W4Vector o;
    o.x = static_cast<float>(ld(c[0]) - rotated.x);                                        // 00639BB3..00639BCB
    o.y = static_cast<float>(ld(c[1]) - rotated.y);                                        // 00639BCF..00639BE3
    o.z = static_cast<float>(ld(c[2]) + at<float>(s, 0x120));                              // 00639BFB..00639C11 (c.z - rotated.z at 00639BE7 is dropped)
    std::memcpy(out, &o, 12);                                                              // 00639C0F..00639C1C (mov)
    return out;
}

// 006391A0 Control_UpdateCameraDesiredOrientation(Quaternion* out, Quaternion q, delta) (RET 18h).
// Arguments: [esp+4] out, [esp+8]..[esp+14h] q, [esp+18h] delta.
extern "C" W4CamQuaternion* __fastcall kotor_fastcall_006391a0(void* s, void*, W4CamQuaternion* out, W4CamQuaternion, float) {
    const U8* const args = KOTOR_ARGS();
    at<U32>(s, 0xA0) = 0; at<U32>(s, 0x64) = 1;                                            // 006391B7..006391BD
    W4Vector unused;
    gob_get_position(target(s), &unused);                                                  // 006391A8..006391C3 (result unused)
    // 006391D5..006391DA: GetMouseSenSetting (ST0) -> fstp dword [esp+10h] (reloaded by 006394F9 fdiv).
    const long double sens = static_cast<float>(kotor_fastcall_0061d5b0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
    W4CamQuaternion cur;
    std::memcpy(&cur, args + 4, 16);                                                       // 006391E4..0063920B (mov)
    if (!(at<float>(s, 0x100) == k_zero) || !(at<float>(s, 0x10C) == k_zero)) {           // 006391DE..00639226 (test 44h ; jp)
        at<U32>(s, 0x80) = 0x7F000000u;                                                    // 00639344
    } else if (at<U32>(s, 0x80) != 0x7F000000u) {                                          // 0063922C..00639237
        void* object = kotor_fastcall_005ed580(client_app(), nullptr, at<U32>(s, 0x80));   // 0063923D..00639247
        if (object != nullptr) {                                                           // 0063924C..00639250
            W4Vector tb;
            const W4Vector* tp = gob_get_position(target(s), &tb);                         // 00639256..00639260
            W4Vector d;
            d.z = static_cast<float>(ld(at<float>(object, 0x2C)) - tp->z);                 // 00639263..00639266 / 00639285
            d.y = static_cast<float>(ld(at<float>(object, 0x28)) - tp->y);                 // 0063926D..00639270 / 00639281
            d.x = static_cast<float>(ld(at<float>(object, 0x24)) - tp->x);                 // 00639273..00639276 / 0063927D
            W4Vector ydir, cross;
            vector_cross_004ab210(&cross, reinterpret_cast<const W4Vector*>(&kotor_world_los::up_007a19b0),
                                  quaternion_ydir_0044f420(&cur, nullptr, &ydir));         // 00639269..00639299
            const long double dot = (ld(cross.z) * reload(d.z) + ld(cross.y) * reload(d.y)) + ld(cross.x) * reload(d.x); // 0063929E..006392C3
            float step = 15.0f;                                                            // 006392AD
            if (dot < k_zero_double_0073edf8) step = -15.0f;                               // 006392C5..006392D2 (fcom qword ; test 5 ; jp)
            if (fabs(dot) < k_cos30_00751628) at<U32>(s, 0x80) = 0x7F000000u;              // 006392DA..006392E9 (fabs ; fcomp qword ; test 5 ; jp)
            const float yaw = static_cast<float>(static_cast<long double>(kotor_m15a_yaw_004aa0f0(&d)) - step); // 006392F3..00639307
            ypr(&cur, yaw, 0.0f, 0.0f);                                                    // 0063930A..0063932F
            at<U32>(s, 0xA0) = word(&desired_turn_rate_007a2434);                          // 00639327..00639333 (mov)
            at<U32>(s, 0x64) = 0;                                                          // 00639339
            at<U32>(s, 0xAC) = 1;                                                          // 0063933C
        }
    }
    (void)kotor_fastcall_005eda40(client_app(), nullptr);                                  // 0063934E..00639357 (result unused)
    I32 free_look = kotor_input_cells_required::word_008338f0;                             // 00639365
    if (at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 8) & 2) free_look = free_look == 0 ? 1 : 0; // 0063935C..0063937D
    if (free_look != 0) {                                                                  // 0063937F..00639387
        if (at<U32>(s, 0xB0) == 0) {                                                       // 00639389..0063938B
            gui_m48_manager_store_mouse_0040a3d0(kotor_input_cells::gui_manager_007a39f4, nullptr); // 0063938D..00639393
            at<U32>(s, 0xB0) = static_cast<U32>(free_look);                                // 00639398
            kotor_fastcall_005ee170(client_app(), nullptr, 0x20);                          // 0063939E..006393A9 HideMouse
        }
    } else if (at<U32>(s, 0xB0) != 0) {                                                    // 006393B0..006393B2
        gui_m48_manager_restore_mouse_0040c7d0(kotor_input_cells::gui_manager_007a39f4, nullptr); // 006393B4..006393BA
        at<U32>(s, 0xB0) = 0;                                                              // 006393BF
        kotor_fastcall_005ee180(client_app(), nullptr, 0x20);                              // 006393C5..006393D0 ShowMouse
    }
    if (gui_m48_client_get_input_class_005eda70(client_app(), nullptr) == 2) {              // 006393D5..006393E5
        at<U32>(s, 0xB0) = 0;                                                              // 006393E7
        kotor_fastcall_005ee180(client_app(), nullptr, 0x20);                              // 006393ED..006393F8
    }
    I32 turning = free_look;                                                               // 006393FF ebp
    if (free_look != 0 && at<U32>(pt(frame_R4_005ed690(client_app(), nullptr)), 0x30) == 0) // 006393FD..00639414
        kotor_fastcall_005ee170(client_app(), nullptr, 0x20);                              // 00639416..00639420
    else
        kotor_fastcall_005ee180(client_app(), nullptr, 0x20);                              // 00639427..00639432
    // 00639437..0063944E: fld qword [0073EDE8] 1.0 ; fld dword [007A2448] ; call 006FC1F0 _CIpow ; fmul dword [esi+100h] ;
    // fstp dword [esp+60h]. pow is the kotor_math.h long double overload (_CIpow semantics, L1).
    long double turn = static_cast<float>(pow(static_cast<long double>(k_one_double_0073ede8), ld(turn_exponent_007a2448))
                                          * at<float>(s, 0x100));
    bool rotate_free = false;                                                              // -> 0063953E
    bool rotate_limited = false;                                                           // -> 0063962F
    // 00639452..0063946B: fld 0.0 ; fcom dword [esi+100h] (kept: the 0.0 is the x and z of the RK4 input below) ;
    // test 44h ; jp / cmp edi ; jne.
    typedef W4RK4State*(__fastcall* Rk4Update)(void*, void*, W4RK4State*, V3W, U32, U32, U32, U32, U32);
    const Rk4Update rk4_update = word_form<Rk4Update>(&kotor_fastcall_006d1190);    // CSWRK4Acceleration::Update (RET 24h)
    if (k_zero == at<float>(s, 0x100) && free_look == 0) {
        // 0063946D..006394AD: input {0.0, [esi+10Ch] through fld/fstp dword, 0.0}, delta pushed as a word.
        W4Vector in;
        in.x = 0.0f;                                                                       // 00639471 fst dword [esp+14h] (the 0.0)
        in.y = static_cast<float>(ld(at<float>(s, 0x10C)));                               // 00639475 fld ; 0063948C fstp dword
        in.z = 0.0f;                                                                       // 00639497 fstp dword (the 0.0)
        W4RK4State state;
        const W4RK4State* st = rk4_update(static_cast<U8*>(s) + 0xB4, nullptr, &state, words3(&in), word(args + 0x14), 1, 0, 0, 0);
        float turn_word;                                                                   // 006394B2..006394B5: mov eax,[eax+10h] ; mov [esp+60h],eax
        std::memcpy(&turn_word, &st->velocity.y, 4);
        turn = turn_word;                                                                  // every later use is fld dword [esp+60h]
        turning = (turn > k_zero) ? 1 : 0;                                                 // 006394B9..006394D3 (test 41h ; jne)
        if (turn < k_zero) rotate_free = true;                                             // 006394D3..006394E2 (test 5 ; jnp) -> 0063953E
        else rotate_limited = true;                                                        // -> 0063962F
    } else {
        // 006394E9..0063952B: input {0.0, turn / sens (fdiv dword [esp+10h]), 0.0}.
        W4Vector in;
        in.x = 0.0f;                                                                       // 006394E9 fst dword [esp+14h] (the 0.0)
        in.y = static_cast<float>(turn / sens);                                            // 006394F1..0063950A
        in.z = 0.0f;                                                                       // 00639519 fstp dword (the 0.0)
        W4RK4State state;
        rk4_update(static_cast<U8*>(s) + 0xB4, nullptr, &state, words3(&in), word(args + 0x14), 1, 0, 0, 0);
        at<U32>(s, 0x10C) = 0;                                                             // 00639532
        if (free_look != 0) rotate_free = true;                                            // 00639530..00639538
    }
    if (rotate_free) {                                                                     // 0063953E
        long double v;
        if (orientation_debug_0083394c != 0 && free_look == 0) {                           // 0063953E..00639548
            v = turn;                                                                      // 0063954A
        } else if (turn_sign_00832920 != 0) {                                              // 00639593..006395A6
            v = !(k_zero > turn) ? turn : ld(k_zero);                                      // 006395A8..006395B5 (test 41h ; jne)
        } else {
            v = !(k_zero < turn) ? turn : ld(k_zero);                                      // 006395DB..006395E8 (test 5 ; jp)
        }
        W4CamQuaternion r, p;
        ypr(&r, static_cast<float>(-v), 0.0f, 0.0f);                                       // 00639553..006395FE (fchs ; fstp dword)
        qmul(&p, &r, &cur);                                                                // 0063956E / 0063960C
        std::memcpy(&cur, &p, 16);                                                         // 00639573..0063958A / 00639611..00639628
        rotate_limited = true;                                                             // falls to 0063962F
    }
    if (rotate_limited && turning != 0) {                                                  // 0063962F..00639631
        long double v;
        if (orientation_debug_0083394c != 0 && free_look == 0) {                           // 00639637..00639641
            v = turn;                                                                      // 00639672
        } else if (turn_sign_00832920 != 0) {                                              // 00639643..00639656
            v = !(k_zero < turn) ? turn : ld(k_zero);                                      // 00639658..0063965D (test 5 ; jp)
        } else {
            v = !(k_zero > turn) ? turn : ld(k_zero);                                      // 00639665..0063966A (test 41h ; jne)
        }
        W4CamQuaternion r, p;
        ypr(&r, static_cast<float>(-v), 0.0f, 0.0f);                                       // 00639676..00639688 (fchs ; fstp dword)
        qmul(&p, &r, &cur);                                                                // 00639690..00639696
        std::memcpy(&cur, &p, 16);                                                         // 0063969B..006396B5
    }
    at<U32>(s, 0x100) = 0;                                                                 // 006396C0
    W4Vector tb;
    std::memcpy(fp(s, 0x44), gob_get_position(target(s), &tb), 12);                        // 006396B9..006396DC
    if (pending_yaw_007a2440 >= k_zero) {                                                  // 006396DF..006396F0 (fcomp ; test 1 ; jne)
        ypr_words(&cur, word(&pending_yaw_007a2440), 0, 0);                                // 006396F2..0063971C (mov ecx,[007A2440] ; push ecx)
        at<U32>(&pending_yaw_007a2440, 0) = 0xBF800000u;                                   // 0063971F
    }
    std::memcpy(out, &cur, 16);                                                            // 0063972F..0063974C
    return out;
}

// 00638F60 Control_CheckAndUpdateCameraStyle: the three option getters (ST0) -> fstp dword [esi+F4h/F8h/FCh].
extern "C" void __fastcall kotor_fastcall_00638f60(void* s, void*) {
    at<float>(s, 0xF4) = static_cast<float>(kotor_fastcall_0061d5e0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr)); // 00638F60..00638F77
    at<float>(s, 0xF8) = static_cast<float>(kotor_fastcall_0061d610(kotor_fastcall_005ed700(client_app(), nullptr), nullptr)); // 00638F7D..00638F92
    at<float>(s, 0xFC) = static_cast<float>(kotor_fastcall_0061d640(kotor_fastcall_005ed700(client_app(), nullptr), nullptr)); // 00638F98..00638FAD
    if (style_request_007a242c != -1) {                                                    // 00638FB3..00638FBB
        kotor_fastcall_00638070(s, nullptr, style_request_007a242c);                       // 00638FBD..00638FC0
        style_request_007a242c = -1;                                                       // 00638FC5
    }
}

// 00638FE0 Control_SetCameraInintalState.
extern "C" void __fastcall kotor_fastcall_00638fe0(void* s, void*) {
    W4CamQuaternion q;
    gob_get_orientation(target(s), &q);                                                    // 00638FE7..00638FF2
    W4Vector v;
    quat_zdir_inline(&v, q);                                                               // 00638FF5..0063904A
    StoredFloat pitch;
    pitch.f = static_cast<float>(ld(at<float>(s, 0x11C)) + at<float>(s, 0x90));          // 0063904E..0063905A fstp dword
    W4Vector n;
    vector_normalized_004ab180(&n, &v);                                                    // 0063905D..0063905F
    const float yaw = static_cast<float>(static_cast<long double>(kotor_m15a_yaw_004aa0f0(&n)) + at<float>(s, 0x90)); // 00639064..00639077
    W4CamQuaternion o;
    ypr_words(&o, word(&yaw), pitch.word(), 0);                                            // 0063907A..0063907B (pitch stored at 0063905A before the calls)
    std::memcpy(fp(s, 0x28), &o, 16);                                                      // 00639080..00639098
    gob_set_orientation(owner(s), words4(&o));                                             // 0063909B..006390BD
    W4Vector p;
    gob_get_position(target(s), &p);                                                       // 006390C0..006390CA
    p.z = static_cast<float>(ld(p.z) + k_one_and_half_0073f414);                           // 006390CD..006390E0
    W4Vector w;
    quat_ydir_inline(&w, q);                                                               // 006390E4..0063912E
    vector_subtract_assign_004a9770(&p, nullptr, &w);                                      // 00639132
    gob_set_position(owner(s), words3(&p));                                                // 00639137..0063915A
    std::memcpy(fp(s, 0x50), &p, 12);                                                      // 0063915D..00639173
    std::memcpy(fp(s, 0x44), &p, 12);
    std::memcpy(fp(s, 0x38), &p, 12);
}

// 00637C20 ResetLastPosition(Vector position, Quaternion q) (RET 1Ch): every copy is a word copy (00637C2D..00637CAF).
extern "C" void __fastcall kotor_fastcall_00637c20(void* s, void*, W4Vector, W4CamQuaternion) {
    const U8* const args = KOTOR_ARGS();                                                   // [esp+4] position, [esp+10h] q
    std::memcpy(fp(s, 0x38), args, 12);                                                    // 00637C2D..00637C40
    std::memcpy(fp(s, 0x50), args, 12);                                                    // 00637C43..00637C4B
    std::memcpy(fp(s, 0x28), args + 12, 16);                                               // 00637C4E..00637C6D
    gob_set_position(owner(s), words3(args));                                              // 00637C5E..00637C80
    gob_set_orientation(owner(s), words4(args + 12));                                      // 00637C83..00637CAF
}

// 00637CC0 CameraFreeStyleControl (debug free-style camera; reached only when [00833938] != 0).
extern "C" U32 __fastcall kotor_fastcall_00637cc0(void*, void*, float) {
    kotor_unreachable_trap(0x00637CC0u, "CSWCameraOnAStick::CameraFreeStyleControl (debug flag 00833938)");
}

// 0063BCB0 CSWCameraOnAStick::Control(float delta) (slot 08h, RET 4). L3 t1: PLAN-v3 writing rule.
extern "C" U32 __fastcall kotor_fastcall_0063bcb0(void* s, void*, float delta) {
    const U32 delta_word = word(KOTOR_ARGS());                                             // [esp+80h] (EBX): pushed as a word
    if (at<U32>(s, 0x84) != 0) {                                                           // 0063BCB8..0063BCC1
        // 0063BCC3..0063BCDE: fld [s+8Ch] ; fmul dword [00741BB4] ; fptan ; fstp st(0) ; fmul [s+88h] ; fstp dword [esp+10h]
        const long double scaled = static_cast<float>(tan(ld(at<float>(s, 0x8C)) * k_half_deg_to_rad_00741bb4) * at<float>(s, 0x88));
        void* camera = reinterpret_cast<void*(__thiscall*)(void*)>(vfn(at<void*>(s, 0x1C), 0x04))(at<void*>(s, 0x1C)); // 0063BCC9..0063BCE2
        const long double view = reinterpret_cast<double(__thiscall*)(void*)>(vfn(camera, 0x48))(camera); // 0063BCE5..0063BCE9 GetViewAngle (ST0)
        // 0063BCEC..0063BCFA: fmul dword [00741BB4] ; fptan ; fstp st(0) ; fdivr dword [esp+10h] ; fstp dword [s+110h]
        at<float>(s, 0x110) = static_cast<float>(scaled / tan(view * k_half_deg_to_rad_00741bb4));
    }
    kotor_fastcall_00638f60(s, nullptr);                                                   // 0063BD00..0063BD02
    typedef U32(__fastcall* Control)(void*, void*, U32);
    if (word_form<Control>(&behavior_control_0049f270)(s, nullptr, delta_word) == 0) return 1; // 0063BD07..0063BD18
    if (free_style_debug_00833938 != 0)                                                    // 0063BD1E..0063BD36
        return word_form<Control>(&kotor_fastcall_00637cc0)(s, nullptr, delta_word);
    if (at<U32>(s, 0x14) == 0) {                                                           // 0063BD39..0063BD3E
        reinterpret_cast<void(__thiscall*)(void*, void*)>(vfn(owner(s), 0x58))(owner(s), nullptr); // 0063BD40..0063BD49 SetBehavior(0)
        return 1;
    }
    if (at<float>(s, 0x44) >= k_flt_max_0074c468) kotor_fastcall_00638fe0(s, nullptr);    // 0063BD5B..0063BD6D (fcomp ; test 1 ; jne)
    {
        typedef W4CamQuaternion*(__fastcall* Desired)(void*, void*, W4CamQuaternion*, Q4W, U32);
        W4CamQuaternion r;
        const W4CamQuaternion* rp =
            word_form<Desired>(&kotor_fastcall_006391a0)(s, nullptr, &r, words4(fp(s, 0x28)), delta_word); // 0063BD72..0063BD9D
        std::memcpy(fp(s, 0x28), rp, 16);                                                  // 0063BDA2..0063BDB8 (through EAX)
    }
    W4Vector camera_position;
    gob_get_position(owner(s), &camera_position);                                          // 0063BDB5..0063BDC2 (the out buffer is read)
    W4Vector head;
    kotor_fastcall_005f5180(s, nullptr, &head);                                            // 0063BDC5..0063BDCC
    typedef W4Vector*(__fastcall* Desired2)(void*, void*, W4Vector*, V3W);
    W4Vector desired;
    word_form<Desired2>(&kotor_fastcall_00637af0)(s, nullptr, &desired, words3(&head)); // 0063BDD1..0063BDF1
    W4Vector cb;
    const W4Vector* camera_position2 = gob_get_position(owner(s), &cb);                    // 0063BDF6..0063BE00 (read through EAX)
    typedef double(__fastcall* Interp)(void*, void*, U32, U32, U32, V3W, V3W, U32);
    at<float>(s, 0x9C) = static_cast<float>(word_form<Interp>(&kotor_fastcall_00637b80)(
        s, nullptr, at<U32>(s, 0x9C), word(&accel_interp_min_007a2430), at<U32>(s, 0xA0),
        words3(camera_position2), words3(&desired), delta_word));                          // 0063BE03..0063BE4E (fstp dword [esi+9Ch])
    typedef W4Vector*(__fastcall* NewPos)(void*, void*, W4Vector*, V3W, V3W, V3W, U32);
    W4Vector next;
    const W4Vector* np = word_form<NewPos>(&kotor_fastcall_006398f0)(
        s, nullptr, &next, words3(&desired), words3(&camera_position), words3(&head), delta_word); // 0063BE54..0063BE9F
    typedef W4Vector*(__fastcall* HitCheck)(void*, void*, W4Vector*, V3W);
    W4Vector hb;
    const W4Vector* hp = word_form<HitCheck>(&kotor_fastcall_0063b050)(s, nullptr, &hb, words3(np)); // 0063BEA4..0063BEC0
    const V3W hit = words3(hp);                                                            // 0063BEC5..0063BED4 (EDI, EBP, [esp+1Ch])
    if (dialog_camera_debug_008327ec != 0 && at<U32>(s, 0x10) != 0) {                      // 0063BECD..0063BEDF
        // 0063BEE1..0063BF28: debug distance probe; every result is discarded (fstp st(0)).
        W4Vector p1, p2, p3, p4, diff;
        gob_get_position(owner(s), &p1); gob_get_position(owner(s), &p2); gob_get_position(owner(s), &p3); // 0063BEE1..0063BF02
        const W4Vector* pp = gob_get_position(owner(s), &p4);                              // 0063BF05..0063BF14
        kotor_cdecl_00423bc0(&diff, pp, &head);                                            // 0063BF17..0063BF1D (pos - head)
        (void)vector_magnitude_004ab100(&diff);                                            // 0063BF22..0063BF28
    }
    gob_set_position(owner(s), hit);                                                       // 0063BF2D..0063BF48
    typedef W4CamQuaternion*(__fastcall* Orient)(void*, void*, W4CamQuaternion*, U32);
    W4CamQuaternion o;
    const W4CamQuaternion* op = word_form<Orient>(&kotor_fastcall_00639760)(s, nullptr, &o, delta_word); // 0063BF4B..0063BF53
    gob_set_orientation(owner(s), words4(op));                                             // 0063BF58..0063BF80
    (void)delta;
    if (at<U32>(s, 0x60) != 0) at<U32>(s, 0x60) = 0;                                       // 0063BF83..0063BF8A
    return 1;                                                                              // 0063BF94
}
