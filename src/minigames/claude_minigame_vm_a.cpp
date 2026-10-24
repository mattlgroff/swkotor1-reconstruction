// Shared minigame VM batch A. Original SWKOTOR x86: 005CD230,005CD280,005CBF70,005CBFB0.
// Client-only minigame fields; no new globals, allocator, or minigame lifecycle.
#include "47f8a19c96b1-claude_minigame_vm_a.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*, void*);

namespace {
using vmx::U;
// Integer-address field access retains original fault sites for null bases.
U read_word(const void* base, U offset) {
    U value;
    std::memcpy(&value, vmx::ptr(vmx::word(base) + offset), sizeof(value));
    return value;
}
void write_word(void* base, U offset, U value) {
    std::memcpy(vmx::ptr(vmx::word(base) + offset), &value, sizeof(value));
}
void* mini_game() {
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    void* client = vmx::ptr(read_word(manager, 4));
    return kotor_m15e_client_get_mini_game_005edb00(client, nullptr);
}
int push_status(U result) { return result != 0 ? 0 : vmx::k_push_failed; }
}

extern "C" int __fastcall kotor_fastcall_005cd230(void*, void*, int, int) {
    float value;
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &value))
        return vmx::k_pop_failed;                                      // 005CD247
    U bits;
    std::memcpy(&bits, &value, sizeof(bits));
    void* game = mini_game();                                          // 005CD252..263
    write_word(game, 0xbc, bits);                                     // 005CD26C (unchecked)
    return 0;
}

extern "C" int __fastcall kotor_fastcall_005cd280(void*, void*, int, int) {
    void* game = mini_game();
    const U bits = read_word(game, 0xbc);                              // 005CD28E (unchecked)
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return push_status(recovered_vm_outer_push_float_005d1030(vmx::vm(), nullptr, value));
}

extern "C" int __fastcall kotor_fastcall_005cbf70(void*, void*, int, int) {
    void* game = mini_game();
    U id = 0xff;                                                      // 005CBF7F
    if (game != nullptr) {
        void* player = vmx::ptr(read_word(game, 0x24));
        if (player != nullptr) id = read_word(player, 8);              // 005CBF8D
    }
    return push_status(recovered_vm_outer_push_object_005d10d0(vmx::vm(), nullptr, id));
}

extern "C" int __fastcall kotor_fastcall_005cbfb0(void*, void*, int routine, int) {
    void* game = mini_game();
    U count = 0;
    if (game != nullptr) {
        if (routine == 612) count = read_word(game, 0x30);             // 005CBFD9
        else if (routine == 614) count = read_word(game, 0x48);        // 005CBFD4
    }
    vmx::I signed_count;
    std::memcpy(&signed_count, &count, sizeof(signed_count));
    return push_status(recovered_vm_outer_push_integer_005d1010(vmx::vm(), nullptr, signed_count));
}
