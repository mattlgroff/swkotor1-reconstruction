#pragma once
// Lane W5 tier 3 (G18 server side, second part): CSWSObject::AIActionOpenDoor 0057D490 and its callees.
//   runner 0057F4A0 (W1) -> action type 14h -> AIActionOpenDoor
//     creature path: ActivityManager(2), GetUseRange, (same area and GetIsInUseRange) ? EvaluateLock + AddActionToFront(14h)
//                    : AddActionToFront(14h) [+ Wait/Unlock] + AddMoveToPointActionToFront
//     open now:      AI event 7 (caller = opener, target = door) -> CSWSDoor::EventHandler 0058B850 -> OpenDoor 00589C70 (t2)
//                    + AddActionToFront(1Eh wait 0.5) ; returns [745DC0] = 2
//     failure:       +1F0 = 0, returns [745DC4] = 3
// Conventions as t1/t2: thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; live_operator_new / live_free only; an
// unimplemented callee traps with its original address.
#include "524ad07f85b0-w5_t2.hpp"
#include "050dfb64d53e-w5_action_queue.hpp"
#include "9c49a8c0ad27-w5_walkmesh.hpp"

extern "C" {
using w5::U; using w5::I; using w5::Vector;

// ---------------- W5 t3 exports ----------------
// 004CCB00 AddActionToFront: W1 tier6 export (ruling); W5 keeps only the producer-shape helper
unsigned __fastcall recovered_object_add_action_to_front_004ccb00(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);
void __cdecl w5_add_action_to_front_body(void* object, U type, U group_word, const U* pairs);   // shared shape (pairs[26])
// 004F87D0 CSWSCreature::ActivityManager(mode): RET 4
void __fastcall w5_creature_activity_manager_004f87d0(void* creature, void*, U mode);
// 004EE440 CSWSCreature::GetUseRange(id, Vector* pos, float* range, flag): RET 10h, EAX 0/1
U __fastcall w5_creature_get_use_range_004ee440(void* creature, void*, U id, Vector* pos, float* range, U flag);
// 004F6000 CSWSCreature::GetIsInUseRange(id, float extra, flag): RET 0Ch, EAX 0/1
U __fastcall w5_creature_get_is_in_use_range_004f6000(void* creature, void*, U id, float extra, U flag);
// 004F0B20 CSWSCreature::EvaluateLock(CSWSObject*, desired lock state): RET 8, EAX 0/1
U __fastcall w5_creature_evaluate_lock_004f0b20(void* creature, void*, void* object, U desired);
// 004F8A50 CSWSCreature::AddMoveToPointActionToFront(17 dwords): RET 44h, EAX 1
U __fastcall w5_creature_add_move_to_point_action_to_front_004f8a50(void* creature, void*, U group, const Vector* point,
    U area_id, U target_id, U run, float range, float a6, U a7, U a8, U a9, U a10, U a11, U a12,
    const Vector* a13, U a14, U a15, U a16);
// 004F34A0 CSWSCreature::SetLockOrientationToObject(id, force): RET 8
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void* creature, void*, U id, U force);
// 005891B0 CSWSDoor::GetActionPoint(Vector* out, state, byte index): RET 0Ch, EAX = out
Vector* __fastcall w5_door_get_action_point_005891b0(void* door, void*, Vector* out, U state, U index);
// 00589240 CSWSDoor::GetNearestActionPoint(Vector* out, const Vector* from, flag): RET 0Ch, EAX = out
Vector* __fastcall w5_door_get_nearest_action_point_00589240(void* door, void*, Vector* out, const Vector* from, U flag);
// 00506400 CSWSArea::PositionWalkable(Vector by value): RET 0Ch, EAX 0/1
U __fastcall w5_area_position_walkable_00506400(void* area, void*, Vector p);
// 00581A10 CSWRoomSurfaceMesh::GetSurfaceMaterialWalkCheckOnly(Vector by value): RET 0Ch, EAX material or 0
U __fastcall w5_surface_mesh_get_material_walk_check_only_00581a10(void* mesh, void*, Vector p);
// 0057D490 CSWSObject::AIActionOpenDoor(node): RET 4 (declared in source/w5_action_bodies.hpp)

// ---------------- imports: existing providers in build/1122 (exact names) ----------------
U __fastcall c2da_current_get_int_00413660(void* table, void*, U row, void* column, I* out); // C2DA::GetINTEntry [0376]
U __fastcall frame_LogIsEmpty_005e58a0(void* string, void*);                             // CExoString::IsEmpty [0457]
U __fastcall async_string_equal_005e5200(void* string, void*, void* other);              // CExoString::operator== [0188]
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U flag);   // GetItemRepository [1086]
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);              // GetItemByGameObjectID [1025]
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U slot);      // CSWInventory::GetItemInSlot [1025]
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data); // SendFeedbackMessage [1201a]
int __fastcall area_clear_line_of_sight_0050c330(void* area, void*, void* a, void* b, void* c, unsigned* d,
    unsigned caller, unsigned target, int flag, int e);                                   // CSWSArea::ClearLineOfSight [0526]
U __fastcall w1x_area_height_bits_004bc380(void* area, void*, void* point, U flag, U* room); // CSWSArea::ComputeHeight (0886 + W1 inc)

// ---------------- imports owned by other lanes, absent from build/1122 (trap fallback via /alternatename) ----
void __fastcall recovered_set_stealth_mode_0050ee50(void* creature, void*, U a, U b);   // W2 t3b
U __fastcall recovered_find_item_with_tag_00555ed0(void* repository, void*, void* tag);  // W2 t3b
}
