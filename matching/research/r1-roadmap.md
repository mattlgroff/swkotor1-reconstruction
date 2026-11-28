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
| `I_CRESREF_DECLARED_COPY`, `L_RESREF_IMPLICIT_COPY` | removed: `CResRef` is merged, with no copy constructor |
| `B_ITEM_REPOSITORY_OPAQUE_GET_ITEM_AT`, `B_SWS_ITEM_OPAQUE_PROPERTY_GETTERS` | open: needs `nwsitem.o` joined |
| `L_OPTIONS_CHECKBOX_INLINE_CTOR`, `L_OPTIONS_FAMILY_NOVTABLE`, `L_RADIO_BUTTON_INLINE`, `L_SLIDER_READONLY_DTOR_TU` | open: the options and save/load panels' constructors and destructors |
| `L_GUI_CONTROL_DRAW_SLOT`, `L_GUI_LABEL_SET_STYLE_SLOT` | open: a virtual slot present only in some units (needs the real `CSWGuiControl` vtable) |
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
   which one template cannot do. The original's out-of-line instances
   (`mtools/growth.py` reads them) split by object:

   | grows to | objects |
   | --- | --- |
   | 8 | `aurscene.o`, `aurmodelparse.o`, `aurtexman.o`, `bsp.o`, `swroomsurfacemesh.o`, and two with no object recorded (0x004A77C0, 0x004AD5E0) |
   | 16 | `clientexoapp.o`, `swguicontrol.o`, `nwsmodule.o`, `nwcjournal.o`, `nwscombatround.o`, `nwscreature.o`, `nwsarea.o`, `nwssendplayermessage.o` |

   So the renderer and the game were built with different versions of the
   template, under the same name (the linker keeps one body per mangled
   name, so a type the two sides share gets either). Two instances break
   the pattern or mark an object boundary: `CExoArrayList<Quaternion>::Add`
   (0x00438AF0) and `CExoArrayList<MdlNodeTriMesh*>::Add` (0x00438B40) grow
   to 16 inside `aurlightmanager.o`'s range, next to `UpdateAnimFootprint`
   (0x00437DB0), which has no object recorded. R1 now counts per library
   (CODE_RULES.md): `matching/libraries.csv` marks the renderer's units and
   the build defines `AURORA_LIBRARY` for them.

   The copies show more than the growth constant. Per library, the
   destructor is `if (element) delete[] element;` in the renderer (seen by
   116 renderer units and 2 game units) and `delete[] element; element = 0;`
   in the game; `SetSize(0)` clears `array_size` outside the null test in
   the renderer and inside it in the game. The game's constructor
   (`CExoArrayList<CExoString>::CExoArrayList(int)`, 0x004058F0) calls
   `Allocate`. Every original `Add` instance calls `Allocate` out of line.

   A merged header built on that (branch `al-merge-wip`) compiles in every
   unit but does not hold the byte gate, because whether a caller expands a
   member differs by unit:

   | game members | lost | won |
   | --- | --- | --- |
   | defined in the class | 88 | 17 |
   | defined after the class, not `inline` | 80 | 16 |
   | declared only, bodies in a header for the 48 units that emit instances | 242 | 6 |

   `CSWGuiManager::PushModalPanel` (0x0040BD90) calls the pointer `Add` at
   0x00671C00, which is in `swminigame.o`, so `swguimanager.o` neither
   expanded it nor emitted its own; other game units do expand `Add`. The
   old copies reproduced this with a declarations-only view in some units
   and inline bodies in others. `#pragma auto_inline(off)` cannot stand in
   for that: MSVC compiles template members where they are instantiated, at
   the end of the unit, so the pragma there takes every member out of line,
   `operator[]` included (`gui_manager.cpp` went from 38 matches to 34).
   Which members a unit expands is a property of the original object, so
   the template waits on joining the split units (step 1). (The game was
   built with automatic inlining too: compiling the game's units at `-Ob1`
   loses 152 matches and gains none.) `CExoLinkedList`
   and `SafePointer` follow.
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
   `CServerExoApp`. They are where R1 blocks most: counting the byte-identical
   functions that a single type blocks on its own, `CClientExoApp` blocks 194,
   `CSWSCreature` 143, `CServerExoApp` 95, `CSWGuiControl` 93, `CGuiInGame`
   85. None merges automatically, because the copies disagree on the class
   hierarchy, not only on members:

   | type | copies | bases the copies give |
   | --- | --- | --- |
   | `CGameObject` | 107 | none; 41 copies are 12 bytes (vtable, `m_idSelf` +0x04, `m_nObjectType` +0x08), six are views up to 244 bytes that carry `CSWSObject`'s members (`m_oidArea` +0x8C, `m_vPosition` +0x90) |
   | `CSWSObject` | 65 | `CGameObject` (40), none (25) |
   | `CSWSCreature` | 103 | none (72), `CSWSObject` (21), `CGameObject` (10) |
   | `CSWCCreature` | 93 | none (58), `CSWCObject` (32), `CGameObject` (2), `CClientObject` (1) |
   | `CSWSArea` | 53 | five different lists, some with several bases (`CResHelperArea`, `CSWSAreaGameObject`, `CSWArea`, `CGameObject`) |

   So each family needs its real bases settled from evidence (vtables,
   constructors, the Mac tables) and merged from the root down:
   `CGameObject`, then `CSWSObject` and `CSWCObject`, then their subclasses.

   `CGameObject` is merged (`shared/game_object.h`). `CSWSObject` is next and
   waits on three by-value member types: `CExoLocString` (+0x0C), `CResRef`
   (+0x20) and `CExoArrayList` (several lists). Its vtable is settled from
   the original, not by vote: the constructor (0x004CFCB0) stores 0x00745F18,
   and `tools/vtread.py 0x745F18 56` names every slot from the Mac map,
   `symbols.csv` and the annotations. The 25 flattened copies (no base list)
   rebase onto `CGameObject` cleanly; the copies named slots differently
   where the vtable shows one function:

   | slot | function | names the copies use |
   | --- | --- | --- |
   | +0x70, +0x78 | `_purecall` | `AIUpdate`, `EventHandler`: pure virtual |
   | +0x7C | `CSWSObject::SetAnimation(int)` (Mac) | `SetAnimation` 9, `SendFeedbackMessage`, `SetDialogDelay7C` (the effect handlers pass animation numbers such as 0x27A1) |
   | +0x80 | `CSWSObject::GetDialogResref()` (Mac) | `GetConversation`, `GetTemplateResRef` |
   | +0x94 | `CSWSObject::GetDead` 0x004CB810 | `GetIsInConversation` 3, `GetObjectState94`, `GetIsDead` |
   | +0x98 | 0x004D01A0; `CSWSCreature::GetMaxHitPoints(int)` (Mac) overrides it | `GetGender(int)` (the definition in `a/sws_object.cpp`), `GetCreatureSize` |
   | +0xA0 | `CSWSObject::DamageHitPoints` 0x004CCF80 | declared non-virtual by the copies |
   | +0xB4 | `CSWSObject::GetDamageImmunityByFlags` 0x004CAF70 | `GetDamageImmunity`, `GetTotalDamageImmunity` |
   | +0xC8 | base 0x004D01C0 returns `m_cTemplateResRef`; `CSWSCreature` 0x004F8370 returns `m_pStats->m_cPortrait` | `GetTemplateResRef`, `GetPortrait` |
   | +0xD0 | 0x004D0210 returns `m_n30` | `Get30`, `GetPortraitId` |
   | +0xDC | `CSWSCreature::UpdateAttributesOnEffect(CGameEffect*)` (Mac) | `OnEffectListChanged` |

   `CSWSCreature`'s vtable is 0x007470C8 (constructor 0x004F7A10),
   `CSWSDoor`'s 0x007498B0, `CSWSPlaceable`'s 0x007494D0, `CSWSItem`'s
   0x00748DA0.
   The flattened copies keep their layout when the intermediate class is put
   back (`class CSWSCreature : public CSWSObject` with `CSWSObject`'s members
   moved there), so the bytes and the mangled names stay the same.

