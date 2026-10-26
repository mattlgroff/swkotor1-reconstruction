#pragma once
// Lane W4 G13 (client creature AIUpdate / animation drive). Declarations only.
// Existing runtime1115 providers: exact map symbol in each comment (checked by tools/check_undefined.py).
#include "353df0bd8b-w4_frame.hpp"

struct W4Quaternion { float x, y, z, w; };
static_assert(sizeof(W4Quaternion) == 16, "Quaternion");

namespace kotor_scene3d { struct Vector { float x, y, z; }; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }   // ?up_007a19b0@kotor_world_los@@3UVector@kotor_scene3d@@A

extern "C" {
// ---- existing runtime1115 providers ----
void* __fastcall kotor_fastcall_005f7dc0(void* client, void*, std::uint32_t id);              // CClientExoAppInternal::GetActiveTimer
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, std::uint32_t* a, std::uint32_t* b); // CWorldTimer::GetWorldTime
unsigned __fastcall kotor_fastcall_005edc10(void* app, void*);                                // CClientExoApp::GetPausedByCombat
std::uint32_t __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*);  // GetPlayerCreatureId
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                      // CClientExoApp::GetPlayerCreature
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                   // CClientExoApp::GetSWParty
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* object, void*);          // CSWCObject::GetServerObject
std::uint32_t __fastcall recovered_creature_incapacitated_004ef890(void* screature, void*);  // CSWSCreature::GetDeadTemp
float __cdecl kotor_m15a_yaw_004aa0f0(const void* vector);                                   // Yaw (naked; result in ST0)
float __cdecl kotor_m15a_pitch_004aa130(const void* vector);                                 // Pitch (naked; result in ST0)
void* __fastcall gob_get_property_0x68_0063c070(void* object, void*);                        // GetProperty0x68 (anim base)
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, void* position, float a); // CExoSoundSource::SetPosition
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, int on);         // CExoSoundSource::Set3D
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);                   // CExoSoundSource::Stop
void __fastcall kotor_m15_source_internal_set_fixed_variance_005db980(void* internal, void*, float v); // CExoSoundSourceInternal::SetFixedVariance
void __fastcall kotor_m15a_creature_set_appearance_006134c0(void* creature, void*, void* app, std::uint32_t a, std::uint32_t b); // SetAppearance
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void* app, void*);           // GetClientAIMaster
void __fastcall kotor_m15e_client_ai_list_remove_object_00604810(void* list, void*, void* object); // CClientAIList::RemoveObject
unsigned char __fastcall recovered_object_array_remove_004d81a0(void* array, void*, std::uint32_t id, void* out); // CGameObjectArray::Delete
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void* out);   // CGameObjectArray::GetGameObject
std::uint32_t __fastcall kotor_m15e_creature_get_pause_ready_animation_0060f020(void* creature, void*); // GetPauseReadyAnimation
std::uint32_t __fastcall kotor_m15a_object_animation_stationary_0063c170(void* object, void*, std::uint32_t anim);
std::uint32_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void* object, void*);
std::uint32_t __fastcall kotor_m15a_object_animation_looping_0063c300(void* object, void*, std::uint32_t anim);
std::uint32_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void* object, void*);
void __fastcall kotor_m15e_object_test_and_reequip_weapons_0063d850(void* object, void*, std::uint32_t old_anim, std::uint32_t new_anim);
std::uint32_t __fastcall kotor_m15e_object_animation_hide_equipped_items_0063c5b0(void* object, void*, std::uint32_t anim);
void __fastcall kotor_m15a_object_abort_fire_forget_animation_0063cad0(void* object, void*, std::uint32_t a);
void* __fastcall kotor_fastcall_00405ed0(void* resref, void*);                               // CResRef::CResRef
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, int index);                // CSWParty::GetCharacter
W4Quaternion* __cdecl quaternion_slerp_004a9e00(W4Quaternion* out, W4Quaternion from, const W4Quaternion* to, float t);
W4Quaternion* __fastcall quaternion_from_axis_angle_004ac960(W4Quaternion* self, void*, W4Vector axis, float angle);
W4Quaternion* __cdecl yaw_pitch_roll_004acac0(W4Quaternion* out, float yaw, float pitch, float roll);
W4Quaternion* __cdecl quaternion_multiply_004a9ac0(W4Quaternion* out, const W4Quaternion* a, const W4Quaternion* b);
W4Vector* __cdecl quaternion_rotate_004a9bb0(W4Vector* out, const W4Quaternion* q, const W4Vector* v);
W4Vector* __cdecl vector_cross_004ab210(W4Vector* out, const W4Vector* a, const W4Vector* b);
W4Vector* __cdecl vector_normalized_004ab180(W4Vector* out, const W4Vector* v);
int __cdecl crt_rand_006fc447();                                                            // _rand
int __fastcall c2da_current_get_int_entry_00413510(void* table, void*, int row, int column, int* out); // C2DA::GetINTEntry
extern std::uint32_t kotor_rules_cell32_007a214c, kotor_rules_cell32_007a2150, kotor_rules_cell32_007a2154,
    kotor_rules_cell32_007a215c, kotor_rules_cell32_007a2170, kotor_rules_cell32_007a217c,
    kotor_rules_cell32_007a218c, kotor_rules_cell32_007a2190, kotor_rules_cell32_007a2194; // 2DA column indices
