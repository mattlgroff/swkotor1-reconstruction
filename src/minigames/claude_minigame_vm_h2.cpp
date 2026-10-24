// T5 h2 (default W5 recipe): original 005CB930 005CBF00 005CC330 005CC580 005CC640 005CC700
// 005CC920 005CC9D0 005CCAA0. None of these originals has an unwind state.
// 005CD160 lives in claude_minigame_vm_h2_f14.cpp (depends on the undelivered t1 F14 A2 body 0066CA90).
#include "c76a5786c4ff-claude_minigame_vm_h2.hpp"
#include "5f7023825481-claude_minigame_vm_h2_pending.hpp"
#include "d1c462888f42-claude_turret_t1_leaves.hpp"
#include "be4e236340a2-claude_turret_t4_leaves.hpp"
#include "9b7fd0ec1b5a-claude_turret_t4_setup.hpp"
#include "e3e5e5154161-claude_minigame_t3_builders.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }  // [007A39FC]
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*, void*);
void* __fastcall kotor_fastcall_0066bf30(void*, void*, vmx::U id, vmx::U type);
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*, void*);
}
namespace {
using vmx::U;
using vmx::I;
U read(const void* p, U off) { U bits; std::memcpy(&bits, static_cast<const unsigned char*>(p) + off, 4); return bits; }
void write(void* p, U off, U bits) { std::memcpy(static_cast<unsigned char*>(p) + off, &bits, 4); }
float as_float(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }
U as_bits(I value) { U bits; std::memcpy(&bits, &value, 4); return bits; }
// [[7A39FC]+4] -> 005EDAF0 -> 0066BF30(id, type); the manager is read at the call site's original point.
void* lookup(U id, U type) {
    void* client = vmx::ptr(read(kotor_menu_window_cells::manager_007a39fc, 4));
    return kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(client, nullptr), nullptr, id, type);
}
// Gun bank vtable slot +8 (005CC600 / 005CC771 / 005CCB20): int thiscall(), no arguments.
U gun_bank_slot8(void* bank) {
    using Fn = U (__fastcall*)(void*, void*);
    Fn fn = reinterpret_cast<Fn>(vmx::ptr(read(vmx::ptr(read(bank, 0)), 8)));
    return fn(bank, nullptr);
}
// Push helpers: VM cell re-read at the push; any nonzero provider result is success (neg/sbb/and/add idiom).
int push_object_status(U id) { return recovered_vm_outer_push_object_005d10d0(vmx::vm(), nullptr, id) ? 0 : vmx::k_push_failed; }
int push_integer_status(I v) { return recovered_vm_outer_push_integer_005d1010(vmx::vm(), nullptr, v) ? 0 : vmx::k_push_failed; }
int push_float_status(U bits) { return recovered_vm_outer_push_float_005d1030(vmx::vm(), nullptr, as_float(bits)) ? 0 : vmx::k_push_failed; }
}

// 005CB930 SWMG_GetLastBulletHitShooter (589).
extern "C" int __fastcall kotor_fastcall_005cb930(void* commands, void*, int, int) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;      // 005CB933
    U id = read(commands, 8);                                       // 005CB939
    void* client = vmx::ptr(read(manager, 4));
    U result = 0xff;                                                // 005CB942
    void* object = kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(client, nullptr), nullptr, id, 0);
    if (object) {
        void* shooter = kotor_fastcall_0066c130(object, nullptr);   // 005CB959
        if (shooter) result = read(shooter, 8);
    }
    return push_object_status(result);                              // 005CB965..005CB97A
}

// 005CBF00 SWMG_SetCameraClip (610): GetMiniGame before the two float pops.
extern "C" int __fastcall kotor_fastcall_005cbf00(void*, void*, int, int) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;      // 005CBF00
    void* game = kotor_m15e_client_get_mini_game_005edb00(vmx::ptr(read(manager, 4)), nullptr);  // 005CBF0C
    float first, second;                                            // [esp+8] / [esp+4] of the original
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &first)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &second)) return vmx::k_pop_failed;
    if (game) kotor_fastcall_00670b50(game, nullptr, first, second);  // 005CBF4B..005CBF57
    return 0;
}

// 005CC330 SWMG_SetMaxHitPoints (618) / SWMG_SetNumLoops (622).
extern "C" int __fastcall kotor_fastcall_005cc330(void*, void*, int routine, int) {
    U id; I value;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);                                 // 005CC366..005CC37D
    if (!follower) return 0;
    if (routine == 618) {
        (void)kotor_fastcall_0066c950(follower, nullptr, value);    // 005CC3AA
    } else if (routine == 622) {
        using Fn = void (__fastcall*)(void*, void*, I);             // [vtbl+38h], RET 4
        Fn fn = reinterpret_cast<Fn>(vmx::ptr(read(vmx::ptr(read(follower, 0)), 0x38)));
        fn(follower, nullptr, value);                               // 005CC398..005CC39F
    }
    return 0;
}

