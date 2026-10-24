// Lane PZ-C (Pazaak, tier a1): CSWGuiPazaakGame (0x86E8, method table 00753358) and CSWGuiManager::IsOnTop.
// Transcribed from the original GOG 1.03 swkotor.exe disassembly; every statement carries its original PCs.
//
//   0040ADF0 CSWGuiManager::IsOnTop(CSWGuiPanel*)                thiscall RET 4
//   0067D4B0 RefreshDisplay                                      thiscall RET
//   0067DA30 ReconnectSidedeckButtons                            thiscall RET
//   0067DB50 HandleQuitDialog(user)          (message-box cb)    thiscall RET 4
//   0067DBB0 SetHandEnabled(int)                                 thiscall RET 4
//   0067DC90 HandleXButton(control) / 0067DCC0 HandleBlackButton thiscall RET 4 (tail jump into SetHandEnabled)
//   0067DCF0 HandleFlipHandCard                                  thiscall RET
//   0067DD80 HandleHandCardRightClick(control) (event 0x44)      thiscall RET 4
//   0067DDA0 HandleFlipButtons(control)        (event 0x27)      thiscall RET 4
//   0067E540 ~CSWGuiPazaakGame / 0067FF80 scalar deleting dtor   thiscall RET / RET 4 (slot +00)
//   0067E770 StartNewGame                                        thiscall RET
//   0067E8F0 HandleInputEvent(event, down)                       thiscall RET 8 (slot +3C)
//   0067E9A0 HandleEndDialog(user)           (message-box cb)    thiscall RET 4
//   0067EA10 HandleTutorialClose(user)       (tutorial cb)       thiscall RET 4 (index bytes 0067EC08 -> table 0067EBEC)
//   0067EC20 HandleContinue / 0067ED00 HandleStand (event 0x27)  thiscall RET 4
//   0067EDE0 HandlePlayHandCard(int hand index)                  thiscall RET 4
//   0067EF00 HandleHandCardDrop (0x17FEB) / 0067EF80 HandleHandCardDoubleClick (0x1F9) thiscall RET 4
//   0067FFA0 OnPanelAdded                                        thiscall RET (slot +48)
//   00680030 DoGameSequence                                      thiscall RET (tables 006806C8 state, 006806FC AI result)
//   00680710 Draw(float)                                         thiscall RET 4 (slot +34)
//   00680790 HandleHandcardAction(control)   (event 0x27)        thiscall RET 4
//   006808A0 CSWGuiPazaakGame(manager, CSWPazaak*, tutorial, opponent) thiscall RET 10h
// Data owned here: method table 00753358, colour 007A23CC (file-initialised .data, no writer), bss flag 00833BB0.
//
// Field meanings (derived from the bodies below):
//   +0x1C   CSWGuiPanel active control         +0x18 CSWGuiManager*        +0x44 panel flags (0x400 = delete)
//   +0x86B4 show tutorials (= tutorial +0x994) +0x86BC const char* sounds[4] = mgs_startturn/drawmain/playside/warnbust
//   +0x86CC const char* pending end-of-set sound (0 = none)                 +0x86D0 CSWPazaak* (owned, freed by dtor)
//   +0x86D4 sequence state 0..12               +0x86D8 float delay timer (<0 = run sequence)
//   +0x86DC tutorial-shown bits (2 first draw, 4 opponent turn, 8 intro, 0x10 twenty)
//   +0x86E0 hand index waiting for the tutorial confirmation (-1 none)
//   +0x86E4 hand mode: 0 none, 1 play card (X button 6FF4), 2 flip card (black button 71C8)
//   buttons: +6C4C BTN_XTEXT (end turn, HandleContinue), +6E20 BTN_YTEXT (stand), +6FF4 play-card, +71C8 flip
//
// Quirks kept (issue #23 list in INTEGRATION.md): OnPanelAdded reads the player creature's stats without a null check;
// HandleHandcardAction reads control+0x50 through a null control when the hand mode is 1; HandleFlipButtons has no
// null check; DoGameSequence state 10 is a deliberate no-op; ReconnectSidedeckButtons links the hand cards in a ring.
#include "0fa9be42e331-claude_gui_char.hpp"
#include "602f99a6ff9a-claude_pazaak_decls.hpp"

namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; }   // [007A39F4] CSWGuiManager* (0392)

extern "C" {
void __fastcall gui_load_nav_set_move_to_00417660(void* nav, void*, w4::U32 direction, void* control); // RET 8
void __fastcall gui_load_text_params_set_alignment_00415f20(void* params, void*, w4::U32 alignment);  // RET 4
void* __fastcall kotor_fastcall_005a4c60(void* stats, void*, void* out);                    // CSWSCreatureStats::GetFullName RET 4
w4::U32 __fastcall w5_sound_play_one_shot_005d5e00(void* sound, void*, void* ref, w4::U32 group, w4::U32 delay,
                                                   w4::U32 volume, float fixed_variance, float pitch_variance); // RET 18h
int __cdecl format_006fadb0(char*, const char*, ...);                                       // sprintf
extern const unsigned char kotor_m15d_color_007a23d8[12];                                   // [007A23D8] (.74,.11,0) existing (0827)

// ---- data owned by PZ-C
// [007A23CC] Vector {0.4f, 1.0f, 1.0f}: file-initialised .data (raw bytes cd cc cc 3e 00 00 80 3f 00 00 80 3f); the
// original has exactly five readers (0067DD3A, 0067EC75, 0067ED47, 0067EE29, 00680154) and no writer.
alignas(4) extern const unsigned char kotor_pazaak_color_007a23cc[12] = {
    0xCD, 0xCC, 0xCC, 0x3E, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F};
// [00833BB0] .bss int: nonzero shows the opponent's hand. One reader (0067D79A), no writer in the original.
volatile w4::I32 kotor_cell_pazaak_show_npc_hand_00833bb0 = 0;
}

using namespace gchar;

namespace {
// .rdata 00753740/30/20/10 (sound resrefs stored as char* at +86BC..+86C8) and 0075355C/50/40/34 (+86CC).
const char k_mgs_startturn_00753740[] = "mgs_startturn";
const char k_mgs_drawmain_00753730[] = "mgs_drawmain";
const char k_mgs_playside_00753720[] = "mgs_playside";
const char k_mgs_warnbust_00753710[] = "mgs_warnbust";
const char k_mgs_winmatch_0075355c[] = "mgs_winmatch";
const char k_mgs_winset_00753550[] = "mgs_winset";
const char k_mgs_losematch_00753540[] = "mgs_losematch";
const char k_mgs_loseset_00753534[] = "mgs_loseset";

constexpr U32 kGrey = 0x3F2AC083u;   // 0.667f immediate (RefreshDisplay)
constexpr U32 kOne = 0x3F800000u;    // 1.0f
constexpr U32 kDelayShort = 0x3ECCCCCDu;   // 0.4f
constexpr U32 kDelayLong = 0x3F4CCCCDu;    // 0.8f
constexpr U32 kCard = 0x31C, kLabel = 0x140, kButton = 0x1C4;

inline void* core_of(void* self) { return lp(self, 0x86D0); }
inline void* tutorial(void* self) { return at(self, 0x7D20); }
inline void* box(void* self) { return at(self, 0x739C); }
inline void* player_board(void* self, U32 i) { return at(self, 0x11E4 + i * kCard); }
inline void* player_hand(void* self, U32 i) { return at(self, 0x2DE0 + i * kCard); }
inline void* npc_board(void* self, U32 i) { return at(self, 0x3A50 + i * kCard); }
inline void* npc_hand(void* self, U32 i) { return at(self, 0x564C + i * kCard); }
inline I32 player_total(void* self) { return kotor_fastcall_006e4360(at(core_of(self), 0x08), nullptr); }
inline I32 npc_total(void* self) { return kotor_fastcall_006e4360(at(core_of(self), 0x78), nullptr); }
inline void set_enabled(void* control, U32 enabled) { vcall<void, U32>(control, 0x88, enabled); }
inline void set_active(void* self, void* control) { vcall<void, void*, U32>(self, 0x08, control, 0); }
inline void fill3(void* dst, U32 bits) { sw(dst, 0, bits); sw(dst, 4, bits); sw(dst, 8, bits); }
inline void delete_flag(void* self) { sw(self, 0x44, (lw(self, 0x44) & 0xFFFFFCFFu) | 0x400u); }
inline void show_help(void* self, U32 message, I32 a, U32 b) { kotor_fastcall_0067dfc0(tutorial(self), nullptr, message, a, b); }
// CResRef(name) on the stack + CExoSound::PlayOneShotSound([007A39EC], &ref, 0xB, 0, 0, 0, 0).
void one_shot(const char* name) {
    menu_resources::Ref ref; ref.construct_00406d80(name);
    (void)w5_sound_play_one_shot_005d5e00(pt(frame_control::sound_manager_007a39ec), nullptr, &ref, 0xB, 0, 0, 0.0f, 0.0f);
}
inline const char* sound(void* self, U32 k) { return static_cast<const char*>(lp(self, 0x86BC + 4 * k)); }
// EndPazaakGame through CClientExoApp ([[007A39FC]+4]): won = sets(player) > sets(npc) (setg), wager, 0, opponent.
void end_game(void* self, U32 won_flag) {
    void* p = core_of(self);
    kotor_fastcall_005eddd0(client_app(), nullptr, static_cast<I32>(won_flag), li(p, 0x00), 0, lw(p, 0x04));
}
// 0067CB10 CSWPazaak::DoAITurn returns PlayAITurn's result in EAX (0067CB24..0067CB29 `mov [esi+22Ch], eax`; ret
// with EAX intact) and DoGameSequence consumes it (00680335 `mov edi, eax`). Declared std::int32_t since header h2.
inline I32 do_ai_turn(void* p) { return kotor_fastcall_0067cb10(p, nullptr); }
// Inlined CSWGuiActivatedButton::setColor (HandleContinue/Stand/PlayHandCard): timer 0.8f, colour [007A23CC].
void flash(void* button) {
    sw(button, 0x1C4, kDelayLong);
    std::memcpy(at(button, 0x1C8), kotor_pazaak_color_007a23cc, 12);
}

// ======================= method table 00753358 (27 slots; base CSWGuiPanel 0073E010) =======================
const Override ovr_game[] = {
    {0x00, fn(&kotor_fastcall_0067ff80)},   // 0067FF80 scalar deleting destructor
    {0x34, fn(&kotor_fastcall_00680710)},   // 00680710 Draw
    {0x3C, fn(&kotor_fastcall_0067e8f0)},   // 0067E8F0 HandleInputEvent
    {0x48, fn(&kotor_fastcall_0067ffa0)},   // 0067FFA0 OnPanelAdded
};
void* table_game[27];
bool built_game = false;
}  // namespace

