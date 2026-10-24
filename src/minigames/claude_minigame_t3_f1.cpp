#include "f1bcb74583b6-claude_minigame_t3_f1.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,unsigned,unsigned) noexcept(false);
}
namespace {
using U=std::uint32_t;using I=std::int32_t;
static_assert(sizeof(void*)==4,"original x86 pointers");
template<class T>T read(const void*p,U offset){T v;std::memcpy(&v,reinterpret_cast<const void*>(reinterpret_cast<U>(p)+offset),sizeof v);return v;}
}
extern "C" void* __fastcall kotor_fastcall_00671570(void*self,void*,I index) noexcept(false){
 if(index>=read<I>(self,0x30))return nullptr;
 void*list=read<void*>(self,0x2c);
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 const U id=read<U>(list,U(index)*4u);
 void*app=read<void*>(manager,4);
 void*array=kotor_fastcall_005edaf0(app,nullptr);
 return kotor_fastcall_0066bf30(array,nullptr,id,2);
}
extern "C" void* __fastcall kotor_fastcall_00671630(void*self,void*,I index) noexcept(false){
 if(index>=read<I>(self,0x48))return nullptr;
 void*list=read<void*>(self,0x44);
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 const U id=read<U>(list,U(index)*4u);
 void*app=read<void*>(manager,4);
 void*array=kotor_fastcall_005edaf0(app,nullptr);
 return kotor_fastcall_0066bf30(array,nullptr,id,3);
}
