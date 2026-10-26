// Lane W5 queued-action bodies (worker w5-object-actions): CSWSObject::AIAction* bodies dispatched by the W1 action
// runner 0057F4A0, plus the callees they need that have no implementation in the candidate (c03-gui map).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are the original instructions.
// Body ABI (every action): original int __thiscall Body(CSWSObjectActionNode* node), RET 4, EAX = disposition
// ([00745DBC] = 1 in progress, [00745DC0] = 2 complete, [00745DC4] = 3 failed), exported __fastcall @12.
// Conventions as W5 t1..t3: thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// EH: the original bodies that own CExoString locals register an SEH/C++ unwind frame; its handlers only run when
// a callee throws (none of the bound callees throws in normal operation). The unwind cleanup is not reproduced.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include <cmath>

using namespace w5;

namespace w2 { std::uint32_t ftol2_006fae8c(double value); }     // __ftol2 006FAE8C (w2_ftol_native: x87 provider)
namespace kotor_list { struct Internal; }
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); } // CServerExoApp::GetPartyTable
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal* list, void*); }
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; }
namespace kotor_sound_rev3 {
void* __fastcall InternalConstruct005daf50(void* self, void*, void* system);          // CExoSoundSourceInternal ctor
void __fastcall InternalSetPriorityGroup005dbb20(void* self, void*, U group);
void __fastcall InternalSetPitchVariance005dba40(void* self, void*, float variance);
}
namespace kotor_sound_rev3_required { void __fastcall res_helper_set_ref_005d5e90(void* self, void*, const void* ref, U flag); }
namespace kotor_sound_path_required {
void __fastcall source_set_volume_005db800(void* self, void*, U volume, U a, U b);
U __fastcall source_play_005db4d0(void* self, void*, U a, U b);                       // RET 8, EAX = [self+58h] (ms)
}

extern "C" {
// ---------------- imports: existing providers in the c03-gui candidate (exact names) ----------------
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                     // GetWorldTimer
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);                  // CWorldTimer::GetWorldTime
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day1, U time1, U day2, U time2,
                                                      U* out_day, U* out_time);                     // SubtractWorldTimes
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                         // GetPlayerCreatureId
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                       // GetSWSMessage
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                       // GetPlayerList
void* __fastcall recovered_object_array_004aed70(void* server, void*);                             // GetObjectArray
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                     // CSWSPlayer::GetGameObject
void* __fastcall recovered_object_render_004cc2b0(void* object, void*);                            // CSWSObject::GetClientObject
double __fastcall kotor_fastcall_0063c0e0(void* client_object, void*, U animation);                // GetAnimationLength (ST0)
U __fastcall kotor_fastcall_0063c380(void* client_object, void*, U animation);                     // AnimationFireAndForget
void* __fastcall c2da_005e9d70(void* list, void*, void* item);                                     // CExoLinkedListInternal::AddHead
void __fastcall recovered_action_node_destroy_004cc4d0(void* node, void*);                         // ~CSWSObjectActionNode
void* __fastcall async_string_cstr_005e5670(void* string, void*);                                  // CExoString::CStr
void* __fastcall assign_text_005e5140(void* string, void*, const char* text);                      // CExoString::operator=(char const*)
U __fastcall async_string_length_005e5790(void* string, void*);                                    // CExoString::GetLength
void* __fastcall r8_copy_construct_005e5b00(void* string, void*, const void* other);               // CExoString::CExoString(CExoString const&)
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* out_strref_record, U flag);      // CTlkTable::Fetch
U __fastcall kotor_fastcall_005eda50(void* client, void*, U id);                                   // ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                               // CClientExoApp::GetGameObject
void __fastcall object_projectile_bark_0062ab30(void* gui, void*, U id, void* text, void* sound, U flag); // CGuiInGame::ShowBarkBubble
U __fastcall c2da_equal_text_004060d0(void* ref, void*, const char* text);                        // CResRef::operator==(char const*)
U __fastcall kotor_m15_source_internal_set_fixed_variance_005db980(void* self, void*, float variance);
void __fastcall kotor_m15_source_internal_set_one_shot_delay_005db340(void* self, void*, U delay);
// CSWMessage / CSWSMessage writers
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);                  // CreateWriteMessage
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                               // WriteOBJECTIDServer
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);                    // WriteDWORD
void __fastcall kotor_fastcall_004d6960(void* message, void*, void* string, U bits);               // WriteCExoString
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);                  // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
struct W5ExoStringArg { void* text; U length; };                                                    // CExoString by value
U __fastcall recovered_send_chat_00571960(void* message, void*, U mode, U speaker, W5ExoStringArg text, U target,
                                          void* extra);                                             // SendServerToPlayerChatMessage (W2)
U __fastcall recovered_creature_acquire_item_005158e0(void* creature, void*, void* item_cell, U container, U owner, U flag); // W6 t4
U __fastcall recovered_placeable_acquire_item_00584b10(void* placeable, void*, void* item_cell, U container, U flag);  // W6 t4
void* __fastcall frame_R4_005ed690(void* client, void*);                                           // GetInGameGui
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, U kind, U value);                        // CGuiInGame::UpdateStatus
void __fastcall recovered_creature_cancel_action_004ef4d0(void* creature, void*);                // CSWSCreature::StopGuiTimingBar
void* __fastcall recovered_message_data_construct_004d69f0(void* self, void*);                   // CSWCCMessageData ctor
void __fastcall recovered_message_data_destroy_004d6a40(void* self, void*);                      // ~CSWCCMessageData
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // SetInteger
U __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, U skill, void* versus, U base_only); // GetSkillRank
I __cdecl crt_rand_006fc447();                                                                    // rand
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U minor, void* data, U flag); // SendServerToPlayerCCMessage
U __fastcall recovered_action_cancel_message_0056b380(void* message, void*, void* player, U a, U b, U c); // SendServerToPlayerGuiTimingEvent
void __fastcall kotor_fastcall_004ef480(void* creature, void*, U milliseconds, U kind);
void __fastcall kotor_fastcall_004ec700(void* creature, void*, void* data);
U __fastcall recovered_object_get_reputation_0057cb80(void* object, void*, U other, U flag);    // CSWSObject::GetReputation (W1)
void __fastcall w5_door_open_door_00589c70(void* door, void*, U opener);                         // CSWSDoor::OpenDoor (W5 t2)
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, U index);           // CSWSItem::GetActiveProperty
U __fastcall recovered_party_table_leader_00563a00(void* party, void*, U id);                    // CSWPartyTable::GetIsLeader
U __fastcall recovered_item_in_repository_00555f40(void* repository, void*, void* item, U flag); // GetItemInRepository
U __fastcall recovered_creature_remove_item_00510ef0(void* creature, void*, void* item, U a, U b, U c); // CSWSCreature::RemoveItem (W2)
void __fastcall recovered_item_update_notify_00569fc0(void* message, void*, void* player, U item_id); // SendServerPlayerItemUpdate_DestroyItem
void __fastcall kotor_fastcall_00569ec0(void* message, void*, void* player, U speaker, U voice);  // SendServerToPlayerVoiceChat_Play
void __fastcall w5_creature_broadcast_voice_chat_004f2520(void* creature, void*, U voice);
U __fastcall w5_item_has_upgrade_004eada0(void* item, void*, U upgrade);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U id);   // CSWBaseItemArray::GetBaseItem
U __fastcall c2da_current_get_string_00413270(void* table, void*, U row, void* column, void* out); // C2DA::GetCExoStringEntry
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const void* string);     // CResRef::CResRef(CExoString const&)
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, void* ref, Vector position, float z_offset,
    U group, U delay, U volume, float fixed_variance, float pitch_variance);                       // CExoSound::Play3DOneShotSound
U __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);               // CItemRepository::RemoveItem (W2)
U __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void* item_cell, U a, U b, U c); // CItemRepository::AddItem
void __fastcall w5_message_send_gui_container_open_00569f40(void* message, void*, void* player, U object_id, U has_items);
void __fastcall w5_placeable_open_inventory_00587420(void* placeable, void*, U opener, U animate);
void* __fastcall w5_placeable_get_client_placeable_00585490(void* placeable, void*);
void __fastcall w5_item_open_inventory_005561a0(void* item, void*, U opener);
void __fastcall kotor_fastcall_0055d800(void* item, void*, U closer, U move_items);
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                     // CSWItem::GetBaseItem
void* __fastcall recovered_item_construct_005530a0(void* self, void*, U id);                      // CSWSItem::CSWSItem
void* __fastcall recovered_localized_assign_005e9e70(void* self, void*, const void* other);        // CExoLocString::operator=
void __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);     // AddObjectToLookupTable
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c);         // CSWSItem::SetPossessor
void __fastcall recovered_item_remove_area_005538a0(void* item, void*);                           // CSWSItem::RemoveFromArea
void __fastcall kotor_fastcall_004f3ed0(void* creature, void*, U amount, U feedback);
U __fastcall w5_creature_remove_pazaak_card_004efc30(void* creature, void*, void* item);
void* __fastcall kotor_fastcall_0055f280(void* item, void*, U count);
U __fastcall kotor_fastcall_0055d950(void* item, void*, void* source);
U __fastcall w5_item_acquire_item_0055dca0(void* item, void*, void* item_cell, U container, U flag);
U __fastcall recovered_placeable_remove_item_00584ac0(void* placeable, void*, void* item);      // CSWSPlaceable::RemoveItem (W2)
extern void* kotor_cell_tlk_007a3a08;     // [007A3A08] CTlkTable*
extern void* kotor_cell_sound_007a39ec;   // [007A39EC] CExoSound*

// ---------------- reconstructed here (no provider in the candidate) ----------------
U __fastcall w5_message_send_chat_str_ref_0056d670(void* message, void*, U player, U speaker, U minor, U strref);
U __fastcall w5_message_send_chat_str_ref_message_00571760(void* message, void*, U mode, U speaker, U strref);
U __fastcall w5_message_send_ai_action_play_sound_0056d4a0(void* message, void*, U player, U speaker, void* sound);
void __fastcall kotor_fastcall_005edbb0(void* client, void*, U id, void* text, void* sound, U flag);
void __fastcall kotor_fastcall_005f2c90(void* internal, void*, U id, void* text, void* sound, U flag);
U __fastcall w5_sound_play_one_shot_005d5e00(void* sound, void*, void* ref, U group, U delay, U volume,
                                             float fixed_variance, float pitch_variance);
U __fastcall w5_sound_internal_play_one_shot_005d7550(void* internal, void*, void* ref, U group, U delay, U volume,
                                                      float fixed_variance, float pitch_variance);
}

namespace {
constexpr U k_in_progress_00745dbc = 1;   // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;      // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;        // .rdata 00745DC4
constexpr U k_all_players_0074c52c = 0xfffffff7u;   // .rdata 0074C52C
constexpr U k_one_0x3f800000 = 0x3f800000u;         // 1.0f immediate
const float k_ms_per_second_0073d6fc = 1000.0f;     // .rdata 0073D6FC
const double k_long_animation_00749410 = 30.0;      // .rdata 00749410 (qword)
const float k_zero_0073d700 = 0.0f;                 // .rdata 0073D700
const float k_sound_range_00747444 = 1000.0f;       // .rdata 00747444
const float k_whisper_range_00747448 = 3.0f;        // .rdata 00747448
const char k_empty_0073d71c[] = "";                 // .rdata 0073D71C
const char k_gui_lockpick_00749400[] = "gui_lockpick";     // .rdata 00749400
const char k_gui_minedisarm_007493f0[] = "gui_minedisarm"; // .rdata 007493F0
const char k_gui_minearm_007493e4[] = "gui_minearm";       // .rdata 007493E4
constexpr U8 k_tlk_flag_0073ecd0 = 0;               // .rdata 0073ECD0 (byte)
constexpr U k_ai_event_close_door_007441cc = 6;   // .rdata 007441CC
constexpr U k_half_0x3f000000 = 0x3f000000u;      // 0.5f immediate
constexpr U k_ai_event_locked_007441e8 = 13;       // .rdata 007441E8
constexpr U k_ai_event_script_007441dc = 10;       // .rdata 007441DC
constexpr U8 k_type_door_00746639 = 10;            // .rdata 00746639 (byte)
constexpr U8 k_type_placeable_00746638 = 9;        // .rdata 00746638 (byte)
constexpr U k_one_and_half_0x3fc00000 = 0x3fc00000u; // 1.5f immediate
constexpr U k_ai_event_unlocked_007441e4 = 12;     // .rdata 007441E4
constexpr U k_ai_event_item_destroyed_007441e0 = 11; // .rdata 007441E0
const float k_voice_range_sq_007455bc = 900.0f;     // .rdata 007455BC
constexpr U8 k_type_item_00746635 = 6;             // .rdata 00746635 (byte)
const float k_ms_to_s_0073d708 = 0.001f;            // .rdata 0073D708
const float k_two_pow_32_0073d770 = 4294967296.0f;  // .rdata 0073D770
const char k_sound_app_type_00749420[] = "SoundAppType"; // .rdata 00749420
const char k_opened_00749418[] = "Opened";          // .rdata 00749418
const float k_give_walk_sq_00745e5c = 25.0f;       // .rdata 00745E5C
constexpr U8 k_found_0074666c = 0;                  // .rdata 0074666C (byte): CGameObjectArray status "found"

inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }   // vtbl+30 AsSWSCreature
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }     // vtbl+10 AsSWSObject
inline void* base_item(U id) {                            // [[007A3A28]+34] CSWBaseItemArray::GetBaseItem
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr, id);
}
inline void* new_script_event(U16 type) {                  // operator new(34h) + CScriptEvent ctor; type word at +0
    void* const memory = live_operator_new(0x34);
    void* const event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;                                 // written without a null check, as the original
    return event;
}
inline void* new_message_data() {                          // operator new(34h) + CSWCCMessageData ctor
    void* const memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
inline void stop_timing_bar(void* self, U slot_984_value, U slot) { // StopGuiTimingBar + creature+slot = value
    recovered_creature_cancel_action_004ef4d0(as_creature(self), nullptr);
    at<U>(as_creature(self), slot) = slot_984_value;
}
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }       // vtbl+18 AsSWSDoor
inline void* as_item(void* o) { return vcall<void*>(o, 0x38); }       // vtbl+38 AsSWSItem
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }  // vtbl+48 AsSWSPlaceable
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
// CServerAIMaster::AddEventDeltaTime(0, 0, caller, target, type, event) on the server AI master (inline view of 004AED80)
inline void ai_event(U caller, U target, U type, void* event) {
    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, caller, target, type, event);
}
inline void* timer() { return kotor_fastcall_004aede0(server_app(), nullptr); }
inline unsigned char* bp(void* p) { return static_cast<unsigned char*>(p); }

}  // namespace

