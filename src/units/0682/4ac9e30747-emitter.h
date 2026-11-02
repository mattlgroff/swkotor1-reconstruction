#pragma once
// lane-3d-emitter (SUBLANES.json area "part_emitter" + the PartEmitter/Particle class bodies its slots reach):
// particle emitters of swkotor.exe (GOG K1, image base 00400000). The main-menu model has six emitters
// (Mist01/Mist: Fountain + Billboard_to_World_Z; Fx_PuffySand..03: Fountain + Normal; texture fx_Smoke;
// node flags +12C = 0x122 / 0x22: P2PSel|Random(|InheritingLocal), so P2P (bit0) is off and MoveFunc selects
// moveParticleStandard 00492450).
//
// Conventions (DECISIONS D1 + D1-amendment, D21, D24, D30-FINAL, D35, D39, D42):
//  * original __thiscall bodies = __fastcall(Self*, void* unused_edx, stack...); callers pass nullptr for EDX;
//    callees never read EDX. No body here returns a struct by value; none is variadic.
//  * objects are raw observed-offset storage (kotor_scene3d::field/at); every offset used is cited per use.
//  * virtual calls reload the receiver's vtable pointer at every call and go only through PUBLISHED typed tables:
//      PartEmitter (and every Part)   -> lane-3d-model aurora_vt::VTable_00743340 (common Part prefix, D39)
//      Gob                            -> lane-3d-render aurora_model::GobVTable (vtables_gen.h, D42)
//      Scene                          -> lane-3d-render SceneVTable (vtables_gen.h, D42)
//      Particle / ChunkyParticle      -> this lane: ParticleVTable (007432D8 / 007432E4)
//  * D30-FINAL: x87 wide intermediates are `double`; `float` only where the original stores with fstp dword
//    (or passes a float through a dword stack slot). frndint+fistp -> fistp_nearest(); _ftol2 -> ftol2().
//  * Randomness: the original CRT rand 006FC447 (per-thread holdrand at _getptd()+14h, LCG 343FDh/269EC3h,
//    (x>>16)&7FFFh). Provider: lane-3d-model crt_rand_006fc447 (model_lifetime.cpp) over task-35 compat_getptd
//    (the reconstructed 006FEEDF) - the same PTD the original srand 006FC43A (M41 services_tail5::Seed via the
//    0x006feedf dispatch) seeds. Never the UCRT rand().
//  * GL entry points: the single adapter owner kotor_renderer_init::<import>_<IAT> (lane-renderer-bodies
//    renderer_gl_imports_gen.h; glEnable/glDisable = M40 adapters already declared by scene3d.h).
#include "6c81d8874d-scene3d.h"          // lane-3d-render canonical 3D header (Cells, Vector/Quaternion, typed Scene/Gob tables)
#include "7ee14b5ec3-model_vtables.hpp"  // lane-3d-model typed Part / PartEmitter tables (D39)
// Math declarations for the long-double build of particle_motion: <cmath> -> kotor_math.h (an LD
// unit may not see <cmath>, whose long double overloads would define ?floor@@... etc.). std::floor -> the C floor
// declared by kotor_math.h (L1), exact as before.
#include "../../math/kotor_math.h"
#include <climits>
#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------- single GL adapter owner (exact copies of
// lane-renderer-bodies src/cohort/renderer_gl_imports_gen.h declarations)
namespace kotor_renderer_init {
using U32 = std::uint32_t; using I32 = std::int32_t; using U8 = std::uint8_t;
extern void __stdcall glClear_0073d21c(U32);
extern void __stdcall glEnableClientState_0073d260(U32);
extern void __stdcall glDisableClientState_0073d264(U32);
extern void __stdcall glStencilOp_0073d270(U32, U32, U32);
extern void __stdcall glStencilMask_0073d278(U32);
extern void __stdcall glDepthMask_0073d28c(U8);
extern void __stdcall glStencilFunc_0073d298(U32, I32, U32);
extern void __stdcall glEnd_0073d2c4();
extern void __stdcall glVertex3f_0073d2c8(float, float, float);
extern void __stdcall glBegin_0073d2d0(U32);
extern void __stdcall glColor3f_0073d2ec(float, float, float);
extern void __stdcall glPopMatrix_0073d2fc();
extern void __stdcall glPushMatrix_0073d300();
extern void __stdcall glColorMask_0073d358(U8, U8, U8, U8);
extern void __stdcall glClearStencil_0073d36c(I32);
extern void __stdcall glPolygonMode_0073d378(U32, U32);
extern U8 __stdcall glIsEnabled_0073d37c(U32);
extern void __stdcall glLineWidth_0073d380(float);
extern void __stdcall glPointSize_0073d384(float);
extern void __stdcall glBlendFunc_0073d38c(U32, U32);
}

