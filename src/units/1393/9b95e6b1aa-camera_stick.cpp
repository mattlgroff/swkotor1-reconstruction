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
#include <cmath>

namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }   // (0,0,1) (0532 world-los cells)
extern "C" float x_axis_007a1998[3];                                     // (1,0,0) (0542 core46 math)

using namespace w4;
using namespace w4cam;

namespace w4cam {
float tilt_up_007a2420 = 30.0f, tilt_down_007a2424 = 30.0f, rotation_007a2428 = 90.0f;
double x87_pow(double base, double exponent) {
    double result;
    __asm { fld qword ptr base }
    __asm { fld qword ptr exponent }
    __asm { call genuine_x87_pow_006fc1f0 }
    __asm { fstp qword ptr result }
    return result;
}
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
const W4Vector& up_axis() { return *reinterpret_cast<const W4Vector*>(&kotor_world_los::up_007a19b0); }
const W4Vector& x_axis() { return *reinterpret_cast<const W4Vector*>(x_axis_007a1998); }
inline float* fp(void* o, U32 off) { return reinterpret_cast<float*>(static_cast<U8*>(o) + off); }
inline void* owner(void* s) { return lp(s, 0x10); }
inline void* target(void* s) { return lp(s, 0x14); }
// Gob-view virtuals on owner/target (original vtable 00741988 layout): +5C SetPosition(ret*, Vector),
// +60 SetOrientation(ret*, Quaternion), +64 GetPosition(ret*), +68 GetOrientation(ret*), +58 SetBehavior.
W4Vector gob_position(void* gob) { W4Vector v; vcall<W4Vector*, W4Vector*>(gob, 0x64, &v); return v; }
W4CamQuaternion gob_orientation(void* gob) { W4CamQuaternion q; vcall<W4CamQuaternion*, W4CamQuaternion*>(gob, 0x68, &q); return q; }
void gob_set_position(void* gob, W4Vector v) { W4Vector r; vcall<void, W4Vector*, W4Vector>(gob, 0x5C, &r, v); }
void gob_set_orientation(void* gob, W4CamQuaternion q) { W4CamQuaternion r; vcall<void, W4CamQuaternion*, W4CamQuaternion>(gob, 0x60, &r, q); }
W4CamQuaternion ypr(float yaw, float pitch, float roll) {
    W4CamQuaternion q; yaw_pitch_roll_004acac0(&q, yaw, pitch, roll); return q;
}
W4CamQuaternion axis_angle(const W4Vector& axis, float angle) {
    W4CamQuaternion q; quaternion_from_axis_angle_004ac960(&q, nullptr, axis, angle); return q;
}
W4CamQuaternion qmul(const W4CamQuaternion& a, const W4CamQuaternion& b) {
    W4CamQuaternion q; quaternion_multiply_004a9ac0(&q, &a, &b); return q;
}
// The quaternion "z axis" expression inlined at 0063A124.. / 00638FF5..: {2(zx+yw), 2(zy-xw), 1-2(y^2+x^2)}.
W4Vector quat_zdir_inline(const W4CamQuaternion& q) {
    W4Vector v;
    v.x = static_cast<float>((static_cast<double>(q.z) * q.x + static_cast<double>(q.y) * q.w) * 2.0);
    v.y = static_cast<float>((static_cast<double>(q.z) * q.y - static_cast<double>(q.x) * q.w) * 2.0);
    v.z = static_cast<float>(k_one - (static_cast<double>(q.y) * q.y + static_cast<double>(q.x) * q.x) * 2.0);
    return v;
}
// The quaternion "y axis" expression inlined at 0063A205.. / 006390E4..: {2(yx-zw), 1-2(z^2+x^2), 2(zy+xw)}.
W4Vector quat_ydir_inline(const W4CamQuaternion& q) {
    W4Vector v;
    v.x = static_cast<float>((static_cast<double>(q.y) * q.x - static_cast<double>(q.z) * q.w) * 2.0);
    v.y = static_cast<float>(k_one - (static_cast<double>(q.z) * q.z + static_cast<double>(q.x) * q.x) * 2.0);
    v.z = static_cast<float>((static_cast<double>(q.z) * q.y + static_cast<double>(q.x) * q.w) * 2.0);
    return v;
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

// ---- lane x87-trig (2026-10-24): original x87 sequences around fpatan/fptan kept whole ----
// fsin/fcos/fptan/fpatan always deliver a 64-bit significand (precision control does not apply to them). The
// helpers below therefore keep each transcendental together with the original's consuming x87 instructions and
// end at the original's store, with the original's operand order and store width. Precondition (as in the
// original, 006FAE75 -> _controlfp(_PC_53, _MCW_PC)): x87 PC = 53 bits, RC = nearest; in the rebuilt image the
// CRT's __initialize_default_precision sets the same. Under PC = 53 every non-transcendental x87 result fits a
// double, so a helper that must hand an ST0 result back to C++ (Roll's ST0 return) stores it to a qword exactly.
// No CALL inside any asm block; extern "C" __cdecl so the calling convention cannot be changed by the optimizer.
// Memory operands use this unit's constants: k_one = [0073D764] 1.0f, k_rad_to_deg_00742ef8 (bits 42652EE1),
// k_half_deg_to_rad_00741bb4 (bits 3C0EFA35).
// 004ACC55..004ACC96 (Roll tail): q = the product quaternion words [0],[4],[8],[C] (copied to [esp+40h..4Ch] by
// the original). ST0 = fpatan(-(2(q3 q1 - q2 q0)), 1 - 2(q2 q2 + q3 q3)) * 57.29578f, returned in ST0 by Roll.
extern "C" __declspec(noinline) double __cdecl x87trig_roll_004acc55(const W4CamQuaternion* q) {
    double result;
    __asm {
        mov eax, q
        fld dword ptr [eax + 0Ch]                 // 004ACC55
        fmul dword ptr [eax + 4]                  // 004ACC59
        fld dword ptr [eax + 8]                   // 004ACC68
        fmul dword ptr [eax]                      // 004ACC6C
        fsubp st(1), st                           // 004ACC70 DE E9
        fadd st, st                               // 004ACC72
        fld dword ptr [eax + 8]                   // 004ACC74
        fmul dword ptr [eax + 8]                  // 004ACC78
        fld dword ptr [eax + 0Ch]                 // 004ACC7C
        fmul dword ptr [eax + 0Ch]                // 004ACC80
        faddp st(1), st                           // 004ACC84 DE C1
        fadd st, st                               // 004ACC86
        fsubr dword ptr k_one        // 004ACC88
        fxch st(1)                                // 004ACC8E
        fchs                                      // 004ACC90
        fxch st(1)                                // 004ACC92
        fpatan                                    // 004ACC94
        fmul dword ptr k_rad_to_deg_00742ef8         // 004ACC96
        fstp qword ptr result                     // (exact under PC 53: the fmul result has a 53-bit significand)
    }
    return result;
}
// 0063BCC3..0063BCDE: [esp+10h] = (float)(tan([s+8C] * 0.008726646f) * [s+88]).
extern "C" __declspec(noinline) void __cdecl x87trig_0063bcc3(const void* s, float* scaled) {
    __asm {
        mov eax, s
        fld dword ptr [eax + 8Ch]                 // 0063BCC3
        fmul dword ptr k_half_deg_to_rad_00741bb4         // 0063BCCC
        fptan                                     // 0063BCD4
        fstp st(0)                                // 0063BCD6
        fmul dword ptr [eax + 88h]                // 0063BCD8
        mov eax, scaled
        fstp dword ptr [eax]                      // 0063BCDE
    }
}
// 0063BCEC..0063BCFA: ST0 = Camera::GetViewAngle (0045C4D0: fld dword [ecx+1D0h]; the double carries it exactly);
// [s+110h] = (float)([esp+10h] / tan(view * 0.008726646f)).
extern "C" __declspec(noinline) void __cdecl x87trig_0063bcec(double view, const float* scaled, void* s) {
    __asm {
        fld qword ptr view
        fmul dword ptr k_half_deg_to_rad_00741bb4         // 0063BCEC
        fptan                                     // 0063BCF2
        fstp st(0)                                // 0063BCF4
        mov eax, scaled
        fdivr dword ptr [eax]                     // 0063BCF6
        mov eax, s
        fstp dword ptr [eax + 110h]               // 0063BCFA
    }
}

// 004ACB90 Roll(Quaternion) (cdecl; the by-value argument is overwritten in place by the original).
extern "C" double __cdecl kotor_cdecl_004acb90(W4CamQuaternion q) {
    const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&q) * k_minus_deg_to_rad_00744100); // 004ACB9D..004ACBB3
    q = qmul(axis_angle(up_axis(), yaw), q);                                               // 004ACBB6..004ACC00
    const float pitch = static_cast<float>(quaternion_pitch_004aa020(&q) * k_minus_deg_to_rad_00744100); // 004ACC04..004ACC1B
    const W4CamQuaternion r = qmul(axis_angle(x_axis(), pitch), q);                           // 004ACC1E..004ACC40
    return x87trig_roll_004acc55(&r);                                                      // 004ACC45..004ACC96 (x87)
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
    const W4CamQuaternion q = gob_orientation(target(s));                                     // 0063A0F5..0063A121
    {
        const W4Vector v = quat_zdir_inline(q);                                            // 0063A124..0063A180
        W4Vector n;
        vector_normalized_004ab180(&n, &v);                                                // 0063A184
        const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&n));                 // 0063A189..0063A18F
        const W4CamQuaternion o = ypr(yaw, lf(s, 0x11C), 0.0f);                               // 0063A196..0063A19B
        std::memcpy(fp(s, 0x28), &o, 16);                                                  // 0063A1A0..0063A1B9
        gob_set_orientation(owner(s), o);                                                  // 0063A1B6..0063A1DE
    }
    W4Vector p = gob_position(target(s));                                                  // 0063A1E1..0063A1EB
    p.z = static_cast<float>(static_cast<double>(p.z) + k_one_and_half_0073f414);          // 0063A1EE..0063A201
    const W4Vector w = quat_ydir_inline(q);                                                // 0063A205..0063A24F
    vector_subtract_assign_004a9770(&p, nullptr, &w);                                      // 0063A253
    gob_set_position(owner(s), p);                                                         // 0063A258..0063A27B
    std::memcpy(fp(s, 0x44), &p, 12);                                                      // 0063A28C..0063A2A1
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
    if (camera == nullptr) { *out = W4Vector{0.0f, 0.0f, 0.0f}; return out; }              // 0063F61A..0063F626
    vcall<W4Vector*, W4Vector*>(camera, 0x64, out);                                        // 0063F609..0063F611
    return out;
}
// 0063F630 SetPosition(Vector) (RET 0Ch).
extern "C" void __fastcall kotor_fastcall_0063f630(void* s, void*, W4Vector v) {
    if (void* camera = lp(s, 0x1C)) gob_set_position(camera, v);                           // 0063F630..0063F658
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
    sf(s, 0xF4, static_cast<float>(kotor_fastcall_0061d5e0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr))); // 00638306..0063831B
    sf(s, 0xF8, static_cast<float>(kotor_fastcall_0061d610(kotor_fastcall_005ed700(client_app(), nullptr), nullptr))); // 00638321..00638336
    sf(s, 0xFC, static_cast<float>(kotor_fastcall_0061d640(kotor_fastcall_005ed700(client_app(), nullptr), nullptr))); // 0063833C..00638350
    if (jcc_jnp(view_angle, k_zero, 0x44)) view_angle = 55.0f;                             // 00638356..00638367 (== 0 -> 425C0000)
    void* camera = vcall<void*>(lp(s, 0x1C), 0x04);                                        // 0063836F..00638374
    vcall<void, float>(camera, 0x44, view_angle);                                          // 00638377..00638380 SetViewAngle
    sw(s, 0x88, lw(s, 0x110));                                                             // 00638383..0063838D
    sf(s, 0x8C, view_angle);                                                               // 00638389..00638393
}

