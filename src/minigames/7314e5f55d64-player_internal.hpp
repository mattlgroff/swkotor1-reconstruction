#pragma once
#include <cstdint>
#include <cstring>
#include "2cc88caf3267-growth.h"
#include "80a83b1dd998-assignment.h"
#include "8f16c687f108-minigame_player_lifecycle.hpp"
namespace t2_private {
using U=std::uint32_t;using I=std::int32_t;using Ref=menu_resources::Ref;using List=kotor_reconstruction::ArrayMutations;
static_assert(sizeof(void*)==4,"original x86 layout");
inline unsigned char* at(void* p,U o){return reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(p)+o);}
inline const unsigned char* at(const void* p,U o){return reinterpret_cast<const unsigned char*>(reinterpret_cast<std::uintptr_t>(p)+o);}
template<class T>T rd(const void* p,U o){T v;std::memcpy(&v,at(p,o),sizeof v);return v;}
template<class T>void wr(void* p,U o,T v){std::memcpy(at(p,o),&v,sizeof v);}
inline void copy_words(void* d,const void* s,U n){for(U o=0;o<n;o+=4)wr(d,o,rd<U>(s,o));}
inline I signed_bits(U u){I v;std::memcpy(&v,&u,4);return v;}
}
extern "C" {
void* __cdecl live_operator_new(std::uint32_t) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
void __fastcall kotor_fastcall_0066d190(void*,void*,std::int32_t);
void __fastcall exo_array_list_destruct_0056b6f0(kotor_reconstruction::ArrayMutations*,void*);
void __fastcall w1x_pointer_list_insert_00616940(void*,void*,std::uint32_t,std::uint32_t) noexcept(false);
void __fastcall deleteAt_006857a0(kotor_reconstruction::ArrayMutations*,void*,std::int32_t);
int __cdecl format_006fadb0(char*,const char*,...);
void __fastcall kotor_fastcall_0066cf20(void*,void*,std::int32_t);
void __fastcall kotor_fastcall_0066d090(void*,void*,std::int32_t);
std::uint32_t __fastcall kotor_fastcall_00670b10(void*,void*);
void* __fastcall gui_load_slot_self_00641db0(void*,void*);
std::uint32_t __fastcall gui_load_slot_zero_0063e7f0(void*,void*);
std::uint32_t __fastcall input_desc_return_true_0063e7a0(void*,void*);
}

