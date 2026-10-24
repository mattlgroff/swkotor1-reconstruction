#include "dd5618d9ad7a-minigame_player_collision.hpp"
#include "2933b71fe68c-scene3d.h"
#include <cstdint>
#include <cstring>
#include "d1c462888f42-claude_turret_t1_leaves.hpp"
#include "aafac7614c0f-claude_turret_t1_events.hpp"
#include "b89d92983330-claude_minigame_t3_c2.hpp"
#include "c6d94c22e2d8-claude_turret_t4_teardown.hpp"
#include "d69b5cbffe54-minigame_planeproject.hpp"
#include "0a911a07647c-minigame_tunnel_offset.hpp"
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,std::uint32_t,std::uint32_t);
std::int32_t __cdecl sphere_intersect_004abdd0(const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,std::uint32_t);
kotor_scene3d::Vector* __fastcall vector_add_assign_004a9740(kotor_scene3d::Vector*,void*,const kotor_scene3d::Vector*);
float* __fastcall vector_divide_assign_004a97d0(float*,void*,float);
}
namespace t2_collision {
using V=kotor_scene3d::Vector;using I=std::int32_t;using U=std::uint32_t;
static_assert(sizeof(V)==12 && sizeof(void*)==4,"x86 original layout");
inline unsigned char* at(void*p,U n){return reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(p)+n);}
template<class T>T rd(const void*p,U n){T v;std::memcpy(&v,reinterpret_cast<const void*>(reinterpret_cast<std::uintptr_t>(p)+n),sizeof v);return v;}
template<class T>void wr(void*p,U n,T v){std::memcpy(at(p,n),&v,sizeof v);}
inline I sint(U u){I v;std::memcpy(&v,&u,4);return v;}
inline void copy3(V* d,const void*s){for(U n=0;n<12;n+=4)wr(d,n,rd<U>(s,n));}
template<class F>F method(void*p,U slot){return rd<F>(rd<void*>(p,0),slot);}
using GetVector=V*(__fastcall*)(void*,void*,V*);
using GetQuaternion=void*(__fastcall*)(void*,void*,void*);
using SetPosition=V*(__fastcall*)(void*,void*,V*,V);
using Play=void(__fastcall*)(void*,void*,const char*,float,U,U);
using ActorPlay=void(__fastcall*)(void*,void*,const char*,float,U);
}
#include "167f30dacee0-collision_math.hpp"
extern "C" std::int32_t __fastcall kotor_fastcall_0066eda0(void* self,void*,void* ids,std::int32_t applyDamage) noexcept(false){
 using namespace t2_collision;
 const I playerModelCount=rd<I>(self,0x6c);U hits=0;
 if(playerModelCount==0)return 0;
 void* app=rd<void*>(kotor_menu_window_cells::manager_007a39fc,4);
 I index=rd<I>(ids,4);I maxDamage=0;
 void* model=rd<void*>(rd<void*>(self,0x68),0);
 void* objects=kotor_fastcall_005edaf0(app,nullptr);
 V position,previous;alignas(4) unsigned char quaternion[16];
 method<GetVector>(model,0x64)(model,nullptr,&position);
 method<GetVector>(model,0x6c)(model,nullptr,&previous);
 void* returnedQuaternion=method<GetQuaternion>(model,0x68)(model,nullptr,quaternion);
 V normal;kotor_fastcall_00670bb0(rd<void*>(self,0x60),nullptr,&normal,returnedQuaternion);
 const float distance=plane_distance(&position,&normal);
 V accumulated{0,0,0},enemyPosition{0,0,0},end{0,0,0},start{0,0,0},zero{0,0,0};
 index=sint(static_cast<U>(index)-1u);
 if(index<0)return 0;
 for(;;){
  void* enemy=kotor_fastcall_0066bf30(objects,nullptr,rd<U>(rd<void*>(ids,0),static_cast<U>(index)*4u),2);
  if(!enemy){
   U count=rd<U>(ids,4)-1u;wr(ids,4,count);
   I move=index;
   while(move<sint(count)){
    void* list=rd<void*>(ids,0);U value=rd<U>(list,static_cast<U>(move)*4u+4u);wr(list,static_cast<U>(move)*4u,value);
    count=rd<U>(ids,4);move=sint(static_cast<U>(move)+1u);
   }
  }else if(rd<unsigned char>(enemy,0x1a0)!=0 || rd<unsigned char>(rd<void*>(self,0x60),0x95)!=0){
   if(rd<I>(enemy,0x6c)<=0)break;
   void* enemyModel=rd<void*>(rd<void*>(enemy,0x68),0);if(!enemyModel)break;
   V queryPosition;copy3(&enemyPosition,method<GetVector>(enemyModel,0x64)(enemyModel,nullptr,&queryPosition));
   enemyModel=rd<I>(enemy,0x6c)>0?rd<void*>(rd<void*>(enemy,0x68),0):nullptr;
   V queryPrevious,enemyPrevious;
   copy3(&enemyPrevious,method<GetVector>(enemyModel,0x6c)(enemyModel,nullptr,&queryPrevious));
   subtract(&end,&position,&enemyPosition);
   const float enemyRadius=rd<float>(enemy,0x84);
   subtract(&start,&previous,&enemyPrevious);
   float combined=radius(enemyRadius,reinterpret_cast<float*>(at(self,0x84)));U radiusBits;std::memcpy(&radiusBits,&combined,4);
   if(sphere_intersect_004abdd0(&start,&end,&zero,radiusBits)){
    if(rd<unsigned char>(enemy,0x1a0)!=0){
     if(applyDamage){
      wr(self,0x50,rd<U>(enemy,8));kotor_fastcall_0066c2d0(self,nullptr,0);
      wr(enemy,0x50,rd<U>(self,8));kotor_fastcall_0066c2d0(enemy,nullptr,0);
     }
    }else{
     V projectedOut;copy3(&enemyPosition,kotor_cdecl_004a9850(&projectedOut,&normal,distance,&enemyPosition));
     V radial;subtract(&radial,&position,&enemyPosition);
     float separationDistance=separation(enemyRadius,reinterpret_cast<float*>(at(self,0x84)));
     V normalized;V* unit=normalize_004ab180(&normalized,&radial);
     V delta;correction(&delta,unit,separationDistance,&enemyPosition,&position);
     vector_add_assign_004a9740(&accumulated,nullptr,&delta);
     if(applyDamage){
      if(!kotor_fastcall_005cb440(enemy,nullptr)){
       kotor_fastcall_0066d490(enemy,nullptr);
       kotor_fastcall_0066e8e0(enemy,nullptr,sint(0u-rd<U>(self,0x94)));
       wr(enemy,0x9c,rd<U>(enemy,0xa0));
      }
      I damage=rd<I>(enemy,0x94);if(maxDamage<=damage)maxDamage=damage;
      wr(self,0x50,rd<U>(enemy,8));kotor_fastcall_0066c2d0(self,nullptr,0);
      wr(enemy,0x50,rd<U>(self,8));kotor_fastcall_0066c2d0(enemy,nullptr,0);
     }
     ++hits;
    }
   }
  }
  index=sint(static_cast<U>(index)-1u);if(index<0)break;
 }
 if(hits==0)return 0;
 vector_divide_assign_004a97d0(reinterpret_cast<float*>(&accumulated),nullptr,count_float(sint(hits)));
 V offset;offset_sum(&offset,&accumulated,reinterpret_cast<V*>(at(self,0x1c4)));
 kotor_fastcall_0066cb40(self,nullptr,&offset);
 V newPosition;new_position(&newPosition,&offset,&position,reinterpret_cast<V*>(at(self,0x1c4)));
 copy3(reinterpret_cast<V*>(at(self,0x1c4)),&offset);
 for(I i=0;i<playerModelCount;i=sint(static_cast<U>(i)+1u)){
  void* current=rd<void*>(rd<void*>(self,0x68),static_cast<U>(i)*8u);V out;
  method<SetPosition>(current,0x5c)(current,nullptr,&out,newPosition);
 }
 if(!cooldown_zero(reinterpret_cast<float*>(at(self,0x9c))))return sint(hits);
 float minimum=load_float(reinterpret_cast<float*>(at(self,0x1d8)));wr(self,0x98,minimum);
 void* trackModel=rd<void*>(self,0x64);
 if(trackModel && (rd<unsigned char>(self,0x88)&1)){
  U loops=rd<U>(self,0x80);float speed=track_speed(minimum);
  method<Play>(trackModel,0x18)(trackModel,nullptr,"track",speed,loops==1?17u:20u,0);
 }
 wr(self,0x9c,rd<U>(self,0xa0));
 if(applyDamage){
  method<ActorPlay>(self,0)(self,nullptr,"damage",1.0f,1);
  method<ActorPlay>(self,0)(self,nullptr,"Ready_01",1.0f,32);
  I banks=rd<I>(self,0x78);
  for(I i=0;i<banks;i=sint(static_cast<U>(i)+1u))kotor_fastcall_00673fd0(rd<void*>(rd<void*>(self,0x74),static_cast<U>(i)*4u),nullptr);
  wr(self,0x19c,0u-static_cast<U>(maxDamage));kotor_fastcall_0066e7a0(self,nullptr,0);wr<unsigned char>(self,0x1d4,1);
 }
 return sint(hits);
}
