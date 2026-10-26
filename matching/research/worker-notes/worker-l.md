# Worker L matching notes

Range: 0x006A0000–0x006EFFFF. Read Kotor RE's historical worker-l notes and
current matching headers; its old next queues include many now-owned targets.

## Current checkpoint, 2026-10-26

- Accepted control-pointer array IndexOf at 0x006C2A40 is a 48-byte exact
  template instantiation; control and panel pointer versions share the code.
- Character panel dispatch 0x006AE980/0x006B0B90/0x006B2220 and generated
  deleting wrapper 0x006B2200 are exact. The regular destructor is external.
  An unannotated constructor emits the vtable/wrapper without an invented
  empty regular destructor. Do not count that constructor as reconstructed.
- CAurEmitter::SetBirthrateA/B at 0x006DAF60/0x006DAFA0 are retained two-diff
  attempts. Rate/timer fields are +0x4C/+0x50 and +0x74/+0x78. Ordered
  timer<old-rate and timer!=0 guard timer -= old-rate - new-rate. The x87
  arithmetic matches; the integer load moves across the second branch.
  Five bounded shape checks found no improvement; defer these scheduling
  ties and continue the wider pool.

## Next family

Screen at least 100 eligible range addresses, then process a complete
related GUI family using known panel headers. Compact ownership anchors
include inventory ctor 0x006B34C0, regular dtor 0x006B40C0, deleting wrapper
0x006B4B90, Draw 0x006B4BB0; equip ctor 0x006BA980, regular dtor
0x006B8840, deleting wrapper 0x006B9450, Draw 0x006BB510 (UpdateInventory
then base Draw); and graphics-advanced ctor 0x006E16E0, regular dtor
0x006E1F40, deleting wrapper 0x006E1F20. Confirm names, call targets and
current source ownership. Compiler-generated wrappers may reference
externally declared real destructors; do not substitute fake callee labels
or copy instruction sequences.

## Options family continuation (2026-10-26, worker-l-r1)

- Prefetched the coordinator's 120 currently eligible range candidates from
  `/private/tmp/kotor-next-l-pool.txt`; screened the complete pool before
  selecting the options panel lifecycle and event family.
- First checkpoint adds 18 new exact annotations: nine deleting wrappers
  and nine regular destructors, covering Gameplay, Feedback, Graphics,
  Sound, Resolution, GraphicsAdvanced, SoundAdvanced, Mouse and Main.
  Canonical x86 confirms all member offsets, destructor callees and EH
  stage ordering. Eight regular destructors omit the derived vptr store;
  isolated novtable declarations reproduce that fact. Feedback restores
  its vptr and uses the normal declaration. Wrapper TUs contain real member
  cleanup; the Feedback constructor is unannotated and not counted.
- Targeted canonical comparison: 18/18 exact. No retained misses yet.
  Continuing the associated event methods before closing this family.
- Event continuation adds 24/24 new exact annotations: seven default-option
  handlers, five description forwards, Gameplay OnPanelAdded, both advanced
  discard callbacks, graphics framebuffer/soft-shadow/vsync toggles, texture
  quality and anisotropy stepping, and sound EAX-left/software toggles.
  Default callees reuse symbol-map names even where old names are misleading
  (SetBrightnessDefaults is the mouse-default call at 0x0061D9D0).
  Running total: 42 attempted / 42 new exact / 0 retained misses.
- Added 23 related panel-opening/toggle attempts: 14 new exact and nine
  retained 3-diff NONMATCH openings. The nine openings differ only in the
  temporary argument register around the external constructor call. Three
  ordinary shapes were checked; this is the same tie already present in
  older options-in-game openings, so no further retries are warranted.
- Bitfield members reproduce the observed masked flag replacement. The
  plain xor-expression shape used the opposite scratch registers; assigning
  the real bitfield eliminated all those differences. Gameplay auto-level
  setter must remain a single call expression so its value is pushed before
  the second GetOptions call.
- Intermediate whole-folder check: 284/331 exact (56 new exact), with no
  baseline MATCH regression; existing in-game option-opening misses remain
  at their prior 3/4-diff quality. Running attempted total 65; retained 9.

