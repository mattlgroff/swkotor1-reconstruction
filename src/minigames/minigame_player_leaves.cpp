#include "5f558436913e-minigame_player_leaves.hpp"

#include <cstddef>
#include <cstring>

namespace {
template<class T> T load(const void* base, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset,
                sizeof(value));
    return value;
}
template<class T> void store(void* base, std::size_t offset, T value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof(value));
}

// Use the original x87 comparison, including its invalid/status behavior for
// unordered inputs. An SSE C++ comparison can have different exception/DAZ rules.
__declspec(noinline) bool nonnegative_x87(float value) {
    const float* operand = &value;
    const float zero = 0.0f;
    unsigned short status;
    __asm {
        mov eax, operand
        fld dword ptr [eax]
        fcomp zero
        fnstsw status
    }
    return (status & 0x0100u) == 0;
}
}

// 0066CF20: signed comparisons accept -1 or positive values, never zero.
extern "C" void __fastcall kotor_fastcall_0066cf20(
    void* self, void*, std::int32_t loops) {
    if (loops >= -1 && loops != 0) {
        store(self, 0x80, loops);
    }
}

// 0066CF40/70/A0: fcomp zero; test C0 rejects negative and unordered values.
// The return always reloads the field, even when the supplied value is rejected.
extern "C" float __fastcall kotor_fastcall_0066cf40(
    void* self, void*, float value) {
    if (nonnegative_x87(value)) {
        store(self, 0x1D8, value);
    }
    return load<float>(self, 0x1D8);
}

extern "C" float __fastcall kotor_fastcall_0066cf70(
    void* self, void*, float value) {
    if (nonnegative_x87(value)) {
        store(self, 0x1DC, value);
    }
    return load<float>(self, 0x1DC);
}

extern "C" float __fastcall kotor_fastcall_0066cfa0(
    void* self, void*, float value) {
    if (nonnegative_x87(value)) {
        store(self, 0x1E0, value);
    }
    return load<float>(self, 0x1E0);
}

// 0066CFD0: the first stack argument is the hidden Vec3 result pointer.
// Do not turn these sequential loads/stores into a preloaded Vec3 or memcpy:
// input, result and fields within self can alias, and the flag is read before
// the third origin component is copied.
extern "C" void* __fastcall kotor_fastcall_0066cfd0(
    void* self, void*, void* result, const void* value) {
    store(self, 0x208, load<std::uint32_t>(value, 0));
    store(self, 0x20C, load<std::uint32_t>(value, 4));
    const bool retain_offset = (load<unsigned char>(self, 0x88) & 1u) != 0;
    store(self, 0x210, load<std::uint32_t>(value, 8));
    if (!retain_offset) {
        store(self, 0x1C4, load<std::uint32_t>(value, 0));
        store(self, 0x1C8, load<std::uint32_t>(value, 4));
        store(self, 0x1CC, load<std::uint32_t>(value, 8));
    }
    store(result, 0, load<std::uint32_t>(self, 0x208));
    store(result, 4, load<std::uint32_t>(self, 0x20C));
    store(result, 8, load<std::uint32_t>(self, 0x210));
    return result;
}

// 0066D090: the original cdq/xor/sub absolute-value check accepts only +/-1.
// Direct equality also preserves INT_MIN rejection without signed C++ overflow.
extern "C" void __fastcall kotor_fastcall_0066d090(
    void* self, void*, std::int32_t loops) {
    if (loops == -1 || loops == 1) {
        store(self, 0x80, loops);
    }
}

// 00670B10: enemy vtable 007528A8, slot +3C. No object memory is read.
extern "C" std::uint32_t __fastcall kotor_fastcall_00670b10(void*, void*) {
    return 2;
}