// 0063A380 ChangeTarget(CAurObject* part, int) (RET 8).
extern "C" void __fastcall kotor_fastcall_0063a380(void* s, void*, void* part, U32) {
    sw(s, 0x100, 0);                                                                       // 0063A394
    if (part != target(s)) {                                                               // 0063A38B..0063A39E
        kotor_fastcall_0049f1f0(s, nullptr, target(s));                                    // 0063A3A0..0063A3A1
        behavior_add_dependency_0049f130(s, nullptr, part);                                // 0063A3A6..0063A3A9
    }
    safe_pointer_assign_00447fa0(static_cast<U8*>(s) + 0x14, nullptr, part);               // 0063A3AE..0063A3B1
    const W4CamQuaternion qa = gob_orientation(target(s));                                    // 0063A3B6..0063A42E
    const W4CamQuaternion qb = gob_orientation(owner(s));                                     // 0063A431..0063A460
    const float roll = static_cast<float>(kotor_cdecl_004acb90(qb));                       // 0063A461..0063A47F
    const float pitch = static_cast<float>(quaternion_pitch_004aa020(&qb));                // 0063A486..0063A490
    const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&qa));                    // 0063A493..0063A49D
    gob_set_orientation(owner(s), ypr(yaw, pitch, roll));                                  // 0063A4A0..0063A4CF
    // 0063A4D2..0063A54C: a = 2(xy - zw); b = 1 - 2(x^2 + z^2); v = (-a, -b, 0) (c = 2(yz + xw) is computed and dropped).
    const double a = (static_cast<double>(qa.x) * qa.y - static_cast<double>(qa.z) * qa.w) * 2.0;
    const double b = k_one - (static_cast<double>(qa.x) * qa.x + static_cast<double>(qa.z) * qa.z) * 2.0;
    W4Vector v = {static_cast<float>(-a), static_cast<float>(-b), 0.0f};
    vector_normalize_004ab130(&v, nullptr);                                                // 0063A554
    const double distance = lf(s, 0x110);                                                  // 0063A559
    const float dx = static_cast<float>(v.x * distance);                                   // 0063A55F..0063A56E
    const float dy = static_cast<float>(v.y * distance);                                   // 0063A572..0063A578
    const W4Vector p = gob_position(target(s));                                            // 0063A563..0063A57E
    const W4Vector r = {static_cast<float>(static_cast<double>(dx) + p.x), static_cast<float>(static_cast<double>(dy) + p.y),
                        lf(s, 0x120)};                                                     // 0063A581..0063A5BE
    std::memcpy(fp(s, 0x44), &r, 12);
    std::memcpy(fp(s, 0x50), &r, 12);
}

