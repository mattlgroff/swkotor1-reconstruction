# Worker A, round 1 (2026-10-26)

Scope: 0x004D0000–0x0051FFFF, isolated branch codex/worker-a-r1.
Read current agreements, audit, A creature declarations/sources, and old worker A
notes read-only. Prefetched all 274 eligible todo rows (at least 100 screened).
Selected substantial coherent CSWSCreature server AI/action, party-follow,
perception/action-list family, expanding beyond the initial compact anchors.

Checkpoint 1: 274 screened, 6 attempted, 4 NEW exact, 2 retained NONMATCH.
Exact: 00513F60, 00511130, 005188E0, 00518940.
Misses: 005178F0 (4 diff), 00517960 (2 diff). Both list removers have an
observed post-dereference null check MSVC elides; three source shapes tried.
The existing symbol name OnIdChanged at 00517960 is retained despite the Mac
hint RemoveFromVisibleList. GetIsPossessed uses the documented folded zero
leaf at 0063E7F0; GetSpellActionPending is already defined at 004D35C0. Shared reports intentionally untouched.

Checkpoint 2: 11 attempted cumulatively, 8 NEW exact, 3 retained NONMATCH.
New exact: 00510FD0, 00513EC0, 005140F0, 005141B0. Camera action 005137C0
retained at 5 diff after early self-ID load reduced 9 to 5; remaining miss is
EAX versus EDX for the second app-manager load, shifting the short branch.
Confirmed typed camera message signature from existing defined symbol; node
float parameters use a local union, not a changed public ABI. World-time
SubtractWorldTimes retains the existing definition's int return type.

Checkpoint 3: 16 attempted, 10 NEW exact, 6 retained NONMATCH.
Exact: 00513E30 CanUnEquipWeapon and 00510E30 CheckProficiencies.
Retained item restrictions: 005145A0 (23), 00514650 (7), 00518210 (17).
After separate early returns, remaining misses are activation-byte AL/CL,
mask construction/test scheduling and nested class-loop register choices.
Three source shapes exhausted; no signature or flag semantics changed for bytes.
CanUnEquipWeapon clears CSWSPlayerStore slot IDs 4/5 through player +0x54;
the store GUI/store names and layout were checked in existing B headers.

Checkpoint 4: 22 attempted, 13 NEW exact, 9 retained NONMATCH.
Invitation family exact: 00518450, 00518530, 00518580. Same 12-byte record
{object id, day, time}; Allocate/DelIndex targets checked against existing
folded 12-byte template instantiations 004186E0/0048DDE0.
Retained: SendInvite 0051B580 and AddToInvitationsIgnored 0051B650 (24 each,
compiler removes the original empty constructor EH envelope); ReplyToInvitation
0051D060 (21, EBX/EBP lifetime and switch failure-tail layout). Two shapes tried.
SendInvite uses its already-proven name despite the reference's
AddToInvitationsOffered alias. Batch-local declarations prevent an array-list
constructor from changing established implicit constructors in shared headers.

Checkpoint 5: 27 attempted, 17 NEW exact, 10 retained NONMATCH.
Exact: 00512A60, 00512B40, 00512C20 (follow wait-destination states 1/3/4),
00514AB0 (path-data ctor). Exact sibling 00512D00 supplied established
CSWFollowPath/GetFollowPoint declarations; each new body checked separately.
Constructor owner chosen as CSWSPathData from existing exact Clear 0050E270
and identical 0x74-byte layout, rather than the uncorroborated CAvoidCreature
playable alias. TerminateClientSidePath 005110B0 retained at 5 diffs after
three receiver-load order shapes; all vector stores and status writes align.

## Final compact-family handoff

32 new attempted addresses, 19 NEW exact, 13 retained NONMATCH; 274 candidates
screened/prefetched. Prior NONMATCH promotions: 0. Prior exact rechecks do not
count toward new wins. No duplicate addresses reported.

Canonical full A-folder run (`-q -j 4`, MSVC 7.0 `-O2 -Gy -GX`, original EXE):
**445/509 exact**, versus starting folder **426/477 exact**. Comparison against
starting status.csv: **0 prior MATCH regressions, 0 old NONMATCH diff-count
changes**. Private temporary report: /private/tmp/worker-a-r1-status.csv; shared
status/symbol reports were not written. Matching-only work needs no VS2022
build or CTest run.

