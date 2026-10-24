#include "97980543594d-claude_turret_t1_lifecycle.hpp"
#include "2cc88caf3267-growth.h"
#include <cstring>
#include <new>
static_assert(sizeof(void*)==4,"KOTOR x86");
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_item_drop_sounds { void* __fastcall ref_default_element_00405ed0(void*,void*); }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*) noexcept(false);
std::uint32_t __fastcall kotor_fastcall_0066c060(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_00673a10(void*,void*) noexcept(false);
void __fastcall kotor_fastcall_00674800(void*,void*) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
void __fastcall exo_array_list_remove_00445080(kotor_reconstruction::ArrayMutations*,void*,void*) noexcept(false);
void __fastcall exo_array_list_destruct_0056b6f0(kotor_reconstruction::ArrayMutations*,void*) noexcept(false);
void* __fastcall safe_pointer_assign_00447fa0(void*,void*,void*) noexcept(false);
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void*,void*) noexcept(false);
void __fastcall kotor_m15_sound_source_stop_005d5a20(void*,void*) noexcept(false);
void __cdecl packed_purecall_006fb8de();
void* __fastcall gui_load_slot_self_00641db0(void*,void*) noexcept(false);
unsigned __fastcall gui_load_slot_zero_0063e7f0(void*,void*) noexcept(false);
}
#define SLOT(f) reinterpret_cast<const void*>(&f)
namespace t1_lifecycle {
const void* const table_00752424[10]={SLOT(packed_purecall_006fb8de),SLOT(packed_purecall_006fb8de),SLOT(kotor_fastcall_0066c420),SLOT(packed_purecall_006fb8de),SLOT(packed_purecall_006fb8de),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(kotor_fastcall_0066c0a0)};
const void* const table_007524c8[16]={SLOT(kotor_fastcall_0066d550),SLOT(kotor_fastcall_0066cad0),SLOT(kotor_fastcall_0066c740),SLOT(kotor_fastcall_0066c970),SLOT(kotor_fastcall_0066c9c0),SLOT(gui_load_slot_self_00641db0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(kotor_fastcall_0066e130),SLOT(kotor_fastcall_0066f7c0),SLOT(kotor_fastcall_0066fff0),SLOT(packed_purecall_006fb8de),SLOT(kotor_fastcall_0066e1f0),SLOT(packed_purecall_006fb8de),SLOT(packed_purecall_006fb8de)};
const void* const table_0075287c[10]={SLOT(kotor_fastcall_006706c0),SLOT(kotor_fastcall_0066dc80),SLOT(kotor_fastcall_0066d180),SLOT(kotor_fastcall_0066d100),SLOT(kotor_fastcall_0066d150),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_self_00641db0),SLOT(kotor_fastcall_0066c0a0)};
}
#undef SLOT
namespace {
using U=std::uint32_t;using I=std::int32_t;using String=kotor_reconstruction::StartupString;using List=kotor_reconstruction::ArrayMutations;
unsigned char* at(void* p,U n){return static_cast<unsigned char*>(p)+n;}
U rd(void* p,U n){U v;std::memcpy(&v,at(p,n),4);return v;}
I ri(void* p,U n){I v;std::memcpy(&v,at(p,n),4);return v;}
void wr(void* p,U n,U v){std::memcpy(at(p,n),&v,4);}
void* rp(void* p,U n){void* v;std::memcpy(&v,at(p,n),4);return v;}
void wp(void* p,U n,const void* v){std::memcpy(at(p,n),&v,4);}
void publish(void* p,const void* const* table){wp(p,0,table);}
String* str(void* p,U n){return reinterpret_cast<String*>(at(p,n));}
List* list(void* p,U n){return reinterpret_cast<List*>(at(p,n));}
void* client(){return rp(kotor_menu_window_cells::manager_007a39fc,4);}
void unregister_safe(void* field){void* target=rp(field,0);if(target)exo_array_list_remove_00445080(list(target,4),nullptr,field);}
void scalar_delete(void* object){using Fn=U(__fastcall*)(void*,void*,U);auto table=rp(object,0);Fn fn;std::memcpy(&fn,table,4);(void)fn(object,nullptr,1);}
// Only original registered unwind actions; explicit states preserve all throw windows.
struct BaseUnwind{void* self;int state=-1;~BaseUnwind() noexcept(false){if(state>=2)str(self,0x44)->~StartupString();if(state>=1)aurora_texture_safe_pointer_destroy_00422380(at(self,0x18),nullptr);if(state>=0)str(self,0x10)->~StartupString();}};
struct FollowerUnwind{void* self;int state=-1;~FollowerUnwind() noexcept(false){if(state>=2)exo_array_list_destruct_0056b6f0(list(self,0x74),nullptr);if(state>=1)exo_array_list_destruct_0056b6f0(list(self,0x68),nullptr);if(state>=0)kotor_fastcall_0066c610(self,nullptr);}};
struct ObstacleUnwind{void* self;int state=-1;~ObstacleUnwind() noexcept(false){if(state>=1)aurora_texture_safe_pointer_destroy_00422380(at(self,0x60),nullptr);if(state>=0)kotor_fastcall_0066c610(self,nullptr);}};
}
extern "C" void* __fastcall kotor_fastcall_0066c540(void* self,void*,U type) noexcept(false){
    BaseUnwind cleanup{self};publish(self,t1_lifecycle::table_00752424);
    ::new(at(self,0x10)) String;cleanup.state=0;
    wr(self,0x18,0);cleanup.state=1;kotor_fastcall_00673a10(at(self,0x1c),nullptr);
    ::new(at(self,0x44)) String;wr(self,0x54,0);wr(self,0x58,0);wr(self,0x5c,0);
    void* app=client();cleanup.state=2;
    void* objects=kotor_fastcall_005edaf0(app,nullptr);wr(self,8,kotor_fastcall_0066c060(objects,nullptr));
    objects=kotor_fastcall_005edaf0(client(),nullptr);wp(objects,4+rd(self,8)*4,self);
    wr(self,4,type);*at(self,0xc)=0;unregister_safe(at(self,0x18));wr(self,0x18,0);wr(self,0x4c,255);wr(self,0x50,255);
    cleanup.state=-1;return self;
}
extern "C" void __fastcall kotor_fastcall_0066c610(void* self,void*) noexcept(false){
    BaseUnwind cleanup{self};publish(self,t1_lifecycle::table_00752424);void* app=client();cleanup.state=2;
    void* objects=kotor_fastcall_005edaf0(app,nullptr);wr(objects,4+rd(self,8)*4,0);
    cleanup.state=1;str(self,0x44)->~StartupString();unregister_safe(at(self,0x18));
    cleanup.state=-1;str(self,0x10)->~StartupString();
}
extern "C" void* __fastcall kotor_fastcall_0066dee0(void* self,void*,void* context,U type) noexcept(false){
    FollowerUnwind cleanup{self};kotor_fastcall_0066c540(self,nullptr,type);publish(self,t1_lifecycle::table_007524c8);cleanup.state=0;
    wr(self,0x6c,0);wr(self,0x70,0);wr(self,0x68,0);wr(self,0x78,0);wr(self,0x7c,0);wr(self,0x74,0);cleanup.state=2;
    for(U n=0;n<10;++n)kotor_item_drop_sounds::ref_default_element_00405ed0(at(self,0xa4+16*n),nullptr);
    kotor_item_drop_sounds::ref_default_element_00405ed0(at(self,0x148),nullptr);
    for(U n=0;n<2;++n){wr(self,0x158+n*12,0);wr(self,0x15c+n*12,0);wr(self,0x160+n*12,0);}
    kotor_fastcall_00673a10(at(self,0x174),nullptr);
    const U flags=rd(self,0x88);wp(self,0x60,context);wr(self,0x64,0);wr(self,0x88,flags&0xfffffffcu);
    wr(self,0x84,0);wr(self,0x94,0);wr(self,0xa0,0);wr(self,0x9c,0);wr(self,0x19c,0);wr(self,0x98,0x42c80000);wr(self,0x80,1);wr(self,0x8c,100);wr(self,0x170,255);wr(self,0x144,0);
    cleanup.state=-1;return self;
}
extern "C" void __fastcall kotor_fastcall_0066e000(void* self,void*) noexcept(false){
    FollowerUnwind cleanup{self};publish(self,t1_lifecycle::table_007524c8);const I models=ri(self,0x6c);cleanup.state=2;
    for(I i=0;i<models;++i){void* model=rp(rp(self,0x68),static_cast<U>(i)*8);if(model)scalar_delete(model);}
    void* modelList=rp(self,0x68);if(modelList){live_free(modelList);wr(self,0x68,0);wr(self,0x70,0);}wr(self,0x6c,0);
    if((rd(self,0x88)&2)==0){for(I i=static_cast<I>(rd(self,0x78)-1u);i>=0;--i){void* gun=rp(rp(self,0x74),static_cast<U>(i)*4);if(gun){kotor_fastcall_00674800(gun,nullptr);live_free(gun);}}}
    void* sound=rp(self,0x144);if(sound){kotor_m15_sound_source_stop_005d5a20(sound,nullptr);sound=rp(self,0x144);if(sound)scalar_delete(sound);}
    void* track=rp(self,0x64);if(track)scalar_delete(track);
    void* guns=rp(self,0x74);cleanup.state=1;live_free(guns);wr(self,0x74,0);
    modelList=rp(self,0x68);cleanup.state=0;live_free(modelList);wr(self,0x68,0);
    cleanup.state=-1;kotor_fastcall_0066c610(self,nullptr);
}
extern "C" void* __fastcall kotor_fastcall_0066f7c0(void* self,void*,U flags) noexcept(false){kotor_fastcall_0066e000(self,nullptr);if(flags&1)live_free(self);return self;}
extern "C" void* __fastcall kotor_fastcall_00670640(void* self,void*,U,void* model) noexcept(false){
    ObstacleUnwind cleanup{self};kotor_fastcall_0066c540(self,nullptr,3);publish(self,t1_lifecycle::table_0075287c);cleanup.state=0;
    wr(self,0x60,0);cleanup.state=1;for(U n=0;n<5;++n)kotor_item_drop_sounds::ref_default_element_00405ed0(at(self,0x64+16*n),nullptr);
    safe_pointer_assign_00447fa0(at(self,0x60),nullptr,model);cleanup.state=-1;return self;
}
extern "C" void __fastcall kotor_fastcall_006706e0(void* self,void*) noexcept(false){
    ObstacleUnwind cleanup{self};publish(self,t1_lifecycle::table_0075287c);void* model=rp(self,0x60);cleanup.state=1;
    if(model)scalar_delete(model);unregister_safe(at(self,0x60));cleanup.state=-1;kotor_fastcall_0066c610(self,nullptr);
}
