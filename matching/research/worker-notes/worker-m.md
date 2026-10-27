# Worker M matching notes

Range: 0x00640000–0x0067FFFF. Consult Kotor RE's earlier worker-m/k notes,
then current annotations and symbol records.

## Current checkpoint, 2026-10-26

- Previous accepted family: journal delete handlers 0x00652290/0x006522E0/
  0x00652330, module handlers 0x00652840/0x00652860 and spell-like record
  ctor 0x006475C0: six new exact matches.
- Current exacts: module Type14/end-game 0x00652AD0 and
  CSWTlkTable::GetObjectGender 0x00674B40. The latter also has an older
  folded CTlkTable::GetGender caller alias; do not annotate it twice.
- Constructors 0x00676DA0 and 0x006770C0 remain NONMATCH: field stores
  agree, but the original has an EH registration/member-cleanup frame.
  Do not promote these based on stores alone. Prefer the established
  CSWCQuestJournal Windows name over the weaker CSWCJournal Mac hint.

## Next family

A 272-row TODO scan yielded a coherent CSWCMessage pool. Start with the
following 20 unclaimed handlers, then continue compatible message families
from the wider pool instead of treating each dispatcher as a batch:

- Ambient handlers: 0x00654380/3E0/450/4D0/530/5A0/600/680 and dispatcher
  0x00655510.
- Sound-object Play/Stop/ChangePosition: 0x0064F050/0A0/0F0; Sound dispatcher
  0x00656BA0.
- GuiQuickbar dispatcher 0x006637C0 and SetButton 0x00652B00.
- MapPin dispatcher 0x00663810, Enabled 0x00652D00, ReferenceNumber
  0x00652D60; GuiInfoPopup 0x006631E0 and GuiInventory 0x0064DEA0.

References cluster in `src/units/1737/`, `1738/` and `1739/`; the larger
448/512-byte handlers have behavioral references too. Recheck ownership and
callees before implementation. Names/reader hints are hypotheses until the
binary and canonical comparison support them.

## Active audited handler batch, worker-m-r1

Prefetched 268 eligible TODO addresses (entire range, names plus source index),
then selected the related CSWCMessage family. Screened rows are not attempts.
Canonical x86 confirms module +0x48 -> area +0x1DC ambient and virtual slots
0x00..0x20. First checkpoint: five actual new attempts, five NEW exacts:
0x00654380, 0x006543E0, 0x006544D0, 0x00654530, 0x006545A0.
All five matched on the first canonical MSVC7 compile. Family work continues.

Second checkpoint: 21 actual NEW attempts total, currently 16 exact / 5 misses.
New exacts beyond the initial five: 0x00665510; 0x0064F050/0A0/0F0,
0x00656BA0; 0x006637C0, 0x006631E0, 0x0064DEA0, 0x006514C0;
0x00663810, 0x00652D00. Retained misses: 0x00654450, 0x00654600,
0x00654680 (three source shapes, register/vtable scheduling); 0x00652D60
(EH register reuse), 0x00652B00 (EH/stack alignment). Work continues.

Compiler lessons: spelling each identical return case separately preserves
MSVC7's GuiInfoPopup jump table, whereas grouped labels fold into a simple
comparison. Materializing the area's receiver before GetMapPin preserves
argument-push placement, matching MapPinEnabled exactly.

Expansion checkpoint: 26 actual new attempts, 21 NEW exacts / 5 misses.
The four remaining compact journal add/remove/picture handlers all matched
first compile: 0x00651ED0, 0x00651FB0, 0x006520A0, 0x00652210. Popup GUI
0x00654230 also matched first compile. Retained established Journal_TypeN
names from symbols.csv, and CSWCQuestJournal from Windows call evidence.
SetJournalPicture takes WORD despite this caller passing a ReadDWORD result;
canonical callee uses only the low word and normal C++ matches exactly.

Third expansion checkpoint: 36 actual NEW attempts, currently 28 NEW exacts
and 8 retained misses. Additional exacts: 0x0064EB90 (item update),
0x0064F430 (trigger update), 0x0064D3A0 (door delete), 0x00656C90
(delete dispatcher), 0x006632A0 (quest add), 0x00654700 (character download),
0x0066A4A0 (game object dispatcher). Additional misses: 0x006529F0
(unused-resource EH state), 0x00652380 (full journal flag scheduling),
0x0064BF30 (custom-token loop scheduling, under review).

Preserved the Windows GetQuestJournal return type CSWCQuestJournal and reused
already-established CSWCJournal method symbols through a minimal base interface.
No nonzero base adjustment is introduced. SJournalEntry is copied from the
existing matched F declarations, including the 0x3C by-value ABI and bitfields.
The 512-byte Type6 journal handler matches on the first compile. The 512-byte
character download matches on the second shape: common post-switch return
shares string cleanup and a materialized client receiver preserves evaluation.

The initial item-update draft had a nonpolymorphic CSWCObject placeholder;
canonical comparison rejected the resulting primary-base layout. Corrected
it to the established polymorphic 0x10-byte base before accepting the exact.


## Completed substantial handler batch, 2026-10-26

