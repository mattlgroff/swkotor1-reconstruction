#pragma once
#include <stdint.h>
#include <string.h>
#include "75d9d34323f7-startup_string.h"
namespace menu_resources {
using Word=uint32_t;using String=kotor_reconstruction::StartupString;
struct Ref {unsigned char bytes[16];__declspec(noinline) Ref* __thiscall construct_00406d80(const char*);};
struct MemoryStatus {Word words[8];};
struct Manager {
 alignas(4) unsigned char bytes[0x60];
 Word word(unsigned n)const{Word v;memcpy(&v,bytes+n,4);return v;}
 void word(unsigned n,Word v){memcpy(bytes+n,&v,4);}
 __declspec(noinline) Manager* __thiscall construct_00409bf0();
 __declspec(noinline) String* __thiscall cache_004085f0(String*,String*,Word);
 __declspec(noinline) Word __thiscall encapsulated_004087a0(String*,Word);
 __declspec(noinline) Word __thiscall image_004087c0(String*);
 __declspec(noinline) Word __thiscall fixed_004087e0(String*);
 __declspec(noinline) Word __thiscall directory_00408800(String*);
 __declspec(noinline) Word __thiscall exists_00408bc0(Ref*,Word,Word*);
};
using ThreadStart=Word(__stdcall*)(void*);
static_assert(sizeof(Ref)==16 && sizeof(Manager)==0x60 && sizeof(MemoryStatus)==32 && sizeof(void*)==4,"observed x86 storage");
}
// These are missing typed integration boundaries, never original addresses.
// Fastcall ECX/unused EDX preserves thiscall's receiver and stack purge.
extern "C" {
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*,void*,menu_resources::Ref*,const char*);
menu_resources::String* __stdcall unresolved_string_copy_00406c70(menu_resources::String*,menu_resources::String*,menu_resources::Word,menu_resources::Word*);
menu_resources::Word __fastcall unresolved_add_key_00406e20(menu_resources::Manager*,void*,menu_resources::String*,menu_resources::Word,menu_resources::Word);
menu_resources::Word __fastcall unresolved_key_entry_00407230(menu_resources::Manager*,void*,menu_resources::Ref*,menu_resources::Word,menu_resources::Word*,menu_resources::Word*);
void* __fastcall unresolved_keylist_ctor_00406cf0(void*,void*);
void* __fastcall unresolved_critical_ctor_005e9290(void*,void*);
void __stdcall unresolved_gff_hash_init_00410520();
void __stdcall unresolved_global_memory_status_0073d1c0(menu_resources::MemoryStatus*);
menu_resources::Word __stdcall unresolved_create_thread_0073d1f0(void*,menu_resources::Word,menu_resources::ThreadStart,void*,menu_resources::Word,menu_resources::Word*);
menu_resources::Word __stdcall unresolved_async_rim_thread_00409b90(void*);
}
