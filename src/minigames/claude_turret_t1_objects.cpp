#include "060407249b58-claude_turret_t1_objects.hpp"
#include "75d9d34323f7-startup_string.h"
#include "e999d224bc23-m15_l0.h"
#include <cstring>
#include <new>
static_assert(sizeof(void*) == 4, "KOTOR x86 ABI");
// Canonical selected providers (bound by symbol; see imports.txt). C-linkage seams that sit inside an
// original registered unwind window, or that the reconstruction lets throw, are explicitly potentially throwing.
// 0073D71C is the canonical one-byte empty literal array (0129 warning_literals.cpp), mangled like a const pointer.
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*);
}
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* ref);
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, unsigned group);
}
extern "C" {
// Same callable type as canonical dependency-equal/ref_string_joins.h, declared here with an explicit potentially-throwing
// specification: that header's unspecified declaration is treated as nounwind under /EHsc, which drops 0066BFB0 state 0.
int __fastcall async_string_equal_005e5200(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*) noexcept(false);
void* __cdecl live_operator_new(std::uint32_t) noexcept(false);
void __cdecl live_free(void*) noexcept(false);
unsigned __fastcall gui_load_resref_is_valid_00406030(void*, void*) noexcept(false);
std::uint32_t __fastcall kotor_fastcall_005d5920(void* source, void*) noexcept(false);
void __fastcall kotor_fastcall_005d61c0(void* source, void*, const void* ref, std::uint32_t mode) noexcept(false);
void __fastcall kotor_fastcall_005d5950(void* source, void*, std::uint32_t volume) noexcept(false);
std::uint32_t __fastcall kotor_fastcall_005d5930(void* source, void*) noexcept(false);
void* __fastcall kotor_fastcall_0066d5d0(void* follower, void*, void* out) noexcept(false);
// Sibling lane t1-A1 owner (0066D2F0 SetTrack true entry, RET8); declaration only.
void* __fastcall kotor_fastcall_0066d2f0(void* self, void*, void* track, std::uint32_t attachment) noexcept(false);
}
namespace {
using U = std::uint32_t;
using I = std::int32_t;
using String = kotor_reconstruction::StartupString;
using Ref = menu_resources::Ref;
unsigned char* at(void* p, U n) { return static_cast<unsigned char*>(p) + n; }
// Every field access is a fresh 32-bit load/store, matching the original's re-reads.
U rd(void* p, U n) { return *reinterpret_cast<volatile U*>(at(p, n)); }
void wr(void* p, U n, U v) { *reinterpret_cast<volatile U*>(at(p, n)) = v; }
void* rp(void* p, U n) { return *reinterpret_cast<void* volatile*>(at(p, n)); }
void wp(void* p, U n, void* v) { *reinterpret_cast<void* volatile*>(at(p, n)) = v; }
U bits(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class F> F slot(void* object, U offset) { F f; std::memcpy(&f, at(rp(object, 0), offset), 4); return f; }
// The original copies these as raw DWORDs (never through x87).
void* player_cast(void* self) { return slot<void*(__fastcall*)(void*, void*)>(self, 0x18)(self, nullptr); }

// 0066BFB0 registered state 0 covers only the equality call: the scratch returned by GetName is destroyed on unwind.
struct ScratchStringGuard {
    String* scratch; bool armed;
    ~ScratchStringGuard() noexcept(false) { if (armed) scratch->~StartupString(); }
};
// 0066EA4E..0066EA78: unwind state 0 frees the raw allocation if the source constructor throws.
struct AllocationGuard {
    void* memory;
    ~AllocationGuard() noexcept(false) { if (memory) live_free(memory); }
};
String* name_from(void* gob, String* out) {
    // 0066CAD0/0066DC80: Gob+0C GetModelName returns const char* (null when the Gob has no model).
    if (gob) {
        const char* text = slot<const char*(__fastcall*)(void*, void*)>(gob, 0xc)(gob, nullptr);
        ::new (static_cast<void*>(out)) String(text);
        return out;
    }
    ::new (static_cast<void*>(out)) String(base_ini_reconstruction::warning_empty_0073d71c);
    return out;
}
// Shared tail of both SetSoundName index-0 publish paths (0066E989..9DA / 0066EA8C..EAF7): every receiver re-read.
void configure_source(void* self, U slotOffset, U volumeOffset) {
    unsigned char position[12];
    void* returned = kotor_fastcall_0066d5d0(self, nullptr, position);
    kotor_m15_sound_source_set_position_005d59e0(rp(self, slotOffset), nullptr, returned, 0.0f);
    kotor_fastcall_005d5950(rp(self, slotOffset), nullptr, static_cast<U>(*reinterpret_cast<volatile unsigned char*>(at(self, volumeOffset))));
    void* player = player_cast(self);
    kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(self, slotOffset), nullptr, player ? 0x17u : 0x18u);
}
}