NEW exact addresses:
00510C20, 00510E30, 00510FD0, 00511130, 00512A60, 00512B40, 00512C20,
00513E30, 00513EC0, 00513F60, 005140F0, 005141B0, 00514AB0, 00515860,
00518450, 00518530, 00518580, 005188E0, 00518940.

Retained NONMATCH addresses (diff instructions):
004F1EF0 (29), 0050FB60 (8), 0050FC90 (4), 005110B0 (5), 005137C0 (5),
005145A0 (23), 00514650 (7), 005178F0 (4), 00517960 (2), 00518210 (17),
0051B580 (24), 0051B650 (24), 0051D060 (21).

The final five attempts were CheckWeaponSlot (00515860, exact), encounter
self-destruction (00510C20, exact), both facing actions and CancelAction.
Important compiler facts: a float getter forces the observed x87 argument
materialization for encounter challenge rating; cache server/event/self ID
before AddEventDeltaTime for the exact event argument order. The no-argument
RET 0 at 00510C20 is preserved despite the playable wrapper's unused node.
CheckWeaponSlot was unnamed in symbols/Mac records: its name is explicitly
descriptive and inferred from its proficiency/occupied-slot/feedback contract.

Completed the selected compact action/equipment/follow/invitation workset.
Larger compatible next work: AIActionCheckForceFollowObject 005136B0, follow
teleport 00512DE0 and speed-factor 00512F10; movement 0050FD50..00510AB0;
UnsummonMyself 005149B0 (EH plus effect ownership); full race/use-item check
005182F0/0051B440. Preserve the batch-local declarations for larger layouts
and use existing folded callees. The >500-byte pathfind/combat state machines
remain separate deeper follow-up work, not fabricated compact attempts.

Publication baseline was advanced by the coordinator during this run to
`ee977a4` / 5,584 exact. No worker push was performed. These 19 new matches
remain local for independent acceptance toward the next 100-new threshold.


# Worker A, round 2 (2026-10-26)

Fresh branch codex/worker-a-r2 at frozen r1 4868ed9. Prefetched 274 raw todo
rows and excluded 32 parent annotations, leaving 242 available candidates.
Checkpoint 1: 8 attempted, 3 NEW exact, 5 retained NONMATCH. Exact:
004FD6F0 CalculateDamagePower, 00501D90 UpdateClassInfo (established symbol
name retained despite pure-alignment hint), 004FDAA0 CancelCombat.
Misses: 005136B0 (4), 005111C0 (5), 00512F10 (12), 004FDA20 (5),
004FD960 (7). Bounded ordinary source shapes exhausted; remaining stack-slot,
loop rotation, x87 multiplication and receiver load scheduling ties.
No parent scheduling misses revisited; shared reports untouched.

Checkpoint 2: 13 attempted cumulatively, 6 NEW exact, 7 retained NONMATCH.
New exact: 0051B720 (generic dword-array assignment), 004FDBD0/004FDC80
(pure good/evil power effect creation). Effect magic subtype is a two-bit
field in the ushort at +0x0a; a ushort bitfield generates the observed
byte-memory AND while a char field creates a register load/store tie.
The effect's complete 0x8c declaration is local to this translation unit.
Retained 0051AFD0 at 2 diff (initial count memory compare) and 00512DE0
at 29 (EBX/EBP receiver/node register allocation). Existing three wait-state
siblings still compare exact after extending their isolated header.

Checkpoint 3: 19 new attempts, 8 NEW exact, 11 retained NONMATCH.
Added exact 004FDD30 (effect-pointer array copy constructor, shallow ownership
contract) and 004FDDD0 (GUI action array SetSize, complete size 0x38).
Copy-constructor/growth implementation is isolated; adding a declaration
for SetSize to the shared array does not change its constructors.
Movement check actions 00510670/005108A0/00510AB0 retained at 11/29/23 diff
after three source forms; all node arguments, lookup, flag/radius checks,
requeue calls, and party state writes follow the canonical instruction flow.
UnsummonMyself 005149B0 retained at 8 diff after three event receiver forms;
all EH, allocation, visual effect and destruction-event bytes match except
the event/app-manager register selection.


