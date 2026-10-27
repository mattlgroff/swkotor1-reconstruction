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

## Round s6 (branch claude/worker-i-s6)

Folder result: 535/599 (main) -> 552/620 (`kotor-match -q -j 2
matching/src/i`). Main cluster: CServerExoAppInternal lifecycle/save/load
plus CServerAIMaster event nodes. 22 annotations attempted: 16 new exact,
1 prior NONMATCH promoted, 5 kept as NONMATCH.

### New exact matches

- CServerExoAppInternal (`server_exo_app_internal_life.cpp`): 0x004B7230
  ~CServerExoAppInternal, 0x004B55E0 ctor.
- `server_exo_app_services.cpp`: 0x004B7C60 StopServices, 0x004B63E0
  StartServices, 0x004B9240 UnloadModule.
- `server_exo_app_module_query.cpp`: 0x004B51A0 GetModulePreviouslyVisited,
  0x004B41C0 GetPlayerListString.
- `server_exo_app_store.cpp`: 0x004B2BA0 StorePlayerCharacters, 0x004B5F90
  EstimateSaveGameSize (placeholder name; writes +0x100C8).
- 0x004BA640 LoadGame (`server_exo_app_load_game.cpp`), 0x004B58A0 SaveGame
  (`server_exo_app_save_game.cpp`).
- 0x004AF410 GetGameObjectDescription (cdecl helper of the AI event log,
  placeholder name, `server_ai_master_debug.cpp`).
- `server_ai_master.cpp`: 0x004AFEA0 CServerAIEventNode::SaveNode, 0x004B0770
  CServerAIList ctor COMDAT (not in functions.csv; SYMBOL annotation).
- 0x004A1600 CAurFontRender::ParseMarkup (placeholder name, `font_render.cpp`).
- 0x004A74F0 AurGrassSetInfo (placeholder name, `grass_record_list.cpp`).
- Promoted: 0x004A6C70 CExoArrayList<CAurGrassRecord>::Allocate (was 26).

### Kept NONMATCH (diff insns)

- 0x004BA920 StartNewModule (25): the original keeps 0 in esi from the
  first Exists call (push esi / store esi) and lays the store-failure block
  inline; ours uses immediates and moves the block to the end.
- 0x004B78F0 PlayerListChange (43, placeholder name): the original has an
  aligned (`and esp,-8`) frame and separate slots for the two name copies.
- 0x004B0290 CServerAIEventNode::LoadNode (60, ~10 real): only the last
  case (9/27 ReadFieldINT) schedules its lea/push pair differently.
- 0x004B7470 LoadCharacterStart (70): the original inlines the lFiles
  (CExoArrayList<CExoString>) destructor on the 0xF return path and calls
  the out-of-line 0x00405920 on the return-0 path; ours differs.
- 0x004C0930 CSWSArea::GridDFSearch (28): the original reuses the nDepth/nZ
  argument slots for locals (8-byte frame); register allocation differs.

### Lessons

- **Inline accessors again**: `GetServerMessage()` (internal ->
  m_pServerMessage) fixed SaveGame's SendServerToPlayerSaveLoad_Status (the
  original loads `this` and the arguments before pushing constants);
  `GetLoadingModule()` gave `mov eax,[..]; cmp eax,1`; `CSWSClient::
  GetPlayerID()` in a call argument fixed GetPlayerListString's load order;
  `CSWCModule::GetCamera()` and `CAppManager::GetSaveLoadStatus()` gave the
  `lea ecx,[eax+..]` forms. A reference accessor (`CExoString& GetPlayerName()`)
  gives `lea ecx,[eax+4]` where a direct member gives `mov ecx,eax; add ecx,4`.
- **Implicit string conversions**: passing `"literal"` or an `int` to a
  `const CExoString&` parameter (or `sList + "x" + nCount`) builds the
  temporaries right-to-left with `lea` pushes; explicit `CExoString(...)`
  temporaries change both order and slots. `WriteGFFFile(sFile.CStr(), ..)`
  only matched with a `const CExoString&` parameter (symbols.csv says
  non-const `CExoString&`) because the temporary must share the op+ slot.
