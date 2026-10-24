#include "dd882c43ac6f-claude_minigame_t3_load.hpp"
#include "d1ffedc48c11-gff_readers.h"
#include "bee7e40f00df-native_string_deps.h"
#include <cstdint>
#include <cstring>
#include <new>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_resource_required { unsigned __fastcall res_release_00409b80(void*,void*) noexcept(false); }
extern "C" {
unsigned __fastcall c2da_current_resource_demand_00409b10(void*,void*) noexcept(false);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*) noexcept(false);
menu_resources::Ref* __fastcall ref_assign_copy_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*) noexcept(false);
unsigned __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*,void*) noexcept(false);
void __fastcall kotor_fastcall_005089e0(void*,void*,menu_resources::Ref) noexcept(false);
void __fastcall recovered_byte_list_allocate_owned_0056b7e0(void*,void*,int) noexcept(false);
void __fastcall recovered_byte_list_add_owned_005704f0(void*,void*,int) noexcept(false);
void __fastcall exo_array_list_destruct_0056b6f0(void*,void*) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
void __fastcall kotor_fastcall_00670b50(void*,void*,float,float) noexcept(false);
void* __fastcall kotor_fastcall_00671820(void*,void*,void*,void*,const menu_resources::Ref*) noexcept(false);
void* __fastcall kotor_fastcall_006719d0(void*,void*,void*,void*,const menu_resources::Ref*,const menu_resources::Ref*,unsigned) noexcept(false);
void* __fastcall kotor_fastcall_00671e40(void*,void*,void*,void*,const menu_resources::Ref*) noexcept(false);
int __fastcall kotor_fastcall_00671bc0(void*,void*,int) noexcept(false);
int __fastcall kotor_fastcall_00671be0(void*,void*,int) noexcept(false);
void* __fastcall kotor_fastcall_005edaf0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066bfb0(void*,void*,kotor_reconstruction::StartupString*) noexcept(false);
int __fastcall kotor_fastcall_0066d0b0(void*,void*,gui_load::Gff*,gui_load::Struct*) noexcept(false);
}
namespace {
using U=std::uint32_t;using I=std::int32_t;using Ref=menu_resources::Ref;using String=kotor_reconstruction::StartupString;
struct Array {void* data;U count,capacity;};
static_assert(sizeof(Array)==12 && sizeof(String)==8,"original local layouts");
template<class T>T rd(const void*p,U off){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v;}
template<class T>void wr(void*p,U off,T v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,sizeof v);}
I si(U u){I i;std::memcpy(&i,&u,4);return i;}
float fl(U u){float f;std::memcpy(&f,&u,4);return f;}
U bits(float f){U u;std::memcpy(&u,&f,4);return u;}
gui_load::Gff* gff(void*self){return rd<gui_load::Gff*>(self,8);}
struct ArrayGuard {Array* p;bool armed=true;~ArrayGuard()noexcept(false){if(armed)exo_array_list_destruct_0056b6f0(p,nullptr);}};
struct StringGuard {String* p;bool armed=true;~StringGuard()noexcept(false){if(armed)p->~StartupString();}};
void reset(Array& a){if(a.data){live_free(a.data);a.data=nullptr;a.capacity=0;}a.count=0;}
// Each call constructs the original empty fallback, reads to distinct result storage,
// and assigns into the existing Ref. The model handle is distinct adjacent storage.
void read_ref(void*self,gui_load::Struct* node,const char*label,U*success,Ref*dest){
 Ref fallback,result,scratch;fallback.construct_00406d80("");
 Ref* returned=gui_load_gff_resref_00411e10(gff(self),nullptr,&result,node,label,success,&fallback);
 ref_assign_copy_00406120(dest,nullptr,&scratch,returned);
}
void actor_load(void*object,gui_load::Gff*resource,gui_load::Struct*node){
 using F=int(__fastcall*)(void*,void*,gui_load::Gff*,gui_load::Struct*);
 F f=rd<F>(rd<void*>(object,0),0x2c);(void)f(object,nullptr,resource,node);
}
}
extern "C" int __fastcall kotor_fastcall_006723d0(void*self,void*) noexcept(false){
 Ref reference;c2da_default_ref_00405ed0(&reference,nullptr);
 Array models={nullptr,0,0};ArrayGuard modelsGuard{&models};
 Array rotating={nullptr,0,0};ArrayGuard rotatingGuard{&rotating};
 alignas(String) unsigned char nameStorage[sizeof(String)];String*name=new(nameStorage)String;StringGuard nameGuard{name};
 // Explicit final cleanup drops each state BEFORE calling its destructor/free.
 auto finish=[&](int result){nameGuard.armed=false;name->~StartupString();rotatingGuard.armed=false;live_free(rotating.data);modelsGuard.armed=false;live_free(models.data);return result;};
 gui_load::Struct node,mini,modelNode;task17_gff::List list,sublist;U success;
 if(!c2da_current_resource_demand_00409b10(gff(self),nullptr))return finish(0);
 gff(self)->GetTopLevelStruct(&node); // both original return values ignored
 gui_load_gff_struct_00411a10(gff(self),nullptr,&mini,&node,"MiniGame");
 U type=gui_load_gff_dword_00411c20(gff(self),nullptr,&mini,"Type",&success,0);
 if(type==1||type==2)wr<U>(self,0x80,type);
 type=rd<U>(self,0x80);float movement=type==1?fl(0x40c00000):type==2?fl(0x42b40000):0.0f;
 wr<float>(self,0x74,gui_load_gff_float_00411d00(gff(self),nullptr,&mini,"MovementPerSec",&success,movement));
 wr<float>(self,0xbc,gui_load_gff_float_00411d00(gff(self),nullptr,&mini,"LateralAccel",&success,fl(0x42700000)));
 U plane=gui_load_gff_dword_00411c20(gff(self),nullptr,&mini,"Bump_Plane",&success,0);if(si(plane)>=0&&si(plane)<=3)wr<U>(self,0x78,plane);
 wr<unsigned char>(self,0x95,gui_load_gff_byte_00411a60(gff(self),nullptr,&mini,"DoBumping",&success,0));
 unsigned char inertia=gui_load_gff_byte_00411a60(gff(self),nullptr,&mini,"UseInertia",&success,0);
 wr<unsigned char>(self,0x94,inertia);
 U dof=gui_load_gff_dword_00411c20(gff(self),nullptr,&mini,"DOF",&success,0);if(!(dof&0xfffffff8u))wr<U>(self,0x7c,dof);
 read_ref(self,&mini,"Music",&success,reinterpret_cast<Ref*>(static_cast<unsigned char*>(self)+0x96));
 float farClip=gui_load_gff_float_00411d00(gff(self),nullptr,&mini,"Far_Clip",&success,fl(0x42c80000));
 float nearClip=gui_load_gff_float_00411d00(gff(self),nullptr,&mini,"Near_Clip",&success,fl(0x3dcccccd));
 kotor_fastcall_00670b50(self,nullptr,nearClip,farClip);
 float angle=gui_load_gff_float_00411d00(gff(self),nullptr,&mini,"CameraViewAngle",&success,fl(0x42820000));
 void*camera=rd<void*>(self,0x20);wr<float>(self,0x70,angle);
 if(camera){using F=U(__fastcall*)(void*,void*,U);F f=rd<F>(rd<void*>(camera,0),0x44);(void)f(camera,nullptr,bits(angle));}
 success=gui_load_gff_struct_00411a10(gff(self),nullptr,&node,&mini,"Player");if(!success)return finish(0);
 success=gff_list_004118c0(gff(self),nullptr,&list,&node,"Models");if(!success)return finish(0);
 const I playerModels=si(gff_list_count_00411940(gff(self),nullptr,&list));
 U byteCount=0;
 for(I i=0;i<playerModels;++i){
  
  gff_list_element_00411990(gff(self),nullptr,&modelNode,&list,U(i));
  read_ref(self,&modelNode,"Model",&success,&reference);
  if(gui_load_resref_is_valid_00406030(&reference,nullptr)){
   kotor_fastcall_005089e0(&models,nullptr,reference);
   unsigned char flag=gui_load_gff_byte_00411a60(gff(self),nullptr,&modelNode,"RotatingModel",&success,1);
   if(byteCount==rotating.capacity){U cap=rotating.capacity;recovered_byte_list_allocate_owned_0056b7e0(&rotating,nullptr,si(cap?cap+cap:16));byteCount=rotating.count;}
   wr<unsigned char>(rotating.data,byteCount,flag);++byteCount;rotating.count=byteCount;
  }
 }
 if(!models.count)return finish(0);
 read_ref(self,&node,"Track",&success,&reference);
 if(!gui_load_resref_is_valid_00406030(&reference,nullptr))return finish(0);
 void*player;
 if(rd<U>(self,0x80)==2){
  Ref cameraRef;c2da_default_ref_00405ed0(&cameraRef,nullptr);read_ref(self,&node,"Camera",&success,&cameraRef);
  if(!gui_load_resref_is_valid_00406030(&cameraRef,nullptr))return finish(0);
  unsigned char attach=gui_load_gff_byte_00411a60(gff(self),nullptr,&node,"CameraRotate",&success,0);
  player=kotor_fastcall_006719d0(self,nullptr,&models,&rotating,&reference,&cameraRef,attach);
 }else player=kotor_fastcall_00671820(self,nullptr,&models,&rotating,&reference);
 reset(models);reset(rotating);if(!player)return finish(0);
 actor_load(player,gff(self),&node);
 success=gui_load_gff_struct_00411a10(gff(self),nullptr,&node,&mini,"Mouse");
 if(success){
  U x=gui_load_gff_dword_00411c20(gff(self),nullptr,&node,"AxisX",&success,0);kotor_fastcall_00671bc0(self,nullptr,si(x));
  U y=gui_load_gff_dword_00411c20(gff(self),nullptr,&node,"AxisY",&success,0);kotor_fastcall_00671be0(self,nullptr,si(y));
  if(gui_load_gff_byte_00411a60(gff(self),nullptr,&node,"FlipAxisX",&success,0))wr<U>(self,0x84,0u-rd<U>(self,0x84));
  if(gui_load_gff_byte_00411a60(gff(self),nullptr,&node,"FlipAxisY",&success,0))wr<U>(self,0x88,0u-rd<U>(self,0x88));
 }
 success=gff_list_004118c0(gff(self),nullptr,&list,&mini,"Enemies");
 if(success){const I enemies=si(gff_list_count_00411940(gff(self),nullptr,&list));
  for(I i=0;i<enemies;++i){
   success=gff_list_element_00411990(gff(self),nullptr,&node,&list,U(i));if(!success)continue;
   success=gff_list_004118c0(gff(self),nullptr,&sublist,&node,"Models");if(!success)continue;
   const I count=si(gff_list_count_00411940(gff(self),nullptr,&sublist));
   for(I j=0;j<count;++j){
    gff_list_element_00411990(gff(self),nullptr,&modelNode,&sublist,U(j));read_ref(self,&modelNode,"Model",&success,&reference);
    if(gui_load_resref_is_valid_00406030(&reference,nullptr)){
     kotor_fastcall_005089e0(&models,nullptr,reference);
     unsigned char flag=gui_load_gff_byte_00411a60(gff(self),nullptr,&modelNode,"RotatingModel",&success,1);
     recovered_byte_list_add_owned_005704f0(&rotating,nullptr,flag);
    }
   }
   if(!models.count)continue;
   read_ref(self,&node,"Track",&success,&reference);
   if(!gui_load_resref_is_valid_00406030(&reference,nullptr)){reset(models);reset(rotating);continue;}
   void*enemy=kotor_fastcall_00671e40(self,nullptr,&models,&rotating,&reference);
   reset(models);reset(rotating);if(enemy)actor_load(enemy,gff(self),&node);
  }
 }
 success=gff_list_004118c0(gff(self),nullptr,&list,&mini,"Obstacles");
 if(success){const I count=si(gff_list_count_00411940(gff(self),nullptr,&list));
  for(I i=0;i<count;++i){
   success=gff_list_element_00411990(gff(self),nullptr,&node,&list,U(i));if(!success)continue;
   read_ref(self,&node,"Name",&success,&reference);if(!gui_load_resref_is_valid_00406030(&reference,nullptr))continue;
   c2da_ref_to_string_00405f70(&reference,nullptr,name);
   void*manager=kotor_menu_window_cells::manager_007a39fc;void*array=kotor_fastcall_005edaf0(rd<void*>(manager,4),nullptr);
   void*object=kotor_fastcall_0066bfb0(array,nullptr,name);if(object)(void)kotor_fastcall_0066d0b0(object,nullptr,gff(self),&node);
  }
 }
 (void)kotor_resource_required::res_release_00409b80(gff(self),nullptr);
 return finish(1);
}