extern "C" void* __fastcall kotor_fastcall_0066bfb0(void* self, void*, String* name) noexcept(false) {
    for (U i = 0; i < 255u; ++i) {
        void* object = rp(self, 4u + i * 4u);
        if (!object) continue;
        alignas(String) unsigned char storage[sizeof(String)];
        String* scratch = reinterpret_cast<String*>(storage);
        // Object+04 GetName(hidden out); compare against the pointer it returned, destroy the scratch address.
        String* returned = slot<String*(__fastcall*)(void*, void*, String*)>(object, 4)(object, nullptr, scratch);
        int equal;
        {
            ScratchStringGuard guard{scratch, true};
            equal = async_string_equal_005e5200(name, nullptr, returned);
            guard.armed = false;
        }
        scratch->~StartupString();
        if (equal) return rp(self, 4u + i * 4u);
    }
    return nullptr;
}

extern "C" void __cdecl kotor_cdecl_0066c6a0(void* gob, const char*, void* follower) noexcept(false) {
    // Locals initialised exactly as 0066C6AE..6DE: position zero, orientation words {1.0f,0,0,0}.
    U position[3] = {0u, 0u, 0u};
    U orientation[4] = {0x3f800000u, 0u, 0u, 0u};
    void* part = slot<void*(__fastcall*)(void*, void*, const char*)>(gob, 0x10c)(gob, nullptr, "modelhook");
    // No null-part guard (original unchecked). Part+0C(position out, orientation out); EAX ignored.
    (void)slot<U(__fastcall*)(void*, void*, U, U)>(part, 0xc)(part, nullptr, bits(position), bits(orientation));
    wr(follower, 0x234, position[0]);
    wr(follower, 0x238, position[1]);
    wr(follower, 0x23c, position[2]);
}

extern "C" void __fastcall kotor_fastcall_0066ca90(void* self, void*, I index, I volume) noexcept(false) {
    if (index < 0 || index >= 2) return;
    wr(self, 0x158u + static_cast<U>(index) * 12u, static_cast<U>(volume));
    // Only source 0 exists (+144); no null-source guard here (SetVolume tests its own internal pointer).
    if (index < 1) kotor_fastcall_005d5950(rp(self, 0x144u + static_cast<U>(index) * 4u), nullptr, static_cast<U>(volume));
}

extern "C" String* __fastcall kotor_fastcall_0066cad0(void* self, void*, String* out) noexcept(false) {
    return name_from(rp(self, 0x64), out);
}

extern "C" String* __fastcall kotor_fastcall_0066dc80(void* self, void*, String* out) noexcept(false) {
    return name_from(rp(self, 0x60), out);
}

