// Lane W4 G14: CSWCModule camera controls (Tilt/Turn/Zoom/Scroll/BumpAndShake/UpdateCameraFollow/
// UpdateCameraCollision dispatch/SetCameraFollow). Transcribed from the original disassembly (dis/<ADDR>.asm).
// Module fields: +0Ch camera mode, +40h Aurora Camera*, +48h area, +70h follow id, +74h..+7Ch saved dist/yaw/pitch,
// +80h camera-dirty flags, +8Ch clear-view request, +94h/+98h/+9Ch tilt/turn/zoom mirrors, +D5h last options mode.
#include "cf86fc3cd2-camera.hpp"
#include "b5015298e2-camera_stick.hpp"
#include <cmath>

using namespace w4;
using namespace w4cam;
namespace w4cam {
w4::U32 push_area_flag_00833a64;
float push_last_distance_00833a7c;
float push_cell_00833a80;
float push_cell_00833a84;
float push_cell_00833a88;
w4::U32 push_state_00833a8c;
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
// Original mode5 Roll/Pitch/Yaw sequence: retain provider ST0 until each original FSTP DWORD.
W4CamQuaternion* camera_mode5_orientation(W4CamQuaternion* out,const W4Vector* direction,U32 angle_bits) {
    W4CamQuaternion q;
    pointto_004acca0(&q,direction);
    float roll_value, pitch_value, yaw_value;
    W4CamQuaternion* returned;
    __asm {
        lea eax, q
        push dword ptr [eax+12]
        push dword ptr [eax+8]
        push dword ptr [eax+4]
        push dword ptr [eax]
        call kotor_cdecl_004acb90
        add esp, 16
        fstp dword ptr roll_value
        lea eax, q
        push eax
        call quaternion_pitch_004aa020
        add esp, 4
        fstp dword ptr pitch_value
        lea eax, q
        push eax
        call quaternion_yaw_004a9f40
        add esp, 4
        fadd dword ptr angle_bits
        fstp dword ptr yaw_value
        push dword ptr roll_value
        push dword ptr pitch_value
        push dword ptr yaw_value
        push out
        call yaw_pitch_roll_004acac0
        add esp, 16
        mov returned, eax
    }
    return returned;
}

// Original0063FEB9..0063FF1D: retain unrounded half-input on ST0 through both clamp tests.
U32 camera_mode5_turn_angle(void* free_look,U32 amount_bits,U32 scale_bits) {
    const float half_input=0.5f, positive_limit=30.0f, negative_limit=-30.0f;
    U32 result;
    __asm {
        fld dword ptr amount_bits
        fmul dword ptr half_input
        fld dword ptr positive_limit
        fcomp st(1)
        fnstsw ax
        test ah, 5
        jnp clamp_upper
        fld st(0)
        fld dword ptr negative_limit
        fcomp st(1)
        fnstsw ax
        fstp st(0)
        test ah, 41h
        jnz clamp_upper
        fstp st(0)
        fld dword ptr negative_limit
        jmp apply_turn
    clamp_upper:
        fld dword ptr positive_limit
        fcomp st(1)
        fnstsw ax
        test ah, 5
        jp apply_turn
        fstp st(0)
        fld dword ptr positive_limit
    apply_turn:
        mov ecx, free_look
        fld dword ptr [ecx+28h]
        fmul dword ptr scale_bits
        fmul st, st(1)
        fchs
        fstp dword ptr result
        fstp st(0)
    }
    return result;
}

void camera_mode5_store_acceleration(void* creature,U32 amount_bits) {
    __asm {
        fld dword ptr amount_bits
        fchs
        mov eax, creature
        fstp dword ptr [eax+448h]
    }
}

const float k_zero = 0.0f;
const float k_minus_sixty_0074fc48 = -60.0f;
const float k_scroll_epsilon_00747504 = 9.999999747378752e-06f;   // bits 3727C5AC
const double k_two_pi_00751840 = 6.2831852;                       // qword 00751840
const float k_thousand_0073d6fc = 1000.0f;
const float k_one_0073d764 = 1.0f;
const float k_deg_to_rad_007415f8 = 0.01745329238474369f;         // bits 3C8EFA35
inline void* behavior(void* camera, I32 type) { return vcall<void*, I32>(camera, 0x80, type); } // Camera::GetBehavior
inline void set_behavior(void* camera, void* b) { vcall<void, void*>(camera, 0x7C, b); }        // Camera::SetBehavior
// Default-mode stick lookup used by Tilt/Turn/AcclTurn/Zoom: GetBehavior(-1)->vtbl[1Ch]().
void* default_stick(void* camera) {
    if (camera == nullptr) return nullptr;
    void* b = behavior(camera, -1);
    if (b == nullptr) return nullptr;
    return vcall<void*>(b, 0x1C);
}
}  // namespace

// 0063FCC0 CSWCModule::TiltCamera(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_0063fcc0(void* module, void*, float a) {
    const U32 mode = lw(module, 0x0C);                                                     // 0063FCC3
    if (mode == 3) {                                                                       // 0063FCC6..0063FCCC
        void* b = behavior(lp(module, 0x40), 0x106A);
        if (b != nullptr) sf(b, 0x104, a);                                                 // 0063FCDB..0063FCE7
        return;
    }
    if (mode == 5) {                                                                       // 0063FCF1..0063FCF4
        void* b = behavior(lp(module, 0x40), 0x106E);
        if (b != nullptr) sf(b, 0x38, a);                                                  // 0063FD03..0063FD0F
        return;
    }
    if (mode == 7) {                                                                       // 0063FD16..0063FD19
        void* b = behavior(lp(module, 0x40), 0x1071);
        if (b != nullptr) sf(b, 0x18, static_cast<float>(static_cast<double>(a) * k_minus_sixty_0074fc48)); // 0063FD2C..0063FD37
        return;
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 0063FD3D..0063FD58
        const float v = static_cast<float>(static_cast<double>(a) + lf(stick, 0x48));      // 0063FD5A..0063FD5E
        sf(stick, 0x48, v);                                                                // 0063FD61 fst
        sf(module, 0x94, v);                                                               // 0063FD64 fstp (same rounding)
    }
    // 0063FD6A..0063FD9C: +80h |= (a != 0 or unordered) ? 1 : 0
    sw(module, 0x80, lw(module, 0x80) | (jcc_jnp(a, k_zero, 0x44) ? 0u : 1u));
}

