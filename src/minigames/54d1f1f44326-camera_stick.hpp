#pragma once
// Lane W4 G14: CSWCameraOnAStick (vtable 0075165C, size 138h) and the CSWRK4 integrator it embeds at +B4h.
// Quaternion layout in all of these: {w, x, y, z} (identity = {1,0,0,0}; ctor 0063A00A, ChangeTarget 0063A3D4).
#include "5617985d0456-camera.hpp"

struct W4CamQuaternion { float w, x, y, z; };
static_assert(sizeof(W4CamQuaternion) == 16, "Quaternion");
struct W4RK4State { W4Vector position, velocity; };
static_assert(sizeof(W4RK4State) == 24, "RK4 state");

extern "C" {
// CSWCameraOnAStick (thiscall as __fastcall).
void* __fastcall kotor_fastcall_00639fc0(void* self, void*, void* owner, void* part);           // ctor (RET 8)
void __fastcall kotor_fastcall_0063a320(void* self, void*);                                     // ~CSWCameraOnAStick
void* __fastcall kotor_fastcall_0063ad40(void* self, void*, std::uint32_t flags);               // deleting dtor (slot 00h)
std::uint32_t __fastcall kotor_fastcall_0063a310(void* self, void*);                            // GetType (slot 10h) = 106Ah
void __fastcall kotor_fastcall_0063a360(void* self, void*);                                     // ClearCombatCamera
void __fastcall kotor_fastcall_0063a380(void* self, void*, void* part, std::uint32_t flag);     // ChangeTarget (RET 8)
W4Vector* __fastcall kotor_fastcall_0063f600(void* self, void*, W4Vector* out);                 // GetPosition (RET 4)
void __fastcall kotor_fastcall_0063f630(void* self, void*, W4Vector position);                 // SetPosition (RET 0Ch)
void __fastcall kotor_fastcall_0063ba80(void* self, void*, void* settings);                     // SaveCameraSettings (RET 4)
void __fastcall kotor_fastcall_00637a30(void* self, void*, const char* message);                // Message (slot 0Ch, RET 4)
void __fastcall kotor_fastcall_00638070(void* self, void*, std::int32_t style);                 // SetCameraStyle (RET 4)
std::uint32_t __fastcall kotor_fastcall_0063bcb0(void* self, void*, float delta);               // Control (slot 08h, RET 4)
void __fastcall kotor_fastcall_00638f60(void* self, void*);                                     // Control_CheckAndUpdateCameraStyle
void __fastcall kotor_fastcall_00638fe0(void* self, void*);                                     // Control_SetCameraInintalState
W4CamQuaternion* __fastcall kotor_fastcall_006391a0(void* self, void*, W4CamQuaternion* out, W4CamQuaternion q, float delta); // RET 18h
W4Vector* __fastcall kotor_fastcall_00637af0(void* self, void*, W4Vector* out, W4Vector head);  // ComputeDesiredPosition (RET 10h)
double __fastcall kotor_fastcall_00637b80(void* self, void*, float cur, float lo, float hi, W4Vector a, W4Vector b, float delta); // RET 28h
W4Vector* __fastcall kotor_fastcall_006398f0(void* self, void*, W4Vector* out, W4Vector a, W4Vector b, W4Vector c, float delta); // RET 2Ch
W4Vector* __fastcall kotor_fastcall_0063b050(void* self, void*, W4Vector* out, W4Vector position); // Control_HitCheckCamera (RET 10h)
W4CamQuaternion* __fastcall kotor_fastcall_00639760(void* self, void*, W4CamQuaternion* out, float delta); // Control_SetCameraOrientation (RET 8)
W4Vector* __fastcall kotor_fastcall_005f5180(void* self, void*, W4Vector* out);                 // GetTargetHeadPosition (RET 4) -- runtime1115 0715 provider (reused)
void __fastcall kotor_fastcall_00637c20(void* self, void*, W4Vector position, W4CamQuaternion q);  // ResetLastPosition (RET 1Ch)
std::uint32_t __fastcall kotor_fastcall_00637cc0(void* self, void*, float delta);               // CameraFreeStyleControl (trap)
std::uint32_t __fastcall kotor_fastcall_00641540(void* module, void*);                          // CSWCModule::SetCombatCamera
std::uint32_t __fastcall kotor_fastcall_00641dc0(void* module, void*);                          // CSWCModule::RestoreCamera
// CAurBehavior::RemoveDependency (absent in runtime1115; W4-provided, see receipts).
void __fastcall kotor_fastcall_0049f1f0(void* behavior, void*, void* object);                   // RET 4
// Roll(Quaternion) (cdecl, quaternion by value; absent in runtime1115; W4-provided).
double __cdecl kotor_cdecl_004acb90(W4CamQuaternion q);
// CSWRK4Acceleration / CSWRK4SplitAcceleration.
void* __fastcall kotor_fastcall_006d0b20(void* self, void*);                                     // CSWRK4Acceleration ctor
void* __fastcall kotor_fastcall_006d10b0(void* self, void*);                                     // CSWRK4SplitAcceleration ctor
W4Vector* __fastcall kotor_fastcall_006d0fc0(void* self, void*, W4Vector* out, const W4Vector* pos, const W4Vector* vel); // GetAccelerationVector (RET 0Ch)
W4Vector* __fastcall kotor_fastcall_006d10d0(void* self, void*, W4Vector* out, const W4Vector* pos, const W4Vector* vel); // split override (RET 0Ch)
void __fastcall kotor_fastcall_006d0bd0(void* self, void*, const W4Vector* input, float delta);  // Accelerate (RET 8)
W4RK4State* __fastcall kotor_fastcall_006d1190(void* self, void*, W4RK4State* out, W4Vector input, float delta,
                                               std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t); // Update (RET 24h)
void __fastcall kotor_fastcall_006d1030(void* self, void*, float y);                             // SetVelocity (RET 4)
W4RK4State* __cdecl kotor_cdecl_006d0a40(W4RK4State* out, const W4RK4State* s, float d);         // operator/(state, float)
W4RK4State* __cdecl kotor_cdecl_00670830(W4RK4State* out, const W4RK4State* s, float k);         // operator*(state, float)
W4RK4State* __cdecl kotor_cdecl_006707a0(W4RK4State* out, const W4RK4State* a, const W4RK4State* b); // operator+
// existing runtime1115 providers used by the stick
void* __fastcall quaternion_from_axis_angle_004ac960(W4CamQuaternion* self, void*, W4Vector axis, float angle); // Quaternion(axis, angle)
double __cdecl quaternion_yaw_004a9f40(const W4CamQuaternion* q);
double __cdecl quaternion_pitch_004aa020(const W4CamQuaternion* q);
void __fastcall vector_normalize_004ab130(W4Vector* self, void*);                                  // Vector::Normalize
const char* __cdecl firstword_00463530(const char* text);
int __cdecl parse_float_004635c0(const char* text, float* out);
int __cdecl kotor_stricmp_0070acaf(const char* a, const char* b);
void __fastcall behavior_destruct_0049fa70(void* behavior, void*);                               // CAurBehavior::~CAurBehavior
void __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);              // CExoString(const char*)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                                // ~CExoString
void* __fastcall gui_m48_manager_store_mouse_0040a3d0(void* gui, void*);                          // StoreCurrentMousePosition
void* __fastcall gui_m48_manager_restore_mouse_0040c7d0(void* gui, void*);                        // RestoreOldMousePosition
void __fastcall kotor_fastcall_005ee170(void* app, void*, std::uint32_t flag);                    // CClientExoApp::HideMouse
void __fastcall kotor_fastcall_005ee180(void* app, void*, std::uint32_t flag);                    // CClientExoApp::ShowMouse
std::uint32_t __fastcall gui_m48_client_get_input_class_005eda70(void* app, void*);               // GetInputClass
void __cdecl genuine_x87_pow_006fc1f0();                                                          // CRT pow on the FPU stack
std::int32_t __cdecl crt_rand_006fc447();
}
namespace options_video { struct Table; struct String; }
namespace menu_tables_required {
int __fastcall C2daFloatEntry_00413430(options_video::Table*, void*, int row, options_video::String* column, float* out);
}
namespace w4cam {
extern float tilt_up_007a2420, tilt_down_007a2424, rotation_007a2428;   // CSWCameraOnAStick::SetCameraStyle outputs
extern void* const camera_stick_vtable_0075165c[8];
}
