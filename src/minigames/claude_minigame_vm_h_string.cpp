// T5 h1 (isolated /EHs recipe, as accepted for G1 005CB730): original 005CBDB0.
#include "6d08ac356b32-claude_minigame_vm_h.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }  // [007A39FC]
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*, void*); }  // CExoString()
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*, void*);
void* __fastcall kotor_fastcall_0066bf30(void*, void*, vmx::U id, vmx::U type);
}
namespace {
using vmx::U;
U read(const void* p, U off) { U bits; std::memcpy(&bits, static_cast<const unsigned char*>(p) + off, 4); return bits; }
void* lookup(void* manager, U id, U type) {
    void* client = vmx::ptr(read(manager, 4));
    return kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(client, nullptr), nullptr, id, type);
}
// Unwind state 0 of the original frame (005CBDDC .. 005CBE6B): ~CExoString on every exit.
struct String {
    vmx::ExoString value;
    String() { kotor_movie::string_element_construct_005b3190(&value, nullptr); }
    ~String() noexcept(false) { frame_LogStringDtor_005e5c20(&value, nullptr); }
    String(const String&) = delete;
    String& operator=(const String&) = delete;
};
}

// 005CBDB0 SWMG_RemoveAnimation (607): object, animation name; plays slot 0 with speed 1.0, flags 0x80.
extern "C" int __fastcall kotor_fastcall_005cbdb0(void*, void*, int, int) {
    String name;                                                    // 005CBDCC
    U id;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;          // 005CBDE4
    if (!recovered_vm_outer_pop_string_005d1080(vmx::vm(), nullptr, &name.value)) return vmx::k_pop_failed;  // 005CBDF8
    void* object = lookup(kotor_menu_window_cells::manager_007a39fc, id, 0);                                // 005CBE28..005CBE40
    if (object) {
        void* table = vmx::ptr(read(object, 0));                    // 005CBE4C: table captured before CStr
        const char* text = WindowCStr005e5670(&name.value, nullptr); // 005CBE5C
        using Play = void (__fastcall*)(void*, void*, const char*, float, U);
        Play fn = reinterpret_cast<Play>(vmx::ptr(read(table, 0)));  // slot 0 read after CStr
        fn(object, nullptr, text, 1.0f, 0x80);                      // 005CBE4E/005CBE53 push 80h, 3F800000h; 005CBE64
    }
    return 0;                                                       // 005CBE67..005CBE7C
}