extern "C" {
const void* __cdecl kotor_pazaak_vtable_00753358() {
    if (!built_game) { vt_build(table_game, 27, gui_load_panel_methods_0073e010, 27, ovr_game, 4); built_game = true; }
    return table_game;
}

// ======================= 0040ADF0 CSWGuiManager::IsOnTop(CSWGuiPanel*) RET 4 =======================
// Topmost panel without flags 0x600 in the list at +94/+98 (top first), else in +88/+8C; 1 when it is `panel`.
std::int32_t __fastcall kotor_fastcall_0040adf0(void* manager, void*, void* panel) {
    I32 i = li(manager, 0x98) - 1;                                                             // 0040ADF0..0040ADF6
    if (i >= 0) {                                                                              // 0040ADFF js
        void* data = lp(manager, 0x94);                                                        // 0040AE01
        for (; i >= 0; --i) {                                                                  // 0040AE10..0040AE1D
            void* p = lp(data, static_cast<U32>(i) * 4u);
            if ((lw(p, 0x44) & 0x600u) == 0) return lp(data, static_cast<U32>(i) * 4u) == panel ? 1 : 0; // 0040AE48..0040AE5B
        }
    }
    i = li(manager, 0x8C) - 1;                                                                 // 0040AE1F..0040AE25
    if (i >= 0) {                                                                              // 0040AE26 js
        void* data = lp(manager, 0x88);                                                        // 0040AE28
        for (; i >= 0; --i) {                                                                  // 0040AE31..0040AE3E
            void* p = lp(data, static_cast<U32>(i) * 4u);
            if ((lw(p, 0x44) & 0x600u) == 0) return lp(data, static_cast<U32>(i) * 4u) == panel ? 1 : 0; // 0040AE5E..0040AE71
        }
    }
    return 0;                                                                                  // 0040AE40..0040AE45
}

// ======================= 0067D4B0 RefreshDisplay RET =======================
void __fastcall kotor_fastcall_0067d4b0(void* self, void*) {
    I32 npc_standing = li(core_of(self), 0xE0);                                                 // 0067D4CB..0067D4DC [S+8]
    I32 player_standing = li(core_of(self), 0x70);                                              // 0067D4D1 [S+C]
    I32 player_changed = 0, npc_changed = 0;                                                   // 0067D4E8..0067D4EC
    if (li(self, 0x86D4) >= 10) { player_standing = 0; npc_standing = 0; }                     // 0067D4E0..0067D4FA (jl)
    for (U32 i = 0; i < 9; ++i) {                                                              // 0067D500..0067D653 (esi 98h..E0h)
        void* card = player_board(self, i);
        void* p = core_of(self);
        const I32 value = li(p, 0x28 + i * 8), flag = li(p, 0x2C + i * 8);                     // 0067D510..0067D51A
        if (li(card, 0x304) != value || li(card, 0x308) != flag) {                             // 0067D51E..0067D536
            kotor_fastcall_0067cd30(card, nullptr, li(p, 0x28 + i * 8), li(p, 0x2C + i * 8));   // 0067D538..0067D544 SetCard
            player_changed = 1;                                                                // 0067D549
        }
        fill3(at(card, 0x90), player_standing != 0 ? kGrey : kOne);                            // 0067D551..0067D5AC
        void* ncard = npc_board(self, i);
        p = core_of(self);
        const I32 nvalue = li(p, 0x98 + i * 8), nflag = li(p, 0x9C + i * 8);                   // 0067D5AF..0067D5BC
        if (li(ncard, 0x304) != nvalue || li(ncard, 0x308) != nflag) {                         // 0067D5C0..0067D5CE
            kotor_fastcall_0067cd30(ncard, nullptr, li(p, 0x98 + i * 8), li(p, 0x9C + i * 8));  // 0067D5D0..0067D5DF
            npc_changed = 1;                                                                   // 0067D5E4
        }
        fill3(at(ncard, 0x90), npc_standing != 0 ? kGrey : kOne);                              // 0067D5EC..0067D650
    }
    I32 any_flip = 0;                                                                          // [S+8] (first pass: ecx = 0)
    I32 any_hand = 0;                                                                          // 0067D661 [S+C] = 0
    for (U32 k = 0; k < 4; ++k) {                                                              // 0067D677..0067D81E (edi 0..18h)
        void* flip = at(self, 0x62BC + k * kButton);                                           // [S+4]
        void* hand = player_hand(self, k);
        void* p = core_of(self);
        const I32 value = li(p, 0x08 + k * 8), flag = li(p, 0x0C + k * 8);                     // 0067D682..0067D690
        const bool flip_card = value >= 12 && value < 18;
        any_flip = (any_flip != 0 || flip_card) ? 1 : 0;                                       // 0067D680..0067D6AA
        if (li(hand, 0x304) != value || li(hand, 0x308) != flag) {                             // 0067D6B2..0067D6BF
            kotor_fastcall_0067cd30(hand, nullptr, li(p, 0x08 + k * 8), li(p, 0x0C + k * 8));   // 0067D6C1..0067D6D1
            set_enabled(flip, flip_card ? 1u : 0u);                                            // 0067D6D6..0067D6F0 slot +88
            const U32 f = lw(flip, 0x44);                                                      // 0067D6F6..0067D71A
            sw(flip, 0x44, f ^ ((f ^ (flip_card ? 2u : 0u)) & 2u));
        }
        fill3(at(hand, 0x90), li(self, 0x86D4) != 3 ? kGrey : kOne);                           // 0067D71D..0067D77F
        if (li(hand, 0x304) >= 0) any_hand = 1;                                                // 0067D782..0067D78C
        p = core_of(self);
        I32 nvalue = li(p, 0x78 + k * 8), nflag = li(p, 0x7C + k * 8);                         // 0067D794..0067D7AA
        void* nhand = npc_hand(self, k);
        if (kotor_cell_pazaak_show_npc_hand_00833bb0 == 0) {                                   // 0067D79A..0067D7AE
            if (nvalue >= 0) { nvalue = -2; nflag = 0; }                                       // 0067D7B0..0067D7BD (face down)
            if (li(nhand, 0x304) != nvalue || li(nhand, 0x308) != nflag)                       // 0067D7C1..0067D7D7
                kotor_fastcall_0067cd30(nhand, nullptr, nvalue, nflag);                        // 0067D7F7..0067D7FF
        } else if (li(nhand, 0x304) != nvalue || li(nhand, 0x308) != nflag) {                  // 0067D7D9..0067D7ED
            kotor_fastcall_0067cd30(nhand, nullptr, li(p, 0x78 + k * 8), li(p, 0x7C + k * 8)); // 0067D7EF..0067D7FF
        }
    }
    const U32 bit = (static_cast<U32>(any_flip) & 1u) * 2u;                                    // 0067D824..0067D831
    sw(self, 0x6A10, (lw(self, 0x6A10) & 0xFFFFFFFDu) | bit);                                  // 0067D828..0067D838 LBL_FLIPICON
    sw(self, 0x6B50, (lw(self, 0x6B50) & 0xFFFFFFFDu) | bit);                                  // 0067D83E..0067D853 LBL_FLIPLEGEND
    if (player_changed != 0) {                                                                 // 0067D849..0067D85A
        Str total(player_total(self));                                                         // 0067D85C..0067D86F CExoString(int)
        gui_load_text_params_set_text_00415e00(at(self, 0x3CC), nullptr, total.p());           // 0067D874..0067D887 LBL_PLRTOTAL
    }
    if (npc_changed != 0) {                                                                    // 0067D899..0067D89F
        Str total(npc_total(self));                                                            // 0067D8A1..0067D8B4
        gui_load_text_params_set_text_00415e00(at(self, 0x50C), nullptr, total.p());           // 0067D8B9..0067D8CC LBL_NPCTOTAL
    }
    if (li(self, 0x86D4) == 10) {                                                              // 0067D8DE..0067D8E5
        I32 i = li(core_of(self), 0x74) - 1;                                                    // 0067D8E7..0067D8F0
        if (i >= 0 && i < 3) {                                                                 // 0067D8F1..0067D8F6 (js / jge)
            menu_resources::Ref ref; ref.construct_00406d80("lbl_winmark02");                  // 0067D8F8..0067D901 (00753110)
            gui_load_border_params_set_fill_00414c00(at(self, 0xAD4 + static_cast<U32>(i) * kLabel), nullptr, &ref, 0); // 0067D906..0067D91A
        }
        i = li(core_of(self), 0xE4) - 1;                                                        // 0067D91F..0067D92B
        if (i >= 0 && i < 3) {                                                                 // 0067D92C..0067D931
            menu_resources::Ref ref; ref.construct_00406d80("lbl_winmark02");                  // 0067D933..0067D93C
            gui_load_border_params_set_fill_00414c00(at(self, 0xE94 + static_cast<U32>(i) * kLabel), nullptr, &ref, 0); // 0067D941..0067D955
        }
    }
    const U32 play = (li(self, 0x86D4) == 3 && lw(self, 0x1C) != 0 && li(self, 0x86E4) == 0 && any_hand != 0) ? 1u : 0u; // 0067D95A..0067D985
    set_enabled(at(self, 0x6FF4), play);                                                       // 0067D985..0067D992
    const U32 flip_enable = (any_hand != 0 && li(self, 0x86E4) == 0) ? 1u : 0u;                // 0067D998..0067D9B3
    set_enabled(at(self, 0x71C8), flip_enable);                                                // 0067D9B3..0067D9C0
    const I32 state = li(self, 0x86D4);
    const U32 turn = (li(core_of(self), 0x70) == 0 && (state == 3 || state == 4) && li(self, 0x86E4) == 0) ? 1u : 0u; // 0067D9C6..0067D9F6
    set_enabled(at(self, 0x6C4C), turn);                                                       // 0067D9F6..0067DA03
    set_enabled(at(self, 0x6E20), turn);                                                       // 0067DA09..0067DA16
}

// ======================= 0067DA30 ReconnectSidedeckButtons RET =======================
void __fastcall kotor_fastcall_0067da30(void* self, void*) {
    I32 active_empty = 0;                                                                      // 0067DA39 [S+14]
    I32 prev = -1;                                                                             // 0067DA41 ebp
    for (I32 i = 3; i >= 0; --i) {                                                             // 0067DA44..0067DA85
        void* card = player_hand(self, static_cast<U32>(i));
        gui_load_nav_set_move_to_00417660(card, nullptr, 0, nullptr);                          // 0067DA50..0067DA56
        gui_load_nav_set_move_to_00417660(card, nullptr, 2, nullptr);                          // 0067DA5B..0067DA61
        if (li(card, 0x304) >= 0) prev = i;                                                    // 0067DA66..0067DA7A
    }
    U32 prev_off = static_cast<U32>(prev) * kCard;                                             // 0067DA87..0067DA8E
    for (I32 c = 3; c >= 0; --c) {                                                             // 0067DA94..0067DB0C
        void* card = player_hand(self, static_cast<U32>(c));
        if (li(card, 0x304) < 0) {                                                             // 0067DAB0..0067DAC2
            if (lp(self, 0x1C) == card) active_empty = 1;                                      // 0067DAC4..0067DAC9
            continue;
        }
        if (prev >= 0) {                                                                       // 0067DAD3..0067DAD5
            void* other = at(self, prev_off + 0x2DE0);                                         // 0067DAD7
            gui_load_nav_set_move_to_00417660(card, nullptr, 3, other);                        // 0067DADE..0067DAE3
            gui_load_nav_set_move_to_00417660(other, nullptr, 1, card);                        // 0067DAE8..0067DAED
        }
        prev = c;                                                                              // 0067DAFA
        prev_off = static_cast<U32>(c) * kCard;                                                // 0067DAFC
    }
    if (active_empty != 0 && li(self, 0x86E4) != 0) {                                          // 0067DB0E..0067DB1E
        if (prev >= 0) set_active(self, at(self, static_cast<U32>(prev) * kCard + 0x2DE0));    // 0067DB20..0067DB38 slot +08
        else set_active(self, nullptr);                                                        // 0067DB43..0067DB45
    }
}

// ======================= 0067DB50 HandleQuitDialog(user) RET 4 =======================
void __fastcall kotor_fastcall_0067db50(void* self, void*, std::uint32_t) {
    void* gui = pt(frame_R4_005ed690(client_app(), nullptr));                                  // 0067DB50..0067DB5B GetInGameGui
    if ((lb(lp(gui, 0x98), 0x64) & 1u) == 0) return;                                           // 0067DB60..0067DB6A
    delete_flag(self);                                                                         // 0067DB6C..0067DB7B
    void* p = core_of(self);
    kotor_fastcall_005eddd0(client_app(), nullptr, 0, li(p, 0x00), 0, lw(p, 0x04));            // 0067DB7E..0067DB98 EndPazaakGame(0, wager, 0, opp)
}

// ======================= 0067DBB0 SetHandEnabled(int) RET 4 =======================
void __fastcall kotor_fastcall_0067dbb0(void* self, void*, std::int32_t enabled) {
    I32 last = -1;                                                                             // 0067DBBD [S+10]
    for (I32 i = 3; i >= 0; --i) {                                                             // 0067DBC5..0067DBF4
        void* card = player_hand(self, static_cast<U32>(i));
        if (li(card, 0x304) >= 0) { sw(card, 0x318, static_cast<U32>(enabled)); last = i; }    // 0067DBD0..0067DBE4
        else sw(card, 0x318, 0);                                                               // 0067DBE6
    }
    const U32 buttons = enabled == 0 ? 1u : 0u;                                                // 0067DBFC..0067DC00 sete
    set_enabled(at(self, 0x6C4C), buttons);                                                    // 0067DC03..0067DC0E
    set_enabled(at(self, 0x6E20), buttons);                                                    // 0067DC14..0067DC21
    set_enabled(at(self, 0x6FF4), buttons);                                                    // 0067DC27..0067DC34
    set_enabled(at(self, 0x71C8), buttons);                                                    // 0067DC3A..0067DC47
    if (enabled != 0) {                                                                        // 0067DC4D..0067DC4F
        if (last >= 0) set_active(self, player_hand(self, static_cast<U32>(last)));            // 0067DC51..0067DC72
    } else {
        set_active(self, at(self, 0x6C4C));                                                    // 0067DC6B..0067DC72
    }
    kotor_fastcall_0067d4b0(self, nullptr);                                                    // 0067DC75..0067DC77
}

// ======================= 0067DC90 HandleXButton / 0067DCC0 HandleBlackButton RET 4 =======================
void __fastcall kotor_fastcall_0067dc90(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) == 0) return;                                  // 0067DC90..0067DC9D
    sw(self, 0x86E4, 1);                                                                       // 0067DC9F (mode: play card)
    kotor_fastcall_0067dbb0(self, nullptr, 1);                                                 // 0067DCA9..0067DCB1 tail jump
}
void __fastcall kotor_fastcall_0067dcc0(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) == 0) return;                                  // 0067DCC0..0067DCCD
    sw(self, 0x86E4, 2);                                                                       // 0067DCCF (mode: flip card)
    kotor_fastcall_0067dbb0(self, nullptr, 1);                                                 // 0067DCD9..0067DCE1 tail jump
}

