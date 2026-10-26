# Worker G matching notes

Range: 0x00420000–0x0044FFFF. Older history and the long next-batch queue are
in Kotor RE's `docs/batches/worker-g-notes.md` (read-only); current source
annotations and reports take precedence over it.

## Round s1, 2026-10-26: GLRender framebuffer effects

Folder result: 438/463 -> 459/490 (`kotor-match -q -j 2 matching/src/g`).
27 functions attempted, 21 new exact, 6 retained NONMATCH.

New files: `src/g/fb_soft_shadow.cpp`, `fb_overlay.cpp`, `fb_bloom.cpp`,
`fb_convolution.cpp`, `fb_modify.cpp`, header `include/g/fb_effects.h`
(GL imports, extension procedure pointers, framebuffer/bloom/soft-shadow
globals). `glrender.cpp` gained three attempts.

New exact (21):
- Soft shadows: CopySoftShadowsATI 0x42d140, SetUpSoftShadows 0x42d1c0,
  FinishSoftShadowsATI 0x42d250, FinishSoftShadows 0x42d9a0.
- Overlays: RenderOverlayFilmNoiseATI 0x432780, RenderOverlayFilmNoise
  0x432ad0, RenderForceDistortionATI 0x432d70, RenderForceDistortion
  0x4331d0, RenderAccumulationBlurATI 0x433970, RenderOverlayOverbrighten
  0x4340f0, RenderOverlayOverbrightenATI 0x434240.
- Bloom: MipmapFilterATI 0x42b370, MipmapFilter 0x42b5d0,
  RenderFrameToTextureATI 0x433580, RenderFrameToTexture 0x4337a0,
  RenderOverlayConvolutionATI 0x42c6e0, RenderOverlayConvolution 0x42cc60.
- Drivers: FrameBufferModificationsATI 0x435bd0,
  FrameBufferModificationsATI2 0x436070, FrameBufferModifications2
  0x4361f0, FrameBufferModifications 0x437520.

Retained NONMATCH (do not retry immediately):
- BloomFilterATI 0x42b810 (114) / BloomFilter 0x42bf80 (106): same two
  issues in both. Stack slots of fInv and the unit-0 s coordinate are
  swapped (orig fInv @+0x10), and the original keeps both arms of the
  unit-0 s ternary whole (fsub/fmul duplicated) where cl hoists the common
  fsub. Tried: if/else, inline-in-call, swapped operand order, negated
  condition, declaration order. The constant upload is an if/else-if chain
  on i; `float f = scale*k; c[0..3] = f;` gives the fst chain; declaring
  `int nRow = 0, nCol = 0;` before `glEnable(GL_BLEND)` fixed their stores.
- RenderSaturationAdjustmentATI 0x4343d0 (79): x87 scheduling of the
  3x3 saturation matrix (Haeberli weights 0.3086/0.6094/0.0820 times the
  0x78db24 tint). The original stores rows to memory as it computes them;
  every source shape tried (ctor, member stores, named weights) gives the
  same canonical cl code. The ninth clamp is the original's typo
  `if (v2.x > 1) v2.z = 1`. 0x434890 (NV twin) is not attempted.
- SetLightmapTexCoordToStage0 0x425280 / SetDecalTexCoordToStage0
  0x4253d0 (18 each): body duplicated per VBO branch matches; only the
  epilogue differs (orig pops edi/esi, stores stage 0, then pops ebx).
- DrawLightmappedGrass 0x426230 (24): VBO path exact
  (`pVertices += nQuads * 0x30` reuses the parameter); the non-VBO copy
  swaps ebx/ebp.

## Round s2, 2026-10-26: leftovers across the bucket, hidden functions

Folder result: 459/490 -> 499/533 (`kotor-match -q -j 2 matching/src/g`).
43 functions attempted, 40 new exact, 3 retained NONMATCH. 17 of the new
exact functions are missing from functions.csv (annotated anyway, marked
"*" below); the coordinator may want to add them.

New files: `gob_tree.cpp`, `mdl_temp.cpp`, `exo_list_list.cpp`,
`font_info.cpp`, `snapshot.cpp`, `static_cache.cpp`, `gl_debug_draw.cpp`,
`aur_control.cpp`, `tex_ptr_list.cpp`, `scene_hit.cpp`, `gl_textures.cpp`,
`nv_constants.cpp`. Header changes: `mdl_ctor.h` (PostProcess +0x60 list,
InternalRemoveTemporaryArrays decls), `glrender.h` (Imposter ctor,
ImposterManager::m_nImposters, SetUp returns int), `aurora.h` (DEVMODE
fields, CAurInternalGL slots), `part.h` (Vector, HitCheck / SetPartPosition
slots, PartAnimMesh).

