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
