// claude_w1_route_bodies.cpp (lane worker w1-routes): originals behind the W1 port router's former "not provided" trap
// cases (srcw1/w1_native_serializer.successor.cpp), transcribed from swkotor.exe. Four sections (small, combat, effects,
// objects, module); each section's file-local helpers live in its own namespace, exports are extern "C" kotor_fastcall_<addr>.

// ================================================================ section: small
#include "842d6b1618-w5_t3.hpp"
// ===================================================================================================================
// Part "small" (lane worker w1-routes): short originals the W1 port router trapped on, transcribed from swkotor.exe.
// ===================================================================================================================
namespace client_services { struct Client { void __thiscall input_005eda10(int, int); }; }   // 0001 CClientExoApp::SetInputActive
namespace w1r_small {
using namespace w5;
extern "C" {
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                  // 0199 CExoString::operator=
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                     // CServerExoApp::GetPlayerCreatureId
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);           // 0168 GetAtPos
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);                // 0168 GetNext
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                           // 0078 CExoString(const CExoString&)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                             // 0459 ~CExoString
const char* __fastcall WindowCStr005e5670(void*, void*);                                       // 0078 CExoString::CStr
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // 0820 GetBaseItem
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                    // W1 b7 GetPlayerList
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                 // 0595 CSWSPlayer::GetGameObject
void __fastcall kotor_fastcall_0055d800(void* item, void*, U id, U take);                      // CSWSItem::CloseInventory
void __fastcall kotor_fastcall_00587560(void* placeable, void*, U closer, U take);             // CSWSPlaceable::CloseInventory
unsigned __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U id_lo, U id_hi); // W1 RemoveEffectById
void* __fastcall combat_reuse_faction_00513fc0(void*, void*);                                  // W1 b7 CSWSCreature::GetFaction
unsigned __fastcall combat_reuse_members_005be780(void*, void*, void**);                       // W1 b7 CSWSFaction::GetMemberList
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                    // GetSWSMessage
unsigned __fastcall recovered_cc_message_00568570(void* message, void*, U player_id, U kind, void* data, U d); // W2
U __fastcall kotor_fastcall_0050dc40(void* area, void*, U id);                                 // CSWSArea::RemoveObjectFromArea
void __fastcall kotor_fastcall_004cb160(void* object, void*, void* area);                     // CSWSObject::SetArea
U __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, U value);                   // CExoArrayList<int>::remove
extern void* kotor_cell_rules_007a3a28;                                                        // [007A3A28] CSWRules*
}
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* vp(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }

extern "C" {
// 004AEFD0 CServerExoApp::SetLastConversation(CExoString const&): ECX=[ECX+4]+0x1B908; jmp CExoString::operator=. RET 4.
void* __fastcall kotor_fastcall_004aefd0(void* server, void*, void* text) {
    return async_assign_string_005e5c50(vp(at<U>(server, 4) + 0x1b908), nullptr, text);       // 004AEFD0..004AEFD9
}

// 004CE9A0 CSWSObject::SetKeepCorpse(int): stores into the matching client creature of the player's area list, else
// into [this+0x208]. RET 4.  The GetAtPos result is dereferenced without a null check (original).
void __fastcall kotor_fastcall_004ce9a0(void* self, void*, U value) {
    void* const srv = server_app();                                                            // 004CE9A1..9A7
    void* const player = kotor_fastcall_004ae910(srv, nullptr, kotor_fastcall_004aea40(srv, nullptr)); // 004CE9AD..9B7
    if (player != nullptr) {                                                                   // 004CE9BE..9C0
        void* pos = at<void*>(at<void*>(at<void*>(player, 0xc), 0), 0);                        // 004CE9C2..9CB
        while (pos != nullptr) {                                                               // 004CE9CF / 9F9
            void* const data = unresolved_list_at_pos_005e9b60(at<void*>(at<void*>(player, 0xc), 0), nullptr, pos); // 004CE9D1..9D7
            if (at<U>(data, 0x2c) == at<U>(self, 4)) { at<U>(data, 0x14c) = value; return; }   // 004CE9DC..9E2, EA0B..EA18
            unresolved_list_next_005e9b90(at<void*>(at<void*>(player, 0xc), 0), nullptr, &pos); // 004CE9E4..9EE
        }
    }
    at<U>(self, 0x208) = value;                                                                // 004CE9FB..9FF
}

// 004CF9B0 CSWSObject::AddMatchedExpressionString(CExoString const&): new CExoString copy appended to the +0x1A0 list.
// RET 4.  SEH state 0 around the copy constructor frees the block (funclet 00719DB0) and rethrows.
void __fastcall kotor_fastcall_004cf9b0(void* self, void*, void* text) {
    void* const block = live_operator_new(8);                                                  // 004CF9C8..9D1
    void* copy = nullptr;
    if (block != nullptr) {                                                                    // 004CF9D8..9E2
        try {
            copy = WindowCopy005e5b00(block, nullptr, text);                                   // 004CF9E4..9F0
        } catch (...) {
            live_free(block);                                                                  // 00719DB0..DB9
            throw;
        }
    }
    unsigned char* const list = static_cast<unsigned char*>(self) + 0x1a0;                     // 004CFA02
    const U capacity = at<U>(self, 0x1a8);
    if (at<U>(self, 0x1a4) == capacity)                                                        // 004CF9F6..A12
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            capacity != 0 ? capacity + capacity : 0x10);       // 004CFA14..A24
    at<void*>(at<void*>(list, 0), 4 * at<U>(list, 4)) = copy;                                  // 004CFA29..A32
    ++at<U>(list, 4);                                                                          // 004CFA35
}

// 004CFA50 CSWSObject::ClearMatchedExpressionStrings(): RET 0.
void __fastcall kotor_fastcall_004cfa50(void* self, void*) {
    if (at<I>(self, 0x1a4) > 0) {                                                              // 004CFA55..A61
        I i = 0;
        do {
            void* const s = at<void*>(at<void*>(self, 0x1a0), 4 * static_cast<U>(i));          // 004CFA64..A6A
            if (s != nullptr) {                                                                // 004CFA6D..A6F
                frame_LogStringDtor_005e5c20(s, nullptr);                                      // 004CFA71..A73
                live_free(s);                                                                  // 004CFA78..A7E
                at<void*>(at<void*>(self, 0x1a0), 4 * static_cast<U>(i)) = nullptr;            // 004CFA7E..A87
            }
            ++i;                                                                               // 004CFA90
        } while (i < at<I>(self, 0x1a4));                                                      // 004CFA8A..A93
    }
    void* const data = at<void*>(self, 0x1a0);                                                 // 004CFA96
    if (data != nullptr) {                                                                     // 004CFA9C..A9E
        live_free(data);                                                                       // 004CFAA0..AA6
        at<U>(self, 0x1a0) = 0;                                                                // 004CFAA9
        at<U>(self, 0x1a8) = 0;                                                                // 004CFAAF
    }
    at<U>(self, 0x1a4) = 0;                                                                    // 004CFAB5
}

// 0064B4F0 CSWSObject::SetSpellTargetPosition(Vector const&): three dword copies to +0x1B0. RET 4.
void __fastcall kotor_fastcall_0064b4f0(void* self, void*, const void* position) {
    at<U>(self, 0x1b0) = at<U>(const_cast<void*>(position), 0);                                // 0064B4F0..B4FC
    at<U>(self, 0x1b4) = at<U>(const_cast<void*>(position), 4);                                // 0064B4FE..B501
    at<U>(self, 0x1b8) = at<U>(const_cast<void*>(position), 8);                                // 0064B504..B507
}

// 0055DC10 CSWSItem::CloseItemForAllPlayers(): RET 0.
void __fastcall kotor_fastcall_0055dc10(void* item, void*) {
    void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                        at<I>(item, 0xc));    // 0055DC14..DC20
    if (base == nullptr || at<U>(base, 0x70) == 0) return;                                     // 0055DC25..DC2E
    void* const players = recovered_server_player_list_004aed90(server_app(), nullptr);         // 0055DC30..DC3A
    void* pos = at<void*>(at<void*>(players, 0), 0);                                           // 0055DC41..DC47
    while (pos != nullptr) {                                                                   // 0055DC4B / DC92
        void* const player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 0055DC50..DC53
        if (player != nullptr) {                                                               // 0055DC5A..DC5C
            void* const object = kotor_fastcall_00561080(player, nullptr);                     // 0055DC5E..DC60
            if (object != nullptr && at<U>(at<void*>(player, 0x58), 0) == at<U>(item, 0x14))  // 0055DC65..DC71
                kotor_fastcall_0055d800(item, nullptr, at<U>(object, 4), 1);                   // 0055DC73..DC7B
        }
        unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos);                   // 0055DC80..DC87
    }
}

// 00587770 CSWSPlaceable::ClosePlaceableForAllPlayers(): RET 0.
void __fastcall kotor_fastcall_00587770(void* placeable, void*) {
    void* const players = recovered_server_player_list_004aed90(server_app(), nullptr);         // 00587771..777D
    void* pos = at<void*>(at<void*>(players, 0), 0);                                           // 00587782..7788
    while (pos != nullptr) {                                                                   // 0058778A / 77D3
        void* const player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 00587791..7794
        if (player != nullptr) {                                                               // 0058779B..779D
            void* const object = kotor_fastcall_00561080(player, nullptr);                     // 0058779F..77A1
            if (object != nullptr && at<U>(at<void*>(player, 0x58), 0) == at<U>(placeable, 4)) // 005877A6..77B2
                kotor_fastcall_00587560(placeable, nullptr, at<U>(object, 4), 1);              // 005877B4..77BC
        }
        unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos);                   // 005877C1..77C8
    }
}

// 00506B30 CSWSArea::SetTransitionPending(int pending, unsigned char id): RET 8 (tail jmp CClientExoApp::SetInputActive
// with the two argument slots rewritten: pending ? (0, 1) : (1, 0)).
void __fastcall kotor_fastcall_00506b30(void* area, void*, U pending, U id) {
    at<U>(area, 0x2c4) = pending;                                                              // 00506B3A
    at<U8>(area, 0x2c9) = static_cast<U8>(id);                                                 // 00506B36, B40
    client_services::Client* const client = static_cast<client_services::Client*>(client_app());
    if (pending != 0) client->input_005eda10(0, 1);                                            // 00506B48..B60
    else client->input_005eda10(1, 0);                                                         // 00506B65..B7E
}

// 00506AC0 CSWSArea::GetNextTransitionPendingID(): byte counter at +0x2C8 that skips 0. RET 0, result in AL.
U8 __fastcall kotor_fastcall_00506ac0(void* area, void*) {
    if (++at<U8>(area, 0x2c8) == 0) at<U8>(area, 0x2c8) = 1;                                  // 00506AC0..AC8
    return at<U8>(area, 0x2c8);                                                                // 00506ACF
}

// 004D0870 CSWSObject::RemoveSomeEffectsOfDurationType(unsigned short type): RET 4.
// Quirks kept: after a removal the index restarts at 1 (ECX := 0 then the shared INC), so element 0 is not
// re-examined; the walk stops after 21 removals (`cmp edi,14h / ja`).
void __fastcall kotor_fastcall_004d0870(void* self, void*, U type) {
    const U16 wanted = static_cast<U16>(type);                                                 // 004D0883 (BP)
    U removed = 0, i = 0;                                                                      // 004D087A..087C
    if (at<U>(self, 0x128) == 0) return;                                                       // 004D0873..0880 (jbe)
    for (;;) {
        void* const effect = at<void*>(at<void*>(self, 0x124), 4 * i);                         // 004D0890..0896
        if (static_cast<U16>(at<U8>(effect, 0xa) & 7) == wanted) {                             // 004D0899..08A4
            recovered_object_remove_effect_004d06c0(self, nullptr, at<U>(effect, 0), at<U>(effect, 4)); // 004D08A6..08AF
            i = 0;                                                                             // 004D08B4
            if (++removed > 0x14) return;                                                      // 004D08B6..08BA
        }
        ++i;                                                                                   // 004D08C2
        if (!(i < at<U>(self, 0x128))) return;                                                 // 004D08BC..08C5 (jb)
    }
}

// 004EC7A0 CSWSCreature::BroadcastStealthDetectionData / 004EC840 BroadcastMineDetectionData (CSWCCMessageData*):
// identical loops over the faction member list sending CC message 0x10 / 0x13 to every player creature in the same
// area. RET 4.  GetFaction is re-called for every bound check (original).
static void broadcast_detection(void* self, void* data, U kind) {
    void** members = nullptr;                                                                  // [esp+8] (pushed ecx slot)
    (void)combat_reuse_members_005be780(combat_reuse_faction_00513fc0(self, nullptr), nullptr, reinterpret_cast<void**>(&members)); // ..A3..B1
    for (I i = 0; i < at<I>(combat_reuse_faction_00513fc0(self, nullptr), 4); ++i) {          // ..B6..C4, 823..82E
        void* const player = kotor_fastcall_004ae910(server_app(), nullptr, addr(members[i])); // ..D0..E1
        if (player == nullptr) continue;                                                       // ..E6..EA
        void* const object = kotor_fastcall_00561080(player, nullptr);                         // ..EC..EE
        if (object == nullptr) continue;                                                       // ..F3..F5
        if (at<U>(self, 0x8c) != at<U>(object, 0x8c)) continue;                                // ..F7..803
        const U player_id = at<U>(player, 4);                                                  // ..805
        recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr, player_id, kind,
                                      data, 0);                                                // ..808..81E
    }
}
void __fastcall kotor_fastcall_004ec7a0(void* self, void*, void* data) { broadcast_detection(self, data, 0x10); }   // 004EC814 push 10h
void __fastcall kotor_fastcall_004ec840(void* self, void*, void* data) { broadcast_detection(self, data, 0x13); }   // 004EC8B4 push 13h

// 004D42E0 CSWSCombatRound::DEBUGDumpCombatRoundToScreen(int, CExoString): RET 8.  The retail body only takes the CStr
// of the label and walks the +0x9B0 list (the position is kept in the first argument slot); nothing is printed.
void __fastcall kotor_fastcall_004d42e0(void* round, void*, U, void* label) {
    (void)WindowCStr005e5670(label, nullptr);                                                  // 004D42E3..42E7
    void* pos = at<void*>(at<void*>(at<void*>(round, 0x9b0), 0), 0);                           // 004D42EC..42F8
    if (pos == nullptr) return;                                                                // 004D42FC
    if (unresolved_list_at_pos_005e9b60(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, pos) == nullptr) return; // 004D42FE..4308
    while (unresolved_list_next_005e9b90(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, &pos) != 0) {} // 004D4310..4324
}

// 0058FEC0 CSWArea::RemoveStaticBoundingBox(unsigned long id): RET 4 -> 0/1.  Ids at +0x50, 24-byte boxes at +0x4C,
// count at +0x48 (re-read in both loops; the first bound check uses the entry count).
U __fastcall kotor_fastcall_0058fec0(void* area, void*, U id) {
    const I n = at<I>(area, 0x48);                                                             // 0058FEC2
    I i = 0;                                                                                   // 0058FEC5
    if (n <= 0) return 0;                                                                      // 0058FEC7..FECA
    for (;;) {
        if (at<U>(at<void*>(area, 0x50), 4 * static_cast<U>(i)) == id) break;                  // 0058FECC..FED5
        ++i;                                                                                   // 0058FEDA
        if (!(i < at<I>(area, 0x48))) return 0;                                                // 0058FED7..FEE0
    }
    if (i < 0 || i >= n) return 0;                                                             // 0058FEEA..FEF0
    at<I>(area, 0x48) = n - 1;                                                                 // 0058FEF2..FEF7
    if (i < n - 1) {                                                                           // 0058FEFA
        U off = static_cast<U>(i) * 0x18;                                                      // 0058FEFC..FEFF
        do {
            unsigned char* const ids = static_cast<unsigned char*>(at<void*>(area, 0x50));
            at<U>(ids, 4 * static_cast<U>(i)) = at<U>(ids, 4 * static_cast<U>(i) + 4);         // 0058FF02..FF09
            unsigned char* box = static_cast<unsigned char*>(at<void*>(area, 0x4c)) + off;     // 0058FF0F..FF12
            at<U>(box, 0) = at<U>(box, 0x18); at<U>(box, 4) = at<U>(box, 0x1c); at<U>(box, 8) = at<U>(box, 0x20); // FF14..FF24
            box = static_cast<unsigned char*>(at<void*>(area, 0x4c)) + off;                    // 0058FF27..FF2A
            at<U>(box, 0xc) = at<U>(box, 0x24); at<U>(box, 0x10) = at<U>(box, 0x28); at<U>(box, 0x14) = at<U>(box, 0x2c); // FF2C..FF3F
            ++i; off += 0x18;                                                                  // 0058FF45..FF46
        } while (i < at<I>(area, 0x48));                                                       // 0058FF42, FF49..FF4B
    }
    return 1;                                                                                  // 0058FF4F
}

// Shared tail of the RemoveFromArea bodies: SetArea(NULL); SetPosition(&{0,0,0}, 1, 1, 0).
static void remove_from_area_tail(void* self) {
    kotor_fastcall_004cb160(self, nullptr, nullptr);                                           // push 0; call 004CB160
    const Vector zero = {0.0f, 0.0f, 0.0f};                                                    // three dword zero stores
    recovered_object_position_004cd1a0(self, nullptr, &zero, 1, 1, 0);                         // push 0,1,1,&v; call 004CD1A0
}

// 00584950 CSWSPlaceable::RemoveFromArea(): RET 0.
void __fastcall kotor_fastcall_00584950(void* self, void*) {
    if (recovered_object_area_004cb120(self, nullptr) != nullptr) {                            // 00584956..495D
        if (at<I>(self, 0x344) >= 0) {                                                         // 0058495F..4967
            (void)kotor_fastcall_0058fec0(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 00584969..4976
            at<U>(self, 0x344) = 0xffffffffu;                                                  // 0058497B
        }
        (void)kotor_fastcall_0050dc40(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 00584985..4992
    }
    remove_from_area_tail(self);                                                               // 00584997..49C5
}

// 0058B490 CSWSDoor::RemoveFromArea(): RET 0.
void __fastcall kotor_fastcall_0058b490(void* self, void*) {
    if (recovered_object_area_004cb120(self, nullptr) != nullptr) {                            // 0058B496..B49D
        (void)kotor_fastcall_0050dc40(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 0058B49F..B4AD
        const U id = at<U>(self, 4);                                                           // 0058B4B2
        unsigned char* const list = static_cast<unsigned char*>(recovered_object_area_004cb120(self, nullptr)) + 0x12c; // 0058B4B5..B4C2
        I count = at<I>(list, 4);
        U hits = 0;                                                                            // 0058B4C7
        if (count > 0) {                                                                       // 0058B4C9..B4CB
            const U* p = static_cast<const U*>(at<void*>(list, 0));                            // 0058B4CD
            do { if (*p == id) ++hits; ++p; } while (--count != 0);                            // 0058B4D0..B4D9
            if (hits == 1)                                                                     // 0058B4DB..B4DE
                recovered_int_list_remove_0066dcc0(static_cast<unsigned char*>(recovered_object_area_004cb120(self, nullptr)) + 0x12c,
                                                   nullptr, at<U>(self, 4));                   // 0058B4E0..B4F1
        }
        (void)kotor_fastcall_0058fec0(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 0058B4F6..B503
    }
    remove_from_area_tail(self);                                                               // 0058B509..B537
}
}
}   // namespace w1r_small

// ================================================================ section: combat
// w1-routes part "combat": CSWSCombatRound / CSWSCombatAttackData / CSWSCreature leaves that the W1 combat timer
// (004FAF20 UpdateCombatRoundTimer) routes to and that had no provider in the c13c candidate:
//   004D4E80 CSWSCombatRound::DecrementPauseTimer            004D45E0 CSWSCombatAttackData::IsSubAttackReady
//   004D45B0 CSWSCombatAttackData::ResolveTopSubAttackToAttack 004D32D0 CSWSCombatAttackData::RemoveTopSubAttack
//   004F1250 CSWSCreature::ResetCombatAnimations
// and their missing callees:
//   004D4560 CSWSCombatAttackData::GetTopSubAttack           004D27E0 CSWSCombatAttackData::ResolveSubAttackToAttack
//   004D2EE0 CExoArrayList<CSWSCombatSubAttack>::RemoveAt
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); live_operator_new / live_free only.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); } // CExoString() [0603]