extern "C" {

// =========================================================================================================
// 0057B5B0 CSWSObject::AIActionWait(node): node +38 = seconds (float); start time at self +BC/+C0.
U __fastcall w5_action_wait_0057b5b0(void* self, void*, void* node) {
    U dead = 0;                                                                     // EDI
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057B5BB..D0
    if (vcall<U>(self, 0x94) == 1) return k_failed_00745dc4;                       // 0057B5D2..DF
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_failed_00745dc4; // 0057B5E5..EF
    if (dead != 0) return k_failed_00745dc4;                                        // 0057B5F5..F7 (test, not cmp 1)
    const float seconds = at<float>(node, 0x38);                                    // 0057B5FD..60C (stored over the arg slot)
    U day = 0, time = 0;                                                            // [esp+C], [esp+8] (written by the callee)
    frame_GetWorldTime_004ade40(timer(), nullptr, &day, &time);                     // 0057B604..621
    U out_day = 0, out_time = 0;                                                    // [esp+14], [esp+10]
    recovered_world_time_difference_004ae460(timer(), nullptr, day, time, at<U>(self, 0xbc), at<U>(self, 0xc0),
                                             &out_day, &out_time);                  // 0057B626..658
    const U limit = w2::ftol2_006fae8c(static_cast<double>(seconds) * k_ms_per_second_0073d6fc); // 0057B65D..667
    if (out_time < limit) return k_in_progress_00745dbc;                            // 0057B66C..67C (unsigned jae)
    return k_complete_00745dc0;                                                     // 0057B67F
}

// =========================================================================================================
// 0057B290 CSWSObject::AIActionPauseDialog(node): +50 = 1; a creature also keeps activity bit 4 (+A00 |= 4).
U __fastcall w5_action_pause_dialog_0057b290(void* self, void*, void*) {
    U dead = 0;
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057B298..AD
    if (vcall<U>(self, 0x94) == 1 || kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1 || dead == 1) { // 0057B2AF..CD
        vcall<void>(self, 0xc0, k_object_invalid);                                  // 0057B2F9..303 ([00746644])
        return k_failed_00745dc4;                                                   // 0057B309
    }
    at<U>(self, 0x50) = 1;                                                          // 0057B2D3
    if (as_creature(self) != nullptr) at<U>(as_creature(self), 0xa00) |= 4;         // 0057B2DA..E8
    return k_complete_00745dc0;                                                     // 0057B2EF
}

// =========================================================================================================
// 0057B320 CSWSObject::AIActionResumeDialog(node): +50 = 0; a creature: ActivityManager(2), clear +A00 bit 4,
// SetActivity(1, 0, 0).
U __fastcall w5_action_resume_dialog_0057b320(void* self, void*, void*) {
    U dead = 0;
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057B328..3D
    if (vcall<U>(self, 0x94) == 1 || kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1 || dead == 1) { // 0057B33F..5D
        vcall<void>(self, 0xc0, k_object_invalid);                                  // 0057B3B5..BF
        return k_failed_00745dc4;
    }
    at<U>(self, 0x50) = 0;                                                          // 0057B363
    if (as_creature(self) != nullptr) {                                             // 0057B36A..6F
        w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2);       // 0057B371..37C
        void* c = as_creature(self);                                                // 0057B381..385
        at<U>(c, 0xa00) = at<U>(c, 0xa00) & 0xfffffffbu;                            // 0057B388..395
        recovered_set_activity_004f00a0(as_creature(self), nullptr, 1, 0, 0);       // 0057B38E..3A6
    }
    return k_complete_00745dc0;                                                     // 0057B3AB
}

// =========================================================================================================
// 0057C7B0 CSWSObject::AIActionSetCommandable(node): +E8 = node +38.
U __fastcall w5_action_set_commandable_0057c7b0(void* self, void*, void* node) {
    U dead = 0;
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057C7B8..CD
    if (vcall<U>(self, 0x94) != 0) return k_failed_00745dc4;                       // 0057C7CF..DB (test, not cmp 1)
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) != 0) return k_failed_00745dc4; // 0057C7DD..E6
    if (dead != 0) return k_failed_00745dc4;                                        // 0057C7E8..EA
    at<U>(self, 0xe8) = at<U>(node, 0x38);                                          // 0057C7EC..F3
    return k_complete_00745dc0;                                                     // 0057C7F9
}

// =========================================================================================================
// 0057B430 CSWSObject::AIActionSpeak(node): node +38 CExoString*, +3C talk volume; chat to every player (target -1).
U __fastcall w5_action_speak_0057b430(void* self, void*, void* node) {
    alignas(4) unsigned char text[8];                                               // CExoString [esp+8]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(text), nullptr); // 0057B44C..450
    if (as_creature(self) != nullptr) {                                             // 0057B455..466
        w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2);       // 0057B468..473
        recovered_set_activity_004f00a0(as_creature(self), nullptr, 1, 0, 0);       // 0057B478..487
    }
    async_assign_string_005e5c50(text, nullptr, at<void*>(node, 0x38));            // 0057B48C..498
    const U volume = at<U>(node, 0x3c);                                             // 0057B49D
    alignas(4) unsigned char extra[8];                                              // CExoString [esp+10]
    frame_LogStringCtor_005e5a90(extra, nullptr, k_empty_0073d71c);                 // 0057B4A0..4A9
    W5ExoStringArg by_value;                                                        // the by-value argument copy
    r8_copy_construct_005e5b00(&by_value, nullptr, text);                           // 0057B4B5..4C8 (destroyed by the callee)
    recovered_send_chat_00571960(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr, volume,
                                 at<U>(self, 4), by_value, 0xffffffffu, extra);    // 0057B4AE..4E2
    frame_LogStringDtor_005e5c20(extra, nullptr);                                   // 0057B4E7..4F0
    const U result = k_complete_00745dc0;                                           // 0057B4F5
    frame_LogStringDtor_005e5c20(text, nullptr);                                    // 0057B4FB..507
    return result;
}

// =========================================================================================================
// 0057B3D0 CSWSObject::AIActionSpeakStrRef(node): node +38 strref, +3C talk volume. No dead/command gate.
U __fastcall w5_action_speak_str_ref_0057b3d0(void* self, void*, void* node) {
    if (as_creature(self) != nullptr) {                                             // 0057B3D3..3DA
        w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2);       // 0057B3DC..3E7
        recovered_set_activity_004f00a0(as_creature(self), nullptr, 1, 0, 0);       // 0057B3EC..3FB
    }
    const U strref = at<U>(node, 0x38), volume = at<U>(node, 0x3c);                 // 0057B400..407
    w5_message_send_chat_str_ref_message_00571760(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                                  volume, at<U>(self, 4), strref);  // 0057B40A..420 (result unused)
    return k_complete_00745dc0;                                                     // 0057B425
}

// =========================================================================================================
// 0057CE00 CSWSObject::AIActionBarkString(node): node +38 strref -> TLK text + sound, shown as a bark bubble.
U __fastcall w5_action_bark_string_0057ce00(void* self, void*, void* node) {
    const U strref = at<U>(node, 0x38);                                             // 0057CE18..20 (EDI)
    struct StrRefRecord { unsigned char text[8]; unsigned char sound[16]; U sound_length; }; // fetch writes +18 (0041E344/0041E350)        // [esp+20] CExoString, [esp+28] CResRef
    alignas(4) StrRefRecord record;
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(record.text), nullptr); // 0057CE23..27
    c2da_default_ref_00405ed0(record.sound, nullptr);                               // 0057CE2C..38
    tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &record, k_tlk_flag_0073ecd0); // 0057CE3D..5A
    alignas(4) unsigned char sound[16];                                             // [esp+10] (four DWORD copies)
    std::memcpy(sound, record.sound, 16);                                           // 0057CE5F..84
    alignas(4) unsigned char text[8];                                               // [esp+8]
    r8_copy_construct_005e5b00(text, nullptr, record.text);                         // 0057CE7B..88
    const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, at<U>(self, 4)); // 0057CE8D..9F
    kotor_fastcall_005edbb0(client_app(), nullptr, client_id, text, sound, 0); // 0057CEA4..BA
    const U result = k_complete_00745dc0;                                           // 0057CEBF
    frame_LogStringDtor_005e5c20(text, nullptr);                                    // 0057CEC5..CE
    frame_LogStringDtor_005e5c20(record.text, nullptr);                             // 0057CED3..DF
    return result;
}

// =========================================================================================================
// 0057CF00 CSWSObject::AIActionPlaySound(node): node +38 CExoString* sound; tells every player whose creature is in
// the same area within 1000 m (squared compare) to play it.
U __fastcall w5_action_play_sound_0057cf00(void* self, void*, void* node) {
    alignas(4) unsigned char sound[8];                                              // CExoString [esp+14]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(sound), nullptr); // 0057CF1D..21
    async_assign_string_005e5c50(sound, nullptr, at<void*>(node, 0x38));           // 0057CF26..3A
    void* const message = recovered_server_sws_message_004aedb0(server_app(), nullptr); // 0057CF3F..4D (over the arg slot)
    void* const players = recovered_server_player_list_004aed90(server_app(), nullptr); // 0057CF51..5E (EBP)
    void* position = at<void*>(at<void*>(players, 0), 0);                          // 0057CF60..67 ([esp+C] = head)
    while (position != nullptr) {                                                   // 0057CF6B / 0057D047..49
        void* const player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, position); // 0057CF72..7B (EDI)
        if (kotor_fastcall_00561080(player, nullptr) != nullptr) {                  // 0057CF7D..86
            void* const creature = kotor_fastcall_00561080(player, nullptr);        // 0057CF8C..93
            if (creature != nullptr) {                                              // 0057CF95..97
                void* const area = recovered_object_area_004cb120(creature, nullptr); // 0057CF9D..9F ([esp+14])
                Vector p; copy_vector(&p, bp(creature) + 0x90);                     // 0057CFA4..C0 ([esp+20])
                if (area == recovered_object_area_004cb120(self, nullptr)) {        // 0057CFBA..CD
                    const double dz = static_cast<double>(p.z) - at<float>(self, 0x98);   // 0057CFCF..D3
                    const double dy = static_cast<double>(p.y) - at<float>(self, 0x94);   // 0057CFD9..DD
                    const double dx = static_cast<double>(p.x) - at<float>(self, 0x90);   // 0057CFE3..E7
                    const double d2 = (dx * dx + dy * dy) + dz * dz;                // 0057CFED..FB
                    const double r2 = static_cast<double>(k_sound_range_00747444) * k_sound_range_00747444; // 0057CFFD..003
                    if (!(r2 < d2 || r2 != r2 || d2 != d2) && message != nullptr)   // 0057D009..1E (fcompp; test ah,1)
                        w5_message_send_ai_action_play_sound_0056d4a0(message, nullptr, at<U>(player, 4),
                                                                      at<U>(self, 4), sound); // 0057D020..31
                }
            }
        }
        unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &position);  // 0057D036..43
    }
    const U result = k_complete_00745dc0;                                           // 0057D050
    frame_LogStringDtor_005e5c20(sound, nullptr);                                   // 0057D056..62
    return result;
}

