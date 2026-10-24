// T5 h1 (default W5 recipe): original 005CBB00, 005CCE70, 005CCF30.
// 005CBDB0 has a CExoString lifetime and lives in claude_minigame_vm_h_string.cpp (isolated /EHs).
#include "6d08ac356b32-claude_minigame_vm_h.hpp"
#include "6f32deb42978-claude_minigame_vm_h_pending.hpp"
#include "5f558436913e-minigame_player_leaves.hpp"
#include "0a911a07647c-minigame_tunnel_offset.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }  // [007A39FC]
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*, void*);                         // client -> object array
void* __fastcall kotor_fastcall_0066bf30(void*, void*, vmx::U id, vmx::U type);  // object array lookup, RET 8
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*, void*);        // GetMiniGame
}
namespace {
using vmx::U;
U read(const void* p, U off) { U bits; std::memcpy(&bits, static_cast<const unsigned char*>(p) + off, 4); return bits; }
void write(void* p, U off, U bits) { std::memcpy(static_cast<unsigned char*>(p) + off, &bits, 4); }
void* lookup(void* manager, U id, U type) {
    void* client = vmx::ptr(read(manager, 4));
    return kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(client, nullptr), nullptr, id, type);
}
// Original: mov eax,[obj]; call [eax+14h]  (follower cast slot, thiscall, no args)
void* follower_cast(void* object) {
    using Cast = void* (__fastcall*)(void*, void*);
    void* table = vmx::ptr(read(object, 0));
    Cast fn = reinterpret_cast<Cast>(vmx::ptr(read(table, 0x14)));
    return fn(object, nullptr);
}
// [[[7A39FC]+4] GetMiniGame]+24h : the game result is dereferenced unchecked (original quirk).
void* player() {
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    void* game = kotor_m15e_client_get_mini_game_005edb00(vmx::ptr(read(manager, 4)), nullptr);
    return vmx::ptr(read(game, 0x24));
}
// 005CCFBE / 005CCFE1 / 005CD015: three ordered dword stores of the popped vector.
void store_vector(void* p, U off, const w5::Vector* v) {
    write(p, off + 0, read(v, 0));
    write(p, off + 4, read(v, 4));
    write(p, off + 8, read(v, 8));
}
}

// 005CBB00 SWMG_GetLastFollowerHit (593) / SWMG_GetLastObstacleHit (594).
extern "C" int __fastcall kotor_fastcall_005cbb00(void* commands, void*, int routine, int) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;      // 005CBB02
    U id = read(commands, 8);                                       // 005CBB08
    void* object = lookup(manager, id, 0);                          // 005CBB0B..005CBB19
    U result = 0xff;                                                // 005CBB22
    if (object) {
        void* hit = nullptr;
        if (routine == 593) {
            hit = kotor_fastcall_0066c160(object, nullptr);         // 005CBB49
        } else if (routine == 594) {
            void* follower = follower_cast(object);                 // 005CBB37..005CBB39
            if (!follower) goto push;                               // 005CBB3E
            hit = kotor_fastcall_0066cb10(follower, nullptr);       // 005CBB42
        } else {
            goto push;                                              // 005CBB35
        }
        if (hit) result = read(hit, 8);                             // 005CBB4E..005CBB52
    }
push:
    return recovered_vm_outer_push_object_005d10d0(vmx::vm(), nullptr, result) ? 0 : vmx::k_push_failed;  // 005CBB55..005CBB6A
}

// 005CCE70 SWMG_SetPlayerSpeed 649 / MinSpeed 650 / AccelerationPerSecond 651 / MaxSpeed 668.
extern "C" int __fastcall kotor_fastcall_005cce70(void*, void*, int routine, int) {
    float value;
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed;  // 005CCE71..005CCE85
    void* p = player();                                             // 005CCE8E..005CCE9C
    if (!p) return 0;                                               // 005CCEA1
    switch (routine) {  // decoded 005CCF1C byte index -> 005CCF08 target
    case 649: (void)kotor_fastcall_0066c870(p, nullptr, value); break;  // 005CCEBF, ST0 discarded
    case 650: (void)kotor_fastcall_0066cf40(p, nullptr, value); break;  // 005CCED1
    case 651: (void)kotor_fastcall_0066cfa0(p, nullptr, value); break;  // 005CCEE3
    case 668: (void)kotor_fastcall_0066cf70(p, nullptr, value); break;  // 005CCEF5
    default: break;                                                 // 652..667 and out of range -> 005CCF01
    }
    return 0;
}

// 005CCF30 SWMG_SetPlayerOffset 647 / TunnelPos 652 / TunnelNeg 654 / Origin 656 / TunnelInfinite 718.
extern "C" int __fastcall kotor_fastcall_005ccf30(void*, void*, int routine, int) {
    w5::Vector value;                                               // [esp+0], zeroed before the pop
    std::memset(&value, 0, sizeof(value));
    if (!recovered_vm_outer_pop_vector_005d1040(vmx::vm(), nullptr, &value)) return vmx::k_pop_failed;  // 005CCF56..005CCF5F
    void* p = player();                                             // 005CCF6A..005CCF79
    if (!p) return 0;                                               // 005CCF7E
    switch (routine) {  // decoded 005CD050 byte index -> 005CD038 target
    case 647: {                                                     // 005CCFA4 SetOffset, Vector result discarded
        w5::Vector result;                                          // [esp+0Ch]
        (void)kotor_fastcall_0066ccf0(p, nullptr, &result, &value);
        break;
    }
    case 652: store_vector(p, 0x1e4, &value); break;                // 005CCFBE
    case 654: store_vector(p, 0x1f0, &value); break;                // 005CCFE1
    case 656: {                                                     // 005CD004 SetOrigin ...
        w5::Vector result;                                          // [esp+18h]
        (void)kotor_fastcall_0066cfd0(p, nullptr, &result, &value);
        // ORIGINAL QUIRK (issue #23): no jump after the call at 005CD010; execution falls through
        // into the 718 case at 005CD015 and also stores the popped vector at player+1FCh.
        store_vector(p, 0x1fc, &value);
        break;
    }
    case 718: store_vector(p, 0x1fc, &value); break;                // 005CD015
    default: break;                                                 // 005CD02F
    }
    return 0;
}