// 005F5180 GetTargetHeadPosition: REUSED from runtime1115 (@kotor_fastcall_005f5180@12, 0715 m48b client sound engine).

// 00637AF0 Control_ComputeDesiredPosition(Vector* out, Vector head) (RET 10h).
extern "C" W4Vector* __fastcall kotor_fastcall_00637af0(void* s, void*, W4Vector* out, W4Vector head) {
    const double distance = lf(s, 0x110);                                                  // 00637AF3
    const float zero_times = static_cast<float>(k_zero * distance);                        // 00637AFD..00637B0E fst
    const W4Vector offset = {zero_times, static_cast<float>(distance), zero_times};        // 00637B15..00637B19
    W4Vector r;
    quaternion_rotate_004a9bb0(&r, fp(s, 0x28), &offset);                                  // 00637B1D
    *out = W4Vector{static_cast<float>(static_cast<double>(head.x) - r.x), static_cast<float>(static_cast<double>(head.y) - r.y),
                    static_cast<float>(static_cast<double>(head.z) - r.z)};                // 00637B24..00637B71
    return out;
}

// 00637B80 Control_AccelerateInterp(cur, min, max, Vector a, Vector b, delta) (RET 28h; ST0).
extern "C" double __fastcall kotor_fastcall_00637b80(void*, void*, float cur, float lo, float hi, W4Vector a, W4Vector b, float delta) {
    const double dx = static_cast<double>(b.x) - a.x, dy = static_cast<double>(b.y) - a.y, dz = static_cast<double>(b.z) - a.z;
    const float d2 = static_cast<float>((dz * dz + dy * dy) + dx * dx);                    // 00637B84..00637BAC fstp float
    double v = cur;                                                                        // 00637B80
    if (jcc_je(d2, k_one, 0x41)) v = static_cast<double>(delta) * k_three_00740c84 + cur;  // 00637BB6..00637BD3 (d2 > 1)
    if (!jcc_jp(d2, k_one, 0x05)) v = v - static_cast<double>(delta) * k_five_00743264;    // 00637BD7..00637BF2 (d2 < 1)



    if (!jcc_je(v, lo, 0x41)) v = lo;                                                      // 00637BF4..00637C01 (v <= lo or NaN)

    if (!jcc_jnp(v, hi, 0x05)) v = hi;                                                     // 00637C05..00637C12 (not v < hi)
    return v;
}

