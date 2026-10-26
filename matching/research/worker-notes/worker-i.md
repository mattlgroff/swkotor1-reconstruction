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
