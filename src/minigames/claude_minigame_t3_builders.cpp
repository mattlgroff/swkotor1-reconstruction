#include "e3e5e5154161-claude_minigame_t3_builders.hpp"
#include "2933b71fe68c-scene3d.h"
#include "d1ffedc48c11-gff_readers.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
extern "C" {
void* __cdecl live_operator_new(unsigned) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*,const char*,std::FILE*,std::FILE*) noexcept(false);
int __fastcall resource_equal_004060b0(menu_resources::Ref*,void*,menu_resources::Ref*) noexcept(false);
void* __fastcall kotor_fastcall_0066eb50(void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066dee0(void*,void*,void*,unsigned) noexcept(false);
void* __fastcall kotor_fastcall_00670640(void*,void*,std::uint32_t,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066e280(void*,void*,void*,unsigned) noexcept(false);
void __fastcall kotor_fastcall_0066d030(void*,void*,void*,std::uint8_t) noexcept(false);
void* __fastcall kotor_fastcall_00671670(void*,void*,void*) noexcept(false);
extern const void* const kotor_turret_vtable_007528a8[16];
}
namespace menu_list_growth {void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned) noexcept(false);}
namespace {
using U=std::uint32_t;using I=std::int32_t;using Ref=menu_resources::Ref;using Gob=aurora_model::Gob;using Scene=aurora_model::Scene;using Vector=kotor_scene3d::Vector;
static_assert(sizeof(void*)==4 && sizeof(Vector)==12,"original layout");
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof v);}
void*at(void*p,U o){return reinterpret_cast<void*>(reinterpret_cast<U>(p)+o);}
I si(U u){I v;std::memcpy(&v,&u,4);return v;}
template<class F>F slot(void*p,U o){return rd<F>(rd<void*>(p,0),o);}
struct Raw {void*p;bool armed=true;~Raw()noexcept(false){if(armed)live_free(p);}};
Gob* model(const Ref*ref,char*name){gui_load_resref_copy_to_string_00405fb0(ref,nullptr,name);return aurora_new_aur_object_00449cc0(name,"",nullptr,nullptr);}
void scene_attach(void*self,Gob*gob){
 Scene*scene=rd<Scene*>(rd<void*>(self,0x28),0x184);
 using F=I(__fastcall*)(Gob*,void*,Scene*);(void)slot<F>(gob,0x4c)(gob,nullptr,scene);
}
void prototype_position(void*self,Gob*gob,const Ref*track){
 const I count=rd<I>(self,0x3c);
 for(I i=0;i<count;++i){void*node=rd<void*>(rd<void*>(self,0x38),U(i)*4);
  if(resource_equal_004060b0(const_cast<Ref*>(track),nullptr,static_cast<Ref*>(node))){
   if(node){using F=Vector*(__fastcall*)(Gob*,void*,Vector*,Vector);F f=slot<F>(gob,0x5c);Vector position=rd<Vector>(node,0x10),out;(void)f(gob,nullptr,&out,position);}break;
  }
 }
}
void* player_body(void*self,void*models,void*flags,const Ref*track,const Ref*cameraRef,unsigned attach,bool withCamera){
 const I count=rd<I>(models,4);
 void*old=rd<void*>(self,0x24);if(old){using F=void*(__fastcall*)(void*,void*,U);(void)slot<F>(old,0x28)(old,nullptr,1);}
 void*allocation=live_operator_new(0x250);void*player=nullptr;
 {Raw raw{allocation};if(allocation)player=kotor_fastcall_0066eb50(allocation,nullptr,self);raw.armed=false;}
 wr<void*>(self,0x24,player);
 char name[17];
 for(I i=0;i<count;++i){const Ref*ref=static_cast<const Ref*>(at(rd<void*>(models,0),U(i)*16));Gob*gob=model(ref,name);
  if(gob){U flag=rd<unsigned char>(rd<void*>(flags,0),U(i));void*current=rd<void*>(self,0x24);using F=Gob*(__fastcall*)(void*,void*,Gob*,U);(void)slot<F>(current,0x34)(current,nullptr,gob,flag);}
 }
 Gob*trackGob=model(track,name);
 if(trackGob){prototype_position(self,trackGob,track);scene_attach(self,trackGob);U type=rd<U>(self,0x80);U mode=type==1?1:type==2?2:0;(void)kotor_fastcall_0066e280(rd<void*>(self,0x24),nullptr,trackGob,mode);}
 if(withCamera){Gob*hook=model(cameraRef,name);if(hook){scene_attach(self,hook);kotor_fastcall_0066d030(rd<void*>(self,0x24),nullptr,hook,static_cast<unsigned char>(attach));}}
 kotor_fastcall_00671670(self,nullptr,rd<void*>(self,0x20));return rd<void*>(self,0x24);
}
}
extern "C" void* __fastcall kotor_fastcall_00671820(void*self,void*,void*models,void*flags,const Ref*track) noexcept(false){return player_body(self,models,flags,track,nullptr,0,false);}
extern "C" void* __fastcall kotor_fastcall_006719d0(void*self,void*,void*models,void*flags,const Ref*track,const Ref*camera,unsigned attach) noexcept(false){return player_body(self,models,flags,track,camera,attach,true);}
extern "C" void* __fastcall kotor_fastcall_00671e40(void*self,void*,void*models,void*flags,const Ref*track) noexcept(false){
 const I count=rd<I>(models,4);void*allocation=live_operator_new(0x1a4);void*enemy=nullptr;
 {Raw raw{allocation};if(allocation){(void)kotor_fastcall_0066dee0(allocation,nullptr,self,2);enemy=allocation;wr<const void*>(allocation,0,kotor_turret_vtable_007528a8);}raw.armed=false;}
 char name[17];
 for(I i=0;i<count;++i){const Ref*ref=static_cast<const Ref*>(at(rd<void*>(models,0),U(i)*16));Gob*gob=model(ref,name);if(gob){U flag=rd<unsigned char>(rd<void*>(flags,0),U(i));using F=Gob*(__fastcall*)(void*,void*,Gob*,U);(void)slot<F>(enemy,0x34)(enemy,nullptr,gob,flag);}}
 Gob*trackGob=model(track,name);if(trackGob){prototype_position(self,trackGob,track);scene_attach(self,trackGob);(void)kotor_fastcall_0066e280(enemy,nullptr,trackGob,0);}
 const U capacity=rd<U>(self,0x34);const U countBefore=rd<U>(self,0x30);const U id=rd<U>(enemy,8);
 if(countBefore==capacity){U next=capacity?capacity+capacity:16;void*old=rd<void*>(self,0x2c);wr<U>(self,0x34,next);void*buffer=live_operator_new(next*4);I n=rd<I>(self,0x30);wr<void*>(self,0x2c,buffer);
  if(n>0){U i=0;do{void*dest=rd<void*>(self,0x2c);U value=rd<U>(old,i*4);wr<U>(dest,i*4,value);I fresh=rd<I>(self,0x30);++i;if(si(i)>=fresh)break;}while(true);}if(old)live_free(old);
 }
 U index=rd<U>(self,0x30);void*buffer=rd<void*>(self,0x2c);wr<U>(buffer,index*4,id);wr<U>(self,0x30,rd<U>(self,0x30)+1);return enemy;
}
extern "C" void* __fastcall kotor_fastcall_00672240(void*self,void*,const Ref*nameRef,const Vector*position) noexcept(false){
 char name[17];Gob*gob=model(nameRef,name);if(!gob)return const_cast<Ref*>(nameRef);
 using Position=Vector*(__fastcall*)(Gob*,void*,Vector*,Vector);Vector out;Vector value=*position;(void)slot<Position>(gob,0x5c)(gob,nullptr,&out,value);
 scene_attach(self,gob);using Animate=U(__fastcall*)(Gob*,void*,U,U,U,U);(void)slot<Animate>(gob,0x18)(gob,nullptr,reinterpret_cast<U>("ready"),0x3f800000,0,0);
 void*allocation=live_operator_new(0xb4);void*obstacle=nullptr;U capacity,count,id;
 {Raw raw{allocation};if(allocation)obstacle=kotor_fastcall_00670640(allocation,nullptr,reinterpret_cast<U>(self),gob);capacity=rd<U>(self,0x4c);count=rd<U>(self,0x48);id=rd<U>(obstacle,8);raw.armed=false;}
 void*list=at(self,0x44);if(count==capacity)menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list),nullptr,capacity?capacity+capacity:16);
 U index=rd<U>(list,4);wr<U>(rd<void*>(list,0),index*4,id);wr<U>(list,4,rd<U>(list,4)+1);return obstacle;
}
extern "C" void __fastcall kotor_fastcall_00670b50(void*self,void*,float nearValue,float farValue) noexcept(false){
 // Direct x87 compare/store sequence preserves both comparisons and near-value
 // conversion/NaN behavior without changing the floating-point environment.
 __asm {
  mov ecx,self
  fld nearValue
  fcom farValue
  fnstsw ax
  test ah,041h
  jne clip_nonswap
  mov eax,farValue
  fstp dword ptr[ecx+06ch]
  mov dword ptr[ecx+068h],eax
  jmp clip_stored
 clip_nonswap:
  fcom farValue
  fstp dword ptr[ecx+068h]
  fnstsw ax
  test ah,5
  jp clip_far_second
  mov edx,farValue
  mov dword ptr[ecx+06ch],edx
  jmp clip_stored
 clip_far_second:
  mov eax,farValue
  mov dword ptr[ecx+06ch],eax
 clip_stored:
 }
 void*camera=rd<void*>(self,0x20);if(camera){using F=U(__fastcall*)(void*,void*,U,U);F set=slot<F>(camera,0x40);U farBits=rd<U>(self,0x6c),nearBits=rd<U>(self,0x68);(void)set(camera,nullptr,nearBits,farBits);}
}