# Worker A, round s1 (2026-10-26)

Branch claude/worker-a-s1 from main c47d071. Family: CSWSArea serialisation,
GIT/ARE/PTH loading and object-array helpers (0x00504870-0x0050E190), all in
nwsarea.o. 223 todo rows screened; 33 new addresses attempted.

Folder `kotor-match -q -j 2 matching/src/a`: 453/528 -> **476/561**.
0 prior MATCH regressions; old NONMATCH diff counts unchanged.

NEW exact (22): 00504870 LoadRooms, 00504A10 ??_ECSWSRoom (vector deleting
dtor COMDAT emitted after LoadRooms; produced by a helper doing delete[]),
00504A70 LoadCreatures, 00504DE0 LoadItems, 00505060 LoadEncounters,
00505360 LoadWaypoints, 00505560 LoadSounds, 005057A0 LoadStores,
00505AF0 LoadAreaEffects, 00505EB0 LoadPlaceableCameras, 00506090
SaveProperties, 00506910 AllocateRoomArrays (descriptive name), 00506BD0 /
00506C30 CResHelper<AreaGFF,2012> dtor / SetResRef, 00506E20 / 005089E0
CExoArrayList<CResRef>::Allocate / Add, 005073D0 LoadRoomInfo, 005086D0
NoNonWalkPolys, 00508400 LoadPathPoints, 00508A30 AIUpdate, 0050A7B0
LoadPlaceables, 0050DD80 LoadGIT.
Prior NONMATCH promoted: 00507750 SaveItems (CSWItem needed a virtual dtor so
CSWSItem keeps CSWItem at +0 and CSWSObject at +0x10).

Retained NONMATCH (diff insns): 00505DA0 LoadMaps (10, return value kept in
memory vs esi), 005062A0 SavePlaceableCameras (17, ebx/ebp swap), 00506400
PositionWalkable (20, mesh/vector copy order), 00506F20
GetFirstObjectIndiceByX (19, this vs last-index register), 00506FE0
UpdatePositionInObjectsArray (16, IndexOf registers + base/index order),
00507220 ActivateAI (25, loop counter vs AI master register), 00507490
LoadProperties (41, all register naming; uses inline getters for the byte
defaults), 0050A0E0 LoadDoors (9, bearing store scheduling), 0050DC40
RemoveObjectFromArea (20) and 0050DFD0 AddObjectToArea (11) (event-argument
register naming), 0050E190 LoadArea (8, both imul operands loaded to regs).

Lessons:
- GIT loaders share one shape: `new T(oid); if (!p) continue;` then
  `if (bUseTemplates) { CResRef cDefault(""); CResRef c = ReadFieldCResRef(..,
  cDefault); if (!bSuccess || !LoadFromTemplate(..)) { delete p; continue; } }
  else if (!LoadX(..)) { delete p; continue; }`. A bLoaded flag or ternary
  gives the right blocks but wrong registers.
- Reads evaluated Z,Y,X are constructor/call arguments (`Vector(X,Y,Z)`,
  `AddToArea(this, X, Y, Z, ..)`); reads X,Y,Z in order are member stores.
- `ReadFieldBYTE(.., m_bInt)` zero-extends the byte; the original's full-dword
  push came from inline getters returning int.
- Orientation normalisation needs an inline `Vector::MagnitudeSquared()`;
  the open-coded expression reassociates.
- ObjectId/template/position success flags can be separate variables; their
  slot sharing with parameter homes decides the frame size (LoadItems).
- When several branches call the same setter with member values, the
  original has one call site behind a change flag (AIUpdate weather).
- CSWSArea + 0x100 is CResHelper<AreaGFF,2012>; the header's m_pGIT at 0x108
  is really that helper's AreaGFF resource (name kept to avoid churn).

Next in-range work: LoadTriggers 0050A350 (CExoLocString/CExoString temps,
EH states 2-4), SetCurrentWeather 00508050 (needs the CGameObject-at-0x11c
cast for GetArea()->m_idSelf), 00508820 (aligned frame, appearance 2DA),
PlayVisualEffect 00507E20, BudgeCreatures 00508180, HandleTransparentDoors
0050C0E0, CSWSArea ctor 0050CF80 and the 0x00506D90 list holder ctor.


