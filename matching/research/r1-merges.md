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

## CResRef: not merged yet (two copy behaviours)

163 copies; the layout (`char m_resRef[16]`) and the method list agree with
the Mac tables, and every method except the copy constructor is an
out-of-line function. The merge was built and gated, then reverted, because
the bytes show two incompatible copy behaviours:

| canonical copy constructor | lost matches | what breaks |
| --- | --- | --- |
| declared, out of line | 37 | getters that return a member by value copy it inline in the original |
| none (implicit, trivial) | 106 | the 2DA loaders: `new C2DA(CResRef("GENDER"), 0)` builds the argument in place, which MSVC 7.0 does only for a class with a non-trivial copy constructor; `res_ref.cpp`'s own by-value returns too |
| user-declared, inline (`memcpy`) | 24 | getters and senders (`GetResRef44`, `GetPortrait`, `GetFilePlaying`, `GetItemDropSound`, ...): the inline copy computes the source address in a different order from the original's trivial copy. No inline body tried (`memcpy` of the array or of `this`, a struct copy, a word loop) reproduces it |
| implicit copy plus `~CResRef() {}` | 163 | the destructor adds unwind states everywhere |

So the units that copy a `CResRef` see a trivially copyable class in some
places and a non-trivial one in others. Either the original had two
declarations (one a plain 16-byte struct used by the getters' classes), or
these getters have a source shape not yet found. Resolving it needs a look
at the getters' callers and member types, not another copy-constructor body.

One part stands on its own and is applied: 0x00406170 is
`operator=(unsigned char*)` in the Mac tables, not
`operator=(const char (&)[16])`. With the real signature the KEY, ERF and TLK
record fields are `unsigned char[16]`, and the three member-pointer casts
that selected the array overload are gone (`d/exo_key_table.cpp`,
`d/tlk_table.cpp`, `d/res_ref_copy.cpp`).

Two more findings for the eventual merge:

- 0x00406DA0 is `CResRef::CResRef(unsigned char*)` in the Mac tables; this
  repository annotates it as the copy constructor (same bytes).
- Several sites assign a `char*` where the original builds a temporary
  `CResRef` first (`CSWCPlayer::SetPortraitId`, `CSWSPlaceable::LoadBodyBag`
  and others: the original frame is 16 bytes larger).

## Next

`rulecheck.py --list R1` ranks what is left. `CExoString` (216 copies) has
one layout apart from member-name variants and the Mac tables name its
methods; check its copy semantics first, as CResRef showed they decide the
merge.