- Passing a `CExoString`-convertible `sModule` straight to
  `Exists(const CResRef&, ...)` builds the CResRef temporary before the
  other arguments (implicit conversion), as the original does.
- Switch case bodies are laid out in source order: read the jump-table
  targets' address order and write the cases in that order (SaveNode,
  LoadNode, ParseMarkup). Nested `switch (p[2])` with two cases emits
  `cmp/je`, `cmp/jne` falling into the first-written case.
- `new T` whose ctor cannot throw (implicit vptr ctor, inline zeroing ctor)
  gets no EH state; out-of-line ctors do.
- `for (int i = 0; i < 4; i++) a[i] = 0.25f;` loads the constant into a
  register (unrolled) where four assignments store immediates; a member
  `Vector` with a zeroing default ctor reproduces the member-first store
  order of CAurGrassRecord. Float fields zero with immediates, ints with a
  zero register: a field stored as `mov [..], 0` next to `mov [..], ebx` is
  a float.
- An interleaved `fSum = a; dst0 = a; fSum += b; dst1 = b; ...` matched the
  x87 sum scheduling that `fSum = a + b + c + d` after the copies did not.
- `delete p` on pointer members with virtual destructors needs `virtual ~T()`
  declared (CSWSMessage, CSWSModule, CSWRules, CSWVirtualMachineCommands);
  class sizes for `new` must be exact (StartServices pushes them).
- Unknown trigger for `and esp,-8` frames: StopServices aligns only with the
  whole g_nAppMode block present, not with any single statement; not solved.

### ABI / naming notes

- CServerExoAppInternal::StartServices takes one ignored int (RET 4) and
  returns nothing in eax; LoadGame is (nSlot, sName, sModule, bLoadFromSave)
  (RET 0x10) and SaveGame is (nSlot, sName, CSWSPlayer*, int) (RET 0x10).
  The CServerExoApp wrappers are tail jumps, so they were updated too.
- CServerExoAppInternal1b770 is CSWPartyTable (typedef kept); +0x100FC
  CSWGlobalVariableTable has ctor 0x0052B170 / dtor 0x00528DE0; 0x00831FDC
  holds the live CServerExoAppInternal (`g_pServerExoAppInternal`,
  placeholder); 0x0074C524 is PLAYER_ID_INVALID (placeholder).
- CServerExoAppInternal +0x1000C is CSWVirtualMachineCommands*, +0x10018 a
  16-entry player id array, +0x100EC a CExoArrayList, +0x10094 a CExoString,
  +0x100C8 the estimated save size, +0x100CC/+0x100D0 a ms timer pair.
- CExoResMan 0x00409460 is called Leaf9460 here (symbols.csv); 0x00409480 is
  NukeDirectory (symbols.csv: Leaf9480).
- 0x004AF630 (event-log formatter) reads the event id from the caller's esi
  (cross-function register use): skip.

### Next in-range queue

- CServerExoAppInternal: 0x004B6DA0 (party/incapacitation update, x87 and
  client classes), 0x004BABB0 MainLoop, 0x004B95B0 LoadModule, 0x004B8300
  DoPCAutosave, 0x004B3110 (load-screen/save path), 0x004B0B70 AI loop.
- 0x004A1770 CAurFontRender::Draw (calls ParseMarkup), 0x004A5C80 /
  0x004A6050 WindManager, the math3d x87 list, the CSWSArea pathfind cluster
  (0x004BCB70-0x004C3260; GridDFSearch layout is now in `i/sws_area.h`).

## Round s7 (branch claude/worker-i-s7)

Resumed from the interrupted WIP commit (TestDirectLine, it compiled at
552/621). Folder result: 552/621 (main) -> 572/642 (`kotor-match -q -j 2
matching/src/i`). Main cluster: LightManager (aurlightmanager.o). 21 new
annotations attempted: 15 new exact, 6 kept as NONMATCH (plus the carried
TestDirectLine); 5 prior NONMATCH promoted.