New exact (40):
- Gob::DisableRenderBumpedOut 0x4485c0.
- MdlNode*::InternalRemoveTemporaryArrays: TriMesh 0x43c090, DanglyMesh
  0x43c130, Skin 0x43c1a0*, AnimMesh 0x43c320*; NodeVertexAnim dtor
  0x43c1b0.
- Templates: CExoArrayList<int>::operator= 0x438bb0,
  CExoArrayList<CExoArrayList<int>>::Allocate 0x43c260,
  CExoArrayList<SafePointer<CAurTexture>>::Allocate 0x43de00,
  ParseList<Vector> 0x421ff0.
- CAurFontInfo ctor 0x4221c0 / ParseField 0x422210; normalization cube map
  getter 0x4233e0; AurSnapshot 0x4211a0; MakeLookUpTexture 0x423d80.
- Statics: AllocateShadowMemory 0x426940, AurGetTime 0x44f320.
- Scene slots: Enable/DisableAnimations 0x44f3a0* / 0x44f3b0*,
  IsAnimating 0x44f3c0*, GetWindVector 0x44f780*, GetObjectScale
  0x44fd40*, SetBlindLights 0x44fd50*.
- ImposterManager::SetUp 0x432090; DrawTriangles 0x4318f0;
  DrawSphereBound 0x431a50; AurControlPart 0x44b900; AurPrintf 0x44d4f0.
- CAurInternalGL: Shutdown 0x44ea30, GetDisplayStyle 0x44d6d0*,
  GetValidMode 0x44d6e0, SetGamma 0x44d990*, GetContext 0x44d970*,
  MakeCurrent 0x44d980*, ??_GCAurora 0x44da40* (annotation only).
- Part slots: HitCheck +0x20 0x43ef70*, SetPartPosition +0x70 0x43f5c0*,
  PartAnimMesh::HitCheck 0x441510*; NV SetCombinerConstant0/1
  0x424940* / 0x424960*.

Retained NONMATCH:
- Gob::ReplaceTexture 0x446720 (68): szFrom stays in ebx in the original
  (frame 0x410 vs our 0x414) and the inline AsModel() is expanded twice with
  the sub/neg/sbb/not mask; cl CSEs a single setne mask.
- SaveScreenShot 0x420f20 (57): esi/ebx swapped for width and the scaled
  image; same instructions otherwise (the else arm really does
  `nOutH = bSquare` and leaves nOutW unset).
- Scene::HitCheckGob 0x44fe00 (6): bits 7 and 10 of the model flags are
  materialised with `and dl, 1` plus a dead store into the pInfo slot.

Not attempted: 0x425d80 / 0x425ea0 (custom register convention, /GL-like).

## Round s3, 2026-10-26: texture upload, shadow volumes, hidden Gob/Part/Scene slots

Folder result: 499/533 -> 523/564 (`kotor-match -q -j 2 matching/src/g`).
31 functions attempted, 24 new exact, 7 retained NONMATCH. 17 of the
attempted functions are missing from functions.csv (marked "*"); 0x43f270
and 0x44fac0 are in functions.csv with oversized lengths (they swallow the
hidden functions after them).

New files: `tex_upload.cpp`, `normal_map.cpp`, `shadow_array.cpp`,
`shadow_build.cpp`, `vertex_bind.cpp`, `part_transform.cpp`, `gob_hit.cpp`,
`gob_static.cpp`, `mdl_light.cpp`, `scene_tile.cpp`, `mdl_free.cpp`,
`part_aabb.cpp`. Header changes: `aur_texture.h` (CAurTexture slots
0x14-0x8c typed: GetImage/GetWidth/.../GetCompressionFormat; 0x1c keeps the
old name IsLightmap = IsBumpMap), `gob.h` (Part GetLocalTransform +0x08,
m_fScale +0x24, m_pGob +0x40; Gob slot +0x28 AddCallback, +0x88
HitCheckPart, +0x94 SetPartWorldTransform (was LookAtPart), +0x8c takes
CHitInfo*; m_fAlphaD8/DC union, m_pTileData +0x170, Model flag bitfields
with getters, Vector::operator*=), `part.h` (Part::HitCheck takes
CHitInfo* / float scale; PartNode AsMdlNodeTriMesh / AsMdlNodeAABB;
PartAABB::HitCheck), `aurora.h` (CAurora slot 0x14 Reshape, m_nWidth /
m_nHeight +0xa0/+0xa4, glViewport). aur_texture.cpp: BuildNormalMap /
BuildTextureNormalMap now take float scale + uchar normal-map flag (the
0x420ea0 NONMATCH (4) is unchanged).

