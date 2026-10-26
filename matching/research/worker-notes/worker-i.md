# Worker i notes (0x004A0000-0x004CFFFF)

Older history for this bucket: `docs/batches/worker-i-notes.md` in the
upstream working repository (Kotor RE).

## Round s1 (branch claude/worker-i-s1)

Folder result: 456/478 -> 499/535 (`kotor-match -q -j 2 matching/src/i`).
57 new annotations: 43 exact, 14 kept as NONMATCH.

### New exact matches

- bsp.o (`bsp.cpp`, `bsp_plane_list.cpp`, `i/bsp.h`): 0x004ACED0
  ~BSPNode, 0x004ACF60 BSPTraverse (sphere), 0x004ACFF0 RFaceSplitTest,
  0x004AD060 BSPFitSphere, 0x004AD0D0 BSPFitBox, 0x004AD1A0 BSPSearch,
  0x004AD260 BSPHitCheck, 0x004AD560 BSPSphereCheck, 0x004AD5E0
  CExoArrayList<Plane>::Add, 0x004AD630 BSPNode ctor, 0x004AD6D0
  BSPTraverse (point).
- math3d.o: 0x004AC110 InterpolateBezier (`math3d_bezier.cpp`),
  0x004ACA00 NormalOf (`math3d_quat.cpp`), 0x004AB250
  planelineintersection, 0x004AB360 segmentproject, 0x004AB630
  RotationArc (`math3d_statics.cpp`, `i/math3d.h`).
- LightManager: 0x004A36B0 SetupLightsForThisPositionAndRadius.
- aurgrass.o (`triangle_list.cpp`, `triangle_bin.cpp`): 0x004A7220 /
  0x004A7240 CAurTriangleListElem ctors, 0x004A72D0 dtor, 0x004A7340
  CExoArrayList<float> copy ctor, 0x004A73F0 / 0x004A77C0 / 0x004A7790 /
  0x004A81B0 CExoArrayList<CAurTriangleListElem> Allocate / Add / dtor /
  copy ctor, 0x004A7690 GrassSortTris, 0x004A82B0 / 0x004A8350
  CAurTriangleBin ctor / dtor, 0x004A8460 DestroyGrassPolys.
