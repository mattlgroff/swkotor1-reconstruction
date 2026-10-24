// Pazaak lane PZ-B tier a1, part 2: wager popup and side-deck (start) panel. Contract: claude_pazaak_decls.hpp (pz-b h1).
//   CSWGuiWagerPopup (0xCAC, table 007534C8): 0067CB60 HandleButtonWager, 0067D340 UpdateWagerText,
//     0067E150 HandleInputEvent (jump table 0067E278 / index bytes 0067E28C), 0067F000 ctor, 0067F630 scalar deleting
//     dtor, 0067F650 dtor
//   CSWGuiPazaakStart (0x8928, table 007532E8): 0067D3B0 HandleWagerExit, 0067D3D0 HandleQuitDialog,
//     0067D450 isOtherCardActiveControl, 0067D480 isSidedeckCardActiveControl, 0067E2B0 dtor, 0067E400 ShowDisabledButtons,
//     0067E530 OnChosenCardEntered, 0067F730 scalar deleting dtor, 0067F750 OnPanelAdded, 0067FB10 AddChosenCard,
//     0067FD10 RemoveChosenCard, 0067FE80 HandleAvailCardDrop, 0067FED0 HandleChosenCardDrop, 006807E0 HandleMoveCard,
//     006816F0 HandleInputEvent (jump table 00681864 / index bytes 00681878), 00681890 HandleStartDialog,
//     006819E0 HandleAcceptButton, 00681A90 ctor
// Transcribed from the original swkotor.exe (GOG 1.03) disassembly; each statement carries its original PCs.
// Quirks kept: the CSWGuiActivatedButton at +6F34 is never laid out (no InitControl); the start ctor writes the wager
// and opponent through the CSWPazaak pointer without a null check (operator new failure faults as in the original);
// the opponent's 10 local side-deck cards only get their flag words zeroed (values come from LoadInitialSideDeck);
// AddChosenCard / RemoveChosenCard / the party-table reads index the per-kind arrays without bounds checks;
// the wager popup destructor does not restore its vptr; CSWPazaak is released with _free (it has no destructor).
#include "0fa9be42e331-claude_gui_char.hpp"
#include "602f99a6ff9a-claude_pazaak_decls.hpp"
#include <cstdio>

namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; }                 // [007A39F4] CSWGuiManager*
namespace kotor_menu_10_19 { struct String; struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); }; }
namespace kotor_m15d {                                                    // 0829-m15d-skills.obj (folded generic thunks)
struct PanelStepThunks {
    void OnMinusButtonPushed_0067cb40(kotor_input_hover::Control*);       // 0067CB40: this->vslot3C(0x2F, 1)
    void OnPlusButtonPushed_0067cb50(kotor_input_hover::Control*);        // 0067CB50: this->vslot3C(0x30, 1)
};
}

extern "C" {
void __fastcall gui_load_panel_button_b_0040b650(void* panel, void*, void* control);              // CSWGuiPanel::OnBButtonPressed
void __fastcall gui_load_nav_set_move_to_00417660(void* nav, void*, w4::U32 direction, void* control); // CSWGuiNavigable::SetMoveToControl
void __fastcall gui_load_text_params_set_strref_00415e50(void* params, void*, w4::U32 strref) noexcept(false); // SetStrRef
w4::I32 __fastcall w2x_creature_gold_004edd60(void* creature, void*);                            // CSWSCreature::GetGold
void __fastcall kotor_fastcall_006c00b0(void* self, void*, w4::U32 delta_bits);                  // 006C00B0 (jmp CSWGuiPanel::Draw)
}

using namespace gchar;
namespace pz = pazaak;

namespace {
void* gui_manager() { return kotor_menu_window_cells::gui_manager_007a39f4; }                    // [007A39F4]
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); } // [007A3A08]
inline void* mgr_of(void* panel) { return lp(panel, 0x18); }
inline void* card_at(void* self, U32 base, I32 i) { return at(self, base + static_cast<U32>(i) * pz::card::kSize); }
// [panel+44] = ([panel+44] & ~0x300) | 0x400: the manager removes and deletes the panel.
inline void mark_delete(void* self) { sw(self, 0x44, (lw(self, 0x44) & 0xFFFFFCFFu) | 0x400u); }
inline void* in_game_box() { return lp(ingame_gui(), 0x98); }                                    // [GetInGameGui()+98h]
constexpr U32 kColorUsedF = 0x3F2AC083u;   // 0.6670 (0067F8E6 / 0067FC4C)
constexpr U32 kOneF = 0x3F800000u;         // 1.0f (0067FDC8)
const char k_empty_0073d71c[] = "";                                                              // 0073D71C
const char k_int_0073d720[] = "%d";                                                              // 0073D720
const char k_int_int_0073d5b4[] = "%d%d";                                                        // 0073D5B4
const char k_space_0073d5d8[] = " ";                                                             // 0073D5D8
const char k_newline_0073d5cc[] = "\n";                                                          // 0073D5CC
// 16-byte control-name templates (.rdata, copied by dword, then sprintf at the x position), byte-exact.
const char k_btn_chosen_00753790[16] = "BTN_CHOSENx    ";
const char k_lbl_chosen_00753780[16] = "LBL_CHOSENx    ";
const char k_btn_avail_00753770[16] = "BTN_AVAILxx    ";
const char k_lbl_avail_00753760[16] = "LBL_AVAILxx    ";
const char k_lbl_availnum_00753750[16] = "LBL_AVAILNUMxx ";
static_assert(sizeof(k_btn_chosen_00753790) == 16 && sizeof(k_lbl_availnum_00753750) == 16, "templates");

// Inline CSWGuiActivatedButton construction (00681B4C..00681B89 / 00681B8C..00681BC9).
void activated(void* self, U32 off, Unwind& u) {
    void* b = button(self, off, u);
    set_vptr(b, kotor_pazaak_vtable_00753120());
    sw(b, 0x1C8, 0); sw(b, 0x1CC, 0); sw(b, 0x1D0, 0);
    sw(b, 0x44, lw(b, 0x44) | 8u);
    sw(b, 0x1C4, 0xBF800000u);                                                                   // -1.0f
}
// Inline CSWGuiSpeedButton construction (0067F07F..0067F094 / 0067F0AE..0067F0C3).
void speed(void* self, U32 off, Unwind& u) {
    void* b = button(self, off, u);
    set_vptr(b, vt_speed_button());                                                              // 007533C8 (existing table)
    sw(b, 0x1C4, 0x3E4CCCCDu);                                                                   // 0.2f
    sw(b, 0x1C8, 0x3F000000u);                                                                   // 0.5f
}
void __fastcall list_free(void* list, void*) { live_free(lp(list, 0)); sw(list, 0, 0); }        // inlined ~CExoArrayList
void set_color3(void* p, U32 bits) { sw(p, 0, bits); sw(p, 4, bits); sw(p, 8, bits); }
// Player creature of the client party (0067F3A3..0067F3D4; same shape as 005F383C..005F386C).
void* player_creature() {
    void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                               // GetSWParty
    void* pc = kotor_fastcall_00635460(party, nullptr);                                         // GetPlayerCharacter
    const U32 id = kotor_fastcall_004aea30(server_app(), nullptr, lw(pc, 4));                   // ClientToServerObjectId
    return kotor_fastcall_004ae770(server_app(), nullptr, id);                                  // GetCreatureByGameObjectID
}
}  // namespace