New exact (24):
- Texture upload: CreateCubeTexture 0x423f90, CreateProcessedCubeTexture
  0x424230, BuildNormalMap 0x4260a0, InitAttenuationTexture 0x4263c0.
- Shadows / arrays: RenderShadowArray 0x426660, BuildShadowArray 0x4269f0,
  AurVARBindVertices 0x425520.
- Gob / Part: Gob::SetPartWorldTransform 0x43f280*, Gob::SetProcessFlag
  0x43f270, PartHasMaterial 0x43ee40*, Part::GetWorldTransform 0x43e4a0*,
  Part::GetLocalTransform 0x43e6a0*, ProcessStaticObject 0x44fac0.
- Model: MdlNodeLight::PostProcess 0x43dfc0*, MaxTreeDestroyNodes 0x44a8e0*,
  ModelDestroy 0x44aa60* (names not recorded; relocation callbacks).
- Scene slots: DisableVisibilityGraph 0x44f800*, GetIgnoreVisibilityGraph
  0x44f810*, SetCurrentRoom 0x44f8c0*, ReplaceTileTexture 0x44fc80*,
  DeleteTile 0x44fca0*, SetTileLights 0x44fb40*, SetTileFlames 0x44fc20*.
- CAurInternalGL::Reshape 0x44e9e0*.

Retained NONMATCH (do not retry immediately):
- CreateTexture 0x424980 (10): only the S3TC level size forms the
  height-based half first in the original; operand order, copies, helper and
  declaration order do not move it.
- CreateProcessedTexture 0x424dd0 (113): register/slot tie (mip flag in ebp,
  w/h slots swapped); flow and calls are right.
- AurBuild2DMipMaps 0x424580 (143): the original does not strength-reduce
  the per-column addresses; every source shape tried gets reduced by cl.
- DownSampleNormalMap 0x420030 (166): x87 — the original spills the four
  alpha weights; cl keeps them on the FPU stack (even as float[4]).
- Gob::HitCheckPart 0x43e930* / Gob::IntersectLine 0x43eb80 (26 each): the
  same dead `mov cl, ah; and cl, 1; mov [pInfo slot], cl` bit-8 store as
  Scene::HitCheckGob; everything after it is a 3-byte shift.
- PartAABB::HitCheck 0x4428f0* (7): fld/fcomp hoisted above the two global
  stores and the mask kept in edi; scheduling.

## Round s5, 2026-10-26: texture front end, model tree walkers, Gob ctor, render shutdown

Folder result: 537/581 -> 545/597 (`kotor-match -q -j 2 matching/src/g`).
16 functions attempted, 8 new exact, 8 retained NONMATCH. Only 44
functions.csv entries were left in the bucket, mostly large; no hidden
functions were added this round.

New files: `tex_basic_ctor.cpp`, `tex_txi.cpp`, `tex_replace.cpp`,
`tex_tile_replace.cpp`, `tex_mem_check.cpp`, `mdl_count.cpp`,
`mdl_sync.cpp`, `mdl_neighbors.cpp`, `mdl_area.cpp`, `maxtree_delete.cpp`,
`gob_amputate.cpp`, `gob_ctor.cpp`, `render_shutdown.cpp`,
`ext_loader.cpp`, `aurora_editor.cpp`. Header changes: `aur_texture.h`
(CAurTextureBasic pads typed: m_f18/1c/24..30, Vector m_v4c, m_nc6/c8/ca,
m_nd4, m_bd8/d9, m_bde/df; CAurFontInfo fields + inline ctor under
G_AUR_FONT_INFO_INLINE_CTOR; sized default list ctor under
G_EXO_LIST_SIZED_DEFAULT; SafePointer::operator= out of line under
G_SAFE_POINTER_ASSIGN_OUT_OF_LINE (+ inline Clear) or __forceinline under
G_SAFE_POINTER_ASSIGN_FORCEINLINE; Remove __forceinline under
G_EXO_ARRAY_LIST_REMOVE_FORCEINLINE; CAurTexture slots 0x90/0x94/0xa0/0xa4
= GetTxiDataSize/GetTxiData/SetTxiData/SetTxiLoaded; procedure slot 0x0c =
ParseField), `aurora.h` (CAurora / CAurInternalGL slot 0x10 InitEditor).

