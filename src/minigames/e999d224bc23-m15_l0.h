#pragma once
// lane-m15-0 (L15-0, Milestone 1.5): single-owner shared leaves for character creation.
// Consumers (L15-A/B/C/D/E/F) include this header (header root: lane-m15-0\src) or copy the
// declarations verbatim. Every symbol below is DEFINED by an L15-0 unit (see PACKET.json):
//   m15-l0-leaves  (m15_leaves.cpp)  containers, server leaves, C2DA string entry, party time
//   m15-l0-sound   (m15_sound.cpp)   3D one-shot sound, sound sources, CSoundSet/CResSSF family
//   m15-l0-client  (m15_client.cpp)  CClientExoApp getters/thunks + internals, load screen, input cool-down
//   m15-l0-gui     (m15_gui.cpp)     GUI base gaps, fade, load-screen panel, tutorial window path
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only.
//
// Conventions (DECISIONS D1/D21/D24): every original __thiscall entry is an extern "C"
// __fastcall(Self*, void* unused_edx, <original stack words>) veneer with identical callee
// cleanup; callers pass nullptr for EDX; no callee reads EDX. Object/struct pointers are void*
// (C linkage: the mangled name is @name@<bytes> regardless of pointee type). Structs returned by
// value in the original are returned through an explicit out-pointer first stack argument
// (D1-amendment a). Event-callback entries (PMFs) are in m15_l0_gui.h.
#include <cstdint>

extern "C" {
// ============================================================ m15-l0-leaves
// 004CC310 CSWSObject::AttachmentFromClientObject(id) RET 4: [this+224h] = id.
void __fastcall kotor_m15_sobject_attachment_from_client_004cc310(void* self, void*, std::uint32_t id);
// 004D2FA0 CExoArrayList<unsigned short>::Allocate(int new_size) RET 4.
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void* list, void*, std::int32_t new_size);
// 004D4330 CExoArrayList<unsigned short>::Add(unsigned short) RET 4 (word pushed; low 16 bits used).
void __fastcall kotor_m15_array_u16_add_004d4330(void* list, void*, std::uint32_t value);
// 005A7670 CExoArrayList<unsigned short>::AddUnique(unsigned short) RET 4.
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void* list, void*, std::uint32_t value);
// 006169A0 CExoArrayList<short>::Insert(short value, int index) RET 8.
void __fastcall kotor_m15_array_i16_insert_006169a0(void* list, void*, std::uint32_t value, std::int32_t index);
// 006F2A20 CExoArrayList<unsigned short>::IndexOf(unsigned short) RET 4 -> index or -1 (Ghidra FUN_006f2a20).
std::int32_t __fastcall kotor_m15_array_u16_index_of_006f2a20(void* list, void*, std::uint32_t value);
// 004EC370 CSWSCreature::GetMovementRateFactor() RET 0 -> float (st0), clamped to [0.125, 1.5].
float __fastcall kotor_m15_screature_get_movement_rate_factor_004ec370(void* self, void*);
// 00552280 CCombatInformation::CCombatInformation() RET 0, EAX=this.
void* __fastcall kotor_m15_combat_information_construct_00552280(void* self, void*) noexcept(false);
// 00563CF0 CSWPartyTable::ResetCurrentSessionStartTime() (static; RET 0; reads no ECX):
// [007A3A20..24] = GetSystemTimeAsFileTime().
void __cdecl kotor_m15_party_reset_session_start_time_00563cf0();
// 0063D7B0 CExoLinkedList<CSWCVisualEffectOnObject>::CExoLinkedList() RET 0, EAX=this.
void* __fastcall kotor_m15_vfx_list_construct_0063d7b0(void* self, void*) noexcept(false);
// 00413DE0 C2DA::GetCExoStringEntry(const CExoString& row, const CExoString& column, CExoString* out) RET 0Ch.
std::int32_t __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row_label, void* column,
                                                                 void* out) noexcept(false);

// ============================================================ m15-l0-sound
// CExoSoundSource (outer: +4 = CExoSoundSourceInternal*)
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, std::uint32_t is3d);        // RET 4
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void* source, void*, std::uint32_t looping); // RET 4
// 005D59E0 SetPosition(const Vector* position, float z_offset) RET 8 (z' = z + offset).
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* position, float z_offset);
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);                               // RET 0
// 005D5E10 CExoSound::Play3DOneShotSound(CResRef* ref, Vector position (3 words BY VALUE), float z_offset,
//   group, delay, volume (byte in a word), float fixed_variance, float pitch_variance) RET 28h (10 words);
// forwards (ref, {x, y, z + z_offset}, group, delay, volume, fixed, pitch) to 005D7680 (005D5E16..005D5E59).
struct KotorM15VectorWords { float x, y, z; };
std::uint32_t __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, void* ref,
    KotorM15VectorWords position, float z_offset, std::uint32_t group, std::uint32_t delay, std::uint32_t volume,
    float fixed_variance, float pitch_variance) noexcept(false);
