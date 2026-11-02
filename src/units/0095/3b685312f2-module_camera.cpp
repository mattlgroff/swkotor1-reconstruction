// Lane W4 G14: CSWCModule camera controls (Tilt/Turn/Zoom/Scroll/BumpAndShake/UpdateCameraFollow/
// UpdateCameraCollision dispatch/SetCameraFollow). Transcribed from the original disassembly (dis/<ADDR>.asm).
// Module fields: +0Ch camera mode, +40h Aurora Camera*, +48h area, +70h follow id, +74h..+7Ch saved dist/yaw/pitch,
// +80h camera-dirty flags, +8Ch clear-view request, +94h/+98h/+9Ch tilt/turn/zoom mirrors, +D5h last options mode.
#include "cf86fc3cd2-camera.hpp"
#include "b5015298e2-camera_stick.hpp"
// L3 MATH-TRIG-CPP t1 a2 (2026-10-26): the whole unit is built as a long double (LD) unit - recipe + /clang:-mno-sse
// /clang:-ffp-model=strict /clang:-mlong-double-80 (PLAN-v3 1.1), so every FP function of it is written in the PLAN-v3
// writing rule (section 6):
//   * a value the original keeps on the x87 stack is a long double; every original fstp/fst dword is one explicit
//     (float); a float the original stores to a stack slot and uses again after calls is a volatile StoredFloat (one
//     store, read back as its word), so no x87 value is spilled narrower than 80 bits (spill gate S1);
//   * no float arithmetic; comparisons spelled from the original's fnstsw mask (ST0 against the operand:
//     test 44h;jp taken unless equal; test 05h;jp taken unless less; test 41h;jne taken unless greater;
//     test 01h;jne taken if less or unordered);
//   * math through kotor_math.h (L1); the two _CIpow sites of 006433B0 (00643672, 0064374D) call the L1
//     pow(long double, long double) directly (no project wrapper); no inline asm (the a1 helpers of 0063FE10 /
//     00640090 held CALLs inside MS inline asm, a clang-cl clobber bug class: they are C++ now);
//   * every float the original copies with integer moves (an argument, a field, a by-value vector) is copied as a word:
//     by-value vectors / quaternions are passed as the word structs V3W / Q4W through the callee's word-typed form;
//   * every helper is __forceinline (the recipe is /Ob0); the w4 float helpers (lf/sf/bits_f/jcc_*) are not used.
#include "../../math/kotor_math.h"
#include <intrin.h>   // _AddressOfReturnAddress (00640240, a2)

using namespace w4;
using namespace w4cam;
namespace w4cam {
// a2: the push-collision state cells and the two chase cells. module_camera is their only definer (c65 kotor.map:
// push_* in module_camera.obj; 007A2460 / 007A2464 have no symbol in the c65 link). They are COFF weak definitions:
// in a link this object's definition is the one used (no other definer), while the value gate binds the references
// to the original cells (spec production "binds") and draws them per case on both sides alike.
#define W4CAM_CELL __attribute__((weak))
W4CAM_CELL w4::U32 push_area_flag_00833a64 = 0;
W4CAM_CELL float push_last_distance_00833a7c = 0.0f;
W4CAM_CELL float push_cell_00833a80 = 0.0f;
W4CAM_CELL float push_cell_00833a84 = 0.0f;
W4CAM_CELL float push_cell_00833a88 = 0.0f;
W4CAM_CELL w4::U32 push_state_00833a8c = 0;
// two read-only .data cells of the chase / EQ camera modes (initial values = the original .data; the only original
// references are the reads 006409EA, 00640A3F, 00640F2B: no writer exists in the image, own xref scan).
W4CAM_CELL float chase_turn_limit_007a2460 = 30.0f;
W4CAM_CELL float chase_turn_scale_007a2464 = 1.0f;
#undef W4CAM_CELL
// 004ACB90 Roll (camera_stick.cpp, C++ linkage; its argument a quaternion as four raw words).
struct RollQuaternion { U32 w, x, y, z; };
long double roll_004acb90(RollQuaternion q);
}  // namespace w4cam


extern "C" {
U32 __fastcall kotor_fastcall_00610490(void*, void*); // GetFreeLookVideoEffect, RET0
void* __cdecl pointto_004acca0(void*,const void*);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*,void*);
void __fastcall kotor_fastcall_006d1870(void*, void*, float); // CSWBehaviorCameraCombat::SetTurn (RET4)
U16 __fastcall kotor_m15a_object_get_current_animation_0063c060(void*, void*); // CSWCObject::GetCurrentAnimation (v5 0817, AX)
void __fastcall w1x_object_set_orientation_004cd170(void*,void*,void*);
}

namespace {
const float k_zero = 0.0f;
const float k_one_0073d764 = 1.0f;
const float k_half_0073e9ac = 0.5f;
const float k_thirty_007455dc = 30.0f, k_minus_thirty_0074fc4c = -30.0f;
const float k_minus_sixty_0074fc48 = -60.0f;
const float k_scroll_epsilon_00747504 = 9.999999747378752e-06f;   // bits 3727C5AC
const double k_two_pi_00751840 = 6.2831852;                       // qword 00751840
const float k_thousand_0073d6fc = 1000.0f;
const float k_deg_to_rad_007415f8 = 0.01745329238474369f;         // bits 3C8EFA35
const float k_minus_180_0074f4f0 = -180.0f, k_180_0073e408 = 180.0f, k_360_00741868 = 360.0f;
const float k_90_0073e40c = 90.0f, k_minus_90_0074fc44 = -90.0f, k_five_00743264 = 5.0f;
const double k_one_double_0073ede8 = 1.0;
const float k_eps_00741870 = 9.999999747378752e-05f;              // bits 38D1B717
const float k_milli_0073d708 = 0.0010000000474974513f;            // bits 3A83126F
const float k_three_and_half_007518bc = 3.5f;
const double k_two_00741df8 = 2.0, k_half_0073f3d0 = 0.5;
const float k_minus_half_00743884 = -0.5f, k_750_0075183c = 750.0f;

struct V3W { U32 x, y, z; };
struct Q4W { U32 w, x, y, z; };
static_assert(sizeof(V3W) == sizeof(W4Vector) && sizeof(Q4W) == sizeof(W4CamQuaternion), "word forms");
__forceinline V3W words3(const void* p) { V3W v; std::memcpy(&v, p, 12); return v; }
__forceinline Q4W words4(const void* p) { Q4W q; std::memcpy(&q, p, 16); return q; }
__forceinline U32 word(const void* p) { U32 w; std::memcpy(&w, p, 4); return w; }
template <class T> __forceinline T& at(void* o, U32 off) { return *reinterpret_cast<T*>(static_cast<U8*>(o) + off); }
__forceinline long double ld(const float& f) { return f; }                       // fld dword
// A float the original stores (fstp dword) to a stack slot and uses again after calls: one volatile store, read back
// as its word (copied / pushed) or loaded from that word (fld / memory operand).
struct StoredFloat {
    volatile float f;
    __forceinline U32 word() const { return *reinterpret_cast<const volatile U32*>(&f); }
    __forceinline void set_word(U32 w) { *reinterpret_cast<volatile U32*>(&f) = w; }
    __forceinline long double value() const { const U32 w = word(); float g; std::memcpy(&g, &w, 4); return g; }
};
// A float argument read from its stack word each time it is used (a mov, then fld): the original reads the argument
// slot itself at every use, and a float parameter clang holds in an x87 register is spilled as 4 bytes (S1).
__forceinline long double word_ld(U32 w) { float f; std::memcpy(&f, &w, 4); return f; }
__forceinline float word_f(U32 w) { float f; std::memcpy(&f, &w, 4); return f; }
__forceinline void* vfn(void* object, U32 slot) { return *reinterpret_cast<void**>(*static_cast<U8**>(object) + slot); }
__forceinline double vcall_st0(void* object, U32 slot) { return reinterpret_cast<double(__thiscall*)(void*)>(vfn(object, slot))(object); }
__forceinline void* vcall_ptr(void* object, U32 slot) { return reinterpret_cast<void*(__thiscall*)(void*)>(vfn(object, slot))(object); }
// A direct callee through its word-typed form (the same stack bytes, so the same ABI and cleanup), through a volatile
// code address so the optimizer cannot turn the words back into the callee's float parameters (measured a2).
template <class F, class G> __forceinline F word_form(G* callee) {
    void* volatile address = reinterpret_cast<void*>(callee);
    return reinterpret_cast<F>(address);
}
inline void* behavior(void* camera, I32 type) { return vcall<void*, I32>(camera, 0x80, type); } // Camera::GetBehavior
inline void set_behavior(void* camera, void* b) { vcall<void, void*>(camera, 0x7C, b); }        // Camera::SetBehavior
// Default-mode stick lookup used by Tilt/Turn/AcclTurn/Zoom: GetBehavior(-1)->vtbl[1Ch]().
void* default_stick(void* camera) {
    if (camera == nullptr) return nullptr;
    void* b = behavior(camera, -1);
    if (b == nullptr) return nullptr;
    return vcall<void*>(b, 0x1C);
}
// Camera +40 SetNearFar(near, far): the near value is the word GetNearFar (+3C) wrote (00640822..00640831: push 42340000 ;
// push [esp+..]).
__forceinline void camera_set_near_45(void* camera, U32 near_word) {
    reinterpret_cast<void(__thiscall*)(void*, U32, U32)>(vfn(camera, 0x40))(camera, near_word, 0x42340000u);
}
}  // namespace

