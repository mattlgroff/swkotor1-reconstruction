// claude_encounter_spawn.cpp (lane "encounter", spawn chain): CSWSEncounter spawn-list building and spawning, plus the
// missing area helpers 004BEE30 / 004BB8B0, transcribed from the original swkotor.exe (read-only, capstone).
//
//   00590DA0 CSWSEncounter::UpdateActivateAreaList(id)                       RET 4    @kotor_fastcall_00590da0@12
//   00590B10 CSWSEncounter::PadOutCreatureCount(list, int* count)            RET 8    @kotor_fastcall_00590b10@16
//   00590F80 CSWSEncounter::BuildInitialSpawnList(list, int* count)          RET 8    @kotor_fastcall_00590f80@16
//   00591AF0 CSWSEncounter::TallyEnemyRadiusPoints()                         RET 0    @kotor_fastcall_00591af0@8
//   00591CA0 CSWSEncounter::SpawnCreatureFromSpawnList()                     RET 0    @kotor_fastcall_00591ca0@8
//   005921A0 CSWSEncounter::ClearSpawnList()                                 RET 0    @kotor_fastcall_005921a0@8
//   00592280 CSWSEncounter::AddCreaturesToSpawnList(list, int*, index, entry by value) RET 30h @kotor_fastcall_00592280@56
//   005935D0 CSWSEncounter::BuildSpawnList()                                 RET 0    @kotor_fastcall_005935d0@8
//   004BEE30 CSWSArea::ComputeNonVisibleLocation(pos, obj, int, Vector* out, float) RET 14h @kotor_fastcall_004bee30@28
//   004BB8B0 CSWSArea::CountVisibleToPlayers(pos, int max)                   RET 8    @kotor_fastcall_004bb8b0@16
//
// CEncounterListEntry field use observed here: +00 CResRef, +10 float CR, +14 float points (header name "cr"),
// +18 used flag, +1C tried flag, +20 single-spawn flag. EncounterSpawnPoint (0x10): x, y, z, orientation.
// Quirks kept (original behaviour):
//  * UpdateActivateAreaList removes during its forward scan without adjusting the index (the next id is skipped).
//  * BuildInitialSpawnList compares the pool against the float 10h bytes BEFORE the creature array (heap header bytes in
//    front of the new[] count cookie) when it starts a same-CR group (00591186).
//  * BuildSpawnList copies only the ResRef and the CR (+10) into the heap spawn entries (+14 points stay 0) and leaks
//    the temporary array when no creature was chosen (count == 0).
//  * SpawnCreatureFromSpawnList normalizes the spawn-point facing into a temporary and then applies the untouched
//    (1,0,0) orientation; it removes the spawn-list pointer with deleteAt without freeing the entry (leak); the
//    "use spawn point" flag is set before the last-entered creature lookup (spawn at (0,0,0) when it is gone).
//  * CountVisibleToPlayers adds 1.5 to its local copy of the source z on every line-of-sight test (cumulative) and
//    looks up the client object by the area's +120h field.
//  * ComputeNonVisibleLocation steps y across the whole span (2r) in its first pass (only the two edges are probed)
//    and x across the span in its second pass; the height probe reuses the previous point's z.
#include "d8bbdade43-claude_encounter_common.hpp"
#include <cmath>

extern "C" {
// ---- extra imports (c40 map; object in brackets) ----
U __fastcall recovered_area_index_00506f20(void* area, void*, U* out, U x_bits);               // GetFirstObjectIndiceByX [1065]
U __fastcall recovered_object_ai_state_reputation_0057cd90(void* object, void*, U id);         // GetAIStateReputation (AL) [w1_native_b7]
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                             // return_zero [0832]
void* __fastcall kotor_fastcall_004aee70(void* server, void*);                                 // GetPartyTable [claude_vm_vmD_b]
U __fastcall recovered_party_member_00563740(void* table, void*, U id);                         // CSWPartyTable::GetIsMember [1124]
U8 __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                       // CSWCreatureStats::GetLevel (AL) [0963]
int __cdecl crt_rand_006fc447();                                                                // _rand [0479]
void* __fastcall recovered_server_creature_construct_004f7a10(void* self, void*, U id, U flag); // CSWSCreature() [0963]
U __fastcall w2x_creature_template_005026d0(void* creature, void*, void* ref, U flag);         // CSWSCreature::LoadFromTemplate [0886]
U __fastcall w1x_area_safe_location_004be860(void* area, void*, U position, U radius_bits, U a, U b, U out); // ComputeSafeLocation [0886]
void __fastcall recovered_creature_placement_004fa100(void* creature, void*, U area, U x, U y, U z, U saved, U flag); // CSWSCreature::AddToArea [1060]
U __fastcall recovered_ai_level_004b08a0(void* ai, void*, void* object, U level);               // SetAILevel [1070]
void __fastcall deleteAt_006857a0(void* list, void*, int index);                               // CExoArrayList::DelIndex [0609]
unsigned __fastcall w2x_area_room_004bb600(void* area, void*, void* position, int* out_room_index); // CSWSArea::GetRoom [0886]
double __fastcall w5_room_compute_height_00579430(void* room, void*, Vector point, I walkable); // CSWSRoom::ComputeHeight ST0 [1206-00]
U __fastcall recovered_area_first_object_005071b0(void* area, void*, U* out);                  // GetFirstObjectInArea [w2_update_native]
U __fastcall recovered_area_next_object_005071e0(void* area, void*, U* out);                   // GetNextObjectInArea [w2_update_native]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                         // GetObjectArray [w2_party_native]
// ---- written here ----
U __fastcall kotor_fastcall_004bee30(void* area, void*, const Vector* pos, void* object, U unused, Vector* out, U unused_radius);
I __fastcall kotor_fastcall_004bb8b0(void* area, void*, const Vector* pos, I max);
}