// ======================= 0067DCF0 HandleFlipHandCard RET =======================
void __fastcall kotor_fastcall_0067dcf0(void* self, void*) {
    void* active = lp(self, 0x1C);                                                             // 0067DCF6
    if (active == nullptr) return;                                                             // 0067DCF9..0067DCFB
    const I32 i = li(active, 0x50);                                                            // 0067DCFE
    if (i < 0 || i >= 4) return;                                                               // 0067DD01..0067DD08
    if (active != player_hand(self, static_cast<U32>(i))) return;                              // 0067DD0A..0067DD1B
    const I32 value = li(core_of(self), 0x08 + static_cast<U32>(i) * 8);                        // 0067DD1D..0067DD2E
    if (value < 12 || value >= 18) return;                                                     // 0067DD27..0067DD37
    kotor_fastcall_0067ca50(at(self, 0x71C8), nullptr, kotor_pazaak_color_007a23cc);           // 0067DD39..0067DD44 setColor
    void* flag = at(core_of(self), 0x0C + static_cast<U32>(i) * 8);                             // 0067DD49..0067DD53
    sw(flag, 0, lw(flag, 0) == 0 ? 1u : 0u);                                                   // 0067DD57..0067DD60 sete
    kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                                     // 0067DD5E..0067DD65 PlayGuiSound(0)
    kotor_fastcall_0067d4b0(self, nullptr);                                                    // 0067DD6B..0067DD71 tail jump
}

