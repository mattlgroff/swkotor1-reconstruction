#include "7314e5f55d64-player_internal.hpp"
#include "9b7fd0ec1b5a-claude_turret_t4_setup.hpp"
using namespace t2_private;
namespace {
struct RawAllocation {void* value;bool armed;~RawAllocation() noexcept(false){if(armed)live_free(value);}};
void reserve_if_full(void* self){
 const U capacity=rd<U>(self,0x7C);
 if(rd<U>(self,0x78)==capacity)menu_list_growth::allocate_005e03c0(reinterpret_cast<List*>(at(self,0x74)),nullptr,capacity?capacity*2u:16u);
}
void append(void* self,void* value){
 reserve_if_full(self);
 const U count=rd<U>(self,0x78);wr(rd<void*>(self,0x74),count*4u,value);wr<U>(self,0x78,rd<U>(self,0x78)+1u);
}
void* construct_bank(void* self,bool enemy,const Ref* name,const Ref* model,const void* bullet,const void* target){
 void* raw=live_operator_new(enemy?0x80u:0x6Cu);
 RawAllocation guard{raw,true};void* result=nullptr;
 if(raw){
  if(enemy){void* current_player=rd<void*>(rd<void*>(self,0x60),0x24);result=kotor_fastcall_00674860(raw,nullptr,self,name,model,bullet,target,current_player);}
  else result=kotor_fastcall_006746d0(raw,nullptr,self,name,model,bullet);
 }
 guard.armed=false;return result;
}
void assign_fire(void* bank,const Ref* fire){
 Ref out;resource_ref_assignment::assign_00406120(reinterpret_cast<Ref*>(at(bank,0x24)),nullptr,&out,fire);
}
void* create_bank(void* self,I index,const Ref* name,const Ref* model,const void* bullet,const void* target,const Ref* fire,bool enemy){
 if(index<0)return nullptr;
 const I count=rd<I>(self,0x78);
 if(count<=index){
  if(count<index){U remaining=static_cast<U>(index)-static_cast<U>(count);do{append(self,nullptr);}while(--remaining!=0);}
  void* bank=construct_bank(self,enemy,name,model,bullet,target);
  assign_fire(bank,fire); // Null allocation follows original raw access here.
  append(self,bank);return bank;
 }
 if(rd<void*>(rd<void*>(self,0x74),static_cast<U>(index)*4u)!=nullptr){
  char message[64];format_006fadb0(message,enemy?"CSWMiniEnemy: Recreating gun bank %d":"CSWMiniPlayer: Recreating gun bank %d",index);return nullptr;
 }
 void* bank=construct_bank(self,enemy,name,model,bullet,target);
 // Original enemy null-slot path omits assignment, including allocation-null.
 if(!enemy)assign_fire(bank,fire);
 w1x_pointer_list_insert_00616940(at(self,0x74),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(bank)),static_cast<U>(index));
 deleteAt_006857a0(reinterpret_cast<List*>(at(self,0x74)),nullptr,signed_bits(static_cast<U>(index)+1u));
 return bank;
}
}
extern "C" void* __fastcall kotor_fastcall_0066f440(void* self,void*,I index,const Ref* name,const Ref* model,const void* bullet,const Ref* fire) noexcept(false){return create_bank(self,index,name,model,bullet,nullptr,fire,false);}
extern "C" void* __fastcall kotor_fastcall_0066f5f0(void* self,void*,I index,const Ref* name,const Ref* model,const void* bullet,const void* target,const Ref* fire) noexcept(false){return create_bank(self,index,name,model,bullet,target,fire,true);}
