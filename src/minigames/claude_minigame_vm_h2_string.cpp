// T5 h2 (isolated /EHs recipe, as accepted for F1/G1): original 005CB560 005CBB80 005CC450 005CC7F0.
// 005CB650 lives in claude_minigame_vm_h2_f14.cpp (depends on the undelivered t1 F14 A2 body 0066BFB0).
// Each original has CExoString unwind states; C++ object lifetimes below reproduce them.
#include "c76a5786c4ff-claude_minigame_vm_h2.hpp"
#include "5f7023825481-claude_minigame_vm_h2_pending.hpp"
#include "be4e236340a2-claude_turret_t4_leaves.hpp"
#include "9b7fd0ec1b5a-claude_turret_t4_setup.hpp"
#include "3522bb9da858-claude_turret_t4_refs.hpp"
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
void* at(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
void* object_array() {
    return kotor_fastcall_005edaf0(vmx::ptr(read(kotor_menu_window_cells::manager_007a39fc, 4)), nullptr);
}
void* lookup(U id, U type) { return kotor_fastcall_0066bf30(object_array(), nullptr, id, type); }
struct String {
    vmx::ExoString value;
    String() { kotor_movie::string_element_construct_005b3190(&value, nullptr); }
    explicit String(const char* text) { frame_LogStringCtor_005e5a90(&value, nullptr, text); }
    ~String() noexcept(false) { frame_LogStringDtor_005e5c20(&value, nullptr); }
    String(const String&) = delete;
    String& operator=(const String&) = delete;
};
// A CExoString constructed by a callee into caller storage (hidden return); destroyed by the caller.
struct Adopted {
    vmx::ExoString* value;
    explicit Adopted(vmx::ExoString* v) : value(v) {}
    ~Adopted() noexcept(false) { frame_LogStringDtor_005e5c20(value, nullptr); }
    Adopted(const Adopted&) = delete;
    Adopted& operator=(const Adopted&) = delete;
};
struct ResRef { alignas(4) unsigned char bytes[16]; };  // CResRef, 16 bytes (00405F70 copies four dwords)
static_assert(sizeof(ResRef) == 16, "CResRef is 16 bytes");
const menu_resources::Ref* as_ref(const ResRef* r) { return reinterpret_cast<const menu_resources::Ref*>(r); }
int push_string_status(String& s) {
    return recovered_vm_outer_push_string_005d1090(vmx::vm(), nullptr, &s.value) ? 0 : vmx::k_push_failed;
}
}

// 005CB560 SWMG_GetLastEventModelName (584). Lookup precedes both strings (as 005CB480).
extern "C" int __fastcall kotor_fastcall_005cb560(void* commands, void*, int, int) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;      // 005CB578
    U id = read(commands, 8);                                       // 005CB57D
    void* object = kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(vmx::ptr(read(manager, 4)), nullptr), nullptr, id, 0);
    String first;                                                   // 005CB59C, state 0
    if (object) {
        void* model = vmx::ptr(read(object, 0x18));                 // 005CB5AD
        if (model) {
            using Name = const char* (__fastcall*)(void*, void*);  // [model vtbl+0Ch]
            Name fn = reinterpret_cast<Name>(vmx::ptr(read(vmx::ptr(read(model, 0)), 0xc)));
            (void)GuiAssign005e5140(&first.value, nullptr, fn(model, nullptr));  // 005CB5B4..005CB5BE
        }
    }
    String second(WindowCStr005e5670(&first.value, nullptr));      // 005CB5C3..005CB5D1, state 1
    return push_string_status(second);                              // 005CB5D6..005CB643
}

// 005CBB80 SWMG_GetObjectName (597).
extern "C" int __fastcall kotor_fastcall_005cbb80(void*, void*, int, int) {
    String name;                                                    // [esp+4], state 0
    U id;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    void* object = lookup(id, 0);                                   // 005CBBE4..005CBBFB
    if (object) {
        vmx::ExoString storage;                                     // [esp+0Ch], constructed by the virtual
        using GetName = vmx::ExoString* (__fastcall*)(void*, void*, vmx::ExoString*);  // [vtbl+4], hidden return
        GetName fn = reinterpret_cast<GetName>(vmx::ptr(read(vmx::ptr(read(object, 0)), 4)));
        vmx::ExoString* returned = fn(object, nullptr, &storage);  // 005CBC0D
        Adopted temporary(&storage);                                // state 1
        (void)async_assign_string_005e5c50(&name.value, nullptr, returned);  // 005CBC1A
    }                                                               // 005CBC28 ~temporary, state 0
    return push_string_status(name);                                // 005CBC2D..005CBC7D
}

// 005CC450 SWMG_GetGunBankBulletModel (625) / GetGunBankGunModel (626).
extern "C" int __fastcall kotor_fastcall_005cc450(void*, void*, int routine, int) {
    String text;                                                    // [esp+8], state 0
    U id; I index;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);                                 // 005CC4C8..005CC4DF
    if (follower) {
        void* bank = kotor_fastcall_0066d240(follower, nullptr, index);  // 005CC4EF
        if (bank) {
            if (routine == 625) c2da_ref_to_string_00405f70(at(bank, 0x04), nullptr, &text.value);  // 005CC511
            else if (routine == 626) c2da_ref_to_string_00405f70(at(bank, 0x14), nullptr, &text.value);  // 005CC507
        }
    }
    return push_string_status(text);                                // 005CC51E..005CC56E
}

// 005CC7F0 SWMG_SetGunBankBulletModel (632) / SetGunBankGunModel (633).
extern "C" int __fastcall kotor_fastcall_005cc7f0(void*, void*, int routine, int) {
    String text;                                                    // [esp+8], state 0
    U id; I index;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_string_005d1080(vmx::vm(), nullptr, &text.value)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);                                 // 005CC87C..005CC892
    if (follower) {
        void* bank = kotor_fastcall_0066d240(follower, nullptr, index);  // 005CC8A3
        if (bank) {
            if (routine == 632) {                                   // 005CC8D8: ResRef at [esp+20h]
                ResRef ref;
                (void)async_ref_construct_string_00406d60(&ref, nullptr, &text.value);
                kotor_fastcall_005cb460(bank, nullptr, as_ref(&ref));
            } else if (routine == 633) {                            // 005CC8BC: ResRef at [esp+10h]
                ResRef ref;
                (void)async_ref_construct_string_00406d60(&ref, nullptr, &text.value);
                kotor_fastcall_006740d0(bank, nullptr, as_ref(&ref));
            }
        }
    }
    return 0;                                                       // 005CC8F3..005CC914
}
