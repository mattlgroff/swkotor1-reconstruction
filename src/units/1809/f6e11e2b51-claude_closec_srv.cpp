// closeC-srv: bodies required by CSWSMessage::HandlePlayerToServerInputMessage 005254C0 (srccloseC\changed-dispatcher)
// and HandlePlayerToServerInputWalkToWaypoint 005235B0. Original thiscall members are exported as
// extern "C" __fastcall(self, edx-unused, stack args...); decorated @N = 8 + 4 * stack args = original RET + 8.
//
//   005B56E0 CSWSCreature::DoCombatStep(step, time, target)            RET 0Ch  @kotor_fastcall_005b56e0@20
//   0056EBE0 CSWSMessage::SendServerToPlayerExamineGui_CreatureData     RET 8    @kotor_fastcall_0056ebe0@16
//   0056EE40 CSWSMessage::SendServerToPlayerExamineGui_ItemData         RET 8    @kotor_fastcall_0056ee40@16
//   0056F0A0 CSWSMessage::SendServerToPlayerExamineGui_PlaceableData    RET 8    @kotor_fastcall_0056f0a0@16
//   0056F370 CSWSMessage::SendServerToPlayerExamineGui_DoorData         RET 8    @kotor_fastcall_0056f370@16
//   00570230 CSWSMessage::SendServerToPlayerDebugInfo_Creature          RET 8    @kotor_fastcall_00570230@16
//   0056A7F0 CSWSMessage::SendServerToPlayerDebugInfo_Item              RET 8    @kotor_fastcall_0056a7f0@16
//   0056A8C0 CSWSMessage::SendServerToPlayerDebugInfo_Placeable         RET 8    @kotor_fastcall_0056a8c0@16
//   0056AA30 CSWSMessage::SendServerToPlayerDebugInfo_Area              RET 8    @kotor_fastcall_0056aa30@16
//   (0056CAC0 Module_ExportReply is provided by closeB claude_closeB_save.cpp)
//   0056AC20 CSWSMessage::HasValidString(CExoLocString*, gender)        RET 8    @kotor_fastcall_0056ac20@16
//   004FD1E0 CSWSCreature::Rest(flag)                                   RET 4    @kotor_fastcall_004fd1e0@12
//   004FCE90 CSWSCreature::AddCounterSpellActions(target)               RET 4    @kotor_fastcall_004fce90@12
//   004F0A60 CSWSCreature::SplitItem(item, count)                       RET 8    @kotor_fastcall_004f0a60@16
//   004F0990 CSWSCreature::MergeItem(item, into)                        RET 8    @kotor_fastcall_004f0990@16
//
// Memory: live_operator_new only (original 006FA7E6). x87: double intermediates (53-bit PC), float where the
// original stores a dword; fpatan/fsin/fcos/asin through non-inlined x87 helpers (the asin helper's CALL lives in
// its own __declspec(noinline) function, per the inline-asm clobber rule).
#include <cstdint>
#include <cstring>

namespace closec_srv {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
struct Vector { float x, y, z; };
struct ResRef16 { U d[4]; };
template<class T> inline T& at(void* p, U off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off); }
template<class R, class... A> inline R vcall(void* self, U slot, A... args) {
    using F = R(__fastcall*)(void*, void*, A...);
    return reinterpret_cast<F>(at<void*>(*static_cast<void**>(self), slot))(self, nullptr, args...);
}
inline U bits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
inline float flt(U u) { float f; std::memcpy(&f, &u, 4); return f; }
}
using namespace closec_srv;

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* self, void*); }   // CExoString() [0603]
extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                     // [007A3A28] rules
void* __cdecl live_operator_new(std::uint32_t size);
// server app / objects
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U id);                       // GetGameObject
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);                       // GetCreatureByGameObjectID
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);                // GetItemByGameObjectID
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);           // GetPlaceableByGameObjectID
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                       // GetAreaByGameObjectID [0530]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                       // GetClientObjectByObjectId
void* __fastcall recovered_object_array_004aed70(void* server, void*);                     // GetObjectArray
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                 // GetServerAIMaster
void* __fastcall server_get_module_004ae6b0(void* server, void*);                          // GetModule
U __fastcall kotor_fastcall_004ae810(void* server, void*, U player_id);                    // GetPlayerLanguage
U __fastcall kotor_fastcall_004ae820(void* server, void*);                                 // GetModuleLanguage
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, U id, void* out);           // CGameObjectArray::GetGameObject (AL)
void __fastcall kotor_fastcall_005d5170(void* net, void*, U player_id);                    // CNetLayer::GetPlayerInfo
void* __fastcall kotor_fastcall_00561080(void* player, void*);                             // CSWSPlayer::GetGameObject
void* __fastcall recovered_last_update_object_00561820(void* player, void*, U id);         // GetLastUpdateObject
U __fastcall kotor_fastcall_00561870(void* player, void*);                                 // PackCreatureIntoMessage
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                 // CFactionManager::GetFaction
// message writers (W2)
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);           // CreateWriteMessage
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                       // WriteOBJECTIDServer
void __fastcall kotor_fastcall_005785c0(void* message, void*, void* loc, U gender);        // WriteCExoLocStringServer
void __fastcall kotor_fastcall_004d68e0(void* message, void*, U value);                    // WriteBOOL
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);            // WriteBYTE
void __fastcall kotor_fastcall_004d6300(void* message, void*, U value, U bits);            // WriteWORD
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);            // WriteDWORD
void __fastcall kotor_fastcall_004d6920(void* message, void*, U value_bits, U scale, U bits); // WriteFLOAT
void __fastcall kotor_fastcall_004d6960(void* message, void*, void* string, U bits);       // WriteCExoString
void __fastcall kotor_fastcall_004d6380(void* message, void*, ResRef16 ref, U bits);       // WriteCResRef
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);          // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
// strings
void __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);         // CExoString(char const*)
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                           // ~CExoString
int __fastcall async_string_length_005e5790(void* string, void*);                          // CExoString::GetLength
void* __fastcall exo_locstring_construct_005e9e10(void* self, void*);                      // CExoLocString()
void __fastcall exo_locstring_destroy_005ea000(void* self, void*);                         // ~CExoLocString
void* __fastcall recovered_localized_assign_005e9e70(void* self, void*, const void* src);  // CExoLocString::operator=
void* __fastcall kotor_fastcall_005e9f80(void* self, void*, const void* src);              // CExoLocString(CExoLocString const&)
U __fastcall kotor_fastcall_005ea240(void* loc, void*, U language, void* out, U gender);   // GetStringLoc RET 0Ch
U __fastcall kotor_fastcall_005e9eb0(void* loc, void*, U index, void* language, void* out, void* gender); // GetString RET 10h
// creature / item
void* __fastcall recovered_object_area_004cb120(void* object, void*);                      // GetArea
U __fastcall recovered_object_damage_level_004cb020(void* object, void*);                  // GetDamageLevel
U __fastcall recovered_object_ai_state_reputation_0057cd90(void* object, void*, U id);     // GetAIStateReputation
U __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                   // CSWSCreatureStats::GetLevel (AL)
U __fastcall recovered_creature_can_use_item_0051b440(void* creature, void*, void* item, U flag); // CanUseItem
U __fastcall kotor_fastcall_00554080(void* item, void*);                                   // GetMinEquipLevel (AL)
U __fastcall recovered_item_weight_005562a0(void* item, void*);                            // GetWeight
void* __fastcall kotor_fastcall_00553990(void* item, void*, U index);                      // GetPassiveProperty
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, U index);      // GetActiveProperty
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                     // CSWSCreature::GetFaction
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U flag);     // GetItemRepository
U __fastcall recovered_creature_armor_class_004ed1d0(void* creature, void*);               // GetArmorClass
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U slot);        // CSWInventory::GetItemInSlot
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);            // ItemListGetItem
U __fastcall recovered_inventory_has_item_005a4950(void* inventory, void*, void* item);    // GetItemInInventory
U __fastcall recovered_item_stack_merge_00553f90(void* item, void*, void* other);          // CSWSItem::MergeItem
U __fastcall recovered_creature_remove_item_00510ef0(void* creature, void*, void* item, U a, U b, U c); // RemoveItem
U __fastcall recovered_object_equip_004f0420(void* creature, void*, void* item, U slot, U, U, U, U, U); // AddEquipItemActions
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // GetBaseItem
void* __fastcall kotor_fastcall_0055f280(void* item, void*, I count);                      // CSWSItem::SplitItem
unsigned __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, unsigned a, unsigned b, unsigned c); // AddItem
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag); // AddToArea
// actions / activity
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data); // SendFeedbackMessage
U __fastcall kotor_fastcall_004f2de0(void* creature, void*, float range, U target, U exclude); // GetNearestEnemy
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);  // ClearAllActions
void __fastcall kotor_fastcall_004eb1b0(void* creature, void*, U mask);                    // ClearActivities
void __fastcall recovered_set_activity_004f00a0(void* creature, void*, U mask, U on, U extra); // SetActivity
void __fastcall recovered_creature_compute_ai_state_004f1460(void* creature, void*);       // ComputeAIState
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                   // CScriptEvent()
void __fastcall recovered_event_integer_004d76b0(void* event, void*, U index, U value);    // SetInteger
U __fastcall recovered_event_insert_004afdb0(void* ai, void*, U day, U tick, U source, U target, U kind, void* data); // AddEventAbsoluteTime
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);
unsigned __fastcall recovered_object_add_action_to_front_004ccb00(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);
U __fastcall w5_creature_add_move_to_point_action_to_front_004f8a50(void* creature, void*, U group, const Vector* point, U area,
    U target, U a4, float a5, float a6, U a7, U a8, U a9, U a10, U a11, U a12, const Vector* a13, U a14, U a15, U a16); // RET 44h