// =========================================================================================================
// 0057D080 CSWSObject::AIActionPlayAnimation(node): node +38 animation, +3C speed, +40 duration, +44 first-run flag,
// +48 GUI-sound-done flag, +4C GUI sound delay (ms). Start time at self +C4/+C8.
U __fastcall w5_action_play_animation_0057d080(void* self, void*, void* node) {
    U dead = 0;                                                                     // ESI
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057D0A1..B6
    if (vcall<U>(self, 0x94) == 1) return k_failed_00745dc4;                       // 0057D0B8..C5
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_failed_00745dc4; // 0057D0CB..D5
    if (dead == 1) return k_failed_00745dc4;                                        // 0057D0DB..DE
    const float speed_f = at<float>(node, 0x3c);                                    // [esp+1C]
    const float duration = at<float>(node, 0x40);                                   // [esp+14]
    U sound_done = at<U>(node, 0x48);                                               // [esp+2C]
    const U animation = at<U>(node, 0x38);                                          // EBP, [esp+18]
    const U first = at<U>(node, 0x44);                                              // ESI
    U sound_delay = at<U>(node, 0x4c);                                              // [esp+60] (arg slot)
    U day = 0, time = 0;                                                            // [esp+24], [esp+20]
    frame_GetWorldTime_004ade40(timer(), nullptr, &day, &time);                     // 0057D103..129
    if (first == 1) {                                                               // 0057D12E..131
        at<U>(self, 0xc4) = day; at<U>(self, 0xc8) = time;                          // 0057D133..141
        at<U>(node, 0x44) = 0;                                                      // 0057D147
        const U id = at<U>(self, 4);                                                // 0057D157
        if (id == kotor_fastcall_004aea40(server_app(), nullptr)) {                 // 0057D14E..161
            const U index = animation - 0x2790u;                                    // 0057D163
            // byte table 0057D454 -> jump table 0057D448: 0 = 250 ms, 1 = 750 ms, 2 = no GUI sound
            static const U8 k_delay_case_0057d454[14] = {0, 2, 2, 0, 1, 2, 1, 1, 2, 2, 2, 2, 1, 1};
            const U kind = index <= 0xd ? k_delay_case_0057d454[index] : 2u;        // 0057D169..175 (ja -> 0057D190)
            if (kind == 0) sound_delay = 0xfa;                                      // 0057D17C
            else if (kind == 1) sound_delay = 0x2ee;                                // 0057D186
            else { sound_done = 1; at<U>(node, 0x48) = 1; }                         // 0057D190..199
            at<U>(node, 0x4c) = sound_delay;                                        // 0057D19C..1A0
        }
    }
    U out_day = 0, elapsed = 0;                                                     // [esp+34], [esp+30]
    recovered_world_time_difference_004ae460(timer(), nullptr, day, time, at<U>(self, 0xc4), at<U>(self, 0xc8),
                                             &out_day, &elapsed);                   // 0057D1A3..1D5
    // 0057D1DA..278: a long (>= 30 s) animation absorbs identical PlayAnimation nodes queued directly behind it
    U stop = 0;                                                                     // EBP
    const bool long_animation = !(static_cast<double>(duration) < k_long_animation_00749410 || duration != duration); // test ah,1
    if (long_animation && at<void*>(at<void*>(self, 0xfc), 0) != nullptr) {        // 0057D1F1..1FA
        do {
            if (stop != 0) break;                                                   // 0057D200..202
            stop = 1;                                                               // 0057D20A
            void* const queued = base_ini_reconstruction::remove_head_005e9c80(
                static_cast<kotor_list::Internal*>(at<void*>(self, 0xfc)), nullptr); // 0057D204..20F (ESI)
            bool merged = false;
            if (queued != nullptr && at<U>(queued, 0) == 6 && at<U>(queued, 0x38) == animation) { // 0057D216..22D
                const float queued_speed = at<float>(queued, 0x3c);                 // 0057D21F / [esp+28]
                if (queued_speed == speed_f &&                                       // 0057D22F..23C (test ah,44h; jp)
                    !(static_cast<double>(duration) < k_long_animation_00749410 || duration != duration)) { // 0057D23E..24D
                    recovered_action_node_destroy_004cc4d0(queued, nullptr);         // 0057D24F..251
                    live_free(queued);                                               // 0057D256..25C
                    stop = 0;                                                        // 0057D25F
                    merged = true;
                }
            }
            if (!merged) c2da_005e9d70(at<void*>(self, 0xfc), nullptr, queued);    // 0057D263..26A (AddHead, even null)
        } while (at<void*>(at<void*>(self, 0xfc), 0) != nullptr);                  // 0057D26F..278
    }
    const float speed_abs = std::fabs(speed_f); // 0057D27A..28A (fabs; fstp [esp+28])
    float length = flt(0x447a0000u);                                                // [esp+10] = 1000.0
    U fired = 0;                                                                    // ESI
    if (recovered_object_render_004cc2b0(self, nullptr) != nullptr) {               // 0057D28E..29B
        const double raw = kotor_fastcall_0063c0e0(recovered_object_render_004cc2b0(self, nullptr), nullptr, animation); // 0057D29D..2A7
        if (!(raw < k_zero_0073d700 || raw == k_zero_0073d700 || raw != raw))       // 0057D2AC..2B7 (test ah,41h)
            length = static_cast<float>(raw);                                       // 0057D2B9
        fired = kotor_fastcall_0063c380(recovered_object_render_004cc2b0(self, nullptr), nullptr, animation); // 0057D2C1..2D0
    }
    if (!(speed_abs == k_zero_0073d700))                                            // 0057D2D2..2E1 (jnp: equal)
        length = static_cast<float>(static_cast<double>(length) / speed_abs);      // 0057D2E3..2EB
    if (!(duration < k_zero_0073d700 || duration == k_zero_0073d700 || duration != duration) && fired == 0) // 0057D2EF..302
        length = static_cast<float>(static_cast<double>(duration) * k_ms_per_second_0073d6fc); // 0057D304..30E
    if (sound_done == 0 && !(sound_delay > elapsed)) {                              // 0057D312..326 (unsigned ja)
        at<U>(node, 0x48) = 1;                                                      // 0057D330
        alignas(4) unsigned char name[8];                                           // CExoString [esp+38]
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(name), nullptr); // 0057D32C..337
        const U index = animation - 0x2790u;                                        // 0057D33C
        // byte table 0057D474 -> jump table 0057D464: 0 lockpick, 1 mine disarm, 2 mine arm, 3 none
        static const U8 k_sound_case_0057d474[14] = {0, 3, 3, 0, 1, 3, 1, 1, 3, 3, 3, 3, 2, 1};
        const U kind = index <= 0xd ? k_sound_case_0057d474[index] : 3u;            // 0057D342..356 (ja -> 0057D3A6)
        if (kind != 3) {
            const char* const text = kind == 0 ? k_gui_lockpick_00749400 : kind == 1 ? k_gui_minedisarm_007493f0
                                                                                     : k_gui_minearm_007493e4;
            assign_text_005e5140(name, nullptr, text);                              // 0057D35D..374
            menu_resources::Ref ref;                                                // [esp+40]
            ref.construct_00406d80(static_cast<const char*>(async_string_cstr_005e5670(name, nullptr))); // 0057D379..387
            w5_sound_play_one_shot_005d5e00(kotor_cell_sound_007a39ec, nullptr, &ref, 0x16, 0, 0, 0.0f, 0.0f); // 0057D38C..3A1
        }
        frame_LogStringDtor_005e5c20(name, nullptr);                                // 0057D3A6..3B2
    }
    const U limit = w2::ftol2_006fae8c(static_cast<double>(length));                // 0057D3B7..3BB
    if (elapsed < limit) {                                                          // 0057D3C0..3CB (unsigned jae)
        at<float>(self, 0xd8) = speed_f;                                            // 0057D3CD..3D1
        vcall<void>(self, 0x7c, animation & 0xffffu);                               // 0057D3D7..3DB (movzx bp)
        return k_in_progress_00745dbc;                                              // 0057D3DE
    }
    if (as_creature(self) != nullptr) {                                             // 0057D3F7..3FC
        at<U>(self, 0xd8) = k_one_0x3f800000;                                       // 0057D407
        vcall<void>(self, 0x7c, 0x2711u);                                           // 0057D400..411
    }
    return k_complete_00745dc0;                                                     // 0057D414
}

// =========================================================================================================
// 0057BAE0 CSWSObject::AIActionTakeItem(node): node +38 item id, +3C container (taken from), +40 status flag.
U __fastcall w5_action_take_item_0057bae0(void* self, void*, void* node) {
    if (at<U16>(node, 0x6e) < 1) return k_failed_00745dc4;                         // 0057BAE1..AED (jae)
    const U flag = at<U>(node, 0x40);                                               // EBP
    const U item_id = at<U>(node, 0x38);                                            // ESI
    const U container = at<U>(node, 0x3c);                                          // EDI
    void* object = nullptr;                                                         // [esp+10] (pushed ECX slot; callee writes)
    void* item = nullptr;                                                           // [esp+18] (node argument slot)
    if (static_cast<U8>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, item_id,
                                                &object)) == k_found_0074666c && object != nullptr) // 0057BB05..35
        item = as_item(object);                                                     // 0057BB37..3C
    if (item == nullptr) return k_failed_00745dc4;                                  // 0057BB40..52
    const U8 type = at<U8>(self, 8);                                                // 0057BB55
    if (type == 5) {                                                                // 0057BB58..5A
        if (recovered_creature_acquire_item_005158e0(as_creature(self), nullptr, &item, container, k_object_invalid,
                                                     flag) != 0 && flag != 0) {     // 0057BB5C..7C
            void* const taker = kotor_fastcall_004ae770(server_app(), nullptr, container); // 0057BB7E..87
            if (taker != nullptr && at<U>(taker, 0xa88) != 0) {                     // 0057BB8C..98
                kotor_fastcall_0062eeb0(frame_R4_005ed690(client_app(), nullptr), nullptr, 8, 0); // 0057BB9A..BAE
                return k_complete_00745dc0;                                         // 0057BBB3
            }
        }
    } else if (type == 9) {                                                         // 0057BBC0..C2
        recovered_placeable_acquire_item_00584b10(as_placeable(self), nullptr, &item, container, 1); // 0057BBC4..BD5
    }
    return k_complete_00745dc0;                                                     // 0057BBDA
}

// =========================================================================================================
// 0057BBF0 CSWSObject::AIActionCloseDoor(node): node +38 door (or placeable) id, +3C run flag (2+ parameters).
U __fastcall w5_action_close_door_0057bbf0(void* self, void*, void* node) {
    U dead = 0;
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057BBFB..10
    if (vcall<U>(self, 0x94) == 1) return k_failed_00745dc4;                       // 0057BC12..1F
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_failed_00745dc4; // 0057BC25..2F
    if (dead == 1) return k_failed_00745dc4;                                        // 0057BC35..38
    if (as_creature(self) != nullptr) w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2); // 0057BC3E..54
    U door_id = at<U>(node, 0x38);                                                  // [esp+C]
    U run = 0;                                                                      // [esp+10]
    if (at<U16>(node, 0x6e) > 1 && at<U>(node, 0x3c) != 0) run = at<U>(node, 0x3c); // 0057BC5D..7A (jbe)
    Vector pos; std::memset(&pos, 0, sizeof(pos));                                  // [esp+1C..24]
    void* object = nullptr;                                                         // [esp+8] (written by the callee)
    if (static_cast<U8>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, door_id,
                                                &object)) != k_found_0074666c)
        return k_failed_00745dc4;                                                   // 0057BC7E..B7
    bool close_now = as_creature(self) == nullptr;                                  // 0057BCBD..C6
    if (!close_now) {
        if (as_door(object) == nullptr && as_placeable(object) == nullptr) return k_failed_00745dc4; // 0057BCCC..E4
        (void)recovered_object_area_004cb120(object, nullptr);                      // 0057BCEA..EF (result unused)
        float range;                                                                // [esp+14] (written by the callee)
        w5_creature_get_use_range_004ee440(as_creature(self), nullptr, door_id, &pos, &range, 0); // 0057BCF4..D0E
        void* const object_area = recovered_object_area_004cb120(as_object(object), nullptr); // 0057BD13..1E (EBX)
        if (recovered_object_area_004cb120(self, nullptr) == object_area &&         // 0057BD23..2F
            w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, door_id, 0.0f, 0) != 0) // 0057BD31..4A
            close_now = true;
        if (!close_now) {
            const U pairs[26] = {3, addr(&door_id), 1, addr(&run)};
            w5_add_action_to_front_body(as_creature(self), 0x15, 0xffff, pairs);    // 0057BD50..D9A
            Vector zero; std::memset(&zero, 0, sizeof(zero));                       // [esp+28]
            w5_creature_add_move_to_point_action_to_front_004f8a50(as_creature(self), nullptr, at<U16>(node, 0x6c), &pos,
                at<U>(as_object(object), 0x8c), k_object_invalid, run, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0057BD9F..E04
            return k_complete_00745dc0;                                             // 0057BE09
        }
    }
    vcall<void>(self, 0xc0, door_id);                                               // 0057BE16..1F
    ai_event(at<U>(self, 4), door_id, k_ai_event_close_door_007441cc, nullptr);     // 0057BE25..4A
    U half = k_half_0x3f000000;                                                     // [esp+18]
    const U pairs[26] = {2, addr(&half)};
    w5_add_action_to_front_body(self, 0x1e, at<U16>(node, 0x6c), pairs);           // 0057BE4F..9C (non-virtual ECX = self)
    return k_complete_00745dc0;                                                     // 0057BEA1
}

