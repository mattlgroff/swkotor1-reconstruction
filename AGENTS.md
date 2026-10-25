# Working agreement

- Continue the clean-room reconstruction across repository milestones.
- Do not stop merely because a commit or push succeeds; those are checkpoints
  during the larger task.
- Commit and push verified, non-proprietary progress when it is useful.
- Parallel sessions follow docs/PARALLEL_WORKFLOW.md: one address range and
  branch per session, new files only, coordinator regenerates reports.
- Promote to V4 with a differential test (tools/differential/x86emu.py runs
  the original x86 under Unicorn) whenever the function can be driven in
  isolation; record why when only V3 is possible.
- Work in batches of up to 50 related unresolved targets: collect canonical
  evidence and semantic context once, prioritize the defensible subset, then
  build, test, refresh reports, and audit once for the batch.
- Before selecting an automatic batch, prefetch a wider exact-address semantic
  candidate pool and choose the final 50 by shared owner/subsystem context,
  address locality, ABI compatibility, and binary evidence score. This keeps
  related layouts and helper boundaries together without making names or type
  hints substitute for executable validation.
- Build and run the complete registered test suite with bounded parallelism
  after confirming tests are isolated. Use `build-gcc` for the canonical suite
  and require at least one registered test: CTest can exit successfully after
  reporting “No tests found” for an unconfigured build directory. A five-run
  benchmark of the current 25 registered tests measured median CTest wall time
  at 757.6 ms serial and 395.1 ms with four workers (47.9% lower); keep the
  full suite and all existing assertions unchanged, and revisit if tests gain
  shared external state.
- When native constructors partially initialize an object, seed the complete
  recovered image with canary bytes and assert the exact write mask. For
  mutators, compare the full image before and after while allowing only the
  binary-observed output ranges to change; this catches accidental clearing
  and collateral writes that field-only assertions miss.
- For owning container clusters, inspect the element constructor, copy helper,
  destructor, and resize cleanup as one unit before promoting append/copy
  methods. If the observed ownership path is anomalous, preserve the proven
  layouts but defer behavior claims until the lifetime contract is reconciled.
- When Ghidra marks a CRT allocator/deallocator thunk non-returning, resolve the
  thunk and inspect raw bytes through the caller's post-call instructions and
  epilogue before accepting a truncated function body or inferred cleanup ABI.
  The decompiler warning alone is not proof that control flow ends at that
  call.
- For x87-heavy math, preserve the instruction-level operation order and every
  float materialization boundary in the source model. Keep precision confidence
  below high until an executable differential harness covers the relevant
  extended-precision and exceptional-value cases.
- Treat the pre-indexed semantic map as an efficient starting hypothesis:
  reuse corroborated names, signatures, ownership, offsets, and call context;
  re-check control flow, constants, side effects, lifetime behavior, vtable
  use, and stack cleanup against the canonical x86 evidence before promotion.
- Keep exact ABI matches and receiver-compatible compiler-label variants in
  the fast lane, while routing genuine ABI/address/layout conflicts to a
  deferred correction queue. Do not spend a full deep-analysis cycle on a
  compact leaf whose binary contract is already independently clear.
- Keep ambiguous ownership, ABI, and layout cases in a deferred queue so they
  do not slow down promotion of compact, independently validated contracts.
- Prefer compact subsystem clusters with a shared receiver/layout contract;
  validate all simple forwarding, direct-storage, and helper-boundary members
  together while keeping unusual ABI or ownership cases deferred.
- Keep semantic-coverage metrics separate from implementation/progress
  metrics: a named target is still V0 until its source, build, tests, and
  binary-grounded contract satisfy the existing validation gate.
- Derive the started-subsystem list from reconstructed functions with known
  subsystem labels instead of maintaining it by hand; this prevents progress
  reporting from silently lagging behind accepted work.
- Stop only when the complete game reconstruction is finished, or when a real
  blocker requires external state or user input.