- Final compact event continuation adds 18 attempted / 16 exact: slider
  enter/exit and mouse sensitivity, sound EAX-right and option hover,
  difficulty-left/enter/exit, anti-alias-right, auto-pause default/hover,
  and graphics shadow/grass toggles are exact. Graphics OnResolution is the
  same retained constructor argument-register tie (3 diff); AntiAliasLeft
  retains 2 branch-shape differences after three bounded shapes. The best
  normal C++ attempts remain annotated.
- Slider thumb parameters occupy +0x18C, with highlighted bit at +0x24.
  An inline SetHighlighted member preserves the original parameter-base
  adjustment; direct bitfield assignment formed the flag-word address
  instead. This resolves the four related 2-diff enter/exit cases.
- Naming audit corrected the prior inferred `CSWGuiOptionsGameplay` and
  `CSWGuiOptionsAutoPause` declarations to Mac-confirmed
  `CSWGuiInGameGameplay` (constructor 0x006E69A0) and
  `CSWGuiInGameAutoPause` (constructor 0x006E76A0). Updated owned prior
  call sites and lifecycle annotations consistently. Other options class
  names are independently confirmed by the Mac constructor mappings.
- Completed this coherent options panel teardown/compact event family:
  **120 prefetched/screened; 83 new attempted; 72 NEW exact; 11 retained
  NONMATCH; 0 prior NONMATCH promotions.** Unannotated Feedback constructor
  and compiler helper emission are not counted.
- Final whole-folder canonical check (private report only): **300/349
  exact**, versus baseline 228/266. Automated address comparison found no
  prior MATCH regression and no increase in any prior NONMATCH diff count.
  Shared status/symbol reports remain the coordinator's responsibility.
- Next coherent work is the options initialization/settings family: real
  constructors, SetDescription/SetUpOptions and persistence handlers. The
  compact event family is exhausted except Gameplay difficulty-right
  (0x006E6930), which needs the rules/2DA receiver layout. Inventory/equip
  lifecycle anchors in the earlier queue remain available for a fresh batch.

### New address outcomes

MATCH:

- 0x006DDB10, 0x006DDB30, 0x006DDB60, 0x006DDB80, 0x006DDFA0, 0x006DDFF0
- 0x006DE200, 0x006DE550, 0x006DE5A0, 0x006DE5F0, 0x006DE640, 0x006DE6A0
- 0x006DE6F0, 0x006DE740, 0x006DE780, 0x006DEAA0, 0x006DED90, 0x006DEDB0
- 0x006DEE00, 0x006DFAA0, 0x006DFBB0, 0x006DFBF0, 0x006DFC50, 0x006DFE60
- 0x006DFEE0, 0x006E0010, 0x006E0020, 0x006E0100, 0x006E0130, 0x006E0140
- 0x006E0150, 0x006E0190, 0x006E0460, 0x006E0470, 0x006E04A0, 0x006E04E0
- 0x006E05E0, 0x006E0680, 0x006E06D0, 0x006E0C50, 0x006E0C70, 0x006E0F50
- 0x006E1270, 0x006E12B0, 0x006E12E0, 0x006E1F20, 0x006E1F40, 0x006E2500
- 0x006E2520, 0x006E2990, 0x006E29B0, 0x006E3420, 0x006E3440, 0x006E3C40
- 0x006E3C60, 0x006E42A0, 0x006E42C0, 0x006E6170, 0x006E62A0, 0x006E62E0
- 0x006E6330, 0x006E6380, 0x006E63D0, 0x006E6710, 0x006E6750, 0x006E68A0
- 0x006E68B0, 0x006E68E0, 0x006E7110, 0x006E7130, 0x006E7570, 0x006E7690

NONMATCH:

- 0x006DE240, 0x006DE2C0, 0x006E0520, 0x006E1660, 0x006E2CF0, 0x006E2D70
- 0x006E2DF0, 0x006E3D80, 0x006E3E00, 0x006E6420, 0x006E64A0

### Intentional symbol-label corrections

- `CSWGuiOptionsGameplay` -> `CSWGuiInGameGameplay`: Mac map exact
  constructor 0x006E69A0, confidence `strings,2/2`, object
  `swguiingamegameplay.o`. Corrects the pre-existing external constructor
  call label at 0x006AAC10 and labels all new family definitions consistently.
- `CSWGuiOptionsAutoPause` -> `CSWGuiInGameAutoPause`: Mac map exact
  constructor 0x006E76A0, confidence `strings,2/2`, object
  `swguiingameautopause.o`. Replaces pre-existing constructor call label
  0x006AAD10 and matched lifecycle names at 0x006E7A70/0x006E7A90.
  The exact old MATCH address set and byte results are preserved.