unsigned __fastcall gui_load_slot_zero_0063e7f0(void*, void*);                             // return_zero 0063E7F0
// combat step
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);               // SetRoundPaused
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);               // SetPauseTimer
double __fastcall kotor_fastcall_004f1310(void* creature, void*, U target, U flag);        // DesiredAttackRange (ST0)
U __fastcall kotor_fastcall_004ebb30(void* creature, void*);                               // RangeWeaponEquipped
int __cdecl crt_rand_006fc447();                                                           // rand
void __cdecl pointto_fasin_006fcff0();                                                     // fasin ST0->ST0
double __cdecl vector_magnitude_004ab100(const Vector* v);                                 // ST0
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float s);
Vector* __cdecl vector_sum_00423b90(Vector* out, const Vector* a, const Vector* b);
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v);
int __fastcall w5_area_test_direct_line_004bcb70(void* area, void*, const Vector* from, const Vector* to,
    const float* radius, float height_band, U unused5, void* walk_hit);
U __fastcall w5_creature_update_subareas_on_move_to_0051b7b0(void* creature, void*, const Vector* from, const Vector* to,
    U use_crossed, void* crossed, U trigger_check);
void __fastcall recovered_object_position_004cd1a0(void* object, void*, const Vector* p, U a, U b, U c); // SetPosition
}

