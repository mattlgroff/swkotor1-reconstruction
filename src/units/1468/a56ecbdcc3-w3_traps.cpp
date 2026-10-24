// lane-w3 explicit D83 traps: original functions that the W3 bodies call, that runtime1115 does not provide and that
// are not reconstructed in this lane delivery. Each trap carries the original address and symbol and never returns.
// When the owning lane (noted per entry) delivers the body under the same symbol, the integrator deletes the entry.
#include "fa114a50b4-w3_client_area.h"

#define W3_TRAP(addr, name) kotor_unreachable_trap(addr, name)

// ---- stale-stubs G5 (Claude): providers used by the 005ED430 UnloadModule and 0060A2E0 ~CSWCArea bodies below.
namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }          // [007A39E4] CExoInput*
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void __fastcall destroy_list_005e9ab0(kotor_list::Internal* list, void*); }   // 005E9AB0 ~CExoLinkedList
namespace menu_tables_required { void __fastcall SoundSetMode_005d5e80(void* sound, void*, unsigned mode); }    // 005D5E80 RET 4
namespace kotor_menu_10_19 { struct ObjectArray { ObjectArray* __thiscall construct_004d7d80(int); }; }        // 004D7D80 RET 4
extern "C" {
extern void* kotor_cell_sound_007a39ec;                                                  // [007A39EC] CExoSound*
extern const void* const recovered_client_area_methods_0074eef0[9];                      // 0074EEF0 CSWCArea vtable
extern const void* const recovered_client_area_object_methods_0074ee80[28];              // 0074EE80 CGameObject sub-object vtable
extern const void* const kotor_vtable_cgameobject_007457a0[];                            // 007457A0 CGameObject vtable
extern const void* const recovered_client_ambient_sound_methods_007541c8[9];             // 007541C8 CSWCAmbientSound vtable
void __cdecl packed_purecall_006fb8de(void);                                             // 006FB8DE _purecall
void __cdecl recovered_set_clear_color_00401690(const char* color);                      // 00401690 SetClearColor
void __fastcall kotor_fastcall_006364c0(void* party, void*, std::uint32_t id);           // 006364C0 CSWParty::AddCharacter RET 4
void __fastcall kotor_fastcall_005df580(void* input, void*);                             // 005DF580 CExoInput::UnpauseRumble
void __fastcall kotor_fastcall_005f7fa0(void* internal, void*, std::uint32_t, std::uint32_t, std::uint32_t);   // 005F7FA0 SetPauseState RET 0Ch
void __fastcall client_internal_clear_pause_exclusion_list_005f7e20(void* internal, void*);   // 005F7E20
void* __fastcall kotor_fastcall_00401060(void* manager, void*, std::uint32_t which);     // 00401060 GetObjectTableManager RET 4
void __fastcall kotor_fastcall_004d7e10(void* array, void*);                             // 004D7E10 ~CGameObjectArray
void __fastcall frame_R4_0062c310(void* gui, void*);                                     // 0062C310 CGuiInGame::ShutDown
void __fastcall w5_player_control_initialize_006792d0(void* control, void*);             // 006792D0 CSWPlayerControl::Initialize
void __fastcall kotor_fastcall_005c5500(void* entry, void*);                             // 005C5500 SJournalEntry::~SJournalEntry
void __fastcall world_journal_entry_destroy_004c4030(void* entry, void*);                // 004C4030 ~CWorldJournalEntry
void __fastcall kotor_fastcall_0058f7a0(void* area, void*);                              // 0058F7A0 CSWArea::~CSWArea
void __fastcall kotor_fastcall_005d6200(void* source, void*);                            // 005D6200 ~CExoStreamingSoundSource
// bodies defined at the end of this file (original missing callees)
std::uint32_t __fastcall kotor_fastcall_004b1d80(void* server_internal, void*, std::uint32_t mask);   // 004B1D80 RET 4
std::uint32_t __fastcall kotor_fastcall_004ae980(void* server, void*, std::uint32_t mask);            // 004AE980 GetPauseState RET 4
std::uint32_t __fastcall kotor_fastcall_004ae650(void* table, void*, std::uint32_t index);            // 004AE650 RemovePlayer RET 4
void __fastcall kotor_fastcall_005edae0(void* client, void*, std::uint32_t value);                    // 005EDAE0 RET 4
void __fastcall kotor_fastcall_00676b00(void* journal, void*);                                        // 00676B00 CSWCJournal::ClearAll
void __fastcall kotor_fastcall_00676fa0(void* journal, void*);                                        // 00676FA0 DeleteAllEntries
void __fastcall kotor_fastcall_005f8290(void* internal, void*) noexcept(false);                       // 005F8290 UnloadModule
void __fastcall kotor_fastcall_00506a70(void* server_area, void*);                                    // 00506A70 DetachFromClientArea
void __fastcall kotor_fastcall_0068d430(void* track, void*);                                          // 0068D430 ~CSWCAmbientSoundTrack
void __fastcall kotor_fastcall_0068e0d0(void* sound, void*);                                          // 0068E0D0 ~CSWCAmbientSound
void __fastcall kotor_fastcall_0060a0f0(void* area, void*) noexcept(false);                           // 0060A0F0 ~CSWCArea
}

