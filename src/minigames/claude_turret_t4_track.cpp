#include "e28b94877d18-claude_turret_t4_track.hpp"
#include "8c7ada52dac9-gui_rt.hpp"
#include <cstring>
// Verbatim named ABI types from scene3d.h, camera_stick.hpp, and m15_l0.h.
// Kept here to avoid unrelated whole-header conflicting legacy extern declarations.
namespace kotor_scene3d {struct Vector{float x,y,z;};struct Quaternion{float w,x,y,z;};}
struct W4CamQuaternion{float w,x,y,z;};
struct KotorM15VectorWords{float x,y,z;};
namespace frame_control {extern unsigned sound_manager_007a39ec;}
namespace kotor_reconstruction {struct ArrayMutations;}
extern "C" {
void* __fastcall behavior_attach_0049fb10(void*,void*,void*,void*,void*) noexcept(false);
void __fastcall behavior_attach_destruct_0049fce0(void*,void*) noexcept(false);
unsigned __fastcall behavior_attach_control_0049f3b0(void*,void*,unsigned) noexcept(false);
void __fastcall behavior_add_dependency_0049f130(void*,void*,void*) noexcept(false);
void* __fastcall safe_pointer_assign_00447fa0(void*,void*,void*) noexcept(false);
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void*,void*) noexcept(false);
void __fastcall exo_array_list_remove_00445080(kotor_reconstruction::ArrayMutations*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_00673a30(void*,void*) noexcept(false);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*) noexcept(false);
unsigned __fastcall kotor_fastcall_006db580(void*,void*);
unsigned __fastcall kotor_fastcall_006db600(void*,void*);
void __fastcall behavior_attach_set_scene_0049f0c0(void*,void*);
void __fastcall behavior_attach_message_0049ed00(void*,void*,char*);
void __fastcall behavior_attach_update_gob_position_0049ecd0(void*,void*);
unsigned __fastcall behavior_return_zero_0063e7f0(void*,void*);
void* __fastcall behavior_self_return_00641db0(void*,void*);
kotor_scene3d::Vector* __cdecl quaternion_rotate_004a9bb0(kotor_scene3d::Vector*,const kotor_scene3d::Quaternion*,const kotor_scene3d::Vector*);
kotor_scene3d::Quaternion* __cdecl quaternion_negate_w_004a9b50(kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*);
kotor_scene3d::Quaternion* __cdecl quaternion_multiply_004a9ac0(kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*);
kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion*,float,float,float);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
float __cdecl kotor_m15a_pitch_004aa130(const void*);
double __cdecl kotor_cdecl_004acb90(W4CamQuaternion);
unsigned __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*,void*);
unsigned __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void*,void*,void*,KotorM15VectorWords,float,unsigned,unsigned,unsigned,float,float) noexcept(false);
}
namespace {
using U=unsigned;using V=kotor_scene3d::Vector;using Q=kotor_scene3d::Quaternion;
static_assert(sizeof(V)==12&&sizeof(Q)==16&&sizeof(KotorM15VectorWords)==12&&sizeof(W4CamQuaternion)==16,"canonical ABI sizes");
template<class T>T& at(void*p,U n){return *reinterpret_cast<T*>(static_cast<unsigned char*>(p)+n);}
template<class T>T read(void*p,U n){return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+n);}
template<class F>F method(void*p,U n){return reinterpret_cast<F>(read<void**>(p,0)[n/4]);}
U bits(float f){U u;std::memcpy(&u,&f,4);return u;}
using GetQ=Q*(__fastcall*)(void*,void*,Q*);
using GetV=V*(__fastcall*)(void*,void*,V*);
using SetQ=Q*(__fastcall*)(void*,void*,Q*,Q);
using SetBehavior=U(__fastcall*)(void*,void*,void*);
using Animate=U(__fastcall*)(void*,void*,const char*,float,U,U);
struct Slots{
 void*(__fastcall* destroy)(void*,void*,U);void(__fastcall* scene)(void*,void*);
 U(__fastcall* control)(void*,void*,float);void(__fastcall* message)(void*,void*,char*);
 U(__fastcall* type)(void*,void*);void*(__fastcall* query)(void*,void*);
 U(__fastcall* zero1)(void*,void*);U(__fastcall* zero2)(void*,void*);void(__fastcall* update)(void*,void*);
};
const Slots track_table={kotor_fastcall_006db610,behavior_attach_set_scene_0049f0c0,kotor_fastcall_006db180,behavior_attach_message_0049ed00,kotor_fastcall_006db580,behavior_self_return_00641db0,behavior_return_zero_0063e7f0,behavior_return_zero_0063e7f0,behavior_attach_update_gob_position_0049ecd0};
const Slots track_fire_table={kotor_fastcall_006db610,behavior_attach_set_scene_0049f0c0,kotor_fastcall_006db3d0,behavior_attach_message_0049ed00,kotor_fastcall_006db600,behavior_self_return_00641db0,behavior_return_zero_0063e7f0,behavior_return_zero_0063e7f0,behavior_attach_update_gob_position_0049ecd0};
struct Guard{void*self;void(__fastcall* cleanup)(void*,void*);bool armed;~Guard()noexcept(false){if(armed)cleanup(self,nullptr);}};
const float zero32=0.0f;const double zero64=0.0;
// z/y/x load and operation order, then original x/y/z FSTP stores.
__declspec(noinline) void add_vector(V*out,const V*a,const V*b){
 __asm {
  mov eax,a
  mov edx,b
  mov ecx,out
  fld dword ptr[eax+8]
  fadd dword ptr[edx+8]
  fld dword ptr[eax+4]
  fadd dword ptr[edx+4]
  fld dword ptr[eax]
  fadd dword ptr[edx]
  fstp dword ptr[ecx]
  fstp dword ptr[ecx+4]
  fstp dword ptr[ecx+8]
 }
}
__declspec(noinline) void subtract_vector(V*out,const V*a,const V*b){
 __asm {
  mov eax,a
  mov edx,b
  mov ecx,out
  fld dword ptr[eax+8]
  fsub dword ptr[edx+8]
  fld dword ptr[eax+4]
  fsub dword ptr[edx+4]
  fld dword ptr[eax]
  fsub dword ptr[edx]
  fstp dword ptr[ecx]
  fstp dword ptr[ecx+4]
  fstp dword ptr[ecx+8]
 }
}
__declspec(noinline) bool in_range(const V*v,const float* radius){unsigned short status;
 __asm {
  mov eax,radius
  mov ecx,v
  fld dword ptr[eax]
  fld st(0)
  fmul st(0),st(1)
  fld dword ptr[ecx+8]
  fmul dword ptr[ecx+8]
  fld dword ptr[ecx+4]
  fmul dword ptr[ecx+4]
  faddp st(1),st(0)
  fld dword ptr[ecx]
  fmul dword ptr[ecx]
  faddp st(1),st(0)
  fcompp
  fnstsw status
  fstp st(0)
 }
 return (status&0x4100u)!=0;
}
__declspec(noinline) void clamp_angle(float*angle,const float*limit,unsigned char*can_fire){
 __asm {
  mov ecx,angle
  mov edx,limit
  fld dword ptr[edx]
  fld dword ptr[ecx]
  fabs
  fcompp
  fnstsw ax
  test ah,41h
  jne done_clamp
  mov eax,can_fire
  mov byte ptr[eax],0
  fld dword ptr[ecx]
  fcomp dword ptr[zero32]
  fnstsw ax
  test ah,5
  jp positive_clamp
  fld dword ptr[edx]
  fchs
  fstp dword ptr[ecx]
  jmp done_clamp
 positive_clamp:
  mov eax,dword ptr[edx]
  mov dword ptr[ecx],eax
 done_clamp:
 }
}
__declspec(noinline) void tick(float*timer,const float*delta){
 __asm {
  mov ecx,timer
  fld dword ptr[ecx]
  fcomp qword ptr[zero64]
  fnstsw ax
  test ah,41h
  jne done_tick
  mov edx,delta
  fld dword ptr[ecx]
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
__declspec(noinline) bool timer_zero(const float*p){unsigned short status;
 __asm {
  mov ecx,p
  fld dword ptr[ecx]
  fcomp qword ptr[zero64]
  fnstsw status
 }
 return (status&0x4400u)==0x4000u;
}
__declspec(noinline) float negate(float v){float out;
 __asm {
  fld v
  fchs
  fstp out
 }
 return out;
}
}
extern "C" void* __fastcall kotor_fastcall_00673080(void*self,void*){
 at<volatile U>(self,0)=0;at<volatile U>(self,4)=0;at<volatile U>(self,8)=0;at<volatile U>(self,0xc)=0;
 at<volatile U>(self,0x1c)=0;at<volatile U>(self,0x18)=0;at<volatile U>(self,0x14)=0;at<volatile U>(self,0x10)=0x3f800000;return self;
}
extern "C" void* __fastcall kotor_fastcall_006db4f0(void*self,void*,void*gob,void*object,void*part,void*target) noexcept(false){
 behavior_attach_0049fb10(self,nullptr,gob,object,part);
 Guard base{self,behavior_attach_destruct_0049fce0,true};at<const Slots*>(self,0)=&track_table;
 kotor_fastcall_00673a30(static_cast<unsigned char*>(self)+0x3c,nullptr);
 at<void*>(self,0x54)=nullptr;at<U>(self,0x58)=0;at<U>(self,0x5c)=0;at<U>(self,0x60)=0;
 Guard safe{static_cast<unsigned char*>(self)+0x54,aurora_texture_safe_pointer_destroy_00422380,true};
 at<unsigned char>(self,0x4c)=0;safe_pointer_assign_00447fa0(static_cast<unsigned char*>(self)+0x54,nullptr,target);
 behavior_add_dependency_0049f130(self,nullptr,target);safe.armed=false;base.armed=false;return self;
}
extern "C" void* __fastcall kotor_fastcall_006db590(void*self,void*,void*gob,void*object,void*part,void*target) noexcept(false){
 kotor_fastcall_006db4f0(self,nullptr,gob,object,part,target);Guard base{self,kotor_fastcall_006db630,true};
 at<const Slots*>(self,0)=&track_fire_table;c2da_default_ref_00405ed0(&at<menu_resources::Ref>(self,0x64),nullptr);
 at<U>(self,0x78)=0;base.armed=false;return self;
}
extern "C" void __fastcall kotor_fastcall_006db630(void*self,void*) noexcept(false){
 void* target=read<void*>(self,0x54);if(target)exo_array_list_remove_00445080(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(static_cast<unsigned char*>(target)+4),nullptr,static_cast<unsigned char*>(self)+0x54);
 behavior_attach_destruct_0049fce0(self,nullptr);
}
extern "C" void* __fastcall kotor_fastcall_006db610(void*self,void*,U flags) noexcept(false){
 kotor_fastcall_006db630(self,nullptr);if(flags&1)live_free(self);return self;
}
extern "C" U __fastcall kotor_fastcall_006db180(void*self,void*,float delta) noexcept(false){
 at<unsigned char>(self,0x4c)=0;if(!behavior_attach_control_0049f3b0(self,nullptr,bits(delta)))return 0;
 void*gob=read<void*>(self,0x10);
 if(!read<void*>(self,0x54)){method<SetBehavior>(gob,0x58)(gob,nullptr,nullptr);return 0;}
 Q orientation;method<GetQ>(gob,0x68)(gob,nullptr,&orientation); // EAX intentionally ignored.
 V position;gob=read<void*>(self,0x10);V* position_ptr=method<GetV>(gob,0x64)(gob,nullptr,&position);
 V rotated;V* rotated_ptr=quaternion_rotate_004a9bb0(&rotated,&orientation,position_ptr);
 V offset_position;add_vector(&offset_position,rotated_ptr,&at<V>(self,0x58));
 Q inverse;Q* inverse_ptr=quaternion_negate_w_004a9b50(&inverse,&orientation);
 V world_point;quaternion_rotate_004a9bb0(&world_point,inverse_ptr,&offset_position); // Later reads designated output.
 void* target=read<void*>(self,0x54);V target_position;V* target_ptr=method<GetV>(target,0x64)(target,nullptr,&target_position);
 V difference;subtract_vector(&difference,target_ptr,&world_point);
 inverse_ptr=quaternion_negate_w_004a9b50(&inverse,&orientation);
 V local;quaternion_rotate_004a9bb0(&local,inverse_ptr,&difference); // Later reads designated output.
 if(!in_range(&local,&at<float>(self,0x40)))return 1;
 float pitch=kotor_m15a_pitch_004aa130(&local);float yaw=kotor_m15a_yaw_004aa0f0(&local);
 at<unsigned char>(self,0x4c)=1;
 clamp_angle(&yaw,&at<float>(self,0x3c),&at<unsigned char>(self,0x4c));
 clamp_angle(&pitch,&at<float>(self,0x44),&at<unsigned char>(self,0x4c));
 W4CamQuaternion roll_input;std::memcpy(&roll_input,&orientation,16);volatile float roll=static_cast<float>(kotor_cdecl_004acb90(roll_input));
 Q ypr;Q* ypr_ptr=yaw_pitch_roll_004acac0(&ypr,yaw,negate(pitch),roll);
 Q composed;Q* composed_ptr=quaternion_multiply_004a9ac0(&composed,ypr_ptr,&orientation);
 gob=read<void*>(self,0x10);Q discarded;method<SetQ>(gob,0x60)(gob,nullptr,&discarded,*composed_ptr);return 1;
}
extern "C" U __fastcall kotor_fastcall_006db3d0(void*self,void*,float delta) noexcept(false){
 if(!kotor_fastcall_006db180(self,nullptr,delta))return 0;
 tick(&at<float>(self,0x78),&delta);
 if(read<unsigned char>(self,0x4c)&&timer_zero(&at<float>(self,0x78))){
  at<U>(self,0x78)=read<U>(self,0x74);void*gob=read<void*>(self,0x10);
  method<Animate>(gob,0x18)(gob,nullptr,"fire",1.0f,1,0);gob=read<void*>(self,0x10);
  method<Animate>(gob,0x18)(gob,nullptr,"ready",1.0f,0x20,0);
  auto* ref=&at<menu_resources::Ref>(self,0x64);
  if(gui_load_resref_is_valid_00406030(ref,nullptr)){
   menu_resources::Ref copy;copy.construct_00406d80(static_cast<const char*>(behavior_self_return_00641db0(ref,nullptr)));
   V position;gob=read<void*>(self,0x10);V* actual=method<GetV>(gob,0x64)(gob,nullptr,&position);
   KotorM15VectorWords sound_position;std::memcpy(&sound_position,actual,12);
   kotor_m15_sound_play_3d_one_shot_005d5e10(reinterpret_cast<void*>(frame_control::sound_manager_007a39ec),nullptr,&copy,sound_position,0.0f,0x18,0,0,0.0f,0.0f);
  }
 }
 return 1;
}


