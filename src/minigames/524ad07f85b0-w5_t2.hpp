#pragma once
// Lane W5 tier 2: the real PC keyboard-movement path (G16).
//   ProcessInput 006227E0 polls the axes and calls CSWPlayerControlCamRelative::UpDown / LeftRight /
//   SetPlayerWalking, then Control(dt) 00679940. Control integrates the drive velocity and calls
//   CSWCCreature::SetPrecalculatedPosition 00614B90, which moves the server creature in-process.
// Conventions are inherited from t1 (source/w5_types.hpp):
//   * thiscall entries are exported as extern "C" __fastcall(self, unused_edx, ...);
//   * x87: double intermediates, float exactly where the original stores a dword; ST0 returns are double;
//   * memory: live_operator_new / live_free only;
//   * an unimplemented path traps with the original address.
#include "4122183abfad-w5_types.hpp"
#include "45d56fc7dd30-w5_server_env.hpp"
#include "cb251177a109-w5_foreign.hpp"

namespace w5 {
// CSWMGPhysicsState (24 bytes): +0 first triple, +C second triple. In CSWPlayerControlCamRelative the
// first triple is the drive velocity and the second the integrated position (derivative = {accel, first}).
struct PhysicsState { Vector a, b; };
static_assert(sizeof(PhysicsState) == 24, "CSWMGPhysicsState is six floats");
struct Quaternion { float w, x, y, z; };   // kotor_scene3d layout (fields unused by W5)
// CWalkHitInfo (004B14B0 ctor, 0x48 bytes): +0 hit flag, +4 hit point, +10 normal, +1C..+3C scratch (+34/+38
// edge direction), +40 blocking object id (7F000000 = none), +44 blocker type (2 = wall edge).
struct WalkHit { U flag_0; Vector point_4; Vector normal_10; float scratch_1c[9]; U blocker_40; U type_44; };
static_assert(sizeof(WalkHit) == 0x48, "CWalkHitInfo is 0x48 bytes");
constexpr U k_player_control_size = 0xCC;
}  // namespace w5

