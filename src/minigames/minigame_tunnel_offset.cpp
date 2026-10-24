#include "0a911a07647c-minigame_tunnel_offset.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

// Reuses the three-axis flow and live field offsets of the source-only donor
// pi-three-bodies/source/mini_three_bodies_providers.cpp (0066CB40).
// Its C++ numeric comparisons/arithmetic are replaced with the original x87
// operations so unordered status, extended bounds and scratch widths survive.
// Donor origin/provenance and unchanged-source hash are recorded in INTEGRATION.
namespace {
template<class T> T load(const void* base, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof(value));
    return value;
}
template<class T> void store(void* base, std::size_t offset, T value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof(value));
}

__declspec(noinline) bool ordered_zero(const void* cell) {
    const float zero = 0.0f;
    unsigned short status;
    __asm {
        mov eax, cell
        fld dword ptr [eax]
        fcomp zero
        fnstsw status
    }
    // Original test AH,44h / JNP: only ordered equality has odd parity.
    return (status & 0x4400u) == 0x4000u;
}

__declspec(noinline) bool clamp_upper(void* value, const void* bound,
                                     const void* origin) {
    unsigned stored = 0;
    __asm {
        mov eax, bound
        mov ecx, origin
        mov edx, value
        fld dword ptr [eax]
        fadd dword ptr [ecx]
        fcom dword ptr [edx]
        fnstsw ax
        test ah, 5
        jp upper_skip
        fstp dword ptr [edx]
        mov stored, 1
        jmp upper_done
    upper_skip:
        fstp st(0)
    upper_done:
    }
    return stored != 0;
}

__declspec(noinline) void clamp_lower(void* value, const void* bound,
                                     const void* origin) {
    __asm {
        mov eax, bound
        mov ecx, origin
        mov edx, value
        fld dword ptr [eax]
        fadd dword ptr [ecx]
        fcom dword ptr [edx]
        fnstsw ax
        test ah, 41h
        jne lower_skip
        fstp dword ptr [edx]
        jmp lower_done
    lower_skip:
        fstp st(0)
    lower_done:
    }
}

__declspec(noinline) void wrap_angle(void* value) {
    const float zero = 0.0f;
    const float circle = 360.0f;
    const float maximum = 359.0f;
    __asm {
        mov edx, value
        fld dword ptr [edx]
        fcomp zero
        fld dword ptr [edx]
        fnstsw ax
        test ah, 5
        jp wrap_high
        fadd circle
        fstp dword ptr [edx]
        jmp wrap_done
    wrap_high:
        fcomp maximum
        fnstsw ax
        test ah, 41h
        jne wrap_done
        fld dword ptr [edx]
        fsub circle
        fstp dword ptr [edx]
    wrap_done:
    }
}

void axis(unsigned char* self, void* value, std::size_t component) {
    // Only the wrap branch reads the owning minigame pointer. The clamp branch
    // remains valid with a null self+60; wrap retains the original raw fault.
    if (ordered_zero(self + 0x1FC + component)) {
        if (!clamp_upper(value, self + 0x1E4 + component,
                         self + 0x208 + component)) {
            clamp_lower(value, self + 0x1F0 + component,
                        self + 0x208 + component);
        }
    } else {
        const void* mini = load<void*>(self, 0x60);
        if (load<std::uint32_t>(mini, 0x80) == 2u) {
            wrap_angle(value);
        }
    }
}
}

extern "C" void __fastcall kotor_fastcall_0066cb40(void* self, void*, void* value) {
    auto* player = static_cast<unsigned char*>(self);
    auto* vector = static_cast<unsigned char*>(value);
    axis(player, vector, 0);
    axis(player, vector + 4, 4);
    axis(player, vector + 8, 8);
}

extern "C" void* __fastcall kotor_fastcall_0066ccf0(
    void* self, void*, void* result, const void* value) {
    store(self, 0x1C4, load<std::uint32_t>(value, 0));
    store(self, 0x1C8, load<std::uint32_t>(value, 4));
    store(self, 0x1CC, load<std::uint32_t>(value, 8));
    kotor_fastcall_0066cb40(self, nullptr,
                          static_cast<unsigned char*>(self) + 0x1C4);
    store(result, 0, load<std::uint32_t>(self, 0x1C4));
    store(result, 4, load<std::uint32_t>(self, 0x1C8));
    store(result, 8, load<std::uint32_t>(self, 0x1CC));
    return result;
}
