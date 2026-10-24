#pragma once

#include <cstdint>

// Original thiscall methods: ECX=self, EDX is an unused fastcall bridge slot.
// Vec3 arguments remain byte-addressed to preserve the original alias order.
extern "C" {
void __fastcall kotor_fastcall_0066cf20(void* self, void*, std::int32_t loops);
float __fastcall kotor_fastcall_0066cf40(void* self, void*, float value);
float __fastcall kotor_fastcall_0066cf70(void* self, void*, float value);
float __fastcall kotor_fastcall_0066cfa0(void* self, void*, float value);
void* __fastcall kotor_fastcall_0066cfd0(void* self, void*, void* result,
                                     const void* value);
void __fastcall kotor_fastcall_0066d090(void* self, void*, std::int32_t loops);
std::uint32_t __fastcall kotor_fastcall_00670b10(void* self, void*);
}
