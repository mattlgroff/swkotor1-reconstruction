#pragma once
#include <cstdint>
// t1 batch A: exact x86 callable contracts, independently derived from original returns.
extern "C" {
std::int32_t __fastcall kotor_fastcall_005cb440(void* self, void*);
std::uint32_t __fastcall kotor_fastcall_0066c060(void* self, void*);
float __fastcall kotor_fastcall_0066c8d0(void* self, void*, float radius);
std::int32_t __fastcall kotor_fastcall_0066c900(void* self, void*, std::int32_t hp);
float __fastcall kotor_fastcall_0066c920(void* self, void*, float invincibility);
std::int32_t __fastcall kotor_fastcall_0066c950(void* self, void*, std::int32_t hp);
std::int32_t __fastcall kotor_fastcall_0066c9f0(void* self, void*, std::int32_t sound);
void __fastcall kotor_fastcall_0066ca10(void* self, void*, std::int32_t sound, std::int32_t frequency);
std::int32_t __fastcall kotor_fastcall_0066ca30(void* self, void*, std::int32_t sound);
void __fastcall kotor_fastcall_0066ca50(void* self, void*, std::int32_t sound, std::int32_t random_flag);
std::int32_t __fastcall kotor_fastcall_0066ca70(void* self, void*, std::int32_t sound);
}