// ======================= 0067DD80 HandleHandCardRightClick / 0067DDA0 HandleFlipButtons RET 4 =======================
void __fastcall kotor_fastcall_0067dd80(void* self, void*, void* control) {
    if (control == nullptr) return;                                                            // 0067DD80..0067DD89
    const U32 saved = lw(self, 0x1C);                                                          // 0067DD8C
    sw(self, 0x1C, ad(control));                                                               // 0067DD8F
    kotor_fastcall_0067dcf0(self, nullptr);                                                    // 0067DD92
    sw(self, 0x1C, saved);                                                                     // 0067DD97
}
void __fastcall kotor_fastcall_0067dda0(void* self, void*, void* control) {
    const U32 i = lw(control, 0x50);                                                           // 0067DDA7 (no null check)
    const U32 saved = lw(self, 0x1C);                                                          // 0067DDB8
    sw(self, 0x1C, ad(self) + i * kCard + 0x2DE0);                                             // 0067DDAA..0067DDBD
    kotor_fastcall_0067dcf0(self, nullptr);                                                    // 0067DDC0
    sw(self, 0x1C, saved);                                                                     // 0067DDC5
}

// ======================= 0067E540 ~CSWGuiPazaakGame RET =======================
void __fastcall kotor_fastcall_0067e540(void* self, void*) {
    set_vptr(self, kotor_pazaak_vtable_00753358());                                            // 0067E55D (00753358)
    if (core_of(self) != nullptr) {                                                             // 0067E563..0067E573
        live_free(core_of(self));                                                               // 0067E575..0067E57B _free (no dtor)
        sw(self, 0x86D0, 0);                                                                   // 0067E57E
    }
    kotor_fastcall_0067df60(tutorial(self), nullptr);                                          // 0067E588..0067E593 ~CSWGuiTutorial
    gui_m48_message_box_destruct_006270a0(box(self), nullptr);                                 // 0067E598..0067E5A3
    for (U32 off : {0x71C8u, 0x6FF4u, 0x6E20u, 0x6C4Cu})
        gui_load_button_destruct_00419da0(at(self, off), nullptr);                             // 0067E5A8..0067E5E3
    gui_load_label_destruct_00419d10(at(self, 0x6B0C), nullptr);                               // 0067E5E8..0067E5F3
    gui_load_label_destruct_00419d10(at(self, 0x69CC), nullptr);                               // 0067E5F8..0067E603
    vector_destruct(at(self, 0x62BC), kButton, 4, &gui_load_button_destruct_00419da0);         // 0067E608..0067E620
    vector_destruct(at(self, 0x564C), kCard, 4, &kotor_fastcall_0067de80);                     // 0067E625..0067E63D
    vector_destruct(at(self, 0x3A50), kCard, 9, &kotor_fastcall_0067de80);                     // 0067E642..0067E65A
    vector_destruct(at(self, 0x2DE0), kCard, 4, &kotor_fastcall_0067de80);                     // 0067E65F..0067E677
    vector_destruct(at(self, 0x11E4), kCard, 9, &kotor_fastcall_0067de80);                     // 0067E67C..0067E694
    vector_destruct(at(self, 0xE24), kLabel, 3, &gui_load_label_destruct_00419d10);            // 0067E699..0067E6B1
    vector_destruct(at(self, 0xA64), kLabel, 3, &gui_load_label_destruct_00419d10);            // 0067E6B6..0067E6CE
    for (U32 off : {0x924u, 0x7E4u, 0x6A4u, 0x564u, 0x424u, 0x2E4u, 0x1A4u, 0x64u})
        gui_load_label_destruct_00419d10(at(self, off), nullptr);                              // 0067E6D3..0067E74B
    gui_load_panel_destruct_0040cf70(self, nullptr);                                           // 0067E750..0067E75A
}

// ======================= 0067E770 StartNewGame RET =======================
void __fastcall kotor_fastcall_0067e770(void* self, void*) {
    kotor_fastcall_006e4640(core_of(self), nullptr);                                            // 0067E776..0067E77C ClearGameBoard
    sw(self, 0x86D4, 0);                                                                       // 0067E783
    kotor_fastcall_0067d4b0(self, nullptr);                                                    // 0067E78D
    const U32 shown = lw(self, 0x86DC) & 0xFFFFFFEFu;                                          // 0067E792..0067E798
    sw(self, 0x86DC, shown);                                                                   // 0067E79F
    if ((shown & 8u) == 0) {                                                                   // 0067E79D..0067E7A5
        sw(self, 0x86DC, shown | 8u);                                                          // 0067E7A7..0067E7AA
        const std::uint32_t pages[2] = {0x96E7u, 0x96E8u};                                     // 0067E7BD..0067E7C5 [esp+4]
        kotor_fastcall_0067e0b0(tutorial(self), nullptr, pages, 2);                            // 0067E7B0..0067E7CD ShowMultipleHelp
    }
    sw(self, 0x86E4, 0);                                                                       // 0067E7D6
    kotor_fastcall_0067dbb0(self, nullptr, 0);                                                 // 0067E7D2..0067E7E0
    set_active(self, at(self, 0x6C4C));                                                        // 0067E7E5..0067E7F2 slot +08
}

// ======================= 0067E8F0 HandleInputEvent(event, down) RET 8 =======================
void __fastcall kotor_fastcall_0067e8f0(void* self, void*, std::int32_t event, std::int32_t down) {
    if (down != 0 && (event == 0x28 || event == 0x2E)) {                                       // 0067E8F1..0067E90D
        if (li(self, 0x86E4) != 0) {                                                           // 0067E90F..0067E917
            sw(self, 0x86E4, 0);                                                               // 0067E91D
            kotor_fastcall_0067dbb0(self, nullptr, 0);                                         // 0067E919..0067E927
        } else {
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                             // 0067E93B..0067E941 PlayGuiSound(0)
            void* quit = lp(pt(frame_R4_005ed690(client_app(), nullptr)), 0x98);               // 0067E946..0067E953
            kotor_fastcall_00627130(quit, nullptr, 1);                                         // 0067E959..0067E95D SetAllowCancel(1)
            gui_load_messagebox_set_callback_00624a40(quit, nullptr, self, ad(fn(&kotor_fastcall_0067db50)), 0); // 0067E962..0067E96C
            vcall<void, U32>(quit, 0x6C, 0xA5B9u);                                             // 0067E971..0067E97A
            kotor_fastcall_0040bc70(manager_of(self), nullptr, quit, 1, 1);                    // 0067E97D..0067E985 AddPanel
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, static_cast<U32>(event), static_cast<U32>(down)); // 0067E92C / 0067E98B
}

// ======================= 0067E9A0 HandleEndDialog(user) RET 4 =======================
void __fastcall kotor_fastcall_0067e9a0(void* self, void*, std::uint32_t) {
    void* p = core_of(self);
    if (li(p, 0x74) < 3 && li(p, 0xE4) < 3) {                                                  // 0067E9A3..0067E9BB (jge)
        show_help(self, 0x96F8, -1, 0x96E1);                                                   // 0067E9BD..0067E9CF
        sw(self, 0x86D4, 11);                                                                  // 0067E9D4 next set
        return;
    }
    show_help(self, 0x96F9, -1, 0x96E2);                                                       // 0067E9E2..0067E9F4
    sw(self, 0x86D4, 12);                                                                      // 0067E9F9 match over
}

// ======================= 0067EA10 HandleTutorialClose(user) RET 4 =======================
// Index bytes 0067EC08 for strref 96E9..96F9: 0,6,6,1,6,6,6,2,3,4,6,1,6,6,6,6,5 -> targets 0067EBEC:
// 0 0067EA69, 1 0067EAB5, 2 0067EAF1, 3 0067EAFA, 4 0067EB1D, 5 0067EB83, 6 0067EBE1 (exit).
void __fastcall kotor_fastcall_0067ea10(void* self, void*, std::uint32_t) {
    static const U8 index_0067ec08[17] = {0, 6, 6, 1, 6, 6, 6, 2, 3, 4, 6, 1, 6, 6, 6, 6, 5};
    void* tut = tutorial(self);                                                                // 0067EA25 edi
    const I32 page = li(self, 0x86B0);                                                         // 0067EA16 tutorial +990
    const U32 strref = page < li(self, 0x86A8) ? lw(lp(tut, 0x984), static_cast<U32>(page) * 4u) : 0u; // 0067EA1C..0067EA38
    if ((lb(self, 0x7D84) & 1u) == 0) {                                                        // 0067EA3A..0067EA47 tutorial +64 bit 0
        if (strref >= 0x96F8u && strref <= 0x96F9u) sw(self, 0x86D4, 12);                     // 0067EBC9..0067EBD7
        return;
    }
    const U32 idx = strref - 0x96E9u;                                                          // 0067EA4D
    if (idx > 0x10u) return;                                                                   // 0067EA52..0067EA55 ja
    switch (index_0067ec08[idx]) {                                                             // 0067EA5B..0067EA62
    case 0: {                                                                                  // 0067EA69
        const std::uint32_t pages[5] = {0x96EAu, 0x96EBu, 0x96ECu, 0x96F3u, 0x96F4u};
        kotor_fastcall_0067e0b0(tut, nullptr, pages, 5);                                       // ..0067EA9A ShowMultipleHelp
        kotor_fastcall_0067d1c0(tut, nullptr, player_hand(self, 0));                           // 0067EA9F..0067EAA8 PlaceAbove
        return;
    }
    case 1: {                                                                                  // 0067EAB5
        const std::uint32_t pages[3] = {0x96EDu, 0x96EEu, 0x96EFu};
        kotor_fastcall_0067e0b0(tut, nullptr, pages, 3);                                       // ..0067EAD6
        kotor_fastcall_0067d1c0(tut, nullptr, at(self, 0x6C4C));                               // 0067EADB..0067EAE4
        return;
    }
    case 2:                                                                                    // 0067EAF1 (stand confirmed)
        sw(core_of(self), 0x70, 1);                                                             // 0067EAF1..0067EAF7 (ecx = 1)
        [[fallthrough]];
    case 3:                                                                                    // 0067EAFA (end turn confirmed)
        sw(self, 0x86D4, 5);                                                                   // 0067EAFC
        sw(self, 0x86D8, kDelayShort);                                                         // 0067EB06
        kotor_fastcall_0067d4b0(self, nullptr);                                                // 0067EB10
        return;
    case 4:                                                                                    // 0067EB1D (play card confirmed)
        (void)kotor_fastcall_006e4860(core_of(self), nullptr, li(self, 0x86E0));                // 0067EB1D..0067EB2A UsePlayerSidedeckCard
        sw(self, 0x86E0, 0xFFFFFFFFu);                                                         // 0067EB3A
        one_shot(sound(self, 2));                                                              // 0067EB2F..0067EB5E mgs_playside
        sw(self, 0x86D4, 4);                                                                   // 0067EB65
        kotor_fastcall_0067d4b0(self, nullptr);                                                // 0067EB6F
        kotor_fastcall_0067da30(self, nullptr);                                                // 0067EB76
        return;
    case 5: {                                                                                  // 0067EB83 (match over confirmed)
        delete_flag(self);                                                                     // 0067EB83..0067EB90
        void* p = core_of(self);
        end_game(self, li(p, 0x74) > li(p, 0xE4) ? 1u : 0u);                                   // 0067EB93..0067EBBC setg
        return;
    }
    default: return;                                                                           // 0067EBE1
    }
}

// ======================= 0067EC20 HandleContinue / 0067ED00 HandleStand RET 4 =======================
void __fastcall kotor_fastcall_0067ec20(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) == 0) return;                                  // 0067EC20..0067EC30
    if (li(core_of(self), 0x70) != 0) return;                                                   // 0067EC36..0067EC41
    const I32 state = li(self, 0x86D4);
    if (state != 3 && state != 4) return;                                                      // 0067EC47..0067EC55
    if (li(self, 0x86E4) != 0) return;                                                         // 0067EC5B..0067EC63
    flash(at(self, 0x6C4C));                                                                   // 0067EC69..0067EC8F
    kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                                     // 0067EC92..0067EC97
    if (li(self, 0x86B4) != 0 && player_total(self) >= 16) {                                   // 0067EC9C..0067ECB7 (jl)
        show_help(self, 0x96F1, 0x7E4C, 0x96E2);                                               // 0067ECB9..0067ECCE
        return;
    }
    sw(self, 0x86D4, 5);                                                                       // 0067ECD9
    sw(self, 0x86D8, kDelayShort);                                                             // 0067ECE3
    kotor_fastcall_0067d4b0(self, nullptr);                                                    // 0067ECED
}
void __fastcall kotor_fastcall_0067ed00(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) == 0) return;                                  // 0067ED00..0067ED10
    if (li(core_of(self), 0x70) != 0) return;                                                   // 0067ED16..0067ED21
    const I32 state = li(self, 0x86D4);
    if (state != 3 && state != 4) return;                                                      // 0067ED27..0067ED35
    flash(at(self, 0x6E20));                                                                   // 0067ED3B..0067ED61
    kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                                     // 0067ED64..0067ED69
    if (li(self, 0x86B4) != 0 && player_total(self) <= 13) {                                  // 0067ED6E..0067ED89 (jg)
        show_help(self, 0x96F0, 0x7E4D, 0x96E2);                                               // 0067ED8B..0067EDA0
        return;
    }
    sw(core_of(self), 0x70, 1);                                                                 // 0067EDA9..0067EDAF stand
    sw(self, 0x86D4, 5);                                                                       // 0067EDB8
    sw(self, 0x86D8, kDelayShort);                                                             // 0067EDC2
    kotor_fastcall_0067d4b0(self, nullptr);                                                    // 0067EDCC
}