extern "C" {
// ======================= CSWGuiWagerPopup =======================
// 0067CB60 HandleButtonWager(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_0067cb60(void* self, void*, void*) {
    vcall<void, U32, U32>(self, 0x3C, 0x2Du, 1u);                                               // 0067CB60..0067CB66
}

// 0067D340 UpdateWagerText() RET
void __fastcall kotor_fastcall_0067d340(void* self, void*) {
    Str s(li(self, 0xC94));                                                                     // 0067D35B..0067D366 CExoString(int)
    gui_load_text_params_set_text_00415e00(at(self, 0x3CC), nullptr, s.p());                    // 0067D36B..0067D37E
}                                                                                               // 0067D38F ~CExoString

// 0067E150 HandleInputEvent(int event, int down) RET 8
void __fastcall kotor_fastcall_0067e150(void* self, void*, I32 event, I32 down) {
    // Jump table 0067E278 (targets 0067E17F accept, 0067E1BB quit, 0067E221 less, 0067E23D more, 0067E269 default),
    // index bytes 0067E28C for event - 0x27 in 0..0x19 (unsigned).
    static constexpr U8 k_index_0067e28c[26] = {0, 1, 4, 4, 4, 4, 0, 1, 2, 3, 3, 2, 4, 4, 4, 4, 4, 4, 3, 2, 2, 3, 3, 2, 2, 3};
    U8 target = 4;
    const U32 index = static_cast<U32>(event) - 0x27u;                                          // 0067E165
    if (down != 0 && index <= 0x19u) target = k_index_0067e28c[index];                          // 0067E155..0067E178
    switch (target) {
    case 0:                                                                                     // 0067E17F accept the wager
        kotor_fastcall_0067d3b0(lp(self, 0xCA8), nullptr, li(self, 0xC94));                    // 0067E17F..0067E18C HandleWagerExit
        kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                      // 0067E191..0067E196
        kotor_fastcall_0040be00(mgr_of(self), nullptr);                                         // 0067E19B..0067E19E PopModalPanel
        frame_RemovePanel_0040c830(mgr_of(self), nullptr, self);                                // 0067E1A3..0067E1A7
        break;
    case 1: {                                                                                   // 0067E1BB quit
        kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                      // 0067E1BB..0067E1C1
        void* box = in_game_box();                                                              // 0067E1C6..0067E1D4
        kotor_fastcall_00627130(box, nullptr, 1);                                               // 0067E1DA..0067E1DE SetAllowCancel(1)
        gui_load_messagebox_set_callback_00624a40(box, nullptr, lp(self, 0xCA8),
            ad(fn(&kotor_fastcall_0067d3d0)), 0);                                               // 0067E1E3..0067E1F3 (0067D3D0)
        vcall<void, U32>(box, 0x6C, 0xA5B8u);                                                   // 0067E1F8..0067E201 SetMessage
        kotor_fastcall_0040bc70(mgr_of(self), nullptr, box, 1, 1);                              // 0067E204..0067E20C AddPanel
        break;
    }
    case 2:                                                                                     // 0067E221 less
        if (li(self, 0xC94) > 1) {                                                              // 0067E221..0067E228 (jle -> default)
            kotor_fastcall_0040a140(mgr_of(self), nullptr, 1);                                  // 0067E22A..0067E22F
            sw(self, 0xC94, lw(self, 0xC94) - 1u);                                              // 0067E234..0067E25E
            kotor_fastcall_0067d340(self, nullptr);                                             // 0067E264 UpdateWagerText
        }
        break;
    case 3:                                                                                     // 0067E23D more
        if (li(self, 0xC94) < li(self, 0xC98)) {                                                // 0067E23D..0067E249 (jge -> default)
            kotor_fastcall_0040a140(mgr_of(self), nullptr, 1);                                  // 0067E24B..0067E250
            sw(self, 0xC94, lw(self, 0xC94) + 1u);                                              // 0067E255..0067E25E
            kotor_fastcall_0067d340(self, nullptr);                                             // 0067E264
        }
        break;
    default: break;                                                                             // 0067E269
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, static_cast<U32>(event), static_cast<U32>(down)); // 0067E1AC/E212/E269 CSWGuiPanel::HandleInputEvent
}

// 0067F000 CSWGuiWagerPopup(CSWGuiManager*, CSWGuiPazaakStart*, int max_wager) RET 0Ch
void* __fastcall kotor_fastcall_0067f000(void* self, void*, void* manager, void* start, I32 max_wager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                  // 0067F026..0067F02B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                  // state 0
    set_vptr(self, kotor_pazaak_vtable_007534c8());                                             // 0067F03E (007534C8)
    label(self, 0x064, u); label(self, 0x1A4, u); label(self, 0x2E4, u);                        // 0067F030..0067F06A states 1..3
    speed(self, 0x424, u);                                                                      // 0067F06F..0067F094 state 4 (BTN_LESS)
    speed(self, 0x5F8, u);                                                                      // 0067F09E..0067F0C3 state 5 (BTN_MORE)
    button(self, 0x7CC, u); button(self, 0x990, u);                                             // 0067F0CD..0067F0EE states 6, 7
    label(self, 0xB54, u);                                                                      // 0067F0F3..0067F101 state 8
    sw(self, 0xCA0, 0); sw(self, 0xCA4, 0); sw(self, 0xC9C, 0);                                 // 0067F106..0067F114 CExoArrayList
    u.push(&list_free, at(self, 0xC9C));                                                        // state 9
    sw(self, 0xCA8, ad(start));                                                                 // 0067F120
    sw(self, 0xC98, static_cast<U32>(max_wager));                                               // 0067F137
    sw(self, 0xC94, static_cast<U32>(max_wager));                                               // 0067F13D
    start_layout(self, "pazaakwager");                                                          // 0067F126..0067F14F (007534BC)
    init_control(self, 0x064, "LBL_TITLE");                                                     // 0067F154..0067F188 (00751A20)
    init_control(self, 0x1A4, "LBL_MAXIMUM");                                                   // 0067F18D..0067F1C4 (007534B0)
    init_control(self, 0x2E4, "LBL_WAGERVAL");                                                  // 0067F1C9..0067F200 (007534A0)
    init_control(self, 0x424, "BTN_LESS");                                                      // 0067F205..0067F236 (00753494)
    init_control(self, 0x5F8, "BTN_MORE");                                                      // 0067F23B..0067F26C (00753488)
    init_control(self, 0x7CC, "BTN_WAGER");                                                     // 0067F271..0067F2A8 (0075347C)
    init_control(self, 0x990, "BTN_QUIT");                                                      // 0067F2AD..0067F2E4 (00753470)
    init_control(self, 0xB54, "LBL_BG");                                                        // 0067F2E9..0067F320 (00753468)
    stop_layout(self);                                                                          // 0067F325..0067F327
    add_event(self, 0x424, static_cast<std::int32_t>(pz::wager::kEventSpeedDown), self,
              pmf_code(&kotor_m15d::PanelStepThunks::OnMinusButtonPushed_0067cb40));             // 0067F32C..0067F339
    add_event(self, 0x5F8, static_cast<std::int32_t>(pz::wager::kEventSpeedDown), self,
              pmf_code(&kotor_m15d::PanelStepThunks::OnPlusButtonPushed_0067cb50));              // 0067F33E..0067F34B
    add_event(self, 0x7CC, 0x27, self, fn(&kotor_fastcall_0067cb60));                           // 0067F350..0067F35E
    add_event(self, 0x990, 0x27, self, fn(&gui_load_panel_button_b_0040b650));                  // 0067F363..0067F371
    sw(self, 0x5E8, 0x3D888889u); sw(self, 0x7BC, 0x3D888889u);                                 // 0067F376..0067F381 (1/15 s repeat)
    sw(self, 0x424 + 0x44, lw(self, 0x424 + 0x44) & 0xFFFFFFFBu);                               // 0067F387..0067F391
    sw(self, 0x5F8 + 0x44, lw(self, 0x5F8 + 0x44) & 0xFFFFFFFBu);                               // 0067F394
    sw(self, 0x810, lw(self, 0x810) & 0xFFFFFFFBu);                                             // 0067F397
    sw(self, 0x9D4, lw(self, 0x9D4) & 0xFFFFFFFBu);                                             // 0067F39D
    void* creature = player_creature();                                                         // 0067F3A3..0067F3D4
    const I32 gold = creature != nullptr ? w2x_creature_gold_004edd60(creature, nullptr) : 0;   // 0067F3D9..0067F3E6
    kotor_fastcall_0067d340(self, nullptr);                                                     // 0067F3E8..0067F3EA UpdateWagerText
    {
        Str total;                                                                              // [esp+38]
        {
            Str s30(gold);                                                                      // 0067F3EF..0067F3F4
            Str s28(k_space_0073d5d8);                                                          // 0067F3F9..0067F40A
            Str s20(k_newline_0073d5cc);                                                        // 0067F40F..0067F420
            Str s18(li(self, 0xC98));                                                           // 0067F425..0067F438
            Str s10(k_space_0073d5d8);                                                          // 0067F43D..0067F44E
            Str t70; Str t68; Str t60; Str t58; Str t50; Str t48; Str t40;
            auto tlk_string = [](Str& out, U32 strref) {
                tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out.p()), strref); out.live = true; };
            auto concat = [](Str& out, Str& lhs, Str& rhs) {
                GuiConcat005e5d10(lhs.ss(), nullptr, out.ss(), rhs.ss()); out.live = true; };
            tlk_string(t70, 0x96C8u);                                                           // 0067F453..0067F46B
            tlk_string(t68, 0x7E41u);                                                           // 0067F470..0067F48A
            concat(t60, t68, s10);                                                              // 0067F48F..0067F4A3
            concat(t58, t60, s18);                                                              // 0067F4A8..0067F4BC
            concat(t50, t58, s20);                                                              // 0067F4C1..0067F4D5
            concat(t48, t50, t70);                                                              // 0067F4DA..0067F4EA
            concat(t40, t48, s28);                                                              // 0067F4EF..0067F503
            concat(total, t40, s30);                                                            // 0067F508..0067F51C
        }                                                                                       // 0067F521..0067F5E8 12 x ~CExoString
        gui_load_text_params_set_text_00415e00(at(self, 0x28C), nullptr, total.p());            // 0067F5ED..0067F5F8
        sw(self, 0x44, lw(self, 0x44) | 0x60u);                                                 // 0067F5FD..0067F607
    }                                                                                           // 0067F612 ~CExoString
    u.dismiss();
    return self;
}