namespace {
using namespace enc;
constexpr U k_one_bits = 0x3f800000u;
constexpr U8 k_lookup_found_0074666c = 0;                 // .rdata 0074666C (byte 0)
constexpr float k_zero_0073d700 = 0.0f;                   // .rdata 0073D700
constexpr float k_tenth_0073e798 = 0.1f;                  // .rdata 0073E798 (3DCCCCCDh)
constexpr float k_epsilon_0073d708 = 0.001f;              // .rdata 0073D708 (3A83126Fh)
constexpr float k_radius_007455dc = 30.0f;                // .rdata 007455DC
constexpr float k_max_radius_007455d8 = 20.0f;            // .rdata 007455D8
constexpr float k_vis_sq_007455bc = 900.0f;               // .rdata 007455BC
constexpr float k_eye_0073f414 = 1.5f;                    // .rdata 0073F414
constexpr float k_half_pi_00741bb8 = 1.5707963705062866f; // .rdata 00741BB8 (3FC90FDBh)
constexpr U k_safe_radius_bits = 0x41a00000u;             // 20.0 (push imm)
constexpr U k_nonvisible_radius_bits = 0x42200000u;       // 40.0 (push imm)
constexpr float k_minus_one = -1.0f;                      // .rdata 00740CBC

inline EncounterListEntry* entries(void* list) { return static_cast<EncounterListEntry*>(list); }
inline EncounterListEntry* creatures(void* self) { return at<EncounterListEntry*>(self, 0x2b0); }
inline float cr_of(const EncounterListEntry& e) { return flt(e.f10); }
inline float points_of(const EncounterListEntry& e) { return e.cr; }
inline void assign_ref(void* dst, const void* src) { ResRef tmp; (void)ref_assign_copy_00406120(dst, nullptr, &tmp, src); }
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server(), nullptr, id); }
inline Vector* position_of(void* object) { return reinterpret_cast<Vector*>(bytes(object) + 0x90); }
inline U limit_250(void* self) { return at<U>(self, 0x250) - at<U>(self, 0x264); }
inline U limit_254(void* self) { return at<U>(self, 0x254) - at<U>(self, 0x264); }
// CResRef::CopyToString into a temporary CExoString + CStr (the original's stripped debug print)
void ref_debug(void* ref) {
    ExoString s; kotor_movie::string_element_construct_005b3190(&s, nullptr);
    c2da_ref_to_string_00405f70(ref, nullptr, &s);
    (void)frame_LogCStr_005e5670(&s, nullptr);
    frame_LogStringDtor_005e5c20(&s, nullptr);
}
// lane x87-trig (2026-10-24): the former x87_sin / x87_cos returned double and the caller narrowed them to float
// (80 -> 64 -> 32); the original stores both results straight to dwords (00591DFA / 00591DFE). See x87trig_00591deb.
}  // namespace
// 00591DE5..00591DFE: fld st(0); fsin; fxch st(1); fcos; fstp dword [esp+28h] (facing.x); fstp dword [esp+2Ch]
// (facing.y). The angle (00591DBC fld [00741BB8]; 00591DDD fsub dword) is a PC-53 result the double carries exactly.
// Precondition as in the original: PC = 53 bits, RC = nearest. No CALL inside the asm.
extern "C" __declspec(noinline) void __cdecl x87trig_00591deb(double angle, float* facing_x, float* facing_y) {
    __asm {
        fld qword ptr angle
        fld st(0)                              // 00591DE5
        fsin                                   // 00591DEB
        fxch st(1)                             // 00591DF6
        fcos                                   // 00591DF8
        mov eax, facing_x
        fstp dword ptr [eax]                   // 00591DFA
        mov eax, facing_y
        fstp dword ptr [eax]                   // 00591DFE
    }
}
namespace {
// GetRoom + CSWSRoom::ComputeHeight(point by value, 0), 0.0 when no room (004BEEB5..E6 shape)
float probe_height(void* area, Vector* v, int* room_index) {
    void* room = reinterpret_cast<void*>(static_cast<std::uintptr_t>(w2x_area_room_004bb600(area, nullptr, v, room_index)));
    return room != nullptr ? static_cast<float>(w5_room_compute_height_00579430(room, nullptr, *v, 0)) : 0.0f;
}
}  // namespace

