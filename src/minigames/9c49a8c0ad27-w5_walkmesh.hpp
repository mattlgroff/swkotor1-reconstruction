#pragma once
// W5 walkmesh queries (G16). Native x86 exports; see w5_types.hpp for conventions.
#include "4122183abfad-w5_types.hpp"

extern "C" {
using w5::U; using w5::I; using w5::Vector; using w5::SurfaceHit;

// ---- authored in W5 (w5_walkmesh.cpp) -------------------------------------------------------
// 004A9810 PlaneHeightAtPoint(normal*, d, point*): cdecl, ST0. 0.0 when normal.z == 0.
double __cdecl w5_plane_height_at_point_004a9810(const Vector* normal, float distance, const Vector* point);
// 00580340 QueryTimer(): cdecl, EDX:EAX microseconds (QueryPerformanceCounter*1e6 / frequency).
std::int64_t __cdecl w5_query_timer_00580340();
// 005805E0 CSWRoomSurfaceMesh::ComputeHeight(face, point): thiscall RET 10h, ST0.
double __fastcall w5_surface_mesh_face_height_005805e0(void* mesh, void*, I face, Vector point);
// 00581610 CSWRoomSurfaceMesh::CheckAABBAll(from, to, hit*): thiscall RET 1Ch, EAX = hit->hit_8.
U __fastcall w5_surface_mesh_check_aabb_all_00581610(void* mesh, void*, Vector from, Vector to, SurfaceHit* hit);
// 00581720 CSWRoomSurfaceMesh::FindTriangle(point*, walkableOnly, nudgeDir): thiscall RET 14h, EAX face or -1.
I __fastcall w5_surface_mesh_find_triangle_00581720(void* mesh, void*, const Vector* point, I walkable, Vector dir);
// 00581B30 CSWRoomSurfaceMesh::ComputeHeight(point, walkableOnly): thiscall RET 10h, ST0 (-9.0 when no face).
double __fastcall w5_surface_mesh_compute_height_00581b30(void* mesh, void*, Vector point, I walkable);
// 00579430 CSWSRoom::ComputeHeight(point, walkableOnly): thiscall RET 10h, ST0 (0.0 without a mesh).
double __fastcall w5_room_compute_height_00579430(void* room, void*, Vector point, I walkable);
// 005970E0 CSWCollisionMesh::GetVertex(index, Vector* out): thiscall RET 8 (out untouched when invalid).
void __fastcall w5_collision_mesh_get_vertex_005970e0(void* mesh, void*, I index, Vector* out);
// 00597090 CSWCollisionMesh::GetVertex(index, float* x, float* y, float* z): thiscall RET 10h.
void __fastcall w5_collision_mesh_get_vertex_xyz_00597090(void* mesh, void*, I index, float* x, float* y, float* z);
// 00580620 CSWRoomSurfaceMesh::GetTriangleAdjacency(unused, face, I* a, I* b, I* c): thiscall RET 14h.
void __fastcall w5_surface_mesh_triangle_adjacency_00580620(void* mesh, void*, I unused, I face, I* a, I* b, I* c);
// 00584220 CSWRoomSurfaceMesh::ClippedLineSegmentWalkable(from*, to*, clipFrom*, clipTo*, zMin*, zMax*, edge*):
// thiscall RET 1Ch, EAX 0/1.
U __fastcall w5_surface_mesh_clipped_segment_walkable_00584220(void* mesh, void*, const Vector* from,
    const Vector* to, Vector* clip_from, Vector* clip_to, float* z_min, float* z_max, U* edge);
// 00579670 CSWSRoom::ClippedLineSegmentWalkable: ecx = room->mesh (+3C, no null test), tail jump to 00584220.
U __fastcall w5_room_clipped_segment_walkable_00579670(void* room, void*, const Vector* from, const Vector* to,
    Vector* clip_from, Vector* clip_to, float* z_min, float* z_max, U* edge);

// W5-owned storage (no other owner in the 1115 map):
extern float w5_aabb_all_elapsed_0083283c;        // CheckAABBAll last elapsed microseconds (fild/fstp)
extern std::int64_t w5_query_timer_frequency_00832848; // QueryTimer cached QueryPerformanceFrequency
extern U w5_query_timer_guard_00832850;          // QueryTimer local-static guard bit 0

// ---- reused existing providers (bound, not authored here) -----------------------------------
// world-los (0526..0529 in 1115)
I __fastcall surface_mesh_check_aabb_node_00580920(void* mesh, void*, I node, Vector* from, Vector* to, SurfaceHit* hit);
Vector* __fastcall collision_mesh_world_to_local_00596a00(void* mesh, void*, Vector* out, const Vector* world);
Vector* __fastcall collision_mesh_local_to_world_00596aa0(void* mesh, void*, Vector* out, const Vector* local);
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
I __cdecl polyhit_004ab9d0(const Vector* points, I count, const Vector* start, const Vector* end, Vector* hit, Vector* normal);
I __cdecl intersect_line_segments_2d_004ac340(const Vector* p1, const Vector* p2, const Vector* p3, const Vector* p4, Vector* out);
// 0483 / 0548
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void*, const Vector* other);
Vector* __fastcall vector_scale_assign_004a97a0(Vector* self, void*, float scale);
// W5 native adapters over existing host-Memory bodies (w5_walkmesh_adapters.cpp; no address credit):
U __fastcall w5_adapt_check_aabb_walkable_00581530(void* mesh, void*, Vector from, Vector to, SurfaceHit* hit);
U __fastcall w5_adapt_compute_clipped_line_segment_00583e30(void* mesh, void*, Vector from, Vector to,
    Vector* clip_from, Vector* clip_to, U* edge);
}
