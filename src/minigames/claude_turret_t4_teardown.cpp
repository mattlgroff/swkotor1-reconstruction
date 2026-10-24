#include "c6d94c22e2d8-claude_turret_t4_teardown.hpp"
#include <cstdint>
#include <cstring>
extern "C" {
void* __cdecl live_operator_new(unsigned);
void __cdecl live_free(void*);
void* __fastcall behavior_attach_0049fb10(void*,void*,void*,void*,void*) noexcept(false);
unsigned __fastcall behavior_return_zero_0063e7f0(void*,void*);
void __fastcall kotor_fastcall_0066e110(void*,void*,unsigned);
// Owned pending complete AddGun, never a placeholder. Candidate adoption waits for it.
void* __fastcall kotor_fastcall_006743b0(void*,void*,void*,void*);
}
// Sole complete table shared with the original bank constructor in the next batch.
extern "C" const KotorT4BankSlots kotor_t4_bank_table_00752ae4={kotor_fastcall_006743b0,kotor_fastcall_00674990,behavior_return_zero_0063e7f0};
namespace {
using U=std::uint32_t; using I=std::int32_t;
template<class T>T& field(void* p,U o){return *reinterpret_cast<T*>(static_cast<unsigned char*>(p)+o);}
template<class T>T load(void* p,U o){return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+o);}
template<class F>F slot(void* p,U o){return reinterpret_cast<F>(load<void**>(p,0)[o/4]);}
using Delete=void*(__fastcall*)(void*,void*,U);
using Detach=void*(__fastcall*)(void*,void*,void*,const char*,I);
using Animate=U(__fastcall*)(void*,void*,const char*,float,U,U);
using Callback=void(__cdecl*)(void*,void*,void*);
using Register=U(__fastcall*)(void*,void*,Callback,const char*,void*,U,U);
using GetBehavior=void*(__fastcall*)(void*,void*,U);
using SetBehavior=U(__fastcall*)(void*,void*,void*);
I previous(I n){U u;std::memcpy(&u,&n,4);--u;std::memcpy(&n,&u,4);return n;}
void* gun_at(void* self,I i){return load<void**>(self,0x34)[i];}
void detach(void* gob){slot<Detach>(gob,0x50)(gob,nullptr,nullptr,nullptr,0);}
void destroy(void* gob){slot<Delete>(gob,0)(gob,nullptr,1);}
void member_cleanup(void* self){live_free(load<void*>(self,0x34));field<void*>(self,0x34)=nullptr;}
// Original member-list unwind 0072DB60 -> 0056B6F0.
struct MemberGuard {void* self;bool armed;~MemberGuard() noexcept(false){if(armed)member_cleanup(self);}};
struct AllocationGuard {void* allocation;~AllocationGuard() noexcept(false){if(allocation)live_free(allocation);}};
void die_animation(void* self,void* gob){
 slot<Animate>(gob,0x18)(gob,nullptr,nullptr,1.0f,0,0);
 if(slot<Animate>(gob,0x18)(gob,nullptr,"die",1.0f,1,0))
  slot<Register>(gob,0x28)(gob,nullptr,kotor_cdecl_00674930,"donedie",self,0x461c3c00u,0);
 else {kotor_fastcall_00674020(self,nullptr,gob);destroy(gob);}
}
}
extern "C" void __fastcall kotor_fastcall_00673fd0(void* self,void*) noexcept(false){
 const I count=load<I>(self,0x38);
 for(I i=0;i<count;++i){void* gob=gun_at(self,i);
  slot<Animate>(gob,0x18)(gob,nullptr,"damage",1.0f,1,0);
  slot<Animate>(gob,0x18)(gob,nullptr,"ready",1.0f,0x20,0);}
}
extern "C" void* __fastcall kotor_fastcall_00674020(void* self,void*,void* gob) noexcept(false){
 const I count=load<I>(self,0x38);I index=0;
 for(;index<count;++index)if(gun_at(self,index)==gob)break;
 if(count<=0||index==count)return nullptr;
 detach(gob);
 field<I>(self,0x38)=previous(load<I>(self,0x38));
 while(index<load<I>(self,0x38)){
  void** list=load<void**>(self,0x34);list[index]=list[index+1];++index;
 }
 return gob;
}
extern "C" int __fastcall kotor_fastcall_006744d0(void* self,void*) noexcept(false){
 const I count=load<I>(self,0x38);
 for(I i=previous(count);i>=0;i=previous(i)){
  detach(gun_at(self,i));void* gob=gun_at(self,i);if(gob)destroy(gob);
 }
 void* list=load<void*>(self,0x34);
 if(list){live_free(list);field<void*>(self,0x34)=nullptr;field<U>(self,0x3c)=0;}
 field<U>(self,0x38)=0;return count;
}
extern "C" void __fastcall kotor_fastcall_00674800(void* self,void*) noexcept(false){
 field<const KotorT4BankSlots*>(self,0)=&kotor_t4_bank_table_00752ae4;
 MemberGuard cleanup{self,true};kotor_fastcall_006744d0(self,nullptr);
 cleanup.armed=false;member_cleanup(self);
}
extern "C" void __cdecl kotor_cdecl_00674930(void* gob,void*,void* bank) noexcept(false){
 if(!bank){if(gob)destroy(gob);return;}
 kotor_fastcall_00674020(bank,nullptr,gob);if(gob)destroy(gob);
 if(load<I>(bank,0x38)==0){
  void* follower=load<void*>(bank,0x68);
  if(follower)kotor_fastcall_0066e110(follower,nullptr,reinterpret_cast<U>(bank));
  kotor_fastcall_00674800(bank,nullptr);live_free(bank);
 }
}
extern "C" void __fastcall kotor_fastcall_00674990(void* self,void*) noexcept(false){
 for(I i=previous(load<I>(self,0x38));i>=0;i=previous(i))die_animation(self,gun_at(self,i));
}
extern "C" void __fastcall kotor_fastcall_00674a10(void* self,void*) noexcept(false){
 for(I i=previous(load<I>(self,0x38));i>=0;i=previous(i)){
  void* gob=gun_at(self,i);void* behavior=slot<GetBehavior>(gob,0x108)(gob,nullptr,0xaaaa);
  if(!behavior||!load<void*>(behavior,0x14)||!load<void*>(behavior,0x18)){
   kotor_fastcall_00674020(self,nullptr,gob);destroy(gob);continue;
  }
  void* allocation=live_operator_new(0x3c);AllocationGuard cleanup{allocation};
  void* attached=allocation?behavior_attach_0049fb10(allocation,nullptr,gob,load<void*>(behavior,0x14),load<void*>(behavior,0x18)):nullptr;
  cleanup.allocation=nullptr; // Original EH state cleared BEFORE SetBehavior.
  slot<SetBehavior>(gob,0x58)(gob,nullptr,attached);
  die_animation(self,gob);
 }
}