extern "C" {

// ======================================================================= 00590DA0 UpdateActivateAreaList(id): RET 4
void __fastcall kotor_fastcall_00590da0(void* self, void*, U id) {
    void* area = recovered_object_area_004cb120(self, nullptr);                                   // 00590DA7..B0
    if (area == nullptr) return;
    for (I i = 0; i < at<I>(self, 0x288); ++i) {                                                  // 00590DBE..16 (re-read)
        void* c = creature_by_id(at<U*>(self, 0x284)[i]);                                         // 00590DD0..E9
        if (c != nullptr && recovered_encounter_in_activate_area_00590090(self, nullptr, position_of(c)) == 0) // 00590DEB..FA
            kotor_fastcall_005903b0(self, nullptr, at<U*>(self, 0x284)[i]);                       // 00590DFC..08 (index not adjusted)
    }
    U index = 0;                                                                                  // [esp+10]
    if (recovered_area_index_00506f20(area, nullptr, &index, at<U>(self, 0x2a0)) == 0) return;    // 00590E18..2D
    if (static_cast<I>(index) >= at<I>(area, 0x194)) return;                                      // 00590E33..3D
    do {
        const U object_id = at<U*>(area, 0x190)[index];                                           // 00590E43..4D
        void* c = creature_by_id(object_id);                                                      // 00590E50..63
        if (c != nullptr) {
            Vector p = *position_of(c);                                                           // 00590E69..89
            if (static_cast<double>(p.x) > static_cast<double>(at<float>(self, 0x2a4))) return;  // 00590E7B..92 (beyond max x)
            if (static_cast<U8>(recovered_object_ai_state_reputation_0057cd90(c, nullptr, at<U>(self, 4))) == 2 && // 00590E98..A5
                vcall<U>(c, 0x94) == 0 &&                                                         // 00590EAB..B7 GetDead
                kotor_m15e_slot010_zero_0063e7f0(c, nullptr) == 0) {                              // 00590EBD..C6
                bool inside = true;
                if (object_id != id) {                                                            // 00590ECC..D0
                    const double y = p.y;
                    inside = y >= static_cast<double>(at<float>(self, 0x2a8)) &&                  // 00590ED2..E1 (C0: less/unordered)
                             y <= static_cast<double>(at<float>(self, 0x2ac)) &&                  // 00590EE3..F2
                             recovered_encounter_in_activate_area_00590090(self, nullptr, &p) != 0; // 00590EF4..02
                }
                if (inside) {
                    if (kotor_fastcall_00590290(self, nullptr, object_id) == 0)                    // 00590F04..0E
                        kotor_fastcall_005902d0(self, nullptr, object_id);                         // 00590F10..13
                    bool activate = true;
                    if (at<U>(self, 0x318) != 0 && at<U>(at<void*>(c, 0xa74), 0x6c) == 0) {       // 00590F18..2D
                        void* party = kotor_fastcall_004aee70(server(), nullptr);                 // 00590F2F..3F
                        activate = party != nullptr && recovered_party_member_00563740(party, nullptr, object_id) == 1; // 00590F41..4C
                    }
                    if (activate) at<U>(self, 0x28c) = 1;                                         // 00590F4E
                }
            }
        }
        ++index;                                                                                  // 00590F58..5D
    } while (static_cast<I>(index) < at<I>(area, 0x194));                                         // 00590F61..67
}

// ======================================================================= 005921A0 ClearSpawnList(): RET 0
void __fastcall kotor_fastcall_005921a0(void* self, void*) {
    if (at<I>(self, 0x25c) <= 0) return;                                                          // 005921A3..AB
    for (I i = at<I>(self, 0x25c) - 1; i >= 0; --i) {                                             // 005921AE..08
        void** data = at<void**>(self, 0x258);
        void* entry = data[i];                                                                    // 005921C6..CC
        const I size = at<I>(self, 0x25c) - 1;
        at<I>(self, 0x25c) = size;                                                                // 005921D6
        for (I j = i; j < at<I>(self, 0x25c); ++j) at<void**>(self, 0x258)[j] = at<void**>(self, 0x258)[j + 1]; // 005921E0..F8
        if (entry != nullptr) live_free(entry);                                                   // 005921FA..04
    }
}

// ======================================================================= 00590B10 PadOutCreatureCount(list, count*): RET 8
void __fastcall kotor_fastcall_00590b10(void* self, void*, void* list_arg, I* count) {
    EncounterListEntry* list = entries(list_arg);
    I remaining = static_cast<I>(limit_250(self)) - *count;                                       // 00590B38..48 [esp+14]
    float threshold = 0.0f;                                                                       // [esp+18]
    if (remaining <= 0) return;                                                                   // 00590B54
    for (;;) {
        I k = at<I>(self, 0x2b4) - 1;                                                             // 00590B60..67
        if (k < 0) return;
        for (;; --k) {                                                                            // 00590B77..E4
            ExoString name; kotor_movie::string_element_construct_005b3190(&name, nullptr);       // 00590B77..7B
            c2da_ref_to_string_00405f70(&creatures(self)[k], nullptr, &name);                     // 00590B80..97 (unused)
            const EncounterListEntry& e = creatures(self)[k];
            const bool pick = static_cast<double>(points_of(e)) > static_cast<double>(threshold) && // 00590BA6..B5
                              (e.f20 == 0 || e.f18 == 0);                                         // 00590BB7..C3
            if (pick) {
                threshold = points_of(creatures(self)[k]);                                        // 00590BFA..0B
                frame_LogStringDtor_005e5c20(&name, nullptr);                                     // 00590C07..17
                break;
            }
            frame_LogStringDtor_005e5c20(&name, nullptr);                                         // 00590BC5..D1
            if (k - 1 < 0) return;                                                                // 00590BDA..E4 (RET 8)
        }
        I first = 0, last = 0, singles = 0, fresh = 1;                                            // [esp+1C] / edx / [esp+20] / [esp+24]
        I i = 0;
        if (at<I>(self, 0x2b4) > 0) {                                                             // 00590C26..38
            for (; i < at<I>(self, 0x2b4); ++i) {                                                 // 00590C40..9A
                EncounterListEntry& e = creatures(self)[i];
                if (points_of(e) == threshold) {                                                  // 00590C4A..59 (unordered -> skip)
                    if (fresh == 1) { first = i; fresh = 0; }                                     // 00590C5B..6C
                    last = i;                                                                     // 00590C77
                    if (e.f20 == 1) ++singles;                                                    // 00590C74..7B
                    e.f1c = 0;                                                                    // 00590C7F
                }
            }
        }
        const I range = last - first + 1;                                                         // 00590C9E..A3 [esp+24]
        I tried = 0;                                                                              // [esp+10]
        for (;;) {
            const I r = crt_rand_006fc447() % range;                                              // 00590CB0..B6 (idiv)
            const I index = r + first;                                                            // 00590CBA..C4
            EncounterListEntry* e = &creatures(self)[index];
            if (e->f20 == 0 || e->f18 == 0) {                                                     // 00590CCF..DB
                assign_ref(&list[*count], e);                                                     // 00590CDD..EE
                list[*count].cr = creatures(self)[index].cr;                                      // 00590CF3..05 (dword move)
                list[*count].f10 = creatures(self)[index].f10;                                    // 00590D09..18
                ++*count;                                                                         // 00590D1C
                creatures(self)[index].f18 = 1;                                                   // 00590D1E..24
                --remaining;                                                                      // 00590D2C
            }
            U* tried_flag = &creatures(self)[index].f1c;                                          // 00590D30..3C
            if (*tried_flag == 0) { ++tried; *tried_flag = 1; }                                   // 00590D40..46
            if (range == singles && tried == singles) break;                                      // 00590D4C..5A
            if (remaining <= 0) return;                                                           // 00590D5C..62
        }
        if (remaining <= 0) return;                                                               // 00590D7C..82
    }
}

// ======================================================================= 00590F80 BuildInitialSpawnList(list, count*): RET 8
void __fastcall kotor_fastcall_00590f80(void* self, void*, void* list_arg, I* count) {
    EncounterListEntry* list = entries(list_arg);
    const double diff = static_cast<double>(at<float>(self, 0x2d8)) - at<float>(self, 0x2dc);     // 00590F8A..92
    const float slack = static_cast<float>(static_cast<double>(k_tenth_0073e798) * diff);         // 00590F9C..A8 [L+20]
    float points = static_cast<float>(diff);                                                      // 00590FAC [L+14]
    EncounterListEntry local;                                                                     // [L+2C] CEncounterListEntry() inline
    (void)kotor_fastcall_00405ed0(&local, nullptr);                                               // 00590FB0
    { ResRef tmp; (void)unresolved_ref_assign_00406290(&local, nullptr, &tmp, ""); }              // 00590FB5..C3 ([0073D71C])
    local.f10 = 0; local.cr = 0.0f; local.f20 = 0; local.f18 = 0; local.f1c = 0;                  // 00590FCE..DE
    if (at<I>(self, 0x2b4) <= 0) return;                                                          // 00590FC8..E2
    I i = 0;                                                                                      // [L+10]
    do {
        if (*count >= static_cast<I>(limit_254(self))) return;                                    // 00591000..12
        if (!(static_cast<double>(points) > static_cast<double>(k_zero_0073d700))) return;       // 00591018..27
        const U off = static_cast<U>(i) * 0x24;                                                   // [L+50]
        EncounterListEntry* src = &creatures(self)[i];
        assign_ref(&local, src);                                                                  // 0059103B..49
        local.f10 = src->f10; local.cr = src->cr; local.f18 = src->f18; local.f1c = src->f1c; local.f20 = src->f20; // 0059104E..70
        bool advance = true;                                                                      // false: i already set (group path)
        if (local.f20 == 1 && local.f18 == 0) {                                                   // 0059106D..78
            assign_ref(&list[*count], &local);                                                    // 0059107A..90
            list[*count].cr = local.cr;                                                           // 00591095..9E (fld/fstp)
            list[*count].f10 = local.f10;                                                         // 005910A2..AB (fld/fstp)
            ++*count;                                                                             // 005910AF..B2
            local.f18 = local.f20;                                                                // 005910B4
        } else if (cr_of(local) != k_zero_0073d700 || cr_of(local) != cr_of(local)) {             // 005910BD..CC (equal -> next)
            const double budget = static_cast<double>(points) + slack;                            // 005910D2..D6 (ST0)
            if (static_cast<double>(local.cr) > budget) {                                         // 005910DA..E5 -> 00591328
            } else if (cr_of(local) == flt(at<U>(bytes(creatures(self)) + off, 0x34))) {          // 005910EB..02 (next entry, same CR)
                I group = 1, singles = 1, tried = 0;                                              // [L+18] [L+24] [L+28]
                local.f1c = 0;                                                                    // 00591120 [L+48]
                I end = i + 1;                                                                    // [L+1C]
                if (end < at<I>(self, 0x2b4)) {                                                   // 00591124..2E
                    for (;;) {                                                                    // 00591140..7D
                        EncounterListEntry& g = creatures(self)[end];
                        if (!(cr_of(g) == cr_of(creatures(self)[i]))) { --end; break; }           // 00591144..51 -> 00591181
                        g.f1c = 0;                                                                // 00591153
                        if (creatures(self)[end].f20 != 0) ++singles;                             // 0059115B..69
                        ++group;                                                                  // 0059116D
                        ++end;
                        if (end >= at<I>(self, 0x2b4)) break;                                     // 00591171..7F
                    }
                }
                // 00591186..94: the budget (ST0) against the float 10h bytes before the creature array
                const float before = *reinterpret_cast<const float*>(bytes(creatures(self)) - 0x10);
                if (!(budget < static_cast<double>(before)) && budget == budget && before == before) {
                    while (*count < static_cast<I>(limit_254(self))) {                            // 005911A0..B2
                        const I r = crt_rand_006fc447() % group;                                  // 005911B8..BE
                        const I index = r + i;                                                    // 005911C2..CC
                        EncounterListEntry* e = &creatures(self)[index];
                        if (e->f20 == 0 || e->f18 == 0) {                                         // 005911D4..E3
                            assign_ref(&list[*count], e);                                         // 005911E5..F4
                            list[*count].cr = creatures(self)[index].cr;                          // 005911FD..0C (dword move)
                            list[*count].f10 = creatures(self)[index].f10;                        // 00591210..1F
                            ++*count;                                                             // 00591223
                            creatures(self)[index].f18 = 1;                                       // 00591225..2B
                            points = static_cast<float>(static_cast<double>(points) - creatures(self)[index].cr); // 005911F9 / 00591233..40
                            e = &creatures(self)[index];
                        }
                        if (e->f1c == 0) { e->f1c = 1; ++tried; }                                 // 00591244..52
                        if (group == singles && tried == group) break;                            // 00591256..64
                        const double left = static_cast<double>(points) + slack;                  // 00591266..70
                        const float cost = creatures(self)[index].cr;
                        if (left < static_cast<double>(cost) || left != left || cost != cost) break; // 00591274..7D (C0)
                    }
                }
                i = end + 1;                                                                      // 00591283..88
                advance = false;
            } else {
                // 00591291..A4: single entry of its CR
                if (static_cast<double>(local.cr) <= budget) {                                          // 0059129B..9E
                    U taken = local.f18;                                                          // ecx (f18, then 1)
                    for (;;) {
                        if (*count >= static_cast<I>(limit_254(self))) break;                     // 005912B0..C0
                        if (local.f20 != 0 && taken != 0) break;                                  // 005912C2..C8
                        assign_ref(&list[*count], &local);                                        // 005912CA..DB
                        list[*count].cr = local.cr;                                               // 005912E0..E9
                        list[*count].f10 = local.f10;                                             // 005912ED..F6
                        ++*count;                                                                 // 005912FA..05
                        taken = 1; local.f18 = 1;                                                 // 00591307..0C
                        points = static_cast<float>(static_cast<double>(points) - local.cr);      // 005912FC..10
                        const double left = static_cast<double>(points) + slack;                  // 00591314..18
                        if (left < static_cast<double>(local.cr) || left != left || local.cr != local.cr) break; // 0059131C..24
                    }
                }
            }
        }
        if (advance) ++i;                                                                         // 0059132A
    } while (i < at<I>(self, 0x2b4));                                                             // 0059132E..38
}

// ======================================================================= 00591AF0 TallyEnemyRadiusPoints(): RET 0
void __fastcall kotor_fastcall_00591af0(void* self, void*) {
    void* area = recovered_object_area_004cb120(self, nullptr);                                   // 00591AF7..00
    if (area == nullptr) return;
    at<float>(self, 0x2d4) = 0.0f;                                                                // 00591B0C
    void* entered = creature_by_id(at<U>(self, 0x2e0));                                           // 00591B06..27
    if (entered == nullptr) return;
    const Vector p = *position_of(entered);                                                       // 00591B2D..57 [B+10]
    U index = 0;                                                                                  // [B+8]
    if (recovered_area_index_00506f20(area, nullptr, &index,
            bits(static_cast<float>(static_cast<double>(p.x) - k_radius_007455dc))) == 0) return; // 00591B3B..62
    if (static_cast<I>(index) >= at<I>(area, 0x194)) return;                                      // 00591B68..72
    do {
        void* c = creature_by_id(at<U*>(area, 0x190)[index]);                                     // 00591B80..A0
        if (c != nullptr) {
            const Vector q = *position_of(c);                                                     // 00591BAA..CA
            if (static_cast<double>(p.x) + k_radius_007455dc < static_cast<double>(q.x)) return;  // 00591BA6..D3 (jnp: less)
            if (static_cast<U8>(recovered_object_ai_state_reputation_0057cd90(c, nullptr, at<U>(self, 4))) == 2 && // 00591BD9..E6
                vcall<U>(c, 0x94) != 1 && kotor_m15e_slot010_zero_0063e7f0(c, nullptr) != 1) {   // 00591BEC..05
                const double lo = static_cast<double>(p.y) - k_radius_007455dc;
                const double hi = static_cast<double>(p.y) + k_radius_007455dc;
                if (lo <= static_cast<double>(q.y) && !(hi < static_cast<double>(q.y)) && hi == hi && q.y == q.y) { // 00591C07..2F
                    I level = recovered_stats_level_005a5fd0(at<void*>(c, 0xa74), nullptr, 0);    // 00591C31..49 (movzx)
                    if (at<U>(c, 0x9d4) == 0) level = level / 2;                                  // 00591C3E..54 (cdq/sub/sar)
                    at<float>(self, 0x2d4) = static_cast<float>(
                        kotor_fastcall_005919e0(self, nullptr, static_cast<float>(level)) +
                        static_cast<double>(at<float>(self, 0x2d4)));                             // 00591C58..6D
                }
            }
        }
        ++index;                                                                                  // 00591C73..7A
    } while (static_cast<I>(index) < at<I>(area, 0x194));                                         // 00591C7E..84
}

// ======================================================================= 00592280 AddCreaturesToSpawnList: RET 30h
void __fastcall kotor_fastcall_00592280(void* self, void*, EncounterListEntry* list, I* count, I index,
                                        EncounterListEntry entry) {
    const double value = kotor_fastcall_005919e0(self, nullptr, cr_of(entry));                   // 00592283..8B
    float points = static_cast<float>(value);                                                     // 00592290 (fst into the by-value copy)
    if (value == static_cast<double>(at<float>(self, 0x2d0))) return;                             // 00592294..9F (jnp: equal)
    if (*count == 1) {                                                                            // 005922B0..B5
        *count = 0;                                                                               // 005922B7
    } else {
        I removed = 0;                                                                            // ecx / [esp+24]
        for (I j = 0; j < *count; ++j) {                                                          // 005922C1..0A (re-read)
            if (j == index) ++removed;                                                            // 005922D0..DB
            else if (removed != 0) (void)kotor_fastcall_00590250(&list[j - removed], nullptr, &list[j]); // 005922DD..F1
        }
        *count -= removed;                                                                        // 0059230C
    }
    const float threshold = static_cast<float>(static_cast<double>(points) - k_epsilon_0073d708); // 0059230E..26
    if (at<I>(self, 0x2b4) <= 0) return;                                                          // 00592312..2A
    for (I k = 0; k < at<I>(self, 0x2b4); ++k) {                                                  // 00592403..17
        if (*count > static_cast<I>(limit_250(self))) return;                                     // 00592330..42 (jg)
        EncounterListEntry* c = &creatures(self)[k];
        if (!(c->cr <= points)) continue;                                                         // 00592351..60 (greater/unordered)
        for (;;) {
            if (!(creatures(self)[k].cr <= threshold)) break;                                     // 00592366..71
            if (*count >= at<I>(self, 0x254)) break;                                              // 00592377..7F
            c = &creatures(self)[k];
            if (c->f20 != 0 && c->f18 != 0) break;                                                // 00592385..91
            assign_ref(&list[*count], c);                                                         // 00592393..A0
            list[*count].cr = creatures(self)[k].cr;                                              // 005923A9..B8 (dword move)
            list[*count].f10 = creatures(self)[k].f10;                                            // 005923BC..CB
            creatures(self)[k].f18 = 1;                                                           // 005923CF..D5
            ++*count;                                                                             // 005923DD
            points = static_cast<float>(static_cast<double>(points) - creatures(self)[k].cr);     // 005923A5 / 005923DF..EE
            if (!(creatures(self)[k].cr <= points)) break;                                        // 005923F2..FD (jnp: less/equal loops)
        }
    }
}

// ======================================================================= 005935D0 BuildSpawnList(): RET 0
void __fastcall kotor_fastcall_005935d0(void* self, void*) {
    const I n = at<I>(self, 0x254);                                                               // 005935ED
    void* memory = live_operator_new(static_cast<U>(n) * 0x24 + 4);                               // 005935F3..FF
    EncounterListEntry* list = nullptr;
    if (memory != nullptr) {
        *static_cast<I*>(memory) = n;                                                             // 00593626 (new[] count cookie)
        list = reinterpret_cast<EncounterListEntry*>(bytes(memory) + 4);
        crt_eh_vector_constructor_006fb55f(list, 0x24, n, reinterpret_cast<void*>(&kotor_fastcall_0058ff80),
                                           reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0)); // 00593615..28
    }
    I count = 0;                                                                                  // [esp+10]
    kotor_fastcall_00590f80(self, nullptr, list, &count);                                         // 0059363B..53
    if (count < static_cast<I>(limit_250(self)) &&
        static_cast<double>(at<float>(self, 0x2d8)) > static_cast<double>(at<float>(self, 0x2d0))) { // 00593658..83
        for (I k = count - 1; k >= 0; --k) {                                                      // 00593685..EA
            if (count >= static_cast<I>(limit_250(self))) break;                                  // 005936A0..AE / 005936EC..FE
            kotor_fastcall_00592280(self, nullptr, list, &count, k, list[k]);                     // 005936B0..CD (9-dword copy)
        }
    }
    if (count < static_cast<I>(limit_250(self))) kotor_fastcall_00590b10(self, nullptr, list, &count); // 00593700..1D
    if (static_cast<double>(at<float>(self, 0x2d8)) < static_cast<double>(at<float>(self, 0x2d0))) // 00593721..32
        at<U>(self, 0x2d8) = 0;                                                                   // 00593734
    if (count == 0) return;                                                                       // 0059373E..40 (temporary leaked)
    if (count > 0) {
        void* spawn_list = bytes(self) + 0x258;                                                   // ebp
        const EncounterListEntry* src = list;                                                     // ebx
        for (I left = count; left != 0; --left, ++src) {                                          // 00593754..F5
            EncounterListEntry* e = static_cast<EncounterListEntry*>(live_operator_new(0x24));    // 00593758..5F
            if (e != nullptr) {                                                                   // CEncounterListEntry() inline 00593774..9D
                (void)kotor_fastcall_00405ed0(e, nullptr);
                { ResRef tmp; (void)unresolved_ref_assign_00406290(e, nullptr, &tmp, ""); }
                e->f10 = 0; e->cr = 0.0f; e->f20 = 0; e->f18 = 0; e->f1c = 0;
            }
            assign_ref(e, src);                                                                   // 005937A1..B1
            e->f10 = src->f10;                                                                    // 005937B6..B9
            if (at<I>(spawn_list, 4) == at<I>(spawn_list, 8)) {                                   // 005937BC..C2
                const I capacity = at<I>(spawn_list, 8);
                menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(spawn_list),
                    nullptr, static_cast<unsigned>(capacity != 0 ? capacity * 2 : 0x10));        // 005937C4..D4
            }
            at<void**>(spawn_list, 0)[at<I>(spawn_list, 4)] = e;                                  // 005937D9..DF
            ++at<I>(spawn_list, 4);                                                               // 005937E2..EE
        }
    }
    if (list != nullptr) {                                                                        // 005937FB..FD
        void* block = bytes(list) - 4;
        c2da_destroy_array_006fa852(list, 0x24, *static_cast<I*>(block),
                                    reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0)); // 005937FF..0E
        live_free(block);                                                                         // 00593813..14
    }
}

