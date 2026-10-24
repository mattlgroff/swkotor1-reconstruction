#include "3522bb9da858-claude_turret_t4_refs.hpp"
// Use the existing W4 declaration of the selected menu_resources::Ref type.
#include "8c7ada52dac9-gui_rt.hpp"
#include <cstring>
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*);
}
extern "C" menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);
extern "C" void __fastcall recovered_int_list_remove_0066dcc0(void*, void*, unsigned);
namespace {
using U = unsigned;
static_assert(sizeof(U) == 4 && sizeof(menu_resources::Ref) == 16 && alignof(menu_resources::Ref) == 1, "selected Ref ABI");
unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
void copy_word(void* dst, const void* src) {
    U word;
    std::memcpy(&word, src, 4);
    std::memcpy(dst, &word, 4);
}
void zero_word(void* p) { const U zero = 0; std::memcpy(p, &zero, 4); }
const float zero_float = 0.0f;
// Original FCOMP m32 / FSUB m32 / FSUBR m32 sequence. No CALL in inline asm.
__declspec(noinline) void change_rate(float* timer, float* rate, const float* new_rate) {
    __asm {
        mov ecx, timer
        mov edx, rate
        fld dword ptr [ecx]
        fcomp dword ptr [edx]
        fnstsw ax
        test ah, 5
        jp store_rate
        fld dword ptr [ecx]
        fcomp dword ptr [zero_float]
        fnstsw ax
        test ah, 44h
        jnp store_rate
        fld dword ptr [edx]
        mov edx, new_rate
        fsub dword ptr [edx]
        fsubr dword ptr [ecx]
        fstp dword ptr [ecx]
    store_rate:
        mov ecx, rate
        mov edx, new_rate
        mov eax, dword ptr [edx]
        mov dword ptr [ecx], eax
    }
}
}
extern "C" void __fastcall kotor_fastcall_005cb460(void* self, void*, const menu_resources::Ref* rhs) {
    menu_resources::Ref discarded;
    resource_ref_assignment::assign_00406120(reinterpret_cast<menu_resources::Ref*>(bytes(self) + 4), nullptr, &discarded, rhs);
}
extern "C" void __fastcall kotor_fastcall_006737e0(void* self, void*, const void* bullet) {
    auto source = static_cast<const unsigned char*>(bullet);
    // Separate loads/stores preserve original alias-sensitive sequential copies.
    copy_word(bytes(self) + 0x174, source);
    copy_word(bytes(self) + 0x178, source + 4);
    copy_word(bytes(self) + 0x17c, source + 8);
    copy_word(bytes(self) + 0x180, source + 0xc);
    copy_word(bytes(self) + 0x184, source + 0x10);
    copy_word(bytes(self) + 0x188, source + 0x14);
    menu_resources::Ref discarded;
    resource_ref_assignment::assign_00406120(reinterpret_cast<menu_resources::Ref*>(bytes(self) + 0x18c), nullptr,
        &discarded, reinterpret_cast<const menu_resources::Ref*>(source + 0x18));
}
extern "C" void* __fastcall kotor_fastcall_00673a10(void* self, void*) {
    c2da_default_ref_00405ed0(reinterpret_cast<menu_resources::Ref*>(bytes(self) + 0x18), nullptr);
    zero_word(bytes(self));
    zero_word(bytes(self) + 0x10);
    zero_word(bytes(self) + 4);
    zero_word(bytes(self) + 8);
    zero_word(bytes(self) + 0xc);
    return self;
}
extern "C" void __fastcall kotor_fastcall_0066e110(void* self, void*, unsigned gun_bank) {
    if ((bytes(self)[0x88] & 2u) != 0) recovered_int_list_remove_0066dcc0(bytes(self) + 0x74, nullptr, gun_bank);
}
extern "C" void __fastcall kotor_fastcall_006daf60(void* self, void*, float new_rate) {
    change_rate(reinterpret_cast<float*>(bytes(self) + 0x50), reinterpret_cast<float*>(bytes(self) + 0x4c), &new_rate);
}
extern "C" void __fastcall kotor_fastcall_006dafa0(void* self, void*, float new_rate) {
    change_rate(reinterpret_cast<float*>(bytes(self) + 0x78), reinterpret_cast<float*>(bytes(self) + 0x74), &new_rate);
}
