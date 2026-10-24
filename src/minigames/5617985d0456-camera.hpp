#pragma once
// Lane W4 G14: client camera (CClientExoAppInternal::UpdateCamera, CSWCModule camera controls,
// CSWCameraOnAStick follow behaviour, CSWRK4 integrator). x86 only; x87 policy in ../common/w4_native.hpp.
#include "fe91ac507a49-w4_frame.hpp"

// ---- existing runtime1115 cells (exact map symbols) ----
extern "C" volatile unsigned char aurora_model_gate_007b8fc0;         // _aurora_model_gate_007b8fc0 (0478)
extern "C" float aurora_hit_box_margin_007a8a4c[3];                   // _aurora_hit_box_margin_007a8a4c (0548)
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }  // CSWGuiManager* (0392)
namespace kotor_input_cells_required { extern volatile int word_008338f0; }  // (0774) free-look mouse flag
namespace menu_tables_m41 { namespace cells {
extern float keyboard_camera_007a2320, keyboard_camera_007a2324, keyboard_camera_007a2328;  // (0366)
} }
extern "C" void* kotor_cell_rules_007a3a28;                            // _kotor_cell_rules_007a3a28 CSWRules*
extern "C" std::uint32_t kotor_rules_cell32_007a20c8;                  // _kotor_rules_cell32_007a20c8

// ---- W4-owned camera cells (no runtime1115 owner; initial values = original .data) ----
namespace w4cam {
extern std::uint32_t invert_mouse_007a22a4;          // 1      (read only: UpdateCamera)
extern float edge_scroll_fraction_007a2444;          // 0.001f (read only: UpdateCamera)
extern std::uint32_t turn_sign_00832920;             // 0      (UpdateCamera, Control_UpdateCameraDesiredOrientation)
extern float accel_interp_min_007a2430;              // 2.5f
extern float desired_turn_rate_007a2434;             // 7.5f
extern std::int32_t style_request_007a242c;          // -1
extern std::uint32_t dialog_camera_debug_008327ec;   // 0 (also read by ShowObject, combat camera, CSWSCreature::AIUpdate)
extern std::uint32_t free_style_debug_00833938;      // 0
extern std::uint32_t orientation_debug_0083394c;     // 0
extern float pending_yaw_007a2440;                   // -1.0f
extern float turn_exponent_007a2448;                 // 2.0f
extern float shake_period_007a246c;                  // 30.0f
extern float shake_degrees_007a2470;                 // 20.0f
extern float follow_pitch_007a245c;                  // 50.0f
extern float follow_7a2458;                          // 1.0f
extern float follow_7a2450;                          // 5.0f
extern float follow_7a2454;                          // 20.0f
extern std::uint32_t lock_camera_007a22b8;           // 1
extern std::uint32_t collision_enabled_007a2468;     // 1
extern float new_position_scale_007a243c;            // 0.5f
}

// ---- existing runtime1115 providers ----
extern "C" {
void* __fastcall kotor_fastcall_005ed500(void* app, void*);                         // CClientExoApp::GetPauseTimer
std::uint32_t __fastcall kotor_fastcall_005ed470(void* app, void*);                 // CClientExoApp::GetCaptureEvents
void* __fastcall kotor_fastcall_005ed580(void* app, void*, std::uint32_t id);       // CClientExoApp::GetGameObject
void* __fastcall client_get_player_creature_005ed540(void* app, void*);             // CClientExoApp::GetPlayerCreature
void* __fastcall camera_get_behavior_0045c220(void* camera, void*, std::int32_t type);  // Camera::GetBehavior (vtable +80h)
void __fastcall behavior_add_dependency_0049f130(void* behavior, void*, void* object);  // CAurBehavior::AddDependency
std::uint32_t __fastcall behavior_control_0049f270(void* behavior, void*, float delta); // CAurBehavior::Control
void* __fastcall safe_pointer_assign_00447fa0(void* safe, void*, void* object);      // SafePointer<CAurObject>::operator=
void __fastcall exo_array_list_remove_00445080(void* list, void*, void* element);   // CExoArrayList::Remove
// math (cdecl unless noted). Functions whose result the original consumes on ST0 are declared `double`.
W4Vector* __cdecl vector_normalized_004ab180(W4Vector* out, const W4Vector* in);   // normalize
W4Vector* __cdecl vector_cross_004ab210(W4Vector* out, const W4Vector* a, const W4Vector* b);
double __cdecl kotor_m15a_yaw_004aa0f0(const W4Vector* v);                         // Yaw(Vector*) (ST0 kept at PC53: D140)
double __cdecl kotor_m15a_pitch_004aa130(const W4Vector* v);                       // Pitch(Vector*)
void* __cdecl yaw_pitch_roll_004acac0(void* out_quaternion, float yaw, float pitch, float roll);  // YawPitchRoll
W4Vector* __fastcall vector_subtract_assign_004a9770(W4Vector* self, void*, const W4Vector* other); // Vector::operator-=
W4Vector* __fastcall vector_add_assign_004a9740(W4Vector* self, void*, const W4Vector* other);      // Vector::operator+=
W4Vector* __fastcall quaternion_zdir_0043e250(const void* quaternion, void*, W4Vector* out);       // Quaternion::zdir
W4Vector* __fastcall quaternion_ydir_0044f420(const void* quaternion, void*, W4Vector* out);       // Quaternion::ydir
W4Vector* __cdecl quaternion_rotate_004a9bb0(W4Vector* out, const void* quaternion, const W4Vector* v); // operator*(Quat,Vector)
void* __cdecl quaternion_multiply_004a9ac0(void* out, const void* a, const void* b);    // operator*(Quat,Quat)
W4Vector* __cdecl vector_divide_00437a90(W4Vector* out, const W4Vector* in, float d);   // operator/(Vector,float)
double __cdecl vector_magnitude_004ab100(const W4Vector* v);                            // vector_magnitude
W4Vector* __cdecl kotor_cdecl_00423bc0(W4Vector* out, const W4Vector* a, const W4Vector* b); // Vector operator-(a,b)
W4Vector* __cdecl vector_sum_00423b90(W4Vector* out, const W4Vector* a, const W4Vector* b);
W4Vector* __cdecl vector_component_mult_00423bf0(W4Vector* out, const W4Vector* a, float s);
double __cdecl genuine_x87_pow_006fc1f0_st(double x, double y);                        // see camera_pow below
void __fastcall kotor_m15a_creature_update_shadow_blob_0060e5b0(void* creature, void*); // CSWCCreature::UpdateShadowBlob
int __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, int row, std::uint32_t column, float* out); // C2DA::GetFLOATEntry(int,int,float*)
}
namespace menu_continuation { struct Client; }