// 005CC580 SWMG_GetGunBankDamage (627) / GetGunBankTarget (631) / IsGunBankTargetting (640).
extern "C" int __fastcall kotor_fastcall_005cc580(void*, void*, int routine, int) {
    I result = 0;                                                   // 005CC58F xor esi,esi
    U id; I index;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);
    if (follower) {
        void* bank = kotor_fastcall_0066d240(follower, nullptr, index);  // 005CC5E1
        if (bank) {
            if (routine == 627) result = static_cast<I>(read(bank, 0x40));
            else if (routine == 631) result = static_cast<I>(read(bank, 0x50));
            else if (routine == 640) result = static_cast<I>(gun_bank_slot8(bank));
        }
    }
    return push_integer_status(result);                             // 005CC613..005CC628
}

// 005CC640 SWMG_GetGunBankTimeBetweenShots (628) / Lifespan (629) / Speed (630).
extern "C" int __fastcall kotor_fastcall_005cc640(void*, void*, int routine, int) {
    U result = 0;                                                   // [esp] = 0 before the first pop
    U id; I index;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);
    if (follower) {
        void* bank = kotor_fastcall_0066d240(follower, nullptr, index);
        if (bank) {
            if (routine == 628) result = read(bank, 0x48);
            else if (routine == 629) result = read(bank, 0x4c);
            else if (routine == 630) result = read(bank, 0x44);
        }
    }
    return push_float_status(result);                               // 005CC6D9..005CC6F2
}

// 005CC700 SWMG_GetGunBank{HorizontalSpread,VerticalSpread,SensingRadius,Inaccuracy} (registered 657..660).
// ORIGINAL BUG (issue #23): the switch at 005CC77C subtracts 661 (the SETTER selectors 661..664, as in 005CCAA0),
// so for the registered selectors 657..660 no case matches and 0.0 is always pushed. Reproduced on purpose.
extern "C" int __fastcall kotor_fastcall_005cc700(void*, void*, int routine, int) {
    U result = 0;
    U id; I index;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);
    if (follower) {
        void* bank = kotor_fastcall_0066d240(follower, nullptr, index);  // 005CC766
        if (bank && gun_bank_slot8(bank) != 0) {                    // 005CC771..005CC77A
            switch (routine) {  // jump table 005CC7D4, index = routine - 661
            case 661: result = read(bank, 0x6c); break;             // 005CC791
            case 662: result = read(bank, 0x74); break;             // 005CC796
            case 663: result = read(bank, 0x70); break;             // 005CC79F
            case 664: result = read(bank, 0x78); break;             // 005CC7A8
            default: break;
            }
        }
    }
    return push_float_status(result);                               // 005CC7B0..005CC7C9
}

// 005CC920 SWMG_SetGunBankDamage (634) / SetGunBankTarget (638).
extern "C" int __fastcall kotor_fastcall_005cc920(void*, void*, int routine, int) {
    U id; I index; I value;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);
    if (!follower) return 0;
    void* bank = kotor_fastcall_0066d240(follower, nullptr, index); // 005CC990
    if (!bank) return 0;
    if (routine == 634) write(bank, 0x40, as_bits(value));          // 005CC9BE
    else if (routine == 638) (void)kotor_fastcall_00673880(bank, nullptr, as_bits(value));  // 005CC9AA..005CC9B1
    return 0;
}

// 005CC9D0 SWMG_SetGunBankTimeBetweenShots (635) / Lifespan (636) / Speed (637).
extern "C" int __fastcall kotor_fastcall_005cc9d0(void*, void*, int routine, int) {
    U id; I index; float value;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);
    if (!follower) return 0;
    void* bank = kotor_fastcall_0066d240(follower, nullptr, index); // 005CCA40
    if (!bank) return 0;
    switch (routine) {  // ST0 results discarded (fstp st(0))
    case 635: (void)kotor_fastcall_00673f50(bank, nullptr, value); break;  // 005CCA84
    case 636: (void)kotor_fastcall_00673860(bank, nullptr, value); break;  // 005CCA70
    case 637: (void)kotor_fastcall_00673840(bank, nullptr, value); break;  // 005CCA5C
    default: break;
    }
    return 0;
}

// 005CCAA0 SWMG_SetGunBank{HorizontalSpread,VerticalSpread,SensingRadius,Inaccuracy} (661..664).
extern "C" int __fastcall kotor_fastcall_005ccaa0(void*, void*, int routine, int) {
    U id; I index; float value;
    if (!recovered_vm_outer_pop_object_005d10c0(vmx::vm(), nullptr, &id)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &index)) return vmx::k_pop_failed;
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed;
    void* follower = lookup(id, 4);
    if (!follower) return 0;
    void* bank = kotor_fastcall_0066d240(follower, nullptr, index); // 005CCB15
    if (!bank || gun_bank_slot8(bank) == 0) return 0;               // 005CCB1C..005CCB29
    switch (routine) {  // jump table 005CCB9C, index = routine - 661; ST0 results discarded
    case 661: (void)kotor_fastcall_006739c0(bank, nullptr, value); break;  // 005CCB40
    case 662: (void)kotor_fastcall_00673990(bank, nullptr, value); break;  // 005CCB57
    case 663: (void)kotor_fastcall_00673970(bank, nullptr, value); break;  // 005CCB6E
    case 664: (void)kotor_fastcall_006739f0(bank, nullptr, value); break;  // 005CCB85
    default: break;
    }
    return 0;
}