- Options class vtable declarations include the independently observed
  HandleInputEvent override at panel slot 0x3C; this leaves the original
  base layout size (0x64), all member offsets and matched bytes unchanged.

## Inventory/equipment/store continuation (2026-10-26, worker-l-r2)

- Screened a fresh 180-address unowned candidate pool, excluding all source
  annotations from the accepted r1 baseline (private pool `/private/tmp/l-r2-pool.txt`).
  Selected the coherent inventory/equipment control/lifecycle family.
- First checkpoint: 13 attempted / 13 NEW exact. Real radio/item/store-entry
  constructors and member destructors, five deleting wrappers, equip Draw,
  and container release callback. Typed drawable member sizes are border
  0x74 and text 0x70; item entries own three borders and one text drawable.
- `novtable` isolates the observed omission of derived vptr restoration;
  radio cleanup genuinely restores its vptr and calls the button destructor.
  Container/inventory/equip constructors used only to emit deleting wrappers
  remain unannotated as constructors and are not counted as reconstructed.
- Exact new addresses: 0x006B66F0, 0x006BE360, 0x006B7EE0, 0x006B71E0,
  0x006B7F80, 0x006B87A0, 0x006B7FA0, 0x006B87C0, 0x006BB510,
  0x006B8770, 0x006B4B90, 0x006B9450, 0x006B8030.
- Second checkpoint extends the family with container regular destructor,
  inventory/equip OnPanelAdded, and item-entry GetNumItems: four more new
  attempts. The callback at 0x006B8770 is now named Update, and its callee
  0x006B8410 SetupGiveMode, following the existing semantic reconstruction.
  Earlier provisional event labels were descriptive; ABI and bytes unchanged.
- The extended equip layout preserves base size 0x64 and models nine slot
  buttons, nine labels, entry array, description/items listboxes and the
  final control/compare state. Shared panel padding now exposes the observed
  control pointer at +0x1c, without shifting any field.

## Key-mapping options family (2026-10-26, worker-l-s1)

- Batch: `CSWGuiInGameOptKeyMappings` (vtable 0x00759358, size 0xF34,
  swguiinOPTKeyMapping.o) and the `CSWGuiKeyMapButton` methods it drives.
  Full layout now lives in `l/sw_gui_keymappings.h`; the options header only
  forward-declares the panel (Gameplay::OnKeyMap includes the new header).
- Whole folder: 317/366 -> **340/395**. 31 attempted: **21 NEW exact**,
  **2 prior NONMATCH promoted** (UpdateColor 0x006EC6E0, SetUnchangable
  0x006EC770, now real Vector globals instead of raw address loads),
  8 retained NONMATCH. Address-set comparison: no prior MATCH regression.
- MATCH: 0x006EC120 GetActionId, 0x006EC350 Draw, 0x006EC3B0 SetEditing,
  0x006EC510 HandleInputEvent, 0x006EC580 FixFilterColors, 0x006EC7D0
  SetDescription, 0x006ECCC0 ResetToDefault, 0x006ECFB0 / 0x006ED550
  deleting dtors, 0x006ED2D0 OnDefaultClick, 0x006ED310 OnAcceptClick,
  0x006ED390/3E0/430 filter callbacks, 0x006ED480 SetCaptureEvent,
  0x006ED570 button dtor (novtable TU), 0x006ED5C0 OnControlEntered,
  0x006ED5E0 OnEventClicked, 0x006EDA80 panel UpdateEvent, 0x006EDC90 ctor,
  0x006EE040 Update; promoted 0x006EC6E0, 0x006EC770.
- NONMATCH kept (ties): 0x006EC250 SetExtent (1, store schedule),
  0x006ED930 FillListWithEvents (1, num=0 store vs proto load order),
  0x006EC3D0 dtor (10, index strength reduction), 0x006ED610 CreateOption
  (12, ForceDisplay/SortPos stack slots swapped), 0x006ECE60
  PostChangeToEvent (15, scratch-register rotation), 0x006ED1E0
  IsKeyUsedAlready (18, eax/ecx rotation), 0x006EDBD0 button UpdateEvent
  (21, ebx/edi swap for key/result), 0x006ECFD0 FillListBox (39, this in
  edi vs ebx; structure matches).