namespace kotor_emitter {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
using EmitterTable = aurora_vt::VTable_00743340;   // PartEmitter layout (34 slots); common Part prefix of every Part table

// Typed dispatch views (object-pointer conversions only, D24): reload [object] at each call.
inline const EmitterTable* part_table(void* part) { return static_cast<const EmitterTable*>(field<void*>(part, 0)); }
inline const aurora_model::GobVTable* gob_table(void* gob) { return *static_cast<const aurora_model::GobVTable* const volatile*>(gob); }
inline aurora_model::Gob* as_gob(void* gob) { return static_cast<aurora_model::Gob*>(gob); }
inline void* as_object(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }

// Particle / ChunkyParticle virtual table (this lane owns the data objects 007432D8 / 007432E4, 3 slots each):
//   +00 deleting destructor (RET 4)   0048DE30 / 0048E190
//   +04 AsChunky (RET 0)              0063E7F0 return_zero / 00641DB0 self_return
//   +08 initialize(emitter, scale) (RET 8)  004921D0 / 00492400
struct ParticleVTable {
    U (__fastcall* s00_deleting_destructor)(void* self, void* unused_edx, U flags);
    U (__fastcall* s04_as_chunky)(void* self, void* unused_edx);
    U (__fastcall* s08_initialize)(void* self, void* unused_edx, U emitter, U scale_bits);
};
static_assert(sizeof(ParticleVTable) == 12, "Particle tables 007432D8/007432E4 hold three slots");
inline const ParticleVTable* particle_table(void* particle) { return *static_cast<const ParticleVTable* const volatile*>(particle); }

// D30-FINAL helpers. W = value on the x87 stack (double); S = original `fstp dword` rounding point.
inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline float F(U bits) { return bits_to_float(bits); }
// frndint (RC nearest-even, CW 027F) + fistp dword: NaN / out of range -> integer indefinite 80000000h.
inline I fistp_nearest(double v) {
    if (!(v >= -2147483648.5 && v < 2147483647.5)) {
        if (!(v == v)) return INT_MIN;
    }
    double r = floor(v);
    const double d = v - r;
    if (d > 0.5 || (d == 0.5 && floor(r * 0.5) * 2.0 != r)) r += 1.0;   // ties to even (r odd -> up)
    if (!(r >= -2147483648.0 && r <= 2147483647.0)) return INT_MIN;
    return static_cast<I>(static_cast<long long>(r));   // L3 (kmlint FA3, rule 6.4): r is integral and in int range, exact
}
// _ftol2 006FAE8C (chop; fistp qword -> EAX = low dword of the 64-bit truncation; NaN/|v|>=2^63 -> 0).
inline I ftol2(double v) {
    if (!(v > -9223372036854775808.0 && v < 9223372036854775808.0)) return 0;
    const std::int64_t t = static_cast<std::int64_t>(v);
    return static_cast<I>(static_cast<U>(static_cast<std::uint64_t>(t)));
}
// x87 fcomp/fnstsw/test ah,mask decode helpers (C0 = ah&1 "less", C3 = ah&40h "equal", C2 = ah&4 unordered):
inline bool x87_less(double a, double b) { return a < b || a != a || b != b; }          // C0 set (unordered sets C0|C2|C3)
inline bool x87_less_or_equal(double a, double b) { return !(a > b); }                // C0|C3 set

// Borrowed original globals with an owner outside this area (Main publishes once; pointers, never copies).
// Cells already in kotor_scene3d::Cells are read through kotor_scene3d::cells (active scene 007B92A0,
// current camera 007BB4E8, frame time 0078E574, GL enum cells 0078D448/44C/450/454/45C/470).
struct EmitterCells {
    const volatile U* gate_0078e614;                 // .data cold 1 (also read by 00450AC0/00456364/0046F900/...)
    PointerList* emitter_parts_007fbf78;             // emitter bucket {data 7FBF78, size 7FBF7C}; ProcessParts 0046D3B0 / ClearBuckets 0046BEF0 write it
    volatile I* triangles_00827fb0;                  // statistics; also written by RenderFlat/PartTriMesh::Draw/grass
};
extern const EmitterCells& emitter_cells;
} // namespace kotor_emitter

