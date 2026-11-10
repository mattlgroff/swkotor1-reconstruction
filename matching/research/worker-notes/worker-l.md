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

## Store, party selection and small in-game panels (2026-10-27, worker-l-s7)

- Whole folder: **545/600 -> 591/655**. 55 functions annotated, **46 NEW
  exact**, 9 retained NONMATCH, no prior MATCH regressed.
- MATCH, store (`l/sw_gui_store.cpp`, `sw_gui_store_ctor.cpp`, header
  `l/sw_gui_store.h`): 0x006C0610 PopulateStore, 0x006C0690 SetDescription,
  0x006C0790/0x006C07F0 GetItemBuy/SellValue, 0x006C0AA0 OnControlEntered,
  0x006C0BE0 BuyItem, 0x006C0D20 BuyItemCallback, 0x006C0D80 SellItem,
  0x006C0EE0 SellItemCallback, 0x006C1130 OnControlStoreAButton, 0x006C13C0
  ShowSellGUI, 0x006C1A50 ShowBuyGUI, 0x006C1B00 OnControlXButton,
  0x006C1B50 InitializeStoreType, 0x006C2190 HandleInputEvent, 0x006C1C00
  ctor, 0x006C1470 dtor, 0x006C1820 ??_G; store item entry 0x006B53F0
  Initialize (`sw_gui_store_entry.cpp`).
- MATCH, party selection (`sw_gui_party_selection.cpp`, `_ctor.cpp`, header
  `l/sw_gui_party_selection.h`): 0x006BE310 SetForcedNPC, 0x006BE370 slot
  SetSelected, 0x006BE490 OnPanelRemoved, 0x006BE4E0 UpdateCount, 0x006BEC90
  ConfirmPartySelection, 0x006BECC0 SetAddRemoveLabel, 0x006BEDE0
  HandleInputEvent, 0x006BF2A0 OnToggled, 0x006BF3B0 OnDone, 0x006BFA40
  ctor; implicit dtors 0x006BE420 (member button) and 0x006BFFB0.
- MATCH, small panels: pause 0x006C00B0 Draw, 0x006C02F0 OnPanelAdded,
  0x006C0350 OnUnpause, 0x006C03B0 ctor, 0x006C05A0 dtor
  (`sw_gui_in_game_pause.cpp`); solo-mode query 0x006C2270 ctor, 0x006C2310
  Draw, 0x006C2400 HandleInputEvent, 0x006C24A0 SetQuery, 0x006C2560 dtor
  (`sw_gui_solo_mode_query.cpp`); area transition 0x006C7D20
  CSWCDoor::GetTransitionName, 0x006C7D50 ctor, 0x006C7EC0
  SetTransitionObject, 0x006C8050 Draw, 0x006C8190 dtor
  (`sw_gui_area_transition.cpp`). All implicit panel dtors share the
  novtable TU `sw_gui_panel_dtors_novtable.cpp`.
- NONMATCH kept: 0x006B2590 CSWGuiListBox::SetTopVisible (8; esi/edx swap,
  own TU `sw_gui_listbox_top_visible.cpp` because the caller otherwise keeps
  ECX live), 0x006B5270 store entry SetExtent (5; same ebx/edx tie as the
  upgrade entry 0x006C2650), 0x006B7270 store entry SetItem (53; frame is 8
  bytes larger, one extra 4-byte slot at the bottom, structure identical),
  0x006C0850 PopulateInventoryItemListBox (71; original spills `this` and
  keeps the inner index in ebp), 0x006C1840 PopulateStoreItemListBox (28;
  ebx/ebp swap + nIndex slot), 0x006C1660 CreateItemEntry (30; ebx/ebp swap
  of pool/entry), 0x006C0F40 OnControlInvAButton (11; second GetLevel keeps
  the stats pointer in edi; a GetStats() getter fixes that but spills pItem),
  0x006C00C0 pause SetPauseReason (47; eax/edx temp colouring only),
  0x006BED70 party member button ctor (3; vptr/field store scheduling).
- functions.csv lacks the starts 0x006B5270, 0x006B53F0, 0x006BE490,
  0x006BEC90, 0x006BEDE0, 0x006BF2A0, 0x006BF3B0, 0x006C00B0, 0x006C02F0,
  0x006C0350, 0x006C0AA0, 0x006C0D20, 0x006C0EE0, 0x006C0F40, 0x006C1130,
  0x006C2310, 0x006C2400, 0x006C8050; the annotations restore them.
- Naming: mac_names attaches "OnControlStoreAButton" to 0x006C0D80 and
  "OnPanelRemoved" to 0x006BE420 by call pattern only; the code at those
  addresses is SellItem (player sells to the store) and the member-button
  destructor, so the descriptive names follow the code. CSWGuiPartySelectionSlot
  (member button) and CSWGuiInGameSoloModeQuery::SetQuery are descriptive.
- Shared-header changes (whole folder verified): CSWGuiListBox +0x2C8 m_nTop
  and SetTopVisible; CSWGuiBorderParams +0x08 m_fRotation and bit aliases
  m_bStyle0/1 over m_nStyle; CSWGuiTextParams full ctor 0x00417120;
  CGuiInGame +0x30, +0xB4, ShowExamineBox/HideStoreGui/SetupPauseGuiExtent/
  HideSoloMode/GetBarkBubbleVisible; CClientExoApp GetGUIString/GetMiniGame/
  GetModule/GetAutoPaused/SetAutoPaused/GetAreaNotReadyToDisplay/
  TogglePartyFollow; CServerExoApp::GetStoreByGameObjectID; CSWPartyTable
  +0x190 and KillNPCObject; CExoString::Find(const CExoString&, int);
  CTlkTable::SetCustomToken; bark CSWCObject +0x08 object type and
  CGameObject slots 0x14 AsSWCDoor / 0x3C AsSWCTrigger; tutorial CSWCCreature
  +0x2D4 bit and PerformSkill(int,int,ulong), CSWCDoor +0x13C; upgrade
  CSWSItem m_bPlotItem/m_bInfinite/GetCost/GetStackSize, CSWSCreature
  GetGold/SetGold/GetItemRepository/m_pStats(+0xA74)/GetStats; inventory
  CSWGuiStoreItemEntry SetExtent/Initialize/SetItem.
- Lessons: (1) Inline accessors again decided register colouring:
  GetServerExoApp()/GetClientExoApp() on some calls (OnPanelAdded,
  Draw, the getter-backed GetStore()), CSWSItem::GetStackSize(),
  CSWCModule::GetArea(), CSWSCreature::GetStats(). (2) Reading the control's
  object id into a local before fetching the server pointer
  (`unsigned long oidItem = pEntry->m_oidItem; CServerExoApp* pServer = ...`)
  fixed every GetItemByGameObjectID(ClientToServerObjectID(..)) lookup.
  (3) `if (n > 1) store->Acquire(item->Split(1)); else { repo->Remove(item);
  store->Acquire(item); }` (tail-merged calls) beat a pSold variable.
  (4) Constructors keep member pointers in callee-saved registers only when
  the source uses pointer locals from the InitControl call on
  (`CSWGuiListBox* pShopItems = &m_lbShopItems; InitControl(pShopItems, ..)`);
  re-using one `pButton` for two buttons reproduces the address-in-ecx store.
  (5) Visibility flag triples written with ShowControl/HideControl give the
  `or [mem], eax` form. (6) A CSWGuiZeroExtent local assigned through
  `static_cast<CSWGuiExtent&>(x) = ext` keeps the ctor zeroing. (7) Counter
  declaration order picks which byte counter gets the immediate zero store
  in a fully unrolled loop (OnDone). (8) Scratch variant script with a
  compile-failure check: /private/tmp/claude-501/swk-l-s7/var.py.
- Next in-range queue: party selection remainder (AcceptParty 0x006BE560,
  OnPanelAdded 0x006BEEB0, OnEnter 0x006BF5B0); the 9 NONMATCHes above;
  store entry Draw 0x006B4D40 / in-game item entry Initialize 0x006B4DB0;
  upgrade remainder (Draw 0x006C33A0, ShowItems 0x006C2F80, Update3DView
  0x006C3630, SetItem 0x006C3EA0, OnPanelAdded 0x006C4D70, OnControlEntered
  0x006C5370, OnSlotSelected 0x006C6500); save/load GUI 0x006C8200-0x006CC680;
  CSWMGBehavior 0x006DB4F0-0x006DBD30; projectile modes 0x006D4520-0x006D55F0.