// 00639760 Control_SetCameraOrientation(Quaternion* out, delta) (RET 8).
extern "C" W4CamQuaternion* __fastcall kotor_fastcall_00639760(void* s, void*, W4CamQuaternion* out, float delta) {
    const W4Vector cam = gob_position(owner(s));                                           // 00639767..00639775
    const W4Vector tgt = gob_position(target(s));                                          // 00639778..00639783
    const W4Vector v = {static_cast<float>(static_cast<double>(tgt.x) - cam.x), static_cast<float>(static_cast<double>(tgt.y) - cam.y), 0.0f}; // 00639786..006397A4
    const W4CamQuaternion current = gob_orientation(owner(s));                                // 006397A8..006397AA
    const double pitch = quaternion_pitch_004aa020(&current);                              // 006397AD..006397AE (kept on ST)
    if (jcc_je(std::fabs(static_cast<double>(lf(s, 0x104))), k_one_double_0073ede8, 0x41))  // 006397B3..006397C9 (|tilt| > 1)
        sf(s, 0x104, static_cast<float>(static_cast<double>(lf(s, 0x104)) * k_dist_scale_0074ea10)); // 006397CB..006397D7
    if (jcc_je(lf(s, 0x104), k_zero, 0x41)) {                                              // 006397DD..006397EE (tilt > 0)
        const double limit = static_cast<double>(tilt_up_007a2420) + lf(s, 0x11C);         // 006397F0..006397F6
        const float cand = static_cast<float>((static_cast<double>(lf(s, 0x134)) * lf(s, 0x104)) * delta + lf(s, 0x108)); // 006397FC..00639812
        sf(s, 0x108, static_cast<float>(jcc_jnp(limit, cand, 0x05) ? limit : static_cast<double>(cand))); // 00639816..00639827
    }
    if (!jcc_jp(lf(s, 0x104), k_zero, 0x05)) {                                             // 0063982D..0063983E (tilt < 0)
        const double limit = static_cast<double>(lf(s, 0x11C)) - tilt_down_007a2424;       // 00639840..00639846
        const float cand = static_cast<float>((static_cast<double>(lf(s, 0x134)) * lf(s, 0x104)) * delta + lf(s, 0x108)); // 0063984C..00639862
        sf(s, 0x108, static_cast<float>(jcc_je(limit, cand, 0x41) ? limit : static_cast<double>(cand))); // 00639866..00639877
    }
    const double d = ((static_cast<double>(lf(s, 0x108)) - pitch) * delta) * k_half_0073e9ac; // 0063987D..0063989C
    sw(s, 0x104, 0);                                                                       // 00639892
    const float new_pitch = static_cast<float>((static_cast<double>(lf(s, 0x108)) - d) + lf(s, 0x94)); // 006398A2..006398B0
    W4Vector n;
    vector_normalized_004ab180(&n, &v);                                                    // 006398B3..006398B7
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&n) + lf(s, 0x90));       // 006398BC..006398CF
    yaw_pitch_roll_004acac0(out, yaw, new_pitch, 0.0f);                                    // 006398D2..006398D3
    return out;
}