// ---------------------------------------------------------------- cells owned by this area
// (.data values from the pinned image; BSS zero. Only PartEmitter-family functions reference them; shared
// ones are listed with their other readers.) Storage: emitter_core.cpp.
extern "C" {
extern volatile kotor_scene3d::U emitter_bucket_enabled_0078e3d8;     // .data cold 1; read only by 004509B0
extern volatile kotor_scene3d::U emitter_frame_threshold_00798b94;    // .data 40A00000h (5.0f); 0048DC90 / 00495B20
extern volatile kotor_scene3d::U emitter_stencil_mode_00798b98;       // .data cold 1; 0049B680
extern volatile kotor_scene3d::I emitter_standard_pool_capacity_00798ba4; // .data 1000; 0048F040 (grows it)
extern volatile kotor_scene3d::I emitter_linked_pool_capacity_00798bac;   // .data 1000; 00495B20 (grows it)
extern volatile kotor_scene3d::I particle_count_0083046c;            // BSS; Particle ctor ++ / dtor --
extern volatile kotor_scene3d::I emitter_particles_drawn_00830474;   // BSS; written by 0048F040 / 00495B20 / 00490820 (framebuffer_bloom)
extern volatile kotor_scene3d::U emitter_bss_00830488;               // BSS; 0049B680
extern volatile kotor_scene3d::U emitter_bss_0083048c;               // BSS; 0049B680
extern volatile kotor_scene3d::U emitter_bss_00830490;               // BSS; 0049B680
extern volatile kotor_scene3d::U emitter_last_spawn_00830494;        // BSS; written by 0048E1B0 (= emitter[+58])
extern volatile kotor_scene3d::U emitter_debug_spheres_00830498;     // BSS; 0048F040 / 00490700 / 00495B20 / 00490820
extern volatile kotor_scene3d::U emitter_bss_0083049c;               // BSS; LightningEmitter::Update 00498B80
extern volatile kotor_scene3d::U emitter_cell_00798ba0;              // .data cold 1; read only by 00492450 (0049296F, 00492BD1)
extern void* volatile emitter_standard_pool_008304a0;                // BSS; RenderStandard static vertex pool
extern volatile kotor_scene3d::U emitter_standard_pool_guard_008304a4; // BSS; static-init guard bit 0
extern void* volatile emitter_linked_pool_008304b0;                  // BSS; RenderLinked static vertex pool
extern volatile kotor_scene3d::U emitter_linked_pool_guard_008304b4;   // BSS; static-init guard bit 0
// Particle tables (this lane): 007432D8 Particle, 007432E4 ChunkyParticle.
extern const kotor_emitter::ParticleVTable particle_vtable_007432d8;
extern const kotor_emitter::ParticleVTable chunky_particle_vtable_007432e4;
}