namespace {
void* server() { return at<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }   // [[007A39FC]+8], reloaded per use
void* objects() { return recovered_object_array_004aed70(server(), nullptr); }
constexpr U k_invalid = 0x7f000000u;                       // .rdata 00746644
struct ExoString {                                         // CExoString local (8 bytes) with EH cleanup
    alignas(4) unsigned char s[8];
    ExoString() { kotor_movie::string_element_construct_005b3190(s, nullptr); }   // 005B3190
    explicit ExoString(const char* t) { frame_LogStringCtor_005e5a90(s, nullptr, t); }
    ~ExoString() { frame_LogStringDtor_005e5c20(s, nullptr); }
};
struct ExoLocString {                                      // CExoLocString local with EH cleanup
    alignas(4) unsigned char s[0x10];
    ExoLocString() { exo_locstring_construct_005e9e10(s, nullptr); }
    explicit ExoLocString(const void* src) { kotor_fastcall_005e9f80(s, nullptr, src); }
    ~ExoLocString() { exo_locstring_destroy_005ea000(s, nullptr); }
};
// lane x87-trig (2026-10-24): the former x87_atan2 / x87_asin / x87_cos / x87_sin rounded every transcendental
// result to double. In the original (005B5928..005B59E6) the fpatan and asin results stay on the x87 stack through
// fadd st,st / fsubp|faddp / fcom / fadd|fsubr, and fcos/fsin store straight to dwords. See x87trig_005b5930.
}  // namespace
namespace x87trig {
#pragma pack(push, 1)
struct X80 { unsigned char bytes[10]; };   // FSTP TBYTE image (64-bit significand, sign/exponent)
#pragma pack(pop)
const float kTwoPi_00740c88 = 6.2831855f;  // bits 40C90FDB
const float kZero_0073d700 = 0.0f;
}
extern "C" {
// 005B5928..005B5932 / 005B5995..005B599F: fld [dir.y]; fld [dir.x]; fpatan; fst dword [esp+54h].
__declspec(noinline) void __cdecl x87trig_005b5930(const float* dir, x87trig::X80* angle, float* slot54) {
    __asm {
        mov eax, dir
        fld dword ptr [eax + 4]                // 005B5928 [esp+30h]
        fld dword ptr [eax]                    // 005B592C [esp+2Ch]
        fpatan                                 // 005B5930
        mov eax, slot54
        fst dword ptr [eax]                    // 005B5932
        mov eax, angle
        fstp tbyte ptr [eax]
    }
}
// 005B5940 / 005B59AD: call 006FCFF0 (asin, ST0 -> ST0; the selected provider pointto_fasin_006fcff0). The operand
// (005B5936 fld [0073E320]; 005B593C fdiv dword [esp+10h]) is a PC-53 quotient, carried exactly by the double.
// The CALL sits alone in this noinline function; nothing is kept in a register across it.
__declspec(noinline) void __cdecl x87trig_asin_006fcff0(double operand, x87trig::X80* result) {
    __asm {
        fld qword ptr operand
        call pointto_fasin_006fcff0
        mov eax, result
        fstp tbyte ptr [eax]
    }
}
// 005B5945..005B597B (mode 4, 0x272A): a = A - 2B; if (a < 0) a += 2pi; ux = cos(a), uy = sin(a) (dwords).
__declspec(noinline) void __cdecl x87trig_005b5966(const x87trig::X80* fpatan_a, const x87trig::X80* asin_b, float* slot54,
                                                   float* ux, float* uy) {
    __asm {
        mov eax, fpatan_a
        fld tbyte ptr [eax]
        mov eax, asin_b
        fld tbyte ptr [eax]
        mov eax, slot54
        fst dword ptr [eax]                    // 005B5945
        fadd st, st                            // 005B5949
        fsubp st(1), st                        // 005B594B DE E9
        fcom dword ptr x87trig::kZero_0073d700 // 005B594D
        fnstsw ax                              // 005B5953
        test ah, 5                             // 005B5955
        jp x87trig_skip4                       // 005B5958
        fadd dword ptr x87trig::kTwoPi_00740c88  // 005B595A
    x87trig_skip4:
        fld st(0)                              // 005B5960
        fcos                                   // 005B5966
        mov eax, ux
        fstp dword ptr [eax]                   // 005B5973 [esp+2Ch] (unit.x)
        fsin                                   // 005B5977
        mov eax, uy
        fstp dword ptr [eax]                   // 005B5979 [esp+30h] (unit.y)
    }
}
// 005B59B2..005B59E8 (mode 5, 0x272B): a = A + 2B; if (a > 2pi) a = 2pi - a (original quirk); cos/sin to dwords.
__declspec(noinline) void __cdecl x87trig_005b59d3(const x87trig::X80* fpatan_a, const x87trig::X80* asin_b, float* slot54,
                                                   float* ux, float* uy) {
    __asm {
        mov eax, fpatan_a
        fld tbyte ptr [eax]
        mov eax, asin_b
        fld tbyte ptr [eax]
        mov eax, slot54
        fst dword ptr [eax]                    // 005B59B2
        fadd st, st                            // 005B59B6
        faddp st(1), st                        // 005B59B8 DE C1
        fcom dword ptr x87trig::kTwoPi_00740c88  // 005B59BA
        fnstsw ax                              // 005B59C0
        test ah, 41h                           // 005B59C2
        jne x87trig_skip5                      // 005B59C5
        fsubr dword ptr x87trig::kTwoPi_00740c88 // 005B59C7
    x87trig_skip5:
        fld st(0)                              // 005B59CD
        fcos                                   // 005B59D3
        mov eax, ux
        fstp dword ptr [eax]                   // 005B59E0
        fsin                                   // 005B59E4
        mov eax, uy
        fstp dword ptr [eax]                   // 005B59E6
    }
}
}  // extern "C"
namespace {
U send_reply(void* message, U player_id, U major, U minor) {
    void* data; U size;
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;
    return kotor_fastcall_00567e60(message, nullptr, player_id, major, minor, data, size);
}
}

