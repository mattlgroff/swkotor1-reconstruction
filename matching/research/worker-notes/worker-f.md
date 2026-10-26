# Worker f notes (bucket 0x00580000-0x005CFFFF)

Older history for this bucket (rounds r1-r11, written in the Kotor RE
checkout) is in `docs/batches/worker-f-notes.md` there.

## 2026-10-26 round s1: CSWRoomSurfaceMesh / CSWCollisionMesh (walkmesh)

Folder `matching/src/f`: 604/651 -> 637/688 (kotor-match -q -j 2).

New files:

- `include/f/room_surface_mesh.h`: shared layout for CSWCollisionMesh
  (vtable slots 0-10), CSWRoomSurfaceMesh (+0x88 adjacency, +0x8c perimeter
  edge list, +0x9c room-end list, +0xac AABB list, each with an external flag;
  +0xbc crossing points, +0xc8 crossing data, +0xd4 AABB root, +0xd8..0xe4
  LineOfSight / Walk / WalkCheck / all surface masks), the 0x88-byte BWM header
  (CSWWalkMeshHeader), CSWCollisionResult, CResBWM / CResMDL.
- `src/f/room_surface_mesh.cpp` (26 annotated, 21 exact).
- `src/f/collision_mesh.cpp` (10 annotated, 10 exact).
- `src/f/collision_result.cpp` (out-of-line CSWCollisionResult ctor, exact).

