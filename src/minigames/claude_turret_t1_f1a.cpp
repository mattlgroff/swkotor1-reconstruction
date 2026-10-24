// t1-A1 tier f1a: 0066E130 0066D550 0066E1F0 0066D2F0 0066E6B0 (swkotor.exe GOG 1.03, static disassembly only).
// Plan: t1-review/F14-PLAN-R3-SNAPSHOT.md (SHA 73AED3EE...), review F14-PLAN-REVIEW.md; root ADMISSIONS-20261022 R4.
#include "4a3c1e98bb80-claude_turret_t1_f1a.hpp"
#include "97980543594d-claude_turret_t1_lifecycle.hpp"  // shared t1 contract (d3): identical E130/D550/E1F0 declarations
#include "aafac7614c0f-claude_turret_t1_events.hpp"     // t1 e3 owners: 0066C0A0, 0066C2D0, 0066E5C0
#include "e999d224bc23-m15_l0.h"                         // 005D59E0 CExoSoundSource::SetPosition (selected 0812)
#include "322371ce1803-m46_cells_storage.h"              // 007B93A8 CHitInfo storage (selected 0537)
#include <cstring>
// 004C4F50 CExoArrayList<8-byte>::Allocate(int capacity) RET 4, selected 0288-lowers-directory-list.obj
// ?AllocateTimes004c4f50@directory_list@@YIXPAUTimeList@1@PAXH@Z. Verbatim declaration from canonical
// m60-compose/s/resource-lowers/directory_list.h; TimeList ({data,count,capacity}, 12 bytes) is only forward
// declared because that header's f200/INI include closure is not composable in this unit. No layout is redefined.
namespace directory_list{struct TimeList;void __fastcall AllocateTimes004c4f50(TimeList* self,void*,std::int32_t capacity);}
static_assert(sizeof(void*)==4,"KOTOR x86");
namespace {
using U=std::uint32_t;using I=std::int32_t;
unsigned char* at(void*p,U n){return static_cast<unsigned char*>(p)+n;}
U rd(void*p,U n){U v;std::memcpy(&v,at(p,n),4);return v;}
I ri(void*p,U n){I v;std::memcpy(&v,at(p,n),4);return v;}
void* rp(void*p,U n){void*v;std::memcpy(&v,at(p,n),4);return v;}
void wr(void*p,U n,U v){std::memcpy(at(p,n),&v,4);}
void wp(void*p,U n,void*v){std::memcpy(at(p,n),&v,4);}
// Virtual slots: the receiver's vtable is reloaded at every call (original mov reg,[obj]; call [reg+slot]).
template<class F>F slot(void*p,U off){F f;std::memcpy(&f,at(rp(p,0),off),4);return f;}
using Callback=void(__cdecl*)(void*,const char*,void*);
using SelfCast=void*(__fastcall*)(void*,void*);                       // follower +18, RET 0
using DeletingDestructor=U(__fastcall*)(void*,void*,U);               // Gob +00 (0044B9E0), RET 4
using PlayAnimation=U(__fastcall*)(void*,void*,const char*,U,U,U);    // Gob +18 (00485BD0), RET 10h
using RegisterCallback=U(__fastcall*)(void*,void*,Callback,const char*,void*,U,U); // Gob +28 (00484E40), RET 14h
using AttachToObject=void*(__fastcall*)(void*,void*,void*,const char*,I); // Gob +50 (00443820), RET 0Ch
using SetPosition=void*(__fastcall*)(void*,void*,void*,U,U,U);        // Gob +5C (0043F0C0) hidden out + Vector by value, RET 10h
using GetVector=U(__fastcall*)(void*,void*,void*);                    // Gob +64 (00449980) / +6C (004499D0), RET 4, EAX = out
using HitCheckPart=U(__fastcall*)(void*,void*,void*,U,U,U,U,U,U,U,void*); // Gob +88 (0043E930), RET 24h
using GetPart=void*(__fastcall*)(void*,void*,const char*);            // Gob +10C (004402F0), RET 4
using PartGetWorld=U(__fastcall*)(void*,void*,void*,void*);           // Part +0C (0043E4A0), RET 8, EAX ignored
void* cast18(void*self){return slot<SelfCast>(self,0x18)(self,nullptr);}
void attach(void*model,void*parent,I mode){(void)slot<AttachToObject>(model,0x50)(model,nullptr,parent,"modelhook",mode);}
// Follower array +68 record (model pointer, attach byte). Only byte +4 is assigned: the original stores the low
// byte of arg2 into an otherwise uninitialized stack DWORD (0066E21C/0066E226) and copies the whole DWORD
// (0066E24B/0066E252). Padding stays unspecified, never zeroed or invented; it is copied, never interpreted.
struct ModelEntry{void* model;unsigned char attach;};
static_assert(sizeof(ModelEntry)==8,"original 8-byte follower model record");
// 0066E140..0066E172: FLD [+9C]; FCOMP +0.0; FNSTSW; TEST AH,41h; JNE (skip unless ordered > 0); FLD [+9C];
// FSUB delta (stays in ST0 at the run-time precision control); FCOM +0.0; TEST AH,41h; nonpositive/unordered:
// FSTP ST0, FLD +0.0; single FSTP dword [+9C]. No call, no control-word change.
const float kZero=0.0f;  // bit pattern 00000000, as original .rdata 0073D700
__declspec(noinline) void countdown_9c(float* field,float delta,const float* zero){
    __asm {
        mov ecx, field
        mov edx, zero
        fld dword ptr [ecx]
        fcomp dword ptr [edx]
        fnstsw ax
        test ah, 0x41
        jne skip
        fld dword ptr [ecx]
        fsub delta
        fcom dword ptr [edx]
        fnstsw ax
        test ah, 0x41
        je keep
        fstp st(0)
        fld dword ptr [edx]
    keep:
        fstp dword ptr [ecx]
    skip:
    }
}
}

