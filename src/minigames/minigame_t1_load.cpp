#include "368380493698-minigame_t1_load.hpp"
#include "d1ffedc48c11-gff_readers.h"
#include "e999d224bc23-m15_l0.h"

// Existing peer owners; no replacement bodies or storage in this unit.
extern "C" {
void __fastcall kotor_fastcall_0066c370(void*, void*) noexcept(false);
void* __fastcall kotor_fastcall_0066e900(void*, void*, std::int32_t,
                                      const menu_resources::Ref*) noexcept(false);
std::int32_t __fastcall kotor_fastcall_0066e490(void*, void*, void*) noexcept(false);
}

namespace {
using Word = std::uint32_t;
using Signed = std::int32_t;
using Ref = menu_resources::Ref;
using Gff = gui_load::Gff;
using Struct = gui_load::Struct;
using Table = const void* const*;
static_assert(sizeof(Ref) == 16 && sizeof(Struct) == 4 &&
              sizeof(task17_gff::List) == 20, "original GFF storage");

template<class T> T read_at(void* object, unsigned offset) {
    return *reinterpret_cast<volatile T*>(static_cast<unsigned char*>(object) + offset);
}
template<class T> void write_at(void* object, unsigned offset, T value) {
    *reinterpret_cast<volatile T*>(static_cast<unsigned char*>(object) + offset) = value;
}
Table table_of(void* object) { return read_at<Table>(object, 0); }
template<class F> F slot(Table table, unsigned offset) {
    return reinterpret_cast<F>(const_cast<void*>(
        *reinterpret_cast<const void* const volatile*>(
            reinterpret_cast<const unsigned char*>(table) + offset)));
}
using SetScript = void (__fastcall*)(void*, void*, Signed, const Ref*);
using LoadScripts = void (__fastcall*)(void*, void*, void*, void*, void*);
using SetLooping = void (__fastcall*)(void*, void*, Signed);
using LoadGun = Signed (__fastcall*)(void*, void*, void*, void*);

// The table pointer is captured BEFORE the GFF read; the entry itself is read
// afterwards, exactly as the original CALL [saved_table+10].
void read_script(void* self, Gff* gff, Struct* scripts, Signed index,
                 const char* label) {
    Ref fallback;
    fallback.construct_00406d80("");
    Table table = table_of(self);
    Ref result;
    Word success;
    Ref* returned = gui_load_gff_resref_00411e10(
        gff, nullptr, &result, scripts, label, &success, &fallback);
    slot<SetScript>(table, 0x10)(self, nullptr, index, returned);
}
}