// 006398F0 Control_ComputeNewCameraPosition(Vector* out, Vector a, Vector b, Vector c, delta) (RET 2Ch).
extern "C" W4Vector* __fastcall kotor_fastcall_006398f0(void* s, void*, W4Vector* out, W4Vector, W4Vector b, W4Vector c, float delta) {
    const W4Vector t = {static_cast<float>(static_cast<double>(b.x) - c.x), static_cast<float>(static_cast<double>(b.y) - c.y), 0.0f}; // 006398F3..00639920
    const W4Vector neg = {-t.x, -t.y, 0.0f};                                               // 00639918..00639934 (arg b reused)
    const float qa_angle = static_cast<float>(kotor_m15a_yaw_004aa0f0(&neg) * k_deg_to_rad_007415f8); // 00639938..0063994E
    const W4CamQuaternion qa = axis_angle(up_axis(), qa_angle);                               // 00639951..00639968
    float y1 = static_cast<float>(quaternion_yaw_004a9f40(&qa));                           // 0063996D..00639977 fst
    if (!jcc_jp(y1, k_zero, 0x05)) y1 = static_cast<float>(static_cast<double>(y1) + k_360_00741868); // 0063997B..00639995
    float y2 = static_cast<float>(quaternion_yaw_004a9f40(reinterpret_cast<const W4CamQuaternion*>(fp(s, 0x28)))); // 00639999..006399A2
    if (!jcc_jp(y2, k_zero, 0x05)) y2 = static_cast<float>(static_cast<double>(y2) + k_360_00741868); // 006399A6..006399C0
    double diff;
    if (!jcc_jne(y1, k_270_0073e404, 0x01) && !jcc_jp(y2, k_90_0073e40c, 0x41)) {         // 006399C4..006399E4
        diff = (static_cast<double>(k_360_00741868) - y1) + y2;                            // 006399E6..006399F0
    } else if (!jcc_jp(y1, k_90_0073e40c, 0x41) && !jcc_jne(y2, k_270_0073e404, 0x01)) {  // 006399F6..00639A16
        diff = -((static_cast<double>(k_360_00741868) - y2) + y1);                         // 00639A18..00639A26
    } else {
        diff = static_cast<double>(y2) - y1;                                               // 00639A2A..00639A2E
    }
    bool to_180 = false;                                                                   // 00639A32..00639A64
    if (jcc_je(diff, k_wrap_hi_lo_0075163c, 0x41) && jcc_jnp(diff, k_wrap_hi_hi_00751638, 0x05)) to_180 = true;
    else if (!jcc_jp(diff, k_wrap_lo_hi_00751634, 0x05) && jcc_je(diff, k_wrap_lo_lo_00751630, 0x41)) to_180 = true;
    if (to_180) diff = k_180_0073e408;                                                     // 00639A66..00639A68
    else if (jcc_je(diff, k_180_0073e408, 0x41)) diff = diff - k_360_00741868;             // 00639A70..00639A7D
    else if (!jcc_jp(diff, k_minus_180_0074f4f0, 0x05)) diff = diff + k_360_00741868;      // 00639A85..00639A92
    double a = (diff * lf(s, 0x9C)) * delta;                                               // 00639A98..00639AA3
    if (lw(s, 0x60) != 0) {                                                                // 00639A9E..00639AA7
        sw(s, 0x60, 0);                                                                    // 00639AB2
        const W4CamQuaternion q = gob_orientation(target(s));                                 // 00639AA9..00639ABC
        std::memcpy(fp(s, 0x28), &q, 16);                                                  // 00639ABF..00639AD5
        a = quaternion_yaw_004a9f40(reinterpret_cast<const W4CamQuaternion*>(fp(s, 0x28)));  // 00639AD4..00639AD8
    }
    if (jcc_je(a, k_360_00741868, 0x41)) a = a - k_360_00741868;                           // 00639AE0..00639AED
    if (!jcc_jp(a, k_zero, 0x05)) a = a + k_360_00741868;                                  // 00639AF3..00639B00
    if (lw(s, 0x64) != 0) { sw(s, 0x64, 0); a = y2; }                                       // 00639B06..00639B16
    else a = a + y1;                                                                       // 00639B1C
    const W4CamQuaternion qb = axis_angle(up_axis(), static_cast<float>(a * k_deg_to_rad_007415f8)); // 00639B20..00639B4C
    const double m = vector_magnitude_004ab100(&t);                                        // 00639B51..00639B56
    const double e = m - lf(s, 0x110);                                                     // 00639B5B
    const double r = (e - static_cast<double>(new_position_scale_007a243c) * e) + lf(s, 0x110); // 00639B61..00639B6B
    const float zr = static_cast<float>(k_zero * r);                                       // 00639B71..00639B79
    const W4Vector offset = {zr, static_cast<float>(r), zr};                               // 00639B7D..00639BA4
    W4Vector rotated;
    quaternion_rotate_004a9bb0(&rotated, &qb, &offset);                                    // 00639BAE
    *out = W4Vector{static_cast<float>(static_cast<double>(c.x) - rotated.x), static_cast<float>(static_cast<double>(c.y) - rotated.y),
                    static_cast<float>(static_cast<double>(c.z) + lf(s, 0x120))};          // 00639BB3..00639C1C
    return out;
}