extern "C" {
using w5::U; using w5::I; using w5::Vector; using w5::PhysicsState; using w5::Quaternion;

// ---------------- W5 t2 exports: CSWPlayerControl / CSWPlayerControlCamRelative ----------------
extern void* const w5_vtable_00752d04[12];                      // CSWPlayerControl vtable instance
extern void* const w5_vtable_00752d34[12];                      // CSWPlayerControlCamRelative vtable instance
void __fastcall w5_player_control_initialize_006792d0(void* self, void*);
void __fastcall kotor_fastcall_00679340(void* self, void*, U object_id);              // SetPlayer, RET 4
void __fastcall kotor_fastcall_006793b0(void* self, void*, void* camera);             // SetCamera, RET 4
void __fastcall w5_player_control_init_006793c0(void* self, void*);                   // Init, RET 0
double __fastcall w5_player_control_get_max_speed_00679510(void* self, void*);       // ST0
void __fastcall w5_player_control_reset_drive_acceleration_006795c0(void* self, void*);
void __fastcall w5_player_control_up_down_00679700(void* self, void*, float v);
double __fastcall w5_player_control_get_up_down_00679710(void* self, void*);
void __fastcall w5_player_control_left_right_00679720(void* self, void*, float v);
double __fastcall w5_player_control_get_left_right_00679730(void* self, void*);
void __fastcall w5_player_control_set_player_walking_00679740(void* self, void*, U walking);
double __fastcall w5_player_control_get_cur_speed_00679750(void* self, void*);
void* __fastcall kotor_fastcall_00679780(void* self, void*, U object_id, void* camera); // ctor, RET 8
Vector* __fastcall w5_player_control_get_acceleration_00679870(void* self, void*, Vector* out, const Vector* vel,
    const Vector* target);                                                            // RET 0Ch
I __fastcall w5_player_control_control_00679940(void* self, void*, float dt);         // RET 4
double __fastcall w5_player_control_return_zero_006796f0(void* self, void*);          // base-vtable slot (fld 0.0)
// CSWMGPhysicsState
PhysicsState* __fastcall w5_physics_state_construct_00670750(PhysicsState* self, void*, const Vector* a, const Vector* b); // RET 8
PhysicsState* __cdecl w5_physics_state_from_006709c0(PhysicsState* out, const Vector* b, const Vector* a);
PhysicsState* __cdecl w5_physics_state_divide_006708c0(PhysicsState* out, const PhysicsState* s, float d);
// data cells read only by GetMaxSpeed (no writer in the image; values are the image's initial values)
extern U w5_cell_max_speed_override_00833bac;   // .bss 0
extern float w5_cell_max_speed_override_007a25f8; // .data 15.0

// ---------------- W5 t2 exports: CSWCCreature members on the keyboard path ----------------

void __fastcall w5_client_creature_reset_camera_to_follow_0060b920(void* creature, void*);
void __fastcall w5_client_creature_set_sw_animation_speed_0060f0d0(void* creature, void*, float speed, U b); // RET 8
double __fastcall w5_client_creature_get_drive_acceleration_00610590(void* creature, void*);
void __fastcall w5_client_creature_compute_z_value_0060bf20(void* creature, void*, void* area, Vector* point,
    float z, U unused);                                                                // RET 10h
I __fastcall w5_client_creature_set_precalculated_position_00614b90(void* creature, void*, const Vector* target); // RET 4
void __fastcall w5_camera_stick_clear_target_006383b0(void* camera, void*);           // +80 = 7F000000
// server-side movement helpers
I __fastcall w5_client_area_test_direct_line_00604c40(void* client_area, void*, U id, const Vector* from,
    const Vector* to, const float* radius, float band, U unused, void* hit);          // RET 1Ch
I __fastcall w5_server_test_direct_line_004aee80(void* server, void*, U id, void* fallback_area, const Vector* from,
    const Vector* to, const float* radius, float band, U unused, void* hit);          // RET 20h
void* __fastcall w5_server_get_creature_pathfind_004aef10(void* server, void*, U client_id); // RET 4
void* __fastcall w5_walk_hit_construct_004b14b0(void* hit, void*);
double __fastcall w5_server_compute_height_004aea50(void* server, void*, void* unused, Vector* point, U flag); // RET 0Ch
void __fastcall w5_server_update_player_position_004aeaa0(void* server, void*, U client_id, const Vector* p,
    const Vector* orientation);                                                       // RET 0Ch
void __fastcall w5_server_internal_update_player_position_004b1cb0(void* internal, void*, U server_id,
    const Vector* p, const Vector* orientation);                                      // RET 0Ch
void __fastcall w5_server_update_subareas_on_move_to_004aead0(void* server, void*, U id, const Vector* from,
    const Vector* to);                                                                // jmp 004B6CC0, RET 0Ch
void __fastcall w5_server_internal_update_subareas_004b6cc0(void* internal, void*, U id, const Vector* from,
    const Vector* to);                                                                // RET 0Ch
I __fastcall w5_creature_bump_friends_004f6390(void* self, void*, void* other, const Vector* from,
    const Vector* to);                                                                // RET 0Ch
Vector* __cdecl w5_line_project_004a98b0(Vector* out, const Vector* a, const Vector* b, const Vector* p);
I __fastcall kotor_fastcall_005188e0(void* creature, void*, U type);         // RET 4
void __fastcall w5_party_follow_process_point_00636a30(void* follow, void*, const Vector* p, float yaw, U radius); // RET 0Ch
extern U w5_cell_debug_008338c0, w5_cell_debug_008338c4, w5_cell_debug_008338c8, w5_cell_debug_008338cc;
extern U w5_cell_follow_smooth_007a2418;                                              // .data 1 (never written)
extern Vector w5_cell_follow_last_point_00833928;                                     // ProcessPoint statics
extern float w5_cell_follow_last_yaw_00833924;

// ---------------- W5 t2 exports: input ----------------
double __fastcall kotor_fastcall_005df4d0(void* input, void*, U event, U input_class); // CExoInput::PollInput
double __fastcall w5_input_internal_poll_input_005e23c0(void* internal, void*, U event, U input_class);
double __fastcall w5_input_internal_scaled_value_005dfd80(void* internal, void*, void* event, U control, I raw); // RET 0Ch
double __fastcall w5_raw_input_get_max_useable_005e3a60(void* raw, void*, U device, U axis);              // RET 8
double __fastcall w5_raw_input_get_min_useable_005e3aa0(void* raw, void*, U device, U axis);              // RET 8
void __fastcall w5_process_input_drive_0062388a(void* client_internal, void*, float dt); // block 0062388A..00623F1F

// ---------------- imports that exist in the 1121 map (exact names) ----------------
void* __fastcall gui_m48_input_internal_get_event_005e2360(void* internal, void*, U event, I input_class); // 0698 (m48), RET 8
int __fastcall gui_m48_input_internal_is_event_in_class_005e0d80(void* internal, void*, U event, I input_class); // 0698 + W5 owner patch
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* object, void*);
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                       // GetClientOptions
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                       // CClientExoApp::GetModule
void* __fastcall server_get_module_004ae6b0(void* server, void*);
void* __fastcall module_get_area_004c30f0(void* module, void*);
void* __fastcall server_internal_get_module_004b14f0(void* internal, void*);
U __fastcall kotor_fastcall_004aea30(void* server, void*, U id);                     // ClientToServerObjectId
U __fastcall kotor_fastcall_004b1f70(void* internal, void*, U id);                   // internal ClientToServerObjectId
void* __fastcall kotor_fastcall_004b1790(void* internal, void*, U id);               // internal GetCreatureByGameObjectID
U __fastcall kotor_fastcall_005eda60(void* client, void*, U input_class, U show);    // SetInputClass
void __fastcall kotor_fastcall_0060f090(void* creature, void*, U mode);              // CSWCCreature::SwitchMode (0580)
void __fastcall kotor_fastcall_006792e0(void* control, void*, U enabled);            // SetEnabled (0580)
void* __fastcall quaternion_zdir_0043e250(const void* q, void*, Vector* out);
U __fastcall vector_is_zero_0046a620(const Vector* v, void*);
double __cdecl kotor_m15a_yaw_004aa0f0(const Vector* v);                              // ST0 degrees
double __cdecl kotor_cdecl_004ab100(const Vector* v);                                 // vector_magnitude, ST0
double __cdecl deg_to_rad_004a96d0(float degrees);
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion* self, void*, Vector axis, float angle); // RET 10h
Vector* __cdecl vector_cross_004ab210(Vector* out, const Vector* a, const Vector* b);
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor);
U __fastcall gob_get_property_0x68_0063c070(void* object, void*);
void* __fastcall c2da_default_ref_00405ed0(void* resref, void*);                      // CResRef::CResRef
U __fastcall c2da_current_get_float_00413430(void* table, void*, U row, void* column_name, float* out); // RET 0Ch
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);
U __fastcall recovered_party_member_00634620(void* party, void*, U id);              // CSWParty::IsPartyMember (W1)
U __fastcall recovered_creature_collision_flag_004f62a0(void* self, void*, void* other); // GetIsCreatureBumpable
extern void* kotor_cell_rules_007a3a28;                                               // [007A3A28] CSWRules*
// W5 t1 bodies used by t2
I __fastcall w5_area_test_direct_line_004bcb70(void* area, void*, const Vector* from, const Vector* to,
    const float* radius, float height_band, U unused5, void* walk_hit);