// ======================================================================= 00591CA0 SpawnCreatureFromSpawnList(): RET 0
void __fastcall kotor_fastcall_00591ca0(void* self, void*) {
    if (at<U>(self, 0x25c) == 0) return;                                                          // 00591CC4..CE
    void* area = recovered_object_area_004cb120(self, nullptr);                                   // 00591CD4..DD
    if (area == nullptr) return;
    U use_point = 0;                                                                              // [B+1C]
    Vector point = {0.0f, 0.0f, 0.0f};                                                            // [B+44]
    Vector orientation = {1.0f, 0.0f, 0.0f};                                                      // [B+50]
    if (at<I>(self, 0x2bc) > 0) {                                                                 // 00591CE3..09
        use_point = 1;                                                                            // 00591D1F
        float best = 0.0f;                                                                        // [B+10]
        void* entered = creature_by_id(at<U>(self, 0x2e0));                                       // 00591D0F..32
        if (entered != nullptr) {
            const Vector pc = *position_of(entered);                                              // 00591D38..57 [B+2C]
            I chosen = 0;                                                                         // esi
            const I n = at<I>(self, 0x2bc);
            const float* sp = at<float*>(self, 0x2b8);
            for (I k = 0; k < n; ++k, sp += 4) {                                                  // 00591D65..B4
                const double dz = static_cast<double>(sp[2]) - pc.z;
                const double dy = static_cast<double>(sp[1]) - pc.y;
                const double dx = static_cast<double>(sp[0]) - pc.x;
                const double d2 = (dx * dx + dy * dy) + dz * dz;                                  // 00591D79..89
                const double b2 = static_cast<double>(best) * best;                               // 00591D8F..93
                if (d2 > b2) { best = static_cast<float>(std::sqrt(d2)); chosen = k; }            // 00591D97..A6 (fsqrt)
            }
            const float* s = at<float*>(self, 0x2b8) + chosen * 4;                                // 00591DB6..C7
            point.x = s[0]; point.y = s[1]; point.z = s[2];                                       // 00591DC9..D9
            const double angle = static_cast<double>(k_half_pi_00741bb8) - s[3];                  // 00591DBC..DD
            Vector facing;                                                                        // [B+20]
            facing.z = 0.0f;                                                                      // 00591DEE
            x87trig_00591deb(angle, &facing.x, &facing.y);                                        // 00591DE5..FE (x87)
            Vector normalized;                                                                    // [B+5C] (result never used)
            (void)normalize_004ab180(&normalized, &facing);                                       // 00591DE0..07
        }
    }
    Vector found = {0.0f, 0.0f, 0.0f};                                                            // [B+38]
    void* memory = live_operator_new(0xac8);                                                      // 00591E0A..2C
    void* creature = memory != nullptr ? recovered_server_creature_construct_004f7a10(memory, nullptr, k_invalid_00746644, 0) : nullptr; // 00591E3B..4A
    void* const spawn_list = bytes(self) + 0x258;                                                 // ebx
    void* entry = at<void**>(spawn_list, 0)[0];                                                   // 00591E4C..52
    if (w2x_creature_template_005026d0(creature, nullptr, entry, 0) == 0) {                       // 00591E5A..6E
        if (creature != nullptr) vcall<void*>(creature, 0x00, 1u);                                // 00591E70..7E
        return;
    }
    at<U>(creature, 0xa24) = at<U>(self, 4);                                                      // 00591E93..96
    at<U>(creature, 0x9d4) = 0;                                                                   // 00591EA2
    float x = 0.0f, y = 0.0f, z = 0.0f;                                                           // [B+18] [B+14] [B+10]
    bool placed = false;
    if (use_point != 0) {                                                                         // 00591E9C..AC
        if (w1x_area_safe_location_004be860(area, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(&point)),
                k_safe_radius_bits, at<U>(creature, 0x340), 0,
                static_cast<U>(reinterpret_cast<std::uintptr_t>(&found))) == 1) {                 // 00591EB2..D4
            y = found.y; x = found.x;                                                             // 00591ED6..ED
            z = flt(w1x_area_height_bits_004bc380(area, nullptr, &found, 1, nullptr));            // 00591EDE..F6
            placed = true;
        } else {
            ref_debug(at<void**>(spawn_list, 0)[0]);                                              // 00591F19..4D
        }
    }
    if (!placed) {
        if (kotor_fastcall_004bee30(area, nullptr, position_of(self), at<void*>(creature, 0x340), 0, &found,
                                    k_nonvisible_radius_bits) == 1) {                             // 00591F52..76
            y = found.y; x = found.x;                                                             // 00591F78..8F
            z = flt(w1x_area_height_bits_004bc380(area, nullptr, &found, 1, nullptr));            // 00591F80..98
        } else if (found.x == k_minus_one && found.y == k_minus_one && found.z == k_minus_one) {  // 00591FBB..F4
            ref_debug(at<void**>(spawn_list, 0)[0]);                                              // 00591FFA..2D
            vcall<void*>(creature, 0x00, 1u);                                                     // 00592032..38
            ref_debug(at<void**>(spawn_list, 0)[0]);                                              // 0059203A..6A
            return;
        } else {
            x = found.x; y = found.y;                                                             // 00592082..99
            z = flt(w1x_area_height_bits_004bc380(area, nullptr, &found, 1, nullptr));            // 0059208A..A2
        }
    }
    ref_debug(at<void**>(spawn_list, 0)[0]);                                                      // 005920A6..DA / 00591EFA.. / 00591F9C..
    recovered_creature_placement_004fa100(creature, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(area)),
                                          bits(x), bits(y), bits(z), 0, 0);                       // 005920DF..F5
    (void)recovered_ai_level_004b08a0(ai(), nullptr, creature, at<I>(area, 0x128) > 0 ? 1u : 0u); // 005920FA..23
    if (use_point != 0) w1x_object_set_orientation_004cd170(creature, nullptr, &orientation);     // 00592128..37
    ++at<U>(self, 0x264);                                                                         // 0059213C
    EncounterListEntry* first = static_cast<EncounterListEntry*>(at<void**>(spawn_list, 0)[0]);
    at<float>(self, 0x2dc) = static_cast<float>(static_cast<double>(first->cr) + at<float>(self, 0x2dc)); // 00592142..4F
    const double pool = static_cast<double>(at<float>(self, 0x2d8)) -
                        static_cast<EncounterListEntry*>(at<void**>(spawn_list, 0)[0])->cr;       // 00592155..5D
    at<float>(self, 0x2d8) = static_cast<float>(pool);                                            // 00592160 (fst)
    if (pool < static_cast<double>(k_zero_0073d700)) at<U>(self, 0x2d8) = 0;                      // 00592166..73 (unrounded)
    deleteAt_006857a0(spawn_list, nullptr, 0);                                                    // 0059217D..81 (entry not freed)
}

