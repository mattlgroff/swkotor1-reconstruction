#include "e86a2bc3654d-claude_minigame_t3_e2.hpp"
#include "2933b71fe68c-scene3d.h"
#include "daf54a60b23e-texture.hpp"
#include <cstdint>
#include <cstring>
struct W4Vector;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,unsigned,unsigned) noexcept(false);
std::int32_t __fastcall kotor_fastcall_0066d4e0(void*,void*,void*) noexcept(false);
std::int32_t __fastcall kotor_fastcall_006730a0(void*,void*,void*,void*,void*) noexcept(false);
std::int32_t __fastcall kotor_fastcall_0066e6b0(void*,void*,void*) noexcept(false);
std::int32_t __fastcall kotor_fastcall_0066eda0(void*,void*,void*,std::int32_t) noexcept(false);
void __fastcall kotor_fastcall_0066c610(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066d5d0(void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_00605fa0(void*,void*,W4Vector*) noexcept(false);
void __fastcall texture_list_remove_00445080(texture::List*,void*,texture::U) noexcept(false);
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void*,void*) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
}
namespace {
using U=std::uint32_t;using I=std::int32_t;
using Vector=kotor_scene3d::Vector;
static_assert(sizeof(void*)==4 && sizeof(Vector)==12,"original x86 layout");
template<class T>T rd(const void*p,U off){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v;}
template<class T>void wr(void*p,U off,T v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,sizeof v);}
I si(U v){I out;std::memcpy(&out,&v,4);return out;}
void* at(void*p,U off){return reinterpret_cast<void*>(reinterpret_cast<U>(p)+off);}
// Original compaction decrements count first and refreshes list/count each iteration.
void compact(void*self,U listOffset,U countOffset,U index){
 const U count=rd<U>(self,countOffset)-1;wr<U>(self,countOffset,count);
 U i=index;
 if(si(i)<si(count)) do {
  void* list=rd<void*>(self,listOffset);const U next=rd<U>(list,i*4+4);wr<U>(list,i*4,next);
  const U fresh=rd<U>(self,countOffset);++i;if(si(i)>=si(fresh))break;
 } while(true);
}
void remove_bullet(void*self,U index,void*wrapper){
 if(wrapper){void*object=rd<void*>(wrapper,0);if(object)texture_list_remove_00445080(static_cast<texture::List*>(at(object,4)),nullptr,reinterpret_cast<U>(wrapper));live_free(wrapper);}
 compact(self,0x50,0x54,index);
}
void update_actor(void*object,float delta){using F=void(__fastcall*)(void*,void*,float);F f=rd<F>(rd<void*>(object,0),0x24);f(object,nullptr,delta);}
}
extern "C" void __fastcall kotor_fastcall_006732f0(void*self,void*) noexcept(false){
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 void*app=rd<void*>(manager,4);const U initial=rd<U>(self,0x54);
 void*array=kotor_fastcall_005edaf0(app,nullptr);
 for(U bulletIndex=initial-1;si(bulletIndex)>=0;--bulletIndex){
  void*wrapper=rd<void*>(rd<void*>(self,0x50),bulletIndex*4);
  void*bullet=rd<void*>(wrapper,0); // original deliberately dereferences a null wrapper
  if(!bullet){remove_bullet(self,bulletIndex,wrapper);continue;}
  void*player=rd<void*>(self,0x24);
  if(player && kotor_fastcall_0066d4e0(player,nullptr,bullet)){
   wrapper=rd<void*>(rd<void*>(self,0x50),bulletIndex*4);remove_bullet(self,bulletIndex,wrapper);continue;
  }
  bool hit=false;
  for(U enemyIndex=rd<U>(self,0x30)-1;si(enemyIndex)>=0;--enemyIndex){
   const U id=rd<U>(rd<void*>(self,0x2c),enemyIndex*4);
   void*enemy=kotor_fastcall_0066bf30(array,nullptr,id,2);
   if(!enemy){compact(self,0x2c,0x30,enemyIndex);continue;}
   if(kotor_fastcall_0066d4e0(enemy,nullptr,bullet)){
    wrapper=rd<void*>(rd<void*>(self,0x50),bulletIndex*4);remove_bullet(self,bulletIndex,wrapper);hit=true;break;
   }
  }
  if(hit)continue;
  for(U obstacleIndex=rd<U>(self,0x48)-1;si(obstacleIndex)>=0;--obstacleIndex){
   const U id=rd<U>(rd<void*>(self,0x44),obstacleIndex*4);
   void*obstacle=kotor_fastcall_0066bf30(array,nullptr,id,3);
   if(!obstacle){compact(self,0x44,0x48,obstacleIndex);continue;}
   void*model=rd<void*>(obstacle,0x60);
   if(kotor_fastcall_006730a0(self,nullptr,bullet,model,obstacle)){
    wrapper=rd<void*>(rd<void*>(self,0x50),bulletIndex*4);remove_bullet(self,bulletIndex,wrapper);break;
   }
  }
 }
 for(U obstacleIndex=rd<U>(self,0x48)-1;si(obstacleIndex)>=0;--obstacleIndex){
  void*player=rd<void*>(self,0x24);
  if(!player || (rd<unsigned char>(player,0x88)&2u))continue;
  const U id=rd<U>(rd<void*>(self,0x44),obstacleIndex*4);
  void*obstacle=kotor_fastcall_0066bf30(array,nullptr,id,3);
  if(obstacle){player=rd<void*>(self,0x24);(void)kotor_fastcall_0066e6b0(player,nullptr,obstacle);}
  else compact(self,0x44,0x48,obstacleIndex);
 }
 void*player=rd<void*>(self,0x24);
 if(player && !(rd<unsigned char>(player,0x88)&2u)){
  for(I pass=0;pass<3;++pass){
   player=rd<void*>(self,0x24);
   if(player && !kotor_fastcall_0066eda0(player,nullptr,at(self,0x2c),pass==0?1:0))break;
  }
 }
}
extern "C" void __fastcall kotor_fastcall_006735d0(void*self,void*,float delta) noexcept(false){
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 void*array=kotor_fastcall_005edaf0(rd<void*>(manager,4),nullptr);
 for(U id=0;id<255;++id){
  void*object=kotor_fastcall_0066bf30(array,nullptr,id,0);
  if(object && rd<unsigned char>(object,0xc)){kotor_fastcall_0066c610(object,nullptr);live_free(object);}
 }
 void*player=rd<void*>(self,0x24);if(player)update_actor(player,delta);
 for(U index=rd<U>(self,0x30)-1;si(index)>=0;--index){
  U id=rd<U>(rd<void*>(self,0x2c),index*4);void*object=kotor_fastcall_0066bf30(array,nullptr,id,2);
  if(object)update_actor(object,delta);else compact(self,0x2c,0x30,index);
 }
 for(U index=rd<U>(self,0x48)-1;si(index)>=0;--index){
  U id=rd<U>(rd<void*>(self,0x44),index*4);void*object=kotor_fastcall_0066bf30(array,nullptr,id,3);
  if(object)update_actor(object,delta);else compact(self,0x44,0x48,index);
 }
 for(U index=rd<U>(self,0x60)-1;si(index)>=0;--index){
  void*entry=rd<void*>(rd<void*>(self,0x5c),index*4);
  auto*gob=static_cast<aurora_model::Gob*>(rd<void*>(entry,0));
  using SetPosition=Vector*(__fastcall*)(aurora_model::Gob*,void*,Vector*,Vector);
  SetPosition position=rd<SetPosition>(rd<void*>(gob,0),0x5c);
  Vector value=rd<Vector>(entry,4),outPosition;
  (void)position(gob,nullptr,&outPosition,value);
  using SetOrientation=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U,U,U);
  SetOrientation orientation=rd<SetOrientation>(rd<void*>(gob,0),0x60); // reload table after callback
  U outOrientation[4];
  const U q0=rd<U>(entry,0x10),q1=rd<U>(entry,0x14),q2=rd<U>(entry,0x18),q3=rd<U>(entry,0x1c);
  (void)orientation(gob,nullptr,reinterpret_cast<U>(outOrientation),q0,q1,q2,q3);
  aurora_texture_safe_pointer_destroy_00422380(entry,nullptr);live_free(entry);
  compact(self,0x5c,0x60,index);
 }
 player=rd<void*>(self,0x24);
 if(rd<unsigned char>(player,0x88)&1u)kotor_fastcall_006732f0(self,nullptr); // no null guard: original fault boundary
 player=rd<void*>(self,0x24);
 if(player && rd<U>(player,0x6c)){
  Vector position;void*actualPosition=kotor_fastcall_0066d5d0(player,nullptr,&position);
  void*room=kotor_fastcall_00605fa0(rd<void*>(self,0x28),nullptr,static_cast<W4Vector*>(actualPosition));
  if(room)wr<void*>(self,0xa8,room); // failed lookup keeps the previous room
 }
 void*area=rd<void*>(self,0x28);
 if(area){void*scene=rd<void*>(area,0x184);if(scene){void*room=rd<void*>(self,0xa8);if(room){
  const U index=rd<U>(room,0x44);using SetRoom=U(__fastcall*)(void*,void*,U);
  SetRoom set=rd<SetRoom>(rd<void*>(scene,0),0x7c);(void)set(scene,nullptr,index);
 }}}
}