Final NEW work: **36 actual attempts, 29 NEW exact matches, 7 retained
NONMATCHs**. The original 20-handler screened pool is fully attempted; the
batch expanded by 16 compatible journal, popup, object-update, download,
custom-token and top-level game-object handlers. No screened-only rows are
counted as attempts. No prior NONMATCH promotions are claimed.

NEW exact addresses:
0x0064BF30, 0x0064D3A0, 0x0064DEA0, 0x0064EB90, 0x0064F050,
0x0064F0A0, 0x0064F0F0, 0x0064F430, 0x006514C0, 0x00651ED0,
0x00651FB0, 0x006520A0, 0x00652210, 0x00652D00, 0x00654230,
0x00654380, 0x006543E0, 0x006544D0, 0x00654530, 0x006545A0,
0x00654700, 0x00656BA0, 0x00656C90, 0x006631E0, 0x006632A0,
0x006637C0, 0x00663810, 0x00665510, 0x0066A4A0.

Retained NEW NONMATCHs:
- 0x00652380: full-journal flags/copy scheduling, best 46 diff insns;
  two source shapes, 880 compiled bytes vs 864-byte canonical extent.
- 0x006529F0: unused-resource EH state/stack slot, 5 diff insns;
  two shapes, 208 vs 224 bytes.
- 0x00652B00: quickbar common-case ECX/push scheduling, best 17 diff
  insns including switch-data differences; two shapes, 512/512 bytes.
- 0x00652D60: allocation/EH zero/one register reuse, best 27 diff insns;
  three shapes, 448/448 bytes.
- 0x00654450 / 0x00654600 / 0x00654680: ambient branch allocation and
  receiver/vtable hoisting, best 10 / 10 / 6 diff insns; three shapes each.

The custom-token loop matched on its second shape: separate the stream count
from a for-loop countdown local to place the entry test before its EBX copy.

Callee-name audit corrected preliminary map/sound semantic labels before
handoff: use the established CSWCArea::FindObject/AddObject with
CSWCAreaObject and CSWCSoundObject::SetPositionElevated. All direct callees
were cross-checked against canonical call targets and existing declarations.
CExoLocString::AddString retains the existing int-language signature even
though its prior implementation remains NONMATCH. The CSWCJournal /
CSWCQuestJournal naming split predates this batch; the minimal zero-offset
interface retains both existing symbol families pending hierarchy reconciliation.
CGuiInGame::ClosePopUpPanel is a descriptive label for folded empty 0x004015A0;
its original specialized name remains unknown.

Whole owned folder canonical MSVC7 result: **259/284 exact**, up from baseline
230/248; the 18 old misses remain and all 29 new exacts pass. No duplicate
annotations. Only owned source/header/notes files changed; no shared reports,
VS2022 sources, status graphics, proprietary artifacts or pushes.

Next coherent queue: larger CSWCMessage object lifecycle/update handlers
(0x0064CC90, 0x0064C3C0, 0x0064F960, 0x00654B10), inventory/container protocol
(0x0064DEE0, 0x0064FEF0), and module/camera loading (0x0064B8E0,
0x0064DCF0). The active-item property handler 0x00651560 has a native out-of-frame
read documented by the playable reference; defer layout/ownership decisions
until its stack contract is reconciled. Do not revisit constructor EH misses
0x00676DA0 / 0x006770C0 as store-only "matches".

## Active next-family batch, worker-m-r2

Screened 180 of the 232 current eligible M TODO rows, then the 38-row current
CSWCMessage pool. Screening is separate from implementation. First five NEW
attempts: 0x0064CC90, 0x0064F960, 0x00654B10, 0x0064FEF0, 0x0064DCF0.
Current result: three NEW exacts (CC90, 54B10, DCF0), two retained misses.
Prior seven source-shape misses remain untouched.

Creature deletion's complete 512-byte EH path matches, including uninitialized
2DA output slots and dead-creature corpse handling. Use existing inline
Get2DArrays / typed table accessors to preserve the original evaluation tree.
ItemScript_Add (400 bytes) matches after materializing the AI receiver and base
item index. Extended item layouts stay local to this new family interface.

The load-progress handler (432 bytes) matches with nested float casts within
one SetLoadBarProgress expression. This places its second argument before the
five reader calls and forces each original float materialization. Preserve the
established GetTutorialFlag(int) symbol at 0x005EDFF0 even though this caller's
semantic reference labels the five returned bytes as load steps. The native
RunFrame name at 0x00401C10 and SendPlayerToServer_04_03 are preserved.

Bounded retained misses: TriggerUpdate_Delete 0x0064F960 (39 diff insns,
496/448 bytes; three shapes, common-success EH epilogue cloned by MSVC7),
GuiContainer 0x0064FEF0 (4 diff insns, 448/448 bytes; two shapes, one EBP pop
scheduled across the second coordinate store). Both retain complete typed C++.

Second local checkpoint: ten actual NEW attempts, seven NEW exacts, three
retained misses. Additional exacts: Camera 0x0064B8E0 (464 bytes), SetGuiSkillRank
0x0064CE90 (160 bytes), GenericObjectUpdate_Update 0x00651880 (624 bytes),
AreaOfEffectUpdate_Add 0x00651680 (304 bytes). ItemUpdate_Add 0x0064E7B0 is
retained NONMATCH after three shapes (36 diff insns, 784/784 bytes; receiver /
area EBX/EBP allocation plus output-slot scheduling). Full typed reuse/create
behavior is retained; no assembly or raw absolute global loads were added.