extern "C" {
// ---- (t2) 00632860 CGuiInGame::Initialize and 00644650 CSWCModule::Update are lane-w4 bodies (kotor_fastcall_*);
// their former lane-w3 traps were removed (coordinator ruling t2-3).
// ---- Module message minors other than LoadModule (minor 1)
// Existing W2 veneers retain the original overflow/underflow-check argument and stack cleanup
// (@kotor_fastcall_004d6230@12 / @kotor_fastcall_004d6240@12 in w2_message_native.obj; both forward to the
// selected 0917 recovered_player_message provider).
unsigned __fastcall kotor_fastcall_004d6230(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004d6240(void*, void*, unsigned);
// 006528B0: actual message receiver, no stack arguments, RET 0.
std::uint32_t __fastcall recovered_client_handle_module_update_time_006528b0(void* self, void*) {
    using namespace kotor_w3;
    const U8 flags = kotor_fastcall_004d6720(self, nullptr, 8);       // 006528B7..006528C0
    // Unselected locals are never committed to the module.
    U8 state = 0;
    U milliseconds = 0, field_b0 = 0, field_ac = 0, field_a8 = 0, field_a4 = 0;
    if ((flags & 1u) != 0) {
        state = kotor_fastcall_004d6720(self, nullptr, 8);            // 006528CA..006528D3
        if (state == 3 || state == 4)
            milliseconds = kotor_fastcall_004d6780(self, nullptr, 32); // 006528E9..006528F2
    }
    if ((flags & 2u) != 0) field_b0 = kotor_fastcall_004d6720(self, nullptr, 8);
    if ((flags & 4u) != 0) field_ac = kotor_fastcall_004d6720(self, nullptr, 8);
    if ((flags & 8u) != 0) field_a8 = kotor_fastcall_004d6720(self, nullptr, 8);
    if ((flags & 0x10u) != 0) field_a4 = kotor_fastcall_004d6780(self, nullptr, 32);
    if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;     // 0065295C..00652972
    void* const module = kotor_fastcall_005ed530(client_app(), nullptr); // 00652973..00652981
    if (module != nullptr) {
        if ((flags & 1u) != 0)
            recovered_client_module_set_time_of_day_state_00640640(module, nullptr, state, milliseconds);
        if ((flags & 2u) != 0) st<U>(module, 0xB0, field_b0);
        if ((flags & 4u) != 0) st<U>(module, 0xAC, field_ac);
        if ((flags & 8u) != 0) st<U>(module, 0xA8, field_a8);
        if ((flags & 0x10u) != 0) st<U>(module, 0xA4, field_a4);
    }
    return 1;                                                     // 006529DC..006529E8, including null module
}
std::uint32_t __fastcall recovered_client_handle_export_reply_006634a0(void*, void*) { W3_TRAP(0x006634A0, "CSWCMessage::HandleServerToPlayerExportReply"); }
// ---- 005ED630 / 00678030 / 00652840 / 00652860 (module start/end handlers; this unit owns these four bodies)
// 005ED630 CClientExoApp::SetInStartNewModule(value): mov eax,[ecx+4]; mov ecx,[esp+4];
// mov [eax+220h],ecx; ret 4. The original leaves EAX = [client+4]; there is no null check, so a null client
// faults on the store exactly as the original does.
void* __fastcall recovered_client_set_in_start_new_module_005ed630(void* client, void*, std::uint32_t value) {
    using namespace kotor_w3;
    void* const internal = rp(client, 4);
    st<U>(internal, 0x220, value);
    return internal;
}

// 00678030 CSWCMessage::SendPlayerToServerLogin_LoginIFOCharacter: new(3) {70h,02h,0Fh}; null -> 0;
// net = [[007A39FC]+4]->vtable[10h] (thiscall, ECX=client); 005D4CB0(net, [0074C528], buffer, 3, 0) RET 10h;
// free(buffer); return the send result. [0074C528] is the immutable .rdata server-player cell FFFFFFFDh.
std::uint32_t __fastcall recovered_client_send_login_ifo_character_00678030(void*, void*) {
    using namespace kotor_w3;
    static const U kServerPlayerId_0074c528 = 0xFFFFFFFDu;   // .rdata 0074C528, writable=false
    unsigned char* const message = static_cast<unsigned char*>(live_operator_new(3));   // 006FA7E6 cdecl
    if (message == nullptr) return 0;                        // 0067803D..00678042
    message[0] = 0x70;                                       // 00678044
    message[1] = 0x02;                                       // 00678047
    message[2] = 0x0F;                                       // 0067804B
    void* const client = client_app();                       // [007A39FC]+4
    void* const net = vslot<void* (__fastcall*)(void*, void*)>(client, 0x10)(client, nullptr);   // 00678057..00678065
    const U result = net_layer_send_message_to_player_005d4cb0(net, nullptr, kServerPlayerId_0074c528,
                                                               message, 3, 0);   // 0067806A
    live_free(message);                                      // 00678072
    return result;                                           // 0067807A..0067807E
}

// 00652840 CSWCMessage::HandleServerToPlayerModule_StartStartNewModule: client=[007A39FC]+4;
// SetInStartNewModule(client, 1); return 1. ECX/EDX (self/dummy) are ignored; RET 0.
std::uint32_t __fastcall recovered_client_handle_start_new_module_start_00652840(void*, void*) {
    using namespace kotor_w3;
    (void)recovered_client_set_in_start_new_module_005ed630(client_app(), nullptr, 1);   // 00652840..0065284A
    return 1;                                                                             // 0065284F..00652854
}

// 00652860 CSWCMessage::HandleServerToPlayerModule_EndStartNewModule: flag=ReadBOOL(self); client=[007A39FC]+4;
// SetInStartNewModule(client, 0) BEFORE the checks; overrun(self,1) then underrun(self,1); either -> 0; only flag==1
// sends the LoginIFOCharacter message; return 1 independently of the send result. RET 0.
std::uint32_t __fastcall recovered_client_handle_start_new_module_end_00652860(void* self, void*) {
    using namespace kotor_w3;
    const unsigned flag = kotor_fastcall_004d66f0(self, nullptr);                   // 00652864
    (void)recovered_client_set_in_start_new_module_005ed630(client_app(), nullptr, 0);   // 0065286B..00652875
    if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;                   // 0065287E..00652885
    if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;                   // 0065288B..00652892
    if (flag == 1) (void)recovered_client_send_login_ifo_character_00678030(self, nullptr);   // 00652894..0065289B
    return 1;                                                                       // 006528A1..006528AC
}
// 006529F0 CSWCMessage::HandleServerToPlayerSaveGameStatus (stale-stubs G5): constructs two unused CExoStrings and a
// CResRef (the original's dead locals), reads one BYTE (8 bits) and discards it; overflow(1) or underflow(1) -> 0,
// otherwise 1. Both strings are destroyed on either exit (CResRef has no destructor). ECX=self, RET 0.
std::uint32_t __fastcall recovered_client_handle_save_game_status_006529f0(void* self, void*) {
    alignas(4) unsigned char first[8], resref[16], second[8];
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(first), nullptr);   // 00652A0B..00652A0F
    (void)kotor_fastcall_00405ed0(resref, nullptr);                                                   // 00652A14..00652A20
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(second), nullptr);  // 00652A25..00652A31
    (void)kotor_fastcall_004d6720(self, nullptr, 8);                                                  // 00652A36..00652A3F
    std::uint32_t result = 1;                                                                         // 00652A7D
    if (kotor_fastcall_004d6230(self, nullptr, 1) != 0 ||                                             // 00652A44..00652A4F
        kotor_fastcall_004d6240(self, nullptr, 1) != 0)                                               // 00652A51..00652A5C
        result = 0;                                                                                   // 00652AB5
    frame_LogStringDtor_005e5c20(second, nullptr);                                                    // 00652A5E..00652A67 / 00652A92..00652A9B
    frame_LogStringDtor_005e5c20(first, nullptr);                                                     // 00652A6C..00652A78 / 00652AA0..00652AAC
    return result;
}
// 00652AD0 CSWCMessage::HandleServerToPlayerEndGame (stale-stubs G5): ReadCExoString(32 bits) into a local, destroy it,
// return 1. No overflow/underflow check in the original. ECX=self, RET 0.
std::uint32_t __fastcall recovered_client_handle_end_game_00652ad0(void* self, void*) {
    alignas(4) unsigned char text[8];
    (void)kotor_fastcall_004d67e0(self, nullptr, text, 0x20);                                         // 00652AD3..00652ADA
    frame_LogStringDtor_005e5c20(text, nullptr);                                                      // 00652ADF..00652AE3
    return 1;                                                                                         // 00652AE8..00652AF0
}
// ---- stale-stubs G5 (Claude): 005ED430 CClientExoApp::UnloadModule = mov ecx,[ecx+4]; jmp 005F8290.
void __fastcall recovered_client_unload_module_005ed430(void* app, void*) {
    kotor_fastcall_005f8290(kotor_w3::rp(app, 4), nullptr);                         // 005ED430..005ED433
}
// ---- render-core leaf absent from runtime1115 (only the trigger node clone needs it)
// ---- CSWCArea members not on the first-area path
// ---- stale-stubs G5 (Claude): 0060A2E0 CSWCArea scalar deleting destructor: ~CSWCArea; flags bit0 -> free. RET 4.
void* __fastcall recovered_client_area_scalar_destructor_0060a2e0(void* self, void*, std::uint32_t flags) {
    kotor_fastcall_0060a0f0(self, nullptr);                                         // 0060A2E1..0060A2E3
    if ((flags & 1u) != 0) live_free(self);                                         // 0060A2E8..0060A2F0
    return self;                                                                    // 0060A2F8
}
// 00604C90 is the unique real area provider under this same retained W3 symbol.
// ---- mini-game areas (not Endar). integration w3-a1 (mg_integration): the four former W3 stoppers are retired.
// 1204-02 CSWCArea::LoadArea keeps its recorded W3 import names; each is bound BY NAME to the selected t3 export
// (t3 e1 / e3load1 / d1 / e3builders3). /alternatename applies only while the W3 name has no definition, and this
// file no longer defines one. RET checked against swkotor.exe: 00671C40 RET 4, 006723D0 RET 0, 006720C0 RET 8,
// 00672240 RET 8. Call sites 00607789..8C (push area; ECX = new 0xF0 block), 0060872B..34 / 006087BB..C4
// (push position; push &ref; ECX = [area+264h]) and 006087DA..E5 (ECX = [area+264h]; TEST EAX).
#pragma comment(linker, "/alternatename:@recovered_client_minigame_construct_00671c40@12=@kotor_fastcall_00671c40@12")   // CSWMiniGame::CSWMiniGame (t3 e1)
#pragma comment(linker, "/alternatename:@recovered_client_minigame_load_006723d0@8=@kotor_fastcall_006723d0@8")          // CSWMiniGame::Load (t3 e3load1)
#pragma comment(linker, "/alternatename:@recovered_client_minigame_add_track_006720c0@16=@kotor_fastcall_006720c0@16")   // CSWMiniGame::AddTrack (t3 d1)
#pragma comment(linker, "/alternatename:@recovered_client_minigame_add_obstacle_00672240@16=@kotor_fastcall_00672240@16") // CSWMiniGame::AddObstacle (t3 e3builders3)
// closeC: the CLYT minigame-track accessors (thiscall(out, index) RET 8, EAX = out) and the [+28] track-count getter
// are leaves; transcribed from swkotor.exe. Track/obstacle names are 16-byte CResRefs, positions 12-byte Vectors.
void* __fastcall recovered_bridge_layout_track_name_005de4d0(void* self, void*, void* out, std::uint32_t index) {
    const unsigned char* const src = static_cast<const unsigned char*>(kotor_w3::rp(self, 0x38)) + (index << 4);   // 005DE4D0..005DE4DA
    for (unsigned k = 0; k < 16; k += 4) std::memcpy(static_cast<unsigned char*>(out) + k, src + k, 4);          // 005DE4E1..005DE4F6
    return out;                                                                                                // 005DE4DC / 005DE4FA
}
void* __fastcall recovered_bridge_layout_track_position_005de500(void* self, void*, void* out, std::uint32_t index) {
    const unsigned char* const src = static_cast<const unsigned char*>(kotor_w3::rp(self, 0x44)) + index * 12u;    // 005DE500..005DE50A
    for (unsigned k = 0; k < 12; k += 4) std::memcpy(static_cast<unsigned char*>(out) + k, src + k, 4);          // 005DE512..005DE521
    return out;                                                                                                // 005DE50D / 005DE525
}
void* __fastcall recovered_bridge_layout_obstacle_name_005de530(void* self, void*, void* out, std::uint32_t index) {
    const unsigned char* const src = static_cast<const unsigned char*>(kotor_w3::rp(self, 0x3c)) + (index << 4);   // 005DE530..005DE53A
    for (unsigned k = 0; k < 16; k += 4) std::memcpy(static_cast<unsigned char*>(out) + k, src + k, 4);          // 005DE541..005DE556
    return out;
}
void* __fastcall recovered_bridge_layout_obstacle_position_005de560(void* self, void*, void* out, std::uint32_t index) {
    const unsigned char* const src = static_cast<const unsigned char*>(kotor_w3::rp(self, 0x48)) + index * 12u;    // 005DE560..005DE56A
    for (unsigned k = 0; k < 12; k += 4) std::memcpy(static_cast<unsigned char*>(out) + k, src + k, 4);          // 005DE572..005DE581
    return out;
}
std::uint32_t __fastcall recovered_layout_track_count_005d64f0(void* self, void*) {
    std::uint32_t v; std::memcpy(&v, static_cast<const unsigned char*>(self) + 0x28, 4); return v;              // 005D64F0: mov eax,[ecx+28h]
}
// 00710E50 GetProperty0x2c: rebound to the existing owner c2da_current_resource_size_00710e50 (0306-tpc-lowers.obj) in t2b.
// ---- area content objects (map pins, placeable statics/lights, room sounds, sound objects, ambient sound)
// 00692540 CSWCMapPin::CSWCMapPin is provided by the map-pin lane object map_pin_ctor.obj.
// ---- creature members outside this delivery
// Legacy W3 name delegates to the original pointer-return getter, RET4/@12.
void* __fastcall object_vfx_00614aa0(void*, void*, std::uint32_t);
void* __fastcall recovered_client_creature_get_creature_light_00614aa0(void* self, void*, std::uint32_t shadowing) {
    return object_vfx_00614aa0(self, nullptr, shadowing);
}
}