// ---------------------------------------------------------------- borrowed from other owners (exact declarations)
extern "C" {
// lane-3d-model (model_lifetime.cpp / model_instance.hpp / model_vtables.cpp)
std::int32_t __cdecl crt_rand_006fc447();                                     // original CRT rand over compat_getptd
extern volatile kotor_scene3d::U emitter_render_normal_00798b6c;              // 1
extern volatile kotor_scene3d::U emitter_render_billboard_local_z_00798b70;   // 2
extern volatile kotor_scene3d::U emitter_render_billboard_world_z_00798b74;   // 3
extern volatile kotor_scene3d::U emitter_render_linked_00798b78;              // 4
extern volatile kotor_scene3d::U emitter_render_aligned_world_z_00798b7c;     // 5
extern volatile kotor_scene3d::U emitter_render_mode_00798b80;                // 6
extern volatile kotor_scene3d::U emitter_frame_blend_default_00798b84;        // 7 (word read)
extern volatile kotor_scene3d::U emitter_render_aligned_particle_dir_00798b88;// 8
extern volatile kotor_scene3d::U emitter_render_motion_blur_00798b8c;         // 9
extern volatile aurora_model::I aurora_emitter_count_00830470;                // ++ ctor 0049B658, -- dtor 0049CFA0
void* __fastcall aurora_node_as_emitter_0043e3c0(void* node, void* unused_edx);   // 0043E3C0 (lane-3d-model)
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
void __fastcall aurora_hit_info_clear_output_00456a60(void* self, void* unused_edx);
void __fastcall aurora_material_load_textures_0047b770(void* material, void* unused_edx, aurora_model::Part* part);
std::uint32_t __fastcall part_animate_derived_00484260(void* self, void* unused_edx, std::uint32_t);   // 00484260 (model_vtables.cpp declaration)
// lane-font (txi reader): original __stricmp 0070ACAF
__declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// lane-font: CExoArrayList<Vector>::Allocate 00421310 (thiscall RET 4 as __fastcall)
void __fastcall vector_list_allocate_00421310(kotor_scene3d::PointerList* list, void* unused_edx, kotor_scene3d::I size);
// task-17 vector-equality (lane-gui-draw writers): 004AA980 operator==(Vector const&, Vector const&), cdecl, EAX 0/1
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
}
// lane-renderer-bodies (D32): CExoArrayList::deleteAt 006857A0
namespace kotor_renderer_gl {
void __fastcall deleteAt_006857a0(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::int32_t index);
}
// lane-aurres: 00445080 CExoArrayList<ptr>::Remove = kotor_reconstruction::ArrayMutations::RemoveReverse32 (task-14).