Camera matched on its first source compile, including the native uninitialized
camera local on the absent-module path. Generic updates matched after removing
invented initializers from conditionally read portrait/speed/animation fields
and preserving two separate mask-4 tests around the float and WORD readers.
SetGuiSkillRank needs plain char ranks and statement-form bounds checking;
changing signed char or a ternary accessor alters the inlined register tree.
AoE allocation/registration matches directly with the 0x17C typed object,
virtual SetObjectID/SetArea slots and established game-object array interface.

Third local checkpoint: fifteen actual NEW attempts, nine NEW exacts / six
retained misses. Inventory 0x0064DEE0 (1,264 bytes) and token-part helper
0x006500B0 (256 bytes) are additional exacts. Per-case returns preserve the
inventory switch's local success epilogues. Token handling requires separate
CTlkTable/CSWTlkTable globals (7A3A08 / 7A3A0C) and a scoped integer-string
local, avoiding reliance on a constructor's EAX result.

New bounded misses: ServerStatus 0x0064F280 (41 diff insns, 432/432 bytes;
three complete source shapes, common EH success placement), LoadBar 0x0064EDD0
(37 diff insns, 640/640; three shapes, inline field accessor/register trees),
and CreatureUpdate_VisEffect 0x0064C3C0 (261 diff insns, 1744/1840; two shapes,
large EH frame/register and temporary scheduling). The latter retains all
three native dummy allocation paths and the first path's inverted array-status
check. Its extended 0x44C creature declaration is isolated in that translation
unit. Existing GAME_OBJECT_VALID, object-type globals, canonical immediate
OBJECT_INVALID and typed base-class conversions are preserved.

Status format data was read directly from canonical .rdata: "%c%s.%s", 's',
"Module", "Run". The original caller uses established GetGameOver/SetGameOver
symbols even though its behavioral reference labels the state "downloaded".
No invented state names replace those proven symbols.

Fourth local checkpoint / recovery state: **21 actual NEW attempts, 12 NEW
exacts, 9 retained NEW NONMATCHs; no prior NONMATCH promotions.** This expands
well past compact dispatchers into complete object creation and inventory
model/sound paths. Two full add handlers are bounded misses: DoorUpdate_Add
0x0064CF30 (29 diff insns, 1120/1136 bytes; three shapes, locstring temporary
slots and register scheduling), TriggerUpdate_Add 0x0064F4B0 (81 diff insns,
1200/1200; three shapes, zero-vector frame/temporary scheduling). Their best
complete sources are retained. Native strings "Model", "VisibleModel",
"ModelName", "v_mnfrag", "EMPTY" were directly checked in canonical .rdata.

The compatible inventory item expansion adds NEW exact PlayDropSound
0x006466D0, AttachAuxiliaryModel 0x00646830 and SetSpecialAttackModel
0x00646880. The latter two names are descriptive; these addresses had no
established symbols. SetEquippedModelFlags 0x00646BA0 is a complete bounded
NONMATCH (9 diff insns, 224/240 bytes; two source shapes, owner-ID evaluation
and model-override load hoisting). All use typed fields and virtual slots.

Canonical whole M-folder check: **271/305 exact**, compared with accepted
baseline **259/284**. All 259 prior exacts survive, the prior 25 misses retain
their prior results, and all twelve claimed new exacts pass. No duplicate
annotations. The six new files/headers pending this checkpoint remain inside
owned M paths; no reports, non-M sources, VS2022 files or pushes are involved.

NEW exact addresses to date:
0x006466D0, 0x00646830, 0x00646880, 0x0064B8E0, 0x0064CC90,
0x0064CE90, 0x0064DCF0, 0x0064DEE0, 0x006500B0, 0x00651680,
0x00651880, 0x00654B10.
NEW NONMATCH addresses to date:
0x00646BA0, 0x0064C3C0, 0x0064CF30, 0x0064E7B0, 0x0064EDD0,
0x0064F280, 0x0064F4B0, 0x0064F960, 0x0064FEF0.

Next compatible queue: item render orientation 0x00646A30 and secondary
SetArea receiver 0x00646930 (the latter needs a defensible secondary-base
class declaration before attempting); CSWCMessage creature add 0x0064C0A0,
item creation 0x0064B540 / active properties 0x0064E3D0, placeable add
0x0064D500, and the local-character upload overload 0x00678080. The GUI
handler at 0x0064EC00 has two fixed stack arrays with an unbounded byte count;
its native stack contract needs reconciliation before a safe typed attempt.
Do not count disassembly-only screening of these queue addresses as attempts.

## Completed Pazaak GUI batch, worker-m-s1 (2026-10-26)