// 0063FCC0 CSWCModule::TiltCamera(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_0063fcc0(void* module, void*, float a) {
    // the argument word itself (a mov copies it in modes 3 / 5: a float parameter would be loaded and stored, quieting an sNaN)
    const volatile U32* const args = reinterpret_cast<const volatile U32*>(static_cast<const U8*>(_AddressOfReturnAddress()) + 4);
    const U32 mode = lw(module, 0x0C);                                                     // 0063FCC3
    if (mode == 3) {                                                                       // 0063FCC6..0063FCCC
        void* b = behavior(lp(module, 0x40), 0x106A);
        if (b != nullptr) at<U32>(b, 0x104) = args[0];                                       // 0063FCE3..0063FCE7 (mov)
        return;
    }
    if (mode == 5) {                                                                       // 0063FCF1..0063FCF4
        void* b = behavior(lp(module, 0x40), 0x106E);
        if (b != nullptr) at<U32>(b, 0x38) = args[0];                                      // 0063FD0B..0063FD0F (mov)
        return;
    }
    if (mode == 7) {                                                                       // 0063FD16..0063FD19
        void* b = behavior(lp(module, 0x40), 0x1071);
        if (b != nullptr) at<float>(b, 0x18) = static_cast<float>(ld(a) * k_minus_sixty_0074fc48); // 0063FD2C..0063FD37
        return;
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 0063FD3D..0063FD58
        const float v = static_cast<float>(ld(a) + at<float>(stick, 0x48));               // 0063FD5A..0063FD5E
        at<float>(stick, 0x48) = v;                                                        // 0063FD61 fst
        at<float>(module, 0x94) = v;                                                       // 0063FD64 fstp (same rounding)
    }
    // 0063FD6A..0063FD9C: +80h |= (a == 0) ? 0 : 1 (test 44h ; jnp: a NaN ORs 1)
    at<U32>(module, 0x80) = at<U32>(module, 0x80) | ((ld(a) == k_zero) ? 0u : 1u);
}

// 0063FDB0 CSWCModule::AcclTiltCamera(float) (thiscall, RET 4). Returns EAX as the original leaves it.
extern "C" U32 __fastcall kotor_fastcall_0063fdb0(void* module, void*, float a) {
    const U32 mode = lw(module, 0x0C);                                                     // 0063FDB0
    if (mode == 3) return ad(behavior(lp(module, 0x40), 0x106A));                          // 0063FDB3..0063FDC5 tail jmp
    if (mode == 5) {                                                                       // 0063FDCB..0063FDCE
        void* b = behavior(lp(module, 0x40), 0x106E);                                      // 0063FDD0..0063FDDA
        if (b != nullptr) at<float>(b, 0x24) = static_cast<float>(-ld(a));                // 0063FDE4..0063FDEA
        return ad(b);
    }
    if (mode == 7) return ad(behavior(lp(module, 0x40), 0x1071));                          // 0063FDF0..0063FE02 tail jmp
    return mode;                                                                           // 0063FDED (eax = mode)
}

// 0063FE10 CSWCModule::TurnCamera(float a, float b) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0063fe10(void* module, void*, U32 a_bits, U32 b_bits) {
    float a, b;                                                                            // [esp+2Ch] / [esp+30h]
    std::memcpy(&a, &a_bits, 4); std::memcpy(&b, &b_bits, 4);
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0) {                             // 0063FE10..0063FE29
        // 0063FE2B..0063FE53: combat camera (behaviour 1070h, CSWBehaviorCameraCombat::SetTurn 006D1870).
        void* const c = behavior(lp(module, 0x40), 0x1070);                                // 0063FE2B..0063FE32
        if (c == nullptr) return;                                                          // 0063FE38..0063FE3A
        const float turn = static_cast<float>(-ld(a));                                     // 0063FE40..0063FE49 fchs ; fstp dword
        at<U32>(c, 0x2C) = 0;                                                              // 0063FE4C
        kotor_fastcall_006d1870(c, nullptr, turn);                                         // 0063FE53 (RET4)
        return;                                                                            // 0063FE58
    }
    const U32 mode = lw(module, 0x0C);                                                     // 0063FE60
    if (mode == 3) {                                                                       // 0063FE63..0063FE66
        void* s = behavior(lp(module, 0x40), 0x106A);                                      // 0063FE68..0063FE6F
        if (s != nullptr) { at<U32>(s, 0x100) = a_bits; at<U32>(s, 0x10C) = 0; }          // 0063FE7D..0063FE87
        return;
    }
    if (mode == 5) {                                                                       // 0063FE99..0063FE9C
        void* const free_look = behavior(lp(module, 0x40), 0x106E);                        // 0063FEA2..0063FEA9
        if (free_look == nullptr) return;                                                  // 0063FEB3
        // 0063FEB9..0063FF1D: h = a * 0.5 ; clamp to [-30, 30] on the stack (a NaN stays) ;
        // angle = -(free_look+28h * b * h) -> fstp dword [esp+2Ch] (over the argument a).
        const long double h = ld(a) * k_half_0073e9ac;
        long double v;
        if (k_thirty_007455dc < h) v = k_thirty_007455dc;                                  // 0063FEC3..0063FED0 / 0063FEEF..0063FF00
        else if (k_minus_thirty_0074fc4c > h) v = k_minus_thirty_0074fc4c;                 // 0063FED2..0063FEE7
        else v = h;
        StoredFloat angle;
        angle.f = static_cast<float>(-((ld(at<float>(free_look, 0x28)) * b) * v));         // 0063FF06..0063FF19
        void* const cc = client_get_player_creature_005ed540(client_app(), nullptr);        // 0063FF09..0063FF1F
        if (cc == nullptr) return;                                                         // 0063FF26..0063FF28
        W4CamQuaternion q;
        at<U32>(cc, 0x448) = 0;                                                            // 0063FF37
        pointto_004acca0(&q, static_cast<U8*>(cc) + 0x30);                                 // 0063FF2E..0063FF41
        // 0063FF46..0063FF91: Roll(q) -> fstp dword (roll argument) ; Pitch(&q) -> fstp dword ; Yaw(&q) + angle ->
        // fstp dword ; YawPitchRoll(rotation, yaw, pitch, roll) (the three stored words).
        w4cam::RollQuaternion rq;
        std::memcpy(&rq, &q, 16);
        StoredFloat roll, pitch;
        roll.f = static_cast<float>(w4cam::roll_004acb90(rq));
        pitch.f = static_cast<float>(quaternion_pitch_004aa020(&q));
        const float yaw = static_cast<float>(static_cast<long double>(quaternion_yaw_004a9f40(&q)) + angle.value());
        W4CamQuaternion out;
        const W4CamQuaternion* r = static_cast<const W4CamQuaternion*>(
            word_form<void*(__cdecl*)(void*, U32, U32, U32)>(&yaw_pitch_roll_004acac0)(&out, word(&yaw), pitch.word(), roll.word()));
        W4CamQuaternion rotation;
        std::memcpy(&rotation, r, 16);                                                     // 0063FF9A..0063FFBF (mov)
        W4Vector direction;
        reinterpret_cast<void(__thiscall*)(void*, void*)>(vfn(cc, 0x88))(cc, quaternion_zdir_0043e250(&rotation, nullptr, &direction)); // 0063FFB6..0063FFCB
        if (kotor_m15a_creature_get_server_creature_0060fb20(cc, nullptr) == nullptr) return; // 0063FFD1..0063FFDA
        void* const server_direction = quaternion_zdir_0043e250(&rotation, nullptr, &direction); // 0063FFE0..0063FFE9
        // The original deliberately performs a second lookup after the direction call.
        w1x_object_set_orientation_004cd170(kotor_m15a_creature_get_server_creature_0060fb20(cc, nullptr), nullptr, server_direction); // 0063FFEE..0063FFF8
        return;
    }
    if (mode == 7) {                                                                       // 00640005..00640008
        void* s = behavior(lp(module, 0x40), 0x1071);                                      // 0064000A..00640011
        if (s != nullptr) at<float>(s, 0x14) = static_cast<float>(ld(a) * k_minus_sixty_0074fc48); // 0064001B..00640025
        return;
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 00640030..0064004B
        const float v = static_cast<float>(ld(a) + at<float>(stick, 0x40));               // 0064004D..00640051
        at<float>(stick, 0x40) = v;                                                        // 00640054 fst
        at<float>(module, 0x98) = v;                                                       // 00640057 fstp
    }
    if (!(ld(a) == k_zero)) {                                                              // 0064005D..0064006C (test 44h ; jnp)
        at<U32>(module, 0x100) = 1;                                                        // 0064007B
        at<U32>(module, 0x80) = at<U32>(module, 0x80) | 1u;                                // 0064006E..00640081
    }
}

// 00640090 CSWCModule::AcclTurnCamera(float a, float b) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_00640090(void* module, void*, U32 a_bits, U32) {
    float a;                                                                               // [esp+8]
    std::memcpy(&a, &a_bits, 4);
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0) {                             // 00640090..006400A5
        // 006400A7..006400CD: combat camera (behaviour 1070h, SetTurn 006D1870(0); then +2C = -a).
        void* const c = behavior(lp(module, 0x40), 0x1070);                                // 006400A7..006400AE
        if (c == nullptr) return;                                                          // 006400B6..006400B8
        kotor_fastcall_006d1870(c, nullptr, 0.0f);                                         // 006400BE..006400C2 (push 0, RET4)
        at<float>(c, 0x2C) = static_cast<float>(-ld(a));                                   // 006400C7..006400CD
        return;                                                                            // 006400D0
    }
    const U32 mode = lw(module, 0x0C);                                                     // 006400D4
    if (mode == 3) {                                                                       // 006400D7..006400DA
        void* s = behavior(lp(module, 0x40), 0x106A);                                      // 006400DC..006400E3
        if (s != nullptr) { at<U32>(s, 0x100) = 0; at<U32>(s, 0x10C) = a_bits; }          // 006400F1..006400FF
        return;
    }
    if (mode == 5) {                                                                       // 00640109..0064010C
        if (behavior(lp(module, 0x40), 0x106E) == nullptr) return;                         // 0064010E..0064011B
        void* const cc = client_get_player_creature_005ed540(client_app(), nullptr);        // 00640121..0064012B
        if (cc == nullptr) return;
        at<float>(cc, 0x448) = static_cast<float>(-ld(a));                                 // 00640138..0064013F
        return;
    }
    if (mode == 7) {                                                                       // 00640148..0064014B
        void* s = behavior(lp(module, 0x40), 0x1071);                                      // 0064014D..00640154
        if (s != nullptr) at<float>(s, 0x14) = static_cast<float>(ld(a) * k_minus_sixty_0074fc48); // 0064015E..00640169
        return;
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 0064016F..0064018A
        const float v = static_cast<float>(ld(a) + at<float>(stick, 0x40));               // 0064018C..00640190
        at<float>(stick, 0x40) = v;                                                        // 00640193 fst
        at<float>(module, 0x98) = v;                                                       // 00640196 fstp
    }
    if (!(ld(a) == k_zero)) {                                                              // 0064019C..006401AB
        at<U32>(module, 0x100) = 1;                                                        // 006401BA
        at<U32>(module, 0x80) = at<U32>(module, 0x80) | 1u;                                // 006401AD..006401C0
    }
}