// 0063FDB0 CSWCModule::AcclTiltCamera(float) (thiscall, RET 4). Returns EAX as the original leaves it.
extern "C" U32 __fastcall kotor_fastcall_0063fdb0(void* module, void*, float a) {
    const U32 mode = lw(module, 0x0C);                                                     // 0063FDB0
    if (mode == 3) return ad(behavior(lp(module, 0x40), 0x106A));                          // 0063FDB3..0063FDC5 tail jmp
    if (mode == 5) {                                                                       // 0063FDCB..0063FDCE
        void* b = behavior(lp(module, 0x40), 0x106E);                                      // 0063FDD0..0063FDDA
        if (b != nullptr) sf(b, 0x24, static_cast<float>(-static_cast<double>(a)));        // 0063FDE0..0063FDEA
        return ad(b);
    }
    if (mode == 7) return ad(behavior(lp(module, 0x40), 0x1071));                          // 0063FDF0..0063FE02 tail jmp
    return mode;                                                                           // 0063FDED (eax = mode)
}

// 0063FE10 CSWCModule::TurnCamera(float a, float b) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0063fe10(void* module, void*, U32 a_bits, U32 b_bits) {
    const float a = bits_f(a_bits);
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0) {                             // 0063FE10..0063FE29
        // 0063FE2B..0063FE53: combat camera (behaviour 1070h, CSWBehaviorCameraCombat::SetTurn 006D1870).
        void* const b = behavior(lp(module, 0x40), 0x1070);                                // 0063FE2B..0063FE32
        if (b == nullptr) return;                                                          // 0063FE38..0063FE3A
        sw(b, 0x2C, 0);                                                                    // 0063FE4C
        kotor_fastcall_006d1870(b, nullptr, -a);                                           // 0063FE40..0063FE53 (RET4)
        return;                                                                            // 0063FE58
    }
    const U32 mode = lw(module, 0x0C);                                                     // 0063FE60
    if (mode == 3) {                                                                       // 0063FE63..0063FE66
        void* b = behavior(lp(module, 0x40), 0x106A);                                      // 0063FE68..0063FE6F
        if (b != nullptr) { sw(b, 0x100, a_bits); sw(b, 0x10C, 0); }                       // 0063FE75..0063FE87
        return;
    }
    if (mode == 5) {                                                                       // 0063FE99..0063FE9C
        void* free_look=behavior(lp(module,0x40),0x106E);
        if (free_look==nullptr) return; // 0063FEB3 -> common return00640087
        const U32 angle_bits=camera_mode5_turn_angle(free_look,a_bits,b_bits);
        void* cc=client_get_player_creature_005ed540(client_app(),nullptr);
        if (cc==nullptr) return;
        sw(cc,0x448,0);
        W4CamQuaternion rotation;
        rotation=*camera_mode5_orientation(&rotation,reinterpret_cast<const W4Vector*>(static_cast<U8*>(cc)+0x30),angle_bits);
        W4Vector direction;
        vcall<void,void*>(cc,0x88,quaternion_zdir_0043e250(&rotation,nullptr,&direction));
        if (kotor_m15a_creature_get_server_creature_0060fb20(cc,nullptr)==nullptr) return;
        void* server_direction=quaternion_zdir_0043e250(&rotation,nullptr,&direction);
        // Original deliberately performs a second lookup after the direction call.
        w1x_object_set_orientation_004cd170(kotor_m15a_creature_get_server_creature_0060fb20(cc,nullptr),nullptr,server_direction);
        return;
    }
    if (mode == 7) {                                                                       // 00640005..00640008
        void* b = behavior(lp(module, 0x40), 0x1071);                                      // 0064000A..00640011
        if (b != nullptr) sf(b, 0x14, static_cast<float>(static_cast<double>(a) * k_minus_sixty_0074fc48)); // 0064001B..00640025
        return;
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 00640030..0064004B
        const float v = static_cast<float>(static_cast<double>(a) + lf(stick, 0x40));      // 0064004D..00640051
        sf(stick, 0x40, v);                                                                // 00640054
        sf(module, 0x98, v);                                                               // 00640057
    }
    if (jcc_jp(a, k_zero, 0x44)) {                                                         // 0064005D..0064006C (a != 0)
        sw(module, 0x100, 1);                                                              // 0064007B
        sw(module, 0x80, lw(module, 0x80) | 1u);                                           // 0064006E..00640081
    }
}

