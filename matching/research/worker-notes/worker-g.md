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
- 0x426054 in functions.csv is the tail of RenderSquares 0x426000 (no
  prologue, ends with the tail jmp to glEnd); it is not a real function.

## Next in-range queue

- Gob tree walkers with inlined list growth: 0x44b670, 0x446ee0, 0x448130,
  0x449200, 0x449cc0, 0x449440 (Gob::Gob), 0x4448c0; recursive part walkers
  0x440550, 0x442d70, 0x4434a0, 0x440cf0 (HitCheckAnimMesh), 0x441570,
  0x443080.
- Texture: 0x422390 CAurTextureBasic::ParseField, 0x422af0 Init,
  0x423150 ctor, 0x4235f0, 0x4237e0, 0x421ac0 (x87), 0x420030.
- GL upload helpers 0x423f90-0x424dd0, 0x425520, 0x4260a0, 0x4263c0,
  shadow arrays 0x426660 / 0x4269f0, 0x426cc0, 0x427490.
- Model post-process: 0x4386b0, 0x4388a0 (x87), 0x438400, 0x439a10,
  0x43f270, 0x44aec0 (MaxTree::operator delete).
- Aurora: 0x44dab0 (Initialize), 0x44e3b0, 0x44fac0, 0x44ff50, 0x432180.
- Saturation NV 0x434890 (same x87 problem as 0x4343d0), pixel-shader
  switches 0x4293f0 / 0x4299c0, 0x42e040, 0x42e600, 0x436490.
