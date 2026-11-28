# Merging the per-bucket type copies (R1)

Each address bucket was given private copies of the classes its functions
use. The copies disagree, so every function that sees one is
`MATCH-PENDING` under R1 (CODE_RULES.md). This log records each merge into
one shared declaration: the evidence behind every choice, what the byte gate
showed, and what is still open.

## Method

1. Inventory the copies: layouts, keywords, and every method each declares
   (`rulecheck.py --list R1` ranks the types by number of definitions).
2. Write the canonical declaration in `include/shared/`, from evidence in
   this order: the Mac symbol tables (`mac_names.csv`: original names,
   parameter types, which functions are out of line), out-of-line copies in
   the executable, then what most matched units already use.
3. `tools/mergetype.py TYPE shared/header.h` replaces every copy with the
   include, and `--compile` lists the units that no longer build.
4. Run the full byte gate and compare every function with the report before
   the merge. A choice that loses matches across many units is the wrong
   choice; a few units that lose a match show what their source really
   looked like.
5. A merge lands only with no lost match. Attempts that get worse are listed
   below for review.

## Vector (`shared/vector.h`, `shared/vector_ops.h`)

244 copies in 98 distinct forms (138 `class`, 106 `struct`). The layout,
`float x, y, z`, agreed everywhere; the methods didn't.

| question | decision | evidence |
| --- | --- | --- |
| `class` or `struct` | `class` | no RTTI for a non-polymorphic type; the math unit and most copies say `class`, and the Aurora engine headers do too |
| default constructor | zeroes `x`, `y`, `z` | the executable keeps an out-of-line copy that zeroes (0x00409E70, used by the vector constructor iterator). Gate: the empty constructor lost 132 matches; the zeroing one lost 16, all fixed below, and turned 4 NONMATCH functions into matches |
| compound assignment, `Normalize`, `IsZero` | out-of-line | `Vector::operator+=`, `-=`, `*=`, `/=` and `Normalize` are functions of `math3d.o` in the Mac tables; `IsZero` is matched at 0x0046A620 |
| arithmetic operators | free inline functions in `vector_ops.h` | the Mac tables name `operator/(Vector const&, float)` as a free function, and the executable keeps out-of-line copies of several free operators (0x00423B90-0x00423BF0, 0x00437A60, 0x00437A90). Member forms in 45 copies became the free forms |
| `operator*(Vector, Vector)` | the dot product, returning `float` | three independent units defined it so. The function at 0x004AB1E0 that `h/renderpaths.h` called `Vector operator*` is `cmul` in the Mac tables, so it is now `cmul` |
| copy assignment | implicit | a user-written `operator=` lost 429 matches |
| `operator const float*()` | kept | `DrawTriangles` passes a temporary `Vector` straight to `glNormal3fv` |

Source changes the gate called for, all byte-checked:

- **Zeroing that the old empty constructor needed by hand** is gone:
  `CSWGuiPanel`, `CSWGuiInGameAreaTransition` and `CScriptLocation`
  constructors no longer zero their `Vector` members in the body.
- **Locals that are not default-constructed in the original**: in
  `CSWCCreature::PartyMenuUseItem` the local moves into the branch that
  uses it; in `CSWSArea::CountVisibleToPlayers` the search loop sits inside
  the `if` that finds the first object, with the eye position declared there.
- **Operand order**: four additions read in the order the x87 code shows
  (`GobOutside`, `SceneAddPartToRooms`, `DynamicPartCalculateMaterial`,
  `CSWCollisionMesh::TransformToWorld`).
- **Open-coded helpers restored**: `vDelta = vLightPos - vPos` in
  `LightManager::GetNearestLights`; `VectorEqual(a, b)` for the one-copy
  `IsEqual` member; the dot operator for the one-copy `Dot` member.
- **`gl_debug_draw.cpp`**: its copy had a user-written `operator=`. The two
  functions that relied on it copy member by member, and
  `*(Vector*)&m[12] = *pCenter` became three element stores.
