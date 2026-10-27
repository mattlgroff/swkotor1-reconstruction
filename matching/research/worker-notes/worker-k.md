# Worker K round s5 — main interface, CSWCAnimBase, wield thunks

Branch `claude/worker-k-s5` (local only). Folder K canonical
`kotor-match -q -j 2 matching/src/k`: **488/542 -> 495/553**. New
annotations 11: NEW exact 5, new NONMATCH 6; prior NONMATCH promotions 2.
No prior MATCH regressed and no old NONMATCH distance changed (checked
against the merged status.csv). No shared reports or other buckets touched.
(Round s4 left no notes; its work is the thunk / trigger / floaty-text
files merged in daed0825.)

## NEW exact (5)

- 0069AA20, 0069AFC0: the two missing CSWCAnimBaseWield vtordisp thunks
  (DetachHeadItem / AttachHeadItem are empty overrides at this layer,
  linker-folded to 004015A0, so one thunk covers both slots 0x88 / 0x8c;
  GetModel thunk). Not in functions.csv.
- 00686BA0 CSWGuiMainInterface::Update (panel slot 0x38)
  (`sw_gui_main_interface_update.cpp`).
- 006898A0 CSWGuiMainInterfaceChar::Initialize
  (`sw_gui_main_interface_char_init.cpp`).
- 0069E420 CSWCAnimBase::PlayAnimSetOnModel (`sw_c_anim_base_play_set.cpp`).

## Promoted (2)

006851B0 CSWGuiMainInterfaceChar::SlideIn (`t = fTime * 4.0f; ...;
t = 1.0f - t;` puts the fsubr after the extent copy), 0068FDD0
CRumblePattern::GetIntensity (last test written `if (!(f < x)) return 0;`).

## New NONMATCH (6)

0068A010 UpdateQueue (stack-slot order), 00689D80 UpdateActions (40, same
ties as PopulateMenus), 0068B9D0 CSWGuiMainInterfaceAction::Initialize
(register rotation), 00689610 CSWGuiTargetActionMenu::UseAction (46,
register), 00686FB0 GetActionIcon (cl cross-jumps the two "power" arms into
the node-0x0f tail, 32 bytes short), 0069E620 CSWCAnimBase::GetAnimationName
(54, register allocation only).

## Header changes (k only)

- CSWGuiControl: real 0x14-0x44 fields (m_pParentControl, m_nToolTipStrRef,
  m_sToolTip, m_nKeyBinding), bit 2 m_bNavigable, AddEvent /
  AddChildControl, virtual 0xa0 TimedUpdate; `class __single_inheritance
  CSWGuiPanel;` forward declaration keeps 4-byte PMFs before the panel
  class is defined.