// ======================= 0067EDE0 HandlePlayHandCard(int hand index) RET 4 =======================
// The contract header types the argument as void*; the original pushes the hand index (callers 0067EF71, 0067EF94,
// 006807B0 pass control+0x50).
void __fastcall kotor_fastcall_0067ede0(void* self, void*, void* index_arg) {
    if (li(self, 0x86D4) != 3) return;                                                         // 0067EDE6..0067EDED
    const I32 index = static_cast<I32>(ad(index_arg));                                         // 0067EDF4
    if (index < 0 || index >= 4) return;                                                       // 0067EDF8..0067EE03
    if (kotor_fastcall_006e4840(core_of(self), nullptr, index) == 0) return;                    // 0067EE09..0067EE17 CanUse
    flash(at(self, 0x6FF4));                                                                   // 0067EE1D..0067EE44
    if (li(self, 0x86B4) != 0 && player_total(self) <= 12) {                                   // 0067EE47..0067EE62 (jg)
        sw(self, 0x86E0, static_cast<U32>(index));                                             // 0067EE69
        kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                                 // 0067EE64..0067EE6F
        show_help(self, 0x96F2, 0x7E4A, 0x96E2);                                               // 0067EE74..0067EE89
        return;
    }
    (void)kotor_fastcall_006e4860(core_of(self), nullptr, index);                               // 0067EE96..0067EE9D UsePlayerSidedeckCard
    one_shot(sound(self, 2));                                                                  // 0067EEA2..0067EEC7 mgs_playside
    sw(self, 0x86D4, 4);                                                                       // 0067EECE
    sw(self, 0x86D8, kDelayShort);                                                             // 0067EED8
    kotor_fastcall_0067d4b0(self, nullptr);                                                    // 0067EEE2
    kotor_fastcall_0067da30(self, nullptr);                                                    // 0067EEE9
}

// ======================= 0067EF00 HandleHandCardDrop(control) RET 4 =======================
void __fastcall kotor_fastcall_0067ef00(void* self, void*, void* control) {
    void* target = lp(kotor_menu_window_cells::gui_manager_007a39f4, 0x08);                    // 0067EF00..0067EF05 [[007A39F4]+8]
    const U32 saved = lw(self, 0x1C);                                                          // 0067EF0E
    if (target == nullptr) return;                                                             // 0067EF08..0067EF11
    if (target == at(self, 0x71C8)) {                                                          // 0067EF13..0067EF1B flip button
        sw(self, 0x1C, ad(control));                                                           // 0067EF1D..0067EF23
        kotor_fastcall_0067dcf0(self, nullptr);                                                // 0067EF26
        sw(self, 0x1C, saved);                                                                 // 0067EF2B
        return;
    }
    if (li(self, 0x86D4) != 3) return;                                                         // 0067EF33..0067EF3A
    bool hit = target == at(self, 0x6FF4);                                                     // 0067EF3C..0067EF4D
    for (U32 i = 0; !hit && i < 9; ++i) hit = target == player_board(self, i);                 // 0067EF4F..0067EF63
    if (!hit) return;                                                                          // 0067EF65
    kotor_fastcall_0067ede0(self, nullptr, lp(control, 0x50));                                 // 0067EF6A..0067EF74
}

// ======================= 0067EF80 HandleHandCardDoubleClick(control) RET 4 =======================
void __fastcall kotor_fastcall_0067ef80(void* self, void*, void* control) {
    if (control == nullptr || li(self, 0x86D4) != 3) return;                                   // 0067EF80..0067EF8F
    kotor_fastcall_0067ede0(self, nullptr, lp(control, 0x50));                                 // 0067EF91..0067EF98 tail jump
}

// ======================= 0067FF80 scalar deleting destructor RET 4 =======================
void* __fastcall kotor_fastcall_0067ff80(void* self, void*, std::uint32_t flags) {
    return deleting(self, &kotor_fastcall_0067e540, flags);                                    // 0067FF80..0067FF9B
}

// ======================= 0067FFA0 OnPanelAdded RET =======================
void __fastcall kotor_fastcall_0067ffa0(void* self, void*) {
    kotor_fastcall_0067e770(self, nullptr);                                                    // 0067FFBC StartNewGame
    void* server = server_app();                                                               // 0067FFC1..0067FFC6
    void* creature = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 0067FFC9..0067FFD3
    {
        Str name;                                                                              // [esp+8]
        void* text = kotor_fastcall_005a4c60(lp(creature, 0xA74), nullptr, name.s);            // 0067FFD8..0067FFE5 (no null check)
        name.live = true;
        gui_load_text_params_set_text_00415e00(at(self, 0x14C), nullptr, text);                // 0067FFEA..0067FFF9 LBL_PLRNAME
    }                                                                                          // 00680002..0068000A ~CExoString
    kotor_fastcall_0067da30(self, nullptr);                                                    // 0068000F..00680011
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 00680016..00680018
}