// 0067F630 scalar deleting destructor RET 4
void* __fastcall kotor_fastcall_0067f630(void* self, void*, U32 flags) {
    return deleting(self, &kotor_fastcall_0067f650, flags);                                     // 0067F633..0067F648
}

// 0067F650 ~CSWGuiWagerPopup() RET (no vptr store in the original)
void __fastcall kotor_fastcall_0067f650(void* self, void*) {
    live_free(lp(self, 0xC9C));                                                                 // 0067F66D..0067F67C _free
    sw(self, 0xC9C, 0);                                                                         // 0067F68A
    gui_load_label_destruct_00419d10(at(self, 0xB54), nullptr);                                 // 0067F684..0067F699
    gui_load_button_destruct_00419da0(at(self, 0x990), nullptr);                                // 0067F69E..0067F6A9
    gui_load_button_destruct_00419da0(at(self, 0x7CC), nullptr);                                // 0067F6AE..0067F6B9
    gui_load_button_destruct_00419da0(at(self, 0x5F8), nullptr);                                // 0067F6BE..0067F6C9
    gui_load_button_destruct_00419da0(at(self, 0x424), nullptr);                                // 0067F6CE..0067F6D9
    gui_load_label_destruct_00419d10(at(self, 0x2E4), nullptr);                                 // 0067F6DE..0067F6E9
    gui_load_label_destruct_00419d10(at(self, 0x1A4), nullptr);                                 // 0067F6EE..0067F6F9
    gui_load_label_destruct_00419d10(at(self, 0x064), nullptr);                                 // 0067F6FE..0067F706
    gui_load_panel_destruct_0040cf70(self, nullptr);                                            // 0067F70B..0067F715
}

// ======================= CSWGuiPazaakStart =======================
// 0067D3B0 HandleWagerExit(int wager) RET 4
void __fastcall kotor_fastcall_0067d3b0(void* self, void*, I32 wager) {
    sw(lp(self, 0x7604), 0, static_cast<U32>(wager));                                           // 0067D3B0..0067D3BA [pazaak+0]
    vcall<void, void*, U32>(self, 0x08, at(self, 0x1A4), 0u);                                  // 0067D3BC..0067D3C7 SetActiveControl(avail[0], 0)
}

// 0067D3D0 HandleQuitDialog (message-box callback) RET 4
void __fastcall kotor_fastcall_0067d3d0(void* self, void*, U32) {
    if ((lb(in_game_box(), 0x64) & 1u) == 0) return;                                            // 0067D3D0..0067D3EA
    if (frame_ContainsPanel_0040bd70(mgr_of(self), nullptr, lp(self, 0x75F4)) != 0)             // 0067D3EC..0067D3FD PanelExists
        frame_RemovePanel_0040c830(mgr_of(self), nullptr, lp(self, 0x75F4));                    // 0067D3FF..0067D409
    void* pazaak = lp(self, 0x7604);                                                            // 0067D411
    mark_delete(self);                                                                          // 0067D40E..0067D423
    kotor_fastcall_005eddd0(client_app(), nullptr, 0, 0, 0, lw(pazaak, 4));                     // 0067D426..0067D439 EndPazaakGame(0,0,0,opp)
}

// 0067D450 isOtherCardActiveControl() RET
I32 __fastcall kotor_fastcall_0067d450(void* self, void*) {
    void* active = lp(self, 0x1C);                                                              // 0067D450
    const I32 id = li(active, 0x50);                                                            // 0067D453
    if (id < 0 || id >= 0x12) return 0;                                                         // 0067D456..0067D45D
    return active == card_at(self, pz::start::kAvail, id) ? 1 : 0;                              // 0067D45F..0067D473
}

// 0067D480 isSidedeckCardActiveControl() RET
I32 __fastcall kotor_fastcall_0067d480(void* self, void*) {
    void* active = lp(self, 0x1C);                                                              // 0067D480
    const I32 id = li(active, 0x50);                                                            // 0067D483
    if (id < 0 || id >= 0x0A) return 0;                                                         // 0067D486..0067D48D
    return active == card_at(self, pz::start::kChosen, id) ? 1 : 0;                             // 0067D48F..0067D4A3
}