extern "C" {

// ======================================================================= 0056AC20 HasValidString
U __fastcall kotor_fastcall_0056ac20(void* message, void*, void* loc, U gender) {
    ExoString text;                                                              // [esp+14] 005B3190
    void* const app = server();                                                  // 0056AC44..4C
    if (app == nullptr) return 0;                                                // 0056AC56..7A
    U language = kotor_fastcall_004ae810(app, nullptr, at<U>(message, 0x4c));    // 0056AC7D..89 [esp+10]
    const U module_language = kotor_fastcall_004ae820(server(), nullptr);        // 0056AC8D..95 EDI
    U gender_slot = gender;                                                      // argument slot [esp+30]
    bool found = kotor_fastcall_005ea240(loc, nullptr, language, text.s, gender) != 0;          // 0056ACA6..BB
    if (!found && language != 0) found = kotor_fastcall_005ea240(loc, nullptr, 0, text.s, gender) != 0; // 0056ACBD..D6
    if (!found && module_language != language && module_language != 0)
        found = kotor_fastcall_005ea240(loc, nullptr, module_language, text.s, gender) != 0;   // 0056ACD8..F2
    if (!found) found = kotor_fastcall_005e9eb0(loc, nullptr, 0, &language, text.s, &gender_slot) != 0; // 0056ACF4..0E
    if (!found) {
        if (at<I>(loc, 4) != -1) return 1;                                       // 0056AD10..27 strref present
    }
    return async_string_length_005e5790(text.s, nullptr) > 0 ? 1 : 0;           // 0056AD29..43 (signed jg)
}

// 0056CAC0 Module_ExportReply: owned by lane closeB (claude_closeB_save.cpp, @kotor_fastcall_0056cac0@12); not duplicated here.

// ======================================================================= 0056F370 / 0056F0A0 ExamineGui door / placeable
U __fastcall kotor_fastcall_0056f370(void* message, void*, void* player, U id) {
    const U player_id = at<U>(player, 4);                                        // EBX
    kotor_fastcall_004d68a0(message, nullptr, 0x80, player_id, 1);               // 0056F37C..86
    void* found = nullptr;                                                       // argument slot [esp+14]
    if (static_cast<U8>(kotor_fastcall_004d8230(objects(), nullptr, id, &found)) == 0) {   // 0056F38B..B8
        void* door = vcall<void*>(found, 0x18);                                  // 0056F3BA..C1
        if (door != nullptr) {
            kotor_fastcall_00578570(message, nullptr, at<U>(door, 4));           // 0056F3CA..D0
            kotor_fastcall_005785c0(message, nullptr, static_cast<unsigned char*>(door) + 0x3a4, 0); // 0056F3D5..E0
        }
    }
    return send_reply(message, player_id, 0x1b, 6);                              // 0056F3E6..14
}
U __fastcall kotor_fastcall_0056f0a0(void* message, void*, void* player, U id) {
    const U player_id = at<U>(player, 4);
    kotor_fastcall_004d68a0(message, nullptr, 0x80, player_id, 1);               // 0056F0AC..B6
    void* found = nullptr;
    if (static_cast<U8>(kotor_fastcall_004d8230(objects(), nullptr, id, &found)) == 0) {   // 0056F0BB..E8
        void* placeable = vcall<void*>(found, 0x48);                             // 0056F0EA..F1
        if (placeable != nullptr) {
            kotor_fastcall_00578570(message, nullptr, at<U>(placeable, 4));      // 0056F0FA..00
            kotor_fastcall_005785c0(message, nullptr, static_cast<unsigned char*>(placeable) + 0x234, 0); // 0056F105..10
        }
    }
    return send_reply(message, player_id, 0x1b, 4);                              // 0056F116..44
}

// ======================================================================= 0056EBE0 ExamineGui_CreatureData
U __fastcall kotor_fastcall_0056ebe0(void* message, void*, void* player, U id) {
    const U player_id = at<U>(player, 4);                                        // EBX, [esp+18]
    kotor_fastcall_005d5170(vcall<void*>(server(), 0x10), nullptr, player_id);   // 0056EC0A..19 GetNetLayer()->GetPlayerInfo (unused)
    void* found = nullptr;                                                       // [esp+10]
    void* const array = objects();                                               // 0056EC1E..37 ESI
    ExoLocString name;                                                           // [esp+1C] 005E9E10
    if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, id, &found)) != 0) return 0; // 0056EC3E..5D
    void* creature = vcall<void*>(found, 0x30);                                  // 0056EC63..6C
    if (creature == nullptr) return 0;                                           // 0056EC70..85
    recovered_localized_assign_005e9e70(name.s, nullptr, static_cast<unsigned char*>(at<void*>(creature, 0xa74)) + 0x58); // 0056EC8A..94
    (void)recovered_last_update_object_00561820(player, nullptr, id);            // 0056EC99..A8 (result unused)
    kotor_fastcall_004d68a0(message, nullptr, 8, player_id, 1);                  // 0056ECAD..B7 (argument slot reused: 8)
    kotor_fastcall_00578570(message, nullptr, at<U>(creature, 4));               // 0056ECBC..C2
    kotor_fastcall_005785c0(message, nullptr, name.s, 0);                        // 0056ECC7..D0
    kotor_fastcall_004d68e0(message, nullptr, 1);                                // 0056ECD5..D9
    kotor_fastcall_005785c0(message, nullptr, vcall<void*>(creature, 0x8c), 0);  // 0056ECDE..ED
    kotor_fastcall_005785c0(message, nullptr, vcall<void*>(creature, 0x90), 0);  // 0056ECF2..01
    const U portrait = vcall<U>(creature, 0xd0);                                 // 0056ED06..14 EBX
    kotor_fastcall_004d6300(message, nullptr, portrait, 0x10);                   // 0056ED10..17
    if (static_cast<U16>(portrait) >= 0xfffe) {                                  // 0056ED1C..21 (unsigned)
        ResRef16 ref; std::memcpy(&ref, static_cast<unsigned char*>(at<void*>(creature, 0xa74)) + 0x16e, 16); // 0056ED23..48
        kotor_fastcall_004d6380(message, nullptr, ref, 0x10);                    // 0056ED4D
    }
    kotor_fastcall_004d62c0(message, nullptr, recovered_object_damage_level_004cb020(creature, nullptr), 8); // 0056ED52..5E
    void* viewer = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30); // 0056ED63..74
    kotor_fastcall_004d62c0(message, nullptr,
        recovered_object_ai_state_reputation_0057cd90(viewer, nullptr, at<U>(creature, 4)), 8); // 0056ED77..81
    void* stats = at<void*>(creature, 0xa74);                                    // 0056ED86 ESI
    U level_bits;                                                                // argument slot [esp+38]
    if (at<U>(stats, 0x6c) != 0) {                                               // 0056ED8C..91
        const I level = static_cast<I>(recovered_stats_level_005a5fd0(stats, nullptr, 0) & 0xff); // movzx al
        level_bits = bits(static_cast<float>(level));                            // 0056EDA3..A7 fild / fstp dword
    } else {
        level_bits = at<U>(stats, 0x84);                                         // 0056EDAD..B3
    }
    kotor_fastcall_004d6920(message, nullptr, level_bits, 0x3f800000u, 0x20);    // 0056EDB7..C5
    return send_reply(message, player_id, 0x1b, 3);                              // 0056EDCA..21 (string destroyed after)
}

// ======================================================================= 0056EE40 ExamineGui_ItemData
U __fastcall kotor_fastcall_0056ee40(void* message, void*, void* player, U id) {
    const U player_id = at<U>(player, 4);                                        // EBX, [esp+14]
    kotor_fastcall_004d68a0(message, nullptr, 0x80, player_id, 1);               // 0056EE63..71
    void* item = recovered_item_lookup_004ae760(server(), nullptr, id);          // 0056EE76..88 EBP
    if (item == nullptr) {
        kotor_fastcall_00578570(message, nullptr, k_invalid);                    // 0056F045..4E
        return send_reply(message, player_id, 0x1b, 2);                          // 0056F053..79
    }
    void* creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30); // 0056EE92..A0 EBX
    kotor_fastcall_00578570(message, nullptr, at<U>(item, 0x14));               // 0056EEA2..A8
    kotor_fastcall_004d68e0(message, nullptr, 1);                                // 0056EEAD..B1
    kotor_fastcall_004d68e0(message, nullptr,
        creature != nullptr ? recovered_creature_can_use_item_0051b440(creature, nullptr, item, 0) : 0); // 0056EEB6..C8
    {
        ExoLocString name(static_cast<unsigned char*>(item) + 0x270);            // 0056EECD..D8 [esp+20]
        if (kotor_fastcall_0056ac20(message, nullptr, name.s, 0) == 0)           // 0056EEDD..F0
            recovered_localized_assign_005e9e70(name.s, nullptr, static_cast<unsigned char*>(item) + 0x278); // 0056EEFA..05
        kotor_fastcall_005785c0(message, nullptr, name.s, 0);                    // 0056EF0A..12
        U usable = 0;                                                            // EDI
        if (creature != nullptr) {                                               // 0056EF17..37
            const U8 level = static_cast<U8>(recovered_stats_level_005a5fd0(at<void*>(creature, 0xa74), nullptr, 0));
            const U8 needed = static_cast<U8>(kotor_fastcall_00554080(item, nullptr));
            if (level >= needed) usable = 1;
        }
        kotor_fastcall_004d68e0(message, nullptr, usable);                       // 0056EF3C..3F
        if (usable == 0) kotor_fastcall_004d62c0(message, nullptr, kotor_fastcall_00554080(item, nullptr), 8); // 0056EF44..54
        kotor_fastcall_004d6340(message, nullptr, at<U>(item, 0x258), 0x20);    // 0056EF59..64
        kotor_fastcall_004d6340(message, nullptr, recovered_item_weight_005562a0(item, nullptr), 0x20); // 0056EF69..75
        const I passive = at<I>(item, 0x24c);                                    // 0056EF7A EDI, [esp+10]
        const I active = at<I>(item, 0x248);                                     // 0056EF80 argument slot [esp+38]
        for (I i = 0; i < passive; ++i) (void)kotor_fastcall_00553990(item, nullptr, static_cast<U>(i)); // 0056EF94..9F
        kotor_fastcall_004d6300(message, nullptr, static_cast<U>(active + passive), 0x10); // 0056EFA1..AC
        U byte_slot = id;                                                        // argument slot [esp+3C]
        for (I i = 0; i < active; ++i) {                                         // 0056EFC0..15
            void* property = kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<U>(i));
            kotor_fastcall_004d6300(message, nullptr, at<U16>(property, 0), 0x10);
            kotor_fastcall_004d6300(message, nullptr, at<U16>(property, 2), 0x10);
            kotor_fastcall_004d6300(message, nullptr, at<U16>(property, 6), 0x10);
            byte_slot = (byte_slot & 0xffffff00u) | at<U8>(property, 9);         // 0056EFF9..00 mov byte [esp+3C], dl
            kotor_fastcall_004d62c0(message, nullptr, byte_slot, 8);
        }
        for (I i = 0; i < passive; ++i) (void)kotor_fastcall_00553990(item, nullptr, static_cast<U>(i)); // 0056F021..2C
    }                                                                            // 0056F02E..3A ~CExoLocString
    return send_reply(message, player_id, 0x1b, 2);                              // 0056F053..79
}