Exact (32 new): 0x00580340 performance-counter timer, 0x00580720 AABB node
ctor, 0x005807C0 room LoadMeshBinary (hidden), 0x005808E0 BWM header ctor,
0x00580620 GetAdjacentFaces, 0x00580BE0 GetPerimeterVertex, 0x00580C60
GetEdgeVertices, 0x00580D50 WeldEdgeVertices, 0x00581320 CSWCollisionResult
ctor, 0x00581370 room SaveBinaryMesh (hidden), 0x00581610 CheckAABBAll,
0x00581720 FindTriangle, 0x00581930 / 0x00581A10 GetSurfaceMaterial(+
WalkCheckOnly), 0x00581B30 ComputeHeightAABB, 0x00581D60
~CExoArrayList<AABBNode>, 0x00581FE0 ~CSWRoomSurfaceMesh, 0x00582180 GetResRef
and 0x005821A0-0x005821D0 mask getters (hidden, inside the dtor's range);
CSWCollisionMesh 0x00596670 LoadMesh, 0x00596890 base LoadMeshText (hidden),
0x005968A0 GetLine, 0x00596AA0 GetWorldPoint, 0x00596C10 ComputeNormals,
0x00596D10 GetMeshBoundingBox, 0x00596E40 GetMeshOutline, 0x00597090
GetVertex(int, float*, float*, float*), 0x00599ED0 TransformToWorld,
0x0059A040 SaveBinaryMesh.
Promoted: 0x005806C0 GetTriangleIndices (surfmesh.cpp, was NONMATCH 4).

NONMATCH kept: 0x00580AF0 TransformAABBNode (4: `[edi+edx]` vs `[edx+edi]`
base/index order on the second node access), 0x00580E90 SetAdjacency (3: the
spilled vDiff2 components sit above the named Vectors instead of below them),
0x00582AE0 ctor (41: the original's `m_lAABB = <empty list>` temporary leaves
no destructor code and stores a constant 0 flag; the per-row
GetINTEntry calls load the 2DA pointer into eax before the pushes),
0x00580920 CheckAABBNode (51: merged `return 0` block placement and
register roles), 0x00581D90 AddCrossing (64: unrolled search loop; register
allocation and FP term order).

Lessons:

- `p[nFace * 3 + k]` written with an explicit `int i = nFace * 3;` and
  `((int *)p)[i + k]` gives the original `lea eax,[eax+eax*2]` +
  `[base+eax*4+k]`; struct indexing (`p[n].a[k]`) gives `shl` + `[esi+edx]`.
  Fixed GetAdjacentFaces, GetTriangleIndices and halved CheckAABBNode.
- Vector `operator+` member: MSVC swaps commutative fadd operands, so write
  `m_vOffset + v` to get `fld v; fadd offset` (TransformToWorld,
  GetWorldPoint `m_vPosition + q * (v + m_vOffset)`). The rule is not uniform
  (TransformAABBNode needs the two different spellings for max and min).
- A local that is only assigned from a by-value return: `Vector v;` (zeroing
  default ctor) followed by `v = F();` inside the loop gives the original
  temp-then-copy; the dead zero stores are removed (GetMeshBoundingBox).
- Several identical getters folded by the linker must be declared as distinct
  methods (CResBWM::GetBWMSize vs CResMDL::GetMDLSize), otherwise cl tail-merges
  the two LoadMeshText call sites (LoadMesh).
- Two temporaries passed by value (`CheckAABBAll(Vector(...), Vector(...))`)
  are built right-to-left; named `vStart` / `vEnd` locals give the original
  slot order.
- Loop-invariant expressions written inside the loop body are hoisted into
  both the unrolled and the remainder preheaders, like the original; the
  search loop only unrolls with the bound in a local (`int nCount = num`).
- An inline in-class CExoArrayList::Insert / Add / Allocate is inlined for
  `int` but stays a call for `Vector` (0x00581C30), as in the original.
- The plane-distance dot product slot order changed with operand order
  (`vNormal.x * v0.x + ...`) even though the emitted FP order did not.
- Commits from a worktree need `git -c user.name=... -c user.email=...`
  (no global identity on this Mac).

Next in range (walkmesh family): 0x005803C0 hidden adjacency builder
(~0x220 bytes, 7-int edge records, heavily strength-reduced), 0x005821E0
border-loop builder (1104), 0x00582630 IsPointInPolygon (1200), 0x00583E30
ComputeClippedLineSegment (1008), 0x00584220 ClippedLineSegmentWalkable
(1616), 0x00582D70 room LoadMeshText (4288), 0x005972B0 NoNonWalkPolys
(11296); retries of the five NONMATCHes above. Other queue from r11 still
applies (dialog, combat, store, faction 0x005BFCC0).

## 2026-10-26 round s3: CSWSEncounter (load / spawn), combat reactions, stats

Folder `matching/src/f`: 643/698 -> 660/729 (kotor-match -q -j 2).
31 new annotations: 17 exact, 14 NONMATCH. CEncounterListEntry::operator=
(0x00590250, already exact) moved from encounter.cpp into encounter_load.cpp.

New file `src/f/encounter_load.cpp` (own CSWSEncounter layout: activate list
+0x280..0x28c, geometry +0x290..0x2ac, creature list +0x2b0/+0x2b4, spawn
points +0x2b8/+0x2bc, pool +0x2c0..0x2e4, scripts +0x2e8, fractional CR list
+0x310, player-only +0x318). Exact (14): 0x00590090 InActivateArea,
0x00590410 LoadEncounterSpawnPoints, 0x00590580 LoadEncounterGeometry,
0x00590820 ReadEncounterScriptsFromGff, 0x00590B10 PadOutCreatureCount,
0x005919E0 CalculatePointsFromCR, 0x00592280 AddCreaturesToSpawnList,
0x005931D0 CalculateSpawnPool, 0x00593270 LoadFractionalCRData, 0x005935D0
BuildSpawnList, 0x00593830 LoadEncounter, 0x00593A90 LoadFromTemplate,
0x00593C70 CSWSEncounter ctor, 0x00594220 EventHandler (function start
missing from functions.csv: it follows AIUpdate's padding; annotated anyway).
Several names are descriptive (no debug-map names): CalculatePointsFromCR,
CalculateSpawnPool, TallyEnemyRadiusPoints, PadOutCreatureCount,
BuildInitialSpawnList, BuildSpawnList, AddCreaturesToSpawnList, InActivateArea.
encounter.cpp's callee names were aligned (UpdateActivateAreaList,
CalculateSpawnPool, BuildSpawnList, m_fNextLevelPoints).

Other exact: 0x005B6DB0 ResolveRangedHit, 0x005B6E90 ResolveRangedMiss
(creature_misc.cpp, which now derives CSWSCreature from a CGameObject /
CSWSObject with the vtable), 0x005A5DF0 GetTalentAllowed (stats_talent.cpp).

NONMATCH kept: 0x00590DA0 UpdateActivateAreaList (26) and 0x00591AF0
TallyEnemyRadiusPoints (26): the area-object loop keeps the index in a
register across the back edge and aligns the loop top; the original reloads
the address-taken index at the top (for / while / do / goto / unsigned forms
all tried). 0x00593FB0 AIUpdate (10, 4 real register picks), 0x00590F80
BuildInitialSpawnList (14: i / fPoints slots swapped), 0x005A0BA0 / 0x005A0C80
/ 0x005A0D10 / 0x005A0DD0 dialog hide-items / head-tracking helpers
(dialog_equip.cpp, 10-30: this/object register roles, GetNext arg order),
0x005B6C40 ResolveRangedAnimations (22), 0x005B7470 ResolveMeleeAnimations
(9: known-zero eax reused for a push 0), 0x005BBA80 ResolveAttack (23),
0x005A7A20 / 0x005A7DB0 melee / ranged damage bonus (31 / 8), 0x005A5EE0
GetSpellGainWithBonus (7).

Lessons:

- The original build is /O2, i.e. /Ob2: any function defined EARLIER in the
  same .cpp may be auto-inlined, per call site. Put callees in the same file
  before the caller to get the original inlining: CEncounterListEntry::
  operator= inlined into BuildInitialSpawnList (206 -> 14 diffs), the
  SetActive/SpawnIfAppropriate body in the encounter ctor, GetIsRangeState0/2
  in ResolveRangedHit (but called in ResolveMeleeAnimations), IsDebilitated,
  GetClass/GetClassLevel in stats, GetWeaponSpecialization in the damage
  bonus pair. Same-TU definitions can still stay calls (GetIsSpecialAppearance,
  GetIsRangeState1/2 in melee) - the heuristic decides.
- `CResStruct cElement;` (4 bytes) declared at function top, not in the loop,
  gives the original frame (LoadEncounterSpawnPoints / Geometry).
- A down-counting loop (`dec; jne`) over a copy: `int nSpawn = nCount;
  for (i = 0; i < nSpawn; i++)` when nCount's address escaped (BuildSpawnList).
- `CResRef(m_sScript)` default argument: pass the CExoString and let the
  implicit conversion build the temporary (re-addressed with lea), as the
  2DA notes said for CExoString temporaries.
- `#include <math.h>` for pow / log intrinsics (_CIpow, fyl2x); a plain
  `double pow(double, double);` C++ declaration is a normal call.
- Rotated `while (cond) { if (x) break; ... }` reproduces the shared
  condition tail; `if (k < 0) return;` + `for (;;) { {scope} if (--k < 0)
  return; }` reproduces a loop that exits through the epilogue copy.
- Inline-getter vs direct member (`g_pAppManager->GetServerExoApp()` vs
  `->m_pServerExoApp`) decides whether the app pointer is loaded before the
  arguments (eax) or straight into ecx; try both per call site.
- Swapping `pA`/`pB` declaration order and `i != n - 1 ? i + 1 : 0` fixed the
  polygon test; float temporaries that the original spills come from a named
  local (`float fAY = pA->y;`).

Next in range: 0x00591CA0 SpawnCreatureFromSpawnList (1280, x87 sin/cos),
0x00592430 ReadEncounterFromGff (3488; its creature list sort / names block
is the LoadEncounter code, and its SpawnList Add inlines Allocate), the
combat cluster in 0x005B4AA0..0x005BBBF0 (ResolveDamage 0x005BB440,
ResolveRangedAttack 0x005BB590, ResolveMeleeAttack 0x005BB890, miss location
0x005B4AA0) best added to creature_misc.cpp / combat_resolve.cpp, the dialog
server set 0x0059EAA0..0x005A27B0, the walkmesh queue from round s1.

## 2026-10-26 round s5: CSWSCreature on-hit / combat resolution, CSWSAreaOfEffectObject

Folder `matching/src/f`: 661/733 -> 673/754 (kotor-match -q -j 2).
21 new annotations: 11 exact, 10 NONMATCH; 1 prior NONMATCH promoted
(0x005B9670 ApplyOnHitConfusion). No prior MATCH regressed. (Round s4 left
only `combat_ai.cpp` with 4 annotations and no notes; this round continues it.)

`src/f/combat_ai.cpp` (CSWSCreature, nwscreatureaicombat.o):
- Exact: 0x005B9770 ApplyOnHitFear, 0x005B9870 ApplyOnHitHold, 0x005B9980
  ApplyOnHitSleep, 0x005B9B80 ApplyOnHitSlow, 0x005B9D40 ApplyOnHitStun,
  0x005BA0D0 ApplyOnHitPoison (handler names descriptive; none of 0x005B9770,
  0x005B9870, 0x005B9D40, 0x005BA0D0, 0x005BA170 has a functions.csv row),
  promoted 0x005B9670 ApplyOnHitConfusion.
- NONMATCH: 0x005B9E40 ApplyOnHitAbilityDamage (31) and 0x005BA170
  ApplyOnHitSlayRacialGroup (22) - eax/ecx/edx rotated by one; 0x005B8C20
  ResolvePostRangedDamage (21); 0x005B9040 ResolveOnHitVisuals (24);
  0x005B78E0 IsProjectileParryable (16); 0x005B7D10 ResolveDamageShields (82);
  0x005B7B30 ResolveMoveToForceJump (37, name descriptive); 0x005BB590
  ResolveRangedAttack (45). ResolveDefensiveEffects (0x005B8E50, s4) went
  65 -> 67 after the shared CSWSObject / creature layout grew.

New `src/f/aoe_shape.cpp` (own CSWSAreaOfEffectObject layout +0x22c..+0x28c,
out-of-line Vector() ctor so `new Vector[4]` uses the vector ctor iterator):
- Exact: 0x005945F0 SetShape, 0x005946E0 GetPosition (calls itself when not
  linked, as the original), 0x005947B0 LoadAreaEffect, 0x00594B00 LoadEffect,
  0x00595450 InSubAreas.
- NONMATCH: 0x00594D80 SaveEffect (35), 0x00595F30 AddToArea (30).

Lessons:
- `mov al,[x]; movzx edx,al` before a call argument = an inline getter that
  returns `unsigned char` (CSWSCombatRound::GetCurrentAttackIndex(),
  CSWItemProperty::GetParam1Value()); a direct member read gives one
  `movzx edx, byte ptr`. Same for `mov ax,[x]; cmp ax,dx` (ushort getter).
- `++m_nAttackID` copied into another byte: inline `GetNextAttackID()` keeps
  the value in cl (`inc cl; mov [..],cl; mov al,cl`).
- A float argument passed with `fld/fstp [esp]` instead of `mov/push` = an
  inline float getter (SaveEffect `GetRadius()`).
- Early `return 0`s that the original sends to one shared epilogue (callee
  saves pushed in the prologue, not just before the loop): rewrite as nested
  ifs with a single trailing `return` (InSubAreas, IsProjectileParryable
  48 -> 16).
- `CExoString("X")` default argument: write `"X"` and let the implicit
  conversion build the temporary (re-addressed with lea); a by-reference
  default built before the call needs a block-scoped named local
  (LoadEffect `{ CExoString sDefault(""); ... }`).
- `g_pRules->Get2DArrays()->m_pTable` (inline getter then member) gives the
  original `mov ecx,[g_pRules]; mov eax,[ecx+b8]`; `g_pAppManager->
  m_pServerExoApp->...` gives `mov ecx,[g]; mov ecx,[ecx+8]`.
- OBJECT_INVALID is sometimes the immediate 0x7F000000 (SetShape) and
  sometimes the global 0x00746644 (GetPosition) in the same file.
- `float fRadius = Read...(); SetShape(0, fRadius, 0.0f);` - a float call
  result passed on needs the local, else the pushes reorder.
- Squared lengths: MSVC swaps the first and last product; write
  `x*x + y*y + z*z` to get `z*z + y*y + x*x` and vice versa; the compare
  `!(a <= b)` gave the original `test ah,1; je` form here.
- Member `a / 2.0f + GetPosition()` evaluates GetPosition first (args before
  the left operand of a member operator+).

Next in range: AoE 0x00595640 UpdateSubAreas (1312), 0x005950C0
LineSegmentIntersectAreaOfEffect (912, x87 heavy); combat 0x005BAFA0
ResolveOnHitEffect (1184, calls the ApplyOnHit* member table),
0x005BA540 / 0x005BA8E0 ranged / melee special attack, 0x005B8820
ResolveSneakAttack, 0x005B92B0 SavingThrowRoll, 0x005B5EA0
SendAttackSummaryFeedback; the s1/s3 queues above (walkmesh, encounter
spawn, dialog server).