// 0067E2B0 ~CSWGuiPazaakStart() RET
void __fastcall kotor_fastcall_0067e2b0(void* self, void*) {
    set_vptr(self, kotor_pazaak_vtable_007532e8());                                             // 0067E2CD
    void* popup = lp(self, 0x75F4);                                                             // 0067E2D3
    if (popup != nullptr) {                                                                     // 0067E2D9..0067E2E3
        vcall<void*, U32>(popup, 0x00, 1u);                                                     // 0067E2E5..0067E2E9 scalar deleting dtor(1)
        sw(self, 0x75F4, 0);                                                                    // 0067E2EB
    }
    void* pazaak = lp(self, 0x7604);                                                            // 0067E2F5
    if (pazaak != nullptr) { live_free(pazaak); sw(self, 0x7604, 0); }                          // 0067E2FB..0067E308 _free
    kotor_fastcall_0067df60(at(self, 0x7F8C), nullptr);                                         // 0067E312..0067E31D ~CSWGuiTutorial
    gui_m48_message_box_destruct_006270a0(at(self, 0x7608), nullptr);                           // 0067E322..0067E32D
    gui_load_label_destruct_00419d10(at(self, 0x741C), nullptr);                                // 0067E332..0067E33D
    gui_load_label_destruct_00419d10(at(self, 0x72DC), nullptr);                                // 0067E342..0067E34D
    gui_load_button_destruct_00419da0(at(self, 0x7108), nullptr);                               // 0067E352..0067E35D
    gui_load_button_destruct_00419da0(at(self, 0x6F34), nullptr);                               // 0067E362..0067E36D
    vector_destruct(at(self, 0x501C), pz::card::kSize, 10, &kotor_fastcall_0067de80);           // 0067E372..0067E38A
    vector_destruct(at(self, 0x399C), 0x140, 18, &gui_load_label_destruct_00419d10);            // 0067E38F..0067E3A7
    vector_destruct(at(self, 0x01A4), pz::card::kSize, 18, &kotor_fastcall_0067de80);           // 0067E3AC..0067E3C4
    gui_load_label_destruct_00419d10(at(self, 0x64), nullptr);                                  // 0067E3C9..0067E3D1
    gui_load_panel_destruct_0040cf70(self, nullptr);                                            // 0067E3D6..0067E3E0
}

// 0067E400 ShowDisabledButtons() RET
void __fastcall kotor_fastcall_0067e400(void* self, void*) {
    vcall<void, U32>(at(self, 0x7108), 0x88, lw(self, 0x7600));                                 // 0067E404..0067E418 accept.SetEnabled(all chosen)
    void* sw_button = at(self, 0x6F34);
    void* active = lp(self, 0x1C);                                                              // 0067E41E
    if (active == nullptr) { vcall<void, U32>(sw_button, 0x88, 0u); return; }                   // 0067E421..0067E423 -> 0067E511
    const I32 id = li(active, 0x50);                                                            // 0067E429
    if (id < 0) { vcall<void, U32>(sw_button, 0x88, 0u); return; }                              // 0067E42C..0067E42E -> 0067E4F9
    if (id < 0x12 && active == card_at(self, pz::start::kAvail, id)) {                          // 0067E434..0067E453
        U32 enable = lw(self, 0x7600) == 0 ? 1u : 0u;                                           // 0067E455..0067E45F
        if (card_at(self, pz::start::kAvail, id) == active && li(self, 0x755C + static_cast<U32>(id) * 4u) < 1) // 0067E462..0067E47D
            enable = 0;                                                                         // 0067E47F..0067E485
        vcall<void, U32>(sw_button, 0x88, enable);                                              // 0067E487..0067E48E / 0067E4E2..0067E4EF
        return;
    }
    if (id < 0 || id >= 0x0A || active != card_at(self, pz::start::kChosen, id)) {             // 0067E498..0067E4BB
        vcall<void, U32>(sw_button, 0x88, 0u); return;                                          // 0067E4F9..0067E507
    }
    U32 enable = 1;                                                                             // 0067E4CE
    if (card_at(self, pz::start::kChosen, id) == active && li(self, 0x75A4 + static_cast<U32>(id) * 8u) < 0) // 0067E4BD..0067E4DE
        enable = 0;                                                                             // 0067E4E0
    vcall<void, U32>(sw_button, 0x88, enable);                                                  // 0067E4E2..0067E4EF
}

// 0067E530 OnChosenCardEntered(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_0067e530(void* self, void*, void*) {
    kotor_fastcall_0067e400(self, nullptr);                                                     // 0067E530
}

// 0067F730 scalar deleting destructor RET 4
void* __fastcall kotor_fastcall_0067f730(void* self, void*, U32 flags) {
    return deleting(self, &kotor_fastcall_0067e2b0, flags);                                     // 0067F733..0067F748
}