namespace w1r_combat {
using namespace w5;

extern "C" {
// ---------------- imports: existing selected providers (c13c map; object in brackets) ----------------
void* __fastcall kotor_fastcall_004d32f0(void* s, void*);                                    // CSWSCombatSubAttack() [claude_combat_attack_round]
U __fastcall kotor_fastcall_004d4620(void* round, void*, U flag);                            // EndCombatRound [claude_combat_attack_round]
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                 // SetRoundPaused [claude_w5_combat_actions]
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                 // SetPauseTimer [claude_w5_combat_actions]
void __fastcall kotor_fastcall_004d2b40(void* round, void*, U type);                         // SetCurrentAction [claude_w5_combat_actions]
void __fastcall recovered_combat_round_increment_timer_004d4c10(void* self, void*, unsigned dt); // IncrementTimer [W1 b7]
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                            // return_zero [0832]
unsigned __fastcall recovered_object_animation_stationary_004cae60(void*, void*, unsigned animation); // AnimationStationary [W1 b7]
U __fastcall w5x_creature_is_debilitated_005b4880(void* creature, void*);                    // IsDebilitated [0886]
U __fastcall kotor_fastcall_004f0f90(void* creature, void*);                                 // GetPauseReadyAnimation [claude_w5_pathfind_walk_distance]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                                // ~CExoString [0459]
const char* __fastcall WindowCStr005e5670(void*, void*);                                     // CExoString::CStr [0078]
void __cdecl frame_LogFormat_005e5680(void*, const char*, ...);                              // CExoString::Format [0459]

// ---------------- exports of this part ----------------
void* __fastcall kotor_fastcall_004d4560(void* data, void*, void* out);
void* __fastcall kotor_fastcall_004d27e0(void* data, void*, const void* sub);
void __fastcall kotor_fastcall_004d2ee0(void* list, void*, I index);
U __fastcall kotor_fastcall_004d45e0(void* data, void*, U elapsed);
void __fastcall kotor_fastcall_004d45b0(void* data, void*);
void __fastcall kotor_fastcall_004d32d0(void* data, void*);
void __fastcall kotor_fastcall_004d4e80(void* round, void*, U dt);
U __fastcall kotor_fastcall_004f1250(void* creature, void*, U overflow);
}

namespace {
constexpr U k_sub_attack_size = 0xd0;                 // sizeof(CSWSCombatSubAttack): rep movsd ecx = 34h
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline const unsigned char* cbytes(const void* p) { return static_cast<const unsigned char*>(p); }
}

extern "C" {
// ---------------------------------------------------------------------------------------------------------
// 004D4560 CSWSCombatAttackData::GetTopSubAttack(CSWSCombatSubAttack* out): RET 4, EAX = out.
// A default-constructed local (fields the constructor leaves unwritten stay indeterminate, as in the original)
// is overwritten by element 0 of the +AC list when its count +B0 is > 0 (signed), then copied whole to out.
void* __fastcall kotor_fastcall_004d4560(void* data, void*, void* out) {
    alignas(8) unsigned char local[k_sub_attack_size];                              // [esp+8] (0xD0, 8-aligned frame)
    kotor_fastcall_004d32f0(local, nullptr);                                        // 004D4570..74
    if (at<I>(data, 0xb0) > 0)                                                      // 004D4579..81 (jle)
        std::memcpy(local, at<void*>(data, 0xac), k_sub_attack_size);               // 004D4583..92 rep movsd 34h
    std::memcpy(out, local, k_sub_attack_size);                                     // 004D4594..A2 rep movsd 34h
    return out;                                                                     // 004D4594 eax = out
}

// 004D27E0 CSWSCombatAttackData::ResolveSubAttackToAttack(CSWSCombatSubAttack* sub): RET 4, EAX = this.
void* __fastcall kotor_fastcall_004d27e0(void* data, void*, const void* sub) {
    unsigned char* d = bytes(data); const unsigned char* s = cbytes(sub);
    d[0x5c] = s[4];                                                                 // 004D27E6..E9
    std::memcpy(d + 0x64, s + 6, 2);                                                // 004D27EC..F0
    d[0x3a] = s[8];                                                                 // 004D27F4..F7
    std::memcpy(d + 0x68, s + 0x20, 12);                                            // 004D27FD..10 (3 dwords)
    std::memcpy(d + 0x10, s + 0x1c, 2);                                             // 004D2813..17
    std::memcpy(d + 0x04, s + 0x10, 4);                                             // 004D281B..1E
    for (U k = 0; k < 0xf; ++k) std::memcpy(d + 0x1c + 2 * k, s + 0x30 + 2 * k, 2); // 004D2821..3D (15 words)
    std::memcpy(d + 0xc0, s + 0x50, 8 * 4);                                         // 004D283F..4D
    std::memcpy(d + 0xe0, s + 0x70, 5 * 4);                                         // 004D284F..5D
    std::memcpy(d + 0xf4, s + 0x84, 8); d[0xfc] = s[0x8c];                          // 004D285F..78
    std::memcpy(d + 0xfe, s + 0x8e, 10 * 4 + 2);                                    // 004D287B..8E (10 dwords + movsw)
    std::memcpy(d + 0x128, s + 0xb8, 6 * 4);                                        // 004D2890..A1
    return data;                                                                    // 004D27E4 eax = this
}

// 004D2EE0 CExoArrayList<CSWSCombatSubAttack>::RemoveAt(int index): RET 4. Count decremented first; the tail is
// shifted down one element while index < count (signed; count re-read each pass). No bounds check (original).
void __fastcall kotor_fastcall_004d2ee0(void* list, void*, I index) {
    const I count = at<I>(list, 4) - 1;                                             // 004D2EE8..EEB
    at<I>(list, 4) = count;                                                         // 004D2EF0
    if (index >= count) return;                                                     // 004D2EEE..F3 (jge)
    U offset = static_cast<U>(index) * k_sub_attack_size;                           // 004D2EF5..F7
    I i = index;
    do {
        unsigned char* element = bytes(at<void*>(list, 0)) + offset;                // 004D2F00..02
        std::memcpy(element, element + k_sub_attack_size, k_sub_attack_size);      // 004D2F05..10 rep movsd 34h
        ++i; offset += k_sub_attack_size;                                           // 004D2F15..16
    } while (i < at<I>(list, 4));                                                   // 004D2F12, 1C..1E (jl)
}

// ---------------------------------------------------------------------------------------------------------
// 004D45E0 CSWSCombatAttackData::IsSubAttackReady(elapsed): RET 4, EAX 1 when (word +8 - elapsed) > top.+0C
// (unsigned compare, ja), else 0.
U __fastcall kotor_fastcall_004d45e0(void* data, void*, U elapsed) {
    alignas(4) unsigned char top[k_sub_attack_size];                                // [esp+8]
    kotor_fastcall_004d4560(data, nullptr, top);                                    // 004D45E8..F1
    const U remaining = static_cast<U>(at<U16>(data, 8)) - elapsed;                 // 004D45F6..05
    return remaining > at<U>(top, 0xc) ? 1u : 0u;                                   // 004D4601..10 (ja)
}

// 004D45B0 CSWSCombatAttackData::ResolveTopSubAttackToAttack: RET 0.
void __fastcall kotor_fastcall_004d45b0(void* data, void*) {
    alignas(4) unsigned char top[k_sub_attack_size];                                // [esp+4]
    kotor_fastcall_004d4560(data, nullptr, top);                                    // 004D45B7..BE
    kotor_fastcall_004d27e0(data, nullptr, top);                                    // 004D45C3..C6 (push eax = &top)
}

// 004D32D0 CSWSCombatAttackData::RemoveTopSubAttack: RET 0. RemoveAt(0) on the +AC list when +B0 > 0 (signed).
void __fastcall kotor_fastcall_004d32d0(void* data, void*) {
    if (at<I>(data, 0xb0) > 0)                                                      // 004D32D0..D8 (jle)
        kotor_fastcall_004d2ee0(bytes(data) + 0xac, nullptr, 0);                    // 004D32DA..E2
}

// ---------------------------------------------------------------------------------------------------------
// 004D4E80 CSWSCombatRound::DecrementPauseTimer(dt): RET 4.
// Master (+9C0) missing: format the (unused) warning text, +9C0 = invalid, EndCombatRound(1).
// Otherwise +960 -= dt; at <= 0 (signed) with no pause mode (+964 == 0): +960 = 0, +958 = 0, +95C = invalid.
void __fastcall kotor_fastcall_004d4e80(void* round, void*, U dt) {
    alignas(4) unsigned char text[8];                                               // CExoString [esp+4]
    kotor_movie::string_element_construct_005b3190(text, nullptr);                  // 004D4E9B..9F
    struct Guard { unsigned char* s; ~Guard() { frame_LogStringDtor_005e5c20(s, nullptr); } } guard{text}; // EH state 0
    if (kotor_fastcall_004ae750(server_app(), nullptr, at<U>(round, 0x9c0)) == nullptr) { // 004D4EA4..C3
        const char* name = WindowCStr005e5670(bytes(at<void*>(round, 0x9b4)) + 0x18, nullptr); // 004D4EC5..CE
        frame_LogFormat_005e5680(text,
            "CSWSCombatRound::DecrementPauseTimer - %s Master cannot be found expire the round; Resetting", // .rdata 00746540
            name);                                                                  // 004D4ED3..E3
        at<U>(round, 0x9c0) = k_object_invalid;                                     // 004D4EEA
        kotor_fastcall_004d4620(round, nullptr, 1);                                 // 004D4EE6..F4 EndCombatRound(1)
        return;                                                                     // 004D4EF9 -> 4F3B (~CExoString by guard)
    }
    const I left = static_cast<I>(at<U>(round, 0x960) - dt);                        // 004D4EFB..05
    at<I>(round, 0x960) = left;                                                     // 004D4F0B
    if (left > 0) return;                                                           // 004D4F09..11 (jg)
    if (at<U>(round, 0x964) != 0) return;                                           // 004D4F13..1B
    at<U>(round, 0x960) = 0;                                                        // 004D4F1D
    at<U>(round, 0x958) = 0;                                                        // 004D4F27
    at<U>(round, 0x95c) = k_object_invalid;                                         // 004D4F31
}

// 004F1250 CSWSCreature::ResetCombatAnimations(overflow): RET 4, EAX 1 when the pause-ready animation was played.
U __fastcall kotor_fastcall_004f1250(void* creature, void*, U overflow) {
    kotor_fastcall_004d28f0(at<void*>(creature, 0x9c8), nullptr, 0, k_object_invalid); // 004F1254..62 SetRoundPaused(0, invalid)
    kotor_fastcall_004d2920(at<void*>(creature, 0x9c8), nullptr, 0, 0);             // 004F1267..6F SetPauseTimer(0, 0)
    recovered_combat_round_increment_timer_004d4c10(at<void*>(creature, 0x9c8), nullptr, overflow); // 004F1274..7F
    kotor_fastcall_004d2b40(at<void*>(creature, 0x9c8), nullptr, 0);                // 004F1284..8B SetCurrentAction(0)
    at<U>(creature, 0x4dc) = 0;                                                     // 004F1294
    if (vcall<U>(creature, 0x94) != 0) return 0;                                    // 004F1290..A2 vtbl+94
    if (kotor_m15e_slot010_zero_0063e7f0(creature, nullptr) != 0) return 0;         // 004F12A4..AD return_zero
    if (recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) return 0; // 004F12AF..B8 GetDeadTemp
    if (recovered_object_animation_stationary_004cae60(creature, nullptr, at<U>(creature, 0xd4)) == 0) return 0; // 004F12BA..CA
    if (w5x_creature_is_debilitated_005b4880(creature, nullptr) != 0) return 0;     // 004F12CC..D5
    if (at<U>(at<void*>(creature, 0x9c8), 0x940) != 0) return 0;                    // 004F12D7..E3
    void** table = *static_cast<void***>(creature);                                 // 004F12E5 (vtable read before the call)
    const U animation = kotor_fastcall_004f0f90(creature, nullptr);                 // 004F12E7..E9 GetPauseReadyAnimation
    using F = void(__fastcall*)(void*, void*, U);
    reinterpret_cast<F>(table[0x7c / 4])(creature, nullptr, animation);             // 004F12EE..F1 vtbl+7C (PlayAnimation)
    return 1;                                                                       // 004F12F5
}
}  // extern "C"
}  // namespace w1r_combat

// ================================================================ section: effects
// W1 route bodies, part "effects": the two W1 UpdateEffectList (004D1730) callees that had no provider in c13c,
//   004EE770 CSWSCreature::ApplyPoisonDamage(int row, CGameEffect* poison, int a3, float strength)   RET 10h -> 0/1
//   004F6790 CSWSCreature::ApplyDiseasePayload(CGameEffect* disease, ulong day, ulong ms)           RET 0Ch -> 0/1
// plus their callees without a provider anywhere:
//   00503F70 CGameEffect::CGameEffect(CGameEffect* source)   RET 4   (copy-construct header fields, fresh payload)
//   004D08D0 CSWSObject::RemoveEffectByType(creator, type, int0) RET 0Ch
//   004EF560 CSWSCreature::DisplayFloatyDataToSelf(int value)   RET 4
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision; float exactly
// where the original stores a dword; live_operator_new / live_free only. Failed allocations are not guarded (the
// original then writes through the null effect pointer); the debug POISONTRACE Format/CStr texts are built and
// discarded exactly as in the original.
#include "842d6b1618-w5_t3.hpp"

// C++-linkage providers (declared at global scope so their mangled names match their owners).
namespace kotor_reconstruction { struct StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other); // CExoString::operator+ [0093]
namespace w2 { namespace g07 { unsigned __cdecl ftol2_low_006fae8c(double value); } }   // 006FAE8C __ftol2 [w2_g07]
namespace frame_control { extern unsigned message_manager_007a3a00; }                    // .data 007A3A00 g_pVirtualMachine [0456]

namespace w1r_effects {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c13c map; object in brackets) ----------------
extern U recovered_effect_rank_scratch_008327e0;                                                // .bss 008327E0 fistp scratch [0886]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);              // CGameEffect::CGameEffect(int) [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                   // CGameEffect::SetCreator [0886]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);       // CGameEffect::SetInteger [0886]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                   // CGameEffect::GetInteger [W1 b7]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, unsigned count);          // CGameEffect::SetNumIntegers [1091]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);   // CSWSObject::ApplyEffect [0886]
void __fastcall recovered_object_remove_effect_004d05c0(void* object, void*, void* effect);     // CSWSObject::RemoveEffect [W1 b7]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                      // GetServerAIMaster [creature_destructor_r2]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                    // GetSWSMessage [creature_destructor_r2]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                  // CSWCCMessageData() [W1 b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                     // ~CSWCCMessageData [W1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);          // CSWCCMessageData::SetInteger [1017]
void __fastcall recovered_event_object_004d7780(void*, void*, U, U);                            // CSWCCMessageData::SetObjectID [1017]
void* __fastcall recovered_active_timer_004ae830(void* server, void*, U id);                    // CServerExoApp::GetActiveTimer [W1 b7]
U __fastcall kotor_fastcall_004adf50(void* timer, void*, U d1, U t1, U d2, U t2, U* out_day, U* out_time); // AddWorldTimes [w4]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);          // CSWRules::RollDice (AX) [W1 b7]
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7); // SavingThrowRoll (AL) [rolls]
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I value);                           // CExoString(int) [0266]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0379]
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned id, unsigned flag); // CVirtualMachine::RunScript [0457]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                    // CExoString::Format [0125]
const char* __fastcall WindowCStr005e5670(void*, void*);                                        // CExoString::CStr [0078]
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                 // CExoString::operator=(char*) [0459]
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, void* construct, void* destroy); // [d1]

// ---------------- exports of this file ----------------
U __fastcall kotor_fastcall_004ee770(void* self, void*, I row, void* poison, U a3, float strength);
U __fastcall kotor_fastcall_004f6790(void* self, void*, void* disease, U day, U ms);
void* __fastcall kotor_fastcall_00503f70(void* self, void*, void* source);
void __fastcall kotor_fastcall_004d08d0(void* self, void*, U creator, U type, U value);
void __fastcall kotor_fastcall_004ef560(void* self, void*, I value);
}

namespace {
// .rdata constants (proven values)
constexpr U16 k_effect_poison_hp_00746714 = 0x26;      // word [00746714] (HP damage effect type)
constexpr U16 k_effect_fp_007467f4 = 0x5f;             // word [007467F4] (FP damage effect type)
constexpr U k_effect_ability_00746710 = 0x25;          // [00746710] (ability decrease effect type; word and dword reads)
constexpr U16 k_effect_visual_007466f4 = 0x1e;         // word [007466F4] (visual effect)
constexpr U k_effect_disease_00746690 = 5;             // [00746690] (disease effect type)
constexpr U16 k_duration_instant_007473b8 = 0;         // word [007473B8]
constexpr U16 k_duration_temporary_007473c0 = 1;       // word [007473C0]
constexpr U k_event_apply_effect_007441c8 = 5;         // [007441C8]
constexpr U k_event_remove_effect_007441ec = 0xe;      // [007441EC]
constexpr I k_hp_scale_00746884 = 0x2000;              // [00746884]
constexpr U k_hp_extra_00746894 = 0;                   // [00746894]
constexpr U k_log_scale_bits_00745ff8 = 0x40549a78u;   // float [00745FF8] = 3.321928 (1/log10(2))
constexpr U k_ms_per_minute_00744178 = 60000;          // [00744178]
const char k_empty_0073d71c[] = "";                                                       // .rdata 0073D71C
const char k_dam_hp_00746ee8[] = "DAM_HP";                                                // .rdata 00746EE8
const char k_dam_fp_00746ee0[] = "DAM_FP";                                                // .rdata 00746EE0
const char k_dam_str_00746ed8[] = "DAM_STR";                                              // .rdata 00746ED8
const char k_dam_dex_00746ed0[] = "DAM_DEX";                                              // .rdata 00746ED0
const char k_dam_con_00746ec8[] = "DAM_CON";                                              // .rdata 00746EC8
const char k_dam_int_00746ec0[] = "DAM_INT";                                              // .rdata 00746EC0
const char k_dam_wis_00746eb8[] = "DAM_WIS";                                              // .rdata 00746EB8
const char k_dam_chr_00746eb0[] = "DAM_CHR";                                              // .rdata 00746EB0
const char k_duration_007469d0[] = "DURATION";                                            // .rdata 007469D0
const char k_period_007469c8[] = "PERIOD";                                                // .rdata 007469C8
const char k_trace_duration_00746e7c[] = "POISONTRACE: Ability Damage duration = %d seconds\n"; // .rdata 00746E7C
const char k_trace_hp_00746e54[] = "POISONTRACE: Applying HP damage: %d\n";              // .rdata 00746E54
const char k_trace_fp_00746e2c[] = "POISONTRACE: Applying FP damage: %d\n";              // .rdata 00746E2C
const char k_trace_str_00746e04[] = "POISONTRACE: Applying STR damage: %d\n";            // .rdata 00746E04
const char k_trace_dex_00746ddc[] = "POISONTRACE: Applying DEX damage: %d\n";            // .rdata 00746DDC
const char k_trace_con_00746db4[] = "POISONTRACE: Applying CON damage: %d\n";            // .rdata 00746DB4
const char k_trace_int_00746d8c[] = "POISONTRACE: Applying INT damage: %d\n";            // .rdata 00746D8C
const char k_trace_wis_00746d64[] = "POISONTRACE: Applying WIS damage: %d\n";            // .rdata 00746D64
const char k_trace_chr_00746d3c[] = "POISONTRACE: Applying CHR damage: %d\n";            // .rdata 00746D3C
const char k_subs_save_007470bc[] = "Subs_Save";                                          // .rdata 007470BC
const char k_dice_007470b4[] = "Dice_";                                                   // .rdata 007470B4
const char k_dam_007470ac[] = "Dam_";                                                     // .rdata 007470AC
const char k_type_007470a4[] = "Type_";                                                   // .rdata 007470A4
const char k_end_incu_script_00747094[] = "End_Incu_Script";                              // .rdata 00747094
const char k_24_hour_script_00747084[] = "24_Hour_Script";                                // .rdata 00747084

using Str = kotor_reconstruction::StartupString;
struct ExoString { alignas(4) unsigned char b[8]; void* p() { return b; } Str* s() { return reinterpret_cast<Str*>(b); } };

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* two_dim() { return at<void*>(rules(), 0xb8); }                         // CSWRules +B8 CTwoDimArrays*
inline void str_text(ExoString& s, const char* t) { frame_LogStringCtor_005e5a90(s.p(), nullptr, t); }
inline void str_empty(ExoString& s) { kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s.p()), nullptr); }
inline void str_free(ExoString& s) { frame_LogStringDtor_005e5c20(s.p(), nullptr); }
inline void set_int(void* e, U index, U value) { recovered_effect_integer_005036a0(e, nullptr, index, value); }
inline U get_int(void* e, U index) { return recovered_effect_integer_00503690(e, nullptr, index); }
// CExoString(name); GetINTEntry(row, name, out); ~CExoString (result ignored)
void read_int(void* table, I row, const char* name, I* out) {
    ExoString column; str_text(column, name);
    (void)c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), column.p(), out);
    str_free(column);
}
// CExoString(); Format(&s, text, value); CStr(); ~CExoString   (debug trace text, discarded)
void trace(const char* format, I value) {
    ExoString text; str_empty(text);
    frame_LogFormat_005e5680(text.p(), format, value);
    (void)WindowCStr005e5670(text.p(), nullptr);
    str_free(text);
}
// operator new(8Ch) + CGameEffect(source); null when the allocation fails
void* copy_effect(void* source) {
    void* memory = live_operator_new(0x8c);
    return memory != nullptr ? kotor_fastcall_00503f70(memory, nullptr, source) : nullptr;
}
inline void set_duration_bits(void* e, U16 type) {
    at<U16>(e, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(e, 0xa)) & 0xfff8u) | type);
}
// fldlg2; fild [x]; fyl2x; fmul dword [00745FF8]; fstp dword; fld; frndint; fistp dword [008327E0]
// (004EEB2B..4EEB48): round(log2(x)) through the float.
U log2_index(I x) {
    I value = x;
    float scaled;
    const float scale = flt(k_log_scale_bits_00745ff8);
    __asm {
        fldlg2
        fild dword ptr [value]
        fyl2x
        fmul dword ptr [scale]
        fstp dword ptr [scaled]
        fld dword ptr [scaled]
        frndint
        fistp dword ptr [recovered_effect_rank_scratch_008327e0]
    }
    return recovered_effect_rank_scratch_008327e0;                                  // 004EEB4E mov eax,[008327E0]
}
// one ability-damage arm of ApplyPoisonDamage (004EECC4.. / 004EEDC2.. / 004EEEC0.. / 004EEFBE.. / 004EF0BC.. / 004EF1BA..)
void ability_arm(void* self, void* poison, U a3, I amount, I duration, U creator, U ability, I strength, const char* format) {
    if (amount <= 0 || duration <= 0) return;                                       // jle / jle
    void* e = copy_effect(poison);                                                  // operator new(8Ch); 00503F70
    at<U16>(e, 8) = static_cast<U16>(k_effect_ability_00746710);                    // word [00746710]
    set_int(e, 0, ability);                                                         // SetInteger(0, ability)
    set_int(e, 1, static_cast<U>(amount) * static_cast<U>(strength));               // SetInteger(1, amount*strength) imul
    at<float>(e, 0xc) = static_cast<float>(duration);                               // fild [esp+10]; fstp dword [esi+C]
    at<U>(e, 0x20) = 0;
    set_duration_bits(e, k_duration_temporary_007473c0);                            // (word & FFF8) | [007473C0]
    kotor_fastcall_004d08d0(self, nullptr, creator, k_effect_ability_00746710, ability); // RemoveEffectByType(creator, [00746710], ability)
    trace(format, static_cast<I>(static_cast<U>(amount) * static_cast<U>(strength)));
    recovered_effect_apply_004d14f0(self, nullptr, e, a3, 0);                       // ApplyEffect(e, a3, 0)
}
}  // namespace