extern void* kotor_cell_rules_007a3a28;                                                      // [007A3A28] CSWRules*
void __fastcall kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(void* visual, void*);
void __fastcall kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(void* visual, void*);
int __fastcall recovered_event_time_compare_004adfa0(void* timer, void*, std::uint32_t d1, std::uint32_t t1, std::uint32_t d2, std::uint32_t t2); // CWorldTimer::CompareWorldTimes
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);    // CSWCCreature::GetServerCreature
std::uint32_t __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, std::uint32_t slot);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, std::uint32_t id);

// ---- cross-lane pending providers (W1/W5 server side, W4 camera fork); see integration/creature.json ----
std::uint32_t __fastcall recovered_world_time_difference_004ae460(void* timer, void*, std::uint32_t d1, std::uint32_t t1,
    std::uint32_t d2, std::uint32_t t2, std::uint32_t* out_day, std::uint32_t* out_tick);     // CWorldTimer::SubtractWorldTimes (W1 donor)
std::int32_t __fastcall recovered_party_index_006349c0(void* party, void*, std::uint32_t id);       // CSWParty::GetIndex: W1 export @recovered_party_index_006349c0@12 (lane-w1/tiers/tier3/exports.txt)
std::uint32_t __fastcall recovered_creature_following_leader_004f7750(void* screature, void*);                    // CSWSCreature::GetCurrentlyFollowingLeader
W4Vector* __fastcall kotor_fastcall_004aeb70(void* server, void*, W4Vector* out, const W4Vector* point); // CServerExoApp::GetNormalOfPoint
std::uint32_t __fastcall kotor_fastcall_004aeae0(void* server, void*, std::uint32_t id);     // CServerExoApp::IsActionsPending
std::uint32_t __fastcall recovered_server_is_injured_004aee40(void* server, void*, std::uint32_t id);     // CServerExoApp::IsInjured
std::uint32_t __fastcall recovered_creature_is_injured_004eff30(void* screature, void*);                    // CSWSCreature::IsInjured
std::uint32_t __fastcall kotor_fastcall_005b4880(void* screature, void*);                    // CSWSCreature::IsDebilitated
std::uint32_t __fastcall kotor_fastcall_005188e0(void* screature, void*, std::uint32_t a);   // CSWSCreature::HasAction
std::int32_t __fastcall recovered_object_get_reputation_0057cb80(void* sobject, void*, std::uint32_t id, std::uint32_t a); // CSWSObject::GetReputation
void __fastcall w1x_object_set_orientation_004cd170(void* sobject, void*, W4Vector* orientation);         // CSWSObject::SetOrientation
void __fastcall kotor_fastcall_0063f9d0(void* module, void*, std::uint32_t id, std::uint32_t a); // CSWCModule::SetCameraFollow (camera fork)
std::uint32_t __fastcall kotor_fastcall_0062dbd0(void* in_game, void*, std::uint32_t id);    // CGuiInGame::IsInDialog (HUD fork)