// =========================================================================================================
// 0057BEC0 CSWSObject::AIActionLockObject(node): node +38 target door/placeable id. Creature +984 = "lock animation
// started". Security skill (6) d20 check against door +2C0 / placeable +275; result broadcast as skill data 0x149.
U __fastcall w5_action_lock_object_0057bec0(void* self, void*, void* node) {
    U changed = 0;                                                                  // [esp+14]
    U take20 = 0;                                                                   // [esp+18]
    U dead = 0;                                                                     // EDI
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057BEEC..F01
    if (vcall<U>(self, 0x94) == 1 || kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1 || dead == 1) { // 0057BF03..29
        if (as_creature(self) == nullptr) return k_failed_00745dc4;                // 0057C76F..778
        stop_timing_bar(self, 0, 0x984);                                            // 0057C77A..78F (EBX = 0)
        return k_failed_00745dc4;                                                   // 0057C795
    }
    if (as_creature(self) != nullptr) w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2); // 0057BF2F..45
    U id = at<U>(node, 0x38);                                                       // 0057BF4A..57 ([esp+10])
    void* const object = game_object(id);                                           // 0057BF51..64 (EBX; no null check)
    void* const target = as_object(object);                                         // 0057BF66..6D (EDI)
    if (target == nullptr) {                                                        // 0057BF6F..73
        if (as_creature(self) == nullptr) return k_failed_00745dc4;                // 0057BF75..7C
        stop_timing_bar(self, 0, 0x984);                                            // 0057BF82..97 (EDI = 0)
        return k_failed_00745dc4;                                                   // 0057BF9D -> 0057C795
    }
    Vector pos; std::memset(&pos, 0, sizeof(pos));                                  // [esp+2C] (zeroed on the creature path)
    if (as_creature(self) != nullptr &&
        w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, id, 0.0f, 0) == 0) { // 0057BFA2..E0
        float range;                                                                // [esp+1C]
        w5_creature_get_use_range_004ee440(as_creature(self), nullptr, id, &pos, &range, 0); // 0057BFE6..FFF
        const U lock_pairs[26] = {3, addr(&id)};
        w5_add_action_to_front_body(self, 0x27, at<U16>(node, 0x6c), lock_pairs);  // 0057C004..046
        const U8 type = at<U8>(object, 8);                                          // 0057C04B
        if (type == k_type_door_00746639 || type == k_type_placeable_00746638) {    // 0057C04E..05C
            const U pairs[26] = {3, addr(&id)};
            w5_add_action_to_front_body(self, 0x13, at<U16>(node, 0x6c), pairs);   // 0057C0AF..0F1
        } else {
            const U pairs[26] = {2, addr(&pos.x), 2, addr(&pos.y), 2, addr(&pos.z)};
            w5_add_action_to_front_body(self, 0x31, at<U16>(node, 0x6c), pairs);   // 0057C05E..0F1
        }
        Vector zero; std::memset(&zero, 0, sizeof(zero));                           // [esp+38]
        w5_creature_add_move_to_point_action_to_front_004f8a50(as_creature(self), nullptr, at<U16>(node, 0x6c), &pos,
            at<U>(target, 0x8c), id, 1, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0057C0F6..155
        return k_complete_00745dc0;                                                 // 0057C15A
    }
    if (as_creature(self) != nullptr && at<U>(as_creature(self), 0x984) == 0) {    // 0057C164..182
        at<U>(as_creature(self), 0x984) = 1;                                        // 0057C188..194
        U one = 1;                                                                  // [esp+20]
        U animation = addr(node);                                                   // [esp+54]: the node argument slot keeps
        const U8 type = at<U8>(object, 8);                                          //  the node pointer for other types
        if (type == k_type_door_00746639) animation = 0x2790;                       // 0057C19A..1B1
        else if (type == k_type_placeable_00746638) animation = 0x2793;             // 0057C1B3..1BB
        U duration = k_one_and_half_0x3fc00000;                                     // [esp+24]
        U speed = k_one_0x3f800000;                                                 // [esp+28]
        const U lock_pairs[26] = {3, addr(&id)};
        w5_add_action_to_front_body(self, 0x27, at<U16>(node, 0x6c), lock_pairs);  // 0057C1C3..21B
        const U anim_pairs[26] = {1, addr(&animation), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
        w5_add_action_to_front_body(self, 6, at<U16>(node, 0x6c), anim_pairs);     // 0057C220..26F
        kotor_fastcall_004ef480(as_creature(self), nullptr, 0x5dc, 8); // 0057C274..284
        return k_complete_00745dc0;                                                 // 0057C289
    }
    if (as_creature(self) != nullptr &&
        w5_creature_evaluate_lock_004f0b20(as_creature(self), nullptr, target, 1) == 1) { // 0057C293..2B2
        stop_timing_bar(self, 0, 0x984);                                            // 0057C2B4..2C9
        return k_complete_00745dc0;                                                 // 0057C2D3
    }
    if (as_creature(self) != nullptr &&
        w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, id, 0.0f, 0) == 0) { // 0057C2DD..306
        stop_timing_bar(self, 0, 0x984);                                            // 0057C73E..75E
        return k_complete_00745dc0;                                                 // 0057C768
    }
    bool script_failure = false;                                                    // -> 0057C460 (OnFailToOpen 22h)
    bool already_locked = false;                                                    // -> 0057C34D (feedback 0Dh)
    if (as_door(target) != nullptr) {                                               // 0057C30C..319
        if (at<U>(as_door(target), 0x2c4) != 0) already_locked = true;              // 0057C31F..32A
        else if (at<U>(as_door(target), 0x2d8) != 0) script_failure = true;         // 0057C363..370
        else if (at<U>(target, 4) == at<U>(self, 4)) { at<U>(as_door(target), 0x2c4) = 1; changed = 1; } // 0057C398..3B1, 0057C4E3
    } else if (as_placeable(target) != nullptr) {                                   // 0057C3B6..3BD
        if (at<U>(as_placeable(target), 0x260) != 0) already_locked = true;         // 0057C3C3..3D0
        else if (at<U>(as_placeable(target), 0x26c) != 0) script_failure = true;    // 0057C428..435
        else if (at<U>(target, 4) == at<U>(self, 4)) { at<U>(as_placeable(target), 0x260) = 1; changed = 1; } // 0057C4CE..4E3
    }
    if (already_locked) {
        if (as_creature(self) == nullptr) return k_failed_00745dc4;                // 0057C32C..335 / 0057C3D2..3DB
        void* const data = new_message_data();                                      // 0057C33B..35E
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(target, 4));        // 0057C3FE..40D
        recovered_creature_feedback_004ede10(as_creature(self), nullptr, 0xd, data); // 0057C412..41E
        stop_timing_bar(self, 0, 0x984);                                            // 0057C77A..78F
        return k_failed_00745dc4;
    }
    if (script_failure) {
        void* const event = new_script_event(0x22);                                 // 0057C372..461
        ai_event(at<U>(self, 4), id, k_ai_event_script_007441dc, event);            // 0057C466..48F
        if (as_creature(self) == nullptr) return k_failed_00745dc4;                // 0057C494..49D
        recovered_creature_feedback_004ede10(as_creature(self), nullptr, 0xf, nullptr); // 0057C4A3..4AF
        stop_timing_bar(self, 0, 0x984);                                            // 0057C4B4..4C9 -> 0057C78F
        return k_failed_00745dc4;
    }
    U roll = addr(node);                                                            // [esp+54] (node slot)
    I skill, dc;                                                                    // EBX, EBP
    bool success = false;
    if (as_creature(self) != nullptr) {                                             // 0057C4E7..4F0
        if (at<U>(as_creature(self), 0x4e0) != 0) roll = static_cast<U>(crt_rand_006fc447() % 20 + 1); // 0057C4F6..517
        else { roll = 0x14; take20 = 1; }                                           // 0057C519..521
        skill = static_cast<I>(static_cast<signed char>(static_cast<U8>(
            w1x_creature_skill_rank_005aa570(at<void*>(as_creature(self), 0xa74), nullptr, 6, nullptr, 0)))); // 0057C525..53F
        if (as_door(target) != nullptr) {                                           // 0057C542..54F
            dc = at<U8>(as_door(target), 0x2c0) < 1 ? 1 : static_cast<I>(at<U8>(as_door(target), 0x2c0)); // 0057C551..56B
            if (skill + static_cast<I>(roll) >= dc) { at<U>(as_door(target), 0x2c4) = 1; success = true; } // 0057C572..58E
        } else if (as_placeable(target) != nullptr) {                               // 0057C590..595
            dc = at<U8>(as_placeable(target), 0x275) < 1 ? 1 : static_cast<I>(at<U8>(as_placeable(target), 0x275)); // 0057C597..5B5
            if (skill + static_cast<I>(roll) >= dc) { at<U>(as_placeable(target), 0x260) = 1; success = true; } // 0057C5BC..5D8
        } else {
            dc = static_cast<I>(roll);                                              // 0057C5DE
        }
    } else {
        skill = static_cast<I>(roll); dc = static_cast<I>(roll);                    // 0057C5DA..5DE (unused below)
    }
    if (success || changed != 0) {                                                  // 0057C5E2..5E8
        ai_event(at<U>(self, 4), id, k_ai_event_locked_007441e8, nullptr);          // 0057C5EE..614
        void* const event = new_script_event(0x1c);                                 // 0057C619..63F (OnLock)
        ai_event(at<U>(self, 4), at<U>(target, 4), k_ai_event_script_007441dc, event); // 0057C644..66E
    }
    if (as_creature(self) != nullptr) {                                             // 0057C673..67C
        void* const data = new_message_data();                                      // 0057C682..6A7
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));          // 0057C6A9..6B9
        recovered_event_integer_004d76b0(data, nullptr, 0, 0x149);                  // 0057C6BE..6C7
        recovered_event_integer_004d76b0(data, nullptr, 1, roll);                   // 0057C6CC..6D5
        recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(skill));  // 0057C6DA..6DF
        recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));     // 0057C6E4..6E9
        recovered_event_integer_004d76b0(data, nullptr, 4, take20);                 // 0057C6EE..6F7
        const U result = skill + static_cast<I>(roll) >= dc ? 1u : take20 != 0 ? 3u : 0u; // 0057C6FC..716 (jl)
        recovered_event_integer_004d76b0(data, nullptr, 5, result);                 // 0057C718..71C
        recovered_event_integer_004d76b0(data, nullptr, 6, 0x149);                  // 0057C721..72A
        kotor_fastcall_004ec700(as_creature(self), nullptr, data); // 0057C72F..739
        stop_timing_bar(self, 0, 0x984);                                            // 0057C73E..75E
    }
    return k_complete_00745dc0;                                                     // 0057C768
}

