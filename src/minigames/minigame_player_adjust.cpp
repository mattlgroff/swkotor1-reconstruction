#include "1297bf7da380-minigame_player_adjust.hpp"
#include <cstring>
extern "C" int __cdecl format_006fadb0(char*,const char*,...);
namespace {
template<class T>T read(const void* p,unsigned o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v;}
void write(void* p,std::uint32_t v){std::memcpy(static_cast<unsigned char*>(p)+0x1d0,&v,4);}
}
extern "C" void __fastcall kotor_fastcall_0066ce60(void* self,void*,std::int32_t value,std::int32_t additive){
 const auto current=read<std::uint32_t>(self,0x1d0);
 if(additive)write(self,current+static_cast<std::uint32_t>(value));
 else {if(current==static_cast<std::uint32_t>(value))return;write(self,static_cast<std::uint32_t>(value));}
 auto signedValue=read<std::int32_t>(self,0x1d0);
 if(signedValue<-10)write(self,static_cast<std::uint32_t>(-10));
 else if(signedValue>10)write(self,10);
 signedValue=read<std::int32_t>(self,0x1d0);
 char name[12];
 if(signedValue)format_006fadb0(name,"Bank%c_%02d",signedValue<0?'L':'R',signedValue<0?-signedValue:signedValue);
 else format_006fadb0(name,"Ready_01");
 using Animate=void(__fastcall*)(void*,void*,const char*,float,std::uint32_t);
 auto table=read<void*>(self,0);read<Animate>(table,0)(self,nullptr,name,1.0f,0x10);
}