// ---------------------------------------------------------------- lowers with no provider yet (PROPOSED for
// scene3d.h "lowers" section; owners per SUBLANES.json). ABIs read from the original bodies.
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;
// core
void __fastcall vector_list_set_size_004213a0(PointerList* list, void* unused_edx, I size);                 // RET 4
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor);                          // returns out
Vector* __fastcall quaternion_zdir_0043e250(const Quaternion* q, void* unused_edx, Vector* out);              // RET 4, returns out
Vector* __fastcall quaternion_ydir_0044f420(const Quaternion* q, void* unused_edx, Vector* out);              // RET 4, returns out
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void* unused_edx, const Vector* v);               // RET 4
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);  // returns out
void __fastcall vector_normalize_004ab130(Vector* self, void* unused_edx);                                    // RET 0
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);                                             // returns out
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion* self, void* unused_edx, Vector axis, float angle); // RET 10h, returns this
Quaternion* __cdecl rotation_arc_004ab630(Quaternion* out, Vector from, Vector to);                           // returns out
void __fastcall exo_array_list_add_unique_0046bd10(PointerList* list, void* unused_edx, U value);             // RET 4
// lane-3d-anim (anim3d.h; animation sub-area) - exact declarations of that owner
Vector* __cdecl vector_interpolate_0046eea0(Vector* out, const Vector* a, const Vector* b, float t);          // out = a*(1-t)+b*t
void __fastcall node_internal_control_004854d0(void* node, void*, void* part, float t0, float t1, float weight); // RET 10h
// lane-gui-render (aur_gui_quad.cpp): GLRender::RenderObjectsVTCQuadUP 00431CC0, cdecl (stride 18h: xyz, rgba8 +0C, uv +10)
void __cdecl kotor_cdecl_00431cc0(float* buffer, std::int32_t count);
// core: WindManager::AddPointSourceWind 004A6510, thiscall RET 18h (Vector by value, two raw dwords, float)
void __fastcall wind_manager_add_point_source_wind_004a6510(void* wind, void* unused_edx, Vector position, U a, U b, float strength);
// no SUBLANES owner (PartEmitter destructor path only; not reached on the first menu frame)
void __cdecl scene_remove_from_scene_manager_004554b0(void* part);                 // 004554B0 cdecl
void __fastcall part_destruct_00440140(void* part, void* unused_edx);              // 00440140 Part::~Part, RET 0
// this lane, deferred (off the menu path: node flag bit0 P2P clear on all six menu emitters)
void __fastcall part_emitter_move_particle_p2p_bezier_00492ea0(void* emitter, void* unused_edx, float delta); // RET 4
void __fastcall part_emitter_move_particle_p2p_grav_00493ae0(void* emitter, void* unused_edx, float delta);   // RET 4
// framebuffer_bloom
void __fastcall part_emitter_render_motion_blurred_00490820(void* emitter, void* unused_edx);                 // RET 0
}