Family: the Pazaak minigame GUI cluster 0x0067CA50-0x0067FFA0 plus the
folded button destructor 0x00644A30. Shared header `include/m/sw_gui_pazaak.h`
(CSWGuiActivatedButton, CSWGuiPazaakCard, CSWGuiTutorial, CSWGuiSpeedButton,
CSWGuiWagerPopup, CSWGuiPazaakStart, CSWGuiPazaakGame, CSWPazaak/CPazaakPlayer/
CPazaakCard). Control/panel layouts follow d/gui_control.h and the message box
from l/sw_gui_tutorial_box.h; all member offsets were checked against the
playable reference (src/minigames/claude_pazaak_*).

Result: **72 actual NEW attempts, 67 NEW exact, 5 retained NONMATCH.**
Folder: 271/305 -> **338/377**. No duplicate annotations; no prior misses touched.

Files: sw_gui_pazaak_card.cpp (15/15), sw_gui_pazaak_tutorial.cpp (8/8),
sw_pazaak_turns.cpp (2/2), sw_gui_pazaak_wager.cpp (13/13, includes the
speed button), sw_gui_pazaak_start.cpp (10/13), sw_gui_pazaak_game.cpp (19/21).
Several exact functions are absent from functions.csv (e.g. 0x0067CAC0,
0x0067CB70, 0x0067D260, 0x0067E860); annotations add them as extents.

Retained NONMATCHs:
- 0x0067E400 ShowDisabledButtons (42 diffs, 256/304): inline-copy register
  choice (ebx sete / id CSE); four shapes.
- 0x0067F750 Start::OnPanelAdded (9 diffs, 944/960): the first SetCard
  argument is built in place (`sub esp,8; mov [$T],esp`) in the original; a
  temporary ctor, inline returner, dtor or copy ctor did not reproduce it
  without breaking the second SetCard call.
- 0x0067FB10 AddChosenCard (7 diffs): final all-chosen loop counter/pointer
  register swap only.
- 0x0067EA10 HandleTutorialClose (18 diffs): the original keeps the
  mgs_playside CResRef in its own slot above the help arrays; named-in-case,
  temp and function-scope arrays all mis-order or overlap the slots.
- 0x0067D4B0 RefreshDisplay (124 diffs, 1328/1408): card comparison copies and
  induction variables; operator!= / GetBoardCard forms tried.

Compiler lessons (all reproduced exactly here):
- An inline accessor's result is materialised in EAX: `GetPazaak()->m_cPlayer.
  GetTotal()` gives `mov eax,[..]; lea ecx,[eax+8]`, `pCreature->GetStats()->`
  gives `mov eax,[eax+0xa74]`; the direct member path gives `mov ecx; add ecx`.
- Virtual calls on member sub-objects (`m_btnSwitch.SetEnabled(x)`) are NOT
  devirtualised by MSVC7 here; they read the member's vptr in place.
- Tail merging keeps the copy from the earlier source case: order switch cases
  so the case whose epilogue the original keeps comes first.
- Strings: named block-scoped `{ CExoString sName("X"); InitControl(&c, sName,
  1); }` and `{ CResRef cLayout("x"); StartLoadFromLayout(cLayout); }` give
  `lea` argument pushes and the original frame size; temporaries push EAX.
- `a + " " + n + "\n" + b` with implicit CExoString conversions reproduces the
  original temporary construction order (all literal/int temps first), unlike
  explicit CExoString(...) operands.
- SEH-free ctor with frame: the dead-`new` sized CExoArrayList ctor again.
  The wager popup's unused list needs an unconditional `delete[]; element = 0`
  dtor (explicit specialisation for CExoArrayList<int>), the tutorial's uses
  the `if (element)` form.
- `CPazaakCard cTmp; cTmp.m_nValue = -1; cTmp.m_bFlag = 0; m_cCard = cTmp;`
  gives the `or eax,-1 / xor ecx,ecx` register stores; `CPazaakCard(-1, 0)`
  assigned gives the same; direct member stores give immediates.
- Bitfield flag via an inline setter (`m_lblFace.SetFlag5(1)`,
  `SetNavigable(0)`) keeps the member pointer register (load/or/store).
- abs(): declare `extern "C" int __cdecl abs(int)` + `#pragma intrinsic(abs)`
  for cdq/xor/sub. sin(): same with `#pragma intrinsic(sin)`; cast the fsin
  result to float to keep dword constants in the following fadd/fmul.
- Vector arithmetic: `m_vColor * s + vColor * (1 - s)` with inline
  operator*/operator+ returning `Vector(x, y, z)` matches the x87 order.

