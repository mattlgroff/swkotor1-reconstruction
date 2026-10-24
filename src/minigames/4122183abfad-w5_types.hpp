#pragma once
// W5 control + interaction lane: shared native conventions (KotOR 1 GOG swkotor.exe, image base 00400000).
//  * original __thiscall entries are exported as extern "C" __fastcall(Self*, void* unused_edx, ...);
//    callers pass nullptr for EDX and callees never read it (same convention as world-los D1).
//  * x87 arithmetic: wide intermediates are double, float only where the original stores a dword
//    (fstp dword). ST0 float returns are exported as double so callers see the unrounded value.
//  * memory: only extern "C" live_operator_new / live_free (originals 006FA7E6 / 006FA390).
//  * unimplemented callees trap through kotor_unreachable_trap(address, reason) and never return.
#include <cstdint>
#include <cstring>

namespace w5 {
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;

struct Vector { float x, y, z; };
static_assert(sizeof(Vector) == 12, "Vector is three packed floats");

// CSWRoomSurfaceMeshHitInfo (00581320 ctor): +0 material mask, +4 child-order bits, +8 hit flag,
// +C face (-1 = none), +10 hit point, +1C material bit of the hit face.
struct SurfaceHit { U mask_0; U order_4; U hit_8; I face_c; Vector point_10; U bit_1c; };
static_assert(sizeof(SurfaceHit) == 0x20, "SurfaceHit is 0x20 bytes");

template<class T> inline T& at(void* object, U offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
inline U bits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
inline float flt(U u) { float f; std::memcpy(&f, &u, 4); return f; }
inline void copy_vector(void* dst, const void* src) { std::memcpy(dst, src, 12); }  // three DWORD moves

// CSWRoomSurfaceMesh field offsets used by W5 (load side: room_mesh_binary_005807c0 / 00597120).
namespace mesh {
constexpr U kind_04 = 0x04;          // BWM type: nonzero = world-space (WorldToLocal is an identity copy)
constexpr U vertex_count_50 = 0x50;  // signed compare in GetVertex
constexpr U vertices_54 = 0x54;      // float[3] per vertex
constexpr U face_count_58 = 0x58;
constexpr U faces_60 = 0x60;         // U[3] per face
constexpr U materials_64 = 0x64;
constexpr U normals_68 = 0x68;       // float[3] per face
constexpr U distances_6c = 0x6c;     // float per face (plane d)
constexpr U adjacency_88 = 0x88;     // I[3] per walkable face (edge index = face*3+k or -1)
constexpr U aabb_count_b0 = 0xb0;
constexpr U aabb_root_d4 = 0xd4;
constexpr U walk_mask_e0 = 0xe0;
constexpr U all_mask_e4 = 0xe4;
constexpr U resref_0c = 0x0c;
}
}  // namespace w5

extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
void* __cdecl live_operator_new(std::uint32_t size);   // original 006FA7E6
void __cdecl live_free(void* block);                   // original 006FA390
}
