# Worker C matching notes

Range: 0x005E0000–0x0061FFFF. Earlier compiler lessons are in Kotor RE's
`docs/batches/worker-c-notes.md`; current source annotations and reports
take precedence over its historical queue.

## Current checkpoint, 2026-10-26

- Prior accepted family: three client-app helpers, PerformFeat/PerformSkill
  and HitClash/Contact/Parry: eight new exact matches.
- In-flight family: SwingShort/Long/Twirl at 0x00610C90/0x00610D10/0x00610D90
  are three new exact matches. The callback source rechecks six exact
  functions, but three of those are prior work.
- Typed callback payload has the owner ID at +4. Use the proven ushort
  SendPlayerToServerInput_Attack overload; do not widen PerformSkill solely
  to chase a dword stack load.
- Retained attempts: 0x00610950 SetLockOrientation (4 diff instructions),
  0x00610A10 SetAttackTarget (9), and 0x00602D40 PlayMovieQueue (2).
  The first has a server virtual slot whose name remains unresolved. The
  second has a real SetAttackTarget/SetCombatMode naming conflict. The movie
  miss is post-call store/pop scheduling. Do not retry these immediately.
- Preserve the shared CSWCCreature base size 0x3E0. Its +0x2F4 field fits
  existing padding; the +0x440 target-state field belongs only in that
  translation unit. Extending the common base shifted CSWCCreatureAnim and
  regressed 0x005F33F0; the isolated layout restores its exact result.

## Next family

Screen at least 100 currently unclaimed range candidates. Select a complete
related creature/app family and continue its compatible leaves rather than
stopping at a few methods. Reuse the common headers cautiously. Candidates
around 0x00610550/0x00610590, 0x006109B0/0x00610A70 and
0x006145D0/0x00614630 require binary/name verification first. Screened
addresses are not implementations or exact matches.

## Isolated worker C r1, active 2026-10-26

- Prefetched 180 of 208 currently eligible range candidates with todo and Mac
  name hints. Selected creature animation/appearance/state family, preserving
  shared creature base size by using a translation-unit-local layout.
- First checkpoint: six new attempts, all six exact: 0x00610550 callback
  deregistration, 0x00610590 drive acceleration, 0x006109B0 server +AB4 rate,
  0x00610A70 draw animation, 0x006145D0 visibility cleanup, 0x00614630 equipped
  item hiding. Unknown names are explicitly descriptive. The server-rate
  fallback constant is 37.0f, verified from the original image.
- Existing Wine IPC requires the escalated matcher on this machine.

- Second checkpoint: seven more attempts; five new exact: 0x00614A30
  LoadModelA, 0x0060E6C0 LoadModelB, 0x00617A70 DeleteSpellLikeAbilityList,
  0x0060EF50 SetPartyMember, 0x0060D540 appearance-property forwarding.
  Total now 13 attempted / 11 new exact / 2 retained NONMATCH.
- 0x0060EF50 return audit corrected the initial claim: EAX=1 and helper
  forwarding do not prove an int public ABI. All nine direct callers ignore
  EAX. Preserve the established void SetPartyMember/SetPlayerCreature
  declarations and retain NONMATCH; the earlier 19-exact checkpoint count
  is superseded by 18 before the sound additions.
- Deferred after three shapes: 0x0060D320 InCombat (12 instructions of
  load/register scheduling). Deferred after two: 0x00617A20 list allocation
  (original retains EH around inlined zero constructor, ours removes it).
- Local list deletion retains the original clear-then-destruct sequence,
  including the destructor freeing a null element pointer. Folder shared
  partial classes were not modified.
- Third checkpoint: eight more new attempts, all exact: 0x0060ED80 projectile
  ID storage, 0x0060ED90 area-ID update, 0x0060EFE0/0060EFF0/0060F000 camera
  forwarding, 0x0060D560/0060D5C0 portrait mutation, 0x0060D070 hand-item lookup.
  Total 21 attempted / 19 NEW exact / 2 retained NONMATCH. Several compact
  entries are real binary starts missing from functions.csv; checked against
  all current annotations before implementation. Continue sound-related
  creature methods rather than treating 20 as a cap.