// ======================================================================= 004BB8B0 CSWSArea::CountVisibleToPlayers(pos, max): RET 8
I __fastcall kotor_fastcall_004bb8b0(void* area, void*, const Vector* pos, I max) {
    I count = 0;                                                                                  // edi
    U id = 0;                                                                                     // [B+0C]
    if (recovered_area_first_object_005071b0(area, nullptr, &id) != 1) return 0;                  // 004BB8B6..C7
    Vector from = *pos;                                                                           // 004BB8CD..E5 [B+1C]
    do {
        if (count >= max) return count;                                                           // 004BB8F0..F2
        void* object = nullptr;                                                                   // [B+38] (argument slot)
        if (static_cast<U8>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server(), nullptr), nullptr, id, &object))
                == k_lookup_found_0074666c &&                                                     // 004BB8F8..1D
            vcall<void*>(object, 0x30) != nullptr &&                                              // 004BB923..2E AsSWSCreature
            kotor_fastcall_004ae910(server(), nullptr, at<U>(area, 0x120)) != nullptr) {          // 004BB934..4B
            Vector to = *position_of(vcall<void*>(object, 0x30));                                 // 004BB951..83 [B+10]
            const double dx = static_cast<double>(from.x) - to.x;
            const double dy = static_cast<double>(from.y) - to.y;
            const double dz = static_cast<double>(from.z) - to.z;
            const double d2 = (dz * dz + dy * dy) + dx * dx;                                      // 004BB98B..99
            if (d2 <= static_cast<double>(k_vis_sq_007455bc)) {                                   // 004BB99B..AC
                from.z = static_cast<float>(static_cast<double>(from.z) + k_eye_0073f414);        // 004BB9AE..CA (cumulative)
                to.z = static_cast<float>(static_cast<double>(to.z) + k_eye_0073f414);            // 004BB9CE..E3
                Vector hit = {0.0f, 0.0f, 0.0f};                                                  // [B+28]
                unsigned out = 0;                                                                 // [B+3C] (argument slot)
                if (area_clear_line_of_sight_0050c330(area, nullptr, &from, &to, &hit, &out,
                        k_invalid_00746644, k_invalid_00746644, 0, 0) == 1) ++count;              // 004BB9B2..10
            }
        }
    } while (recovered_area_next_object_005071e0(area, nullptr, &id) == 1);                       // 004BBA11..20
    return count;
}