// 004EE770 CSWSCreature::ApplyPoisonDamage(int row, CGameEffect* poison, int a3, float strength): RET 10h, EAX 0/1
U __fastcall kotor_fastcall_004ee770(void* self, void*, I row, void* poison, U a3, float strength) {
    const I level = static_cast<I>(w2::g07::ftol2_low_006fae8c(static_cast<double>(strength))); // 004EE78E..7A3 fld; __ftol2 (EBX)
    const U creator = at<U>(poison, 0x1c);                                          // 004EE7A5..7B9 [esp+2C]
    void* table = at<void*>(two_dim(), 0x90);                                       // [[[7A3A28]+B8]+90] poison.2da
    if (table == nullptr) return 0;                                                 // 004EE7B7..7D2
    I hp, fp, str, dex, con, intel, wis, chr;
    read_int(table, row, k_dam_hp_00746ee8, &hp);                                   // 004EE7D5..810 [esp+14]
    read_int(table, row, k_dam_fp_00746ee0, &fp);                                   // 004EE810..848 [esp+34]
    read_int(table, row, k_dam_str_00746ed8, &str);                                 // 004EE848..880 [esp+18]
    read_int(table, row, k_dam_dex_00746ed0, &dex);                                 // 004EE880..8B8 [esp+1C]
    read_int(table, row, k_dam_con_00746ec8, &con);                                 // 004EE8B8..8F0 [esp+20]
    read_int(table, row, k_dam_int_00746ec0, &intel);                               // 004EE8F0..928 [esp+24]
    read_int(table, row, k_dam_wis_00746eb8, &wis);                                 // 004EE928..960 [esp+28]
    read_int(table, row, k_dam_chr_00746eb0, &chr);                                 // 004EE960..998 [esp+30]
    I duration = 0, base = 0, period = 0;                                           // 004EE998..9A8 [esp+10] [esp+38] [esp+3C]
    if (str > 0 || dex > 0 || con > 0 || intel > 0 || wis > 0 || chr > 0) {         // 004EE9AC..9CA (signed jg chain)
        read_int(table, row, k_duration_007469d0, &base);                           // 004EE9D0..A08
        read_int(table, row, k_period_007469c8, &period);                           // 004EEA08..A40
        duration = static_cast<I>(static_cast<U>(base) -
                                  (static_cast<U>(level) - 1u) * static_cast<U>(period)); // 004EEA40..A52
        trace(k_trace_duration_00746e7c, duration);                                 // 004EEA4E..A87
    }
    if (hp > 0) {                                                                   // 004EEA8C..A92
        const I scale = k_hp_scale_00746884;                                        // [esp+44] = [00746884]
        const U extra = k_hp_extra_00746894;                                        // [esp+48] = [00746894]
        void* e = copy_effect(poison);                                              // 004EEAA3..AD7
        at<U16>(e, 8) = k_effect_poison_hp_00746714;                                // 004EEAD9..AE6
        set_duration_bits(e, k_duration_instant_007473b8);                          // 004EEAE0..AFE
        recovered_effect_resize_00503630(e, nullptr, 0x16);                         // 004EEAEA..B02 SetNumIntegers(22)
        for (U i = 0; static_cast<I>(i) < 0xf; ++i) set_int(e, i, 0xffffffffu);     // 004EEB07..B29
        const U slot = log2_index(scale);                                           // 004EEB2B..B4E
        set_int(e, slot, static_cast<U>(hp));                                       // 004EEB4A..B57
        set_int(e, 0xe, static_cast<U>(hp));                                        // 004EEB5C..B65
        set_int(e, 0x10, 0x3e8);                                                    // 004EEB6A..B73
        set_int(e, 0x11, static_cast<U>(scale));                                    // 004EEB78..B81
        set_int(e, 0x12, extra);                                                    // 004EEB86..B8F
        set_int(e, 0x15, 1);                                                        // 004EEB94..B9A
        ExoString text; str_empty(text);                                            // 004EEB9F..BA3
        frame_LogFormat_005e5680(text.p(), k_trace_hp_00746e54, hp);                // 004EEBA8..BC4
        (void)WindowCStr005e5670(text.p(), nullptr);                                // 004EEBC7..BCB
        void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);      // 004EEBD0..BD8
        recovered_event_enqueue_004b08d0(ai, nullptr, 0, 0, at<U>(e, 0x18), at<U>(self, 4),
                                         k_event_apply_effect_007441c8, e);         // 004EEBDD..BF7
        str_free(text);                                                             // 004EEBFC..C04
    }
    if (fp > 0) {                                                                   // 004EEC09..C0F
        void* e = copy_effect(poison);                                              // 004EEC15..C41
        at<U16>(e, 8) = k_effect_fp_007467f4;                                       // 004EEC43..C49
        set_int(e, 0, static_cast<U>(fp));                                          // 004EEC4D..C5A
        set_duration_bits(e, k_duration_instant_007473b8);                          // 004EEC5F..C76
        ExoString text; str_empty(text);                                            // 004EEC7A
        frame_LogFormat_005e5680(text.p(), k_trace_fp_00746e2c, fp);                // 004EEC7F..C9B
        (void)WindowCStr005e5670(text.p(), nullptr);                                // 004EEC9E..CA2
        recovered_effect_apply_004d14f0(self, nullptr, e, a3, 0);                   // 004EECA7..CB2 ApplyEffect(e, a3, 0)
        str_free(text);                                                             // 004EECB7..CBF
    }
    ability_arm(self, poison, a3, str, duration, creator, 0, level, k_trace_str_00746e04);   // 004EECC4..DBD
    ability_arm(self, poison, a3, dex, duration, creator, 1, level, k_trace_dex_00746ddc);   // 004EEDC2..EBB
    ability_arm(self, poison, a3, con, duration, creator, 2, level, k_trace_con_00746db4);   // 004EEEC0..FB9
    ability_arm(self, poison, a3, intel, duration, creator, 3, level, k_trace_int_00746d8c); // 004EEFBE..0B7
    ability_arm(self, poison, a3, wis, duration, creator, 4, level, k_trace_wis_00746d64);   // 004EF0BC..1B5
    ability_arm(self, poison, a3, chr, duration, creator, 5, level, k_trace_chr_00746d3c);   // 004EF1BA..2B3
    return 1;                                                                       // 004EF2BE
}

// 004F6790 CSWSCreature::ApplyDiseasePayload(CGameEffect* disease, ulong day, ulong ms): RET 0Ch, EAX
U __fastcall kotor_fastcall_004f6790(void* self, void*, void* disease, U day, U ms) {
    if (static_cast<U>(at<U16>(disease, 8)) != k_effect_disease_00746690) return 0; // 004F67B4..7DB
    U result = 1;                                                                   // 004F67EB [esp+1C]
    void* timer = recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)); // 004F67DE..7FE [esp+3C]
    void* table = at<void*>(two_dim(), 0x94);                                       // 004F67F8..812 [esp+38] disease.2da
    const I row = static_cast<I>(get_int(disease, 0));                              // 004F680E..824 [esp+14]
    I dc;
    read_int(table, row, k_subs_save_007470bc, &dc);                                // 004F681B..85D [esp+20]
    if (static_cast<U8>(kotor_fastcall_005b92b0(self, nullptr, 1, static_cast<U>(dc), 5, at<U>(disease, 0x18), 1, 0, 0)) != 0) {
                                                                                    // 004F6862..87E SavingThrowRoll(1, dc, 5, creator, 1, 0, 0)
        if (get_int(disease, 1) == 1) {                                             // 004F6880..890
            kotor_fastcall_004ef560(self, nullptr, 0);                              // 004F688E..892
            const U id = at<U>(self, 4);                                            // 004F689D
            void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);  // 004F68A0..8B1
            recovered_event_enqueue_004b08d0(ai, nullptr, 0, 0, id, id, k_event_remove_effect_007441ec, disease); // 004F68A9..8B8
        } else {
            kotor_fastcall_004ef560(self, nullptr, 0);                              // 004F688E, 68BF
            set_int(disease, 1, 1);                                                 // 004F68C4..8CA
        }
        if (get_int(disease, 2) == 0) set_int(disease, 2, 1);                       // 004F68CF..8E2
        result = 0;                                                                 // 004F68E7
    } else {
        ExoString dice, dam, type;
        str_text(dice, k_dice_007470b4);                                            // 004F68F4..8FD [esp+68]
        str_text(dam, k_dam_007470ac);                                              // 004F6902..916 [esp+60]
        str_text(type, k_type_007470a4);                                            // 004F691B..92C [esp+58]
        for (I k = 1; k < 4; ++k) {                                                 // 004F6939, 6B42..6B4E [esp+18]
            ExoString number; kotor_fastcall_005e5bc0(number.p(), nullptr, k);     // 004F6941..94A [esp+50]
            I count, sides, damage_type;                                            // [esp+2C] [esp+30] [esp+34]
            ExoString t1;                                                           // [esp+80]
            Str* c1 = GuiConcat005e5d10(dice.s(), nullptr, t1.s(), number.s());     // 004F694F..968 "Dice_" + k
            bool ok;                                                                // [esp+13]
            if (c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), c1, &count) == 0) { // 004F696D..989
                ok = false;
            } else {
                ExoString t2;                                                       // [esp+70]
                Str* c2 = GuiConcat005e5d10(dam.s(), nullptr, t2.s(), number.s());  // 004F698F..9A2 "Dam_" + k
                const U r2 = c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), c2, &sides); // 004F69A2..9C0
                str_free(t2);                                                       // 004F69BC..9CC
                if (r2 == 0) {
                    ok = false;                                                     // 004F69D5..9D7
                } else {
                    ExoString t3;                                                   // [esp+78]
                    Str* c3 = GuiConcat005e5d10(type.s(), nullptr, t3.s(), number.s()); // 004F69D9..9E7 "Type_" + k
                    const U r3 = c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), c3, &damage_type); // 004F69EC..A0A
                    str_free(t3);                                                   // 004F6A06..A16
                    ok = r3 != 0;                                                   // 004F6A1B..A28
                }
            }
            str_free(t1);                                                           // 004F6A2D..A3C
            if (ok) {                                                               // 004F6A41..A47
                const U amount = recovered_rules_roll_dice_00550b60(rules(), nullptr, static_cast<U>(sides),
                                                                    static_cast<U>(count)) & 0xffffu; // 004F6A4D..A6A ("Dam_", "Dice_"), movzx ax
                void* memory = live_operator_new(0x8c);                             // 004F6A65..A7C
                void* e = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 004F6A84..A95
                recovered_effect_creator_00503a00(e, nullptr, at<U>(disease, 0x18));// 004F6A95..AA3
                at<U>(e, 0xc) = at<U>(disease, 0xc);                                // 004F6AA8..AAB
                U bitsw = (static_cast<U>(at<U16>(e, 0xa)) & 0xffe7u) | (static_cast<U>(at<U8>(disease, 0xa)) & 0x18u);
                at<U16>(e, 0xa) = static_cast<U16>(bitsw);                          // 004F6AAE..AC3
                bitsw = (bitsw & 0xfff8u) | (static_cast<U>(at<U8>(disease, 0xa)) & 7u);
                at<U16>(e, 0xa) = static_cast<U16>(bitsw);                          // 004F6AC7..AD6
                at<U>(e, 0x1c) = at<U>(disease, 0x1c);                              // 004F6ADA..ADD
                at<U16>(e, 8) = static_cast<U16>(k_effect_ability_00746710);        // 004F6AE0..AE7 word [00746710]
                set_int(e, 0, static_cast<U>(damage_type));                         // 004F6AEB..AF4
                set_int(e, 1, amount);                                              // 004F6AF9..B02
                const U id = at<U>(self, 4);                                        // 004F6B0D
                void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr); // 004F6B10..B21
                recovered_event_enqueue_004b08d0(ai, nullptr, 0, 0, id, id, k_event_apply_effect_007441c8, e); // 004F6B19..B28
            }
            str_free(number);                                                       // 004F6B31..B3D
        }
        void* visual = copy_effect(disease);                                        // 004F6B54..B87 [esp+18]
        at<U16>(visual, 8) = k_effect_visual_007466f4;                              // 004F6B8B..B94
        set_int(visual, 0, 0x54);                                                   // 004F6B92..BA4
        recovered_effect_apply_004d14f0(self, nullptr, visual, 0, 0);               // 004F6BA9..BB4
        ExoString script;                                                           // [esp+50]
        if (get_int(disease, 2) == 0) {                                             // 004F6BB9..BC8
            str_empty(script);                                                      // 004F6BCA
            ExoString column; str_text(column, k_end_incu_script_00747094);         // 004F6BCF..BE0 [esp+48]
            const I found = c2da_current_get_string_00413270(table, nullptr, row, column.p(), script.p()); // 004F6BE5..C03
            str_free(column);                                                       // 004F6C03..C11
            if (found != 0)
                frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)),
                                  nullptr, reinterpret_cast<unsigned*>(script.p()), at<U>(self, 4), 1); // 004F6C16..C2B
            set_int(disease, 2, 1);                                                 // 004F6C30..C36
        } else {
            str_empty(script);                                                      // 004F6C3D
            ExoString column; str_text(column, k_24_hour_script_00747084);          // 004F6C42..C53 [esp+24]
            const I found = c2da_current_get_string_00413270(table, nullptr, row, column.p(), script.p()); // 004F6C58..C76
            str_free(column);                                                       // 004F6C76..C84
            if (found != 0)
                frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)),
                                  nullptr, reinterpret_cast<unsigned*>(script.p()), at<U>(self, 4), 1); // 004F6C89..C9E
        }
        str_free(script);                                                           // 004F6CA3..CAF
        kotor_fastcall_004ef560(self, nullptr, 0);                                  // 004F6CB4..CB8
        str_free(type);                                                             // 004F6CBD..CC9
        str_free(dam);                                                              // 004F6CCE..CDA
        str_free(dice);                                                             // 004F6CDF..CEE
    }
    void* module = server_get_module_004ae6b0(server_app(), nullptr);               // 004F6CF3..D01
    const U hours = static_cast<U>(at<U8>(module, 0x1a0)) * k_ms_per_minute_00744178 * 24u; // 004F6D01..D22 (imul, *3, shl 3)
    U next_day, next_ms;                                                            // [esp+40] [esp+44]
    (void)kotor_fastcall_004adf50(timer, nullptr, day, ms, 0, hours, &next_day, &next_ms); // 004F6D0F..D2E AddWorldTimes
    set_int(disease, 3, next_day);                                                  // 004F6D33..D3C
    set_int(disease, 4, next_ms);                                                   // 004F6D41..D4A
    return result;                                                                  // 004F6D56
}

// 00503F70 CGameEffect::CGameEffect(CGameEffect* source): RET 4, EAX = this. Copies the header fields only; the
// integer payload is reallocated (zeroed) at the source's count, the six strings reset to "".
void* __fastcall kotor_fastcall_00503f70(void* self, void*, void* source) {
    crt_eh_vector_constructor_006fb55f(static_cast<unsigned char*>(self) + 0x48, 8, 6,
        reinterpret_cast<void*>(&kotor_menu20::ConstructString005b3190),
        reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));                    // 00503F8A..FA2
    at<U>(self, 0) = 0; at<U>(self, 4) = 0; at<U>(self, 0x20) = 0;                  // 00503FAD..FB2
    at<U16>(self, 8) = 0; at<U16>(self, 0xa) = 0;                                   // 00503FB5..FB9
    at<U>(self, 0xc) = 0; at<U>(self, 0x10) = 0; at<U>(self, 0x14) = 0;             // 00503FBD..FC3
    at<U>(self, 0x18) = 0x7f000000u;                                                // 00503FC6
    at<U>(self, 0x28) = 0; at<U>(self, 0x2c) = 0; at<U>(self, 0x34) = 0;            // 00503FCD..FD3
    at<U>(self, 0x1c) = 0xffffffffu; at<U>(self, 0x24) = 0;                         // 00503FD6..FDD
    recovered_effect_resize_00503630(self, nullptr, at<U>(source, 0x30));           // 00503FE0..FEA SetNumIntegers(source count)
    at<U>(self, 0x38) = 0; at<U>(self, 0x3c) = 0; at<U>(self, 0x40) = 0; at<U>(self, 0x44) = 0; // 00503FEF..FFC
    for (U i = 0; i < 6; ++i)
        (void)frame_LogAssign_005e5140(static_cast<unsigned char*>(self) + 0x48 + 8 * i, nullptr, k_empty_0073d71c); // 00504010..5028
    at<U>(self, 0x78) = 0x7f000000u; at<U>(self, 0x7c) = 0x7f000000u;               // 0050402A..4032
    at<U>(self, 0x80) = 0x7f000000u; at<U>(self, 0x84) = 0x7f000000u;               // 00504035..403B
    at<U>(self, 0) = at<U>(source, 0);                                              // 00504041..4043
    at<U>(self, 4) = at<U>(source, 4);                                              // 00504045..4048
    at<U16>(self, 0xa) = at<U16>(source, 0xa);                                      // 0050404B..404F
    at<U>(self, 0xc) = at<U>(source, 0xc);                                          // 00504053..4056
    at<U>(self, 0x10) = at<U>(source, 0x10);                                        // 00504059..405C
    at<U>(self, 0x14) = at<U>(source, 0x14);                                        // 0050405F..4062
    at<U>(self, 0x18) = at<U>(source, 0x18);                                        // 00504065..4068
    at<U>(self, 0x88) = 0;                                                          // 00504072
    at<U>(self, 0x1c) = at<U>(source, 0x1c);                                        // 0050406B, 4079
    return self;                                                                    // 0050407C
}