// =========================================================================================================
// 0057D9D0 CSWSObject::AIActionUnlockObject(node): node +38 target id, +3C tool item id, +40 tool property index.
// Creature +9A4 = "approach voice done", +980 = "unlock animation started". Security (6) d20 + tool upgrade bonus
// against door +2BF / placeable +274; result broadcast as skill data 0x149 (take-20 and DC >= 60 -> 5).
U __fastcall w5_action_unlock_object_0057d9d0(void* self, void*, void* node) {
    U changed = 0;                                                                  // [esp+24]
    U take20 = 0;                                                                   // [esp+28]
    U dead = 0;                                                                     // EBX
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057D9FC..A11
    auto fail_reset = [&](bool stop) -> U {                                         // 0057E865 / 0057E870 / 0057E87E
        if (as_creature(self) == nullptr) return k_failed_00745dc4;
        if (stop) recovered_creature_cancel_action_004ef4d0(as_creature(self), nullptr);
        at<U>(as_creature(self), 0x980) = 0;                                        // EDI = 0 on every path into it
        at<U>(as_creature(self), 0x9a4) = 0;
        return k_failed_00745dc4;                                                   // 0057E898
    };
    auto done_reset = [&]() -> U {                                                  // 0057E829..85E
        if (as_creature(self) != nullptr) {
            recovered_creature_cancel_action_004ef4d0(as_creature(self), nullptr);
            at<U>(as_creature(self), 0x980) = 0;
            at<U>(as_creature(self), 0x9a4) = 0;
        }
        return k_complete_00745dc0;
    };
    if (vcall<U>(self, 0x94) == 1 || kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1 || dead == 1) // 0057DA13..39
        return fail_reset(true);
    if (as_creature(self) != nullptr) w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2); // 0057DA3F..55
    U id = at<U>(node, 0x38);                                                       // [esp+10]
    U tool_id = at<U>(node, 0x3c);                                                  // [esp+1C]
    U property = at<U>(node, 0x40);                                                 // [esp+2C]
    void* const object = game_object(id);                                           // 0057DA6B..82 (EBP)
    if (object == nullptr) return fail_reset(true);                                 // 0057DA84..86
    void* const target = as_object(object);                                         // 0057DA8C..94 (EDI)
    if (target == nullptr) return fail_reset(true);                                 // 0057DA96..ACB (same three writes)
    Vector pos; std::memset(&pos, 0, sizeof(pos));                                  // [esp+34]
    float range;                                                                    // [esp+20]
    auto queue_unlock_and_approach = [&]() {                                        // 0057DB2D..C1D / 0057DCD8..DC8
        const U unlock_pairs[26] = {3, addr(&id), 3, addr(&tool_id)};
        w5_add_action_to_front_body(self, 0x26, at<U16>(node, 0x6c), unlock_pairs);
        const U8 type = at<U8>(object, 8);
        if (type == k_type_door_00746639 || type == k_type_placeable_00746638) {
            const U pairs[26] = {3, addr(&id)};
            w5_add_action_to_front_body(self, 0x13, at<U16>(node, 0x6c), pairs);
        } else {
            const U pairs[26] = {2, addr(&pos.x), 2, addr(&pos.y), 2, addr(&pos.z)};
            w5_add_action_to_front_body(self, 0x31, at<U16>(node, 0x6c), pairs);
        }
    };
    if (as_creature(self) != nullptr) {                                             // 0057DAD0..D2
        if (w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, id, 0.0f, 0) == 0) { // 0057DAD8..B09
            w5_creature_get_use_range_004ee440(as_creature(self), nullptr, id, &pos, &range, 0); // 0057DB0F..B28
            queue_unlock_and_approach();                                            // 0057DB2D..C1D
            Vector zero; std::memset(&zero, 0, sizeof(zero));                       // [esp+40]
            w5_creature_add_move_to_point_action_to_front_004f8a50(as_creature(self), nullptr, at<U16>(node, 0x6c), &pos,
                at<U>(target, 0x8c), id, 1, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0057DC22..C84
            return k_complete_00745dc0;                                             // 0057DC89
        }
        if (at<U>(as_creature(self), 0x9a4) == 0) {                                 // 0057DC93..CA2
            at<U>(as_creature(self), 0x9a4) = 1;                                    // 0057DCA8..CB6
            w5_creature_get_use_range_004ee440(as_creature(self), nullptr, id, &pos, &range, 0); // 0057DCAF..CD3
            queue_unlock_and_approach();                                            // 0057DCD8..DC8
            w5_creature_broadcast_voice_chat_004f2520(as_creature(self), nullptr, 0x17); // 0057DDCD..DD8
            return k_complete_00745dc0;                                             // 0057DDDD
        }
    }
    if (as_creature(self) != nullptr && at<U>(as_creature(self), 0x980) == 0) {    // 0057DDE7..E05
        at<U>(as_creature(self), 0x980) = 1;                                        // 0057DE0B..E17
        vcall<void>(self, 0xc0, id);                                                // 0057DE1D..E26
        U animation = 0x2793;                                                       // [esp+5C] (node slot)
        U one = 1;                                                                  // [esp+28]
        const U8 type = at<U8>(object, 8);                                          // 0057DE2C
        if (type == k_type_door_00746639) animation = 0x2790;                       // 0057DE2F..E4C
        else if (type == k_type_placeable_00746638) animation = 0x2793;             // 0057DE4E..E56
        U speed = k_one_0x3f800000;                                                 // [esp+20]
        U duration = k_one_and_half_0x3fc00000;                                     // [esp+24]
        const U unlock_pairs[26] = {3, addr(&id), 3, addr(&tool_id)};
        w5_add_action_to_front_body(self, 0x26, at<U16>(node, 0x6c), unlock_pairs); // 0057DE5A..EB5
        const U anim_pairs[26] = {1, addr(&animation), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
        w5_add_action_to_front_body(self, 6, at<U16>(node, 0x6c), anim_pairs);     // 0057DEBA..F06
        kotor_fastcall_004ef480(as_creature(self), nullptr, 0x5dc, 7); // 0057DF0B..F1B
        return k_complete_00745dc0;                                                 // 0057DF20
    }
    void* const door = as_door(target);                                             // 0057DF2A..35 (EBP)
    void* const placeable = as_placeable(target);                                   // 0057DF31..3C (EBX)
    // 0057DF3A..E06D: a hostile-owned (reputation < 90, other faction) lockable reports OnFailToOpen (1Ah) and stops
    bool hostile = false;
    if (door != nullptr && at<U>(door, 0x2e8) != 0 && as_creature(self) != nullptr) {        // 0057DF3A..53
        if (static_cast<I>(recovered_object_get_reputation_0057cb80(door, nullptr, at<U>(self, 4), 0)) < 0x5a && // 0057DF55..65
            at<U>(door, 0x2b8) != at<U>(at<void*>(as_creature(self), 0xa74), 0x78)) hostile = true; // 0057DF6B..89
    } else if (placeable != nullptr && at<U>(placeable, 0x278) != 0 && as_creature(self) != nullptr) { // 0057DFB7..D6
        if (static_cast<I>(recovered_object_get_reputation_0057cb80(placeable, nullptr, at<U>(self, 4), 0)) < 0x5a && // 0057DFDC..EC
            at<U>(placeable, 0x23c) != at<U>(at<void*>(as_creature(self), 0xa74), 0x78)) hostile = true; // 0057DFF2..10
    }
    if (hostile) {
        void* const event = new_script_event(0x1a);                                 // 0057DF8F..E03A
        ai_event(at<U>(self, 4), id, k_ai_event_script_007441dc, event);            // 0057E03F..068
        return fail_reset(false);                                                   // 0057E06D -> 0057E87E
    }
    if (as_creature(self) != nullptr &&
        w5_creature_evaluate_lock_004f0b20(as_creature(self), nullptr, target, 0) == 1) { // 0057E072..095
        if (door != nullptr) {
            w5_door_open_door_00589c70(door, nullptr, at<U>(self, 4));              // 0057E09D..0A3
        } else if (placeable != nullptr) {
            U placeable_id = at<U>(placeable, 4);                                   // [esp+28]
            const U pairs[26] = {3, addr(&placeable_id)};
            w5_add_action_to_front_body(self, 0x28, at<U16>(node, 0x6c), pairs);   // 0057E0AE..0EC
        }
        return done_reset();                                                        // 0057E0F1..106 -> 0057E84B
    }
    if (as_creature(self) != nullptr &&
        w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, id, 0.0f, 0) == 0) // 0057E10B..12F
        return done_reset();                                                        // -> 0057E829
    // 0057E135..3F4: already unlocked -> feedback 0Eh; plot/script-locked -> OnFailToOpen (22h) + failed skill data
    bool script_failure = false;
    if (door != nullptr) {
        if (at<U>(door, 0x2c4) == 0) {                                              // 0057E13D..145 -> 0057E26A
            if (as_creature(self) == nullptr) return k_failed_00745dc4;
            recovered_creature_feedback_004ede10(as_creature(self), nullptr, 0xe, nullptr); // 0057E279..287
            return fail_reset(true);                                                // 0057E28C -> 0057E870
        }
        if (at<U>(door, 0x2d8) != 0) script_failure = true;                         // 0057E14B..153
        else if (at<U>(target, 4) == at<U>(self, 4)) {                              // 0057E232..238
            at<U>(door, 0x2c4) = 0;                                                 // 0057E23E
            w5_door_open_door_00589c70(door, nullptr, at<U>(self, 4));              // 0057E248..24E
            changed = 1;                                                            // 0057E40B
        }
    } else if (placeable != nullptr) {                                              // 0057E258..25A
        if (at<U>(placeable, 0x260) == 0) {                                         // 0057E260..268 -> 0057E26A
            if (as_creature(self) == nullptr) return k_failed_00745dc4;
            recovered_creature_feedback_004ede10(as_creature(self), nullptr, 0xe, nullptr);
            return fail_reset(true);
        }
        if (at<U>(placeable, 0x26c) != 0) script_failure = true;                    // 0057E291..299
        else if (at<U>(target, 4) == at<U>(self, 4)) {                              // 0057E3F9..3FF
            at<U>(placeable, 0x260) = 0;                                            // 0057E401
            changed = 1;                                                            // 0057E40B
        }
    }
    if (script_failure) {
        void* const event = new_script_event(0x22);                                 // 0057E159..181 / 0057E29F..2C7
        ai_event(at<U>(self, 4), id, k_ai_event_script_007441dc, event);            // 0057E186..1AE / 0057E2CC..2F4
        if (as_creature(self) != nullptr) {                                         // 0057E1B3..1BC
            recovered_creature_feedback_004ede10(as_creature(self), nullptr, 0xf, nullptr); // 0057E1BE..1CA
            recovered_creature_cancel_action_004ef4d0(as_creature(self), nullptr);  // 0057E1CF..1D8
            at<U>(as_creature(self), 0x980) = 0;                                    // 0057E1DD..1E4
            at<U>(as_creature(self), 0x9a4) = 0;                                    // 0057E1EA..1F1
        }
        if (as_creature(self) == nullptr) return k_failed_00745dc4;                // 0057E1F7..200
        void* const data = new_message_data();                                      // 0057E206..22B
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));          // 0057E369..374
        recovered_event_object_004d7780(data, nullptr, 1, id);                      // 0057E379..382
        recovered_event_integer_004d76b0(data, nullptr, 0, 0x149);                  // 0057E387..38F
        recovered_event_integer_004d76b0(data, nullptr, 1, 0);                      // 0057E394..399
        recovered_event_integer_004d76b0(data, nullptr, 2, 0);                      // 0057E39E..3A3
        recovered_event_integer_004d76b0(data, nullptr, 3, 0);                      // 0057E3A8..3AD
        recovered_event_integer_004d76b0(data, nullptr, 4, 0);                      // 0057E3B2..3B7
        recovered_event_integer_004d76b0(data, nullptr, 5, 5);                      // 0057E3BC..3C2
        recovered_event_integer_004d76b0(data, nullptr, 6, 0x149);                  // 0057E3C7..3D0
        kotor_fastcall_004ec700(as_creature(self), nullptr, data); // 0057E3D5..3DF
        w5_creature_broadcast_voice_chat_004f2520(as_creature(self), nullptr, 0x18); // 0057E3E4..3EF
        return k_failed_00745dc4;                                                   // 0057E3F4 -> 0057E898
    }
    U roll = 0;                                                                     // [esp+20] (creature path only)
    I skill = 0;                                                                    // [esp+18]
    I dc = static_cast<I>(addr(node));                                              // EBP = [esp+5C] when not resolved
    void* tool = nullptr;                                                           // [esp+14]
    bool success = false;
    if (as_creature(self) != nullptr) {                                             // 0057E413..41C
        if (at<U>(as_creature(self), 0x4e0) != 0) roll = static_cast<U>(crt_rand_006fc447() % 20 + 1); // 0057E422..445
        else { roll = 0x14; take20 = 1; }                                           // 0057E447..44F
        skill = static_cast<I>(static_cast<signed char>(static_cast<U8>(
            w1x_creature_skill_rank_005aa570(at<void*>(as_creature(self), 0xa74), nullptr, 6, nullptr, 0)))); // 0057E457..482
        I bonus = 0;                                                                // EBX
        tool = recovered_item_lookup_004ae760(server_app(), nullptr, tool_id);      // 0057E471..48F
        if (tool != nullptr) {                                                      // 0057E493
            void* const active = kotor_m15_menu_active_property_00553960(tool, nullptr, property); // 0057E495..4A3 (EBP)
            if (active != nullptr) {                                                // 0057E4A5..4A7
                property = (property & 0xffffff00u) | at<U8>(active, 0x11);         // 0057E4A9..4AC (byte store into the slot)
                if (w5_item_has_upgrade_004eada0(tool, nullptr, property) != 0)     // 0057E4B0..4C0
                    bonus = static_cast<I>(at<U16>(active, 6));                     // 0057E4C2 (movzx word)
            }
        }
        skill += bonus;                                                             // 0057E4C6..4D0
        if (as_door(target) != nullptr) {                                           // 0057E4CA..4DB
            dc = at<U8>(as_door(target), 0x2bf) < 1 ? 1 : static_cast<I>(at<U8>(as_door(target), 0x2bf)); // 0057E4DD..4F9
            if (skill + static_cast<I>(roll) >= dc) {                               // 0057E500..50C (jl)
                at<U>(as_door(target), 0x2c4) = 0;                                  // 0057E512..519
                w5_door_open_door_00589c70(as_door(target), nullptr, at<U>(self, 4)); // 0057E523..530
                success = true;                                                     // -> 0057E629
            }
        } else if (as_placeable(target) != nullptr) {                               // 0057E53A..541
            dc = at<U8>(as_placeable(target), 0x274) < 1 ? 1 : static_cast<I>(at<U8>(as_placeable(target), 0x274)); // 0057E547..565
            if (skill + static_cast<I>(roll) >= dc) {                               // 0057E56C..578
                at<U>(as_placeable(target), 0x260) = 0;                             // 0057E57E..585
                if (at<U>(at<void*>(as_creature(self), 0xa74), 0x6c) != 0 ||       // 0057E58F..5A1
                    recovered_party_table_leader_00563a00(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), nullptr,
                                                          at<U>(self, 4)) != 0) {   // 0057E5A3..5BE
                    U placeable_id = at<U>(target, 4);                              // [esp+30]
                    const U pairs[26] = {3, addr(&placeable_id)};
                    w5_add_action_to_front_body(self, 0x28, at<U16>(node, 0x6c), pairs); // 0057E5C0..616
                }
                success = true;                                                     // -> 0057E629
            }
        }
    }
    if (success || changed != 0) {                                                 // 0057E621..627
        ai_event(at<U>(self, 4), id, k_ai_event_unlocked_007441e4, nullptr);        // 0057E629..64F
        if (as_creature(self) != nullptr) w5_creature_broadcast_voice_chat_004f2520(as_creature(self), nullptr, 0x19); // 0057E654..679
    } else if (as_creature(self) != nullptr) {                                      // 0057E663..66C
        w5_creature_broadcast_voice_chat_004f2520(as_creature(self), nullptr, 0x18); // 0057E66E..679
    }
    if (as_creature(self) == nullptr) return done_reset();                          // 0057E67E..687 -> 0057E829
    void* const data = new_message_data();                                          // 0057E68D..6B2
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 0057E6B4..6C4
    recovered_event_object_004d7780(data, nullptr, 1, id);                          // 0057E6C9..6D2
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x149);                      // 0057E6D7..6E0
    recovered_event_integer_004d76b0(data, nullptr, 1, roll);                       // 0057E6E5..6EE
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(skill));      // 0057E6F3..6FC
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));         // 0057E701..706
    recovered_event_integer_004d76b0(data, nullptr, 4, take20);                     // 0057E70B..714
    const U result = skill + static_cast<I>(roll) >= dc ? 1u                         // 0057E719..723 (jl)
                   : take20 != 0 ? (dc >= 0x3c ? 5u : 3u) : 0u;                     // 0057E727..73E
    recovered_event_integer_004d76b0(data, nullptr, 5, result);                     // 0057E740..744
    recovered_event_integer_004d76b0(data, nullptr, 6, 0x149);                      // 0057E749..752
    kotor_fastcall_004ec700(as_creature(self), nullptr, data);    // 0057E757..761
    if (tool != nullptr &&                                                          // 0057E766..76C
        recovered_item_in_repository_00555f40(recovered_creature_inventory_004ef770(as_creature(self), nullptr, 1), nullptr,
                                              tool, 1) != 0) {                      // 0057E772..790
        if (static_cast<I>(at<U>(tool, 0x28c)) > 1) {                               // 0057E796..7A3 (jle)
            at<U>(tool, 0x28c) = at<U>(tool, 0x28c) - 1;                            // 0057E7A5..7A6
        } else if (recovered_creature_remove_item_00510ef0(as_creature(self), nullptr, tool, 1, 1, 0) != 0) { // 0057E7AE..7C5
            void* const player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4)); // 0057E7C7..7D9
            if (player != nullptr)
                recovered_item_update_notify_00569fc0(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                                      player, at<U>(tool, 0x14));   // 0057E7DD..7F6
            ai_event(at<U>(self, 4), at<U>(tool, 0x14), k_ai_event_item_destroyed_007441e0, nullptr); // 0057E7FB..824
        }
    }
    return done_reset();                                                            // 0057E829
}

