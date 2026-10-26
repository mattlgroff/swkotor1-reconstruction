# Matching worker agreement

The root CLAUDE.md and AGENTS.md remain the exact copies requested by the
user. This file supplies their matching-specific paths and operating rules
for this repository. See research/claude-workflow-audit.md for the evidence.

## Scope and isolation

- Work only on the original-toolchain matching track. Never edit the VS2022
  sources, build configuration, or gameplay implementation.
- Each worker gets its own Git worktree and `codex/worker-<range>-r<N>`
  branch, using the exact address buckets in CONTRIBUTING.md. Only edit
  `matching/src/<range>/`, `matching/include/<range>/`, and
  `matching/research/worker-notes/worker-<range>.md`.
- Use a fresh worker context per completed batch when agent slots permit.
  The current platform permits four concurrent agents, including the
  coordinator: three workers and one coordinator. Do not claim nine workers.
- Set the correct checkout as the explicit working directory for every
  command. On this machine use non-login shells so commands cannot silently
  return to the old Kotor RE checkout. Keep proprietary inputs outside Git.

## Batch selection and implementation

- Read the range's prior notes and current annotations before selecting work.
  Prefetch a broad pool, normally at least 100 available addresses, then work
  complete related class families with shared layouts and callees. Expand the
  family when adjacent compatible methods remain. Do not turn each small
  helper or three-function subcluster into a separate batch.
- Historical Claude matching batches commonly covered 20–75 functions; a
  documented GUI-control batch covered 109. These are observed sizes, not
  quotas. Do not manufacture attempts, names, or matches to reach a number.
- Prefer existing matched headers and canonical binary evidence. The
  behavioral source is optional semantic context, especially for unnamed
  functions; it does not replace ABI, control-flow, or callee verification.
- Write ordinary C++98 classes, padding, members and virtual slots. Declare
  external callees and globals. Do not copy instruction sequences, use raw
  absolute-address global loads, or alter public signatures solely for bytes.
- Preserve shared base sizes when extending partial layouts. Isolate a
  larger layout in the batch's translation unit if extending the common base
  would shift existing derived members.
- Use canonical MSVC 7.0 `-O2 -Gy -GX` and the purchased original executable.
  Run verbose comparisons while developing, then check the whole owned
  folder. Keep the best defensible attempt annotated as NONMATCH.
- Use `-v -j 1` for paired disassembly when checking multiple files: the
  original tool's diagnostic stdout capture is process-global. Quiet
  comparisons and report generation can use bounded parallelism.
- For known register-allocation, stack-slot, cross-function or x87 scheduling
  ties, move on after two or three unsuccessful source-shape retries.
  Twenty minutes or fifteen compiles is an upper bound, not a target.

## Checkpoints and handoff

- Commit locally about every five functions so a usage interruption cannot
  lose the work. Worker checkpoints contain only owned source/header/notes
  files; do not regenerate or commit shared reports from worker worktrees.
- Do not push worker branches. The user's current publication rule requires
  at least 100 NEW exact matches since the last push, as one combined commit.
  The coordinator preserves local checkpoints and consolidates only its own
  unpublished work before publishing. Never rewrite published history.
- Record screened candidates, implemented/attempted addresses, NEW exact
  matches, retained NONMATCH attempts, and prior NONMATCH promotions
  separately. Existing exact functions rechecked in a file are not new wins.
- Update the range notes with addresses, outcomes, reusable compiler lessons,
  real naming/ABI conflicts and the next coherent family. Then freeze code
  and report the worker branch and checkpoint commits to the coordinator.

## Coordinator acceptance

- Verify each worker's path ownership, annotations and claimed folder result
  before integrating its batch. Checkpoint commits are not acceptance proof.
- After integration run the full matcher and regenerate `status.csv`. Reject
  duplicate addresses and every prior MATCH regression. Review deterioration
  of old NONMATCH attempts when shared declarations change.
- Only AFTER the matcher finishes, regenerate `symbols.csv`: symbolmap reads
  the MATCH set from status.csv. Do not overlap those dependent report runs.
  Investigate new symbol conflicts or changed mapped addresses.
- Commit sources, headers, notes and both reports together locally. Do not
  edit the CI-generated progress graphic. Matching-only batches need no
  VS2022 build or CTest run; tool changes need their relevant tool tests.
- Publish only after checking the 100-new-MATCH threshold against the exact
  last-pushed report, with no previous MATCH regressions. Incorporate any
  verified tracker-only remote update before the ordinary fast-forward push.