// ======================================================================= 0056AA30 / 0056A7F0 / 0056A8C0 / 00570230 DebugInfo
U __fastcall kotor_fastcall_0056aa30(void* message, void*, void* player, U id) {
    void* area = kotor_fastcall_004ae780(server(), nullptr, id);                 // 0056AA31..43 EDI
    if (area == nullptr) return 0;                                               // 0056AA4A..4C
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                // 0056AA4E..56
    kotor_fastcall_00578570(message, nullptr, id);                               // 0056AA5B..5E
    kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(area) + 0x158, 0x20); // 0056AA63..6E
    return send_reply(message, at<U>(player, 4), 0x21, 4);                       // 0056AA73..A9
}
U __fastcall kotor_fastcall_0056a7f0(void* message, void*, void* player, U id) {
    void* item = recovered_item_lookup_004ae760(server(), nullptr, id);          // 0056A7F1..08 ESI
    if (item == nullptr) return 0;
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                // 0056A80E..16
    kotor_fastcall_00578570(message, nullptr, id);                               // 0056A81B..1E
    kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(item) + 0x28, 0x20); // 0056A823..2B
    kotor_fastcall_004d6340(message, nullptr, at<U>(item, 0x24c), 0x20);        // 0056A830..3B
    for (I i = 0; i < at<I>(item, 0x24c); ++i) {                                 // 0056A840..72 (count reloaded)
        void* property = kotor_fastcall_00553990(item, nullptr, static_cast<U>(i));
        if (property != nullptr) kotor_fastcall_004d6300(message, nullptr, at<U16>(property, 0), 0x10);
    }
    return send_reply(message, at<U>(player, 4), 0x21, 2);                       // 0056A874..AA
}
U __fastcall kotor_fastcall_0056a8c0(void* message, void*, void* player, U id) {
    void* placeable = recovered_placeable_lookup_004ae7a0(server(), nullptr, id); // 0056A8C1..D4 EDI
    if (placeable == nullptr) return 0;
    void* internal = at<void*>(server(), 4);                                     // 0056A8E3..EC
    if (internal == nullptr) return 0;
    void* factions = at<void*>(internal, 0x10054);                               // 0056A8F7..FF
    if (factions == nullptr) return 0;
    void* faction = kotor_fastcall_0052b3b0(factions, nullptr, at<I>(placeable, 0x23c)); // 0056A905..11 EBX
    if (faction == nullptr) return 0;
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                // 0056A91B..23
    kotor_fastcall_00578570(message, nullptr, id);                               // 0056A928..2B
    kotor_fastcall_005785c0(message, nullptr, vcall<void*>(placeable, 0x8c), 0); // 0056A930..3F
    kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(placeable) + 0x18, 0x20); // 0056A944..4C
    kotor_fastcall_004d62c0(message, nullptr, at<U8>(placeable, 0x258), 8);     // 0056A951..5E
    kotor_fastcall_004d6300(message, nullptr, vcall<U>(placeable, 0x9c, 1u), 0x10); // 0056A963..74
    kotor_fastcall_004d62c0(message, nullptr, at<U8>(placeable, 0x314), 8);     // 0056A979..86
    kotor_fastcall_004d62c0(message, nullptr, at<U8>(placeable, 0x316), 8);     // 0056A98B..98
    kotor_fastcall_004d62c0(message, nullptr, at<U8>(placeable, 0x315), 8);     // 0056A99D..AA
    ResRef16 ref; std::memcpy(&ref, static_cast<unsigned char*>(placeable) + 0x378, 16); // 0056A9AF..D2
    kotor_fastcall_004d6380(message, nullptr, ref, 0x10);                        // 0056A9D4
    kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(faction) + 0x18, 0x20); // 0056A9D9..E1
    return send_reply(message, at<U>(player, 4), 0x21, 3);                       // 0056A9E6..1D
}
U __fastcall kotor_fastcall_00570230(void* message, void*, void* player, U id) {
    void* creature = kotor_fastcall_004ae770(server(), nullptr, id);             // 0057023E..60 ESI
    if (creature == nullptr) return 0;
    if (combat_reuse_faction_00513fc0(creature, nullptr) == nullptr) return 0;   // 0057026C..75
    if (at<void*>(creature, 0xa2c) == nullptr) return 0;                         // 0057027B..81
    if (recovered_creature_inventory_004ef770(creature, nullptr, 1) == nullptr) return 0; // 00570287..92
    if (at<void*>(creature, 0xa74) == nullptr) return 0;                         // 00570298..9E
    kotor_fastcall_004d68a0(message, nullptr, 0x100, 0xffffffffu, 1);            // 005702A4..AF
    kotor_fastcall_00578570(message, nullptr, id);                               // 005702B4..B7
    kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(creature) + 0x18, 0x20); // 005702BC..C4
    kotor_fastcall_004d6960(message, nullptr,
        static_cast<unsigned char*>(combat_reuse_faction_00513fc0(creature, nullptr)) + 0x18, 0x20); // 005702C9..D8
    alignas(4) unsigned char resref_out[16];                                     // [esp+24]
    const void* resref = vcall<void*>(creature, 0x80, static_cast<void*>(resref_out)); // 005702DD..E6
    ResRef16 ref; std::memcpy(&ref, resref, 16);                                 // 005702EC..06
    kotor_fastcall_004d6380(message, nullptr, ref, 0x10);                        // 0057030B
    kotor_fastcall_004d6300(message, nullptr, vcall<U>(creature, 0x9c, 1u), 0x10); // 00570310..21
    kotor_fastcall_004d6300(message, nullptr, vcall<U>(creature, 0x98, 1u), 0x10); // 00570326..37
    kotor_fastcall_004d6300(message, nullptr, recovered_creature_armor_class_004ed1d0(creature, nullptr), 0x10); // 0057033C..48
    kotor_fastcall_004d6340(message, nullptr, at<U>(creature, 0x78), 0x20);     // 0057034D..55
    kotor_fastcall_004d6920(message, nullptr, at<U>(at<void*>(creature, 0xa74), 0x84), 0x3f800000u, 0x20); // 0057035A..76
    if (recovered_object_area_004cb120(creature, nullptr) != nullptr) {          // 0057037B..84
        kotor_fastcall_004d6960(message, nullptr,
            static_cast<unsigned char*>(recovered_object_area_004cb120(creature, nullptr)) + 0x158, 0x20); // 00570386..97
    } else {
        ExoString empty("");                                                     // 0057039E..A7 literal 0073D71C
        kotor_fastcall_004d6960(message, nullptr, empty.s, 0x20);                // 005703AC..B9
    }
    U mask = 0;                                                                  // argument slot [esp+48]
    for (U slot = 0; slot < 0x12; ++slot)                                        // 005703CF..09
        if (recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 1u << slot) != nullptr) mask |= 1u << slot;
    kotor_fastcall_004d6340(message, nullptr, mask, 0x20);                       // 0057040B..14
    for (U slot = 0; slot < 0x12; ++slot) {                                      // 00570419..4A
        void* equipped = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 1u << slot);
        if (equipped != nullptr) kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(equipped) + 0x28, 0x20);
    }
    kotor_fastcall_004d6340(message, nullptr,
        at<U>(recovered_creature_inventory_004ef770(creature, nullptr, 1), 0x10), 0x20); // 0057044C..5D
    for (U i = 0; i < at<U>(recovered_creature_inventory_004ef770(creature, nullptr, 1), 0x10); ++i) { // 00570462..A3 (unsigned)
        void* carried = w2x_inventory_item_005560b0(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr, i);
        if (carried != nullptr) kotor_fastcall_004d6960(message, nullptr, static_cast<unsigned char*>(carried) + 0x28, 0x20);
    }
    return send_reply(message, at<U>(player, 4), 0x21, 1);                       // 005704A5..D2
}

