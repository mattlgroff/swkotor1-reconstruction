#include "63c4971a535b-claude_turret_t4_callbacks.hpp"
#include "8c7ada52dac9-gui_rt.hpp"
#include <cstdio>
#include <cstring>
// Canonical named ABI types, copied verbatim from their owning headers.
namespace kotor_scene3d {struct Vector{float x,y,z;};struct Quaternion{float w,x,y,z;};}
namespace kotor_world_los {struct Vector{float x,y,z;};}
namespace aurora_model {struct Gob;}
namespace gui_load {struct ResRef;}
extern "C" {
int __cdecl kotor_sscanf_006fae08(const char*,const char*,...);
int __cdecl format_006fadb0(char*,const char*,...);
std::int32_t __cdecl crt_rand_006fc447();
void __fastcall exo_array_list_free_0047abc0(void*,void*) noexcept(false);
void __fastcall gui_load_resref_copy_to_string_00405fb0(const gui_load::ResRef*,void*,char*);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*,const char*,std::FILE*,std::FILE*) noexcept(false);
kotor_scene3d::Vector* __cdecl quaternion_rotate_004a9bb0(kotor_scene3d::Vector*,const kotor_scene3d::Quaternion*,const kotor_scene3d::Vector*);
kotor_scene3d::Quaternion* __cdecl quaternion_negate_w_004a9b50(kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*);
kotor_scene3d::Quaternion* __cdecl quaternion_multiply_004a9ac0(kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*);
kotor_scene3d::Quaternion* __fastcall quaternion_construct_004ac960(kotor_scene3d::Quaternion*,void*,kotor_scene3d::Vector,float);
kotor_scene3d::Vector* __cdecl vector_sum_00423b90(kotor_scene3d::Vector*,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*);
void __fastcall vector_normalize_004ab130(kotor_scene3d::Vector*,void*);
kotor_scene3d::Vector* __fastcall vector_scale_assign_004a97a0(kotor_scene3d::Vector*,void*,float);
kotor_scene3d::Vector* __fastcall vector_subtract_assign_004a9770(kotor_scene3d::Vector*,void*,const kotor_scene3d::Vector*);
kotor_world_los::Vector* __cdecl normalize_004ab180(kotor_world_los::Vector*,const kotor_world_los::Vector*);
kotor_world_los::Vector* __cdecl vector_component_mult_00423bf0(kotor_world_los::Vector*,const kotor_world_los::Vector*,float);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
void* __cdecl pointto_004acca0(void*,const void*);
void* __fastcall kotor_fastcall_006dafe0(void*,void*,void*,void*,const void*) noexcept(false);
void __fastcall kotor_fastcall_006738d0(void*,void*) noexcept(false);
unsigned __fastcall kotor_fastcall_00672fa0(void*,void*,void*) noexcept(false);
std::int32_t __fastcall kotor_fastcall_006730a0(void*,void*,void*,void*,void*) noexcept(false);
}
namespace {
using U=unsigned;using I=std::int32_t;using V=kotor_scene3d::Vector;using Q=kotor_scene3d::Quaternion;using WV=kotor_world_los::Vector;
static_assert(sizeof(V)==12&&sizeof(WV)==12&&sizeof(Q)==16,"original ABI layouts");
template<class T>T& at(void*p,U n){return *reinterpret_cast<T*>(static_cast<unsigned char*>(p)+n);}
template<class T>T read(void*p,U n){return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+n);}
template<class F>F slot(void*p,U n){return reinterpret_cast<F>(read<void**>(p,0)[n/4]);}
using Name=const char*(__fastcall*)(void*,void*);
using Enum=U(__fastcall*)(void*,void*,U(__cdecl*)(void*),void*);
using AddGun=void*(__fastcall*)(void*,void*,void*,void*);
using GetBehavior=void*(__fastcall*)(void*,void*,U);
using Query=U(__fastcall*)(void*,void*);
using GetQ=Q*(__fastcall*)(void*,void*,Q*);
using GetV=V*(__fastcall*)(void*,void*,V*);
using GetHook=U(__fastcall*)(void*,void*,const char*,V*,Q*);
using Animate=U(__fastcall*)(void*,void*,const char*,float,U,U);
using Delete=void*(__fastcall*)(void*,void*,U);
using GetScene=void*(__fastcall*)(void*,void*);
using AttachScene=I(__fastcall*)(void*,void*,void*);
using SetV=V*(__fastcall*)(void*,void*,V*,V);
using SetQ=Q*(__fastcall*)(void*,void*,Q*,Q);
using SetBehavior=U(__fastcall*)(void*,void*,void*);
struct List{void**data;I count;I capacity;};
struct ListGuard{List*list;bool armed;~ListGuard()noexcept(false){if(armed)exo_array_list_free_0047abc0(list,nullptr);}};
struct Allocation{void*p;~Allocation()noexcept(false){if(p)live_free(p);}};
// Original malformed fire%d reads whatever DWORD was in its stack slot; no default.
__declspec(noinline) I stack_word(const I*p){I result;
 __asm {
  mov ecx,p
  mov eax,dword ptr[ecx]
  mov result,eax
 }
 return result;
}
const float zero32=0.0f;
__declspec(noinline) bool positive(const float*p){unsigned short status;
 __asm {
  mov ecx,p
  fld dword ptr[ecx]
  fcomp dword ptr[zero32]
  fnstsw status
 }
 return !(status&0x4100u);
}
__declspec(noinline) float integer_float(I value){float out;
 __asm {
  fild value
  fstp out
 }
 return out;
}
__declspec(noinline) float product(const float*a,const float*b){float out;
 __asm {
  mov ecx,a
  mov edx,b
  fld dword ptr[ecx]
  fmul dword ptr[edx]
  fstp out
 }
 return out;
}
__declspec(noinline) void subtract(V*out,const V*a,const V*b){
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
__declspec(noinline) void add(V*out,const V*a,const V*b){
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
__declspec(noinline) float dot(const V*a,const V*b){float out;
 __asm {
  mov ecx,a
  mov edx,b
  fld dword ptr[ecx+8]
  fmul dword ptr[edx+8]
  fld dword ptr[ecx+4]
  fmul dword ptr[edx+4]
  faddp st(1),st(0)
  fld dword ptr[ecx]
  fmul dword ptr[edx]
  faddp st(1),st(0)
  fstp out
 }
 return out;
}
}
extern "C" U __cdecl kotor_cdecl_0066c720(void*part) noexcept(false){
 const char*name=slot<Name>(part,4)(part,nullptr);
 // Existing selected UCRT wrapper; bounded installed English data ruling only.
 return _strnicmp(name,"gunbank",7)==0?1u:0u;
}
extern "C" I __fastcall kotor_fastcall_0066e490(void*self,void*,void*model) noexcept(false){
 List list{nullptr,0,0};ListGuard cleanup{&list,true};
 // Original frame: one backing S+20..S+19F, message slots +0x00/+0x80/+0x100 (0066E526/0066E53A/0066E565).
 char messages[0x180];
 slot<Enum>(model,0x110)(model,nullptr,kotor_cdecl_0066c720,&list);I attached=0;
 for(I i=0;i<*static_cast<volatile I*>(&list.count);++i){
  void*part=(*static_cast<void** volatile*>(&list.data))[i];I id;
  const char*name=slot<Name>(part,4)(part,nullptr);
  if(kotor_sscanf_006fae08(name,"gunbank%d-",&id)!=1){
   part=(*static_cast<void** volatile*>(&list.data))[i];name=slot<Name>(part,4)(part,nullptr);
   format_006fadb0(messages,"CSWTrackFollower: Could not find bank ID in attach part name %s.",name);
  }else if(id<0){format_006fadb0(messages+0x80,"CSWTrackFollower: Bank ID %d < 0.",id);
  }else if(id<read<I>(self,0x78)){
   void*bank=read<void**>(self,0x74)[id];part=(*static_cast<void** volatile*>(&list.data))[i];
   slot<AddGun>(bank,0)(bank,nullptr,model,part);++attached;
  }else{format_006fadb0(messages+0x100,"CSWTrackFollower: Trying to attach gun to a non-existent bank ID: %d",id);}
 }
 cleanup.armed=false;if(list.data)live_free(list.data);return attached;
}
extern "C" I __fastcall kotor_fastcall_0066d4e0(void*self,void*,void*bullet) noexcept(false){
 void*behavior=slot<GetBehavior>(bullet,0x108)(bullet,nullptr,0xaaab);
 if(behavior&&read<void*>(behavior,0x3c)==self)return 0;
 const I count=read<I>(self,0x6c);
 for(I i=0;i<count;++i){void*target=read<void**>(self,0x68)[static_cast<U>(i)*2u];
  if(kotor_fastcall_006730a0(read<void*>(self,0x60),nullptr,bullet,target,self))return 1;
 }
 return 0;
}
extern "C" void __cdecl kotor_cdecl_00673a40(void*gun,void*event_storage,void*bank) noexcept(false){
 const char*event=static_cast<const char*>(event_storage);
 if(_strnicmp(event,"fire",4)!=0){
  if(_strnicmp(event,"start",5)==0&&_strnicmp(event+5,"fire",4)==0)kotor_fastcall_006738d0(bank,nullptr);
  return;
 }
 V hook_position{0.0f,0.0f,0.0f};Q hook_orientation{1.0f,0.0f,0.0f,0.0f};
 I hook_id; // Intentionally not initialized: original unchecked sscanf quirk.
 // Original frame: ONE backing S+BC..S+1CF (0x114 bytes). Hook name at +0x00 (00673AB3/00673ADB),
 // log text at +0x14 (00673B5F/00673EDE). bullethook-2147483648 (22 bytes) overlaps the log slot by 2 bytes.
 char scratch[0x114];char*const hook_name=scratch;char*const message=scratch+0x14;
 kotor_sscanf_006fae08(event,"fire%d",&hook_id);
 const I hook_number=stack_word(&hook_id);format_006fadb0(hook_name,"bullethook%d",hook_number);
 if(!slot<GetHook>(gun,0x98)(gun,nullptr,hook_name,&hook_position,&hook_orientation)){
  format_006fadb0(message,"FireGunCallback: no part called bullethook%d",stack_word(&hook_id));return;
 }
 menu_resources::Ref bullet_ref;std::memcpy(&bullet_ref,static_cast<unsigned char*>(bank)+4,16);char model[0x14]; // Original S+A8..S+BB.
 gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const gui_load::ResRef*>(&bullet_ref),nullptr,model);
 void*bullet=aurora_new_aur_object_00449cc0(model,"",nullptr,nullptr);
 if(!bullet){format_006fadb0(message,"FireGunCallback: could not create bullet %s",model);return;}
 if(slot<Query>(bank,8)(bank,nullptr)){
  void*behavior=slot<GetBehavior>(gun,0x108)(gun,nullptr,0xaaaa);
  if(!behavior){slot<Delete>(bullet,0)(bullet,nullptr,1);return;}
  float accuracy;U accuracy_bits=read<U>(behavior,0x48);std::memcpy(&accuracy,&accuracy_bits,4);
  void*target=read<void*>(behavior,0x54);Q target_orientation;
  slot<GetQ>(target,0x68)(target,nullptr,&target_orientation); // Original ignores EAX.
  target=read<void*>(behavior,0x54);V target_position;
  V*position=slot<GetV>(target,0x64)(target,nullptr,&target_position);
  V rotated;V*rotated_ptr=quaternion_rotate_004a9bb0(&rotated,&target_orientation,position);
  V sum;V*sum_ptr=vector_sum_00423b90(&sum,rotated_ptr,&at<V>(behavior,0x58));
  Q inverse;Q*inverse_ptr=quaternion_negate_w_004a9b50(&inverse,&target_orientation);
  V aim;quaternion_rotate_004a9bb0(&aim,inverse_ptr,sum_ptr); // Later uses designated aim.
  if(positive(&accuracy)){
   V original_aim;std::memcpy(&original_aim,&aim,12);
   const float first=integer_float(crt_rand_006fc447()-0x3fff);
   const float second=integer_float(crt_rand_006fc447()-0x3fff);
   const float third=integer_float(crt_rand_006fc447()-0x3fff);
   V random{third,second,first};vector_normalize_004ab130(&random,nullptr);
   const float scale=product(&at<float>(behavior,0x50),&accuracy);vector_scale_assign_004a97a0(&random,nullptr,scale);
   V difference;subtract(&difference,&original_aim,&hook_position);WV normalized;
   WV*normal_ptr=normalize_004ab180(&normalized,reinterpret_cast<const WV*>(&difference));
   V normal;std::memcpy(&normal,normal_ptr,12);
   V displaced;add(&displaced,&original_aim,&random);
   const float projection=dot(&normal,&random);WV scaled;
   WV*scaled_ptr=vector_component_mult_00423bf0(&scaled,reinterpret_cast<const WV*>(&normal),projection);
   V corrected;float*result=kotor_cdecl_00423bc0(&corrected.x,&displaced.x,&scaled_ptr->x);std::memcpy(&aim,result,12);
  }
  vector_subtract_assign_004a9770(&aim,nullptr,&hook_position);
  U angle_bits=0x3fc90fdb;float angle;std::memcpy(&angle,&angle_bits,4);
  Q axis_quaternion;Q*axis_ptr=quaternion_construct_004ac960(&axis_quaternion,nullptr,V{1.0f,0.0f,0.0f},angle);
  Q pointing;void*pointing_ptr=pointto_004acca0(&pointing,&aim);
  Q composed;Q*composed_ptr=quaternion_multiply_004a9ac0(&composed,static_cast<const Q*>(pointing_ptr),axis_ptr);
  std::memcpy(&hook_orientation,composed_ptr,16);
 }
 slot<Animate>(bullet,0x18)(bullet,nullptr,"ready",1.0f,0,0);
 void**captured_bullet_table=read<void**>(bullet,0); // Captured BEFORE gun GetScene.
 void*scene=slot<GetScene>(gun,0x104)(gun,nullptr);
 reinterpret_cast<AttachScene>(captured_bullet_table[0x4c/4])(bullet,nullptr,scene);
 V ignored_position;slot<SetV>(bullet,0x5c)(bullet,nullptr,&ignored_position,hook_position);
 Q ignored_orientation;slot<SetQ>(bullet,0x60)(bullet,nullptr,&ignored_orientation,hook_orientation);
 void*memory=live_operator_new(0x40);Allocation cleanup{memory};
 void*behavior=memory?kotor_fastcall_006dafe0(memory,nullptr,bullet,read<void*>(bank,0x68),static_cast<unsigned char*>(bank)+0x40):nullptr;
 cleanup.p=nullptr;slot<SetBehavior>(bullet,0x58)(bullet,nullptr,behavior);
 void*follower=read<void*>(bank,0x68);kotor_fastcall_00672fa0(read<void*>(follower,0x60),nullptr,bullet);
}
