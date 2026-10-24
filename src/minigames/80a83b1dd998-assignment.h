#pragma once
#include "d8f703239b61-resources.h"
#include <type_traits>
namespace resource_ref_assignment {
using U8=unsigned char; using U32=unsigned int;
using Ref=menu_resources::Ref;
using Lowercase=int(__cdecl*)(int);
struct Bindings { Lowercase lower_006fb766; };
// Main provides the sole immutable descriptor, selecting the existing genuine
// lowercase provider. No production descriptor or replacement locale here.
extern const Bindings bindings;
static_assert(sizeof(void*)==4 && sizeof(Ref)==16 && alignof(Ref)==1,"selected resource Ref epoch");
static_assert(std::is_trivially_default_constructible<Ref>::value && std::is_trivially_destructible<Ref>::value,"live Ref lifetime; assignment owns no cleanup");
__declspec(noinline) Ref* __fastcall assign_00406120(Ref*,void*,Ref*,const Ref*);
}