// 006391A0 Control_UpdateCameraDesiredOrientation(Quaternion* out, Quaternion q, delta) (RET 18h).
extern "C" W4CamQuaternion* __fastcall kotor_fastcall_006391a0(void* s, void*, W4CamQuaternion* out, W4CamQuaternion q, float delta) {
    sw(s, 0xA0, 0); sw(s, 0x64, 1);                                                        // 006391B7..006391BD
    (void)gob_position(target(s));                                                         // 006391A8..006391C3
    const float sens = static_cast<float>(kotor_fastcall_0061d5b0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr)); // 006391C6..006391DA
    W4CamQuaternion cur = q;                                                                  // 006391E4..0063920B
    if (jcc_jp(lf(s, 0x100), k_zero, 0x44) || jcc_jp(lf(s, 0x10C), k_zero, 0x44)) {        // 006391DE..00639226
        sw(s, 0x80, 0x7F000000u);                                                          // 00639344
    } else if (lw(s, 0x80) != 0x7F000000u) {                                               // 0063922C..00639237
        void* object = kotor_fastcall_005ed580(client_app(), nullptr, lw(s, 0x80));        // 0063923D..00639247
        if (object != nullptr) {                                                           // 0063924C..00639250
            const W4Vector tp = gob_position(target(s));                                   // 00639256..00639260
            const W4Vector d = {static_cast<float>(static_cast<double>(lf(object, 0x24)) - tp.x),
                                static_cast<float>(static_cast<double>(lf(object, 0x28)) - tp.y),
                                static_cast<float>(static_cast<double>(lf(object, 0x2C)) - tp.z)}; // 00639263..00639285
            W4Vector ydir, cross;
            quaternion_ydir_0044f420(&cur, nullptr, &ydir);                                // 00639269..00639289
            vector_cross_004ab210(&cross, &up_axis(), &ydir);                              // 0063928E..00639299
            const double dot = (static_cast<double>(cross.z) * d.z + static_cast<double>(cross.y) * d.y) + static_cast<double>(cross.x) * d.x; // 0063929E..006392C3
            float step = 15.0f;                                                            // 006392AD
            if (!jcc_jp(dot, k_zero_double_0073edf8, 0x05)) step = -15.0f;                 // 006392C5..006392D2 (dot < 0)
            if (!jcc_jp(std::fabs(dot), k_cos30_00751628, 0x05)) sw(s, 0x80, 0x7F000000u); // 006392DA..006392E9 (|dot| < 0.866)
            const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&d) - step);      // 006392F3..00639307
            cur = ypr(yaw, 0.0f, 0.0f);                                                    // 0063930A..0063932F
            sf(s, 0xA0, desired_turn_rate_007a2434);                                       // 00639327..00639333
            sw(s, 0x64, 0);                                                                // 00639339
            sw(s, 0xAC, 1);                                                                // 0063933C
        }
    }
    (void)kotor_fastcall_005eda40(client_app(), nullptr);                                  // 0063934E..00639357 (result unused)
    I32 free_look = kotor_input_cells_required::word_008338f0;                             // 00639365
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 8) & 2) free_look = free_look == 0 ? 1 : 0; // 0063935C..0063937D
    if (free_look != 0) {                                                                  // 0063937F..00639387
        if (lw(s, 0xB0) == 0) {                                                            // 00639389..0063938B
            gui_m48_manager_store_mouse_0040a3d0(kotor_input_cells::gui_manager_007a39f4, nullptr); // 0063938D..00639393
            sw(s, 0xB0, static_cast<U32>(free_look));                                      // 00639398
            kotor_fastcall_005ee170(client_app(), nullptr, 0x20);                          // 0063939E..006393A9 HideMouse
        }
    } else if (lw(s, 0xB0) != 0) {                                                         // 006393B0..006393B2
        gui_m48_manager_restore_mouse_0040c7d0(kotor_input_cells::gui_manager_007a39f4, nullptr); // 006393B4..006393BA
        sw(s, 0xB0, 0);                                                                    // 006393BF
        kotor_fastcall_005ee180(client_app(), nullptr, 0x20);                              // 006393C5..006393D0 ShowMouse
    }
    if (gui_m48_client_get_input_class_005eda70(client_app(), nullptr) == 2) {              // 006393D5..006393E5
        sw(s, 0xB0, 0);                                                                    // 006393E7
        kotor_fastcall_005ee180(client_app(), nullptr, 0x20);                              // 006393ED..006393F8
    }
    I32 turning = free_look;                                                               // 006393FF ebp
    if (free_look != 0 && lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0x30) == 0)     // 006393FD..00639414
        kotor_fastcall_005ee170(client_app(), nullptr, 0x20);                              // 00639416..00639420
    else
        kotor_fastcall_005ee180(client_app(), nullptr, 0x20);                              // 00639427..00639432
    float turn = static_cast<float>(x87_pow(k_one_double_0073ede8, turn_exponent_007a2448) * lf(s, 0x100)); // 00639437..0063944E
    bool rotate_free = false;                                                              // -> 0063953E
    bool rotate_limited = false;                                                           // -> 0063962F
    if (!jcc_jp(k_zero, lf(s, 0x100), 0x44) && free_look == 0) {                           // 00639452..0063946B ([100] == 0, no free look)
        W4RK4State state;
        kotor_fastcall_006d1190(static_cast<U8*>(s) + 0xB4, nullptr, &state, W4Vector{0.0f, lf(s, 0x10C), 0.0f}, delta, 1, 0, 0, 0); // 0063946D..006394AD
        turn = state.velocity.y;                                                           // 006394B2..006394B5
        turning = jcc_je(turn, k_zero, 0x41) ? 1 : 0;                                      // 006394B9..006394D3 (turn > 0)
        if (jcc_jnp(turn, k_zero, 0x05)) rotate_free = true;                               // 006394D3..006394E2 (turn < 0) -> 0063953E
        else rotate_limited = true;                                                        // -> 0063962F
    } else {
        W4RK4State state;                                                                  // 006394E9..0063952B
        const float scaled = static_cast<float>(static_cast<double>(turn) / sens);
        kotor_fastcall_006d1190(static_cast<U8*>(s) + 0xB4, nullptr, &state, W4Vector{0.0f, scaled, 0.0f}, delta, 1, 0, 0, 0);
        sw(s, 0x10C, 0);                                                                   // 00639532
        if (free_look != 0) rotate_free = true;                                            // 00639530..00639538
    }
    if (rotate_free) {                                                                     // 0063953E
        double v;
        if (orientation_debug_0083394c != 0 && free_look == 0) {                           // 0063953E..00639548
            v = turn;                                                                      // 0063954A
        } else if (turn_sign_00832920 != 0) {                                             // 00639593..006395A6
            v = jcc_jne(k_zero, turn, 0x41) ? static_cast<double>(turn) : static_cast<double>(k_zero); // 006395A8..006395B5
        } else {
            v = jcc_jp(k_zero, turn, 0x05) ? static_cast<double>(turn) : static_cast<double>(k_zero);  // 006395DB..006395E8
        }
        const W4CamQuaternion r = ypr(static_cast<float>(-v), 0.0f, 0.0f);                    // 00639553..006395FE
        cur = qmul(r, cur);                                                                // 0063956E / 0063960C
        rotate_limited = true;                                                             // falls to 0063962F
    }
    if (rotate_limited && turning != 0) {                                                  // 0063962F..00639631
        double v;
        if (orientation_debug_0083394c != 0 && free_look == 0) {                           // 00639637..00639641
            v = turn;                                                                      // 00639672
        } else if (turn_sign_00832920 != 0) {                                              // 00639643..00639656
            v = jcc_jp(k_zero, turn, 0x05) ? static_cast<double>(turn) : static_cast<double>(k_zero);  // 00639658..0063965D
        } else {
            v = jcc_jne(k_zero, turn, 0x41) ? static_cast<double>(turn) : static_cast<double>(k_zero); // 00639665..0063966A
        }
        const W4CamQuaternion r = ypr(static_cast<float>(-v), 0.0f, 0.0f);                    // 00639676..00639688
        cur = qmul(r, cur);                                                                // 00639690..00639696
    }
    sw(s, 0x100, 0);                                                                       // 006396C0
    const W4Vector tp = gob_position(target(s));                                           // 006396B9..006396C9
    std::memcpy(fp(s, 0x44), &tp, 12);                                                     // 006396CC..006396DC
    if (!jcc_jne(pending_yaw_007a2440, k_zero, 0x01)) {                                    // 006396DF..006396F0 (pending >= 0)
        cur = ypr(pending_yaw_007a2440, 0.0f, 0.0f);                                       // 006396F2..0063971C
        pending_yaw_007a2440 = -1.0f;                                                      // 0063971F
    }
    *out = cur;                                                                            // 0063972F..0063974C
    return out;
}