// 006401D0 CSWCModule::ZoomCamera(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006401d0(void* module, void*, float a) {
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 006401D3..006401F1
        const float v = static_cast<float>(ld(a) + at<float>(stick, 0x34));               // 006401F3..006401F7
        at<float>(stick, 0x34) = v;                                                        // 006401FA fst
        at<float>(module, 0x9C) = v;                                                       // 006401FD fstp
    }
    if (!(ld(a) == k_zero)) {                                                              // 00640203..00640212
        at<U32>(module, 0x104) = 1;                                                        // 00640221
        at<U32>(module, 0x80) = at<U32>(module, 0x80) | 1u;                                // 00640214..00640227
    }
}

// 00640240 CSWCModule::ScrollCamera(Vector) (thiscall, Vector by value at [esp+4], RET 0Ch).
extern "C" void __fastcall kotor_fastcall_00640240(void* module, void*, W4Vector) {
    const W4Vector* const v = reinterpret_cast<const W4Vector*>(static_cast<const U8*>(_AddressOfReturnAddress()) + 4);
    // 00640243..00640279: (z*z + y*y) + x*x ; fcomp eps ; test 41h ; jnp -> return when m <= eps (a NaN goes on).
    const long double m = (ld(v->z) * v->z + ld(v->y) * v->y) + ld(v->x) * v->x;
    if (!(m > k_scroll_epsilon_00747504) && !(m != m)) return;
    W4Vector tmp;
    const W4Vector* got = reinterpret_cast<W4Vector*(__thiscall*)(void*, W4Vector*)>(vfn(lp(module, 0x40), 0x20))(lp(module, 0x40), &tmp); // 00640288..00640292
    W4Vector p;
    std::memcpy(&p, got, 12);                                                              // 00640295..006402AE (mov)
    vector_add_assign_004a9740(&p, nullptr, v);                                            // 0064029E..006402B2 (the argument itself)
    W4Vector returned;                                                                     // [esp+20h] hidden return slot
    reinterpret_cast<void(__thiscall*)(void*, W4Vector*, V3W)>(vfn(lp(module, 0x40), 0x18))(lp(module, 0x40), &returned, words3(&p)); // 006402B7..006402DA
    at<U32>(module, 0x80) = 1;                                                             // 006402DD
}

// 00641760 CSWCModule::UpdateCameraBumpAndShake(float delta) (thiscall, RET 4).
// L3 t1: PLAN-v3 writing rule. The shake amplitude 00641890..006418C8 keeps cos (006418A2) live at 64 bits across
// sin (006418B4) - both long double; the (float) conversions are the original's fstp dword stores.
extern "C" void __fastcall kotor_fastcall_00641760(void* module, void*, float delta) {
    void* b_stick = behavior(at<void*>(module, 0x40), 0x106A);                             // 00641768..00641773 ebx
    void* b_106d = behavior(at<void*>(module, 0x40), 0x106D);                              // 00641779..00641785 [esp+20h]
    void* b_free = behavior(at<void*>(module, 0x40), 0x106E);                              // 0064178B..0064179B [esp+14h]
    void* b_combat = behavior(at<void*>(module, 0x40), 0x1070);                            // 006417A1..006417AF [esp+1Ch]
    if (!b_stick && !b_106d && !b_free && !b_combat) return;                               // 006417B5..006417CB
    if ((lb(kotor_fastcall_005ed700(client_app(), nullptr), 0) & 4) == 0) {                // 006417D1..006417E1
        if (at<U32>(module, 0xF0) != 0) {                                                  // 006417E3..006417E9
            at<U32>(module, 0xF0) = 0; at<U32>(module, 0xF4) = 0; at<U32>(module, 0xF8) = 1; // 006417EB..006417F7
        } else if (at<I32>(module, 0xDC) > 0) {                                            // 00641803..00641809
            at<U32>(module, 0xDC) = 0; at<U32>(module, 0xE4) = 0; at<U32>(module, 0xE0) = 0; at<U32>(module, 0xEC) = 1; // 0064180B..0064181D
        }
    }
    // 00641827..00641831: fld delta ; fmul dword [0073D6FC] ; call __ftol2 (int64 truncation of ST0, low dword).
    I32 ms = static_cast<I32>(static_cast<long long>(static_cast<long double>(delta) * k_thousand_0073d6fc));
    const I32 remaining = at<I32>(module, 0xDC);                                           // 00641836 ([esp+28h])
    if (remaining <= 0) return;                                                            // 0064183C..00641844
    const I32 wait = at<I32>(module, 0xE4);                                                // 0064184A
    if (wait > 0) {                                                                        // 00641850..00641852
        if (wait <= ms) { ms -= wait; at<U32>(module, 0xE4) = 0; }                         // 00641854..0064185A
        else at<I32>(module, 0xE4) = wait - ms;                                            // 00641862..00641864
    }
    if (at<U32>(module, 0xE4) != 0) return;                                                // 0064186A..00641870
    long double second;                                                                    // ST0 at 0064190E
    U32 first_bits;                                                                        // [esp+28h] (copied with mov)
    if (remaining <= ms) {                                                                 // 00641876..00641878
        second = k_zero;                                                                   // 0064187A fld [0073D700]
        at<U32>(module, 0xDC) = 0;                                                         // 00641880
        first_bits = 0;                                                                    // 00641886
    } else {
        // 00641890..006418C8: fild [esp+28h] ; fmul qword [00751840] ; fild [module+E8h] ; fdivr st,st(1) ; fcos ;
        // fadd [0073D764] ; fld [007A246C] ; fdivp st(2),st ; fxch ; fsin ; fxch ; fxch ; fmulp ; fmul [007A2470] ;
        // fmul [007415F8] ; fstp dword [esp+10h].
        const long double t = static_cast<long double>(remaining) * k_two_pi_00751840;
        const long double c = cos(t / static_cast<long double>(at<I32>(module, 0xE8))) + k_one_0073d764;
        const long double amplitude = static_cast<float>(c * sin(t / shake_period_007a246c) * shake_degrees_007a2470 * k_deg_to_rad_007415f8);
        const I32 r1 = ((crt_rand_006fc447() + 1) % 2) - 1;                                // 006418CC..006418DE
        StoredFloat first;
        first.f = static_cast<float>(static_cast<long double>(r1) * amplitude);           // 006418DF..006418EB fild ; fmul ; fstp dword [esp+28h]
        first_bits = first.word();
        const I32 r2 = ((crt_rand_006fc447() + 1) % 2) - 1;                                // 006418EF..00641901
        second = static_cast<long double>(r2) * amplitude;                                 // 00641902..0064190A fild ; fmul [esp+10h]
    }
    if (b_stick) { at<float>(b_stick, 0x94) = static_cast<float>(second); at<U32>(b_stick, 0x90) = first_bits; }               // 0064190E..0064191C
    else if (b_combat) { at<float>(b_combat, 0x18) = static_cast<float>(second); at<U32>(b_combat, 0x14) = first_bits; }      // 00641924..00641933
    else if (b_106d) { at<float>(b_106d, 0x30) = static_cast<float>(second); at<U32>(b_106d, 0x2C) = first_bits; }            // 00641938..00641947
    else if (b_free) { at<float>(b_free, 0x74) = static_cast<float>(second); at<U32>(b_free, 0x70) = first_bits; }            // 0064194C..0064195B
    at<U32>(module, 0xDC) = at<U32>(module, 0xDC) - static_cast<U32>(ms);                  // 00641962
}

// 00644170 CSWCModule::UpdateCameraCollision(float delta, float zoom) (thiscall, RET 8; ST0).
// The arguments are read as their stack words (volatile: [esp+8] / [esp+0Ch] of the original), so no float travels
// through an x87 register here: the original only copies them (mov ; push) or returns zoom (fld [esp+0Ch]).
extern "C" double __fastcall kotor_fastcall_00644170(void* module, void*, float, float) {
    const volatile U32* const args = reinterpret_cast<const volatile U32*>(static_cast<const U8*>(_AddressOfReturnAddress()) + 4);
    if (lw(module, 0x8C) != 0) { const U32 zw = args[1]; float z; std::memcpy(&z, &zw, 4); return z; } // 00644173..00644182 (fld [esp+0Ch])
    typedef double(__fastcall* Collision)(void*, void*, U32, U32);
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 2)                     // 00644185..00644196
        return word_form<Collision>(&kotor_fastcall_006433b0)(module, nullptr, args[0], args[1]); // 00644198..006441A4 (mov ; push)
    return word_form<Collision>(&kotor_fastcall_006422e0)(module, nullptr, args[0], args[1]);     // 006441AD..006441B9
}

