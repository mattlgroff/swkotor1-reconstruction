#pragma once
// t1-A1 tier f1a (ROOT ADMISSIONS-20261022 R4): CSWTrackFollower model/track/update/hit bodies.
// Original thiscall entries as extern "C" __fastcall(self, unused EDX, stack words...), callee cleanup unchanged.
// E130/D550/E1F0 are declared identically by the shared t1 contract header t1/tiers/d3/claude_turret_t1_lifecycle.hpp.
#include <cstdint>
extern "C" {
// 0066E130 CSWTrackFollower::Update(float delta) RET 4.
void __fastcall kotor_fastcall_0066e130(void*,void*,float) noexcept(false);
// 0066D550 CSWTrackFollower::PlayAnimation(const char* name, float speed, unsigned flags) RET 0Ch.
void __fastcall kotor_fastcall_0066d550(void*,void*,const char*,float,std::uint32_t) noexcept(false);
// 0066E1F0 CSWTrackFollower::AddModel(Gob* model, unsigned attach) RET 8; EAX = model (0 for null model).
void* __fastcall kotor_fastcall_0066e1f0(void*,void*,void*,std::uint32_t) noexcept(false);
// 0066D2F0 CSWTrackFollower::SetTrack(Gob* track, unsigned attachment) RET 8; distinct true entry,
// also reached by the 0066E280 tail jump. EAX = fresh follower+64.
void* __fastcall kotor_fastcall_0066d2f0(void*,void*,void*,std::uint32_t) noexcept(false);
// 0066E6B0 CSWTrackFollower::DoObstacleHitCheck(CSWMiniGameObject* obstacle) RET 4; EAX 1 hit / 0.
std::int32_t __fastcall kotor_fastcall_0066e6b0(void*,void*,void*) noexcept(false);
// Import, owned by t1-A2: 0066C6A0 SetInitialHook, cdecl (Gob* model, const char* event, void* follower), RET 0.
// Only its address is used here (Gob +28 RegisterCallback argument at 0066D39D).
void __cdecl kotor_cdecl_0066c6a0(void*,const char*,void*) noexcept(false);
}