// 0066E130 CSWTrackFollower::Update.
extern "C" void __fastcall kotor_fastcall_0066e130(void*self,void*,float delta) noexcept(false){
    kotor_fastcall_0066c0a0(self,nullptr,delta);                     // 0066E13B base update first
    countdown_9c(reinterpret_cast<float*>(at(self,0x9c)),delta,&kZero);
    if(!rp(self,0x144))return;                                        // 0066E178
    U position[3]={0,0,0};                                            // 0066E187..0066E197
    if(rd(self,0x6c)!=0){                                             // 0066E182 count != 0 (not > 0)
        void* model=rp(rp(self,0x68),0);
        U out[3];
        const U returned=slot<GetVector>(model,0x64)(model,nullptr,out); // 0066E1AD
        std::memcpy(position,reinterpret_cast<void*>(static_cast<std::uintptr_t>(returned)),12); // 0066E1B0..B5 via EAX
    }
    kotor_m15_sound_source_set_position_005d59e0(rp(self,0x144),nullptr,position,0.0f); // 0066E1D1 fresh +144, push 0
}

// 0066D550 CSWTrackFollower::PlayAnimation.
extern "C" void __fastcall kotor_fastcall_0066d550(void*self,void*,const char*name,float speed,U flags) noexcept(false){
    U speed_bits;std::memcpy(&speed_bits,&speed,4);                   // raw stack word, never loaded to x87
    const I count=ri(self,0x6c);                                      // 0066D55A cached
    for(I i=0;i<count;++i){                                           // signed JLE/JL
        void* model=rp(rp(self,0x68),static_cast<U>(i)*8);            // 0066D563 array reread
        (void)slot<PlayAnimation>(model,0x18)(model,nullptr,name,speed_bits,flags,0);
    }
    if(!cast18(self))return;                                          // 0066D584
    if(!rp(cast18(self),0x220))return;                                // 0066D58F, cast result +220
    void* extra=rp(cast18(self),0x220);                               // 0066D5A0 third fresh cast
    (void)slot<PlayAnimation>(extra,0x18)(extra,nullptr,name,speed_bits,flags,0);
}

// 0066E1F0 CSWTrackFollower::AddModel.
extern "C" void* __fastcall kotor_fastcall_0066e1f0(void*self,void*,void*model,U flag) noexcept(false){
    if(!model)return nullptr;                                         // 0066E1FD, EAX 0
    const I count=ri(self,0x6c);
    if(count>0){void* data=rp(self,0x68);for(I i=0;i<count;++i)if(rp(data,static_cast<U>(i)*8)==model)return model;}
    ModelEntry entry;                                                 // stack DWORD at [esp+10h]; only low byte set
    entry.model=model;
    entry.attach=static_cast<unsigned char>(flag);                    // 0066E21C/0066E226
    const I capacity=ri(self,0x70);
    if(ri(self,0x6c)==capacity)                                       // 0066E22D equality only
        directory_list::AllocateTimes004c4f50(reinterpret_cast<directory_list::TimeList*>(at(self,0x68)),nullptr,
            capacity?static_cast<I>(static_cast<U>(capacity)*2u):16); // 0066E235 ADD EAX,EAX / 0066E239 10h
    const U index=rd(self,0x6c);                                      // 0066E246 count reread
    std::memcpy(at(rp(self,0x68),index*8),&entry,8);                  // 0066E24F model, 0066E252 whole second DWORD
    wr(self,0x6c,rd(self,0x6c)+1);                                    // 0066E256 INC [esi+4]
    void* track=rp(self,0x64);
    if(track)attach(model,track,0);                                   // 0066E260..0066E26C
    return model;
}