// 006433B0 CSWCModule::UpdatePushCameraCollision(float delta, float zoom) (camera mode 2 only; thiscall RET 8; ST0).
// a2: transcribed from the original instructions (notes/dis/006433B0.asm and its frame-offset view
// notes/dis/006433B0.frame.txt; F = esp after the four register pushes, delta = [F+138h], zoom = [F+13Ch]).
extern "C" {
void* __fastcall aurora_hit_info_construct_004578f0(void* self, void*, U32 a, U32 b, U32 c); // @...@20 RET0Ch
void __fastcall chit_info_destruct_00456b30(void* self, void*);                               // @...@8
void __fastcall kotor_fastcall_00456af0(void* self, void*, U32 a, U32 b, U32 c);             // RET0Ch
U32 __fastcall kotor_fastcall_0063f5e0(void* creature, void*);                                // HasGobModel
U32 __fastcall kotor_fastcall_00605370(void* area, void*, W4Vector from, W4Vector to, void* out); // ClearLOS RET1Ch
W4CamQuaternion* __cdecl quaternion_slerp_004a9e00(W4CamQuaternion* out, W4CamQuaternion from,
                                                   const W4CamQuaternion* to, float t);
W4CamQuaternion* __cdecl quaternion_normalize_004a9d60(W4CamQuaternion* out, W4CamQuaternion q);
}

namespace w4cam {
// One canonical P4 push-state block (original 00833A64/7C/80/84/88/8C; BSS zero).
extern U32 push_area_flag_00833a64;
extern float push_last_distance_00833a7c;
extern float push_cell_00833a80;
extern float push_cell_00833a84;
extern float push_cell_00833a88;
extern U32 push_state_00833a8c;
}

namespace {
struct P4HitInfo {                                                                        // [F+B4h] (0x64 bytes)
    alignas(4) U8 b[0x64];
    P4HitInfo() { aurora_hit_info_construct_004578f0(b, nullptr, 0, 0xBFFFFFFFu, 0); }       // 00643508..00643516
    ~P4HitInfo() { chit_info_destruct_00456b30(b, nullptr); }                              // 00643F06..00643F18
};
// The probe: [00833A64] != 0 -> CSWCArea::ClearLineOfSightOneWay 00605370 (area = module+48h; result 1 when it returns
// 0), else the scene's vtable +38h (from, to, &out, hit info; RET 20h). Vectors passed as their words.
__forceinline U32 p4_probe(void* module, void* scene, V3W from, V3W to, W4Vector* out, void* hit) {
    if (push_area_flag_00833a64 != 0)
        return word_form<U32(__fastcall*)(void*, void*, V3W, V3W, void*)>(&kotor_fastcall_00605370)(at<void*>(module, 0x48), nullptr, from, to, out) == 0 ? 1u : 0u;
    return reinterpret_cast<U32(__thiscall*)(void*, V3W, V3W, W4Vector*, void*)>(vfn(scene, 0x38))(scene, from, to, out, hit);
}
}  // namespace

