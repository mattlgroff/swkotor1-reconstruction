#include "73427a97cdda-claude_minigame_t3_hit.hpp"
#include "2933b71fe68c-scene3d.h"
#include "322371ce1803-m46_cells_storage.h"
#include <cstdint>
#include <cstring>
extern "C" {

void* __cdecl live_operator_new(unsigned) noexcept(false);
void __fastcall kotor_fastcall_00671460(void*,void*,const void*) noexcept(false);
void __fastcall kotor_fastcall_0066c190(void*,void*,unsigned) noexcept(false);
void* __fastcall kotor_fastcall_00673080(void*,void*) noexcept(false);
void* __fastcall safe_pointer_assign_00447fa0(void*,void*,void*) noexcept(false);
void* __fastcall GuiAssign005e5140(void*,void*,const char*) noexcept(false);
void* __cdecl pointto_004acca0(void*,const void*) noexcept(false);
kotor_scene3d::Quaternion* __fastcall quaternion_construct_004ac960(kotor_scene3d::Quaternion*,void*,kotor_scene3d::Vector,float) noexcept(false);
kotor_scene3d::Quaternion* __cdecl quaternion_multiply_004a9ac0(kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*) noexcept(false);
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations*,void*,std::uint32_t) noexcept(false);
void __cdecl w3_bullet_die_00670b20(void*) noexcept(false);
}
namespace {
unsigned char* hit_buffer(){return reinterpret_cast<unsigned char*>(&scene_hit_info_007b93a8);}
using U=std::uint32_t;using Gob=aurora_model::Gob;using Part=aurora_model::Part;
using Vector=kotor_scene3d::Vector;using Quaternion=kotor_scene3d::Quaternion;
static_assert(sizeof(void*)==4 && sizeof(Vector)==12 && sizeof(Quaternion)==16,"original x86 values");
template<class T>T rd(const void*p,U off){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v;}
template<class T>void wr(void*p,U off,T v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,sizeof v);}
void* at(void*p,U off){return reinterpret_cast<void*>(reinterpret_cast<U>(p)+off);}
float fl(U bits){float v;std::memcpy(&v,&bits,4);return v;}
template<class F>F slot(void*p,U off){return rd<F>(rd<void*>(p,0),off);}
}
extern "C" std::int32_t __fastcall kotor_fastcall_006730a0(void*self,void*,void*bulletRaw,void*targetRaw,void*receiver) noexcept(false){
 Gob*bullet=static_cast<Gob*>(bulletRaw);Gob*target=static_cast<Gob*>(targetRaw);
 using GetBehavior=U(__fastcall*)(Gob*,void*,U);
 void*behavior=reinterpret_cast<void*>(slot<GetBehavior>(bullet,0x108)(bullet,nullptr,0xaaab));
 if(!behavior)return 0;
 using GetPart=void*(__fastcall*)(Gob*,void*,const char*);
 void*part=slot<GetPart>(target,0x10c)(target,nullptr,"hitbullet");if(!part)return 0;
 using GetVector=U(__fastcall*)(Gob*,void*,U);
 Vector currentOut,previousOut;
 void*current=reinterpret_cast<void*>(slot<GetVector>(bullet,0x64)(bullet,nullptr,reinterpret_cast<U>(&currentOut)));
 void*previous=reinterpret_cast<void*>(slot<GetVector>(bullet,0x6c)(bullet,nullptr,reinterpret_cast<U>(&previousOut)));
 // Original captures current components first, then previous, after both callbacks.
 const U cx=rd<U>(current,0);
 using HitPart=U(__fastcall*)(Gob*,void*,U,U,U,U,U,U,U,U,U);
 HitPart hitPart=slot<HitPart>(target,0x88);
 const U cy=rd<U>(current,4),cz=rd<U>(current,8);
 const U px=rd<U>(previous,0),py=rd<U>(previous,4),pz=rd<U>(previous,8);
 if(!hitPart(target,nullptr,reinterpret_cast<U>(part),px,py,pz,cx,cy,cz,0,reinterpret_cast<U>(hit_buffer())))return 0;
 kotor_fastcall_00671460(receiver,nullptr,at(behavior,0x14));
 void*shooter=rd<void*>(behavior,0x3c);wr<U>(receiver,0x4c,rd<U>(shooter,8));
 const char*partName=rd<const char*>(hit_buffer(),0x4c);
 GuiAssign005e5140(at(receiver,0x44),nullptr,partName);
 wr<U>(receiver,0x54,rd<U>(hit_buffer(),0x1c));
 wr<U>(receiver,0x58,rd<U>(hit_buffer(),0x20));
 wr<U>(receiver,0x5c,rd<U>(hit_buffer(),0x24));
 kotor_fastcall_0066c190(receiver,nullptr,0);
 using Animate=U(__fastcall*)(Gob*,void*,U,U,U,U);
 if(!slot<Animate>(bullet,0x18)(bullet,nullptr,reinterpret_cast<U>("explode"),0x3f800000,3,0)){
  using Delete=void*(__fastcall*)(Gob*,void*,U);
  (void)slot<Delete>(bullet,0)(bullet,nullptr,1);return 1;
 }
 void*allocation=live_operator_new(0x20);
 void*explosion=allocation?kotor_fastcall_00673080(allocation,nullptr):nullptr;
 // No new null guard or unwind ownership: original immediately binds this result.
 safe_pointer_assign_00447fa0(explosion,nullptr,bullet);
 Quaternion pointing;
 void*pointed=pointto_004acca0(&pointing,hit_buffer()+0x40);
 wr<U>(explosion,0x10,rd<U>(pointed,0));wr<U>(explosion,0x14,rd<U>(pointed,4));
 wr<U>(explosion,0x18,rd<U>(pointed,8));wr<U>(explosion,0x1c,rd<U>(pointed,12));
 Quaternion quarterTurn,product;
 Vector axis={1.0f,0.0f,0.0f};
 Quaternion*left=quaternion_construct_004ac960(&quarterTurn,nullptr,axis,fl(0x3fc90fdb));
 Quaternion*result=quaternion_multiply_004a9ac0(&product,left,static_cast<const Quaternion*>(at(explosion,0x10)));
 wr<U>(explosion,0x10,rd<U>(result,0));wr<U>(explosion,0x14,rd<U>(result,4));
 wr<U>(explosion,0x18,rd<U>(result,8));wr<U>(explosion,0x1c,rd<U>(result,12));
 // Shared hit position is freshly read after all preceding callbacks/math.
 wr<U>(explosion,4,rd<U>(hit_buffer(),0x1c));wr<U>(explosion,8,rd<U>(hit_buffer(),0x20));wr<U>(explosion,12,rd<U>(hit_buffer(),0x24));
 using SetBehavior=U(__fastcall*)(Gob*,void*,void*);(void)slot<SetBehavior>(bullet,0x58)(bullet,nullptr,nullptr);
 using Callback=void(__cdecl*)(void*,void*,Part*);
 using Register=U(__fastcall*)(Gob*,void*,Callback,const char*,Part*,U,U);
 // Reviewed x86 ABI view: retained original callback ignores caller-cleaned extra arguments.
 auto retainedCallback=&w3_bullet_die_00670b20;
 static_assert(sizeof(Callback)==sizeof(retainedCallback),"x86 callback pointer representation");
 Callback callback;std::memcpy(&callback,&retainedCallback,sizeof callback);
 (void)slot<Register>(bullet,0x28)(bullet,nullptr,callback,"doneexplode",nullptr,0x461c3c00,0);
 gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(at(self,0x5c)),nullptr,reinterpret_cast<U>(explosion));
 return 1;
}