// 0067F750 OnPanelAdded() RET
void __fastcall kotor_fastcall_0067f750(void* self, void*) {
    if (lp(self, 0x75F4) != nullptr) {                                                          // 0067F773..0067F77C
        vcall<void, void*, U32>(self, 0x08, nullptr, 0u);                                       // 0067F77E..0067F784 SetActiveControl(0, 0)
        kotor_fastcall_0040bc70(mgr_of(self), nullptr, lp(self, 0x75F4), 1, 1);                 // 0067F787..0067F795 AddPanel(popup, 1, 1)
    }
    void* table = party_table();                                                                // 0067F79A..0067F7A3
    for (U32 i = 0; i < 18; ++i) {                                                              // 0067F7AC..0067F807
        const U32 count = lw(table, 0x120 + i * 4u);                                            // 0067F7C6
        sw(self, 0x755C + i * 4u, count);                                                       // 0067F7CB
        if (count == 0) {                                                                       // 0067F7C9..0067F7CD
            sw(self, 0x755C + i * 4u, 0xFFFFFFFFu);                                             // 0067F7D4
            kotor_fastcall_0067cd30(card_at(self, pz::start::kAvail, static_cast<I32>(i)), nullptr, -1, 0); // 0067F7CF..0067F7ED
        }
    }
    sw(self, 0x7600, 1);                                                                        // 0067F813
    for (U32 k = 0; k < 10; ++k) {                                                              // 0067F82B..0067F893
        const U32 slot = 0x75A4 + k * 8u;
        sw(self, slot, lw(table, 0x168 + k * 4u));                                              // 0067F833..0067F835
        sw(self, slot + 4, 0);                                                                  // 0067F837..0067F839
        const I32 kind = li(self, slot);                                                        // 0067F83C
        const U32 left_off = 0x755C + static_cast<U32>(kind) * 4u;
        if (kind >= 0 && li(self, left_off) > 0) {                                              // 0067F83E..0067F84B
            sw(self, left_off, lw(self, left_off) - 1u);                                        // 0067F84D..0067F84E
        } else {
            sw(self, slot, 0xFFFFFFFFu); sw(self, slot + 4, 0);                                 // 0067F857..0067F85E
        }
        kotor_fastcall_0067cd30(card_at(self, pz::start::kChosen, static_cast<I32>(k)), nullptr,
                                li(self, slot), li(self, slot + 4));                             // 0067F861..0067F86A SetCard
        if (li(self, slot) < 0) sw(self, 0x7600, 0);                                            // 0067F86F..0067F874
    }
    for (U32 i = 0; i < 18; ++i) {                                                              // 0067F895..0067F937
        const I32 left = li(self, 0x755C + i * 4u);                                            // 0067F8B0
        if (left >= 0) {                                                                        // 0067F8B2..0067F8B4
            Str s(left);                                                                        // 0067F8B6..0067F8BB
            gui_load_text_params_set_text_00415e00(at(self, 0x3A84 + i * 0x140u), nullptr, s.p()); // 0067F8C0..0067F8CB
        }                                                                                       // 0067F8DC
        if (li(self, 0x755C + i * 4u) == 0) {                                                   // 0067F8E1..0067F8E4
            void* card = card_at(self, pz::start::kAvail, static_cast<I32>(i));
            set_color3(at(card, 0x90), kColorUsedF);                                            // 0067F8E6..0067F915
            sw(card, 0x314, 0);                                                                 // 0067F918
        }
    }
    for (I32 i = 1; i <= 10; ++i) {                                                             // 0067F93D..0067F9DE (ebx = chosen[i-1])
        void* card = card_at(self, pz::start::kChosen, i - 1);
        gui_load_nav_set_move_to_00417660(card, nullptr, 1, card_at(self, pz::start::kChosen, (i + 8) % 10)); // 0067F950..0067F96D
        gui_load_nav_set_move_to_00417660(card, nullptr, 3, card_at(self, pz::start::kChosen, i % 10));       // 0067F972..0067F98E
        gui_load_nav_set_move_to_00417660(card, nullptr, 0, card_at(self, pz::start::kChosen, (i + 7) % 10)); // 0067F993..0067F9B0
        gui_load_nav_set_move_to_00417660(card, nullptr, 2, card_at(self, pz::start::kChosen, (i + 1) % 10)); // 0067F9B5..0067F9D2
    }
    for (I32 e = 6; e - 6 < 0x12; ++e) {                                                        // 0067F9E4..0067FA9E (ebx = avail[e-6])
        void* card = card_at(self, pz::start::kAvail, e - 6);
        gui_load_nav_set_move_to_00417660(card, nullptr, 1, card_at(self, pz::start::kAvail, (e + 6) % 18));  // 0067F9F0..0067FA0D
        gui_load_nav_set_move_to_00417660(card, nullptr, 3, card_at(self, pz::start::kAvail, e % 18));        // 0067FA12..0067FA2E
        const I32 row6 = ((e - 6) / 6) * 6;                                                     // 0067FA33..0067FA52 (magic /6, *3, *2)
        gui_load_nav_set_move_to_00417660(card, nullptr, 0, card_at(self, pz::start::kAvail, (e - 1) % 6 + row6)); // 0067FA47..0067FA68
        gui_load_nav_set_move_to_00417660(card, nullptr, 2, card_at(self, pz::start::kAvail, (e - 5) % 6 + row6)); // 0067FA6D..0067FA8C
    }
    if (lp(self, 0x75F4) == nullptr)                                                            // 0067FAA4..0067FAAC
        vcall<void, void*, U32>(self, 0x08, at(self, 0x1A4), 0u);                               // 0067FAAE..0067FABB SetActiveControl(avail[0], 0)
    kotor_fastcall_0067e400(self, nullptr);                                                     // 0067FABE..0067FAC0 ShowDisabledButtons
    const U32 help[3] = {0x96E3u, 0x96E4u, 0x96E5u};                                            // 0067FAD2..0067FAE2 [esp+24..2C]
    kotor_fastcall_0067e0b0(at(self, 0x7F8C), nullptr, help, 3);                                // 0067FAC5..0067FAEA ShowMultipleHelp
    gui_load_panel_on_added_0040b870(self, nullptr);                                            // 0067FAEF..0067FAF1 CSWGuiPanel::OnPanelAdded
}

// 0067FB10 AddChosenCard(int kind, int slot) RET 8 -> 1 added / 0
I32 __fastcall kotor_fastcall_0067fb10(void* self, void*, I32 kind, I32 slot) {
    const U32 left_off = 0x755C + static_cast<U32>(kind) * 4u;
    if (li(self, left_off) < 1) return 0;                                                       // 0067FB30..0067FB4C
    if (slot < 0) {                                                                             // 0067FB54..0067FB56
        slot = 0;                                                                               // 0067FB58
        while (li(self, 0x75A4 + static_cast<U32>(slot) * 8u) >= 0) {                           // 0067FB60..0067FB63
            if (++slot >= 10) return 0;                                                         // 0067FB65..0067FB81
        }
    }
    if (slot >= 10) return 0;                                                                   // 0067FB84..0067FB9C
    sw(self, 0x75A8 + static_cast<U32>(slot) * 8u, 0);                                          // 0067FBA2
    sw(self, 0x75A4 + static_cast<U32>(slot) * 8u, static_cast<U32>(kind));                     // 0067FBA9
    const I32 left = li(self, left_off) - 1;                                                    // 0067FBB0..0067FBB7
    sw(self, left_off, static_cast<U32>(left));                                                 // 0067FBBF
    {
        Str s(left);                                                                            // 0067FBBA..0067FBC6
        gui_load_text_params_set_text_00415e00(at(self, 0x3A84 + static_cast<U32>(kind) * 0x140u), nullptr, s.p()); // 0067FBCB..0067FBE5
    }                                                                                           // 0067FBF6
    void* source = card_at(self, pz::start::kAvail, kind);                                      // ebp + 1A4
    void* chosen = card_at(self, pz::start::kChosen, slot);                                     // edi + 501C
    U8 fill[16]; std::memcpy(fill, at(source, 0xC0), 16);                                       // 0067FC0C..0067FC38 CResRef by value
    gui_load_border_params_set_fill_00414c00(at(chosen, 0x80), nullptr, fill, 0);               // 0067FC25..0067FC3C
    if (li(self, left_off) == 0) {                                                              // 0067FC41..0067FC4A
        set_color3(at(source, 0x90), kColorUsedF);                                              // 0067FC4C..0067FC7B
        sw(source, 0x314, 0);                                                                   // 0067FC7E
    }
    Str text; WindowCopy005e5b00(text.p(), nullptr, at(source, 0x2AC)); text.live = true;       // 0067FC88..0067FC93 CExoString(const&)
    gui_load_text_params_set_text_00415e00(at(chosen, 0x2AC), nullptr, text.p());              // 0067FC98..0067FCAC
    sw(self, 0x7600, 1);                                                                        // 0067FCB1
    for (U32 k = 0; k < 10; ++k)                                                                // 0067FCB7..0067FCCC
        if (li(self, 0x75A4 + k * 8u) < 0) { sw(self, 0x7600, 0); break; }                      // 0067FCC0..0067FCD0
    kotor_fastcall_0067e400(self, nullptr);                                                     // 0067FCDA..0067FCDC
    return 1;                                                                                   // 0067FCE1..0067FCF8 (~CExoString)
}

// 0067FD10 RemoveChosenCard(int slot) RET 4 -> 1 removed / 0
I32 __fastcall kotor_fastcall_0067fd10(void* self, void*, I32 slot) {
    const U32 slot_off = 0x75A4 + static_cast<U32>(slot) * 8u;
    const I32 kind = li(self, slot_off);                                                        // 0067FD31
    if (kind < 0) return 0;                                                                     // 0067FD38..0067FD4F
    sw(self, 0x7600, 0);                                                                        // 0067FD54
    kotor_fastcall_0067e400(self, nullptr);                                                     // 0067FD52..0067FD5E ShowDisabledButtons
    sw(self, slot_off, 0xFFFFFFFFu); sw(self, slot_off + 4, 0);                                 // 0067FD63..0067FD6F
    const U32 left_off = 0x755C + static_cast<U32>(kind) * 4u;
    const I32 left = li(self, left_off) + 1;                                                    // 0067FD76..0067FD7D
    sw(self, left_off, static_cast<U32>(left));                                                 // 0067FD83
    {
        Str s(left);                                                                            // 0067FD7E..0067FD8A
        gui_load_text_params_set_text_00415e00(at(self, 0x3A84 + static_cast<U32>(kind) * 0x140u), nullptr, s.p()); // 0067FD8F..0067FDA9
    }                                                                                           // 0067FDBA
    void* source = card_at(self, pz::start::kAvail, kind);                                      // eax + 1A4
    set_color3(at(source, 0x90), kOneF);                                                        // 0067FDC8..0067FDF7
    sw(source, 0x314, 1);                                                                       // 0067FDFA..0067FE08
    void* chosen = card_at(self, pz::start::kChosen, slot);
    menu_resources::Ref ref; ref.construct_00406d80(k_empty_0073d71c);                          // 0067FDFF..0067FE0E CResRef("")
    gui_load_border_params_set_fill_00414c00(at(chosen, 0x80), nullptr, &ref, 0);               // 0067FE13..0067FE28
    {
        Str empty(k_empty_0073d71c);                                                            // 0067FE2D..0067FE36
        gui_load_text_params_set_text_00415e00(at(chosen, 0x2AC), nullptr, empty.p());          // 0067FE3B..0067FE4A
    }                                                                                           // 0067FE5B
    return 1;
}

