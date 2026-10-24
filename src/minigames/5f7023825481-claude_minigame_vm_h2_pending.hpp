#pragma once
// Callable contracts for h2 callees without a shared header (each checked against the original RET / call sites).
extern "C" {
// t1/tiers/c1 export @kotor_fastcall_0066c130@8 (GetLastBulletHitShooter; original RET 0, pointer in EAX).
void* __fastcall kotor_fastcall_0066c130(void*, void*);
// t1 F14 A2 (ADMISSIONS R4), not yet delivered: 0066BFB0 CSWMiniGameObjectArray::GetMiniGameObjectByName,
// original thiscall RET 4 (0066C037/0066C053), argument = CExoString* (005CB6C4 lea edx,[esp+4]; push edx).
void* __fastcall kotor_fastcall_0066bfb0(void* array, void*, const void* name);
// t1 F14 A2, not yet delivered: 0066CA90 CSWTrackFollower::SetSoundVolume, original RET 8 (0066CABF),
// pushes at 005CD1E1..005CD1EA: (sound, value) like 0066CA10/0066CA50.
void __fastcall kotor_fastcall_0066ca90(void* self, void*, int sound, int volume);
// CExoString / CResRef helpers selected in c63 (C linkage).
void* __fastcall GuiAssign005e5140(void* s, void*, const char* text);                 // CExoString::operator=(const char*) [0093]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);       // CExoString(const char*) [0459]
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out_string);       // CResRef -> CExoString& [0204]
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const void* s); // CResRef(const CExoString&) [0200]
}