// ---- W4 camera definitions (thiscall as __fastcall) ----
extern "C" {
void* __fastcall kotor_fastcall_005df610(void* input, void*, float* dx, float* dy);  // CExoInput::GetMouseDelta (returns nothing; RET 8)
void __fastcall kotor_fastcall_005e00f0(void* internal, void*, float* dx, float* dy); // CExoInputInternal::GetMouseDelta
double __fastcall kotor_fastcall_005df4d0(void* input, void*, std::uint32_t event, std::uint32_t cls); // CExoInput::PollInput (W5 pending)
std::uint32_t __fastcall kotor_fastcall_005ede70(void* app, void*);                 // GetCombatMode
double __fastcall kotor_fastcall_0061d5b0(void* options, void*);                    // GetMouseSenSetting
double __fastcall kotor_fastcall_0061d5e0(void* options, void*);                    // GetCameraKeyboardDPS
double __fastcall kotor_fastcall_0061d610(void* options, void*);                    // GetCameraKeyboardAcceleration
double __fastcall kotor_fastcall_0061d640(void* options, void*);                    // GetCameraKeyboardDeceleration
std::uint32_t __cdecl kotor_cdecl_0060b2a0(const char* arg);                        // lockcamera
double __fastcall kotor_fastcall_0060f010(void* creature, void*);                   // CSWCCreature::GetCameraOffset
double __fastcall kotor_fastcall_00697580(void* appearance, void*);                 // CSWCCreatureAppearance::GetCameraOffset
void __fastcall kotor_fastcall_0063fcc0(void* module, void*, float a);              // TiltCamera
std::uint32_t __fastcall kotor_fastcall_0063fdb0(void* module, void*, float a);     // AcclTiltCamera (returns EAX)
void __fastcall kotor_fastcall_006401d0(void* module, void*, float a);              // ZoomCamera
void __fastcall kotor_fastcall_00640240(void* module, void*, W4Vector v);           // ScrollCamera
void __fastcall kotor_fastcall_00641760(void* module, void*, float delta);          // UpdateCameraBumpAndShake
void __fastcall kotor_fastcall_006419a0(void* module, void*, float delta);          // UpdateCameraFollow
double __fastcall kotor_fastcall_00644170(void* module, void*, float delta, float zoom); // UpdateCameraCollision
double __fastcall kotor_fastcall_006422e0(void* module, void*, float delta, float zoom); // UpdateNormalCameraCollision
double __fastcall kotor_fastcall_006433b0(void* module, void*, float delta, float zoom); // UpdatePushCameraCollision
void __fastcall kotor_fastcall_0063f9d0(void* module, void*, std::uint32_t id, std::uint32_t flag); // SetCameraFollow
}

namespace w4cam {
using namespace w4;
// x87 `_pow` (006FC1F0 is the CRT __CIpow-style entry taking its operands on the FPU stack).
double x87_pow(double x, double y);
}
