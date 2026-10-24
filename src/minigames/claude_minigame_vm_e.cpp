// Original minigame info/predicate handlers, complete selector behavior.
#include "b1b3ba66e348-claude_minigame_vm_e.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells{extern void* volatile manager_007a39fc;}
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,vmx::U,vmx::U);
}
namespace {
using vmx::U;
U read_word(const void* p,U off){U v;std::memcpy(&v,vmx::ptr(vmx::word(p)+off),4);return v;}
unsigned char read_byte(const void* p,U off){return *static_cast<const volatile unsigned char*>(vmx::ptr(vmx::word(p)+off));}
void* lookup(void* manager,U id){void* client=vmx::ptr(read_word(manager,4));void* array=kotor_fastcall_005edaf0(client,nullptr);return kotor_fastcall_0066bf30(array,nullptr,id,0);}
void* cast_slot(void* object,U offset){
 using Cast=void* (__fastcall*)(void*,void*);
 void* table=vmx::ptr(read_word(object,0));
 Cast fn=reinterpret_cast<Cast>(vmx::ptr(read_word(table,offset)));
 return fn(object,nullptr);
}
int push_integer(U bits){vmx::I value;std::memcpy(&value,&bits,4);return recovered_vm_outer_push_integer_005d1010(vmx::vm(),nullptr,value)?0:vmx::k_push_failed;}
}
extern "C" int __fastcall kotor_fastcall_005cb870(void* commands,void*,int routine,int){
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 U id=read_word(commands,8);
 void* object=lookup(manager,id);
 U value=0xffffffffu;
 if(object!=nullptr){
  switch(routine){
  case 587:value=read_word(object,0x1c);break;
  case 588:value=read_word(object,0x2c);break;
  case 595:
   if(cast_slot(object,0x14)!=nullptr)value=read_word(cast_slot(object,0x14),0x174);
   break;
  case 596:
   if(cast_slot(object,0x14)!=nullptr)value=read_word(cast_slot(object,0x14),0x184);
   break;
  default:break;
  }
 }
 return push_integer(value);
}
extern "C" int __fastcall kotor_fastcall_005cbc80(void*,void*,int routine,int){
 U id;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 void* object=lookup(manager,id);
 U value=0;
 if(object!=nullptr){
  switch(routine){
  case 599:value=cast_slot(object,0x14)!=nullptr?1u:0u;break;
  case 600:value=cast_slot(object,0x18)!=nullptr?1u:0u;break;
  case 601:value=cast_slot(object,0x1c)!=nullptr?1u:0u;break;
  case 603:
   if(cast_slot(object,0x20)!=nullptr)value=1;
   [[fallthrough]]; // original005CBD0B follows obstacle cast, even when true
  case 602:
   if(cast_slot(object,0x1c)!=nullptr && read_byte(cast_slot(object,0x1c),0x1a0)!=0)value=1;
   break;
  default:break;
  }
 }
 return push_integer(value);
}
