// t11: CSWCProjectile::AIUpdate 006D7BC0 (table 00757CC8 slot +074) and the two CSWCCreature helpers it reaches.
//   006D7BC0 AIUpdate                       object_projectile_006d7bc0  void fastcall @8   RET 0    (FuncInfo 007862A8)
//   0060D730 impact Gob (creature)          object_projectile_0060d730  U  fastcall @24  RET 10h  (FuncInfo 0077829C)
//   0060D070 hand -> equipped item id       object_projectile_0060d070  U  fastcall @12  RET 4
// Ownership: reassigned by user from the sol projectile lanes, 2026-10-15, PENDING-REGISTRATION.
// Written from the original instructions (static disassembly of swkotor.exe); nothing is carried over from the
// paused lanes' naked transcription. Declarations: shared/object_projectile_internal.h (006D7BC0, 006D7590),
// shared/object_projectile_impact.h (0060D730, 0060D070), unchanged pinned copies. Every callee is a selected
// provider, another projectile export under its exact name, or this tier's 006FC0E0 export.
// x87: the game runs with the CRT default control word 027Fh (53-bit precision, nearest; no game code outside the
// CRT loads a control word). Where C++ arithmetic equals the original's under that control word it is written in
// C++; fsin/fcos, the x87 NaN-selection of the two-NaN additions of the facing dot product and the 80-bit ST0
// transport around 006FC0E0 are minimal inline helpers that execute the original instructions.
#include "f55386dba3-object_projectile_internal.h"
#include "f458a91af3-object_projectile_impact.h"
#include "a9f6c72f90-object_projectile_providers.h"
#include "6bd11f4280-object_projectile_literals.h"
#include "e23e1bf538-crt_acos_006fc0e0.h"
#include "38fba4337f-m15e.h"
#include "8baf7de067-m15e_animbase.h"
#include "f58c269829-scene3d.h"
#include "9963eab06a-vtables_gen.h"
#include "de055aa89e-startup_string.h"
#include "87c3351508-resources.h"
#include "15f9ed81f3-allocator_live.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>
// L3 MATH-TRIG-CPP t1 (2026-10-24): LD unit (recipe + -mno-sse -ffp-model=strict -mlong-double-80); math declarations.
#include "../../math/kotor_math.h"

using U = std::uint32_t;
using B = std::uint8_t;
using S = kotor_reconstruction::StartupString;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Vector;

namespace menu_continuation { struct WorldTimer; }
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* ref);
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, unsigned int group);
}
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString* destination, const char* format, ...); }
extern "C" {
S* __fastcall GuiAssign005e5140(S*, void*, const char*);
extern void* kotor_cell_rules_007a3a28;
extern void* kotor_cell_sound_007a39ec;
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*, void*, U*, U*);
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day, U tick, U last_day, U last_tick, U* out_day, U* out_tick);
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* position, float z_offset);
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, U is3d);
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void* source, void*, U looping);
U __fastcall kotor_fastcall_005d5930(void* source, void*);
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, void* ref, Vector position, float a5, U a6, U a7, U a8, float a9, float a10);
void* __fastcall kotor_fastcall_005ed530(void* client, void*);
double __cdecl quaternion_yaw_004a9f40(const Quaternion* q);
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void* self, void*);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);
U __fastcall frame_R4_005ed5b0(void* client, void*, U id);
U __fastcall kotor_fastcall_0063c380(void* self, void*, U animation);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* self, void*);
U __fastcall recovered_creature_incapacitated_004ef890(void* server_creature, void*);
U __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*, void*);
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, U slot);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, U id);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* self, void*, std::int32_t id);
float __cdecl kotor_m15a_pitch_004aa130(const void* vector);
float __cdecl kotor_m15a_yaw_004aa0f0(const void* vector);
Quaternion* __cdecl yaw_pitch_roll_004acac0(Quaternion* out, float yaw, float pitch, float roll);
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);
void* __fastcall kotor_m15a_combat_attack_data_construct_0060b630(void* self, void*);
void __fastcall kotor_fastcall_004015a0(void* self, void*);
S* __fastcall resman_string_from_int_005e5bc0(S* self, void*, std::int32_t value);
std::int32_t __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row_label, void* column, void* out);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
Vector* __cdecl vector_cross_004ab210(Vector* out, const Vector* p, const Vector* q);
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion* self, void* unused_edx, Vector axis, float angle);
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
}

// The +074 slot type of the canonical CSWCObject table (t10 binds this export at 00757CC8 +074).
static_assert(std::is_same_v<decltype(&object_projectile_006d7bc0), decltype(KotorM15ObjectMethods_007516c8::s074_ai_update)>, "+074");