// 00638F60 Control_CheckAndUpdateCameraStyle.
extern "C" void __fastcall kotor_fastcall_00638f60(void* s, void*) {
    sf(s, 0xF4, static_cast<float>(kotor_fastcall_0061d5e0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr))); // 00638F60..00638F77
    sf(s, 0xF8, static_cast<float>(kotor_fastcall_0061d610(kotor_fastcall_005ed700(client_app(), nullptr), nullptr))); // 00638F7D..00638F92
    sf(s, 0xFC, static_cast<float>(kotor_fastcall_0061d640(kotor_fastcall_005ed700(client_app(), nullptr), nullptr))); // 00638F98..00638FAD
    if (style_request_007a242c != -1) {                                                    // 00638FB3..00638FBB
        kotor_fastcall_00638070(s, nullptr, style_request_007a242c);                       // 00638FBD..00638FC0
        style_request_007a242c = -1;                                                       // 00638FC5
    }
}

// 00638FE0 Control_SetCameraInintalState.
extern "C" void __fastcall kotor_fastcall_00638fe0(void* s, void*) {
    const W4CamQuaternion q = gob_orientation(target(s));                                     // 00638FE7..00638FF2
    const W4Vector v = quat_zdir_inline(q);                                                // 00638FF5..0063904A
    const float pitch = static_cast<float>(static_cast<double>(lf(s, 0x11C)) + lf(s, 0x90)); // 0063904E..0063905A
    W4Vector n;
    vector_normalized_004ab180(&n, &v);                                                    // 0063905D..0063905F
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&n) + lf(s, 0x90));       // 00639064..00639077
    const W4CamQuaternion o = ypr(yaw, pitch, 0.0f);                                          // 0063907A..0063907B
    std::memcpy(fp(s, 0x28), &o, 16);                                                      // 00639080..00639098
    gob_set_orientation(owner(s), o);                                                      // 0063909B..006390BD
    W4Vector p = gob_position(target(s));                                                  // 006390C0..006390CA
    p.z = static_cast<float>(static_cast<double>(p.z) + k_one_and_half_0073f414);          // 006390CD..006390E0
    const W4Vector w = quat_ydir_inline(q);                                                // 006390E4..0063912E
    vector_subtract_assign_004a9770(&p, nullptr, &w);                                      // 00639132
    gob_set_position(owner(s), p);                                                         // 00639137..0063915A
    std::memcpy(fp(s, 0x50), &p, 12);                                                      // 0063915D..00639173
    std::memcpy(fp(s, 0x44), &p, 12);
    std::memcpy(fp(s, 0x38), &p, 12);
}