# Worker A, round s2 (2026-10-26, resumed after usage limit)

Branch claude/worker-a-s2 (local). Resumed from the WIP commit b8e5a24, which
did not compile: sws_area_surface.cpp redefined C2DA/C2DAs/CSWRules after the
WIP moved them into sws_area.h (fixed by adding m_pSurfaceMaterial at C2DAs
+0x14 and deleting the local copies); a scratch zz_stub.cpp with duplicate
annotations was removed. Family: CSWSArea (nwsarea.o) walk/LOS, effects,
doors, GIT load/save.

Folder `kotor-match -q -j 2 matching/src/a`: main 476 -> **485/577**.
0 prior MATCH regressions; old NONMATCH diff counts unchanged.

NEW exact (9): 00506D90 CExoLinkedList ctor (from the WIP), 00508820
FlagCreaturesWithHead, 00508010 ListContainsObject (annotation moved from
0x00507FF0: the real start is 0x00508010, the function is missing from
functions.csv; it is CSWSArea vtable slot +4), 005064D0 GetDoorCrossed,
0050C0E0 UnloadArea (functions.csv lacks the 0x0050C1E0 start, matcher
still reports exact), 0050C1E0 HandleTransparentDoors (vtable +0x18, missing
from functions.csv), 0050C330 ClearLineOfSight, 0050C6B0 ApplyEffect,
0050A350 LoadTriggers.

Retained NONMATCH (diff insns): 00506650 NoNonWalkPolysDetailed (1: frame
0x34 vs 0x28, slot layout only), 0050CA10 NoNonWalkPolysInDoors (12: frame
0x38 vs 0x30), 00508050 SetCurrentWeather (13: dead `add eax,0x11c` from a
real CGameObject base cast + esi ordering), 0050CCF0 GetObstructionFactor
(28: frame +0xc and `fxch; fadd [vFrom]` order), 00508180 BudgeCreatures
(35: this/list/index register permutation), 00507E20 PlayVisualEffect (53:
index in the dead pPosition slot, this/table register order), 0050BA00
SaveGIT (128: pGFF vs loop index register; orig list dtors show no
`element = NULL` store).

Lessons:
- CSWSArea is polymorphic (CSWArea vptr at 0): slot +4 ListContainsObject,
  +0x18 HandleTransparentDoors. Replacing pad_0x0 by the virtuals changed no
  existing match.
- Inline getters decide registers/load order: `GetAppearance()`,
  `GetType()`, `GetCreator()`, `GetDuration()`, `GetSpellId()`,
  `GetGenericType()` each fixed operand order or register choice where
  the direct member access did not (FlagCreaturesWithHead,
  HandleTransparentDoors, ApplyEffect).
- An argument loaded before a virtual call and pushed after it is the
  parameter of an inline wrapper (`AddAreaOfEffect(oid)` around AddUnique).
- `new CExoArrayList` only gets its EH state with the sized
  `CExoArrayList(int nSize = 0)` ctor (worker-i trick); sws_area.h now has
  it plus `~CExoArrayList() { delete[] element; element = NULL; }` and an
  `AddInline` (inline Add body; other TUs keep the out-of-line Add).
- Single-exit with a flag: `if (!bOpen) {...} if (bOpen || bArg8) { loop }`
  reproduced the je-to-cleanup of ClearLineOfSight where an early return
  duplicated the destructor epilogue.
- `if (vStart.x < vEnd.x) { min = start; max = end; }`: assigning min first
  in each branch and declaring the float temporaries at function scope
  fixed the x87 stack/memory split in the two walk-box functions.
- A float constant loaded first (`fld [const]; fadd [mem]`) is an extern
  const float, not a literal (PlayVisualEffect 250.0f at 0x00747440).
- Comparisons: `a <= b` -> test ah,41h/jnp; `a >= b` -> test ah,1/je.
- `&m_aGameObjects` kept in a pointer that is null-tested in the original
  (BudgeCreatures); MSVC does not fold the test.
- Two success flags: the ObjectId read in LoadTriggers uses its own int
  (separate stack slot from the template-path reads).
