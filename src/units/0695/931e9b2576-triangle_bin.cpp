// lane-3d-bsp / triangle_bin.cpp: CAurTriangleBin grass bins (SUBLANES "bsp_visibility").
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, image base 00400000), lane-3d-bsp/d/<ADDR>.txt; every behavior
// cites its instruction addresses. .rdata constants are the pinned image values (rd.py).
// D30-FINAL: x87 values kept on the FPU stack are `double`, every `fstp dword` is a float rounding, operand order of
// each fadd/fmul chain is the original one. frndint+fistp = FloatToInt (kotor_x87.h), the original's own inline-asm
// helper: rounds in the current x87 mode (CW 027F: nearest-even), indefinite 80000000h for NaN / out of range;
// __ftol2 006FAE8C (ST0 input, not expressible as a C ABI) = truncation with the same indefinite result for NaN
// (only reachable input class here: values clamped to [0,255] or NaN).
// Unwind: 004A6D30 (handler 00718169: state 1 frees the 201Ch block if VertexProgram::VertexProgram throws) and
// 004A8500 (handler 007182E8: local CExoArrayList<CAurTriangleListElem> destroyed) are cited, not emitted; the only
// C++-linkage callees are VertexProgram::Construct_004a27d0 / Enable_004a24d0 (lane-renderer-bodies, no throw site)
// and services_tail5::Seed; live_operator_new returns null rather than throwing.
#include "ea549265b3-triangle_bin.h"
#include <cmath>
#include "../../math/kotor_x87.h"   // FloatToInt: the original's inline-asm frndint/fistp helper (L1, PLAN-v3 6.12)

namespace {
using namespace kotor_scene3d_bsp;
using kotor_scene3d::I64;

// .rdata constants (pinned image)
constexpr U kZero_0073d700 = 0x00000000;       // 0.0f
constexpr U kOne_0073d764 = 0x3F800000;        // 1.0f
constexpr U kTwo32_0073d770 = 0x4F800000;      // 4294967296.0f (fild unsigned fix-up)
constexpr U kPointFour_0073e320 = 0x3ECCCCCD;  // 0.4f
constexpr U kTenth_0073e798 = 0x3DCCCCCD;      // 0.1f
constexpr U kHalf_0073e9ac = 0x3F000000;       // 0.5f
constexpr U k255_0073ede4 = 0x437F0000;        // 255.0f
constexpr U kTwoPi_00740c88 = 0x40C90FDB;      // 6.28319f
constexpr U kTenThousandth_00741870 = 0x38D1B717; // 0.0001f
constexpr U kFive_00743264 = 0x40A00000;       // 5.0f
constexpr U kMillion_00743750 = 0x49742400;    // 1e6f
constexpr U kMinusHalf_00743884 = 0xBF000000;  // -0.5f
constexpr U kTenMillion_00743fe0 = 0x4B189680; // 1e7f
constexpr U kDivisorTwo = 0x40000000;          // 2.0f immediate (004A67B9)
// GL enums read from .rdata (0073F3A4 / 0073F39C / 0073F398) and immediates
constexpr U kGlVertexArray_0073f3a4 = 0x8074, kGlNormalArray_0073f39c = 0x8075, kGlTexCoordArray_0073f398 = 0x8078;
constexpr U kGlTextureEnv = 0x2300, kGlTextureEnvMode = 0x2200, kGlReplace = 0x1E01, kGlModulate = 0x2100;

inline double F(U bits) { return Wb(bits); }
inline U8 flags_of(void* bin) { return field<U8>(bin, 2); }
inline unsigned char* byte_ptr(U address) { return reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(address)); }
inline U address_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U word_at(U address) { return *reinterpret_cast<const U*>(static_cast<std::uintptr_t>(address)); }
inline float float_at(U address) { return bits_to_float(word_at(address)); }
inline void put_word(U address, U value) { *reinterpret_cast<U*>(static_cast<std::uintptr_t>(address)) = value; }
inline void put_float(U address, float value) { put_word(address, float_to_bits(value)); }
inline void copy_vector(void* destination, const void* source) { std::memcpy(destination, source, 12); }  // three DWORD movs
// [00831E44] + index*54h (004A69F6..004A6A04 imul 54h; address arithmetic is 32-bit)
inline U grass_record(U index) { return *grass_cells.grass_info_00831e44 + index * 0x54u; }
inline void texture_bind(void* texture) { slots<TextureSlots>(texture).bind(texture, nullptr); }
inline texture::Basic* as_texture(void* texture) { return static_cast<texture::Basic*>(texture); }
// fild of a DWORD then `test reg,reg; jge; fadd [0073D770]` (unsigned reinterpretation)
inline double fild_unsigned(U value) {
    double v = static_cast<double>(static_cast<I>(value));
    if (static_cast<I>(value) < 0) v += F(kTwo32_0073d770);
    return v;
}
// __ftol2 006FAE8C (truncation toward zero, 64-bit result; indefinite for NaN), low byte taken by `mov [..],al`.
inline U8 ftol2_low_byte(double value) {
    if (!(value == value) || !(value > -9.2233720368547758e18 && value < 9.2233720368547758e18)) return 0;
    return static_cast<U8>(static_cast<std::uint64_t>(static_cast<I64>(value)));
}
// rand() % 10000 (cdq; idiv 2710h; remainder in EDX)
inline I rand_10000() { return crt_rand_006fc447() % 10000; }
inline void add_word(PointerList* list, U value) {
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    reinterpret_cast<U*>(static_cast<std::uintptr_t>(list->data))[list->size] = value;
    list->size = list->size + 1;
}
// texture of a triangle-list element: [[[elem+10h]+44h]+4] (004A855F..004A8569, 004A862D..004A863A, 004A86F5..004A8706)
inline void* element_texture(U element) {
    const U part = word_at(element + 0x10);
    const U material = word_at(part + 0x44);
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(material + 4)));
}
} // namespace

