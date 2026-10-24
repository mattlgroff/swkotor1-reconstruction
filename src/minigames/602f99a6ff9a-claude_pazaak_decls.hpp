#pragma once
// Pazaak lane (PZ-A / PZ-B / PZ-C): shared layout and export contract. Owner: PZ-B. Tier pz-b/h2 (2026-10-24; h1 + DoAITurn returns the new AI state).
// Self-contained (only <cstdint>), so the W4 GUI recipe (srcw4/src/gui), the W5 recipe and the 0854 VM unit can all
// include it. It declares ONLY the 94 Pazaak exports, the 6 Pazaak method-table accessors and layout constants; it
// declares no existing provider (use gui_rt.hpp / claude_gui_char.hpp / w4_providers.hpp for those).
//
// Naming contract (frozen):
//   original thiscall at XXXXXXXX -> extern "C" __fastcall kotor_fastcall_xxxxxxxx(void* self, void* edx, stack args...)
//   decorated @N with N = 8 + the original `ret` byte count (verified against the original for all 94; tail-jump
//   bodies take their jump target's ABI). Method table at 00753XXX -> extern "C" const void* __cdecl
//   kotor_pazaak_vtable_00753xxx() (built once; the original writes the .rdata address, the reconstruction writes the
//   accessor's result). Parameter NAMES/TYPES below are advisory; COUNT, ORDER and the decorated name are the contract.
//   Pointer and int parameters are 4-byte DWORDs; a float parameter is passed as its raw bits (std::uint32_t).
//
// Sources of every number: original GOG 1.03 swkotor.exe disassembly (read-only), PCs quoted. Card/label/button/panel
// base sizes: CSWGuiButton 0x1C4, CSWGuiLabel 0x140, CSWGuiPanel 0x64, CSWGuiMessageBox 0x984 (from the member
// offsets of the constructors below).
#include <cstdint>