New exact (8):
- CAurTextureBasic::CAurTextureBasic 0x423150, CAurTextureBasic::ParseField
  0x422390, AurTextureReplaceSingleReference 0x4235f0.
- MdlNodeCountNodes 0x438400, MdlNodeBuildMap 0x439a10,
  MdlNodeTriMesh::BuildNeighborLists 0x4386b0 (name not recorded).
- Gob::Gob 0x449440, RenderShutdown 0x432180.

Retained NONMATCH (do not retry immediately):
- CAurTextureBasic::Init 0x422af0 (25): cl keeps a zero register (ebx) in
  the .txi branch, so ebp is pushed in the prologue instead of only around
  the stored-TXI loop; the original also pushes the SEH handler after
  reading fs:[0].
- AurTextureReplaceTileReferences 0x4237e0 (115): register/spill tie
  (zero in ebp, frame 0x20) plus layout of the found branch.
- AurTextureMemCheck 0x421ac0 (66): pMem edi vs ebp swap only.
- MaxTree::operator delete 0x44aec0 (134): the inlined MdlNodeDelete
  double type read, same issue as ~MdlNode 0x44a040.
- MdlNodeTriMesh surface area 0x4388a0 (80): x87 scheduling.
- Gob::Amputate 0x44b670 (122): stack list kept in ebp/edi in the original.
- LoadExtensions 0x436490 (582, a 4-byte shift after +0xe0a): 28
  capability checks and 201 entry points match (generated from the
  disassembly); cl hoists the first strlen setup of both vendor arms above
  the branch.
- CAurInternalGL::InitEditor 0x44e3b0 (90): only return-path layout /
  epilogue duplication differs.

## Lessons

- SafePointer assignment: route the back-reference calls through inline
  SafePointerTarget::AddRef/RemoveRef helpers (target+4 list); calling
  `m_p->m_lRefs.Remove(this)` directly swaps `lea ecx` / `push` order.
- `delete[]` declared `throw()` removes the EH state stores but keeps the
  frame: that is how the member-list dtors with an "empty" SEH frame match
  (NodeVertexAnim::~NodeVertexAnim).
- A tail `jmp` to the base InternalRemoveTemporaryArrays comes from a plain
  last-statement call of a non-inline member in the same TU.
- An inline `strcmp(s, "NULL")` compiles to `repe cmpsb` with ecx=5.
- MSVC folds `x * (1.0f/255) * 255.99f` into one multiply; storing the
  first product in a named float keeps two fmuls.
- 2D array indexing (`aRamp[c][i]`) instead of a walking pointer fixed the
  store/cmp order in SetGamma.
- For `new T(args)` where args have side effects (AurPrintf), write the
  argument expressions inline in the new-expression: the original computes
  them after operator new.
- Returning bool (not unsigned char) gives `xor al, al` fail paths plus the
  `mov eax, 1` / `xor eax, eax` compare tail; separate `if` returns put the
  fail block right after the test.
- A zero-and-size helper `ClearDevMode()` (inline) let the dmSize stores
  sink past the EnumDisplaySettings pushes; direct statements did not.
- Bit-field getters returning unsigned char reproduce the shr/test per-bit
  pattern (Scene::HitCheckGob).
- Hidden-function scan: decode each function to its final ret/jmp and look
  for 16-aligned code before the next functions.csv start; vtable dwords in
  .rdata identify the class (scratch scripts dz.py / vt.py / hidden.py).

- Corner loops `for (i < 4)` with `(i == 0 || i == 1) ? 0 : w` and
  `(i == 0 || i == 3) ? h : 0` inline in the call arguments reproduce the
  originals directly. Loop counter type matters: `jb` = unsigned, `jl` = int
  (overbrighten uses unsigned, the others int).
- Nested ternary `(i==0||i==1) ? (bStatic ? 0 : 7.5f) : 0` gives the
  "store 0; jne; store 7.5" shape; `&&` does not.
- `void* hRC = g_hGLContext; void* hDC = g_hGLDC;` at the top reproduces
  the early load of the main context into callee-saved registers.
- Ordinary `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)` gives the
  302h/303h immediates; `_glBlendFunc(0, 1)` (0x401670) is a real call in
  FrameBufferModifications2.
- Late byte stores move with the source: in 0x437520 `g_bBloomRendered = 0`
  sits before the final matrix pops.