## Save/load screen (2026-10-27, worker-l-s8)

- Whole folder: **591/655 -> 622/688**. 33 functions annotated, **31 NEW
  exact**, 2 retained NONMATCH, no prior MATCH regressed. The previous s8
  worker left no commits; this batch started from the merged main state.
- Header `l/sw_gui_save_load.h` (swguisaveload.o): CSWGuiSaveLoad (0x1160,
  vtable 0x00757650), CSWGuiSaveLoadEntry (0x248, vtable 0x00757740, a
  CSWGuiRadioButton whose ctor/dtor are inlined), CSWGuiSaveNamePanel
  (0x690, vtable 0x007576D0), CSWGuiSaveGameEditBox (vtable 0x007575B0),
  CStatusSummary, KOTOR_AUTOSAVE_PARAMS, plus local CResGFF/CERFFile/
  CExoFile/CExoAliasList/CExoBase declarations.
- MATCH: entry 0x006C8200 GetPlayerPortrait (`_entry.cpp`, implicit CResRef
  copy), 0x006C8250 GetGameDirectory, 0x006C8E50 LoadData
  (`_entry_load.cpp`), 0x006C9780 SetName (`_entry_name.cpp`), 0x006CB940
  ctor, 0x006CBA90 dtor, 0x006CBA70 ??_G; edit box 0x006C82F0 HandleChar,
  0x006CB0A0 ??_G; save-name panel 0x006C8410 OnPanelAdded, 0x006C8460
  OnPanelRemoved, 0x006C8470 HandleCancelButton, 0x006CB040
  HandleInputEvent, 0x006CAE70 ctor, 0x006CC060 dtor, 0x006CC040 ??_G;
  save/load 0x006C86D0 HandleInputEvent, 0x006C8750 LoadBarCallback,
  0x006C8900 DeleteGame, 0x006C89D0 ShowGame (`_show.cpp`), 0x006C9A90
  ExtractSaveGame, 0x006CAA90 PromptDeleteGame, 0x006CAAF0
  PrepareGameInProgress, 0x006CB820 PromptForSaveName, 0x006CC0E0
  VerifyLoadGame, 0x006CC160 PopulateGameList (`_list.cpp`), 0x006CC680
  ctor, 0x006CA110 dtor, 0x006CB0C0 ??_G; GFF readers 0x006C8490
  CStatusSummary::LoadFromGFF, 0x006C9DE0 KOTOR_AUTOSAVE_PARAMS::LoadFromGFF
  (`_gff.cpp`). Dtors without derived vptr stores are in
  `sw_gui_save_load_dtors_novtable.cpp`.
- NONMATCH kept: 0x006C8790 WriteGame (17; Format/SendSaveGameRequest
  argument registers eax/ecx/edx/esi colouring only), 0x006CBB60
  HandleSaveButton (70; original reads pControl->m_n4c before the pushes
  and spills the int64 comparand to an extra 4-byte slot, which shifts every
  frame offset; structure otherwise identical).
- functions.csv lacks the starts 0x006C82F0, 0x006C8410, 0x006C8460,
  0x006C8470, 0x006C8750, 0x006C8900, 0x006CB040, 0x006CC0E0; the
  annotations restore them.
- Naming: mac_names puts "VerifyLoadGame" on 0x006CC060, but that is the
  save-name panel destructor (called from ??_G 0x006CC040); the real
  VerifyLoadGame is 0x006CC0E0 (the BTN_SAVELOAD load callback). Descriptive
  names: DeleteGame, PromptDeleteGame, ExtractSaveGame,
  PrepareGameInProgress, LoadBarCallback, HandleCancelButton, SetName and
  the entry flag names (m_bSavedGame, m_bCorrupt, m_bRebootAutoSave,
  m_bPCAutoSave, m_bMissingContent, m_bCheatUsed). ExtractSaveGame and
  PrepareGameInProgress are thiscall members that never use `this` (callers
  leave ECX untouched, RET 8/0xC).
- Shared-header changes: CSWGuiControl slot 0x28 is now `GetPanel()`;
  CSWGuiLabel::SetFillImage(const CResRef&) inline wrapper; CSWGuiManager
  Save/MoveMouseToPosition/RestoreOldMousePosition and g_pGuiManager;
  CClientExoApp DisplayMainMenu/SetLoadBarProgress/SendSaveGameRequest/
  GetTutorialFlag/HideLoadScreen; CExoResMan CreateDirectory2/
  Add/RemoveResourceDirectory/CleanDirectory/NukeDirectory/GetFreeDiskSpace;
  CExoArrayList::InsertInline; macros L_RADIO_BUTTON_INLINE
  (inventory_equip.h), L_RESREF_IMPLICIT_COPY (bark_bubble.h),
  L_SW_GUI_SAVE_LOAD_FULL (hides the padded CSWGuiSaveLoad in
  options_in_game.h). m_pOwner (+0x34) stays void* (keymap button assigns
  its owner panel); cast to CSWGuiControl* for GetPanel.
