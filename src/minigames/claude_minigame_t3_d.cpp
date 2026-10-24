#include "3d80a7486eda-claude_minigame_t3_d.hpp"
#include "d8f703239b61-resources.h"
#include <cstdint>
#include <cstring>
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned) noexcept(false); }
extern "C" {
void* __cdecl live_operator_new(unsigned) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*) noexcept(false);
int __fastcall resource_equal_004060b0(menu_resources::Ref*,void*,menu_resources::Ref*) noexcept(false);
menu_resources::Ref* __fastcall ref_assign_copy_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*) noexcept(false);
}
namespace {
using U=std::uint32_t;using I=std::int32_t;
static_assert(sizeof(void*)==4,"x86 only");
template<class T>T rd(const void*p,U n){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+n,sizeof v);return v;}
template<class T>void wr(void*p,U n,T v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,sizeof v);}
void* plus(void*p,U n){return reinterpret_cast<void*>(reinterpret_cast<U>(p)+n);}
I signed_bits(U u){I i;std::memcpy(&i,&u,4);return i;}
void grow(void*l,U cap){menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(l),nullptr,cap);}
}
extern "C" void* __fastcall kotor_fastcall_006720c0(void*self,void*,const void*name,const void*position) noexcept(false){
 const I initial=rd<I>(self,0x3c);
 for(I i=0;i<initial;++i){void*node=rd<void*>(rd<void*>(self,0x38),U(i)*4);if(resource_equal_004060b0(static_cast<menu_resources::Ref*>(node),nullptr,const_cast<menu_resources::Ref*>(static_cast<const menu_resources::Ref*>(name))))return node;}
 void*node=live_operator_new(0x1c);
 try {if(node){c2da_default_ref_00405ed0(static_cast<menu_resources::Ref*>(node),nullptr);wr<U>(node,0x10,0);wr<U>(node,0x14,0);wr<U>(node,0x18,0);}}
 catch(...){live_free(node);throw;}
 menu_resources::Ref scratch;
 ref_assign_copy_00406120(static_cast<menu_resources::Ref*>(node),nullptr,&scratch,static_cast<const menu_resources::Ref*>(name));
 wr<U>(node,0x10,rd<U>(position,0));wr<U>(node,0x14,rd<U>(position,4));wr<U>(node,0x18,rd<U>(position,8));
 const U capacity=rd<U>(self,0x40);
 if(rd<U>(self,0x3c)==capacity){
  const U next=capacity?capacity+capacity:16;void*old=rd<void*>(self,0x38);wr<U>(self,0x40,next);
  void*buffer=live_operator_new(next*4);const I count=rd<I>(self,0x3c);wr<void*>(self,0x38,buffer);
  if(count>0){U i=0;do {void*dest=rd<void*>(self,0x38);wr<U>(dest,i*4,rd<U>(old,i*4));const I fresh=rd<I>(self,0x3c);++i;if(signed_bits(i)>=fresh)break;}while(true);}
  if(old)live_free(old);
 }
 const U count=rd<U>(self,0x3c);void*buffer=rd<void*>(self,0x38);wr<void*>(buffer,count*4,node);wr<U>(self,0x3c,rd<U>(self,0x3c)+1);return node;
}
extern "C" int __fastcall kotor_fastcall_006721f0(void*self,void*) noexcept(false){
 const U initial=rd<U>(self,0x3c);U index=initial-1;
 while(signed_bits(index)>=0){live_free(rd<void*>(rd<void*>(self,0x38),index*4));--index;}
 void*buffer=rd<void*>(self,0x38);if(buffer){live_free(buffer);wr<U>(self,0x38,0);wr<U>(self,0x40,0);}wr<U>(self,0x3c,0);return signed_bits(initial);
}
extern "C" unsigned __fastcall kotor_fastcall_00672fa0(void*self,void*,void*object) noexcept(false){
 void*wrapper=live_operator_new(4);
 try {if(wrapper){wr<void*>(wrapper,0,object);if(object){void*list=plus(object,4);const U count=rd<U>(object,8);const U cap=rd<U>(list,8);if(count==cap)grow(list,cap?cap+cap:16);const U at=rd<U>(list,4);wr<void*>(rd<void*>(list,0),at*4,wrapper);wr<U>(list,4,rd<U>(list,4)+1);}}}
 catch(...){live_free(wrapper);throw;}
 const U cap=rd<U>(self,0x58);const U count=rd<U>(self,0x54);void*list=plus(self,0x50);
 if(count==cap)grow(list,cap?cap+cap:16);
 const U at=rd<U>(list,4);void*buffer=rd<void*>(list,0);wr<void*>(buffer,at*4,wrapper);const U result=rd<U>(list,4)+1;wr<U>(list,4,result);return result;
}