// ---------------------------------------------------------------- authored in this area
extern "C" {
// scene entry points (Scene vtbl+110 typed by vtables_gen.h; UpdateEmitters declared by scene3d.h)
U __fastcall scene_do_emitter_bucket_004509b0(void* scene, void* unused_edx);                                 // RET 0 (tail jmp 0048F000)
int __cdecl emitter_order_cmp_0044f5a0(const void* a, const void* b);                                        // qsort comparator
// BSP / rooms / wind
void __fastcall nodedata_add_emitter_0046bdb0(void* node, void* unused_edx, void* emitter);                    // RET 4
void __fastcall node_emitter_internal_control_00485590(void* node, void* unused_edx, void* part, float t0, float t1, float weight); // RET 10h (lane-3d-anim consumer spelling)
bool __fastcall emitter_node_is_affected_by_wind_0048d780(void* node, void* unused_edx);                     // AL = bit 2 of [+12C] (lane-3d-bsp consumer spelling)
void __fastcall wind_manager_register_windy_emitter_004a6470(void* wind, void* unused_edx, void* emitter);    // RET 4 (tail jmp AddUnique +84C)
// PartEmitter
U __fastcall part_animate_derived_0048dce0(void* self, void* unused_edx, U a0);                               // Part vtbl+68 (jmp 00484260)
double __fastcall part_emitter_frame_scale_0048dc90(void* emitter, void* unused_edx);                         // ST0
I __fastcall part_emitter_next_frame_0048dd50(void* emitter, void* unused_edx, I frame);                      // RET 4
void __fastcall part_emitter_calculate_radius_0048e1b0(void* emitter, void* unused_edx);                      // RET 0
void __fastcall part_emitter_update_particle_frame_0048e460(void* emitter, void* unused_edx, float delta);    // RET 4
void __fastcall part_emitter_render_bounding_sphere_0048ef10(void* emitter, void* unused_edx);
void __cdecl part_emitter_start_emitter_layer_0048efa0();
void __cdecl part_emitter_stop_emitter_layer_0048f000();
void __fastcall part_emitter_render_standard_0048f040(void* emitter, void* unused_edx);
void __fastcall part_emitter_render_nothing_00490700(void* emitter, void* unused_edx);
void __fastcall part_emitter_animate_particles_00494da0(void* emitter, void* unused_edx, float delta);        // RET 4
void __fastcall part_emitter_render_linked_00495b20(void* emitter, void* unused_edx);
void __fastcall part_emitter_render_func_0049b680(void* emitter, void* unused_edx);
// slot bodies (lane-3d-model tables reference these symbols; types = the table slot types)
U __fastcall part_emitter_deleting_destructor_0049d740(void* self, void* unused_edx, U flags);
U __fastcall part_emitter_deleting_destructor_0049d760(void* self, void* unused_edx, U flags);
U __fastcall fountain_emitter_update_00496e40(void* self, void* unused_edx, U delta_bits);
void __cdecl set_emitter_target_0049b240(void* gob, void* event, aurora_model::Part* part);                   // Gob::RegisterCallBack target
void __cdecl add_emitter_to_rooms_0046cd70(void* scene, void* emitter);                                       // cdecl (ManageSceneBSP 0046E7BC)
void __fastcall wind_manager_deregister_windy_emitter_004a6480(void* wind, void* unused_edx, void* emitter);   // RET 4
void __fastcall part_emitter_destruct_0049cfa0(void* self, void* unused_edx);                                 // RET 0
void __fastcall part_emitter_destruct_0049d780(void* self, void* unused_edx);                                 // RET 0 (jmp 0049CFA0)
// 0048DDA0 ResizeEmitterPool: original ECX = old count, EAX = new count, [esp+4] = old buffer, caller-cleaned.
// REVIEW (register transport): exported as fastcall with the new count in EDX and callee cleanup of the one stack word.
void* __fastcall resize_emitter_pool_0048dda0(I old_count, I new_count, void* old_buffer);
U __fastcall explosion_emitter_update_00497640(void* self, void* unused_edx, U delta_bits);                     // RET 4
U __fastcall single_emitter_update_004979c0(void* self, void* unused_edx, U delta_bits);                       // RET 4
// particles (particles.cpp / particle_motion.cpp)
void __fastcall part_emitter_chk_random_0048dcf0(void* emitter, void* unused_edx, void* particle);             // RET 4
void __fastcall part_emitter_chk_particle_life_00494b30(void* emitter, void* unused_edx, float delta);         // RET 4
void __fastcall part_emitter_move_func_00494d40(void* emitter, void* unused_edx, float delta);                 // RET 4
void* __fastcall particle_construct_00494c40(void* self, void* unused_edx, void* emitter, U delta_bits);       // RET 8
void* __fastcall chunky_particle_construct_00494cb0(void* self, void* unused_edx, void* emitter, U delta_bits); // RET 8
void __fastcall particle_destruct_0048d860(void* self, void* unused_edx);                                      // RET 0
U __fastcall particle_deleting_destructor_0048de30(void* self, void* unused_edx, U flags);                     // RET 4
void __fastcall chunky_particle_destruct_0048dc30(void* self, void* unused_edx);                               // RET 0
U __fastcall chunky_particle_deleting_destructor_0048e190(void* self, void* unused_edx, U flags);              // RET 4
U __fastcall particle_initialize_004921d0(void* self, void* unused_edx, U dt_bits, U scale_bits);             // RET 8 (Particle vtbl+08)
U __fastcall chunky_particle_initialize_00492400(void* self, void* unused_edx, U dt_bits, U scale_bits);       // RET 8 (ChunkyParticle vtbl+08)
void __fastcall particle_random_position_0048d870(void* self, void* unused_edx, I range_x, I range_y);         // RET 8
void __fastcall particle_random_direction_0048de60(void* self, void* unused_edx, I unused_a, I unused_b);      // RET 8 (args never read)
void __fastcall particle_inherit_velocity_0048d9e0(void* self, void* unused_edx, float dt);                    // RET 4
void __fastcall part_emitter_move_particle_standard_00492450(void* emitter, void* unused_edx, float dt);       // RET 4
}

