#include "aafac7614c0f-claude_turret_t1_events.hpp"
#include "d8f703239b61-resources.h"
#include "e999d224bc23-m15_l0.h"
#include <cstring>
#include <new>
static_assert(sizeof(void*)==4,"KOTOR x86");
namespace frame_control {extern std::uint32_t message_manager_007a3a00;}
namespace kotor_item_drop_sounds {void* __fastcall ref_default_element_00405ed0(void*,void*);}
namespace resource_ref_assignment {menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*);}
extern "C" {
extern void* kotor_cell_sound_007a39ec;
unsigned __fastcall gui_load_resref_is_valid_00406030(void*,void*) noexcept(false);
void* __fastcall c2da_ref_to_string_00405f70(void*,void*,void*) noexcept(false);
void __fastcall frame_R4_005d0fc0(void*,void*,unsigned*,unsigned,unsigned) noexcept(false);
void* __fastcall gui_load_slot_self_00641db0(void*,void*) noexcept(false);
int __fastcall c2da_equal_text_004060d0(void*,void*,const char*) noexcept(false);
unsigned __fastcall w5_sound_play_one_shot_005d5e00(void*,void*,void*,unsigned,unsigned,unsigned,float,float) noexcept(false);
void __fastcall kotor_m15_sound_source_stop_005d5a20(void*,void*) noexcept(false);
void __fastcall kotor_fastcall_00673fd0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066d280(void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_006717f0(void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_006715b0(void*,void*,void*) noexcept(false);
}
namespace {
using U=std::uint32_t;using I=std::int32_t;using Ref=menu_resources::Ref;using String=kotor_reconstruction::StartupString;
unsigned char* at(void*p,U n){return static_cast<unsigned char*>(p)+n;}
U rd(void*p,U n){U v;std::memcpy(&v,at(p,n),4);return v;}
I ri(void*p,U n){I v;std::memcpy(&v,at(p,n),4);return v;}
I signed_bits(U v){I i;std::memcpy(&i,&v,4);return i;}
void wr(void*p,U n,U v){std::memcpy(at(p,n),&v,4);}
void* rp(void*p,U n){void*v;std::memcpy(&v,at(p,n),4);return v;}
float float_bits(U v){float f;std::memcpy(&f,&v,4);return f;}
template<class F>F slot(void*p,U off){F f;std::memcpy(&f,at(rp(p,0),off),4);return f;}
void* cast(void*p,U off){return slot<void*(__fastcall*)(void*,void*)>(p,off)(p,nullptr);}
Ref* script(void*self,Ref*out,I index){return slot<Ref*(__fastcall*)(void*,void*,Ref*,I)>(self,0xc)(self,nullptr,out,index);}
bool valid(Ref*ref){return gui_load_resref_is_valid_00406030(ref,nullptr)!=0;}
struct StringCleanup{String* p;bool armed;~StringCleanup() noexcept(false){if(armed)p->~StartupString();}};
void run(void*self,Ref*ref){
    alignas(String) unsigned char storage[sizeof(String)];String* text=::new(storage) String;StringCleanup cleanup{text,true};
    c2da_ref_to_string_00405f70(ref,nullptr,text);
    const U id=rd(self,8);void* vm=reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00));
    frame_R4_005d0fc0(vm,nullptr,reinterpret_cast<unsigned*>(text),id,1);
    cleanup.armed=false;text->~StartupString();
}
bool override_script(void*self,U flag,I index){if(flag)return false;Ref ref;script(self,&ref,index);if(!valid(&ref))return false;run(self,&ref);return true;}
void own_animation(void*self,const char*name,float speed,U flags){slot<void(__fastcall*)(void*,void*,const char*,float,U)>(self,0)(self,nullptr,name,speed,flags);}
U model_animation(void*model,const char*name,float speed,U flags){return slot<U(__fastcall*)(void*,void*,const char*,float,U,U)>(model,0x18)(model,nullptr,name,speed,flags,0);}
void scalar(void*obj,U off=0){(void)slot<U(__fastcall*)(void*,void*,U)>(obj,off)(obj,nullptr,1);}
}
extern "C" void __fastcall kotor_fastcall_0066c0a0(void*self,void*,float) noexcept(false){Ref ref;script(self,&ref,4);if(valid(&ref))run(self,&ref);}
extern "C" void __fastcall kotor_fastcall_0066c2d0(void*self,void*,U flag) noexcept(false){(void)override_script(self,flag,2);}
extern "C" void __fastcall kotor_fastcall_0066c370(void*self,void*) noexcept(false){
    Ref ref;kotor_item_drop_sounds::ref_default_element_00405ed0(&ref,nullptr);
    if(cast(self,0x14)){Ref returned,discarded;Ref* source=script(self,&returned,0);resource_ref_assignment::assign_00406120(&ref,nullptr,&discarded,source);if(valid(&ref))run(self,&ref);}
}
extern "C" void __fastcall kotor_fastcall_0066c190(void*self,void*,U flag) noexcept(false){
    if(override_script(self,flag,1))return;
    void* follower=cast(self,0x14);if(follower){U mask=slot<U(__fastcall*)(void*,void*)>(follower,0x3c)(follower,nullptr);if(rd(self,0x2c)&mask)kotor_fastcall_0066e8e0(follower,nullptr,signed_bits(0u-rd(self,0x1c)));}
    auto* source=reinterpret_cast<Ref*>(at(self,0x34));if(valid(source)){
        const U group=cast(self,0x18)?0x17u:0x18u;
        const char* chars=static_cast<const char*>(gui_load_slot_self_00641db0(source,nullptr));Ref temporary;temporary.construct_00406d80(chars);
        KotorM15VectorWords position;std::memcpy(&position,at(self,0x54),12);
        (void)kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec,nullptr,&temporary,position,0.0f,group,0,0,0.0f,0.0f);
    }
}
extern "C" void __fastcall kotor_fastcall_0066d440(void*self,void*) noexcept(false){
    void* track=rp(self,0x64);if(!track||(rd(self,0x88)&1u)==0)return;
    (void)model_animation(track,"track",0.0f,(rd(self,0x80)==1?0u:3u)+0x11u);
    wr(self,0x88,rd(self,0x88)&0xfffffffeu);void* sound=rp(self,0x144);if(sound)kotor_m15_sound_source_stop_005d5a20(sound,nullptr);
}
extern "C" void __fastcall kotor_fastcall_0066d490(void*self,void*) noexcept(false){
    own_animation(self,"damage",1.0f,1);own_animation(self,"Ready_01",1.0f,0x20);
    const I count=ri(self,0x78);for(I i=0;i<count;++i)kotor_fastcall_00673fd0(rp(rp(self,0x74),static_cast<U>(i)*4),nullptr);
}
extern "C" void __cdecl kotor_cdecl_0066de50(void*model,const char*,void*self) noexcept(false){
    if(!self){if(model)scalar(model);return;}
    kotor_fastcall_0066d280(self,nullptr,model);if(model)scalar(model);
    if(rd(self,0x6c)!=0)return;
    const bool dying=(rd(self,0x88)&2)!=0;void* mini=rp(self,0x60);
    if(dying){const I count=ri(self,0x78);for(I i=0;i<count;++i)wr(rp(rp(self,0x74),static_cast<U>(i)*4),0x68,0);}
    if(rp(mini,0x24)==self){kotor_fastcall_006717f0(mini,nullptr,nullptr);return;}
    kotor_fastcall_006715b0(mini,nullptr,self);scalar(self,0x28);
}
extern "C" void __fastcall kotor_fastcall_0066e2a0(void*self,void*,U flag) noexcept(false){
    if(override_script(self,flag,6))return;
    kotor_fastcall_0066d440(self,nullptr);
    Ref sound;for(U n=0;n<16;n+=4){U word=rd(self,0x148+n);std::memcpy(sound.bytes+n,&word,4);}
    if(valid(&sound)){const U group=cast(self,0x18)?0x17u:0x18u;const U volume=c2da_equal_text_004060d0(&sound,nullptr,"mgs_accelpad")?127u:100u;(void)w5_sound_play_one_shot_005d5e00(kotor_cell_sound_007a39ec,nullptr,&sound,group,0,volume,0.0f,0.0f);}
    for(I i=signed_bits(rd(self,0x6c)-1u);i>=0;--i){void* model=rp(rp(self,0x68),static_cast<U>(i)*8);(void)model_animation(model,nullptr,1.0f,0);
        if(model_animation(model,"die",1.0f,1)){
            using Callback=void(__cdecl*)(void*,const char*,void*);using Register=void(__fastcall*)(void*,void*,Callback,const char*,void*,float,U);
            slot<Register>(model,0x28)(model,nullptr,&kotor_cdecl_0066de50,"donedie",self,float_bits(0x461c3c00u),0);
        }else{kotor_fastcall_0066d280(self,nullptr,model);scalar(model);}
    }
    const I guns=ri(self,0x78);for(I i=0;i<guns;++i){void* gun=rp(rp(self,0x74),static_cast<U>(i)*4);slot<void(__fastcall*)(void*,void*)>(gun,4)(gun,nullptr);}
    if(rd(self,0x6c)==0){void* mini=rp(self,0x60);if(rp(mini,0x24)==self)kotor_fastcall_006717f0(mini,nullptr,nullptr);else *at(self,0xc)=1;}
    kotor_fastcall_006715b0(rp(self,0x60),nullptr,self);
}
extern "C" void __fastcall kotor_fastcall_0066e5c0(void*self,void*,U flag) noexcept(false){
    if(override_script(self,flag,8))return;const I count=ri(self,0x6c);
    for(I i=0;i<count;++i){void* model=rp(rp(self,0x68),static_cast<U>(i)*8);slot<void(__fastcall*)(void*,void*,void*)>(model,0x58)(model,nullptr,nullptr);}
    const U flags=rd(self,0x88);if((flags&2)==0){wr(self,0x88,flags|2);kotor_fastcall_0066e2a0(self,nullptr,0);}
}
extern "C" void __fastcall kotor_fastcall_0066e7a0(void*self,void*,U flag) noexcept(false){
    if(override_script(self,flag,5))return;
    const U delta=rd(self,0x19c);const U sum=rd(self,0x8c)+delta;wr(self,0x8c,sum);
    if(signed_bits(delta)<0){if(signed_bits(sum)<=0){const U flags=rd(self,0x88);if((flags&2)==0){wr(self,0x88,flags|2);kotor_fastcall_0066e2a0(self,nullptr,0);}}else{own_animation(self,"damage",1.0f,1);own_animation(self,"Ready_01",1.0f,0x20);}}
    else{const U max=rd(self,0x90);wr(self,0x8c,signed_bits(sum)<signed_bits(max)?sum:max);}
}
extern "C" void __fastcall kotor_fastcall_0066e8e0(void*self,void*,I delta) noexcept(false){U bits;std::memcpy(&bits,&delta,4);wr(self,0x19c,bits);kotor_fastcall_0066e7a0(self,nullptr,0);}
