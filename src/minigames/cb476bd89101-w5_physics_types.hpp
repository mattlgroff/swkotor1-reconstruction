#pragma once
// Exact type-only extraction from selected w5_t2.hpp, SHA256:
// 5E646FBCB15FB43E3A8C5F1CFF7D08BFB625CA4EC862BF315BC81EE3EDA5302B.
// Full header also has obsolete incompatible add/scale declarations; deliberately excluded.
namespace w5 { struct Vector { float x,y,z; }; struct PhysicsState { Vector a,b; }; }
extern "C" w5::PhysicsState* __cdecl w5_physics_state_divide_006708c0(w5::PhysicsState*,const w5::PhysicsState*,float);