extern "C" {

// 004A67B0 CAurTriangleBin::FinishGeometry, thiscall RET 0.
void __fastcall triangle_bin_finish_geometry_004a67b0(void* bin, void*) {
    Vector sum;
    sum.x = S(W(field<float>(bin, 0x14)) + W(field<float>(bin, 0x20)));   // 004A67B6..004A67CA
    sum.y = S(W(field<float>(bin, 0x18)) + W(field<float>(bin, 0x24)));   // 004A67CF..004A67D5
    sum.z = S(W(field<float>(bin, 0x1C)) + W(field<float>(bin, 0x28)));   // 004A67D9..004A67DF
    Vector centre;
    const Vector* half = vector_divide_00437a90(&centre, &sum, bits_to_float(kDivisorTwo)); // 004A67B9..004A67E3
    copy_vector(at(bin, 0x2C), half);                                      // 004A67E8..004A67F8
    Vector extent;
    extent.x = S(W(field<float>(bin, 0x14)) - W(field<float>(bin, 0x20))); // 004A67FB..004A6806
    extent.y = S(W(field<float>(bin, 0x18)) - W(field<float>(bin, 0x24))); // 004A680A..004A6810
    extent.z = S(W(field<float>(bin, 0x1C)) - W(field<float>(bin, 0x28))); // 004A6814..004A681A
    field<float>(bin, 0x08) = S(vector_magnitude_004ab100(&extent) * F(kHalf_0073e9ac)); // 004A681E..004A682C
}

// 004A68B0 CAurTriangleBin::UpdateGrassForWind(WindManager*), thiscall RET 4.
void __fastcall triangle_bin_update_grass_for_wind_004a68b0(void* bin, void*, void* wind) {
    if (((~static_cast<U>(flags_of(bin))) & 4u) != 0) return;            // 004A68B4..004A68BF (built bit required)
    if (grass_wind_update_007a1974 == 0) return;                          // 004A68C5..004A68D0
    void* scene = *grass_cells.active_scene_007b92a0;                    // 004A68D6
    if (scene_slots(scene).s0a8_scene_is_animating(scene, nullptr) == 0) return; // 004A68DC..004A68E6 (vtbl+A8)
    U dt_bits = kTenth_0073e798;                                          // 004A690A
    if (Wb(*grass_cells.frame_time_0078e574) < F(kTenth_0073e798))       // 004A68EC..004A68FD (test ah,5 / jp)
        dt_bits = *grass_cells.frame_time_0078e574;                       // 004A68FF..004A6904
    U guard = grass_wind_guard_00831e8c;                                  // 004A6912 function-static guards
    if ((guard & 1u) == 0) { guard |= 1u; grass_wind_guard_00831e8c = guard; grass_offset_00831e80 = Vector{0, 0, 0}; }      // 004A6917..004A6937
    if ((guard & 2u) == 0) { guard |= 2u; grass_wind_guard_00831e8c = guard; grass_wind_sum_00831e74 = Vector{0, 0, 0}; }    // 004A6941..004A6961
    if ((guard & 4u) == 0) { guard |= 4u; grass_wind_guard_00831e8c = guard; grass_global_wind_00831e68 = Vector{0, 0, 0}; } // 004A696B..004A698B
    if ((guard & 8u) == 0) { guard |= 8u; grass_wind_guard_00831e8c = guard; grass_point_wind_00831e5c = Vector{0, 0, 0}; }  // 004A6995..004A69B5
    if ((guard & 0x10u) == 0) { guard |= 0x10u; grass_wind_guard_00831e8c = guard; grass_point_00831e50 = Vector{0, 0, 0}; } // 004A69BF..004A69DF
    const double magnitude = vector_magnitude_004ab100(static_cast<const Vector*>(at(wind, 0x18))); // 004A69E9..004A69F1
    const float height = float_at(grass_record(field<U>(bin, 0x10)) + 0x24); // 004A69F6..004A6A10
    float scale = bits_to_float(kOne_0073d764);                           // 004A6A08 (stored over the argument slot)
    if (magnitude > F(kFive_00743264)) scale = S(F(kFive_00743264) / magnitude); // 004A6A14..004A6A29
    for (I index = 0; index < field<I>(bin, 0x0C); ++index) {             // 004A6A2D..004A6A38, 004A6C46..004A6C57
        const U record = field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u; // [edi+3Ch] + esi (esi += 30h)
        const float r00 = float_at(record + 0x00);                        // 004A6A46 -> [esp+14h]
        const float r04 = float_at(record + 0x04);                        // 004A6A4C -> [esp+18h]
        grass_point_00831e50.z = float_at(record + 0x08);                 // 004A6A53..004A6A56 (raw DWORD)
        const float r10 = float_at(record + 0x10);                        // 004A6A5F -> [esp+28h]
        const float r0c = float_at(record + 0x0C);                        // 004A6A69 -> [esp+24h]
        grass_point_00831e50.x = S((W(r0c) + W(r00)) * F(kHalf_0073e9ac)); // 004A6A70..004A6A93
        grass_point_00831e50.y = S((W(r10) + W(r04)) * F(kHalf_0073e9ac)); // 004A6A99..004A6AA7
        Vector global_out;
        const Vector* global = wind_manager_get_global_wind_no_point_004a5950(wind, nullptr, &global_out,
            &grass_point_00831e50, bits_to_float(kOne_0073d764));        // 004A6A7C..004A6AAD
        grass_global_wind_00831e68.x = S(W(scale) * W(global->x));        // 004A6AB2..004A6ADC
        grass_global_wind_00831e68.y = S(W(scale) * W(global->y));        // 004A6AC6..004A6AF1
        grass_global_wind_00831e68.z = S(W(scale) * W(global->z));        // 004A6AD1..004A6AF7
        Vector point_out;
        const Vector* point = wind_manager_get_global_point_wind_004a6440(wind, nullptr, &point_out,
            &grass_point_00831e50, bits_to_float(dt_bits));               // 004A6AB6..004A6AFC
        copy_vector(&grass_point_wind_00831e5c, point);                   // 004A6B01..004A6B15
        const float r24 = float_at(field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u + 0x24); // 004A6B1B..004A6B1E
        const float r28 = float_at(field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u + 0x28); // 004A6B22..004A6B30
        grass_offset_00831e80.z = 0.0f;                                   // 004A6B26
        grass_offset_00831e80.x = S(W(r00) - W(r24));                     // 004A6B34..004A6B3A
        grass_offset_00831e80.y = S(W(r04) - W(r28));                     // 004A6B40..004A6B48
        const double a = W(grass_global_wind_00831e68.x) + W(grass_offset_00831e80.x); // 004A6B4E..004A6B54
        const double b = W(grass_global_wind_00831e68.y) + W(grass_offset_00831e80.y); // 004A6B5A..004A6B60
        const float global_z = grass_global_wind_00831e68.z;              // 004A6B66..004A6B6C
        const float sum_x = S(a + W(grass_point_wind_00831e5c.x));        // 004A6B70..004A6B78
        grass_wind_sum_00831e74.x = sum_x;                                // 004A6B88
        const float sum_y = S(W(grass_point_wind_00831e5c.y) + b);        // 004A6B7C..004A6B8D
        grass_wind_sum_00831e74.y = sum_y;                                // 004A6B97
        const float sum_z = S(W(grass_point_wind_00831e5c.z) + W(global_z)); // 004A6B9D..004A6BA7
        grass_wind_sum_00831e74.z = sum_z;                                // 004A6BB9
        if (!(sum_x == 0.0f) || !(sum_y == 0.0f) || !(sum_z == 0.0f)) {   // 004A6BAB..004A6BE6 (test ah,44h)
            const double k = W(grass_wind_scale_007a1978);
            const double nx = W(r24) + ((W(sum_x) * k) * W(height)) * Wb(dt_bits); // 004A6BE8..004A6BFD
            const double ny = (((W(sum_y) * k) * W(height)) * Wb(dt_bits)) + W(r28); // 004A6BFF..004A6C11
            const U base = field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u;
            put_float(base + 0x24, S(nx));                                // 004A6C15..004A6C17
            put_float(field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u + 0x28, S(ny)); // 004A6C1E
            put_float(field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u + 0x18, S((nx - W(r00)) + W(r0c))); // 004A6C25..004A6C2F
            put_float(field<U>(bin, 0x3C) + static_cast<U>(index) * 0x30u + 0x1C, S((ny - W(r04)) + W(r10))); // 004A6C33..004A6C3E
        }
    }
}

// 004A6D30 CAurTriangleBin::RenderGrassPolys(int material, Vector* eye, WindManager* wind), thiscall RET 0Ch.
// `eye` is not read by the original.
void __fastcall triangle_bin_render_grass_polys_004a6d30(void* bin, void*, U material, const Vector* eye, void* wind) {
    (void)eye;
    using namespace kotor_renderer_init;
    if (!aur_ati_fragment_shaders_bump_map_available_0045fb90() &&        // 004A6D4B..004A6D52
        aur_multi_texture_available_0045f710() == 0) return;              // 004A6D54..004A6D5B
    if (((~static_cast<U>(flags_of(bin))) & 4u) != 0) return;            // 004A6D61..004A6D6A
    if (field<I>(bin, 0x0C) == 0) return;                                 // 004A6D70..004A6D79
    if (material != field<U>(bin, 0x10)) return;                          // 004A6D7F..004A6D86
    const U no_fade = grass_no_fade_00831e40;                             // 004A6D8C
    double near_distance = W(grass_near_007a1964);                        // 004A6D91
    double far_distance = W(bsp_bin_radius_sq_007a1968);                  // 004A6D99
    if (no_fade != 0) { near_distance = F(kMillion_00743750); far_distance = F(kTenMillion_00743fe0); } // 004A6DA1..004A6DAB
    float alpha;                                                          // [esp+0Ch]
    if (W(field<float>(bin, 4)) < near_distance) {                        // 004A6DB1..004A6DBB
        alpha = bits_to_float(kOne_0073d764);                             // 004A6DBF
    } else {
        if (!(far_distance > W(field<float>(bin, 4)))) return;            // 004A6DCB..004A6DD3
        const double fraction = (W(field<float>(bin, 4)) - near_distance) / (far_distance - near_distance); // 004A6DD9..004A6DE2
        const double remaining = F(kOne_0073d764) - fraction;              // 004A6DE4 fsubr
        alpha = S(remaining);                                             // 004A6DEA
        if (remaining == F(kZero_0073d700)) return;                       // 004A6DF0..004A6DFB (test ah,44h / jnp)
    }
    const U offset = material * 0x54u;                                    // 004A6E07
    float one = bits_to_float(kOne_0073d764);                             // 004A6E14
    gl_material_fv_0044f880(0, 1, homogenize_004aaab0(reinterpret_cast<const Vector*>(byte_ptr(*grass_cells.grass_info_00831e44 + offset + 0x34)), &one)); // 004A6E01..004A6E25
    gl_material_fv_0044f880(0, 2, homogenize_004aaab0(reinterpret_cast<const Vector*>(byte_ptr(*grass_cells.grass_info_00831e44 + offset + 0x28)), &alpha)); // 004A6E2A..004A6E43
    if (grass_wind_enabled_007a196c != 0 &&                               // 004A6E48..004A6E52
        W(field<float>(bin, 4)) < W(grass_near_007a1964)) {               // 004A6E54..004A6E62
        field<U>(bin, 0x3C) = field<U>(bin, 0x38);                        // 004A6E64..004A6E6B
        triangle_bin_update_grass_for_wind_004a68b0(bin, nullptr, wind);  // 004A6E6E..004A6E71
        field<U>(bin, 0x3C) = 0;                                          // 004A6E76
    }
    if (grass_textured_007a1970 != 0) texture_bind(field<void*>(bin, 0x54)); // 004A6E79..004A6E86
    glEnableClientState_0073d260(kGlVertexArray_0073f3a4);                // 004A6E8C..004A6E9A
    glEnableClientState_0073d260(kGlNormalArray_0073f39c);                // 004A6E9C..004A6EA3
    glEnableClientState_0073d260(kGlTexCoordArray_0073f398);              // 004A6EA5..004A6EAB
    const I attribute_stride = static_cast<I>(field<U16>(bin, 0)) - 12;   // 004A6EAD..004A6EB9 (stored over arg 1)
    if ((flags_of(bin) & 8u) != 0 && grass_textured_007a1970 != 0) {      // 004A6EB0..004A6EC9
        if (kotor_vertex_program::ValidateNonRadeon_0045f5e0() == 0) {    // 004A6ECF..004A6ED6
            glEnable_0073d390(*grass_cells.vertex_program_0078d46c);      // 004A6EDC..004A6EE3
            if ((grass_program_guard_00831e94 & 1u) == 0) {               // 004A6EE9..004A6EF0
                grass_program_guard_00831e94 = grass_program_guard_00831e94 | 1u; // 004A6EF2
                void* memory = live_operator_new(0x201C);                  // 004A6EF9..004A6F02
                kotor_vertex_program::VertexProgram* program = nullptr;  // 004A6F3A
                if (memory != nullptr) {                                  // 004A6F0E..004A6F15
                    char* source = (*grass_cells.programs_0078e5ec != 0)  // 004A6F17..004A6F24
                        ? kotor_trimesh_data::arb_grass_program_0078feb8 : kotor_trimesh_data::nv_program_0079ab28;
                    program = static_cast<kotor_vertex_program::VertexProgram*>(memory)->Construct_004a27d0(
                        source, static_cast<std::uint32_t*>(*grass_cells.bss_00830560)); // 004A6F29..004A6F33
                }
                grass_vertex_program_00831e90 = program;                  // 004A6F3C
            }
            grass_vertex_program_00831e90->Enable_004a24d0(nullptr, 1);  // 004A6F49..004A6F52
            const U record = *grass_cells.grass_info_00831e44 + offset;  // 004A6F57
            gl_program_env_parameter_0044f820(0x5E, float_at(record + 0x28), float_at(record + 0x2C),
                                              float_at(record + 0x30), alpha); // 004A6F5C..004A6F72
            const U record2 = *grass_cells.grass_info_00831e44 + offset;  // 004A6F77
            gl_program_env_parameter_0044f820(0x5F, float_at(record2 + 0x34), float_at(record2 + 0x38),
                                              float_at(record2 + 0x3C), bits_to_float(kOne_0073d764)); // 004A6F7C..004A6F92
        }
        bool client_tail = false;
        if (aur_ati_fragment_shaders_bump_map_available_0045fb90()) {      // 004A6F9A..004A6FA1
            gl_render_activate_client_texture_stage_00425250(1);         // 004A6FA7..004A6FA9
            glEnableClientState_0073d260(kGlTexCoordArray_0073f398);      // 004A6FAE..004A6FB8
            gl_render_activate_client_texture_stage_00425250(0);         // 004A6FBA..004A6FBB
            I first = 0;                                                  // ebp
            for (I index = 0; index < list_at(bin, 0x58)->size; ++index) { // 004A6FC0..004A6FCC, 004A700D..004A7014 (size re-read)
                (*grass_cells.enable_dispatch_007a47e0)(0x1A, as_texture(field<void*>(bin, 0x54)),
                    as_texture(elements(list_at(bin, 0x64))[index]), nullptr, nullptr); // 004A6FD1..004A6FE2
                gl_render_draw_lightmapped_grass_00426230(field<void*>(bin, 0x38), 0x0C, at(bin, 0x44), attribute_stride,
                    first, reinterpret_cast<I*>(elements(list_at(bin, 0x58)))[index]); // 004A6FE8..004A6FFC
                first += reinterpret_cast<I*>(elements(list_at(bin, 0x58)))[index] * 4; // 004A7001..004A7010 (lea ebp,[ebp+ecx*4])
            }
            (*grass_cells.enable_dispatch_007a47e0)(0, nullptr, nullptr, nullptr, nullptr); // 004A7016..004A7020
            gl_render_activate_client_texture_stage_00425250(1);         // 004A7026..004A7028
            client_tail = true;                                           // 004A7030 jmp 004A7196
        } else if (aur_multi_texture_available_0045f710() != 0) {        // 004A7035..004A703C
            gl_render_activate_client_texture_stage_00425250(1);         // 004A7042..004A7044
            glEnableClientState_0073d260(kGlTexCoordArray_0073f398);      // 004A7049..004A7052
            gl_render_activate_client_texture_stage_00425250(0);         // 004A7054..004A7055
            glDisable_0073d22c(*bsp_cells.gl_lighting_0078d45c);          // 004A705A..004A7064
            glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                   // 004A706A..004A707E
            kotor_renderer_gl::ActivateTextureStage_0041fe80(0);          // 004A7084..004A7086
            glEnable_0073d390(*bsp_cells.gl_texture_2d_0078d458);         // 004A708B..004A7095
            texture_bind(field<void*>(bin, 0x54));                        // 004A709B..004A70A0
            glTexEnvi_0073d2f4(kGlTextureEnv, kGlTextureEnvMode, static_cast<I>(kGlReplace)); // 004A70A6..004A70BB
            I first = 0;                                                  // ebx
            for (I index = 0; index < list_at(bin, 0x58)->size; ++index) { // 004A70BD..004A70C4, 004A7125..004A7131
                kotor_renderer_gl::ActivateTextureStage_0041fe80(1);      // 004A70C6..004A70C8
                glEnable_0073d390(*bsp_cells.gl_texture_2d_0078d458);     // 004A70CD..004A70D7
                texture_bind(elements(list_at(bin, 0x64))[index]);        // 004A70DD..004A70E5
                glTexEnvi_0073d2f4(kGlTextureEnv, kGlTextureEnvMode, static_cast<I>(kGlModulate)); // 004A70EB..004A70FA
                kotor_renderer_gl::ActivateTextureStage_0041fe80(0);      // 004A70FC..004A70FE
                gl_render_draw_lightmapped_grass_00426230(at(bin, 0x40), 0x0C, at(bin, 0x44), attribute_stride,
                    first, reinterpret_cast<I*>(elements(list_at(bin, 0x58)))[index]); // 004A7103..004A711A
                first += reinterpret_cast<I*>(elements(list_at(bin, 0x58)))[index] * 4; // 004A711F..004A712E
            }
            kotor_renderer_gl::ActivateTextureStage_0041fe80(0);          // 004A7133..004A7135
            glTexEnvi_0073d2f4(kGlTextureEnv, kGlTextureEnvMode, static_cast<I>(kGlModulate)); // 004A713D..004A714C
            kotor_renderer_gl::ActivateTextureStage_0041fe80(1);          // 004A714E..004A7150
            glDisable_0073d22c(*bsp_cells.gl_texture_2d_0078d458);        // 004A7155..004A715E
            kotor_renderer_gl::ActivateTextureStage_0041fe80(0);          // 004A7164..004A7166
            glColor4f_0073d224(0.0f, 0.0f, 0.0f, 1.0f);                   // 004A716E..004A7179
            glEnable_0073d390(*bsp_cells.gl_lighting_0078d45c);           // 004A717F..004A7186
            gl_render_activate_client_texture_stage_00425250(1);         // 004A718C..004A718E
            client_tail = true;
        }
        if (client_tail) {
            glDisableClientState_0073d264(kGlTexCoordArray_0073f398);     // 004A7196..004A719D
            gl_render_activate_client_texture_stage_00425250(0);         // 004A71A3..004A71A5
        }
        if (kotor_vertex_program::ValidateNonRadeon_0045f5e0() == 0)      // 004A71AD..004A71B4
            glDisable_0073d22c(*grass_cells.vertex_program_0078d46c);     // 004A71B6..004A71BC
    } else {
        gl_render_draw_lightmapped_grass_00426230(at(bin, 0x40), 0x0C, at(bin, 0x44), attribute_stride, 0,
                                                  field<I>(bin, 0x0C));   // 004A71C4..004A71D4
    }
    *grass_cells.triangles_00827fb0 = *grass_cells.triangles_00827fb0 + field<I>(bin, 0x0C); // 004A71DC..004A71E9
}

// 004A8460 CAurTriangleBin::DestroyGrassPolys, thiscall RET 0.
void __fastcall triangle_bin_destroy_grass_polys_004a8460(void* bin, void*) {
    if (((~static_cast<U>(flags_of(bin))) & 4u) != 0) return;            // 004A8463..004A846F
    if ((flags_of(bin) & 1u) != 0) {                                      // 004A8478..004A847A
        live_free(field<void*>(bin, 0x3C));                               // 004A847C..004A8480
        field<U>(bin, 0x3C) = 0;                                          // 004A8489
        field<U>(bin, 0x40) = 0;                                          // 004A848C
        field<U>(bin, 0x44) = 0;                                          // 004A848F
        live_free(field<void*>(bin, 0x38));                               // 004A8485..004A8492
        field<U8>(bin, 2) = static_cast<U8>(flags_of(bin) ^ 1u);          // 004A8497..004A84A2
        field<U>(bin, 0x38) = 0;                                          // 004A849F
    }
    if ((flags_of(bin) & 8u) != 0) {                                      // 004A84A5..004A84A9
        if (field<U>(bin, 0x58) != 0) {                                   // 004A84AB..004A84B0
            live_free(field<void*>(bin, 0x58));                           // 004A84B2..004A84B3
            field<U>(bin, 0x58) = 0;                                      // 004A84BB
        }
        field<U>(bin, 0x60) = 0;                                          // 004A84BE
        field<U>(bin, 0x5C) = 0;                                          // 004A84C1
        const U textures = field<U>(bin, 0x64);                           // 004A84C4
        if (textures != 0) {                                              // 004A84C7..004A84C9
            c2da_destroy_array_006fa852(byte_ptr(textures), 4, static_cast<I>(word_at(textures - 4)),
                                        aurora_texture_safe_pointer_destroy_00422380); // 004A84CB..004A84DB
            live_free(byte_ptr(textures - 4));                            // 004A84E0..004A84E1
            field<U>(bin, 0x64) = 0;                                      // 004A84E9
        }
        field<U>(bin, 0x6C) = 0;                                          // 004A84ED
        field<U>(bin, 0x68) = 0;                                          // 004A84F0
    }
    field<U8>(bin, 2) = static_cast<U8>(flags_of(bin) ^ 4u);              // 004A84F3
}

// 004A8500 CAurTriangleBin::SortTriangles, thiscall RET 0.
void __fastcall triangle_bin_sort_triangles_004a8500(void* bin, void*) {
    if (field<I>(bin, 0x4C) == 1) {                                       // 004A851B..004A8526
        add_word(list_at(bin, 0x58), word_at(list_at(bin, 0x48)->data + 0x14)); // 004A852C..004A855C
        safe_texture_list_add_004a8230(list_at(bin, 0x64), nullptr,
            SafeTextureArg(element_texture(list_at(bin, 0x48)->data)));   // 004A855F..004A85A8
        return;
    }
    PointerList local;                                                    // [esp+24h] (EH state 0)
    triangle_list_copy_construct_004a81b0(&local, nullptr, list_at(bin, 0x48)); // 004A85C0..004A85C8
    const I size = local.size;                                            // 004A85CD
    const U data = local.data;                                            // 004A85D1
    qsort_006fbe00(byte_ptr(data), static_cast<U>(size), 0x18, grass_sort_tris_004a7690); // 004A85D5..004A85E4
    I run = 0;                                                            // ebp
    I tail = 1;                                                           // 004A85E9 eax = 1
    if (size > 1) {                                                       // 004A85F1..004A85F3
        U element = data;                                                 // ebx = data + 14h
        for (I remaining = size - 1; remaining != 0; --remaining, element += 0x18u) { // 004A85FE..004A86CD
            if (word_at(element + 0x10) == word_at(element + 0x28)) {    // 004A8611..004A8617
                run += static_cast<I>(word_at(element + 0x14));           // 004A8619
                continue;
            }
            const U original = list_at(bin, 0x48)->data + (element - data); // 004A8620..004A862D (unsorted list, same index)
            safe_texture_list_add_004a8230(list_at(bin, 0x64), nullptr,
                SafeTextureArg(element_texture(original)));               // 004A8631..004A867D
            add_word(list_at(bin, 0x58), word_at(element + 0x14) + static_cast<U>(run)); // 004A8682..004A86B8
            run = 0;                                                      // 004A86BF
        }
        tail = size;                                                      // 004A86D7
    }
    const U last = static_cast<U>(tail) * 0x18u;                          // 004A86DB..004A86DE
    if (word_at(data + last - 8) == word_at(data + last - 0x20))          // 004A86E1..004A86E9
        run += static_cast<I>(word_at(data + last - 4));                  // 004A86EB
    else
        run = static_cast<I>(word_at(data + last - 4));                   // 004A86F1
    safe_texture_list_add_004a8230(list_at(bin, 0x64), nullptr,
        SafeTextureArg(element_texture(list_at(bin, 0x48)->data + last - 0x18))); // 004A86F5..004A8745
    add_word(list_at(bin, 0x58), static_cast<U>(run));                    // 004A874A..004A877E
    const U local_data = local.data;                                      // 004A8777
    if (local_data != 0) {                                                // 004A877C..004A8789
        c2da_destroy_array_006fa852(byte_ptr(local_data), 0x18, static_cast<I>(word_at(local_data - 4)),
                                    triangle_list_elem_destruct_004a72d0); // 004A878B..004A879A
        live_free(byte_ptr(local_data - 4));                              // 004A879F..004A87A0
    }
}

// 004A87C0 CAurTriangleBin::CreateArrays, thiscall RET 0.
void __fastcall triangle_bin_create_arrays_004a87c0(void* bin, void*) {
    U count_words = field<U>(bin, 0x0C) << 2;                             // 004A87CD..004A87D3
    void* buffer = live_operator_new(static_cast<U>(field<U16>(bin, 0)) * count_words); // 004A87D0..004A87DB
    field<void*>(bin, 0x38) = buffer;                                     // 004A87E4
    count_words = (count_words * 3u) << 2;                                // 004A87EB..004A87EE
    field<U>(bin, 0x44) = count_words;                                    // 004A87F1
    const U vertices = field<U>(bin, 0x38);                               // 004A87F8 (edi)
    field<U>(bin, 0x3C) = vertices;                                       // 004A87FB
    if ((flags_of(bin) & 8u) != 0) triangle_bin_sort_triangles_004a8500(bin, nullptr); // 004A8802..004A880C
    const I elements = field<I>(bin, 0x4C);                               // 004A8818
    field<U8>(bin, 2) = static_cast<U8>(flags_of(bin) | 1u);              // 004A8815..004A8821
    U attribute = (count_words >> 2) - 1u;                                // 004A881E..004A8824 (esi)
    const Vector axis = {0.0f, 0.0f, bits_to_float(kOne_0073d764)};       // [esp+DCh] 004A8827..004A883D
    I vertex = -1;                                                        // [esp+30h] 004A8848
    Quaternion rotation = {bits_to_float(kOne_0073d764), 0.0f, 0.0f, 0.0f}; // [esp+70h] 004A8868..004A8880
    if (elements <= 0) {                                                  // 004A8888
        field<U>(bin, 0x3C) = 0;                                          // 004A9611
        field<U8>(bin, 2) = static_cast<U8>(flags_of(bin) | 4u);          // 004A961C..004A9625
        return;
    }
    U element_offset = 0;                                                 // ebx / [esp+84h]
    for (I remaining = elements; remaining != 0; --remaining, element_offset += 0x18u) { // 004A95EC..004A9605
        U guard = grass_corner_guard_00831ee4;                            // 004A888E / 004A95E6
        if ((guard & 1u) == 0) { guard |= 1u; grass_corner_guard_00831ee4 = guard; grass_corner0_00831ed8 = Vector{0, 0, 0}; } // 004A88A8..004A88CA
        if ((guard & 2u) == 0) { guard |= 2u; grass_corner_guard_00831ee4 = guard; grass_corner1_00831ecc = Vector{0, 0, 0}; } // 004A88D4..004A88F6
        if ((guard & 4u) == 0) { guard |= 4u; grass_corner_guard_00831ee4 = guard; grass_corner2_00831ec0 = Vector{0, 0, 0}; } // 004A8900..004A8922
        const U corners = word_at(list_at(bin, 0x48)->data + element_offset);  // [[bin+48]+ebx] (re-read per DWORD)
        copy_vector(&grass_corner0_00831ed8, byte_ptr(corners));          // 004A892C..004A8952
        copy_vector(&grass_corner1_00831ecc, byte_ptr(corners + 0x0C));   // 004A8958..004A897F
        copy_vector(&grass_corner2_00831ec0, byte_ptr(corners + 0x18));   // 004A8985..004A89AC
        const I blades = static_cast<I>(word_at(list_at(bin, 0x48)->data + element_offset + 0x14)); // 004A89B2..004A89BB
        if (blades == 0) continue;                                        // 004A89BF
        const float seed_sum = S((W(grass_corner2_00831ec0.z) + W(grass_corner1_00831ecc.y)) + W(grass_corner0_00831ed8.x)); // 004A89C5..004A89DF
        I seed;                                                           // [esp+6Ch] (pointer kept at [esp+4Ch])
        FloatToInt(&seed, seed_sum);                                      // 004A89DF..004A89ED fstp dword; fld dword; mov edx; frndint; fistp [edx]
        services_tail5::Seed(static_cast<U>(seed));                       // 004A89EF..004A89F4 push [esp+6Ch]; call srand 006FC43A
        const double height = W(float_at(grass_record(field<U>(bin, 0x10)) + 0x24)); // 004A89F9..004A8A0C
        const double low = F(kMinusHalf_00743884) * height;               // 004A8A1B..004A8A2C
        const double half = height * F(kHalf_0073e9ac);                   // 004A8A61..004A8A63
        const Vector corner_b8 = {S(low), 0.0f, 0.0f};                    // [B8],[BC],[C0] 004A8A10..004A8A4F
        const Vector corner_a0 = {S(half), 0.0f, 0.0f};                   // [A0],[A4],[A8] 004A8A69
        const Vector corner_ac = {S(half), 0.0f, S(height)};              // [AC],[B0],[B4] 004A8A70..004A8A79
        const Vector corner_94 = {S(low), 0.0f, S(height)};               // [94],[98],[9C] 004A8A80..004A8A87
        const U element = list_at(bin, 0x48)->data + element_offset;      // 004A8A8E
        const U face = word_at(element + 0x0C);                           // 004A8A91 (ebp)
        const U part = word_at(element + 0x10);                           // 004A8A98 ([esp+48h])
        const U trimesh = address_of(aurora_node_as_trimesh_0043e400(
            reinterpret_cast<aurora_model::MdlNode*>(byte_ptr(word_at(part + 4))), nullptr)); // 004A8A9B..004A8AA7
        float lightmap[3] = {0.0f, 0.0f, 0.0f};                           // [60],[64] = 0 (004A8AB1..004A8AB9); [68]
        U8 red = 0, green = 0, blue = 0, alpha_byte = 0x7F;               // [18],[1A],[19],[1B] 004A8AC1..004A8AD0
        if (blades <= 0) continue;                                        // 004A8AD5
        for (I blade = blades; blade != 0; --blade) {                     // [esp+4Ch] 004A8AE5, 004A95CF..004A95D3
            const float a = S(static_cast<double>(rand_10000()) * F(kTenThousandth_00741870)); // 004A8AF0..004A8B0B
            const float b = S(static_cast<double>(rand_10000()) * F(kTenThousandth_00741870)); // 004A8B0F..004A8B2A
            const double c_wide = static_cast<double>(rand_10000()) * F(kTenThousandth_00741870); // 004A8B2E..004A8B43
            const float c = S(c_wide);                                    // 004A8B49 fst
            const double sum = (c_wide + W(b)) + W(a);                    // 004A8B4D..004A8B51
            float w0, w1, w2;                                             // [20],[24],[28] -> [34],[38],[3C]
            if (!(sum == F(kZero_0073d700))) {                            // 004A8B55..004A8B60
                const double inverse = F(kOne_0073d764) / sum;            // 004A8B62 fdivr
                w0 = S(W(a) * inverse);                                   // 004A8B68..004A8B6E
                w1 = S(W(b) * inverse);                                   // 004A8B72..004A8B78
                w2 = S(inverse * W(c));                                   // 004A8B7C..004A8B80
            } else {
                w0 = a; w1 = b; w2 = c;                                   // 004A8B86..004A8B9C (raw DWORD copies)
            }
            if ((flags_of(bin) & 8u) != 0) {                              // 004A8BB0..004A8BC1
                const U model = address_of(aurora_tree_as_model_0043e1c0(
                    reinterpret_cast<aurora_model::MaxTree*>(byte_ptr(word_at(trimesh + 8))), nullptr)); // 004A8BC7..004A8BCE
                const U base = address_of(aurora_pool_lock_00423f10(word_at(model + 0xAC), 0, 0, 8)); // 004A8BD3..004A8BE0
                const U stride = word_at(trimesh + 0x14C);
                auto texcoord = [&](U index_offset) {                     // movsx word [face+..] * [node+14C] + [node+164] + [node+194]
                    const U index = static_cast<U>(static_cast<I>(*reinterpret_cast<const std::int16_t*>(byte_ptr(face + index_offset))));
                    return base + index * stride + word_at(trimesh + 0x164) + word_at(trimesh + 0x194);
                };
                const U v0 = texcoord(0x1A), v1 = texcoord(0x1C), v2 = texcoord(0x1E); // 004A8BEF..004A8C4C
                const double s0 = W(float_at(v0)), t0 = W(float_at(v0 + 4));
                const double s1 = W(float_at(v1)), t1 = W(float_at(v1 + 4));
                const double s2 = W(float_at(v2)), t2 = W(float_at(v2 + 4));
                const float c4 = S(W(w2) * s2);                           // 004A8C52..004A8C58
                const float c8 = S(W(w2) * t2);                           // 004A8C5F..004A8C65
                const float cc = S(W(w2) * F(kZero_0073d700));            // 004A8C70..004A8C7A
                const float d0 = S(W(w1) * s1);                           // 004A8C81..004A8C87
                const float d4 = S(W(w1) * t1);                           // 004A8C8E..004A8C94
                const float d8 = S(W(w1) * F(kZero_0073d700));            // 004A8C9F..004A8CA9
                const float x124 = S(W(w0) * s0);                         // 004A8CB0..004A8CB6
                const double w0t0 = W(w0) * t0;                           // 004A8CBD..004A8CC3 (kept on the stack)
                const double w0z = W(w0) * F(kZero_0073d700);             // 004A8CC7..004A8CCB
                const float e8 = S(W(x124) + W(d0));                      // 004A8CD1..004A8CDF
                const float ec = S(w0t0 + W(d4));                         // 004A8CE6..004A8CEF
                const double zsum = w0z + W(d8);                          // 004A8CF6
                lightmap[0] = S(W(e8) + W(c4));                           // 004A8CFD..004A8D20 -> [60]
                lightmap[1] = S(W(ec) + W(c8));                           // 004A8D19..004A8D44 -> [64]
                lightmap[2] = S(zsum + W(cc));                            // 004A8D3D..004A8D59 -> [68]
                const U model2 = address_of(aurora_tree_as_model_0043e1c0(
                    reinterpret_cast<aurora_model::MaxTree*>(byte_ptr(word_at(trimesh + 8))), nullptr)); // 004A8D48..004A8D5D
                aurora_pool_unlock_00423f30(word_at(model2 + 0xAC));      // 004A8D62..004A8FB9
            } else if ((flags_of(bin) & 0x10u) != 0) {                    // 004A8D6E..004A8D70
                const U base = address_of(aurora_pool_lock_00423f10(word_at(part + 0x74), 0, 0, 8)); // 004A8D76..004A8D84
                auto colour = [&](U index_offset) {
                    return base + static_cast<U>(static_cast<I>(*reinterpret_cast<const std::int16_t*>(byte_ptr(face + index_offset)))) * 4u;
                };
                const U p2 = colour(0x1E), p1 = colour(0x1C), p0 = colour(0x1A); // 004A8D89..004A8D9F
                auto byte_of = [](U p, U k) { return static_cast<U>(*byte_ptr(p + k)); };
                const double r = ((fild_unsigned(word_at(p0) & 0xFFu) * W(w0)) + (fild_unsigned(word_at(p2) & 0xFFu) * W(w2)))
                                 + (fild_unsigned(word_at(p1) & 0xFFu) * W(w1)); // 004A8DA2..004A8E15
                const float g = S(((fild_unsigned(byte_of(p1, 1)) * W(w1)) + (fild_unsigned(byte_of(p2, 1)) * W(w2)))
                                  + (fild_unsigned(byte_of(p0, 1)) * W(w0))); // 004A8E05..004A8E53 -> [40]
                const float bl = S(((fild_unsigned(byte_of(p1, 2)) * W(w1)) + (fild_unsigned(byte_of(p2, 2)) * W(w2)))
                                   + (fild_unsigned(byte_of(p0, 2)) * W(w0))); // 004A8E2F..004A8E8F -> [10]
                const float al = S(((fild_unsigned(byte_of(p1, 3)) * W(w1)) + (fild_unsigned(byte_of(p2, 3)) * W(w2)))
                                   + (fild_unsigned(byte_of(p0, 3)) * W(w0))); // 004A8E6F..004A8EB7 -> [1C]
                auto clamp = [](double v) {                               // fld 255 / fcomp (test ah,5 jnp); fld 0 / fcomp (test ah,41h jne)
                    if (F(k255_0073ede4) < v) return F(k255_0073ede4);
                    if (!(F(kZero_0073d700) > v)) return v;
                    return F(kZero_0073d700);
                };
                red = ftol2_low_byte(clamp(r));                           // 004A8EBB..004A8EFA
                green = ftol2_low_byte(clamp(W(g)));                      // 004A8EF0..004A8F39
                blue = ftol2_low_byte(clamp(W(bl)));                      // 004A8F2F..004A8F78
                alpha_byte = ftol2_low_byte(clamp(W(al)));                // 004A8F6E..004A8FAD
                aurora_pool_unlock_00423f30(word_at(part + 0x74));        // 004A8FB1..004A8FB9
            }
            // triangle point (004A8FC1..004A9087; stack order of the original kept)
            const double w2x = W(w2) * W(grass_corner2_00831ec0.x), w2y = W(w2) * W(grass_corner2_00831ec0.y);
            const float x174 = S(W(w2) * W(grass_corner2_00831ec0.z));
            const double w1x = W(w1) * W(grass_corner1_00831ecc.x), w1y = W(w1) * W(grass_corner1_00831ecc.y);
            const float x168 = S(W(w1) * W(grass_corner1_00831ecc.z));
            const double w0x = W(w0) * W(grass_corner0_00831ed8.x);
            const float x11c = S(W(w0) * W(grass_corner0_00831ed8.y));
            const float x120 = S(W(w0) * W(grass_corner0_00831ed8.z));
            const float x10c = S(w0x + w1x);                              // 004A9037..004A9039
            const double y01 = W(x11c) + w1y;                             // 004A9040..004A9047
            const float x114 = S(W(x120) + W(x168));                      // 004A904D..004A905B
            const float px = S(W(x10c) + w2x);                            // 004A9062..004A906B -> [50]
            const float py = S(y01 + w2y);                                // 004A906F..004A9071 -> [54]
            const float pz = S(W(x114) + W(x174));                        // 004A9079..004A9087 -> [58]
            const float angle = S((static_cast<double>(rand_10000()) * F(kTenThousandth_00741870)) * F(kTwoPi_00740c88)); // 004A908B..004A90BF
            Quaternion built;
            const Quaternion* q = quaternion_from_axis_angle_004ac960(&built, nullptr, axis, angle); // 004A90A6..004A90D9
            std::memcpy(&rotation, q, 16);                                // 004A90DE..004A90F5
            double pick = static_cast<double>(rand_10000()) * F(kTenThousandth_00741870); // 004A90F9..004A911F
            const U weights = grass_record(field<U>(bin, 0x10)) + 0x40;   // 004A9106..004A9129
            I cell = 0;                                                   // [esp+10h] = 0 (004A9125)
            for (I j = 0; j < 4; ++j) {                                   // 004A9130..004A9142
                if (pick < W(float_at(weights + static_cast<U>(j) * 4u))) { cell = j; break; } // test ah,5 / jnp 004A914A
                pick -= W(float_at(weights + static_cast<U>(j) * 4u));    // 004A9139
            }
            const float u0 = ((cell % 2) == 1) ? bits_to_float(kHalf_0073e9ac) : 0.0f;       // 004A914C..004A9170 -> [5C]
            const float v0 = (((cell / 2) % 2) == 1) ? bits_to_float(kHalf_0073e9ac) : 0.0f; // 004A9172..004A9190 -> [44]
            const float scale = S(((static_cast<double>(rand_10000()) * F(kTenThousandth_00741870)) - F(kHalf_0073e9ac))
                                  * F(kPointFour_0073e320) + F(kOne_0073d764)); // 004A9198..004A91DA -> [1C]
            const U nx = word_at(face), ny = word_at(face + 4), nz = word_at(face + 8); // [ebp], [ebp+4], [ebp+8] (re-read per vertex; face is not written)
            auto dword = [&](U index) { return vertices + index * 4u; };
            auto extras = [&]() {                                         // 004A925D..004A92AC (same shape at each vertex)
                const U8 flags = flags_of(bin);
                if ((flags & 8u) != 0) {
                    ++attribute; put_float(dword(attribute), lightmap[0]); // [60]
                    ++attribute; put_float(dword(attribute), lightmap[1]); // [64]
                } else if ((flags & 0x10u) != 0) {
                    const U packed = (((((static_cast<U>(alpha_byte) << 8) | blue) << 8) | green) << 8) | red;
                    ++attribute; put_float(dword(attribute), S(fild_unsigned(packed)));
                }
            };
            auto position = [&](double x, double y, float z) {            // [bin+3C] + vertex*12 (re-read per store)
                ++vertex;
                const U offset = static_cast<U>(vertex) * 12u;
                put_float(field<U>(bin, 0x3C) + offset, S(x));
                put_float(field<U>(bin, 0x3C) + offset + 4, S(y));
                put_float(field<U>(bin, 0x3C) + offset + 8, z);
            };
            // vertex 1: corner B8, no scale (004A91A5..004A92AC)
            Vector out1;
            const Vector* r1 = quaternion_rotate_004a9bb0(&out1, &rotation, &corner_b8);
            position(W(px) + W(r1->x), W(py) + W(r1->y), S(W(pz) + W(r1->z)));
            put_word(dword(attribute + 1), nx); put_word(dword(attribute + 2), ny); put_word(dword(attribute + 3), nz);
            put_float(dword(attribute + 4), u0); put_float(dword(attribute + 5), v0);
            attribute += 5;
            extras();
            // vertex 2: corner A0, no scale (004A92AF..004A939C)
            Vector out2;
            const Vector* r2 = quaternion_rotate_004a9bb0(&out2, &rotation, &corner_a0);
            position(W(px) + W(r2->x), W(py) + W(r2->y), S(W(pz) + W(r2->z)));
            const float u1 = S(W(u0) + F(kHalf_0073e9ac));                // 004A9320..004A9338 -> [10]
            put_word(dword(attribute + 1), nx); put_word(dword(attribute + 2), ny); put_word(dword(attribute + 3), nz);
            put_float(dword(attribute + 4), u1); put_float(dword(attribute + 5), v0);
            attribute += 5;
            extras();
            // vertex 3: corner AC, scaled (004A939F..004A94B6)
            Vector out3;
            const Vector* r3 = quaternion_rotate_004a9bb0(&out3, &rotation, &corner_ac);
            {
                const double sx = W(scale) * W(r3->x);                    // 004A93B9..004A93BD (kept)
                const float sy = S(W(scale) * W(r3->y));                  // 004A93C3..004A93D5 -> [F8]
                const float sz = S(W(scale) * W(r3->z));                  // 004A93DC..004A93EC -> [FC]
                position(sx + W(px), W(sy) + W(py), S(W(sz) + W(pz)));    // 004A93F4..004A9435
            }
            const float v1 = S(W(v0) + F(kHalf_0073e9ac));                // 004A9456..004A9460 -> [10]
            put_word(dword(attribute + 1), nx); put_word(dword(attribute + 2), ny); put_word(dword(attribute + 3), nz);
            put_float(dword(attribute + 4), u1); put_float(dword(attribute + 5), v1);
            attribute += 5;
            extras();
            // vertex 4: corner 94, scaled (004A94B9..004A95C8)
            Vector out4;
            const Vector* r4 = quaternion_rotate_004a9bb0(&out4, &rotation, &corner_94);
            {
                const double sx = W(scale) * W(r4->x);                    // 004A94D3..004A94D7
                const float sy = S(W(scale) * W(r4->y));                  // 004A94DD..004A94EA -> [104]
                const float sz = S(W(scale) * W(r4->z));                  // 004A94F1..004A9504 -> [108]
                position(sx + W(px), W(sy) + W(py), S(W(sz) + W(pz)));    // 004A950C..004A954F
            }
            put_word(dword(attribute + 1), nx); put_word(dword(attribute + 2), ny); put_word(dword(attribute + 3), nz);
            put_float(dword(attribute + 4), u0); put_float(dword(attribute + 5), v1);
            attribute += 5;
            extras();
        }
    }
    field<U>(bin, 0x3C) = 0;                                              // 004A9611
    field<U8>(bin, 2) = static_cast<U8>(flags_of(bin) | 4u);              // 004A9618..004A9625
}

// 004A9630 CAurTriangleBin::BuildGrassPolys, thiscall RET 0 (tail jmp 004A87C0).
void __fastcall triangle_bin_build_grass_polys_004a9630(void* bin, void*) {
    const U8 flags = flags_of(bin);                                       // 004A9630
    if ((flags & 4u) != 0) return;                                        // 004A9633..004A9635
    if (field<I>(bin, 0x0C) == 0) return;                                 // 004A9637..004A963C
    const bool lightmapped = (flags & 8u) != 0;                           // 004A9643..004A9647
    const bool coloured = !lightmapped && (flags & 0x10u) != 0;           // 004A964B..004A964F
    field<U16>(bin, 0) = 0x20;                                            // 004A9653
    if (lightmapped) field<U16>(bin, 0) = 0x28;                           // 004A965A
    else if (coloured) field<U16>(bin, 0) = 0x24;                         // 004A9669
    triangle_bin_create_arrays_004a87c0(bin, nullptr);                    // 004A9660 / 004A966F jmp
}

} // extern "C"
