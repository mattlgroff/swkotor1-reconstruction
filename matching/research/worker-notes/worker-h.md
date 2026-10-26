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