namespace {
// Original constants not in the t9 literal port (tools/check_literals_t11.py compares each with the image).
constexpr unsigned char kLiteral_00740cbc[4] = {0x00, 0x00, 0x80, 0xbf};          // -1.0f
constexpr char kLiteral_0074f350[6] = "blood";
constexpr char kLiteral_0074f358[16] = "imp_impact_node";

template<class T> T rd(const void* p, U n) { T v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
template<class T> void wr(void* p, U n, T v) { std::memcpy(static_cast<B*>(p) + n, &v, sizeof v); }
void* at(void* p, U n) { return static_cast<B*>(p) + n; }
void* ptr(const void* p, U n) { return rd<void*>(p, n); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
S* string_at(void* p, U n) { return static_cast<S*>(at(p, n)); }
menu_resources::Ref* ref_at(void* p, U n) { return static_cast<menu_resources::Ref*>(at(p, n)); }
U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
void* pointer_of(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
const KotorM15ObjectMethods_007516c8* object_table(const void* o) { return rd<const KotorM15ObjectMethods_007516c8*>(o, 0); }
const aurora_model::GobVTable* gob_table(const void* g) { return rd<const aurora_model::GobVTable*>(g, 0); }
const KotorM15eAnimBaseVtbl* model_table(const void* m) { return rd<const KotorM15eAnimBaseVtbl*>(m, 0); }
aurora_model::Gob* as_gob(void* g) { return static_cast<aurora_model::Gob*>(g); }
// Slot +0 scalar deleting destructor of the CExoSoundSource at [self+150h] (called with 1).
using ScalarDeleting = void* (__fastcall*)(void* self, void* edx, U flags);
void scalar_delete(void* object) { (void)rd<ScalarDeleting>(rd<const void*>(object, 0), 0)(object, nullptr, 1u); }

// [module+40h] is the module camera (005ED530 -> 00644760: mov eax,[ecx+18h]; the object GetModuleCamera 005EE6F0
// returns). Its slot +24h is 0045C470 Camera::GetOrientation: canonical kotor_scene3d::CameraVTable (vtables2_gen.h).
const kotor_scene3d::CameraVTable* camera_table(const void* c) { return rd<const kotor_scene3d::CameraVTable*>(c, 0); }
// GAP CONTRACT (no canonical typed table): CSWSCreature (from 0060FB20) slot +94h. Typed from 006D7EAE..006D7EBA:
// no argument, RET 0, EAX tested (the f-input lane types the same slot identically at 006232DE).
struct ServerCreatureSlots {
    const void* s000_090[37];
    U (__fastcall* s094)(void* self, void* edx);
};
static_assert(offsetof(ServerCreatureSlots, s094) == 0x94, "+94");

// ---- minimal x87 helpers
// L3 t1 (2026-10-24): 006D7BC0 AIUpdate (ai_body) is plain C++ in the PLAN-v3 writing rule; this unit is an LD unit.
// The inline-asm helpers x87_sin_to_float / x87_cos_to_float (006D7CDF / 006D7D07) are replaced by sin / cos of
// kotor_math.h. Literal reads are __forceinline (the recipe is /Ob0) so each is a direct memory operand.
__forceinline float l3lit(const unsigned char* bytes) { float v; std::memcpy(&v, bytes, sizeof v); return v; }
// L3 t1 a2: the a1 inline-asm helpers x87_facing_dot / x87_load_float / x87_store_float and the t11b acos replica
// crt_acos_x87_006fc0e0 are replaced by long double C++ and the L1 acos(long double) (METHOD.md); raw float words are
// moved as U (the original's integer moves) with these force-inlined forms.
__forceinline float float_of_word(U u) { float f; std::memcpy(&f, &u, sizeof f); return f; }
__forceinline U word_of_float(float f) { U u; std::memcpy(&u, &f, sizeof u); return u; }
__forceinline Vector vector_of_words(U x, U y, U z) {
    Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v;
}

// 0060D790..0060D812: row = CExoString(number) (005E5BC0) through a temporary destroyed after the assignment
// (unwind states 2/3/4: the temporary, parent state 1).
struct ReturnedString {
    alignas(S) B bytes[sizeof(S)];
    bool active = false;
    S* get() { return reinterpret_cast<S*>(bytes); }
    ~ReturnedString() noexcept(false) { if (active) get()->~StartupString(); }
};
void assign_number(S* row, std::int32_t number) {
    ReturnedString temporary;
    (void)resman_string_from_int_005e5bc0(temporary.get(), nullptr, number);   // 005E5BC0
    temporary.active = true;                                                    // state 2 / 3 / 4
    (void)row->Assign005e5c50(temporary.get());                                 // 005E5C50
    temporary.active = false;                                                   // state 1
    temporary.get()->~StartupString();                                          // 005E5C20
}


// 006D7CC4..006D7D14: Yaw(&o) (ST0) ; fsub dword [0073E40C] (90) ; fmul dword [007415F8] (deg->rad) ; fsin ;
// fstp dword [esp+24h] (copied to facing y) ; Yaw(&o) again ; the same with fcos ; fstp dword [esp+40h] (facing x).
__declspec(noinline) void facing_from_yaw(const Quaternion* orientation, float* facing) {
    facing[1] = static_cast<float>(sin((static_cast<long double>(quaternion_yaw_004a9f40(orientation)) - l3lit(object_projectile_literal_0073e40c))
                                       * l3lit(object_projectile_literal_007415f8)));
    facing[0] = static_cast<float>(cos((static_cast<long double>(quaternion_yaw_004a9f40(orientation)) - l3lit(object_projectile_literal_0073e40c))
                                       * l3lit(object_projectile_literal_007415f8)));
}
// 006D8185..006D81A1: Pitch (ST0) ; fstp dword [esp] ; Yaw (ST0) ; fstp dword [esp] (the two arguments) ;
// YawPitchRoll(&turn, yaw, pitch, 0). The pitch crosses the Yaw call as a long double (exact), then each is stored
// to float once as the original does.
__declspec(noinline) const Quaternion* turn_of(const void* orientation_vector, Quaternion* turn) {
    const long double pitch = kotor_m15a_pitch_004aa130(orientation_vector);
    const long double yaw = kotor_m15a_yaw_004aa0f0(orientation_vector);
    return yaw_pitch_roll_004acac0(turn, static_cast<float>(yaw), static_cast<float>(pitch), 0.0f);
}
// 0060D9B2..0060DA20 (0060D730): the unit direction (copied with mov), the facing dot ((z*z' + y*y') + x*x',
// registers), clamped above at 1.0 (fcomp [0073D764] ; test ah,41h ; jne: greater -> mov 3F800000h, else the sum
// again ; fstp dword [esp+1Ch]), then fld ; call 006FC0E0 (_CIacos, 80-bit; the L1 acos) ; fstp dword [esp+1Ch].
// Returns the stored angle word.
__declspec(noinline) U facing_angle(const Vector* n, const float* facing) {
    float unit_words[3];                                                                     // [esp+10h] / [esp+14h] / [esp+18h] (mov)
    std::memcpy(unit_words, n, sizeof unit_words);
    const long double dot = (static_cast<long double>(unit_words[2]) * facing[2] + static_cast<long double>(unit_words[1]) * facing[1]) +
                            static_cast<long double>(unit_words[0]) * facing[0];
    const long double cosine = dot > l3lit(object_projectile_literal_0073d764) ? 1.0f : static_cast<float>(dot);
    return word_of_float(static_cast<float>(acos(cosine)));
}
// 0060DAD7..0060DAE9: fld dword [esp+1Ch] ; fchs ; fstp dword (the angle argument), as a word.
__declspec(noinline) U negated_word(U angle) {
    return word_of_float(static_cast<float>(-static_cast<long double>(float_of_word(angle))));
}

enum class Exit { store_time, self_deleted };

// 006D7C72..006D82D0: everything between the motion update and the final time store. Returns whether the object
// deleted itself (006D7D68 jumps past the time store).
Exit ai_body(void* self) {
    if (rd<U>(self, 0x15c) != 1) return Exit::store_time;                                   // 006D7C72..006D7C78
    if (void* const sound = ptr(self, 0x150))                                                // 006D7C7E..006D7C86
        kotor_m15_sound_source_set_position_005d59e0(sound, nullptr, at(self, 0x24), 1.5f);   // 006D7C88..006D7C91 (push 3FC00000h)
    if (rd<U>(self, 0x164) == 1) {                                                           // 006D7C96..006D7C9C
        void* const module = kotor_fastcall_005ed530(client(), nullptr);                     // 006D7CA2..006D7CAA
        void* const camera = ptr(module, 0x40);                                              // 006D7CAF
        Quaternion orientation;                                                              // [esp+58h]
        (void)camera_table(camera)->s024_camera_get_orientation(camera, nullptr, word_of(&orientation));   // 006D7CB2..006D7CB9 (0045C470)
        if (rd<U>(self, 0x168) != 0) {                                                       // 006D7CBC..006D7CC2
            float facing[3];                                                                 // [esp+3Ch] (z = 0 at 006D7D0C)
            facing_from_yaw(&orientation, facing);                                           // 006D7CC4..006D7D14
            facing[2] = 0.0f;
            object_table(self)->s088_set_orientation(self, nullptr, facing);                 // 006D7D18
        }
        const U elapsed = rd<U>(self, 0x44), remaining = rd<U>(self, 0x120);                // 006D7D1E..006D7D27
        wr<U>(self, 0x120, elapsed < remaining ? remaining - elapsed : 0u);                  // 006D7D29 jb (unsigned)
        if (rd<U>(self, 0x120) != 0) return Exit::store_time;                                // 006D7D3B..006D7D41
        (void)object_table(self)->s094_attach_to_scene(self, nullptr, nullptr);              // 006D7D47..006D7D4C
        if (void* const sound = ptr(self, 0x150)) scalar_delete(sound);                      // 006D7D52..006D7D5F
        (void)object_table(self)->s000_scalar_deleting_destructor(self, nullptr, 1u);        // 006D7D61..006D7D66
        return Exit::self_deleted;                                                           // 006D7D68
    }
    // 006D7D6D..006D7D9E: GetCurrentAnimation (re-called per comparison, WORD compares)
    if (!(kotor_m15a_object_get_current_animation_0063c060(self, nullptr) == 1 ||
          kotor_m15a_object_get_current_animation_0063c060(self, nullptr) == 3 ||
          kotor_m15a_object_get_current_animation_0063c060(self, nullptr) == 4 ||
          kotor_m15a_object_get_current_animation_0063c060(self, nullptr) == 0x2710))
        return Exit::store_time;
    if (rd<U>(self, 0x160) == 0) {                                                           // 006D7DA4..006D7DAA
        if (void* const old = ptr(self, 0x150)) {                                            // 006D7DB0..006D7DB8
            scalar_delete(old);                                                              // 006D7DBA..006D7DBD
            wr<U>(self, 0x150, 0);
        }
        const auto* const name = static_cast<const char*>(kotor_m15e_slot00c_self_00641db0(at(self, 0x130), nullptr));   // 006D7DC5..006D7DCD
        if (*name != 0) {                                                                    // 006D7DD2..006D7DD5
            void* const memory = live_operator_new(8u);                                      // 006D7DD7..006D7DD9
            void* source = nullptr;
            if (memory != nullptr) {                                                         // 006D7DE5 (state 1: free on throw)
                try { source = kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, at(self, 0x130)); }   // 006D7DF1
                catch (...) { live_free(memory); throw; }
            }
            wr(self, 0x150, source);                                                         // 006D7E01
            if (source != nullptr) {                                                         // 006D7E07
                kotor_sound_rev3::SourceSetPriorityGroup005d5900(source, nullptr, 0xeu);     // 006D7E09..006D7E0D
                kotor_m15_sound_source_set_3d_005d5910(ptr(self, 0x150), nullptr, 1u);      // 006D7E12..006D7E19
                kotor_m15_sound_source_set_looping_005d59d0(ptr(self, 0x150), nullptr, 1u); // 006D7E1E..006D7E25
                kotor_m15_sound_source_set_position_005d59e0(ptr(self, 0x150), nullptr, at(self, 0x24), 1.5f);   // 006D7E2A..006D7E39
                (void)kotor_fastcall_005d5930(ptr(self, 0x150), nullptr);                    // 006D7E3E..006D7E44
            }
        }
    }
    if (rd<U>(self, 0x120) <= 0x6eu && rd<U>(self, 0x16c) >= 0xeu && rd<U>(self, 0x1a4) == 0) {   // 006D7E49..006D7E69 (unsigned)
        void* const target = pointer_of(frame_R4_005ed5b0(client(), nullptr, rd<U>(self, 0x124)));   // 006D7E6F..006D7E84
        if (target != nullptr) {
            // 006D7E8E..006D7E97: 0063C060 writes AX only; the pushed EAX keeps the high word of the 005ED5B0 result.
            const std::uint16_t current = kotor_m15a_object_get_current_animation_0063c060(target, nullptr);
            if (kotor_fastcall_0063c380(target, nullptr, (word_of(target) & 0xffff0000u) | current) == 0) {   // 006D7E98..006D7E9F
                void* const server = kotor_m15a_creature_get_server_creature_0060fb20(target, nullptr);        // 006D7EA1..006D7EA3
                if (server != nullptr &&                                                                       // 006D7EAA
                    rd<const ServerCreatureSlots*>(server, 0)->s094(server, nullptr) == 0 &&                   // 006D7EAE..006D7EBA
                    recovered_creature_incapacitated_004ef890(server, nullptr) == 0) {                         // 006D7EBC..006D7EC5
                    const U mode = rd<U>(self, 0x16c);                                                         // 006D7EC7..006D7ED5
                    if (mode == 0xe || mode == 0xf) {
                        const int r = crt_rand_006fc447();                                                     // 006D7ED7
                        const U animation = r % 2 != 0 ? 0x283cu : 0x283du;                                    // 006D7EDC..006D7EF1
                        void* const model = pointer_of(gob_get_property_0x68_0063c070(as_gob(target), nullptr));      // 006D7EEF..006D7EF7
                        (void)model_table(model)->s044(model, nullptr, animation, 1u);                         // 006D7EFC..006D7F03
                        wr<U>(self, 0x1a4, 1);                                                                 // 006D7F06
                    }
                }
            }
        }
    }
    const U remaining = rd<U>(self, 0x120);                                                  // 006D7F15
    wr<U>(self, 0x160, 1);                                                                   // 006D7F1B
    if (remaining != 0) return Exit::store_time;                                             // 006D7F21
    (void)object_projectile_006d4250(self, nullptr, 2u);                                     // 006D7F27..006D7F2B PlayAnimation(2)
    void* const sound = ptr(self, 0x150);                                                    // 006D7F30
    wr<U>(self, 0x164, 1);                                                                   // 006D7F38
    if (sound != nullptr) {                                                                  // 006D7F3E
        kotor_m15_sound_source_stop_005d5a20(sound, nullptr);                                // 006D7F40
        if (void* const again = ptr(self, 0x150)) scalar_delete(again);                      // 006D7F45..006D7F52
        wr<U>(self, 0x150, 0);                                                               // 006D7F54
    }
    if (gui_load_resref_is_valid_00406030(ref_at(self, 0x140), nullptr) == 1) {              // 006D7F5A..006D7F69
        // 006D7F6B..006D7F91: push 0 x4 ; push 0Eh ; push 0 ; sub esp,0Ch ; three integer DWORD copies of the position
        // [self+24h] ; push ref. L3 t1 a2: the by-value Vector is passed as the three words the original moves (a
        // float-typed Vector argument is copied with fld/fstp dword by clang: a 4-byte spill, S1, and a quieted sNaN),
        // through a word-typed pointer to the same declared function (the same stack image: 11 argument DWORDs).
        using Play3dWords = U (__fastcall*)(void*, void*, void*, U, U, U, U, U, U, U, U, U);
        const Play3dWords play = reinterpret_cast<Play3dWords>(reinterpret_cast<std::uintptr_t>(&kotor_m15_sound_play_3d_one_shot_005d5e10));
        U where[3];
        std::memcpy(where, at(self, 0x24), sizeof where);                                    // 006D7F78..006D7F87 (mov)
        (void)play(kotor_cell_sound_007a39ec, nullptr, at(self, 0x140), where[0], where[1], where[2],
                   0u, 0xeu, 0u, 0u, 0u, 0u);                                                // 006D7F6B..006D7F91 (0.0f words)
    }
    if (rd<U>(self, 0x154) == 0) {                                                           // 006D7F96..006D7F9C
        wr<U>(self, 0x120, 0);                                                               // 006D82D0
        return Exit::store_time;
    }
    void* target = nullptr;                                                                  // [esp+1Ch]
    void* const array = kotor_fastcall_005ed570(client(), nullptr);                          // 006D7FA2..006D7FAB
    if (kotor_fastcall_004d8230(array, nullptr, rd<U>(self, 0x124), &target) != object_projectile_literal_0074666c[0]) {   // 006D7FB0..006D7FC9
        wr(self, 0x120, rd<U>(self, 0x170));                                                 // 006D82C2..006D82C8
        return Exit::store_time;
    }
    const B mode = rd<B>(self, 0x158);                                                       // 006D7FCF
    void* attach_to = nullptr;                                                               // EBP
    Vector root_position{0.0f, 0.0f, 0.0f};                                                  // [esp+3Ch] 006D8000..006D8010
    Quaternion root_orientation{1.0f, 0.0f, 0.0f, 0.0f};                                     // [esp+48h] 006D7FE0..006D7FF8
    if (mode == 0) {                                                                         // 006D801C -> 006D808B
        if (gob_get_property_0x68_0063c070(as_gob(target), nullptr) != 0) {                         // 006D808B..006D8092
            if (rd<U>(target, 0x18) == 0)                                                    // 006D8094..006D8099
                attach_to = object_table(target)->s098_get_model(target, nullptr, 0xffu, 1u);   // 006D809B..006D80AC
            else
                wr<U>(self, 0x170, 0);                                                       // 006D80B0
        }
    } else if (mode == 2) {                                                                  // 006D8021 -> 006D8031
        void* const app = client();                                                          // 006D8031..006D8038
        void* const creature = object_table(target)->s02c_as_creature(target, nullptr);     // 006D803B..006D803D
        const U item_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x20u);   // 006D8040..006D8042
        if (void* const item = kotor_m15_client_get_item_by_id_005ed5a0(app, nullptr, item_id)) {        // 006D8047..006D8053
            const void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(
                ptr(kotor_cell_rules_007a3a28, 0x34), nullptr, rd<std::int32_t>(item, 0xc));                  // 006D8055..006D8061
            if (rd<B>(base, 8) == 0) {                                                       // 006D8066..006D806B
                if (rd<U>(target, 0x18) == 0) {                                              // 006D806D..006D8072
                    void* const object = at(item, 0x10);                                     // 006D8074..006D8077
                    attach_to = object_table(object)->s098_get_model(object, nullptr, 0xffu, 1u);   // 006D807A..006D8081
                } else {
                    wr<U>(self, 0x170, 0);                                                   // 006D80B0
                }
            }
        }
    }
    void* const own = object_table(self)->s098_get_model(self, nullptr, 0xffu, 1u);          // 006D80BA..006D80CD
    // 006D80CB..006D8111: EBP and EDI non-null; mode 1 and 3 skip explicitly, mode 0 attaches, mode 2 attaches with
    // a shield node; any other mode skips (cmp al,2; jne 006D8207).
    if (attach_to != nullptr && own != nullptr && (mode == 0 || mode == 2)) {
        if (mode == 0) {
            (void)gob_table(own)->s050_gob_attach_to_object(own, nullptr, attach_to,
                                                            WindowCStr005e5670(string_at(self, 0x128), nullptr), 0);   // 006D80F7..006D810A
        } else {
            S shield;                                                                        // [esp+28h] 006D8117..006D811B (state 2)
            const int r = crt_rand_006fc447();                                               // 006D8125
            kotor_reconstruction::StartupFormat005e5680(&shield, object_projectile_literal_00757e18, r % 2 + 1);   // 006D812A..006D8142
            (void)gob_table(own)->s050_gob_attach_to_object(own, nullptr, attach_to, WindowCStr005e5670(&shield, nullptr), 0);   // 006D8147..006D815B
        }                                                                                    // ~shield 006D815E..006D8167 (state 0)
        (void)gob_table(own)->s098_gob_get_part_position(as_gob(own), nullptr, word_of(object_projectile_literal_00740c00),
                                                         word_of(&root_position), word_of(&root_orientation));   // 006D816C..006D817F
        Quaternion turn;                                                                     // [esp+58h]
        const Quaternion* const q = turn_of(at(self, 0x30), &turn);                          // 006D8185..006D81A1
        root_orientation = *q;                                                               // 006D81A6..006D81BF
        U position_words[3], q_words[4];
        std::memcpy(position_words, &root_position, sizeof position_words);
        std::memcpy(q_words, q, sizeof q_words);
        (void)gob_table(own)->s094_gob_set_part_position(as_gob(own), nullptr, word_of(object_projectile_literal_00740c00),
                                                         position_words[0], position_words[1], position_words[2],
                                                         q_words[0], q_words[1], q_words[2], q_words[3]);   // 006D81C3..006D81FD
    }
    if (object_table(target)->s02c_as_creature(target, nullptr) != nullptr && rd<B>(self, 0xfc) != 0) {   // 006D8207..006D821E
        Vector delta;                                                                        // [esp+28h]
        (void)kotor_cdecl_00423bc0(&delta.x, static_cast<const float*>(at(self, 0x24)), static_cast<const float*>(at(self, 0x100)));   // 006D8224..006D8234
        void* const memory = live_operator_new(0x48u);                                       // 006D8239..006D823B
        void* attack = nullptr;
        if (memory != nullptr) {                                                             // 006D8247 (state 3: free on throw)
            try { attack = kotor_m15a_combat_attack_data_construct_0060b630(memory, nullptr); }   // 006D8252
            catch (...) { live_free(memory); throw; }
        }
        wr<B>(attack, 0xc, static_cast<B>((rd<U>(self, 0x110) != 0 ? 1 : 0) + 1));          // 006D825D..006D826A
        wr<std::uint16_t>(attack, 0xa, rd<std::uint16_t>(self, 0x10c));                      // 006D826D..006D8274
        wr<U>(attack, 0x20, 1);                                                              // 006D827A
        const B flag = rd<B>(self, 0xfc);                                                    // 006D8278..006D8281 (state 0 at 006D8299)
        void* const creature = object_table(target)->s02c_as_creature(target, nullptr);     // 006D828E..006D82A1
        (void)object_projectile_0060d730(creature, nullptr, flag, attack, &delta, string_at(self, 0x128));   // 006D82A4..006D82A6
        kotor_fastcall_004015a0(attack, nullptr);                                            // 006D82AB..006D82AD
        live_free(attack);                                                                   // 006D82B2..006D82B3
        wr<B>(self, 0xfc, 0);                                                                // 006D82BB
    }
    wr(self, 0x120, rd<U>(self, 0x170));                                                     // 006D82C2..006D82C8
    return Exit::store_time;
}
}  // namespace

extern "C" {

// 006D7BC0..006D8307 CSWCProjectile::AIUpdate (table +074). Void: all paths end in
// the local CExoString destructor; its incidental EAX is not a callable result.
void __fastcall object_projectile_006d7bc0(void* self, void*) {
    S unused;                                                                                // 006D7BDE..006D7BE2 (state 0)
    U day, tick;
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(), nullptr), nullptr, &day, &tick);   // 006D7BE7..006D7C06
    const U last_tick = rd<U>(self, 0x40), last_day = rd<U>(self, 0x3c);                     // 006D7C19 / 006D7C21
    U elapsed_day, elapsed_tick;                                                             // [esp+24h] / [esp+18h]
    (void)recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client(), nullptr), nullptr, day, tick,
                                                   last_day, last_tick, &elapsed_day, &elapsed_tick);   // 006D7C0B..006D7C37
    wr(self, 0x44, elapsed_tick);                                                            // 006D7C4D
    if (rd<U>(self, 0x164) == 1) {                                                           // 006D7C40..006D7C52
        const U saved = rd<U>(self, 0x120);
        wr<U>(self, 0x120, 0);
        object_projectile_006d7590(self, nullptr);                                           // 006D7C60
        wr(self, 0x120, saved);
    } else {
        object_projectile_006d7590(self, nullptr);                                           // 006D7C6D
    }
    if (ai_body(self) == Exit::store_time) {
        wr(self, 0x3c, day);                                                                 // 006D82D6..006D82E1
        wr(self, 0x40, tick);
    }
    // ~unused 006D82E4..006D82F0; no synthesized return value.
}

