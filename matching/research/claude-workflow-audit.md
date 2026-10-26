# Claude matching workflow audit

Audited 2026-10-26 against the actual Kotor RE checkout, its worker notes,
Git commits and registered worktrees. The purpose is to reproduce that
matching process in this repository. No VS2022 source or build is involved.

## Evidence

- Kotor RE `docs/HANDOFF.md:101–122` records the working coordinator process:
  fresh agents, one worktree/branch per batch, local scope, checkpoints about
  every five functions, independent branch verification, integration and
  regenerated reports. It records up to twelve concurrent workers.
- `docs/MATCHING_WORKFLOW.md:11–39` gives the same a–n address buckets used
  here. Lines 73–105 specify real 2002-style class layouts, canonical
  comparisons, best NONMATCH retention and related class batches.
- The handoff's later pilot finding at lines 44–57 keeps the playable
  reconstruction as optional semantic context; it does not support making
  reference lookup the organizing bottleneck. Lines 124–140 preserve
  compiler lessons and recommend leaving known scheduling ties after two or
  three tries.
- Batch sizes were substantial and variable. `worker-d-notes.md:506–543`
  records 109 GUI-control annotations and 99 exact results in one batch.
  `worker-c-notes.md:666–684`, `723–750` and `792–823` record 58/45, 62/55
  and 40/33 attempted/exact sets. M history includes `6a83c3bf` with 41 new
  annotations/36 exact and round 4 (`d0ac6e76`) with 75 new/69 exact.
  `dca18cd7` records a 25/25 sender family.
- History also contains small WIP checkpoints, for example `222cc6d5` (3/3)
  and `611f9f92` (34/34). A checkpoint is not a reason to stop the family.
  `67c2c26f` refreshed reports after integrating six worker branches.
- New-repository `matching/CONTRIBUTING.md:63–88,123–165` retains range
  ownership, ordinary classes, canonical checks and coordinator reports.
- Root CLAUDE.md and AGENTS.md were compared byte-for-byte with the requested
  Kotor RE files and are unchanged by this audit. No complete historical
  Claude prompt transcript or scratch merge script is checked into Kotor RE;
  the audit does not claim access to either.

## Differences and corrections

| Area | What this run did differently | Correction |
| --- | --- | --- |
| Batch definition | Counted 1–3-function subclusters as batches; the last ten contained 23 new attempts and 20 exact matches. | Use a broad pool and complete related class families. Record actual attempts and new exact matches instead of a mini-batch count. Historical 20–109-function sets guide scale without imposing a fake quota. |
| Worker isolation | Three workers edited disjoint folders in one checkout. | Give each worker its own worktree and branch before the next implementation batch. Coordinator alone integrates and writes shared reports. |
| Worker context | Reused the same long-lived workers for multiple rounds. | Use fresh worker contexts per batch when slots are available, passing persistent range notes and a concrete family. |
| Checkpoints | Waited for a coordinator group commit. | Restore local worker checkpoints about every five functions; retain them if usage ends mid-batch. |
| Notes | Most new compiler lessons and queues lived in messages or private scratch files. | Maintain range notes under `matching/research/worker-notes/` and include them in worker checkpoints. |
| Candidate selection | Repeatedly picked a few named tiny functions and consulted the playable source first. | Reuse existing headers and binary contracts, screen a broad pool, and use the playable source where it clarifies semantics. Continue compatible methods in the same family. |
| Hard misses | Some known register/stack/x87 ties received more retries than the handoff's fast-deferral advice. | Stop those shapes after two or three unsuccessful retries; keep honest NONMATCH sources and the reason. |
| Source shape | A worker initially used literal global addresses/raw record arithmetic; review corrected these before publication. | Require extern globals, typed records, proper members and vtable slots from the first draft. Keep established contracts when return evidence is unresolved. |
| Shared layouts | An in-flight `CSWCCreature` extension shifted a derived layout and regressed an old match. | Restore the shared base size and isolate the extended declaration in its own translation unit. The affected old function rechecks exact; the full checker is rerun before acceptance. |
| Report dependency | One symbol-map run overlapped the full matcher and read the old status report. | Always finish `matchcmp --report` before starting symbolmap. The stale map was discarded and regenerated before the prior push. |
| Tool correctness | A verified COFF relocation-addend bug was fixed rather than preserving the old mapper verbatim. | Retain the correction and its synthetic regression tests: all 70 tool tests passed, and real symbol ambiguities fell from 113 to 55. This is a deliberate correctness fix, not a relaxation of matching. |
| Publication | Earlier published every ten mini-clusters under the user's then-current instruction. | The latest user instruction supersedes that cadence: at least 100 NEW exact matches since the previous push, published as one combined commit. Local checkpoints continue. |

## Necessary adaptations

The platform provides four concurrent agent slots including the coordinator,
so three workers can run at once. Claude's twelve-worker concurrency and the
requested three-by-three fan-out cannot be reproduced within that limit.
This is separate from batch size: each available worker can still process a
large related workset.

This repository uses `matching/tools/` and its own playable `src/` tree;
Kotor RE used `tools/matching/` and a reference submodule. Explicit target
paths replace stale wrappers. Worker branches use the current required
`codex/` prefix. The matching algorithm, flags, range boundaries and original
inputs remain the same.

The desktop managed-worktree tool has no repository argument, while this
chat is rooted in Kotor RE. To avoid creating a worktree in the wrong repo,
worker worktrees are created with Git's explicit target-repository path.
Only clean, integrated worker checkouts are removed; checkpoint branches
preserve recoverable work.

Historical worker pushes and immediate coordinator pushes are intentionally
replaced by the user's local-checkpoint/100-new-match rule. At the audit
checkpoint, the last-published baseline was `76d68ce`, with 5,462 exact
functions; that publication cycle requires at least 5,562 and no former
MATCH regressions. Later cycles use their own last-published report.

The first large isolated families completed after these changes were N
(58 new attempts, 52 exact) and M (36 attempts, 29 exact). Independent
coordinator folder checks confirmed both results before integration. The
workers kept their own local checkpoint histories and persistent notes.

The operating rules are in `matching/AGENTS.md`. The audit is a process
correction, not a claim that screened candidates or near misses are matches.
