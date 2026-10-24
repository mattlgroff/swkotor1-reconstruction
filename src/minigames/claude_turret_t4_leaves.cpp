// Original K1 minigame gun/behavior leaves. No new storage or heap owner.
#include "be4e236340a2-claude_turret_t4_leaves.hpp"
#include <cstring>

namespace {
using U = std::uint32_t;
static_assert(sizeof(void*) == 4, "KOTOR x86 ABI");
const double zero = 0.0;
const double minus_one = -1.0;

// FCOMP, unlike UCOMISS, raises invalid for both quiet and signaling NaNs.
// Explicit pointers avoid MS asm's reference-operand ambiguity. No inline CALL.
__declspec(noinline) unsigned short compare_x87(const float* value, const double* limit) {
    unsigned short status;
    __asm {
        mov eax, value
        mov ecx, limit
        fld dword ptr [eax]
        fcomp qword ptr [ecx]
        fnstsw ax
        mov status, ax
    }
    return status;
}

unsigned char* bytes(void* self) { return static_cast<unsigned char*>(self); }
U word(const void* self, U offset) {
    U result;
    std::memcpy(&result, static_cast<const unsigned char*>(self) + offset, sizeof result);
    return result;
}
void copy_input(void* self, U offset, const float* value) {
    std::memcpy(bytes(self) + offset, value, sizeof *value);
}
float result_at(void* self, U offset) {
    float result;
    std::memcpy(&result, bytes(self) + offset, sizeof result);
    return result;
}
float nonnegative(void* self, U offset, const float* value) {
    if ((compare_x87(value, &zero) & 0x0100u) == 0) copy_input(self, offset, value);
    return result_at(self, offset);
}
float spread(void* self, U offset, const float* value) {
    bool accept = (compare_x87(value, &zero) & 0x0100u) == 0;
    if (!accept) {
        // Original TEST AH,44h / JP: only ordered equality has odd parity.
        accept = (compare_x87(value, &minus_one) & 0x4400u) == 0x4000u;
    }
    if (accept) copy_input(self, offset, value);
    return result_at(self, offset);
}
}

extern "C" void* __fastcall kotor_fastcall_0066d240(void* self, void*, std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(word(self, 0x78))) return nullptr;
    const auto table = reinterpret_cast<const void*>(word(self, 0x74));
    return reinterpret_cast<void*>(word(table, static_cast<U>(index) * 4));
}
extern "C" float __fastcall kotor_fastcall_00673840(void* self, void*, float value) {
    return nonnegative(self, 0x44, &value);
}
extern "C" float __fastcall kotor_fastcall_00673860(void* self, void*, float value) {
    if ((compare_x87(&value, &zero) & 0x4100u) == 0) copy_input(self, 0x4c, &value);
    return result_at(self, 0x4c);
}
extern "C" float __fastcall kotor_fastcall_00673970(void* self, void*, float value) {
    return nonnegative(self, 0x70, &value);
}
extern "C" float __fastcall kotor_fastcall_00673990(void* self, void*, float value) {
    return spread(self, 0x74, &value);
}
extern "C" float __fastcall kotor_fastcall_006739c0(void* self, void*, float value) {
    return spread(self, 0x6c, &value);
}
extern "C" float __fastcall kotor_fastcall_006739f0(void* self, void*, float value) {
    return nonnegative(self, 0x78, &value);
}
extern "C" void* __fastcall kotor_fastcall_00673a30(void* self, void*) {
    auto slots = static_cast<volatile U*>(self);
    slots[0] = 0;
    slots[2] = 0;
    slots[1] = 0;
    slots[3] = 0;
    return self;
}
extern "C" U __fastcall kotor_fastcall_006dae60(void*, void*) { return 0xaaad; }
extern "C" U __fastcall kotor_fastcall_006db080(void*, void*) { return 0xaaab; }
extern "C" U __fastcall kotor_fastcall_006db580(void*, void*) { return 0xaaac; }
extern "C" U __fastcall kotor_fastcall_006db600(void*, void*) { return 0xaaaa; }
