# Claude interrupted-batch recovery, 2026-10-26

Baseline: local main d816b93, 7113 MATCH. Recovered the saved A-s5,
F-s6, K-s1, L-s6 and M-s6 worktree snapshots. Their original worktrees,
including uncommitted changes, remain untouched. Folder comparisons against
baseline respectively gained 7, 3, 19, 29 and 10 exact matches, with no
previous MATCH lost in those folders. These counts include promotions of
previous NONMATCH attempts. See status.csv for individual outcomes.

K relocates 0x00690940 and 0x00690960 into the visual-effect translation
unit. Review names and vtable evidence independently; relocation masking
alone does not establish symbol names. The NONMATCH attempts at 0x00691970
and 0x0068FDD0 contain uncertain control-flow or field interpretations and
must not be treated as exact behavior. The A NONMATCH at 0x004E4790 also
needs its failure path checked.

C's uncommitted area update did not compile. I's matrix routines use a
single Matrix type for distinct 3x3 and 4x4 layouts, and its circle helper
exposes an existing caller argument-order conflict. B's untracked file
contained placeholder functions and its recovered functions need callee and
vtable naming review. Those three saved batches were deferred for separate
recovery; no placeholder stubs were accepted.

The rebuilt symbol map has 12,941 entries and reports 10 conflicts (67
ambiguous names omitted). No shared symbol changed address. Three earlier
provisional names for the area visual-effect object disappeared because its
methods now use CSWCVisualEffect: the constructor stores vtable 0x00754278,
whose first slot in the original executable is the deleting destructor
0x00690960; LoadModel's original diagnostic names CSWCVisualEffect. Its
regular destructor is 0x0068FF70 and its Announce method is 0x00690940.

Subsequent recovery checkpoints: B repaired the real VM handlers and omitted
placeholder stubs; C restored the area lifecycle and kept UnloadArea as a
NONMATCH; I separated 3x3 Matrix from the explicit 4x4 output-buffer routine
and corrected the existing circle caller's native argument order. H recovered
17 camera and scene entry points missing from the coarse function index.

The original 0x00606090 constructor passes its CSWCArea `this` pointer to
CSWCAmbientSound at 0x0068DF40. The K declaration and stored member were
corrected from an unsigned object ID to CSWCArea* to match this native call.

At 7206 MATCH the symbol map had 12,977 entries, 67 ambiguous names omitted,
10 conflict lines, and no previously mapped symbol moved. Two old names were
replaced: CSWCAmbientSound's unsigned-long constructor alias now uses the
native area-pointer type, and GetIsActionTimed at 0x0057A240 was replaced by
GetAcceptableAction (Mac debug map 2/2). The existing area emitter stubs need
cleanup before these source snapshots are integrated into local main.

At 7242 MATCH, the rebuilt symbol map has 13,029 entries and 10 conflict
lines (67 ambiguous names omitted); no retained name moved address. Removed
names are superseded declarations for bark/streaming sound, creature movement,
combat mode and quick chat, as recorded in worker notes and bark-abi-recovery.
The duplicate area lifetime definitions were removed; their generated thunk
and scalar deleting destructor remain matched in the real constructor TU.

Final batch comparison: 7,249 MATCH / 8,124 annotated, with 875 NONMATCH;
no duplicate addresses, prior MATCH regressions, or worsening of prior
NONMATCH instruction counts relative to the previous full comparison. This is
959 new matches relative to the last pushed status (6,290). User requested
no push unless the unpushed total is over 1,100; this checkpoint remains local.
Worker H's 0x00456B60 near-miss has an unresolved global alias and remains on
its worker branch rather than in this accepted batch. Additional C near-miss
work also remains saved on the worker branch.
The final symbol map has 13,043 entries, 10 conflict lines and 67 ambiguous
names omitted. No name from the previous combined map was removed or moved.

Next batch comparison: 7,275 MATCH / 8,154 annotated, 879 NONMATCH. The
full report found no duplicate addresses, previous MATCH losses, or worsening
of prior NONMATCH differences. The remote baseline has 6,290 MATCH, so this
checkpoint contains 985 newly matched functions since that push. The user
explicitly authorized publishing this checkpoint below the usual threshold.
The symbol map contains 13,084 entries with 10 conflict lines and 67
ambiguous names omitted. No retained name moved. The removed
SendServerToPlayerItemScriptAdd alias was replaced by the native
CSWSArea-pointer argument signature supported by the Mac debug map.