### New exact matches

- `light_manager_life.cpp`: 0x004A38B0 LightManager ctor (19 member lists,
  empty-state SEH frame from the sized list ctor), 0x004A2BC0
  ~LightManager, 0x004A39F0 AddLight, 0x004A3AE0 RemoveLight (byte flag),
  0x004A3CA0 AddRenderLight.
- `light_manager_slots.cpp`: 0x004A2DB0 SetupLightsForThisGob, 0x004A2DE0
  SetupLightsForThisRoom, 0x004A2E00 Reset, 0x004A2E10 UpdateLights,
  0x004A2F30 PrioritizeLights, 0x004A2FF0 EnableHeadLight, 0x004A3080
  EnableLights, 0x004A4480 EnableLightsDynamicOnly.
- `light_prioritize.cpp`: 0x004A3E30 PrioritizeShadow, 0x004A3EE0
  PrioritizeBump.
- Not in functions.csv (starts inside the previous entry's range):
  0x004A2DB0, 0x004A2DE0, 0x004A2E00, 0x004A2E10, 0x004A2E70, 0x004A2F30,
  0x004A2FF0, 0x004A3080 (inside 0x004A2BC0); 0x004A39F0, 0x004A3AE0,
  0x004A3CA0 (inside 0x004A38B0); 0x004A3E30, 0x004A3EE0 (inside
  0x004A3CF0); 0x004A4480 and 0x004A4580 ControlLightsFade (inside
  0x004A3F70); 0x004C6BB0 (`lea eax,[ecx-1Ch]; ret`) and 0x004C6BC0
  (`sub ecx,1Ch; jmp 0x004C8940` adjustor thunk) inside 0x004C68A0.

### Promoted (prior NONMATCH -> MATCH)

- 0x004A4F10 DisableThisLight, 0x004A4FF0 EnableThisLight, 0x004A5680
  DisableBumpMapLights, 0x004A5710 EnableBumpMapLights: see the first
  lesson below.
- 0x004A6480 WindManager::DeregisterWindyEmitter: member
  `PointerList::Contains` (computes `this+84Ch` first) instead of an
  open-coded loop.

### Kept NONMATCH (diff insns)

- 0x004BCB70 CSWSArea::TestDirectLine (89): the original keeps the `done`
  flag in memory and edi = 0; cl threads `done = 1; continue;` away in
  every loop shape tried (while, for(;;) + break/continue).
- 0x004C13E0 SmoothSelectionRestartable (65): the original copy-propagates
  the four buffer locals in ebx/ebp/edi across the passes; cl keeps a zero
  register in ebx instead. Local order, reference outputs, shared result
  variable did not move it.
- 0x004C17A0 SmoothPointsOnPathRestartable (15): one stack dword fewer; the
  original shares the pPoints spill slot with the corner buffer pointer.
- 0x004A3CF0 PrioritizeIllumination (13): VectorEqual's two address
  arguments are computed before both pushes in the original (also with
  inline `operator==`/member wrappers); register colouring in the Add.
- 0x004A2E70 EnableHighestPriorityLights (3): `pop ebp` before the trailing
  glDisable loop's `cmp esi, 3`.
- 0x004A24D0 VertexProgram::Enable (96): bLighting lives in bl and `this`
  in ebp; cl folds the ARB/NV `if/else` calls into one `call eax` (the
  original hoists only the pushes). Function-pointer ternaries keep the
  size (672) but score 102.
- 0x004A4B10 LightManager::GetShadowLights (27, ~4 real): the mode-2 exit
  and the final return share one `lea eax,[esi+98h]` block in the
  original; cl CSEs &m_l98 into edi and merges the final if/else
  assignments. A shared exit (else block / goto) re-colours the whole
  function. The front-insert matched only when written out (inline
  Allocate expansion, `n` reused as the shift counter).

### Lessons

- **Visible bodies of out-of-line callees change caller codegen.** With
  the CExoArrayList copy ctor and operator= defined in the class (they
  stay calls: the forceinline AddCopy loops make them too big to inline),
  cl stops reloading a local list's element pointer after the call and
  after later unrelated calls (DisableBumpMapLights, Disable/
  EnableThisLight, GetShadowLights). Opaque declarations force reloads.
  The same with `__declspec(noinline)` did not help SafePointer::Add
  (0x004A8230).
- Big functions.csv entries often hide several functions: probe them with
  empty stubs and look for `ret` + padding + aligned new prologues.
- The inliner budget reproduces mixed inline/out-of-line Adds by itself
  (AddLight: two inline Adds, the third calls 0x00444FE0).
- A float that the original copies with integer moves into a dead argument
  slot and reloads three times is a named float local (`float fScale =
  p->m_fMultiplier; Vector vColor(fScale * c.x, ...)`), not the inline
  `operator*(const Vector&, float)`.
- Quaternion in this bucket is {w, x, y, z} with the default ctor storing
  z, y, x, w (stores land at +0xc, +8, +4, +0).
- `CExoArrayList<T>::Allocate` is now defined (template, out of class) in
  `i/light_manager.h`; cl still emits calls to it where the original does.
- Pointer-list Add (0x00444FE0) and Remove (0x00445080) are folded COMDATs
  shared by every pointer element type; Allocate<T*> is 0x005E03C0.

### ABI / naming notes

- LightManager layout is now complete for 0x00..0xF4 (19 lists; +0xA4 the
  chosen shadow light, +0xA8 int). Vtable slots +0x44
  SetupLightsForThisPositionAndRadiusIn(pos, radius, list*), +0x4C..+0x54
  Prioritize{Illumination,Shadow,Bump}, +0x58, +0x5C ControlLightsFade,
  +0x60 EnableHeadLight added to `i/light_manager.h`.
- Light (PartLight) now has a vptr: +0x04 node (+0x90 shadow priority),
  +0x0C GetWorldTransform, +0x40 dynamic type, +0x5C cast shadow, +0x80
  priority, +0x90 shadow fade.
- 0x0078E388 is g_bHeadLight (short) in symbols.csv; 0x007B92A0 has
  conflicting names (g_bRegisterCombiners / g_nChunkVisible) but is read
  as the active Scene* (+0x80 LightManager*) by VertexProgram::Enable.

### Next in-range queue

- LightManager: 0x004A4580 ControlLightsFade (hidden start, x87 fades),
  0x004A3F70 EnableHighestPriorityLightsFade, 0x004A50D0
  SetUpVertexProgramLightShaderConstants, 0x004A30E0 (sprintf-heavy).
- Triangle bin / grass (x87): 0x004A68B0 UpdateGrassForWind, 0x004A6D30,
  0x004A7860 AddTriangle, 0x004A8500.
- CSWSArea pathfind cluster (0x004BCB70-0x004C3260) and the
  CServerExoAppInternal queue from s6 remain open; 0x004C68A0
  ~CSWSModule (large MI destructor).

## Round s8 (branch claude/worker-i-s8)

Folder: 572/646 -> 576/655 (kotor-match -q -j 2 matching/src/i). The first
s8 worker (cut off by a usage limit) left RunDialogOneLiner (17),
StartDialog (62) and SpellCastAndImpact (82) as NONMATCH plus an empty
LoadActionQueue stub; the resumed worker finished the stub and continued.
The bucket's todo list is down to 51 entries, all large and mostly x87.

### New exact matches

- 0x004CECB0 CSWSObject::LoadActionQueue (sws_object_save.cpp): memset of
  the 13 param/type arrays, switch order 1,2,4,3,5 like SaveActionQueue,
  separate cleanup-type array, AddAction with all 13 pairs.
- 0x004ACCA0 PointTo (vector_point_to.cpp): two function-static Vectors;
  the +Z axis must be two *temporaries* `Vector(0,0,1)` (one for the
  cross, one for the dot) so cl constant-folds the dot to (y+x)*0+z.
- 0x004ABF00 DistanceBetweenLines (vector_line_distance.cpp): plane
  distance as a named float before planelineintersection.
- 0x004AB750 polyhit(points, n, normal, dist, start, end, hit)
  (vector_polyhit.cpp, SYMBOL ?polyhit@@YAHPAVVector@@HABV1@ABM110@Z).

### Kept NONMATCH (diff insns)

- 0x004CF160 CSWSObject::DoDamageImmunity (18): hidden function after
  LoadActionQueue (functions.csv merges them; 0x004CECB0's listed size
  1760 includes it). Only `this`/result swap ebp/ebx; result-first,
  early-return and in-place variants are worse.
- 0x004CD760 CSWSObject::BroadcastDialog (88): original keeps 0 in ebp and
  reloads the escaped object pointer; using the escaped variable directly
  loses the zero register (101).
- 0x004A5C80 WindManager::PointSourceWindVector (42): matches with
  `scale * NormalizeVector(d)` (operator*(float, const Vector&) so the
  call is evaluated first and fDistance goes to memory); remaining diff is
  eax/esi for the element base.
- 0x004AB9D0 polyhit(points, n, start, end, hit, normal) (45): the
  original's first loop uses IV i+2 with (i+1) as `edi-1` plus a count-down;
  every source shape tried gives IV i+1.
- 0x004BE3A0 CSWSArea::ComputeBestCorner (85): x87 keeps the diffs on the
  stack before fsqrt; not pursued.
- 0x004BE5E0 CSWSArea::ValidateSafePosition (39, placeholder name): the
  original recomputes `pos.z - 0.1f` for fZLow instead of reusing
  vStart.z, and places the shared `return 0` right after GetRoom.

### Lessons

- Loop `for (...; bInside && i < n; ...)` (flag first) gives the
  original's unrotated loop with the bound in memory; `i < n && bInside`
  rotates and keeps n in a register.
- For x87 the evaluation order of an inline operator's arguments decides
  whether a float local lives in memory: a call evaluated first forces
  earlier floats to memory (PointSourceWindVector).
- Constant-folding of an inline dot product happens only with a
  temporary constant Vector, not a named local whose address escaped.
- Overloaded free functions need `SYMBOL:` annotations; the mangled name
  counts the function name as name back-reference 0 (Vector is `1`).

### ABI / naming notes

- CSWSObject vtable +0xB4 is a char-returning damage-immunity query by
  flags (placeholder GetTotalDamageImmunity); the non-virtual
  GetDamageImmunityByFlags at 0x004CAF70 already uses that name.
- CSWSExpression +0x04 is a CExoArrayList<CExoString*> of matched words;
  CSWSObject +0x19C is an int listening flag (placeholder m_bListening).
- CPathfindInformation +0x04 radius and +0x14 height (placeholders); room
  +0x3C walk mesh with 0x00583E30 (placeholder ClipSegmentToRoom).
- 0x007440FC is 1e-8f, 0x007440F8 is -1e-5f, 0x00741870 is 0.0001f,
  0x00749B30 is 1e-4f (two constants), 0x0073DFEC 0.01f, 0x0073E798 0.1f.

### Next in-range queue

- Remaining todo (all >= 500 B): 0x004AC340 IntersectLineSegments2d
  (goto-heavy), 0x004A6050 WindManager::GetGlobalWind (quaternion x87),
  0x004CBC00 CalculateSpellRangedMissTarget (atan/cos/sin), 0x004CE220,
  0x004CFCB0, 0x004A68B0 / 0x004A6D30 / 0x004A7860 / 0x004A8500
  (triangle bin), the CSWSArea pathfind cluster 0x004BCEE0-0x004C3260,
  0x004C68A0 ~CSWSModule, 0x004A30E0 (sprintf vertex program).
- Cheap retries: RunDialogOneLiner (17, argument load order before the
  pushes), DoDamageImmunity (18), SetGroupInterruptable /
  DeleteCurrentAIAction (0/2: an unthreaded null test of the list head).