void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, void* direction);   // W1 0886 wrapper (ruling)

// ---------------- imports owned by other lanes, absent from the 1121 map (trap fallback via /alternatename) ----
// W3 t2
void __fastcall recovered_client_creature_set_moving_orientation_0060b380(void* creature, void*, const Vector* dir); // RET 4
// W4
U __fastcall kotor_fastcall_0060f0f0(void* creature, void*);                         // IsDriveAnimationBlocked
void* __fastcall kotor_fastcall_006d10b0(void* rk4, void*);                          // CSWRK4SplitAcceleration ctor
PhysicsState* __cdecl kotor_cdecl_00670830(PhysicsState* out, const PhysicsState* s, float k); // operator*
PhysicsState* __cdecl kotor_cdecl_006707a0(PhysicsState* out, const PhysicsState* a, const PhysicsState* b); // operator+
double __fastcall kotor_fastcall_0061d5e0(void* options, void*);                     // GetCameraKeyboardDPS
double __fastcall kotor_fastcall_0061d610(void* options, void*);                     // ...Acceleration
double __fastcall kotor_fastcall_0061d640(void* options, void*);                     // ...Deceleration
void* __fastcall kotor_fastcall_00634cc0(void* follow, void*, int index);            // GetPathData
int __fastcall kotor_fastcall_00634da0(void* follow, void*);                         // FindFurthestPartyMemeber_Current
double __fastcall kotor_fastcall_00634e00(void* follow, void*, int index, int squared); // PointOnLineDistanceFromLeader
void __fastcall kotor_fastcall_00641dc0(void* module, void*);                        // CSWCModule::RestoreCamera
void __fastcall kotor_fastcall_0063a360(void* camera, void*);                        // ClearCombatCamera
void __fastcall kotor_fastcall_006345e0(void* member, void*, U message);             // SetCombatMessage
void __fastcall kotor_fastcall_0063fdb0(void* module, void*, float tilt);            // AcclTiltCamera
// W1 / 0886 TU wrappers requested by W5 (w5_t2_server_mainloop_adapters.inc)
unsigned __fastcall w2x_area_safe_point_004be5e0(void* area, void*, void* position, unsigned pathfind); // W2 0886 binding, RET 8
U __fastcall w5x_creature_is_debilitated_005b4880(void* creature, void*);
// unowned / later W5 tiers: explicit traps (w5_t2_foreign_traps.cpp)
void __fastcall w5_t2_party_follow_smooth_path_00635af0(void* follow, void*);
U __fastcall w5_t2_area_no_nonwalk_polys_in_doors_0050ca10(void* area, void*, const Vector* a, const Vector* b,
    float lo, float hi, float radius, U object_id, U* out_id, U f8, U f9, U f10);   // RET 28h
U __fastcall w5_t2_area_compute_safe_location_in_direction_004c0fa0(void* area, void*, const Vector* p,
    const Vector* dir, float distance, void* pathfind, U flag, Vector* out);         // RET 18h
}
