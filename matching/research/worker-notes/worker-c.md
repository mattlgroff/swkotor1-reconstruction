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

## Worker C s1 (branch claude/worker-c-s1), 2026-10-26

Family: CClientExoAppInternal client-app lifecycle (load screen, music,
movies, pause, death, rumble) plus its small template/destructor helpers.
Folder canonical check: **767/859 -> 799/898**; no prior MATCH regressed.

- 39 NEW attempts, **32 NEW exact**, 7 retained NONMATCH, 0 promotions.
- New exact: 005F1020 (CExoImeUI dtor), 005F1B20, 005F2980, 005F3480,
  005F3650, 005F3810, 005F3F10, 005F4760, 005F4B50, 005F4C60, 005F5020,
  005F50F0, 005F51C0, 005F6480, 005F6960, 005F6C60, 005F7500, 005F7710,
  005F7C10, 005F7D00, 005F7FA0, 005F8290, 005F9AF0, 005FB3F0, 005FBBF0,
  005FCA30, 005FCB50, 00602650, 00602AF0, 00602C10, 00602C90, 00602DC0.
- NONMATCH (diff insns): 005FB540 GetGUIString (14, ebp/edi bounds swap),
  005F7B20 UpdateLoadingScreenHint (9, elapsed sum register), 005F5CF0
  AttachPlayerGameObjectToArea (5, area-scene arg in ecx), 005F7200
  RunDeathSequence (12, known creature-lookup tie), 005F6F20 WaitForMovie
  (35, bStopped kept in a stack slot, no ebp), 005F7960
  ChangeCharacterToNextLivingPartyMember (56, register assignment / slot
  coalescing of uninitialised pPC), 005F7330 UpdateDeathSequence (31, x87).
- Files: src/c/client_exo_app_internal_s1.cpp (most), client_gui_string_list.cpp
  (CGuiStringStruct list + GetGUIString), exo_array_list_add_unique.cpp,
  sw_c_sorted_journal_dtor.cpp, sw_c_journal_dtor.cpp, exo_ime_ui_dtor.cpp;
  ResetGuis/UnloadModule/SetAutoPaused/AttachPlayer went into
  client_exo_app_internal.cpp (they need the inlined GetPausedByCombat,
  SetPausedByCombat and GetPlayerCreature bodies from that TU); RunDeathSequence
  into _r5.cpp (inline LookUpCreature).

Lessons (all verified on this batch):
- Implicit conversions matter: pass `"MusicResRef"` / a CExoString to a
  `const CExoString&` / `const CResRef&` parameter instead of writing
  `CExoString("...")` / `CResRef(s)`. Explicit temporaries use the ctor's eax
  return; implicit ones are addressed with lea and reuse temp slots. Fixed
  GetLoadMusicByModuleName, SetLoadScreenByModuleName, StartLoadMusic,
  WaitForMovie's resrefs, AddMovieToExoArrayList (`new CExoFile(s, t, "rb")`
  produces the original's conditional-destruction flag bits).
- Inline getters force eax chains: `GetPC()->GetId()` (new CGameObject::GetId),
  `GetLoadScreen()`, `g_pRules->Get2DArrays()->GetX()`, `GetModule()->GetArea()`
  + `pArea->GetAreaScene()` turned 18/5/24/39-insn misses into exact matches.
- Where a local is initialised changes store scheduling: AddMovieToExoArrayList
  needed `int bFound = 0;` after the first Format, not at the top.
- `if (!(a > b))` reproduces `test ah,41h / je`; `a <= b` gives `jp`.
- A dtor whose body nulls a member before the base list dtor matched only as
  a member subobject with its own inline dtor (CSWCSortScratch).
- `#pragma inline_depth(0)` keeps template calls out of line, but it applies
  to every instantiation in the TU: give such a template its own file.
