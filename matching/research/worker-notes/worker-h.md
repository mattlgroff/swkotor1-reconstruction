# Worker H notes

## Claude continuation, 2026-10-26

- Range 0x00450000–0x0047FFFF. The canonical todo report had only 60
  candidates; eight are compiler fragments and almost all remaining listed
  functions are at least 528 bytes. A broader native-entry scan of the modern
  reconstruction found additional real functions inside coarse function-index
  extents. Their entry points and bodies were checked against the purchased
  executable; none was already annotated in the status report.
- Attempted 17 additional entries; 17 matched in their translation units.
  New camera functions: 0x0045BF60 EndViewAngleAnimation, 0x0045BFA0
  SetViewPort, 0x0045BFE0 HitCheck, 0x0045C060 SetOrientation, 0x0045C470
  GetOrientation, 0x0045C4A0 GetViewPortHeight, 0x0045C4B0 GetClipDist,
  0x0045C4D0 GetViewAngle and 0x0045D760 GetViewVolume.
- New Scene methods: 0x00453E00 RenderGobFirst, 0x00453E20 RenderGobLast,
  0x00453E70 RenderGobAfter, 0x00453EC0 RenderGobBefore, 0x00458B40
  SetFogColor, 0x00458B60 GetFogRange, 0x00458B80 SetFogRange and 0x00458BC0
  IsGeomFadeEnabled. Names without independent symbol evidence are descriptive.
- Camera has CAurCamera at +0, Gob at +4, and its Gob quaternion at Camera
  +0x88. GetViewVolume returns a Plane by value from the list at +0x1F8.
  HitCheck explicitly forwards to Gob::IntersectLine at 0x0043EB80.
- The Scene Gob ordering list is at +0x54. Inline IndexOf with an early return
  plus an explicit list pointer local reproduces the native loop and early
  branch; initializing an index then breaking produced 10 differences.
  Out-of-line Remove and Insert map to 0x00445080 and 0x00445020.
- For the bool fade query, `condition ? true : false` emits `cmp; sete al`
  rather than loading into a register and clearing eax.
- Next: inspect other native entry points hidden in the long Scene and Camera
  extents before attempting remaining large listed functions. Existing close
  attempts and limits are in the old Kotor RE worker-h notes.

## Follow-up checkpoint

- Ten further exact entries: 0x00458BA0 Scene::SetSceneFocus,
  0x0045C4E0 Camera::SetFadePlanes, 0x0045C530 Camera::ViewVolumeSet,
  0x0045C030 SetPosition, 0x0045C450 GetPosition, 0x0045C440 AsAurObject,
  0x0045C160 SetAttachmentLight, 0x0045C210 AttachToPart, 0x0045C220
  GetAttachment, and 0x0045C230 SetAttachment. Their forwarding target names
  and prototypes come from matched Gob functions in symbols.csv; several
  labels in the modern Camera reconstruction use generic behavior names.
- Attempted 0x00450270 GobOrderCmp (70 differences, register allocation and
  branch placement) and 0x004738C0 PartLight::Mat (19 differences, x87 vs
  direct argument loads). Both are genuine native entries, retained NONMATCH;
  the labels for local view types and the associated globals are descriptive.
- The native-entry scan yielded about 162 candidate addresses before filtering
  for internal comments and labels; inspect each boundary and status before
  annotating. Only 60 addresses appear in the coarse todo report.

## Camera view and follow

- Additional exact: 0x0045BEC0 SetClipDist, 0x0045BEE0 SetViewAngle,
  0x0045C0A0 AttachToScene, 0x0045C200 AttachToObject, 0x0045C0C0
  SetFollow. SetFollow uses the known 0x0049FE80 camera-follow constructor
  with a 0x70-byte allocation and the Camera +0x7C virtual attachment slot.
  The exact relationship between CAurBehavior and the GobAttachment name
  used by existing matching files remains a naming question; review before
  treating their C++ declarations as a linked class hierarchy.

## More hidden entries

- Exact individually: 0x00450970 Scene::DoStaticMeshBeams (global list at
  0x007FBF6C and Part Draw slot +0x64), 0x00458BD0 Scene::AddObject (tail
  forwarding via ChildAdd virtual slot +0xB0), 0x00458B30 Scene::SetFog,
  and compiler-emitted 0x0045C250 CAurCamera scalar deleting destructor.