// CExoStreamingSoundSource leaves (outer: +0 = internal)
void __fastcall kotor_m15_stream_source_set_3d_005d6290(void* source, void*, std::uint32_t is3d);               // RET 4
void __fastcall kotor_m15_stream_source_set_distance_005d62a0(void* source, void*, float min_d, float max_d);   // RET 8
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void* source, void*, const void* position);       // RET 4
// 005D7680 CExoSoundInternal::Play3DOneShotSound(CResRef* ref, Vector position (3 words by value),
//   group, delay, volume, fixed_variance, pitch_variance) RET 24h (9 words).
std::uint32_t __fastcall kotor_m15_sound_internal_play_3d_one_shot_005d7680(void* internal, void*, void* ref,
    KotorM15VectorWords position, std::uint32_t group, std::uint32_t delay, std::uint32_t volume,
    float fixed_variance, float pitch_variance) noexcept(false);
void __fastcall kotor_m15_sound_internal_add_delayed_one_shot_005d7bb0(void* internal, void*, void* source);    // RET 4
// CExoSoundSourceInternal
void __fastcall kotor_m15_source_internal_set_looping_005db130(void* self, void*, std::uint32_t looping);      // RET 4
void __fastcall kotor_m15_source_internal_shut_down_005db2b0(void* self, void*);                                // RET 0
void __fastcall kotor_m15_source_internal_set_position_005db2c0(void* self, void*, const void* position);      // RET 4
void __fastcall kotor_m15_source_internal_set_3d_005db320(void* self, void*, std::uint32_t is3d);              // RET 4
void __fastcall kotor_m15_source_internal_set_one_shot_delay_005db340(void* self, void*, std::uint32_t delay); // RET 4
std::uint32_t __fastcall kotor_m15_source_internal_set_fixed_variance_005db980(void* self, void*, float variance); // RET 4
// Sound sets
// 006788D0 CSoundSetList::GetSoundSet(index) RET 8: returns CResRef BY VALUE (hidden out pointer = first stack word).
void* __fastcall kotor_m15_sound_set_list_get_006788d0(void* list, void*, void* out_ref, std::uint32_t index);
// 006789A0 CResHelper<CResSSF,2060>::SetResRef(const CResRef* ref, int request) RET 8.
void __fastcall kotor_m15_ssf_helper_set_ref_006789a0(void* helper, void*, const void* ref, std::uint32_t request) noexcept(false);
void __fastcall kotor_m15_ssf_helper_destruct_00678940(void* helper, void*);                                     // RET 0
void* __fastcall kotor_m15_ssf_helper_deleting_destruct_00678b00(void* helper, void*, std::uint32_t flags);      // RET 4
// 00678B20 CSoundSet::CSoundSet(const CResRef* ref) RET 4, EAX=this (0x1C bytes: helper +0..+1B).
void* __fastcall kotor_m15_sound_set_construct_00678b20(void* self, void*, const void* ref) noexcept(false);
void __fastcall kotor_m15_sound_set_destruct_00678ba0(void* self, void*) noexcept(false);                                         // RET 0
void* __fastcall kotor_m15_sound_set_deleting_destruct_00678ea0(void* self, void*, std::uint32_t flags) noexcept(false);          // RET 4
void* __fastcall kotor_m15_res_ssf_construct_006db650(void* self, void*) noexcept(false);                         // RET 0
void __fastcall kotor_m15_res_ssf_destruct_006db670(void* self, void*);                                           // RET 0
std::uint32_t __fastcall kotor_m15_res_ssf_freed_006db680(void* self, void*);                                     // RET 0
std::uint32_t __fastcall kotor_m15_res_ssf_serviced_006db690(void* self, void*);                                  // RET 0
void* __fastcall kotor_m15_res_ssf_deleting_destruct_006db6b0(void* self, void*, std::uint32_t flags);            // RET 4

// ============================================================ m15-l0-client
// CClientExoApp (outer; +4 = CClientExoAppInternal*)
std::uint32_t __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, std::uint32_t id);                   // RET 4
void* __fastcall kotor_m15_client_get_sound_set_list_005ed910(void* app, void*);
std::int32_t __fastcall kotor_m15_client_is_player_creature_005eda30(void* app, void*, void* object);           // RET 4
// 005EDA80 ShowLoadScreen(a1 = start fade, a2 = start load music, a3 = alt strref) RET 0Ch.
void __fastcall kotor_m15_client_show_load_screen_005eda80(void* app, void*, std::uint32_t fade, std::uint32_t music,
                                                           std::uint32_t alt) noexcept(false);
