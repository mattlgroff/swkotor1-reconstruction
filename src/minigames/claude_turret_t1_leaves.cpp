#include "d1c462888f42-claude_turret_t1_leaves.hpp"

static_assert(sizeof(void*) == 4, "KOTOR x86 ABI");
namespace {
using U = std::uint32_t;
using I = std::int32_t;
template<class T> volatile T& field(void* self, U offset) {
    return *reinterpret_cast<volatile T*>(reinterpret_cast<std::uintptr_t>(self) + offset);
}
}

extern "C" I __fastcall kotor_fastcall_005cb440(void* self, void*) {
    // 005CB440..45B: FCOMP rather than an SSE or unordered comparison; retains x87 status effects.
    const volatile float* pointer = &field<float>(self, 0x9c);
    const float zero_value = 0.0f;
    unsigned short status;
    __asm {
        mov ecx, pointer
        fld dword ptr [ecx]
        fcomp dword ptr zero_value
        fnstsw ax
        mov status, ax
    }
    return (status & 0x4100u) == 0 ? 1 : 0;
}

extern "C" U __fastcall kotor_fastcall_0066c060(void* self, void*) {
    // 0066C060..09C: 255 slots; a full scan returns255 and restores the cursor by wrapping naturally.
    U index = field<U>(self, 0);
    const U start = index;
    if (field<U>(self, 4u + index * 4u) != 0) {
        do {
            ++index;
            field<U>(self, 0) = index;
            if (index == 255u) field<U>(self, 0) = 0;
            index = field<U>(self, 0);
            if (index == start) return 255u;
        } while (field<U>(self, 4u + index * 4u) != 0);
    }
    return field<U>(self, 0);
}

extern "C" float __fastcall kotor_fastcall_0066c8d0(void* self, void*, float radius) {
    // 0066C8D0..8F1: test C0 only. Negative and unordered inputs preserve the existing field.
    const float zero_value = 0.0f;
    unsigned short status;
    __asm {
        fld dword ptr radius
        fcomp dword ptr zero_value
        fnstsw ax
        mov status, ax
    }
    if ((status & 0x0100u) == 0) field<float>(self, 0x84) = radius;
    return field<float>(self, 0x84);
}

extern "C" I __fastcall kotor_fastcall_0066c900(void* self, void*, I hp) {
    // 0066C900..914: signed strictly-positive input, no clamp to maxHP.
    if (hp > 0) field<I>(self, 0x8c) = hp;
    return field<I>(self, 0x8c);
}

extern "C" float __fastcall kotor_fastcall_0066c920(void* self, void*, float invincibility) {
    // 0066C920..941: same FCOMP/store boundary as SetSphereRadius, distinct destination+A0.
    const float zero_value = 0.0f;
    unsigned short status;
    __asm {
        fld dword ptr invincibility
        fcomp dword ptr zero_value
        fnstsw ax
        mov status, ax
    }
    if ((status & 0x0100u) == 0) field<float>(self, 0xa0) = invincibility;
    return field<float>(self, 0xa0);
}

extern "C" I __fastcall kotor_fastcall_0066c950(void* self, void*, I hp) {
    if (hp > 0) field<I>(self, 0x90) = hp;
    return field<I>(self, 0x90);
}

extern "C" I __fastcall kotor_fastcall_0066c9f0(void* self, void*, I sound) {
    if (sound < 0 || sound >= 2) return 0;
    return field<I>(self, 0x15cu + static_cast<U>(sound) * 12u);
}

extern "C" void __fastcall kotor_fastcall_0066ca10(void* self, void*, I sound, I frequency) {
    if (sound >= 0 && sound < 2) field<I>(self, 0x15cu + static_cast<U>(sound) * 12u) = frequency;
}

extern "C" I __fastcall kotor_fastcall_0066ca30(void* self, void*, I sound) {
    if (sound < 0 || sound >= 2) return 0;
    return field<I>(self, 0x160u + static_cast<U>(sound) * 12u);
}

extern "C" void __fastcall kotor_fastcall_0066ca50(void* self, void*, I sound, I random_flag) {
    if (sound >= 0 && sound < 2) field<I>(self, 0x160u + static_cast<U>(sound) * 12u) = random_flag;
}

extern "C" I __fastcall kotor_fastcall_0066ca70(void* self, void*, I sound) {
    if (sound < 0 || sound >= 2) return 0;
    return field<I>(self, 0x158u + static_cast<U>(sound) * 12u);
}