// 0060D730..0060DC4A CSWCCreature impact Gob: flag (BYTE) == 1 places an "impact" Gob from visualeffects.2da column
// imp_impact_node in the first free (or finished: animation time -1.0) of the 12 slots at creature+FCh,
// orientated from the hit direction against the creature facing. RET 10h; EAX 1 when a slot was processed, else 0.
U __fastcall object_projectile_0060d730(void* creature, void*, U flag, void* attack, const Vector* direction, const S* name) {
    S node(*name);                                                                           // 0060D751..0060D75E 005E5B00 (state 0)
    if (static_cast<B>(flag) != 1) return 0;                                                 // 0060D763..0060D771
    S row;                                                                                   // 0060D777..0060D77B (state 1)
    if (rd<U>(attack, 0x20) != 0) assign_number(&row, 0xfb8);                                // 0060D780..0060D7B8 "4024"
    else if (rd<B>(attack, 0x28) == 7) assign_number(&row, 0xfa3);                           // 0060D7BA..0060D7E8 "4003"
    else assign_number(&row, 0xfa4);                                                         // 0060D7EA..0060D812 "4004"
    S cell;                                                                                  // 0060D81F..0060D823 (state 5)
    {
        S column(kLiteral_0074f358);                                                         // 0060D828..0060D839 (state 6)
        void* const table = ptr(ptr(kotor_cell_rules_007a3a28, 0xb8), 0x88);                // 0060D83E..0060D84A visualeffects.2da
        (void)kotor_m15_c2da_get_string_entry_00413de0(table, nullptr, &row, &column, &cell);   // 0060D850..0060D869
    }                                                                                        // 0060D86E..0060D87A (state 5)
    U index = 0;                                                                             // [esp+20h]
    for (; index < 12; ++index) {                                                            // 0060D87F..0060D8D1
        void* const slot = ptr(creature, 0xfc + 4 * index);                                  // 0060D890
        if (slot == nullptr) break;                                                          // 0060D892..0060D894
        float time = 0.0f;                                                                   // [esp+48h] = 0 (0060D8A6)
        (void)gob_table(slot)->s01c_gob_get_animation(as_gob(slot), nullptr, word_of(object_projectile_literal_00743318),
                                                      word_of(&time), 0u);                   // 0060D896..0060D8AE
        if (time == l3lit(kLiteral_00740cbc)) {                                              // 0060D8B1..0060D8C0 fcomp ; test ah,44h ; jnp (ordered equal)
            (void)gob_table(slot)->s000_gob_destruct_gob(as_gob(slot), nullptr, 1u);          // 0060D8D8..0060D8DE
            wr<U>(creature, 0xfc + 4 * index, 0);                                            // 0060D8E0..0060D8E4
            break;
        }
    }
    if (index == 12) return 1;                                                               // 0060D8D3 -> 0060DBFB
    aurora_model::Gob* const gob = aurora_new_aur_object_00449cc0(WindowCStr005e5670(&cell, nullptr), kLiteral_0074f350, nullptr, nullptr);   // 0060D8EF..0060D907
    if (gob == nullptr) return 0;                                                            // 0060D90C..0060D95C
    if (node.BorrowedEquals005e5290(base_ini_reconstruction::warning_empty_0073d71c) != 0)   // 0060D95F..0060D96F
        (void)GuiAssign005e5140(&node, nullptr, object_projectile_literal_00743318);         // 0060D971..0060D97A "impact"
    const char* const node_text = WindowCStr005e5670(&node, nullptr);                       // 0060D981..0060D987
    void* const model = object_table(creature)->s098_get_model(creature, nullptr, 0xffu, 1u);   // 0060D98C..0060D998
    (void)gob_table(gob)->s050_gob_attach_to_object(gob, nullptr, model, node_text, 3);       // 0060D99E..0060D9A1
    // 0060D9A4..0060DA20: cosine of the hit direction against the creature facing, clamped above at 1.0, then acos.
    Vector unit;                                                                             // [esp+78h]
    const Vector* const n = normalize_004ab180(&unit, direction);                            // 0060D9A4..0060D9AD
    const auto* const facing = static_cast<const float*>(at(creature, 0x30));               // EDI = creature+30h
    // The angle is kept as the stored word: the original reloads it with fld only for the negation (0060DAD7) and
    // moves it as a DWORD otherwise (0060DB7A).
    const U angle = facing_angle(n, facing);                                                 // 0060D9B2..0060DA20
    void* const app = client();                                                              // 0060DA24..0060DA2C
    const U item_id = object_projectile_0060d070(creature, nullptr, rd<B>(attack, 0x28));    // 0060DA2F..0060DA36
    if (void* const item = kotor_m15_client_get_item_by_id_005ed5a0(app, nullptr, item_id))  // 0060DA3B..0060DA45
        (void)kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                rd<std::int32_t>(item, 0xc));   // 0060DA47..0060DA54 (result unused)
    Vector facing_unit, axis;                                                                // [esp+6Ch] / [esp+84h]
    const Vector* const f = normalize_004ab180(&facing_unit, static_cast<const Vector*>(at(creature, 0x30)));   // 0060DA59..0060DA5F
    const Vector* const cross = vector_cross_004ab210(&axis, direction, f);                  // 0060DA64..0060DA71
    // fld dword [0073D700] ; fcom [cross.z] ; test ah,41h ; jp (0 > z or unordered: second branch). The original then
    // stores the loaded 0.0 with fst dword into the axis words; here the words are copied (the same bits, mov).
    const U zero = 0u;                                                                       // the image word at 0073D700 (.rdata 00000000h: 0.0f)
    const U one = 0x3f800000u;                                                               // 3F800000h immediates (1.0f)
    Quaternion first, second, product, scratch;
    const Quaternion* p;
    if (l3lit(object_projectile_literal_0073d700) <= cross->z) {                             // 0060DA76..0060DA91
        const Quaternion* const a = quaternion_construct_004ac960(&first, nullptr, vector_of_words(one, zero, zero), float_of_word(one));    // [esp+F0h] 0060DAD2
        const float negative = float_of_word(negated_word(angle));                          // 0060DAD7..0060DAE9
        const Quaternion* const b = quaternion_construct_004ac960(&second, nullptr, vector_of_words(zero, zero, one), negative);   // [esp+B0h] 0060DB02
        p = quaternion_multiply_004a9ac0(&product, b, a);                                    // [esp+90h] 0060DB07..0060DB10
    } else {
        const Quaternion* const a = quaternion_construct_004ac960(&first, nullptr, vector_of_words(one, zero, zero), float_of_word(one));    // [esp+A0h] 0060DB75
        const Quaternion* const b = quaternion_construct_004ac960(&second, nullptr, vector_of_words(zero, zero, one), float_of_word(angle));   // [esp+C0h] 0060DB7A..0060DBA0 (mov)
        p = quaternion_multiply_004a9ac0(&product, b, a);                                    // [esp+E0h] 0060DBA5..0060DBAE
    }
    U q_words[4];
    std::memcpy(q_words, p, sizeof q_words);                                                 // 0060DB15..0060DB2B / 0060DBB3..0060DBC9
    (void)gob_table(gob)->s060_gob_set_orientation(gob, nullptr, word_of(&scratch), q_words[0], q_words[1], q_words[2], q_words[3]);   // 0060DBD3..0060DBD8
    (void)gob_table(gob)->s018_gob_play_animation(gob, nullptr, word_of(object_projectile_literal_00743318), one, 1u, 0u);   // 0060DBDB..0060DBED
    wr(creature, 0xfc + 4 * index, gob);                                                     // 0060DBF0..0060DBF4
    return 1;                                                                                // ~cell, ~row, ~node 0060DBFB..0060DC2C
}

// 0060D070..0060D0BB hand -> item id. Jump table 0060D0C0 (DEC EAX; CMP 5; JA unsigned).
U __fastcall object_projectile_0060d070(void* creature, void*, U hand) {
    switch (hand) {
    case 1:
    case 6: return kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x10u);   // 0060D081..0060D089 (tail JMP)
    case 2: return kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x20u);   // 0060D08E..0060D096 (tail JMP)
    case 3: return rd<U>(creature, 0x24c);                                                   // 0060D09B
    case 4: return rd<U>(creature, 0x250);                                                   // 0060D0A4
    case 5: return rd<U>(creature, 0x254);                                                   // 0060D0AD
    default: return 0x7f000000u;                                                             // 0060D0B6
    }
}

}  // extern "C"