// ======================= 00680030 DoGameSequence RET =======================
// State table 006806C8: 0 00680057, 1 00680070, 2 006800DD, 3/4 006801E3, 5 00680212, 6 00680298, 7 0068032A,
// 8 00680475, 9 006805F1, 10 006806BA (no-op), 11 00680667, 12 0068067C. AI table 006806FC (DoAITurn result 1..4):
// 0068034A, 006803AF, 006803E7, 006803EF. Returns [esp+8] (1 = run the next state in the same frame).
std::int32_t __fastcall kotor_fastcall_00680030(void* self, void*) {
    I32 result = 0;                                                                            // 00680045 [esp+8]
    const U32 state = lw(self, 0x86D4);
    if (state > 12u) return result;                                                            // 00680042..00680049 ja 006806BB
    switch (state) {                                                                           // 00680050
    case 0:                                                                                    // 00680057
        sw(self, 0x86D4, 1);
        result = 1;
        return result;
    case 1:                                                                                    // 00680070
        if (li(core_of(self), 0x70) != 0) {                                                     // 00680076..00680079 player stands
            sw(self, 0x86D4, 5);                                                               // 006801F3
            result = 1;                                                                        // 006801FD
            return result;
        }
        sw(self, 0x828, lw(self, 0x828) | 2u);                                                 // 0068007F..0068008C LBL_PLRTURN
        sw(self, 0x968, lw(self, 0x968) & 0xFFFFFFFDu);                                        // 00680092 LBL_NPCTURN
        one_shot(sound(self, 0));                                                              // 00680099..006800BA mgs_startturn
        sw(self, 0x86D4, 2);                                                                   // 006800C3
        sw(self, 0x86D8, kDelayShort);                                                         // 006800CA
        return result;
    case 2: {                                                                                  // 006800DD
        kotor_fastcall_0067cae0(core_of(self), nullptr);                                        // 006800DD..006800E3 DrawPlayerCard
        one_shot(sound(self, 1));                                                              // 006800E8..00680109 mgs_drawmain
        const U32 shown = lw(self, 0x86DC);                                                    // 0068010E
        if ((shown & 2u) == 0) {                                                               // 00680114..00680116
            sw(self, 0x86DC, shown | 2u);                                                      // 0068011F..0068012F
            show_help(self, 0x96E9, -1, 0x96E2);                                               // 00680118..00680135
            kotor_fastcall_0067d110(tutorial(self), nullptr, player_board(self, 0));           // 0068013A..00680143 PlaceRightOf
        }
        if (li(core_of(self), 0x70) != 0) {                                                     // 00680148..00680151
            kotor_fastcall_0067ca50(at(self, 0x6E20), nullptr, kotor_pazaak_color_007a23cc);   // 00680153..0068015E setColor
            sw(self, 0x86D8, kDelayShort);                                                     // 00680163
        }
        if (player_total(self) > 20) one_shot(sound(self, 3));                                 // 0068016D..006801A4 mgs_warnbust
        sw(self, 0x86D4, 3);                                                                   // 006801AD
        void* active = vcall<void*>(self, 0x44);                                               // 006801A9..006801B7 GetActiveControl
        set_active(self, nullptr);                                                             // 006801BA..006801C2
        set_active(self, active);                                                              // 006801C5..006801CB
        kotor_fastcall_0067d4b0(self, nullptr);                                                // 006801CE..006801D0
        return result;
    }
    case 3:
    case 4:                                                                                    // 006801E3 (wait for input)
        if (li(core_of(self), 0x70) == 0) return result;                                        // 006801E3..006801EC
        sw(self, 0x86D4, 5);                                                                   // 006801F2..006801F3
        result = 1;
        return result;
    case 5: {                                                                                  // 00680212
        const I32 total = player_total(self);                                                  // 00680212..0068021B
        if (total > 20) show_help(self, 0x96F7, -1, 0x96E2);                                   // 00680220..0068025A
        else if (total == 20 && (lw(self, 0x86DC) & 0x10u) == 0) {                             // 00680233..0068023D
            sw(self, 0x86DC, lw(self, 0x86DC) | 0x10u);                                        // 00680244..00680249
            show_help(self, 0x96F6, -1, 0x96E2);                                               // 0068023F..0068025A
        }
        if (kotor_fastcall_006e48a0(core_of(self), nullptr) != 0) {                             // 0068025F..0068026C DetermineWinner
            sw(self, 0x86D4, 8);                                                               // 0068026F
            result = 1;
            return result;
        }
        sw(self, 0x86D4, 6);                                                                   // 0068028E
    }
        [[fallthrough]];
    case 6:                                                                                    // 00680298
        if (li(core_of(self), 0xE0) != 0) {                                                     // 00680298..006802A4 opponent stands
            sw(self, 0x86D4, 8);                                                               // 0068026E..0068026F
            result = 1;
            return result;
        }
        sw(self, 0x828, lw(self, 0x828) & 0xFFFFFFFDu);                                        // 006802A6
        sw(self, 0x968, lw(self, 0x968) | 2u);                                                 // 006802AD
        one_shot(sound(self, 0));                                                              // 006802B4..006802D5 mgs_startturn
        sw(self, 0x86D8, kDelayShort);                                                         // 006802E2
        sw(self, 0x86D4, 7);                                                                   // 006802EC
        if ((lw(self, 0x86DC) & 4u) != 0) return result;                                       // 006802DA..006802F6
        sw(self, 0x86DC, lw(self, 0x86DC) | 4u);                                               // 00680303..00680311
        show_help(self, 0x96F5, -1, 0x96E2);                                                   // 006802FC..00680317
        return result;
    case 7: {                                                                                  // 0068032A
        const I32 ai = do_ai_turn(core_of(self));                                               // 0068032A..00680335 DoAITurn
        switch (ai) {                                                                          // 00680337..00680343 (table 006806FC)
        case 1:                                                                                // 0068034A drew a card
            one_shot(sound(self, 1));                                                          // mgs_drawmain
            if (npc_total(self) > 20) one_shot(sound(self, 3));                                // 00680370..006803A8 mgs_warnbust
            break;
        case 2:                                                                                // 006803AF played a side card
            kotor_fastcall_0067ca50(at(self, 0x6FF4), nullptr, kotor_m15d_color_007a23d8);     // 006803AF..006803BA
            one_shot(sound(self, 2));                                                          // 006803BF..006803E0 mgs_playside
            break;
        case 3:                                                                                // 006803E7 stood
        case 4:                                                                                // 006803EF ended turn
            kotor_fastcall_0067ca50(at(self, ai == 3 ? 0x6E20u : 0x6C4Cu), nullptr, kotor_m15d_color_007a23d8); // 006803F5..006803FA
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                             // 006803FF..00680403
            break;
        default: break;                                                                        // 0068033D ja 00680408
        }
        if (ai == 1 || ai == 2) {                                                              // 00680408..00680410
            sw(self, 0x86D8, kDelayLong);                                                      // 0068041B
            if (npc_total(self) == 20 && (lw(self, 0x86DC) & 0x10u) == 0) {                    // 00680412..00680437
                sw(self, 0x86DC, lw(self, 0x86DC) | 0x10u);                                    // 00680440..0068044E
                show_help(self, 0x96F6, -1, 0x96E2);                                           // 00680439..00680454
            }
        }
        if (ai != 0) {                                                                         // 00680459..0068045B
            kotor_fastcall_0067d4b0(self, nullptr);                                            // 006801CE
            return result;
        }
        sw(self, 0x86D8, kDelayShort);                                                         // 00680461
        sw(self, 0x86D4, 8);                                                                   // 0068046B
    }
        [[fallthrough]];
    case 8: {                                                                                  // 00680475
        const I32 winner = kotor_fastcall_006e48a0(core_of(self), nullptr);                     // 00680475..00680480 DetermineWinner
        if (winner == 0) {                                                                     // 00680482..00680484
            sw(self, 0x86D4, 1);                                                               // 006805D4
            kotor_fastcall_0067d4b0(self, nullptr);                                            // 006805DE
            return result;
        }
        kotor_fastcall_00627130(box(self), nullptr, 0);                                        // 0068048A..00680494 SetAllowCancel(0)
        if (winner == 1) {                                                                     // 00680499..0068049C
            void* p = core_of(self);
            sw(p, 0x74, lw(p, 0x74) + 1u);                                                     // 006804A4 inc
            if (li(core_of(self), 0x74) >= 3) {                                                 // 006804A7..006804B3 (jl)
                vcall<void, U32>(box(self), 0x6C, 0x7E50u);                                    // 006804B5..006804BC
                sw(self, 0x86CC, ad(k_mgs_winmatch_0075355c));                                 // 006804C1
            } else {
                vcall<void, U32>(box(self), 0x6C, 0x7E4Eu);                                    // 006804E8..006804EF
                sw(self, 0x86CC, ad(k_mgs_winset_00753550));                                   // 006804F4
            }
        } else if (winner == 2) {                                                              // 0068051B..0068051E
            void* p = core_of(self);
            sw(p, 0xE4, lw(p, 0xE4) + 1u);                                                     // 0068052A inc
            if (li(core_of(self), 0xE4) >= 3) {                                                 // 00680530..0068053F
                vcall<void, U32>(box(self), 0x6C, 0x7E51u);                                    // 00680541..00680548
                sw(self, 0x86CC, ad(k_mgs_losematch_00753540));                                // 0068054D
            } else {
                vcall<void, U32>(box(self), 0x6C, 0x7E4Fu);                                    // 00680574..0068057B
                sw(self, 0x86CC, ad(k_mgs_loseset_00753534));                                  // 00680580
            }
        } else {
            vcall<void, U32>(box(self), 0x6C, 0x7E52u);                                        // 006805A7..006805B0 tie
        }
        sw(self, 0x86D4, 9);                                                                   // 006804CB / 006804FE / 00680557 / 0068058A / 006805B5
        kotor_fastcall_0067d4b0(self, nullptr);
        return result;
    }
    case 9:                                                                                    // 006805F1
        if (lw(self, 0x86CC) != 0) {                                                           // 006805F1..006805F9
            one_shot(static_cast<const char*>(lp(self, 0x86CC)));                              // 006805FB..0068061C
            sw(self, 0x86CC, 0);                                                               // 00680621
        }
        gui_load_messagebox_set_callback_00624a40(box(self), nullptr, self, ad(fn(&kotor_fastcall_0067e9a0)), 0); // 00680627..00680636
        kotor_fastcall_0040bc70(manager_of(self), nullptr, box(self), 1, 1);                   // 0068063B..00680643 AddPanel
        sw(self, 0x86D4, 10);                                                                  // 0068064A
        kotor_fastcall_0067d4b0(self, nullptr);                                                // 00680654
        return result;
    case 10:                                                                                   // 006806BA (no-op)
        return result;
    case 11:                                                                                   // 00680667
        kotor_fastcall_0067e770(self, nullptr);                                                // StartNewGame
        return result;
    default: {                                                                                 // 12: 0068067C
        delete_flag(self);                                                                     // 0068067C..0068068B
        void* p = core_of(self);
        end_game(self, li(p, 0x74) > li(p, 0xE4) ? 1u : 0u);                                   // 0068068E..006806B5 setg
        return result;
    }
    }
}