// 00637C20 ResetLastPosition(Vector position, Quaternion q) (RET 1Ch).
extern "C" void __fastcall kotor_fastcall_00637c20(void* s, void*, W4Vector position, W4CamQuaternion q) {
    std::memcpy(fp(s, 0x38), &position, 12);                                               // 00637C2D..00637C40
    std::memcpy(fp(s, 0x50), &position, 12);                                               // 00637C43..00637C4B
    std::memcpy(fp(s, 0x28), &q, 16);                                                      // 00637C4E..00637C6D
    gob_set_position(owner(s), position);                                                  // 00637C5E..00637C80
    gob_set_orientation(owner(s), q);                                                      // 00637C83..00637CAF
}

// 00637CC0 CameraFreeStyleControl (debug free-style camera; reached only when [00833938] != 0).
extern "C" U32 __fastcall kotor_fastcall_00637cc0(void*, void*, float) {
    kotor_unreachable_trap(0x00637CC0u, "CSWCameraOnAStick::CameraFreeStyleControl (debug flag 00833938)");
}

// 0063BCB0 CSWCameraOnAStick::Control(float delta) (slot 08h, RET 4).
extern "C" U32 __fastcall kotor_fastcall_0063bcb0(void* s, void*, float delta) {
    if (lw(s, 0x84) != 0) {                                                                // 0063BCB8..0063BCC1
        float scaled;                                                                      // [esp+10h]
        x87trig_0063bcc3(s, &scaled);                                                      // 0063BCC3..0063BCDE (x87)
        void* camera = vcall<void*>(lp(s, 0x1C), 0x04);                                    // 0063BCC9..0063BCE2
        const double view = vcall<double>(camera, 0x48);                                   // 0063BCE5..0063BCE9 GetViewAngle
        x87trig_0063bcec(view, &scaled, s);                                                // 0063BCEC..0063BCFA (x87)
    }
    kotor_fastcall_00638f60(s, nullptr);                                                   // 0063BD00..0063BD02
    if (behavior_control_0049f270(s, nullptr, delta) == 0) return 1;                       // 0063BD07..0063BD18
    if (free_style_debug_00833938 != 0) return kotor_fastcall_00637cc0(s, nullptr, delta); // 0063BD1E..0063BD36
    if (lw(s, 0x14) == 0) {                                                                // 0063BD39..0063BD3E
        vcall<void, void*>(owner(s), 0x58, nullptr);                                       // 0063BD40..0063BD49 SetBehavior(0)
        return 1;
    }
    if (!jcc_jne(lf(s, 0x44), k_flt_max_0074c468, 0x01)) kotor_fastcall_00638fe0(s, nullptr); // 0063BD5B..0063BD6D
    {
        W4CamQuaternion q;
        std::memcpy(&q, fp(s, 0x28), 16);
        W4CamQuaternion r;
        kotor_fastcall_006391a0(s, nullptr, &r, q, delta);                                 // 0063BD72..0063BD9D
        std::memcpy(fp(s, 0x28), &r, 16);                                                  // 0063BDA2..0063BDB8
    }
    const W4Vector camera_position = gob_position(owner(s));                               // 0063BDB5..0063BDC2
    W4Vector head;
    kotor_fastcall_005f5180(s, nullptr, &head);                                            // 0063BDC5..0063BDCC
    W4Vector desired;
    kotor_fastcall_00637af0(s, nullptr, &desired, head);                                   // 0063BDD1..0063BDF1
    const W4Vector camera_position2 = gob_position(owner(s));                              // 0063BDF6..0063BE00
    sf(s, 0x9C, static_cast<float>(kotor_fastcall_00637b80(s, nullptr, lf(s, 0x9C), accel_interp_min_007a2430, lf(s, 0xA0),
                                                          camera_position2, desired, delta))); // 0063BE03..0063BE4E
    W4Vector next;
    kotor_fastcall_006398f0(s, nullptr, &next, desired, camera_position, head, delta);     // 0063BE54..0063BE9F
    W4Vector hit;
    kotor_fastcall_0063b050(s, nullptr, &hit, next);                                       // 0063BEA4..0063BEC0
    if (dialog_camera_debug_008327ec != 0 && lw(s, 0x10) != 0) {                           // 0063BECD..0063BEDF
        // 0063BEE1..0063BF28: debug distance probe; every result is discarded (fstp st(0)).
        (void)gob_position(owner(s)); (void)gob_position(owner(s)); (void)gob_position(owner(s)); // 0063BEE1..0063BF02
        const W4Vector p4 = gob_position(owner(s));                                        // 0063BF05..0063BF14
        W4Vector diff;
        kotor_cdecl_00423bc0(&diff, &p4, &head);                                           // 0063BF17..0063BF1D (pos - head)
        (void)vector_magnitude_004ab100(&diff);                                            // 0063BF22..0063BF28
    }
    gob_set_position(owner(s), hit);                                                       // 0063BF2D..0063BF48
    W4CamQuaternion o;
    kotor_fastcall_00639760(s, nullptr, &o, delta);                                        // 0063BF4B..0063BF53
    gob_set_orientation(owner(s), o);                                                      // 0063BF58..0063BF80
    if (lw(s, 0x60) != 0) sw(s, 0x60, 0);                                                  // 0063BF83..0063BF8A
    return 1;                                                                              // 0063BF94
}