// 004D08D0 CSWSObject::RemoveEffectByType(ulong creator, int type, int int0): RET 0Ch. Index advances after a removal
// (the shifted-in next effect is skipped, as in the original).
void __fastcall kotor_fastcall_004d08d0(void* self, void*, U creator, U type, U value) {
    if (at<I>(self, 0x128) <= 0) return;                                            // 004D08D3..8DE
    for (I i = 0; i < at<I>(self, 0x128); ++i) {                                    // 004D08DA, 0924..092D (jl)
        void* e = at<void**>(self, 0x124)[i];                                       // 004D08F0..8F6
        if (at<U>(e, 0x1c) != creator) continue;                                    // 004D08F9..8FC
        if (static_cast<U>(at<U16>(e, 8)) != type) continue;                        // 004D08FE..906
        if (recovered_effect_integer_00503690(e, nullptr, 0) != value) continue;    // 004D0908..911
        recovered_object_remove_effect_004d05c0(self, nullptr, at<void**>(self, 0x124)[i]); // 004D0913..91F
    }
}

// 004EF560 CSWSCreature::DisplayFloatyDataToSelf(int value): RET 4. CC message 0Bh with object 0 = self,
// integer 9 = 5Dh, integer 0 = value, to the controlling player.
void __fastcall kotor_fastcall_004ef560(void* self, void*, I value) {
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4));  // 004EF57A..58C
    if (player == nullptr) return;                                                  // 004EF58E..590
    void* memory = live_operator_new(0x34);                                         // 004EF597..5A1
    void* data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 004EF5A5..5BC
    if (data == nullptr) return;                                                    // 004EF5BE..5C8
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 004EF5CA..5D2
    recovered_event_integer_004d76b0(data, nullptr, 9, 0x5d);                       // 004EF5D7..5DD
    recovered_event_integer_004d76b0(data, nullptr, 0, static_cast<U>(value));      // 004EF5E2..5EB
    const U player_id = at<U>(player, 4);                                           // 004EF5F0
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);   // 004EF5F3..607
    (void)recovered_cc_message_00568570(message, nullptr, player_id, 0xb, data, 0); // 004EF5FC..609
    recovered_message_data_destroy_004d6a40(data, nullptr);                         // 004EF60E..610
    live_free(data);                                                                // 004EF615..61B
}

}  // namespace w1r_effects

// ================================================================ section: objects
// W1 route bodies, part "objects" (lane worker w1-routes / fork objects):
//   004CDB00 CSWSObject::BroadcastSafeProjectile        (kotor_fastcall_004cdb00, RET 28h)
//   004F0FD0 CSWSCreature::RemoveFromArea               (kotor_fastcall_004f0fd0, RET 4)
//   00584EC0 CSWSPlaceable::AIActionCastSpell           (kotor_fastcall_00584ec0, RET 4)
//   0058D570 CSWSTrigger::OnEnterMine                   (kotor_fastcall_0058d570, RET 4)
//   00635350 CSWParty::IsPartyTogether                  (kotor_fastcall_00635350, RET 0)
// plus their callees without a provider in the c13c candidate map:
//   0056E5F0 CSWSMessage::SendServerToPlayerSafeProjectileProjectile, 004D71B0 CSWMessage::WriteVector,
//   005178F0 CSWSCreature::ClearVisibleList, 005962D0 CSWSAreaOfEffectObject::RemoveFromArea,
//   00595B60 CSWSAreaOfEffectObject::RemoveFromSubAreas, 0058D4A0 CSWSTrigger::GetCanFireMineOnObject.
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
#include "842d6b1618-w5_t3.hpp"
#include <cstdint>
#include <cstring>

namespace frame_control { extern unsigned message_manager_007a3a00; }   // [007A3A00] CVirtualMachine* cell [0456]
// Foreign namespaced providers (global scope so the names bind to the existing objects).
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
#endif
namespace resource_ref_assignment {   // 00406120 CResRef::operator= (this, hidden result, source) [0278]
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*);
}
namespace kotor_reconstruction { struct StartupString; }
namespace directory_list {            // 005E6080 CExoString::LowerCase(CExoString* result) [0288]
kotor_reconstruction::StartupString* __fastcall LowerCase005e6080(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*);
}
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // 004AEE70 [0575]

namespace w1r_objects {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c13c candidate map; object in brackets) ----------------
unsigned __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* client, void*);       // CClientExoApp::GetPlayerCreatureId [0811]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                          // CSWSCreature::GetFaction [W1 b7]
unsigned __fastcall recovered_object_get_reputation_0057cb80(void* self, void*, unsigned target, unsigned a2); // GetReputation [W1 b7]
unsigned __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, unsigned kind, unsigned other); // GetEffectImmunity [1094]
unsigned __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);         // CItemRepository::RemoveItem [w2_party_native]
unsigned __fastcall recovered_inventory_remove_item_005a4ba0(void* inventory, void*, void* item); // CSWInventory::RemoveItem [W1 b7]
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned id, unsigned flag); // CVirtualMachine::RunScript [0457]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster [r2]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // CSWCCMessageData::SetInteger [1017]
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, unsigned id); // GetVisibleListElement [W1 b7]
void __fastcall recovered_creature_remove_visible_00517960(void* creature, void*, unsigned id);  // RemoveFromVisibleList [W1 b7]
unsigned __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, unsigned value);           // CExoArrayList<int>::remove [W1 b7]
unsigned __fastcall recovered_aoe_in_area_of_effect_00595450(void* aoe, void*, float x, float y, float z); // InAreaOfEffect [w2_party_native]
void __fastcall kotor_fastcall_004cb160(void* object, void*, void* area);                        // CSWSObject::SetArea [claude_combat_spell_mine]
U __fastcall kotor_fastcall_0050dc40(void* area, void*, U id);                                   // CSWSArea::RemoveObjectFromArea [claude_combat_spell_mine]
void* __cdecl combat_reuse_message_004aedb0();                                                   // CServerExoApp::GetSWSMessage [W1 b7]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U a, U b, U c);                    // CSWMessage::CreateWriteMessage [w2_message]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                             // WriteOBJECTIDServer [w2_message]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);                  // WriteDWORD [w2_message]
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);                  // WriteBYTE [w2_message]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);                // GetWriteMessage [w2_message]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage [w2_message]
void __fastcall kotor_fastcall_004d6260(void* message, void*, unsigned extra);                   // CSWMessage::ExtendWriteBuffer [w2_message]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);         // CSWSpellArray::GetSpell [0820]
void __fastcall kotor_fastcall_004cb9e0(void* object, void*, U flags);                           // CalculateLastSpellProjectileTime [claude_combat_spell]
void __fastcall kotor_fastcall_004cdd30(void* object, void*, U caster, U target, const Vector* from, const Vector* to,
    U a5, U a6, U a7, U a8, U a9, U a10, U a11);                                                 // BroadcastSafeProjectile (spell) [claude_combat_spell]
void __fastcall kotor_fastcall_004cdf50(void* object, void*, U spell, const Vector* location, U target, U a4, U a5,
    U a6, U a7, U a8, U a9);                                                                     // SpellCastAndImpact [claude_combat_spell]

void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                      // CServerExoApp::GetPlayerList [W1 b7]
void* __fastcall exo_locstring_construct_005e9e10(void* loc, void*);                             // CExoLocString() [0265]
void __fastcall exo_locstring_destroy_005ea000(void* loc, void*);                                // ~CExoLocString [0265]
void __fastcall exo_locstring_clear_005e9ea0(void* loc, void*);                                  // CExoLocString::ClearLocString [0265]
void* __fastcall recovered_localized_assign_005e9e70(void* dst, void*, void* src);              // CExoLocString::operator= [0886]
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, unsigned hp);                   // CSWSObject::SetCurrentHitPoints [0886 w1x]
U __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);       // CSWSModule::AddObjectToLookupTable [1011]
void __fastcall recovered_placeable_finish_00584870(void* placeable, void*);                    // CSWSPlaceable::PostProcess [0886]
void* __fastcall recovered_placeable_construct_005877e0(void* self, void*, unsigned id);         // CSWSPlaceable::CSWSPlaceable [0963]
void __fastcall recovered_placeable_rotation_005853c0(void* self, void*, void* quaternion);     // CSWSPlaceable::SetOrientation [0886]
void* __fastcall recovered_item_construct_005530a0(void* self, void*, unsigned id);              // CSWSItem::CSWSItem [0963]
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);                  // CSWSItem::LoadFromTemplate [0995]
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                 // CItemRepository::ItemListGetItem [0886]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, unsigned id);          // GetPlaceableByGameObjectID [0886]
unsigned __fastcall w2x_creature_gold_004edd60(void* creature, void*);                          // CSWSCreature::GetGold [0886]
unsigned __fastcall recovered_placeable_acquire_item_00584b10(void* placeable, void*, void* item_cell, unsigned container, unsigned flag); // AcquireItem [W6 t4]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                // CServerExoApp::GetModule [0886]
double __cdecl kotor_m15a_pitch_004aa130(const void* v);                                         // Pitch(Vector const&), ST0 [0816]

// ---------------- exports reconstructed in this file ----------------
U __fastcall kotor_fastcall_00570180(void* message, void*, U id);
unsigned char __fastcall kotor_fastcall_004ef2e0(void* creature, void*);
U __fastcall kotor_fastcall_004f3770(void* creature, void*, U a0, U a1, U a2);
U __fastcall kotor_fastcall_00587710(void* placeable, void*, U a0);
unsigned char __fastcall kotor_fastcall_005851e0(void* placeable, void*);
void __fastcall kotor_fastcall_00553040(void* object, void*, void* tag);
void __fastcall kotor_fastcall_004edd90(void* creature, void*, I gold);
U __fastcall kotor_fastcall_005864b0(void* placeable, void*, U appearance);
U __fastcall kotor_fastcall_00588690(void* placeable, void*, U id, U flag);
U __fastcall kotor_fastcall_004ce220(void* self, void*);
U __fastcall kotor_fastcall_00635350(void* party, void*);
U __fastcall kotor_fastcall_0058d4a0(void* trigger, void*, U id, U flag);
void __fastcall kotor_fastcall_0058d570(void* trigger, void*, U value);
void __fastcall kotor_fastcall_005178f0(void* creature, void*);
void __fastcall kotor_fastcall_00595b60(void* aoe, void*, U flag);
void __fastcall kotor_fastcall_005962d0(void* aoe, void*);
void __fastcall kotor_fastcall_004f0fd0(void* creature, void*, U flag);
void __fastcall kotor_fastcall_004d71b0(void* message, void*, const Vector* v);
U __fastcall kotor_fastcall_0056e5f0(void* message, void*, void* player, U a0, U a1, Vector va, Vector vb,
    U a4, U a5, U a6, U a7, U a8, U a9);
void __fastcall kotor_fastcall_004cdb00(void* self, void*, U a0, U a1, const Vector* from, const Vector* to,
    U a4, U a5, U a6, U a7, U a8, U a9);
U __fastcall kotor_fastcall_00584ec0(void* self, void*, void* node);
}

constexpr U k_complete_00745dc0 = 2;                    // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;                      // .rdata 00745DC4
constexpr U k_event_script_007441dc = 10;               // .rdata 007441DC
constexpr U k_event_destroy_007441e0 = 11;              // .rdata 007441E0
constexpr unsigned char k_lookup_ok_0074666c = 0;       // .rdata 0074666C (byte)
constexpr unsigned k_immunity_mine_00746910 = 5;        // .rdata 00746910 (byte, zero-extended)
const float k_party_range_007455bc = 900.0f;            // .rdata 007455BC (squared 30 m)
const float k_projectile_range_00747440 = 250.0f;       // .rdata 00747440
constexpr unsigned char k_type_creature_00746634 = 5;   // .rdata 00746634 (byte)
constexpr unsigned char k_type_placeable_00746638 = 9;  // .rdata 00746638 (byte)
constexpr U k_event_body_bag_007441f8 = 0x11;           // .rdata 007441F8
const char k_empty_0073d71c[] = "";                     // .rdata 0073D71C
const char k_appearance_00746efc[] = "Appearance";      // .rdata 00746EFC
const char k_body_bag_00746ef0[] = "Body_Bag";          // .rdata 00746EF0
const char k_body_bag_007494c8[] = "BodyBag";           // .rdata 007494C8
const char k_body_bag_00745efc[] = "BODY_BAG";          // .rdata 00745EFC
const char k_corpse_00745ef4[] = "Corpse";              // .rdata 00745EF4
const char k_name_00745eec[] = "Name";                  // .rdata 00745EEC
const char k_strref_0074868c[] = "StrRef";              // .rdata 0074868C
const char k_body_bag_tag_007497f4[] = "Body Bag";      // .rdata 007497F4
const char k_body_bag_portrait_007497e8[] = "PO_PLC_B04_"; // .rdata 007497E8
const char k_credits_template_007471c0[] = "g_i_credits001"; // .rdata 007471C0

inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline void* new_script_event() {                       // operator new(34h) + CScriptEvent; null when the allocation fails
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
}
inline void* sws_message() { return combat_reuse_message_004aedb0(); }
inline void* rules_table(U offset) { return at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), offset); }   // CSWRules +B8 2DA set
// C2DA::GetINTEntry through a temporary CExoString column name; returns the provider's status
inline U table_int(void* table, I row, const char* column, I* out) {
    alignas(4) unsigned char name[8];
    frame_LogStringCtor_005e5a90(name, nullptr, column);
    const U ok = c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), name, out);
    frame_LogStringDtor_005e5c20(name, nullptr);
    return ok;
}
// temporary CExoString(text) assigned to the string at `field`, then destroyed (00586604..26 shape)
inline void assign_text(void* field, const char* text) {
    alignas(4) unsigned char tmp[8];
    frame_LogStringCtor_005e5a90(tmp, nullptr, text);
    (void)async_assign_string_005e5c50(field, nullptr, tmp);
    frame_LogStringDtor_005e5c20(tmp, nullptr);
}
// CResRef(text) temporary assigned with CResRef::operator= (result temporary discarded)
inline void assign_resref_text(void* field, const char* text) {
    menu_resources::Ref tmp, result;
    tmp.construct_00406d80(text);
    (void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(field), nullptr, &result, &tmp);
}
// CExoArrayList<CSWSItem*> append with the inline growth (size == allocated -> Allocate(2*allocated or 10h))
inline void item_list_append(U* list, void* item) {
    if (list[1] == list[2])
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            list[2] != 0 ? list[2] * 2 : 0x10);
    reinterpret_cast<void**>(static_cast<std::uintptr_t>(list[0]))[list[1]] = item;
    list[1] += 1;
}
// fld dword / fstp dword: a signalling NaN comes out quiet; every other value is unchanged
inline U x87_float_copy(U v) {
    if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0) v |= 0x00400000u;
    return v;
}

extern "C" {
// =========================================================================================================
// 00635350 CSWParty::IsPartyTogether: RET 0, EAX 0/1. Every listed member within 30 m of the player creature.
U __fastcall kotor_fastcall_00635350(void* party, void*) {
    const U client_id = kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);   // 00635350..5F
    const U id = kotor_fastcall_004aea30(server_app(), nullptr, client_id);                         // 00635364..6E
    void* leader = kotor_fastcall_004ae770(server_app(), nullptr, id);                             // 00635373..7D
    if (leader == nullptr) return 0;                                                                // 00635382..8D (EAX = 0)
    (void)recovered_object_area_004cb120(leader, nullptr);                                          // 0063538F..91 (result unused)
    const float px = at<float>(leader, 0x90), py = at<float>(leader, 0x94), pz = at<float>(leader, 0x98); // 00635396..B2
    for (I i = 0; i < at<I>(party, 0); ++i) {                                                        // 006353A8..B6, 00635438..43
        const U member_client = at<U>(party, 0x24 + static_cast<U>(i) * 0x88);                     // 006353BC..C0
        const U member_id = kotor_fastcall_004aea30(server_app(), nullptr, member_client);          // 006353C2..CC
        void* member = kotor_fastcall_004ae770(server_app(), nullptr, member_id);                   // 006353D1..DB
        if (member == nullptr) continue;                                                            // 006353E0..E2
        const double dx = static_cast<double>(px) - at<float>(member, 0x90);                       // 006353E4..F9
        const double dy = static_cast<double>(py) - at<float>(member, 0x94);                       // 006353FD..05
        const double dz = static_cast<double>(pz) - at<float>(member, 0x98);                       // 0063540D..11
        const double d2 = (dz * dz + dy * dy) + dx * dx;                                            // 00635415..23
        if (d2 > k_party_range_007455bc) return 0;                                                  // 00635425..36 (test ah,41h / je: greater, ordered)
    }
    return 1;                                                                                       // 00635449..54
}

// =========================================================================================================
// 0058D4A0 CSWSTrigger::GetCanFireMineOnObject(ulong id, int flag): RET 8, EAX 0/1.
U __fastcall kotor_fastcall_0058d4a0(void* self, void*, U id, U flag) {
    U result = 1;                                                                                   // 0058D4B6 ebp
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 0x2e4));             // 0058D4A6..BB
    void* target = kotor_fastcall_004ae770(server_app(), nullptr, id);                              // 0058D4C0..D0
    if (target == nullptr) return result;                                                           // 0058D4D5..D9
    if (flag == 0) {                                                                                // 0058D4DF..E5
        if (creator != nullptr) {                                                                   // 0058D4E7..E9
            if (combat_reuse_faction_00513fc0(creator, nullptr) != nullptr) {                       // 0058D4EB..F4
                const I rep = static_cast<I>(recovered_object_get_reputation_0057cb80(target, nullptr, at<U>(creator, 4), 0)); // 0058D4F6..FE
                if (rep > 10) result = 0;                                                           // 0058D503..06 (jg)
                else if (at<U>(self, 0x2b8) == at<U>(at<void*>(target, 0xa74), 0x78)) result = 0;  // 0058D508..17, 0058D53A
            }
        } else {
            const I rep = static_cast<I>(recovered_object_get_reputation_0057cb80(target, nullptr, at<U>(self, 4), 0)); // 0058D519..21
            if (rep > 10) result = 0;                                                               // 0058D526..29 (jg)
            else if (at<U>(at<void*>(target, 0xa74), 0x78) == at<U>(self, 0x2b8)) result = 0;       // 0058D52B..3A
        }
    }
    if (recovered_stats_effect_query_005a6960(at<void*>(target, 0xa74), nullptr, k_immunity_mine_00746910, 0) != 0) // 0058D53E..56
        return 0;                                                                                   // 0058D558..5E
    return result;                                                                                  // 0058D561..67
}