- functions.csv lacks starts 0x006EC250, 0x006EC350, 0x006EC510,
  0x006ED5C0, 0x006ED5E0 (they were folded into the previous function).
  Annotating them restores the boundaries for matchcmp; the coordinator
  should add them to functions.csv when regenerating reports.
- Lessons: (1) `g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(row,
  "Column", &n)` with the implicit const char* -> CExoString conversion is
  the original shape: an explicit `CExoString("Column")` temporary uses the
  ctor's returned eax instead of lea and loses the table/ecx order.
  (2) `bool bFailed = GetINTEntry(...) == 0;` in a block scope reproduces
  neg/sbb/inc + temp destruction before the test. (3) `x && y != 0` gives
  the setne/test materialisation. (4) A switch with separate identical
  `case 1:`/`case 2:` bodies gives the sub/dec/dec chain. (5) Sorted insert:
  `nPos = -1; for (j) { if (greater) { nPos = j; break; } nPos = j + 1; }`
  then `if (nPos == -1) Add else insert`. (6) CSWGuiControl slot 0x38 Draw
  is only declared under `L_GUI_CONTROL_DRAW_SLOT` to avoid changing other
  classes that declare non-virtual Draw; use a separate TU. (7) Filter
  buttons use local `CSWGuiButton*` pointers for AddEvent + nav-bit clear.
- Next in-range queue: options settings/constructors (0x006DDF40,
  0x006DED30, 0x006DFB60, 0x006DFAD0, 0x006DEFE0, 0x006DEF30, 0x006DFE70,
  0x006E74C0 autopause), level-up panel 0x006EE5F0/0x006EEE70/0x006EEE90,
  pazaak core 0x006E4580-0x006E4C60, upgrade selection 0x006C2A70-0x006C5B70.

## Pazaak rules + workbench upgrade family (2026-10-26, worker-l-s3)

- Options settings/constructors from the s1 queue were already owned by
  merged work, so this round took the next two queued families: the Pazaak
  rules core (swpazaak.o) and the workbench upgrade screens (swguiupgrade.o).
- Whole folder: **390/447 -> 427/488**. 41 attempted, **37 NEW exact**,
  4 retained NONMATCH, 0 prior NONMATCH promotions. Misses went 57 -> 61
  (only the four new ones); no prior MATCH regressed.
- MATCH (Pazaak, `l/sw_pazaak.cpp`, header `l/sw_pazaak.h`): 0x006E4580
  ChooseSidedeck, 0x006E45F0 AddGameCard, 0x006E4640 ClearGameBoard,
  0x006E4730 ShuffleDeck, 0x006E4800 DrawCard, 0x006E4840
  CanUsePlayerSidedeckCard, 0x006E4860 UsePlayerSidedeckCard, 0x006E48A0
  DetermineWinner, 0x006E4910 FindPotentialSidecardTotal, 0x006E4A40
  LoadInitialSideDeck, 0x006E4B50 CPazaakPlayer ctor, 0x006E4C00 CSWPazaak
  ctor.
- MATCH (upgrade, header `l/sw_gui_upgrade.h`): selection 0x006C4450 dtor,
  0x006C5B50 ??_G, 0x006C2AB0 GetUpgradableItems, 0x006C2B10
  HandleInputEvent, 0x006C2B60 OnTypeSelected, 0x006C2BD0 OnEnter; item
  select 0x006C47A0 dtor, 0x006C5B70 ??_G, 0x006C2A70/0x006C2A90 selected
  entry owner/slot, 0x006C2C80 SetDescription, 0x006C2D30
  HandleInputEvent; entry 0x006C3E00 ctor, 0x006C5AB0 ??_G (from ctor TU),
  0x006C5AD0 dtor (novtable TU), 0x006C27C0 Draw, 0x006C2830 Initialize;
  panel 0x006C4AD0 dtor, 0x006C6100 ??_G, 0x006C25F0 OnPanelRemoved,
  0x006C32E0 SetDescription, 0x006C4D30 CloseItems, 0x006C6190 OnAssemble,
  0x006C61F0 CancelPanel, 0x006C6A80 HandleInputEvent.