// =========================================================================================================
namespace {
// 0057EDA1..EE67 / 0057F07E..F144: the placeable's "Opened" sound: placeables.2da (rules+B8 -> +2C) SoundAppType for
// appearance byte +230, then placeableobjsnds.2da (+70) "Opened"; a missing entry leaves "" in the sound string.
static void placeable_opened_sound(void* placeable, void* sound) {
    alignas(4) unsigned char column[8];
    frame_LogStringCtor_005e5a90(column, nullptr, k_sound_app_type_00749420);
    I app_type = 0;   // the original leaves it unwritten; GetINTEntry writes it whenever it reports success
    const U found = c2da_current_get_int_00413660(at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x2c), nullptr,
                                                  at<U8>(placeable, 0x230), column, &app_type);
    frame_LogStringDtor_005e5c20(column, nullptr);
    if (found == 0) return;
    frame_LogStringCtor_005e5a90(column, nullptr, k_opened_00749418);
    const U ok = c2da_current_get_string_00413270(at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x70), nullptr,
                                                  static_cast<U>(app_type), column, sound);
    frame_LogStringDtor_005e5c20(column, nullptr);
    if (ok == 0) assign_text_005e5140(sound, nullptr, k_empty_0073d71c);          // neg/sbb/inc: bl = (eax == 0)
}
// 0057EF01..EF8F / 0057F1FA..F288: wait length = max(open animation 0x138 length, sound length) in seconds; 0.5 s
// without a client placeable.
static float open_wait_seconds(void* placeable, void* client_placeable, void* sound) {
    if (client_placeable == nullptr) return flt(k_half_0x3f000000);
    float length = static_cast<float>(kotor_fastcall_0063c0e0(client_placeable, nullptr, 0x138) * k_ms_to_s_0073d708);
    menu_resources::Ref ref;
    async_ref_construct_string_00406d60(&ref, nullptr, sound);
    Vector p; copy_vector(&p, bp(placeable) + 0x90);
    const U played = kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &ref, p, 0.0f, 0x16,
                                                              0, 0, 0.0f, 0.0f);
    double ms = static_cast<double>(static_cast<I>(played));                         // fild dword
    if (static_cast<I>(played) < 0) ms += k_two_pow_32_0073d770;                     // jge / fadd 2^32
    const double seconds = ms * k_ms_to_s_0073d708;
    if (!(seconds < length || seconds == length || seconds != seconds))             // fcom; test ah,41h
        length = static_cast<float>(seconds);
    return length;
}
// 0057F01B..F079 / 0057F317..F377: no player: drop the open flag, the USER plays 0x275B, the sound plays in 3D
static void play_open_without_player(void* self, void* placeable, void* sound) {
    if (at<U>(placeable, 0x450) != 0) at<U>(placeable, 0x450) = 0;
    vcall<void>(self, 0x7c, 0x275bu);                                                // ECX = EDI (the acting object)
    menu_resources::Ref ref;
    async_ref_construct_string_00406d60(&ref, nullptr, sound);
    Vector p; copy_vector(&p, bp(placeable) + 0x90);
    kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &ref, p, 0.0f, 0x16, 0, 0, 0.0f, 0.0f);
}
}  // namespace

// =========================================================================================================
// 0057E8C0 CSWSObject::AIActionUseObject(node): node +38 target id. Placeables (9): open/close/use with sound and
// OnUsed (19h); items (6) with an inventory base item: open or close the item's inventory.
U __fastcall w5_action_use_object_0057e8c0(void* self, void*, void* node) {
    U dead = 0;                                                                     // ESI
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057E8E5..8FC
    if (vcall<U>(self, 0x94) == 1) return k_failed_00745dc4;                       // 0057E8FE..90B
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_failed_00745dc4; // 0057E911..91B
    if (dead == 1) return k_failed_00745dc4;                                        // 0057E921..924
    if (as_creature(self) != nullptr && at<U>(as_creature(self), 0xa88) != 0 &&     // 0057E92A..944
        at<U>(frame_R4_005ed690(client_app(), nullptr), 0xb4) != 0) return k_failed_00745dc4; // 0057E946..95C
    U id = at<U>(node, 0x38);                                                       // [esp+10]
    void* object = nullptr;                                                         // [esp+1C] (written by the callee)
    if (static_cast<U8>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, id,
                                                &object)) != k_found_0074666c) return k_failed_00745dc4; // 0057E962..98D
    void* target = as_object(object);                                               // 0057E993..99C (ESI; no null check)
    U target_dead = 0;                                                              // EBX
    if (as_creature(target) != nullptr) target_dead = recovered_creature_incapacitated_004ef890(as_creature(target), nullptr); // 0057E99E..9B9
    if (vcall<U>(target, 0x94) == 1) return k_failed_00745dc4;                     // 0057E9BB..9C8
    if (kotor_m15e_slot010_zero_0063e7f0(target, nullptr) == 1) return k_failed_00745dc4; // 0057E9CE..9D8
    if (target_dead == 1) return k_failed_00745dc4;                                 // 0057E9DE..9E1
    if (recovered_object_area_004cb120(target, nullptr) != nullptr && as_creature(self) != nullptr) { // 0057E9E7..9FF
        void* const creature = as_creature(self);                                   // 0057EA05..0C (EBX)
        w5_creature_activity_manager_004f87d0(creature, nullptr, 2);                // 0057EA0E..12
        recovered_set_activity_004f00a0(creature, nullptr, 4, 0, 0);                // 0057EA17..1F
        if (w5_creature_get_is_in_use_range_004f6000(creature, nullptr, id, 0.0f, 0) == 0) { // 0057EA24..36
            Vector pos; std::memset(&pos, 0, sizeof(pos));                          // [esp+3C]
            float range;                                                            // [esp+14]
            U one_a = 1;                                                            // [esp+28]
            w5_creature_get_use_range_004ee440(creature, nullptr, id, &pos, &range, 0); // 0057EA3C..63
            U one_b = 1;                                                            // [esp+18]
            const U use_pairs[26] = {3, addr(&id)};
            w5_add_action_to_front_body(self, 0x28, at<U16>(node, 0x6c), use_pairs); // 0057EA68..AB1
            U half = k_half_0x3f000000;                                             // [esp+8]
            const U wait_pairs[26] = {2, addr(&half)};
            w5_add_action_to_front_body(as_creature(self), 0x1e, 0xfffe, wait_pairs); // 0057EAB6..B0B
            const U face_pairs[26] = {3, addr(&id)};
            w5_add_action_to_front_body(self, 0x13, 0xfffe, face_pairs);            // 0057EB10..B50
            const U check_pairs[26] = {3, addr(&id), 1, addr(&one_b), 2, addr(&range), 2, addr(&range), 1, addr(&one_a)};
            w5_add_action_to_front_body(self, 0x11, at<U16>(node, 0x6c), check_pairs); // 0057EB55..BA3
            Vector zero; std::memset(&zero, 0, sizeof(zero));                       // [esp+48]
            w5_creature_add_move_to_point_action_to_front_004f8a50(creature, nullptr, at<U16>(node, 0x6c), &pos,
                at<U>(self, 0x8c), id, one_b, range, 0.0f, 0, 0, 0, 0, 0, 1, &zero, 0, 0, 0); // 0057EBA8..BFD
            return k_complete_00745dc0;                                             // 0057EC02
        }
    }
    vcall<void>(self, 0xc0, id);                                                    // 0057EC1B..24
    const U8 type = at<U8>(target, 8);                                              // 0057EC2A
    if (type == k_type_placeable_00746638) {                                        // 0057EC2D..33
        void* const placeable = as_placeable(target);                               // 0057EC39..40 (ESI)
        if (at<U>(placeable, 0x328) == 0) return k_failed_00745dc4;                 // 0057EC42..4A (not usable)
        if (at<U>(placeable, 0x278) != 0 && as_creature(self) != nullptr &&         // 0057EC50..67
            static_cast<I>(recovered_object_get_reputation_0057cb80(placeable, nullptr, at<U>(self, 4), 0)) < 0x5a && // 0057EC6D..7D
            at<U>(placeable, 0x23c) != at<U>(at<void*>(as_creature(self), 0xa74), 0x78)) { // 0057EC83..99
            void* const event = new_script_event(0x1a);                             // 0057EC9B..CC0
            ai_event(at<U>(self, 4), id, k_ai_event_script_007441dc, event);        // 0057ECC5..F5
            return k_failed_00745dc4;                                               // 0057ECFA
        }
        if (at<U>(placeable, 0x324) != 0) {                                         // 0057ED13..1B (has inventory)
            bool proceed = true;
            const bool locked_branch = at<U>(placeable, 0x260) != 0;                // 0057ED21..29 (branch fixed here;
                                                                                    //  EvaluateLock may clear +260)
            if (locked_branch && as_creature(self) != nullptr &&     // 0057ED21..38 (locked)
                w5_creature_evaluate_lock_004f0b20(as_creature(self), nullptr, placeable, 0) == 0) { // 0057ED3A..4D
                void* const data = new_message_data();                              // 0057ED4F..74
                recovered_event_object_004d7780(data, nullptr, 0, at<U>(placeable, 4)); // 0057ED76..86
                recovered_creature_feedback_004ede10(as_creature(self), nullptr, 0xd, data); // 0057ED8B..97
                proceed = false;                                                    // -> 0057F38D
            }
            if (proceed) {
                alignas(4) unsigned char sound[8];                                  // [esp+20] / [esp+34]
                frame_LogStringCtor_005e5a90(sound, nullptr, k_empty_0073d71c);     // 0057EDA1..AA / 0057F07E..87
                placeable_opened_sound(placeable, sound);                           // 0057EDAF..E67 / 0057F08C..144
                if (kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4)) == nullptr) { // 0057EE6C..82 / 0057F149..15D
                    play_open_without_player(self, placeable, sound);               // 0057F01B..079 / 0057F317..377
                } else {
                    if (at<U>(placeable, 0x338) != 0) {                             // 0057EE88..8E / 0057F163..16B (already open)
                        frame_LogStringDtor_005e5c20(sound, nullptr);               // 0057EFEE..000 / 0057F2EA..2FC
                        return k_complete_00745dc0;
                    }
                    // the locked branch reads GetClientPlaceable only when it animates; the unlocked one reads it first
                    void* client_placeable = !locked_branch ? w5_placeable_get_client_placeable_00585490(placeable, nullptr)
                                                                          : nullptr; // 0057F171..178
                    if (at<U>(placeable, 0x450) != 0) {                             // 0057EE94..9B / 0057F17A..184
                        w5_placeable_open_inventory_00587420(placeable, nullptr, at<U>(self, 4), 0); // 0057EE9D..EA3 / 0057F186..18C
                        at<U>(placeable, 0x450) = 0;                                // 0057EEA8 / 0057F191
                        at<U>(placeable, 0xd8) = k_one_0x3f800000;                  // 0057EEAE / 0057F19B
                    } else {
                        const U use_pairs[26] = {3, addr(&id)};
                        w5_add_action_to_front_body(self, 0x28, at<U16>(node, 0x6c), use_pairs); // 0057EEC1..EED / 0057F1AA..1ED
                        if (locked_branch)
                            client_placeable = w5_placeable_get_client_placeable_00585490(placeable, nullptr); // 0057EEF2..EF4
                        U wait = bits(open_wait_seconds(placeable, client_placeable, sound)); // [esp+14]
                        const U wait_pairs[26] = {2, addr(&wait)};
                        w5_add_action_to_front_body(self, 0x1e, at<U16>(node, 0x6c), wait_pairs); // 0057EF91..FD3 / 0057F28A..2CF
                        vcall<void>(placeable, 0x7c, 0x275bu);                      // 0057EFD8..FE1 / 0057F2D4..2DD
                        at<U>(placeable, 0x450) = 1;                                // 0057EFE4 / 0057F2E0
                        frame_LogStringDtor_005e5c20(sound, nullptr);               // 0057EFEE..000 / 0057F2EA..2FC
                        return k_complete_00745dc0;                                 //  (no OnUsed on this path)
                    }
                }
                frame_LogStringDtor_005e5c20(sound, nullptr);                       // 0057F37C..388
            }
        }
        void* const used = new_script_event(0x19);                                  // 0057F38D..3B2 (OnUsed)
        ai_event(at<U>(self, 4), id, k_ai_event_script_007441dc, used);             // 0057F3B7..3E6
        return k_complete_00745dc0;                                                 // 0057F3EB
    }
    if (type == k_type_item_00746635) {                                             // 0057F404..40A
        void* const item = as_item(target);                                         // 0057F40C..410 (ESI)
        if (at<U>(base_item(at<U>(item, 0xc)), 0x70) == 0) return k_complete_00745dc0; // 0057F413..42C
        void* const player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4)); // 0057F42E..43B
        if (player == nullptr) return k_complete_00745dc0;                          // 0057F440..442
        if (at<U>(at<void*>(player, 0x58), 0) != id)                                // 0057F444..44F
            w5_item_open_inventory_005561a0(item, nullptr, at<U>(self, 4));        // 0057F451..455
        else
            kotor_fastcall_0055d800(item, nullptr, at<U>(self, 4), 1);              // 0057F473..479
    }
    return k_complete_00745dc0;                                                     // 0057F45A / 0057F47E
}