// 0067FE80 HandleAvailCardDrop(CSWGuiControl* dragged) RET 4
void __fastcall kotor_fastcall_0067fe80(void* self, void*, void* control) {
    const I32 kind = li(control, 0x50);                                                         // 0067FE80..0067FE85
    void* target = lp(gui_manager(), 0x08);                                                     // 0067FE8B..0067FE91
    if (target == nullptr) return;                                                              // 0067FE94..0067FE96
    const I32 id = li(target, 0x50);                                                            // 0067FE99
    if (id < 0 || id >= 0x0A || target != card_at(self, pz::start::kChosen, id)) return;        // 0067FE9C..0067FEB6
    kotor_fastcall_0067fd10(self, nullptr, id);                                                 // 0067FEB8..0067FEBB
    kotor_fastcall_0067fb10(self, nullptr, kind, id);                                           // 0067FEC0..0067FEC4
}

// 0067FED0 HandleChosenCardDrop(CSWGuiControl* dragged) RET 4
void __fastcall kotor_fastcall_0067fed0(void* self, void*, void* control) {
    const I32 from = li(control, 0x50);                                                         // 0067FED0..0067FEDB
    const I32 moved = li(self, 0x75A4 + static_cast<U32>(from) * 8u);                           // 0067FEE6..0067FEED [esp+0C]
    void* target = lp(gui_manager(), 0x08);                                                     // 0067FED4..0067FEDE
    if (target == nullptr) return;                                                              // 0067FEE1..0067FEF1
    const I32 id = li(target, 0x50);                                                            // 0067FEF9
    if (id < 0) return;                                                                         // 0067FEFC..0067FEFE
    U32 mode;
    if (id < 0x0A && target == card_at(self, pz::start::kChosen, id)) mode = 2;                 // 0067FF00..0067FF18
    else if (id < 0x12 && target == card_at(self, pz::start::kAvail, id)) mode = 1;             // 0067FF1F..0067FF37
    else return;
    kotor_fastcall_0067fd10(self, nullptr, from);                                               // 0067FF3C..0067FF3F
    if (mode != 2) return;                                                                      // 0067FF44..0067FF47
    const I32 other = li(self, 0x75A4 + static_cast<U32>(id) * 8u);                             // 0067FF49
    kotor_fastcall_0067fd10(self, nullptr, id);                                                 // 0067FF50..0067FF53
    if (other >= 0) kotor_fastcall_0067fb10(self, nullptr, other, from);                        // 0067FF58..0067FF60
    if (moved >= 0) kotor_fastcall_0067fb10(self, nullptr, moved, id);                          // 0067FF65..0067FF71
}

// 006807E0 HandleMoveCard(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_006807e0(void* self, void*, void* control) {
    if (control == nullptr || lw(control, 0x4C) == 0) return;                                   // 006807E4..006807F4
    const I32 id = li(control, 0x50);                                                           // 006807FE
    kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                          // 006807FA..00680804
    void* active = lp(self, 0x1C);                                                              // 00680809
    const I32 active_id = li(active, 0x50);                                                     // 0068080C
    I32 done = -1;                                                                              // -1: no add/remove attempted
    if (active_id >= 0 && active_id < 0x0A && active == card_at(self, pz::start::kChosen, active_id)) { // 0068080F..00680830
        if (card_at(self, pz::start::kChosen, id) == active)                                    // 00680832..00680843
            done = kotor_fastcall_0067fd10(self, nullptr, id);                                  // 00680845..00680848
    } else if (kotor_fastcall_0067d450(self, nullptr) != 0                                      // 0068084F..00680858
               && card_at(self, pz::start::kAvail, id) == active) {                             // 0068085A..0068086B
        done = kotor_fastcall_0067fb10(self, nullptr, id, -1);                                  // 0068086D..00680872
    }
    if (done > 0) kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                            // 00680877..00680880
    kotor_fastcall_0067e400(self, nullptr);                                                     // 00680885..00680887
}

