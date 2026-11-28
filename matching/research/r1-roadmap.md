# R1: what stands between the project and one declaration per type

After the merges in `r1-merges.md`, R1 is the only [match] rule that still
blocks most byte matches: about 7,100 byte-identical functions use a type
that is defined differently somewhere (562 types in 5,751 places). This note
records why, measured on the tree, and the order of work it suggests.

## The units are not the original's units

The Mac symbol tables name the object file of most functions. Against the
annotated functions:

- 259 original objects appear; **174 are split across several project
  files**: `aurmodel.o` across 43, `clientexoappinternal.o` and
  `nwchandlemessage.o` across 42, `nwvmachinecommands.o` across 38,
  `nwscreature.o` across 37, `nwccreature.o` across 32, `guiingame.o`
  across 31.
- 110 project files mix functions of several original objects.

Each project file carries its own partial copies of the classes it touches,
which is where most of the 5,751 definitions come from. The split also
changes code generation: MSVC 7.0 at `-O2` inlines a function defined in the
same unit by itself (`-Ob1` loses 218 matches and wins none, so the original
was built with automatic inlining). A function the original defined out of
line in its own object is inlined by its neighbours there and called from
everywhere else. Split the object and the neighbours can no longer see it,
so the reconstructions steered it back:

- `#pragma auto_inline(off)` in 24 units;
- header toggles a unit `#define`s before including the header (`#ifdef
  M_INLINE_TARGETTING_INFO`, `#ifndef B_ITEM_REPOSITORY_OPAQUE_GET_ITEM_AT`),
  which give that unit a different class body (now an R1 finding in
  `rulecheck.py`);
- helper variants that exist only to choose inlining per call site
  (`AddInline`, `AddOutOfLine`, `AllocateOutOfLine`, `SetSizeInline`,
  `SetSizeOutOfLine` in the `CExoArrayList` copies).

Example, fixed: the `CSWMiniHitInfo` and `CSWMGTargettingInfo` constructors
(0x00673A10, 0x00673A30) follow the gun-bank functions and are inlined by the
gun-bank constructors. Defined in `m/sw_mg_gun_bank.cpp`, they need no
toggle: that unit inlines them and `CSWMiniEnemy::LoadGunBank` calls them.

Example, open: `CItemRepository::GetItemAt` (0x005560B0) is inlined by
`CalculateContentsWeight` (`b/sws_item3.cpp`) and `SaveContainerItems`
(`b/sws_item2.cpp`) and called from `sws_message_debug_creature.cpp`. The
original `nwsitem.o` (0x00553020-0x00560970) is seven project files.

## Header toggles

22 types were toggled this way; 18 remain.

| toggle | state |
| --- | --- |
| `G_SAFE_POINTER_ASSIGN_FORCEINLINE`, `H_ADD_OUT_OF_LINE`, `L_GUI_TEXT_PARAMS_EXACT` | removed: no unit needed them |
| `B_EXO_ARRAY_LIST_OPAQUE_ADD` | removed: the inline `Add` is closer (65 -> 35 differing instructions) |
| `M_INLINE_MINI_HIT_INFO`, `M_INLINE_TARGETTING_INFO` | removed: the constructors live in their own unit |
| `L_GUI_BUTTON_INLINE_CTOR`, `M_GUI_BUTTON_INLINE_CTOR`, `M_INLINE_RES_HELPER_CTOR` | removed: the inline body suits every unit |
| `B_ITEM_REPOSITORY_OPAQUE_GET_ITEM_AT`, `B_SWS_ITEM_OPAQUE_PROPERTY_GETTERS` | open: needs `nwsitem.o` joined |
| `L_OPTIONS_CHECKBOX_INLINE_CTOR`, `L_OPTIONS_FAMILY_NOVTABLE`, `L_RADIO_BUTTON_INLINE`, `L_SLIDER_READONLY_DTOR_TU` | open: the options and save/load panels' constructors and destructors |
| `L_GUI_CONTROL_DRAW_SLOT`, `L_GUI_LABEL_SET_STYLE_SLOT` | open: a virtual slot present only in some units (needs the real `CSWGuiControl` vtable) |
| `I_CRESREF_DECLARED_COPY`, `L_RESREF_IMPLICIT_COPY` | open: `CResRef`'s copy constructor (`r1-merges.md`) |
| `G_AUR_FONT_INFO_INLINE_CTOR`, `G_EXO_LIST_SIZED_DEFAULT`, `G_EXO_LIST_CONTAINS` | open: `CAurTextureBasic`'s unit and the `CExoArrayList` copies |
| `B_COMBAT_INFO_NODE_INLINE`, `SWS_COMBAT_ROUND_OUTLINE_GETATTACK`, `M_OUTLINE_MINI_STATE`, `B_PLAYER_CLIENT_ENTRY_FIELDS`, `B_OUT_OF_LINE_FACTION_MANAGER` | open |

`mtools/trytoggle.py MACRO defined|undefined` resolves a toggle one way for
every unit that can see the header and reports what changes; a toggle whose
both resolutions lose matches is a unit-boundary case.

## Order of work

1. **Join split objects**, one original object per unit, functions in
   address order (R15). A joined unit first needs its files' class copies
   reconciled, which is the R1 work done locally; the shared headers then
   take one copy per type.
2. **Templates.** `CExoArrayList` has 220 copies. Most share `element`,
   `num`, `array_size`, but `Add` grows to 16 in some and to 8 in others,
   which one template cannot do. Either the renderer (Aurora) and the game
   had separate array classes, or some copies are wrong. The inlining
   variants go once the units are joined. `CExoLinkedList` and
   `SafePointer` follow.
   `Quaternion` (90 copies) has the same problem in small: 36 copies have no
   constructor, 13 an empty one, and 37 an identity constructor that stores
   its members in one of three orders (z, y, x, w; x, y, z, w; w, x, y, z),
   each tuned to its units. Merging the fullest copy compiles everywhere but
   loses 88 matches (`CResGFF::ReadFieldQuaternion` among them), so
   which units used which class (or whether there were two) needs evidence.
3. **The GUI hierarchy** (`CSWGuiControl`, `CSWGuiLabel`, `CSWGuiButton`,
   `CSWGuiListBox`, `CSWGuiPanel`, `CSWGuiManager`): 20 to 40 copies each,
   including views that reach members through padding
   (`*(CSWGuiBorder*)pad_0x5c`) and vtable views with placeholder slots
   (`v00`, `v04`).
4. **The large game classes**: `CSWCCreature` (89 copies), `CGuiInGame`
   (76), `CClientExoApp` (parked: `wip_CClientExoApp.patch`), `CSWSCreature`,
   `CServerExoApp`.

`mtools/automerge.py` (driven by `drive.py`) merges the types whose fullest
copy covers the others; the ones it skips are listed with the reason in the
driver log, and most wait on a type in the list above.
