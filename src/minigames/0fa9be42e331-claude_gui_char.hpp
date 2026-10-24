#pragma once
// Lane W4 GUI successor (worker "gui-char"): declarations shared by claude_gui_char_*.cpp (character, abilities,
// party selection, examine / quest item, store, mini-game). Only existing c24 candidate-map providers (none in the c24
// trap census) and functions defined in claude_gui_char_*.cpp are declared here.
#include "d796cdfe6547-gui_panels.hpp"

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs); // CExoString::operator+
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned capacity); // CExoArrayList::Allocate
}
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable

extern "C" {
// ---- GUI manager / panel / controls
void __fastcall kotor_fastcall_0040a140(void* manager, void*, w4::U32 sound);                     // CSWGuiManager::PlayGuiSound
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, w4::U32, w4::U32);     // CSWGuiManager::AddPanel
void* __fastcall kotor_fastcall_0040be00(void* manager, void*);                                   // CSWGuiManager::PopModalPanel
void __fastcall frame_RemovePanel_0040c830(void* manager, void*, void* panel);                    // CSWGuiManager::RemovePanel
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                             // CSWGuiPanel::OnPanelAdded
void __fastcall gui_load_panel_on_removed_0040c170(void* panel, void*);                           // CSWGuiPanel::OnPanelRemoved
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, w4::U32 event, w4::U32 down); // CSWGuiPanel::HandleInputEvent
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, w4::U32 delta_bits);                // CSWGuiPanel::Draw(float)
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                      // CSWGuiListBox::ClearItems
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, w4::I32 index, w4::I32 flag); // SetSelectedControl
void* __fastcall kotor_fastcall_0041a5a0(void* listbox, void*, w4::I32 index);                    // CSWGuiListBox::GetControl
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, w4::U32, w4::U32, w4::U32); // CSWGuiListBox::AddControls
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, void* object, w4::U32, w4::U32); // SetCallback
w4::U32 __fastcall frame_R4_0062cba0(void* gui, void*, w4::U32 flag);                             // CGuiInGame::HideSWInGameGui
void __fastcall gui_m48_client_set_input_class_005eda60(void* app, void*, w4::U32 input_class, w4::U32 show); // SetInputClass
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, w4::I32) noexcept(false); // SetFillImage
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, w4::U32, w4::U32, w4::U32); // SetPulsingAlpha
// ---- client / server apps
void* __fastcall recovered_server_info_004aec90(void* server, void*);                             // CServerExoApp::GetServerInfo
w4::U32 __fastcall recovered_npc_object_00564700(void* table, void*, w4::I32 index, w4::U32, w4::U32); // CSWPartyTable::GetNPCObject
w4::U32 __fastcall kotor_fastcall_005636b0(void* table, void*, w4::I32 index);                    // CSWPartyTable::GetIsNPCAvailable
w4::U32 __fastcall kotor_fastcall_005eda50(void* app, void*, w4::U32 id);                         // ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                       // CClientExoApp::GetSWParty
void* __fastcall kotor_fastcall_00635460(void* party, void*);                                     // CSWParty::GetPlayerCharacter
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, w4::U32 index);                  // CSWParty::GetCharacter
w4::U32 __fastcall kotor_fastcall_004aea40(void* server, void*);                                  // CServerExoApp::GetPlayerCreatureId
void* __fastcall kotor_fastcall_004ae770(void* server, void*, w4::U32 id);                        // CServerExoApp::GetCreatureByGameObjectID
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, w4::U32 id);                 // CServerExoApp::GetItemByGameObjectID
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);         // CSWCCreature::GetServerCreature
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                           // CClientExoApp::GetPlayerCreature
w4::U32 __fastcall kotor_fastcall_005edc10(void* app, void*);                                     // CClientExoApp::GetPausedByCombat
void __fastcall kotor_fastcall_005edc20(void* app, void*, w4::U32, w4::U32, w4::U32);             // CClientExoApp::SetPausedByCombat
w4::U32 __fastcall kotor_fastcall_005edf80(void* app, void*, w4::U32, w4::U32, w4::U32);          // ChangeCharacterToNextLivingPartyMember
int __fastcall show_tutorial_window_005edf40(void* app, void*, w4::U32, w4::U32, w4::U32, w4::U32); // ShowTutorialWindow
// ---- strings
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, w4::U32 id);               // CClientExoApp::GetGUIString
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, w4::I32 value);                       // CExoString::CExoString(int)
// ---- colours (.data)
extern const unsigned char borrowed_exit_color_007a23b4[12];                                     // [007A23B4] (0, .66, .98)
extern const unsigned char borrowed_enter_color_007a23c0[12];                                    // [007A23C0]
extern const unsigned char kotor_m15d_color_007a23d8[12];                                        // [007A23D8] (.74, .11, 0) red
extern const unsigned char kotor_m15d_color_007a23e4[12];                                        // [007A23E4] (.28, .92, .11) green
extern const unsigned char kotor_m1_color_007a2378[12];                                          // [007A2378]
extern const unsigned char kotor_m1_color_007a2384[12];                                          // [007A2384]
extern const unsigned char kotor_m1_color_007a2408[12];                                          // [007A2408]
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
extern volatile w4::U32 kotor_cell_do3dgui_0078d1e4;                                             // [0078D1E4] g_bDo3DGui
}