// 00640090 CSWCModule::AcclTurnCamera(float a, float b) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_00640090(void* module, void*, U32 a_bits, U32) {
    const float a = bits_f(a_bits);
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0) {                             // 00640090..006400A5
        // 006400A7..006400CD: combat camera (behaviour 1070h, SetTurn 006D1870; then +2C = -a).
        void* const b = behavior(lp(module, 0x40), 0x1070);                                // 006400A7..006400AE
        if (b == nullptr) return;                                                          // 006400B6..006400B8
        kotor_fastcall_006d1870(b, nullptr, 0.0f);                                         // 006400BE..006400C2 (RET4)
        sf(b, 0x2C, -a);                                                                   // 006400C7..006400CD
        return;                                                                            // 006400D0
    }
    const U32 mode = lw(module, 0x0C);                                                     // 006400D4
    if (mode == 3) {                                                                       // 006400D7..006400DA
        void* b = behavior(lp(module, 0x40), 0x106A);                                      // 006400DC..006400E3
        if (b != nullptr) { sw(b, 0x100, 0); sw(b, 0x10C, a_bits); }                       // 006400E9..006400FF
        return;
    }
    if (mode == 5) {                                                                       // 00640109..0064010C
        if (behavior(lp(module,0x40),0x106E)==nullptr) return;
        void* cc=client_get_player_creature_005ed540(client_app(),nullptr);
        if (cc==nullptr) return;
        camera_mode5_store_acceleration(cc,a_bits);
        return;
    }
    if (mode == 7) {                                                                       // 00640148..0064014B
        void* b = behavior(lp(module, 0x40), 0x1071);                                      // 0064014D..00640154
        if (b != nullptr) sf(b, 0x14, static_cast<float>(static_cast<double>(a) * k_minus_sixty_0074fc48)); // 0064015E..00640169
        return;
    }
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 0064016F..0064018A
        const float v = static_cast<float>(static_cast<double>(a) + lf(stick, 0x40));      // 0064018C..00640190
        sf(stick, 0x40, v);                                                                // 00640193
        sf(module, 0x98, v);                                                               // 00640196
    }
    if (jcc_jp(a, k_zero, 0x44)) {                                                         // 0064019C..006401AB
        sw(module, 0x100, 1);                                                              // 006401BA
        sw(module, 0x80, lw(module, 0x80) | 1u);                                           // 006401AD..006401C0
    }
}

// 006401D0 CSWCModule::ZoomCamera(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006401d0(void* module, void*, float a) {
    if (void* stick = default_stick(lp(module, 0x40))) {                                   // 006401D3..006401F1
        const float v = static_cast<float>(static_cast<double>(a) + lf(stick, 0x34));      // 006401F3..006401F7
        sf(stick, 0x34, v);                                                                // 006401FA
        sf(module, 0x9C, v);                                                               // 006401FD
    }
    if (jcc_jp(a, k_zero, 0x44)) {                                                         // 00640203..00640212
        sw(module, 0x104, 1);                                                              // 00640221
        sw(module, 0x80, lw(module, 0x80) | 1u);                                           // 00640214..00640227
    }
}

// 00640240 CSWCModule::ScrollCamera(Vector) (thiscall, Vector by value, RET 0Ch).
extern "C" void __fastcall kotor_fastcall_00640240(void* module, void*, W4Vector v) {
    // 00640243..00640279: (z*z + y*y) + x*x
    const double m = (static_cast<double>(v.z) * v.z + static_cast<double>(v.y) * v.y) + static_cast<double>(v.x) * v.x;
    if (jcc_jnp(m, k_scroll_epsilon_00747504, 0x41)) return;                               // 0064027B..00640286 (<= eps)
    W4Vector tmp;
    W4Vector p = *vcall<W4Vector*, W4Vector*>(lp(module, 0x40), 0x20, &tmp);              // 00640288..006402AE GetPosition
    vector_add_assign_004a9740(&p, nullptr, &v);                                           // 0064029E..006402B2
    W4Vector returned;                                                                     // [esp+20h] hidden return slot
    vcall<void, W4Vector*, W4Vector>(lp(module, 0x40), 0x18, &returned, p);                // 006402B7..006402DA SetPosition
    sw(module, 0x80, 1);                                                                   // 006402DD
}

// ---- lane x87-trig (2026-10-24): 00641890..006418C8 kept whole ----
// The fcos (006418A2) and fsin (006418B4) results feed fadd / fmulp with their 64-bit significands; the former
// w4::x87_fcos / x87_fsin rounded them to double first. One x87 sequence from the two fild through the dword store
// [esp+10h], with the original operand order (fdivr st,st(1); fdivp st(2),st; the fxch pair). Memory operands:
// k_two_pi_00751840 (qword 401921FB4D12D84A), k_one_0073d764, k_deg_to_rad_007415f8 (bits 3C8EFA35), and the two
// tunables [007A246C] / [007A2470] by address. Precondition as in the original: PC = 53 bits, RC = nearest.
// No CALL inside the asm; extern "C" __cdecl.
extern "C" __declspec(noinline) void __cdecl x87trig_00641890(const I32* remaining, const void* module,
                                                             const float* period, const float* degrees, float* dst) {
    __asm {
        mov eax, remaining
        fild dword ptr [eax]                   // 00641890 ([esp+28h] = [module+DCh])
        fmul qword ptr k_two_pi_00751840       // 00641894
        mov eax, module
        fild dword ptr [eax + 0E8h]            // 0064189A
        fdivr st, st(1)                        // 006418A0 D8 F9
        fcos                                   // 006418A2
        fadd dword ptr k_one_0073d764          // 006418A4
        mov eax, period
        fld dword ptr [eax]                    // 006418AA [007A246C]
        fdivp st(2), st                        // 006418B0 DE FA
        fxch st(1)                             // 006418B2
        fsin                                   // 006418B4
        fxch st(1)                             // 006418B6
        fxch st(1)                             // 006418B8
        fmulp st(1), st                        // 006418BA DE C9
        mov eax, degrees
        fmul dword ptr [eax]                   // 006418BC [007A2470]
        fmul dword ptr k_deg_to_rad_007415f8   // 006418C2
        mov eax, dst
        fstp dword ptr [eax]                   // 006418C8 [esp+10h]
    }
}

