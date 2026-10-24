#pragma once
// lane-3d-render: main-menu 3D scene path (swkotor.exe GOG K1, image base 00400000).
// Scope: CSWGuiScene/CAurScene/CAurCamera construction, camera attach, per-frame
// animate + camera render top (projection, frustum planes, view transform, scene render entry).
// Model LOADING (FindModel/LoadModel/NewCAurObject/Gob::Gob/CreateInstanceTree/PartInitMaterial,
// PlayAnimation) is owned by lane-3d-model and is only declared here.
//
// Conventions (DECISIONS.md D1): original __thiscall entries are declared __fastcall(Self*, void* edx_ignored, ...);
// callers pass nullptr for EDX; identical stack layout and callee cleanup. Virtual slots are reached
// by reloading the receiver's vtable pointer at every call (original `mov reg,[obj]; call [reg+slot]`).
// Layouts are byte-offset views: every offset used is cited from an observed instruction; no field is named
// beyond what the instruction shows (Ghidra placeholder names quoted where they exist in swkotor.exe.h).
#include <cstdint>
#include <cstring>
#include "2cc88caf3267-growth.h"   // M41 owner menu_list_growth::allocate_005e03c0 + kotor_reconstruction::ArrayMutations

// lane-3d-model types (model_load.hpp / model_instance.hpp): identical forward declarations so both headers
// can be included in one unit (D42). Aliases repeat the same types.
namespace aurora_model {
using U = std::uint32_t;
using I = std::int32_t;
struct MaxTree;
struct Gob;
struct Part;
struct Scene;
struct MdlNode;
}