Naming: CSWGuiWagerPopup::OnLessButton/OnMoreButton/OnWagerButton and
CSWGuiPazaakStart::IsAvailCard/IsChosenCard are descriptive. 0x00644A30 is
annotated as ??1CSWGuiSpeedButton (folded identical ~CSWGuiButton thunk) and
0x0067EFA0 as ??_GCSWGuiSpeedButton (also CSWGuiActivatedButton's slot 0).

Next in-range queue: the CSWGuiPazaakGame tail and constructor live in bucket
k (0x00680030+). In bucket m: journal2 CSWCJournal/CSWCWorldJournal cluster
(0x006760F0, 0x00676260, 0x006763D0, 0x00676530, 0x00676610, 0x00676B00,
0x00676C20, 0x00676E60, 0x00676F00, 0x00676FA0, 0x00677000, 0x00677130,
0x00677300, 0x00675F10), then module camera (0x006401D0..0x00641760).

## Completed CSWCModule camera batch, worker-m-s3 (2026-10-26)

The journal2 queue from s1 was already annotated on main (only 0x00676610
CSWCJournal::Add remains, 1,264 bytes), so this round took the next queue
item: the CSWCModule camera cluster 0x00640090-0x00644170 (nwcmodule.o).

Result: **22 actual NEW attempts, 19 NEW exact, 3 retained NONMATCH, plus 1
prior NONMATCH promoted** (0x006412B0 IsCameraAnimated: the plain
`if (m_pCamera) return m_pCamera->GetEventTime(...) != 0;` form matches).
Folder: 352/394 -> **372/416**. No duplicate annotations.

Files: sw_c_module_camera_controls.cpp (9/9), sw_c_module_camera_modes.cpp
(5/5), sw_c_module_dialog_camera.cpp (4/4 incl. promotion),
sw_c_module_lifecycle.cpp (ctor/dtor 2/2), sw_c_module_camera_follow_modes.cpp
(1/2), sw_c_module_camera_restore.cpp (0/1), sw_c_module_camera_next_position.cpp
(0/1). Shared header include/m/sw_c_module_camera.h (layouts follow
n/sw_behavior_camera.h; lifecycle TU declares the full polymorphic layout).

NEW exact: 0x00640090 AcclTurnCamera, 0x006401D0 ZoomCamera, 0x00640240
ScrollCamera, 0x00640700 UpdateCameraModeChaseCam, 0x00640A80
UpdateCameraModeBG, 0x00640F70 SetCameraModel, 0x00641010 SetAnimatedCamera,
0x006412F0 InitializeShotCamera, 0x006413C0 SetFreeLookCamera, 0x00641540
EnterCombatMode, 0x00641610 SetNavigateCamera, 0x006416F0 ShakeCamera,
0x00641730 BumpCamera, 0x00641760 UpdateCameraBumpAndShake, 0x006419A0
UpdateCameraFollow, 0x00641CB0 ~CSWCameraSettings, 0x00641CD0 ~CSWCModule,
0x00643F40 CSWCModule ctor, 0x00644170 UpdateCameraCollision.
Names other than symbols.csv ones (InitializeShotCamera, EnterCombatMode,
SetCameraModel, ctor/dtor) are descriptive, from the playable reference.

Retained NONMATCHs:
- 0x00640B90 UpdateCameraModeEQ: stack-slot tie only (player spill slot sits
  between far/near clip outputs); 4 local-order shapes tried.
- 0x00641DC0 RestoreCamera (50 diffs, 1296/1312): MSVC7 merges the combat
  early `return 1` into the final epilogue; the original keeps a separate
  EH epilogue there. if/else restructure did not help.
- 0x006402F0 GetNextCameraPosition (56 diffs, 832/848): pow/clamps/flow
  agree; original copies the normalized quaternion into a named local before
  ToDirection and adds m_vCameraUp first for y/z.

Compiler lessons (reproduced here):
- `CClientExoApp* pApp = g_pAppManager->m_pClientExoApp; pApp->F(arg)`
  loads the app manager before the argument (ECX app / EDX arg). The direct
  `g_pAppManager->m_pClientExoApp->F(m_x)` form loads the argument first.
  Fixed three functions at once (SetFreeLookCamera, SetAnimatedCamera tail,
  UpdateCameraFollow).
- `mov al,[eax+0x6d]; cmp al,3` comes from `unsigned char nMode = ...;
  if (nMode == 3)`; the direct field compare gives `cmp byte ptr`.
  `int nMode = ...` gives movzx into a register (UpdateCameraFollow).
- if/else-if chains on the int mode with code laid out default-first are
  a `switch` (case 3/5/7/default); EH states number in source order.
- MSVC7 sinks stores: a global read hoisted to the top of a long store run
  (ctor 0x00643F40) comes from writing that assignment *first*.
- GobRef/CExoArrayList::Remove must be `throw()` or destructors get an extra
  EH state store (~CSWCModule).
- x87 `fxch st(1); fxch st(1)` pair before fmulp: separate float locals
  (`float fEnvelope = (float)cos(..) + 1.0f; float fWave = (float)sin(..);`).
  (float)-casting the intrinsic result keeps dword constants.
- Clamp code that re-evaluates the min after the max test and assigns via
  integer moves is the classic macro pair
  `MAX(lo, MIN(v, hi))` with `#define MIN(a,b) ((a)<(b)?(a):(b))`.
- Early `return` in a void function placed before a common tail vs.
  `if (cond) {...} else { m_n84 = 0; }` decides whether the tail block lands
  at the end (ChaseCam fabs <= 1 path).
- `Vector v(0,0,0); if (... <= eps) return; v = Get(); v += d; Set(v);`
  reproduces ScrollCamera's dead zero stores and jnp test.

Next in-range queue: the three big x87 collision functions 0x006422E0
(4,304 B), 0x006433B0 (2,960 B), 0x006441D0 FindClearView (1,152 B) finish
this family but are x87-scheduling heavy. Other open rows: 0x00676610
CSWCJournal::Add (1,264 B, journal2), stats/feedback helpers around
0x0064B2B0-0x0064B4F0 and ctor 0x0064B2D0 (no playable definition), the
0x00678xxx-0x0067Bxxx sound/player-control/GUI rows.

## Completed load screen / main menu batch, worker-m-s5 (2026-10-26)

(The s4 round - creature stats, CSoundSet, CanChooseFeat, AppendForceActions -
was merged without a notes section; see its WIP commits.)

Family: the title-screen GUI cluster 0x0067A6D0-0x0067CA4F: CSWGuiLoadScreen
(vtable 0x00752DC0), CSWGui3DSceneView (0x00752E30) and CSWGuiMainMenu
(0x00752F70). Shared header `include/m/sw_gui_main_menu.h` (self-contained;
copied/trimmed from m/sw_gui_pazaak.h and l/sw_gui_upgrade.h). Files:
sw_gui_load_screen.cpp (6/7), sw_gui_main_menu.cpp (20/23),
sw_gui_main_menu_modules.cpp (1/1).

Result: **31 actual NEW attempts, 27 NEW exact, 4 retained NONMATCH.**
Folder: 421/467 -> **448/498**. No duplicate annotations.

Missing from functions.csv (annotated as extents): 0x0067AC10 MainMenu::Draw,
0x0067ACB0 3DSceneView::Draw, 0x0067AFB0/B1A0/B250/B2F0 the four menu button
handlers, 0x0067B450/B470 enter/exit hilight, 0x0067B490 LaunchDashboard,
0x0067B4A0 OnQuitButtonPressed, 0x0067C4B0 OnWarpButtonPressed.

Names: handler names follow the playable reference (OnNewGamePicked,
OnLoadSaveGame, OnMoviesPicked, OnOptionsPicked, OnWarpButtonPressed,
OnQuitButtonPressed, LaunchDashboard, OnEnterButton, OnExitButton).
Descriptive: CSWGuiLoadScreen::AppendHintToDialogBuffer (0x0067AA80; callers
call it CSWGuiPanelR5::Destroy), CSWGuiMainMenu::LoadFromLayout,
ClearModuleNames, OnModuleSelected, PopulateModules, ShowWarpList,
CSWGuiModuleEntry (0x18-byte list element whose dtor folds into 0x0059EA30).
Callers name 0x0067A6D0/0x0067A710 CSWGuiLoadScreenS1 (placeholder class);
here the real class CSWGuiLoadScreen is used. The load screen has no user
destructor (0x0067AB80 is compiler generated: no vptr store). 0x0067B4D0 is
the out-of-line ~CExoArrayList<CSWGuiModuleEntry> used by EH unwind funclets
(explicit instantiation emits it). CSWPartyTable::ResetCurrentSessionStartTime
ignores ECX, so it is declared static here.

Retained NONMATCHs:
- 0x0067AA80 AppendHintToDialogBuffer (6 diffs): app-manager load in ECX vs
  EAX (string address EDX); implicit "", named sEmpty and pApp-local tried.
- 0x0067B1A0 OnLoadSaveGame (8 diffs): post-AddPanel flag test uses EDX/EAX
  vs EAX/ECX; explicit mask, base pointer, int AddPanel tried.
- 0x0067B9D0 OnModuleSelected (16 diffs, rest exact): pControl->m_n4c is
  loaded into EAX before `mov edi,ecx` in the original; return/if-block tried.
- 0x0067B6C0 OnPanelAdded (27 diffs, 576/592): the __int64 required-space
  value keeps its high word in a stack home and `this` lives in EBP in the
  original; ours spills this. Signed/unsigned, declaration order and inline
  +2 forms tried.

Compiler lessons (all reproduced here):
- Inline constructor arguments evaluate right to left: the load screen's
  `SetExtent(CSWGuiExtent(0, 0, w, h))` loads height first and fixes a
  register rotation that persisted through the rest of a 688-byte ctor.
  Field-by-field stores (any order) or aggregate init never matched.
- Visibility bit via an inline setter `void SetVisible(int b) { m_bVisible =
  b; }` keeps the member's address in a register / RMW `or [mem], eax`;
  direct `m_nFlags |= 2` or bitfield stores get split into load/op/store and
  scheduled pairwise. Fixed HandleInputEvent, ShowWarpList and the 1,424-byte
  ctor at once (ctor register allocation of the button pointers too).
- Implicit CExoString->CResRef conversion (`Exists(sModule, ...)`) builds the
  temporary before the other arguments are pushed; an explicit `CResRef(s)`
  pushes the constants first.
- Explicit `CExoString("x")` argument temporaries push EAX; implicit
  conversions from a literal push via LEA (also for `s + ":modules"`).
- `int nPos = s.Find(CExoString("_s.rim"), 0); if (nPos != -1)` keeps the
  int across the temp's dtor; an inline comparison produces setne.
- EH state order follows source order: put the `zzz` else-branch first
  (`if (n >= max) {...} else {...}`) to number its temporary first.
- A new'd CSWGuiButton is constructed inline (CSWGuiNavigable ctor + vptr +
  members) in PopulateModules but members call 0x0041C0F0 elsewhere: the
  modules TU defines M_GUI_BUTTON_INLINE_CTOR for an inline `CSWGuiButton() {}`.
