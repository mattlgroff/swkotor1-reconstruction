#pragma once
// Dependencies of W5 bodies that belong to OTHER lanes and have no provider in the runtime1115 map yet.
// Each is imported under a W5 import name. If the owner's export is not linked, the linker falls back
// (via /alternatename, emitted in w5_foreign_traps.cpp) to a W5 explicit trap that names the original
// address. The integrator binds an import by defining the import name as a forwarder to the owner's export.
#include "4122183abfad-w5_types.hpp"
extern "C" {
using w5::U; using w5::I;
// W2 exports (coordinator ruling; lane-w2/tiers/t2/RECEIPT.json), exact W2 signatures. Trap fallbacks via
// /alternatename in w5_foreign_traps.cpp when W2's objects are not linked.
// CSWMessage::ReadFLOAT 004D67B0 (scale as IEEE bits, bits) RET 8; ReadWORD 004D6750 / ReadBYTE 004D6720 (bits) RET 4
float __fastcall kotor_fastcall_004d67b0(void* message, void*, unsigned scale_bits, unsigned bits);
unsigned short __fastcall kotor_fastcall_004d6750(void* message, void*, unsigned bits);
unsigned char __fastcall kotor_fastcall_004d6720(void* message, void*, unsigned bits);
// CSWSMessage::ReadOBJECTIDServer 00578540 (RET 0; DWORD & 7FFFFFFF, 0 on overrun): W2 message-reader family,
// not yet exported by W2 -> requested under W2's naming convention, trap fallback in w5_foreign_traps.cpp.
unsigned __fastcall kotor_fastcall_00578540(void* message, void*);
// CSWSPlayer::GetLastUpdateObject 00561820 (RET 4) and CSWSCreature::SetActivity 004F00A0 (RET 0Ch): W2-owned
void* __fastcall recovered_last_update_object_00561820(void* player, void*, unsigned id);
void __fastcall recovered_set_activity_004f00a0(void* creature, void*, unsigned mask, unsigned on, unsigned extra);
// W1 effects: CGameEffect::~CGameEffect 00503590 (thiscall RET 0; trap in 1115)
void __fastcall w5_import_game_effect_destroy_00503590(void* effect, void*);
// VM lane: CVirtualMachine::DeleteScriptSituation 005D1110 (thiscall RET 4)
void __fastcall w5_import_vm_delete_script_situation_005d1110(void* vm, void*, void* situation);
// W1 exports (lane-w1 tier3, linked in 1121 as 1201a-w1t3_w1_native.obj): called directly (coordinator ruling)
unsigned __fastcall recovered_party_index_006349c0(void* party, void*, unsigned id);               // CSWParty::GetIndex
void __fastcall recovered_creature_compute_ai_state_004f1460(void* creature, void*);               // ComputeAIState
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, unsigned id, void* data); // SendFeedbackMessage
void __fastcall recovered_action_node_destroy_004cc4d0(void* node, void*);                        // ~CSWSObjectActionNode
// W1 integration wrappers in the 0886 TU (w1_0886_exports.inc, linked in 1121):
unsigned __fastcall w1x_area_height_bits_004bc380(void* area, void*, void* point, unsigned flag, unsigned* room); // float bits
unsigned __fastcall w1x_area_safe_location_004be860(void* area, void*, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e);
}