// ======================================================================= 004FD1E0 CSWSCreature::Rest
void __fastcall kotor_fastcall_004fd1e0(void* self, void*, U) {
    ExoString unused;                                                            // [esp+14] 005B3190, destroyed at exit
    U zero = 0;                                                                  // [esp+C]
    if (vcall<U>(self, 0x94) != 0) return;                                       // 004FD205..1B
    if (gui_load_slot_zero_0063e7f0(self, nullptr) != 0) return;                 // 004FD221..2A
    U party = at<U>(self, 0x9d4);                                                // 004FD230..38 [esp+8]
    if (party != 0 && recovered_object_area_004cb120(self, nullptr) != nullptr &&
        at<U>(recovered_object_area_004cb120(self, nullptr), 0xb0) != 0) {      // 004FD23C..56
        recovered_creature_feedback_004ede10(self, nullptr, 0x36, nullptr);      // 004FD258..5D
        return;
    }
    if (kotor_fastcall_004f2de0(self, nullptr, 30.0f, k_invalid, k_invalid) != k_invalid) { // 004FD267..84 (41F00000)
        recovered_creature_feedback_004ede10(self, nullptr, 0xba, nullptr);      // 004FD286..8C
        return;
    }
    w5_object_clear_all_actions_004ccd80(self, nullptr, 1);                      // 004FD296..98
    if (party != 0 && at<U>(self, 0x384) != 0) {                                 // 004FD29D..A9 (unsigned jbe)
        recovered_creature_feedback_004ede10(self, nullptr, 0x11, nullptr);      // 004FD2AB..B0
        return;
    }
    at<U>(self, 0x994) = 0; at<U>(self, 0x998) = 0; at<U>(self, 0x99c) = 0; at<U>(self, 0x9a0) = 0; // 004FD2BE..D0
    kotor_fastcall_004eb1b0(self, nullptr, 0xf);                                 // 004FD2BA..D6
    if ((at<U8>(self, 0xa00) & 8) == 0) {                                        // 004FD2DB..E2
        if (at<U8>(self, 0x9fc) & 8) recovered_set_activity_004f00a0(self, nullptr, 8, 0, 0); // 004FD2E8..F7
        for (U bit = 0x10; bit <= 0x80; bit <<= 1)                               // 004FD2FC..7B (unrolled 10h/20h/40h/80h)
            if ((at<U8>(self, 0x9fc) & bit) && !(at<U8>(self, 0xa00) & bit)) recovered_set_activity_004f00a0(self, nullptr, bit, 0, 0);
        at<U>(self, 0x9fc) |= 8;                                                 // 004FD380
    }
    if (kotor_fastcall_004ae910(server(), nullptr, at<U>(self, 4)) != nullptr) { // 004FD387..9B
        recovered_creature_compute_ai_state_004f1460(self, nullptr);            // 004FD39D..9F
        w5_object_clear_all_actions_004ccd80(self, nullptr, 1);                  // 004FD3A4..A8
    }
    if (party != 0) {                                                            // 004FD3AD..B1
        recovered_creature_feedback_004ede10(self, nullptr, 0x12, nullptr);      // 004FD3B5..BA
        void* module = server_get_module_004ae6b0(server(), nullptr);            // 004FD3BF..C7 EBP
        void* memory = live_operator_new(0x34);                                  // 004FD3CC..D5
        void* event = nullptr;
        if (memory != nullptr) event = recovered_event_construct_004d7540(memory, nullptr); // 004FD3DC..EC
        at<U16>(event, 0) = 0x23;                                                // 004FD3FB (unchecked as original)
        recovered_event_integer_004d76b0(event, nullptr, 0, 1);                  // 004FD3F2..400
        (void)recovered_event_insert_004afdb0(recovered_server_ai_master_004aed80(server(), nullptr), nullptr,
            0, 0, at<U>(self, 4), at<U>(module, 0x20), 10, event);               // 004FD405..27 ([007441DC] = 0Ah)
    }
    (void)recovered_object_add_action_004cea20(self, nullptr, 0x2a, 0xffff, 1, &party, 1, &zero,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr);                                                 // 004FD42E..5B
}

