# Worker d notes (bucket 0x00400000-0x0041FFFF)

Older history for this bucket (rounds 1-9 in the Kotor RE checkout):
`~/Kotor RE/docs/batches/worker-d-notes.md`.

## Round s2 (2026-10-27)

Folder `matching/src/d`: 634/702 -> 651/708 (17 new exact: 3 new
annotations, 14 promoted NONMATCH attempts; 3 new NONMATCH annotations).

### New exact matches (17)

- movie_player.cpp: 0x00405a50 ~CExoMoviePlayerInternal, 0x00405b30
  CExoMoviePlayerInternal::ThreadProc (not a function start in
  functions.csv: it is merged into the 704-byte 0x00405a50 entry).
- aur_texture.cpp: 0x0041fec0 convertHeightFieldToNormalMap (name from the
  playable build's comment).
- exo_res_man.cpp: 0x00406540 CExoResMan::FreeResourceData.
- gui_parts.cpp: 0x00416710 CSWGuiScene::RemoveModel, 0x00416750 GetModel,
  0x00416de0 CSWGuiImageParams::operator=, 0x004150c0 / 0x00415000
  CSWGuiBorderParams::operator= / ctor, 0x004171e0 CSWGuiTextParams::operator=,
  0x00417050 CSWGuiTextParams ctor, 0x00415f20 SetAlignment, 0x00417310
  CSWGuiText::Initialize, 0x00414f10 CSWGuiText::GetTextExtent, 0x00417460
  ~CSWGuiEditText (now compiler-generated, SYMBOL annotation).
- gui_control.cpp: 0x0041c740 ~CSWGuiEditbox.
- gui_manager_draw.cpp: 0x0040ae80 CSWGuiManager::DrawPCBG.

### Retained NONMATCH attempts (new this round)

- 0x0040b060 CSWGuiManager::ActivateSoftwareMouse (gui_mouse.cpp, 13): only
  the camera call; the original loads the camera vtable after building the
  Vector argument. gui.h now has a minimal CSWGuiScene / CAurCamera and
  m_pMouseObject is a CSWGuiScene*; ActivateSoftwareMouse returns int (1/0).
- 0x004053e0 CExoMoviePlayerInternal::OpenMovie(const CExoString&) (65):
  body and calls line up; the original keeps the constant 1 in edi from the
  entry block and its frame is 4 bytes larger.
- 0x0041dd30 CTlkTable::ParseTokens(CExoString*) (42, renamed from ParseStr
  to the symbols.csv name): the original evaluates `sText + nTokenStart`
  separately per branch (cl hoists it above the '>' scan) and spills the
  token array / hash.
- 0x0040bec0 DrawSafeRegion now 3 (frame fixed; `add eax, 0x50` schedule).

### Lessons

- `p ? p - 6 : p` style pointer selects give `add eax, -6`; an if/assign of
  `p - 6` gives `sub eax, 6` (FreeResourceData: ternary inside delete[]).
- A float member copied with fld/fstp (not mov) means the source read it
  through an inline getter returning float (GuiParams operator= family).
- Block-scoped named locals ({ CResRef c(...); f(c); }) share frame slots
  with earlier temporaries; function-scope ones do not (TextParams ctor,
  DrawPCBG, DrawSafeRegion extents, ActivateSoftwareMouse).
- `xor eax,eax; mov al,[x]; and eax, 0xffffff3f` is a 6-bit bitfield passed
  to an `unsigned char` parameter (CAurGUIString::SetAlignment).
- A derived dtor without the vfptr store is compiler-generated: delete the
  user-declared empty dtor and annotate with SYMBOL. To have it inlined in
  another TU, its first use (e.g. a ctor's unwind path) must sit in an
  auto_inline(on) region (CSWGuiEditbox ctor/dtor in gui_control.cpp).
- A register holding 0 across calls for a result field = the result object
  default-constructed at the top (GetTextExtent `CSWGuiExtent r;`).
- `#pragma auto_inline(off)` at file top also stops in-class operator[] of
  a template list from inlining; use `.element[i]`.
- `(fInv * -fDX) * 127 + 128` keeps fchs; `-(fInv * fDX) * 127 + 128` is
  folded into fsubr.
- A loop that re-uses a list element pointer in a call (OpenMovie(sMovie))
  wants a `CExoString& sMovie = list[j]` local (ThreadProc).
- Store order of bitfield members follows assignment order with the other
  scalars (BorderParams ctor: m_fEdgeScale before m_b2).

### Tried without success

- 0x00414cd0 GetCornerSize (11), 0x004187c0 HitCheckMouse (CSE of left),
  0x0041ad40 Button::HandleInputEvent (cl cross-jumps the two GetPanel
  calls), 0x0040b870 OnPanelAdded (the original keeps a store cl proves
  redundant), 0x0041be40 ListBox ctor (flag store schedule), 0x004065a0
  GetResRefFromFile (temp slot sharing), 0x004089f0 Demand / 0x00407020 Free
  (unthreaded `je`), 0x0040d500 Hash, 0x0041e9c0 AurTextureSortOrdered.

### Next in-range queue

- Todo (large): 0x004094a0 (CExoResMan async module RIM loader, 1632 B; the
  CResRef local at [esp+0x40] is assigned with operator=(CExoString) without
  a ctor and later constructed in place by CResRef(const CExoString&); the
  GetKeyEntry out-pointers swap slots between queries), 0x00403800
  ReInitAurora, 0x004041f0 WinMain, 0x004024f0 (+ WndProc 0x00402800),
  0x00401a30, 0x00408e90 (aligned frame, directory scan), 0x0040f3c0 /
  0x0040fb80 (CExoKeyTable, conditional `new CExoFile` temporaries),
  0x004143b0 C2DA::Load2DArray.
- Near misses: 0x0040bec0 DrawSafeRegion (3), 0x0040b060 (13), the gui
  HandleInputEvent family, 0x0041f9b0 LoadImagePLT (dead reload of the
  loader pointer), 0x0041f570 / 0x0041fa30 (byte store schedule).

## Round s1 (2026-10-27)

Folder `matching/src/d`: main 620 MATCH -> 634/702 (the cut-off WIP of the
previous s1 worker compiled to 623/683 once finished).

### New exact matches (14)

- GUI controls (previous worker's WIP, finished): 0x00418840
  CSWGuiControl::Load(owner, gff, list, tag), 0x0041c400
  CSWGuiListBox::HandleMouseMove, 0x00418d00 CSWGuiLabel::SetEnabled. The
  naked-asm 0x004184b0 CSWGuiEditbox::SetEnabled file was deleted and replaced
  by C++ in gui_control_draw.cpp (still MATCH).
- gui_listbox.cpp: 0x0041a2d0 CSWGuiListBox::OrganizeOversized, 0x0041d3e0
  CSWGuiListBox::LoadProtoItem (descriptive name).
- gui_control.cpp: 0x0041cc30 CSWGuiSlider::Load.
- app_resolution.cpp: 0x00403cf0 ChangeResolutionForMovie, 0x00403de0
  RestoreResolutionFromMovie (descriptive names).
- app_window.cpp: 0x004036f0 InitRenderWindow, 0x00403f20 InitApplication.
- app_console.cpp: 0x00401870 HandleFrameInfoKey, 0x00401f00
  HandleFrameInfoChar (command line / history / tab completion).
- app_keys.cpp: 0x00402240 HandleKeyDown (capture event, EnableCheats toggle).
- tlk_table.cpp: 0x0041e5a0 CTlkTable::OpenFile (StringTokens.2da, insertion
  sort by CRC).

### Retained NONMATCH attempts (new this round)

- 0x0040ae80 CSWGuiManager::DrawPCBG (2 insns + frame 12 bytes larger: the
  two Vector temporaries do not share a stack slot; `add eax,0x70` before the
  m_nStyle write suggests an inline params setter).
- 0x0040bec0 CSWGuiManager::DrawSafeRegion (4, same Vector-temp frame issue).
- 0x0040c1e0 CSWGuiManager::HandleMouseMove (7): only the parent walk loop;
  the original is not rotated (test at the top), every while/for/goto form
  tried is rotated by cl.
- 0x0040c8e0 CSWGuiManager::HandleInputEvent (132, 896/880): structure and
  jump tables right; cl keeps bDown in ebp and spills the loop index, the
  original spills bDown (param slot) and keeps j in ebp.
- 0x00418a90 CSWGuiControl::DisplayToolTip (17): only the GetINTEntry
  argument scheduling (original loads g_nFirstKeyIndex into edi first).
- 0x00419ee0 CSWGuiListBox::OrganizeUnequal (23): `lea ecx,[eax+eax]; mov
  edx,ecx` width computation (3 bytes) shifts the rest.
- 0x0041b140 CSWGuiListBox::OrganizeControls (45): one hoisted
  `movsx edx,ax` (CSE of m_nVisible between the bFlag9 / else branches).
- 0x0041d5b0 CSWGuiListBox::Load (12): eax/edx choice in the m_bFlag6
  bitfield write.
- Kept from the WIP: 0x00419060 HitCheckSlider (9, return-1/return-3 tail
  merge direction), 0x004197c0 HitCheckScrollbar (96, x/y register tie).

Tried again without success: 0x00407020 CExoResMan::Free (0 insns, one
non-threaded `je` of the inlined GetHead ternary).

### Lessons

- A global array next to scalar globals that the original reloads after
  every char store is one global struct: g_szFrameInfo and the cursor at
  0x007a3a40.. modelled as `CFrameInfoLine` made the stores/reloads of
  HandleFrameInfoChar match (separate externs let cl drop the clamp store).
- `&CSWGuiTextParams()` as an argument (temporary whose ctor result is kept
  in ebx) instead of a named local fixed DrawSafeRegion/DrawPCBG ordering.
- Named CResRef locals construct before temporaries in the same call;
  temporaries in an argument list are built right-to-left.
- A local `CExoArrayList` with an inline dtor `if (element) delete[] element;`
  inlines; the d/exo_key_table.h one (which also zeroes element) is called
  out of line.
- `if (!h) return; g = h;` instead of `if (h) g = h;` moved the EH-state
  store after the branch (InitRenderWindow).
- The inline `GetClientExoApp()` getter trick again fixed an eax/edx choice
  (HandleKeyDown).
- Calls whose result feeds an argument list that also has constants: the
  original evaluated the call first when it was a separate statement
  (`char* s = ConsoleExecute(..); AurPostString(s, ...)`).
- functions.csv merges 0x00401870 with a second function at 0x00401a30
  (splash/window thread, ret 4) and probably 0x004024f0 with WndProc
  0x00402800; 0x00401870 still compares OK.
- gui.h DisplayToolTip/ChangeToolTipText/SetText now take const CExoString&
  (they were `int`), matching the callers' pointer pushes.

### Next in-range queue

- 0x00403800 ReInitAurora (1264 B, Win32 + window recreate), 0x004041f0
  WinMain (2288), 0x004024f0 (4608, contains WndProc 0x00402800),
  0x00401a30 (not in functions.csv), 0x0040b060 ActivateSoftwareMouse,
  0x0041dd30 CTlkTable::ParseStr (960), 0x00405a50 / 0x004053e0 movie,
  0x00408e90, 0x004094a0, 0x0040f3c0, 0x0040fb80, 0x004143b0 Load2DArray,
  0x0041fec0 (x87).
- Near misses worth another pass: DrawPCBG/DrawSafeRegion (frame), the
  listbox Organize pair, DisplayToolTip.
