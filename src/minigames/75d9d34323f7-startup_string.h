#pragma once
#include <cstddef>
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 reconstruction.
#endif
namespace kotor_reconstruction {
// The observed eight-byte CExoString storage, with its actual lifetime entries.
#pragma pack(push,1)
struct StartupString {
    char* volatile data;
    volatile std::uint32_t capacity;
    __declspec(noinline) __thiscall StartupString();
    __declspec(noinline) __thiscall StartupString(const char* volatile input);
    // Support-only lifetime shell; WindowCopy remains the sole algorithm owner.
    __declspec(noinline) __thiscall StartupString(const StartupString& input);
    __declspec(noinline) __thiscall ~StartupString();
    __declspec(noinline) StartupString* __thiscall Assign005e5c50(StartupString* rhs);
    // Adapted reviewed owner methods; no additional equality/length address credit.
    __declspec(noinline) int __thiscall BorrowedEquals005e5290(const char*);
    __declspec(noinline) std::int32_t __thiscall BorrowedLength005e5790();
    __declspec(noinline) int __thiscall ComparePrefix005e5900(StartupString*,std::int32_t);
};
#pragma pack(pop)
static_assert(sizeof(StartupString)==8 && alignof(StartupString)==1 &&
              offsetof(StartupString,capacity)==4,"Observed x86 string layout");
}

#define KOTOR_STARTUP_STRING_OWNED_COPY_LIFETIME 1