// ======================================================================= 004FCE90 AddCounterSpellActions
U __fastcall kotor_fastcall_004fce90(void* self, void*, U target) {
    Vector position{0.0f, 0.0f, 0.0f};                                           // [esp+18..20]
    if (target == k_invalid) return 0;                                           // 004FCE97..BF
    void* other = kotor_fastcall_004ae770(server(), nullptr, target);            // 004FCEC2..D3 EDI
    if (other == nullptr) return 0;
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return 0;      // 004FCEE4..ED
    if (target == at<U>(self, 4)) return 0;                                      // 004FCEEF..F6
    if (recovered_object_area_004cb120(other, nullptr) == nullptr) return 0;     // 004FCEF8..01
    void* area = recovered_object_area_004cb120(other, nullptr);                 // 004FCF03..0C
    if (area == nullptr) return 1;                                               // 004FCF0E [00745DBC] (unreachable)
    const U area_id = at<U>(area, 0x120);                                        // 004FCF1C EBX
    position = at<Vector>(other, 0x90);                                          // 004FCF22..3D
    U range = at<U>(kotor_cell_rules_007a3a28, 0x14);                            // 004FCF31..50 [esp+C]
    U one_a = 1, one_b = 1;                                                      // [esp+10], [esp+14]
    if ((at<U8>(self, 0x9f0) & 2) == 0) return 0;                                // 004FCF44..5C
    U target_slot = target;                                                      // argument slot [esp+34]
    (void)recovered_object_add_action_to_front_004ccb00(self, nullptr, 0x32, 0xffff, 3, &target_slot,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr);                                     // 004FCF62..A5
    (void)recovered_object_add_action_to_front_004ccb00(self, nullptr, 0x11, 0xfffe, 3, &target_slot,
        1, &one_a, 2, &range, 2, &range, 1, &one_b,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr); // 004FCFAA..F7
    Vector zero{0.0f, 0.0f, 0.0f};                                               // [esp+24..2C]
    (void)w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, 0xfffe, &position, area_id, k_invalid,
        one_a, flt(range), 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);              // 004FCFFC..49
    at<U>(self, 0x4d4) = target_slot;                                            // 004FD04E..52
    return 1;
}

// ======================================================================= 004F0A60 / 004F0990 split / merge
void __fastcall kotor_fastcall_004f0a60(void* self, void*, void* item, U count) {
    void* repository = recovered_creature_inventory_004ef770(self, nullptr, 1);  // 004F0A65..75 EBX
    const U possessor = at<U>(item, 0x268);                                      // 004F0A77
    if (possessor != at<U>(self, 4)) {                                           // 004F0A7D..7F
        void* holder = kotor_fastcall_004ae750(server(), nullptr, possessor);    // 004F0A81..90 (unchecked)
        if (vcall<void*>(holder, 0x38) == nullptr) return;                       // 004F0A92..9B
        void* container = vcall<void*>(holder, 0x38);                            // 004F0A9D..A1
        if (at<U>(container, 0x268) == at<U>(self, 4) && at<void*>(container, 0x26c) != nullptr)
            repository = at<void*>(container, 0x26c);                            // 004F0AA4..B9
    }
    void* split = kotor_fastcall_0055f280(item, nullptr, static_cast<I>(count)); // 004F0ABB..C9 [esp+10]
    if (split == nullptr) return;
    if (recovered_inventory_insert_0055d330(repository, nullptr, &split, 0, 0, 1) != 0) return; // 004F0ACF..E3
    recovered_item_add_to_area_00555720(split, nullptr, recovered_object_area_004cb120(self, nullptr),
        at<float>(self, 0x90), at<float>(self, 0x94), at<float>(self, 0x98), 1); // 004F0AE5..08
}
void __fastcall kotor_fastcall_004f0990(void* self, void*, void* item, void* into) {
    void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                   at<int>(item, 0xc)); // 004F0995..A4
    if (at<U16>(base, 0x24) <= 1) return;                                        // 004F09A9..AE (stack size)
    if (recovered_item_stack_merge_00553f90(item, nullptr, into) != 0) {         // 004F09BA..C4
        (void)recovered_creature_remove_item_00510ef0(self, nullptr, into, 0, 0, 0); // 004F09C6..CF
        if (into != nullptr) vcall<void*>(into, 0x0, 1u);                        // 004F09D4..DF scalar deleting destructor
        return;
    }
    if (recovered_inventory_has_item_005a4950(at<void*>(self, 0xa2c), nullptr, item) != 1) return; // 004F09E8..F7
    for (U slot = 0; slot < 0x12; ++slot) {                                      // 004F0A00..27
        void* equipped = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, 1u << slot);
        if (equipped != nullptr && at<U>(equipped, 0x14) == at<U>(item, 0x14)) {
            (void)recovered_object_equip_004f0420(self, nullptr, into, 1u << slot, 0, 0, 0, 0, 0); // 004F0A30..47
            return;
        }
    }
}

