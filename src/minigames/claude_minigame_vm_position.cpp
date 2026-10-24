#include "65501125d829-claude_minigame_vm_position.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,vmx::U,vmx::U);
void* __fastcall kotor_fastcall_0066d5d0(void*,void*,void*);
}
extern "C" int __fastcall kotor_fastcall_005cc280(void*,void*,int,int){
 vmx::Vector value{};
 vmx::U id;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 vmx::U client;
 std::memcpy(&client,static_cast<unsigned char*>(manager)+4,4);
 void* array=kotor_fastcall_005edaf0(vmx::ptr(client),nullptr);
 void* follower=kotor_fastcall_0066bf30(array,nullptr,id,4);
 if(follower){
  const void* returned=kotor_fastcall_0066d5d0(follower,nullptr,&value);
  // The original reads EAX's vector, which need not be the supplied buffer.
  vmx::U bits[3];
  std::memcpy(bits,returned,sizeof(bits));
  std::memcpy(&value,bits,sizeof(bits));
 }
 return recovered_vm_outer_push_vector_005d1050(vmx::vm(),nullptr,value)?0:vmx::k_push_failed;
}

