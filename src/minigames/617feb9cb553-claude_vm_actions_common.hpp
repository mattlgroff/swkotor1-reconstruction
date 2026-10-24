#pragma once
// Lane endar-actions: shared conventions for NWScript engine-command (ACTION) bodies
// (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only).
//  * Original handler: int __thiscall CSWVirtualMachineCommands::ExecuteCommandX(int routine, int argc), RET 8.
//    Export: extern "C" int __fastcall recovered_vm_<snake>_<addr>(void* self, void* edx, int routine, unsigned argc)
//    (decorated @recovered_vm_<snake>_<addr>@16), resolved by address in the 0854 Commands::resolve chain.
//  * Return: 0 success, -2001 (0xFFFFF82F) pop failure, -2000 (0xFFFFF830) push failure (original literals).
//  * Only bindings to existing C-linkage providers of the current candidate (c17 map; object in brackets);
//    missing callees are reconstructed as kotor_fastcall_<addr> / kotor_cdecl_<addr> in the lane files.
//  * Memory: live_operator_new / live_free only (w5_types.hpp).  No original address is ever dereferenced.
#include "25ffbc410ea0-w5_t3.hpp"
#include <cstdint>
#include <cstring>

namespace frame_control { extern unsigned message_manager_007a3a00; }   // g_pVirtualMachine 007A3A00 [0456]

extern "C" {
using w5::U; using w5::I; using w5::Vector;
// ---- CVirtualMachine outer stack API (thiscall on CVirtualMachine*, [007A3A00]) ----
U __fastcall recovered_vm_outer_pop_integer_005d1000(void* vm, void*, I* out);                 // StackPopInteger [vm_actions_t1]
U __fastcall recovered_vm_outer_push_integer_005d1010(void* vm, void*, I value);               // StackPushInteger [1196]
U __fastcall recovered_vm_outer_pop_float_005d1020(void* vm, void*, float* out);               // StackPopFloat [vm_actions_t2]
U __fastcall recovered_vm_outer_push_float_005d1030(void* vm, void*, float value);             // StackPushFloat [vm_actions_t4]
U __fastcall recovered_vm_outer_pop_vector_005d1040(void* vm, void*, Vector* out);             // StackPopVector [1194n]
U __fastcall recovered_vm_outer_push_vector_005d1050(void* vm, void*, Vector value);           // StackPushVector (by value) [1194n]
U __fastcall recovered_vm_outer_pop_string_005d1080(void* vm, void*, void* exo_string);        // StackPopString [vm_actions_t1]
U __fastcall recovered_vm_outer_push_string_005d1090(void* vm, void*, void* exo_string);       // StackPushString [vm_actions_t1]
U __fastcall recovered_vm_outer_pop_engine_structure_005d10a0(void* vm, void*, I type, void** out);  // StackPopEngineStructure [1194n]
U __fastcall recovered_vm_outer_push_engine_structure_005d10b0(void* vm, void*, I type, void* value); // StackPushEngineStructure [1194n]
U __fastcall recovered_vm_outer_pop_object_005d10c0(void* vm, void*, U* out);                  // StackPopObject [1196]
U __fastcall recovered_vm_outer_push_object_005d10d0(void* vm, void*, U id);                   // StackPushObject [1196]
U __fastcall recovered_vm_stack_pop_command_005d10e0(void* vm, void*, void* situation);        // StackPopCommand [1194s]
// ---- CExoString (8 bytes: +0 char*, +4 length) ----
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                                  // ~CExoString [0459]
const char* __fastcall WindowCStr005e5670(void* s, void*);                                     // CExoString::CStr [0078]
void* __fastcall WindowCopy005e5b00(void* s, void*, const void* other);                        // CExoString(const CExoString&) [0078]
void* __fastcall resman_string_from_int_005e5bc0(void* s, void*, int value);                   // CExoString(int) [0266]
void* __fastcall resman_string_counted_005e5b70(void* s, void*, const char* text, int length); // CExoString(const char*, int) [0267]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);                    // CExoString::operator= [0199]
}

namespace vmx {
using namespace w5;
constexpr I k_pop_failed = -2001;    // 0xFFFFF82F
constexpr I k_push_failed = -2000;   // 0xFFFFF830
constexpr U k_invalid = 0x7f000000u; // [00746644] OBJECT_INVALID
struct ExoString { char* text; U length; };
static_assert(sizeof(ExoString) == 8, "CExoString is 8 bytes");
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
// CExoString::CExoString() is an inline two-store in the original handlers (mov [s],0; mov [s+4],0)
inline void exo_default(ExoString* s) { s->text = nullptr; s->length = 0; }
inline void exo_destroy(ExoString* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
// CGameObjectArray::GetGameObject through the server object array (inline view of 004AED70); AL 0 = found
inline bool lookup(U id, void** out) {
    return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)) == 0;
}
}  // namespace vmx