// 00641760 CSWCModule::UpdateCameraBumpAndShake(float delta) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00641760(void* module, void*, float delta) {
    void* b_stick = behavior(lp(module, 0x40), 0x106A);                                    // 00641768..00641773 ebx
    void* b_106d = behavior(lp(module, 0x40), 0x106D);                                     // 00641779..00641785 [esp+20h]
    void* b_free = behavior(lp(module, 0x40), 0x106E);                                     // 0064178B..0064179B [esp+14h]
    void* b_combat = behavior(lp(module, 0x40), 0x1070);                                   // 006417A1..006417AF [esp+1Ch]
    if (!b_stick && !b_106d && !b_free && !b_combat) return;                               // 006417B5..006417CB
    if ((lb(kotor_fastcall_005ed700(client_app(), nullptr), 0) & 4) == 0) {                // 006417D1..006417E1
        if (lw(module, 0xF0) != 0) {                                                       // 006417E3..006417E9
            sw(module, 0xF0, 0); sw(module, 0xF4, 0); sw(module, 0xF8, 1);                 // 006417EB..006417F7
        } else if (li(module, 0xDC) > 0) {                                                 // 00641803..00641809
            sw(module, 0xDC, 0); sw(module, 0xE4, 0); sw(module, 0xE0, 0); sw(module, 0xEC, 1); // 0064180B..0064181D
        }
    }
    I32 ms = static_cast<I32>(static_cast<double>(delta) * k_thousand_0073d6fc);           // 00641827..00641831 __ftol2
    const I32 remaining = li(module, 0xDC);                                                // 00641836
    if (remaining <= 0) return;                                                            // 0064183C..00641844
    const I32 wait = li(module, 0xE4);                                                     // 0064184A
    if (wait > 0) {                                                                        // 00641850..00641852
        if (wait <= ms) { ms -= wait; sw(module, 0xE4, 0); }                               // 00641854..0064185A
        else sw(module, 0xE4, static_cast<U32>(wait - ms));                                // 00641862..00641864
    }
    if (lw(module, 0xE4) != 0) return;                                                     // 0064186A..00641870
    double second;                                                                         // ST0 at 0064190E
    U32 first_bits;                                                                        // [esp+28h]
    if (remaining <= ms) {                                                                 // 00641876..00641878
        second = k_zero;                                                                   // 0064187A
        sw(module, 0xDC, 0);                                                               // 00641880
        first_bits = 0;                                                                    // 00641886
    } else {
        float amplitude;                                                                   // [esp+10h]
        x87trig_00641890(&remaining, module, &shake_period_007a246c, &shake_degrees_007a2470, &amplitude); // 00641890..006418C8 (x87)
        const I32 r1 = ((crt_rand_006fc447() + 1) % 2) - 1;     // 006418CC..006418DE ((rand+1)%2)-1
        first_bits = f_bits(static_cast<float>(static_cast<double>(r1) * amplitude));     // 006418DF..006418EB
        const I32 r2 = ((crt_rand_006fc447() + 1) % 2) - 1;     // 006418EF..00641901
        second = static_cast<double>(r2) * amplitude;                                      // 00641902..0064190A
    }



    if (b_stick) { sf(b_stick, 0x94, static_cast<float>(second)); sw(b_stick, 0x90, first_bits); }          // 0064190E..0064191C

    else if (b_combat) { sf(b_combat, 0x18, static_cast<float>(second)); sw(b_combat, 0x14, first_bits); }  // 00641924..00641933
    else if (b_106d) { sf(b_106d, 0x30, static_cast<float>(second)); sw(b_106d, 0x2C, first_bits); }        // 00641938..00641947
    else if (b_free) { sf(b_free, 0x74, static_cast<float>(second)); sw(b_free, 0x70, first_bits); }        // 0064194C..0064195B
    sw(module, 0xDC, lw(module, 0xDC) - static_cast<U32>(ms));                             // 00641962
}

// 00644170 CSWCModule::UpdateCameraCollision(float delta, float zoom) (thiscall, RET 8; ST0).
extern "C" double __fastcall kotor_fastcall_00644170(void* module, void*, float delta, float zoom) {
    if (lw(module, 0x8C) != 0) return zoom;                                                // 00644173..00644182
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 2)                     // 00644185..00644196
        return kotor_fastcall_006433b0(module, nullptr, delta, zoom);                      // 00644198..006441A4
    return kotor_fastcall_006422e0(module, nullptr, delta, zoom);                          // 006441AD..006441B9
}

// 006433B0 CSWCModule::UpdatePushCameraCollision (camera mode 2 only).
// Full body derived from orig-006433B0-push.txt and the read-only Ghidra decompilation.

// ---- declarations used by P4 (local; no header edit) ----
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
struct P4HitInfo {
    alignas(4) U8 b[0x64];
    P4HitInfo() { aurora_hit_info_construct_004578f0(b, nullptr, 0, 0xBFFFFFFFu, 0); }
    ~P4HitInfo() { chit_info_destruct_00456b30(b, nullptr); }
};

float p4_clamp_ordered(float v, float lo, float hi) {
    float candidate = jcc_jp(v, static_cast<double>(hi), 0x05) ? hi : v;
    if (!jcc_jne(static_cast<double>(lo), static_cast<double>(candidate), 0x41))
        candidate = lo;
    else if (!jcc_jnp(static_cast<double>(candidate), static_cast<double>(hi), 0x05))
        candidate = hi;
    return candidate;
}

W4Vector p4_adjust_probe(const W4Vector& p, const W4Vector& dir) {
    return W4Vector{p.x + dir.x * 0.001f, p.y + dir.y * 0.001f, p.z + dir.z * 0.001f};
}
}  // namespace

