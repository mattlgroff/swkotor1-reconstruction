#pragma once
// W5 action-queue producers. See ACTION-ABI.md.
#include "4122183abfad-w5_types.hpp"
static_assert(sizeof(void*) == 4, "x86 node parameter pointers are stored as DWORDs");

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }   // 0262 owner of 005B3190

extern "C" {
using w5::U; using w5::Vector;
// authored here
// W1 (lane-w1 tier5b) exports
void __fastcall recovered_object_add_action_node_parameter_004cac30(void* object, void*, void* node, unsigned index, unsigned type, void* value);
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);
void __cdecl w5_add_action_body(void* object, U type, U group_word, const U* pairs);   // shared body (pairs[26])
U __fastcall w5_add_drive_action_004eb510(void* creature, void*, U group, const Vector* position, U heading,
    U arg4, U flags, U mode);
// reused natives present in the 1115 map
void* __fastcall recovered_object_area_004cb120(void* self, void*);                        // 1060
void* __fastcall resource_append_005e9dc0(void* list, void*, void* value);                // 0264 AddTail
unsigned __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);    // 0168 GetNext
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);             // 0199 CExoString::operator=
}
inline void* assign_string_005e5c50(void* self, void*, void* other) { return async_assign_string_005e5c50(self, nullptr, other); }