Smaller cases that need evidence rather than a script:

- `ScenePart`: `h/scene.h` names vtable slots +0x34 and +0x54 `GetLight` and
  `GetEmitter`; `h/sceneaddpart.cpp` names them `AsDynamic` and `AsEmitter`,
  next to `AsGob`, `AsStatic`, `AsLight` and `AsTileSlot`. Aurora's other
  downcasts are `As<Type>` (`MdlNode::AsMdlNodeLight`, `MaxTree::AsModel`),
  but `room.cpp` and `scene.cpp` use the +0x34 result as a light.
- `GobRef`: its inline bodies reach `Gob::m_lWatchers`, so it waits on
  `Gob`; one copy declares `operator=(CAurObject*)` at the address others
  declare `operator=(Gob*)` (0x00447FA0).
- `TriMeshPart::GetTriMesh` returns `MdlNode*` in one copy and
  `MdlNodeTriMesh*` in another; `VertexPC` is `Vector v` in one copy and
  `x`, `y`, `z` in another; `VertexPrimitive` has pure virtuals in its
  fullest copy that `part_material.cpp`'s subclasses do not override.
- 38 types whose copies lay the same bytes out differently: a `Vector`
  against three floats (`CSWSModuleEntryInfo`), an explicit `m_pVtbl`
  against a real vtable (`Particle`, `TilePart`), a byte array against named
  fields (`CCreatureAppearance`), a struct against the fields it holds
  (`m_cHit` in `CSWMiniGameObject`).

`mtools/automerge_vt.py` adds a slot-by-slot vtable merge: copies that are
vtable views (placeholder slots `V00`, `V04` and the few slots their unit
calls) give the shared class those slots' names, and two names for one slot
stop the merge.

`mtools/automerge.py` (driven by `drive.py`) merges the types whose fullest
copy covers the others; the ones it skips are listed with the reason in the
driver log, and most wait on a type in the list above.
