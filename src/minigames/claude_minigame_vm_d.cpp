// Four complete minigame VM commands. Dependencies use immutable reviewed contracts.
#include "538b4ff5e119-claude_minigame_vm_d.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
#include "d1c462888f42-claude_turret_t1_leaves.hpp"
#include "4f65a6036765-claude_minigame_t3_a.hpp"
namespace kotor_menu_window_cells{extern void* volatile manager_007a39fc;}
extern "C" {
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*,void*);
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,vmx::U,vmx::U);
}
namespace {
using vmx::U;using vmx::I;
U read_word(const void* base,U off){U value;std::memcpy(&value,vmx::ptr(vmx::word(base)+off),4);return value;}
I read_int(const void* base,U off){I value;std::memcpy(&value,vmx::ptr(vmx::word(base)+off),4);return value;}
void* client(){void* manager=kotor_menu_window_cells::manager_007a39fc;return vmx::ptr(read_word(manager,4));}
void* follower(U id){void* array=kotor_fastcall_005edaf0(client(),nullptr);return kotor_fastcall_0066bf30(array,nullptr,id,4);}
int push_status(U result){return result!=0?0:vmx::k_push_failed;}
}
extern "C" int __fastcall kotor_fastcall_005cbe90(void*,void*,int routine,int){
 float value=0.0f;
 void* game=kotor_m15e_client_get_mini_game_005edb00(client(),nullptr);
 if(game!=nullptr){
  if(routine==608)kotor_fastcall_00670b30(game,nullptr,&value,nullptr);
  else if(routine==609)kotor_fastcall_00670b30(game,nullptr,nullptr,&value);
 }
 return push_status(recovered_vm_outer_push_float_005d1030(vmx::vm(),nullptr,value));
}
extern "C" int __fastcall kotor_fastcall_005cc0a0(void*,void*,int routine,int){
 U id;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 void* p=follower(id);I value=0;
 if(p!=nullptr){
  switch(routine){
  case 616:value=read_int(p,0x8c);break;
  case 617:value=read_int(p,0x90);break;
  case 621:value=read_int(p,0x80);break;
  case 624:value=read_int(p,0x78);break;
  case 665:value=kotor_fastcall_005cb440(p,nullptr);break;
  default:break;
  }
 }
 return push_status(recovered_vm_outer_push_integer_005d1010(vmx::vm(),nullptr,value));
}
extern "C" int __fastcall kotor_fastcall_005cc3c0(void*,void*,int routine,int){
 U id;float value;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_float_005d1020(vmx::vm(),nullptr,&value))return vmx::k_pop_failed;
 void* p=follower(id);
 if(p!=nullptr){
  if(routine==620)(void)kotor_fastcall_0066c8d0(p,nullptr,value);
  else if(routine==648)(void)kotor_fastcall_0066c920(p,nullptr,value);
 }
 return 0;
}
extern "C" int __fastcall kotor_fastcall_005cd0a0(void*,void*,int routine,int){
 U id;I sound;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_integer_005d1000(vmx::vm(),nullptr,&sound))return vmx::k_pop_failed;
 void* p=follower(id);I value=0;
 if(p!=nullptr){
  switch(routine){
  case 683:value=kotor_fastcall_0066c9f0(p,nullptr,sound);break;
  case 685:value=kotor_fastcall_0066ca30(p,nullptr,sound);break;
  case 687:value=kotor_fastcall_0066ca70(p,nullptr,sound);break;
  default:break;
  }
 }
 return push_status(recovered_vm_outer_push_integer_005d1010(vmx::vm(),nullptr,value));
}