// ======================= 00680710 Draw(float) RET 4 =======================
void __fastcall kotor_fastcall_00680710(void* self, void*, std::uint32_t delta_bits) {
    if (!jcc_jne(static_cast<double>(lf(self, 0x86D8)), 0.0, 0x01)) {                          // 00680713..00680724 fcomp 0.0; test ah,1; jne
        const double left = static_cast<double>(lf(self, 0x86D8)) - static_cast<double>(bits_f(delta_bits)); // 00680726..0068072C
        sf(self, 0x86D8, static_cast<float>(left));                                            // 00680730 fst dword
        if (!jcc_jp(left, 0.0, 0x05)) {                                                        // 00680736..00680741 fcomp 0.0; test ah,5; jp
            kotor_fastcall_0067d4b0(self, nullptr);                                            // 00680743 timer ran out: refresh
            kotor_panel_draw_0040B760(self, nullptr, delta_bits);                              // 00680748..0068074F
            return;
        }
    } else {
        while (kotor_fastcall_0040adf0(manager_of(self), nullptr, self) != 0 &&                // 00680760..0068076B IsOnTop
               kotor_fastcall_00680030(self, nullptr) != 0) {                                  // 0068076D..00680776
        }
    }
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 00680778..0068077F CSWGuiPanel::Draw
}

// ======================= 00680790 HandleHandcardAction(control) RET 4 =======================
void __fastcall kotor_fastcall_00680790(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) == 0) return;                                  // 00680790..006807A0
    const I32 mode = li(self, 0x86E4);                                                         // 006807A2
    if (mode == 1) {                                                                           // 006807A8..006807AB
        // 006807AD: reads control+0x50 even when control is null (original fault kept).
        const U32 index = *reinterpret_cast<volatile U32*>(static_cast<std::uintptr_t>(ad(control) + 0x50u));
        kotor_fastcall_0067ede0(self, nullptr, pt(index));                                     // 006807B0..006807B3
    } else if (mode == 2) {                                                                    // 006807BA..006807BD
        kotor_fastcall_0067dcf0(self, nullptr);                                                // 006807BF..006807C1
    }
    sw(self, 0x86E4, 0);                                                                       // 006807CA
    kotor_fastcall_0067dbb0(self, nullptr, 0);                                                 // 006807C6..006807D4
}