- Lessons: (1) Message-box accepted checks want a `CSWGuiMessageBox* pBox`
  local (gives `mov eax,[eax+0x98]; test [eax+0x64]`). (2) A temporary
  CExoString/CResRef whose address is pushed with `lea` instead of the
  ctor's eax comes from an implicit conversion (`RemoveResourceDirectory(
  "TEMP:")` style) or a named local in a block; explicit `CExoString(..)`
  temps push eax. (3) A CResRef temp built before the other arguments are
  pushed means an inline wrapper (`label.SetFillImage(img)` ->
  `m_cBorder.SetFillImage(img, 0)`). (4) Member buttons called through the
  vtable need a pointer expression (`(&m_btnDelete)->SetEnabled(1)`).
  (5) A byte default read through a stack slot (`mov dl,[esi+x]; mov
  [esp+n],dl; mov eax,[esp+n]; push eax`) is a named `unsigned char` local.
  (6) EH state numbering follows source order: when the second-laid-out
  branch has the lower states, swap the if/else. (7) `n = max(..)` with the
  `cmp ebx,eax; jbe; mov eax,ebx; mov ebx,eax` shape is
  `if (nMax > n) n = nMax; nMax = n;`. (8) The add-then-shift insertion in
  PopulateGameList is an inline Insert(t, 0) using the old count as the
  loop counter. (9) var.py now also captures stderr (compile failures).
- Next in-range queue: save/load remainder LoadGame 0x006CB0E0 and
  LoadPCAutoSave 0x006CA250 (both need many CClientExoApp/CServerExoApp
  declarations); the 2 NONMATCHes above; party selection remainder
  (AcceptParty 0x006BE560, OnPanelAdded 0x006BEEB0, OnEnter 0x006BF5B0);
  upgrade remainder (0x006C2F80, 0x006C33A0, 0x006C3630, 0x006C3EA0,
  0x006C4D70, 0x006C5370, 0x006C6500); CSWMGBehavior 0x006DB4F0-0x006DBD30;
  projectile modes 0x006D4520-0x006D55F0; title movies 0x006DCE80-0x006DDAF0.

## Character creation frame, step panels and AI script picker (2026-10-27, worker-l-s10)

- Whole folder: **640/710 -> 679/753**. 43 functions annotated, **39 NEW
  exact**, 4 retained NONMATCH, no prior MATCH regressed. (The s9 round left
  no notes section: it added the class selection screen, CResSSF and the MG
  track behaviours, see its commits.)
- Header `l/sw_gui_main_chargen.h` (on top of `sw_gui_levelup_chargen.h`):
  CSWGuiMainCharGen (0x22DC, vtable 0x007592A8; 27 labels, model view, name
  strings), CSWGuiQuickOrCustomPanel (pointer fields only), CSWGuiQuickPanel
  (0x1360), CSWGuiCustomPanel (0x2188), CSWGuiPortraitCharGen /
  CSWGuiNameChargen (ctor + SetParentPanel only), CSWCAnimBase, CAurObject
  (slots 0x18 PlayAnimation, 0x104 GetScene). Header `l/sw_gui_script_select.h`:
  CSWGuiScriptSelect (0xC40, vtable 0x007590A8) with CExoArrayListZ (ctor +
  zeroing dtor + releasing SetSize).
- MATCH, main chargen (`sw_gui_main_chargen.cpp`, `_ctor.cpp`, `_dtor.cpp`):
  0x006EA760 SetVisiblePanel, 0x006EA810 OnPanelAdded, 0x006EA830
  RemoveSubPanels, 0x006EA910 DetermineClass, 0x006EA9B0 SetNameString,
  0x006EA9F0 PortraitChanged (x87 camera framing), 0x006EAC30
  AbilitiesChanged, 0x006EB010 ClearAbilities, 0x006EB320 BeginPlayGame,
  0x006EB340 Draw, 0x006EB420 ctor (2752 bytes), 0x006EBEE0 ??_G,
  0x006EBF00 dtor (novtable TU).
- MATCH, custom panel (`sw_gui_custom_panel.cpp`, `_ctor.cpp`, `_dtor.cpp`):
  0x006EEF70 CSWGuiMainCharGen::NameChanged (emitted in this object),
  0x006EF120 OnSelectPortrait, 0x006EF220 OnSelectPlay, 0x006EF240
  OnSelectName, 0x006EF2D0 OnSelectFeats, 0x006EF360 OnSelectSkills,
  0x006EF3F0 OnSelectAbilities, 0x006EF480 CancelledByChild, 0x006EF5B0
  ConfirmCancel, 0x006EF610 HandleInputEvent, 0x006EF6A0 OnCancelPressed,
  0x006EF730 ctor, 0x006EFB20 ??_G, 0x006EFB40 dtor.
- MATCH, quick panel (`sw_gui_quick_panel.cpp`): 0x006EFD60 OnSelectPlay,
  0x006EFD80 OnSelectName, 0x006EFE10 OnSelectPortrait, 0x006EFF10
  CancelledByChild.
- MATCH, script select (`sw_gui_script_select.cpp`): 0x006E9AC0 dtor,
  0x006E9BC0 HandleInputEvent, 0x006E9CA0 SetCreatureID, 0x006E9D50
  SetDescription, 0x006E9E70 OnScriptSelected, 0x006E9EF0 ??_G, 0x006E9F10
  CreateOption, 0x006E9FE0 OnEnterOption.
- NONMATCH kept: 0x006EEFD0 custom SetState and 0x006EFC10 quick SetState
  (35 each; the original strength-reduces the step arrays to element base
  pointers and does read-modify-write on the text flags, mine keeps
  flag-field pointers; same tie as level-up ChangeState 0x006EE0E0; tried
  bitfield writes, SetHighlighted, pointer locals, ul masks), 0x006EFFD0
  DoQuickStats (97; every byte argument goes through a stack slot in the
  original, `mov [esp+n],cl; mov eax,[esp+n]; push eax`; named locals and
  getters did not reproduce it), 0x006EA000 ScriptSelect ctor (15; frame is
  8 bytes smaller in the original, stack-slot packing only).
- functions.csv lacks the starts 0x006EB340, 0x006EF120, 0x006EF220,
  0x006EF240, 0x006EF2D0, 0x006EF360, 0x006EF3F0, 0x006EF5B0, 0x006EF6A0,
  0x006EFD60, 0x006EFD80, 0x006EFE10, 0x006E9FE0; the annotations restore
  them.
- Naming: descriptive RemoveSubPanels, ConfirmCancel, OnScriptSelected,
  CreateOption, OnEnterOption, CSWClass::GetACBonus (0x005BE770),
  CSWCLevelUpStats ClearPackage/AddDefaultPackageFeats/
  AddDefaultPackageSkills/UpdateAbilityModifiedSkillRanks, the class package
  ability bytes CSWClass +0x17B..+0x180 and CSWSCreatureStats +0x13A
  m_nAIState. The sub-step panels store the custom panel in their +0x68
  parent (declared CSWGuiLevelUpPanel*), cast at the store.
- Shared-header changes (whole folder verified): CSWCCreatureStats m_nSTR/
  m_nDEX/m_nINT (0x34/0x35/0x37), m_nACBonus 0x40, m_nAppearance 0x8C;
  CSWClass package bytes; CSWSCreatureStats m_nAIState; CExoArrayListL
  SetSizeFree; CSWCCreature m_pAppearance (0x21C), GetAnimBase,
  CGPauseCycle, SetAppearance; CSWGuiControl slot 0x34 GetHeight(); Vector
  operator-= / Normalize; CAurCamera slots 0x18 SetPosition / 0x1C
  SetOrientation.
- Lessons: (1) The original sometimes duplicates a short helper's body
  instead of calling it (custom ConfirmCancel/OnCancelPressed inline the
  CancelPanel body while HandleInputEvent calls it out of line): write the
  body twice. (2) `dec eax; jne` on a member compare comes from a
  one-case `switch`. (3) Arguments computed inside the call expression
  (`YawPitchRoll(Yaw(v) + 180.0f, 90.0f - Pitch(v), 0.0f)`) fstp straight
  into the pushed slot; separate float locals cost a frame slot. (4) A
  vptr-less implicit dtor of a big panel needs only the novtable TU (labels
  out of line). (5) A label fill image from a by-value virtual return
  (`push eax`) wants the inline label wrapper with the virtual called
  directly, not through a by-value inline helper. (6) Stat reads into named
  locals in the order of the original loads fixed AbilitiesChanged
  (hit die before base HP). (7) `CAurScene* pScene = member; if
  (x->GetScene() != pScene) Set(member)` reproduces the reload after a
  virtual call.
- Next in-range queue: the 4 NONMATCHes above plus level-up ChangeState
  (same loop shape); quick panel remainder is outside the bucket (ctor
  0x006F0390, OnPanelAdded 0x006F0260, HandleInputEvent 0x006F0280);
  save/load LoadGame 0x006CB0E0 / LoadPCAutoSave 0x006CA250; party selection
  remainder (0x006BE560, 0x006BEEB0, 0x006BF5B0); upgrade remainder
  (0x006C2F80, 0x006C33A0, 0x006C3630, 0x006C3EA0, 0x006C4D70, 0x006C5370,
  0x006C6500); title movies 0x006DCE80-0x006DDAF0; projectile modes
  0x006D4520-0x006D55F0; RK4 / camera helpers 0x006D0A40-0x006D30C0.

## Inventory, equipment and container callbacks (2026-11-09, worker-l-s11)

- Whole folder: **679/753 -> 727/805**. 52 functions annotated, **48 NEW
  exact**, 4 retained NONMATCH, no prior MATCH regressed.
- Header `l/sw_gui_inventory.h` (on top of `sw_gui_store.h`): equip slot
  table, CSWCCreatureStats race, CSWCCreaturePortraitVirtuals (slot 0x148),
  CSWSCombatRound, CSWCMessage, CSWSArea/CSWCArea, CSWCItem. Header
  `l/sw_gui_in_game_character.h` (on top of `sw_gui_in_game_abilities.h`):
  the portrait buttons of CSWGuiInGameCharacter (+0x4B2C/+0x4CF0/+0x5078,
  member index +0x59F0).
- MATCH, inventory (`sw_gui_inventory.cpp`, `_switch.cpp`): 0x006B2A80
  SetNextFilter, 0x006B2C90 CheckFilter, 0x006B2DC0 UpdatePortraits,
  0x006B2EC0 OnChangeCharacter, 0x006B2FD0 OnSwitchLeft, 0x006B3250
  OnSwitchRight, 0x006B3ED0 HandleInputEvent, 0x006B4430 OnPanelRemoved,
  0x006B4BB0 Draw, and the message-box refusals 0x006B26E0 / 0x006B2740 /
  0x006B27A0 / 0x006B2800 / 0x006B2860.
- MATCH, container (same file): 0x006B5250 OnPanelRemoved, 0x006B51D0
  OnPanelAdded, 0x006B7170 GiveItem, 0x006B92F0 HandleInputEvent.
- MATCH, item entry (`sw_gui_inventory_entry.cpp`): 0x006B4D40 Draw,
  0x006B4DB0 Initialize, 0x006B5060 SetCanUse; 0x006B60D0
  ??_GCSWGuiRadioButton (emitted by the existing radio dtor).
- MATCH, equip (`sw_gui_equip.cpp`, `_switch.cpp`): 0x006B4C90
  GetSlotItemIdIndex, 0x006B5600 GetIcon, 0x006B56B0 SetDescription,
  0x006B5890 EquipItem(CSWSItem*), 0x006B5910 UnequipItem, 0x006B59F0
  ShowCantEquipMessage, 0x006B60F0 OnSwitchLeft, 0x006B6370 OnSwitchRight,
  0x006B65E0 CopyItem, 0x006B66A0 ItemExists, 0x006B7590 SetCharacter,
  0x006B7680 ShowDescription, 0x006B7E30 CloseDescription, 0x006B8E10
  OnPanelRemoved, 0x006B9160 OnOKPressed, 0x006B91C0 AddItemEntryToList,
  0x006BA820 OnChangeCharacter.
- MATCH, other panels: abilities 0x006AC3E0 OnSwitchLeft, 0x006AC660
  OnSwitchRight, 0x006AC8D0 SetButtonColor
  (`sw_gui_in_game_abilities_switch.cpp`); character 0x006AEC80
  DoAutoLevelup, 0x006AECD0 SetLabelBonusPenaltyColour, 0x006AF350
  OnChangeCharacter, 0x006AF450 OnSwitchLeft, 0x006AF6D0 OnSwitchRight
  (`sw_gui_in_game_character_switch.cpp`); 0x006E9B90
  CSWGuiScriptSelect::OnPanelAdded.
- NONMATCH kept: 0x006B5760 EquipItem(oid) (13; the original pushes the base
  item type before loading g_pRules and computes `bClear == 0` before the
  object's vptr load; accessor/local/early-return shapes all gave the same
  schedule), 0x006B8EB0 OnSlotSelected (44; pControl in ebx vs ebp, and my
  four message-box blocks tail-merge where the original keeps the 0x5E2 and
  0xA568 copies with the vptr load before the strref push), 0x006B25E0
  inventory OnControlSelected (27; pure esi/edi swap of `this` and the
  server/message-box pointer), 0x006B3D10 inventory OnControlEntered (15;
  ebx/ebp swap of item id and item, pControl load order).
- functions.csv lacks the starts 0x006B25E0, 0x006B26E0, 0x006B2740,
  0x006B27A0, 0x006B2800, 0x006B2860, 0x006B2FD0, 0x006B3250, 0x006B3D10,
  0x006B4D40, 0x006B4DB0, 0x006B51D0, 0x006B60F0, 0x006B6370, 0x006B7170,
  0x006B8EB0, 0x006B9160, 0x006BA820, 0x006AC3E0, 0x006AC660, 0x006AC8D0,
  0x006AEC80, 0x006AF450, 0x006AF6D0, 0x006E9B90 (they sit inside larger
  rows: 0x006B2EC0, 0x006B4C90, 0x006B5060, 0x006B60D0, 0x006B6DC0,
  0x006B8E10, 0x006ABEC0, 0x006AF350 are much shorter than listed).
  Unannotated starts seen but not attempted: 0x006AE5F0, 0x006AEAC0,
  0x006AED90, 0x006AEDE0, 0x006B0E40, 0x006B2250, 0x006BA3F0, 0x006BB740,
  0x006BB7B0, 0x006BCF50, 0x006C5510, 0x006D10D0, 0x006D14D0, 0x006D1FC0.
- Naming: descriptive OnCantEquip/OnNotUseable/OnNotUseableOnHawk/
  OnFullHealth/OnSquadFullHealth (inventory refusals), OnSlotSelected,
  OnItemSelected, GiveItem, CSWCCreature::CanEquipItem (0x00616520),
  CSWSArea +0x2B0 m_bNoEquipChange, CSWSCreature +0x4E0 m_bCombatState,
  CSWBaseItem +0x04 m_nEquipableSlots, CGuiInGame +0xBC1
  m_nInventoryFilter, CSWCMessage::SendPlayerToServerCloseContainer
  (0x00677630). Equip 0x006B7680 is ShowDescription (was
  SetDescriptionVisible), 0x006B9160 OnOKPressed.
- Shared-header changes (whole folder verified): CSWGuiManager
  PlayGuiSoundI(unsigned char); CGuiInGame m_nInventoryFilter and
  HideContainerGui; CServerExoApp::GetObjectArray; CClientExoApp
  GetItemByGameObjectID; CSWGuiBorderParams::GetFillImage() (+0x40);
  CGameObject slot 0x38 AsSWSItem, CGameObjectArray; CSWParty m_nMembers /
  GetPartyMember; CSWCObject +0x20 m_pArea; CSWCCreature CanEquipItem /
  PerformGiveItem; CSWRules::GetBaseItemArray; CSWGuiButton SetFillImage /
  SetHilightFillImage wrappers; CExoArrayList SetSize / AddUnique;
  CSWSItem ctor/CompareItem/CopyItem, bit 7 of +0x288, size 0x29C;
  CSWSCreature +0x1CC, +0x4E0, +0x9C8, +0xAB0, +0xAC0 and the action
  queue calls; CItemRepository::ClearNewItems; CSWBaseItem +0x04, +0x1B;
  CSWGuiContainer m_nMode is a byte; inventory +0x1DD8..+0x1DE4 fields.
- Lessons: (1) Three spellings of the app pointer give three schedules:
  `g_pAppManager->m_pXExoApp->F(a)` loads the pointer after the argument,
  `GetXExoApp()` gives `mov eax,[g]; mov edx,arg; mov ecx,[eax+N]`, and a
  `CXExoApp* p = ...;` local loads the pointer before the pushes. Mixed use
  inside one function is common (ItemExists, GiveItem, ShowDescription).
  (2) The compiler reuses an argument's stack slot for an address-taken
  local when the argument is dead (ItemExists `&pObject` at [esp+4]).
  (3) `xor eax,eax; mov al,[m]; push eax` is an `unsigned char` parameter;
  a `char` or `int` parameter gives movzx or no extension. (4) Ten visible
  bits written one after another (store before the next load) need a
  pointer helper (`SetControlVisible(&label, b)`), direct bitfield writes
  interleave two at a time. (5) `for (..; cond && !bFound; ..)` reproduces
  the `test ebp,ebp; jne exit` at the top of the body. (6) A string that is
  destroyed at the end of each if/else arm (shared dtor tail, `lea` pushed)
  is a named local in that arm, not a temporary. (7) matchcmp masks call
  targets and global addresses: read the colour/global addresses from the
  original listing (SetLabelBonusPenaltyColour matched with the wrong
  colours until checked).
- The inventory (0x006B40C0) and equip (0x006B8840) destructors have real
  bodies (ClearControls, delete the entries, count = 0) and inline the
  CSWGuiLabel destructor (vtable 0x0073E5B8, text at +0xD0, border at
  +0x5C, then CSWGuiControl::~CSWGuiControl 0x00419A20); they need a label
  class with real CSWGuiBorder/CSWGuiText members and an implicit dtor.
- Next in-range queue: the 4 NONMATCHes above; equip HandleInputEvent
  0x006BA3F0, UpdatePortraits 0x006B5B60, OnItemSelected 0x006B7920,
  OnEnterSlot 0x006B9470, UpdateInventory 0x006B9970; inventory
  PopulateItemListBox 0x006B4810 and 0x006B44D0; container SetContainer
  0x006B8130, SetupGiveMode 0x006B8410, ctor 0x006B6DC0; the two panel
  dtors with the inline label dtor; character HandleInputEvent 0x006B2250,
  RotateCharacter 0x006AED90/0x006AEDE0; dialog camera family
  0x006BB590-0x006BB7B0 and 0x006BCF50 (no header yet); debug
  CSWGuiCreateItemSubMenu 0x006BDDE0-0x006BE2B0; earlier queues (save/load
  LoadGame 0x006CB0E0, party selection 0x006BE560/0x006BEEB0/0x006BF5B0,
  upgrade remainder, title movies, RK4 / camera helpers).

## Debug sub menu, dialogue/combat cameras, AoE object, quest items (2026-11-10, worker-l-s12)

- Whole folder: **727/805 -> 762/846**. 41 functions annotated, **35 NEW
  exact**, 6 retained NONMATCH, no prior MATCH regressed (762 = 727 + 35).
- MATCH, `CSWGuiCreateItemSubMenu` (swguiSubCreateItem.o, vtable 0x00756B30,
  header `l/sw_gui_create_item_sub_menu.h`, `sw_gui_create_item_sub_menu.cpp`):
  0x006BD980 HandleInputEvent, 0x006BDBA0 OnBack, 0x006BDBE0 OnItemSelected,
  0x006BDC60 ctor, 0x006BDDE0 ??_G, 0x006BDE00 implicit dtor, 0x006BDEA0
  LoadItems (UTI scan via GetResOfType / CResGFF, 1040 bytes), 0x006BE2B0
  ClearItems. Mac names: class, LoadItems, ClearItems.
- MATCH, `CSWBehaviorCameraDialog` (vtable 0x00756AE8, `l/sw_behavior_camera_dialog.h`):
  0x006BB590 SetShot, 0x006BB610 GetLineOfActionSide, 0x006BB740 GetType
  (0x106D), 0x006BB750 dtor, 0x006BB790 ??_G, 0x006BCEC0 AddLineOfAction.
  symbols.csv labels SetShot `CAurCamera::SetShot` from its caller, but the
  callee works on the behaviour returned by GetCamera(0x106D) (CAurBehavior
  +0x10 gob), so it is defined on CSWBehaviorCameraDialog.
- MATCH, `CSWBehaviorCameraCombat` (swbehaviorcameracombat.o, vtable
  0x00757BDC, `l/sw_behavior_camera_combat.h`): 0x006D14D0 GetType (0x1070),
  0x006D1870 SetTurn, 0x006D18C0 ??_G (its implicit dtor is the folded
  0x006DB4E0 alias). Camera spring integrators (`sw_camera_clip.cpp`, the
  playable source's CSWRK4Acceleration / split; names kept from
  symbols.csv: CSWCameraClipBase / CSWCameraClipHelper): 0x006D0A40 state
  operator/, 0x006D0FC0 / 0x006D10D0 GetAccelerationVector, 0x006D1030
  SetVelocity, 0x006D10B0 helper ctor, 0x006D1190 Update. The helper's
  +0x40/+0x44/+0x48 are mass / gain / release gain (set from the
  GetFarClip* option getters in the combat ctor).
- MATCH, `CSWCAreaOfEffectObject` (vtable 0x00757E28, `l/sw_c_area_of_effect.h`
  on top of `sw_c_projectile.h`): 0x006D90D0 ReleaseModel, 0x006D91D0
  UnloadModel, 0x006D91F0 SetAnimation, 0x006D9300 ResetInstanceDuration,
  0x006D9520 SetMoveTimer, 0x006D9820 dtor, 0x006D9890 ??_G. Instances are a
  `new CSWCAnimBase[n]` in CSWCObject::m_pAnimationBase (+0x68).
- MATCH, `CSWGuiQuestItem` (`l/sw_gui_quest_item.h`, rewritten
  `sw_gui_quest_item_lifecycle.cpp`): 0x006D2460 the out-of-line
  CExoLinkedList<CSWGuiInGameItemEntry> ctor, 0x006D24D0 HandleInputEvent,
  0x006D2580 OnControlEntered, 0x006D26C0 ctor, 0x006D28F0 dtor (the old
  empty unannotated dtor is replaced; ??_G/OnPanelAdded still exact).
- NONMATCH kept: 0x006BDA90 sub menu CreateButton (8; same `this` spill tie
  as the three debug-menu copies; pointer-local border forms reorder the
  calls), 0x006BB670 dialog camera ctor (3; &m_rListener formed in ebp
  before its zero store), 0x006D12B0 combat ctor (3; EH link reload before
  the last fstp), 0x006D22C0 combat RestoreSettings (19; edi/ebx swap of
  PC / app pointer and the target-position slot is shared with the checked
  position in the original), 0x006D0B20 clip base ctor (16; two zero Vector
  temporaries live at once, interleaved re-zero stores), 0x006D29F0 quest
  PopulateItemListBox (14; array-before-count load in the inline append,
  then eax/ecx/edx rotation).
- functions.csv lacks the starts 0x006BB740, 0x006BDBA0, 0x006BDBE0,
  0x006D14D0, 0x006D10D0, 0x006D91D0 (folded into the previous row); the
  annotations restore them. 0x006BCF50 (dialog camera Control, inside the
  0x006BCEC0 row) is still unannotated.
- Shared-header changes (whole folder verified): bark-bubble CResRef
  CopyToString; CClientExoApp GetPlayerCreatureId / CreatureAcquireItem;
  CGuiInGameDebug HideSubItemCreateMenu / ShowItemCreateMenu; projectile
  CSWCAnimBase slot 0x38 GetAnimationLength (was v38); bark-bubble
  CExoLinkedList ctor (out of class) / dtor / IsEmpty / RemoveHead / AddTail
  and CExoLinkedListInternal members; CSWGuiInGameItemEntry::SetItem
  (0x006B6710).
- Lessons: (1) **Empty-state SEH frame without a list member**: a member
  with an inline ctor containing a dead throwing call (`GobRef(Gob* p = 0)
  { m_p = p; if (m_p) m_p->m_lWatchers.Add(&m_p); }`) gives the frame with
  no state stores. (2) `x = f() ? A : B` style returns: a ternary of two
  struct temporaries (`return f != 0 ? State(a, b) : State(0, 0)`) builds
  one temp and copies it out; an if/else into a local adds a second copy.
  (3) `a[i].x = Call(); ... return a[i].x;` reproduced the original store
  order where a named local did not (AddLineOfAction). (4) A by-pointer
  local for the GetString result (`CExoString* p = ...; ref = p->CStr();`)
  fixed the eax/ecx choice in OnItemSelected. (5) `if (a < b)` vs `if (b >
  a)` decides which operand is loaded first. (6) `pObj->Get(1)` on a long
  call chain pushes the argument first; split the chain into a local to push
  it after. (7) The combat ctor's `and esp, -8` frame appeared on its own
  from the six CExoString/float 2DA reads (copied from n's stick ctor).
- Next in-range queue: dialog camera big pieces (CalculateLineOfActionSide
  0x006BC600, Control 0x006BCF50, 0x006BD260, 0x006BB7B0); combat camera
  0x006D14E0 / 0x006D16F0 / 0x006D1760 / Control 0x006D1FC0 / CheckPosition
  0x006D18E0 and the RK4 step 0x006D0BD0 (all x87-heavy); AoE remainder
  (ctor 0x006D8310, LoadEffect 0x006D8490, SetEnvMap 0x006D9590 /
  0x006D96E0, 0x006D98B0, 0x006DA330, 0x006DA520); title movies
  0x006DCE80-0x006DDAF0; earlier queues (equip/inventory/container
  remainder, save/load LoadGame, party selection, upgrade remainder).

## Title movies, AoE object, mini-game gun/bullet behaviours (2026-11-10, worker-l-s13)

- Whole folder: **762/846 -> 779/865**. 19 functions annotated, **17 NEW
  exact**, 2 retained NONMATCH, no prior MATCH regressed (779 = 762 + 17).
- MATCH, `CSWGuiTitleMovies` (vtable 0x00758130, new standalone header
  `l/sw_gui_title_movies.h`, `sw_gui_title_movies.cpp`): 0x006DCE80
  HandleInputEvent, 0x006DCF10 OnMoviePicked, 0x006DCFE0 dtor (inline label
  dtor), 0x006DDAF0 ??_G, 0x006DD100 PopulateMovies (2064 bytes), 0x006DD910
  ctor.
- MATCH, `CSWCAreaOfEffectObject` (`sw_c_area_of_effect.cpp`): 0x006D8310
  ctor, 0x006D96E0 SetEnvMapPart, 0x006DA330 UpdateInstances (descriptive),
  0x006DA9B0 AIUpdate.
- MATCH, mini-game behaviours (`l/sw_mg_behavior.h`): `CAurGunEmitter`
  (`aur_gun_emitter.cpp`, vtable 0x00757F70) 0x006DADF0 ctor, 0x006DAE60
  GetType (0xAAAD), 0x006DAE70 Control, 0x006DAEC0 Fire (slot 0x24; m/ calls
  it Restart); 0x006DB3D0 CSWMGBehaviorTrackAndFire::Control
  (`sw_mg_behavior.cpp`); `CSWMiniBulletInfo` (`sw_mg_bullet.cpp`, vtable
  0x00757F98, name from m/) 0x006DAFE0 ctor, 0x006DB080 GetType (0xAAAB).
- NONMATCH kept: 0x006D9590 AoE SetEnvMap (28; the inlined CSWCArea::GetEnvMap
  copy keeps the area pointer in esi and loads three dwords before storing,
  mine uses eax and alternates; tried named/temp/inline-helper/pArea forms),
  0x006DB090 bullet Control (2; `fstp` of the scaled fDelta comes before the
  second push in the original).
- functions.csv lacks the starts 0x006DCF10, 0x006DAE60, 0x006DAE70,
  0x006DAEC0, 0x006DB080, 0x006DB090, 0x006DB3D0; the annotations restore them.
- Naming: CClientOptions 0x0061D3D0 is declared GetMovieShown here (c/ names it
  GetTexturePackBit; CTwoDimArrays +0x11C is the movies table, c/ calls it
  m_pTexturePacks). CSWCAnimBase slot 0x04 declared AttachToScene(CAurScene*)
  in `sw_c_projectile.h` (symbols.csv: SetVisible; it forwards to the model's
  slot 0x4c and the AoE passes its scene). AoE fields 0x124-0x178 named
  descriptively (m_nFinishTimer, m_bFinished, m_bEnding, m_cImpactSound,
  m_nImpactSoundChance, m_nEffectRow, m_bSoundStarted, m_oidTarget);
  m_pVisualEffect is a CSWCVisualEffect (k/ layout, +0x114 m_bDone).
- Shared-header changes (whole folder verified): `sw_c_projectile.h` CExoString
  GetLength; CResRef(CExoString), CopyToString(CExoString&), operator==,
  GetResRef; CAurObject slot 0x64 GetPosition; CSWCAnimBase 0x98/0x9c
  SetEnvMapPart/SetEnvMap; CSWCObject 0x94 AttachToScene, 0xac/0xb0 env map
  slots; CGameObject 0x0c AsSWCObject; CSWCArea/CSWCModule; C2DA/C2DAs
  visual effects table; CSWRules +0xB8; CWorldTimer SubtractWorldTimes.
  `sw_c_area_of_effect.h` m_pnAnimationLengths is unsigned long* (the AIUpdate
  compares are unsigned; SetAnimation still exact). `sw_mg_behavior.h`
  CAurBehavior, CAurObject/Gob slots, Quaternion::GetYAxis, Vector + and
  float*Vector, CExoSound, CAurBehaviorAttach +0x10 m_pObject and slot 0x24.
- Lessons: (1) Two GetWorldTime outputs: the store order of the two results
  (`m_nCreateDay = nDay;` before `m_nCreateTime = nTime;`) decided which local
  got the lower slot; declaration order and names did not matter. (2)
  CExoArrayList::Insert inlined as `n = num; grow-if-full; element[num++] = t;
  for (; n > i; n--) shift; element[i] = t;` decrementing n itself (a separate
  loop counter cost 8 frame bytes). (3) Struct-returning call chains read
  through eax (`fmul [eax]`) only when used inside one expression with inline
  operators; binding them to const references or locals copies them out. (4)
  `if (x <= limit) {...} else x -= limit;` reproduces the shared subtract tail
  placed after the loop body. (5) A 2DA chain `g_pRules->Get2DArrays()->
  GetMovies()->GetINTEntry(...)` with inline accessors loads the table before
  the pushes; direct members interleave. (6) `GetClientExoApp()` (TU-local
  inline) vs `g_pAppManager->m_pClientExoApp` again picked the load order
  (OnMoviePicked, AIUpdate). (7) The scratch reloc checker (compile, walk the
  COFF relocations, resolve the original's rel32/abs32 at the same offset)
  verified every callee, global and string of this batch.
- Next in-range queue: AoE SetPosition 0x006DA520 (x87 + walkmesh helper,
  0x004578F0), PlaceInstance 0x006D98B0 and LoadEffect 0x006D8490 (large);
  CSWMGBehaviorTrack::Control 0x006DB180 (x87); the two NONMATCHes above;
  earlier queues (dialog camera 0x006BC600/0x006BCF50/0x006BD260/0x006BB7B0,
  combat camera x87 set, equip/inventory/container remainder, save/load
  LoadGame 0x006CB0E0, party selection, upgrade remainder).

## Character screen, dialog family, inventory/equip dtors (2026-11-10, worker-l-s14)

- Whole folder: **779/865 -> 800/893**. 28 functions annotated, **21 NEW
  exact**, 7 retained NONMATCH, no prior MATCH regressed (800 = 779 + 21).
- MATCH, `CSWGuiInGameCharacter` (`l/sw_gui_in_game_character.h` now has the
  full 0x59F8 layout from the destructor and includes
  `l/sw_gui_script_select.h`; new `sw_gui_in_game_character_view.cpp`):
  0x006AE990 SetModText, 0x006AEBF0 AttachModel, 0x006AED20 RotateCharacter,
  0x006AED90 / 0x006AEDE0 RotateCharacterLeft/Right, 0x006B0BB0
  ShowLevelUpGUI, 0x006B0CB0 StartAutoLevelup, 0x006AF970 destructor;
  0x006AEAC0 CSWGuiBlackenedLabel::Draw (class now in `sw_gui_dialog.h`,
  vtable 0x00753CC0). `sw_gui_levelup_chargen.h` only forward-declares the
  character class; its two .cpp users include the character header.
- MATCH, CSWCAnimBase tail (nwcanimbase.o, `l/sw_c_anim_base.h` copied from
  k/, `sw_c_anim_base_tail.cpp`): 0x006A03A0 ??_G, 0x006A03C0 PlayAnimSet
  (vtable slot 0x44, k/ name), 0x006A03E0 Hilite, 0x006A0440 UnHilite.
- MATCH, 0x006D9070 ??_ECSWCAnimBaseVisualEffect (vector deleting dtor; an
  unannotated `new CSWCAnimBaseVisualEffect[n]` helper in
  `sw_c_area_of_effect_vfx_array.cpp` instantiates it, as LoadEffect does).
- MATCH: 0x006E6930 CSWGuiInGameGameplay::OnDifficultyRight, 0x006C33A0
  CSWGuiUpgrade::Draw (model spin; CAurObject slots 0x60/0x68/0x108 in
  `sw_gui_upgrade.h`), 0x006B40C0 ~CSWGuiInGameInventory
  (`sw_gui_inventory_dtor.cpp`), 0x006B8840 ~CSWGuiInGameEquip
  (`sw_gui_equip_dtor.cpp`), 0x006A85B0 CSWGuiDialog ctor (replaces the old
  NewDialog vtable stub), 0x006A8400 CSWGuiDialogComputer::SetType, 0x006A95F0
  CSWGuiDialogComputerCamera ctor (`sw_gui_dialog_computer.cpp`).
- NONMATCH kept: 0x006B2250 character HandleInputEvent (58, only the
  this/pClient esi/edi swap; jump table identical), 0x006DE000 options
  checkbox SetExtent (9; the constant 25 lands in ecx, not edx, and `this` is
  saved earlier), 0x006B8130 container SetContainer (25; the original's
  inline SetSize(n) has no inner `num = 0` store), 0x006B8410 SetupGiveMode
  (16; loop bound register and an `and [mem]` RMW), 0x006A86A0
  CSWGuiDialog::SetReplies (62, only an ebx/edi swap of `this` and the zero /
  loop register; the version with the list declared first is 19 diffs but
  has the wrong init order), 0x006A8240 DialogComputer::UpdateSkills (26,
  ebp/edi swap only), 0x006A78C0 CSWGuiDialogTop ctor (22; the text params
  local is kept in ebp from its ctor return in the original).
- functions.csv lacks the starts 0x006AEAC0, 0x006AED90, 0x006AEDE0,
  0x006B2250, 0x006A03C0, 0x006A0440, 0x006D9070, 0x006E6930, 0x006C33A0,
  0x006DE000; the annotations restore them.
- Naming (Mac debug map): 0x006A86A0 / 0x006A8DE0 / 0x006A94E0 are
  SetReplies(int, CExoString*, unsigned long*, int, int) (were AddReply);
  0x006A8240 is CSWGuiDialogComputer::UpdateSkills (was LayoutEntries).
- Shared-header changes (whole folder verified): `sw_gui_dialog.h`
  CSWGuiManager +0/+4 mouse position and HandleMouseMove; CExoString
  operator==(const char*); CSWGuiTextParams m_vColor (+0x1c) and
  UpdateFont; CSWGuiLabelText m_extent; CSWGuiLabel::Draw (0x00417750) and
  an `L_GUI_LABEL_SET_STYLE_SLOT` SetStyle slot 0x9c; CSWGuiListBox list
  flags bitfield (bit 5 m_bMoreBelow) and m_nLastVisible (+0x2ca); CGuiInGame
  +0x10c m_bLevelUpAvailable; CClientExoApp GetCharacterChangeInProgress;
  CServerExoApp GetPlaceableByGameObjectID; CSWGuiPanel SetBackground /
  AddControl; CClientOptions SetDifficulty. `sw_gui_tutorial_box.h`
  CSWCCreature m_vOrientation (+0x30), CSWRules m_anExperienceTable (+0x38).
  `sw_gui_in_game_abilities.h` CSWCCreatureVirtuals slot 0x88 SetOrientation.
  `sw_gui_options_in_game.h` toggle GetSelectedBorder/GetSelectedHilight,
  checkbox SetExtent. `sw_gui_title_movies.h` ListBox ClearControls and
  CExoArrayList SetSize.
- Real sizes: CSWGuiTextParams is 0x54 bytes (stack temporaries in the dialog
  top ctor); CSWGuiLabel has a trailing dword at 0x13c. Only the
  `L_GUI_TEXT_PARAMS_EXACT` TU uses that layout so far.
- Lessons: (1) Two separate early `return 0;` tests keep the return block
  inline after the branch; a combined `||` moves it to the end (Hilite).
  (2) A string or CResRef passed through an implicit conversion
  (`InitControl(p, "NAME", 1)`, `SetBackground(sValue)`,
  `GetCExoStringEntry(n, "Col", &s)`, `SetText(nInt)`) is pushed by `lea`;
  an explicit `CExoString("...")` temp pushes the ctor's eax. (3) A block
  local `{ CExoString s(n); X(s); }` also pushes by lea. (4) A function-scope
  body (early `return` instead of a nested if) stopped MSVC sharing the
  `new` temp's stack slot (SetType frame size). (5) Inventory/equip panel
  dtors inline ~CSWGuiLabel: the title-movies drawable family (real border /
  text members) matches them in one try. (6) A pointer member reloaded for
  `this` after argument calls means the source used the member directly
  (`((V*)m_pPreview)->SetOrientation(... m_pPreview->m_vOrientation)`), not
  an inline accessor. (7) `if/else` with two identical calls tail-merges the
  pushed argument before the branch (upgrade Draw). (8) `int b = member; if
  (b)` picked eax for the flag load (ShowLevelUpGUI). (9) An inline setter
  (`GetOptions()->SetDifficulty((unsigned char)m_n)`) reads the value before
  the call, a plain member store reads it after.
- Next in-range queue: the NONMATCH register ties above;
  CSWGuiDialogComputer ctor 0x006A8EB0 (~1.5 KB, mechanical InitControl
  list plus skill icons) and CSWGuiDialogCinematic ctor 0x006A8B40;
  CSWGuiInGameCharacter::UpdatePortraits 0x006AEE30 (1.3 KB) and SetStats
  0x006AFDA0 / ctor 0x006B0E40; abilities UpdatePortraits 0x006ABEC0; visual
  effect StopVisualEffect 0x006A6590; upgrade OnSlotSelected 0x006C6500;
  earlier queues (AoE SetPosition / PlaceInstance / LoadEffect, camera x87
  set, save/load LoadGame, party selection).

## CSWCVisualEffectOnObject remainder (2026-11-10, worker-l-s15)

- Whole folder: **800/893 -> 808/904**. 11 functions annotated (4 NEW exact,
  7 retained NONMATCH) and 4 earlier NONMATCHes promoted; no prior MATCH
  regressed (808 = 800 + 4 + 4).
- NEW MATCH: 0x006A6590 StopVisualEffect(int) and 0x006A5A40
  ShowVisualEffect(int) (`sw_c_visual_effect_life.cpp`), 0x006A5FC0
  StartVisualEffect (`sw_c_visual_effect.cpp`, same TU as GetBeamNode /
  GetImpactDuration: the original keeps edx live across those calls),
  0x006A0DF0 AttachLight (descriptive; `sw_c_visual_effect_life.cpp`).
- Promoted: 0x006A6950 Reset (DetachAll-style per-phase dispatch), 0x006A4FA0
  LoadLight (separate early `return 0`s), 0x006A14E0 ApplySpeedBlur (two
  identical case bodies, cross-jumped into the dec chain), 0x006A1650
  AttachVFXModelToTargetModel (Mac name; was AttachModel; explicit
  `CExoString(m_nEffect)` row temp).
- NONMATCH kept: 0x006A6A50 Update(unsigned long) (7, beam vtable / reset arg
  registers), 0x006A4060 ApplyChunk (20, descriptive; offset-Vector slot
  shared with the "bloodcolr" temp, dead nSize reload), 0x006A4240 ApplyMirv
  (56, nEffect spilled to its home slot in the original) and 0x006A30E0
  ApplyLightsaberThrow (89, descriptive) in `sw_c_visual_effect_launch.cpp`
  (out-of-line `::operator-` 0x00423BC0), 0x006A2AF0 ApplyCheat (46, own TU
  `sw_c_visual_effect_cheat.cpp` because it inlines the member operator-;
  rotated eax/ecx/edx from the zero-Vector compare on, same rotation as Mirv),
  0x006A5000 ReAttachVisualEffect (88, this/placeable esi/edi swap; with an
  out-of-line GetHardCodedEffect call the registers agree), 0x006A38E0
  ApplyLightSource (case 33 keeps the target id in ebx across
  GetPlayerCreature; PlayAnimation argument load order).
- Naming (Mac debug map): ApplyImpactEffect -> ApplyCheat, ApplyEffect4240 ->
  ApplyMirv, ApplyDurationEffect -> ApplyLightSource, AttachModel ->
  AttachVFXModelToTargetModel(CAurObject*, CAurObject*, const CExoString&).
  Descriptive: ApplyEffect4060 -> ApplyChunk, ApplyBeamEffect ->
  ApplyLightsaberThrow, AttachLight. 0x006A6A50 is Update(unsigned long)
  (overload of 0x006A69E0 Update(unsigned short, unsigned long)).
- Layout fixes in `sw_c_visual_effect.h`: +0x34 is a Vector
  m_vTargetPosition (its default ctor reproduces the ctor's zeroing, the
  body stores were dropped); 0xac-0xc0 are int flags (m_bAC..m_bC0); +0xc8 is
  a byte m_bHasDuration; CSWCModule +0x48 is the area (m_pArea), area +0x184
  the scene (slot 0x68 SetPlayerLight, descriptive; was "GuiInGame/Panel");
  CGameObject slot 0 is the virtual dtor; CSWCObject +0x30 m_vOrientation and
  slots 0x14/0x4c/0x78/0x88/0x8c/0x94; CSWCCreature pad starts at 0x3c;
  CExoSoundSource has a 4-byte member (operator new 8); a minimal
  CSWCProjectile (0x1C8) for the launch effects; Quaternion(w,x,y,z) ctor
  body stores z,y,x,w (AttachLight's identity temp needed that order).
- Lessons: (1) Identical switch-case bodies are cross-jumped and keep a dec
  chain; merged `case 1: case 2:` becomes a range test. (2) Callees defined
  earlier in the same TU let cl keep edx live across them (Update,
  StartVisualEffect); put callers next to GetBeamNode/ResetBeamTimer. (3)
  Locals that the original keeps above the temporaries (StopVisualEffect
  floats) belong at function scope; block-scope floats grabbed the temp slot.
  (4) The same inline helper (GetHardCodedEffect) was not inlined into the
  large ReAttach body; the explicit if-chain inlines but swaps registers. (5)
  `m_nBeamTimer[3] = <inline returning 0/1000>` tail-merged with the
  GetImpactDuration store only through a TU-local inline helper.
- Next in-range queue: LoadVisualEffect/Load 0x006A1880 (4.7 KB, 2DA string
  lookups with LowQuality/LowViolence overrides, see todo.py); the NONMATCH
  register ties above; x87 UpdateSelfIlluminationPulse / UpdateAlpha;
  earlier queues (dialog computer ctor 0x006A8EB0, cinematic ctor 0x006A8B40,
  character UpdatePortraits/SetStats/ctor, abilities UpdatePortraits, AoE
  SetPosition / PlaceInstance / LoadEffect, camera x87 set, save/load
  LoadGame, party selection).

## Combat camera, AoE StopEffect, dialog ctors (2026-11-10, worker-l-s16)

- Whole folder: **808/904 -> 813/912**. 8 functions annotated (4 NEW exact,
  4 retained NONMATCH) and 1 earlier NONMATCH promoted; no prior MATCH
  regressed (813 = 808 + 4 + 1).
- NEW MATCH: 0x006D16F0 CSWBehaviorCameraCombat::GetNextYaw, 0x006D1760
  GetNextPosition (`sw_behavior_camera_combat.cpp`), 0x006D9350
  CSWCAreaOfEffectObject::StopEffect (`sw_c_area_of_effect.cpp`, inlines the
  TU's SetAnimation), 0x006A8B40 CSWGuiDialogCinematic ctor (new
  `sw_gui_dialog_cinematic.cpp`).
- Promoted: 0x006D12B0 combat camera ctor (`float fReleaseGain = ...GetFarClip3();
  m_cClip.m_fReleaseGain = fReleaseGain;` moved the saved-EH load).
- NONMATCH kept: 0x006D14E0 CalculateDesiredLocation (9, pCreature ebx/ebp
  swap only; needs `vStart.z = 0.0f` before the add, which folds z + 0),
  0x006D1FC0 Control (22; the original packs vHead and vPosition into one
  slot; a `{ vHead; vDesired; vSafe = CheckPosition(...); }` block gets the
  frame and registers but adds a copy), 0x006A8EB0 CSWGuiDialogComputer ctor
  (new `sw_gui_dialog_computer_ctor.cpp`, ~38-44, eax/ecx/edx rotations
  only), 0x006AD180 CSWGuiInGameAbilities::OnEnterSkill (6, description tail
  rotation).
- functions.csv lacks 0x006D1FC0 (combat camera Control, vtable slot 0x08);
  the annotation restores it.
- **Constant fix:** the 3/7 letterbox factor is 0x3EDB6DB9 (`0.4285715f`), not
  `0.42857143f` (0x3EDB6DB7). matchcmp masks constant addresses, so four
  MATCH-claimed dialog/letterbox functions (0x006A74B0, 0x006A7540,
  0x006A7EF0, plus 0x006A7AE0) loaded the wrong value; fixed in
  `sw_gui_dialog.cpp` / `sw_gui_dialog_computer.cpp`. The cinematic ctor uses
  -0.4285715f (0xBEDB6DB9). A scratch checker comparing every `__real@` and
  string-literal relocation against the exe found no other mismatches in
  the folder (the 0x006C3460 hits are partial copies of "TEXA"/"ModelA").
- Header changes: `sw_behavior_camera_dialog.h` Vector::MagnitudeSquared
  (inline; needed for the x*x first order); `sw_behavior_camera_combat.h`
  CSWCCreature/appearance (+0x21c), CSWCModuleCamera slot 0x24,
  GetPlayerCharacter returns CSWCCreature*, HideMouse/ShowMouse/GetInputClass,
  CSWGuiManager mouse save/restore, g_bRenderQA (0x008327EC), Magnitude /
  Normalize / Quaternion operator*; `sw_gui_dialog.h` CSWGuiTextParams +0x3c
  `m_szFont[16]` (base font resref chars), CExoSoundSource ctor/SetVolume/
  SetLooping and real size 8, dialog cinematic/computer ctor decls,
  CSWGuiDialog::ResetReplyColor is now a plain member (still exact; needed to
  pass it to AddEvent); AoE header CExoSoundSource::Stop, CSWCVisualEffect::
  SetAnimation; abilities CSWSkill::GetDescriptionText.
- Lessons: (1) x87 sums: MSVC evaluates register-resident terms first; an
  inline `MagnitudeSquared()` member reproduced memory-first `x*x + y*y + z*z`.
  (2) Struct temporaries in one expression get distinct slots
  (`Normalize(v) * f + m_pGob->GetPosition()`). (3) Discarded debug calls
  (`m_pGob->GetPosition()` x3 in Control) were named locals in the original
  (distinct slots). (4) Integer locals for list-box width (`int nWidth = ...;
  CSWGuiExtent e = {0, 0, nWidth, 20};`) fixed a register rotation in the
  cinematic ctor but not in the computer ctor. (5) `CExoString("+") + nBonus`
  (implicit int conversion) pushes by lea and shares the leading int ctor
  across if/else branches. (6) Always check constants: masked `__real@`
  relocations hide wrong float literals.
- Next in-range queue: CheckPosition/DetermineSafePosition 0x006D18E0 (1.7 KB
  EH, walkmesh rays); the NONMATCH ties above; abilities UpdatePortraits
  0x006ABEC0 / character UpdatePortraits 0x006AEE30; upgrade OnSlotSelected
  0x006C6500 / OnControlEntered 0x006C5370; inventory CreateItemEntry
  0x006B44D0 / PopulateItemListBox 0x006B4810; container ctor 0x006B6DC0;
  create-item menu ctor 0x006D0620; missing starts 0x006AE5F0, 0x006BA3F0,
  0x006BCF50, 0x006BEEB0, 0x006C5510; earlier queues (LoadVisualEffect,
  AoE SetPosition / PlaceInstance / LoadEffect, projectile motion set).

## Audit fixes 2026-11-10

- 0x006A9F90 CSWGuiBarkBubble::OnPanelAdded: signed `__alldiv` on the
  snapshot time (`(__int64)` cast).
- l/sw_gui_title_movies.h: removed operator new[]/delete[] declarations.
  MSVC 7.0 calls ??_U/??_V only when they are declared; the original calls
  ??2/??3 (equip/inventory dtors, title movies dtor and PopulateMovies).
- ~CSWGuiInGameCharacter member types from the ctor's vtable stores:
  m_lblLight/m_lblDark CSWGuiBlackenedLabel (0x00753CC0), m_sldAlign
  CSWGuiSliderReadOnly (0x00756060, dtor 0x006AF940, ??_G 0x006AF950),
  m_btn3DChar CSWGuiSpeedButton (0x007533C8). Their destructors are implicit:
  the body inlines them (direct base dtor, no vptr restore -> novtable under
  L_CHARACTER_VIEW_TU), funclets call the folded copies. 0x006AF940 moved to
  sw_gui_slider_readonly_dtor_novtable.cpp (novtable: bare jmp).
- Options family dtors: CSWGuiOptionsCheckbox's dtor is implicit in the
  family-dtor TU (bodies call ~CSWGuiButtonToggle 0x0041CAD0, funclets call
  0x006E00F0).
- g_aEquipSlots is at 0x00756560 with layout {mask, control tag, icon,
  strref, strref}; the old {icon, mask} at 0x00756568 matched only because
  the base shift cancelled the field shift.
- Lesson: body-calls-base-dtor + funclet-calls-thunk = inline derived dtor;
  read member types from the ctor's vtable stores, not from what matches.