extern "C" double __fastcall kotor_fastcall_006433b0(void* module, void*, float, float) {
    // delta = [F+138h], zoom = [F+13Ch]: read as their stack words at every use (word_ld); the result is written back
    // into the zoom slot and returned from it (00643EFF / 00643F1D), as the original does.
    volatile U32* const args = reinterpret_cast<volatile U32*>(static_cast<U8*>(_AddressOfReturnAddress()) + 4);
    void* const behavior_default = behavior(at<void*>(module, 0x40), -1);                  // 006433D0..006433D8
    if (behavior_default == nullptr) return 0.0;                                           // 006433E2 -> 006433F3 fld 0.0
    void* const following = vcall_ptr(behavior_default, 0x1C);                             // 006433E4..006433ED [F+5Ch]
    if (following == nullptr) return 0.0;                                                  // 006433F1 -> 006433F3
    void* const scene = at<void*>(module, 0x48) != nullptr ? at<void*>(at<void*>(module, 0x48), 0x184) : nullptr; // 006433FE..0064340F [F+48h]
    U8* const player = static_cast<U8*>(client_get_player_creature_005ed540(client_app(), nullptr)); // 00643413..00643421 edi
    if (collision_enabled_007a2468 == 0 || at<void*>(module, 0x40) == nullptr || scene == nullptr || player == nullptr ||
        kotor_fastcall_0063f5e0(player, nullptr) == 0)
        return word_f(args[1]);                                                                       // 00643423..00643453 -> 00643F1D
    void* const camera = at<void*>(module, 0x40);
    W4Vector camera_pos;                                                                   // [F+14h] (copied with mov)
    {
        W4Vector tmp;
        std::memcpy(&camera_pos, reinterpret_cast<W4Vector*(__thiscall*)(void*, W4Vector*)>(vfn(camera, 0x20))(camera, &tmp), 12); // 00643459..0064347D
    }
    W4Vector target;                                                                       // [F+50h]
    {
        W4Vector ppos, sum;
        std::memcpy(&ppos, player + 0x24, 12);                                             // 00643472..00643497 (mov)
        std::memcpy(&target, vector_sum_00423b90(&sum, &ppos, reinterpret_cast<const W4Vector*>(static_cast<U8*>(module) + 0x64)), 12); // 0064348B..006434BE
    }
    // 006434B6..00643502: d = target - camera (fst dword each) ; (dx*dx + dy*dy) + dz*dz ; fcomp 0 ; test 41h ; jne.
    StoredFloat dz, dy, dx;
    dz.f = static_cast<float>(ld(target.z) - camera_pos.z);                                // 006434B6..006434C5 fst [F+34h]
    dy.f = static_cast<float>(ld(target.y) - camera_pos.y);                                // 006434C9..006434D1 fst [F+30h]
    dx.f = static_cast<float>(ld(target.x) - camera_pos.x);                                // 006434D5..006434DD fst [F+38h]
    {
        const long double dx = ld(target.x) - camera_pos.x, dy = ld(target.y) - camera_pos.y, dz = ld(target.z) - camera_pos.z;
        if (!((dx * dx + dy * dy) + dz * dz > k_zero)) return word_f(args[1]);                         // 006434E1..00643502
    }
    {   // the hit info's lifetime: 00643508 (constructor) .. 00643F18 (destructor), before the return value is loaded
    P4HitInfo hit;                                                                         // 00643508..00643516
    U32 ret_word = args[1];                                                            // 0064351F / 00643536 [F+4Ch]
    W4Vector probe_out = {0.0f, 0.0f, 0.0f};                                               // 00643553..00643563 [F+3Ch]
    U32 camera_valid = 1;                                                                  // 0064356B [F+74h]
    StoredFloat selected;                                                                  // [F+10h]
    selected.set_word(0);                                                                  // 0064356F
    U32 hit_flag = 0;                                                                      // 00643577 [F+60h]
    W4Vector dir;                                                                          // [F+14h] (copied with mov)
    {
        W4Vector in, out;
        at<U32>(&in, 0) = dx.word(); at<U32>(&in, 4) = dy.word(); at<U32>(&in, 8) = dz.word(); // 0064351B..0064357B (the stored words)
        std::memcpy(&dir, vector_normalized_004ab180(&out, &in), 12);                      // 0064353E..0064359E
    }
    if ((push_state_00833a8c & 1u) == 0) {                                                 // 00643590..006435A2
        push_state_00833a8c |= 1u;                                                         // 006435A4
        at<U32>(&push_cell_00833a80, 0) = 0; at<U32>(&push_cell_00833a84, 0) = 0; at<U32>(&push_cell_00833a88, 0) = 0; // 006435AA..006435BE
    }
    if (ld(push_cell_00833a80) == k_zero && ld(push_cell_00833a84) == k_zero && ld(push_cell_00833a88) == k_zero) { // 006435C8..0064361F (test 44h ; jp)
        at<U32>(&push_cell_00833a80, 0) = word(player + 0x24);                             // 00643621..00643636 (mov)
        at<U32>(&push_cell_00833a84, 0) = word(player + 0x28);
        at<U32>(&push_cell_00833a88, 0) = word(player + 0x2C);
    }
    // 0064363C..0064366E: yaw = GetYaw (ST0) + delta * [module+98h] ; pitch = GetPitch + delta * [module+94h] (fstp dword).
    StoredFloat yaw, pitch;
    yaw.f = static_cast<float>(static_cast<long double>(vcall_st0(camera, 0x64)) + word_ld(args[0]) * at<float>(module, 0x98));
    pitch.f = static_cast<float>(static_cast<long double>(vcall_st0(camera, 0x68)) + word_ld(args[0]) * at<float>(module, 0x94));
    // 00643672..006436A1: factor = _CIpow(2.0, (delta * zoom) * -0.5) (fstp dword) ; dist = GetDist * factor (fstp dword).
    StoredFloat factor, dist;
    factor.f = static_cast<float>(pow(static_cast<long double>(k_two_00741df8), (word_ld(args[0]) * word_ld(args[1])) * k_minus_half_00743884));
    dist.f = static_cast<float>(static_cast<long double>(vcall_st0(camera, 0x6C)) * factor.value());
    // 006436A5..006436F5: pitch clamp: cand = (pitch < hi) ? pitch : hi ; lo > cand -> lo (word) ; else !(pitch < hi) -> hi (word).
    {
        const long double cand = (pitch.value() < follow_pitch_007a245c) ? pitch.value() : ld(follow_pitch_007a245c);
        if (ld(follow_7a2458) > cand) pitch.set_word(word(&follow_7a2458));
        else if (!(pitch.value() < follow_pitch_007a245c)) pitch.set_word(word(&follow_pitch_007a245c));
    }
    // 006436F9..00643749: the same clamp of dist to [007A2450, 007A2454].
    {
        const long double cand = (dist.value() < follow_7a2454) ? dist.value() : ld(follow_7a2454);
        if (ld(follow_7a2450) > cand) dist.set_word(word(&follow_7a2450));
        else if (!(dist.value() < follow_7a2454)) dist.set_word(word(&follow_7a2454));
    }
    // 0064374D..00643779: t = 1.0f - _CIpow(0.5, delta * 750) (fstp dword [F+34h]).
    StoredFloat t;
    t.f = static_cast<float>(ld(k_one_0073d764) - pow(static_cast<long double>(k_half_0073f3d0), word_ld(args[0]) * k_750_0075183c));
    typedef W4Vector*(__cdecl* ComponentMult)(W4Vector*, const W4Vector*, U32);
    W4Vector interp;                                                                       // [F+24h]
    {
        W4Vector up, head, head_scaled, state_scaled, sum;
        at<U32>(&up, 0) = 0; at<U32>(&up, 4) = 0; at<U32>(&up, 8) = 0x3F800000u;          // 0064377D..0064378D
        std::memcpy(&head, vector_sum_00423b90(&sum, reinterpret_cast<const W4Vector*>(player + 0x24), &up), 12); // 00643795..006437BC
        const W4Vector* hs = word_form<ComponentMult>(&vector_component_mult_00423bf0)(&head_scaled, &head, t.word()); // 006437C0 (t pushed as its word)
        const W4Vector* ss = vector_component_mult_00423bf0(&state_scaled, reinterpret_cast<const W4Vector*>(&push_cell_00833a80),
                                                            static_cast<float>(ld(k_one_0073d764) - t.value())); // 006437C5..006437E4 (1 - t fstp dword)
        std::memcpy(&interp, vector_sum_00423b90(&sum, ss, hs), 12);                       // 006437EC..00643814
    }
    // 0064380A..00643849: YawPitchRoll(q, yaw, pitch, 0) with the stored words ; q at [F+64h].
    W4CamQuaternion q;
    {
        W4CamQuaternion o;
        std::memcpy(&q, word_form<void*(__cdecl*)(void*, U32, U32, U32)>(&yaw_pitch_roll_004acac0)(&o, yaw.word(), pitch.word(), 0), 16);
    }
    // 00643855..006438B1: slerp(out, *GetOrientation (+24h) by value, &q, t) ; normalize(out, slerped by value).
    W4CamQuaternion normalized;
    {
        W4CamQuaternion prev_buf, slerped, nout;
        const W4CamQuaternion* prev = reinterpret_cast<W4CamQuaternion*(__thiscall*)(void*, W4CamQuaternion*)>(vfn(camera, 0x24))(camera, &prev_buf);
        const W4CamQuaternion* s = word_form<W4CamQuaternion*(__cdecl*)(W4CamQuaternion*, Q4W, const W4CamQuaternion*, U32)>(
            &quaternion_slerp_004a9e00)(&slerped, words4(prev), &q, t.word());
        std::memcpy(&normalized, word_form<W4CamQuaternion*(__cdecl*)(W4CamQuaternion*, Q4W)>(&quaternion_normalize_004a9d60)(&nout, words4(s)), 16);
    }
    // 006438DD..00643915: probe_to = interp + zdir(normalized) * dist (dist pushed as its word).
    W4Vector probe_to;
    {
        W4Vector zdir, offset, sum;
        const W4Vector* z = quaternion_zdir_0043e250(&normalized, nullptr, &zdir);
        const W4Vector* off = word_form<ComponentMult>(&vector_component_mult_00423bf0)(&offset, z, dist.word());
        std::memcpy(&probe_to, vector_sum_00423b90(&sum, &interp, off), 12);
    }
    // 0064391A..00643960: cells 80/84/88 = interp (mov) ; flag = !(zoom > 0) (test 41h ; je).
    at<U32>(&push_cell_00833a80, 0) = word(&interp.x);
    at<U32>(&push_cell_00833a84, 0) = word(&interp.y);
    at<U32>(&push_cell_00833a88, 0) = word(&interp.z);
    const U32 zoom_not_positive = (word_ld(args[1]) > k_zero) ? 0u : 1u;
    kotor_fastcall_00456af0(hit.b, nullptr, 0, 0xBFFFFFFFu, 0);                            // 00643966..00643976 CHitInfo::Reset
    at<U32>(hit.b, 0) = 1;                                                                 // 00643982
    if (p4_probe(module, scene, words3(&target), words3(&probe_to), &probe_out, hit.b) != 0) { // 0064397B..00643A0B
        // 00643A11..00643A6B: probe_out += dir * 0.001: x through the float product (fstp dword [F+14h]), y and z
        // with the unrounded product (fadd st(1) / st(2)).
        const long double kz = ld(dir.z) * k_milli_0073d708, ky = ld(dir.y) * k_milli_0073d708;
        const long double kx = static_cast<float>(ld(dir.x) * k_milli_0073d708);           // 00643A25..00643A2F
        const float pz = static_cast<float>(ld(probe_out.z) + kz);                         // 00643A33..00643A39
        const float py = static_cast<float>(ld(probe_out.y) + ky);                         // 00643A3D..00643A4F fst
        const float px = static_cast<float>(kx + probe_out.x);                             // 00643A57..00643A5F
        probe_out.x = px; probe_out.y = py; probe_out.z = pz;                              // 00643A47..00643A84 (mov copies)
        // 00643A73..00643AA5: |probe_out - target| (fstp dword x3 ; vector_magnitude) -> fstp dword selected.
        W4Vector diff;
        diff.z = static_cast<float>(ld(pz) - target.z);
        diff.y = static_cast<float>(ld(py) - target.y);
        diff.x = static_cast<float>(ld(px) - target.x);
        selected.f = static_cast<float>(vector_magnitude_004ab100(&diff));
        if (ld(push_last_distance_00833a7c) == k_zero)                                     // 00643AAC..00643ABD (test 44h ; jp)
            push_last_distance_00833a7c = static_cast<float>(vcall_st0(camera, 0x6C));     // 00643AC3..00643ACB
    } else if (ld(push_last_distance_00833a7c) > k_zero) {                                 // 00643AD6..00643AE7 (test 41h ; jne)
        W4Vector from2;                                                                    // [F+24h]
        {
            W4Vector scaled, diffv;
            const W4Vector* sc = word_form<ComponentMult>(&vector_component_mult_00423bf0)(&scaled, &dir, word(&push_last_distance_00833a7c)); // 00643AED..00643B01
            std::memcpy(&from2, kotor_cdecl_00423bc0(&diffv, &target, sc), 12);           // 00643B06..00643B3C
        }
        kotor_fastcall_00456af0(hit.b, nullptr, 0, 0xBFFFFFFFu, 0);                        // 00643B24..00643B40
        at<U32>(hit.b, 0) = 1;                                                             // 00643B48
        // 00643B55..00643B68: GetDist (ST0, unrounded) fcomp [00833A7C] ; test 5 ; jp.
        U32 r2;
        if (static_cast<long double>(vcall_st0(camera, 0x6C)) < push_last_distance_00833a7c)
            r2 = p4_probe(module, scene, words3(&probe_to), words3(&from2), &probe_out, hit.b); // 00643B6E..00643BEC
        else
            r2 = p4_probe(module, scene, words3(&from2), words3(&probe_to), &probe_out, hit.b); // 00643BEE..00643C65
        if (r2 != 0) {                                                                     // 00643C68..00643C6D
            // 00643C73..00643CCD: probe_out += dir * 0.001: x and y through the float products (fstp dword
            // [F+14h] / [F+20h]), z with the unrounded product.
            const long double kz = ld(dir.z) * k_milli_0073d708;
            const long double ky = static_cast<float>(ld(dir.y) * k_milli_0073d708);       // 00643C7D..00643C87
            const long double kx = static_cast<float>(ld(dir.x) * k_milli_0073d708);       // 00643C8B..00643C95
            const long double pz = kz + probe_out.z;                                       // 00643C99
            const long double py = ld(probe_out.y) + ky;                                   // 00643C9D..00643CA1
            const float px = static_cast<float>(kx + probe_out.x);                         // 00643CA5..00643CAD
            const float pyf = static_cast<float>(py);                                      // 00643CB9
            const float pzf = static_cast<float>(pz);                                      // 00643CC5
            probe_out.x = px; probe_out.y = pyf; probe_out.z = pzf;                        // 00643CB1..00643CCD (mov copies)
            // 00643CD1..00643CFE: candidate = GetDist + (last - GetDist) * delta * 3.5 (fstp dword each).
            StoredFloat adjustment;
            adjustment.f = static_cast<float>(((ld(push_last_distance_00833a7c) - vcall_st0(camera, 0x6C)) * word_ld(args[0])) * k_three_and_half_007518bc);
            selected.f = static_cast<float>(static_cast<long double>(vcall_st0(camera, 0x6C)) + adjustment.value());
            // 00643D02..00643D47: s = (dx*dx + dy*dy) + dz*dz of probe_out - target ; candidate^2 < s (or NaN) keeps
            // the candidate, else selected = fsqrt(s) (fstp dword).
            const long double ex = ld(px) - target.x, ey = ld(pyf) - target.y, ez = ld(pzf) - target.z;
            const long double s = (ex * ex + ey * ey) + ez * ez;
            const long double c2 = selected.value() * selected.value();
            if (c2 >= s) selected.f = static_cast<float>(sqrt(s));                          // 00643D30..00643D43 (test 1 ; jne)
        } else {
            // 00643D49..00643DAA: selected = GetDist + (last - GetDist) * delta * 3.5 ; hit_flag = 1 ;
            // selected >= last (test 1 ; jne) -> selected = last (word), last = 0.
            StoredFloat adjustment;
            adjustment.f = static_cast<float>(((ld(push_last_distance_00833a7c) - vcall_st0(camera, 0x6C)) * word_ld(args[0])) * k_three_and_half_007518bc);
            const long double sel = static_cast<long double>(vcall_st0(camera, 0x6C)) + adjustment.value();
            hit_flag = 1;                                                                  // 00643D79
            selected.f = static_cast<float>(sel);                                          // 00643D81
            if (selected.value() >= push_last_distance_00833a7c) {                         // 00643D85..00643D94
                selected.set_word(word(&push_last_distance_00833a7c));                     // 00643D96..00643D9C
                at<U32>(&push_last_distance_00833a7c, 0) = 0;                              // 00643DA0
            }
        }
    }
    if (zoom_not_positive == 0) at<U32>(&push_last_distance_00833a7c, 0) = 0;              // 00643DAE..00643DB6
    // 00643DC0..00643E37: (selected == 0 and GetDist < lo) or (selected > 0 and lo - selected >= eps) ->
    // camera_valid = 0, last = max(last, lo) (word), ret = 0.
    bool drop = false;
    if (selected.value() == k_zero) drop = static_cast<long double>(vcall_st0(camera, 0x6C)) < follow_7a2450; // 00643DC0..00643DE4
    if (!drop && selected.value() > k_zero && (ld(follow_7a2450) - selected.value()) >= k_eps_00741870) drop = true; // 00643DE6..00643E0C (test 1 ; jne)
    if (drop) {
        camera_valid = 0;                                                                  // 00643E14
        if (ld(push_last_distance_00833a7c) < follow_7a2450)                               // 00643E0E..00643E21 (test 5 ; jp)
            at<U32>(&push_last_distance_00833a7c, 0) = word(&follow_7a2450);               // 00643E23..00643E28
        ret_word = 0;                                                                      // 00643E2D
    }
    if (kotor_cdecl_0060b2a0(nullptr) != 0) {                                              // 00643E3B..00643E47
        at<U32>(following, 0x64) = camera_valid != 0 ? 0x3F800000u : 0u;                    // 00643E4D..00643E62
        if (selected.value() > k_zero) {                                                   // 00643E69..00643E78 (test 41h ; jne)
            at<U32>(following, 0x30) = selected.word();                                    // 00643E7A..00643E82
            if (hit_flag == 0) ret_word = 0;                                               // 00643E85..00643E8D
            at<U32>(module, 0x80) = 1;                                                     // 00643E95
        }
        float ret_f; std::memcpy(&ret_f, &ret_word, 4);
        // 00643E9F..00643ECB: GetDist <= lo (test 41h ; jp) and ret > 0 -> ret = 0.
        {
            const long double cd = vcall_st0(camera, 0x6C);
            if ((cd < follow_7a2450 || cd == follow_7a2450) && ld(ret_f) > k_zero) ret_word = 0;
        }
        std::memcpy(&ret_f, &ret_word, 4);
        // 00643ECD..00643EF3: GetDist >= hi (test 1 ; jne) and ret < 0 -> ret = 0.
        if (static_cast<long double>(vcall_st0(camera, 0x6C)) >= follow_7a2454 && ld(ret_f) < k_zero) ret_word = 0;
        args[1] = ret_word;                                                  // 00643EFB..00643EFF ([F+13Ch] = ret)
    }
    }   // 00643F06..00643F18 ~CHitInfo
    return word_f(args[1]);                                                                           // 00643F1D fld dword [F+13Ch]
}