// ======================================================================= 004BEE30 CSWSArea::ComputeNonVisibleLocation: RET 14h
U __fastcall kotor_fastcall_004bee30(void* area, void*, const Vector* pos, void* object, U, Vector* out, U) {
    I best = 0x60;                                                                                // ebx
    if (object == nullptr) return 0;                                                              // 004BEE35..44
    out->x = k_minus_one; out->y = k_minus_one; out->z = k_minus_one;                             // 004BEE4A..7B
    at<void*>(area, 0x1ac) = object;                                                              // 004BEE7E
    const float step = at<float>(object, 4);                                                      // 004BEE84 [B+1C]
    Vector v = *pos;                                                                              // [B+38]
    int room_index = 0;
    v.z = probe_height(area, &v, &room_index);                                                    // 004BEE99..E7
    // test one candidate point (shared by the three probe sites)
    auto probe = [&](bool check_distance) -> bool {
        if (w2x_area_safe_point_004be5e0(area, nullptr, &v, static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(object))) != 1)
            return false;
        if (check_distance) {
            Vector d;                                                                             // [B+50] / [B+5C]
            d.x = static_cast<float>(static_cast<double>(pos->x) - v.x);
            d.y = static_cast<float>(static_cast<double>(pos->y) - v.y);
            d.z = static_cast<float>(static_cast<double>(pos->z) - v.z);
            const double m = kotor_cdecl_004ab100(&d);                                            // vector_magnitude
            if (!(m <= static_cast<double>(k_radius_007455dc))) return false;                     // test 41h / jp
        }
        return true;
    };
    if (probe(false)) {                                                                           // 004BEEE6..FA
        const I n = kotor_fastcall_004bb8b0(area, nullptr, pos, 0x60);                            // 004BEEFC..01
        if (n < 0x60) { *out = v; best = n; }                                                     // 004BEF06..23
        if (n == 0) return 1;                                                                     // 004BEF25..27
    }
    Vector hit = {0.0f, 0.0f, 0.0f};                                                              // [B+44]
    unsigned los_out = 0;                                                                         // [B+30]
    auto candidate = [&]() -> bool {                                                              // LOS + count, true = done
        if (area_clear_line_of_sight_0050c330(area, nullptr, const_cast<Vector*>(pos), &v, &hit, &los_out,
                k_invalid_00746644, k_invalid_00746644, 0, 0) != 1) return false;
        const I n = kotor_fastcall_004bb8b0(area, nullptr, pos, best);
        if (best > n) { *out = v; best = n; }
        return n == 0;
    };
    float r = flt(k_one_bits);                                                                    // [B+10]
    do {
        const float xmin = static_cast<float>(static_cast<double>(pos->x) - r);                   // 004BEF50..56 [B+14]
        float x = xmin;                                                                           // [B+6C]
        const float xmax = static_cast<float>(static_cast<double>(r) + pos->x);                   // 004BEF5A..68 [B+18]
        float ymin = static_cast<float>(static_cast<double>(pos->y) - r);                         // [B+20]
        float ymax = static_cast<float>(static_cast<double>(r) + pos->y);                         // [B+24]
        if (xmin <= xmax) {                                                                       // 004BEF82..8F
            do {
                if (ymin <= ymax) {                                                               // 004BEFA0..B5
                    float y = ymin;                                                               // [B+70]
                    do {
                        v.x = x; v.y = y;                                                         // 004BEFC0..D8
                        v.z = probe_height(area, &v, &room_index);                                // 004BEFDC..16
                        if (probe(true) && candidate()) return 1;                                 // 004BF01A..BC
                        const double t = (static_cast<double>(ymax) - ymin) + y;                  // 004BF0C2..CA
                        y = static_cast<float>(t);                                                // 004BF0CE fst dword
                        if (!(t <= static_cast<double>(ymax))) break;                             // 004BF0D2..DB unrounded ST0
                    } while (true);
                }
                const double t = static_cast<double>(x) + step;                                   // 004BF0E1..E5
                x = static_cast<float>(t);                                                        // 004BF0E9 fst dword
                if (!(t <= static_cast<double>(xmax))) break;                                     // 004BF0ED..F6 unrounded ST0
            } while (true);
        }
        x = xmin;                                                                                 // 004BF0FC..08
        if (xmin <= xmax) {                                                                       // 004BF104..11
            ymin = static_cast<float>(static_cast<double>(ymin) + step);                          // 004BF117..1F
            ymax = static_cast<float>(static_cast<double>(ymax) - step);                          // 004BF123..2B
            do {
                float y = ymin;                                                                   // 004BF130..34
                if (y <= ymax) {                                                                  // 004BF138..45
                    do {
                        v.x = x; v.y = y;                                                         // 004BF150..68
                        v.z = probe_height(area, &v, &room_index);                                // 004BF16C..A6
                        if (probe(true) && candidate()) return 1;                                 // 004BF1AA..4C
                        const double t = static_cast<double>(y) + step;                           // 004BF24E..52
                        y = static_cast<float>(t);                                                // 004BF256 fst dword
                        if (!(t <= static_cast<double>(ymax))) break;                             // 004BF25A..63 unrounded ST0
                    } while (true);
                }
                const double t = (static_cast<double>(xmax) - xmin) + x;                          // 004BF269..71
                x = static_cast<float>(t);                                                        // 004BF275 fst dword
                if (!(t <= static_cast<double>(xmax))) break;                                     // 004BF279..82 unrounded ST0
            } while (true);
        }
        const double t = static_cast<double>(r) + step;                                           // 004BF288..8C
        r = static_cast<float>(t);                                                                // 004BF290 fst dword
        if (!(t < static_cast<double>(k_max_radius_007455d8))) break;                             // 004BF294..9F unrounded ST0 (test ah,5)
    } while (true);
    return 0;
}

}  // extern "C"