extern "C" double __fastcall kotor_fastcall_006433b0(void* module, void*, float delta, float zoom) {
    void* const camera = lp(module, 0x40);                                                // 006433D0
    void* const behavior = vcall<void*, I32>(camera, 0x80, -1);                           // 006433D0..006433D8
    if (behavior == nullptr) return 0.0;                                                  // 006433F3
    void* const following = vcall<void*>(behavior, 0x1C);                                 // 006433E4..006433ED
    if (following == nullptr) return 0.0;                                                 // 006433F3
    void* const area = lp(module, 0x48);                                                  // 006433FE
    void* const scene = area != nullptr ? lp(area, 0x184) : nullptr;                      // 00643403..0064340F
    void* const player = client_get_player_creature_005ed540(client_app(), nullptr);      // 00643413..00643421
    if (collision_enabled_007a2468 == 0 || camera == nullptr ||
        scene == nullptr || player == nullptr || kotor_fastcall_0063f5e0(player, nullptr) == 0)
        return zoom;                                                                      // 00643423..00643453, 00643F1D
    W4Vector camera_pos;
    vcall<W4Vector*, W4Vector*>(camera, 0x20, &camera_pos);                               // 00643459..00643466
    W4Vector target;
    vector_sum_00423b90(&target, reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x24),
                        reinterpret_cast<const W4Vector*>(static_cast<U8*>(module) + 0x64)); // 0064346E..006434A6
    const W4Vector d{static_cast<float>(static_cast<double>(target.x) - camera_pos.x),
                     static_cast<float>(static_cast<double>(target.y) - camera_pos.y),
                     static_cast<float>(static_cast<double>(target.z) - camera_pos.z)};    // 006434B6..006434F1
    if (!(static_cast<double>(d.x) * d.x + static_cast<double>(d.y) * d.y + static_cast<double>(d.z) * d.z > 0.0))
        return zoom;                                                                      // 006434F1..006434F9
    P4HitInfo hit;                                                                        // 00643508..00643516
    W4Vector dir;
    vector_normalized_004ab180(&dir, &d);                                                 // 0064353E..0064358C
    if ((push_state_00833a8c & 1u) == 0) {                                                // 00643590..006435A4
        push_state_00833a8c |= 1u;
        push_cell_00833a80 = 0.0f;
        push_cell_00833a84 = 0.0f;
        push_cell_00833a88 = 0.0f;
    }
    if (push_cell_00833a80 == 0.0f && push_cell_00833a84 == 0.0f && push_cell_00833a88 == 0.0f) {
        push_cell_00833a80 = lf(player, 0x24);                                             // 00643625
        push_cell_00833a84 = lf(player, 0x28);                                             // 0064362E
        push_cell_00833a88 = lf(player, 0x2C);                                             // 00643636
    }
    const double camera_yaw = vcall<double>(camera, 0x64);                                // 0064363C..00643643
    const float yaw = static_cast<float>(camera_yaw + static_cast<double>(delta) * lf(module, 0x98));
    const double camera_pitch = vcall<double>(camera, 0x68);                              // 00643651..00643658
    float pitch = static_cast<float>(camera_pitch + static_cast<double>(delta) * lf(module, 0x94));
    const float factor = static_cast<float>(w4cam::x87_pow(2.0, -0.5 * static_cast<double>(delta) * zoom)); // 00643672..00643691
    float dist = static_cast<float>(vcall<double>(camera, 0x6C) * static_cast<double>(factor)); // 0064369A..006436A1
    pitch = p4_clamp_ordered(pitch, follow_7a2458, follow_pitch_007a245c);                 // 006436A5..006436F5
    dist = p4_clamp_ordered(dist, follow_7a2450, follow_7a2454);                           // 006436F9..00643749
    const float t = static_cast<float>(1.0 - w4cam::x87_pow(0.5, static_cast<double>(delta) * 750.0)); // 0064374D..00643779
    const W4Vector up{0.0f, 0.0f, 1.0f};                                                   // 0064377D..0064378D
    W4Vector head;
    vector_sum_00423b90(&head, reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x24), &up); // 00643795
    W4Vector head_scaled;
    vector_component_mult_00423bf0(&head_scaled, &head, t);                                // 006437C0
    const W4Vector state{push_cell_00833a80, push_cell_00833a84, push_cell_00833a88};
    W4Vector state_scaled;
    vector_component_mult_00423bf0(&state_scaled, &state, 1.0f - t);                       // 006437D2..006437E4
    W4Vector interp;
    vector_sum_00423b90(&interp, &state_scaled, &head_scaled);                             // 006437F5
    W4CamQuaternion q;
    yaw_pitch_roll_004acac0(&q, yaw, pitch, 0.0f);                                        // 00643821
    const W4CamQuaternion target_q = q;
    W4CamQuaternion* const previous = vcall<W4CamQuaternion*, W4CamQuaternion*>(camera, 0x24, &q); // 0064385D
    W4CamQuaternion slerped;
    quaternion_slerp_004a9e00(&slerped, *previous, &target_q, t);                         // 00643889
    W4CamQuaternion normalized;
    quaternion_normalize_004a9d60(&normalized, slerped);                                  // 006438B1
    W4Vector zdir;
    quaternion_zdir_0043e250(&normalized, nullptr, &zdir);                                 // 006438F4
    W4Vector offset;
    vector_component_mult_00423bf0(&offset, &zdir, dist);                                  // 00643902
    W4Vector probe_to;
    vector_sum_00423b90(&probe_to, &interp, &offset);                                      // 00643915
    push_cell_00833a80 = interp.x;                                                         // 0064393B
    push_cell_00833a84 = interp.y;                                                         // 00643948
    push_cell_00833a88 = interp.z;                                                         // 0064394E
    const float zoom_nonpositive = jcc_jne(zoom, 0.0, 0x41) ? 1.0f : 0.0f;                // 0064391A..00643966
    kotor_fastcall_00456af0(hit.b, nullptr, 0, 0xBFFFFFFFu, 0);                           // 00643966..00643976
    sw(hit.b, 0, 1);                                                                       // 00643982
    W4Vector probe_out{0.0f, 0.0f, 0.0f};                                                  // 006435FC..006435F4
    float selected = 0.0f;
    int hit_flag = 0;
    int camera_valid = 1;

    const auto do_probe = [&](const W4Vector& from, const W4Vector& to) -> U32 {
        if (push_area_flag_00833a64 == 0) {
            return vcall<U32, W4Vector, W4Vector, W4Vector*, void*>(scene, 0x38, from, to, &probe_out, hit.b);
        } else {
            const U32 los = kotor_fastcall_00605370(area, nullptr, from, to, &probe_out);
            return los == 0 ? 1u : 0u;
        }
    };

    const U32 first_hit = do_probe(target, probe_to);                                      // 006439C9..00643A09
    if (first_hit == 0) {
        if (0.0f < push_last_distance_00833a7c) {                                          // 00643A11..00643A1B
            W4Vector scaled;
            vector_component_mult_00423bf0(&scaled, &dir, push_last_distance_00833a7c);    // 00643A1B
            W4Vector from2;
            kotor_cdecl_00423bc0(&from2, &target, &scaled);                                // 00643A2F..00643A4F
            kotor_fastcall_00456af0(hit.b, nullptr, 0, 0xBFFFFFFFu, 0);                   // 00643966 equivalent
            sw(hit.b, 0, 1);
            const float camera_dist = static_cast<float>(vcall<double>(camera, 0x6C));      // 00643ACB..00643AD6
            U32 second_hit;
            if (!(camera_dist < push_last_distance_00833a7c))                          // 00643B58..00643B68 (unordered -> from2)
                second_hit = do_probe(from2, probe_to);
            else
                second_hit = do_probe(probe_to, from2);
            if (second_hit == 0) {                                                         // 00643C68..00643D49
                const float d2 = static_cast<float>(vcall<double>(camera, 0x6C));
                const float adjustment = static_cast<float>((static_cast<double>(push_last_distance_00833a7c) - d2) *
                                                            static_cast<double>(delta) * 3.5);
                const float d3 = static_cast<float>(vcall<double>(camera, 0x6C));
                const float last = push_last_distance_00833a7c;
                selected = static_cast<float>(static_cast<double>(d3) + adjustment);
                hit_flag = 1;
                if (push_last_distance_00833a7c <= selected) {
                    push_last_distance_00833a7c = 0.0f;
                    selected = last;
                }
            } else {                                                                       // 00643C73..00643D49
                probe_out = p4_adjust_probe(probe_out, dir);
                const float d2 = static_cast<float>(vcall<double>(camera, 0x6C));
                const float adjustment = static_cast<float>((static_cast<double>(push_last_distance_00833a7c) - d2) *
                                                            static_cast<double>(delta) * 3.5);
                const float d3 = static_cast<float>(vcall<double>(camera, 0x6C));
                const float candidate = static_cast<float>(static_cast<double>(d3) + adjustment);
                W4Vector diff;
                kotor_cdecl_00423bc0(&diff, &probe_out, &target);
                const float magnitude = static_cast<float>(vector_magnitude_004ab100(&diff));
                selected = (static_cast<double>(magnitude) * magnitude <= static_cast<double>(candidate) * candidate)
                               ? magnitude : candidate;
            }
        }
    } else {                                                                               // 00643A11..00643C68 hit path
        probe_out = p4_adjust_probe(probe_out, dir);
        W4Vector diff;
        kotor_cdecl_00423bc0(&diff, &probe_out, &target);
        selected = static_cast<float>(vector_magnitude_004ab100(&diff));                   // 00643D57
        if (push_last_distance_00833a7c == 0.0f)
            push_last_distance_00833a7c = static_cast<float>(vcall<double>(camera, 0x6C));
    }
    float ret_dist = zoom;                                                                 // 00643536 / 00643F1D
    if (zoom_nonpositive == 0.0f)                                                          // 00643DAE..00643DB6
        push_last_distance_00833a7c = 0.0f;
    if ((selected == 0.0f && static_cast<float>(vcall<double>(camera, 0x6C)) < follow_7a2450) ||
        (0.0f < selected && 1e-4f <= follow_7a2450 - selected)) {                         // 00643D89..00643DB6
        if (push_last_distance_00833a7c < follow_7a2450) push_last_distance_00833a7c = follow_7a2450;
        ret_dist = 0.0f;
        camera_valid = 0;
    }
    if (kotor_cdecl_0060b2a0(nullptr) != 0) {                                              // 00643E0E..00643E28
        sf(following, 0x64, camera_valid == 0 ? 0.0f : 1.0f);                              // 00643E2D
        if (0.0f < selected) {                                                             // 00643E3B
            sf(following, 0x30, selected);                                                 // 00643E7A
            if (hit_flag == 0) ret_dist = 0.0f;
            sw(module, 0x80, 1);                                                           // 00643E88
        }
        const float cd1 = static_cast<float>(vcall<double>(camera, 0x6C));                 // 00643EB5
        if (((cd1 < follow_7a2450) != (cd1 == follow_7a2450)) && 0.0f < ret_dist) ret_dist = 0.0f;
        const float cd2 = static_cast<float>(vcall<double>(camera, 0x6C));                 // 00643EDB
        if (follow_7a2454 <= cd2 && ret_dist < 0.0f) ret_dist = 0.0f;
    }
    return ret_dist;                                                                       // 00643F18..00643F3C
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
            sf(ds, 0x68, 5.0f); sf(ds, 0x6C, 20.0f); sf(ds, 0x5C, 1.0f); sf(ds, 0x60, 50.0f); // 00640791..006407B0
            follow_7a2450 = 5.0f; follow_7a2454 = 20.0f; follow_7a2458 = 1.0f; follow_pitch_007a245c = 50.0f; // 006407B3..006407D1
            sf(module, 0x64, 0.0f); sf(module, 0x68, 0.0f); sf(module, 0x6C, 1.0f);      // 006407D7..00640801
            sf(ds, 0x24, 0.0f); sf(ds, 0x28, 0.0f); sf(ds, 0x2C, 1.0f);                  // 00640804..0064080D
            float near_dist, far_dist;
            vcall<void, float*, float*>(lp(module, 0x40), 0x3C, &near_dist, &far_dist);  // 00640814..0064081F
            vcall<void, float, float>(lp(module, 0x40), 0x40, near_dist, 45.0f);         // 00640822..00640831
            sf(ds, 0x44, 50.0f); sf(ds, 0x30, 20.0f);                                    // 00640838..0064083D
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
    W4CamQuaternion q;
    W4CamQuaternion* const qp = vcall<W4CamQuaternion*, W4CamQuaternion*>(lp(module, 0x40), 0x24, &q); // 00640929..00640933
    const double camera_yaw = quaternion_yaw_004a9f40(qp);                               // 00640936..00640937
    const float camera_yaw_f = static_cast<float>(camera_yaw);                           // 0064093C
    const double player_yaw = kotor_m15a_yaw_004aa0f0(reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x30)); // 00640940..00640944
    double angle = player_yaw - static_cast<double>(camera_yaw_f);                       // 00640949
    while (jcc_jnp(angle, -180.0, 0x05)) angle += 360.0;                                 // 00640950..00640973
    while (jcc_je(angle, 180.0, 0x41)) angle -= 360.0;                                   // 00640973..00640993
    if (lw(player, 0x138) == 0) {                                                        // 00640993..00640999
        if (!jcc_jne(angle, 90.0, 0x41) && lb(module, 0x90) == 2) angle = -90.0;        // 0064099B..006409B9
        else if (!jcc_jp(angle, -90.0, 0x05) && lb(module, 0x90) == 1) angle = 90.0;     // 006409BB..006409D3
    }
    if (jcc_jne(std::fabs(angle), 1.0, 0x41)) { sw(module, 0x84, 0); return; }           // 006409D9..00640A62
    if (lw(module, 0x84) != 0 || !jcc_jne(std::fabs(angle), 30.0, 0x41)) {              // 006409DB..00640A09
        double clamped = angle;
        if (!jcc_jne(clamped, 90.0, 0x41)) clamped = 90.0;                              // 00640A0B..00640A20
        else if (!jcc_jp(clamped, -90.0, 0x05)) clamped = -90.0;                         // 00640A22..00640A35
        if (stick != nullptr) sf(stick, 0x40, static_cast<float>(clamped));              // 00640A37..00640A47
        sw(module, 0x80, 1); sw(module, 0x84, 1);                                         // 00640A4A..00640A52
    }
}
extern "C" void __fastcall kotor_fastcall_00640a80(void* module, void*) {
    if (lw(module, 0xD8) == 0) return;                                                    // 00640A86..00640A8E
    void* const b = behavior(lp(module, 0x40), -1);                                        // 00640A94..00640A9B
    if (b == nullptr) return;                                                              // 00640AA1..00640AA3
    void* const s = vcall<void*>(b, 0x1C);                                                // 00640AA9..00640AB5
    if (s == nullptr) return;                                                              // 00640AB3..00640AB5
    sf(s, 0x68, 5.0f); sf(s, 0x6C, 20.0f); sf(s, 0x5C, 1.0f); sf(s, 0x60, 50.0f);         // 00640ABB..00640AD1
    follow_7a2450 = 5.0f; follow_7a2454 = 20.0f; follow_7a2458 = 1.0f; follow_pitch_007a245c = 50.0f; // 00640AD8..00640AF6
    sf(module, 0x64, 0.0f); sf(module, 0x68, 0.0f); sf(module, 0x6C, 1.0f);                // 00640B00..00640B2F
    sf(s, 0x24, 0.0f); sf(s, 0x28, 0.0f); sf(s, 0x2C, 1.0f);                               // 00640B32..00640B3B
    float near_dist, far_dist;
    vcall<void, float*, float*>(lp(module, 0x40), 0x3C, &near_dist, &far_dist);            // 00640B42..00640B4D
    vcall<void, float, float>(lp(module, 0x40), 0x40, near_dist, 45.0f);                   // 00640B50..00640B5F
    sf(s, 0x30, 20.0f); sf(s, 0x44, 50.0f);                                                // 00640B62..00640B69
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
        sf(stick, 0x5C, 1.0f); sf(stick, 0x60, 60.0f); sf(stick, 0x68, 6.0f); sf(stick, 0x6C, 6.0f); // 00640BFF..00640C15
        follow_7a2450 = 6.0f; follow_7a2454 = 6.0f; follow_7a2458 = 1.0f; follow_pitch_007a245c = 60.0f; // 00640C18..00640C2E
        sf(module, 0x64, 0.0f); sf(module, 0x68, 0.0f); sf(module, 0x6C, 2.0f);          // 00640C38..00640C67
        sf(stick, 0x24, 0.0f); sf(stick, 0x28, 0.0f); sf(stick, 0x2C, 2.0f);             // 00640C6A..00640C73
        float near_dist, far_dist;
        vcall<void, float*, float*>(lp(module, 0x40), 0x3C, &near_dist, &far_dist);       // 00640C7A..00640C85
        vcall<void, float, float>(lp(module, 0x40), 0x40, near_dist, 45.0f);              // 00640C88..00640C97
        sf(stick, 0x30, 6.0f); sf(stick, 0x44, 60.0f);                                    // 00640C9E..00640CA3
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
    const double camera_yaw = vcall<double>(lp(module, 0x40), 0x64);                     // 00640D8D..00640D92
    const float camera_yaw_f = static_cast<float>(camera_yaw);                           // 00640D95
    const double player_yaw = kotor_m15a_yaw_004aa0f0(reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x30)); // 00640D99..00640D9D
    const float player_yaw_f = static_cast<float>(player_yaw);                           // 00640DA2
    double angle = player_yaw - static_cast<double>(camera_yaw_f);                       // 00640DA6
    const float angle_raw_f = static_cast<float>(angle);                                 // 00640DB3 (pre-wrap spill)
    while (jcc_jnp(angle, -180.0, 0x05)) angle += 360.0;                                 // 00640DAD..00640DD3
    while (jcc_je(angle, 180.0, 0x41)) angle -= 360.0;                                   // 00640DD3..00640DF3
    if (lw(player, 0x138) == 0) {                                                        // 00640DF3..00640DF9
        if (!jcc_jne(angle, 90.0, 0x41) && lb(module, 0x90) == 2) angle = -90.0;        // 00640DFB..00640E19
        else if (!jcc_jp(angle, -90.0, 0x05) && lb(module, 0x90) == 1) angle = 90.0;     // 00640E1B..00640E33
    }
    if (jcc_jne(std::fabs(angle), 1.0, 0x41)) { sw(module, 0x84, 0); return; }           // 00640E39..00640F59
    double b_angle = angle * 5.0;                                                        // 00640E4E
    if (!jcc_jne(b_angle, 180.0, 0x41)) b_angle = 180.0;                                // 00640E54..00640E63
    else if (!jcc_jp(b_angle, -180.0, 0x05)) b_angle = -180.0;                           // 00640E6B..00640E7A
    double c_angle = static_cast<double>(player_yaw_f) - (static_cast<double>(camera_yaw_f) + b_angle * static_cast<double>(delta)); // 00640E80..00640E8A
    while (jcc_jnp(c_angle, -180.0, 0x05)) c_angle += 360.0;                             // 00640E8E..00640EB3
    while (jcc_je(c_angle, 180.0, 0x41)) c_angle -= 360.0;                               // 00640EB3..00640ED3
    double selected;
    if (!jcc_jne(b_angle, 9.999999747378752e-05, 0x41)) {                                // 00640ED3..00640EE0
        if (jcc_jnp(c_angle, 9.999999747378752e-05, 0x05)) selected = static_cast<double>(angle_raw_f) / static_cast<double>(delta); // 00640EE2..00640F17
        else selected = b_angle;
    } else if (jcc_jp(b_angle, 9.999999747378752e-05, 0x05)) {                           // 00640EEF..00640EFC
        selected = b_angle;
    } else if (jcc_jne(c_angle, 9.999999747378752e-05, 0x41)) {                          // 00640EFE..00640F09
        selected = b_angle;
    } else {
        selected = static_cast<double>(angle_raw_f) / static_cast<double>(delta);            // 00640F0B..00640F15
    }
    if (stick != nullptr) sf(stick, 0x40, static_cast<float>(selected));                 // 00640F27..00640F33
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
void set_stick_heights(void* stick, void* creature) {
    sw(stick, 0x68, ad(vcall<void*>(creature, 0xD4)));                                     // 00641B6C..00641B7E
    const float head = static_cast<float>(vcall<double>(creature, 0xD8));                  // 00641B81..00641B8B fstp float
    sf(stick, 0x6C, static_cast<float>(kotor_fastcall_0060f010(creature, nullptr) + head)); // 00641B8F..00641B9B
}
}  // namespace