extern "C" void* __fastcall kotor_fastcall_0066d280(void* self, void*, void* model) noexcept(false) {
    const I count = static_cast<I>(rd(self, 0x6c));
    if (count <= 0) return nullptr;
    void* entries = rp(self, 0x68);
    I index = 0;
    while (rp(entries, static_cast<U>(index) * 8u) != model) {
        if (++index >= count) return nullptr;
    }
    const I remaining = static_cast<I>(rd(self, 0x6c) - 1u);
    wr(self, 0x6c, static_cast<U>(remaining));
    if (index < remaining) {
        do {
            // Fresh base each pass; both DWORDs (pointer and flag/padding word) move in order.
            void* base = rp(self, 0x68);
            const U offset = static_cast<U>(index) * 8u;
            wr(base, offset, rd(base, offset + 8u));
            wr(base, offset + 4u, rd(base, offset + 12u));
            ++index;
        } while (index < static_cast<I>(rd(self, 0x6c)));
    }
    // Gob+50 AttachToObject(null, null, 0) detaches the removed model.
    (void)slot<U(__fastcall*)(void*, void*, void*, const char*, U)>(model, 0x50)(model, nullptr, nullptr, nullptr, 0u);
    return model;
}

extern "C" void* __fastcall kotor_fastcall_0066e280(void* self, void*, void* track, U attachment) noexcept(false) {
    void* current = rp(self, 0x64);
    if (track == current) return current;
    return kotor_fastcall_0066d2f0(self, nullptr, track, attachment);
}

extern "C" void* __fastcall kotor_fastcall_0066e900(void* self, void*, I index, const Ref* ref) noexcept(false) {
    if (index < 0) return nullptr;
    if (index >= 1) {
        if (index >= 2) return nullptr;
        // Index 1 only records the name (+148); there is no second source.
        Ref discarded;
        resource_ref_assignment::assign_00406120(reinterpret_cast<Ref*>(at(self, 0x138u + static_cast<U>(index) * 16u)), nullptr, &discarded, ref);
        return nullptr;
    }
    const U slotOffset = 0x144u + static_cast<U>(index) * 4u;
    const U volumeOffset = 0x158u + static_cast<U>(index) * 12u;
    void* existing = rp(self, slotOffset);
    if (existing) {
        const U playing = kotor_fastcall_005d5920(existing, nullptr);
        if (playing) kotor_m15_sound_source_stop_005d5a20(rp(self, slotOffset), nullptr);
        const unsigned valid = gui_load_resref_is_valid_00406030(const_cast<Ref*>(ref), nullptr);
        void* current = rp(self, slotOffset);
        if (!valid) {
            // Invalid name: scalar-deleting destructor (slot 0, flag 1), then clear and return the re-read slot.
            if (current) (void)slot<U(__fastcall*)(void*, void*, U)>(current, 0)(current, nullptr, 1u);
            wp(self, slotOffset, nullptr);
            return rp(self, slotOffset);
        }
        kotor_fastcall_005d61c0(current, nullptr, ref, 1u);
        kotor_m15_sound_source_set_3d_005d5910(rp(self, slotOffset), nullptr, 1u);
        configure_source(self, slotOffset, volumeOffset);
        if (playing) (void)kotor_fastcall_005d5930(rp(self, slotOffset), nullptr);
        return rp(self, slotOffset);
    }
    if (!gui_load_resref_is_valid_00406030(const_cast<Ref*>(ref), nullptr)) return rp(self, slotOffset);
    void* memory = live_operator_new(8u);
    void* created;
    {
        AllocationGuard guard{memory};
        created = memory ? kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, ref) : nullptr;
        guard.memory = nullptr;
    }
    wp(self, slotOffset, created);
    // Set3D receives the constructed pointer directly (not re-read); a failed allocation reaches it as null.
    kotor_m15_sound_source_set_3d_005d5910(created, nullptr, 1u);
    configure_source(self, slotOffset, volumeOffset);
    return rp(self, slotOffset);
}

extern "C" void __fastcall kotor_fastcall_006706c0(void* self, void*, const char* name, float speed, U flags) noexcept(false) {
    void* model = rp(self, 0x60);
    if (!model) return;
    // Gob+18 PlayAnimation(name, speed, flags, 0); the speed word is forwarded as raw bits.
    U speedBits;
    std::memcpy(&speedBits, &speed, 4);
    (void)slot<U(__fastcall*)(void*, void*, const char*, U, U, U)>(model, 0x18)(model, nullptr, name, speedBits, flags, 0u);
}
