// T5 h2 F14-dependent unit (isolated /EHs recipe): original 005CB650 and 005CD160.
// Kept separate because their callees 0066BFB0 / 0066CA90 are t1 F14 A2 bodies not yet delivered;
// select this object only together with that t1 tier. No stub, no placeholder.
#include "c76a5786c4ff-claude_minigame_vm_h2.hpp"
#include "5f7023825481-claude_minigame_vm_h2_pending.hpp"
#include "d1c462888f42-claude_turret_t1_leaves.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }  // [007A39FC]
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*, void*); }  // CExoString()
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*, void*);
void* __fastcall kotor_fastcall_0066bf30(void*, void*, vmx::U id, vmx::U type);
}
namespace {
using vmx::U;
using vmx::I;
U read(const void* p, U off) { U bits; std::memcpy(&bits, static_cast<const unsigned char*>(p) + off, 4); return bits; }
void* object_array() {
    return kotor_fastcall_005edaf0(vmx::ptr(read(kotor_menu_window_cells::manager_007a39fc, 4)), nullptr);
}
void* lookup(U id, U type) { return kotor_fastcall_0066bf30(object_array(), nullptr, id, type); }
struct String {
    vmx::ExoString value;
    String() { kotor_movie::string_element_construct_005b3190(&value, nullptr); }
    ~String() noexcept(false) { frame_LogStringDtor_005e5c20(&value, nullptr); }
    String(const String&) = delete;
    String& operator=(const String&) = delete;
};
}

// 005CB650 SWMG_GetObjectByName (585).
extern "C" int __fastcall kotor_fastcall_005cb650(void*, void*, int, int) {
    String name;                                                    // 005CB66D, state 0
    U result = 0xff;                                                // 005CB685
    if (!recovered_vm_outer_pop_string_005d1080(vmx::vm(), nullptr, &name.value)) return vmx::k_pop_failed;
    void* object = kotor_fastcall_0066bfb0(object_array(), nullptr, &name.value);  // 005CB6BB..005CB6D0
    if (object) result = read(object, 8);
    return recovered_vm_outer_push_object_005d10d0(vmx::vm(), nullptr, result) ? 0 : vmx::k_push_failed;
}

// 005CD160 SWMG_SetSoundFrequency (684) / SetSoundFrequencyIsRandom (686) / SetSoundVolume (688).
extern "C" int __fastcall kotor_fastcall_005cd160(void*, void*, int routine, int) {
    U id; I sound; I value;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;    // [esp+8]
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &sound)) return vmx::k_pop_failed; // [esp+4]
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed; // [esp]
    void* follower = lookup(id, 4);                                 // 005CD1AA..005CD1C0
    if (!follower) return 0;
    if (routine == 684) kotor_fastcall_0066ca10(follower, nullptr, sound, value);       // 005CD20F
    else if (routine == 686) kotor_fastcall_0066ca50(follower, nullptr, sound, value);  // 005CD1F8
    else if (routine == 688) kotor_fastcall_0066ca90(follower, nullptr, sound, value);  // 005CD1E1
    return 0;
}