- CSWGuiMainInterfaceAction::SetIcon second parameter is `const CExoString&`
  (the action's name: UpdateActions passes the entry address); mangled name
  changes from `...ABVCResRef@@HHH@Z` to `...ABVCResRef@@ABVCExoString@@HH@Z`.
- CSWGuiButtonToggle bit 0 at 0x1c8 (m_bSelected); CSWSCreature now
  polymorphic (GetDead 0x94) with pad shrunk by 4; CSWParty member slots
  (CSWPartyMemberData::SetCombatMessage 006345E0).
- CSWCAnimBase: GetAnimationName(unsigned short), slot 0xe0
  ServerToClientAnimation(unsigned short) (matches l's header), CExoString
  copy / char ctor, operator+, IsEmpty, operator==(const char*).

## Lessons (MSVC 7.0)

- `f != -1.0f` against a constant compiles to an integer compare of the
  bits (and the constant stays in a register for the later store).
- Sequential bitfield stores to many controls: the inline `SetVisible()`
  call keeps them unscheduled; direct bitfield assignments are interleaved.
- An inline wrapper (`pParty->SetCombatMessage(i, id)`) placed the push
  between the imul and the lea as the original.
- A float alpha `float a = 1.0f; f = f / d; if (f < 0.5) a = f + f;` gives
  the fld 1.0 / fstp st0 pattern.
- `pButton = &m_btnChar; pButton->m_nFlags &= ~4; pButton->AddEvent(...)`
  fixed the whole tail of the char Initialize (ebx holds the button).
- `bTarget == 0` written in each InitControl call (not a local) matches the
  late sete; pointer locals for the up / down / icon buttons fix the frame
  size of the action Initialize.
- `if (++nIndex != 0)` reproduces `inc esi; je` (a `!= -1` test is
  jump-threaded away); a named `CExoString sNum(szNum)` reproduces the
  temporary slot order of `"cut" + sNum + sSuffix`.
- GetActionIcon: `int nId = 0xc; if (pNode) nId = pNode->m_nActionId;`
  gives `mov ecx, 0xc; je; mov ecx, [eax]`; GetServerExoApp() fixed the
  item arms' registers.

## Next in-range queue

Everything left in 0x00680000-0x0069FFFF is >= 592 bytes. Integer-heavy
and plausible: 0069E8D0 SetAnimationInternal (592), 00687860 UpdateTarget /
UpdatePortraits (1504), 0068A310 UpdateParty (2048), 0068C100
CSWGuiMainInterface ctor (4784, long but mechanical: member ctors, EH
states, InitControl calls), 00693F60 / 00694D50 / 00695180 map ctors (border
param temporaries). x87-heavy (skip unless needed): 006916B0
CSWCTrigger::GetCenter, 006889C0 UpdateStickyTarget, 00688100 fog of war,
00686090 PositionMenu, 0069EDC0 / 0069F630 GetBoundingRect. Near misses
worth one more idea: 0068A010 (slot order), 00691CC0 AddToRenderList (the
original zeroes TriggerColor from a register before the pushes).

# Worker K round s3 — map panels, pazaak, main-interface callbacks

Branch `claude/worker-k-s3` (local only). Folder K canonical
`kotor-match -q -j 2 matching/src/k`: **376/417 -> 405/454**. New
annotations 37: NEW exact 29, new NONMATCH 8 (4 from the first s3 session:
the three map-note list functions and DisplayPlanet). Prior NONMATCH
promotions 0; no prior MATCH regressed. No shared reports, functions.csv or
other buckets touched.

## NEW exact (29)

- Map panels (swguiingamemap.o): 00695BF0 ~CSWGuiInGameMap, 006958C0
  ~CSWGuiInGameGalaxyMap (`sw_gui_map_dtors.cpp`), 006942D0 ~CSWGuiMapHider
  (`sw_gui_map_hider_dtor.cpp`), 00692810 CSWGuiInGameMap::Draw, 00692930
  HitCheckMouse (`sw_gui_in_game_map_draw.cpp`), 00693BA0 OnPanelRemoved,
  00693BC0 HandleInputEvent; galaxy 00694B40 OnPanelAdded, 006927D0
  OnPanelRemoved, 00695980 HandleInputEvent.
- 00695E10 CSWCSpellCastVisual::Update; CSWCAnimBase 0069DE90
  SetGroundTilt (`sw_c_anim_base_tilt.cpp`), 0069EB20 SetAlpha (vtable 0x94),
  0069EC00 RefreshHilite (`sw_c_anim_base_hilite.cpp`).
- Pazaak (`include/k/sw_gui_pazaak.h` = copy of m's header + 0x0068xxxx
  methods): CSWGuiPazaakGame 00680710 Draw, 00680790 HandleHandCardAction;
  CSWGuiPazaakStart 006807E0 HandleMoveCard, 006816F0 HandleInputEvent,
  006819E0 HandleAcceptButton.
- CSWGuiMainInterface callbacks: 006856C0 OnPersonalActionFocus, 00688490
  OnTargetActionFocus, 006884B0 / 00688520 OnTargetPrev/NextClicked,
  0068B970 OnActionClicked, 00688590 OnPauseClicked, 00688610 OnSoloClicked,
  00688640 OnStealthClicked, 006883C0 OnMenuButtonPressed, 00687700
  SetCombatMessage (all but the last two descriptive names).

Not in functions.csv (annotated anyway): 00694B40, 00692930, 006927D0,
00695980, 00693BC0, 00693E70, 0069EB20, 00680790, 006819E0, 006856C0,
00688490, 006884B0, 00688520, 0068B970, 00688590, 00688610, 00688640,
00688690. Function-table spans hiding further starts: 006943D0
CSWGuiMapHider::Draw (inside 006942D0), 0069BA30 / 00699890 / 0069A330
vtordisp thunks, 00699D00 / 0069AA10 thunks, 00691B00 CSWCTrigger
GetReticlePoint.

## Retained NONMATCH (8)

006933A0 DisplayPlanet (34; CExoString stack-slot order and EDI/EBX swap),
00692C70 / 00692E80 / 00693090 map-note list (20 / 38 / 38, first s3
session), 00693E70 OnMapNoteClicked (19, this/oid EDI/EBX swap), 00681890
CSWGuiPazaakStart::HandleStartDialog (9, EAX/EDI arg scheduling), 0068BF50
CSWGuiTargetActionMenu::Initialize (7, byte-load scheduling), 00688690
OnCharacterClicked (6, original re-tests the DEC result).

## Lessons (MSVC 7.0)

- Compiler-generated panel destructors that store no vtable: declare the
  class `__declspec(novtable)` with sized member classes in declaration
  order; the empty user dtor then matches.
- Jump tables are compared as data: a wrong case label shows up only as
  index-byte diffs after the code (HandleInputEvent 0x29 vs 0x2a).
- Switch arms that each call the same function with different arguments
  are tail-merged by cl; write the call in every arm (SetCombatMessage)
  rather than selecting a pointer first.
- `cond ? a : b` assigned to an int gives `neg/sbb/add`; an if/else gives
  `test; mov; jne; mov` as the original.
- `CExtent ext = member.m_extent;` before a call produces the 16-byte copy
  the original keeps (HitCheckMouse); a quaternion result needs a named
  local before the by-value virtual call (SetGroundTilt).
- A dead call chain whose result feeds an empty inline (`pArea->X(arg)`)
  keeps exactly the calls; hoist `pArea` into a local to put the getter
  before the argument call (RefreshHilite).
- Value passed as `Vector` by value (SetColorShifting / SetIllumination)
  explains the 12-byte stack copies; `Vector(1,1,1)` temporaries give the
  stack-store-then-copy pattern.
- `== 0` on a hoisted call result gives `sete`; `!call()` inline in an
  argument gives `neg/sbb/inc` and moves the call after the pushes.
- Inline accessor (GetClientExoApp) again fixed an EAX/ECX choice.

## Next in-range queue

Remaining todo in bucket are mostly large: 00693650 CSWGuiInGameMap
OnPanelAdded (1360), 006943D0 CSWGuiMapHider::Draw (1904, x87 heavy),
ctors 00693F60 / 00694D50 / 00695180 (EH + border params), pazaak 00680030
DoGameSequence (1760), 006808A0 / 00681A90 ctors; anim base 0069E420
PlayAnimSetOnModel (512, switch), 0069E620 GetAnimationName (688),
0069E8D0 (592); CSWGuiMainInterfaceAction::Initialize 0068B9D0 (1136); main
interface 0068B7C0 AddFloatyText (528), 0068BE40 target menu ctor (272).

# Worker K round 1 — placeable / door lifecycle and actions

Branch `codex/worker-k-r1`, base `5edb148`, owned range 00680000–0069FFFF.
Read matching agreements, workflow audit and old worker K notes. Prefetched all
136 eligible candidates with todo.py; selected the related CSWCPlaceable /
CSWCDoor family, including adjacent unlisted callbacks discovered in executable.
Current shared base declarations remain untouched; extended layouts are isolated.

Counts pending canonical comparison. Screened: 136. No screened address is
counted as an implementation or exact match. No shared reports modified.

Checkpoint 1: actual attempts 10, NEW exact 8, retained misses 2. Exact:
6825F0, 682620, 682660 (new unlisted callback), 683D00, 683D40, 683D90,
683E00, 683E50. Misses 6826A0 / 683E90: known tutorial-dispatch register /
global-load scheduling; three normal shapes tried, defer. The speed/animation
members are +48/+4C. Animation action IDs are full-word arguments (original
passes them directly to GetAnimationLength without narrowing); SetAnimation
uses a short and stores it. Invert SetOpenState's zero test to get the zero
arm after the positive-range arm. A named GetMessage local orders its call
before loading the object ID. The ShowTutorialWindow wrapper has an existing
void/int semantic conflict in symbols; callers here consume EAX, retained int
caller view and deferred ABI correction rather than changing shared contracts.

Checkpoint 2: cumulative actual attempts 17, NEW exact 15, retained misses 2.
Added exact 682BE0 / 6831B0 / 683200 / 683630 / 6836A0 / 683BB0 / 683F90.
Lifecycle now covers both full destructors and door constructor; placeable
scalar deleting destructor compiled separately so it calls the full destructor.
Inline module GetArea preserves ECX for UnloadLight (direct field read gives
EAX plus a move). A local app pointer before the timer getter matches ctor
argument scheduling, as predicted by old notes. IsHostile deliberately performs
the repeated server lookup, including the original null-result dereference
edge; caching the first lookup would alter observable calls.

Checkpoint 3: cumulative actual attempts 23, NEW exact 21, retained misses 2.
Six additional exact functions: 682270, 6823C0 (unlisted GetName), 682DD0
(unlisted lock action), 683F00, 6836C0, 684CD0 (unlisted door AI update).
Both AI functions share a typed macro, preserving object-array status global,
world-time output order and destruction after successful array removal. Load
object ID into a named local before retrieving the array to match EAX/EDX.
GetAIList inline accessor yields the original LEA; a direct empty list member
expression produced MOV/ADD. Canonical GetServerObject symbol retained via a
polymorphic link base at offset zero; a nonpolymorphic empty base would receive
an MSVC7 +4 adjustment and is incorrect. All lifecycle members were rechecked.

## Final lifecycle/action/model family result

- Screened 136 eligible in-range addresses; actual new annotations/attempts 28.
- NEW exact 23; retained NEW NONMATCH 5; prior NONMATCH promotions 0.
- Whole K folder canonical `-q -j4` result: **323/355**, from **300/327**.
  Compared a private `/tmp/worker-k-final-status.csv` to checked-in baseline:
  no prior MATCH regressions and no changed old NONMATCH distances.
- No shared reports, symbols, assets, VS2022 source, old-repository files or
  out-of-range addresses changed. No push. Local checkpoint commits only.

NEW exact addresses: 00682270, 006823C0, 006823F0, 006825F0, 00682620,
00682660, 00682BE0, 00682DD0, 006831B0, 00683200, 00683630, 006836A0,
006836C0, 00683BB0, 00683D00, 00683D40, 00683D90, 00683E00, 00683E50,
00683F00, 00683F90, 006842E0, 00684CD0.

Retained misses: 006826A0 (6), 00682C60 (17), 00682E20 (19),
00682F20 (11), 00683E90 (8). 2DA misses retain the best of three normal
receiver/temporary shapes; no instruction-copying, raw-address loads or
artificial signature tricks were used. Strings were read from original image:
SetAppearance uses `HitCheck` / `NonCull`, shadow uses `ShadowSize` and the
three `Fx_Shadow_*` literals. GetCanSeeHeight uses `CanSeeHeight`.

Both model loaders now match. Preserve argument order `(resref, part, unused)`;
the third stack word is unused. Door's +D0 write occurs before the null test,
as in the original. The model event callback is cdecl with event second and
client-door receiver third. Placeable's implicit CExoString suffix argument
and Quaternion-before-Vector construction reproduce all temporaries and EH
cleanup in its 512-byte loader. Model APIs use the established CAnimBase
(0x14) / CSWCAnimBase (0xF0) hierarchy; class-size assertions also preserve
CSWCObject 0xFC, placeable 0x150 and door 0x144. SetAnimation consumes an int
animation ID for the canonical int/float PlayAnim interface and stores its low
word, matching the original full stack-word argument reads. CanPerformAction
is the GUI elapsed-action timer gate at 0062F9A0; FadeObject at 0063DF60
advances the fade and reports timed deletion, rather than a simple getter.

No V4 differential promotion is claimed: this matching-only family depends on
original app/timer/model virtual interfaces and compiler EH runtime. Byte
identity and original side effects/ABI were checked with the original toolchain.

### Next coherent follow-on queue

The lifecycle, model allocation/teardown and compact action-dispatch workset is
complete, with the five honest misses retained. Larger animation/light/menu
work remains screened but **not counted as attempted**: LoadLight 006826E0,
project-target 00682FF0, placeable UpdateAnimation 006832A0, target menu
006837D0, door UpdateAnimation 00684030 and interaction 00684660. The two
animation methods need a shared next-animation record and audio contract;
original short output slots retain unwritten high bytes, and the sound API
005D5E10 has conflicting typed signatures in existing symbols. Keep those
contracts honest rather than manufacture high-word values or a false call
signature. Original disassembly of these functions was screened; no code is
being claimed for them. Subsequent separate family: area VFX lifecycle or
client player appearance; do not jump into stale 006A-range queues.

# Worker K round s2 — creature appearance, spell visuals, map GUI

Branch `claude/worker-k-s2` (local only), base `ec401c4`. Folder K canonical
`kotor-match -q -j 2 matching/src/k`: **342/376 -> 376/417**. New annotations
41: NEW exact 34, new NONMATCH 7, prior NONMATCH promotions 0. All 34 old
NONMATCH distances unchanged; no prior MATCH regressed. No shared reports,
functions.csv or other buckets touched.

## NEW exact (34)

- CSWCCreatureAppearance (`sw_c_creature_appearance.cpp`, Mac names from
  nwccreatureappearance.o): 00696CA0 GetGenderChar, 00696D40 GetAnimType,
  00696E10 RequestSimple, 006973B0 GetHeadNodeHeight (descriptive),
  00697460 GetCameraFollowPart, 006974A0 GetFollowingBody, 006974D0
  GetHeadHeightOffset, 00697580 GetCameraOffset, 006975D0 GetRunRate
  (descriptive), 00697610 GetBTypeModelAndTextureResRef, 006978A0
  ForceAnimate, 00697A20/00697B00/00697BC0 CreateHead/Tail/WingAppearance,
  00697C80 RequestBType, 00697CE0 CreateBTypeBody, 00698150
  SetAppearanceAlignment, 00698550 CreateSimple, 00698A40
  RequestBodyAppearance, 00698B10 CreateBodyAppearance.
- CSWCSpellCastVisual: 00695EF0 donefade callback (descriptive free
  function), 00695F30 LoadConjureVisual, 00696250 LoadCastVisual.
- CSWCWaypoint (object type 12, vtable 00754598): 00692540 ctor, 006925B0
  dtor, 00692610 scalar deleting dtor.
- CSWGuiMapHider 006932A0 SetMapNote; CSWGuiButtonToggle 00685540
  SetSelected (bitfield, emitted in swguimaininterfacepc2.o).
- CSWGuiInGameGalaxyMap: 006935A0 OnPlanetClicked, 00693610 Draw,
  00694BF0 SelectNextPlanet, 00694CA0 SelectPreviousPlanet (cycling names
  descriptive). CSWGuiInGameMap: 006929B0 SetMapNoteText (descriptive),
  00692B00 ReturnToEbonHawk.

Not in functions.csv (annotated anyway): 00692B00, 00693610, 00695EF0,
00697460, 006974A0. The Mac map attributes ReturnToEbonHawk to 006929B0
(the table's start for that range); 006929B0 is the map-note label setter,
the RET 4 message-box callback is 00692B00.

## Retained NONMATCH (7)

00696430 appearance ctor (7, zero/0xFF register swap), 00696F60
RequestHeadAppearance (30; condition preloads both fields, gender-row
register choice), 006964C0 CreateHead (121; 2 KB, layout/ternary
scheduling — structure is complete), 00692E30 ClearMapNotes (4), 00693300
HitCheckMouse (4), 00692BC0 OnReturnToEbonHawk (6) — register ties;
00692630 CSWCMessage::ReadOBJECTIDClient (3, dead `oid = 0` stack store).

## Lessons (MSVC 7.0)

- 2DA access: `g_pRules->Get2DArrays()->GetAppearanceTable()` (two inline
  accessors) puts the table in EAX then `mov ecx, eax`, as the original.
  Direct member chains or one static helper load straight into ECX.
- Column-name and CResRef arguments: pass `"RACE"` / `sValue` / `""` and let
  the converting constructor build the temporary. Explicit `CExoString("x")`
  pushes the ctor's EAX instead of re-LEAing, and explicit `CResRef(...)`
  temporaries make cl load `this`/vtable before the arguments, which also
  breaks the if/else tail merge of SetEnvMapPart.
- A call-result argument evaluated before the vtable load (AttachToObject
  with GetModel) needs a named local for the argument.
- Inline accessor results land in EAX (GetMessageBox, Get2DArrays); use one
  when the original tests a loaded pointer in EAX.
- `int b; ... b = 0;` placed after the CExoString constructors reproduced a
  zero register (EDI) used early for constants; `int b = 0;` at the top did
  not.
- Unused `CExoArrayList` with `if (element) delete[] element;` dtor keeps
  the three zero stores and an EH state but emits no dtor code.
- `!(x < 0.0f)` gives `test ah,5 / jp`; `>= 0.0f` gives `test ah,1`.
- Chained `a = b = v` stores b first; `szTex[3] = szModel[5] = c` matched.
- Empty polymorphic-base vs empty plain base: make the member-pointer base
  (CSWGuiObject) polymorphic so `this` is not adjusted by +4.
- MSVC folds `f * 1000.0f * 0.37f`; `0.37f * (f * 1000.0f)` keeps both fmuls.
- Message-box callbacks: `(void (CSWGuiObject::*)(CSWGuiControl*))&X::F`
  is a single 4-byte push for single inheritance.

## Next in-range queue

Galaxy/in-game map family continues: 00692C70 (448), 00692E80 (528),
00693090 (528), 006933A0 DisplayPlanet (512), 00693650 OnPanelAdded
(1360), 00693BA0 OnMapNoteClicked (960); CSWGuiMapHider 00693F60 ctor and
006942D0 Draw; galaxy/in-game map ctors 00694D50 / 00695180. Then pazaak
panels (006807E0, 00680710, 006816F0, 00681890). Large appearance leftovers
are only the three NONMATCHes above.