// =========================================================================================================
// 0058D570 CSWSTrigger::OnEnterMine(int value): RET 4.
void __fastcall kotor_fastcall_0058d570(void* self, void*, U value) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 0x29c));            // 0058D574..84
    if (creature == nullptr) return;                                                                // 0058D589..8D
    if (kotor_fastcall_0058d4a0(self, nullptr, at<U>(creature, 4), value) == 0) return;             // 0058D593..A5
    void* tag = bytes(self) + 0x27c;
    // the argument slot [esp+10h] is reused for the item id
    U item_id = recovered_find_item_with_tag_00555ed0(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr, tag); // 0058D5AC..C8
    if (item_id == k_object_invalid) {                                                              // 0058D5C3..CC
        for (I slot = 0; slot < 0x12; ++slot) {                                                     // 0058D5CF..0C (no break: last match)
            void* item = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 1u << slot); // 0058D5D1..E3
            if (item == nullptr) continue;                                                          // 0058D5E8..EC
            if (async_string_equal_005e5200(bytes(item) + 0x28, nullptr, tag) == 0) continue;       // 0058D5EE..FF
            item_id = at<U>(item, 0x14);                                                            // 0058D601..04
        }
    }
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);                    // 0058D613..22
    if (item != nullptr) {                                                                          // 0058D624..26
        if (at<U>(self, 0x2cc) != 0) {                                                              // 0058D628..30
            recovered_remove_item_00555fd0(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr, item); // 0058D632..3E
            recovered_inventory_remove_item_005a4ba0(at<void*>(creature, 0xa2c), nullptr, item);    // 0058D643..4A
            (void)vcall<void*>(item, 0, 1u);                                                        // 0058D64F..55 scalar deleting destructor
        }
        at<U>(self, 0x2a4) = at<U>(self, 0x29c);                                                    // 0058D65A..69
        frame_R4_005d0fc0(vm(), nullptr, reinterpret_cast<unsigned*>(bytes(self) + 0x26c), at<U>(self, 4), 1); // 0058D657..76
    } else {
        recovered_creature_feedback_004ede10(creature, nullptr, 0x52, nullptr);                     // 0058D685..8B
        frame_R4_005d0fc0(vm(), nullptr, reinterpret_cast<unsigned*>(bytes(self) + 0x264), at<U>(self, 4), 1); // 0058D690..A3
        frame_R4_005d0fc0(vm(), nullptr, reinterpret_cast<unsigned*>(bytes(self) + 0x24c), at<U>(self, 4), 1); // 0058D6A8..BB
        if (at<U>(self, 0x2d0) == 0) return;                                                        // 0058D6C0..C8
    }
    const U id = at<U>(self, 4);                                                                    // 0058D6D9
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0, id, id,
                                     k_event_destroy_007441e0, nullptr);                            // 0058D6D3..EC
}

// =========================================================================================================
// 005178F0 CSWSCreature::ClearVisibleList: RET 0. Removes and frees every visible-list element (+920 data,
// +924 size) except the creature's own entry, walking from the end.
void __fastcall kotor_fastcall_005178f0(void* self, void*) {
    for (I i = at<I>(self, 0x924) - 1; i >= 0; --i) {                                              // 005178F4..FB, 00517957..58
        void* element = at<void**>(self, 0x920)[i];                                                 // 00517900..06
        if (at<U>(element, 0) == at<U>(self, 4)) continue;                                          // 00517909..0E
        const I size = at<I>(self, 0x924) - 1;                                                      // 00517910..16
        at<I>(self, 0x924) = size;                                                                  // 0051791D
        for (I k = i; k < at<I>(self, 0x924); ++k)                                                  // 00517919..23, 00517930..48
            at<void**>(self, 0x920)[k] = at<void**>(self, 0x920)[k + 1];
        if (element != nullptr) live_free(element);                                                 // 0051794A..54
    }
}

// =========================================================================================================
// 00595B60 CSWSAreaOfEffectObject::RemoveFromSubAreas(int flag): RET 4. No area null guard (faults at +194).
void __fastcall kotor_fastcall_00595b60(void* self, void*, U flag) {
    void* area = recovered_object_area_004cb120(self, nullptr);                                     // 00595B7C..8D
    void* array = object_array();                                                                   // 00595B85..9C inline view of 004AED70
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                                    // 00595B96..A8, 00595CD4..E9 (count re-read)
        void* object = nullptr;                                                                     // [esp+18] (stale on failure; unused then)
        if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, at<U*>(area, 0x190)[i], &object)) !=
            k_lookup_ok_0074666c) continue;                                                         // 00595BB0..CE
        if (at<unsigned char>(object, 8) != 5) continue;                                            // 00595BD4..DC
        void* creature = vcall<void*>(object, 0x30);                                                // 00595BE2..E7
        const float* pos = &at<float>(creature, 0x90);                                              // 00595BE9..01
        if (recovered_aoe_in_area_of_effect_00595450(self, nullptr, pos[0], pos[1], pos[2]) == 0) continue; // 00595C04..0D
        const I n = at<I>(creature, 0x334);                                                         // 00595C13
        const U self_id = at<U>(self, 4);                                                           // 00595C19
        if (n <= 0) continue;                                                                       // 00595C24..26
        U matches = 0;
        const U* data = at<U*>(creature, 0x330);
        for (I k = 0; k < n; ++k) if (data[k] == self_id) ++matches;                                 // 00595C2C..39
        if (matches == 0) continue;                                                                 // 00595C3B..3D
        recovered_int_list_remove_0066dcc0(bytes(creature) + 0x330, nullptr, self_id);              // 00595C43..44
        void* event = new_script_event();                                                           // 00595C49..6E
        at<U16>(event, 0) = 0xd;                                                                    // 00595C70 (null store kept)
        recovered_event_object_004d7780(event, nullptr, 0, at<U>(creature, 4));                     // 00595C75..85
        if (flag == 0) {                                                                            // 00595C8A..90
            recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
                                             at<U>(creature, 4), at<U>(self, 4), k_event_script_007441dc, event); // 00595C92..B5
        } else {
            vcall<void>(self, 0x78, k_event_script_007441dc, at<U>(creature, 4), event, 0u, 0u);   // 00595CBC..D1 EventHandler
        }
    }
}

// 005962D0 CSWSAreaOfEffectObject::RemoveFromArea: RET 0.
void __fastcall kotor_fastcall_005962d0(void* self, void*) {
    if (recovered_object_area_004cb120(self, nullptr) != nullptr) {                                // 005962D4..DD
        kotor_fastcall_00595b60(self, nullptr, 1);                                                  // 005962DF..E3
        kotor_fastcall_0050dc40(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 005962E8..F5
        recovered_int_list_remove_0066dcc0(bytes(recovered_object_area_004cb120(self, nullptr)) + 0x1a0, nullptr,
                                           at<U>(self, 4));                                         // 005962FA..0B
    }
    kotor_fastcall_004cb160(self, nullptr, nullptr);                                                // 00596310..14
    const Vector zero = {0.0f, 0.0f, 0.0f};                                                         // 00596326..36
    recovered_object_position_004cd1a0(self, nullptr, &zero, 1, 1, 0);                              // 00596319..3E
    vcall<void>(self, 0x7c, 0u);                                                                    // 00596343..49
}

// =========================================================================================================
// 004F0FD0 CSWSCreature::RemoveFromArea(int keep_perception): RET 4.
void __fastcall kotor_fastcall_004f0fd0(void* self, void*, U flag) {
    if (recovered_object_area_004cb120(self, nullptr) != nullptr)                                   // 004F0FEB..F4
        kotor_fastcall_0050dc40(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 004F0FF6..03
    if (flag == 0) {                                                                                // 004F1008..0E
        kotor_fastcall_005178f0(self, nullptr);                                                     // 004F1014..16
        if (recovered_object_area_004cb120(self, nullptr) != nullptr) {                            // 004F101B..24
            void* area = recovered_object_area_004cb120(self, nullptr);                             // 004F102A..3C
            void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);                  // 004F1031..4B
            for (I i = 0; i < at<I>(area, 0x194); ++i) {                                            // 004F1045..57, 004F1182..93
                void* viewer = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(area, 0x190)[i]); // 004F1060..73
                if (viewer == nullptr) continue;                                                    // 004F1078..7C
                void* element = recovered_creature_visible_element_00517a20(viewer, nullptr, at<U>(self, 4)); // 004F1082..88
                if (element == nullptr) continue;                                                   // 004F108D..91
                if (at<unsigned char>(element, 4) & 2) {                                            // 004F1097..9B
                    void* event = new_script_event();                                               // 004F109D..BC
                    at<U16>(event, 0) = 1;                                                          // 004F10CC (null store kept)
                    recovered_event_integer_004d76b0(event, nullptr, 0, 1);                         // 004F10BE..D1
                    recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));             // 004F10D6..DE
                    recovered_event_enqueue_004b08d0(ai, nullptr, 0, 0, at<U>(self, 4), at<U>(viewer, 4),
                                                     k_event_script_007441dc, event);               // 004F10E3..FB
                }
                if (at<unsigned char>(element, 4) & 1) {                                            // 004F1104..08 (re-read)
                    void* event = new_script_event();                                               // 004F110A..2F
                    at<U16>(event, 0) = 1;                                                          // 004F113F (null store kept)
                    recovered_event_integer_004d76b0(event, nullptr, 0, 3);                         // 004F1131..44
                    recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));             // 004F1149..51
                    recovered_event_enqueue_004b08d0(ai, nullptr, 0, 0, at<U>(self, 4), at<U>(viewer, 4),
                                                     k_event_script_007441dc, event);               // 004F1156..6E
                }
                recovered_creature_remove_visible_00517960(viewer, nullptr, at<U>(self, 4));        // 004F1177..7D
            }
        }
    }
    void* array = object_array();                                                                   // 004F119A..A8 inline view of 004AED70
    for (I i = 0; i < at<I>(self, 0x328); ++i) {                                                    // 004F11AA..B4, 004F11FB..04
        void* object = nullptr;                                                                     // [esp+14] (stale on failure; unused then)
        if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, at<U*>(self, 0x324)[i], &object)) !=
            k_lookup_ok_0074666c) continue;                                                         // 004F11C0..DC
        if (vcall<void*>(object, 0x50) == nullptr) continue;                                        // 004F11DE..E9 AsSWSAreaOfEffectObject
        kotor_fastcall_005962d0(vcall<void*>(object, 0x50), nullptr);                               // 004F11EB..F6
    }
    kotor_fastcall_004cb160(self, nullptr, nullptr);                                                // 004F1206..0A
    const Vector zero = {0.0f, 0.0f, 0.0f};                                                         // 004F121C..2C
    recovered_object_position_004cd1a0(self, nullptr, &zero, 1, 1, 0);                              // 004F120F..34
}

// =========================================================================================================
// 004D71B0 CSWMessage::WriteVector(const Vector*): RET 4. Three 4-byte float writes (+4 buffer, +8 capacity,
// +0C write position, +1C written count), each growing the buffer by 4 first when it would overflow.
void __fastcall kotor_fastcall_004d71b0(void* self, void*, const Vector* v) {
    const U* src = reinterpret_cast<const U*>(v);
    for (U k = 0; k < 3; ++k) {                                                                     // 004D71B5..7264 (unrolled)
        const U value = x87_float_copy(src[k]);                                                     // [esp+10] fld/fstp
        if (at<U>(self, 0xc) + 4 > at<U>(self, 8)) kotor_fastcall_004d6260(self, nullptr, 4);       // 004D71C7..D3 (jbe)
        std::memcpy(at<unsigned char*>(self, 4) + at<U>(self, 0xc), &value, 4);                     // 004D71D8..E2
        at<U>(self, 0xc) += 4;                                                                      // 004D71E5..EF
        at<U>(self, 0x1c) += 4;                                                                     // 004D71F2
    }
}

// 0056E5F0 CSWSMessage::SendServerToPlayerSafeProjectileProjectile(player, a0, a1, Vector va, Vector vb, a4..a9):
// RET 3Ch. Message 0x2F, sent as major 22h minor 2.
U __fastcall kotor_fastcall_0056e5f0(void* self, void*, void* player, U a0, U a1, Vector va, Vector vb,
    U a4, U a5, U a6, U a7, U a8, U a9) {
    void* data = nullptr; U size = 0;                                                               // [esp+C], [esp+8]
    kotor_fastcall_004d68a0(self, nullptr, 0x2f, 0xffffffffu, 1);                                  // 0056E5F5..07
    kotor_fastcall_00578570(self, nullptr, a0);                                                     // 0056E60C..13
    kotor_fastcall_00578570(self, nullptr, a1);                                                     // 0056E618..1F
    kotor_fastcall_004d71b0(self, nullptr, &va);                                                    // 0056E624..2B
    kotor_fastcall_004d71b0(self, nullptr, &vb);                                                    // 0056E630..37
    kotor_fastcall_004d6340(self, nullptr, a4, 0x20);                                               // 0056E63C..45
    kotor_fastcall_004d62c0(self, nullptr, a5, 8);                                                  // 0056E64A..53
    kotor_fastcall_004d6340(self, nullptr, a6, 0x20);                                               // 0056E658..61
    kotor_fastcall_004d62c0(self, nullptr, a7, 8);                                                  // 0056E666..6F
    kotor_fastcall_004d62c0(self, nullptr, a8, 8);                                                  // 0056E674..7D
    kotor_fastcall_004d6340(self, nullptr, a9, 0x20);                                               // 0056E682..8B
    if (kotor_fastcall_004d6440(self, nullptr, &data, &size) == 0) return 0;                       // 0056E690..A3, 0056E6CA
    return kotor_fastcall_00567e60(self, nullptr, at<U>(player, 4), 0x22, 2, data, size);           // 0056E6A5..BD
}

// 004CDB00 CSWSObject::BroadcastSafeProjectile(a0, a1, Vector* from, Vector* to, a4..a9): RET 28h. Every creature
// of the object's area with a client and within 250 m of either end gets the projectile message.
void __fastcall kotor_fastcall_004cdb00(void* self, void*, U a0, U a1, const Vector* from, const Vector* to,
    U a4, U a5, U a6, U a7, U a8, U a9) {
    void* object = nullptr;                                                                         // [esp+10]
    (void)kotor_fastcall_004d8230(object_array(), nullptr, at<U>(self, 0x8c), &object);             // 004CDB10..2A (status unused)
    if (object == nullptr) return;                                                                  // 004CDB2F..35
    if (vcall<void*>(object, 0x28) == nullptr) return;                                              // 004CDB3B..42 AsSWSArea
    object = nullptr;                                                                               // 004CDB4E
    (void)kotor_fastcall_004d8230(object_array(), nullptr, at<U>(self, 0x8c), &object);             // 004CDB48..68
    const U as_area = object != nullptr ? static_cast<U>(reinterpret_cast<std::uintptr_t>(vcall<void*>(object, 0x28))) : 0; // 004CDB6D..7C
    if (as_area + 0x190 == 0) return;                                                               // 004CDB7E..85 (address of +190 tested, not the area)
    unsigned char* list = bytes(recovered_object_area_004cb120(self, nullptr)) + 0x190;             // 004CDB8B..9A (no null guard)
    if (at<I>(list, 4) <= 0) return;                                                                // 004CDB94..AA
    // [esp+34] (the `to` argument slot) is overwritten with the squared range; `to` is already in EDI
    const float range2 = static_cast<float>(static_cast<double>(k_projectile_range_00747440) * k_projectile_range_00747440); // 004CDC3E..4A
    for (I i = 0; i < at<I>(list, 4); ++i) {                                                        // 004CDBA6..B8, 004CDD05..13 (count re-read)
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(list, 0)[i]);       // 004CDBC0..D3
        if (creature == nullptr) continue;                                                          // 004CDBD8..DC
        void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4));          // 004CDBE2..EF
        if (client == nullptr) continue;                                                            // 004CDBF4..F8
        const double dx1 = static_cast<double>(at<float>(creature, 0x90)) - from->x;               // 004CDBFE..04
        const double dy1 = static_cast<double>(at<float>(creature, 0x94)) - from->y;               // 004CDC06..0C
        const double dz1 = static_cast<double>(at<float>(creature, 0x98)) - from->z;               // 004CDC0F..15
        const float dx2 = static_cast<float>(static_cast<double>(at<float>(creature, 0x90)) - to->x); // 004CDC18..20
        const float dy2 = static_cast<float>(static_cast<double>(at<float>(creature, 0x94)) - to->y); // 004CDC24..2D
        const float dz2 = static_cast<float>(static_cast<double>(at<float>(creature, 0x98)) - to->z); // 004CDC31..3A
        const double near_from = (dz1 * dz1 + dy1 * dy1) + dx1 * dx1;                              // 004CDC4E..5C
        if (!(near_from <= range2)) {                                                               // 004CDC5E..6D (jnp: <= ordered)
            const double near_to = (static_cast<double>(dz2) * dz2 + static_cast<double>(dy2) * dy2) +
                                   static_cast<double>(dx2) * dx2;                                 // 004CDC6F..89
            if (!(near_to <= range2)) continue;                                                     // 004CDC8B..94 (jp: skip)
        }
        kotor_fastcall_0056e5f0(sws_message(), nullptr, client, a0, a1, *from, *to, a4, a5, a6, a7, a8, a9); // 004CDC96..FC
    }
}

// =========================================================================================================
// 00584EC0 CSWSPlaceable::AIActionCastSpell(CSWSObjectActionNode*): RET 4, EAX = action disposition.
// Node parameters: +38 spell id, +40 target id, +44/+48/+4C target location, +50 flags (bit 31: projectile).
U __fastcall kotor_fastcall_00584ec0(void* self, void*, void* node) {
    if (vcall<U>(self, 0x94) == 1) return k_failed_00745dc4;                                        // 00584EC6..F4 (GetDead)
    const U spell = at<U>(node, 0x38);                                                              // 00584F19
    const U target = at<U>(node, 0x40);                                                             // 00584F06
    const U flags = at<U>(node, 0x50);                                                              // 00584F02
    Vector location;                                                                                // [esp+18..20]
    std::memcpy(&location.x, bytes(node) + 0x44, 4);                                                // 00584EFB, 00584F2D
    std::memcpy(&location.y, bytes(node) + 0x48, 4);                                                // 00584EFE, 00584F20
    std::memcpy(&location.z, bytes(node) + 0x4c, 4);                                                // 00584F0D, 00584F4B
    const U projectile = flags >> 31;                                                               // 00584F28..2A
    // the node argument slot [esp+34] keeps node+44 with its low byte replaced by the kind (6, or 7 when the
    // flags' low byte is nonzero)
    const U kind = (at<U>(node, 0x44) & 0xffffff00u) | static_cast<U>((static_cast<unsigned char>(flags) != 0 ? 1 : 0) + 6); // 00584F39..59
    if (kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
                                                   static_cast<int>(spell)) == nullptr)
        return k_failed_00745dc4;                                                                   // 00584F3F..72
    std::memcpy(bytes(self) + 0x1b0, &location, 12);                                                // 00584F75..8C
    at<U>(self, 0x1c0) = spell;                                                                     // 00584F92
    at<U>(self, 0x1bc) = target;                                                                    // 00584F98
    at<U>(self, 0x1cc) = 0;                                                                         // 00584F9E
    kotor_fastcall_004cb9e0(self, nullptr, flags);                                                  // 00584F8F..A8
    if (projectile == 0) {                                                                          // 00584FAD..B3
        kotor_fastcall_004cdf50(self, nullptr, spell, &location, target, 0xff, k_object_invalid, 0, 0, flags, 0); // 00585046..61
        return k_complete_00745dc0;                                                                 // 00585066..72
    }
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);                          // 00584FB9..C3
    void* as_object = object != nullptr ? vcall<void*>(object, 0x10) : nullptr;                     // 00584FC8..D3
    const Vector* from = reinterpret_cast<const Vector*>(bytes(self) + 0x90);
    if (as_object != nullptr) {                                                                     // 00584FD5
        kotor_fastcall_004cdd30(self, nullptr, at<U>(self, 4), target, from,
                                reinterpret_cast<const Vector*>(bytes(as_object) + 0x90), at<U>(self, 0x1c8), kind,
                                spell, 0, flags, 0, 0);                                             // 00584FD7..FD, 00585026..28
    } else {
        kotor_fastcall_004cdd30(self, nullptr, at<U>(self, 4), target, from,
                                reinterpret_cast<const Vector*>(bytes(self) + 0x1b0), at<U>(self, 0x1c8), kind,
                                spell, 0, flags, 0, 0);                                             // 00584FFF..28
    }
    at<U>(self, 0x1cc) = 1;                                                                         // 0058502F
    return k_complete_00745dc0;                                                                     // 00585039..43
}