- **Out-of-line copies of the inline constructors** are annotated with
  `SYMBOL:` where the compiler emits them (`d/app_misc.cpp` for 0x00409E70,
  `d/movie_player.cpp` for 0x00404AE0); `d/vector.cpp` is gone.
- **Aggregate initialisers** `Vector v = {0, 0, 0}` became `Vector v(0.0f,
  0.0f, 0.0f)`; `SYMBOL:` annotations that spelled `struct Vector`
  (`UVector@@`) now spell `class Vector`.

Result: 8,474 / 9,538 byte-identical (was 8,470): no match lost, four new.
Matched under the rules: 579 (was 533).

### NONMATCH attempts that got worse

Still to review. Most mix several types, and the zeroing constructor or the
free operators changed their register allocation.

| function | address | diff insns before → after |
| --- | --- | --- |
| `CollapsePartTree` | 0x00453F10 | 33 → 40 |
| `HitPartOnBSPNode` | 0x0046B4C0 | 24 → 27 |
| `AddObjectToRooms` | 0x0046CE70 | 6 → 11 |
| `LightNodeTile` | 0x0046DB60 | 53 → 59 |
| `Gob::PartProject` | 0x00474710 | 65 → 87 |
| `RenderLightMappedBumpSpecular` | 0x00475450 | 192 → 193 |
| `NewController::GetVectorValue` | 0x00482D20 | 101 → 108 |
| `MdlNodeAnimMesh::InternalControl` | 0x00485660 | 64 → 137 |
| `LightManager::GetNearestLights` | 0x004A3770 | 5 → 10 |
| `CSWSModule::SetIntraAreaGoal` | 0x004C3EA0 | 20 → 30 |
| `CSWSCreature::DesiredAttackRange` | 0x004F1310 | 23 → 32 |
| `CSWSMessage::TestPartyUpdateDifferences` | 0x0056E850 | 6 → 9 |
| `CSWRoomSurfaceMesh::TransformAABBNode` | 0x00580AF0 | 4 → 10 |
| `CSWSPlaceable::GetNearestActionPoint` | 0x00584C20 | 50 → 66 |
| `CClientExoAppInternal::UpdateSoundEngine` | 0x005F5370 | 49 → 61 |
| `CSWCCreature::SetDesiredOrientationForLock` | 0x0060BA10 | 10 → 13 |
| `CSWCCreature::GetMenuPoint` | 0x0060FF30 | 12 → 16 |
| `CSWBehaviorCameraCombat::CalculateDesiredLocation` | 0x006D14E0 | 9 → 11 |

### Still open for Vector

- Duplicate free inline operators remain in about 40 headers and units
  (`inline Vector operator-(const Vector&, const Vector&)` and friends). They
  are R16 work, not R1: fold them into `vector_ops.h` unit by unit.

## CExoString (`shared/exo_string.h`)

216 copies; the layout (`char* m_sString; unsigned long m_nBufferLength`)
agreed everywhere but in member names, and the Mac tables name all 36
methods, every one an out-of-line function that `c/exo_string_r8.cpp`
matches. The canonical declares them all, in address order.

The byte gate lost two functions, both for the same reason: they assign a
`char*` to a `CExoString` and the original builds a temporary `CExoString`
for it, destroyed at once, although 275 matched call sites elsewhere use
`operator=(const char*)` directly. Their old copies lacked that overload, so
the conversion happened implicitly. Neither `CExoString(x)` nor a cast
reproduces the code; a named temporary in its own block does, and each site
carries a `// NOTE:` saying so (`CSWSEncounter::ReadEncounterScriptsFromGff`,
`CSWSPlaceable::LoadBodyBag`). One copy's `IsNotEmpty(0)` was
`operator[](0)` under another name.

Result: no match lost.

## CAppManager and CSWReentrantServerStats (`shared/app_manager.h`, `shared/reentrant_server_stats.h`)