namespace gchar {
using namespace w4;
using namespace w4gui;
inline void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
inline void* ingame_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }               // CClientExoApp::GetInGameGui
inline void* manager_of(void* panel) { return lp(panel, 0x18); }                                 // CSWGuiPanel +18h
inline void play_sound(void* panel, U32 sound) { kotor_fastcall_0040a140(manager_of(panel), nullptr, sound); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
// Original store through a possibly null object pointer (the original faults at the same address).
inline void raw_store(void* object, U32 off, U32 value) {
    *reinterpret_cast<volatile U32*>(static_cast<std::uintptr_t>(ad(object) + off)) = value;
}

// CExoString living on the caller's stack (8 bytes). Constructed by a constructor call or by a callee writing a
// by-value result into it; destroyed by ~CExoString at scope end (also while unwinding = the original EH state).
struct Str {
    U32 s[2]; bool live = false;
    Str() = default;
    explicit Str(const char* text) { frame_LogStringCtor_005e5a90(s, nullptr, text); live = true; }
    explicit Str(I32 value) { kotor_fastcall_005e5bc0(s, nullptr, value); live = true; }      // CExoString(int) 005E5BC0
    Str(const Str&) = delete; Str& operator=(const Str&) = delete;
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }
    void empty() { exo_default(s); live = true; }                                              // CExoString() 005B3190
    void* p() { return s; }
    kotor_reconstruction::StartupString* ss() { return static_cast<kotor_reconstruction::StartupString*>(static_cast<void*>(s)); }
};
// GetGUIString(out, id) into a stack temporary.
inline void* gui_string(Str& out, U32 id) { void* r = kotor_fastcall_005edeb0(client_app(), nullptr, out.s, id); out.live = true; return r; }
// CExoString passed by value (the callee owns and destroys the 8 bytes).
struct ExoVal { U32 w[2]; };
static_assert(sizeof(ExoVal) == 8, "CExoString by value");

// Inline colour choice used by the panels: >0 green [007A23E4], <0 red [007A23D8], 0 [007A23B4] (signed jle/jge).
inline const void* bonus_color(I32 value) {
    if (value > 0) return kotor_m15d_color_007a23e4;
    if (value < 0) return kotor_m15d_color_007a23d8;
    return borrowed_exit_color_007a23b4;
}

// Stack CExoArrayList<T*> {data, size, capacity}; the destructor is the original _free(data).
struct PtrList {
    void** data = nullptr; I32 size = 0; I32 capacity = 0;
    PtrList() = default; PtrList(const PtrList&) = delete; PtrList& operator=(const PtrList&) = delete;
    ~PtrList() { live_free(data); }
};
static_assert(sizeof(PtrList) == 12, "CExoArrayList");
// Inlined CExoArrayList::Add: grow to capacity*2 (0x10 when empty) through Allocate when full, then append.
inline void list_add(PtrList& list, void* value) {
    if (list.size == list.capacity)
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr,
                                            list.capacity != 0 ? static_cast<U32>(list.capacity) * 2u : 0x10u);
    list.data[list.size] = value;
    ++list.size;
}
}  // namespace gchar

// ---- functions defined in claude_gui_char_*.cpp and used across them
extern "C" {
w4::U32 __fastcall kotor_fastcall_005ee190(void* app, void*);                                    // GetCharacterChangeInProgress
void __fastcall kotor_fastcall_0062d440(void* gui, void*);                                       // CGuiInGame::HideExamineBox
w4::U32 __fastcall kotor_fastcall_00563710(void* table, void*, w4::I32 npc);                     // CSWPartyTable::GetIsMember
void* __fastcall kotor_fastcall_004ae7b0(void* server, void*, w4::U32 id);                       // CServerExoApp::GetStoreByGameObjectID
}