// =====================================================================================================================
// stale-stubs G5 (Claude): faithful bodies of the original callees of 005F8290 / 0060A0F0 that no linked object
// provides. Transcribed from swkotor.exe (read-only); comment addresses are original instructions.
namespace {
using kotor_w3::U;
inline U* cell(void* base, unsigned off) { return static_cast<U*>(kotor_w3::at(base, off)); }
inline void* below(void* data) { return static_cast<unsigned char*>(data) - 4; }
// eh_vector_destructor_iterator(data, size, count=[data-4], dtor) as 006FA852 runs it: last element first.
void destroy_counted_array(void* data, unsigned size, void (__fastcall* dtor)(void*, void*)) {
    U count = kotor_w3::rd32(below(data), 0);
    while (count-- != 0) dtor(kotor_w3::at(data, count * size), nullptr);
}
const void* const ambient_sound_base_methods_00747470[9] = {   // 00747470: nine _purecall (006FB8DE) slots
    reinterpret_cast<const void*>(&packed_purecall_006fb8de), reinterpret_cast<const void*>(&packed_purecall_006fb8de),
    reinterpret_cast<const void*>(&packed_purecall_006fb8de), reinterpret_cast<const void*>(&packed_purecall_006fb8de),
    reinterpret_cast<const void*>(&packed_purecall_006fb8de), reinterpret_cast<const void*>(&packed_purecall_006fb8de),
    reinterpret_cast<const void*>(&packed_purecall_006fb8de), reinterpret_cast<const void*>(&packed_purecall_006fb8de),
    reinterpret_cast<const void*>(&packed_purecall_006fb8de),
};
}