// 006816F0 HandleInputEvent(int event, int down) RET 8
void __fastcall kotor_fastcall_006816f0(void* self, void*, I32 event, I32 down) {
    if (down != 0) {                                                                            // 006816FA..006816FF
        I32 code = event;                                                                       // 0068170B
        if (event == 0xBA)                                                                      // 00681705..0068170D
            code = kotor_fastcall_0067d480(self, nullptr) != 0 ? 0x35 : 0x36;                   // 0068170F..0068173D (inlined)
        // Jump table 00681864 (0068175C quit, 006817BC move, 006817D6 to-available, 0068180B to-chosen, 00681853 default),
        // index bytes 00681878 for code - 0x28 in 0..0x0E (unsigned).
        static constexpr U8 k_index_00681878[15] = {0, 4, 1, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 2, 3};
        const U32 index = static_cast<U32>(code) - 0x28u;                                       // 00681742
        const U8 target = index <= 0x0Eu ? k_index_00681878[index] : 4;                         // 00681745..00681755
        switch (target) {
        case 0: {                                                                               // 0068175C quit
            kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                  // 0068175C..00681762
            void* box = in_game_box();                                                          // 00681767..00681775
            kotor_fastcall_00627130(box, nullptr, 1);                                           // 0068177B..0068177F
            gui_load_messagebox_set_callback_00624a40(box, nullptr, self, ad(fn(&kotor_fastcall_0067d3d0)), 0); // 00681784..0068178E
            vcall<void, U32>(box, 0x6C, 0xA5B8u);                                               // 00681793..0068179C
            kotor_fastcall_0040bc70(mgr_of(self), nullptr, box, 1, 1);                          // 0068179F..006817A7
            break;
        }
        case 1:                                                                                 // 006817BC
            kotor_fastcall_006807e0(self, nullptr, lp(self, 0x1C));                             // 006817BC..006817C2 HandleMoveCard(active)
            break;
        case 2:                                                                                 // 006817D6 switch to the available cards
            if (kotor_fastcall_0067d480(self, nullptr) != 0) sw(self, 0x75F8, lw(self, 0x1C));  // 006817D6..006817E4
            if (kotor_fastcall_0067d450(self, nullptr) == 0) {                                  // 006817EA..006817F3
                vcall<void, void*, U32>(self, 0x08, lp(self, 0x75FC), 0u);                      // 006817F5..00681801
                gui_load_text_params_set_strref_00415e50(at(self, 0x70A0), nullptr, 0x96C9u);   // 00681804..00681844
                kotor_fastcall_0040a140(mgr_of(self), nullptr, 1);                              // 00681849..0068184E
            }
            break;
        case 3:                                                                                 // 0068180B switch to the chosen cards
            if (kotor_fastcall_0067d450(self, nullptr) != 0) sw(self, 0x75FC, lw(self, 0x1C));  // 0068180B..00681819
            if (kotor_fastcall_0067d480(self, nullptr) == 0) {                                  // 0068181F..00681828
                vcall<void, void*, U32>(self, 0x08, lp(self, 0x75F8), 0u);                      // 0068182A..00681836
                gui_load_text_params_set_strref_00415e50(at(self, 0x70A0), nullptr, 0x96CAu);   // 00681839..00681844
                kotor_fastcall_0040a140(mgr_of(self), nullptr, 1);                              // 00681849..0068184E
            }
            break;
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, static_cast<U32>(event), static_cast<U32>(down)); // 00681853..00681857
}

// 00681890 HandleStartDialog (message-box callback) RET 4
void __fastcall kotor_fastcall_00681890(void* self, void*, U32) {
    if ((lb(self, 0x766C) & 1u) == 0) return;                                                   // 006818A9..006818B0 [box+64h] bit 0
    void* table = party_table();                                                                // 006818B6..006818BE
    for (U32 k = 0; k < 10; ++k) sw(table, 0x168 + k * 4u, lw(self, 0x75A4 + k * 8u));         // 006818C3..0068193B PT_PAZSIDELIST
    kotor_fastcall_006e4580(at(lp(self, 0x7604), pz::core::kPlayer), nullptr, at(self, 0x75A4)); // 00681941..0068194B ChooseSidedeck
    kotor_fastcall_0040be00(mgr_of(self), nullptr);                                             // 00681950..00681953 PopModalPanel
    mark_delete(self);                                                                          // 00681958..0068196A
    void* game = nullptr;
    if (void* raw = live_operator_new(pz::game::kSize)) {                                       // 00681965..00681983 operator new(0x86E8)
        auto g = on_throw([&]() { live_free(raw); });                                           // state 0
        void* pazaak = lp(self, 0x7604);
        game = kotor_fastcall_006808a0(raw, nullptr, mgr_of(self), pazaak, li(self, 0x8920), lw(pazaak, 4)); // 00681985..0068199E
        g.armed = false;
    }
    sw(self, 0x7604, 0);                                                                        // 006819B8
    kotor_fastcall_0040bc70(mgr_of(self), nullptr, game, 3, 1);                                 // 006819A8..006819C2 AddPanel(game, 3, 1)
}

// 006819E0 HandleAcceptButton(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_006819e0(void* self, void*, void* control) {
    if (control == nullptr || lw(control, 0x4C) == 0) return;                                   // 006819E4..006819F4
    kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                          // 006819FA..00681A00
    if (lw(self, 0x7600) != 0) {                                                                // 00681A05..00681A0D
        void* box = at(self, 0x7608);
        vcall<void, U32>(box, 0x6C, 0x7E42u);                                                   // 00681A0F..00681A22
        gui_load_messagebox_set_callback_00624a40(box, nullptr, self, ad(fn(&kotor_fastcall_00681890)), 0); // 00681A25..00681A2F
        kotor_fastcall_0040bc70(mgr_of(self), nullptr, box, 1, 1);                              // 00681A34..00681A3C
        return;
    }
    void* box = in_game_box();                                                                  // 00681A46..00681A54
    kotor_fastcall_00627130(box, nullptr, 0);                                                   // 00681A5A..00681A5E SetAllowCancel(0)
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, 0, 0);                     // 00681A63..00681A6B
    vcall<void, U32>(box, 0x6C, 0x96E6u);                                                       // 00681A70..00681A79
    kotor_fastcall_0040bc70(mgr_of(self), nullptr, box, 1, 1);                                  // 00681A7C..00681A84
}