// 00640700 / 00640A80 / 00640B90 UpdateCameraModeChaseCam / BG / EQ (options camera modes 0 / 1 / 2).
extern "C" void __fastcall kotor_fastcall_00640700(void* module, void*) {
    void* stick = nullptr;
    {
        void* const b = behavior(lp(module, 0x40), -1);                                  // 00640708..00640723
        if (b != nullptr) stick = vcall<void*>(b, 0x1C);                                 // 0064071C..00640727
    }
    void* const scene = lw(module, 0x48) != 0 ? lp(lp(module, 0x48), 0x184) : nullptr;   // 0064072D..0064073A
    void* const player = client_get_player_creature_005ed540(client_app(), nullptr);     // 0064073C..0064074D
    if (scene == nullptr || player == nullptr) return;                                   // 00640751..00640759
    if (lw(module, 0xD8) != 0) {                                                         // 0064075F..00640765
        void* const db = behavior(lp(module, 0x40), -1);                                 // 0064076B..00640772
        void* const ds = db != nullptr ? vcall<void*>(db, 0x1C) : nullptr;               // 00640780..00640787
        if (ds != nullptr) {                                                             // 00640789..0064078B
            at<U32>(ds, 0x68) = 0x40A00000u; at<U32>(ds, 0x6C) = 0x41A00000u; at<U32>(ds, 0x5C) = 0x3F800000u; at<U32>(ds, 0x60) = 0x42480000u; // 00640791..006407B0
            at<U32>(&follow_7a2450, 0) = 0x40A00000u; at<U32>(&follow_7a2454, 0) = 0x41A00000u;
            at<U32>(&follow_7a2458, 0) = 0x3F800000u; at<U32>(&follow_pitch_007a245c, 0) = 0x42480000u; // 006407B3..006407D1
            at<U32>(module, 0x64) = 0; at<U32>(module, 0x68) = 0; at<U32>(module, 0x6C) = 0x3F800000u; // 006407D7..00640801
            at<U32>(ds, 0x24) = 0; at<U32>(ds, 0x28) = 0; at<U32>(ds, 0x2C) = 0x3F800000u; // 00640804..0064080D
            U32 near_far[2];
            vcall<void, U32*, U32*>(lp(module, 0x40), 0x3C, &near_far[0], &near_far[1]);  // 00640814..0064081F
            camera_set_near_45(lp(module, 0x40), near_far[0]);                            // 00640822..00640831
            at<U32>(ds, 0x44) = 0x42480000u; at<U32>(ds, 0x30) = 0x41A00000u;              // 00640838..0064083D
            sw(module, 0xD8, 0);                                                         // 00640844
        }
    }
    bool special = false;
    if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2712) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2713) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2714) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x275E) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x275F) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2764) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2765) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2766) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2767) special = true; // 0064084A..006408E4
    if (special) {
        if (lw(module, 0xFC) != 0) { sw(module, 0x88, 0); sw(module, 0xFC, 0); }         // 006408E6..006408F9
    } else {
        sw(module, 0x4C, lw(player, 0x24)); sw(module, 0x50, lw(player, 0x28)); sw(module, 0x54, lw(player, 0x2C)); // 006408C3..006408DE
        sw(module, 0xFC, 1);                                                             // 006408DE
    }
    const U32 dirty80 = lw(module, 0x80);                                                // 006408FF
    if (dirty80 != 0) { sw(module, 0x88, 1); sw(module, 0x84, 0); }                      // 006408FF..0064090F
    if (lw(module, 0x88) != 0 || dirty80 != 0) return;                                   // 00640915..00640923
    // 00640929..00640949: camera yaw = Yaw(GetOrientation (+24h)) -> fstp dword ; angle = Yaw(player+30h) - it.
    StoredFloat camera_yaw;
    {
        W4CamQuaternion q;
        camera_yaw.f = static_cast<float>(quaternion_yaw_004a9f40(
            reinterpret_cast<W4CamQuaternion*(__thiscall*)(void*, W4CamQuaternion*)>(vfn(lp(module, 0x40), 0x24))(lp(module, 0x40), &q)));
    }
    long double angle = static_cast<long double>(kotor_m15a_yaw_004aa0f0(reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x30)))
                        - camera_yaw.value();                                            // 00640940..00640949
    while (angle < k_minus_180_0074f4f0) angle = angle + k_360_00741868;                 // 00640950..00640971 (test 5 ; jnp loop)
    while (angle > k_180_0073e408) angle = angle - k_360_00741868;                       // 00640973..00640991 (test 41h ; je loop)
    if (lw(player, 0x138) == 0) {                                                        // 00640993..00640999
        if (angle > k_90_0073e40c && lb(module, 0x90) == 2) angle = k_minus_90_0074fc44; // 0064099B..006409B9
        else if (angle < k_minus_90_0074fc44 && lb(module, 0x90) == 1) angle = k_90_0073e40c; // 006409BB..006409D3
    }
    if (!(fabs(angle) > k_one_double_0073ede8)) { sw(module, 0x84, 0); return; }         // 006409D9..006409E8 / 00640A60
    // 006409EA..00640A09: proceed when |angle| > [007A2460] or +84h != 0.
    if (!(fabs(angle) > chase_turn_limit_007a2460) && lw(module, 0x84) == 0) return;
    long double clamped = angle;
    if (clamped > k_90_0073e40c) clamped = k_90_0073e40c;                                // 00640A0B..00640A20
    else if (clamped < k_minus_90_0074fc44) clamped = k_minus_90_0074fc44;               // 00640A22..00640A35
    if (stick != nullptr) at<float>(stick, 0x40) = static_cast<float>(ld(chase_turn_scale_007a2464) * clamped); // 00640A37..00640A47
    sw(module, 0x80, 1); sw(module, 0x84, 1);                                             // 00640A4A..00640A52
}
extern "C" void __fastcall kotor_fastcall_00640a80(void* module, void*) {
    if (lw(module, 0xD8) == 0) return;                                                    // 00640A86..00640A8E
    void* const b = behavior(lp(module, 0x40), -1);                                        // 00640A94..00640A9B
    if (b == nullptr) return;                                                              // 00640AA1..00640AA3
    void* const s = vcall<void*>(b, 0x1C);                                                // 00640AA9..00640AB5
    if (s == nullptr) return;                                                              // 00640AB3..00640AB5
    at<U32>(s, 0x68) = 0x40A00000u; at<U32>(s, 0x6C) = 0x41A00000u; at<U32>(s, 0x5C) = 0x3F800000u; at<U32>(s, 0x60) = 0x42480000u; // 00640ABB..00640AD1
    at<U32>(&follow_7a2450, 0) = 0x40A00000u; at<U32>(&follow_7a2454, 0) = 0x41A00000u;
    at<U32>(&follow_7a2458, 0) = 0x3F800000u; at<U32>(&follow_pitch_007a245c, 0) = 0x42480000u; // 00640AD8..00640AF6
    at<U32>(module, 0x64) = 0; at<U32>(module, 0x68) = 0; at<U32>(module, 0x6C) = 0x3F800000u; // 00640B00..00640B2F
    at<U32>(s, 0x24) = 0; at<U32>(s, 0x28) = 0; at<U32>(s, 0x2C) = 0x3F800000u;           // 00640B32..00640B3B
    U32 near_far[2];
    vcall<void, U32*, U32*>(lp(module, 0x40), 0x3C, &near_far[0], &near_far[1]);          // 00640B42..00640B4D
    camera_set_near_45(lp(module, 0x40), near_far[0]);                                    // 00640B50..00640B5F
    at<U32>(s, 0x30) = 0x41A00000u; at<U32>(s, 0x44) = 0x42480000u;                        // 00640B62..00640B69
    sw(module, 0xD8, 0);                                                                   // 00640B70
}
extern "C" void __fastcall kotor_fastcall_00640b90(void* module, void*, float delta) {
    void* const scene = lw(module, 0x48) != 0 ? lp(lp(module, 0x48), 0x184) : nullptr;   // 00640B98..00640BA4
    void* const player = client_get_player_creature_005ed540(client_app(), nullptr);     // 00640BAA..00640BBB
    if (scene == nullptr || player == nullptr) return;                                   // 00640BBF..00640BC7
    void* stick = nullptr;
    {
        void* const b = behavior(lp(module, 0x40), -1);                                  // 00640BCD..00640BD4
        if (b != nullptr) stick = vcall<void*>(b, 0x1C);                                 // 00640BDE..00640BE5
    }
    if (lw(module, 0xD8) != 0 && stick != nullptr) {                                     // 00640BEB..00640BF9
        at<U32>(stick, 0x5C) = 0x3F800000u; at<U32>(stick, 0x60) = 0x42780000u; at<U32>(stick, 0x68) = 0x40C00000u; at<U32>(stick, 0x6C) = 0x40C00000u; // 00640BFF..00640C15
        at<U32>(&follow_7a2450, 0) = 0x40C00000u; at<U32>(&follow_7a2454, 0) = 0x40C00000u;
        at<U32>(&follow_7a2458, 0) = 0x3F800000u; at<U32>(&follow_pitch_007a245c, 0) = 0x42780000u; // 00640C18..00640C2E (62.0: the checkpoint had 60.0)
        at<U32>(module, 0x64) = 0; at<U32>(module, 0x68) = 0; at<U32>(module, 0x6C) = 0x40000000u; // 00640C38..00640C67
        at<U32>(stick, 0x24) = 0; at<U32>(stick, 0x28) = 0; at<U32>(stick, 0x2C) = 0x40000000u; // 00640C6A..00640C73
        U32 near_far[2];
        vcall<void, U32*, U32*>(lp(module, 0x40), 0x3C, &near_far[0], &near_far[1]);      // 00640C7A..00640C85
        camera_set_near_45(lp(module, 0x40), near_far[0]);                                // 00640C88..00640C97
        at<U32>(stick, 0x30) = 0x40C00000u; at<U32>(stick, 0x44) = 0x42780000u;            // 00640C9E..00640CA3
        sw(module, 0xD8, 0);                                                              // 00640CAA
    }
    bool special = false;
    if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2712) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2713) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2714) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x275E) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x275F) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2764) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2765) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2766) special = true;
    else if (kotor_m15a_object_get_current_animation_0063c060(player, nullptr) == 0x2767) special = true; // 00640CB0..00640D49
    if (special) {
        if (lw(module, 0xFC) != 0) { sw(module, 0x88, 0); sw(module, 0xFC, 0); }         // 00640D4B..00640D59
    } else {
        sw(module, 0x4C, lw(player, 0x24)); sw(module, 0x50, lw(player, 0x28)); sw(module, 0x54, lw(player, 0x2C)); // 00640D29..00640D3F
        sw(module, 0xFC, 1);                                                             // 00640D3F
    }
    const U32 dirty80 = lw(module, 0x80);                                                // 00640D5F
    if (dirty80 != 0) { sw(module, 0x88, 1); sw(module, 0x84, 0); }                      // 00640D5F..00640D73
    if (lw(module, 0x88) != 0 || dirty80 != 0) return;                                   // 00640D79..00640D87
    // 00640D8D..00640DB3: camera yaw (GetYaw ST0) -> fstp dword ; player yaw: fst dword (kept) ; angle = it - camera
    // yaw ; fcom -180 ; fst dword (the raw angle, kept).
    StoredFloat camera_yaw, player_yaw, angle_raw;
    camera_yaw.f = static_cast<float>(vcall_st0(lp(module, 0x40), 0x64));
    const long double pyaw = kotor_m15a_yaw_004aa0f0(reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x30));
    player_yaw.f = static_cast<float>(pyaw);                                             // 00640DA2 fst
    long double angle = pyaw - camera_yaw.value();                                       // 00640DA6 fsub
    angle_raw.f = static_cast<float>(angle);                                             // 00640DB3 fst
    while (angle < k_minus_180_0074f4f0) angle = angle + k_360_00741868;                 // 00640DAD..00640DD1
    while (angle > k_180_0073e408) angle = angle - k_360_00741868;                       // 00640DD3..00640DF1
    if (lw(player, 0x138) == 0) {                                                        // 00640DF3..00640DF9
        if (angle > k_90_0073e40c && lb(module, 0x90) == 2) angle = k_minus_90_0074fc44; // 00640DFB..00640E19
        else if (angle < k_minus_90_0074fc44 && lb(module, 0x90) == 1) angle = k_90_0073e40c; // 00640E1B..00640E33
    }
    if (!(fabs(angle) > k_one_double_0073ede8)) { sw(module, 0x84, 0); return; }         // 00640E39..00640E48 / 00640F53
    long double b_angle = angle * k_five_00743264;                                       // 00640E4E
    if (b_angle > k_180_0073e408) b_angle = k_180_0073e408;                              // 00640E54..00640E63
    else if (b_angle < k_minus_180_0074f4f0) b_angle = k_minus_180_0074f4f0;             // 00640E6B..00640E7A
    // 00640E80..00640E8A: c = player_yaw - (camera_yaw + delta * b) (fsubr).
    long double c_angle = player_yaw.value() - (camera_yaw.value() + ld(delta) * b_angle);
    while (c_angle < k_minus_180_0074f4f0) c_angle = c_angle + k_360_00741868;           // 00640E8E..00640EB1
    while (c_angle > k_180_0073e408) c_angle = c_angle - k_360_00741868;                 // 00640EB3..00640ED1
    long double selected;
    if (b_angle > k_eps_00741870 && c_angle < k_eps_00741870) {                          // 00640ED3..00640EED
        selected = angle_raw.value() / delta;                                            // 00640F17..00640F1F
    } else if (!(b_angle < k_eps_00741870)) {                                            // 00640EEF..00640EFC
        selected = b_angle;
    } else if (!(c_angle > k_eps_00741870)) {                                            // 00640EFE..00640F09
        selected = b_angle;
    } else {
        selected = angle_raw.value() / delta;                                            // 00640F0B..00640F11
    }
    if (stick != nullptr) at<float>(stick, 0x40) = static_cast<float>(ld(chase_turn_scale_007a2464) * selected); // 00640F27..00640F33
    sw(module, 0x80, 1); sw(module, 0x84, 1);                                             // 00640F36..00640F4A
}
// 00638670 CSWCameraNavigate::CSWCameraNavigate / 0063A5D0 CSWCameraFreeLook::CSWCameraFreeLook (modes 7 / 5).
extern "C" void* __fastcall kotor_fastcall_00638670(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_0063a5d0(void*, void*, void*, void*, U32);

namespace {
// 00641B4C..00641B9B / 0063FA40..0063FAE3 common: new CSWCameraOnAStick(camera->AsAurObject(), creature->GetCameraFollowPart()).
void* new_stick(void* module, void* creature) {
    void* memory = live_operator_new(0x138);                                               // 00641B2D..00641B34 / 0063FA40..0063FA45
    if (memory == nullptr) return nullptr;                                                 // 00641B44..00641B4A
    void* stick = nullptr;
    try {                                                                                  // original state 0
        void* const camera = lp(module, 0x40);                                             // 00641B4E (after allocation, before part)
        void* const part = vcall<void*>(creature, 0xD0);                                   // 00641B4C..00641B59
        void* const owner = vcall<void*>(camera, 0x04);                                    // 00641B5A..00641B62
        stick = kotor_fastcall_00639fc0(memory, nullptr, owner, part);                     // 00641B63..00641B6A
    } catch (...) {
        live_free(memory);                                                                 // state-0 callback 00729CE6
        throw;
    }
    return stick;                                                                          // 00641B6A / 00641B6C reset
}
// stick+68h = creature->GetFollowingBody(); stick+6Ch = (float)(head height offset) + GetCameraOffset().
__forceinline void set_stick_heights(void* stick, void* creature) {
    sw(stick, 0x68, ad(vcall<void*>(creature, 0xD4)));                                     // 00641B6C..00641B7E
    StoredFloat head;                                                                      // [esp+14h] / [esp+10h]
    head.f = static_cast<float>(vcall_st0(creature, 0xD8));                                // 00641B81..00641B8B fstp dword
    at<float>(stick, 0x6C) = static_cast<float>(static_cast<long double>(kotor_fastcall_0060f010(creature, nullptr)) + head.value()); // 00641B8F..00641B9B
}
}  // namespace

// 006419A0 CSWCModule::UpdateCameraFollow(float delta) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006419a0(void* module, void*, float) {
    const volatile U32* const args = reinterpret_cast<const volatile U32*>(static_cast<const U8*>(_AddressOfReturnAddress()) + 4);
    const U32 delta_word = args[0];                                                        // [esp+..] delta, only copied (push)
    const U32 mode = lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D);             // 006419B8..006419C9 ebp
    if (mode == 4) return;                                                                 // 006419CD..006419D4
    if (lw(module, 0x8C) != 0) return;                                                     // 006419DA..006419E2
    if (lw(module, 0x40) == 0) return;                                                     // 006419E8..006419ED
    if (mode != lw(module, 0x0C)) {                                                        // 006419F3..006419F8
        void* object = kotor_fastcall_005ed580(client_app(), nullptr, lw(module, 0x70));  // 006419FE..00641A0B
        if (object == nullptr) return;                                                     // 00641A10..00641A14
        void* client_object = vcall<void*>(object, 0x0C);                                  // 00641A1A..00641A1E
        if (client_object == nullptr) return;                                              // 00641A21..00641A23
        void* model = vcall<void*, U32, U32>(client_object, 0x98, 0xFF, 1);                // 00641A29..00641A34
        if (model == nullptr) return;                                                      // 00641A3A..00641A3E
        if (lw(module, 0x0C) == 3) set_behavior(lp(module, 0x40), nullptr);                // 00641A44..00641A51
        if (mode == 3) {                                                                   // 00641A54..00641A59
            void* creature = vcall<void*>(object, 0x2C);                                   // 00641B26..00641B2A
            void* stick = new_stick(module, creature);                           // 00641B2D..00641B6A
            set_stick_heights(stick, creature);                                            // 00641B6C..00641B9B
            set_behavior(lp(module, 0x40), stick);                                         // 00641B9E..00641BA3
        } else if (mode == 5) {                                                            // 00641A5F..00641A62
            void* const creature = vcall<void*>(object, 0x2C);                         // 00641ABD..00641AC1
            void* const memory = live_operator_new(0xC4);                             // 00641AC4..00641AD0
            void* made = nullptr;
            if (memory != nullptr) {                                                  // 00641AD9..00641AE3
                try {                                                                 // original state1; saved allocation only
                    void* const camera = lp(module, 0x40);                            // cached before all three argument calls
                    const U32 effect = kotor_fastcall_00610490(creature, nullptr);     // 00641AE8..00641AEA
                    void* const part = vcall<void*>(creature, 0xD0);                   // 00641AEF..00641AF4
                    void* const owner = vcall<void*>(camera, 0x04);                    // 00641AFB..00641B00
                    made = kotor_fastcall_0063a5d0(memory, nullptr, owner, part, effect); // 00641B03..00641B06
                } catch (...) {
                    live_free(memory);                                                // state1 callback00729CDB
                    throw;
                }
            }
            set_behavior(lp(module, 0x40), made);                                      // state-1; reload camera00641B9E..00641BA3
        } else if (mode == 7) {                                                            // 00641A64..00641A67
            void* const memory = live_operator_new(0x30);                                  // 00641A77..00641A80
            void* made = nullptr;
            if (memory != nullptr) {                                                       // 00641A87..00641A91
                try {                                                                      // original state2; saved allocation only
                    void* const owner = vcall<void*>(lp(module, 0x40), 0x04);              // 00641A93..00641A9B AsCAurObject
                    made = kotor_fastcall_00638670(memory, nullptr, owner);                // 00641A9C..00641AA3 real ctor, RET4
                } catch (...) {
                    live_free(memory);                                                     // state2 callback00729CD0
                    throw;
                }
            }
            set_behavior(lp(module, 0x40), made);                                          // state reset before call; 00641AA7..00641AB8
        } else {
            vcall<void, void*>(lp(module, 0x40), 0x5C, model);                             // 00641A69..00641A6F Camera::SetFollow
        }
        sw(module, 0x0C, mode);                                                            // 00641BA6..00641BAA
    }
    if (lb(module, 0xD5) != lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D))      // 00641BAD..00641BC3
        sw(module, 0xD8, 1);                                                               // 00641BC5
    const U32 current = lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D);          // 00641BCF..00641BDD
    switch (current) {                                                                     // 00641BE1..00641BE6 table 00641C9C
    case 0: kotor_fastcall_00640700(module, nullptr); break;                               // 00641BED
    case 1: kotor_fastcall_00640a80(module, nullptr); break;                               // 00641BF6
    case 2: word_form<void(__fastcall*)(void*, void*, U32)>(&kotor_fastcall_00640b90)(module, nullptr, delta_word); break; // 00641BFF (push the argument word)
    case 3: case 4: return;                                                                // table -> 00641C84 epilogue
    default: break;                                                                        // ja 00641C0B
    }
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 0 ||
        lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 1) {                   // 00641C0B..00641C32
        void* b = behavior(lp(module, 0x40), -1);                                          // 00641C34..00641C3B
        if (b != nullptr) (void)vcall<void*>(b, 0x1C);                                     // 00641C41..00641C49
    }
    at<float>(module, 0x74) = static_cast<float>(vcall_st0(lp(module, 0x40), 0x6C));        // 00641C4C..00641C54 GetDist
    at<float>(module, 0x78) = static_cast<float>(vcall_st0(lp(module, 0x40), 0x64));        // 00641C57..00641C5F GetYaw
    at<float>(module, 0x7C) = static_cast<float>(vcall_st0(lp(module, 0x40), 0x68));        // 00641C62..00641C6A GetPitch
    sb(module, 0xD5, lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D));            // 00641C6D..00641C7E
}