- Header facts: CSWCModulePlayer (module +0x48) is really the client area
  (CSWCArea: +0x184 scene, +0x1dc ambient sound, 0x605C40/0x609410);
  CSWCModule has a virtual deleting dtor at slot 0; the +0x14c "save/load"
  object is CSWCMessage; +0x2a0 is CSWPlayerControlCamRelative; 0x005F1B20
  is a thiscall member (StartLoadMusic calls it with ecx=this).

Next in-range queue (screened, not attempted): 005F3070 GetCanSendDialog
(624 B, server dialog objects), 005F6640 GetObjectName (800 B, per-type name
lookup), 005F4460 PlayerFlourishWeapons (mixes inline and out-of-line
GetPlayerCreature), 005FB6E0 CrownFutureLeader (inline CExoArrayList<ulong>
copy), 005F5910 setUpEventPlayBack, 005F1830 SetVideoMode, 005F14A0
SetTexturePack, 005F0AD0 LoadItemDropSoundList, 005FB960 rumble cut-off,
006027C0 AddFloatyText, 005F1090 (unknown 0x34-byte class dtor: strings at
+0xC/+0x30, empty-dtor member at +0x14), 005F4FB0 (12-byte POD list find
with callback).

## Worker C s3 (branch claude/worker-c-s3), 2026-10-26

Folder canonical check: **803/902 -> 868/970**; no prior MATCH regressed, no
address annotated twice. 68 NEW attempts, **65 NEW exact**, 3 retained
NONMATCH, 0 promotions.

Main family: the client cheat console in nwcarea.o (0x0060A300-0x0060B020),
35 functions, all exact (sw_c_area_cheats.cpp). Only 0x0060A300/0060A480/
0060A590/0060ABD0 are in functions.csv; the other 31 are real starts missing
from it (per-skill/per-ability entry points, XP/gold/alignment/items/warp,
option toggles). Names are descriptive (Cheat*); GetCheatsTurnedOn is the
Mac name. Plus CSWCCreature action-menu builders and small leaves.

- New exact: 0060A300, 0060A460, 0060A480, 0060A510-0060A580 (8 skill stubs),
  0060A590, 0060A690, 0060A6A0, 0060A730, 0060A7D0, 0060A860, 0060A8F0,
  0060A980, 0060A9E0, 0060AA20, 0060AA40, 0060AA80, 0060AAC0, 0060AB30,
  0060ABB0, 0060ABD0, 0060AC50, 0060ACD0, 0060AD60, 0060ADF0, 0060AE80,
  0060AF10, 0060AF50, 0060B020 (cheats); 0061F760, 0061F790, 0061F7E0,
  0061F830, 0061FA00 (ClearInputState), 0061FAA0, 0061FB50 (input leaves);
  005F1090, 005F4FB0, 005F5180 (client app leaves); 00616940, 006169A0
  (ArrayList Insert <ulong>/<ushort>); 0060CA80, 0060CBD0, 0060CD50,
  0060CED0 (equipped-item / last-instant slot accessors); 00616FA0,
  00616FB0, 00616FD0, 00616FE0, 00616FF0, 00617030, 00617070, 00617090,
  006170C0 (object/creature virtual leaves, 8 missing from functions.csv);
  00619DB0 GetPersonalActions, 00619C20 GetTargetActions, 00619B10,
  00619950 GetCombatFeatsSkills; 00604C90 ApplyFireAndForgetVisualEffect.
- NONMATCH: 00610490 GetFreeLookVideoEffect (17, row in si/table load
  order), 006196A0 AddHostileItem (9, base/value load order), 00618A30
  AddFeatAction (14, register choice for app/feat loads).

Lessons (verified this batch):
- CSWGuiInterfaceAction's callback at +0x0C is a pointer to member of a
  class declared incomplete (16-byte general PMF representation, align 8):
  wrap the struct in `#pragma pack(push, 4)`. Assigning
  `&CSWCCreature::Method` then emits the {addr,0,0,0} stores; passing such a
  PMF by value (AddFeatAction) also produces the `and esp, -8` frame.
- Clamps: the alignment cheats re-test like a double-evaluating macro:
  `MAX(MIN(n, 100), -100)` with MIN(a,b) ((a)>(b)?(b):(a)), MAX(a,b)
  ((a)<(b)?(b):(a)). The gold cheat uses stdlib `__max(n, 99)`.
