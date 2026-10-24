#pragma once
#include <stdint.h>
#include <stddef.h>
#include "68d07cacdc29-array_mutations.h"
namespace font {
using U=uint32_t;
struct Texture;
struct Safe {Texture* volatile target;};
using List=kotor_reconstruction::ArrayMutations;
struct Texture {void** volatile vtable;List registrations;Safe reference;U environment;};
struct Gui {unsigned char prefix[0x18];Safe font;unsigned char middle[0x10];U volatile dirty;unsigned char suffix[0x18];};
struct Providers {
 U (__fastcall *remove)(List*,void*,void*);
 void (__fastcall *allocate_list)(List*,void*,U);
 void* (__cdecl *allocate)(U);
 Texture* (__cdecl *lookup)(const char*,const char*,void*,U);
 Texture* (__fastcall *texture_ctor)(void*,void*,const char*,const char*);
 Texture* (__fastcall *reference_ctor)(void*,void*);
 U (__fastcall *load)(Texture*,void*,void*);
 List* cache;
};
extern Providers providers;
extern "C" {
 U __cdecl release_00420d70(Texture*);
 Texture* __cdecl get_00423490(const char*,Texture*,const char*,void*,U);
 Texture* __cdecl reference_00423a60(const char*,const char*);
 Safe* __fastcall assign_00447fa0(Safe*,void*,Texture*);
 U __fastcall font_0045bd70(Gui*,void*,const char*);
}
static_assert(sizeof(void*)==4&&sizeof(Safe)==4&&sizeof(List)==12&&sizeof(Texture)==24&&sizeof(Gui)==72,"observed x86 prefix storage");
static_assert(offsetof(Texture,registrations)==4&&offsetof(Texture,reference)==16&&offsetof(Gui,font)==0x18&&offsetof(Gui,dirty)==0x2c,"actual embedded slots");
}
