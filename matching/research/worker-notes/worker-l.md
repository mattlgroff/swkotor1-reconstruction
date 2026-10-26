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