// =========================================================================================================
// Body bags
// ---------------------------------------------------------------------------------------------------------
// 00570180 CSWSMessage::SendServerToAllPlayersCreatureUpdate_StripEquippedItems(ulong id): RET 4, EAX = 1.
U __fastcall kotor_fastcall_00570180(void* self, void*, U id) {
    void* list = recovered_server_player_list_004aed90(server_app(), nullptr);                       // 00570183..94
    void* position = at<void*>(at<void*>(list, 0), 0);                                              // 00570196..9C head
    while (position != nullptr) {                                                                   // 005701A0, 0057020D..13
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(list, 0), nullptr, position);     // 005701A8..B0
        if (recovered_last_update_object_00561820(player, nullptr, id) != nullptr) {                // 005701B2..BC
            kotor_fastcall_004d68a0(self, nullptr, 4, 0xffffffffu, 1);                              // 005701BE..C6
            kotor_fastcall_00578570(self, nullptr, id);                                             // 005701CB..CE
            void* data = nullptr; U size = 0;                                                       // [esp+18], [esp+14]
            if (kotor_fastcall_004d6440(self, nullptr, &data, &size) != 0)                          // 005701D3..E6
                (void)kotor_fastcall_00567e60(self, nullptr, at<U>(player, 4), 5, 6, data, size);   // 005701E8..FC
        }
        (void)unresolved_list_next_005e9b90(at<void*>(list, 0), nullptr, &position);                // 00570201..08
    }
    return 1;                                                                                       // 00570218
}

// 004EF2E0 CSWSCreature::GetBodyBagAppearance: RET 0, AL. Body-bag 2DA (+D4) "Appearance" by the creature's
// body-bag row (+9DA); on a miss, the appearance 2DA (+0C) "Body_Bag" column (row: appearance word +A60, 0 on a
// miss) selects the body-bag row; 3 when that also misses.
unsigned char __fastcall kotor_fastcall_004ef2e0(void* self, void*) {
    // [esp+8] is never initialised in the original; read back only after a successful lookup wrote it, or on the
    // first-lookup success path. Modelled as 0 (indeterminate in the original).
    I appearance = 0;
    if (table_int(rules_table(0xd4), at<unsigned char>(self, 0x9da), k_appearance_00746efc, &appearance) != 0) // 004EF2FC..57
        return static_cast<unsigned char>(appearance);                                             // 004EF431..45
    I row = 0;                                                                                      // [esp+C]
    if (table_int(rules_table(0xc), at<U16>(self, 0xa60), k_body_bag_00746ef0, &row) == 0) row = 0; // 004EF35D..B8
    if (table_int(rules_table(0xd4), row, k_appearance_00746efc, &appearance) != 0)                 // 004EF3C0..19
        return static_cast<unsigned char>(appearance);                                             // 004EF431..45
    return 3;                                                                                       // 004EF41B..30
}

// 005851E0 CSWSPlaceable::GetBodyBagAppearance: RET 0, AL. As 004EF2E0 with the placeable's body-bag row (+394)
// and the placeables 2DA (+0C) "BodyBag" column by the appearance byte (+230).
unsigned char __fastcall kotor_fastcall_005851e0(void* self, void*) {
    I appearance = 0;                                                                               // [esp+8] (see 004EF2E0)
    if (table_int(rules_table(0xd4), at<unsigned char>(self, 0x394), k_appearance_00746efc, &appearance) != 0) // 005851FC..57
        return static_cast<unsigned char>(appearance);                                             // 00585331..45
    I row = 0;                                                                                      // [esp+C]
    if (table_int(rules_table(0xc), at<unsigned char>(self, 0x230), k_body_bag_007494c8, &row) == 0) row = 0; // 0058525D..B8
    if (table_int(rules_table(0xd4), row, k_appearance_00746efc, &appearance) != 0)                 // 005852C0..19
        return static_cast<unsigned char>(appearance);
    return 3;
}

// 004F3770 CSWSCreature::GetItemCount(int droppable_only, int skip_equipped, int unstolen_only): RET 0Ch.
U __fastcall kotor_fastcall_004f3770(void* self, void*, U a0, U a1, U a2) {
    U count = 0;                                                                                    // ebp
    if (a1 == 0 && at<void*>(self, 0xa2c) != nullptr) {                                             // 004F377D..8C
        for (U slot = 0; slot < 0xe; ++slot) {                                                      // 004F378E..CF (jb)
            void* item = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, 1u << slot); // 004F3790..A2
            if (item == nullptr) continue;                                                          // 004F37A7..A9
            if (a0 != 0 && (at<unsigned char>(item, 0x288) & 8) == 0) continue;                     // 004F37AB..BA
            if (a2 != 0 && at<U>(item, 0x108) != 0) continue;                                       // 004F37BC..C8
            ++count;                                                                                // 004F37CA
        }
    }
    void* repository = recovered_creature_inventory_004ef770(self, nullptr, 1);                     // 004F37D1..D5
    if (repository == nullptr) return count;                                                        // 004F37DA..DE
    for (U i = 0; i < at<U>(repository, 0x10); ++i) {                                               // 004F37E0..E7, 004F381C..22 (jb)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                           // 004F37F0..F3
        if (item == nullptr) continue;                                                              // 004F37F8..FA
        if (a0 != 0 && (at<unsigned char>(item, 0x288) & 8) == 0) continue;                         // 004F37FC..0B
        if (a2 != 0 && at<U>(item, 0x108) != 0) continue;                                           // 004F380D..19
        ++count;                                                                                    // 004F381B
    }
    return count;                                                                                   // 004F3824..2A
}

// 00587710 CSWSPlaceable::GetItemCount(int droppable_only): RET 4.
U __fastcall kotor_fastcall_00587710(void* self, void*, U a0) {
    if (at<U>(self, 0x324) == 0) return 0;                                                          // 00587710..1C
    U count = 0;                                                                                    // ebx
    void* repository = at<void*>(self, 0x36c);                                                      // 00587721
    if (repository == nullptr) return count;                                                        // 00587729..2B
    for (U i = 0; i < at<U>(repository, 0x10); ++i) {                                               // 0058772D..35, 0058775A..60 (jb)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                           // 00587740..43
        if (item == nullptr) continue;                                                              // 00587748..4A
        if (a0 != 0 && (at<unsigned char>(item, 0x288) & 8) == 0) continue;                         // 0058774C..57
        ++count;                                                                                    // 00587759
    }
    return count;                                                                                   // 00587764..68
}

// 00553040 CSWSObject::SetTag(CExoString*): RET 4. +18 = lower-cased copy.
void __fastcall kotor_fastcall_00553040(void* self, void*, void* tag) {
    alignas(4) unsigned char lower[8];                                                              // [esp+4]
    void* result = directory_list::LowerCase005e6080(static_cast<kotor_reconstruction::StartupString*>(tag), nullptr,
                                                     reinterpret_cast<kotor_reconstruction::StartupString*>(lower)); // 00553059..64
    (void)async_assign_string_005e5c50(bytes(self) + 0x18, nullptr, result);                        // 00553069..75
    frame_LogStringDtor_005e5c20(lower, nullptr);                                                   // 0055307A..86
}

// 004EDD90 CSWSCreature::SetGold(int): RET 4. Clamped to 999999999; party members store it in the party table.
void __fastcall kotor_fastcall_004edd90(void* self, void*, I gold) {
    if (gold > 0x3b9ac9ff) gold = 0x3b9ac9ff;                                                       // 004EDD91..9D (jle)
    if (at<U>(self, 0xa88) != 0) {                                                                  // 004EDDA2..AA
        at<I>(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), 0xfc) = gold;    // 004EDDAC..B9
        return;
    }
    at<I>(self, 0x9d0) = gold;                                                                      // 004EDDC3
}

// 005864B0 CSWSPlaceable::LoadBodyBag(BYTE appearance): RET 4, EAX = 1.
U __fastcall kotor_fastcall_005864b0(void* self, void*, U appearance_arg) {
    const unsigned char appearance = static_cast<unsigned char>(appearance_arg);                   // [ebp+8] byte
    alignas(4) unsigned char loc[8];                                                                // [esp+18] CExoLocString
    (void)exo_locstring_construct_005e9e10(loc, nullptr);                                           // 005864D3..D7
    alignas(4) unsigned char unused[8];                                                             // [esp+30] CExoString
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(unused), nullptr); // 005864DE..E6
    I strref = 0;                                                                                   // [esp+14]
    if (table_int(rules_table(0x2c), appearance, k_strref_0074868c, &strref) == 0) strref = 0;      // 005864F0..4A
    at<I>(loc, 4) = strref;                                                                         // 0058654E..5B
    assign_resref_text(bytes(self) + 0x378, k_empty_0073d71c);                                      // 00586552..74
    {
        alignas(4) unsigned char tag[8];
        frame_LogStringCtor_005e5a90(tag, nullptr, k_body_bag_tag_007497f4);                        // 00586579..82
        kotor_fastcall_00553040(self, nullptr, tag);                                                // 00586587..93 SetTag
        frame_LogStringDtor_005e5c20(tag, nullptr);                                                 // 00586598..A0
    }
    (void)recovered_localized_assign_005e9e70(bytes(self) + 0x228, nullptr, loc);                   // 005865A5..B0
    at<U>(self, 0x270) = 0;                                                                         // 005865BE
    at<U>(self, 0x23c) = 1;                                                                         // 005865C4
    at<U>(self, 0x280) = 1;                                                                         // 005865CA
    at<U>(self, 0x284) = 1;                                                                         // 005865D0
    at<unsigned char>(self, 0x27c) = 0xf;                                                           // 005865D6
    at<unsigned char>(self, 0x276) = 0;                                                             // 005865DD
    at<U>(self, 0x278) = 0;                                                                         // 005865E4
    at<U>(self, 0x288) = 1;                                                                         // 005865EA
    at<unsigned char>(self, 0x290) = 0;                                                             // 005865F0
    at<U>(self, 0xf8) = 0;                                                                          // 005865F7
    at<unsigned char>(self, 0x274) = 0x12;                                                          // 005865FD
    assign_text(bytes(self) + 0x264, k_empty_0073d71c);                                             // 005865B5, 00586604..26
    at<unsigned char>(self, 0x230) = appearance;                                                    // 0058662B..32
    at<U>(self, 0xe0) = 0xf;                                                                        // 00586638
    w1x_object_set_hp_004cc280(self, nullptr, 0xf);                                                 // 0058662E..42
    at<unsigned char>(self, 0x314) = 0x10;                                                          // 00586657
    at<unsigned char>(self, 0x315) = 0;                                                             // 0058665E
    at<unsigned char>(self, 0x316) = 0;                                                             // 00586665
    at<U>(self, 0x330) = 0;                                                                         // 0058666C
    at<unsigned char>(self, 0x258) = static_cast<unsigned char>((appearance != 0xd9 ? 1 : 0) * 4 + 1); // 00586647..4B, 00586672..79
    at<U>(self, 0x260) = 0;                                                                         // 0058667F
    at<U>(self, 0x328) = 1;                                                                         // 00586685
    at<U>(self, 0x324) = 1;                                                                         // 0058668B
    at<U>(self, 0x26c) = 0;                                                                         // 00586691
    at<unsigned char>(self, 0x275) = 0;                                                             // 00586697
    {
        menu_resources::Ref portrait;                                                               // [esp+20]
        portrait.construct_00406d80(k_body_bag_portrait_007497e8);                                  // 0058664E..9E
        vcall<void>(self, 0xcc, static_cast<void*>(&portrait));                                     // 005866A3..AC
    }
    assign_resref_text(bytes(self) + 0x240, k_empty_0073d71c);                                      // 005866B2..D0
    exo_locstring_clear_005e9ea0(loc, nullptr);                                                     // 005866D5..D9
    at<U>(loc, 4) = 0x96d4;                                                                         // 005866E9
    (void)recovered_localized_assign_005e9e70(bytes(self) + 0x234, nullptr, loc);                   // 005866DE..F1
    static const U k_empty_fields[] = {0x2e4, 0x294, 0x29c, 0x2a4, 0x2ac, 0x2b4, 0x2bc, 0x2c4, 0x2cc, 0x2d4,
                                       0x2dc, 0x2ec, 0x2f4, 0x2fc, 0x304, 0x30c};                   // 005866F6..F1 (in this order)
    for (U field : k_empty_fields) assign_text(bytes(self) + field, k_empty_0073d71c);
    at<U>(self, 0x398) = 0;                                                                         // 005869FF
    vcall<void>(self, 0x7c, 0x2710u);                                                               // 005869F6..05
    (void)recovered_module_register_tag_004c7de0(server_get_module_004ae6b0(server_app(), nullptr), nullptr,
                                                 bytes(self) + 0x18, at<U>(self, 4));               // 00586A08..20
    recovered_placeable_finish_00584870(self, nullptr);                                             // 00586A25..27
    frame_LogStringDtor_005e5c20(unused, nullptr);                                                  // 00586A2C..35
    exo_locstring_destroy_005ea000(loc, nullptr);                                                   // 00586A3A..46
    return 1;                                                                                       // 00586A51
}

// 00588690 CSWSPlaceable::AcquireItemsFromObject(ulong id, int droppable_only): RET 8, EAX = items acquired.
U __fastcall kotor_fastcall_00588690(void* self, void*, U id, U flag) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);                            // 005886A8..C1
    void* placeable = recovered_placeable_lookup_004ae7a0(server_app(), nullptr, id);               // 005886C7..D1
    if (creature == nullptr && placeable == nullptr) return 0;                                      // 005886D8..F5
    if (creature != nullptr) {
        if (at<void*>(creature, 0xa2c) == nullptr) return 0;                                        // 005886F8..FE
        if (recovered_creature_inventory_004ef770(creature, nullptr, 1) == nullptr) return 0;       // 00588700..0B
    }
    if (placeable != nullptr && at<void*>(placeable, 0x36c) == nullptr) return 0;                   // 0058870D..17
    void* repository;                                                                               // [esp+14]
    if (creature != nullptr) {
        repository = recovered_creature_inventory_004ef770(creature, nullptr, 1);                   // 0058871D..26
    } else {
        repository = at<void*>(placeable, 0x36c);                                                   // 00588730..36
        flag = 0;                                                                                   // 0058873A (argument slot cleared)
    }
    U acquired = 0;                                                                                 // [esp+18]
    U list[3] = {0, 0, 0};                                                                          // [esp+28] data, size, allocated
    if (creature != nullptr) {                                                                      // 00588753..59
        for (U slot = 0; slot < 0xe; ++slot) {                                                      // 0058875B..D1
            void* item = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 1u << slot); // 00588763..75
            if (item == nullptr) continue;                                                          // 0058877A..80
            if (flag != 0 && (at<unsigned char>(item, 0x288) & 8) == 0) continue;                   // 00588782..91
            item_list_append(list, item);                                                           // 00588793..C3
        }
    }
    for (U i = 0; i < at<U>(repository, 0x10); ++i) {                                               // 005887D5..DE, 00588837..41 (jb)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                           // 005887E0..E5
        if (item == nullptr) continue;                                                              // 005887EA..F0
        if (flag != 0 && (at<unsigned char>(item, 0x288) & 8) == 0) continue;                       // 005887F2..01
        item_list_append(list, item);                                                               // 00588803..33
    }
    void* cell = nullptr;                                                                           // [esp+10] (item cell handed to AcquireItem)
    for (I k = 0; k < static_cast<I>(list[1]); ++k) {                                               // 00588843..7F (jle/jl)
        cell = reinterpret_cast<void**>(static_cast<std::uintptr_t>(list[0]))[k];                   // 00588850..59
        if (cell == nullptr) continue;                                                              // 0058885D
        if (recovered_placeable_acquire_item_00584b10(self, nullptr, &cell, id, 1) != 0) ++acquired; // 0058885F..78
    }
    if (creature != nullptr && w2x_creature_gold_004edd60(creature, nullptr) != 0) {                // 00588881..93
        void* memory = live_operator_new(0x29c);                                                    // 00588895..9F
        void* gold = memory != nullptr ? recovered_item_construct_005530a0(memory, nullptr, k_object_invalid) : nullptr; // 005888A6..BF
        cell = gold;                                                                                // 005888CD
        menu_resources::Ref resref;                                                                 // [esp+30]
        resref.construct_00406d80(k_credits_template_007471c0);                                     // 005888BF..D1
        (void)recovered_item_template_005608b0(cell, nullptr, &resref);                             // 005888D6..DF
        at<U>(cell, 0x28c) = w2x_creature_gold_004edd60(creature, nullptr);                         // 005888E4..F3
        kotor_fastcall_004edd90(creature, nullptr, 0);                                              // 005888EF..F9
        (void)recovered_placeable_acquire_item_00584b10(self, nullptr, &cell, k_object_invalid, 1); // 005888FE..0E (result unused)
    }
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list[0])));                       // 00588913..20 (free(NULL) when empty)
    return acquired;                                                                                // 00588929
}

// 004CE220 CSWSObject::SpawnBodyBag: RET 0, EAX = new body-bag placeable id or 7F000000.
U __fastcall kotor_fastcall_004ce220(void* self, void*) {
    const unsigned char type = at<unsigned char>(self, 8);                                          // 004CE244
    if (type != k_type_creature_00746634 && type != k_type_placeable_00746638) return k_object_invalid; // 004CE247..55
    void* creature = vcall<void*>(self, 0x30);                                                      // 004CE25B..5F AsSWSCreature
    void* placeable = vcall<void*>(self, 0x48);                                                     // 004CE262..68 AsSWSPlaceable
    if (creature == nullptr && placeable == nullptr) return k_object_invalid;                       // 004CE26D..79
    (void)kotor_fastcall_00570180(sws_message(), nullptr, at<U>(self, 4));                          // 004CE27F..93 StripEquippedItems
    void* area_object = nullptr;                                                                    // [esp+10]
    (void)kotor_fastcall_004d8230(object_array(), nullptr, at<U>(self, 0x8c), &area_object);        // 004CE298..B8 (status unused)
    if (area_object == nullptr) return k_object_invalid;                                            // 004CE2BD..C3
    if (vcall<void*>(area_object, 0x28) == nullptr) return k_object_invalid;                        // 004CE2C9..D0 AsSWSArea
    U keep = 0;                                                                                     // [esp+1C]
    U bag = 0;                                                                                      // [esp+18] (low byte used)
    if (creature != nullptr) {                                                                      // 004CE2D6..DC
        bag = kotor_fastcall_004ef2e0(creature, nullptr);                                           // 004CE2E2..E9
        I row = at<unsigned char>(creature, 0x9da);                                                 // 004CE2ED..F6 [esp+10]
        if (row == 0) {                                                                             // 004CE2FA
            if (table_int(rules_table(0xc), at<U16>(creature, 0xa60), k_body_bag_00745efc, &row) == 0) row = 0; // 004CE2FC..53
        }
        I corpse = 0;                                                                               // [esp+20]
        if (table_int(rules_table(0xd4), row, k_corpse_00745ef4, &corpse) != 0) {                   // 004CE357..A9
            keep = corpse != 0 ? 1 : 0;                                                             // 004CE3AB..B8
        }
        if (keep == 0) {                                                                            // 004CE3BC
            if (kotor_fastcall_004f3770(creature, nullptr, 1, 0, 0) == 0 &&                         // 004CE3BE..CB
                w2x_creature_gold_004edd60(creature, nullptr) == 0)                                 // 004CE3CD..D6
                return k_object_invalid;                                                            // 004CE3D8..EF
        }
    } else if (placeable != nullptr) {                                                              // 004CE3F0..F2
        if (kotor_fastcall_00587710(placeable, nullptr, 0) == 0) return k_object_invalid;           // 004CE3F4..FE
        bag = kotor_fastcall_005851e0(placeable, nullptr);                                          // 004CE404..0B
    }
    void* memory = live_operator_new(0x454);                                                        // 004CE40F..1C
    void* bag_object = memory != nullptr ? recovered_placeable_construct_005877e0(memory, nullptr, k_object_invalid) : nullptr; // 004CE420..38
    if (bag_object == nullptr) return k_object_invalid;                                             // 004CE43A..44
    if (kotor_fastcall_005864b0(bag_object, nullptr, bag) == 0) {                                   // 004CE44A..5C
        (void)vcall<void*>(bag_object, 0, 1u);                                                      // 004CE45E..60 scalar deleting destructor
        return k_object_invalid;                                                                    // 004CE462..79
    }
    (void)kotor_fastcall_00588690(bag_object, nullptr, at<U>(self, 4), 1);                          // 004CE45A, 004CE47A..7E
    at<U>(bag_object, 0x440) = 1;                                                                   // 004CE485
    if (creature != nullptr) {                                                                      // 004CE483, 004CE48F
        at<U>(bag_object, 0x448) = at<U>(self, 4);                                                  // 004CE491..94
        at<U>(bag_object, 0x444) = 0;                                                               // 004CE49A
    }
    U* node = static_cast<U*>(live_operator_new(0x10));                                             // 004CE4A4..AB
    if (node == nullptr) {                                                                          // 004CE4AE..B4
        (void)vcall<void*>(bag_object, 0, 1u);                                                      // 004CE50F..15
        return k_object_invalid;                                                                    // 004CE517..2E
    }
    node[1] = 0; node[2] = 0; node[3] = 0;                                                          // 004CE4B6..C0
    node[0] = at<U>(bag_object, 4);                                                                 // 004CE4C3..C6
    std::memcpy(&node[1], bytes(self) + 0x90, 12);                                                  // 004CE4C8..DF
    alignas(4) unsigned char loc[8];                                                                // [esp+28] CExoLocString
    (void)exo_locstring_construct_005e9e10(loc, nullptr);                                           // 004CE4DB..E2
    I name_row = -1;
    bool has_row = false;
    if (creature != nullptr) {                                                                      // 004CE4E7..F2
        name_row = at<unsigned char>(creature, 0x9da); has_row = true;                              // 004CE4F4..02
    } else if (placeable != nullptr) {                                                              // 004CE52F..35
        name_row = at<unsigned char>(placeable, 0x394); has_row = true;                             // 004CE537..45
    }
    if (has_row) {
        I name = 0;                                                                                 // [esp+14]
        if (table_int(rules_table(0xd4), name_row, k_name_00745eec, &name) != 0) {                  // 004CE550..86
            at<I>(loc, 4) = name;                                                                   // 004CE588..90
            (void)recovered_localized_assign_005e9e70(bytes(bag_object) + 0x228, nullptr, loc);     // 004CE58C..9B
        }
    }
    const U no_keep = keep == 0 ? 1 : 0;                                                            // 004CE5A0..A8
    at<U>(bag_object, 0x44c) = keep;                                                                // 004CE5AD
    at<U>(bag_object, 0x334) = no_keep;                                                             // 004CE5B3
    if (keep != 0 && at<U>(at<void*>(bag_object, 0x36c), 0x10) == 0) at<U>(bag_object, 0x324) = 0;  // 004CE5B9..C8
    at<U>(bag_object, 0xf8) = no_keep;                                                              // 004CE5DB
    const void* orientation = bytes(self) + 0x9c;                                                   // 004CE5D2
    const float pitch = static_cast<float>(kotor_m15a_pitch_004aa130(orientation));                 // 004CE5D8..E6
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(static_cast<const Vector*>(orientation))); // 004CE5E9..EF
    alignas(4) unsigned char quaternion[16];                                                        // [esp+44]
    recovered_placeable_rotation_005853c0(bag_object, nullptr, yaw_pitch_roll_004acac0(quaternion, yaw, pitch, 0.0f)); // 004CE5F2..02
    void* area = recovered_object_area_004cb120(self, nullptr);                                     // 004CE607..09 (no null guard)
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0x1f4,
                                     at<U>(self, 4), at<U>(area, 0x120), k_event_body_bag_007441f8, node); // 004CE60E..3E
    at<U>(bag_object, 0x218) = 1;                                                                   // 004CE643
    const U result = at<U>(bag_object, 4);                                                          // 004CE64D
    exo_locstring_destroy_005ea000(loc, nullptr);                                                   // 004CE650..5C
    return result;                                                                                  // 004CE666..75
}
}  // extern "C"

}  // namespace w1r_objects