// 005EDA90 HideLoadScreen(a1 = start fade, a2 = update screen) RET 8.
void __fastcall kotor_m15_client_hide_load_screen_005eda90(void* app, void*, std::uint32_t fade, std::uint32_t update) noexcept(false);
std::uint32_t __fastcall kotor_m15_client_get_area_not_ready_005edad0(void* app, void*);
// 005EDCF0 SetLoadScreenByModuleName(const CExoString& module) RET 4.
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(void* app, void*, void* module) noexcept(false);
// 005EDD40 SetLoadBarProgress(int progress, int update_hint) RET 8.
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* app, void*, std::int32_t progress, std::uint32_t update_hint) noexcept(false);
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(void* app, void*, std::uint32_t strref) noexcept(false); // RET 4
// 005EDF40 ShowTutorialWindow(reason, a2, a3, a4) RET 10h -> int. Name required by consumer
// m48/t/f-input/src/client_input_event.h:23.
int __fastcall show_tutorial_window_005edf40(void* app, void*, std::uint32_t reason, std::uint32_t a2, std::uint32_t a3,
                                             std::uint32_t a4) noexcept(false);
// 005DF4B0 CExoInput::CoolDownEvent(event, enable) RET 8. Name required by consumer client_input_event.h:47.
void __fastcall cool_down_event_005df4b0(void* input, void*, std::uint32_t event, std::uint32_t enable) noexcept(false);
// CClientExoAppInternal
void* __fastcall kotor_m15_client_internal_get_item_by_id_005f1130(void* internal, void*, std::uint32_t id);     // RET 4
void __fastcall kotor_m15_client_internal_stop_load_music_005f1b90(void* internal, void*);                       // RET 0
std::int32_t __fastcall kotor_m15_client_internal_is_player_creature_005f2b50(void* internal, void*, void* object); // RET 4
void __fastcall kotor_m15_client_internal_set_load_screen_by_module_005f3480(void* internal, void*, void* module) noexcept(false); // RET 4
void __fastcall kotor_m15_client_internal_set_load_screen_hint_005f37f0(void* internal, void*, std::uint32_t strref) noexcept(false); // RET 4
int __fastcall kotor_m15_client_internal_show_tutorial_window_005f4120(void* internal, void*, std::uint32_t reason,
    std::uint32_t a2, std::uint32_t a3, std::uint32_t a4) noexcept(false);                                       // RET 10h
void __fastcall kotor_m15_client_internal_show_load_screen_005f6c60(void* internal, void*, std::uint32_t fade,
    std::uint32_t music, std::uint32_t alt) noexcept(false);                                                    // RET 0Ch
void __fastcall kotor_m15_client_internal_hide_load_screen_005f6e20(void* internal, void*, std::uint32_t fade,
    std::uint32_t update) noexcept(false);                                                                      // RET 8
void __fastcall kotor_m15_client_internal_update_loading_screen_hint_005f7b20(void* internal, void*) noexcept(false); // RET 0
void __fastcall kotor_m15_client_internal_set_load_bar_progress_005fb3f0(void* internal, void*, std::int32_t progress,
    std::uint32_t update_hint) noexcept(false);                                                                 // RET 8
void __fastcall kotor_m15_input_internal_cool_down_event_005e0670(void* internal, void*, std::uint32_t event,
    std::uint32_t enable) noexcept(false);                                                                      // RET 8

// ============================================================ m15-l0-gui
void __fastcall kotor_m15_panel_set_background_0040a870(void* panel, void*, const void* ref) noexcept(false); // RET 4
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void* panel, void*, std::uint32_t visible) noexcept(false); // RET 4
void* __fastcall kotor_m15_gui_scene_get_model_00416750(void* scene, void*, std::int32_t index);           // RET 4
// 006244E0 CSWGuiFade::SetFade(int fade_in, word a2 (+64h), word a3 (+68h), const Vector* color) RET 10h.
void __fastcall kotor_m15_fade_set_fade_006244e0(void* fade, void*, std::uint32_t fade_in, std::uint32_t a2,
                                                 std::uint32_t a3, const void* color) noexcept(false);
// 0062ABF0 CGuiInGame::StartGlobalFade(same four words) RET 10h.
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void* in_game, void*, std::uint32_t fade_in, std::uint32_t a2,
                                                            std::uint32_t a3, const void* color) noexcept(false);
std::int32_t __fastcall kotor_m15_ingame_set_tutorial_reason_0062b0d0(void* in_game, void*, std::uint32_t reason) noexcept(false); // RET 4
std::int32_t __fastcall kotor_m15_ingame_tutorial_reason_will_show_0062f420(void* in_game, void*, std::uint32_t reason) noexcept(false); // RET 4
void __fastcall kotor_m15_load_screen_set_image_0067a6d0(void* screen, void*, const void* ref) noexcept(false);  // RET 4
void __fastcall kotor_m15_load_screen_set_hint_0067a9c0(void* screen, void*, std::uint32_t strref) noexcept(false); // RET 4
void __fastcall kotor_m15_load_screen_add_hint_to_buffer_0067aa80(void* screen, void*) noexcept(false);         // RET 0
std::int32_t __fastcall kotor_m15_tutorial_box_reason_is_valid_006aa1a0(void* box, void*, std::uint32_t reason) noexcept(false); // RET 4
std::int32_t __fastcall kotor_m15_tutorial_box_set_tutorial_reason_006aa900(void* box, void*, std::uint32_t reason) noexcept(false); // RET 4
}