namespace kotor_scene3d {
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
using I64 = std::int64_t;

// 12-byte vector (x,y,z) - every consumer reads [+0],[+4],[+8] as floats (004AB210, 004A9BB0).
struct Vector { float x, y, z; };
// Quaternion, observed component order (w,x,y,z): 004A9BC0 identity test on [q+0]==+-1.0,
// 00425D1E acos([q+0]), 004A9BF2.. rotation uses [q+4],[q+8],[q+C] as the vector part.
struct Quaternion { float w, x, y, z; };
// Frustum plane rows (16 bytes; 0045C27E shl 4): normal (a,b,c) at +0..+8, distance at +C.
struct Plane { float a, b, c, d; };
// CExoArrayList<Plane> at Camera+1F8: data +0, count +4, capacity +8 (0045C282 / 0045C2B7 / 0045D8F7).
struct PlaneList { Plane* data; I count; I capacity; };
// Generic CExoArrayList (data,count,capacity) as used by 00416770 / 0056B6F0 / 005E03C0: the M41 type.
using PointerList = kotor_reconstruction::ArrayMutations;
static_assert(sizeof(Vector) == 12 && sizeof(Quaternion) == 16 && sizeof(Plane) == 16, "x86 layouts");
static_assert(sizeof(PlaneList) == 12 && sizeof(PointerList) == 12, "x86 layouts");

// Sizes from the allocations in the original.
constexpr U kSceneSize = 0x2C4;   // 00458E86 push 2C4h
constexpr U kCameraSize = 0x234;  // 0045D946 push 234h
constexpr U kGobSize = 0x1CC;     // Camera: Gob at +4, first Camera field at +1D0 (0045D835 / 0045D8A5); Scene: Gob at +F8 ends at 2C4

// Byte-offset field access over original object storage.
template<class T> inline T& field(void* object, U offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
inline void* at(void* object, U offset) { return static_cast<unsigned char*>(object) + offset; }

// Raw float bit helpers (no FPU load/store canonicalisation of copied DWORDs).
inline float bits_to_float(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }
inline U float_to_bits(float f) { U u; std::memcpy(&u, &f, 4); return u; }

// ---------------------------------------------------------------- original globals (borrowed views)
// Main publishes `cells` once over the owning objects (renderer cells / window cells / this lane's
// proposed storage, see PACKET.json "cells"). Pointers are never copied values.
// Extension procedure type; identical to kotor_renderer_gl_procs::PFN_glProgramEnvParameter4fARB / PFN_glProgramParameter4fNV.
using ProgramParameter4f = void (__stdcall*)(std::uint32_t, std::uint32_t, float, float, float, float);

using Callback = void (__cdecl*)();
using BindBuffer = void (__stdcall*)(std::uint32_t, std::uint32_t);   // == kotor_renderer_gl_procs::PFN_glBindBufferARB
using ClientActiveTexture = void (__stdcall*)(std::uint32_t);   // == PFN_glClientActiveTextureARB

struct Cells {
    // window extent (M40 owners: kotor_renderer_gl_cells::width_0078d1d4, kotor_menu_window_cells::height_0078d1d8)
    const volatile U* width_0078d1d4;          // 0045C5D2 cmp edi,[0078D1D4] (signed jge)
    const volatile U* height_0078d1d8;         // 0045C5EF
    // camera render
    const volatile U* wireframe_007bb4f0;      // 0045C55A test; sete -> polygon-mode table index
    const volatile U* alt_view_007bb4f4;       // 0045CB33 nonzero -> SetViewTransform(7BB504,78E440)
    Vector* alt_view_position_007bb504;        // 0045CB41 push 7BB504h (BSS)
    Quaternion* alt_view_orientation_0078e440; // 0045CB3C push 78E440h (.data)
    const volatile I* plane_index_007bb4ec;    // 0045CBC4 mov eax,[007BB4EC]; shl eax,4
    Vector* plane_normal_007bb4f8;             // 0045CBD6..0045CBEE 3 DWORD copies (7BB4F8/7BB4FC/7BB500)
    volatile float* plane_distance_007bb4e4;   // 0045CBF8 fstp [007BB4E4]
    void* volatile* current_camera_007bb4e8;   // 0045CC0F = camera, 0045CC3B = 0
    // view transform / math flags (.data, cold value 1, no writer instruction found in .text)
    const volatile U* matrix_view_0078d494;    // 00425CBE
    const volatile U* quaternion_shortcut_007a1980; // 004A9BB0
    // timer (BSS)
    volatile U* timer_guard_007b9418;          // 0044F320 bit 0 = frequency cached
    volatile I64* timer_frequency_007b9410;    // 0044F353/0044F359
    // scene animate / render bookkeeping
    volatile I64* animate_start_007b9298;      // 00451C8A / 00451C96
    volatile I64* animate_end_007b9288;        // 00451D3E / 00451D4F
    volatile float* animate_ms_007b92d0;       // 00451D5C
    const volatile U* animations_0078e3b8;     // 00451C8F (.data cold 1)
    const volatile U8* interleaved_007b92f4;   // 00451CA4 byte (AurEnable/DisableInterleavedAnimations 0044F760/0044F770)
    void* volatile* active_scene_007b92a0;     // 00451C9C / 00451D29 / 0045138A / 0045140F
    void* volatile* render_scene_007b935c;     // 004512DE
    volatile I64* render_start_007b9280;       // 004512E9 / 004512F5
    volatile I64* render_end_007b9290;         // 004514A9 / 004514B8
    volatile float* render_ms_007b92d4;        // 004514C4
    volatile I* frame_mod3_0078e3f8;           // 00451317..0045132B (.data cold 2)
    const volatile U* render_enabled_0078e3f4; // 00451325 (.data cold 1)
    volatile U* counter_007b92e0;              // 00451337 = 0
    volatile U* counter_007b9310;              // 0045133D = 0
    volatile U8* texture_flag_0078e330;        // 00451372 byte = 1
    const volatile U* gl_alpha_test_0078d44c;  // 004513CA glEnable([0078D44C]) (.data GL enum cell)
    const volatile U* gl_lighting_0078d45c;    // 004513EC glEnable([0078D45C]) (.data GL enum cell)
    const volatile float* alpha_ref_007b92e8;  // 004513D9 glAlphaFunc ref (raw DWORD pushed)
    const volatile U* frame_time_0078e574;     // 00451420 WindManager::updateTimer arg (raw DWORD)
    volatile I* stat_007b9338;                 // 0045146C..
    volatile I* stat_007b933c;
    volatile I* stat_007b9340;
    volatile I* stat_007b9344;
    volatile I* stat_007b9348;
    const volatile I* stat_007a46b0;           // 0045144E
    // AurATIValidate 0045F640 (renderer capability cells; kotor_renderer_gl_cells owns 7BB85C/4FC/4D0)
    volatile U* ati_cache_0078e50c;            // cold 0xFFFFFFFF
    const volatile U* mask_0078e4d0;
    const volatile U* capabilities_007bb85c;
    const volatile U* control_0078e4fc;
    // CSWGuiScene ctor (.data cold -1.0f x3)
    const volatile U* gui_scene_color_0078d3d8; // 00417569..00417580 three DWORDs 78D3D8/78D3DC/78D3E0
    // Scene ctor
    volatile I* focus_counter_007b92d8;        // 004588EB / 0045890D
    PointerList* scene_list_007b9398;          // 00458A64..00458A97 (data 7B9398, count 7B939C, capacity 7B93A0)
    // ---- RenderSinglePass 004514F0 (GL enum cells: lane-gui-draw kotor_renderer_cells owns 448/44C/45C/474)
    const volatile U* gl_blend_0078d448;       // 004515A8 glEnable / 004519C8
    const volatile U* gl_fog_0078d450;         // 004519D0 glDisable (0x0B60; no owner yet)
    const volatile U* gl_stencil_test_0078d470;// 004515C8 / 0045199F glDisable (0x0B90; no owner yet)
    const volatile U* gl_depth_test_0078d474;  // 00451984 glDisable / 004519BF glEnable
    const volatile U* static_geometry_0078e400;// 00451644 gate (.data cold 1)
    const volatile U* shadows_0078e3a8;        // 00451750 gate (.data cold 1)
    const volatile U* dynamic_geometry_0078e3fc; // 004517A1 gate (.data cold 1)
    const volatile U* stencil_pass_007b92e4;   // 0045168B / 0045183B gate (BSS)
    const volatile I* framebuffer_mode_007fbfc4; // 00451953 (BSS) >0 -> 3Fh else 1Fh
    void* volatile* distortion_texture_007a68d8; // 00451940 = scene+E8 (BSS)
    const volatile U* programs_0078e5ec;       // 00451572 (renderer cells programs_0078e5ec, cold 1)
    ProgramParameter4f const volatile* program_env_parameter_007bb850; // 00451590 (kotor_renderer_gl_procs owner)
    ProgramParameter4f const volatile* program_parameter_nv_007bb7ac;  // 00451598 (kotor_renderer_gl_procs owner)
    volatile I* stencil_bits_0078e54c;         // 0045FD00 cache (cold 0xFFFFFFFF) written by glGetIntegerv(0D57)
    float* homogenize_00831f18;                // 004AAAB6..004AAAD9 static float[4] (BSS)
    volatile float* pass_ms_007b92b8;          // 00451703 / 004518AF (accumulated)
    volatile float* pass_ms_007b92bc;          // 00451791
    volatile float* pass_ms_007b92c0;          // 0045182B
    volatile float* pass_ms_007b92c4;          // 004517E0
    volatile float* pass_ms_007b92c8;          // 00451904
    volatile float* pass_ms_007b92cc;          // 00451A29
    volatile float* pass_ms_007b92b0;          // 00450CF9
    volatile float* pass_ms_007b92b4;          // 00450D3F
    volatile I* stencil_clear_0078e550;        // 0045FD48 cache (cold 0xFFFFFFFF)
    volatile U* vertex_program_epoch_00830540; // 004A23B0 inc (BSS)
    // ---- DoGobBuckets 00457B90
    volatile U* gob_bucket_flag_007b92fc;      // 00457BAD = 0
    void** const volatile* gob_bucket_007fbfcc;   // 00457BAC qsort base / 00457C20 (BSS)
    const volatile I* gob_bucket_count_007fbfd0;  // 00457B93 / loop bound re-read (BSS)
    volatile I* gob_bucket_snapshot_007b92ec;  // 00457BDA
    Callback const volatile* callback_0078e39c;   // 00457C15 call [0078E39C] (winmain callback_78e39c; cold 004015A0, WinMain 00404418 stores 00401510)
    const volatile U* depth_toggle_007b92f0;   // 00457C69 / 00457E55 (BSS)
    volatile U* cutscene_guard_007b9440;       // 00457CE6 static-init guard bits
    Vector* cutscene_position_007b9434;        // 00457D50
    Quaternion* cutscene_orientation_007b9424; // 00457D50
    const volatile U* focus_lights_007b9300;   // 00457DDA (BSS)
    const volatile U* render_gobs_0078e3c4;    // 00457E43 (.data cold 1)
    // ---- appended by lane-3d-construct (construction + lights sub-areas)
    const volatile U* dynamic_lights_007b92dc; // 004A3773 GetNearestLights gate (BSS cold 0; no writer: a raw-image search for
                                               // the address finds only the reads 004A3080 / 004A3773 / 004A449B)
    PointerList* static_light_rooms_007fbfe4;  // 0046CAA0 AddUnique target / 0046DDA0..0046DDC0 (BSS {data,size,capacity};
                                               // ClearBuckets 0046BEF0 writes size 0046BF6B; static dtor 0073BB30)
    const volatile float* shadow_distance_0078e3c8; // 004506BA fcomp (.data 35.0f)
    const volatile U* shadow_distance_check_0078e3cc; // 00450678 (.data cold 1)
    // ---- lane-m46 fork C2 (DoStaticMeshBeams 00450970 / DoDynamicMeshProjections 00453BB0)
    const volatile U* static_mesh_beams_0078e3d0;      // 00450970 gate (.data cold 1; no writer found)
    const volatile float* projection_min_radius_0078e3d4; // 00453C6B fcomp / 00453C78 raw copy (.data 5.0f)
    // ---- GobOrderCmp / VisibilityCheck / DoFog / DoBinBucket / MultiTexture
    const volatile U* order_by_flag_0078e3b4;  // 004502DD (.data cold 1)
    const volatile U* visibility_test_007b92f8; // 004579CE (BSS)
    const volatile float* visibility_ratio_0078e3c0;   // 00457A4C (.data 0.04f)
    const volatile float* visibility_interval_0078e3bc;// 00457A8C (.data 0.33f)
    const volatile U* force_fog_007b930c;      // 00450DE0 (BSS)
    const volatile U* black_fog_008305e0;      // 00450E30 / 004501A3 (BSS)
    const volatile U* gl_fog_start_0078d484;   // .data 0x0B63
    const volatile U* gl_fog_end_0078d488;     // .data 0x0B64
    const volatile U* gl_fog_density_0078d48c; // .data 0x0B62
    const volatile U* gl_fog_color_0078d490;   // .data 0x0B66
    const volatile U* gl_cull_face_0078d454;   // .data 0x0B44 (lane-gui-draw kotor_renderer_cells::enable_0078d454)
    const volatile U* bin_bucket_0078e3ac;     // 00450150 (.data cold 1)
    const volatile float* bin_alpha_ref_0078e3b0; // 004501DA (.data 0.4f)
    ClientActiveTexture const volatile* client_active_texture_007bb6b0; // 0045019D (kotor_renderer_gl_procs::glClientActiveTextureARB_007bb6b0)
    volatile U* current_stage_007a6898;        // 004501A9 (renderer_system::bindings().zero_007a6898)
    const volatile I* grass_materials_00831e48;   // 004501EF (BSS)
    const volatile I* bin_count_0080aa6c;      // 00450205 (BSS)
    void* const* const volatile* bins_0080aa68;   // 0045021B (BSS)
    volatile U* multitexture_cache_0078e514;   // 0045F710 (cold 0xFFFFFFFF)
    const volatile U* mask_0078e484;           // 0045F71A (renderer loader mask)
    // ---- DoMeshBuckets 00455710
    PointerList* part_list_00827f40;           // 0045573C (data 827F40, size 827F44)
    PointerList* mesh_buckets_008194e0;        // 004557BD array of {data,size,capacity}, indexed by texture ordering
    // ---- GLRender::Transform 0046F060 / Gob::PartDraw / Gob::Render
    volatile U* transform_count_00827fa8;      // 0046F078 ++
    float* transform_matrix_008280a0;          // 0046F093 static float[16]
    volatile U* transform_guard_0082809c;      // 0046F0AF static-init guard bits
    Vector* transform_position_00828090;       // 0046F118
    Quaternion* transform_orientation_00828080;// 0046F13E
    Vector* part_draw_vector_0078e8ac;         // 00473EB7 saved/restored per part (.data)
    const volatile U* transform_enabled_0078e5f0; // 00473EDA / 004784E0 (.data cold 1)
    volatile I* part_depth_0078e5e0;           // 00473F39 recursion budget (.data cold 999999)
    const volatile U* render_enabled_0078e860; // 0047826E (.data cold 1)
    const volatile U* render_flagged_0078e85c; // 00478292 (.data cold 1)
    const volatile U* render_all_00828024;     // 004782F7 (BSS)
    const volatile U* fade_test_0082806c;      // 00478340 (BSS)
    const volatile float* fade_time_0078e5a0;  // 004783FD (.data 0.25f)
    const volatile float* fade_minimum_0078e858;  // 0047843A (.data 0.25f)
    volatile U* gob_render_count_00827fac;     // 004784D0 ++
    PointerList* deferred_gobs_007fbf90;       // 00478552 CExoArrayList<int>::Add target
    const volatile I* debug_spheres_00828060;  // 00478599 cmp ...,1 (BSS)
    const volatile U* debug_frames_0082805c;   // 004785DF (BSS)
    // ---- batch 8
    const volatile U* event_time_gate_00798b14;   // 004865F5 (.data cold 1)
    PointerList* drawable_squares_007a693c;    // 0043273C (D55: binds lane-world-los kotor_world_los::drawable_squares_007a693c)
    PointerList* saber_bucket_007fbff0;        // 00450450 (data 7FBFF0, size 7FBFF4)
    // ---- AurTextureGetOrdering 00421970 (lane-texture-epoch scheduler cells: ids_007a478c, ordered_007a47a4, maximum_id_007a46bc)
    PointerList* texture_ids_007a478c;
    PointerList* texture_ordered_007a47a4;
    volatile U* texture_max_id_007a46bc;
    // ---- RenderPersonalSpace / RenderPaths
    const volatile U* personal_space_all_007b9314;   // 00450FD0 (BSS)
    const volatile U* render_paths_007b9318;   // 004510E1 (BSS)
    const volatile U* gl_texture_2d_0078d458;  // .data 0x0DE1 (lane-gui-draw kotor_renderer_cells::enable_0078d458)
    // ---- RenderLensFlares 00457EC0
    const volatile U* lens_flares_0078e3f0;    // 00457EDB (.data cold 1)
    volatile U* flare_guard_007b9458;          // static-init guard bits 1/2/4 (BSS)
    U* flare_pool_a_007b9454;                  // 00457F40 (BSS)
    U* flare_pool_b_007b9444;                  // 004585B5 (BSS)
    const volatile U* flare_hit_test_0078e3e4; // 0045804D (.data cold 1)
    const volatile U* flare_hit_order_0078e3ec;// 004580C8 (.data cold 1)
    Vector* flare_hit_vector_007b9448;         // 004580B6 static Vector
    const volatile float* flare_fade_rate_0078e3e8;   // 0045815E (.data 3.0f)
    const volatile float* flare_size_scale_0078e3e0;  // 00458562 (.data 0.1f)
    BindBuffer const volatile* bind_buffer_007bb7b4;   // 00431DFD (kotor_renderer_gl_procs::glBindBufferARB_007bb7b4)
    // ---- batch 15
    unsigned char* const volatile* grass_material_table_00831e44;   // 004A6853 (0x54-byte records)
    const volatile float* trigger_alpha_008304b8;   // 0049DA43
    const float* walkmesh_colors_0078e6a8;     // .data float[32][3]
    volatile U* walkmesh_guard_008280e4; U** walkmesh_table_008280e0;
    volatile U* walkmesh_inverse_guard_008280ec; U** walkmesh_inverse_table_008280e8;
    volatile U* rotation_arc_guard_00831f74; Quaternion* rotation_arc_00831f64;
    // ---- batch 16
    void* scene_hit_info_007b93a8;             // static CHitInfo used when HitCheck gets null (00456B73)
    void* volatile* bsp_hit_info_007fbf48;     // 00456B83 / 00456CF3 (lane-3d-bsp reads it as hit_mask_007fbf48)
    const volatile U* soft_requested_0078e420; // renderer cells soft_requested_0078e420
    const volatile U8* soft_enabled_0078dae9;  // renderer cells soft_enabled_0078dae9
    const volatile U* walkmesh_debug_007fbf5c; // 00471578 (BSS)
    volatile U* aabb_guard_00828118; U** aabb_colors_00828114; U** aabb_inverse_colors_00828110;
    volatile U* aabb_face_count_00827fb0;      // 00471768
    const volatile U* trigger_highlight_0078e614;   // 00456364 (.data cold 1)
    PointerList* trigger_list_0080aa74;        // data 80AA74 / size 80AA78
    // ---- ConstructCameraFadePlane 0046B7C0
    volatile U* fade_plane_guard_00827f74;     // static-init guard (BSS)
    Vector* fade_plane_up_00827f68;            // static Vector (0,0,1)
    const volatile float* fade_plane_width_0078e5c0;    // .data 1.0f
    const volatile float* fade_plane_fraction_0078e5bc; // .data 0.3333f
    const volatile float* fade_plane_offset_007fbf64;   // BSS
    // ---- PartLightsaber::Draw 00471860
    const volatile U* saber_enabled_0078e840;  // .data 1 (00471866)
    const volatile U* saber_simple_0078e828;   // .data 1 (00471988: plain strips while history < 2)
    const volatile U* saber_glow_0078e834;     // .data 1 (00472819: second, extrapolated pass)
    const volatile float* saber_step_0078e838; // .data 0.001f
    const volatile float* saber_min_ratio_0078e83c;   // .data 0.5f
    const volatile float* saber_max_accum_00798b30;   // .data 0.012f
    const volatile U* saber_additive_00828028; // BSS (00471908: GL_ONE/GL_ONE, no depth/stencil writes)
    volatile U* saber_strip_count_00828030;    // BSS (strip counter, reset 00471D57)
    const volatile U* saber_wireframe_00828034;// BSS (0047195A: polygon mode LINE when set)
    const volatile U* saber_lit_00828038;      // BSS (0047192E: lighting left on when set)
};
extern const Cells& cells;

// Vtable objects (data). Defined by the vtable owner; this lane stores their address only.
extern void* const gui_drawable_vtable_0073e324[];  // 00414F80 (unwind)
} // namespace kotor_scene3d

// Existing owners reused (exact declarations copied from the owner headers).
namespace kotor_renderer_init {
void __stdcall glEnable_0073d390(std::uint32_t);    // M40 aurora-windows-gl-imports
void __stdcall glDisable_0073d22c(std::uint32_t);   // M40 aurora-windows-gl-imports
bool __cdecl ATIValidate_0045f640();                // lane-upload (D52)
std::uint32_t __stdcall VertexBufferAvailable_0045fc90();   // lane-renderer-bodies extension_loader.h
}
namespace kotor_soft_shadow {   // lane-renderer-bodies capabilities.h
std::uint32_t __cdecl SoftAvailable_0045f7b0();
std::uint32_t __cdecl ATISoftAvailable_0045f7e0();
}
extern "C" {
// task-22 aurora-menu-layers-r1 (lane-gui-draw writers packet)
std::uint32_t __cdecl kotor_AurGUISetupViewport_004592F0(std::uint32_t x, std::uint32_t y, std::uint32_t width,
    std::uint32_t height, void* color, std::uint32_t depth_test_zero, std::uint32_t alpha_bits);
void __stdcall kotor_AurGUICloseViewport_00459580();
void __cdecl aurora_viewport_00459170(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
// M41 allocator family (006FA7E6 / 006FA390)
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
// lane-3d-model model_lifetime.cpp: the single original CRT rand port (D52)
int __cdecl crt_rand_006fc447();
// lane-texture-epoch scheduler.h (extern "C" __stdcall, EAX result dropped by 00451379)
std::uint32_t __stdcall texture_make_current_00421ee0();
}

// ---------------------------------------------------------------- authored here
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::I64;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PlaneList;
using kotor_scene3d::PointerList;
// GUI glue
void __fastcall gui_3d_scene_view_draw_0067acb0(void* view, void*, U delta_bits);          // vtbl+38, tail jmp
void __fastcall gui_scene_draw_00414fb0(void* gui_scene, void*, U delta_bits);              // RET 4
void* __fastcall gui_scene_construct_004174b0(void* gui_scene, void*);                      // RET 0
void __fastcall gui_drawable_destruct_00414f80(void* object, void*);                        // RET 0
PointerList* __fastcall exo_array_list_construct_00416770(PointerList*, void*, U size);     // RET 4
void __fastcall exo_array_list_destruct_0056b6f0(PointerList*, void*);                      // RET 0
// Scene
void* __cdecl new_aur_scene_00458e70(const char* name);
void* __fastcall scene_construct_00458790(void* scene, void*, const char* name);            // RET 4
void __fastcall scene_set_fog_00458b30(void* scene, void*, U fog);                          // RET 4
void __fastcall scene_disable_visibility_graph_0044f800(void* scene, void*);                // RET 0
void __fastcall void_return_14_005c8650(void*, void*, U, U, U, U, U);                       // RET 14h
void __fastcall scene_animate_00451c80(void* scene, void*, U delta_bits);                   // RET 4
void __fastcall scene_render_004512d0(void* scene, void*);                                  // RET 0
// Camera
void __fastcall camera_base_destruct_0045c240(void* camera, void*);                        // RET 0 (unwind state 0)
void __fastcall exo_array_list_free_0047abc0(void* list, void*);                            // RET 0 (unwind)
void* __cdecl new_aur_camera_0045d930(const char* name);
void* __fastcall camera_construct_0045d800(void* camera, void*, const char* name);          // RET 4
void __fastcall plane_list_allocate_0045c270(PlaneList*, void*, I size);                    // RET 4
void __fastcall camera_set_clip_dist_0045bec0(void* camera, void*, float near_clip, float far_clip); // RET 8
void __fastcall camera_set_view_angle_0045bee0(void* camera, void*, float angle);           // RET 4
I __fastcall camera_attach_to_scene_0045c0a0(void* camera, void*, void* scene);             // RET 4, EAX=1
void* __fastcall camera_attach_to_object_0045c200(void* camera, void*, void* object, const char* part, I mode); // RET 0Ch
void __fastcall camera_render_scene_0045c540(void* camera, void*);                          // RET 0
// Gob scene-graph attach
void __fastcall gob_set_scene_0043f200(void* gob, void*, void* scene);                      // RET 4
void* __fastcall gob_attach_to_object_00443820(void* gob, void*, void* object, const char* part, I mode); // RET 0Ch
// Math / GL wrappers
double __cdecl deg_to_rad_004a96d0(float degrees);                                         // ST0 wide (D30-FINAL)
void __cdecl glu_perspective_0045c300(double fovy, double aspect, double z_near, double z_far);
void __cdecl gl_ortho_00404b60(double, double, double, double, double, double);
Vector* __cdecl vector_cross_004ab210(Vector* out, const Vector* p, const Vector* q);
double __cdecl vector_magnitude_004ab100(const Vector*);                                    // ST0
Vector* __cdecl normal_of_004aca00(Vector* out, const Vector* a, const Vector* b, const Vector* c);
Vector* __cdecl quaternion_rotate_004a9bb0(Vector* out, const Quaternion* q, const Vector* v);
Quaternion* __cdecl quaternion_slerp_004a9e00(Quaternion* out, Quaternion from, const Quaternion* to, float t);   // lane-3d-anim (core)
aurora_model::MdlNode* __fastcall aurora_node_as_saber_0043e3a0(aurora_model::MdlNode* self, void* unused_edx);   // lane-3d-model model_load.hpp
U __fastcall part_lightsaber_draw_00471860(void* part, void*, U);   // Part vtbl+64 (lane-3d-model model_vtables s064)
Quaternion* __cdecl quaternion_negate_w_004a9b50(Quaternion* out, const Quaternion* q);
float* __cdecl make_matrix4x4_004aa9c0(float* out16, const Vector* translation, const Quaternion* q);
Vector* __fastcall quaternion_axis_00423c40(const Quaternion* q, void*, Vector* out);        // RET 4
void __cdecl gl_set_view_transform_00425ca0(const Vector* position, const Quaternion* orientation);
I64 __cdecl query_timer_0044f320();                                                         // EDX:EAX
void __fastcall scene_render_single_pass_004514f0(void* scene, void*);                      // Scene vtbl+B8, RET 0
float* __fastcall quaternion_get_matrix_0044f470(const Quaternion* q, void*, float* out9);   // RET 4 -> out
float* __fastcall matrix_construct_00437ba0(float* out9, void*, Vector r0, Vector r1, Vector r2); // RET 24h -> out
float* __cdecl homogenize_004aaab0(const Vector* v, const float* w);                       // -> static 00831F18
I __cdecl aur_get_available_stencil_bits_0045fd00();
I __cdecl aur_get_stencil_clear_value_0045fd20();
void __cdecl aur_vertex_program_make_current_004a23b0();
void __fastcall scene_render_static_geometry_00450c30(void* scene, void*);                  // Scene vtbl+C4
void __fastcall scene_render_dynamic_geometry_00450b50(void* scene, void*);                 // Scene vtbl+C0
void __fastcall scene_render_only_gobs_00450d60(void* scene, void*);                        // Scene vtbl+120
void __fastcall scene_do_gob_buckets_00457b90(void* scene, void*);                          // Scene vtbl+E0
int __cdecl gob_order_cmp_00450270(const void*, const void*);                               // qsort comparator
bool __fastcall gob_visibility_check_004579c0(void* gob, void*);                            // AL = [gob+18]
Vector* __cdecl vector_sum_00423b90(Vector* out, const Vector* a, const Vector* b);
I __cdecl aur_multi_texture_available_0045f710();
void __fastcall scene_do_fog_00450de0(void* scene, void*);                                  // Scene vtbl+124
void __fastcall scene_do_bin_bucket_00450150(void* scene, void*);                           // Scene vtbl+F0
void __fastcall scene_do_mesh_buckets_00455710(void* scene, void*);                         // Scene vtbl+D8
PointerList* __fastcall int_list_copy_construct_0046d250(PointerList* list, void*, const PointerList* source); // RET 4
void __fastcall int_list_add_00444fe0(PointerList* list, void*, U value);                    // RET 4
void __cdecl gl_transform_0046f060(const Vector* position, const Quaternion* orientation, U force);
void __fastcall gob_part_draw_00473ea0(void* gob, void*, void* part, U flag);               // RET 8 (recursive)
void __fastcall gob_proxy_part_draw_00474c90(void* gob, void*, void* part_ignored, U flag); // RET 8
void __fastcall gob_render_00478260(void* gob, void*, U propagate);                         // Gob vtbl+10, RET 4
void __fastcall wind_manager_fill_noise_field_004a57c0(void* wind, void*, float* field);   // RET 4
PointerList* __cdecl aur_texture_get_ordering_00421970();                                   // -> list 007A478C (core per SUBLANES)
void __cdecl transform_0046f6b0(const Vector* position, const Quaternion* orientation, U force);
void __fastcall chit_info_destruct_00456b30(void* hit, void*);                               // RET 0
void __fastcall scene_render_lens_flares_00457ec0(void* scene, void*);                       // Scene vtbl+C8
void __cdecl construct_camera_fade_plane_0046b7c0(void* scene);                              // lane-3d-bsp contract (original: EAX)
void __cdecl gl_scalef_00459150(float x, float y, float z);
Vector* __fastcall camera_set_position_0045c030(void* camera, void*, Vector* out, Vector position);  // Camera vtbl+18, RET 10h
U __fastcall part_draw_004714b0(void* part, void*, U flag);                                  // Part vtbl+64 (lane-3d-model tables)
void __cdecl gl_draw_from_verts_00451d70(U mode, const PointerList* vertices);
void __cdecl render_squares_00426000(U pool_index);
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
void __cdecl gl_render_objects_vtc_quad_00431db0(U pool, U count);
bool __cdecl aur_grass_material_in_use_004a6840(U material);                               // AL
void __fastcall part_trigger_draw_volume_0049da00(void* part, void*);
U __fastcall part_trigger_draw_0049da40(void* part, void*, U flag);                          // PartTrigger table +64, RET 4
U* __cdecl get_walk_mesh_colors_0046f200();
U* __cdecl get_inverse_walk_mesh_colors_0046f300();
Quaternion* __cdecl rotation_arc_004ab630(Quaternion* out, Vector from, Vector to);
void __cdecl gl_draw_triangles_004318f0(const PointerList* faces, const Vector* vertices, const U* colors);
void __cdecl gl_draw_sphere_bound_00431a50(float radius, const void* center, const float* color, float scale, U billboard);
void __cdecl gl_draw_axis_aligned_frame_0046ef10(const void* minimum, const void* maximum, const void* colour);
I __fastcall scene_hit_check_gob_0044fe00(void* scene, void*, Vector start, Vector end, Vector* out, void* hit);   // RET 20h
U __fastcall part_aabb_draw_00471560(void* part, void*, U flag);                             // PartAABB table +64 (lane-3d-model type)
void __cdecl qsort_006fbe00(void* base, U num, U width, int (__cdecl* comp)(const void*, const void*)); // original CRT qsort (D34)
}

// lane-3d-room (room_spawn sub-lane; lane-3d-room/source/room_spawn.cpp) - adopted from scene3d.h.proposed.diff (D42)
extern "C" {
PointerList* __fastcall vector_list_construct_00421fc0(PointerList* list, void*, I size);                 // RET 4
void* __fastcall scene_get_object_0044fdb0(void* scene, void*, const char* name);                        // Scene vtbl+0C, RET 4
void* __fastcall aur_room_construct_004548a0(void* room, void*, const char* name);                       // RET 4
void* __fastcall scene_spawn_room_00456f30(void* scene, void*, const char* name,
    const Vector* position, const Quaternion* orientation);                                              // Scene vtbl+70, RET 0Ch
void __fastcall scene_set_scene_focus_00458ba0(void* scene, void*, Vector focus);                        // Scene vtbl+48, RET 0Ch
}

// ---------------------------------------------------------------- lowers with no provider (undefined externals)
extern "C" {
// lane-m46 fork C2 callees (no provider yet; identical declarations in lane-m46-3dcore m46_scene_externs_c2.h)
PointerList* __fastcall light_manager_get_shadow_lights_004a4b10(void* lights, void* unused_edx, U mode);      // RET 4
void __fastcall plane_transform_004ab4c0(kotor_scene3d::Plane* plane, void* unused_edx, const Vector* translation,
    const Quaternion* rotation);                                                                     // RET 8
void __cdecl set_beaming_00471450();
double __cdecl round_004a96e0(float value, float precision);                                         // ST0
void* __fastcall triangle_bin_construct_004a82b0(void* self, void* unused_edx);                      // RET 0, EAX=this
void __fastcall triangle_bin_add_triangle_004a7860(void* bin, void* unused_edx, void* face, void* part, void* node); // RET 0Ch
// lane-3d-model (model loading / Gob lifetime)
aurora_model::Gob* __fastcall gob_construct_00449440(aurora_model::Gob* self, void* unused_edx, const char* name);   // RET 4
aurora_model::MaxTree* __fastcall model_construct_0044aa70(aurora_model::MaxTree* self, void* unused_edx, const char* name);   // RET 4
aurora_model::I __fastcall model_add_ref_0043e240(aurora_model::MaxTree* self, void* unused_edx, void* unused);   // RET 4 (arg = Gob*, unused by callee)
// sub-lane vp_nv_ati (SUBLANES.json)
bool __cdecl aur_nv2x_level_shaders_available_0045fbd0();                                   // AL
void __cdecl gl_program_env_parameter_0044f820(U index, float x, float y, float z, float w);
// sub-lane framebuffer_bloom
void __cdecl gl_set_film_noise_texture_00426650(void* texture);
void __cdecl gl_frame_buffer_modifications_00437520(U mask);
void __cdecl gl_frame_buffer_modifications2_004361f0(U mask);
// core successors (per-frame core, not yet authored)
bool __fastcall gob_is_in_cutscene_0043e8a0(void* gob, void*);                              // AL
// lane-3d-model owns CHitInfo::CHitInfo 004578F0 (model_instance.cpp)
void* __fastcall aurora_hit_info_construct_004578f0(void* self, void* unused_edx, aurora_model::U a, aurora_model::U b, aurora_model::U c); // RET 0Ch
// sub-lane bsp_visibility (grass bins)
void __fastcall triangle_bin_render_grass_polys_004a6d30(void* bin, void*, U material, const Vector* eye, void* wind); // RET 0Ch
// sub-lane trimesh_skin (Material)
void __fastcall material_bind_texture0_0047adf0(void* material, void*);                     // RET 0
void __fastcall material_set_blending_mode_0047af70(void* material, void*, U mode);         // RET 4
// core debug draws (gated by BSS debug flags 00828060 / 0082805C)
// sub-lane bsp_visibility / core successors (batch 8)
void __cdecl scene_bsp_add_gob_0046c420(void* scene, void* gob);
void __cdecl scene_bsp_remove_gob_0046c4a0(void* scene, void* gob);
void __fastcall scene_list_special_parts_r_004531a0(void* scene, void*, void* part);      // RET 4
void __fastcall unlist_special_parts_00453550(void* scene, void*, void* part);            // RET 4
// lane-world-los geometry_core_handover.cpp (D55 single provider)
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor);
Vector* __cdecl plane_line_intersection_004ab250(Vector* out, const Vector* normal, float distance, const Vector* a, const Vector* b);
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
void __fastcall vector_normalize_004ab130(Vector* self, void*);                             // lane-3d-model
// lane-3d-model model_load.hpp (vertex pools)
std::uint32_t __cdecl aurora_pool_request_00423ee0(std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t unused4, std::uint32_t* out);
void* __cdecl aurora_pool_lock_00423f10(std::uint32_t handle, std::uint32_t unused2, std::uint32_t unused3, std::uint32_t a4);
std::uint32_t __cdecl aurora_pool_unlock_00423f30(std::uint32_t handle);
// lane-3d-construct construct3d.h (lights)
PointerList* __fastcall light_manager_get_lens_flare_lights_004a28b0(void* lights, void*);
// camera behaviours (no provider yet)
Vector* __fastcall behavior_camera_follow_get_target_world_position_0049f0e0(void* behavior, void*, Vector* out);   // RET 4
// camera / gui-scene destructor bodies
void __fastcall camera_destruct_0045c330(void* camera, void*);
void __fastcall gui_scene_destruct_004165e0(void* gui_scene, void*);                         // CSWGuiScene::~CSWGuiScene body (not authored)
void __fastcall gob_destruct_0044b2f0(aurora_model::Gob* gob, void*);                        // Gob::~Gob body (no provider yet)
// lane-3d-model: CHitInfo::ClearOutput
void __fastcall aurora_hit_info_clear_output_00456a60(void* self, void* unused_edx);
// lane-3d-bsp bsp.h
U __cdecl hit_part_on_bsp_node_0046b4c0(void* node, Vector start, Vector end, Vector* hit);
U __cdecl bsp_hit_check_004ad260(void* node, Vector start, Vector end, Vector* hit, U (__cdecl* fn)(void*, Vector, Vector, Vector*));
// lane-world-los geometry_core_handover
I __cdecl box_intersect_004aa610(const Vector* line_start, const Vector* line_end, const Vector* box_min, const Vector* box_max);
// lane-3d-fb fb_bloom.h
void __cdecl gl_set_up_soft_shadows_0042d1c0();
void __cdecl gl_finish_soft_shadows_0042d9a0();
// lane-font font_txi.hpp (CExoArrayList<Vector>::Allocate)
void __fastcall vector_list_allocate_00421310(PointerList* list, void*, I capacity);         // RET 4
// sub-lane lights
void __fastcall light_manager_get_nearest_lights_004a3770(void* lights, void*, void* scene, Vector position, float radius, U flags); // RET 18h
// handoff lane-3d-model
aurora_model::Part* __cdecl find_part_0043fe00(aurora_model::Part* part, const char* name);
void __cdecl render_personal_space_00450fa0(void* scene);
void __cdecl render_paths_004510e0(void* scene);
void __cdecl gl_render_drawable_squares_00432720();
// scene/aurora lowers (this lane's successors)
void* __cdecl find_scene_0044fd60(const char* name);
void* __cdecl find_gob_00440460(const char* name);
void* __fastcall light_manager_construct_004a38b0(void*, void*);
void* __fastcall wind_manager_construct_004a6640(void*, void*);
void* __cdecl aur_texture_get_normalization_cube_map_004233e0();
void* __fastcall safe_pointer_assign_00447fa0(void* safe_pointer, void*, void* object);     // RET 4
void* __cdecl aur_texture_get_reference_00423a60(const char* name, U flags);
I __cdecl aur_ati_fragment_shader_available_0045f560();
void __cdecl vertex_program_manager_reset_004a2470();
void __fastcall scene_update_emitters_004511f0(void* scene, void*);
void __cdecl aur_texture_force_controller_update_00420e50();
void __cdecl manage_scene_bsp_0046e5c0(void* scene);
void __fastcall wind_manager_update_timer_004a5e30(void* wind, void*, U time_bits);         // RET 4
void* __fastcall behavior_attach_0049fb10(void*, void*, void* gob, void* object, void* part);              // RET 0Ch
void* __fastcall behavior_attach_translate_0049fdc0(void*, void*, void* gob, void* object, void* part);
void* __fastcall behavior_attach_rotate_0049fe00(void*, void*, void* gob, void* object, void* part);
void* __fastcall behavior_attach_no_orientation_0049fe40(void*, void*, void* gob, void* object, void* part);
}

// Typed Scene / Gob vtables (D42), generated: gen_vtables.py
#include "a4c4a08645ec-vtables_gen.h"
#include "5973f18f4e22-vtables2_gen.h"
