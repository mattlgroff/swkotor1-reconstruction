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

## 2026-10-27 round s7: CSWSDialog server loaders, CSWSJournal, CSWSStore / CSWSWaypoint loaders

Folder `matching/src/f`: 676/762 -> 690/783 (kotor-match -q -j 2).
21 new annotations: 14 exact, 7 NONMATCH. No prior MATCH regressed
(race_room.cpp's CSWFeat ctor stays NONMATCH 18).

New `src/f/dialog_server.cpp` (own CSWSDialog layout: speaker-id cache
+0x84/+0x88 as CSWSDialogSpeaker {oid, CExoString}, stunt models +0x48/+0x4c,
camera-angle table +0x9c..+0xf8, ambient track +0xfc; node classes
CSWSDialogBase (0x84) + CSWSDialogCamera (0x24) = CSWSDialogNode, entry 0xb0 /
reply 0xb4, reply links 0x18 (out-of-line ctor) / entry links 0x14 (inline ctor)):
- Exact: 0x005A0E80 CSWSDialog ctor, 0x0059EAA0 LoadDialogCamera, 0x0059FF50
  SetDialogDelay, 0x0059EF00 AddJournalEntry, 0x0059FC30 GetSpeaker (returns
  the CSWSObject*, not int as dialog.cpp declares it; GetNearestObjectByTag
  takes a float range here), 0x005A1050 BarkOneLine, 0x0059F5F0 LoadDialogBase.
- NONMATCH: 0x005A27B0 Cleanup (5: reply loop `[edi+reg]` vs `[reg+edi]`; the
  identical entry loop matches), 0x005A2AE0 LoadDialog (56: everything up to
  the StuntList matches; the stunt `new[]` result lives in ebp zeroed before
  the count test), 0x005A03C0 HandleReply (81, descriptive name; symbols.csv
  only has FUN_005a03c0; stack slots of pPos/pClient/pFound rotated).

New `src/f/journal_set.cpp`: exact 0x005C63F0 CSWSJournal::SetDate,
0x005C6650 SetTime; NONMATCH 0x005C68B0 Destroy (8: frame 4 bytes larger,
the LowerCase temporaries do not share a slot).

New `src/f/store_items.cpp`: exact 0x005C7610 CSWSStore::AcquireItem;
NONMATCH 0x005C6F70 SellItem (28: cl cross-jumps the three AcquireItem tails).
New `src/f/store_template.cpp`: exact 0x005C7760 LoadFromTemplate, 0x005C6CD0
SaveStore; NONMATCH 0x005C7180 LoadStore (70: the original parks pItem /
nPosition in dead argument slots). New `src/f/waypoint_template.cpp`: exact
0x005C83B0 CSWSWaypoint::LoadFromTemplate; NONMATCH 0x005C7F30 LoadWaypoint
(15: identical code, frame 8 bytes larger). `race_room.cpp`: exact 0x005C7890
CSWRace::CSWRace (function start missing from functions.csv; it follows
CSWSStore::LoadFromTemplate's padding).
(The checkpoint commit "AcquireItem exact" before "AcquireItem/LoadFromTemplate
exact" was premature; AcquireItem became exact in the later one.)

Lessons:

- Declaring struct locals (CResStruct / CResList / loop elements) at function
  top fixed whole-function stack layouts (LoadDialogBase 31 -> 0, LoadDialog
  121 -> 56); MSVC packs block-scoped struct locals into temp slots otherwise.
- "Search, then act" loops whose found branch jumps straight past the not-found
  code: write `int bFound = 0; for (...) if (match) { ...; bFound = 1; break; }
  if (!bFound) {...}` - cl threads the flag away (SetDate/SetTime, Destroy).
  `for (...) if (match) break; if (i < n)` re-tests the index, and a `return`
  inside the loop duplicates the destructor tail.
- Getter vs direct member (`g_pAppManager->GetServerExoApp()` vs
  `->m_pServerExoApp`) is per call site and interacts with neighbouring calls;
  brute-forcing the 2^n combinations with a scratch script was fastest
  (BarkOneLine 65 -> 0). A call argument computed into a named local first
  (`int bIgnore = ...; client->BarkString(..., bIgnore)`) changes when the
  `this` pointer is loaded; same for `unsigned long oid = pItem->m_oid;`
  before `GetGameObject(oid)` (AcquireItem).
- An inline flag getter (`GetFadeOnBark()`) moved a member test from ecx to eax.
- `if (x == 2) a = 1; if (x == 1) {...} else {...}` reproduced the original
  "cmp 2 / cmp 1 placed after the loop" layout where a switch gave a dec chain.
- `if (!n) p = 0; else p = new T[n];` stores the known-zero count register;
  the reverse order stores an immediate 0.
- A CExoLinkedList head-object getter has two shapes: `void *p = head; if (p)
  p = p->pObject;` and the ternary `head ? head->pObject : 0`.
- An empty out-of-line SJournalEntry dtor must be inline (`~SJournalEntry() {}`)
  in callers that destroy a local entry (SetDate).
- `delete p` on a class with a virtual dtor emits the null test itself; an
  explicit `if (p)` is not needed.

Next in range: retry SellItem (separate AcquireItem tails), LoadStore and
LoadWaypoint frame sizes; dialog server send paths 0x005A13D0
SendDialogEntryNode (2096), 0x005A1C00 SendDialogReplyNode (2992), 0x005A3820
SendDialogReplies (2032), 0x005A4010 SendDialogEntry (2128) - they share the
dialog_server.cpp layout; 0x005C5A40 CSWSJournal::SetState (2480, 2DA/JRL);
CSWRace::LoadFeatsTable 0x005C7AD0 (496) next to race_room.cpp; the s1/s3/s5
queues (walkmesh, encounter spawn, combat).

## 2026-10-27 round s8: CSWSDoor / CSWSTrigger / CSWSPlaceable geometry, ctors, loaders and savers

Folder `matching/src/f`: 690/783 -> 710/808 (kotor-match -q -j 2).
25 new annotations: 20 exact, 5 NONMATCH. No prior MATCH regressed.
(The round was started by an earlier worker cut off at the usage limit; its
WIP commit already held OpenDoor / placeable SetOrientation / GetBoundingBox.)

Exact:
- `door_s8.cpp` (own CSWSDoor / CSWSPlaceable / CSWSTrigger layouts): 0x00589C70
  CSWSDoor::OpenDoor, 0x005853C0 CSWSPlaceable::SetOrientation, 0x0058CC60
  CSWSTrigger::GetBoundingBox, 0x005897D0 CSWSDoor::InDoorArea, 0x00589980
  CSWSDoor::CheckPlacement, 0x0058CE40 CSWSTrigger::InTrigger, 0x0058D8C0
  CSWSTrigger::GetClosestPointWithinTrigger.
- `trigger_geometry.cpp` 0x0058D060 CSWSTrigger::LoadGeometry; `trigger_create.cpp`
  0x0058D210 CSWSTrigger::CreateNewGeometry; `trigger_ctor.cpp` 0x0058EAE0
  CSWSTrigger::CSWSTrigger and 0x0058ED60 CSWSTrigger::GetLocName (function start
  missing from functions.csv: `lea eax,[ecx+0x228]; ret` after the ctor);
  `trigger_save.cpp` 0x0058E660 CSWSTrigger::SaveTrigger.
- `door_ctor.cpp` 0x00589EE0 CSWSDoor::CSWSDoor, 0x0058A1B0 CSWSDoor::GetConversation
  and 0x0058A1E0 CSWSDoor::GetLocName (both starts missing from functions.csv);
  `door_load.cpp` 0x0058C5F0 CSWSDoor::LoadInstance (descriptive name; template
  path or LoadDoor, then AddObjectToLookupTable); `door_save.cpp` 0x00588AD0
  CSWSDoor::SaveDoor.
- `placeable_ctor.cpp` 0x005877E0 CSWSPlaceable::CSWSPlaceable.
- `res_mesh.cpp` 0x005CE8B0 CSWDoorSurfaceMesh::GetResRef ("%s%d" with the door
  state at +0x88); `door_mesh_load.cpp` 0x005CE770 CSWDoorSurfaceMesh::LoadMeshBinary.

NONMATCH: 0x005896E0 CSWSDoor::SetOrientation (2, q.z*q.w operand order),
0x00584C20 CSWSPlaceable::GetNearestActionPoint (50, use-point addresses kept in
ebx/ebp), 0x00589240 CSWSDoor::GetNearestActionPoint (82, the original spills the
difference vectors before squaring), 0x00587170 CSWSPlaceable::AddToArea (21,
load order and sete register in the 2nd/3rd 2DA lookups), 0x00586A70
CSWSPlaceable::SavePlaceable (70 by count, really 5 lines: bearing slot / ecx
scheduling at the end).

Lessons:

- A default/conversion argument written as an implicit conversion (`..., bSuccess, "")`
  into a `const CResRef &` / `const CExoString &` parameter) recomputes the temporary's
  address with `lea` before the push; an explicit `CResRef("")` pushes the ctor's eax.
  Same for `WriteFieldCResRef(pStruct, m_sScripts[i], ...)` (CExoString -> CResRef).
  Door LoadInstance went 0x5c-frame NONMATCH -> exact with it.
- `(a / b) * c` and `a / b * c` compile differently: the parenthesised form keeps the
  division first (InDoorArea). `nLast = m_n - 1` declared inside the loop body was
  hoisted and spilled exactly like the original; declared before the loop cl
  rematerialised it from the reloaded count.
- Polygon loops over an index list: load `nIndex = m_pnIdx[i]` before computing j,
  and form pB before pA (InTrigger).
- Getter vs member again decides evaluation order: `g_pRules->Get2DArrays()->GetX()`
  evaluates the object chain into eax/ecx before the pushes; an inline float getter
  (`GetBearing()`, `GetHighlightHeight()`) turns a mov copy into fld/fstp.
- `x = y = z = 0; w = 1` (chained) stores z, y, x, w - the Quaternion ctor order.
- An int local whose address is passed (2DA GetINTEntry) lands in a dead argument
  slot only when it is block-scoped (AddToArea PreciseUse).
- Reusing one `Vector v` for position then orientation (instead of two locals)
  reproduced SaveTrigger's shared stack slot; the struct locals (CResList, CResStruct)
  at function top fixed the rest.
- When cl inlines an out-of-line helper that the original calls (CResDWK::GetUsePoint
  defined in res_mesh.cpp), move the caller to its own TU (door_mesh_load.cpp).
- Field names: the savers give authoritative trigger/door names (trigger +0x230
  LinkedTo, +0x238 LinkedToModule, +0x240 LinkedToFlags, +0x30c TransitionDestination;
  door +0x2a0 Appearance, +0x2a1 GenericType, +0x390 LinkedToModule, +0x3c8
  TransitionDestination). door_life.cpp / door_trigger.cpp still use older names for
  some of these offsets (no byte impact).

Next in range: retry the five NONMATCHes above; 0x0058C8A0 CSWSTrigger nearest
outline point (960, x87), 0x00584EC0 CSWSPlaceable::AIActionCastSpell (448,
argument-slot reuse), 0x00588690 CSWSPlaceable::AcquireItemsFromObject (688),
0x0058DA80 trigger fields loader (3040), 0x0058A1F0 CSWSDoor::LoadDoor (4576),
0x00587A70 / 0x00585670 placeable template / fields loaders; then the s7 queue
(dialog server send paths, CSWSJournal::SetState, CSWRace::LoadFeatsTable).

## 2026-10-27 round s9: level-up / experience / attack bonus, CTwoDimArrays drivers, misc

Folder `matching/src/f`: 710/808 (s8 end) -> 720/828 (kotor-match -q -j 2).
The round was started by an earlier worker cut off at the usage limit; its WIP
commit held `stats_talent_random.cpp` (4 talent pickers, all NONMATCH, compiled).
20 new annotations (4 from the WIP commit): 10 exact, 10 NONMATCH. No prior MATCH regressed.

Exact:
- `stats_levelup.cpp` (own copy of the stats layout, GetClass / GetClassLevel inline):
  0x005AFB00 CSWSCreatureStats::FeatRequirementsMet, 0x005AF6A0 AddExperience,
  0x005A7B60 GetRangedAttackBonus.
- `two_dim_arrays_life.cpp`: 0x005BFB60 CTwoDimArrays::CTwoDimArrays, 0x005BFCC0
  ~CTwoDimArrays, 0x005C5070 UnLoad2DArrays (keeps the +0x54 test / +0x50 unload
  quirk), 0x005C4C20 Load2DArrays.
- `race_room.cpp` 0x005C7AD0 CSWRace::LoadFeatTable (name from its diagnostic string).
- `encounter_load.cpp` 0x00594310 CSWSEncounter::RemoveFromActiveCreatureCount.
- `pathfind.cpp` 0x005CFC70 CAvoidCreature::IsPointInHex.

NONMATCH: 0x005B2530 CanChooseFeat (32, nFeat register), 0x005AF950 ValidateLevelUp
(22, edi/esi swap), 0x005AF480 SetExperience (61, level-down loop registers and the
two CExoString slots swapped), 0x005A7770 GetMeleeAttackBonus (30, lea vs add for the
DEX+STR sum, slot-lookup registers), 0x005CFAE0 CAvoidCreature::GetClosestPointTo (4,
x87 pops of the last unrolled iteration), 0x00588690 CSWSPlaceable::AcquireItemsFromObject
(105, first draft in `placeable_acquire.cpp`), and the four WIP talent pickers
0x005AC370 (67), 0x005B23A0 GetCreatureTalent (6), 0x005AB9E0 (61), 0x005AC5D0 (116).

Lessons:

- Zero register: `int nPenalty; ... nPenalty = 0;` (declared at the top, assigned just
  before its block) made cl keep 0 in the variable's register from the prologue and use
  it for `push 0` / `cmp reg, 0` / `mov [x], 0` (GetRangedAttackBonus 23 -> 16 -> exact,
  GetMeleeAttackBonus 60 -> 30). `int n = 0;` at the top does not (worker-k saw the same).
- `g_pRules->m_pBaseItemArray->GetBaseItem(pItem->GetBaseItemType())` with an inline
  `int GetBaseItemType() { return m_nBaseItem; }` loads the argument into eax before the
  object chain (`mov eax,[x+0xc]; mov edx,[g]; mov ecx,[edx+0x34]; push eax`); the plain
  member form uses ecx/edx. Fixed the last ranged-bonus diffs.
- Inline getters for call arguments reproduce the original right-to-left argument
  evaluation into stack temps when the first argument is itself an inline expansion
  (FeatRequirementsMet: `GetSpellLevelRequirement(info.GetLevel(0), pFeat->GetMinSpellLevel(),
  nClass, GetRace(), GetCHABase())`). With plain members cl evaluates the inline first.
- A loop whose exit re-tests a found flag the compiler could prove zero: write
  `for (i = 0; !bFound; i++) { if (i >= n) break; ... }`; do/while(!bFound) and the
  for(;;) forms fold the test away.
- Load2DArrays: every early-out `goto failed;` with `return 1; failed: return 0;` at the
  end reproduced "fail block before the final return 1"; early `return 0`s put the block
  after the first test, `&&` chains put `return 1` first.
- Local declaration order decides byte-slot order in the frame (AddExperience: declaring
  `nHighest`, then `nBest`, then `nFavored` fixed both the slots and the use of esi for
  g_pRules); a member read cached by cl gets its spill slot after the named locals.
- `(char)`-typed ternaries: `int nDEXMod; if (deb) { nDEXMod = m_nDEXMod; nDEXMod =
  __min(nDEXMod, 0); } else nDEXMod = m_nDEXMod; bonus = (char)nDEXMod;` gives the
  original `movsx ecx; setge; dec; and; movsx ebx, al`; `__min(m_nDEXMod, 0)` stays byte.
- A Vector dot product `n.x*d.x + n.y*d.y + n.z*d.z` vs `d.x*n.x + ...` picks which
  Normalize result gets the lower frame slot (IsPointInHex). A float literal
  (3.402823466e+38F) instead of an extern constant lets cl compare against memory in the
  first unrolled iteration (GetClosestPointTo 26 -> 4).
- CSWSCreatureStats +0x28 is the level-stats list (CExoArrayList<CSWSStats*>): the
  out-of-line 0x005A6170 named GetEffect in stats_getters.cpp is really GetLevelStats
  (PC-only), inlined in SetExperience. stats_getters.cpp keeps the old name/type (no
  byte impact).
- 0x005A5DF0-style "TalentAllowed" expansion and the talent pickers are register ties;
  the zero-register trick moved the prologue closer but not the loop body.

Next in range (all remaining todo items are large or x87):
- LevelUp 0x005AABF0 (1520) / LevelDown 0x005AB1E0 (1584): fit `stats_levelup.cpp`
  (inline CExoArrayList Add/IndexOf/Remove on the class spell lists, ability switch).
- Retry the near-misses above with the zero-register trick (SetExperience, ValidateLevelUp,
  CanChooseFeat), AcquireItemsFromObject (list fields cached in esi/edi).
- 0x005CF8D0 CAvoidCreature::SideClearOfObstructions (528, CWalkHitInfo + switch),
  0x005CF1F0 (1120 x87), 0x00591CA0 SpawnCreatureFromSpawnList (1280, x87),
  0x005950C0 AoE segment test (912, x87), 0x005A8BE0 GetDamageBonus (1136, debug strings),
  0x005B92B0 SavingThrowRoll (960), dialog send paths 0x005A13D0 / 0x005A1C00 / 0x005A3820 /
  0x005A4010, CSWSJournal::SetState 0x005C5A40.