213 copies of `CAppManager`. The layout and the seven out-of-line methods
follow `d/app_misc.cpp`, the unit that defines them; the Mac tables name the
same seven. Copies used other names for the same slots (`m_pServer`,
`m_pClient`, `m_p0`, `GetServer()`), which `layout.py` and `fixmembers.py`
mapped by offset. Two copies were layout views: `CAppManagerMenu`, a subclass
that redeclared `m_pServerExoApp` "at 0x08" and was reached through a cast,
and `dialog.cpp`'s `CServerMessage`, which was really `CClientExoApp` (the
Mac tables name `CClientExoApp::ServerToClientObjectId`).

The record at `+0x14` had six names: `CSWReentrantServerStats` (the Mac
tables name its destructor), `CAppPad`, `CAppManager14`, `CSaveLoadStatus`
and `CUnknown004b13e0`. They agree at every offset and are now one class of
0x3C bytes. The save-game code reaches it through an inline accessor, which
`CAppManager` now declares (`GetReentrantServerStats`, a descriptive name);
without it `CServerExoAppInternal::SaveGame` allocates registers differently.

Result: no match lost.

## CSWVirtualMachineCommands and CVirtualMachineCmdImplementer (`shared/sw_virtual_machine_commands.h`)

Four and three copies. Both classes follow `b/vm_commands.h`: the base's
vtable (0x007450AC) has `_purecall` in every slot but the destructor, so the
base declares its slots pure and its destructor inline; the Mac tables name
that destructor's out-of-line copy (0x004B1330), which the compiler emits
with the vtables in the unit that constructs the commands object, where its
annotation now is. The minigame unit's 43 `ExecuteCommandSWMG_*`
declarations joined the class, and its wrapper struct around the command
table became the table itself (`m_pCommands[i]`).

Result: no match lost.

## CResRef: merged (`shared/res_ref.h`)

163 copies. The layout (`char m_resRef[16]`) and the methods follow the Mac
debug map, all out of line, and there is no user-written copy constructor:
the map lists none, and a CResRef copies as its 16 bytes.

The earlier attempt found two copy behaviours: getters copy a member inline
as a trivial copy does, while the 2DA loaders build the argument in place,
which MSVC 7.0 does for an explicit temporary only when the copy
constructor is non-trivial. The second behaviour came from the source, not
the class: the loaders called `new C2DA(CResRef("GENDER"), 0)`, where the
original passes the string and lets the conversion build the by-value
parameter in place (`new C2DA("GENDER", 0)`). With that, `f/two_dim_arrays.cpp`
matches 77/77 with the trivially copyable class (4/77 with an inline
converting constructor instead). The same holds for the CAurora* texture and
model constructors and `ReplaceTexture(CResRef, float)`; each takes its
CResRef by value and has no overload that a string could select instead.

Other sites the merge corrected:

- 0x00406DA0 is `CResRef(unsigned char*)`, as the map names it; it had been
  annotated as the copy constructor (same bytes). `CSWMessage::ReadCResRef`
  constructs its result from the byte buffer instead of casting the buffer
  to a CResRef.
- `CSWCPlayer::SetPortraitId` calls `SetPortrait(sResRef.CStr())`, the
  by-value setter just above it, and `CSWSPlaceable::LoadBodyBag` assigns its
  two empty resrefs through by-value setters: the original assigns from the
  argument (`lea` of it, not the constructor's result), as an expanded
  setter does.
- `res_ref.cpp` and `res_gff.cpp` defined their own inline copy constructor;
  both are gone.
- `c/client_exo_app_internal_dtor_s8.cpp`'s CClientPlayer has a 16-byte
  member at +0x14 that StopServices destroys through the shared empty
  function 0x004015A0, which a CResRef never needs; it is a placeholder type
  until the element type is known.

Byte gate: 8474/9538, no match lost.

## Next

`rulecheck.py --list R1` ranks what is left. `CExoString` (216 copies) has
one layout apart from member-name variants and the Mac tables name its
methods; check its copy semantics first, as CResRef showed they decide the
merge.
