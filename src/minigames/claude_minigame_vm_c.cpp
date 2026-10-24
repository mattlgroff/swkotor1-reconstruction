// Shared minigame VM follower-field commands005CBD60/005CC190/005CC1E0.
#include "a6c59fb9bb1b-claude_minigame_vm_c.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,vmx::U,vmx::U);
}
namespace {
using vmx::U;
U read_word(const void* base,U offset){U value;std::memcpy(&value,vmx::ptr(vmx::word(base)+offset),4);return value;}
void write_word(void* base,U offset,U value){std::memcpy(vmx::ptr(vmx::word(base)+offset),&value,4);}
void* follower(void* manager,U id){
 void* client=vmx::ptr(read_word(manager,4));
 void* array=kotor_fastcall_005edaf0(client,nullptr);
 return kotor_fastcall_0066bf30(array,nullptr,id,4); // original push4,pushid
}
int push_status(U result){return result!=0?0:vmx::k_push_failed;}
}
extern "C" int __fastcall kotor_fastcall_005cbd60(void* commands,void*,int,int){
 void* manager=kotor_menu_window_cells::manager_007a39fc; // 005CBD63
 U id=read_word(commands,8);                            // 005CBD69
 void* p=follower(manager,id);
 U bits=p!=nullptr?read_word(p,0x19c):0;
 vmx::I value;std::memcpy(&value,&bits,4);
 return push_status(recovered_vm_outer_push_integer_005d1010(vmx::vm(),nullptr,value));
}
extern "C" int __fastcall kotor_fastcall_005cc190(void*,void*,int,int){
 U id;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 void* p=follower(manager,id);
 if(p!=nullptr)write_word(p,0x9c,read_word(p,0xa0)); // bitcopy, no float arithmetic
 return 0;
}
extern "C" int __fastcall kotor_fastcall_005cc1e0(void*,void*,int routine,int){
 U id;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 void* p=follower(manager,id);
 U bits=0;
 if(p!=nullptr){
  if(routine==619)bits=read_word(p,0x84);
  else if(routine==642)bits=read_word(p,0xa0);
 }
 float value;std::memcpy(&value,&bits,4);
 return push_status(recovered_vm_outer_push_float_005d1030(vmx::vm(),nullptr,value));
}