- NONMATCH kept: 0x006E4360 GetTotal (35, the fully unrolled 9x loop picks
  its strength-reduced base one card later; three loop shapes tried),
  0x006E4C60 PlayAITurn (33, cross-jumping keeps the later copy of the
  play-card and stand tails where the original keeps the earlier; ||,
  split-if and inverted forms all compile identically), 0x006C2650
  entry SetExtent (5, border vptr load hoisted earlier in the original),
  0x006C4880 item-select OnControlEntered (8, the original loads the
  control argument after copying this; prologue tie).
- functions.csv lacks the starts 0x006C2650, 0x006C2830, 0x006C2B60,
  0x006C4880, 0x006C6190 (folded into the previous function). The
  annotations restore the boundaries; the coordinator should add them.
  0x006C33A0 (CSWGuiUpgrade::Draw) is also a missing start but is not yet
  annotated; 0x006C32E0 still compares OK without it.
- Shared-header changes (all verified by the whole-folder run):
  `CSWGuiBorderParams` is really 0x54 bytes (the stack temporary in
  Initialize proves it); the l header was 0x60 with the owner's trailing
  0xC folded in. It is now 0x54 and every containing class (label, button,
  toggle, both CSWGuiBorder copies) carries an explicit 0xC pad, so no
  offset moved. CSWGuiControl slot 0x60 is now `GetRadioButton()`, +0x54
  is `m_nSound`; CSWGuiText exposes `m_cParams` at +0x18; inventory
  CSWGuiBorder exposes `m_cParams` and `GetInnerExtent`; Vector gained the
  3-float ctor; CSWGuiBorderParams gained inline `SetColor`;
  CServerExoApp::GetItemByGameObjectID and CSWPartyTable::GetPartyItems were
  added.
- Lessons: (1) Pazaak cards are assigned whole: `x = CPazaakCard(-1, 0)`
  gives the `or eax,-1`/`xor` register stores and makes cl fully unroll
  the 9- and 4-card loops; field-by-field stores give immediates and a
  rolled loop. (2) An inline helper taking `const CPazaakCard&` gave the
  re-read of the card value after the flag store in
  FindPotentialSidecardTotal; a by-value helper did not. (3)
  LoadInitialSideDeck: `if (!Load2DArray()) return;` (not a nested if)
  fixes the CResRef-argument temp slot; `int nRows = m_nNumRows;` before
  `rand()` keeps it in esi; a two-case switch on the sign char gives the
  original branch layout. (4) A callback taking `const CExoString&` and
  passed `""` must use the implicit conversion, not `CExoString("")`. (5)
  Calls on embedded member objects stay virtual under MSVC 7 (no
  devirtualisation), so `m_lbl.m_cTextBox.GetHeight()` is exact. (6)
  `CSWGuiBorderParams& r = member; r = ...; r.SetColor(...)` reproduces the
  original's address kept in edi across operator=; named CResRef locals
  in a block reproduce construct-before-push, and scoping the final extent
  lets it share the Vector temporary's slot. (7) Pointer-select ternaries
  layout: `type != 1 ? &a : &b` gives the original `lea a; jne; lea b`.
- Next in-range queue (upgrade family remainder): CSWGuiUpgrade Draw
  0x006C33A0 (needs CSWGuiScene::GetModel 0x00416750, the do3dgui flag
  0x0078D1E4, Quaternion helpers), ShowItems 0x006C2F80, UpdateItems
  0x006C3AA0, OnEnterSlot 0x006C3C30, VerifyUpgrade 0x006C6120,
  CreateItemEntry 0x006C58C0, InsertUpgrade 0x006C59A0, entry SetItem
  0x006C3EA0, item select OnUpgradableSelected 0x006C2DF0 / BuildItemList
  0x006C4960 / BuildUpgradeList 0x006C5B90 / FinishUpgrading 0x006C5E90 /
  OnPanelAdded 0x006C6330, selection OnPanelAdded 0x006C4520, and the three
  constructors 0x006C78D0 / 0x006C7630 / 0x006C6B60. Other open clusters:
  level-up panel 0x006EE5F0/0x006EEE70/0x006EEE90, CSWCAnimBase*
  0x006E4E50-0x006E6040, CSWMGBehavior 0x006DB4F0-0x006DBD30.

## Level-up screens + client anim bases (2026-10-26, worker-l-s5)

