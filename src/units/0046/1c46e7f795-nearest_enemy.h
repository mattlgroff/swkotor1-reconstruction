#pragma once
#include <cstdint>
namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { using Vector = kotor_scene3d::Vector; }
extern "C" {
std::uint32_t __fastcall kotor_fastcall_004f2de0(void*, void*, std::uint32_t /*radius: a float, taken as its raw word (a2: 004F2DE0 copies it with mov)*/, std::uint32_t, std::uint32_t);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall kotor_fastcall_004ae750(void*, void*, std::uint32_t);
void* __fastcall kotor_fastcall_004ae770(void*, void*, std::uint32_t);
std::uint32_t __fastcall recovered_object_ai_state_reputation_0057cd90(void*, void*, std::uint32_t);
void* __fastcall recovered_creature_visible_element_00517a20(void*, void*, std::uint32_t);
std::uint32_t __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
std::uint32_t __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);
float* __cdecl kotor_cdecl_00423bc0(float*, const float*, const float*);
float __cdecl kotor_cdecl_004ab100(const float*);
std::int32_t __fastcall area_clear_line_of_sight_0050c330(void*, void*, kotor_world_los::Vector*,
    kotor_world_los::Vector*, kotor_world_los::Vector*, std::uint32_t*, std::uint32_t,
    std::uint32_t, std::int32_t, std::int32_t);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