- strcpy after an optional `*strchr(s, ' ') = 0` is duplicated in both
  branches of the `if` in the original source.
- A loop counter declared in the `for` (not with the count at the top) keeps
  the post-clamp re-test of the count (no jump threading).
- Toggle `x = !x` through two getter calls matched only via a local
  (`int b = get()->x; get()->x = !b;`).
- Inline getters again decided load order: GetClientExoApp(),
  GetServerExoApp(), CSWCItem::GetBaseItem(), item GetBaseItemId/StackSize.
  But the same `g_pAppManager->m_pClientExoApp` chain in AddFeatAction
  wants the direct member (edx then ecx).
- Feat/arg values computed into a local before the call
  (`nFeat = CheckFeat(...); AddFeatAction(nFeat, ...)`) keep the original
  call order; a local id before GetReputation fixed GetTargetActions.
- 0x0061FAA0's switch must be on (int) with an explicit `case -1`, and case
  order decides the jump-table block order.
- `CExoString` returned by value needs the declared destructor to get the
  original's construction-flag store (CSWCCreature::GetName).

Unresolved real starts seen while screening (not attempted): 0x0061FA90
(cdecl int equality helper after ClearInputState).

Next in-range queue (screened, not attempted): 006191F0
GetHostileForcePowers (432 B, like the 616230 NONMATCH), 00605510 GetRoom
(CExoString compare; original has an 8-aligned frame, cause unknown),
00605E50 RestartMiniGame (CSWCMiniGame track/obstacle rebuild), 0060E2F0
ApplyShadowBlob (appearance 2DA + shadow model by size), 0060BDF0 (x87
distance step), 0060D600 (portrait/name string build), 005EC7F0
CExoLocStringInternal::Compare, 0060F1E0 scene hit check, then the
remaining CSWCArea members (604DB0 SetWeather, 605090, 605C40, 606090 ctor,
60A0F0 dtor).

## Worker C s5 (branch claude/worker-c-s5), 2026-10-26

Folder canonical check (`kotor-match -q -j 2 matching/src/c`): **909/1014 ->
926/1039**. No prior MATCH regressed, no address annotated twice, every prior
NONMATCH kept its diff-insn count.

- 25 NEW attempts, **15 NEW exact**, 10 retained NONMATCH, **2 prior NONMATCH
  promotions** (0060FB80 SetOverrideLightsaberState, 006116A0 BlurWeapon).
- New exact: 005EB6B0 CExoAliasListInternal::ResolveFileName, 005EC2B0
  CExoDebugInternal::Warning, 005ED0E0 CExoRandInternal ctor, 005F0AD0
  LoadItemDropSoundList, 005F5910 setUpEventPlayBack, 005F5AB0
  ReadAndSetVideoMode, 005F0CE0 ReadVideoModeSettings, 005F14A0
  SetTexturePack, 006027C0 AddFloatyText, 00605E50 CSWCArea::RestartMiniGame,
  00605C40 CSWCArea::HiliteAreaObjects (descriptive), 0060D600
  SetFirstAndLastName (descriptive), 0060C540 GetSaveCreatureFilename,
  0060F1E0 SceneHitCheck (free cdecl, playable name), 00613BA0
  EquipToInventorySlot.
- NONMATCH (diff insns): 005EBA40/005EBC60 CExoFileInternal::Read/Write (16
  each, post-loop jump threading), 005EBF30 CExoAliasListInternal::Add (21,
  loop rotation + zero register), 005ECEA0 CExoRandInternal::GetString (48,
  length kept in edx/spill), 005EC7F0 CExoLocStringInternal::Compare (16),
  0060E2F0 ApplyShadowBlob (5 real + jump table), 005F1830 SetVideoMode (79),
  00604DB0 CSWCArea::SetWeather (33, register naming only), 005F3070
  GetCanSendDialog (4 real: vtable eax/edx), 0060DC50 PlaySwingSound (32,
  ebp/edi swap).