namespace pazaak {
using u32 = std::uint32_t;
using i32 = std::int32_t;

// ---------------------------------------------------------------- card values (CSWGuiPazaakCard::SetCard 0067CD30,
// CPazaakPlayer::GetTotal 006E4360). A card is {i32 value, i32 flag} (8 bytes) everywhere (hand, board, deck, chosen).
//   -1       empty slot (GetTotal stops at the first -1 board value; SetCard shows "" text, empty fill image)
//   0..5     +1..+6 side cards            6..11  -1..-6 side cards
//   12..17   +/-1..+/-6 flip cards: flag != 0 -> negative ("-n" text, lbl_cardraref), flag == 0 -> positive
//   18..27   main-deck cards 1..10 (lbl_cardstand); value table [007A26B8] (PZ-A)
constexpr i32 kCardEmpty = -1;
constexpr i32 kCardFlipFirst = 12, kCardFlipEnd = 18, kCardCount = 28;

// ---------------------------------------------------------------- CPazaakPlayer (0x70, no vtable; 006E4B50)
namespace player {
constexpr u32 kSize = 0x70;
constexpr u32 kHand = 0x00;        // card[4]  (ChooseSidedeck 006E4580 writes [this + 8*k])
constexpr u32 kHandCount = 4;
constexpr u32 kBoard = 0x20;       // card[9]  (AddGameCard 006E45F0: first value == -1 from +0x20; ctor 006E4B50)
constexpr u32 kBoardCount = 9;
constexpr u32 kStanding = 0x68;    // i32; AddGameCard sets 1 at 9 cards or total 20; DetermineWinner sets 1 on bust
constexpr u32 kSetsWon = 0x6C;     // i32 (CSWPazaak +0x74 / +0xE4)
}
// ---------------------------------------------------------------- CSWPazaak (0x230, no vtable; ctor 006E4C00; new at 00681CFC)
namespace core {
constexpr u32 kSize = 0x230;
constexpr u32 kWager = 0x000;      // i32; Start ctor 00681D40 stores the wager, then 1 when wager > 0 (00681D60)
constexpr u32 kOpponent = 0x004;   // OBJECT_ID (ctor 0x7F000000; Start ctor 00681D50)
constexpr u32 kPlayer = 0x008;     // CPazaakPlayer (human)
constexpr u32 kNpc = 0x078;        // CPazaakPlayer (opponent)
constexpr u32 kDeck = 0x0E8;       // card[40] (DrawCard 006E4800 reads [this + 8*index + 0xE8])
constexpr u32 kDeckCount = 40;
constexpr u32 kDeckIndex = 0x228;  // i32 next card index, -1 = shuffle first (ctor -1)
constexpr u32 kAiState = 0x22C;    // i32 (DoAITurn 0067CB10 round-trips PlayAITurn's result)
}
// ---------------------------------------------------------------- CSWGuiPazaakCard (0x31C, CSWGuiButton base, table 007531C0; ctor 0067DDD0)
namespace card {
constexpr u32 kSize = 0x31C;
constexpr u32 kBorderObject = 0x06C;   // button border sub-object (own table: +4 SetExtent, +C Draw); extent at +0x70
constexpr u32 kBorderFill = 0x080;     // CSWGuiBorderParams (SetFillImage 0067CEA4)
constexpr u32 kBorderColor = 0x090;    // Vector (OnPanelAdded 0067F8F8 / AddChosenCard writes (0.6670, x3) / RemoveChosenCard 1.0)
constexpr u32 kFillResRef = 0x0C0;     // CResRef (first byte tested, 0067D07D)
constexpr u32 kTextObject = 0x0E0;     // text sub-object (own table: +C Draw)
constexpr u32 kPulse = 0x0F4;          // CSWGuiBorderParams (SetPulsingAlpha 0067CD20)
constexpr u32 kButtonText = 0x16C;     // CSWGuiTextParams
constexpr u32 kLabel = 0x1C4;          // embedded CSWGuiLabel (0x140) = the card face label; its text params at +0x2AC
constexpr u32 kLabelText = 0x2AC;
constexpr u32 kValue = 0x304;          // i32 card value (ctor -1)
constexpr u32 kFlag = 0x308;           // i32 card flag
constexpr u32 kDownX = 0x30C, kDownY = 0x310;  // mouse position at HandleLMouseDown ([CSWGuiManager+0/+4])
constexpr u32 kDraggable = 0x314;      // i32 (ctor 1)
constexpr u32 kSelectable = 0x318;     // i32 (ctor 1; GetIsSelectable = (flags>>3) & this & 1)
// Drag/drop: the card sets [CSWGuiManager+0x54] (dragged control) when moved > 12 px with the capture; LMouseUp
// fires HandleInputEvent(0x17FEB, 1) on the dragged card (= the drop event the panels register).
constexpr u32 kEventDrop = 0x17FEB;
}
// ---------------------------------------------------------------- CSWGuiActivatedButton (0x1D4, CSWGuiButton base, table 00753120)
// NO constructor exists in the original: every owner inlines (Start 00681B4C.., Game 00680A9B..)
//   CSWGuiButton::CSWGuiButton(b); [b] = table 00753120; [b+1C8]=[b+1CC]=[b+1D0]=0; [b+1C4]=0xBF800000 (-1.0f); [b+44] |= 8.
// Slot +00 is CSWGuiSpeedButton's scalar deleting destructor 0067EFA0 (existing body).
namespace activated {
constexpr u32 kSize = 0x1D4;
constexpr u32 kTimer = 0x1C4;          // float; setColor 0067CA50 writes 0x3F4CCCCD (0.8f)
constexpr u32 kColor = 0x1C8;          // Vector target colour (setColor copies 12 bytes)
constexpr u32 kText = 0x16C;           // CSWGuiTextParams of the button
}
// ---------------------------------------------------------------- CSWGuiTutorial (0x99C, CSWGuiMessageBox base, table 00753260; ctor 0067DEF0)
namespace tutorial {
constexpr u32 kSize = 0x99C;
constexpr u32 kButtonAText = 0x460;    // CSWGuiTextParams of message-box button +0x2F4 (ShowHelp arg 2 / 0x96DF)
constexpr u32 kButtonBText = 0x624;    // CSWGuiTextParams of message-box button +0x4B8 (ShowHelp arg 3 / 0x96E0)
constexpr u32 kPages = 0x984;          // CExoArrayList<u32 strref> {data +984, size +988, capacity +98C}
constexpr u32 kPage = 0x990;           // i32 current page
constexpr u32 kEnabled = 0x994;        // i32 show tutorials (ctor 1; Start +0x8920 / Game +0x86B4 overwrite it)
constexpr u32 kAnchor = 0x998;         // CSWGuiControl* PlaceAbove anchor
}
// ---------------------------------------------------------------- CSWGuiWagerPopup (0xCAC, CSWGuiPanel base, table 007534C8; ctor 0067F000; new at 00681D5B)
namespace wager {
constexpr u32 kSize = 0xCAC;
constexpr u32 kTitle = 0x064;          // LBL_TITLE
constexpr u32 kMaximum = 0x1A4;        // LBL_MAXIMUM (text params +0x28C)
constexpr u32 kValueLabel = 0x2E4;     // LBL_WAGERVAL (text params +0x3CC)
constexpr u32 kLess = 0x424;           // BTN_LESS  CSWGuiSpeedButton (0x1D4; table 007533C8 = existing w4gui::vt_speed_button())
constexpr u32 kMore = 0x5F8;           // BTN_MORE  CSWGuiSpeedButton
constexpr u32 kAccept = 0x7CC;         // BTN_WAGER
constexpr u32 kQuit = 0x990;           // BTN_QUIT
constexpr u32 kBackground = 0xB54;     // LBL_BG
constexpr u32 kValue = 0xC94;          // i32 current wager
constexpr u32 kMax = 0xC98;            // i32 maximum wager
constexpr u32 kList = 0xC9C;           // CExoArrayList {data +C9C, size +CA0, capacity +CA4} (unused; freed by the dtor)
constexpr u32 kOwner = 0xCA8;          // CSWGuiPazaakStart*
constexpr u32 kEventSpeedDown = 0x16256;   // CSWGuiSpeedButton L-button / repeat event (0067E813), the AddEvent id of BTN_LESS/MORE
}
// ---------------------------------------------------------------- CSWGuiPazaakStart (0x8928, CSWGuiPanel base, table 007532E8; ctor 00681A90; new at 005F38C0)
namespace start {
constexpr u32 kSize = 0x8928;
constexpr u32 kTitle = 0x0064;         // LBL_TITLE
constexpr u32 kAvail = 0x01A4;         // card[18] BTN_AVAILrc (r = i/6, c = i%6), face label LBL_AVAILrc
constexpr u32 kAvailCount = 18;
constexpr u32 kAvailNum = 0x399C;      // CSWGuiLabel[18] LBL_AVAILNUMrc (text params at +0xE8)
constexpr u32 kChosen = 0x501C;        // card[10] BTN_CHOSENi, face label LBL_CHOSENi
constexpr u32 kChosenCount = 10;
constexpr u32 kSwitch = 0x6F34;        // CSWGuiActivatedButton (never laid out; SetStrRef 0x96C9/0x96CA, SetEnabled)
constexpr u32 kAccept = 0x7108;        // CSWGuiActivatedButton BTN_ATEXT
constexpr u32 kLeftText = 0x72DC;      // LBL_LTEXT
constexpr u32 kRightText = 0x741C;     // LBL_RTEXT
constexpr u32 kAvailLeft = 0x755C;     // i32[18] cards still available per kind (from PartyTable+0x120)
constexpr u32 kChosenCards = 0x75A4;   // card[10] chosen side deck {value, flag}
constexpr u32 kWagerPopup = 0x75F4;    // CSWGuiWagerPopup* (0 when no wager)
constexpr u32 kLastChosenControl = 0x75F8;   // last active chosen card (ctor: &chosen[0])
constexpr u32 kLastAvailControl = 0x75FC;    // last active available card (ctor: &avail[0])
constexpr u32 kAllChosen = 0x7600;     // i32 all 10 chosen (accept enabled)
constexpr u32 kPazaak = 0x7604;        // CSWPazaak* (ownership moves to the Game panel at 006819B8)
constexpr u32 kMessageBox = 0x7608;    // CSWGuiMessageBox (accept confirmation; result bit [box+0x64]&1)
constexpr u32 kTutorial = 0x7F8C;      // CSWGuiTutorial (its +0x994 = start +0x8920 = show-tutorial argument)
constexpr u32 kShowTutorial = 0x8920;
}
// ---------------------------------------------------------------- CSWGuiPazaakGame (0x86E8, CSWGuiPanel base, table 00753358; ctor 006808A0) - PZ-C owns
// Member construction order from the ctor (PZ-C re-derives field meanings):
namespace game {
constexpr u32 kSize = 0x86E8;
constexpr u32 kLabels = 0x0064;        // 8 x CSWGuiLabel: PLRNAME +64, NPCNAME +1A4, PLRTOTAL +2E4, NPCTOTAL +424,
                                       //   PLRSIDEDECK +564, NPCSIDEDECK +6A4, PLRTURN +7E4, NPCTURN +924
constexpr u32 kPlayerScore = 0x0A64;   // CSWGuiLabel[3] LBL_PLRSCOREn
constexpr u32 kNpcScore = 0x0E24;      // CSWGuiLabel[3] LBL_NPCSCOREn
constexpr u32 kPlayerBoard = 0x11E4;   // card[9]
constexpr u32 kPlayerHand = 0x2DE0;    // card[4]
constexpr u32 kNpcBoard = 0x3A50;      // card[9]
constexpr u32 kNpcHand = 0x564C;       // card[4]
constexpr u32 kFlipButtons = 0x62BC;   // CSWGuiButton[4]
constexpr u32 kFlipIcon = 0x69CC, kFlipLegend = 0x6B0C;   // LBL_FLIPICON / LBL_FLIPLEGEND
constexpr u32 kButtonX = 0x6C4C, kButtonY = 0x6E20, kButton3 = 0x6FF4, kButton4 = 0x71C8;   // CSWGuiActivatedButton x4
constexpr u32 kMessageBox = 0x739C;    // CSWGuiMessageBox
constexpr u32 kTutorial = 0x7D20;      // CSWGuiTutorial (callback HandleTutorialClose 0067EA10)
constexpr u32 kShowTutorial = 0x86B4;  // = tutorial +0x994
constexpr u32 kTextureNames = 0x86BC;  // const char*[4] (.rdata 00753740/30/20/10)
constexpr u32 kField86CC = 0x86CC;
constexpr u32 kPazaak = 0x86D0;        // CSWPazaak* (owned)
constexpr u32 kState = 0x86D4;         // i32 DoGameSequence state
constexpr u32 kTimer = 0x86D8;         // float (ctor -1.0f)
constexpr u32 kField86DC = 0x86DC, kField86E0 = 0x86E0 /* ctor -1 */, kField86E4 = 0x86E4;
}
// Panel delete protocol used by Start/Game/Wager: [panel+0x44] = ([panel+0x44] & ~0x300) | 0x400 (manager deletes
// through slot +00 with flags 1).
}  // namespace pazaak

