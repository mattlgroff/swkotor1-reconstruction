#include "a0dd4ae6243e-claude_turret_t1_scripts.hpp"
#include <cstring>
static_assert(sizeof(void*)==4,"KOTOR x86 ABI");
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref* self,void*,menu_resources::Ref* out,const menu_resources::Ref* source);
}
namespace kotor_item_drop_sounds { void* __fastcall ref_default_element_00405ed0(void* self,void*); }
namespace {
using U=std::uint32_t;
using I=std::int32_t;
using Ref=menu_resources::Ref;
Ref* field(void* self,U offset) { return reinterpret_cast<Ref*>(reinterpret_cast<std::uintptr_t>(self)+offset); }
void copy_words(Ref* out,const Ref* source) {
    // Four sequential DWORD loads/stores from0066C988..9A3/0066D115..130.
    // Source/output may overlap; never combine these into memcpy of16bytes.
    for(U n=0;n<16;n+=4) {
        U word;
        std::memcpy(&word,source->bytes+n,4);
        std::memcpy(out->bytes+n,&word,4);
    }
}
}
extern "C" Ref* __fastcall kotor_fastcall_0066c970(void* self,void*,Ref* out,I index) {
    if(index>=0 && index<10) copy_words(out,field(self,0xa4u+static_cast<U>(index)*16u));
    else kotor_item_drop_sounds::ref_default_element_00405ed0(out,nullptr);
    return out;
}
extern "C" void __fastcall kotor_fastcall_0066c9c0(void* self,void*,I index,const Ref* source) {
    if(index>=0 && index<10) {
        Ref discarded;
        resource_ref_assignment::assign_00406120(field(self,0xa4u+static_cast<U>(index)*16u),nullptr,&discarded,source);
    }
}
extern "C" Ref* __fastcall kotor_fastcall_0066d100(void* self,void*,Ref* out,I index) {
    if(index>=0 && index<5) copy_words(out,field(self,0x64u+static_cast<U>(index)*16u));
    else kotor_item_drop_sounds::ref_default_element_00405ed0(out,nullptr);
    return out;
}
extern "C" void __fastcall kotor_fastcall_0066d150(void* self,void*,I index,const Ref* source) {
    if(index>=0 && index<5) {
        Ref discarded;
        resource_ref_assignment::assign_00406120(field(self,0x64u+static_cast<U>(index)*16u),nullptr,&discarded,source);
    }
}