- Strings 0x747xxx etc. can be read from swkotor.exe with a small PE
  reader in the scratch dir (TriggerList, LinkedToModule, ...).

Next in-range queue (CSWSArea leftovers, all need structural work):
CSWSArea ctor 0050CF80 and dtor 0050D370 (need the real CSWArea /
CResHelper@0x100 / CGameObject@0x11c bases), EventHandler 0050D6C0 (1408 B,
script/movie/transition calls), then the creature action family
0050E400/0050E900/0050EE80/0050F170/0050FD50/0050FF20 (claude_w5_* units).


# Worker A, round s4 (2026-10-26)

Branch claude/worker-a-s4 (local). Family: CSWSEffectListHandler apply/remove
handlers 0x004DBA50-0x004E4790 (new file effect_list_handler5.cpp) plus a few
CSWSCreature helpers (sws_creature_disguise.cpp).

Folder `kotor-match -q -j 2 matching/src/a`: 504/600 -> **518/628**.
28 attempted, 14 NEW exact, 14 retained NONMATCH; no earlier MATCH lost.

NEW exact (14): 004E46E0 OnApplyBonusFeat, 004E31D0 OnApplyDisguise,
004DBA50 OnApplySlow, 004E2650 OnRemoveForceResistanceDecrease, 004E2F40
OnApplyVision, 004E2570 OnRemoveForceResistanceIncrease, 004E3C30
OnRemoveInvisibility, 004E14D0 OnRemoveACIncrease, 004E16E0
OnRemoveACDecrease, 004E18D0 / 004E19A0 OnRemoveDamageImmunityIncrease /
Decrease, 004E22F0 OnRemoveSetState, 004E3B70 CEffectIDList::AddUnique,
004F28D0 CSWSCreature::UpdateAmbientAnimationState.

Untabled starts annotated (functions.csv rows span them; do not edit the
csv): 004E31D0 (row 004E30D0), 004E2490/004E2570/004E22F0 (row 004E1C20),
004E14D0/004E16E0/004E18D0/004E19A0 (row 004E13C0), 004E2D50/004E2F40 (row
004E2A90). Still untabled and unwritten: 004E2720 OnApplyDisease (row
004E2650, 2DA strings + EH), 004E43C0 OnApplyDispelBestMagic (row 004E40D0).

Retained NONMATCH (diff insns): 004E35B0 OnApplyTrueSeeing (3), 004E3320
OnApplyUltraVision (4), 004EE150 UnDisguise (4), 004EE010 Disguise (7),
004E2D50 OnRemoveHasteOrSlow (7), 004E2490 OnRemoveMovementSpeedChange (9),
004DCD30 OnApplyBlindness (10), 004E3690 OnRemoveTrueSeeing (13), 004DCB40
OnApplyTimestop (22), 004E3480 OnRemoveUltraVision (33), 004F6D70
UpdateAppearance/stealth refresh (35), 004E30D0 OnRemoveEffectIcon (46),
004E40D0 OnApplyDispelAllMagic (78), 004E2A90 OnApplyHasteOrSlow (100).

Lessons:
- Early `if (!pObject->AsSWSCreature()) return 1;` lets the creature share
  pObject's register (esi); the outer-if form pushes extra registers and
  keeps both (OnRemoveInvisibility, OnApplyBlindness went the other way:
  there the outer-if form was right). Try both.
- Re-reading a member in an inner test (`pOther->m_nType == 0x22` instead of
  the cached nType) stops MSVC from jump-threading the redundant compare the
  original keeps (OnRemoveForceResistanceIncrease).
- `GetID()` (inline returning the __int64) makes RemoveEffectById's argument
  load lo before hi like the original; the plain member loads hi first.
- Clamp code with a duplicated `cmp x,1` comes from the stdlib macros
  `__max(__min(x, 1), -1)` (double evaluation); write the expression inside
  the macro, not a pre-computed local.
- `bHaste = nType == 1` with nType read into a local before the stores gave
  the original sete/mov order (OnRemoveHasteOrSlow 66 -> 7).
- Locals the original passes uninitialised to out-parameter calls must stay
  uninitialised (GetWorldTime/AddWorldTimes outputs in OnApplyTimestop).
