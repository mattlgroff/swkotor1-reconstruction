// Shared minigame VM getters, original005CCC60 and005CCD20.
#include "04eec24501dc-claude_minigame_vm_b.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*,void*);
namespace {
using vmx::U;
U read_word(const void* base,U offset) {
    U value;
    std::memcpy(&value,vmx::ptr(vmx::word(base)+offset),sizeof(value));
    return value;
}
void* player() {
    void* manager=kotor_menu_window_cells::manager_007a39fc;
    void* client=vmx::ptr(read_word(manager,4));
    void* game=kotor_m15e_client_get_mini_game_005edb00(client,nullptr);
    return vmx::ptr(read_word(game,0x24)); // original unchecked game, checked player
}
int push_status(U result) { return result!=0?0:vmx::k_push_failed; }
}
extern "C" int __fastcall kotor_fastcall_005ccc60(void*,void*,int routine,int) {
    U bits=0;
    void* p=player();
    if(p!=nullptr) {
        switch(routine) { // decoded005CCCFC byte index ->005CCCE8 target
        case 643: bits=read_word(p,0x98); break;
        case 644: bits=read_word(p,0x1d8); break;
        case 645: bits=read_word(p,0x1e0); break;
        case 667: bits=read_word(p,0x1dc); break;
        default: break;
        }
    }
    float value;
    std::memcpy(&value,&bits,sizeof(value));
    return push_status(recovered_vm_outer_push_float_005d1030(vmx::vm(),nullptr,value));
}
extern "C" int __fastcall kotor_fastcall_005ccd20(void*,void*,int routine,int) {
    U words[3]={0,0,0};
    void* p=player();
    if(p!=nullptr) {
        U offset=0;
        switch(routine) { // decoded005CCE14 byte index ->005CCDFC target
        case 641: offset=0x1c4; break;
        case 646: offset=0x1e4; break;
        case 653: offset=0x1f0; break;
        case 655: offset=0x208; break;
        case 717: offset=0x1fc; break;
        default: break;
        }
        if(offset!=0) {
            words[0]=read_word(p,offset);
            words[1]=read_word(p,offset+4);
            words[2]=read_word(p,offset+8);
        }
    }
    w5::Vector value;
    static_assert(sizeof(value)==sizeof(words),"VM vector is three32bit words");
    std::memcpy(&value,words,sizeof(value));
    return push_status(recovered_vm_outer_push_vector_005d1050(vmx::vm(),nullptr,value));
}