// ================================================================ section: module
// W1 route bodies (module part): StartNewModule callees that had no provider in candidate c13c.
// Transcribed from the original swkotor.exe (read-only); comments cite original instruction addresses.
// Original thiscall members are exported as __fastcall(self, edx-dummy, stack args...) with C linkage.
#include <cstdint>
#include <cstring>

// ---- namespaced providers / cells (declared outside w1r_module so their mangling matches the definitions) ----
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; extern void* volatile input_007a39e4; }
namespace frame_control { extern unsigned message_manager_007a3a00; extern unsigned resource_manager_007a39e8; extern unsigned sound_manager_007a39ec; }
namespace kotor_input_cells_required { extern volatile int word_008338f0; extern volatile int word_008338f4; extern volatile int word_008338f8; }
namespace kotor_reconstruction { struct StartupString; }
namespace kotor_window_shell { struct String; }
namespace kotor_game_prepare {
kotor_window_shell::String* __fastcall kotor_shell_string_construct_005b3190(kotor_window_shell::String*, void*);  // CExoString()
void __fastcall SetGlobalUnpauseableState0062b410(void* gui, void*, unsigned value);                              // CGuiInGame::SetGlobalUnpauseableState
}
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*); }   // CExoString::CompareNoCase
namespace kotor_sound_path_required { unsigned __fastcall remove_playing_005d78f0(void* sound, void*, void* source); } // RemoveFromPlayingList
namespace sound_lower29 { unsigned __fastcall ActorPlaying005db0e0(void* source, void*); }                             // CExoSoundSourceInternal::IsPlaying
namespace kotor_sound_internal { unsigned __fastcall ListContains005e9d40(void* list, void*, void* value); }           // CExoLinkedListInternal::Contains
namespace lane_config_required { void __fastcall PlayMoviesAsync_0070c200(void* player, void*, void* movies, std::uint32_t flags, void* skippable); }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs);                     // CExoString::operator+

namespace w1r_module {
inline unsigned w1r_module_at_pos(void* list, void*, void* pos) {   // GetAtPos as a word (header declares void*)
  return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(unresolved_list_at_pos_005e9b60(list, nullptr, pos)));
}
extern "C" {
// CExoString
// (declared by w5_t2.hpp / w5_t3.hpp) void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);              // CExoString(const char*)
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);                           // ~CExoString
// (declared by w5_t2.hpp / w5_t3.hpp) int __fastcall frame_LogIsEmpty_005e58a0(void*, void*);                               // CExoString::IsEmpty
const char* __fastcall WindowCStr005e5670(void*, void*);                              // CExoString::CStr
// lists
// (declared by w5_t2.hpp / w5_t3.hpp) unsigned __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);              // CExoLinkedListInternal::GetAtPos
unsigned __fastcall unresolved_list_next_005e9b90(void*, void*, void**);               // CExoLinkedListInternal::GetNext
void* __fastcall c2da_005e9d70(void*, void*, void*);                                  // CExoLinkedListInternal::AddHead
// abi-a1: 004059A0 CExoArrayList<CExoString>::Add(CExoString) - selected export claude_0886_bodies_vars, original ABI
// (thiscall, the 8-byte CExoString by value in the two stack dwords, RET 8, callee destroys it).
struct ClaudeExoStringArg { std::uint32_t data, length; };
// Both can throw (the game allocator): noexcept(false) keeps 004B71E0 potentially throwing, as the former Add
// declaration did, so callers such as 00602650 keep their unwind state around the call.
void __fastcall claude_exo_string_list_add_004059a0(void* list, void*, ClaudeExoStringArg value) noexcept(false);
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other) noexcept(false);           // 0078 CExoString(const CExoString&)
void __fastcall kotor_fastcall_00405920(void* list, void*);                           // CExoArrayList<CExoString>::~CExoArrayList
// server / client
void* __fastcall recovered_server_get_module_004b14f0(void*, void*);                  // CServerExoAppInternal::GetModule (W1)
void* __fastcall recovered_server_sws_message_004aedb0(void*, void*);                 // CServerExoApp::GetSWSMessage
unsigned __fastcall recovered_start_start_new_module_0056cbf0(void*, void*);          // SendServerToPlayerModule_StartStartNewModule
const char* __cdecl kotor_cdecl_00401430(unsigned);                                   // ClearLogCounters
void __fastcall kotor_fastcall_005df4e0(void*, void*);                                // CExoInput::ClearEvents
void __fastcall kotor_fastcall_005edba0(void*, void*) noexcept(false);                // CClientExoApp::ResetDriveAcceleration
void* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*);                          // CClientExoApp::GetWorldTimer
void* __fastcall kotor_fastcall_004aede0(void*, void*);                               // CServerExoApp::GetWorldTimer
void __fastcall kotor_fastcall_004adff0(void*, void*);                                // CWorldTimer::PauseWorldTimer
void __fastcall frame_R4_004adbd0(void*, void*) noexcept(false);                      // CWorldTimer::SetSnapshotTime
void __fastcall disable_input_005edbe0(void*, void*);                                 // CClientExoApp::DisableInput
unsigned __fastcall w1x_server_load_module_004b95b0(void*, void*, void*, unsigned);   // CServerExoAppInternal::LoadModule (0886 adapter)
void* __fastcall kotor_fastcall_00561080(void*, void*);                               // CSWSPlayer::GetGameObject
void* __fastcall recovered_object_area_004cb120(void*, void*);                        // CSWSObject::GetArea
void __fastcall frame_R4_005d0fc0(void*, void*, unsigned*, unsigned, unsigned);       // CVirtualMachine::RunScript
void* __fastcall kotor_fastcall_005ed700(void*, void*);                               // CClientExoApp::GetClientOptions
void __fastcall frame_R4_005d5e80(void* sound, void*, unsigned);                      // SetSoundMode
void __cdecl kotor_cdecl_00403cf0();                                                  // ChangeResolutionForMovie
void __fastcall kotor_m15_client_internal_show_load_screen_005f6c60(void* internal, void*, std::uint32_t fade,
    std::uint32_t music, std::uint32_t alt) noexcept(false);                          // CClientExoAppInternal::ShowLoadScreen, RET 0Ch
void __fastcall kotor_fastcall_0070c210(void* player, void*);                         // CExoMoviePlayer::ShowFadePanel
void __stdcall c2da_sleep_0073d1e4(std::uint32_t);                                    // [0073D1E4] Sleep
// C2DA / resources
int __fastcall kotor_fastcall_00414110(void* table, void*, void* row, void* column, int* out);              // C2DA::GetINTEntry(CExoString row, column)
// (declared by w5_t2.hpp / w5_t3.hpp) int __fastcall c2da_current_get_int_00413660(void*, void*, int, void*, int*);                              // C2DA::GetINTEntry(int row, column)
int __fastcall c2da_current_get_string_00413270(void*, void*, int, void*, void*);                          // C2DA::GetCExoStringEntry(int row, column)
unsigned __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row, void* column, void* out); // C2DA::GetCExoStringEntry(CExoString row, column)
unsigned __fastcall gui_load_resman_remove_file_00406b20(void*, void*, void*, unsigned);                    // CExoResMan::RemoveFile
// GFF writer (0873) / saves (W2)
void* __fastcall recovered_gff_construct_004105a0(void*, void*);                                           // CResGFF::CResGFF
unsigned __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version);       // CResGFF::CreateGFFFile
void __fastcall gff_resource_destroy_00411570(void* gff, void*);                                           // CResGFF::~CResGFF
unsigned __fastcall recovered_gff_00412450(void* gff, void*, void* list, void* root, const char* label);   // CResGFF::AddList
unsigned __fastcall recovered_gff_004124e0(void* gff, void*, void* element, void* list, unsigned type);    // CResGFF::AddListElement
unsigned __fastcall recovered_gff_00413030(void* gff, void*, void* path, unsigned type);                   // CResGFF::WriteGFFFile
unsigned __fastcall recovered_save_creature_00500610(void* creature, void*, void* gff, void* top);         // CSWSCreature::SaveCreature
unsigned __fastcall recovered_save_item_0055ccd0(void* item, void*, void* gff, void* top);                 // CSWSItem::SaveItem
void __fastcall recovered_clear_items_0055f330(void* repository, void*);                                   // CItemRepository::ClearItems
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, unsigned index);                     // CItemRepository::ItemListGetItem
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);                                          // CServerExoApp::GetCreatureByGameObjectID
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);                              // CSWSObject::ClearAllActions
void* __fastcall GuiAssign005e5140(void*, void*, const char*);                                             // CExoString::operator=(const char*)
void __fastcall assign_string_005e5c50(void*, void*, void*) noexcept(false);                               // CExoString::operator=
void* __cdecl live_operator_new(unsigned);                                                                 // 006FA7E6
}

using U = std::uint32_t;
using I = std::int32_t;
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline U rd(const void* p, U off) { U v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void* rp(const void* p, U off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void wr(void* p, U off, U v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline void* client_app() { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4] CClientExoApp*
inline void* server_app() { return rp(kotor_menu_window_cells::manager_007a39fc, 8); }   // [[007A39FC]+8] CServerExoApp*
template <class R, class... A> R vcall(void* self, U slot, A... a) {
  using F = R(__thiscall*)(void*, A...);
  return reinterpret_cast<F>(rp(rp(self, 0), slot))(self, a...);
}
// CExoString storage (8 bytes) with the original destructor run on scope exit (EH-state cleanup order).
struct ExoString {
  alignas(4) unsigned char raw[8];
  bool live = false;
  ~ExoString() { if (live) frame_LogStringDtor_005e5c20(raw, nullptr); }
  void* p() { return raw; }
  kotor_reconstruction::StartupString* s() { return reinterpret_cast<kotor_reconstruction::StartupString*>(raw); }
  void text(const char* t) { frame_LogStringCtor_005e5a90(raw, nullptr, t); live = true; }
  void empty() { kotor_game_prepare::kotor_shell_string_construct_005b3190(reinterpret_cast<kotor_window_shell::String*>(raw), nullptr); live = true; }
  void destroy() { live = false; frame_LogStringDtor_005e5c20(raw, nullptr); }
};
// GFF labels are read up to 16 bytes (stopping at the NUL); keep them in 16-byte arrays.
struct Label { char text[16]; };
// .rdata literals
const char k_empty_0073d71c[] = "";
const char k_delete_save_group_0074515c[] = "DeleteSaveGroupOnEnter";
const char k_game_in_progress_0074514c[] = "GAMEINPROGRESS:";
const char k_save_group_00745140[] = "SaveGroup";
const char k_module_name_00745134[] = "ModuleName";
const char k_auto_save_00745484[] = "AutoSaveOnEnter";
const char k_force_0074547c[] = "force";
const char k_no_00745478[] = "no";
const char k_yes_00745474[] = "yes";
const char k_version_00745338[] = "V2.0";
const char k_ifo_00745330[] = "IFO ";
constexpr Label k_player_list_00745320 = {"Mod_PlayerList"};
const char k_pifo_00745318[] = "pifo";
const char k_temp_00745310[] = "TEMP:";
const char k_utc_00747368[] = "UTC ";
const char k_inv_007490dc[] = "INV ";
constexpr Label k_item_list_00747210 = {"ItemList"};
const char k_inventory_007490c0[] = "GAMEINPROGRESS:INVENTORY";
constexpr U k_creature_list_struct_0xbead = 0xbead;   // 004B2CE7 push 0BEADh
constexpr U k_res_utc_0x7eb = 0x7eb, k_res_ifo_0x7de = 0x7de;
constexpr U k_sound_mode_0074c5f8 = 3;      // .rdata 0074C5F8 = 3 (SetSoundMode argument)
constexpr U k_res_type_0x809 = 0x809;       // 004B24B9 push 0x809

// ===================================================================================== ExecuteLoadModule
// 004BA820 CServerExoAppInternal::ExecuteLoadModule(CExoString* name): thiscall, RET 4, EAX = LoadModule result.
extern "C" unsigned __fastcall kotor_fastcall_004ba820(void* self, void*, void* name) noexcept(false) {
  const char* text = k_empty_0073d71c;                                                        // 004BA851 push 0073D71C
  if (recovered_server_get_module_004b14f0(self, nullptr) != nullptr &&                       // 004BA823..004BA82A
      !frame_LogIsEmpty_005e58a0(bytes(recovered_server_get_module_004b14f0(self, nullptr)) + 0x5c, nullptr)) // 004BA82C..004BA83D
    text = WindowCStr005e5670(bytes(recovered_server_get_module_004b14f0(self, nullptr)) + 0x5c, nullptr);   // 004BA83F..004BA84E
  (void)kotor_cdecl_00401430(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(text)));  // 004BA856..004BA863 ClearLogCounters
  (void)recovered_start_start_new_module_0056cbf0(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr); // 004BA85B..004BA86D
  kotor_fastcall_005df4e0(kotor_menu_window_cells::input_007a39e4, nullptr);                  // 004BA872..004BA878
  kotor_input_cells_required::word_008338f0 = 0;                                              // 004BA883
  kotor_input_cells_required::word_008338f4 = 0;                                              // 004BA88D
  kotor_input_cells_required::word_008338f8 = 0;                                              // 004BA897
  kotor_fastcall_005edba0(client_app(), nullptr);                                             // 004BA8A1..004BA8A4
  kotor_fastcall_004adff0(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr);      // 004BA8A9..004BA8B9
  kotor_fastcall_004adff0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr);           // 004BA8BE..004BA8CD
  frame_R4_004adbd0(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr);            // 004BA8D2..004BA8E2
  frame_R4_004adbd0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr);                 // 004BA8E7..004BA8F7
  disable_input_005edbe0(client_app(), nullptr);                                              // 004BA8FC..004BA904
  return w1x_server_load_module_004b95b0(self, nullptr, name, 0);                             // 004BA909..004BA918
}

// ===================================================================================== StopAllOneShots
// 005D8220 CExoSoundInternal::StopAllOneShots: thiscall, RET 0.
// Quirk kept: a null source at a nonnull position re-tests the same position forever (005D8240 -> 005D82AD -> 005D8240).
extern "C" void __fastcall kotor_fastcall_005d8220(void* self, void*) noexcept(false) {
  void* pos = rp(rp(self, 0x40), 0);                                                          // 005D8225..005D822A [esp+8]
  void* src = pos ? rp(pos, 8) : nullptr;                                                     // 005D822E..005D8239
  if (!pos) return;                                                                           // 005D8237..005D823B
  do {
    if (src) {                                                                                // 005D8240..005D8242
      if (rd(src, 0x28)) {                                                                    // 005D8244..005D8249
        pos = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            kotor_sound_path_required::remove_playing_005d78f0(self, nullptr, src)));         // 005D824B..005D8255
        if (sound_lower29::ActorPlaying005db0e0(src, nullptr)) {                              // 005D8253..005D8264
          if (!kotor_sound_internal::ListContains005e9d40(rp(self, 0x48), nullptr, src))      // 005D8260..005D826D
            (void)c2da_005e9d70(rp(self, 0x48), nullptr, src);                                // 005D826F..005D8273
        } else if (!kotor_sound_internal::ListContains005e9d40(rp(self, 0x48), nullptr, src)) { // 005D827A..005D8281
          (void)vcall<void*>(src, 0, 1u);                                                     // 005D8283..005D8289 scalar deleting dtor
        }
        if (!pos) return;                                                                     // 005D828B..005D8291
        src = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            w1r_module_at_pos(rp(self, 0x40), nullptr, pos)));                  // 005D8293..005D82AB
      } else {
        src = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            unresolved_list_next_005e9b90(rp(self, 0x40), nullptr, &pos)));                   // 005D829E..005D82AB
      }
    }
  } while (pos);                                                                              // 005D82AD..005D82B3
}
// 005D5E70 CExoSound::StopAllOneShots: ecx = [ecx] (internal); null -> ret; else jmp 005D8220.
extern "C" void __fastcall kotor_fastcall_005d5e70(void* self, void*) noexcept(false) {
  void* internal = rp(self, 0);                                                               // 005D5E70
  if (!internal) return;                                                                      // 005D5E72..005D5E7B
  kotor_fastcall_005d8220(internal, nullptr);                                                 // 005D5E76
}

