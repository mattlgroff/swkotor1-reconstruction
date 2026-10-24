#pragma once
#include <cstdint>
#include "d8f703239b61-resources.h"
// t1 F14 lane A2 (tier f1b): exact x86 callable contracts from the original returns/pushes.
extern "C" {
// 0066BFB0 CSWMiniGameObjectArray::GetMiniGameObjectByName(const CExoString&) RET4.
void* __fastcall kotor_fastcall_0066bfb0(void* self, void*, kotor_reconstruction::StartupString* name) noexcept(false);
// 0066C6A0 SetInitialHook callback: cdecl(Gob*, const char* unused event, follower) RET0.
void __cdecl kotor_cdecl_0066c6a0(void* gob, const char* event, void* follower) noexcept(false);
// 0066CA90 CSWTrackFollower::SetSoundVolume(int index, int volume) RET8.
void __fastcall kotor_fastcall_0066ca90(void* self, void*, std::int32_t index, std::int32_t volume) noexcept(false);
// 0066CAD0 CSWTrackFollower::GetName / 0066DC80 CSWMGObstacle::GetName: hidden out, RET4, EAX=out.
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_0066cad0(void*, void*, kotor_reconstruction::StartupString*) noexcept(false);
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_0066dc80(void*, void*, kotor_reconstruction::StartupString*) noexcept(false);
// 0066D280 CSWTrackFollower::RemoveModel(Gob*) RET4, EAX=removed model or 0.
void* __fastcall kotor_fastcall_0066d280(void*, void*, void*) noexcept(false);
// 0066E280 CSWTrackFollower::ForceSetTrack(Gob* track, attachment) RET8.
void* __fastcall kotor_fastcall_0066e280(void* self, void*, void* track, std::uint32_t attachment) noexcept(false);
// 0066E900 CSWTrackFollower::SetSoundName(int index, const CResRef*) RET8.
void* __fastcall kotor_fastcall_0066e900(void* self, void*, std::int32_t index, const menu_resources::Ref* ref) noexcept(false);
// 006706C0 CSWMGObstacle::PlayAnimation(const char*, float, flags) RET12.
void __fastcall kotor_fastcall_006706c0(void*, void*, const char*, float, std::uint32_t) noexcept(false);
}