- s4 ended at the usage limit without notes; its upgrade work (items,
  ctors, panel ctor attempt 0x006C6B60) was already merged. The remaining
  upgrade members (Draw 0x006C33A0, ShowItems 0x006C2F80, Update3DView
  0x006C3630, SetItem 0x006C3EA0, OnPanelAdded 0x006C4D70,
  OnControlEntered 0x006C5370, OnSlotSelected 0x006C6500) are all large and
  were left for later.
- Whole folder: **438/503 -> 501/551**. 48 new functions annotated, **41 NEW
  exact**, 7 retained NONMATCH, **22 prior NONMATCH promotions**.
- MATCH, level-up summary (`l/sw_gui_levelup_chargen.cpp`, ctor TU
  `l/sw_gui_levelup_chargen_ctor.cpp`): 0x006E7B40 OnPanelAdded, 0x006E7DD0
  DetermineClass, 0x006E7EC0 SetNameString, 0x006E8500 NewAbilityScores,
  0x006E85A0 ClearAbilities, 0x006E8730 ClearSkills, 0x006E9A50
  HandleInputEvent, 0x006E7B70 dtor, 0x006E88A0 BeginPlayGame, 0x006E89F0
  CopyCreatureStats, 0x006E89D0 ??_G.
- MATCH, level-up step panel: 0x006EE7D0 ctor + 0x006EEE70 ??_G
  (`sw_gui_levelup_panel_ctor.cpp`), 0x006EEE90 dtor (novtable TU
  `sw_gui_levelup_panel_dtor.cpp`), 0x006EE350/0x006EE3D0/0x006EE480/
  0x006EE500 OnSelect{Powers,Feats,Skills,Abilities}Button, 0x006EE5F0
  OnCancelPressed, 0x006EE780 OnSelectAcceptButton.
- MATCH, client anim bases (`l/sw_c_anim_base_objects.cpp`, header
  `l/sw_c_anim_base_objects.h`, base layout copied from k's
  sw_c_anim_base.h with slot 0xE0 = ServerToClientAnimation): placeable
  0x006E4E50 ctor, 0x006E4E70 dtor, 0x006E4EC0 LoadModel, 0x006E5180 ??_G,
  0x006E53F0 ServerToClientAnimation; door 0x006E56A0 ctor, 0x006E56C0
  dtor, 0x006E5710 LoadModel, 0x006E59A0 ??_G, 0x006E59C0
  GetAnimationName, 0x006E5AC0 ServerToClientAnimation; visual effect
  (vtable 0x00758B68, named CSWCAnimBaseVisualEffect, descriptive)
  0x006E5CE0 ctor, 0x006E5D00 dtor, 0x006E5D50 LoadModel, 0x006E5DC0
  GetAnimationName, 0x006E5E60 PlayAnimOnModel; trigger 0x006E5F40 ctor,
  0x006E5F60 dtor, 0x006E5FB0 LoadModel, 0x006E6020 ??_G, 0x006E6040
  GetAnimationName.
- Promotions: CSWGuiOptionsInGame::On{LoadGame,SaveGame,Gameplay,Feedback,
  AutoPause,Graphics,Sound}, CSWGuiOptionsMain::On*Opt (5),
  Sound/Graphics::OnAdvanced, InGameGameplay::OnMouse/OnKeyMap,
  Graphics::OnResolution, GraphicsAdvanced::OnAntiAliasLeft, upgrade item
  select OnControlEntered 0x006C4880, ClearPowers 0x006E87E0, level-up
  panel SetCanCancel 0x006EE640 and HandleInputEvent 0x006EE720.
- NONMATCH kept: 0x006E7F70 AbilitiesChanged (79; register rotation in the
  ability-delta block), 0x006E8EF0 chargen ctor (126; ecx/edx/eax rotation
  shifted by one from the SetClassLevel block on, code otherwise identical),
  0x006EE0E0 ChangeState (56; original re-tests bBack and reloads m_nState,
  loop not rotated; nested-if, for(;;), inline helper all compile alike),
  0x006E4F30 placeable PlayAnimation (52; block order of the default and
  once ranges), 0x006E51A0 placeable GetAnimationName (55; early copy-out
  return when the 2DA name is non-empty), 0x006E57B0 door PlayAnimOnModel
  (22; esi saved in the prologue instead of shrink-wrapped), 0x006E6100
  trigger ServerToClientAnimation (11; default block placed first).