extern "C" {
// 004B1D80 (Ghidra name CServerExoAppInternal::IsInjured; it is GetPauseState's body): (BYTE [this+10078h] & (BYTE)mask) != 0.
std::uint32_t __fastcall kotor_fastcall_004b1d80(void* server_internal, void*, std::uint32_t mask) {
    const unsigned char bits = static_cast<unsigned char>(kotor_w3::ld<unsigned char>(server_internal, 0x10078) &
                                                          static_cast<unsigned char>(mask));   // 004B1D82..004B1D88
    return bits != 0 ? 1u : 0u;                                                     // 004B1D8C..004B1D92 (cmp/sbb/neg)
}
// 004AE980 CServerExoApp::GetPauseState: mov ecx,[ecx+4]; jmp 004B1D80.
std::uint32_t __fastcall kotor_fastcall_004ae980(void* server, void*, std::uint32_t mask) {
    return kotor_fastcall_004b1d80(kotor_w3::rp(server, 4), nullptr, mask);
}
// 004AE650 CObjectTableManager::RemovePlayer(index): -1 or >= 60h -> 0; empty slot -> 0; else free the table's
// data block and the table, clear the slot, return 1.
std::uint32_t __fastcall kotor_fastcall_004ae650(void* table, void*, std::uint32_t index) {
    if (index == 0xFFFFFFFFu || index >= 0x60u) return 0;                           // 004AE656..004AE666
    void* const entry = kotor_w3::rp(table, index * 4);                             // 004AE66A
    if (entry == nullptr) return 0;                                                 // 004AE66D..004AE676
    if (void* const data = kotor_w3::rp(entry, 0)) live_free(data);                 // 004AE679..004AE685
    live_free(entry);                                                               // 004AE688..004AE68E
    kotor_w3::st<U>(table, index * 4, 0);                                           // 004AE692
    return 1;                                                                       // 004AE69A
}
// 005EDAE0 CClientExoApp::SetAreaNotReadyToDisplay(value): [[this+4]+288h] = value.
void __fastcall kotor_fastcall_005edae0(void* client, void*, std::uint32_t value) {
    kotor_w3::st<U>(kotor_w3::rp(client, 4), 0x288, value);                         // 005EDAE0..005EDAE7
}
// 00676B00 / 00676FA0: bodies owned by claude_s2p_small_journal2.cpp (journal lane); declarations above.

// 005F8290 CClientExoAppInternal::UnloadModule. ECX this (client internal), RET.
// .rdata bound to proven values: [0074EA80] = "0 0 0" (clear colour text), [00746644] = 7F000000h (OBJECT_INVALID),
// [0074C5EC] = 0 (sound mode). [GuiInGame+B4h] is read without a null check, as in the original.
void __fastcall kotor_fastcall_005f8290(void* self, void*) noexcept(false) {
    using namespace kotor_w3;
    recovered_set_clear_color_00401690("0 0 0");                                    // 005F82A8..005F82AF
    const U invalid = 0x7F000000u;                                                  // 005F82BA [00746644]
    st<U>(self, 0x20, invalid);                                                     // 005F82C6
    if (rp(self, 0x270) != nullptr) kotor_fastcall_006364c0(rp(self, 0x270), nullptr, invalid);   // 005F82B4..005F82CE
    const U gui_state = rd32(rp(self, 0x40), 0xB4);                                 // 005F82D3..005F82D6
    if (gui_state == 1 || gui_state == 0) {                                         // 005F82DC..005F82E3
        void* const server = rp(kotor_menu_window_cells::manager_007a39fc, 8);      // 005F82E5..005F82EB
        const U paused = server != nullptr ? kotor_fastcall_004ae980(server, nullptr, 2) : 0;   // 005F82EE..005F82FB
        const U flags = rd32(self, 0x37C);                                          // 005F82FD
        if ((flags & 4u) == 0 && paused != 0) {                                     // 005F8303..005F830A
            st<U>(self, 0x37C, flags | 4u);                                         // 005F830C..005F830F
            st<U8>(self, 0x388, 0);                                                 // 005F8315
            st<U>(self, 0x380, 0);                                                  // 005F831B
            kotor_fastcall_005df580(kotor_menu_window_cells::input_007a39e4, nullptr);   // 005F8321..005F8327
            menu_tables_required::SoundSetMode_005d5e80(kotor_cell_sound_007a39ec, nullptr, 0);   // 005F832C..005F8339
            st<U>(self, 0x4C4, 0);                                                  // 005F833E
        }
    }
    kotor_fastcall_005f7fa0(self, nullptr, 2, 0, 0);                                // 005F8344..005F834A
    kotor_fastcall_005f7fa0(self, nullptr, 1, 0, 0);                                // 005F834F..005F8355
    client_internal_clear_pause_exclusion_list_005f7e20(self, nullptr);             // 005F835A..005F835C
    void* const module = rp(self, 0x18);                                            // 005F8361
    if (module == nullptr) return;                                                  // 005F8364..005F8366
    delete_object(module);                                                          // 005F836C..005F8371 vtbl[0](1)
    st<void*>(self, 0x18, nullptr);                                                 // 005F8374
    void* const table = kotor_fastcall_00401060(kotor_menu_window_cells::manager_007a39fc, nullptr, 0);   // 005F8377..005F837E
    (void)kotor_fastcall_004ae650(table, nullptr, 0);                               // 005F8373, 005F8383..005F8385
    if (void* const objects = rp(self, 0x14)) {                                     // 005F838A..005F838F
        kotor_fastcall_004d7e10(objects, nullptr);                                  // 005F8391..005F8393
        live_free(objects);                                                         // 005F8398..005F8399
    }
    st<U>(self, 0x248, 0);                                                          // 005F83A6
    if (rp(self, 0x3C) != nullptr) kotor_fastcall_00676fa0(rp(self, 0x3C), nullptr);   // 005F83A1..005F83AF
    if (rp(self, 0x38) != nullptr) kotor_fastcall_00676b00(rp(self, 0x38), nullptr);   // 005F83B4..005F83BB
    if (rp(self, 0x40) != nullptr) frame_R4_0062c310(rp(self, 0x40), nullptr);      // 005F83C0..005F83C7
    if (rp(self, 0x2A0) != nullptr) w5_player_control_initialize_006792d0(rp(self, 0x2A0), nullptr);   // 005F83CC..005F83D6
    void* objects = nullptr;                                                        // 005F83FC
    if (void* const memory = live_operator_new(0x18)) {                             // 005F83DB..005F83EF
        try {
            objects = static_cast<kotor_menu_10_19::ObjectArray*>(memory)->construct_004d7d80(1);   // 005F83F1..005F83F5
        } catch (...) {
            live_free(memory);                                                      // EH state 0 unwind: delete the block
            throw;
        }
    }
    st<void*>(self, 0x14, objects);                                                 // 005F83FE
    kotor_fastcall_005edae0(client_app(), nullptr, 1);                              // 005F8401..005F8414
    const U bits = rd32(self, 0x384) & 0xFFFFFFFEu;                                 // 005F8419..005F841F
    st<U>(self, 0x38C, 0); st<U>(self, 0x390, 0); st<U>(self, 0x394, 0);            // 005F8422..005F842E
    st<U>(self, 0x3A0, 0);                                                          // 005F8434
    st<U8>(self, 0x398, 0xFF);                                                      // 005F843A
    st<U>(self, 0x384, bits);                                                       // 005F8441
    st<U>(self, 0x324, 0); st<U>(self, 0x328, 0); st<U>(self, 0x320, 0);            // 005F8447..005F8453
}

// 00506A70 CSWSArea::DetachFromClientArea: [this+2D0h] = 0.
void __fastcall kotor_fastcall_00506a70(void* server_area, void*) {
    kotor_w3::st<U>(server_area, 0x2D0, 0);                                         // 00506A70
}
// 0068D430 CSWCAmbientSoundTrack::~CSWCAmbientSoundTrack: four CExoStrings, +18h, +10h, +8, +0.
void __fastcall kotor_fastcall_0068d430(void* track, void*) {
    frame_LogStringDtor_005e5c20(kotor_w3::at(track, 0x18), nullptr);               // 0068D44D..0068D458
    frame_LogStringDtor_005e5c20(kotor_w3::at(track, 0x10), nullptr);               // 0068D45D..0068D465
    frame_LogStringDtor_005e5c20(kotor_w3::at(track, 0x08), nullptr);               // 0068D46A..0068D472
    frame_LogStringDtor_005e5c20(track, nullptr);                                   // 0068D477..0068D481
}
// 0068E0D0 CSWCAmbientSound::~CSWCAmbientSound: vtable 007541C8; free the two streaming sources (+E0h, +E4h);
// destroy the five tracks (+ACh..+2Ch, last first); base vtable 00747470 (abstract: nine _purecall slots).
void __fastcall kotor_fastcall_0068e0d0(void* sound, void*) {
    using namespace kotor_w3;
    st<const void*>(sound, 0, recovered_client_ambient_sound_methods_007541c8);    // 0068E0EE
    if (void* const music = rp(sound, 0xE0)) {                                      // 0068E0F4..0068E104
        kotor_fastcall_005d6200(music, nullptr);                                    // 0068E106..0068E108
        live_free(music);                                                           // 0068E10D..0068E10E
        st<void*>(sound, 0xE0, nullptr);                                            // 0068E116
    }
    if (void* const ambient = rp(sound, 0xE4)) {                                    // 0068E120..0068E128
        kotor_fastcall_005d6200(ambient, nullptr);                                  // 0068E12A..0068E12C
        live_free(ambient);                                                         // 0068E131..0068E132
        st<void*>(sound, 0xE4, nullptr);                                            // 0068E13A
    }
    for (unsigned off = 0xAC; off >= 0x2C; off -= 0x20)                             // 0068E144..0068E186
        kotor_fastcall_0068d430(at(sound, off), nullptr);
    st<const void*>(sound, 0, ambient_sound_base_methods_00747470);                // 0068E18F
}

// 0060A0F0 CSWCArea::~CSWCArea. ECX this, RET.
void __fastcall kotor_fastcall_0060a0f0(void* self, void*) noexcept(false) {
    using namespace kotor_w3;
    st<const void*>(self, 0, recovered_client_area_methods_0074eef0);              // 0060A10F
    st<const void*>(self, 0x100, recovered_client_area_object_methods_0074ee80);   // 0060A115
    recovered_client_area_unload_area_00609410(self, nullptr);                      // 0060A127
    if (void* const ambient = rp(self, 0x1DC)) {                                    // 0060A12C..0060A136
        kotor_fastcall_0068e0d0(ambient, nullptr);                                  // 0060A138..0060A13A
        live_free(ambient);                                                         // 0060A13F..0060A140
    }
    st<void*>(self, 0x1DC, nullptr);                                                // 0060A150
    if (void* const rooms = rp(self, 0x260)) {                                      // 0060A148..0060A156
        if (rd32(below(rooms), 0) != 0)                                             // 0060A158..0060A160
            delete_object_flags(rooms, 3);                                          // 0060A162..0060A166 vtbl[0](3)
        else
            live_free(below(rooms));                                                // 0060A16A..0060A16B
    }
    st<void*>(self, 0x260, nullptr);                                                // 0060A17B
    if (void* const object = rp(self, 0x264)) delete_object(object);                // 0060A173..0060A187 vtbl[0](1)
    if (void* const server_area = rp(self, 0x27C)) {                                // 0060A189..0060A191
        kotor_fastcall_00506a70(server_area, nullptr);                              // 0060A193
        st<void*>(self, 0x27C, nullptr);                                            // 0060A198
    }
    if (void* const list = rp(self, 0x280)) {                                       // 0060A19E..0060A1AB
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(list), nullptr);   // 0060A1AD..0060A1AF
        live_free(list);                                                            // 0060A1B4..0060A1B5
    }
    if (void* const block = rp(self, 0x244)) live_free(block);                      // 0060A1BD..0060A1CD
    if (void* const block = rp(self, 0x238)) live_free(block);                      // 0060A1D5..0060A1E5
    live_free(rp(self, 0x224)); st<void*>(self, 0x224, nullptr);                    // 0060A1ED..0060A1FE
    live_free(rp(self, 0x218)); st<void*>(self, 0x218, nullptr);                    // 0060A204..0060A215
    live_free(rp(self, 0x20C)); st<void*>(self, 0x20C, nullptr);                    // 0060A21B..0060A22C
    if (void* const list = rp(self, 0x204)) {                                       // 0060A232..0060A242
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(list), nullptr);   // 0060A244..0060A246
        live_free(list);                                                            // 0060A24B..0060A24C
    }
    live_free(rp(self, 0x1D0)); st<void*>(self, 0x1D0, nullptr);                    // 0060A254..0060A265
    live_free(rp(self, 0x1C4)); st<void*>(self, 0x1C4, nullptr);                    // 0060A26B..0060A285
    frame_LogStringDtor_005e5c20(at(self, 0x17C), nullptr);                         // 0060A27F..0060A290
    live_free(rp(self, 0x11C)); st<void*>(self, 0x11C, nullptr);                    // 0060A295..0060A2A6
    st<const void*>(self, 0x100, kotor_vtable_cgameobject_007457a0);               // 0060A2B1
    kotor_fastcall_0058f7a0(self, nullptr);                                         // 0060A2AF..0060A2C3
}
}