- Switch-looking if/else chains: the original UpdateAmbientAnimationState is
  an if/else chain with one SetAnimation tail; the default `(n == 5) ? 0xc : 0`
  gives setne/dec/and.
- `pObject->m_appliedEffects` lives on CSWSObject (+0x124); moving it from
  CSWSCreature into CSWSObject in effect_list_handler.h changed nothing else.
- Real signatures used as overloads: SetDamageImmunity(unsigned short, char),
  SetForceResistance(unsigned char) (both in symbols.csv as "defines").
- Unsolved pattern (several functions): the original compares the effect
  type word in memory against a constant held in a register
  (`mov eax,0x49; cmp word ptr [edi+8],ax`, or 0x49 hoisted into ebp), where
  MSVC here loads the word and compares with an immediate. Locals, casts,
  GetType() and continue-forms did not reproduce it.

Next in-range queue: 004E2720 OnApplyDisease, 004E43C0 OnApplyDispelBestMagic,
004E4790 OnApplyEffectIcon, 004E3D10/004E3E70 Sanctuary apply/remove,
004E3800 OnApplyForcePush, 004DD270 OnRemoveDisappearAppear, 004DDAC0
OnRemoveWounding, 004DC7A0 OnRemoveHitPointChangeWhenDying, 004E0750
OnApplyHeal; creature: 004F19E0 death XP, 004EF930 ReceiveExperience,
004F1310 desired attack range, 004FD490 max force points.

## Worker A s8, 2026-10-26

Screened the 138 currently listed in-range todo rows; several are large or
contain multiple function starts. Recovered the locally saved 0x005101A0
AIActionCheckMoveToObject from codex/worker-a-r2 commit 81e6e8c, using the
current base's unchanged layout plus an explicitly named +0x314 vector.
The full 528-byte function compares MATCH using MSVC 7.0. The related existing
five movement attempts in the same file retain their previous outcomes.
The callee 0x004EE440 GetUseRange signature is also supported by both Mac
builds; 0x004F6000 GetIsInUseRange behavior is independently corroborated by
the playable-source reconstruction. Next: related movement/collision and
creature helper candidates; whole-folder validation after the batch.

Additional s8 attempts, all retained NONMATCH: 0x0050FD50
AIActionCheckMoveAwayFromLocation (41 diffs, this/node register allocation
and x87 scheduling); 0x004F62A0 GetCollisionFlag (15 diffs, descriptive name,
return-block placement/registers); 0x0051B7B0 UpdateSubareasOnMoveTo (7
diffs, list construction/cleanup stores and stack scheduling). One alternate
collision return shape worsened its result; one alternate subarea lifetime
shape worsened its result, so both were reverted. The movement helper at
0x004BDF80 returns a Vector via its hidden output pointer and takes the two
Vector references, float distance and path information; the subarea methods
at 0x004BF2C0/0x004BF470 have reference signatures supported by both Mac
builds. These attempts are not claimed as exact matches.

Final owned-folder comparison: 525/634 -> 526/638, one new exact and three
new NONMATCH. No prior MATCH regressions, no previous NONMATCH deterioration,
and no duplicate annotations.

## Worker A continuation, 2026-10-26

Screened 134 indexed candidates and 22 additional starts found in source
references inside coarse address spans. Three newly reconstructed functions:
0x004EADD0 CSWSActionRecord constructor (96/96), 0x004D8A10
OnRemoveSummonCreature (48-byte body; coarse extent spans following code), and
0x004DA870 OnRemoveSetStateInternal (160/160); all compare MATCH. The state
handler uses native switch cases and a +0x260 creature script reference.
Retained NONMATCH: 0x004D8D80 OnApplyACIncrease (80 diffs) and 0x004D8F80
OnApplyACDecrease (72 diffs), early-return placement and register allocation.
Their AC byte layout follows the existing matched OnRemoveAC handlers.
- Promoted 0x004EE150 CSWSCreature::UnDisguise from 4 differences to exact
  (192/192). Naming `m_pFollowInfo` in a local before ComputeSafeLocation
  produces the original ECX/EDX argument order without changing semantics.
  Corrected the related 0x004DCAC0 caller's provisional void alias to the
  established `int UnDisguise(CGameEffect*)`; the caller remains NONMATCH 2.
