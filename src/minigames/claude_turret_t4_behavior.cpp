#include "43846f2ecdea-claude_turret_t4_behavior.hpp"
#include "8c7ada52dac9-gui_rt.hpp"
#include <cstring>
namespace resource_ref_assignment {menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*);}
namespace frame_control {extern unsigned sound_manager_007a39ec;}
extern "C" {
void* __fastcall behavior_attach_0049fb10(void*,void*,void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0049f110(void*,void*,void*) noexcept(false);
void __fastcall behavior_attach_destruct_0049fce0(void*,void*);
void __fastcall behavior_destruct_0049fa70(void*,void*);
unsigned __fastcall behavior_attach_control_0049f3b0(void*,void*,unsigned);
unsigned __fastcall behavior_control_0049f270(void*,void*,unsigned);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*) noexcept(false);
void* __fastcall kotor_fastcall_00673a10(void*,void*) noexcept(false);
unsigned __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*,void*);
unsigned __fastcall w5_sound_play_one_shot_005d5e00(void*,void*,void*,unsigned,unsigned,unsigned,float,float);
void* __fastcall behavior_attach_scalar_delete_004a0010(void*,void*,unsigned);
void* __fastcall kotor_fastcall_006d18c0(void*,void*,unsigned);
void __fastcall behavior_attach_set_scene_0049f0c0(void*,void*);
void __fastcall behavior_attach_message_0049ed00(void*,void*,char*);
void __fastcall behavior_attach_update_gob_position_0049ecd0(void*,void*);
void __fastcall behavior_void_return_004015a0(void*,void*);
void __fastcall behavior_void_return_4_0060e760(void*,void*,char*);
unsigned __fastcall behavior_return_zero_0063e7f0(void*,void*);
void* __fastcall behavior_self_return_00641db0(void*,void*);
unsigned __fastcall kotor_fastcall_006dae60(void*,void*);
unsigned __fastcall kotor_fastcall_006db080(void*,void*);
}
namespace {
using U=unsigned;
struct V {float x,y,z;};struct Q{float x,y,z,w;};
template<class T>T& at(void*p,U n){return *reinterpret_cast<T*>(static_cast<unsigned char*>(p)+n);}
template<class T>T read(void*p,U n){return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+n);}
template<class F>F method(void*p,U n){return reinterpret_cast<F>(read<void**>(p,0)[n/4]);}
U bits(float f){U u;std::memcpy(&u,&f,4);return u;}
using Delete=void*(__fastcall*)(void*,void*,U);
using Animate=U(__fastcall*)(void*,void*,const char*,float,U,U);
using GetQ=Q*(__fastcall*)(void*,void*,Q*);
using GetV=V*(__fastcall*)(void*,void*,V*);
using SetV=V*(__fastcall*)(void*,void*,V*,V);
struct BaseSlots{Delete destroy;void(__fastcall* scene)(void*,void*);U(__fastcall* control)(void*,void*,float);void(__fastcall* message)(void*,void*,char*);U(__fastcall* type)(void*,void*);void*(__fastcall* query)(void*,void*);U(__fastcall* zero1)(void*,void*);U(__fastcall* zero2)(void*,void*);};
struct FireSlots{BaseSlots base;void(__fastcall* update)(void*,void*);U(__fastcall* fire)(void*,void*);};
// All entries present; each table has its original bounded extent.
const FireSlots fire_table={{behavior_attach_scalar_delete_004a0010,behavior_attach_set_scene_0049f0c0,kotor_fastcall_006dae70,behavior_attach_message_0049ed00,kotor_fastcall_006dae60,behavior_self_return_00641db0,behavior_return_zero_0063e7f0,behavior_return_zero_0063e7f0},behavior_attach_update_gob_position_0049ecd0,kotor_fastcall_006daec0};
// +14 is a null-return slot, so use a typed zero provider with pointer result.
extern "C" void* __fastcall behavior_null_return_0063e7f0(void*,void*);
const BaseSlots bullet_table={kotor_fastcall_006d18c0,behavior_void_return_004015a0,kotor_fastcall_006db090,behavior_void_return_4_0060e760,kotor_fastcall_006db080,behavior_null_return_0063e7f0,behavior_return_zero_0063e7f0,behavior_return_zero_0063e7f0};
struct BaseGuard{void*self;void(__fastcall* cleanup)(void*,void*);bool armed;~BaseGuard() noexcept(false){if(armed)cleanup(self,nullptr);}};
const double zero64=0.0;const float zero32=0.0f;
__declspec(noinline) void tick(float* timer,const float* delta){
 __asm {
  mov ecx,timer
  fld dword ptr[ecx]
  fcomp qword ptr[zero64]
  fnstsw ax
  test ah,41h
  jne done_tick
  fld dword ptr[ecx]
  mov edx,delta
  fsub dword ptr[edx]
  fld dword ptr[zero32]
  fcomp st(1)
  fnstsw ax
  test ah,41h
  jne store_tick
  fstp st(0)
  fld dword ptr[zero32]
 store_tick:
  fstp dword ptr[ecx]
 done_tick:
 }
}
__declspec(noinline) bool timer_zero(const float* timer){unsigned short status;
 __asm {
  mov ecx,timer
  fld dword ptr[ecx]
  fcomp qword ptr[zero64]
  fnstsw status
 }
 return (status&0x4400u)==0x4000u;
}
// FST stores life, but the subsequent comparison consumes the unrounded result.
__declspec(noinline) bool advance_life(float* life,const float* delta){unsigned short status;
 __asm {
  mov ecx,life
  mov edx,delta
  fld dword ptr[ecx]
  fsub dword ptr[edx]
  fst dword ptr[ecx]
  fcomp dword ptr[zero32]
  fnstsw status
 }
 const U mask=status&0x4100u;return mask==0||mask==0x4100u;
}
__declspec(noinline) float product(float a,float b){float out;
 __asm {
  fld a
  fmul b
  fstp out
 }
 return out;
}
__declspec(noinline) float sum(float a,float b){float out;
 __asm {
  fld a
  fadd b
  fstp out
 }
 return out;
}
}
extern "C" V* __fastcall quaternion_ydir_0044f420(const void*,void*,V*);
extern "C" void* __fastcall kotor_fastcall_006dadf0(void*self,void*,void*gob,void*object,void*part) noexcept(false){
 behavior_attach_0049fb10(self,nullptr,gob,object,part);
 BaseGuard cleanup{self,behavior_attach_destruct_0049fce0,true};
 at<const FireSlots*>(self,0)=&fire_table;
 c2da_default_ref_00405ed0(&at<menu_resources::Ref>(self,0x3c),nullptr);
 at<U>(self,0x50)=0;cleanup.armed=false;return self;
}
extern "C" U __fastcall kotor_fastcall_006dae70(void*self,void*,float delta) noexcept(false){
 if(!behavior_attach_control_0049f3b0(self,nullptr,bits(delta)))return 0;
 tick(&at<float>(self,0x50),&delta);return 1;
}
extern "C" U __fastcall kotor_fastcall_006daec0(void*self,void*) noexcept(false){
 if(!timer_zero(&at<float>(self,0x50)))return 0;
 at<U>(self,0x50)=read<U>(self,0x4c);
 void*gob=read<void*>(self,0x10);method<Animate>(gob,0x18)(gob,nullptr,"fire",1.0f,1,0);
 gob=read<void*>(self,0x10);method<Animate>(gob,0x18)(gob,nullptr,"ready",1.0f,0x20,0);
 auto* ref=&at<menu_resources::Ref>(self,0x3c);
 if(gui_load_resref_is_valid_00406030(ref,nullptr)){
  menu_resources::Ref copy;copy.construct_00406d80(static_cast<const char*>(behavior_self_return_00641db0(ref,nullptr)));
  w5_sound_play_one_shot_005d5e00(reinterpret_cast<void*>(frame_control::sound_manager_007a39ec),nullptr,&copy,0x17,0,0,0.0f,0.0f);
 }
 return 1;
}
extern "C" void* __fastcall kotor_fastcall_006dafe0(void*self,void*,void*gob,void*bank,const void*bullet) noexcept(false){
 kotor_fastcall_0049f110(self,nullptr,gob);BaseGuard cleanup{self,behavior_destruct_0049fa70,true};
 at<const BaseSlots*>(self,0)=&bullet_table;kotor_fastcall_00673a10(static_cast<unsigned char*>(self)+0x14,nullptr);
 for(U i=0;i<6;++i){U word;std::memcpy(&word,static_cast<const unsigned char*>(bullet)+i*4,4);std::memcpy(static_cast<unsigned char*>(self)+0x14+i*4,&word,4);}
 menu_resources::Ref discarded;resource_ref_assignment::assign_00406120(&at<menu_resources::Ref>(self,0x2c),nullptr,&discarded,reinterpret_cast<const menu_resources::Ref*>(static_cast<const unsigned char*>(bullet)+0x18));
 at<void*>(self,0x3c)=bank;cleanup.armed=false;return self;
}
extern "C" U __fastcall kotor_fastcall_006db090(void*self,void*,float delta) noexcept(false){
 if(!behavior_control_0049f270(self,nullptr,bits(delta)))return 0;
 if(!advance_life(&at<float>(self,0x20),&delta)){void*gob=read<void*>(self,0x10);if(gob)method<Delete>(gob,0)(gob,nullptr,1);return 0;}
 const float distance=product(delta,read<float>(self,0x18));
 Q orientation;V axis;void*gob=read<void*>(self,0x10);
 Q* actual=method<GetQ>(gob,0x68)(gob,nullptr,&orientation);V* direction=quaternion_ydir_0044f420(actual,nullptr,&axis);
 V offset{product(distance,direction->x),product(distance,direction->y),product(distance,direction->z)};
 V position;gob=read<void*>(self,0x10);V* current=method<GetV>(gob,0x64)(gob,nullptr,&position);
 V next{sum(offset.x,current->x),sum(offset.y,current->y),sum(offset.z,current->z)};V discarded;
 gob=read<void*>(self,0x10);method<SetV>(gob,0x5c)(gob,nullptr,&discarded,next);return 1;
}