// =========================================================================================================
// 0057B6A0 CSWSObject::AIActionGiveItem(node): node +38 item id, +3C receiver id, +40 split count (-1 = all),
// +44 status flag. Needs 2+ parameters.
U __fastcall w5_action_give_item_0057b6a0(void* self, void*, void* node) {
    void* item = nullptr;                                                           // [esp+C]
    if (at<U16>(node, 0x6e) < 2) return k_failed_00745dc4;                         // 0057B6A9..6C4 (jae)
    U item_id = at<U>(node, 0x38);                                                  // [esp+18]
    U split = at<U>(node, 0x40);                                                    // [esp+1C]
    U receiver_id = at<U>(node, 0x3c);                                              // [esp+44] (node slot)
    U status = at<U>(node, 0x44);                                                   // [esp+20]
    void* const receiver = game_object(receiver_id);                                // 0057B6DB..6F3 (EBP)
    if (receiver == nullptr) return k_failed_00745dc4;                              // 0057B6F5..704
    if (as_creature(self) != nullptr &&
        w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, receiver_id, 0.0f, 0) == 0) { // 0057B707..72F
        Vector pos; std::memset(&pos, 0, sizeof(pos));                              // [esp+28]
        U run = 0;                                                                  // [esp+14]
        float range;                                                                // [esp+10]
        U one = 1;                                                                  // [esp+24]
        w5_creature_get_use_range_004ee440(as_creature(self), nullptr, receiver_id, &pos, &range, 0); // 0057B735..767
        const double dx = static_cast<double>(at<float>(self, 0x90)) - pos.x;       // 0057B76C..772
        const double dy = static_cast<double>(at<float>(self, 0x94)) - pos.y;       // 0057B776..77C
        const double dz = static_cast<double>(at<float>(self, 0x98)) - pos.z;       // 0057B780..786
        const double d2 = (dz * dz + dy * dy) + dx * dx;                            // 0057B78A..798
        if (!(d2 < k_give_walk_sq_00745e5c || d2 == k_give_walk_sq_00745e5c || d2 != d2)) run = 1; // 0057B79A..7AD (test ah,41h)
        const U give_pairs[26] = {3, addr(&item_id), 3, addr(&receiver_id), 1, addr(&split), 1, addr(&status)};
        w5_add_action_to_front_body(self, 0x22, at<U16>(node, 0x6c), give_pairs);  // 0057B7B1..7FD
        const U point_pairs[26] = {2, addr(&pos.x), 2, addr(&pos.y), 2, addr(&pos.z)};
        w5_add_action_to_front_body(self, 0x31, at<U16>(node, 0x6c), point_pairs); // 0057B802..853
        const U check_pairs[26] = {3, addr(&receiver_id), 1, addr(&run), 2, addr(&range), 2, addr(&range), 1, addr(&one)};
        w5_add_action_to_front_body(self, 0x11, at<U16>(node, 0x6c), check_pairs); // 0057B858..8A7
        Vector zero; std::memset(&zero, 0, sizeof(zero));                           // [esp+34]
        w5_creature_add_move_to_point_action_to_front_004f8a50(as_creature(self), nullptr, at<U16>(node, 0x6c), &pos,
            at<U>(self, 0x8c), receiver_id, run, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0057B8AC..90B
        return k_complete_00745dc0;                                                 // 0057B910
    }
    item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);          // 0057B91E..932
    if (item == nullptr) return k_failed_00745dc4;                                  // 0057B936 -> 0057B6F9
    if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) == 0x17) { // 0057B93C..94A (credits)
        if (as_creature(self) != nullptr)
            kotor_fastcall_004f3ed0(as_creature(self), nullptr, at<U>(item, 0x28c), 1); // 0057B94C..96D
    } else if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) == 0x2a) { // 0057B974..984 (pazaak card)
        if (as_creature(self) != nullptr)
            (void)w5_creature_remove_pazaak_card_004efc30(as_creature(self), nullptr, item); // 0057B986..99F
    }
    U feedback = 1;                                                                 // 0057B9A8 (ESI)
    if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) == 0x17) feedback = 0; // 0057B9A4..9BB
    if (split != 0xffffffffu) {                                                     // 0057B9BD..9C4
        void* const part = kotor_fastcall_0055f280(item, nullptr, split);       // 0057B9C6..9CB
        if (part != nullptr) item = part;                                           // 0057B9D0..9D4
    }
    const U8 type = at<U8>(receiver, 8);                                            // 0057B9DE
    if (type == 5) {                                                                // 0057B9E1..9E4
        recovered_creature_acquire_item_005158e0(as_creature(receiver), nullptr, &item, at<U>(item, 0x268),
                                                 k_object_invalid, feedback);       // 0057B9E6..A02
        if (status != 0 && at<U>(as_creature(receiver), 0xa88) != 0)               // 0057BA07..A23
            kotor_fastcall_0062eeb0(frame_R4_005ed690(client_app(), nullptr), nullptr, 7, 0); // 0057BA29..A3D
    } else if (type == 9) {                                                         // 0057BA50..A53
        recovered_placeable_acquire_item_00584b10(as_placeable(receiver), nullptr, &item, at<U>(item, 0x268), feedback); // 0057BA55..A6C
    } else if (type == 6) {                                                         // 0057BA7F..A82
        if (at<U>(base_item(at<U>(as_item(receiver), 0xc)), 0x70) == 1)             // 0057BA84..AA1
            w5_item_acquire_item_0055dca0(as_item(receiver), nullptr, &item, at<U>(item, 0x268), feedback); // 0057BAA3..ABE
    }
    return k_complete_00745dc0;                                                     // 0057BA42 / 0057BA71 / 0057BAC3
}

// =========================================================================================================
// SHARED HELPERS: reconstructed callees with no provider in the c03-gui candidate. Other action groups are likely
// to need several of them (gui timing bar, skill data / voice chat broadcast, inventory open/close, item
// copy/split/acquire, chat strref / play-sound messages); the coordinator should dedupe by original address.
// ---------------------------------------------------------------------------------------------------------
// 0056D670 CSWSMessage::SendServerToPlayerChat_StrRef(player, speaker, minor, strref): RET 10h, EAX send result.
U __fastcall w5_message_send_chat_str_ref_0056d670(void* message, void*, U player, U speaker, U minor, U strref) {
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                  // 0056D674..67C
    kotor_fastcall_00578570(message, nullptr, speaker);                             // 0056D681..688
    kotor_fastcall_004d6340(message, nullptr, strref, 0x20);                        // 0056D68D..696
    void* data;                                                                     // [esp+8]
    U size;                                                                         // [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;     // 0056D69B..6B4 (EAX = 0)
    return kotor_fastcall_00567e60(message, nullptr, player, 9, minor, data, size); // 0056D6B7..6CF
}

// 00571760 CSWSMessage::SendServerToPlayerChatStrRefMessage(mode byte, speaker, strref): RET 0Ch.
// mode 9: to every player; 8 / 10: to players in the speaker's area within 1000 / 3 m; others: 0.
U __fastcall w5_message_send_chat_str_ref_message_00571760(void* message, void*, U mode, U speaker, U strref) {
    U result = 1;                                                                   // EDI
    void* const players = recovered_server_player_list_004aed90(server_app(), nullptr); // 00571763..77E (EBP)
    if (players == nullptr) return 0;                                               // 00571780..78A (review fix: original returns 0)
    const U8 kind = static_cast<U8>(mode);                                          // 0057178E..792 (movzx bl)
    if (kind == 9)                                                                  // 0057179A..79B
        return w5_message_send_chat_str_ref_0056d670(message, nullptr, k_all_players_0074c52c, speaker, mode, strref) & 1; // 005717AE..7C8
    if (kind != 8 && kind != 10) return 0;                                          // 005717A0..7AB
    void* position = nullptr;                                                       // [esp+3C] (mode slot reused)
    Vector origin; std::memset(&origin, 0, sizeof(origin));                         // [esp+20..28] = 0
    const float range = kind == 8 ? k_sound_range_00747444 : k_whisper_range_00747448; // 005717FA..80E ([esp+14])
    void* area = nullptr;                                                           // [esp+18] (unwritten when not found)
    void* object = nullptr;                                                         // [esp+10] (written by the callee)
    if (static_cast<U8>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, speaker,
                                                &object)) == k_found_0074666c &&
        as_object(object) != nullptr) {                                             // 00571812..843
        area = recovered_object_area_004cb120(as_object(object), nullptr);          // 00571845..85B
        copy_vector(&origin, bp(as_object(object)) + 0x90);                          // 00571855..87C
        position = at<void*>(at<void*>(players, 0), 0);                            // 00571873..880
    }
    while (position != nullptr) {                                                   // 0057188A..88C / 00571944..94A
        void* const player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, position); // 00571892..896 (ESI)
        if (kotor_fastcall_00561080(player, nullptr) != nullptr) {                  // 0057189D..8A6
            void* const creature = kotor_fastcall_00561080(player, nullptr);        // 005718AC..8AE
            const float dx = static_cast<float>(static_cast<double>(at<float>(creature, 0x90)) - origin.x); // 005718B3..8C4
            const float dy = static_cast<float>(static_cast<double>(at<float>(creature, 0x94)) - origin.y); // 005718C8..8CF
            const float dz = static_cast<float>(static_cast<double>(at<float>(creature, 0x98)) - origin.z); // 005718D3..8DA
            if (area == recovered_object_area_004cb120(kotor_fastcall_00561080(player, nullptr), nullptr)) { // 005718DE..8EE
                const double d2 = (static_cast<double>(dz) * dz + static_cast<double>(dy) * dy) + static_cast<double>(dx) * dx; // 005718F0..90A
                const double r2 = static_cast<double>(range) * range;              // 0057190C..910
                if (!(r2 < d2 || r2 != r2 || d2 != d2))                             // 00571914..91B (fcompp; test ah,1)
                    result &= w5_message_send_chat_str_ref_0056d670(message, nullptr, at<U>(player, 4), speaker, mode, strref); // 0057191D..935
            }
        }
        unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &position);  // 00571937..93F
    }
    return result;                                                                  // 00571950..959
}

// 0056D4A0 CSWSMessage::SendServerToPlayerAIActionPlaySound(player, speaker, CExoString* sound): RET 0Ch.
U __fastcall w5_message_send_ai_action_play_sound_0056d4a0(void* message, void*, U player, U speaker, void* sound) {
    kotor_fastcall_004d68a0(message, nullptr, async_string_length_005e5790(sound, nullptr) + 8, 0xffffffffu, 1); // 0056D4A3..4BA
    kotor_fastcall_00578570(message, nullptr, speaker);                             // 0056D4BF..4C6
    kotor_fastcall_004d6960(message, nullptr, sound, 0x20);                         // 0056D4CB..4D0
    void* data;                                                                     // [esp+8] (the pushed ECX slot)
    U size;                                                                         // [esp+18] (the sound argument slot)
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;     // 0056D4D5..4ED
    return kotor_fastcall_00567e60(message, nullptr, player, 9, 7, data, size);     // 0056D4F0..505
}

// 005EDBB0 (named CServerExoApp::BarkString; ECX is the client app): ECX = [ECX+4], tail jump 005F2C90. RET 10h.
void __fastcall kotor_fastcall_005edbb0(void* client, void*, U id, void* text, void* sound, U flag) {
    kotor_fastcall_005f2c90(at<void*>(client, 4), nullptr, id, text, sound, flag); // 005EDBB0..B3
}

// 005F2C90 CClientExoAppInternal::BarkString(id, text, sound, flag): RET 10h. A known client object barks itself
// (vtbl+C then vtbl+120); otherwise the in-game GUI (+40) shows an anonymous bubble.
void __fastcall kotor_fastcall_005f2c90(void* internal, void*, U id, void* text, void* sound, U flag) {
    void* const object = kotor_fastcall_005ed580(client_app(), nullptr, id);       // 005F2C90..CA1
    if (object != nullptr) {                                                        // 005F2CA6..CA8
        void* const client_object = vcall<void*>(object, 0xc);                      // 005F2CAA..CAE
        vcall<void>(client_object, 0x120, id, text, sound, flag);                   // 005F2CB1..CC5
        return;
    }
    object_projectile_bark_0062ab30(at<void*>(internal, 0x40), nullptr, k_object_invalid, text, sound, flag); // 005F2CD0..CE7
}

// 005D5E00 CExoSound::PlayOneShotSound(ref, group, delay, volume, fixed, pitch): RET 18h; null internal -> 0.
U __fastcall w5_sound_play_one_shot_005d5e00(void* sound, void*, void* ref, U group, U delay, U volume,
                                             float fixed_variance, float pitch_variance) {
    void* const internal = at<void*>(sound, 0);                                     // 005D5E00
    if (internal == nullptr) return 0;                                              // 005D5E02..E0D
    return w5_sound_internal_play_one_shot_005d7550(internal, nullptr, ref, group, delay, volume, fixed_variance,
                                                    pitch_variance);                // 005D5E06 (tail jump)
}