- Files: new exo_file_internal_s5, exo_alias_list_internal_s5,
  exo_rand_internal_s5, exo_loc_string_internal_s5, exo_debug_internal_s5,
  client_exo_app_internal_s5, sw_c_area_s5, sw_c_creature_s5,
  sw_c_scene_hit_s5; ReadVideoModeSettings went into
  client_exo_app_valid_resolution.cpp, EquipToInventorySlot and the two
  promotions into sw_c_creature_s3.cpp, ApplyShadowBlob into
  sw_c_creature_r6.cpp.
- Header facts: client app +0x88 is the capture/playback CExoFile* (renamed
  m_pCaptureFile, forwarder GetCaptureFile), +0x98 a CExoLinkedList (playback
  events), +0x1a0 m_nVideoMode, +0x1a4 m_nTexturePack; client options +0x14
  bit 0x10 enables floaty text; CGameObject slot 0x10 is AsSWSObject;
  SetVideoMode is (ulong, uchar) returning int, SetTexturePack (uchar, int)
  returning int, setUpEventPlayBack returns void. 0x61FBA0 (event playback
  file parser, declared LoadPlayBackEvents) is not attempted.

Lessons (verified this batch):
- **Same-TU callee register knowledge.** The original compiler skips
  `mov ecx, esi` (or keeps edx live) across calls to functions compiled
  earlier in the same object file. Put the caller in the file that defines
  the callee, after it: ReadVideoModeSettings (40 diffs -> exact) after
  IsValidResolution; EquipToInventorySlot, SetOverrideLightsaberState and
  BlurWeapon after GetEquippedItemID/SetEquippedItemID/GetLastInstant in
  sw_c_creature_s3.cpp. Look for calls without an ecx reload in the
  original before blaming register allocation.
- Inline getters again: `g_pAppManager->GetClientExoApp()`/
  `GetServerExoApp()`, `g_pRules->Get2DArrays()->GetX()`, an inline
  `GetPlayerId()` fixed load order in setUpEventPlayBack, HiliteAreaObjects,
  GetCanSendDialog and the lightsaber promotions.
- Implicit conversions: `sText = nValue;` (CExoString(int) temp addressed by
  lea) matched AddFloatyText where `CExoString(nValue)` did not; passing a
  CExoString to a `const CResRef&` parameter reuses a dead temp slot.
- Two calls differing only in a constant arg (`if (b) f(x, 2); else
  f(x, 1);`) give the original push/jmp/push tail-merge; `b ? 2 : 1` gives
  setne/inc.
- A switch whose cases share code must keep separate (identical) case
  bodies to avoid the compiler's range check (SetWeather lightning).
- Out-of-line function with a cached copy of a member pointer (e.g.
  `pStats = m_pStats` used twice) and `unsigned long oid = m_idSelf;` before
  a call fixed register choice (SetFirstAndLastName).
- MSVC loop-head alignment and jump threading differ for retry loops; both
  Read/Write are at 16 diffs after six shapes.

Next in-range queue (screened, not attempted): 005F6640 GetObjectName (object
type constants, STR_RES inline + out-of-line ctor), 005FB960
LookUpAndPerformRumbleWithCutOff (bool-materialized ?: lookups, x87
distance), 005FB6E0 CrownFutureLeader (inline CExoArrayList copies),
006134C0 SetAppearance, 005F97E0 AddCreatureModelToIgnoreList (same `or bl,
0xff` tie as 005F99F0), 0060A0F0 CSWCArea dtor (member list known: 0x11c,
0x17c CExoString, 0x1c4, 0x1d0, 0x204 linked list, 0x20c/0x218/0x224
arrays, 0x238/0x244 arrays with null-check dtor, 0x280 linked list),
0061FBA0 LoadPlayBackEvents (sscanf parser). Consider moving more prior
creature NONMATCHes next to their earlier-address callees.

## Worker C s6 recovery, 2026-10-26

