#pragma once
// Lane W4 G15: declarations shared by the in-game GUI construction units (ctor/dtor entry points and built tables).
#include "8c7ada52dac9-gui_rt.hpp"

namespace w4 { extern const volatile U32 ingame_gui_single_panel_007a2370; }   // owned in src/hud/ingame_gui.cpp

namespace w4gui {
// built method tables (src/gui/gui_vtables.cpp, generated)
const void* vt_letterbox();
const void* vt_bark_bubble();
const void* vt_area_transition();
const void* vt_pause();
const void* vt_solo_mode_query();
const void* vt_examine();
const void* vt_blackened_label();
const void* vt_speed_button();
const void* vt_slider_read_only();
const void* vt_script_select();
const void* vt_character();
const void* vt_equip();
const void* vt_inventory();
const void* vt_abilities();
const void* vt_store();
const void* vt_party_selection();
const void* vt_party_selection_data();
const void* vt_ingame_options();
const void* vt_galaxy_map();
const void* vt_map_hider();
const void* vt_ingame_map();
const void* vt_journal();
const void* vt_quest_item();
const void* vt_ingame_menu();
const void* vt_status_summary();
const void* vt_container();
const void* vt_messages();
const void* vt_pfs_debug();
const void* vt_create_item_sub_menu();
const void* vt_create_item_menu();
const void* vt_load_module_debug();
}

extern "C" {
// W4 panel constructors: thiscall(manager) RET 4, return this.
void* __fastcall kotor_fastcall_006d0620(void*, void*, void*);   // CSWGuiCreateItemMenu
void* __fastcall kotor_fastcall_006cfa70(void*, void*, void*);   // CSWGuiLoadModuleDebugMenu
void* __fastcall kotor_fastcall_006cf5a0(void*, void*, void*);   // CSWGuiPowersFeatsSkillsDebugMenu
void* __fastcall kotor_fastcall_006bdc60(void*, void*, void*);   // CSWGuiCreateItemSubMenu
void* __fastcall kotor_fastcall_006ce9c0(void*, void*, void*);   // CSWGuiExamine
void* __fastcall kotor_fastcall_006a9770(void*, void*, void*);   // CSWGuiBarkBubble
void* __fastcall kotor_fastcall_006b6dc0(void*, void*, void*);   // CSWGuiContainer
void* __fastcall kotor_fastcall_006a8b40(void*, void*, void*);   // CSWGuiDialogCinematic
void* __fastcall kotor_fastcall_006a8eb0(void*, void*, void*);   // CSWGuiDialogComputer
void* __fastcall kotor_fastcall_006a95f0(void*, void*, void*);   // CSWGuiDialogComputerCamera
void* __fastcall kotor_fastcall_006a8930(void*, void*, void*);   // CSWGuiDialogLetterbox
void* __fastcall kotor_fastcall_00627980(void*, void*, void*);   // CSWGuiInGameMenu
void* __fastcall kotor_fastcall_006c03b0(void*, void*, void*);   // CSWGuiInGamePause
void* __fastcall kotor_fastcall_006c2270(void*, void*, void*);   // CSWGuiInGameSoloModeQuery
void* __fastcall kotor_fastcall_006c7d50(void*, void*, void*);   // CSWGuiInGameAreaTransition
void* __fastcall kotor_fastcall_006bfa40(void*, void*, void*);   // CSWGuiPartySelection
void* __fastcall kotor_fastcall_006c1c00(void*, void*, void*);   // CSWGuiStore
void* __fastcall kotor_fastcall_006ba980(void*, void*, void*);   // CSWGuiInGameEquip
void* __fastcall kotor_fastcall_006b34c0(void*, void*, void*);   // CSWGuiInGameInventory
void* __fastcall kotor_fastcall_006b0e40(void*, void*, void*);   // CSWGuiInGameCharacter
void* __fastcall kotor_fastcall_006272a0(void*, void*, void*);   // CSWGuiStatusSummary
void* __fastcall kotor_fastcall_0068c100(void*, void*, void*);   // CSWGuiMainInterface
void* __fastcall kotor_fastcall_00694d50(void*, void*, void*);   // CSWGuiInGameMap
void* __fastcall kotor_fastcall_006adda0(void*, void*, void*);   // CSWGuiInGameAbilities
void* __fastcall kotor_fastcall_00644a40(void*, void*, void*);   // CSWGuiInGameJournal
void* __fastcall kotor_fastcall_006ab2f0(void*, void*, void*);   // CSWGuiInGameOptions
void* __fastcall kotor_fastcall_00626400(void*, void*, void*);   // CSWGuiInGameMessages
void* __fastcall kotor_fastcall_00695180(void*, void*, void*);   // CSWGuiInGameGalaxyMap
// Initialize family
w4::U8 __fastcall kotor_fastcall_005edff0(void* app, void*, w4::U32 which);   // CClientExoApp::GetLoadStep
w4::U32 __fastcall kotor_fastcall_00632860(void* in_game, void*);             // CGuiInGame::Initialize
w4::U32 __fastcall kotor_fastcall_005f1410(void* client, void*);              // CClientExoAppInternal::InitializeInGameGUI
w4::U32 __fastcall kotor_fastcall_005ed730(void* app, void*);                 // CClientExoApp::InitializeInGameGUI
}