// ======================================================================= 005B56E0 CSWSCreature::DoCombatStep
void __fastcall kotor_fastcall_005b56e0(void* self, void*, U step_arg, U, U target_arg) {
    Vector destination{0.0f, 0.0f, 0.0f};                                        // [esp+20..28]
    U hit_default = 0;                                                           // [esp+1C]
    auto fail = [&]() {                                                          // 005B5806 / 005B5C06 (identical)
        kotor_fastcall_004d28f0(at<void*>(self, 0x9c8), nullptr, 0, k_invalid);
        kotor_fastcall_004d2920(at<void*>(self, 0x9c8), nullptr, 0, 0);
        vcall<void>(self, 0x7c, 0x2711u);
    };
    void* found = kotor_fastcall_004ae750(server(), nullptr, target_arg);        // 005B56E3..1B EDI
    if (found == nullptr || vcall<void*>(found, 0x10) == nullptr) { fail(); return; } // 005B571D..2E
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) { fail(); return; } // 005B5734..3D
    void* other = vcall<void*>(found, 0x10);                                     // 005B5743..4A EBX
    if ((at<U8>(self, 0x9f0) & 2) == 0) { fail(); return; }                      // 005B574C..53
    Vector* const self_pos = &at<Vector>(self, 0x90);                            // EBP
    Vector* const other_pos = &at<Vector>(other, 0x90);                          // EDI
    Vector delta;                                                                // [esp+44..4C]
    delta.x = static_cast<float>(static_cast<double>(self_pos->x) - other_pos->x);
    delta.y = static_cast<float>(static_cast<double>(self_pos->y) - other_pos->y);
    delta.z = static_cast<float>(static_cast<double>(self_pos->z) - other_pos->z);
    const float distance = static_cast<float>(vector_magnitude_004ab100(&delta)); // 005B578E..93 [esp+10]
    delta.x = static_cast<float>(static_cast<double>(self_pos->x) - other_pos->x);
    delta.y = static_cast<float>(static_cast<double>(self_pos->y) - other_pos->y);
    delta.z = static_cast<float>(static_cast<double>(self_pos->z) - other_pos->z);
    Vector normal_out; const Vector* n = normalize_004ab180(&normal_out, &delta); // 005B57BE
    Vector dir = *n;                                                             // [esp+2C..34]
    U8 step = static_cast<U8>(step_arg);
    U slot_54 = step_arg;                                                        // argument slot [esp+54]
    U slot_5c = target_arg;                                                      // argument slot [esp+5C]
    int mode;                                                                    // 0..5 dispatch; 0x2728..0x272B
    if (step == 0 || step == 1) {                                                // 005B57D6..E4
        slot_5c = bits(static_cast<float>(kotor_fastcall_004f1310(self, nullptr, target_arg, 0))); // 005B583D..4B
        float other_range;                                                       // [esp+14]
        if (vcall<void*>(other, 0x30) != nullptr) {                              // 005B584F..58
            (void)kotor_fastcall_004f1310(vcall<void*>(other, 0x30), nullptr, at<U>(self, 4), 0); // 005B585A..6E
            other_range = static_cast<float>(kotor_fastcall_004f1310(vcall<void*>(other, 0x30), nullptr, at<U>(self, 4), 1)); // 005B5870..84
        } else {
            other_range = 1.0f;                                                  // 005B588A
        }
        const U ranged = kotor_fastcall_004ebb30(self, nullptr);                 // 005B5892..A3 EBX
        const double range = flt(slot_5c);
        U kind;
        const double near_edge = range - 0.25;                                   // 005B5899..9D [0073EDF0]
        const bool beyond = near_edge > static_cast<double>(distance);           // fcomp; ah&41h == 0
        if ((beyond && ranged == 0) ||
            (ranged == 1 && static_cast<double>(distance) < static_cast<double>(other_range))) { // 005B58A5..C6
            kind = 0x2729;                                                       // 005B58C8
        } else if (range + 0.25 < static_cast<double>(distance)) {               // 005B58CF..E2
            kind = 0x2728;                                                       // 005B58E4
        } else {
            if (static_cast<U8>(slot_54) == 0) { fail(); return; }               // 005B58EB..F1 (byte [esp+54])
            kind = (crt_rand_006fc447() % 2) != 0 ? 0x272a : 0x272b;             // 005B58F7..0C
        }
        if (ranged == 1 && kind != 0x2729) { fail(); return; }                   // 005B5911..1B
        mode = kind == 0x272a ? 4 : kind == 0x272b ? 5 : kind == 0x2728 ? 2 : 3; // 005B5921..8D
        step = static_cast<U8>(slot_54);                                         // 005B5A1B / 005B5A8F reload al
    } else if (step == 2) { mode = 2; } else if (step == 3) { mode = 3; }
    else if (step == 4) { mode = 4; } else if (step == 5) { mode = 5; }
    else { fail(); return; }                                                     // 005B57E6..800
    if (mode == 4 || mode == 5) {                                                // 005B5928 / 005B5995
        x87trig::X80 angle, half;                                                // fpatan / asin results (80-bit)
        float slot54_copy;                                                       // [esp+54] copies (not read later)
        x87trig_005b5930(&dir.x, &angle, &slot54_copy);                          // 005B5928..32 / 005B5995..9F
        x87trig_asin_006fcff0(static_cast<double>(0.4f) / static_cast<double>(distance), &half); // [0073E320] / dist
        Vector unit;                                                             // [esp+20..28]
        if (mode == 4)
            x87trig_005b5966(&angle, &half, &slot54_copy, &unit.x, &unit.y);     // 005B5945..7B (x87)
        else
            x87trig_005b59d3(&angle, &half, &slot54_copy, &unit.x, &unit.y);     // 005B59B2..E8 (x87, fsubr quirk kept)
        unit.z = destination.z;                                                  // [esp+28] still 0
        Vector scaled; vector_component_mult_00423bf0(&scaled, &unit, distance); // 005B597D / 005B59EA
        Vector sum; const Vector* r = vector_sum_00423b90(&sum, other_pos, &scaled); // 005B5AE6 / 005B59F6
        destination = *r;                                                        // -> [esp+20..28]
    } else {                                                                     // mode 2 (toward) / 3 (away)
        Vector heading;                                                          // [esp+38..40]
        if (mode == 2) {
            heading.x = -dir.x; heading.y = -dir.y; heading.z = -dir.z;          // 005B5A21..53 fchs
        } else {
            heading = dir;                                                       // 005B5A95..A9
        }
        U amount;                                                                // [esp+54]
        if (step == 1 || step == 0) {                                            // je / test al,al
            const double gap = mode == 2 ? static_cast<double>(distance) - flt(slot_5c)   // 005B5A67..6B
                                         : static_cast<double>(flt(slot_5c)) - distance;  // 005B5ABD..C1
            if (gap < 0.8f || gap == static_cast<double>(0.8f) || gap != gap) amount = bits(static_cast<float>(gap)); // 005B5A6F..7A -> 5AC7
            else amount = 0x3f4ccccdu;                                           // 005B5A7C..86 0.8f
        } else {
            amount = 0x3f4ccccdu;                                                // 005B5A5D / 005B5AB3
        }
        Vector scaled; float_times_vector_00437a60(&scaled, flt(amount), &heading); // 005B5ACB..DA
        Vector sum; const Vector* r = vector_sum_00423b90(&sum, self_pos, &scaled); // 005B5ADF..E6
        destination = *r;
    }
    void* pf = at<void*>(self, 0x340);                                           // 005B5B02 EDI
    const float radius = at<float>(pf, 8);                                       // [esp+18]
    const float band = at<float>(pf, 0x14);                                      // [esp+54]
    at<void*>(recovered_object_area_004cb120(self, nullptr), 0x1ac) = pf;        // 005B5B12..1D
    const U saved = at<U>(at<void*>(self, 0x340), 0x30);                         // 005B5B23..29
    at<U>(at<void*>(self, 0x340), 0x30) = k_invalid;                             // 005B5B30
    const int direct = w5_area_test_direct_line_004bcb70(recovered_object_area_004cb120(self, nullptr), nullptr,
        self_pos, &destination, &radius, band, 0, nullptr);                      // 005B5B2C..50
    const U clear = direct == 1 ? 1u : hit_default;                              // 005B5B55..5F EBX
    at<U>(at<void*>(self, 0x340), 0x30) = saved;                                 // 005B5B63..69
    // 005B5B6C..CE: x < 0, 10*width < x, y < 0, 10*height < y (ordered) leave the area; NaN passes.
    if (destination.x < 0.0f) { fail(); return; }
    if (static_cast<double>(at<I>(recovered_object_area_004cb120(self, nullptr), 8)) * 10.0f < destination.x) { fail(); return; }
    if (destination.y < 0.0f) { fail(); return; }
    if (static_cast<double>(at<I>(recovered_object_area_004cb120(self, nullptr), 0xc)) * 10.0f < destination.y) { fail(); return; }
    if (clear != 1) { fail(); return; }                                          // 005B5BD4..D7
    (void)w5_creature_update_subareas_on_move_to_0051b7b0(self, nullptr, self_pos, &destination, 0, nullptr, 0); // 005B5BD9..E7
    recovered_object_position_004cd1a0(self, nullptr, &destination, clear, clear, 0); // 005B5BEC..F7
}

}  // extern "C"
