#include "78deac106104-claude_minigame_vm_g.hpp"
#include "7202c2a448bc-claude_minigame_vm_g_pending.hpp"
#include "d1c462888f42-claude_turret_t1_leaves.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*,void*); }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,vmx::U,vmx::U);
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*,void*);

}
namespace {
using vmx::U;using vmx::I;
U read(const void* p,U off){U bits;std::memcpy(&bits,static_cast<const unsigned char*>(p)+off,4);return bits;}
void* lookup(void* manager,U id,U type){
 void* client=vmx::ptr(read(manager,4));
 return kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(client,nullptr),nullptr,id,type);
}
void* follower_cast(void* object){
 using Cast=void* (__fastcall*)(void*,void*);
 void* table=vmx::ptr(read(object,0));
 Cast fn=reinterpret_cast<Cast>(vmx::ptr(read(table,0x14)));
 return fn(object,nullptr);
}
struct String {
 vmx::ExoString value;
 String(){kotor_movie::string_element_construct_005b3190(&value,nullptr);}
 ~String() noexcept(false){frame_LogStringDtor_005e5c20(&value,nullptr);}
 String(const String&)=delete;String& operator=(const String&)=delete;
};
}
extern "C" int __fastcall kotor_fastcall_005cb730(void*,void*,int,int){
 String name;U id;I a,b,c;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_string_005d1080(vmx::vm(),nullptr,&name.value))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_integer_005d1000(vmx::vm(),nullptr,&a))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_integer_005d1000(vmx::vm(),nullptr,&b))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_integer_005d1000(vmx::vm(),nullptr,&c))return vmx::k_pop_failed;
 void* object=lookup(kotor_menu_window_cells::manager_007a39fc,id,0);
 if(object){
  U flags=(a==0?1u:0u)|(c!=0?8u:0u)|(b!=0?32u:0u);
  void* table=vmx::ptr(read(object,0));
  const char* text=WindowCStr005e5670(&name.value,nullptr);
  using Play=void (__fastcall*)(void*,void*,const char*,float,U);
  Play fn=reinterpret_cast<Play>(vmx::ptr(read(table,0)));
  fn(object,nullptr,text,1.0f,flags);
 }
 return 0;
}
extern "C" int __fastcall kotor_fastcall_005cb990(void*,void*,int routine,int){
 U id;I value;
 if(!recovered_vm_outer_pop_object_005d10c0(vmx::vm(),nullptr,&id))return vmx::k_pop_failed;
 if(!recovered_vm_outer_pop_integer_005d1000(vmx::vm(),nullptr,&value))return vmx::k_pop_failed;
 void* follower=lookup(kotor_menu_window_cells::manager_007a39fc,id,4);
 if(follower){
  if(routine==590)kotor_fastcall_0066e8e0(follower,nullptr,value);
  else if(routine==604)(void)kotor_fastcall_0066c900(follower,nullptr,value);
 }
 return 0;
}
extern "C" int __fastcall kotor_fastcall_005cba20(void* commands,void*,int routine,int){
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 U id=read(commands,8);
 void* object=lookup(manager,id,0);
 if(!object)return 0;
 if(routine==591)kotor_fastcall_0066c190(object,nullptr,1);
 else if(routine==592||routine==598||routine==605){
  if(follower_cast(object)){
   void* follower=follower_cast(object);
   if(routine==592)kotor_fastcall_0066e5c0(follower,nullptr,1);
   else if(routine==598)kotor_fastcall_0066e2a0(follower,nullptr,1);
   else kotor_fastcall_0066e7a0(follower,nullptr,1);
  }
 }
 return 0;
}
extern "C" int __fastcall kotor_fastcall_005cc000(void*,void*,int routine,int){
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 void* game=kotor_m15e_client_get_mini_game_005edb00(vmx::ptr(read(manager,4)),nullptr);
 I index;
 if(!recovered_vm_outer_pop_integer_005d1000(vmx::vm(),nullptr,&index))return vmx::k_pop_failed;
 U id=0xff;
 if(game){
  void* object;
  if(routine==613)object=kotor_fastcall_00671570(game,nullptr,index);
  else if(routine==615)object=kotor_fastcall_00671630(game,nullptr,index);
  else{U bits;std::memcpy(&bits,&routine,4);object=vmx::ptr(bits);}
  if(object)id=read(object,8);
 }
 return recovered_vm_outer_push_object_005d10d0(vmx::vm(),nullptr,id)?0:vmx::k_push_failed;
}

