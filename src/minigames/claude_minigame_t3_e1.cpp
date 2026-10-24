// Complete admitted controller lifecycle. Source-only; pending imports are real owned providers.
#include "fd7e4d92f22b-claude_minigame_t3_e1.hpp"
#include "d8f703239b61-resources.h"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace menu_tables_required {
void* __fastcall StreamConstruct_005d5a50(void*,void*) noexcept(false);
void __fastcall StreamDestroy_005d6200(void*,void*) noexcept(false);
void __fastcall StreamSetLooping_005d5b40(void*,void*,unsigned) noexcept(false);
}
extern "C" {
void* __cdecl live_operator_new(unsigned) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
void* __cdecl claude_0886_area_table(unsigned) noexcept(false);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*) noexcept(false);
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*,void*,menu_resources::Ref*,const char*) noexcept(false);
void __fastcall minigame_set_resref_00506c30(void*,void*,const menu_resources::Ref*,unsigned) noexcept(false);
void __fastcall kotor_fastcall_00506bd0(void*,void*) noexcept(false);
void __fastcall exo_array_list_destruct_0056b6f0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_005edaf0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,unsigned,unsigned) noexcept(false);
void* __fastcall kotor_fastcall_00671670(void*,void*,void*) noexcept(false);
void __fastcall kotor_fastcall_006706e0(void*,void*) noexcept(false);
int __fastcall kotor_fastcall_006721f0(void*,void*) noexcept(false);
}
extern "C" const void* const kotor_turret_vtable_00752914[1]={reinterpret_cast<const void*>(&kotor_fastcall_00673060)};
namespace {
using U=std::uint32_t;using I=std::int32_t;
static_assert(sizeof(void*)==4 && sizeof(menu_resources::Ref)==16,"original x86 layout");
template<class T>T rd(const void*p,U off){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v;}
template<class T>void wr(void*p,U off,T v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,sizeof v);}
void* at(void*p,U off){return reinterpret_cast<void*>(reinterpret_cast<U>(p)+off);}
const menu_resources::Ref* ref_at(void*p,U off){return static_cast<const menu_resources::Ref*>(at(p,off));}
using Destroy=void(__fastcall*)(void*,void*);
struct Cleanup {
 void* object;Destroy destroy;bool armed;
 Cleanup(void*p,Destroy f):object(p),destroy(f),armed(true){}
 ~Cleanup() noexcept(false){if(armed)destroy(object,nullptr);}
 Cleanup(const Cleanup&)=delete;Cleanup&operator=(const Cleanup&)=delete;
};
struct RawAllocation {
 void* object;bool armed;
 explicit RawAllocation(void*p):object(p),armed(true){}
 ~RawAllocation() noexcept(false){if(armed)live_free(object);}
 RawAllocation(const RawAllocation&)=delete;RawAllocation&operator=(const RawAllocation&)=delete;
};
void zero_list(void*self,U off){wr<U>(self,off+4,0);wr<U>(self,off+8,0);wr<U>(self,off,0);}
void scalar_delete(void*object){
 using F=void*(__fastcall*)(void*,void*,unsigned);
 F f=rd<F>(rd<void*>(object,0),0x28);(void)f(object,nullptr,1);
}
}
extern "C" void* __fastcall kotor_fastcall_00671c40(void*self,void*,void*area) noexcept(false){
 // Base constructor state is not armed until both reference operations finish.
 wr<void*>(self,0,claude_0886_area_table(0x74767c));
 auto* reference=static_cast<menu_resources::Ref*>(at(self,0xc));
 c2da_default_ref_00405ed0(reference,nullptr);
 menu_resources::Ref result;
 wr<U>(self,8,0);
 unresolved_ref_assign_00406290(reference,nullptr,&result,"");
 wr<U>(self,4,0);
 wr<const void*>(self,0,kotor_turret_vtable_00752914);
 Cleanup base(self,kotor_fastcall_00506bd0); // state0
 zero_list(self,0x2c);Cleanup enemies(at(self,0x2c),exo_array_list_destruct_0056b6f0);
 zero_list(self,0x38);Cleanup tracks(at(self,0x38),exo_array_list_destruct_0056b6f0);
 zero_list(self,0x44);Cleanup obstacles(at(self,0x44),exo_array_list_destruct_0056b6f0);
 zero_list(self,0x50);Cleanup bullets(at(self,0x50),exo_array_list_destruct_0056b6f0);
 zero_list(self,0x5c);Cleanup explosions(at(self,0x5c),exo_array_list_destruct_0056b6f0); // state5
 c2da_default_ref_00405ed0(static_cast<menu_resources::Ref*>(at(self,0x96)),nullptr);
 wr<U>(self,0xb0,0);wr<U>(self,0xb4,0);wr<U>(self,0xb8,0);
 wr<U>(self,0xc0,0);wr<U>(self,0xc4,0);wr<U>(self,0xc8,0);
 wr<U>(self,0xcc,0);wr<U>(self,0xd0,0);wr<U>(self,0xd4,0);
 wr<U>(self,0xcc,0);wr<U>(self,0xc0,0);wr<U>(self,0xd0,0);wr<U>(self,0xc4,0);wr<U>(self,0xd4,0);wr<U>(self,0xc8,0);
 wr<U>(self,0xd8,0);wr<U>(self,0xdc,0);wr<U>(self,0xe0,0);
 wr<U>(self,0xe4,0);wr<U>(self,0xe8,0);wr<U>(self,0xec,0);
 wr<U>(self,0xd8,0);wr<U>(self,0xe4,0);wr<U>(self,0xdc,0);wr<U>(self,0xe8,0);wr<U>(self,0xe0,0);wr<U>(self,0xec,0);
 wr<void*>(self,0x28,area);wr<U>(self,0x24,0);wr<U>(self,0x20,0);wr<U>(self,0x8c,0);
 wr<U>(self,0x68,0x3dcccccd);wr<U>(self,0x70,0x42820000);wr<U>(self,0x6c,0x42c80000);
 wr<U>(self,0x78,0);wr<U>(self,0x7c,0);wr<U>(self,0x80,0);wr<U>(self,0x84,0);wr<U>(self,0x88,0);
 wr<U>(self,0xac,1);wr<U>(self,0xa8,0);
 void* sound=live_operator_new(4);
 {RawAllocation raw(sound); // state6: only construction owns this raw block.
  if(sound)sound=menu_tables_required::StreamConstruct_005d5a50(sound,nullptr);
  raw.armed=false;
 }
 // Restore state5: later failures intentionally do not destroy the sound object.
 wr<void*>(self,0x1c,sound);
 menu_tables_required::StreamSetLooping_005d5b40(sound,nullptr,1);
 minigame_set_resref_00506c30(self,nullptr,ref_at(area,0x10c),0);
 explosions.armed=false;bullets.armed=false;obstacles.armed=false;tracks.armed=false;enemies.armed=false;base.armed=false;
 return self;
}
extern "C" int __fastcall kotor_fastcall_00672040(void*self,void*) noexcept(false){
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 void*app=rd<void*>(manager,4);const I count=rd<I>(self,0x30);
 void*array=kotor_fastcall_005edaf0(app,nullptr);
 for(I i=0;i<count;++i){void*list=rd<void*>(self,0x2c);const U id=rd<U>(list,U(i)*4);void*object=kotor_fastcall_0066bf30(array,nullptr,id,2);if(object)scalar_delete(object);}
 void*list=rd<void*>(self,0x2c);if(list){live_free(list);wr<U>(self,0x2c,0);wr<U>(self,0x34,0);}wr<U>(self,0x30,0);return count;
}
extern "C" int __fastcall kotor_fastcall_00672350(void*self,void*) noexcept(false){
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 void*app=rd<void*>(manager,4);const I count=rd<I>(self,0x48);
 void*array=kotor_fastcall_005edaf0(app,nullptr);
 for(I i=0;i<count;++i){void*list=rd<void*>(self,0x44);const U id=rd<U>(list,U(i)*4);void*object=kotor_fastcall_0066bf30(array,nullptr,id,3);if(object){kotor_fastcall_006706e0(object,nullptr);live_free(object);}}
 void*list=rd<void*>(self,0x44);if(list){live_free(list);wr<U>(self,0x44,0);wr<U>(self,0x4c,0);}wr<U>(self,0x48,0);return count;
}
extern "C" void* __fastcall kotor_fastcall_006717f0(void*self,void*,void*player) noexcept(false){
 void*prior=rd<void*>(self,0x24);if(prior)scalar_delete(prior);
 void*camera=rd<void*>(self,0x20);wr<void*>(self,0x24,player);
 kotor_fastcall_00671670(self,nullptr,camera);return rd<void*>(self,0x24);
}
extern "C" void __fastcall kotor_fastcall_00672eb0(void*self,void*) noexcept(false){
 wr<const void*>(self,0,kotor_turret_vtable_00752914);
 Cleanup base(self,kotor_fastcall_00506bd0);
 Cleanup enemies(at(self,0x2c),exo_array_list_destruct_0056b6f0);
 Cleanup tracks(at(self,0x38),exo_array_list_destruct_0056b6f0);
 Cleanup obstacles(at(self,0x44),exo_array_list_destruct_0056b6f0);
 Cleanup bullets(at(self,0x50),exo_array_list_destruct_0056b6f0);
 Cleanup explosions(at(self,0x5c),exo_array_list_destruct_0056b6f0);
 void*player=rd<void*>(self,0x24);if(player)scalar_delete(player);
 void*camera=rd<void*>(self,0x20);wr<U>(self,0x24,0);kotor_fastcall_00671670(self,nullptr,camera);
 kotor_fastcall_00672040(self,nullptr);kotor_fastcall_006721f0(self,nullptr);kotor_fastcall_00672350(self,nullptr);
 void*sound=rd<void*>(self,0x1c);if(sound){menu_tables_required::StreamDestroy_005d6200(sound,nullptr);live_free(sound);}
 // Each cleanup state drops before its explicit free, matching original unwind.
 void*p=rd<void*>(self,0x5c);explosions.armed=false;live_free(p);wr<U>(self,0x5c,0);
 p=rd<void*>(self,0x50);bullets.armed=false;live_free(p);wr<U>(self,0x50,0);
 p=rd<void*>(self,0x44);obstacles.armed=false;live_free(p);wr<U>(self,0x44,0);
 p=rd<void*>(self,0x38);tracks.armed=false;live_free(p);wr<U>(self,0x38,0);
 p=rd<void*>(self,0x2c);enemies.armed=false;live_free(p);wr<U>(self,0x2c,0);
 base.armed=false;kotor_fastcall_00506bd0(self,nullptr);
}
extern "C" void* __fastcall kotor_fastcall_00673060(void*self,void*,unsigned flags) noexcept(false){
 kotor_fastcall_00672eb0(self,nullptr);if(flags&1u)live_free(self);return self;
}
