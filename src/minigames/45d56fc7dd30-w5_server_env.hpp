#pragma once
// W5 bindings to EXISTING natives (all present in the runtime1115 map; owner object in brackets) and
// small inline views of trivial accessors that the original calls but that have no address-named
// provider (marked "inline view of <addr>": identical observable behaviour, no address credit).
#include "4122183abfad-w5_types.hpp"

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // [0053]
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); } // [M41]

extern "C" {
using w5::U; using w5::I; using w5::Vector;
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U id);              // CServerExoApp::GetGameObject [0530]
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);              // GetCreatureByGameObjectID [0595]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);              // GetClientObjectByObjectId [0595]
void* __fastcall kotor_fastcall_004b2a50(void* internal, void*, U id);            // CServerExoAppInternal::GetClientObjectByObjectId [0595]
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, U id, void* out); // CGameObjectArray::GetGameObject, AL status [0457]
unsigned __fastcall kotor_fastcall_005eda50(void* client, void*, U id);           // ServerToClientObjectId [0453]
unsigned __fastcall frame_R4_005ed5b0(void* client, void*, U id);                 // CClientExoApp::GetCreatureByGameObjectID [0457]
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                    // CClientExoApp::GetSWParty [0707]
void* __fastcall recovered_object_area_004cb120(void* object, void*);             // CSWSObject::GetArea [1060]
unsigned __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*); // CSWSCreature::GetDeadTemp [0963]
void __fastcall recovered_object_position_004cd1a0(void* object, void*, const Vector* p, U a, U b, U c); // SetPosition [0886]
void* __fastcall recovered_event_construct_004d7540(void* event, void*);          // CScriptEvent::CScriptEvent [1017]
void __fastcall recovered_event_object_004d7780(void* event, void*, U index, U id); // CSWCCMessageData::SetObjectID [1017]
void __fastcall recovered_event_destroy_004d7590(void* event, void*);             // CScriptEvent::~CScriptEvent [0886]
unsigned __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U caller, U target, U type, void* event); // AddEventDeltaTime [1017]
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations* list, void*, U value); // CExoArrayList::Add [0293]
void* __fastcall kotor_fastcall_0051b720(void* self, void*, void* other);         // CExoArrayList<ulong>::operator= [0778]
unsigned __fastcall recovered_creature_collision_flag_004f62a0(void* self, void*, void* other); // GetIsCreatureBumpable [1080]
extern float recovered_trigger_primary_range_007a206c;                            // .data 007A206C [0963]
// world-los / 3d (see w5_walkmesh.hpp for the vector set)
I __cdecl box_intersect_004aa610(const Vector* a, const Vector* b, const Vector* box_min, const Vector* box_max); // [0529]
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void*, const Vector* other);
Vector* __fastcall vector_subtract_assign_004a9770(Vector* self, void*, const Vector* other); // [0542]
Vector* __fastcall vector_scale_assign_004a97a0(Vector* self, void*, float scale);
void __fastcall vector_normalize_004ab130(Vector* self, void*);                   // [0483]
Vector* __cdecl kotor_cdecl_00423bc0(Vector* out, const Vector* a, const Vector* b);   // Vector operator- [0457]
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float s); // [0528]
Vector* __cdecl vector_sum_00423b90(Vector* out, const Vector* a, const Vector* b);    // [0464]
void* __cdecl yaw_pitch_roll_004acac0(void* out_quaternion, float yaw, float pitch, float roll); // [0542]
Vector* __cdecl quaternion_rotate_004a9bb0(Vector* out, const void* q, const Vector* v);       // [0464]
}

namespace w5 {
inline void* manager() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* server_app() { return at<void*>(manager(), 8); }                    // [[7A39FC]+8]
inline void* client_app() { return at<void*>(manager(), 4); }                    // [[7A39FC]+4]
inline void* ai_master() { return at<void*>(at<void*>(server_app(), 4), 0x10044); }    // inline view of 004AED80
inline void* object_array() { return at<void*>(at<void*>(server_app(), 4), 0x1005c); } // inline view of 004AED70
constexpr U k_object_invalid = 0x7f000000u;
constexpr U k_event_queue_type_007441dc = 10;                                     // .rdata 007441DC (dword 0Ah)
// virtual call through the object's vtable at byte offset `slot` (thiscall; EDX unused)
template<class R, class... A> inline R vcall(void* self, U slot, A... args) {
    using F = R(__fastcall*)(void*, void*, A...);
    return reinterpret_cast<F>(at<void*>(*static_cast<void**>(self), slot))(self, nullptr, args...);
}
inline void* game_object(U id) { return kotor_fastcall_004ae750(server_app(), nullptr, id); }
// queue a CScriptEvent of `type` from `caller` to `target` with object id 0 = `object` (00516xxx/0051CFxx shape)
inline void queue_script_event(U16 type, U caller, U target, U object_id) {
    void* memory = live_operator_new(0x34);
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;
    recovered_event_object_004d7780(event, nullptr, 0, object_id);
    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, caller, target, k_event_queue_type_007441dc, event);
}
}  // namespace w5