- A 64x64x4 byte image written as `unsigned char a[64][64][4]` with
  `a[r][c][k]` stores gives the original's +1-biased pointer (`lea esi,
  [esp+15h]`, stores at -1..+2); a flat pointer walk does not.
- A function-scope static `new T[n]` reproduces the -1 SEH frame; copy the
  static into a local after the early return to get the late spill.
- Inline `operator-(a, b) { return Vector(a.x - b.x, ...); }` evaluates x
  first, but `Vector(p.x - q.x, ...)` written directly as a call argument
  evaluates z first (right-to-left): both forms occur in Part::GetLocal.
- Byte-only type tests (`m_nType & 0x21`) narrow to `and al`; for 0x221 the
  original's word ops come from `short f = m_nType; f &= 0x221;`.
- When a helper defined in the same TU gets inlined (Gob::SetScene into
  ProcessStaticObject), move the caller to its own TU.
- Local float compares `f <= g` against a global give the original's
  `test ah, 41h; jp/jnp` pairs directly.
- Hidden-function scan: scripts hid2.py (walks every known start,
  repeatedly, and ignores later jumps), peek.py and refs.py in
  /private/tmp/claude-501/swk-g-s3/; vtable dwords identify the slot.
- 0x426054 in functions.csv is the tail of RenderSquares 0x426000 (no
  prologue, ends with the tail jmp to glEnd); it is not a real function.

- The sized list ctor with a dead `new` also fixes an inlined `new`ed
  object's lists: Gob::Gob matched once CAurObjectLightContainer's two lists
  used it (it supplies the `mov byte [state], 9` after the new and, oddly,
  also fixed a strcpy operand order and a lea/push order elsewhere).
- `push esi / push edi` inside one branch can come from the `repe cmpsb`
  of inline strcmp, not only from an inlined helper (ParseField).
- Functions with EH states (a `new`) are never inlined, even with
  __forceinline.
- For a dot product written as `-(v * n)` vs `-(n * v)` the operand order of
  the last fmul flips; a reference local for the vertex plus `v0 * normal`
  matched BuildNeighborLists. A user-defined Vector::operator= made the
  NormalOf result store evaluate its destination first.
- Two inline-recursion depths: MdlNodeCountNodes needs inline_depth(10),
  MdlNodeBuildMap inline_depth(5) (its GetName is defined inline in the
  TU; the deeper copies call it out of line). A `bFound` flag (not
  `j == num`) gives the fall-through "not found" block.
- Pass-through Vector temporaries: `m_v = Vector(1, 1, 1)` gives the
  stack temp + copy pattern; when the z store sinks late, move the
  assignment later in the source (Gob::Gob m_vSelfIllum).
- Long straight-line GL/WGL setup (LoadExtensions, RenderShutdown) can be
  generated from the disassembly with a script (genext.py in
  /private/tmp/claude-501/swk-g-s5/); strings and mask globals come from
  the push / or patterns.

## Next in-range queue

Done since the s3 queue: 0x423150, 0x422390, 0x4235f0, 0x438400, 0x439a10,
0x4386b0, 0x449440, 0x432180 (exact); 0x422af0, 0x4237e0, 0x421ac0,
0x44aec0, 0x4388a0, 0x44b670, 0x436490, 0x44e3b0 (NONMATCH).

- Aurora: CAurInternalGL::Initialize 0x44dab0 (slot 0x08, same window /
  pixel-format / vertex-program shape as InitEditor 0x44e3b0; expect the
  same epilogue-layout issue), FuncInterp 0x44c1f0, 0x44ff50 (tile).
- Model post-process: 0x43cf00 / 0x43d990 (InternalPostProcess with a big
  inlined vertex-buffer fill), 0x43a120, 0x43b380, 0x438fa0, 0x43c400,
  0x43a820, 0x437db0; node vertex generators 0x439df0, 0x43bd40, 0x43d450.
- Gob: EnableRenderBumpedOut 0x448130 (TXI scan like Init, inline helpers),
  ReattachReference 0x446ee0 (x87 quaternion arc), 0x440cf0 / 0x441570 hit
  checks.
- Hidden Part slots (x87 heavy): 0x440960, 0x440ff0, 0x4429d0, 0x443330;
  Part::GetMinimumSphere 0x443080; Scene: 0x44f8d0 SpawnRooms, 0x44f5a0,
  0x44f470.
- Framebuffer / shaders: 0x427c90, 0x4293f0, 0x4299c0, 0x42e600
  (EnableCombiners, 13 KB of combiner setup: a generator like genext.py
  may work), 0x434890; not attempted: 0x425d80 / 0x425ea0.
