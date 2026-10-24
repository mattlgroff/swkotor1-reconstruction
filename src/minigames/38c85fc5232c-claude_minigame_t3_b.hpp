#pragma once
#include <cstdint>
struct Vector3;
struct Quaternion;
struct W4Vector;
namespace menu_resources { struct Ref; }
extern "C" {
Vector3* __fastcall quaternion_xdir_0044f3d0(const Quaternion* q, void*, Vector3* out);
void __fastcall kotor_fastcall_005089e0(void* self, void*, menu_resources::Ref value) noexcept(false);
void* __fastcall kotor_fastcall_00605fa0(void* self, void*, W4Vector* position);
}