// 00681A90 CSWGuiPazaakStart(CSWGuiManager*, int max_wager, int deck, int tutorial, OBJECT_ID opponent) RET 14h
void* __fastcall kotor_fastcall_00681a90(void* self, void*, void* manager, I32 max_wager, I32 deck, I32 tutorial,
                                         U32 opponent) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                  // 00681AB9..00681AC1
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                  // state 0
    set_vptr(self, kotor_pazaak_vtable_007532e8());                                             // 00681AD2 (007532E8)
    label(self, 0x64, u);                                                                       // 00681AC8..00681AD8 state 1
    vector_member(at(self, 0x01A4), pz::card::kSize, 18, &kotor_fastcall_0067ddd0, &kotor_fastcall_0067de80, u); // 00681ADD..00681AFD
    vector_member(at(self, 0x399C), 0x140, 18, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 00681B02..00681B22
    vector_member(at(self, 0x501C), pz::card::kSize, 10, &kotor_fastcall_0067ddd0, &kotor_fastcall_0067de80, u); // 00681B27..00681B47
    activated(self, 0x6F34, u);                                                                 // 00681B4C..00681B89 state 5
    activated(self, 0x7108, u);                                                                 // 00681B8C..00681BC9 state 6
    label(self, 0x72DC, u); label(self, 0x741C, u);                                             // 00681BCC..00681BED states 7, 8
    for (U32 k = 0; k < 10; ++k) sw(self, 0x75A8 + k * 8u, 0);                                  // 00681BFA..00681C0B
    kotor_original::kotor_fastcall_00626df0(at(self, 0x7608), nullptr, manager);                // 00681C0D..00681C17
    u.push(&gui_m48_message_box_destruct_006270a0, at(self, 0x7608));                           // state 9
    kotor_fastcall_0067def0(at(self, 0x7F8C), nullptr, manager);                                // 00681C1C..00681C2B
    u.push(&kotor_fastcall_0067df60, at(self, 0x7F8C));                                         // state 0Ah
    sw(self, 0x75F4, 0); sw(self, 0x7600, 0);                                                   // 00681C30..00681C36
    sw(self, 0x75F8, ad(at(self, 0x501C))); sw(self, 0x75FC, ad(at(self, 0x1A4)));              // 00681C3C..00681C4E
    for (U32 i = 0; i < 18; ++i) sw(self, 0x755C + i * 4u, 0);                                  // 00681C54..00681C64 rep stosd
    for (U32 k = 0; k < 10; ++k) { sw(self, 0x75A4 + k * 8u, 0xFFFFFFFFu); sw(self, 0x75A8 + k * 8u, 0); } // 00681C66..00681CF6
    sw(self, 0x8920, static_cast<U32>(tutorial));                                               // 00681D09 (tutorial +994h)
    void* pazaak = nullptr;
    if (void* raw = live_operator_new(pz::core::kSize)) {                                       // 00681CFC..00681D25 operator new(0x230)
        auto g = on_throw([&]() { live_free(raw); });                                           // state 0Bh
        pazaak = kotor_fastcall_006e4c00(raw, nullptr);                                         // 00681D27..00681D29 CSWPazaak()
        g.armed = false;
    }
    sw(self, 0x7604, ad(pazaak));                                                               // 00681D3A
    raw_store(pazaak, 0x0, static_cast<U32>(max_wager));                                        // 00681D40 (no null check)
    raw_store(lp(self, 0x7604), 0x4, opponent);                                                 // 00681D42..00681D50
    if (max_wager > 0) {                                                                        // 00681D35 cmp / 00681D53 jle
        raw_store(lp(self, 0x7604), 0x0, 1);                                                    // 00681D55..00681D60
        void* popup = nullptr;
        if (void* raw = live_operator_new(pz::wager::kSize)) {                                  // 00681D5B..00681D7C operator new(0xCAC)
            auto g = on_throw([&]() { live_free(raw); });                                       // state 0Ch
            popup = kotor_fastcall_0067f000(raw, nullptr, manager, self, max_wager);            // 00681D7E..00681D86
            g.armed = false;
        }
        sw(self, 0x75F4, ad(popup));                                                            // 00681D97
    }
    {
        U32 opponent_cards[20];                                                                 // [esp+98] card[10]
        for (U32 k = 0; k < 10; ++k) opponent_cards[k * 2 + 1] = 0;                            // 00681D9D..00681DB6 flags only
        kotor_fastcall_006e4a40(lp(self, 0x7604), nullptr, deck, opponent_cards);               // 00681DB8..00681DCA LoadInitialSideDeck
        kotor_fastcall_006e4580(at(lp(self, 0x7604), pz::core::kNpc), nullptr, opponent_cards); // 00681DCF..00681DE0 ChooseSidedeck
    }
    start_layout(self, "pazaaksetup");                                                          // 00681DE5..00681E00 (007537C4)
    init_control(self, 0x0064, "LBL_TITLE");                                                    // 00681E05..00681E39 (00751A20)
    init_control(self, 0x7108, "BTN_ATEXT");                                                    // 00681E3E..00681E75 (007537B8)
    init_control(self, 0x72DC, "LBL_LTEXT");                                                    // 00681E7A..00681EB1 (007537AC)
    init_control(self, 0x741C, "LBL_RTEXT");                                                    // 00681EB6..00681EED (007537A0)
    char btn_chosen[16]; std::memcpy(btn_chosen, k_btn_chosen_00753790, 16);                   // 00681EF2..00681F3B [esp+2C]
    char lbl_chosen[16]; std::memcpy(lbl_chosen, k_lbl_chosen_00753780, 16);                   // [esp+3C]
    for (I32 i = 0; i < 10; ++i) {                                                              // 00681F45..00682017
        void* card = card_at(self, pz::start::kChosen, i);
        std::sprintf(btn_chosen + 10, k_int_0073d720, i);                                       // 00681F45..00681F50
        std::sprintf(lbl_chosen + 10, k_int_0073d720, i);                                       // 00681F55..00681F60
        init_control(self, pz::start::kChosen + static_cast<U32>(i) * pz::card::kSize, btn_chosen); // 00681F68..00681F99
        init_control(self, pz::start::kChosen + static_cast<U32>(i) * pz::card::kSize + 0x1C4, lbl_chosen); // 00681F9E..00681FD5
        sw(card, 0x50, static_cast<U32>(i));                                                    // 00681FE4
        add_event(self, pz::start::kChosen + static_cast<U32>(i) * pz::card::kSize, 0, self, fn(&kotor_fastcall_0067e530)); // 00681FDA..00681FE7
        add_event(self, pz::start::kChosen + static_cast<U32>(i) * pz::card::kSize, 0x17FEB, self, fn(&kotor_fastcall_0067fed0)); // 00681FEC..00681FF9
        add_event(self, pz::start::kChosen + static_cast<U32>(i) * pz::card::kSize, 0x27, self, fn(&kotor_fastcall_006807e0)); // 00681FFE..00682008
    }
    char btn_avail[16]; std::memcpy(btn_avail, k_btn_avail_00753770, 16);                      // 0068201D..00682096 [esp+54]
    char lbl_avail[16]; std::memcpy(lbl_avail, k_lbl_avail_00753760, 16);                      // [esp+74]
    char lbl_num[16]; std::memcpy(lbl_num, k_lbl_availnum_00753750, 16);                        // [esp+64]
    for (I32 i = 0; i < 18; ++i) {                                                              // 0068209A..00682209
        const I32 row = i / 6, col = i % 6;                                                     // 006820A6..006820AE
        const U32 off = pz::start::kAvail + static_cast<U32>(i) * pz::card::kSize;
        const U32 num = pz::start::kAvailNum + static_cast<U32>(i) * 0x140u;
        std::sprintf(btn_avail + 9, k_int_int_0073d5b4, row, col);                              // 006820B0..006820C4
        std::sprintf(lbl_avail + 9, k_int_int_0073d5b4, row, col);                              // 006820C9..006820E0
        std::sprintf(lbl_num + 12, k_int_int_0073d5b4, row, col);                               // 006820E5..006820FC
        init_control(self, off, btn_avail);                                                     // 00682104..00682135
        init_control(self, off + 0x1C4, lbl_avail);                                             // 0068213A..00682171
        init_control(self, num, lbl_num);                                                       // 00682176..006821B7
        sw(at(self, off), 0x50, static_cast<U32>(i));                                           // 006821C2
        add_event(self, off, 0, self, fn(&kotor_fastcall_0067e530));                            // 006821BC..006821C9
        add_event(self, off, 0x17FEB, self, fn(&kotor_fastcall_0067fe80));                      // 006821CE..006821DB
        add_event(self, off, 0x27, self, fn(&kotor_fastcall_006807e0));                         // 006821E0..006821EA
        sw(self, num + 0x44, lw(self, num + 0x44) | 0x20u);                                     // 006821EF..006821F3
    }
    stop_layout(self);                                                                          // 0068220F..00682211
    add_event(self, 0x6F34, 0x27, self, fn(&kotor_fastcall_006807e0));                          // 00682216..00682226
    add_event(self, 0x7108, 0x27, self, fn(&kotor_fastcall_006819e0));                          // 0068222B..00682239
    sw(self, 0x6F34 + 0x44, lw(self, 0x6F34 + 0x44) & 0xFFFFFFFBu);                             // 0068223E..0068224B
    sb(self, 0x48, 2);                                                                          // 0068224F
    u.dismiss();
    return self;
}
}  // extern "C"

// ======================= method tables (original diff against CSWGuiPanel 0073E010; gui_vtables.cpp pattern) ===========
namespace {
const Override ovr_start[] = {
    {0x00, fn(&kotor_fastcall_0067f730)},   // 0067F730 scalar deleting dtor
    {0x3C, fn(&kotor_fastcall_006816f0)},   // 006816F0 HandleInputEvent
    {0x48, fn(&kotor_fastcall_0067f750)},   // 0067F750 OnPanelAdded
};
const Override ovr_wager[] = {
    {0x00, fn(&kotor_fastcall_0067f630)},   // 0067F630 scalar deleting dtor
    {0x34, fn(&kotor_fastcall_006c00b0)},   // 006C00B0 (existing, jmp CSWGuiPanel::Draw)
    {0x3C, fn(&kotor_fastcall_0067e150)},   // 0067E150 HandleInputEvent
};
void* table_start[27]; bool built_start = false;
void* table_wager[27]; bool built_wager = false;
}  // namespace
extern "C" {
const void* __cdecl kotor_pazaak_vtable_007532e8() {        // CSWGuiPazaakStart (27 slots; base 0073E010)
    if (!built_start) { vt_build(table_start, 27, gui_load_panel_methods_0073e010, 27, ovr_start, 3); built_start = true; }
    return table_start;
}
const void* __cdecl kotor_pazaak_vtable_007534c8() {        // CSWGuiWagerPopup (27 slots; base 0073E010)
    if (!built_wager) { vt_build(table_wager, 27, gui_load_panel_methods_0073e010, 27, ovr_wager, 3); built_wager = true; }
    return table_wager;
}
}  // extern "C"
