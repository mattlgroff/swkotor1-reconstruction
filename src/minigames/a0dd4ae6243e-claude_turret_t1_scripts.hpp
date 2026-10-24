#pragma once
#include <cstdint>
#include "d8f703239b61-resources.h"
static_assert(sizeof(menu_resources::Ref)==16 && alignof(menu_resources::Ref)==1,"CResRef ABI");
extern "C" {
menu_resources::Ref* __fastcall kotor_fastcall_0066c970(void* self, void*, menu_resources::Ref* out, std::int32_t index);
void __fastcall kotor_fastcall_0066c9c0(void* self, void*, std::int32_t index, const menu_resources::Ref* source);
menu_resources::Ref* __fastcall kotor_fastcall_0066d100(void* self, void*, menu_resources::Ref* out, std::int32_t index);
void __fastcall kotor_fastcall_0066d150(void* self, void*, std::int32_t index, const menu_resources::Ref* source);
}