// ===================================================================================== PlayLoadModuleMovies
// 005EDB60 CClientExoApp::GetMoveToModuleMovies: [[ecx+4]] + 0x2D8 (address of the internal's movie list).
extern "C" void* __fastcall kotor_fastcall_005edb60(void* self, void*) { return bytes(rp(self, 4)) + 0x2d8; }
// 004B71E0 CExoArrayList<CExoString>::CExoArrayList(const CExoArrayList<CExoString>&): RET 4, EAX = this.
// The original copy-constructs each element into Add's by-value slot (004B71FA..004B7207) and Add destroys it.
extern "C" void* __fastcall kotor_fastcall_004b71e0(void* self, void*, void* source) noexcept(false) {
  wr(self, 4, 0); wr(self, 8, 0); wr(self, 0, 0);                                            // 004B71EB..004B71F1
  for (I i = 0; i < static_cast<I>(rd(source, 4)); ++i)                                       // 004B71F3..004B71F6, 004B7213..004B7219
  {
    ClaudeExoStringArg value;                                                                 // 004B71FA sub esp,8 (by-value slot)
    (void)WindowCopy005e5b00(&value, nullptr, bytes(rp(source, 0)) + static_cast<U>(i) * 8); // 004B71F8..004B7207
    claude_exo_string_list_add_004059a0(self, nullptr, value);                                // 004B720C..004B720E
  }
  return self;                                                                                // 004B721B
}
// 00602650 CClientExoAppInternal::PlayLoadModuleMovies: thiscall, RET 0, EAX = 1 when movies were started.
extern "C" int __fastcall kotor_fastcall_00602650(void* self, void*) noexcept(false) {
  if (!rd(self, 0x12c)) return 0;                                                             // 0060266B..00602673
  if (rd(self, 0x1f8)) return 0;                                                              // 00602679..00602681
  struct List {
    alignas(4) unsigned char raw[12];
    ~List() { kotor_fastcall_00405920(raw, nullptr); }                                        // 00602758 / 0060277E (EH state 0)
  };
  void* movies = kotor_fastcall_005edb60(client_app(), nullptr);                              // 00602687..0060268F
  List list;
  (void)kotor_fastcall_004b71e0(list.raw, nullptr, movies);                                   // 00602694..00602699
  if (static_cast<I>(rd(list.raw, 4)) <= 0) return 0;                                         // 0060269E..006026AC, 00602772..00602794
  if (server_app() != nullptr) {                                                              // 006026B2..006026BC
    kotor_fastcall_004adff0(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr);    // 006026BE..006026C8
    kotor_fastcall_004adff0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr);         // 006026CD..006026DD
    kotor_game_prepare::SetGlobalUnpauseableState0062b410(rp(self, 0x40), nullptr, 1);        // 006026E2..006026E7
    kotor_fastcall_005df4e0(kotor_menu_window_cells::input_007a39e4, nullptr);                // 006026EC..006026F2
    frame_R4_005d5e80(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::sound_manager_007a39ec)),
                      nullptr, k_sound_mode_0074c5f8);                                        // 006026F7..00602704
  }
  kotor_cdecl_00403cf0();                                                                     // 00602709 ChangeResolutionForMovie
  kotor_m15_client_internal_show_load_screen_005f6c60(self, nullptr, 1, 1, 0);                // 0060270E..00602716
  kotor_fastcall_0070c210(rp(self, 0x12c), nullptr);                                          // 0060271B..00602721
  lane_config_required::PlayMoviesAsync_0070c200(rp(self, 0x12c), nullptr, list.raw, 0, nullptr); // 00602726..00602735
  c2da_sleep_0073d1e4(2);                                                                     // 0060273A..0060273C
  wr(self, 0x200, 1);                                                                         // 00602746
  return 1;                                                                                   // 00602742..00602771 (list destroyed)
}
// 005EDB20 CClientExoApp::PlayLoadModuleMovies: ecx = [ecx+4]; jmp 00602650.
extern "C" int __fastcall kotor_fastcall_005edb20(void* self, void*) noexcept(false) {
  return kotor_fastcall_00602650(rp(self, 4), nullptr);                                       // 005EDB20..005EDB23
}

// ===================================================================================== ForceRunAreaOnExitScripts
// 004B4F50 CServerExoAppInternal::ForceRunAreaOnExitScripts: thiscall, RET 0.
extern "C" void __fastcall kotor_fastcall_004b4f50(void* self, void*) noexcept(false) {
  void* pos = rp(rp(rp(self, 0x10064), 0), 0);                                                // 004B4F54..004B4F60 [esp+4]
  if (!pos) return;                                                                           // 004B4F5E..004B4F64
  do {
    void* node = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        w1r_module_at_pos(rp(rp(self, 0x10064), 0), nullptr, pos)));            // 004B4F67..004B4F70
    void* player = vcall<void*>(node, 4);                                                     // 004B4F75..004B4F79 AsSWSPlayer
    if (player) {                                                                             // 004B4F7C..004B4F7E
      void* object = kotor_fastcall_00561080(player, nullptr);                                // 004B4F80..004B4F82
      if (object) {                                                                           // 004B4F87..004B4F89
        void* creature = vcall<void*>(object, 0x30);                                          // 004B4F8B..004B4F8F AsSWSCreature
        if (creature) {                                                                       // 004B4F92..004B4F96
          void* area = recovered_object_area_004cb120(creature, nullptr);                     // 004B4F98..004B4F9A
          if (area) {                                                                         // 004B4F9F..004B4FA1
            wr(area, 0x1d8, rd(creature, 4));                                                 // 004B4FA3, 004B4FAE
            frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)),
                              nullptr, reinterpret_cast<unsigned*>(bytes(area) + 0x17c), rd(area, 0x120), 1); // 004B4FA6..004B4FC1
          }
        }
      }
    }
    (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10064), 0), nullptr, &pos);             // 004B4FC6..004B4FD3
  } while (pos);                                                                              // 004B4FD8..004B4FDE
}

// ===================================================================================== DoModuleEnterSaveCleanup
// 004B2380 CServerExoAppInternal::DoModuleEnterSaveCleanup(CExoString* name, C2DA* table): thiscall, RET 8.
extern "C" void __fastcall kotor_fastcall_004b2380(void*, void*, void* name, void* table) noexcept(false) {
  int group = 0;                                                                              // [esp+14] (only read after success)
  int found;
  {
    ExoString column; column.text(k_delete_save_group_0074515c);                              // 004B23A1..004B23AA
    found = kotor_fastcall_00414110(table, nullptr, name, column.p(), &group);                // 004B23AF..004B23CA
  }                                                                                           // 004B23CF..004B23DD
  if (!found) return;                                                                         // 004B23E2..004B23E4
  ExoString prefix; prefix.text(k_game_in_progress_0074514c);                                 // 004B23EA..004B23F3 [esp+30]
  ExoString module; module.empty();                                                           // 004B23F8..004B2404 [esp+18]
  for (int row = 0; row < static_cast<I>(rd(table, 0x24)); ++row) {                           // 004B2409..004B2415, 004B24D7..004B24DD
    int value = 0;                                                                            // [esp+10]
    int ok;
    {
      ExoString column; column.text(k_save_group_00745140);                                   // 004B2420..004B2429
      ok = c2da_current_get_int_00413660(table, nullptr, row, column.p(), &value);            // 004B242E..004B2440
    }                                                                                         // 004B2445..004B2450
    if (!ok || value != group) continue;                                                      // 004B2455..004B2465
    {
      ExoString column; column.text(k_module_name_00745134);                                  // 004B2467..004B2470
      ok = c2da_current_get_string_00413270(table, nullptr, row, column.p(), module.p());     // 004B2475..004B2487
    }                                                                                         // 004B248C..004B2497
    if (!ok) continue;                                                                        // 004B249C..004B249E
    ExoString path;                                                                           // [esp+38]
    GuiConcat005e5d10(prefix.s(), nullptr, path.s(), module.s()); path.live = true;           // 004B24A0..004B24AE
    (void)gui_load_resman_remove_file_00406b20(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::resource_manager_007a39e8)),
                                               nullptr, path.p(), k_res_type_0x809);          // 004B24B3..004B24C4
  }                                                                                           // 004B24C9..004B24D2 (~path)
}                                                                                             // 004B24E3..004B24FD (~module, ~prefix)

// ===================================================================================== AutoSaveOnEnteringModule
// 004B5050 CServerExoAppInternal::AutoSaveOnEnteringModule(CExoString* name, C2DA* table): thiscall, RET 8, EAX = BOOL.
extern "C" int __fastcall kotor_fastcall_004b5050(void* self, void*, void* name, void* table) noexcept(false) {
  ExoString value; value.empty();                                                             // 004B506D..004B5071 [esp+0C]
  unsigned got;
  {
    ExoString column; column.text(k_auto_save_00745484);                                      // 004B5078..004B5085
    got = kotor_m15_c2da_get_string_entry_00413de0(table, nullptr, name, column.p(), value.p()); // 004B508A..004B50A2
  }                                                                                           // 004B50AD..004B50B8
  if (got == 0) {                                                                             // 004B50A7..004B50C3 (bl = !got)
    wr(self, 0x1b930, 0); wr(self, 0x1b938, 0);                                               // 004B50C5..004B50CB
    return 1;                                                                                 // 004B50D1..004B50F4
  }
  int result;
  if (ini_comparison::CompareNC005e58c0(value.s(), nullptr, k_force_0074547c)) {              // 004B50F7..004B5104
    result = 1;                                                                               // 004B5199
  } else if (ini_comparison::CompareNC005e58c0(value.s(), nullptr, k_no_00745478)) {         // 004B510A..004B511A
    return 0;                                                                                 // 004B511C..004B511E (no cell writes)
  } else if (rd(self, 0x1b938) == 0) {                                                        // 004B5120..004B512A
    return 0;                                                                                 // 004B512C..004B513B
  } else if (!ini_comparison::CompareNC005e58c0(value.s(), nullptr, k_yes_00745474)) {        // 004B513D..004B5149
    result = 1;                                                                               // 004B5199
  } else {
    result = static_cast<int>((rd(kotor_fastcall_005ed700(client_app(), nullptr), 8) >> 2) & 1); // 004B514B..004B515E
    if (result == 0) return 0;                                                                // 004B5161..004B5163 -> 004B5171
  }
  wr(self, 0x1b930, 0); wr(self, 0x1b938, 0);                                                 // 004B5165..004B516B
  return result;                                                                              // 004B5171..004B5196
}

// ===================================================================================== StorePlayerCharacters cascade
// GFF handles: CResStruct = 4 bytes (struct index), CResList = 20 bytes (index + 16-byte label); CResGFF = 0xA0 bytes.
// 00563620 CSWPartyTable::GetFilename(CExoString* out, int index), RET 8: linked W2 provider (w2_party_native).
extern "C" void* __fastcall recovered_party_filename_00563620(void* party, void*, void* out, unsigned index);
// 00563E80 CSWPartyTable::SaveMember(int index, int clear_actions): thiscall, RET 8.
// Quirk kept: a failed CResGFF allocation still calls CreateGFFFile/SaveCreature/WriteGFFFile with a null GFF.
extern "C" void __fastcall kotor_fastcall_00563e80(void* self, void*, int index, int clear_actions) noexcept(false) {
  if (index < 0 || index >= 9) return;                                                        // 00563EA0..00563EB1
  void* creature = kotor_fastcall_004ae770(server_app(), nullptr, rd(self, 0xc + static_cast<U>(index) * 4)); // 00563EB7..00563EC4
  if (!creature) return;                                                                      // 00563EC9..00563ECD
  if (clear_actions) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);              // 00563ED3..00563EDE
  ExoString filename;                                                                         // [esp+28]
  (void)recovered_party_filename_00563620(self, nullptr, filename.p(), static_cast<unsigned>(index)); filename.live = true;    // 00563EE3..00563EEB
  void* mem = live_operator_new(0xa0);                                                        // 00563EF2..00563EFB
  void* gff = mem ? recovered_gff_construct_004105a0(mem, nullptr) : nullptr;                 // 00563F03..00563F17
  alignas(4) unsigned char top[4];                                                            // [esp+0C] CResStruct
  {
    ExoString version; version.text(k_version_00745338);                                      // 00563F19..00563F27 [esp+18]
    ExoString type; type.text(k_utc_00747368);                                                // 00563F2E..00563F3B [esp+10]
    (void)recovered_gff_00411260(gff, nullptr, top, type.p(), version.p());                   // 00563F40..00563F56
  }                                                                                           // 00563F5B..00563F71
  (void)recovered_save_creature_00500610(creature, nullptr, gff, top);                        // 00563F76..00563F7E
  ExoString path;                                                                             // [esp+20]
  {
    ExoString prefix; prefix.text(k_game_in_progress_0074514c);                               // 00563F83..00563F8C [esp+10]
    GuiConcat005e5d10(prefix.s(), nullptr, path.s(), filename.s()); path.live = true;         // 00563F91..00563FA2
  }                                                                                           // 00563FA9..00563FB1
  {
    ExoString text; text.text(WindowCStr005e5670(path.p(), nullptr));                         // 00563FB6..00563FC4 [esp+18]
    (void)recovered_gff_00413030(gff, nullptr, text.p(), k_res_utc_0x7eb);                    // 00563FC9..00563FDA
  }                                                                                           // 00563FDF..00563FE7
  if (gff) (void)vcall<void*>(gff, 0, 1u);                                                    // 00563FEC..00563FF6 scalar deleting dtor
}                                                                                             // 00563FF8..00564012 (~path, ~filename)
// 00565530 CSWPartyTable::UpdateMembers(int clear_actions): thiscall, RET 4.
extern "C" void __fastcall kotor_fastcall_00565530(void* self, void*, int clear_actions) noexcept(false) {
  for (int i = 0; i < 9; ++i) kotor_fastcall_00563e80(self, nullptr, i, clear_actions);       // 00565539..0056554D
}
// 00564030 CSWPartyTable::UpdateInventory(int clear_items): thiscall, RET 4.
extern "C" void __fastcall kotor_fastcall_00564030(void* self, void*, int clear_items) noexcept(false) {
  if (!rd(self, 0x118)) return;                                                               // 0056404B..00564056
  void* mem = live_operator_new(0xa0);                                                        // 0056405E..00564063
  void* gff = mem ? recovered_gff_construct_004105a0(mem, nullptr) : nullptr;                 // 0056406B..00564082
  alignas(4) unsigned char top[4];                                                            // [esp+10] CResStruct
  {
    ExoString version; version.text(k_version_00745338);                                      // 00564087..00564094 [esp+20]
    ExoString type; type.text(k_inv_007490dc);                                                // 00564099..005640AA [esp+18]
    (void)recovered_gff_00411260(gff, nullptr, top, type.p(), version.p());                   // 005640AF..005640C5
  }                                                                                           // 005640CA..005640E0
  alignas(4) unsigned char list[20];                                                          // [esp+38] CResList
  (void)recovered_gff_00412450(gff, nullptr, list, top, k_item_list_00747210.text);           // 005640E5..005640F6
  for (U i = 0; i < rd(rp(self, 0x118), 0x10); ++i) {                                         // 005640FB..00564104, 00564136..00564142
    void* item = w2x_inventory_item_005560b0(rp(self, 0x118), nullptr, i);                   // 00564106..00564107
    if (!item) continue;                                                                      // 0056410C..00564110
    alignas(4) unsigned char element[4];                                                      // [esp+14] CResStruct
    if (recovered_gff_004124e0(gff, nullptr, element, list, 0))                               // 00564112..00564127
      (void)recovered_save_item_0055ccd0(item, nullptr, gff, element);                        // 00564129..00564131
  }
  ExoString path; path.text(k_inventory_007490c0);                                            // 00564147..00564150 [esp+30]
  {
    ExoString text; text.text(WindowCStr005e5670(path.p(), nullptr));                         // 00564155..0056416B [esp+28]
    (void)recovered_gff_00413030(gff, nullptr, text.p(), 0);                                  // 00564170..0056417E
  }                                                                                           // 00564183..0056418C
  if (clear_items) recovered_clear_items_0055f330(rp(self, 0x118), nullptr);                  // 00564191..0056419F
  if (gff) (void)vcall<void*>(gff, 0, 1u);                                                    // 005641A4..005641AE
}                                                                                             // 005641B0..005641B8 (~path)
// 004B2BA0 CServerExoAppInternal::StorePlayerCharacters: thiscall, RET 0, EAX = (stored count > 0).
extern "C" int __fastcall kotor_fastcall_004b2ba0(void* self, void*) noexcept(false) {
  void* players = rp(self, 0x10064);                                                          // 004B2BC5 edi / [esp+20]
  struct Gff {
    alignas(8) unsigned char raw[0xa0];                                                       // [esp+50] CResGFF
    Gff() { (void)recovered_gff_construct_004105a0(raw, nullptr); }                           // 004B2BCF..004B2BD7
    ~Gff() { gff_resource_destroy_00411570(raw, nullptr); }                                   // 004B2C57..004B2C66 / 004B2E48..004B2E4C
  } gff;
  alignas(4) unsigned char top[4];                                                            // [esp+38] CResStruct
  unsigned created;
  {
    ExoString version; version.text(k_version_00745338);                                      // 004B2BDE..004B2BEE [esp+10]
    ExoString type; type.text(k_ifo_00745330);                                                // 004B2BF3..004B2C04 [esp+18]
    created = recovered_gff_00411260(gff.raw, nullptr, top, type.p(), version.p());           // 004B2C09..004B2C24
  }                                                                                           // 004B2C2F..004B2C4E
  if (created == 0) return 0;                                                                 // 004B2C29..004B2C82
  alignas(4) unsigned char list[20];                                                          // [esp+3C] CResList
  (void)recovered_gff_00412450(gff.raw, nullptr, list, top, k_player_list_00745320.text);     // 004B2C83..004B2C96
  int stored = 0;                                                                             // ebp
  void* pos = rp(rp(players, 0), 0);                                                          // 004B2C9B..004B2CA1 [esp+24]
  while (pos) {                                                                               // 004B2CA5, 004B2D30..004B2D36
    void* node = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        w1r_module_at_pos(rp(players, 0), nullptr, pos)));                      // 004B2CB0..004B2CB3
    void* player = vcall<void*>(node, 4);                                                     // 004B2CB8..004B2CBC AsSWSPlayer
    if (player) {                                                                             // 004B2CBF..004B2CC3
      wr(player, 0x80, 0xffffffffu);                                                          // 004B2CC8
      void* creature = kotor_fastcall_004ae770(server_app(), nullptr, rd(player, 0x38));      // 004B2CC5..004B2CDC
      if (creature) {                                                                         // 004B2CE1..004B2CE5
        alignas(4) unsigned char element[4];                                                  // [esp+34] CResStruct
        (void)recovered_gff_004124e0(gff.raw, nullptr, element, list, k_creature_list_struct_0xbead); // 004B2CE7..004B2CFA
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                           // 004B2CFF..004B2D03
        (void)recovered_save_creature_00500610(creature, nullptr, gff.raw, element);          // 004B2D08..004B2D14
        wr(player, 0x80, static_cast<U>(stored));                                             // 004B2D19
        ++stored;                                                                             // 004B2D1F
      }
    }
    (void)unresolved_list_next_005e9b90(rp(players, 0), nullptr, &pos);                       // 004B2D24..004B2D2B
  }
  if (stored > 0) {                                                                           // 004B2D3C..004B2D3E
    ExoString name; name.empty();                                                             // 004B2D44..004B2D48 [esp+18]
    ExoString path; path.empty();                                                             // 004B2D4D..004B2D59 [esp+10]
    (void)GuiAssign005e5140(name.p(), nullptr, k_pifo_00745318);                              // 004B2D60..004B2D70
    (void)GuiAssign005e5140(path.p(), nullptr, k_temp_00745310);                              // 004B2D75..004B2D7E
    {
      ExoString sum;                                                                          // [esp+28]
      GuiConcat005e5d10(path.s(), nullptr, sum.s(), name.s()); sum.live = true;               // 004B2D83..004B2D91
      assign_string_005e5c50(path.p(), nullptr, sum.p());                                     // 004B2D96..004B2DA3
    }                                                                                         // 004B2DA8..004B2DB3
    {
      ExoString text; text.text(WindowCStr005e5670(path.p(), nullptr));                       // 004B2DB8..004B2DC6 [esp+28]
      (void)recovered_gff_00413030(gff.raw, nullptr, text.p(), k_res_ifo_0x7de);              // 004B2DCB..004B2DE1
    }                                                                                         // 004B2DE6..004B2DF1
  }                                                                                           // 004B2DF6..004B2E13 (~path, ~name)
  void* party = bytes(self) + 0x1b770;                                                        // 004B2E18..004B2E1C CSWPartyTable
  kotor_fastcall_00565530(party, nullptr, 1);                                                 // 004B2E22..004B2E26
  kotor_fastcall_00564030(party, nullptr, 1);                                                 // 004B2E2B..004B2E2F
  return stored > 0 ? 1 : 0;                                                                  // 004B2E34..004B2E68 (~gff)
}
}  // namespace w1r_module