// 006419A0 CSWCModule::UpdateCameraFollow(float delta) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006419a0(void* module, void*, float delta) {
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
    case 2: kotor_fastcall_00640b90(module, nullptr, delta); break;                        // 00641BFF
    case 3: case 4: return;                                                                // table -> 00641C84 epilogue
    default: break;                                                                        // ja 00641C0B
    }
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 0 ||
        lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 1) {                   // 00641C0B..00641C32
        void* b = behavior(lp(module, 0x40), -1);                                          // 00641C34..00641C3B
        if (b != nullptr) (void)vcall<void*>(b, 0x1C);                                     // 00641C41..00641C49
    }
    sf(module, 0x74, static_cast<float>(vcall<double>(lp(module, 0x40), 0x6C)));           // 00641C4C..00641C54 GetDist
    sf(module, 0x78, static_cast<float>(vcall<double>(lp(module, 0x40), 0x64)));           // 00641C57..00641C5F GetYaw
    sf(module, 0x7C, static_cast<float>(vcall<double>(lp(module, 0x40), 0x68)));           // 00641C62..00641C6A GetPitch
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
        else position = W4Vector{0.0f, 0.0f, 0.0f};                                        // 0063FAA0..0063FAB0
        set_stick_heights(stick, creature);                                                // 0063FAB8..0063FAE3
        kotor_fastcall_0063a380(stick, nullptr, vcall<void*>(creature, 0xD0), flag);       // 0063FAE6..0063FAF3 ChangeTarget
        sw(module, 0x70, id);                                                              // 0063FAFA..0063FAFE
        if (flag != 0) return;                                                             // 0063FB01
        kotor_fastcall_0063f630(stick, nullptr, position);                                 // 0063FB07..0063FB22 SetPosition
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
        sf(stick, 0x44, follow_pitch_007a245c);                                            // 0063FBEC..0063FBF3
        sw(stick, 0x38, 0x443B8000u);                                                      // 0063FBF6 750.0f
        if (kotor_cdecl_0060b2a0(nullptr) != 0) {                                          // 0063FBF1..0063FC07
            sw(stick, 0x58, 0x3F800000u); sw(stick, 0x64, 0x3F800000u);                    // 0063FC09..0063FC11
        }
        sf(stick, 0x5C, follow_7a2458);                                                    // 0063FC14..0063FC1A
        sf(stick, 0x60, follow_pitch_007a245c);                                            // 0063FC1D..0063FC23
        sf(stick, 0x68, follow_7a2450);                                                    // 0063FC26..0063FC2B
        sf(stick, 0x6C, follow_7a2454);                                                    // 0063FC2E..0063FC34
        if (jcc_jp(lf(module, 0x74), k_zero, 0x44)) {                                      // 0063FC37..0063FC45 (+74h != 0)
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
