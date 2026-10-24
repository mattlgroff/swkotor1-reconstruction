#include "9b7fd0ec1b5a-claude_turret_t4_setup.hpp"
#include "c6d94c22e2d8-claude_turret_t4_teardown.hpp"
#include "8c7ada52dac9-gui_rt.hpp"
#include "75d9d34323f7-startup_string.h"
#include <cstdio>
#include <cstring>
namespace menu_resources {using String=kotor_reconstruction::StartupString;}
namespace kotor_reconstruction {struct ArrayMutations;}
namespace aurora_model {struct Gob;}
namespace gui_load {struct ResRef;}
namespace resource_ref_assignment {menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*);}
extern "C" {
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*) noexcept(false);
unsigned __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*,void*);
void __fastcall gui_load_resref_copy_to_string_00405fb0(const gui_load::ResRef*,void*,char*);
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*,void*,menu_resources::String*) noexcept(false);
void __fastcall frame_R4_005d0fc0(void*,void*,kotor_reconstruction::StartupString*,unsigned,unsigned) noexcept(false);
int __cdecl format_006fadb0(char*,const char*,...);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*,const char*,std::FILE*,std::FILE*) noexcept(false);
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations*,void*,unsigned) noexcept(false);
void __fastcall kotor_fastcall_006737e0(void*,void*,const void*) noexcept(false);
void __fastcall kotor_fastcall_006daf60(void*,void*,float) noexcept(false);
void __fastcall kotor_fastcall_006dafa0(void*,void*,float) noexcept(false);
void* __fastcall kotor_fastcall_006dadf0(void*,void*,void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_006db590(void*,void*,void*,void*,void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_0066d260(void*,void*,std::int32_t) noexcept(false);
unsigned __fastcall input_desc_return_true_0063e7a0(void*,void*);
// Owned pending complete callback; no substitute is defined here.
void __cdecl kotor_cdecl_00673a40(void*,void*,void*) noexcept(false);
}
namespace {
using U=unsigned;using I=std::int32_t;using Ref=menu_resources::Ref;
template<class T>T& at(void*p,U n){return *reinterpret_cast<T*>(static_cast<unsigned char*>(p)+n);}
template<class T>T read(const void*p,U n){return *reinterpret_cast<T const volatile*>(static_cast<const unsigned char*>(p)+n);}
template<class F>F slot(void*p,U n){return reinterpret_cast<F>(read<void**>(p,0)[n/4]);}
U bits(float f){U u;std::memcpy(&u,&f,4);return u;}
I previous(I n){U u;std::memcpy(&u,&n,4);--u;std::memcpy(&n,&u,4);return n;}
using GetBehavior=void*(__fastcall*)(void*,void*,U);
using GetType=U(__fastcall*)(void*,void*);
using SetBehavior=U(__fastcall*)(void*,void*,void*);
using AttachPart=U(__fastcall*)(void*,void*,void*,void*,U);
using Animate=U(__fastcall*)(void*,void*,const char*,float,U,U);
using Callback=void(__cdecl*)(void*,void*,void*);
using Register=U(__fastcall*)(void*,void*,Callback,const char*,void*,U,U);
using Delete=void*(__fastcall*)(void*,void*,U);
using GetScript=Ref*(__fastcall*)(void*,void*,Ref*,I);
using AsPlayer=void*(__fastcall*)(void*,void*);
const KotorT4BankSlots targeting_table={kotor_fastcall_00674530,kotor_fastcall_00674a10,input_desc_return_true_0063e7a0};
const char target_error[]="CSWMGGunBank: Bullet Target Type (%d) is invalid";
const double zero64=0.0,minus_one64=-1.0;
struct Allocation{void*p;~Allocation()noexcept(false){if(p)live_free(p);}};
struct MemberList{void*self;bool armed;~MemberList()noexcept(false){if(armed){live_free(read<void*>(self,0x34));at<void*>(self,0x34)=nullptr;}}};
void assign(Ref*dst,const Ref*src){Ref ignored;resource_ref_assignment::assign_00406120(dst,nullptr,&ignored,src);}
void copy_word(void*dst,const void*src){U word;std::memcpy(&word,src,4);std::memcpy(dst,&word,4);}
__declspec(noinline) void copy_float(float*dst,const float*src){
 __asm {
  mov ecx,dst
  mov edx,src
  fld dword ptr[edx]
  fstp dword ptr[ecx]
 }
}
// Constructors use FCOM retaining the value and FSTP on accepted branches.
__declspec(noinline) void validate(float*dst,const float*src,U kind){
 __asm {
  mov ecx,dst
  mov edx,src
  fld dword ptr[edx]
  fcom qword ptr[zero64]
  fnstsw ax
  cmp kind,1
  je positive_only
  test ah,1
  je accepted
  cmp kind,2
  jne rejected
  fcom qword ptr[minus_one64]
  fnstsw ax
  test ah,44h
  jp rejected
  jmp accepted
 positive_only:
  test ah,41h
  jne rejected
 accepted:
  fstp dword ptr[ecx]
  jmp done_validate
 rejected:
  fstp st(0)
 done_validate:
 }
}
__declspec(noinline) bool nonnegative(const float*p){unsigned short status;
 __asm {
  mov ecx,p
  fld dword ptr[ecx]
  fcomp qword ptr[zero64]
  fnstsw status
 }
 return !(status&0x100);
}
void ready_register(void*gob,void*bank){
 slot<Register>(gob,0x28)(gob,nullptr,kotor_cdecl_00673a40,"*",bank,0x461c3c00,0);
 slot<Animate>(gob,0x18)(gob,nullptr,"ready",1.0f,0,0);
 gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(static_cast<unsigned char*>(bank)+0x34),nullptr,reinterpret_cast<U>(gob));
}
void* new_model(const Ref*ref){char model[17];gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const gui_load::ResRef*>(ref),nullptr,model);return aurora_new_aur_object_00449cc0(model,"",nullptr,nullptr);}
}
extern "C" U __fastcall kotor_fastcall_00673880(void*self,void*,U value) noexcept(false){
 if(value&0x11111110){char discarded[256];format_006fadb0(discarded,target_error,value);return read<U>(self,0x50);}
 at<U>(self,0x50)=value;return value;
}
extern "C" float __fastcall kotor_fastcall_00673f50(void*self,void*,float value) noexcept(false){
 if(nonnegative(&value)){
  const I count=read<I>(self,0x38);at<U>(self,0x48)=bits(value);
  for(I i=0;i<count;++i){void*gob=read<void**>(self,0x34)[i];void*behavior=slot<GetBehavior>(gob,0x108)(gob,nullptr,~0u);
   U type=slot<GetType>(behavior,0x10)(behavior,nullptr);
   if(type==0xaaad)kotor_fastcall_006daf60(behavior,nullptr,value);else if(type==0xaaaa)kotor_fastcall_006dafa0(behavior,nullptr,value);
  }
 }
 return read<float>(self,0x48);
}
extern "C" void __fastcall kotor_fastcall_006738d0(void*self,void*) noexcept(false){
 Ref script;void*follower=read<void*>(self,0x68);slot<GetScript>(follower,0xc)(follower,nullptr,&script,7); // designated output, EAX unused
 follower=read<void*>(self,0x68);kotor_fastcall_006737e0(follower,nullptr,static_cast<unsigned char*>(self)+0x40);
 if(gui_load_resref_is_valid_00406030(&script,nullptr)){
  kotor_reconstruction::StartupString text;
  c2da_ref_to_string_00405f70(&script,nullptr,&text);
  follower=read<void*>(self,0x68);
  frame_R4_005d0fc0(reinterpret_cast<void*>(frame_control::message_manager_007a3a00),nullptr,&text,read<U>(follower,8),1);
 }
}
extern "C" void* __fastcall kotor_fastcall_006743b0(void*self,void*,void*parent,void*part) noexcept(false){
 auto*model=&at<Ref>(self,0x14);if(!gui_load_resref_is_valid_00406030(model,nullptr)||!parent||!part)return nullptr;
 void*gob=new_model(model);if(!gob)return nullptr;
 void*memory=live_operator_new(0x54);Allocation guard{memory};
 void*behavior=memory?kotor_fastcall_006dadf0(memory,nullptr,gob,parent,part):nullptr;
 guard.p=nullptr;kotor_fastcall_006daf60(behavior,nullptr,read<float>(self,0x48));
 assign(&at<Ref>(behavior,0x3c),&at<Ref>(self,0x24));
 slot<SetBehavior>(gob,0x58)(gob,nullptr,behavior);ready_register(gob,self);return gob;
}
extern "C" void* __fastcall kotor_fastcall_00674530(void*self,void*,void*parent,void*part) noexcept(false){
 auto*model=&at<Ref>(self,0x14);if(!gui_load_resref_is_valid_00406030(model,nullptr)||!parent||!part)return nullptr;
 void*gob=new_model(model);if(!gob)return nullptr;
 void*memory=live_operator_new(0x7c);Allocation guard{memory};void*behavior=nullptr;
 if(memory){void*target=kotor_fastcall_0066d260(read<void*>(self,0x7c),nullptr,0);behavior=kotor_fastcall_006db590(memory,nullptr,gob,parent,part,target);}
 guard.p=nullptr;kotor_fastcall_006dafa0(behavior,nullptr,read<float>(self,0x48));
 for(U o=0;o<16;o+=4)copy_word(static_cast<unsigned char*>(behavior)+0x3c+o,static_cast<unsigned char*>(self)+0x6c+o);
 copy_float(&at<float>(behavior,0x50),&at<float>(read<void*>(self,0x7c),0x84));
 assign(&at<Ref>(behavior,0x64),&at<Ref>(self,0x24));
 void*target=read<void*>(self,0x7c);
 if(slot<AsPlayer>(target,0x18)(target,nullptr)){
  target=read<void*>(self,0x7c);void*player=slot<AsPlayer>(target,0x18)(target,nullptr);
  for(U o=0;o<12;o+=4)copy_word(static_cast<unsigned char*>(behavior)+0x58+o,static_cast<unsigned char*>(player)+0x214+o);
 }
 slot<SetBehavior>(gob,0x58)(gob,nullptr,behavior);ready_register(gob,self);return gob;
}
extern "C" void __fastcall kotor_fastcall_006740d0(void*self,void*,const Ref*model) noexcept(false){
 assign(&at<Ref>(self,0x14),model);I remaining=read<I>(self,0x38);if(remaining<=0)return;
 // Original EDI starts as RHS once, and persists even across unsupported-type iterations.
 void*replacement=const_cast<Ref*>(model);
 do {
  void*old=read<void**>(self,0x34)[0];void*behavior=slot<GetBehavior>(old,0x108)(old,nullptr,~0u);
  if(behavior){U type=slot<GetType>(behavior,0x10)(behavior,nullptr);
   if(type==0xaaaa||type==0xaaad){
    void*target=type==0xaaaa?read<void*>(behavior,0x54):nullptr;
    void*parent=read<void*>(behavior,0x14);void*part=read<void*>(behavior,0x18);
    if(parent&&part){
     replacement=new_model(&at<Ref>(self,0x14));
     U again=slot<GetType>(behavior,0x10)(behavior,nullptr);
     if(again==0xaaad){
      void*memory=live_operator_new(0x54);Allocation guard{memory};
      void*next=memory?kotor_fastcall_006dadf0(memory,nullptr,replacement,parent,part):nullptr;
      float rate;copy_float(&rate,&at<float>(behavior,0x4c));guard.p=nullptr;
      kotor_fastcall_006daf60(next,nullptr,rate);assign(&at<Ref>(next,0x3c),&at<Ref>(behavior,0x3c));
      slot<SetBehavior>(replacement,0x58)(replacement,nullptr,next);
      slot<Register>(replacement,0x28)(replacement,nullptr,kotor_cdecl_00673a40,"*",self,0x461c3c00,0);
     }else if(again==0xaaaa){
      void*memory=live_operator_new(0x7c);Allocation guard{memory};
      void*next=memory?kotor_fastcall_006db590(memory,nullptr,replacement,parent,part,target):nullptr;
      for(U o=0;o<16;o+=4)copy_word(static_cast<unsigned char*>(next)+0x3c+o,static_cast<unsigned char*>(behavior)+0x3c+o);
      copy_float(&at<float>(next,0x50),&at<float>(behavior,0x50));
      for(U o=0;o<12;o+=4)copy_word(static_cast<unsigned char*>(next)+0x58+o,static_cast<unsigned char*>(behavior)+0x58+o);
      float rate;copy_float(&rate,&at<float>(behavior,0x74));guard.p=nullptr;
      kotor_fastcall_006dafa0(next,nullptr,rate);assign(&at<Ref>(next,0x64),&at<Ref>(behavior,0x64));
      slot<SetBehavior>(replacement,0x58)(replacement,nullptr,next);
      slot<Register>(replacement,0x28)(replacement,nullptr,kotor_cdecl_00673a40,"*",self,0x461c3c00,0);
     }
     slot<AttachPart>(replacement,0x54)(replacement,nullptr,parent,part,0);
    }
   }
  }
  slot<Animate>(replacement,0x18)(replacement,nullptr,"ready",1.0f,0,0);
  at<I>(self,0x38)=previous(read<I>(self,0x38));
  for(I i=0;i<read<I>(self,0x38);++i){void**list=read<void**>(self,0x34);list[i]=list[i+1];}
  slot<Delete>(old,0)(old,nullptr,1);
  U capacity=read<U>(self,0x3c);
  if(read<U>(self,0x38)==capacity){
   U next=capacity?capacity+capacity:16;void**old_list=read<void**>(self,0x34);at<U>(self,0x3c)=next;
   at<void*>(self,0x34)=live_operator_new(next*4u);
   for(I i=0;i<read<I>(self,0x38);++i)read<void**>(self,0x34)[i]=old_list[i];
   if(old_list)live_free(old_list);
  }
  read<void**>(self,0x34)[read<U>(self,0x38)]=replacement;at<U>(self,0x38)=read<U>(self,0x38)+1;
  remaining=previous(remaining);
 }while(remaining!=0);
}
extern "C" void* __fastcall kotor_fastcall_006746d0(void*self,void*,void*follower,const Ref*name,const Ref*model,const void*bullet) noexcept(false){
 at<const KotorT4BankSlots*>(self,0)=&kotor_t4_bank_table_00752ae4;
 c2da_default_ref_00405ed0(&at<Ref>(self,4),nullptr);c2da_default_ref_00405ed0(&at<Ref>(self,0x14),nullptr);c2da_default_ref_00405ed0(&at<Ref>(self,0x24),nullptr);
 at<U>(self,0x38)=0;at<U>(self,0x3c)=0;at<void*>(self,0x34)=nullptr;MemberList cleanup{self,true};
 c2da_default_ref_00405ed0(&at<Ref>(self,0x58),nullptr);
 at<U>(self,0x40)=0;at<U>(self,0x50)=0;at<U>(self,0x44)=0;at<U>(self,0x48)=0;at<U>(self,0x4c)=0;
 assign(&at<Ref>(self,4),name);kotor_fastcall_006740d0(self,nullptr,model);at<U>(self,0x40)=read<U>(bullet,0);
 validate(&at<float>(self,0x44),static_cast<const float*>(bullet)+1,0);
 kotor_fastcall_00673f50(self,nullptr,read<float>(bullet,8));
 validate(&at<float>(self,0x4c),static_cast<const float*>(bullet)+3,1);
 U target=read<U>(bullet,0x10);if(target&0x11111110){char ignored[256];format_006fadb0(ignored,target_error,target);}else at<U>(self,0x50)=target;
 assign(&at<Ref>(self,0x58),reinterpret_cast<const Ref*>(static_cast<const unsigned char*>(bullet)+0x18));at<void*>(self,0x68)=follower;cleanup.armed=false;return self;
}
extern "C" void* __fastcall kotor_fastcall_00674860(void*self,void*,void*follower,const Ref*name,const Ref*model,const void*bullet,const void*params,void*target) noexcept(false){
 kotor_fastcall_006746d0(self,nullptr,follower,name,model,bullet);at<const KotorT4BankSlots*>(self,0)=&targeting_table;
 at<U>(self,0x6c)=0;at<U>(self,0x74)=0;at<U>(self,0x70)=0;at<U>(self,0x78)=0;
 validate(&at<float>(self,0x70),static_cast<const float*>(params)+1,0);
 validate(&at<float>(self,0x6c),static_cast<const float*>(params),2);
 validate(&at<float>(self,0x74),static_cast<const float*>(params)+2,2);
 validate(&at<float>(self,0x78),static_cast<const float*>(params)+3,0);
 at<void*>(self,0x7c)=target;return self;
}

