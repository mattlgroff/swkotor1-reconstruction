#pragma once
#include <cstddef>
#include <cstdint>

namespace kotor_reconstruction {
// Fixed x86 data-pointer representation; preserves unchecked 32-bit address arithmetic.
// Only these removal instantiations are recovered. No allocation or element lifetime.
struct ArrayMutations {
    volatile std::uint32_t data;
    volatile std::int32_t size;
    volatile std::int32_t capacity;
    void __thiscall RemoveReverse32(std::uint32_t value);
    void __thiscall RemoveAtTag40(std::int32_t index);
    void __thiscall RemoveAtCombat208(std::int32_t index);
    void __thiscall RemoveFirst16(std::uint16_t value);
    void __thiscall RemoveFirst32(std::int32_t value);
    void __thiscall DeleteAt32(std::int32_t index);
};
static_assert(sizeof(void*) == 4, "x86 ABI required");
static_assert(sizeof(ArrayMutations) == 12 && offsetof(ArrayMutations, size) == 4 &&
    offsetof(ArrayMutations, capacity) == 8, "verified array prefix");
}
