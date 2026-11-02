#include "1c46e7f795-nearest_enemy.h"
#include <cstring>
static_assert(sizeof(void*) == 4, "NearestEnemy requires the original x86 pointer ABI");

namespace {
using U = std::uint32_t;
using I = std::int32_t;
template<class T> T read(const void* p, U offset) {
    T value; std::memcpy(&value, static_cast<const unsigned char*>(p) + offset, sizeof value); return value;
}
template<class T> void write(void* p, U offset, T value) {
    std::memcpy(static_cast<unsigned char*>(p) + offset, &value, sizeof value);
}
void* pointer(const void* p, U offset) { return read<void*>(p, offset); }
I signed_bits(U value) { I result; std::memcpy(&result, &value, sizeof result); return result; }
void* server_app() { return pointer(kotor_menu_window_cells::manager_007a39fc, 8); }
void* cast(void* object, U offset) {
    using Getter = void* (__fastcall*)(void*, void*);
    return read<Getter>(pointer(object, 0), offset)(object, nullptr);
}
U dead(void* creature, U /*original fault site*/) { // null creature: original faults on the vtable read below
    using Getter = U (__fastcall*)(void*, void*);
    return read<Getter>(pointer(creature, 0), 0x94)(creature, nullptr);
}
void copy_position(void* out, const void* object) {
    write<U>(out, 0, read<U>(object, 0x90));
    write<U>(out, 4, read<U>(object, 0x94));
    write<U>(out, 8, read<U>(object, 0x98));
}
// a2 (L3 t1): the inline-asm helpers are C++ under the writing rule (register values long double, one (float)
// conversion per original fst/fstp dword; the compares are the original fcomp + test ah,mask outcomes). The original
// has these blocks inline in 004F2DE0 (the helpers are calls under /Ob0).
__forceinline long double ld_word(U w) { float f; std::memcpy(&f, &w, 4); return f; }   // fld dword
__forceinline U float_word(float f) { U w; std::memcpy(&w, &f, 4); return w; }
// 004F3052..004F305D / 004F3327..004F332C: vector_magnitude(difference) (cdecl, result in ST0), fstp dword [esp+20h].
U distance_bits(const float* difference) {
    const float distance = kotor_cdecl_004ab100(difference);
    return float_word(distance);
}
// 004F3078..004F3096 / 004F3347..004F3365: fld [distance] ; fsub [self radius] ; fsub [candidate radius] ; fst dword
// [adjusted] (ST0 kept) ; fcomp dword [*best] ; test ah,5 ; jp skip -> proceeds only when the UNROUNDED adjusted value
// compares ordered-less than *best (C0 alone).
U subtract_radii_less(U distance, U self_radius, U candidate_radius, U threshold, U* stored_distance) {
    const long double adjusted = (ld_word(distance) - ld_word(self_radius)) - ld_word(candidate_radius);
    *stored_distance = float_word(static_cast<float>(adjusted));          // 004F3089 / 004F3358 fst dword
    return adjusted < ld_word(threshold) ? 1u : 0u;                       // original comparison uses unrounded ST0; accepted threshold uses stored DWORD
}
// 004F30AE..004F30C0 / 004F30E4..004F30F0 (and the backward arm's 004F3377..004F3398 / 004F33BD..004F33C9): fld z ; fadd [0073F414] (1.5) ; fstp dword.
U height_bits(U z) {
    return float_word(static_cast<float>(ld_word(z) + 1.5f));            // original0073F414 DWORD3FC00000
}
// 004F3156..004F316B / 004F342C..004F3441: fld high ; fsub low ; fcomp dword [*best] ; test ah,41h ; jne -> true only when
// the difference is ordered-greater (neither C0 nor C3; unordered does not stop this side).
bool axis_exceeded(U high, U low, U threshold) {
    return (ld_word(high) - ld_word(low)) > ld_word(threshold);
}

// Shared source mechanics for the two original scan arms; native call/reload order remains explicit.
bool inspect_candidate(void* self, void* object, U id, U reference, bool backward,
                       unsigned char* frame, U* best, U* closest) {
    bool hostile;
    if (reference != 0x7F000000u) {
        if (cast(object, 0x30) != nullptr)
            hostile = (recovered_object_ai_state_reputation_0057cd90(cast(object, 0x30), nullptr, reference) & 0xFFu) == 2;
        else hostile = false;
    } else {
        hostile = (recovered_object_ai_state_reputation_0057cd90(self, nullptr, id) & 0xFFu) == 2;
    }
    if (cast(object, 0x30) != nullptr && hostile &&
        dead(cast(object, 0x30), backward ? 0x004F2F6Ful : 0x004F323Eul) == 0 &&
        kotor_m15e_slot010_zero_0063e7f0(cast(object, 0x30), nullptr) == 0 &&
        recovered_creature_incapacitated_004ef890(cast(object, 0x30), nullptr) == 0) {
        void* const element = recovered_creature_visible_element_00517a20(self, nullptr, read<U>(object, 4));
        if (element != nullptr && (read<unsigned char>(element, 4) & 1u) != 0) {
            const U reference_offset = backward ? 0x40u : 0x20u;
            write<U>(frame, reference_offset, 0);
            write<U>(frame, reference_offset + 4, 0);
            write<U>(frame, reference_offset + 8, 0);
            if (reference != 0x7F000000u) {
                void* const reference_creature = kotor_fastcall_004ae770(server_app(), nullptr, reference);
                if (reference_creature != nullptr) copy_position(frame + reference_offset, reference_creature);
            } else copy_position(frame + reference_offset, self);

            void* creature = cast(object, 0x30);
            float* const difference = kotor_cdecl_00423bc0(reinterpret_cast<float*>(frame + (backward ? 0x98 : 0xA4)),
                reinterpret_cast<const float*>(frame + reference_offset),
                reinterpret_cast<const float*>(static_cast<unsigned char*>(creature) + 0x90));
            const U distance = distance_bits(difference); // original FSTP DWORD004F305D/004F332C
            const U self_radius = read<U>(pointer(self, 0x340), 8); // cached before repeated virtual30
            creature = cast(object, 0x30);
            U adjusted;
            if (subtract_radii_less(distance, self_radius, read<U>(pointer(creature, 0x340), 8), *best, &adjusted) != 0) {
                const U from_offset = backward ? 0x64u : 0x8Cu;
                const U to_offset = backward ? 0x70u : 0x54u;
                if (backward) copy_position(frame + from_offset, self);
                else {
                    write<U>(frame, from_offset, read<U>(frame, reference_offset));
                    write<U>(frame, from_offset + 4, read<U>(frame, reference_offset + 4));
                    write<U>(frame, from_offset + 8, read<U>(frame, reference_offset + 8));
                }
                write<U>(frame, from_offset + 8, height_bits(read<U>(frame, from_offset + 8)));
                creature = cast(object, 0x30);
                copy_position(frame + to_offset, creature);
                write<U>(frame, to_offset + 8, height_bits(read<U>(frame, to_offset + 8)));
                if (recovered_object_area_004cb120(self, nullptr) != nullptr) {
                    const U self_id = read<U>(self, 4); // cache before second GetArea004F3128/004F33FE
                    void* const area = recovered_object_area_004cb120(self, nullptr);
                    if (area_clear_line_of_sight_0050c330(area, nullptr,
                        reinterpret_cast<kotor_world_los::Vector*>(frame + from_offset),
                        reinterpret_cast<kotor_world_los::Vector*>(frame + to_offset),
                        reinterpret_cast<kotor_world_los::Vector*>(frame + 0x80),
                        reinterpret_cast<U*>(frame + 0x7C), self_id, id, 1, 0) != 0) {
                        *best = adjusted; *closest = id;
                    }
                }
            }
        }
    }
    if (cast(object, 0x10) != nullptr) {
        void* const base = cast(object, 0x10);
        if (backward) {
            const U self_x = read<U>(self, 0x90);
            const U other_x = read<U>(base, 0x90);
            return axis_exceeded(self_x, other_x, *best);
        }
        const U other_x = read<U>(base, 0x90);
        const U self_x = read<U>(self, 0x90);
        return axis_exceeded(other_x, self_x, *best);
    }
    return false;
}
}