## STL generated entries

- Exact individually: 0x0047DDC0 `std::vector<unsigned short>::_Buy` and
  0x0047E620 `std::vector<NvStripInfo*>::vector()` using the existing VC7
  STL instantiations in nvstrip_stl.cpp. The first has an existing symbol-map
  reference; the second is the element constructor used by the vector array
  per the native EH-vector construction context.

- 0x00456B30 CHitInfo::~CHitInfo matches using the existing member destructors
  from h/scene.h. Direct native disassembly shows the SafePointer removal and
  list free, with return at 0x00456B57. The Mac name table instead labels
  0x00456B30 as Scene::HitCheck; that method starts at 0x00456B60 in the
  native executable, so do not transfer the Mac label without correcting its
  candidate boundary.

- Promoted 0x004738C0 PartLight::Mat from 19 differences to MATCH. An inline
  `float ReadColor(const float&)` for the green and blue components emits the
  original x87 stack argument loads; a direct read emits integer pushes.

- 0x004568D0 Scene::LoadVisibility now matches. It clears each room's
  `m_lVisible.num` (+0x60), opens the `.vis` resource, and parses room and
  neighbor names before setting visibility. Both name buffers are 0x40 bytes;
  declare the second-name buffer first to match the native stack layout.
  Explicitly assign `AurResGetNextLine()` to a local before the second
  sscanf, and retain the `room && count != 0` check before the inner loop.
  Resource helpers use existing 0x0044C740 / 0x0044BFA0 / 0x0044C8F0
  declarations, and SetVisibility is the already-matched 0x00454940 method.

## H s2: shader constants and native exception methods

- Screened 109 source-identified native H addresses plus 58 indexed todo
  entries. 0x0047A280 and 0x0047A2A0 are exact 32-byte ATI fragment shader
  constant setters, using the existing 0x004AAAB0 color helper and native
  glSetFragmentShaderConstantATI pointer at 0x007BB5D8.
- Scanned 804 emitted stock MSVC7 STL methods against the native code. Added
  0x0047CE00 std::runtime_error::what and 0x0047DA80 std::out_of_range copy
  constructor; the full STL translation unit matches 55/55. Native vtable
  0x0075FADC points to 0x0047CE00, its RTTI identifies std::out_of_range, and
  the constructor calls the runtime_error copy constructor at 0x0047D220.
- Tried source-shape variants for prior NONMATCH OpenGL capability queries
  0x0045F560 and 0x0045F770; byte output did not improve, so prior code is
  unchanged. The 0x00456B60 scene collision WIP remains in the prior worker
  branch because its global at 0x007B93A8 overlaps the existing mini-game hit
  output and requires a shared-storage model before it can be safely accepted.
- 0x0047A2C0 ReportARBError also matches, using native glGetString and
  glGetIntegerv imports. The next real function starts at 0x0047A2F0 inside
  the coarse function-table extent, verified in native bytes.
- 0x0046F1A0 PartTriMesh::WithinRange is retained NONMATCH (1): equivalent
  x87 code differs only in whether stack cleanup occurs before or after the
  addition. The 2.0f constant was read directly from the original image.

## H s1 (Mac): hidden Scene/Camera entries

- Folder: 406/447 -> 440/486. 39 annotated, 34 NEW exact, 5 NONMATCH.
  None of the 39 addresses is in functions.csv (all are native entries
  inside coarse extents, checked against the purchased executable);
  functions.csv was not edited.
- Camera (camera.cpp): Camera's Gob-side vtable 0x00741988 holds 13
  `sub ecx, 4; jmp` adjustor thunks. Declaring the shared methods as
  virtuals of both CAurCamera and Gob makes MSVC emit them with the
  vtable (`?X@Camera@@W3AE...`, `??_ECamera@@W3AEPAXI@Z`); annotate with
  SYMBOL. Exact: 0x0045C3A0..0x0045C420 (9), 0x0045D7C0..0x0045D7F0 (4),
  plus Gob-introduced overrides 0x0045BEA0 Camera::Render (Gob slot +0x10,
  `this` arrives as Gob*) and 0x0045C430 AsAurCamera (`lea eax,[ecx-4]`,
  Gob slots +0x04 and +0x160 share it).