extern "C" {
void __fastcall kotor_fastcall_0066c420(void* self, void*, void* raw_gff,
                                      void*, void* raw_scripts) noexcept(false) {
    auto* gff = static_cast<Gff*>(raw_gff);
    auto* scripts = static_cast<Struct*>(raw_scripts);
    read_script(self, gff, scripts, 0, "OnCreate");
    read_script(self, gff, scripts, 1, "OnHitBullet");
    read_script(self, gff, scripts, 2, "OnHitFollower");
    read_script(self, gff, scripts, 3, "OnAnimEvent");
    read_script(self, gff, scripts, 4, "OnHeartbeat");
}

void __fastcall kotor_fastcall_0066d180(void* self, void*, void* gff,
                                      void* parent, void* scripts) noexcept(false) {
    kotor_fastcall_0066c420(self, nullptr, gff, parent, scripts);
}

void __fastcall kotor_fastcall_0066c740(void* self, void*, void* raw_gff,
                                      void* parent, void* raw_scripts) noexcept(false) {
    kotor_fastcall_0066c420(self, nullptr, raw_gff, parent, raw_scripts);
    auto* gff = static_cast<Gff*>(raw_gff);
    auto* scripts = static_cast<Struct*>(raw_scripts);
    read_script(self, gff, scripts, 5, "OnDamage");
    read_script(self, gff, scripts, 6, "OnDeath");
    read_script(self, gff, scripts, 7, "OnFire");
    read_script(self, gff, scripts, 8, "OnHitObstacle");
    read_script(self, gff, scripts, 9, "OnTrackLoop");
}

Signed __fastcall kotor_fastcall_0066d0b0(void* self, void*, void* raw_gff,
                                       void* raw_parent) noexcept(false) {
    auto* gff = static_cast<Gff*>(raw_gff);
    auto* parent = static_cast<Struct*>(raw_parent);
    Struct scripts;
    if (gui_load_gff_struct_00411a10(gff, nullptr, &scripts, parent, "Scripts")) {
        slot<LoadScripts>(table_of(self), 8)(self, nullptr, raw_gff, raw_parent, &scripts);
    }
    kotor_fastcall_0066c370(self, nullptr);
    return 1;
}

void __fastcall kotor_fastcall_0066f7e0(void* self, void*, void* raw_gff,
                                      void*, void* raw_sounds) noexcept(false) {
    auto* gff = static_cast<Gff*>(raw_gff);
    auto* sounds = static_cast<Struct*>(raw_sounds);
    Ref fallback;
    fallback.construct_00406d80("");
    Ref result;
    Word success;
    Ref* returned = gui_load_gff_resref_00411e10(
        gff, nullptr, &result, sounds, "Engine", &success, &fallback);
    void* sound = kotor_fastcall_0066e900(self, nullptr, 0, returned);
    if (sound) kotor_m15_sound_source_set_looping_005d59d0(sound, nullptr, 1);
    fallback.construct_00406d80("");
    returned = gui_load_gff_resref_00411e10(
        gff, nullptr, &result, sounds, "Death", &success, &fallback);
    kotor_fastcall_0066e900(self, nullptr, 1, returned);
}

Signed __fastcall kotor_fastcall_0066fff0(void* self, void*, void* raw_gff,
                                       void* raw_parent) noexcept(false) {
    auto* gff = static_cast<Gff*>(raw_gff);
    auto* parent = static_cast<Struct*>(raw_parent);
    Word success;
    Word hp = gui_load_gff_dword_00411c20(gff, nullptr, parent, "Hit_Points", &success, 0);
    if (static_cast<Signed>(hp) > 0) write_at<Word>(self, 0x8c, hp);
    hp = gui_load_gff_dword_00411c20(gff, nullptr, parent, "Max_HPs", &success, 0);
    if (static_cast<Signed>(hp) > 0) write_at<Word>(self, 0x90, hp);
    float radius = gui_load_gff_float_00411d00(gff, nullptr, parent, "Sphere_Radius", &success, -1.0f);
    if (radius >= 0.0f) write_at<float>(self, 0x84, radius);

    Table table = table_of(self); // 0067006B precedes the INT reader.
    Signed loops = gui_load_gff_int_00411c90(gff, nullptr, parent, "Num_Loops", &success, -10);
    slot<SetLooping>(table, 0x38)(self, nullptr, loops);
    Signed damage = gui_load_gff_int_00411c90(gff, nullptr, parent, "Bump_Damage", &success, 0);
    write_at<Signed>(self, 0x94, damage);
    float period = gui_load_gff_float_00411d00(gff, nullptr, parent, "Invince_Period", &success, 0.0f);
    if (period >= 0.0f) write_at<float>(self, 0xa0, period);

    task17_gff::List banks;
    if (gff_list_004118c0(gff, nullptr, &banks, parent, "Gun_Banks")) {
        Signed count = static_cast<Signed>(gff_list_count_00411940(gff, nullptr, &banks));
        for (Signed i = 0; i < count; ++i) {
            Struct entry;
            if (gff_list_element_00411990(gff, nullptr, &entry, &banks, static_cast<Word>(i))) {
                slot<LoadGun>(table_of(self), 0x30)(self, nullptr, raw_gff, &entry);
            }
        }
        // Original TEST count, not count>0: negative counts still attach models.
        if (count != 0) {
            Signed model_count = read_at<Signed>(self, 0x6c);
            for (Signed i = 0; i < model_count; ++i) {
                void* models = read_at<void*>(self, 0x68);
                void* model = read_at<void*>(models, static_cast<Word>(i) * 8u);
                kotor_fastcall_0066e490(self, nullptr, model);
            }
        }
    }
    Struct child;
    if (gui_load_gff_struct_00411a10(gff, nullptr, &child, parent, "Scripts")) {
        slot<LoadScripts>(table_of(self), 8)(self, nullptr, raw_gff, raw_parent, &child);
    }
    if (gui_load_gff_struct_00411a10(gff, nullptr, &child, parent, "Sounds")) {
        kotor_fastcall_0066f7e0(self, nullptr, raw_gff, raw_parent, &child);
    }
    kotor_fastcall_0066c370(self, nullptr);
    return 1;
}
}