// 005D7550 CExoSoundInternal::PlayOneShotSound: a 2D CExoSoundSourceInternal (0xA0), delayed or played now.
U __fastcall w5_sound_internal_play_one_shot_005d7550(void* internal, void*, void* ref, U group, U delay, U volume,
                                                      float fixed_variance, float pitch_variance) {
    U result = 0;                                                                   // EBX
    if (c2da_equal_text_004060d0(ref, nullptr, k_empty_0073d71c) != 0) return 0;   // 005D756E..593
    void* const memory = live_operator_new(0xa0);                                   // 005D7596..5A0
    void* const source = memory != nullptr ? kotor_sound_rev3::InternalConstruct005daf50(memory, nullptr, internal)
                                           : nullptr;                               // 005D75A7..5BF
    kotor_sound_rev3_required::res_helper_set_ref_005d5e90(source, nullptr, ref, 0); // 005D75C1..5CE (no null check)
    at<U>(source, 0x28) = 1;                                                        // 005D75DA
    kotor_sound_rev3::InternalSetPriorityGroup005dbb20(source, nullptr, group);     // 005D75D3..5E1
    if (static_cast<U8>(volume) != 0)                                               // 005D75E6..5EC (test al,al)
        kotor_sound_path_required::source_set_volume_005db800(source, nullptr, volume, 0, k_one_0x3f800000); // 005D75EE..5F8
    if (!(fixed_variance == k_zero_0073d700))                                       // 005D75FD..60C (jnp: equal)
        (void)kotor_m15_source_internal_set_fixed_variance_005db980(source, nullptr, fixed_variance); // 005D760E..615
    if (!(pitch_variance == k_zero_0073d700))                                       // 005D761A..629
        kotor_sound_rev3::InternalSetPitchVariance005dba40(source, nullptr, pitch_variance); // 005D762B..632
    if (delay != 0) {                                                               // 005D7637..63F
        kotor_m15_source_internal_set_one_shot_delay_005db340(source, nullptr, delay); // 005D7641..642
        return result;                                                              // 005D7647 -> 005D7663 (EBX = 0)
    }
    result = kotor_sound_path_required::source_play_005db4d0(source, nullptr, 0, 0); // 005D7649..005D7652 mov ebx,eax
    if (at<U>(source, 0x2c) != 0) vcall<void>(source, 0, 1u);                       // 005D7654..661 (scalar deleting dtor)
    return result;                                                                  // 005D7663..676
}

// ---------------------------------------------------------------------------------------------------------
// 004EF480 CSWSCreature::StartGuiTimingBar(milliseconds, kind): RET 8; only for a player-controlled creature (+9D4 == 1).
void __fastcall kotor_fastcall_004ef480(void* creature, void*, U milliseconds, U kind) {
    if (at<U>(creature, 0x9d4) != 1) return;                                        // 004EF480..487
    void* const player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)); // 004EF489..4A5 (ESI)
    void* const message = recovered_server_sws_message_004aedb0(server_app(), nullptr); // 004EF49C..4A7
    if (player == nullptr || message == nullptr) return;                            // 004EF4AC..4B2
    recovered_action_cancel_message_0056b380(message, nullptr, player, 1, kind, milliseconds); // 004EF4B4..4C3
}
// 004EC700 CSWSCreature::BroadcastSkillData(CSWCCMessageData* data): RET 4; sends CC message 9 to the creature's
// player (when it has one), then destroys and frees the data.
void __fastcall kotor_fastcall_004ec700(void* creature, void*, void* data);   // single owner elsewhere (coordinator dedupe)
// 004F2520 CSWSCreature::BroadcastVoiceChat(voice): RET 4. Every player of the area (+190 id array, +194 count)
// whose creature is in the same area id within 30 m (squared <= 900) gets VoiceChat_Play(speaker, voice).
void __fastcall w5_creature_broadcast_voice_chat_004f2520(void* creature, void*, U voice) {
    if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return;        // 004F2520..52A
    void* const area = recovered_object_area_004cb120(creature, nullptr);           // 004F2532..539
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                    // 004F253B..54B / 004F25EE..5F4 (re-read)
        void* const player = kotor_fastcall_004ae910(server_app(), nullptr, at<const U*>(area, 0x190)[i]); // 004F2552..565
        if (player == nullptr) continue;                                            // 004F2567..569
        void* const other = kotor_fastcall_00561080(player, nullptr);               // 004F256F..576
        if (other == nullptr || at<U>(creature, 0x8c) != at<U>(other, 0x8c)) continue; // 004F2578..586
        const double dx = static_cast<double>(at<float>(other, 0x90)) - at<float>(creature, 0x90); // 004F2588..58E
        const double dy = static_cast<double>(at<float>(other, 0x94)) - at<float>(creature, 0x94); // 004F2594..59A
        const double dz = static_cast<double>(at<float>(other, 0x98)) - at<float>(creature, 0x98); // 004F25A0..5A6
        const double d2 = (dz * dz + dy * dy) + dx * dx;                            // 004F25AC..5BA
        if (!(d2 < k_voice_range_sq_007455bc || d2 == k_voice_range_sq_007455bc)) continue; // 004F25BC..5CD (test ah,41h; jp)
        kotor_fastcall_00569ec0(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr, player,
                                at<U>(creature, 4), voice);                         // 004F25CF..5E9
    }
}
// 004EADA0 CSWSItem::HasUpgrade(byte upgrade): RET 4. 0xFF -> 1; otherwise bit (upgrade & 31) of +294.
U __fastcall w5_item_has_upgrade_004eada0(void* item, void*, U upgrade) {
    const U8 n = static_cast<U8>(upgrade);                                         // 004EADA2 (CL)
    if (n == 0xff) return 1;                                                        // 004EADA6..A9
    return (at<U>(item, 0x294) & (1u << (n & 31u))) != 0 ? 1u : 0u;                // 004EADAB..C4 (shl edx,cl)
}
// 00569F40 CSWSMessage::SendServerToPlayerGuiContainerObject_Open(player, object id, has items): RET 0Ch.
void __fastcall w5_message_send_gui_container_open_00569f40(void* message, void*, void* player, U object_id, U has_items) {
    U size = 8;                                                                     // [esp+4] = 8 (overwritten below)
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                  // 00569F44..54
    kotor_fastcall_00578570(message, nullptr, object_id);                           // 00569F59..60
    kotor_fastcall_004d6340(message, nullptr, has_items, 0x20);                     // 00569F65..6E
    void* data;                                                                     // [esp+8]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return;       // 00569F73..8C
    kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x19, 1, data, size); // 00569F8F..A7
}
// 00587420 CSWSPlaceable::OpenInventory(opener, animate): RET 8.
void __fastcall w5_placeable_open_inventory_00587420(void* placeable, void*, U opener, U animate) {
    if (at<U>(placeable, 0x338) != 0 || at<U>(placeable, 0x324) == 0) return;      // 00587439..450
    if (at<U>(frame_R4_005ed690(client_app(), nullptr), 0xb4) != 0) {               // 00587456..46B
        vcall<void>(placeable, 0x7c, 0x275cu);                                      // 0058746D..476
        return;
    }
    void* const player = kotor_fastcall_004ae910(server_app(), nullptr, opener);   // 0058748D..49A
    if (player != nullptr)                                                          // 0058749F..4A1
        w5_message_send_gui_container_open_00569f40(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
            player, at<U>(placeable, 4), at<U>(at<void*>(placeable, 0x36c), 0x10) != 0 ? 1u : 0u); // 005874A3..4CA
    void* const event = new_script_event(0x16);                                     // 005874CF..4F5 (OnOpen)
    ai_event(opener, at<U>(placeable, 4), k_ai_event_script_007441dc, event);       // 005874FA..521
    if (animate != 0) vcall<void>(placeable, 0x7c, 0x275bu);                        // 00587526..537
    at<U>(placeable, 0x338) = 1;                                                    // 0058753A
}
// 00585490 CSWSPlaceable::GetClientPlaceable: client object vtbl+4C (tail call), 0 without a client object.
void* __fastcall w5_placeable_get_client_placeable_00585490(void* placeable, void*) {
    if (recovered_object_render_004cc2b0(placeable, nullptr) == nullptr) return nullptr; // 00585493..49A
    return vcall<void*>(recovered_object_render_004cc2b0(placeable, nullptr), 0x4c); // 0058549C..4A8
}
// 005561A0 CSWSItem::OpenInventory(opener): RET 4 (item +288 bit 1 = open).
void __fastcall w5_item_open_inventory_005561a0(void* item, void*, U opener) {
    if ((at<U8>(item, 0x288) & 2) != 0) return;                                     // 005561B9..1C1
    if (at<U>(base_item(at<U>(item, 0xc)), 0x70) == 0) return;                      // 005561C7..1DD
    void* const player = kotor_fastcall_004ae910(server_app(), nullptr, opener);   // 005561E3..1F1
    if (player != nullptr)                                                          // 005561F6..1F8
        w5_message_send_gui_container_open_00569f40(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
            player, at<U>(item, 0x14), at<U>(at<void*>(item, 0x26c), 0x10) != 0 ? 1u : 0u); // 005561FA..221
    void* const event = new_script_event(0x16);                                     // 00556226..24C (OnOpen)
    ai_event(opener, at<U>(item, 0x14), k_ai_event_script_007441dc, event);         // 00556251..278
    at<U>(item, 0x288) |= 2;                                                        // 0055627D
}
// 0055D800 CSWSItem::CloseInventory(closer, move_items): RET 8. With move_items, every contained item moves
// (last first) into the closer's creature repository. Replaces the W5 trap w5_item_close_inventory_0055d800.
void __fastcall kotor_fastcall_0055d800(void* item, void*, U closer, U move_items);   // single owner elsewhere (coordinator dedupe)
// 004F3ED0 CSWSCreature::RemoveGold(amount, feedback): RET 8. Party gold (party table +FC) for a party
// creature (+A88), else creature +9D0; floor 0, cap 999999999; feedback 95h with the amount removed.
void __fastcall kotor_fastcall_004f3ed0(void* creature, void*, U amount, U feedback);   // single owner elsewhere (coordinator dedupe)
// 004EFC30 CSWSCreature::RemovePazaakCard(item): RET 4. Party table card count [+120 + (card-1)*4] -= min(have,
// stack); returns the number removed.
U __fastcall w5_creature_remove_pazaak_card_004efc30(void* creature, void*, void* item) {
    (void)creature;
    const U index = static_cast<U>(at<U8>(item, 0x264)) - 1;                        // 004EFC3E..C45
    void* const party = lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); // 004EFC30..C46
    const I have = at<I>(party, 0x120 + index * 4);                                 // 004EFC4B
    I removed = at<I>(item, 0x28c);                                                 // 004EFC52
    if (have < removed) removed = have;                                             // 004EFC58..C5C (jge)
    at<I>(party, 0x120 + index * 4) = have - removed;                               // 004EFC5E..C60
    return static_cast<U>(removed);                                                 // 004EFC68
}
// 0055F280 CSWSItem::SplitItem(count): RET 4. 0 < count < stack: a new item (0x29C, id invalid) copied from this
// one takes `count` from the stack.
void* __fastcall kotor_fastcall_0055f280(void* item, void*, U count);   // single owner elsewhere (coordinator dedupe)
// 0055D950 CSWSItem::CopyItem(source): RET 4. Refuses (0) a null source or one whose own inventory holds items.
U __fastcall kotor_fastcall_0055d950(void* item, void*, void* source);   // single owner elsewhere (coordinator dedupe)
// 0055DCA0 CSWSItem::AcquireItem(CSWSItem** cell, container, flag): RET 0Ch. Moves the item into this container
// item's repository (+26C) after removing it from its possessor.
U __fastcall w5_item_acquire_item_0055dca0(void* item, void*, void* item_cell, U container, U flag) {
    (void)recovered_object_array_004aed70(server_app(), nullptr);                   // 0055DCA1..CB3 (result unused)
    void** const cell = static_cast<void**>(item_cell);                             // EDI
    U possessor = at<U>(*cell, 0x268);                                              // 0055DCBC..CC4 (ESI)
    void* holder = nullptr;                                                         // EBX
    if (possessor != container) {                                                   // 0055DCCA..CCC
        holder = recovered_item_lookup_004ae760(server_app(), nullptr, possessor);  // 0055DCCE..CDD
        if (holder == nullptr || at<U>(holder, 0x268) != container) return 0;       // 0055DCDF..CED
        possessor = at<U>(*cell, 0x268);                                            // 0055DCF3..CF5
    }
    void* const repository = at<void*>(item, 0x26c);                                // 0055DD04 (EBP)
    void* const previous = game_object(possessor);                                  // 0055DCFB..D10 (ESI)
    if (previous != nullptr) {                                                      // 0055DD12..D14
        const U8 type = at<U8>(previous, 8);                                        // 0055DD1A
        if (type == 5)                                                              // 0055DD1D..D36
            recovered_creature_remove_item_00510ef0(as_creature(previous), nullptr, *cell, 1, flag, 0);
        else if (type == 9)                                                         // 0055DD88..D98
            (void)recovered_placeable_remove_item_00584ac0(as_placeable(previous), nullptr, *cell);
        else if (type == 6 && at<void*>(as_item(previous), 0x26c) != nullptr)       // 0055DD9F..DC4
            recovered_remove_item_00555fd0(at<void*>(as_item(previous), 0x26c), nullptr, *cell);
    } else if (holder != nullptr && at<U>(base_item(at<U>(*cell, 0xc)), 0x70) != 0 && // 0055DDCE..DEE
               at<U>(base_item(at<U>(holder, 0xc)), 0x70) != 0) {                  // 0055DDF4..E0B
        return 0;                                                                   // 0055DE11
    }
    const U result = recovered_inventory_insert_0055d330(repository, nullptr, cell, 1, 0, 1); // 0055DD3B..D49
    if (result != 1) return result;                                                 // 0055DD4B..D4E
    if (previous == nullptr) {                                                      // 0055DD50..D52
        if (*cell == nullptr) return result;                                        // 0055DD54..D58
        recovered_item_remove_area_005538a0(*cell, nullptr);                        // 0055DD5A
    }
    if (*cell != nullptr)                                                           // 0055DD5F..D63
        recovered_item_owner_00553210(*cell, nullptr, at<U>(item, 0x14), 1, flag, k_object_invalid); // 0055DD65..D79
    return result;
}
}  // extern "C"