Recovered Claude's interrupted CSWCArea construction/destruction/cleanup
family from claude/worker-c-s6, including the unsaved UnloadArea work.
Screened and attempted 3 functions: 0x00606090 CSWCArea constructor and
0x0060A0F0 destructor MATCH; 0x00609410 UnloadArea NONMATCH (19 instruction
diffs, two DelIndex register/order ties). No prior annotations intentionally
changed. File comparison used the licensed executable and MSVC 7.0.

The missing CGameObject AsSWCCreature/AsSWCPlaceable virtual slots are +0x2c
and +0x4c, consistent with the native calls. Declared CSWCCreature as a
CClientObject for the AI list call. Introducing a named CClientAIMaster
local before RemoveObject cut cleanup differences from 36 to 19. The +0x250
allocation is explicitly deallocated via operator delete, reflecting its
native call to 0x006FA390 without inventing the allocation's element type.

At 0x00606382 the native constructor passes the CSWCArea `this` pointer to
0x0068DF40 CSWCAmbientSound's constructor. This file declares the pointer
signature; the preexisting K source declares an unsigned-long area ID, which
needs a separate K-owned correction. CWorldTimer::GetWorldTime is 0x004ADE40.

Folder comparison: 926/1039 -> 928/1042, no prior MATCH regressions,
no prior NONMATCH deterioration, and no duplicate annotations.

## Worker C s7 (branch claude/worker-c-s7), 2026-10-27