// ======================= 006808A0 CSWGuiPazaakGame(manager, CSWPazaak*, tutorial, opponent) RET 10h =======================
void* __fastcall kotor_fastcall_006808a0(void* self, void*, void* manager, void* pazaak_object, std::int32_t show_tutorial,
                                         std::uint32_t opponent) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006808C9..006808D1
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                 // state 0
    set_vptr(self, kotor_pazaak_vtable_00753358());                                            // 006808E2 (00753358)
    for (U32 off : {0x64u, 0x1A4u, 0x2E4u, 0x424u, 0x564u, 0x6A4u, 0x7E4u, 0x924u})
        label(self, off, u);                                                                   // 006808D8..0068096D states 1..8
    vector_member(at(self, 0xA64), kLabel, 3, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 00680972..00680992
    vector_member(at(self, 0xE24), kLabel, 3, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 00680997..006809B7
    vector_member(at(self, 0x11E4), kCard, 9, &kotor_fastcall_0067ddd0, &kotor_fastcall_0067de80, u); // 006809BC..006809DC
    vector_member(at(self, 0x2DE0), kCard, 4, &kotor_fastcall_0067ddd0, &kotor_fastcall_0067de80, u); // 006809E1..00680A01
    vector_member(at(self, 0x3A50), kCard, 9, &kotor_fastcall_0067ddd0, &kotor_fastcall_0067de80, u); // 00680A06..00680A26
    vector_member(at(self, 0x564C), kCard, 4, &kotor_fastcall_0067ddd0, &kotor_fastcall_0067de80, u); // 00680A2B..00680A4B
    vector_member(at(self, 0x62BC), kButton, 4, &gui_load_button_construct_0041c0f0, &gui_load_button_destruct_00419da0, u); // 00680A50..00680A70
    label(self, 0x69CC, u);                                                                    // 00680A75..00680A83
    label(self, 0x6B0C, u);                                                                    // 00680A88..00680A96
    for (U32 off : {0x6C4Cu, 0x6E20u, 0x6FF4u, 0x71C8u}) {                                     // 00680A9B..00680B92 inline CSWGuiActivatedButton
        void* b = button(self, off, u);
        set_vptr(b, kotor_pazaak_vtable_00753120());                                           // [b] = 00753120
        sw(b, 0x1C8, 0); sw(b, 0x1CC, 0); sw(b, 0x1D0, 0);
        sw(b, 0x1C4, 0xBF800000u);                                                             // -1.0f
        sw(b, 0x44, lw(b, 0x44) | 8u);
    }
    kotor_original::kotor_fastcall_00626df0(box(self), nullptr, manager);                      // 00680B96..00680BA8 CSWGuiMessageBox
    u.push(&gui_m48_message_box_destruct_006270a0, box(self));                                 // state 15h
    kotor_fastcall_0067def0(tutorial(self), nullptr, manager);                                 // 00680BAD..00680BBC CSWGuiTutorial
    u.push(&kotor_fastcall_0067df60, tutorial(self));                                          // state 16h
    sw(self, 0x86D0, ad(pazaak_object));                                                       // 00680BC1..00680BCA
    sw(self, 0x86BC, ad(k_mgs_startturn_00753740));                                            // 00680BE0
    sw(self, 0x86C0, ad(k_mgs_drawmain_00753730));                                             // 00680BEA
    sw(self, 0x86C4, ad(k_mgs_playside_00753720));                                             // 00680BF4
    sw(self, 0x86C8, ad(k_mgs_warnbust_00753710));                                             // 00680BFE
    sw(self, 0x86D4, 0);                                                                       // 00680C08
    sw(self, 0x86D8, 0xBF800000u);                                                             // 00680C0E -1.0f
    sw(self, 0x86DC, 0);                                                                       // 00680C18
    sw(self, 0x86E0, 0xFFFFFFFFu);                                                             // 00680C1E
    sw(self, 0x86CC, 0);                                                                       // 00680C28
    sw(self, 0x86E4, 0);                                                                       // 00680C2E
    gui_load_messagebox_set_callback_00624a40(tutorial(self), nullptr, self, ad(fn(&kotor_fastcall_0067ea10)), 0); // 00680BC4..00680C34
    sw(self, 0x86B4, static_cast<U32>(show_tutorial));                                         // 00680C39..00680C43
    kotor_fastcall_00627130(box(self), nullptr, 0);                                            // 00680C3C..00680C49 SetAllowCancel(0)
    start_layout(self, "pazaakgame");                                                          // 00680C4E..00680C66 (00753704)
    init_control(self, 0x64, "LBL_PLRNAME");                                                   // 00680C6B..00680C9E (007536F8)
    init_control(self, 0x1A4, "LBL_NPCNAME");                                                  // 00680CA3..00680CD9 (007536EC)
    init_control(self, 0x2E4, "LBL_PLRTOTAL");                                                 // 00680CDE..00680D14 (007536DC)
    init_control(self, 0x424, "LBL_NPCTOTAL");                                                 // 00680D19..00680D4F (007536CC)
    init_control(self, 0x564, "LBL_PLRSIDEDECK");                                              // 00680D54..00680D8A (007536BC)
    init_control(self, 0x6A4, "LBL_NPCSIDEDECK");                                              // 00680D8F..00680DC5 (007536AC)
    init_control(self, 0x7E4, "LBL_PLRTURN");                                                  // 00680DCA..00680E00 (007536A0)
    init_control(self, 0x924, "LBL_NPCTURN");                                                  // 00680E05..00680E3B (00753694)
    sw(self, 0x828, lw(self, 0x828) & 0xFFFFFFFDu);                                            // 00680E40..00680E4D
    sw(self, 0x968, lw(self, 0x968) & 0xFFFFFFFDu);                                            // 00680E53
    init_control(self, 0xA64, "LBL_PLRSCORE0");                                                // 00680E59..00680E8F (00753684)
    init_control(self, 0xBA4, "LBL_PLRSCORE1");                                                // 00680E94..00680ECA (00753674)
    init_control(self, 0xCE4, "LBL_PLRSCORE2");                                                // 00680ECF..00680F05 (00753664)
    init_control(self, 0xE24, "LBL_NPCSCORE0");                                                // 00680F0A..00680F40 (00753654)
    init_control(self, 0xF64, "LBL_NPCSCORE1");                                                // 00680F45..00680F7B (00753644)
    init_control(self, 0x10A4, "LBL_NPCSCORE2");                                               // 00680F80..00680FB6 (00753634)
    init_control(self, 0x69CC, "LBL_FLIPICON");                                                // 00680FBB..00680FF1 (00753624)
    init_control(self, 0x6B0C, "LBL_FLIPLEGEND");                                              // 00680FF6..0068102C (00753614)
    {   // 16-byte name templates copied by dword from .rdata, then sprintf("%d") at the 'x' (00681031..006810CD).
        char btn_plr[16], btn_npc[16], lbl_plr[16], lbl_npc[16];
        std::memcpy(btn_plr, "BTN_PLRx       ", 16);                                           // 00753604 -> [esp+1C]
        std::memcpy(btn_npc, "BTN_NPCx       ", 16);                                           // 007535F4 -> [esp+4C]
        std::memcpy(lbl_plr, "LBL_PLRx       ", 16);                                           // 007535E4 -> [esp+34]
        std::memcpy(lbl_npc, "LBL_NPCx       ", 16);                                           // 007535D4 -> [esp+64]
        for (I32 i = 0; i < 9; ++i) {                                                          // 006810E0..00681264
            format_006fadb0(btn_plr + 7, "%d", i);                                             // 006810E0..006810EF (0073D720)
            format_006fadb0(btn_npc + 7, "%d", i);                                             // 006810F4..00681103
            format_006fadb0(lbl_plr + 7, "%d", i);                                             // 00681108..00681117
            format_006fadb0(lbl_npc + 7, "%d", i);                                             // 0068111C..0068112E
            const U32 card = static_cast<U32>(i) * kCard;
            init_control(self, 0x11E4 + card, btn_plr);                                        // 00681136..0068116F BTN_PLRi
            init_control(self, 0x11E4 + card + 0x1C4, lbl_plr);                                // 00681174..006811AE LBL_PLRi (face label)
            init_control(self, 0x3A50 + card, btn_npc);                                        // 006811B3..006811EA BTN_NPCi
            init_control(self, 0x3A50 + card + 0x1C4, lbl_npc);                                // 006811EF..00681229 LBL_NPCi
            void* npc = npc_board(self, static_cast<U32>(i));
            sw(npc, 0x44, lw(npc, 0x44) | 0x20u);                                              // 0068122E..00681232
            set_enabled(player_board(self, static_cast<U32>(i)), 0);                           // 00681235..0068123E slot +88
            sw(player_board(self, static_cast<U32>(i)), 0x314, 0);                             // 0068124D not draggable
        }
    }
    {   // 0068126A..0068134B templates; sprintf at offset 0x0B (side) / 8 (flip).
        char btn_plrside[16], btn_npcside[16], lbl_plrside[16], lbl_npcside[16], btn_flip[16];
        std::memcpy(btn_plrside, "BTN_PLRSIDEx   ", 16);                                       // 007535C4 -> [esp+64]
        std::memcpy(btn_npcside, "BTN_NPCSIDEx   ", 16);                                       // 007535B4 -> [esp+8C]
        std::memcpy(lbl_plrside, "LBL_PLRSIDEx   ", 16);                                       // 007535A4 -> [esp+4C]
        std::memcpy(lbl_npcside, "LBL_NPCSIDEx   ", 16);                                       // 00753594 -> [esp+7C]
        std::memcpy(btn_flip, "BTN_FLIPx      ", 16);                                          // 00753584 -> [esp+34]
        for (I32 i = 0; i < 4; ++i) {                                                          // 00681355..00681574
            format_006fadb0(btn_plrside + 11, "%d", i);                                        // 00681355..00681364
            format_006fadb0(btn_npcside + 11, "%d", i);                                        // 00681369..0068137B
            format_006fadb0(lbl_plrside + 11, "%d", i);                                        // 00681380..0068138F
            format_006fadb0(lbl_npcside + 11, "%d", i);                                        // 00681394..006813A6
            format_006fadb0(btn_flip + 8, "%d", i);                                            // 006813AB..006813BA
            const U32 card = static_cast<U32>(i) * kCard;
            void* hand = player_hand(self, static_cast<U32>(i));                               // edi
            void* flip = at(self, 0x62BC + static_cast<U32>(i) * kButton);                     // [esp+10]
            init_control(self, 0x2DE0 + card, btn_plrside);                                    // 006813C2..006813F2
            init_control(self, 0x2DE0 + card + 0x1C4, lbl_plrside);                            // 006813F7..0068142D
            init_control(self, 0x62BC + static_cast<U32>(i) * kButton, btn_flip);              // 00681432..00681466
            sw(hand, 0x50, static_cast<U32>(i));                                               // 00681473
            sw(flip, 0x50, static_cast<U32>(i));                                               // 0068147D
            init_control(self, 0x564C + card, btn_npcside);                                    // 00681476..006814B2
            init_control(self, 0x564C + card + 0x1C4, lbl_npcside);                            // 006814B7..006814ED
            gui_load_control_add_event_0041ab20(hand, nullptr, 0x27, self, fn(&kotor_fastcall_00680790));    // 006814F2..006814FC
            gui_load_control_add_event_0041ab20(hand, nullptr, 0x17FEB, self, fn(&kotor_fastcall_0067ef00)); // 00681501..0068150E
            gui_load_control_add_event_0041ab20(hand, nullptr, 0x1F9, self, fn(&kotor_fastcall_0067ef80));   // 00681513..00681520
            gui_load_control_add_event_0041ab20(hand, nullptr, 0x44, self, fn(&kotor_fastcall_0067dd80));    // 00681525..0068152F
            void* npc = npc_hand(self, static_cast<U32>(i));
            sw(npc, 0x44, lw(npc, 0x44) | 0x20u);                                              // 00681534..00681549
            gui_load_control_add_event_0041ab20(flip, nullptr, 0x27, self, fn(&kotor_fastcall_0067dda0));    // 0068153A..0068154F
        }
    }
    init_control(self, 0x6C4C, "BTN_XTEXT");                                                   // 0068157A..006815B0 (00753578)
    init_control(self, 0x6E20, "BTN_YTEXT");                                                   // 006815B5..006815EB (0075356C)
    stop_layout(self);                                                                         // 006815F0..006815F2
    add_event(self, 0x6C4C, 0x27, self, fn(&kotor_fastcall_0067ec20));                         // 006815F7..00681601 HandleContinue
    add_event(self, 0x6E20, 0x27, self, fn(&kotor_fastcall_0067ed00));                         // 00681606..00681614 HandleStand
    add_event(self, 0x6FF4, 0x27, self, fn(&kotor_fastcall_0067dc90));                         // 00681619..00681627 HandleXButton
    add_event(self, 0x71C8, 0x27, self, fn(&kotor_fastcall_0067dcc0));                         // 0068162C..0068163A HandleBlackButton
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, opponent);                 // 0068163F..0068164C
    if (creature != nullptr) {                                                                 // 00681651..00681653
        Str name;                                                                              // [esp+1C] state 33h
        void* text = kotor_fastcall_005a4c60(lp(creature, 0xA74), nullptr, name.s);            // 00681655..00681662 GetFullName
        name.live = true;
        gui_load_text_params_set_text_00415e00(at(self, 0x28C), nullptr, text);                // 00681667..00681676 LBL_NPCNAME
    }                                                                                          // 0068167B..00681686
    if (vcall<I32>(at(self, 0x6DA0), 0x08) > li(self, 0x6C5C))                                 // 0068168B..006816A0 (jle)
        gui_load_text_params_set_alignment_00415f20(at(self, 0x6DB8), nullptr, 0xA);           // 006816A2..006816AA
    if (vcall<I32>(at(self, 0x6F74), 0x08) > li(self, 0x6E30))                                 // 006816AF..006816C4
        gui_load_text_params_set_alignment_00415f20(at(self, 0x6F8C), nullptr, 0xA);           // 006816C6..006816CE
    sb(self, 0x48, 2);                                                                         // 006816DB
    u.dismiss();
    return self;                                                                               // 006816DF..006816ED
}
}  // extern "C"