- `__forceinline` CExoArrayList::AddInline was needed in the large function;
  plain inline was not expanded. The list dtor must be `delete[] element;
  element = 0;` (no `if`): pointer lists free unconditionally, class-type
  lists get the vector-delete null check anyway, and dead lists vanish.
- A single-expression `GetCornerSize() * 2 + GetHeight()` spills the first
  call result to the stack like the original; a named local keeps it in EDI.
- Declaring loop counters before a struct copy schedules their zero stores
  before the copy's last store.
- CExoIni is 4 bytes: it is built in the dead parameter slot.

Next in-range queue: remaining M TODO rows are mostly large: CSWCMessage
handlers 0x00650B90, 0x00652F20, 0x006501B0, 0x00653850, 0x00655E40,
0x00663840 (no playable definitions), journal GUI 0x00644790/0x00645610/
0x00645100/0x00645330/0x00645E00/0x006456E0 (src/units/1746), camera
collision 0x006422E0/0x006433B0/0x006441D0, player control 0x00679870, and
small leftovers 0x006460B0, 0x00646080, 0x0064B4D0/0x0064B4F0, 0x006449D0.

## Unique object-ID array insertion, 2026-10-26

- Screened the 56 remaining indexed M candidates. Reconstructed 0x006449D0
  as CExoArrayList<unsigned long>::AddUnique: byte-identical 96/96. The Mac
  debug map supports AddUnique (1/2); native code scans the entire list,
  counts duplicates, then grows capacity to 16 or doubles it before appending.
  Allocate resolves to the existing 0x005E03C0. Five other small candidates
  require further ownership or lifetime evidence before naming.
