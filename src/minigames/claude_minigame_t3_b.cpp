// Two shared minigame leaves; selected providers remain sole owners.
#include "38c85fc5232c-claude_minigame_t3_b.hpp"
#include "d8f703239b61-resources.h"
#include <cstring>
#include <cstdint>
static_assert(sizeof(void*) == 4 && sizeof(menu_resources::Ref) == 16,
              "Selected x86 Ref layout");
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void __fastcall recovered_ref_array_grow_00506e20(void*, void*, unsigned) noexcept(false);
menu_resources::Ref* __fastcall ref_assign_copy_00406120(
    menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*) noexcept(false);
std::int32_t __fastcall kotor_fastcall_004aeaf0(
    void* server, void*, void* area_key, W4Vector* position, std::int32_t mode);
}
namespace {
using U = std::uint32_t;
using I = std::int32_t;
template<class T> T read(const void* p, U off) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(p) + off, sizeof value);
    return value;
}
template<class T> void write(void* p, U off, T value) {
    std::memcpy(static_cast<unsigned char*>(p) + off, &value, sizeof value);
}
}
extern "C" void __fastcall kotor_fastcall_005089e0(
    void* self, void*, menu_resources::Ref value) noexcept(false) {
    // 005089E6..9FE: unsigned capacity arithmetic; no invented capacity guard.
    const U capacity = read<U>(self, 8);
    if (read<U>(self, 4) == capacity) {
        recovered_ref_array_grow_00506e20(self, nullptr, capacity ? capacity + capacity : 16u);
    }
    // Original reloads count/data after grow and increments before assignment.
    const U count = read<U>(self, 4);
    const U data = read<U>(self, 0);
    auto* destination = reinterpret_cast<menu_resources::Ref*>(data + (count << 4));
    write<U>(self, 4, count + 1u);
    menu_resources::Ref result;
    (void)ref_assign_copy_00406120(destination, nullptr, &result, &value);
}
extern "C" void* __fastcall kotor_fastcall_00605fa0(
    void* self, void*, W4Vector* position) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    void* server = read<void*>(manager, 8);
    const I index = kotor_fastcall_004aeaf0(
        server, nullptr, static_cast<unsigned char*>(self) + 0x10c, position, 0);
    if (index == -1) return nullptr;
    // Original reloads rooms after the call; only -1 is the null sentinel.
    const U rooms = read<U>(self, 0x260);
    return reinterpret_cast<void*>(rooms + static_cast<U>(index) * 72u);
}