- functions.csv lacks the starts 0x006E4EC0, 0x006E4F30, 0x006E5180,
  0x006E53F0, 0x006E5710, 0x006E57B0, 0x006E5AC0, 0x006E5D50, 0x006E5DC0,
  0x006E5E60, 0x006E5FB0, 0x006E6100, 0x006EE350, 0x006EE3D0, 0x006EE480,
  0x006EE500, 0x006EE780 (all folded into the previous entry); the
  annotations restore them.
- Shared-header changes (whole folder verified, no MATCH regressed): l
  CSWCCreatureStats gained names/fields up to 0x124 (names, race, bases,
  HP/FP, saves, BAB, portrait, alignment, skill ranks) and accessors;
  CSWSCreatureStats gained the ability modifiers 0xEE/0xF2/0xF4; CSWClass
  hit/force die 0x54/0x55; CSWCCreature ctor, 0x440 flag, size 0x44C and
  inline GetClientStats(); CSWCObject::GetId(); CSWCCreatureVirtuals slots
  0x70/0x8C/0x94/0xD8 named (descriptive); CExoString Find/Left/Right;
  CGuiInGame +0x14 and +0x9C (skill info box); CClientExoApp GetMessage/
  GetInputClass; CSWGuiControl slot 0x40 SetFlag0(int) and inline
  IsActivated(); CSWGuiPanel inline GetManager().
- Lessons: (1) **Inline accessors change register colouring.** Replacing a
  direct member chain with a trivial inline getter (GetManager(),
  GetSnapshotStats(), GetLevelUpStats(), GetClientStats(),
  Get2DArrays()->GetAnimations(), pCreature->GetId(),
  pControl->IsActivated(), SetVisibleBit()) fixed eax/ecx/edx choices and
  load order that no statement reshuffle could; this promoted the whole
  3-diff `new Panel(m_pManager)` options family. Try it first on any
  "same code, other register" tie. (2) `int n = byteOrWordExpr; if (n > 0)`
  reproduces movzx/test/jle where `if (expr > 0)` folds to cmp/jbe. (3) A
  separate `CExoString s(n); x.SetText(s);` (named local) passes the
  variable's address; a temporary passes the ctor's eax. (4) Switch case
  bodies are laid out in source order: read the jump table (script in the
  s5 scratch dir) to recover the original case order. (5) Clamp
  `if (n >= 0) { int v = n; if (v > 100) v = 100; return v; } return 0;`
  gives the short-test-then-int-compare shape. (6) `if (p == 0) return 0;`
  before an EH local vs wrapping the body in `if (p)` decides whether the
  return-0 block is shared at the end.
- Next in-range queue: the 7 NONMATCHes above; upgrade remainder listed at
  the top of this section; CSWCAnimBaseDoor/Placeable tails; level-up chargen
  pieces are now complete except the ctor/AbilitiesChanged ties; other
  open clusters: CSWMGBehavior 0x006DB4F0-0x006DBD30, save/load GUI
  0x006C8250-0x006CC680, projectile modes 0x006D4520-0x006D55F0.

## Character-creation completion notifications, 2026-10-26

- Seven exact matches: custom panel 0x006EF4C0/4E0/500/520/530 and quick
  panel 0x006EFF60/FF80. They notify the parent for portrait, name or ability
  updates and set the appropriate creation state. AbilityCreationCompleted
  and AbilitiesChanged have Mac debug-map support; the remaining descriptive
  names follow their native callers and character-creation flow.
- The parent pointer is at +0x68. SetState callees are 0x006EEFD0 for custom
  and 0x006EFC10 for quick; state values were checked against native code.
- Four more exact matches in the same family: 0x006EEF60 SetParentPanel,
  0x006EF540 ClearAbilities, 0x006EF570 CancelPanel and 0x006EFFA0
  GetCurrentName. The caller retains the native double parent check after
  SetVisiblePanel, and GetCurrentName returns a CExoString copy from +0x22D4.
  ClearAbilities delegates to the Mac-supported parent method and resets the
  ability-point byte at the creature stats +0xAC.
- Four compiler-generated scalar deleting destructors match 32/32 bytes:
  0x006C0580 Pause, 0x006C2540 SoloModeQuery, 0x006C8170 AreaTransition,
  0x006BFF90 PartySelection. Native relative calls target their respective
  real destructors 0x006C05A0/0x006C2560/0x006C8190/0x006BFFB0. Class
  identities are supported by Mac debug names and the native vtable contexts.
  The unannotated emitter constructors are not reconstructed constructors.