- Scene passes (new scene_buckets.cpp, local minimal Scene with vtable
  0x00741708 slots): exact 0x00450150 DoBinBucket, 0x004503A0
  DoDistortionBuckets, 0x00450440 DoLightSaberBucket, 0x004504D0
  DoBlurBuckets, 0x00450550 DoGobShadowsForThisLight, 0x00450730
  RenderStencilShadowsForThisLight, 0x004509B0 DoEmitterBucket, 0x00450AC0
  DoTriggerBucket, 0x00450B50 RenderDynamicGeometry, 0x00450BB0 (slot +0xCC,
  named RenderTransparentGeometry; playable calls it RenderMotionBlur),
  0x00450C30 RenderStaticGeometry, 0x004514F0 RenderSinglePass (1376 B),
  0x00451A50 RenderShadows, 0x00457B90 DoGobBuckets.
- Image header readers (image.cpp): exact 0x0045E010 ImageRead16bitHeader,
  0x0045E180 ImageReadS3TCHeader(char*, ...) (overload of the FILE* one at
  0x0045E0F0), 0x0045E440 ImageReadHeader.
- Texture controllers (texctl_perlin.cpp): exact 0x0045FE50
  PerlinTextureController::BuildPerlinImage, 0x00462800
  RandomTextureController::Update. texctl.h gained AurTexture GetData (+0x3C),
  UpdateImage (+0xAC), inline GetPixelCount/GetImageSize,
  TextureController::DistortImage (+0x14, 0x00460D70, was v14) and
  Height2Channel (0x00461060). texctl.cpp still 22/24 as before.
- NONMATCH kept: 0x0045D300 Camera::HitCheckViewVolume (8, lea ecx/eax and
  copy register swap), 0x0045CF30 Camera::MouseDir (12, viewport copy order;
  x87 part matches), 0x0045FD70 TextureController::Distort (19) and
  0x0045FDD0 PerlinTextureController::Init (4) (both: dead `push ecx` spill of
  m_pTexture, same unsolved pattern as CycleTID Update 0x00460970),
  0x004512D0 Scene::Render (51, only the stats max block: native sums
  `lea eax,[B+A]; add Bind` and compares the peak from memory).

### Lessons

- Overloaded virtuals are laid out in reverse declaration order; declare
  `EnableHighestPriorityLights(int,int)` (+0x24) before `(int)` (+0x20).
- __int64 timer globals 0x007B9280/88/90/98: declare them `static` in the
  TU. MSVC then knows `this` members cannot alias them and delays the high
  dword store past member loads exactly like the original (fixed
  RenderStaticGeometry's 3-insn tie and Scene::Render's prologue).
- Constant folding: `a * 0.5f * 255.0f` becomes one fmul by 127.5; write
  `255.0f * (a * 0.5f)` to keep both fmuls.
- Function-local statics (`static Vector s_v(0,0,0); static Quaternion
  s_q;`) reproduce the guard-bit blocks in DoGobBuckets; `static T* p = new
  T[n]` gives the EH-framed guard in RenderStencilShadowsForThisLight.
- Model flags at +0x50 need `unsigned short` bitfields plus an inline
  `unsigned char Get()` accessor for `mov cl,[]; shr cl,2; test cl,1`; a
  char bitfield or mask gives `test byte ptr [],4`.
- `for (r) { row = base + r*size; ... }` reproduces the strength-reduced row
  pointer kept in the argument slot (BuildPerlinImage).
- Hoisting an inline soft-shadow test into `static __forceinline bool`
  is required; plain `static inline` was emitted out of line.
- `if (n > 0) flags = 0x3f;` instead of a ternary avoids setcc/dec/and.

### Next in-range queue

- Scene: DoMeshBuckets 0x00455710 (EH, local list copy), DoGobShadows
  0x00453950, DoDynamicMeshProjections 0x00453BB0, DoFadeBuckets 0x00455B60,
  DoHighlightTriggers 0x00456300, RenderLensFlares 0x00457EC0 — the
  scene_buckets.cpp Scene/Gob/SceneLights declarations cover most of their
  callees. Scene::Render stats block (see above).