// ---- G13 definitions (creature fork) ----
void __fastcall kotor_fastcall_0061ad60(void* creature, void*);                              // CSWCCreature::AIUpdate
void __fastcall kotor_fastcall_00615960(void* creature, void*);                              // DriveAnimationUpdate
void __fastcall kotor_fastcall_0061a980(void* creature, void*);                              // AnimationUpdate
void __fastcall kotor_fastcall_00615e80(void* creature, void*);                              // FollowAnimationUpdate
void __fastcall kotor_fastcall_0061a670(void* creature, void*);                              // AnimateFireAndForget
void __fastcall kotor_fastcall_00611850(void* creature, void*, W4Vector* orientation);       // RotateCreatureToOrientation
std::uint32_t __fastcall kotor_fastcall_0063c380(void* object, void*, std::uint32_t anim);  // CSWCObject::AnimationFireAndForget
void __fastcall kotor_fastcall_0060bdf0(void* creature, void*, W4Vector* target);            // MoveCreatureToPosition
void __fastcall kotor_fastcall_00610840(void* creature, void*);                              // UpdateRollingFootstepSound
std::uint32_t __fastcall kotor_fastcall_0063df60(void* object, void*);                      // CSWCObject::FadeObject
void __fastcall kotor_fastcall_00610a70(void* creature, void*, std::uint32_t anim);          // PlayDrawAnimation
std::uint32_t __fastcall kotor_fastcall_0060f0f0(void* creature, void*);                    // IsDriveAnimationBlocked
void* __fastcall kotor_fastcall_005ed660(void* app, void*, std::uint32_t id);               // CClientExoApp::GetActiveTimer
std::uint32_t __fastcall kotor_fastcall_00604bc0(void* area, void*, std::uint32_t id);      // CSWCArea::GetIsObjectCorpse
void __fastcall kotor_fastcall_0069de90(void* anim_base, void*, W4Vector* normal, W4Vector* orientation); // SetGroundTilt
void __fastcall kotor_fastcall_0060bb20(void* creature, void*);                              // HandleConjuringCreature
std::uint32_t __fastcall kotor_fastcall_00611700(void* creature, void*, std::uint32_t a, std::uint32_t b); // ComputeWeaponOverlays
void __fastcall kotor_fastcall_00614a30(void* creature, void*);                              // ForceAppearanceUpdate
void __fastcall kotor_fastcall_0060e6c0(void* creature, void*);                              // ReAttachCamera
void __fastcall kotor_fastcall_0063d270(void* object, void*, std::uint32_t on, std::uint32_t strength); // SetWind
void* __fastcall kotor_fastcall_005ed570(void* app, void*);                                 // GetGameObjectArray
unsigned __fastcall kotor_fastcall_005d5930(void* source, void*);                            // CExoSoundSource::Play (EAX = ms)
std::uint8_t __fastcall kotor_fastcall_005ed9f0(void* app, void*);                          // CClientExoApp::GetActivePauseState
std::uint32_t __fastcall kotor_fastcall_0062d700(void* in_game, void*, std::uint32_t anim); // IsFireForgetDialogAnimation
std::uint32_t __fastcall kotor_fastcall_0062d5e0(void* in_game, void*, std::uint32_t anim); // IsLoopingDialogAnimation
std::uint32_t __fastcall kotor_fastcall_0063c4a0(void* object, void*, std::uint32_t anim);  // AnimationDialog
std::uint32_t __fastcall kotor_fastcall_0063c1e0(void* object, void*, std::uint32_t anim);  // AnimationPause
std::uint32_t __fastcall kotor_fastcall_0063c410(void* object, void*, std::uint32_t anim);  // AnimationOverlay
std::uint32_t __fastcall kotor_fastcall_0063c550(void* object, void*, std::uint32_t anim);  // AnimationAttack
std::uint32_t __fastcall kotor_fastcall_0063c470(void* object, void*, std::uint32_t anim);  // AnimationPlayOutOfPlace
std::uint32_t __fastcall kotor_fastcall_0063c610(void* object, void*, std::uint16_t* anim, float* speed, std::uint32_t* time, std::uint32_t* data); // GetTopFireForgetAnimation
void __fastcall kotor_fastcall_0063c9c0(void* object, void*);                                // RemoveTopFireForgetAnimation
std::uint32_t __fastcall kotor_fastcall_00614b60(void* creature, void*);                    // CSWCCreature::IsInjured
void __fastcall kotor_fastcall_00614630(void* creature, void*, void* ref_a, void* ref_b);   // HideEquippedItems
void __fastcall kotor_fastcall_0060ba10(void* creature, void*);                              // SetDesiredOrientationForLock
void __fastcall kotor_fastcall_0060b3c0(void* creature, void*, W4Vector* direction);         // setStationaryOrentation
void __fastcall kotor_fastcall_006107c0(void* creature, void*, std::uint32_t run);           // PlayRollingFootstepSound
void __fastcall kotor_fastcall_00695e10(void* visual, void*, std::uint32_t delta);           // CSWCSpellCastVisual::UpdateTimer
std::uint32_t __fastcall kotor_fastcall_004ae570(void* timer, void*, std::uint32_t day, std::uint32_t tick); // GetTimeDifferenceFromWorldTime
void __fastcall kotor_fastcall_005d5a30(void* source, void*, float variance);                // CExoSoundSource::SetFixedVariance
std::uint32_t __fastcall kotor_fastcall_005e9bd0(void* list, void*, void* position);         // CExoLinkedListInternal::GetPrev
float __fastcall kotor_fastcall_0063c0e0(void* object, void*, std::uint32_t anim);          // GetAnimationLength (naked; ST0)
float __fastcall kotor_fastcall_0063dee0(void* object, void*);                              // GetFinalFadeAlpha (naked-equivalent; ST0)
float __fastcall kotor_fastcall_0063de50(void* object, void*);                              // GetVisualEffectAlpha
extern volatile std::uint32_t kotor_cell_007a2484;                                          // [007A2484] (W4 main owner)
// W4-creature-owned .data cells that the original never writes (constants in writable .data). Other lanes bind to these.
extern float kotor_cell_drive_run_ratio_007a22f4;   // 0.6f  (also read by 0051C610, 00615F93)
extern float kotor_cell_wind_strength_007a231c;     // 2.5f
extern float kotor_cell_turn_rate_007a25f4;         // 900.0f (also read by 0067A5E9)
// trap stubs (creature_traps.cpp)
void __fastcall kotor_fastcall_0063c760(void* object, void*, std::uint32_t anim, std::uint32_t duration, float speed, std::uint32_t data, std::uint32_t delay); // InsertFireForgetAnimation
std::uint32_t __fastcall kotor_fastcall_004adf50(void* timer, void*, std::uint32_t d1, std::uint32_t t1, std::uint32_t d2, std::uint32_t t2, std::uint32_t* out_day, std::uint32_t* out_tick); // CWorldTimer::AddWorldTimes
void* __fastcall kotor_fastcall_0063bfd0(void* entry, void*);                               // fire-and-forget entry ctor (unlabelled FUN_0063bfd0)
void __stdcall vector_constructor_iterator_00401130(void* array, std::uint32_t size, std::uint32_t count, void* ctor); // `vector constructor iterator` (map)
void __fastcall kotor_fastcall_0061a2d0(void* creature, void*);                              // PlayFootstep
std::uint32_t __fastcall kotor_fastcall_00617470(void* creature, void*, void* data, std::uint32_t a); // PlayHitSound, original EAX0/1
void* __fastcall kotor_fastcall_0060b6b0(void* self, void*, void* data);                    // CSWCCombatAttackData ctor
std::uint32_t __fastcall kotor_fastcall_0060ef50(void* creature, void*, std::uint32_t in_party); // SetInParty
std::uint8_t __fastcall kotor_fastcall_006a0920(void* effect, void*);                       // GetHardCodedEffectClass
}

namespace w4c {
// Calls whose original keeps the callee's ST0 result unrounded (D140): executed with inline asm and the x87
// result stored to double (exact under PC53).
double yaw_st0(const void* vector);
double pitch_st0(const void* vector);
double anim_length_st0(void* object, std::uint32_t anim);   // CSWCObject::GetAnimationLength (0063C0E0), ST0 result
}