// 0063F9D0 CSWCModule::SetCameraFollow(id, flag) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0063f9d0(void* module, void*, U32 id, U32 flag) {
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0) return;                       // 0063F9DE..0063F9FD
    if (lw(module, 0x0C) == 3) {                                                           // 0063FA03..0063FA10
        void* creature = pt(frame_R4_005ed5b0(client_app(), nullptr, id));                 // 0063FA16..0063FA1B
        if (creature == nullptr) return;                                                   // 0063FA20..0063FA24
        void* stick = behavior(lp(module, 0x40), 0x106A);                                  // 0063FA2A..0063FA34
        if (stick == nullptr) {                                                            // 0063FA3C..0063FA3E
            stick = new_stick(module, creature);                                 // 0063FA40..0063FA75
            set_behavior(lp(module, 0x40), stick);                                         // 0063FA7A..0063FA8A
        }
        W4Vector position;                                                                 // [esp+14h]
        if (void* owner = lp(stick, 0x1C)) vcall<void, W4Vector*>(owner, 0x64, &position); // 0063FA8D..0063FA9B
        else { sw(&position, 0, 0); sw(&position, 4, 0); sw(&position, 8, 0); }             // 0063FAA0..0063FAB0
        set_stick_heights(stick, creature);                                                // 0063FAB8..0063FAE3
        kotor_fastcall_0063a380(stick, nullptr, vcall<void*>(creature, 0xD0), flag);       // 0063FAE6..0063FAF3 ChangeTarget
        sw(module, 0x70, id);                                                              // 0063FAFA..0063FAFE
        if (flag != 0) return;                                                             // 0063FB01
        word_form<void(__fastcall*)(void*, void*, V3W)>(&kotor_fastcall_0063f630)(stick, nullptr, words3(&position)); // 0063FB07..0063FB22 SetPosition (words)
        std::memcpy(static_cast<U8*>(stick) + 0x50, &position, 12);                        // 0063FB27..0063FB3B
        return;
    }
    void* object = kotor_fastcall_005ed580(client_app(), nullptr, id);                     // 0063FB43..0063FB48
    if (object == nullptr) return;                                                         // 0063FB4D..0063FB4F
    void* client_object = vcall<void*>(object, 0x0C);                                      // 0063FB55..0063FB59
    if (client_object == nullptr) return;                                                  // 0063FB5C..0063FB60
    void* model = vcall<void*, U32, U32>(client_object, 0x98, 0xFF, 1);                    // 0063FB66..0063FB72
    if (model == nullptr) return;                                                          // 0063FB78..0063FB7C
    const U32 previous = lw(module, 0x70);                                                 // 0063FB82..0063FB89
    void* camera = lp(module, 0x40);                                                       // 0063FB8D
    sw(module, 0x70, id);                                                                  // 0063FB92
    sw(module, 0x8C, flag);                                                                // 0063FB95
    if (camera == nullptr) return;                                                         // 0063FB90..0063FB9B
    vcall<void, void*>(camera, 0x5C, model);                                               // 0063FBA1..0063FBA4 SetFollow
    {
        W4Vector tmp;
        const W4Vector* p = vcall<W4Vector*, W4Vector*>(model, 0x64, &tmp);                // 0063FBA7..0063FBB0
        std::memcpy(static_cast<U8*>(module) + 0x58, p, 12);                               // 0063FBB3..0063FBC3
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 0063FBC6..0063FBE6
        at<U32>(stick, 0x44) = word(&follow_pitch_007a245c);                               // 0063FBEC..0063FBF3 (mov)
        sw(stick, 0x38, 0x443B8000u);                                                      // 0063FBF6 750.0f
        if (kotor_cdecl_0060b2a0(nullptr) != 0) {                                          // 0063FBF1..0063FC07
            sw(stick, 0x58, 0x3F800000u); sw(stick, 0x64, 0x3F800000u);                    // 0063FC09..0063FC11
        }
        at<U32>(stick, 0x5C) = word(&follow_7a2458);                                       // 0063FC14..0063FC1A (mov)
        at<U32>(stick, 0x60) = word(&follow_pitch_007a245c);                               // 0063FC1D..0063FC23
        at<U32>(stick, 0x68) = word(&follow_7a2450);                                       // 0063FC26..0063FC2B
        at<U32>(stick, 0x6C) = word(&follow_7a2454);                                       // 0063FC2E..0063FC34
        if (!(ld(at<float>(module, 0x74)) == k_zero)) {                                    // 0063FC37..0063FC45 (test 44h ; jnp: +74h != 0)
            sw(stick, 0x30, lw(module, 0x74));                                             // 0063FC47..0063FC4A
            sw(stick, 0x3C, lw(module, 0x78));                                             // 0063FC4D..0063FC50
            sw(stick, 0x44, lw(module, 0x7C));                                             // 0063FC53..0063FC56
        }
        std::memcpy(static_cast<U8*>(stick) + 0x24, static_cast<U8*>(module) + 0x64, 12);  // 0063FC59..0063FC6C
    }
    if (void* old = pt(frame_R4_005ed5b0(client_app(), nullptr, previous)))                // 0063FC6F..0063FC84
        kotor_m15a_creature_update_shadow_blob_0060e5b0(old, nullptr);                     // 0063FC86..0063FC88
    if (vcall<void*>(client_object, 0x2C) != nullptr)                                      // 0063FC8D..0063FC97
        kotor_m15a_creature_update_shadow_blob_0060e5b0(vcall<void*>(client_object, 0x2C), nullptr); // 0063FC99..0063FCA3
}