extern "C" {
// ================================================================= PZ-A: core rules, AI, RNG, VM entry, client app
int __fastcall kotor_fastcall_00540fd0(void* commands, void*, int routine, unsigned argc);   // ExecuteCommandPlayPazaak (VM 364) RET 8
void __fastcall kotor_fastcall_005eddc0(void* client_app, void*, std::int32_t deck, void* end_script /*CExoString*/,
    std::int32_t max_wager, std::int32_t tutorial, std::uint32_t opponent);                  // CClientExoApp::StartPazaakGame (jmp 005F3810)
void __fastcall kotor_fastcall_005eddd0(void* client_app, void*, std::int32_t won, std::int32_t wager,
    std::int32_t restart, std::uint32_t opponent);                                           // CClientExoApp::EndPazaakGame (jmp 005F3950)
void __fastcall kotor_fastcall_005f3810(void* internal, void*, std::int32_t deck, void* end_script,
    std::int32_t max_wager, std::int32_t tutorial, std::uint32_t opponent);                  // RET 14h
void __fastcall kotor_fastcall_005f3950(void* internal, void*, std::int32_t won, std::int32_t wager,
    std::int32_t restart, std::uint32_t opponent);                                           // RET 10h
void __fastcall kotor_fastcall_0067cae0(void* pazaak, void*);                                // CSWPazaak::DrawPlayerCard RET
std::int32_t __fastcall kotor_fastcall_0067cb10(void* pazaak, void*);                        // CSWPazaak::DoAITurn RET -> new AI state
    // (h2) 0067CB24..0067CB30: [this+22C] = PlayAITurn(...) and EAX is returned unchanged; DoGameSequence 00680335
    // (mov edi,eax) dispatches on it through the table 006806FC (result-1 in 0..3).
std::int32_t __fastcall kotor_fastcall_006e4360(void* player, void*);                        // CPazaakPlayer::GetTotal RET
void __fastcall kotor_fastcall_006e4580(void* player, void*, void* cards10 /*card[10]*/);     // CPazaakPlayer::ChooseSidedeck RET 4 (4 x rand)
void __fastcall kotor_fastcall_006e45f0(void* player, void*, std::int32_t value, std::int32_t flag); // AddGameCard(card by value) RET 8
void __fastcall kotor_fastcall_006e4640(void* pazaak, void*);                                // CSWPazaak::ClearGameBoard RET
void __fastcall kotor_fastcall_006e4730(void* pazaak, void*);                                // CSWPazaak::ShuffleDeck RET (40 x rand)
void* __fastcall kotor_fastcall_006e4800(void* pazaak, void*, void* out_card);              // CSWPazaak::DrawCard RET 4 (returns out_card)
std::int32_t __fastcall kotor_fastcall_006e4840(void* pazaak, void*, std::int32_t hand_index);   // CanUsePlayerSidedeckCard RET 4
std::int32_t __fastcall kotor_fastcall_006e4860(void* pazaak, void*, std::int32_t hand_index);   // UsePlayerSidedeckCard RET 4
std::int32_t __fastcall kotor_fastcall_006e48a0(void* pazaak, void*);                        // DetermineWinner RET (0 not both standing, 1 player, 2 npc, 3 tie)
std::int32_t __fastcall kotor_fastcall_006e4910(void* pazaak, void*, std::uint32_t a, std::uint32_t b, std::uint32_t c); // FindPotentialSidecardTotal RET 0Ch
void __fastcall kotor_fastcall_006e4a40(void* pazaak, void*, std::int32_t deck_row, void* out_cards10); // LoadInitialSideDeck RET 8
void* __fastcall kotor_fastcall_006e4b50(void* player, void*);                              // CPazaakPlayer::CPazaakPlayer RET
void* __fastcall kotor_fastcall_006e4c00(void* pazaak, void*) noexcept(false);                            // CSWPazaak::CSWPazaak RET
std::int32_t __fastcall kotor_fastcall_006e4c60(void* pazaak, void*, void* npc, void* player, std::int32_t ai_state); // PlayAITurn RET 0Ch

// ================================================================= PZ-B: controls, tutorial, wager, start panel
// ---- CSWGuiActivatedButton (table 00753120)
void __fastcall kotor_fastcall_0067ca50(void* button, void*, const void* color);             // setColor(const Vector&) RET 4
void __fastcall kotor_fastcall_0067cb70(void* button, void*, std::uint32_t delta_bits);      // Draw(float) RET 4 (x87 fsin)
void __fastcall kotor_fastcall_0067cc90(void* button, void*, std::int32_t enabled);          // SetEnabled(int) RET 4
// ---- CSWGuiPazaakCard (table 007531C0)
void __fastcall kotor_fastcall_0067ca80(void* card, void*, std::uint32_t delta_bits);        // Draw(float) RET 4
void __fastcall kotor_fastcall_0067cac0(void* card, void*, std::int32_t event, std::int32_t down); // HandleInputEvent (tail jumps) RET 8
void __fastcall kotor_fastcall_0067cd00(void* card, void*, void* a, void* b);                // Load(2 args -> CSWGuiButton::Load) RET 8
void __fastcall kotor_fastcall_0067cd30(void* card, void*, std::int32_t value, std::int32_t flag); // SetCard(card by value) RET 8
void __fastcall kotor_fastcall_0067cf00(void* card, void*, std::int32_t x, std::int32_t y);   // DrawDragDrop(x, y) RET 8
void __fastcall kotor_fastcall_0067d040(void* card, void*);                                  // HandleLMouseDown RET
std::int32_t __fastcall kotor_fastcall_0067d060(void* card, void*, std::int32_t x, std::int32_t y); // HandleMouseCapturedMovement RET 8 (returns 0)
void __fastcall kotor_fastcall_0067d0d0(void* card, void*);                                  // HandleLMouseUp RET
void* __fastcall kotor_fastcall_0067ddd0(void* card, void*) noexcept(false);                            // CSWGuiPazaakCard() RET (vector element ctor)
std::int32_t __fastcall kotor_fastcall_0067de60(void* card, void*);                          // GetIsSelectable RET
void __fastcall kotor_fastcall_0067de80(void* card, void*);                                  // ~CSWGuiPazaakCard RET (vector element dtor)
void* __fastcall kotor_fastcall_0067efc0(void* card, void*, std::uint32_t flags);            // scalar deleting dtor RET 4
// ---- CSWGuiTutorial (table 00753260)
void __fastcall kotor_fastcall_0067d110(void* tutorial, void*, void* control);               // PlaceRightOf(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_0067d1c0(void* tutorial, void*, void* control);               // PlaceAbove(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_0067d260(void* tutorial, void*, std::int32_t event, std::int32_t down); // HandleInputEvent RET 8
void* __fastcall kotor_fastcall_0067def0(void* tutorial, void*, void* manager) noexcept(false);           // CSWGuiTutorial(CSWGuiManager*) RET 4
void __fastcall kotor_fastcall_0067df60(void* tutorial, void*);                              // ~CSWGuiTutorial RET
void __fastcall kotor_fastcall_0067dfc0(void* tutorial, void*, std::uint32_t message_strref,
    std::int32_t button_a_strref, std::uint32_t button_b_strref);                           // ShowHelp RET 0Ch (-1/0x96E2 -> no cancel, 0x96DF)
void __fastcall kotor_fastcall_0067e0b0(void* tutorial, void*, const std::uint32_t* strrefs, std::int32_t count); // ShowMultipleHelp RET 8
void* __fastcall kotor_fastcall_0067efe0(void* tutorial, void*, std::uint32_t flags);        // scalar deleting dtor RET 4
// ---- CSWGuiWagerPopup (table 007534C8)
void __fastcall kotor_fastcall_0067cb60(void* popup, void*, void* control);                  // HandleButtonWager (AddEvent 0x27 on BTN_WAGER) RET 4
void __fastcall kotor_fastcall_0067d340(void* popup, void*);                                 // UpdateWagerText RET
void __fastcall kotor_fastcall_0067e150(void* popup, void*, std::int32_t event, std::int32_t down); // HandleInputEvent RET 8
void* __fastcall kotor_fastcall_0067f000(void* popup, void*, void* manager, void* start, std::int32_t max_wager) noexcept(false); // ctor RET 0Ch
void* __fastcall kotor_fastcall_0067f630(void* popup, void*, std::uint32_t flags);           // scalar deleting dtor RET 4
void __fastcall kotor_fastcall_0067f650(void* popup, void*);                                 // ~CSWGuiWagerPopup RET
// ---- CSWGuiPazaakStart (table 007532E8)
void __fastcall kotor_fastcall_0067d3b0(void* start, void*, std::int32_t wager);             // HandleWagerExit(int) RET 4
void __fastcall kotor_fastcall_0067d3d0(void* start, void*, std::uint32_t user);             // HandleQuitDialog (message-box callback) RET 4
std::int32_t __fastcall kotor_fastcall_0067d450(void* start, void*);                         // isOtherCardActiveControl RET
std::int32_t __fastcall kotor_fastcall_0067d480(void* start, void*);                         // isSidedeckCardActiveControl RET
void __fastcall kotor_fastcall_0067e2b0(void* start, void*);                                 // ~CSWGuiPazaakStart RET
void __fastcall kotor_fastcall_0067e400(void* start, void*);                                 // ShowDisabledButtons RET
void __fastcall kotor_fastcall_0067e530(void* start, void*, void* control);                  // OnChosenCardEntered (AddEvent 0) RET 4
void* __fastcall kotor_fastcall_0067f730(void* start, void*, std::uint32_t flags);           // scalar deleting dtor RET 4
void __fastcall kotor_fastcall_0067f750(void* start, void*);                                 // OnPanelAdded RET
std::int32_t __fastcall kotor_fastcall_0067fb10(void* start, void*, std::int32_t kind, std::int32_t slot); // AddChosenCard RET 8
std::int32_t __fastcall kotor_fastcall_0067fd10(void* start, void*, std::int32_t slot);      // RemoveChosenCard RET 4
void __fastcall kotor_fastcall_0067fe80(void* start, void*, void* control);                  // HandleAvailCardDrop (0x17FEB) RET 4
void __fastcall kotor_fastcall_0067fed0(void* start, void*, void* control);                  // HandleChosenCardDrop (0x17FEB) RET 4
void __fastcall kotor_fastcall_006807e0(void* start, void*, void* control);                  // HandleMoveCard (0x27) RET 4
void __fastcall kotor_fastcall_006816f0(void* start, void*, std::int32_t event, std::int32_t down); // HandleInputEvent RET 8
void __fastcall kotor_fastcall_00681890(void* start, void*, std::uint32_t user);             // HandleStartDialog (message-box callback) RET 4
void __fastcall kotor_fastcall_006819e0(void* start, void*, void* control);                  // HandleAcceptButton (0x27) RET 4
void* __fastcall kotor_fastcall_00681a90(void* start, void*, void* manager, std::int32_t max_wager,
    std::int32_t deck, std::int32_t tutorial, std::uint32_t opponent) noexcept(false);       // ctor RET 14h
// ---- PZ-B method tables
const void* __cdecl kotor_pazaak_vtable_00753120();   // CSWGuiActivatedButton (40 slots)
const void* __cdecl kotor_pazaak_vtable_007531c0();   // CSWGuiPazaakCard (40 slots)
const void* __cdecl kotor_pazaak_vtable_00753260();   // CSWGuiTutorial (33 slots)
const void* __cdecl kotor_pazaak_vtable_007532e8();   // CSWGuiPazaakStart (27 slots)
const void* __cdecl kotor_pazaak_vtable_007534c8();   // CSWGuiWagerPopup (27 slots)

// ================================================================= PZ-C: game panel and GUI-manager leaf
std::int32_t __fastcall kotor_fastcall_0040adf0(void* manager, void*, void* panel);          // CSWGuiManager::IsOnTop RET 4
void __fastcall kotor_fastcall_0067d4b0(void* game, void*);                                  // RefreshDisplay RET
void __fastcall kotor_fastcall_0067da30(void* game, void*);                                  // ReconnectSidedeckButtons RET
void __fastcall kotor_fastcall_0067db50(void* game, void*, std::uint32_t user);              // HandleQuitDialog (message-box callback) RET 4
void __fastcall kotor_fastcall_0067dbb0(void* game, void*, std::int32_t enabled);            // SetHandEnabled RET 4
void __fastcall kotor_fastcall_0067dc90(void* game, void*, void* control);                   // HandleXButton RET 4
void __fastcall kotor_fastcall_0067dcc0(void* game, void*, void* control);                   // HandleBlackButton RET 4
void __fastcall kotor_fastcall_0067dcf0(void* game, void*);                                  // HandleFlipHandCard RET
void __fastcall kotor_fastcall_0067dd80(void* game, void*, void* control);                   // HandleHandCardRightClick (0x44) RET 4
void __fastcall kotor_fastcall_0067dda0(void* game, void*, void* control);                   // HandleFlipButtons RET 4
void __fastcall kotor_fastcall_0067e540(void* game, void*);                                  // ~CSWGuiPazaakGame RET
void __fastcall kotor_fastcall_0067e770(void* game, void*);                                  // StartNewGame RET
void __fastcall kotor_fastcall_0067e8f0(void* game, void*, std::int32_t event, std::int32_t down); // HandleInputEvent RET 8
void __fastcall kotor_fastcall_0067e9a0(void* game, void*, std::uint32_t user);              // HandleEndDialog (message-box callback) RET 4
void __fastcall kotor_fastcall_0067ea10(void* game, void*, std::uint32_t user);              // HandleTutorialClose (callback; jump table 0067EBEC/0067EC08) RET 4
void __fastcall kotor_fastcall_0067ec20(void* game, void*, void* control);                   // HandleContinue RET 4
void __fastcall kotor_fastcall_0067ed00(void* game, void*, void* control);                   // HandleStand RET 4
void __fastcall kotor_fastcall_0067ede0(void* game, void*, void* control);                   // HandlePlayHandCard RET 4
void __fastcall kotor_fastcall_0067ef00(void* game, void*, void* control);                   // HandleHandCardDrop (0x17FEB) RET 4
void __fastcall kotor_fastcall_0067ef80(void* game, void*, void* control);                   // HandleHandCardDoubleClick (0x1F9) RET 4
void* __fastcall kotor_fastcall_0067ff80(void* game, void*, std::uint32_t flags);            // scalar deleting dtor RET 4
void __fastcall kotor_fastcall_0067ffa0(void* game, void*);                                  // OnPanelAdded RET
std::int32_t __fastcall kotor_fastcall_00680030(void* game, void*);                          // DoGameSequence RET (loop while nonzero)
void __fastcall kotor_fastcall_00680710(void* game, void*, std::uint32_t delta_bits);        // Draw(float) RET 4
void __fastcall kotor_fastcall_00680790(void* game, void*, void* control);                   // HandleHandcardAction (0x27) RET 4
void* __fastcall kotor_fastcall_006808a0(void* game, void*, void* manager, void* pazaak,
    std::int32_t tutorial, std::uint32_t opponent) noexcept(false);                          // ctor RET 10h
const void* __cdecl kotor_pazaak_vtable_00753358();   // CSWGuiPazaakGame (27 slots)
}

// Exceptions (h1): the six constructors are noexcept(false) so /EHsc keeps the original unwind funclets of their
// callers (operator new + ctor frees the block on a throw: 00681D25 state 0Bh, 00681D74 0Ch, 0068197B, 005F38D3) and
// of the eh_vector_constructor_iterator element constructors. Add the same specifier on every definition.
// Callback ABIs (original): AddEvent handlers are thiscall(object, CSWGuiControl* control) RET 4; message-box
// callbacks (SetCallback(object, func, user) 00624A40 -> box +6C/+68/+70) are thiscall(object, user) RET 4 (006251DF).
// Both equal the __fastcall(self, edx, one dword) shape above; pass fn addresses as `reinterpret_cast<const void*>(&f)`.