- Camera: MapToScreen 0x0045D040, HitCheck(x,y) 0x0045CC90,
  SceneBoundaryCheck 0x0045D490, Animate 0x0045D220 (x87 heavy).
- Texture controllers: Life Init/Update 0x00460120/0x00460250, Arturo
  0x00460560/0x00460690, Ring Update 0x00460A90, Perlin Update 0x004615A0
  (x87 heavy). Gob::Render 0x00478260 (EH, 1008 B).

## H s3: model parsers, recursive tree walkers

- Folder: 448/502 -> 459/521. 19 annotated, 11 NEW exact, 8 NONMATCH.
  Hidden entries (inside coarse functions.csv extents, checked in the
  purchased exe; functions.csv not edited): 0x0045CC90, 0x0045D040,
  0x00460D70, 0x00473EA0 has its own todo entry. 0x004741A0 and 0x0046D2F0
  are real starts shorter than their coarse extents.
- Exact: 0x00465560 MdlNode::InternalParseField, 0x00469150
  MdlNodeLight::InternalParseField, 0x004658B0
  MdlNodeEmitter::InternalParseField (10 KB, all MDL_FIELD /
  MDL_CONTROLLER macros) in mdlnode_parse.cpp; 0x00463C20 Parse_AABB
  (aabb_parse.cpp); 0x004531A0 Scene::ListSpecialPartsR and 0x00453550
  Scene::UnlistSpecialParts (scene_special.cpp, inline_depth 6);
  0x00454470 ExpandReferenceParts (scene_reference.cpp, inline_depth 7);
  0x0046D2F0 SceneBSPRemoveTiledata (scene_tiledata.cpp); 0x00452000
  findnode and 0x00452500 SetPartTextureRotate (tile_tree.cpp,
  inline_depth 9); 0x00473EA0 Gob::PartDraw (gob_partdraw.cpp,
  inline_depth 2).
- NONMATCH kept: 0x0045D040 Camera::MapToScreen (36, viewport-copy
  prologue only, same as MouseDir), 0x0045CC90 Camera::HitCheck(x,y) (56,
  orthographic path keeps offsets on the x87 stack), 0x00469700
  MdlNodeTriMesh::InternalParseField (280, this/s register swap around the
  lightmap block), 0x00465320 AddBezierController (91, same pattern as
  AddGenericController), 0x0045FF40 WaterTextureController::Init (2, imul
  operand register), 0x00451DC0 ComputePartBBox (16, one `add esp`
  scheduled later), 0x004741A0 ProcessFaces (5, original keeps the counter
  in the dead third-argument slot), 0x00460D70
  TextureController::DistortImage (42).
- MdlNode InternalParseField returns void in the original (no eax on the
  parent-NULL path); modelparse.cpp still declares int, harmless there.
- texctl.h: WaterTextureController +0x4c/+0x50 phases, +0x5c/+0x60 byte
  images, +0x64 counter; Vector gained operator/= (0x004A97D0).

### Lessons

- Recursion depth: try inline_depth 2..9; the matching depth equals the
  number of inlined levels + 1 for some functions and differs for others
  (findnode/SetPartTextureRotate need 9 to get 6 inlined levels). An
  inline helper used inside the recursion (GetName, _glScalef) is expanded
  only in the outermost level, exactly like the original.
- Locals packed into a dead parameter home slot (emitter flags, renderorder
  temp): put the address-taken locals in their own `{ }` block.
- `Vector c = a + b` with member-wise inline operators: MSVC may commute
  the first add, so try both operand orders per expression.
- Per-branch `p->n = 1 << (c ? 1 : 4);` reproduces tail-duplicated
  returns; a shared `nAxis` variable merges the tails.
- `*ppNode` reload pattern and pass-through pointer: a `(T*)p` reuse of a
  null pointer register keeps `MdlNode* pParent = (MdlNode*)pTree;` exact.

### Next in-range queue

- Scene::Scene 0x00458790 (EH, members already in h/scene.h; zeroing
  order suggests zero-constructing member types).