// Original004F2DE0..004F3496,1718bytes, RET0C/nativefastcall@20.
extern "C" std::uint32_t __fastcall kotor_fastcall_004f2de0(void* self, void*, U radius_bits,
                                                         U excluded, U reference) {
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return 0x7F000000u;
    void* const area = recovered_object_area_004cb120(self, nullptr);
    void* const list = static_cast<unsigned char*>(area) + 0x190;
    const I count = read<I>(list, 4);
    U index = 0;
    if (count > 0) {
        const U self_id = read<U>(self, 4);
        const unsigned char* entry = static_cast<const unsigned char*>(pointer(list, 0));
        while (signed_bits(index) < count) {
            if (read<U>(entry, 0) == self_id) break;
            ++index; entry += 4;
        }
    }
    if (signed_bits(index) >= count) return 0x7F000000u;

    U best = radius_bits;   // a2: the radius is a stack word the original copies with mov (not a float parameter: fld/fstp would quiet a signalling NaN)
    U closest = 0x7F000000u;
    U backward_index = index, forward_index = index;
    U backward_offset = index * 4u, forward_offset = index * 4u;
    bool backward_active = true, forward_active = true;
    // Original locals160bytes plus saved-register frame16: highest Vector write A4..AF.
    // Preserve exact shared LOS hit[80..88] and DWORD blocker[7C], not overlapping fabricated vectors.
    alignas(4) unsigned char frame[0xB0];
    write<U>(frame, 0x80, 0); write<U>(frame, 0x84, 0); write<U>(frame, 0x88, 0);
    for (;;) {
        if (signed_bits(backward_index) < 0 && signed_bits(forward_index) >= read<I>(list, 4)) return closest;
        if (backward_active) {
            if (signed_bits(backward_index) < 0) backward_active = false;
            else {
                const U id = read<U>(pointer(list, 0), backward_offset);
                void* const object = kotor_fastcall_004ae750(server_app(), nullptr, id);
                if (object != nullptr && id != read<U>(self, 4) && id != excluded)
                    if (inspect_candidate(self, object, id, reference, true, frame, &best, &closest)) backward_active = false;
            }
        }
        if (forward_active) {
            if (signed_bits(forward_index) >= read<I>(list, 4)) forward_active = false;
            else {
                const U id = read<U>(pointer(list, 0), forward_offset);
                void* const object = kotor_fastcall_004ae750(server_app(), nullptr, id);
                if (object != nullptr && id != read<U>(self, 4) && id != excluded)
                    if (inspect_candidate(self, object, id, reference, false, frame, &best, &closest)) forward_active = false;
            }
        }
        forward_offset += 4; backward_offset -= 4;
        --backward_index; ++forward_index; // original signed tests on wrapping DWORD indices, even when both sides inactive
    }
}
