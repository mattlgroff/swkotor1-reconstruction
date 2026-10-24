#include "7314e5f55d64-player_internal.hpp"
#include <initializer_list>
#include "0ae13273b4e7-claude_turret_t1_lifecycle.hpp"
using namespace t2_private;
#define SLOT(f) reinterpret_cast<const void*>(&f)
extern "C" {
const void* const kotor_turret_vtable_007525f0[16]={SLOT(kotor_fastcall_0066d550),SLOT(kotor_fastcall_0066cad0),SLOT(kotor_fastcall_0066c740),SLOT(kotor_fastcall_0066c970),SLOT(kotor_fastcall_0066c9c0),SLOT(gui_load_slot_self_00641db0),SLOT(gui_load_slot_self_00641db0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(kotor_fastcall_0066e130),SLOT(kotor_fastcall_0066f870),SLOT(kotor_fastcall_006702f0),SLOT(kotor_fastcall_0066f890),SLOT(kotor_fastcall_0066e1f0),SLOT(kotor_fastcall_0066cf20),SLOT(input_desc_return_true_0063e7a0)};
const void* const kotor_turret_vtable_007528a8[16]={SLOT(kotor_fastcall_0066d550),SLOT(kotor_fastcall_0066cad0),SLOT(kotor_fastcall_0066c740),SLOT(kotor_fastcall_0066c970),SLOT(kotor_fastcall_0066c9c0),SLOT(gui_load_slot_self_00641db0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(gui_load_slot_self_00641db0),SLOT(gui_load_slot_zero_0063e7f0),SLOT(kotor_fastcall_0066e130),SLOT(kotor_fastcall_006714b0),SLOT(kotor_fastcall_006705f0),SLOT(kotor_fastcall_0066fb20),SLOT(kotor_fastcall_0066e1f0),SLOT(kotor_fastcall_0066d090),SLOT(kotor_fastcall_00670b10)};
}
#undef SLOT
namespace {
// Original FuncInfo77DDB4. Only armed during construction failure or destruction.
struct Cleanup {
 void* self;int state;
 ~Cleanup() noexcept(false){
  if(state>=3){state=2;exo_array_list_destruct_0056b6f0(reinterpret_cast<List*>(at(self,0x1B8)),nullptr);}
  if(state>=2){state=1;exo_array_list_destruct_0056b6f0(reinterpret_cast<List*>(at(self,0x1AC)),nullptr);}
  if(state>=1){state=0;exo_array_list_destruct_0056b6f0(reinterpret_cast<List*>(at(self,0x1A0)),nullptr);}
  if(state>=0){state=-1;kotor_fastcall_0066e000(self,nullptr);}
 }
};
}
extern "C" void* __fastcall kotor_fastcall_0066eb50(void* self,void*,void* mini) noexcept(false){
 kotor_fastcall_0066dee0(self,nullptr,mini,1);
 Cleanup guard{self,0};
 wr(self,0,kotor_turret_vtable_007525f0);
 for(U base: {0x1A0u,0x1ACu,0x1B8u}){wr<U>(self,base+4,0);wr<U>(self,base+8,0);wr<U>(self,base,0);}
 for(U base: {0x1C4u,0x1E4u,0x1F0u,0x1FCu,0x208u,0x214u,0x228u,0x234u}){wr<U>(self,base,0);wr<U>(self,base+4,0);wr<U>(self,base+8,0);}
 wr<U>(self,0x24C,0);wr<U>(self,0x248,0);wr<U>(self,0x244,0);wr<U>(self,0x240,0x3F800000);
 guard.state=3;
 wr<U>(self,0x1D0,0);wr<U>(self,0x1D8,0x42C80000);wr<U>(self,0x1DC,0x42C80000);wr<U>(self,0x1E0,0);wr<std::uint8_t>(self,0x1D4,0);wr<void*>(self,0x220,nullptr);
 kotor_fastcall_0066d190(at(self,0x1A0),nullptr,4);wr<U>(self,0x1A4,4);
 kotor_fastcall_0066d190(at(self,0x1AC),nullptr,4);wr<U>(self,0x1B0,4);
 menu_list_growth::allocate_005e03c0(reinterpret_cast<List*>(at(self,0x1B8)),nullptr,8);wr<U>(self,0x1BC,8);
 wr<U>(self,0x1A4,0);wr<U>(self,0x1B0,0);guard.state=-1;return self;
}
extern "C" void __fastcall kotor_fastcall_0066ecf0(void* self,void*) noexcept(false){
 wr(self,0,kotor_turret_vtable_007525f0);Cleanup guard{self,3};
 void* hook=rd<void*>(self,0x220);
 if(hook){using Delete=U(__fastcall*)(void*,void*,U);rd<Delete>(rd<void*>(hook,0),0)(hook,nullptr,1);}
 guard.state=2;live_free(rd<void*>(self,0x1B8));wr<void*>(self,0x1B8,nullptr);
 guard.state=1;live_free(rd<void*>(self,0x1AC));wr<void*>(self,0x1AC,nullptr);
 guard.state=0;live_free(rd<void*>(self,0x1A0));wr<void*>(self,0x1A0,nullptr);
 guard.state=-1;kotor_fastcall_0066e000(self,nullptr);
}
extern "C" void* __fastcall kotor_fastcall_0066f870(void* self,void*,U flags) noexcept(false){kotor_fastcall_0066ecf0(self,nullptr);if(flags&1)live_free(self);return self;}
extern "C" void __fastcall kotor_fastcall_006714d0(void* self,void*) noexcept(false){kotor_fastcall_0066e000(self,nullptr);}
extern "C" void* __fastcall kotor_fastcall_006714b0(void* self,void*,U flags) noexcept(false){kotor_fastcall_006714d0(self,nullptr);if(flags&1)live_free(self);return self;}