- 0x00644790 CSWGuiJournalItemEntry::SetEncouraged matches 192/192 bytes.
  The flag is a one-bit field; native toggles border alpha and two color
  vectors. Global color addresses were checked against the existing map.
- 0x00645610 CSWGuiInGameJournal::MarkNewQuestsObserved matches: its virtual
  +0x6c lookup clears bit 2 of each viewed entry and clears the ID list.
  The coarse function extent includes the next routine; matcher confirms
  the 96-byte emitted body against its corresponding original instructions.
  0x00646070 OnPanelRemoved tail-calls that routine and matches 16/16 bytes.
- 0x00644960 CSWGuiInGameJournal::GetQuestEntryAt matches 112/112 bytes;
  it selects active/done list according to the GUI flag and bounds-checks
  before calling existing entry getters. The two count accessor names are
  descriptive folded-getter aliases, not independently recovered names.
- 0x00645670 CSWGuiInGameJournal::Draw matches 112/112 bytes. Two separate
  GetInGameGui calls mirror the native short-circuit path before panel draw.
- 0x00644730 journal black-button handler has exact executable instructions,
  but remains NONMATCH (11) because the original uses int3 padding and the
  object output uses nop padding. The +0x60 virtual action name is provisional.
- 0x00676DE0 CSWCWorldJournal::~CSWCWorldJournal matches 128/128 bytes.
  The two array member destructors use unconditional delete[]; MSVC emits
  the needed null check only for the nontrivial entry array.
- 0x0064B4D0 and 0x0064B4F0 are exact 32-byte vector copies to projectile
  offsets +0x114 and +0x1B0. Target/source names follow the existing verified
  projectile layout; the latter native body is also used by server objects
  for their spell-target position and is a folded implementation.
- 0x0066BFB0 CSWMiniGameObjectArray::GetObjectByTag matches 176/176 bytes.
  It scans the 255-entry object table, compares each object's virtual
  GetModelName() result with the requested string, and returns the first match.
- 0x0066C190 CSWMiniGameObject::RunHitBulletScript matches 320/320 bytes.
  It runs the hit script if present, otherwise applies damage to a compatible
  follower and plays the hit sound. Play3DOneShotSound arguments follow the
  Mac debug signature (2/2), including float and resource-reference types.

## Client admin message, 2026-10-26

- 0x00675690 retained NONMATCH (9). It allocates a packet, prefixes 's',
  copies the message and sends it to PLAYERID_SERVER. Two source shapes gave
  11 and 9 differing instructions; null-return block placement and memcpy
  register scheduling remain. Class/method name and void return are
  descriptive pending independent ABI evidence; its known caller ignores EAX.

## Completed s7 batch (2026-10-27)

Folder: 469/526 -> **482/547**. 21 new annotations: **12 NEW exact**, 9 retained
NONMATCH, plus one promotion (0x00644730, previously the NONMATCH
`OnBlackButtonPressed` with int3/nop padding, is really the vcall thunk
`??_9@$BGA@AE`; sw_gui_journal_black.cpp was removed and the thunk is now
annotated in sw_gui_in_game_journal.cpp). No duplicate annotations.