// 0066D2F0 CSWTrackFollower::SetTrack (true entry; 0066E280 jumps here when the track differs).
extern "C" void* __fastcall kotor_fastcall_0066d2f0(void*self,void*,void*track,U attachment) noexcept(false){
    void* old=rp(self,0x64);
    if(old&&old!=track)(void)slot<DeletingDestructor>(old,0)(old,nullptr,1); // 0066D30C
    const U flags=rd(self,0x88)&0xfffffffeu;                          // 0066D30E read after the destructor
    wp(self,0x64,track);wr(self,0x88,flags);                          // 0066D319, 0066D31C
    if(!track)return rp(self,0x64);
    void* hook=slot<GetPart>(track,0x10c)(track,nullptr,"modelhook"); // 0066D331, unchecked as original
    U position[3]={0,0,0};                                            // [esp+10h..18h]
    U orientation[4]={0x3f800000u,0,0,0};                             // [esp+28h..34h] Quaternion{w=1,x,y,z}
    (void)slot<PartGetWorld>(hook,0xc)(hook,nullptr,position,orientation); // 0066D37D (position, orientation)
    if(cast18(self)){                                                 // 0066D384
        void* current=rp(self,0x64);
        (void)slot<RegisterCallback>(current,0x28)(current,nullptr,&kotor_cdecl_0066c6a0,"starttrack",self,
            0x461C3C00u,0);                                           // 0066D390..0066D3A2 raw 9999.0f
    }
    const I count=ri(self,0x6c);                                      // 0066D3A5 cached
    for(I i=0;i<count;++i){
        unsigned char* entry=at(rp(self,0x68),static_cast<U>(i)*8);   // 0066D3B0 array reread
        void* model=rp(entry,0);
        const unsigned char attach_byte=*(entry+4);                   // 0066D3B6 BYTE consumer
        attach(model,rp(self,0x64),attach_byte?static_cast<I>(attachment):0);
        U scratch[3];                                                 // hidden output [esp+1Ch], never read
        (void)slot<SetPosition>(model,0x5c)(model,nullptr,scratch,position[0],position[1],position[2]); // 0066D402
    }
    if(cast18(self)){                                                 // 0066D40E
        void* extra=rp(self,0x220);                                   // 0066D415 follower +220 (not the cast result)
        if(extra)attach(extra,rp(self,0x64),static_cast<I>(attachment));
    }
    return rp(self,0x64);                                             // 0066D432 fresh
}

// 0066E6B0 CSWTrackFollower::DoObstacleHitCheck.
extern "C" std::int32_t __fastcall kotor_fastcall_0066e6b0(void*self,void*,void*obstacle) noexcept(false){
    void* gob=rp(obstacle,0x60);
    if(!gob)return 0;
    void* part=slot<GetPart>(gob,0x10c)(gob,nullptr,"hitbump");       // 0066E6D1
    if(!part)return 0;
    if(rd(self,0x6c)==0)return 0;                                     // 0066E6E4 count != 0
    void* model=rp(rp(self,0x68),0);
    U previous[3];U current[3];
    (void)slot<GetVector>(model,0x6c)(model,nullptr,previous);        // 0066E6FA local [esp+1Ch]
    (void)slot<GetVector>(model,0x64)(model,nullptr,current);         // 0066E706 local [esp+10h]
    if(!slot<HitCheckPart>(gob,0x88)(gob,nullptr,part,previous[0],previous[1],previous[2],
            current[0],current[1],current[2],0,&scene_hit_info_007b93a8)) // 0066E747
        return 0;
    wr(self,0x170,rd(obstacle,8));                                    // 0066E75C before OnHitObstacle
    kotor_fastcall_0066e5c0(self,nullptr,0);                          // 0066E762
    wr(obstacle,0x50,rd(self,8));                                     // 0066E767/0066E76E fresh self ID
    kotor_fastcall_0066c2d0(obstacle,nullptr,0);                      // 0066E771
    return 1;
}
