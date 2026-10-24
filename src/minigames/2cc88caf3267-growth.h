#pragma once
#include "68d07cacdc29-array_mutations.h"
#include <cstdint>
namespace menu_list_growth {
using Word=std::uint32_t;
using List=kotor_reconstruction::ArrayMutations;
struct Bindings {void* (__cdecl* allocate_006fa7e6)(Word);void (__cdecl* free_006fa390)(void*);};
// One support binding producer; no original global or copied List declaration.
extern const Bindings& bindings;
__declspec(noinline) void __fastcall allocate_005e03c0(List*,void*,Word);
}
