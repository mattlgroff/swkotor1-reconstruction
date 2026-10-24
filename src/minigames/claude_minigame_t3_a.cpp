// Shared turret/swoop controller leaves. Original addresses are identities only.
// No allocation, callbacks, globals, or range guards beyond the original.
#include "4f65a6036765-claude_minigame_t3_a.hpp"
#include <cstring>
#include <cstddef>
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
extern "C" void __fastcall kotor_fastcall_00670b30(
    void* self, void*, void* near_out, void* far_out) {
    // 00670B30..4A: the second load follows the first potentially aliasing store.
    if (near_out) write<U>(near_out, 0, read<U>(self, 0x68));
    if (far_out) write<U>(far_out, 0, read<U>(self, 0x6c));
}
extern "C" U __fastcall kotor_fastcall_00670c10(void* self, void*) {
    const U mode = read<U>(self, 0x80);
    return mode == 1 ? 1u : (mode == 2 ? 2u : 0u);
}
extern "C" void* __fastcall kotor_fastcall_006715b0(
    void* self, void*, void* enemy) {
    // 006715B5 executes even when the list count is nonpositive. Volatile keeps
    // that observable raw access before the count test; it is not a null guard.
    const U id = *reinterpret_cast<const volatile U*>(
        static_cast<const unsigned char*>(enemy) + 8);
    const I count = read<I>(self, 0x30);
    I index = 0;
    if (count <= 0) return nullptr;
    const auto* scan = read<const unsigned char*>(self, 0x2c);
    while (read<U>(scan, 0) != id) {
        ++index;
        scan += 4;
        if (index >= count) return nullptr;
    }
    // Original's index==-1 check cannot be reached from this nonnegative scan.
    // It remains explicit to retain its branch without imposing a new domain.
    if (index == -1) return nullptr;
    write<I>(self, 0x30, read<I>(self, 0x30) - 1);
    while (index < read<I>(self, 0x30)) {
        auto* array = read<unsigned char*>(self, 0x2c);
        const U next = read<U>(array, static_cast<U>(index) * 4u + 4u);
        write<U>(array, static_cast<U>(index) * 4u, next);
        ++index;
    }
    return enemy;
}
extern "C" void* __fastcall kotor_fastcall_00671610(
    void* self, void*, I index) {
    if (index >= read<I>(self, 0x3c)) return nullptr;
    const auto* array = read<const unsigned char*>(self, 0x38);
    // Original signed comparison deliberately permits negative indexing.
    return read<void*>(array + static_cast<std::ptrdiff_t>(index) * 4, 0);
}
extern "C" I __fastcall kotor_fastcall_00671bc0(void* self, void*, I axis) {
    if (axis >= 0 && axis <= 3) write<I>(self, 0x84, axis);
    return read<I>(self, 0x84);
}
extern "C" I __fastcall kotor_fastcall_00671be0(void* self, void*, I axis) {
    if (axis >= 0 && axis <= 3) write<I>(self, 0x88, axis);
    return read<I>(self, 0x88);
}