- 0x0047A2F0 EnableFragmentShaders (switch, integer only, 2.2 KB).
- 0x004680E0 ParseNode (EH, As* destroy chain).
- x87 heavy: Camera::SceneBoundaryCheck 0x0045D490, 0x00452CC0
  AddFacesToBins, 0x00464B50 CompressQuaternionKey, texture controller
  Update/Control 0x004615A0 / 0x00461820 / 0x00462030 / 0x00462520.

## H s4: ATI fragment shaders, Scene ctor, ParseNode, PartAABB draw

- Folder: 459/521 -> 464/528. 7 annotated, 5 NEW exact, 2 NONMATCH.
- Exact: 0x004794E0 InitializeATIFragmentShaders (3488 B) and 0x0047A2F0
  EnableFragmentShaders (2256 B, switch; cases in source order
  0,3,6,13,7,19,20,32,26,22,37,24,29,2) in ati_fragment_shaders.cpp;
  0x00458790 Scene::Scene (scene_ctor.cpp, TU-local zero-constructing
  layout); 0x004680E0 ParseNode (parse_node.cpp, returns MdlNode*);
  0x00471560 PartAABB::Draw (part_aabb_draw.cpp).
- NONMATCH kept: 0x00473900 PartTriMesh::Mat (128 by count, one cause: cl
  keeps the constant 1 in ebx for guard bit / fog flag / bDynamic, the
  original uses immediates; with any one of the two byte stores changed to
  another value the rest is byte-identical); 0x0045B7A0
  CAurGUIStringInternal::GetTextHeight (10; hidden entry, descriptive name;
  the original packs both branches' two list copies into the same two
  stack slots and has `movsx esi,bx; imul esi,eax`).
- functions.csv: 0x0045B7A0 is a hidden entry (not listed); 0x00471560's
  extent is coarse (9056 B, it also covers 0x00471860 PartLightsaber draw);
  0x00473900 is hidden inside an earlier extent. functions.csv not edited.
- Names: shader globals g_nATIShader_<addr>, g_bFSEnvMapOn/g_bFSTexture1On,
  g_nFocusGobCounter, g_bShowWalkMesh, g_bMat* are descriptive.

### Lessons

- A read-loop that the original keeps top-tested with a back `jmp`
  (`call; test; je exit; ...; jmp top`) came from a flag-driven
  `while (!bDone) { if (!line) bDone = 1; else { ...; if (end) bDone = 1;
  else Parse(); } }`; `for(;;)`+break and `while ((p = Next()))` both get
  rotated by cl.
- Shared if/else tail (`glEnable; glBind...`) written once after the
  if/else is tail-duplicated by cl when small; writing it in both arms
  changes the scratch-register rotation of everything that follows.
- `m_v = Vector0(a,b,c)` (derived zero-ctor type with a 3-float ctor, no
  user operator=) gives the interleaved stack-temp copy; a store between
  the two vector assignments in source (`m_v88 = ...; m_bFog = 0;
  m_nMode = 0; m_vFog = ...`) reproduces the delayed last component store.
- Inlined GLRender::Transform: the angle needs
  `(float)(acos(w) * 2.0) * g_fRadToDeg` in the inline copy (the
  out-of-line 0x0046F060 matches without the cast). The inline copy is
  reached through an inline Transform(pPos,pOri,0) wrapper (both leas are
  formed before the g_bTransformParts test).
- A byte global declared file-`static` lets cl schedule its store after a
  following member reload (`mov cl,[m_bFog]; test; mov byte [flag],1; jne`).
- `new Model(...)` with a class operator new maps to AurNew 0x0043E1B0.
- cl does not inline (even __forceinline) helpers that own destructible
  locals, so stack-slot sharing between branches cannot come from that.

### Next in-range queue

- PartTriMesh::Mat 0x00473900: find what removes one use of constant 1.
- 0x00456F30 Scene::SpawnRoom (697 insns, no x87, 0xF8 frame),
  0x0046E5C0 ManageSceneBSP (537 insns, inline list growth, repe cmpsb),
  0x00454C80 spawn tile, 0x00471860 PartLightsaber draw (x87 heavy),
  0x0045A2F0 / 0x004599B0 / 0x0045A850 / 0x0045B290 GUI string layout and
  render (hidden entries, x87), 0x0046D3B0 ProcessParts.