Folder canonical check (`kotor-match -q -j 2 matching/src/c`): **928/1042 ->
940/1059**. No prior MATCH regressed, no prior NONMATCH got worse, no address
annotated twice (checked against main's status.csv with a scratch report).

- 17 NEW attempts, **10 NEW exact**, 7 retained NONMATCH, **2 prior NONMATCH
  promotions** (00614B60 CSWCCreature::IsInjured, 0060B8A0 GetSoundSetStrres).
- New exact: 0060B400 HitGroundEvent (cdecl model-event callback), 0061A980
  AnimationUpdate, 0061A2D0 PlayFootstep, 006142F0 SetAttackAnimationData (in
  sw_c_creature_s3.cpp after GetEquippedItemID), 0060BB20 HandleConjuring
  (descriptive), 006170D0 ~CSWCCreature (new sw_c_creature_dtor_s7.cpp),
  0061A670 AnimateFireAndForget (descriptive), 00615E80 FollowAnimationUpdate,
  0060C1E0 RequestModel (new sw_c_creature_model_s7.cpp) and 0060C4A0
  AdvanceWayPoint (descriptive, sw_c_creature_vslots_s4.cpp).
- **Missing from functions.csv** (annotated, not edited): 0x0060C1E0
  CSWCCreature::RequestModel (vtable 0x158; mac_names wrongly puts that name on
  0x0060BF60, which is the movement-scale x87 helper) and 0x0060C4A0 (vtable
  0x15c). Annotating 0x0060C4A0 is what gives RequestModel its correct extent.
- NONMATCH (diff insns): 006134C0 SetAppearance (19, the `!(body||head)` path
  is jump-threaded by us, not by the original), 006146E0 LookAt (51,
  this/target esi/edi swap), 006191F0 GetHostileForcePowers (40, this/target
  ebx/edi swap), 0060BDF0 MoveCreatureToPosition (32, snap-block placement and
  one stack slot; interpolation is identical), 006193A0 CreateUsableItemEntry
  (16, same SetSize/name-store register rotation as AddHostileItem), 00616310
  LipSync (13, stack-slot rotation of pTimes/pShapes/fLength), 005F6640
  CClientExoAppInternal::GetObjectName (105, block layout + frame size; new
  client_exo_app_internal_s7.cpp).
- Files: new sw_c_creature_s7.cpp (shared TU-local layout for most of the
  batch), sw_c_creature_dtor_s7.cpp, sw_c_creature_model_s7.cpp,
  client_exo_app_internal_s7.cpp; additions to sw_c_creature_s3.cpp,
  sw_c_creature_action_menu.cpp (CSWGuiInterfaceAction +0x30 is now a bitfield:
  bit 0 enabled, bits 1-4 disabled reason), sw_c_creature_vslots_s4.cpp,
  sw_c_creature_menu_leafs.cpp, sw_c_creature_r6.cpp.

Lessons (verified this batch):
- **Separate early returns stay in line.** `if (a) return 0; if (b) return 0;`
  keeps the `return 0` block right after the tests (AdvanceWayPoint, and it
  promoted GetSoundSetStrres); `if (a || b) return 0;` moves it to the end.
  Conversely a nested `if (ok) { ...; return x; } cleanup; return 0;` puts the
  failure block last (LipSync's `!LoadLip` path).
- **Inline budget.** MSVC 7.0 stopped expanding the inline
  `GetServerCreature()` (ternary on GetServerObject) at its third use in
  FollowAnimationUpdate and emitted an out-of-line call; `__forceinline` on the
  TU-local helper restores the original. STR_RES's inline ctor shows the same
  effect in the original itself (expanded once, out of line at 0x00414DC0 in
  GetObjectName).
- `CResRef` built from `CStr()` straight in the call (implicit conversion of a
  `const CResRef&` parameter) is constructed before the other args and reuses
  a dead temp slot; a named CResRef local needs its own slot (HitGroundEvent,
  PlayFootstep). Implicit `const char*` -> `const CExoString&` arguments are
  addressed with `lea` again at the push.
- x87: a float threshold kept on the FPU stack matched only as
  `f = member; f *= g_global;` (the one-line product put the global first);
  `!(a > b)`, `!(a >= b)`, `!(a <= b)` select the `test ah,5 jp` / `test ah,1
  je` / `test ah,0x41 jnp` encodings; three-stage Vector math through named
  Vector temporaries (diff, scaled, sum) reproduces the store-heavy sequence.
- `int x = m_member;` before a call that also loads globals keeps the
  original's "argument first" order (IsInjured/IsPartyMember calls), and
  `bConjuring = 1;` before `pSpell = GetSpell(nSpell)` with the id in a local.
- Overloaded virtuals: declare `SetOrientation(Vector*)` before
  `SetOrientation(Quaternion*)` to get Vector at 0x64 and Quaternion at 0x60
  (MSVC groups overloads in reverse).
- Classes with virtual bases (CSWCAnimBaseHead/Wield/HeadWield) get the hidden
  `push 1` most-derived flag and the vbtable adjust on conversion for free.
- Destructor: virtual-deleting `delete p; p = 0;` without an `if` stores the
  null even when p was null (sound-source loop), with `if (p) {...}` only then.

Next in-range queue (screened, not attempted): 0060F7C0 CGPauseCycle (rand
idle-animation timer, 864 B), 006100F0 Setup3DScene (string compares on
model names), 00617AD0 (line-of-sight with the static CHitInfo), 00617470
PlayHitSound (1360 B, many 2DA lookups), 00619E10 footstep dust (Vector
temporaries + x87), 0060BF60 movement scale (x87), 0060FF30 GetMenuPoint
(quaternion maths), 00618C20 GetEquipmentForcePowers, 005F94D0/005F4460/
005F4180 client app internals. Retry SetAppearance/LookAt/LipSync only with a
new idea; GetObjectName needs the list-first layout (try a goto-free form that
keeps esi = pObject and no ebx).

## Worker C s9 (branch claude/worker-c-s9), 2026-10-27

Folder canonical check (`kotor-match -q -j 2 matching/src/c`): **950/1075 ->
952/1082**. No prior MATCH regressed, no prior NONMATCH got worse, no address
annotated twice (checked against main's status.csv).

The in-range todo list is down to 43 functions, all 528 B or larger (most
1-8 KB with EH and x87), so this round is a small CClientExoAppInternal /
input / area batch rather than a 20-40 function family.

- 7 NEW attempts, **1 NEW exact**, 6 retained NONMATCH, **1 prior NONMATCH
  promotion** (005F99F0 AddItemModelDirectlyToIgnoreList, moved from
  client_exo_app_internal.cpp into the new client_exo_app_internal_ignore_s9.cpp).
- New exact: 005F97E0 CClientExoAppInternal::AddCreatureModelToIgnoreList.
- NONMATCH (diff insns): 005F4460 PlayerFlourishWeapons (10 reported, 2 real:
  `mov ecx, esi` / `push eax` order of the final +0x44 call), 005F5370
  UpdateSoundEngine (49, inline-budget tie on operator*), 005FB050
  SelectNearestObject (105, register permutation plus a zero kept in edx),
  005F3AD0 (119, descriptive UpdatePartyCombatTargets; register assignment and
  a 0x20 vs 0x18 frame), 00606E30 CSWCArea::AddPlaceableObjectLight (137, the
  original has an `and esp, -8` frame), 005E12A0 CExoInputInternal::BufferEvent
  (MSG*) (88, final shared call gets eax/ebx instead of edx/ecx).
- Not source: 0x005E50C0 is the DirectInput8Create import thunk
  (`jmp [0x73D000]`); it needs no source.
- Header: matching/include/c/exo_input.h names CExoInputInternal +0x37c/+0x380
  (m_nMouseX/m_nMouseY, layout unchanged) and declares BufferEvent(tagMSG*).
- Files: new client_exo_app_internal_s9.cpp (PlayerFlourishWeapons,
  UpdateSoundEngine), client_exo_app_internal_select_s9.cpp,
  client_exo_app_internal_combat_s9.cpp, client_exo_app_internal_ignore_s9.cpp,
  sw_c_area_light_s9.cpp, exo_input_internal_s9.cpp.

Lessons (verified this batch):
- **CSWCObject/CGob::FindPart takes the part number as `unsigned char`.** A
  byte loop `for (unsigned char n = 0xff; n >= 0xfb; n--) FindPart(n, 1)`
  gives the original's counter in the dead `oid` argument slot and its
  `or bl, 0xff` form with no casts (this promoted 005F99F0).
- A TU-local `CExoArrayList::AddInline` must be `__forceinline` when a function
  has several adds; plain in-class inline was emitted out of line.
- Inline budget, again: in UpdateSoundEngine the original inlines
  `operator*(const Vector&, float)` in the first branch and calls the
  out-of-line COMDAT copy (0x00423BF0) in the next two. Declaring it inline
  matches the first branch but inlines the second too.
- Inline `GetClientExoApp()` accessor (instead of `->m_pClientExoApp`) puts
  the app load before the pushed argument for the cached player-creature
  pattern (PlayerFlourishWeapons 32 -> 10).
- Win32 message handlers: `GET_X_LPARAM` / `LOWORD` give the original's
  movsx/movzx word loads; a `__forceinline` helper that reads x and y into
  locals inside the `if` reproduces each case body, and the switch cases must
  follow the original block order (LDOWN, LDBLCLK, LUP, RDOWN, RDBLCLK, RUP,
  WHEEL, MDOWN, MUP) for the jump table to line up.
- A pointer to the array entry for check-then-assign
  (`CSelectableObject& e = element[i]; if (e.x == 0xff) e.x = ...;`) plus a
  re-read of `element[i].x` matches the original's cached `lea reg, [..]`.
- Unsolved: `and esp, -8` frames (AddPlaceableObjectLight): neither POD
  vectors nor a dead double trigger it.

Next in-range queue (screened, not attempted): 0061FBA0 LoadPlayBackEvents
(sscanf parser, 1344 B, no EH/x87), 005F9C60 SetSelectedObject-like caller of
SelectNearestObject (2368 B), 005FA5A0, 00618C20 GetEquipmentForcePowers
(EH, CExoArrayList<short> inserts), 00616A20 CSWCCreature constructor (EH,
needs the full creature layout), 0060E7D0 EvaluateNewWayPoints, 005E8CF0
CExoBaseInternal::GetDirectoryList. Retry SelectNearestObject/005F3AD0 only
with a register-allocation idea.