- aurwindmanager.o (`wind_manager_life.cpp`): 0x004A57C0 FillNoiseField,
  0x004A58C0 PointSourceWind ctor (not in functions.csv; inside
  0x004A57C0's range), 0x004A5F50 / 0x004A5F80 point-source list dtor /
  Allocate, 0x004A6570 SafePointerList<WindManager> dtor, 0x004A6640
  WindManager ctor, 0x004A6710 ~WindManager.
- aurio.o: 0x004A0580 IODispatcher::GetRef, 0x004A1260 InputBinary::Read.
- Fonts (`font_render.cpp`): 0x004A21D0 ~CAurFontRender, 0x004A22E0
  AurDeleteAllFonts.
- aurvertexprog.o: 0x004A2320 FindNumberOfAllowedLights, 0x004A2400
  AurVertexProgramTrackMatrices, 0x004A2830 ~VertexProgram.

### Kept NONMATCH (diff insns)

- 0x004AC960 Quaternion(Vector, float) (27) and 0x004ACAC0 YawPitchRoll
  (21): the original keeps `this` / the hidden return pointer in edx
  across a call (cross-function register knowledge). Skip.
- 0x004AB4C0 Plane::Transform (45): register tie, vNormal.x kept in ebx.
- 0x004AD7E0 BSPTraverseVolumeApprox (33): cl tail-merges the split==1
  Add into the split==2 one; plane pointer in ecx instead of edx.
- 0x004A4F10 / 0x004A4FF0 Disable/EnableThisLight (14) and 0x004A5680 /
  0x004A5710 Disable/EnableBumpMapLights (3): the original reuses the
  copied list's element pointer after calls (no reload); ours reloads.
  `const` local and the sized ctor did not help.
- 0x004A3770 GetNearestLights (5): `add esp, 4` scheduled after `fadd`.
- 0x004A2A00 SetLightGL (31): cl hoists `GL_LIGHT0 + nIndex` above the
  branch; the original recomputes it per branch.
- 0x004A8230 CExoArrayList<SafePointer<CAurTexture>>::Add (8): the
  original loads t.m_p after the num store and keeps it across the call.
- 0x004A5E30 WindManager::UpdateTimer (5): num - 1 in edi vs esi.
- 0x004A13A0 / 0x004A14B0 SyncInput::Read (25 / 9): register tie and
  merged `return 0` placement.

### Lessons

- **Tail recursion**: BSP walkers written as plain recursion
  (`return Fn(child, ...)`) get cl's tail-recursion-to-loop rewrite and
  then match, including the canonical x87 operand order that a
  hand-written loop never reproduced. A named local whose address is
  taken (`Plane flipped(-n, -d)` passed by reference) blocks the rewrite
  (BSPFitBox keeps real recursive calls).
- Dot products: operand order is canonicalised by cl; when the loop form
  refuses to move, change the control-flow shape instead of the
  expression. `(p - a) * v` via an inline dot operator matched where
  explicit terms did not (segmentproject).
- **EH frame with no state stores** in a constructor (or a function
  static init) = the sized `CExoArrayList(int nSize = 0)` ctor with the
  dead `new`. It fixed IODispatcher::GetRef, WindManager() and
  CAurTriangleBin() at once.
- SafePointer back-list removal comes in three flavours: out-of-line
  `Remove` (0x00445080) for assignment, `RemoveNoThrow() throw()` when no
  EH state store precedes the call, and the inlined loop
  (`RemoveInline`) in host-list destructors. Host formed before the push
  (`pHost = &m_p->list; pHost->Remove(this)`) vs push first decides the
  `lea ecx` / `push` order.
- Dead zeroed stack temps: `Vector q[3]` with a zeroing default ctor
  (the array init loop is not eliminated); `Vector r = expr; return r;`
  gives the copy-out through edx.
- `x = n; x = n > 3 ? 3 : n;` reproduces the double store of
  FindNumberOfAllowedLights.
- `(1.0f / s)` repeated (CSE) instead of a named `fInv` frees the last
  multiply to consume the reciprocal (RotationArc).
- Store order of `a = b = Vector()` follows the right-to-left chain.
- Explicit full-class instantiation instantiates every inline member;
  instantiate members individually when some do not compile for T.
- Commit identity: the worktree has no git user; use
  `git -c user.name=RayesDiyab -c user.email=114070898+RayesDiyab@users.noreply.github.com`.

### Next in-range queue

- 0x004A74F0 (no src; appends 0x54-byte records to the global list
  0x00831E44 with a normalised 4-float block) and 0x004A7860
  CAurTriangleBin::AddTriangle.
- WindManager: 0x004A5C80 PointSourceWindVector (x87), 0x004A6050.
- LightManager: 0x004A3CF0 PrioritizeIllumination, 0x004A2BC0
  ~LightManager (19 lists), 0x004A38B0, 0x004A3F70, 0x004A4B10,
  0x004A50D0; 0x004A30E0 SetupVertexProgramLights (sprintf-heavy).
- VertexProgram::Enable 0x004A24D0.
- math3d.o x87: 0x004A99B0, 0x004A9BB0, 0x004A9E00, 0x004AA9C0,
  0x004AAB50, 0x004AB750 / 0x004AB9D0, 0x004ABDD0, 0x004ABF00,
  0x004AC340, 0x004ACB90, 0x004ACCA0.
- The queue in the older notes (CSWSObject, CServerExoAppInternal,
  CSWSArea path code, CSWSModule leftovers) is still open.

## Round s2 (branch claude/worker-i-s2)

Resumed from the interrupted WIP commit (it did not compile: the
`m_10048` -> `CWorldTimer*` rename needed a cast in
`CServerExoApp::FUN_004aede0`). Folder result: 499/535 (main) ->
513/560 (`kotor-match -q -j 2 matching/src/i`). 25 annotations
attempted, 14 new exact, 11 kept as NONMATCH.

### New exact matches

- CServerExoAppInternal (`server_exo_app_internal2.cpp`): 0x004B4AB0
  UpdateShutdownTimer, 0x004B6950 UpdateClientGameObjects, 0x004B6CC0
  UpdateSubareasOnMoveTo, 0x004B8110 SetPauseState, 0x004B5050
  AutoSaveOnEnteringModule, 0x004B2380 DoModuleEnterSaveCleanup,
  0x004B6270 RunModule, 0x004B2E70 StoreCurrentModule.
- CServerExoAppInternal (`server_exo_app_internal3.cpp`): 0x004B69C0
  Shutdown.
- CSWSObject (`sws_object_misc.cpp`): 0x004CAE60
  GetIsAnimationStationary (placeholder name), 0x004CAEC0
  GetCurrentHitPoints (virtual +0x9c), 0x004CAF20 SetDamageImmunity,
  0x004CB020 GetDamageLevel, 0x004CB1A0 SetDialogDelay.
- Not in functions.csv (function starts inside the previous entry's
  range): 0x004CAEC0, 0x004CAEE0, 0x004CAF70.

### Kept NONMATCH (diff insns)

- 0x004B4960 ResolvePlayerByFirstName (3): the original loads
  m_nPlayerID into ebx after the CExoLocString temporary ctor; a named
  local or a direct argument both lose.
- 0x004B6C40 GetActiveTimer (3) and 0x004B8020 SetWorldTimeOnList (3,
  inlines it): `and al, 1; test al, al` vs `test al, 1` for the bit-0
  test, same quirk as GetActiveExclusionList. `>= 1` gives `and al,1;
  cmp al,1` but breaks SetPauseState (which shares the inline helper).
- 0x004B3D10 MovePlayerToArea (6): pathfind pointer / &vSafe register
  swap in the ComputeSafeLocation call.
- 0x004B3A90 InitiateModuleForPlayer (28): `xor ecx,ecx; sete cl; mov
  eax,ecx` for `bNew = m_b40 == 0`; `(int)(bool)!x` is closest, a
  separate load local fixes the bool but rotates all later registers.
- 0x004B5C50 LoadCharacterFinish (43): one stack dword fewer than the
  original, `mov al,[p+24h]; cmp al,1`, and argument/app-pointer
  scheduling around AddEventDeltaTime.
- 0x004CAC30 AddActionNodeParameter (12): if-chain matches the compare
  order, but the type-2 block is not cross-jumped into the type-3 one.
- 0x004CAD20 SetPortraitId (16): the portraits 2DA pointer is computed
  into eax before the arguments (`mov ecx, eax` at the call); ours
  goes straight to ecx.
- 0x004CAEE0 GetDamageImmunity (4) and 0x004CAF70
  GetDamageImmunityByFlags (19): the +/-100 clamp shape (byte compare
  for 100, movsx + dword compare for -100, re-test of 100) was not
  reproduced by if/ternary/min-max-macro forms.
- 0x004CC0C0 SetLastHostileActor (17): register colouring only.

### Lessons

- Scoped named locals: `{ CExoString sColumn("..."); b = p->Get(..,
  sColumn, ..); }` gives the `lea ecx, [esp+x]; push ecx` argument the
  original uses; a temporary `CExoString("...")` pushes the ctor's eax
  instead. Same for 2DA lookups in AutoSave/DoModuleEnterSaveCleanup
  and the CResRef passed by reference in SetPortraitId (the second one
  in its own `{}` block so its slot is shared with dead temporaries).
- `bool bMissing = !p->Get(...)` inside the block gives `neg; sbb bl,
  bl; inc bl`.
- A call result that is used as `this` for a later call with an
  argument: write `CSWSMessage* p = Get(); p->Send(1);` when the
  original pushes the argument after the getter call.
- Arguments vs object expression order: `GetServerExoApp()->F(x)`
  (inline accessor) loads the app first, `g_pAppManager->
  m_pServerExoApp->F(x)` loads x first. Pick per call site.
- CExoArrayList Clear / Contains / Add as inline template members (in
  `i/server_exo_app.h`) matched Shutdown's three inlined loops; Contains
  needs the searched id read into a local first.
- `if (r <= 0.0f) return 5; return 4;` gives the `mov al,5; test
  ah,41h; jnp` tail of GetDamageLevel.
- SetDamageImmunity matched only with `(unsigned short, char)`
  parameters modified in place; symbols.csv had (int, int) from another
  bucket's call declaration.

### Naming notes

- 0x004CC0C0 is CSWSObject::SetLastHostileActor here (it calls
  AsSWSCreature on `this`); symbols.csv has a `calls` entry naming it
  CSWSCreature::SetLastHostileActor.
- CServerExoApp::FUN_004aed80 now returns CServerAIMaster*
  (`m_pServerAIMaster`, +0x10044); FUN_004aede0's member is
  `m_pWorldTimer`.
- CSWSCreature (server header) fields: +0x310 m_oidDesiredArea, +0x314
  m_vDesiredAreaLocation, +0x320 m_bDesiredAreaUpdateComplete; size
  0xAC8. CGameObject slot 0 is now the virtual destructor.

### Next in-range queue

- CSWSObject: 0x004CD580 UpdateDialog, 0x004CD2D0 IsDialogDelay (both
  need client GUI/sound declarations), 0x004CB890 BroadcastSpellData,
  0x004CCFC0 DoSpellImmunity, 0x004CB5C0 GetNearestObjectByName,
  0x004CCD80 ClearAllActions, 0x004CC7E0 SaveActionQueue, 0x004CEA20
  AddAction.
- CSWSModule save path: 0x004C8960 SaveModuleStart, 0x004CA680
  SaveModuleFinish, 0x004C5980, 0x004C5C70, 0x004C8B90, 0x004C3960.
- CServerExoAppInternal: 0x004B5F90 (save-size estimate), 0x004B2BA0,
  0x004B7230 dtor, 0x004B55E0 ctor, 0x004B71E0 CExoStringMovieList ctor.
- The s1 queue (math3d x87, LightManager, WindManager) is still open.

## Round s4 (branch claude/worker-i-s4)

(Round s3 ended at a usage limit without a notes update; its work is in
`sws_object_*.cpp`, `grass_record_list.cpp`, `string_list_copy.cpp` and
`triangle_bin_finish.cpp`.)

Folder result: 521/575 (main) -> 531/590 (`kotor-match -q -j 2
matching/src/i`). One CSWSModule (nwsmodule.o) cluster: 15 annotations,
10 new exact, 5 kept as NONMATCH. New files `sws_module_save.cpp`,
`sws_module_load.cpp`, `sws_module_res.cpp`; layout additions in
`i/sws_module.h`.

### New exact matches

- Save path: 0x004C8960 SaveModuleStart, 0x004CA680 SaveModuleFinish,
  0x004C8B90 SaveModuleIFOFinish, 0x004C5980 SaveStatic, 0x004C3960
  SaveModuleFAC, 0x004C5C70 GetPlayerIndexInPlayerList.
- Load path: 0x004C5880 LoadModuleFinish, 0x004C8C70 LoadLimboCreatures,
  0x004C8E90 PackPlayerCharacterListIntoMessage.
- Resources: 0x004C44D0 RemoveModuleResources (1712 bytes, first try).

### Kept NONMATCH (diff insns)

- 0x004C4150 AddModuleResources (31): the loop's `nProgress += nStep`
  folds the stack operand (`add esi,[esp+14h]`); the original loads it
  into ecx first, which shifts the app pointer to edx.
- 0x004C5720 LoadModuleInProgress (20): the original keeps the area index
  argument in its stack slot (reloaded twice); ours caches it in ebx.
- 0x004C3C70 SavePrimaryPlayerInfo (17): in `sDirectory + "\\" +
  "Player"` the original pushes the operands before loading `this`
  (`mov ecx,[ebp+0Ch]`); explicit CExoString temporaries change the frame.
- 0x004C7050 SaveModuleIFOStart (158): only register rotation, starting
  at the 14 script CResRef blocks (the original always uses eax for
  `lea eax,[ebp+off]`). Accessors, helpers and declaration order did not
  move it.
- 0x004C7870 SavePlayers (109): the original keeps the server app in ebx
  through the first loop and spills the player index to an argument slot;
  ours puts the index in ebx and spills the app.

### Lessons

- **Inline accessors pin values to eax.** A load done through an inline
  member function that returns by value (`GetInternal()->m_pFactionManager`,
  `GetNextCharArrayID(i)`, `GetIsSaveGame()`, `GetCreatureStats()`,
  `GetIsPrimaryPlayer()`) gets eax and does not consume a slot in the
  register rotation. This fixed SaveModuleFAC, GetPlayerIndexInPlayerList
  and the first third of SaveModuleIFOStart. An `int` passed to a BYTE
  parameter through an accessor is pushed as the loaded dword, as in the
  original; a direct field read gives `xor/mov dl`.
- `g_pAppManager->m_pServerExoApp->F(x)` vs `GetServerExoApp()->F(x)`
  decides whether the app or the argument is loaded first. Pick per call
  site.
- A loop that rebuilds one CResRef in the same stack slot on every pass
  (`SaveStatic`) needs placement new on a function-scope CResRef; block
  locals and temporaries got a second slot.
- `CExoArrayList(int nSize = 0)` with the dead `new` (EH state, store
  order num/array_size/element) also matched `new CExoArrayList<...>` in
  SaveModuleStart and the stack list in SavePlayers.
- Declaring a `char` local for a byte argument (`unsigned char bResult =
  m->m_b5c; F(bResult);`) reproduces the store-byte/push-dword sequence
  and fixed the frame layout in AddModuleResources.
- For loops that the original rotates (`i = 0` stored before the count
  test), write `for (int i = 0; i < Count(); i++)` rather than a guarded
  do/while.
- `CSWSArea` is polymorphic here (vtable at +0, `m_cResRef` at +0x10c,
  size 0x2D4); CSWSModule +0x1E8 / +0x1F4 are unions of the temporary
  area data and the save-time ERF / player-id list.

### Next in-range queue

- CSWSModule: 0x004C84A0 ctor, 0x004C68A0 dtor (many member dtors),
  0x004C9050 LoadModuleStart (5680 bytes); retry SavePlayers / IFOStart
  register ties with new ideas only.
- 0x004CC790 SetGroupInterruptable (0 diff, jump-target only: the original
  jumps to the second null test instead of threading it) and 0x004CCD30
  DeleteCurrentAIAction (2) — GetHeadInline and ternary forms did not help.
- CServerExoAppInternal: 0x004B5F90, 0x004B2BA0, 0x004B7230 dtor,
  0x004B55E0 ctor; the s1 queue (math3d x87, LightManager, WindManager)
  is still open.

## Recovery of interrupted Claude i-s5, 2026-10-26

- Recovered 9 attempted functions from `claude/worker-i-s5`, including its
  untracked circle source. Four new exact: 0x004A99B0 Transpose,
  0x004A9E00 Slerp, 0x004AA9C0 MakeMatrix4x4, 0x004ABDD0 SphereIntersect.
- Five retained NONMATCH: 0x004A9BB0 quaternion/vector product (28 insns),
  0x004A9F40 GetPitch (14), 0x004AA610 BoxIntersect (45), 0x004AAB50
  IntersectCircle2d (20), 0x004ACB90 GetEulerRoll (21).
- Corrected the 3x3 Matrix definition for Transpose (only 9 floats), while
  0x004AA9C0 is an independent 4x4 matrix writer taking an explicit output
  buffer, translation and quaternion. Its void output-buffer form matches;
  native callers 0x0046F098 and 0x00471658 pass the existing global buffer
  0x008280A0. A previously inferred Matrix-return alias remains in range g
  and needs separate coordinator review.
- Corrected the ABI of 0x004BC0E0 SetHitInfoForCreature and its circle call
  to (center, radius, segment start, segment end, hit info). The 2/2 Mac name
  and native forwarding call confirm that order. Native instructions compare
  distances from segment start, but compute the normal from the center. Its
  old NONMATCH remains at 102 differing instructions after these fixes.
- Circle signature matches center, radius, start, end, outputs, flag. The
  untracked attempt remains NONMATCH; native ABI is independent of that score.