NEW exact:
- CSWGuiInGameJournal (include/m/sw_gui_in_game_journal.h, src
  sw_gui_in_game_journal.cpp): ~CSWGuiInGameJournal 0x00644850,
  OnControlEntered 0x00645100, PopulateItemListBox 0x00645330, OnPanelAdded
  0x00645E00, vcall thunk ??_9@$BFM@AE 0x00644720 (OnYButtonPressed +0x5c;
  0x00644720/0x00644730 are not in functions.csv).
- CSWMiniPlayer::ClampPosition 0x0066CB40 (sw_mini_game_object.cpp).
- CSWCMiniGame::CSWCMiniGame(CSWCArea*) 0x00671C40 (sw_c_mini_game_ctor.cpp;
  CResHelper base ctor inlined via M_INLINE_RES_HELPER_CTOR).
- CSWCMessage: HandleServerToPlayerActiveItemProperties 0x0064E3D0,
  HandleServerToPlayerActiveItemPropertyUpdate 0x00651560
  (sw_c_message_item_properties.cpp), HandleServerToPlayerCreatureUpdate_Add
  0x0064C0A0 (sw_c_message_creature_add.cpp), CreateItem 0x0064B540
  (sw_c_message_create_item.cpp), HandleServerToPlayerExportReply 0x006634A0
  (sw_c_message_export_reply.cpp).

Retained NONMATCH (best attempts, reasons in the source comments):
- 0x00644A40 journal ctor (50): only the else branch of the swap-text if/else
  allocates eax/ecx where the original repeats the if branch's ecx/edx.
- 0x006456E0 journal HandleInputEvent (70): the two cases' sText locals get
  each other's stack slots; one `!flag` register rotation.
- 0x006740D0 CSWMGGunBank::SetGunModel (5): frame 0x58 vs 0x48 (original
  shares the first CResRef temp with the gun-case temp).
- 0x0066CD30 CSWMiniPlayer::GetInertia (6, descriptive name): frame slot reuse.
- 0x0064EC00 CSWCMessage::HandleServerToPlayerLogin (19): frame layout only.
- 0x00678080 SendPlayerToServerLogin_LoginLocalCharacter(CResRef, int) (15):
  frame layout only.
- 0x00644650 CSWCModule::Update (3): load/store order of the two clear-view
  outputs.
- 0x0064D500 HandleServerToPlayerPlaceableUpdate_Add (52): register rotations,
  flag-store scheduling, hoisted hitcheck push.
- 0x0064BAB0 HandleServerToPlayerArea (119): original has an 8-aligned EBP
  frame; ours does not (trigger unknown).

Compiler lessons (verified this batch):
- String concatenation `tlk(a) + " " + tlk(b)` with an *implicit* literal
  conversion builds the separator temporary first (before both
  GetSimpleString calls), exactly like the original; an explicit
  `CExoString(" ")` is evaluated after the right operand. Same for
  `InitControl`-style named locals: `{ CExoString sName("X"); Fn(sName); }`.
- Choosing between two member functions with a conditional,
  `(p->*(cond ? &C::A : &C::B))(args)`, reproduces the original's
  `mov eax, offset A; jne; mov eax, offset B; call eax`.
- `pPanel->AddEvent(0x27, this, &CSWGuiPanel::VirtualFn)` emits the shared
  `??_9@$B..@AE` vcall thunks; annotate them with SYMBOL.
- A CSWGuiListBox method named SetActiveControl silently overrides the base
  virtual; call the non-virtual 0x0041C160 as SelectControl.
- Inline accessors decide object-vs-argument load order:
  `g_pAppManager->GetClientExoApp()->F(x)` loads the app before x, the direct
  member after. Two accesses to `m_pInternal` matched only as
  `GetClientExoApp()->GetInternal()` in both the test and the store.
- Float member copies that the original does via fld/fstp come from inline
  getters (`pOld->GetBirthrate()`, `GetBump()`); plain member copies use mov.
- `if (x == 1) F(1,0,0,0); else F(0,0,0,0);` gives the original's merged call
  with `push eax` / `push 0`; a `== 1` argument gives setcc code.
- Stack slots of locals follow the order of their *later uses*, not their
  declaration: swapping two stores (ActiveItemProperties) or keeping the
  eight placeable flag stores in read order fixed slot assignments.
- `nType == 1 ? sImage : sResRef` (lvalue conditional) selects the address
  without copies.
- Temporaries in `CExoFile(path, 0x7df, "wb")` inside `new` get the
  conditional-destruction flag automatically.

Next in-range queue: 0x00651C10 HandleServerToPlayerCheat_DoNasty (704,
quaternion math), 0x00676610 CSWCJournal::Add (1,264), 0x00678280
SendPlayerToServer_LevelUp (1,440), 0x0066D640 CSWMiniPlayer::Control and
0x0066EDA0 DoFollowerHitCheck (x87), 0x006723D0 CSWMiniGame::Load (2,784,
GFF reads, structured), 0x006710E0 CSWMiniGame::Control (x87), the big
CSWCMessage handlers 0x00650B90/0x00652F20/0x006501B0/0x00653850/0x00655E40/
0x00663840/0x00668640/0x00667CC0, and camera collision 0x006422E0/0x006433B0/
0x006441D0. Revisit the frame-layout NONMATCHes once the 8-alignment/slot
rules are better understood.
