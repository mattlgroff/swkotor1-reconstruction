#include "1be3d11e3161-claude_turret_t1_records.hpp"
#include <cstring>
static_assert(sizeof(void*)==4,"KOTOR x86 ABI");
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*);
}
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,std::uint32_t,std::uint32_t);
}
namespace {
using U=std::uint32_t;
unsigned char* at(void* p,U n) { return static_cast<unsigned char*>(p)+n; }
const unsigned char* at(const void* p,U n) { return static_cast<const unsigned char*>(p)+n; }
U word(const void* p,U n) { U v; std::memcpy(&v,at(p,n),4); return v; }
void store(void* p,U n,U v) { std::memcpy(at(p,n),&v,4); }
void* last_hit(void* self,U offset,U type) {
    // Original loads manager before ID, then app; ID stays captured across calls.
    void* manager=kotor_menu_window_cells::manager_007a39fc;
    const U id=word(self,offset);
    void* app; std::memcpy(&app,at(manager,4),4);
    void* objects=kotor_fastcall_005edaf0(app,nullptr);
    void* result=kotor_fastcall_0066bf30(objects,nullptr,id,type);
    if(!result) store(self,offset,255);
    return result;
}
}
extern "C" void* __fastcall kotor_fastcall_0066c130(void* self,void*) { return last_hit(self,0x4c,4); }
extern "C" void* __fastcall kotor_fastcall_0066c160(void* self,void*) { return last_hit(self,0x50,4); }
extern "C" void* __fastcall kotor_fastcall_0066cb10(void* self,void*) { return last_hit(self,0x170,3); }
extern "C" void __fastcall kotor_fastcall_00671460(void* self,void*,const void* record) {
    // Each store precedes the next load; preserve overlapping-record effects.
    for(U n=0;n<24;n+=4) store(self,0x1c+n,word(record,n));
    menu_resources::Ref discarded;
    resource_ref_assignment::assign_00406120(reinterpret_cast<menu_resources::Ref*>(at(self,0x34)),nullptr,&discarded,reinterpret_cast<const menu_resources::Ref*>(at(record,0x18)));
}