- Fourth checkpoint: five more attempts in creature sound/party state.
  0x006107C0 rolling playback, 0x00610840 rolling update, 0x0060B7C0 sound-set
  replacement are new exact. Playback needs explicit if/else calls to keep
  float materialization on the outgoing stack; a ternary materializes into
  the incoming run argument. Reversed ushort comparison matches SetSoundSet.
- Retained NONMATCH: 0x006105B0 rolling sound load (47 instructions; table-load
  scheduling, cold return placement and CResRef argument materialization),
  0x0060B750 party-status constructor (5; zero/EH-state register selection).
  Three constructor shapes and two sound-load shapes are enough for this pass.
- Corrected cumulative total: 26 new attempts / 21 NEW exact / 5 NONMATCH.
  No prior NONMATCH promotions have been claimed.
- Fifth checkpoint: five more new attempts, all exact: 0x0060F150 queued
  animation callback, 0x0060E490/0060E520 equipped visual-state forwarding,
  0x0060B630 combat attack record constructor, 0x0060B6B0 memberwise copy.
  Total 31 attempted / 26 NEW exact / 5 retained NONMATCH.
- Canonical callback period is 9999.0f (0x461C3C00), despite the playable
  source's incorrect 10000 comment. Vector default initialization must use
  three statements in x/y/z order, not chained assignment (z/y/x stores).
  Combat copy is a descriptive CopyFrom method: original final EAX is the
  source +40 field, proving it is not a destination-returning constructor.

## Worker C r1 handoff (frozen after final folder audit)

- Broad current pool: 180 prefetched catalog entries out of 208 eligible.
  Closer family review: 37 entries; 36 implemented/attempted. 0x006142F0 was
  read as a possible extension but belongs to the larger impact-node/equipped
  appearance update family and was not counted as an attempt.
- Final batch: **36 NEW attempts, 27 NEW exact, 9 retained NONMATCH; zero
  prior NONMATCH promotions**. No count includes pre-existing exact functions.
- New exact addresses: 0060B630, 0060B6B0, 0060B7C0, 0060D070, 0060D540,
  0060D560, 0060D5C0, 0060E490, 0060E520, 0060E6C0, 0060ED80, 0060ED90,
  0060EFE0, 0060EFF0, 0060F000, 0060F150, 00610550, 00610590, 006107C0,
  00610840, 006109B0, 00610A70, 00610BC0, 006145D0, 00614630, 00614A30,
  00617A70 (all prefixed 0x).
- Retained new NONMATCH and current diff-insn counts: 0060B750 (5),
  0060D140 (29), 0060D320 (12), 0060EF50 (7), 006105B0 (47),
  00611470 (29), 00613710 (62), 00614110 (110), 00617A20 (9).
  Reaction/ranged selectors have cleanup/dispatch block placement differences;
  weapon-class update has byte-local register/stack and branch placement
  differences; voice playback has table loads and argument scheduling ties.
  Best defensible attempts are retained after two or three natural shapes.
- Final full owned-folder canonical check: **767/859 MATCH**, up from the
  checkpoint report's **740/823**. No former MATCH regressed; every former
  NONMATCH retained its prior diff-insn count. Command used canonical
  `-O2 -Gy -GX`, `-q -j 4`, original executable and licensed MSVC7 paths.
  Temporary diagnostics: `/tmp/worker-c-r1-status.csv` and
  `/tmp/worker-c-r1-folder.log`. Shared reports were never changed.
- Layout audit: original STR_RES includes float sound length at +18 (size
  1C); the voice helper now uses this full established layout. Perception
  cleanup uses typed CPerceptionRecord pointers. The spell-like-ability list
  shares a 12-byte element declaration with its exact allocator; constructor
  writes are dword +0=0, dword +4=1, byte +8=0. The shared creature base size
  remains unchanged.
- Next coherent larger family: equipped appearance/impact visual refresh
  (0060CA80 GetEquippedItemID, 006134C0 SetAppearance, 006142F0
  SetAttackAnimationData), then full step/impact audio (0061A2D0 PlayFootstep,
  00617470 PlayHitSound). These need additional item/area/table contracts;
  do not reopen the deferred allocation or scheduling ties immediately.
