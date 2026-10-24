#include "0720af487ac1-minigame_camera_hook.hpp"
#include <cstring>
extern "C" std::uint32_t __fastcall kotor_fastcall_00670c10(void*, void*);
namespace {
template<class T> T load(const void* p, unsigned offset) {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + offset, sizeof v); return v;
}
template<class T> void store(void* p, unsigned offset, T v) {
    std::memcpy(static_cast<unsigned char*>(p) + offset, &v, sizeof v);
}
using Delete = std::uint32_t (__fastcall*)(void*, void*, std::uint32_t);
using Attach = void (__fastcall*)(void*, void*, void*, const char*, std::uint32_t);
}
extern "C" void __fastcall kotor_fastcall_0066d030(
    void* self, void*, void* hook, std::uint8_t attach) {
    const std::uint8_t original_attach = attach;
    const std::uint32_t type = original_attach != 0
        ? kotor_fastcall_00670c10(load<void*>(self, 0x60), nullptr) : 0;
    void* const old_hook = load<void*>(self, 0x220);
    if (old_hook != nullptr) {
        void* const table = load<void*>(old_hook, 0);
        load<Delete>(table, 0)(old_hook, nullptr, 1);
    }
    // Deliberately no old_hook == hook guard: original deletes before storing.
    store(self, 0x220, hook);
    store(self, 0x224, original_attach);
    if (hook != nullptr) {
        void* const parent = load<void*>(self, 0x64);
        if (parent != nullptr) {
            void* const table = load<void*>(hook, 0);
            load<Attach>(table, 0x50)(hook, nullptr, parent, "modelhook", type);
        }
    }
}
