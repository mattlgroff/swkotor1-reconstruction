# Worker O notes (bucket o, 0x006F0000-0x0073CFFF)

## Survey of the range (round s1)

- 0x006F0260-0x006FA390: character-generation / level-up GUI panels:
  CSWGuiFeatsCharGen, CSWGuiPowersLevelUp, CSWGuiAbilitiesCharGen,
  CSWGuiSkillsCharGen, CSWGuiPortraitCharGen, CSWGuiQuickOrCustomPanel,
  CSWGuiNameChargen, CSWGuiQuickPanel (~110 functions, many 0.2-4.5 KB,
  x87 and EH heavy). Not started.
- 0x006FA390-0x0070C0A0 and 0x00713190-0x00713E80: stock LIBCMT
  (crt_libcmt.csv, hidden by todo.py). A few unmatched CRT-looking leftovers
  (acos/asin/pow x87 shells, _mbs*) need no game source.
- 0x0070C0A0-0x0070C170: std::_String_base::_Xran/_Xlen and import thunks.
- 0x0070C170-0x0070C250: CExoMoviePlayer (pimpl over bucket d's
  CExoMoviePlayerInternal). Done.
- 0x0070C280-0x0070C6C0: CLIP + CResHelper<CResLIP,3004> + CResLIP. Done.
- 0x0070C6E0-0x0070DA60: CAuroraInterface statics (GetRaster /
  GetLayeredRaster / GetResource / Release*, Read*RasterHeader, texture
  packs, FindNearestVideoMode, CExoLinkedList<CAuroraModel> ctor,
  RequestModel, IsResident, ctor/dtor). Raster accessors and the four
  Read*/Get16Bit raster header readers done; GetResource 0x0070C920 onwards
  not started.
- 0x0070DA60-0x00710B30: CAurora* wrappers (Layered/Texture/Visibility/
  Processed/TXI/ModelX/Model/Compressed/16Bit), each a CResHelper<CResX,n>
  subclass. Done except two TPC size computations.
- 0x0070DE20 GetModelResHelper (not started), 0x0070E080-0x0070EB60
  CAuroraInterface Get*/Release*Internal + Get* forwarders. Done.
- 0x00710B50-0x00711070 and 0x007121B0-0x00713170: CRes subclasses PLT, VIS,
  TXI, MDX, DDS, 4PC, LTR, TGA, TPC. Done except the RLE decoders, the PLT
  and TPC OnResourceServiced and CResLTR::OnResourceServiced (0x00712410).
- 0x00711090-0x007121A0: CSWNameGen. Done except the two GetRandomName
  overloads (0x00711390 has missing starts inside its functions.csv range;
  0x007118C0 is 2.2 KB x87).
- 0x00713E90-0x0073CFFF: EH funclets (Unwind@/FrameHandler), static
  CExoArrayList init/dispose thunks, PartSkin. Skip.

## Round s1 results

Files: src/o/{aurora_cres,aurora_textures,aurora_interface,aurora_raster,
res_tga_tpc,lip,exo_movie_player,sw_name_gen}.cpp, include/o/{aurora_res,
aurora_res_helper,aurora_textures,lip,exo_movie_player,sw_name_gen}.h.

203 functions annotated (all NEW; the bucket had none), 194 MATCH.
Missing starts annotated (not in functions.csv): 0x0070C850, 0x0070C870,
0x0070CBD0, 0x0070CC60, 0x0070CD10, 0x0070CDD0, 0x0070CE80, 0x00710E60,
0x00710F30, 0x00711010, 0x00711030, 0x00712200, 0x00712FD0, 0x00712FF0.
Every callee, vtable, global and string of the matched code was checked
against the original with a relocation dump.

NONMATCH kept (best attempts):
- 0x00710C10 CResPLT::ReadRLEPLT, 0x00712A70 CResTGA::ReadColorMappedRLETGA,
  0x00712B00 ReadUnmappedRLETGA (4/4/17): the original tests the RLE flag
  as `and dl,0x80; ...; test dl,dl; je`; every source form tried (unsigned
  char / char / int flag variable, == 0x80, bitfield, masks in place)
  compiles to `test dl,dl; jns`. Rest of the code is identical.
- 0x00710CC0 CResPLT::OnResourceServiced (12): ours keeps 0 in ECX and
  compares memory; the original loads each field and tests it. Not fixed
  by separate ifs, == 0 forms or chained stores.
- 0x0070F440 CAuroraProcessedTexture::Load (33) and 0x0070F690
  GetTextureSize (24): stack-slot order of the GetTPCAttrib outputs
  (width/height swapped) and the loop register picks. Declaration order
  has no effect.
- 0x00712FF0 CResTPC::OnResourceServiced (49): register for the constant 1
  (original keeps it in EAX) and the pushes around it.
- 0x0070C870 CAuroraInterface::ReleaseRaster (5): the original hoists the
  pData / nTXISize loads above the compressed-size select.
- 0x0070CBD0 CAuroraInterface::ReleaseResource (15): the original reads
  the tag type with `mov cx` + movzx and advances the tag pointer in two
  steps; ours folds both into addressing.

## Lessons

- CResHelper<T, nType> (copied from bucket j) compiles every instantiation
  here unchanged. Its ??_G is emitted by any inline use of the ctor, so the
  CAurora* ctors in the same file are enough; `template class` lines
  instantiate the dtor and SetResRef.
- The CAurora* wrappers tag the payload with a 6-byte CResHelperInfo
  {unsigned short type; void* helper} (pack 2): before the data for the
  header-prefixed resources (m_bHeaderPrefix = 1), after the pixels for
  TGA/PLT/TPC (m_bLargePrefix = 1).
- /Ob2: the small accessors (GetTextureAttrib, Get*DataPtr) are plain .cpp
  definitions that get inlined into later functions of the same file and
  also keep their out-of-line copy. GetTextureData-style accessors that
  have no out-of-line copy are in-class inline.
- Identical code is folded everywhere (0x00710100 is every wrapper's
  Unload, 0x0070FAF0 every "data - 6" GetPtrToResHelperInfo, 0x00712F90 /
  0x00712FA0 the PLT/TGA/TPC data getters, 0x00404DD0 every "+0x30" data
  getter). Name the copy after the class whose run it sits in and declare
  the others without a body.
- Early-out chains `if (a || b) return 0;` must be written as separate ifs
  to get the original's merged return-0 placement (DDS/4PC/TGA services,
  ReadTextureHeader).
- `if (!Load()) {...} else { return p; } return 0;` gives the original
  layout of the Get*Internal tails.
- Keyframe reads from packed 5-byte LIP records use memcpy into a local
  (the original stores into the dead parameter slot and reloads); a plain
  field read compiles to movzx / fld.
- CResTGA::OnResourceServiced: the two RLE cases share one failure check
  through a `bRead` flag set to 1 before the switch.
- /Ob2 inlines functions defined LATER in the same file too
  (ReleaseLayeredRaster pulled in ReleaseLayeredTextureInternal), so
  callers that the original keeps as calls go in a separate file
  (aurora_raster.cpp). For the same reason the wrappers' GetTextureData /
  GetTXIData accessors are .cpp definitions (their out-of-line copies are
  folded into 0x0070F380 / 0x0070F3E0 / 0x007107B0), not in-class inline.
- A CExoString built for one call and destroyed right after it, with the
  address re-taken by lea, is a named local in its own block.
- `CResRef cResRef(sName); SetResRef(cResRef, 0);` (named local), not a
  temporary in the call, for the push order in LoadLip / LoadNameTable.

## Naming choices

- Mac debug names were preferred: CExoMoviePlayer::CancelMovie /
  IsMoviePlaying (bucket c calls the latter GetState), and the same names
  for the internal callees (bucket d: SetSkip / GetState).
- Descriptive names (no Mac pairing): CAuroraModel::GetModelDataSize
  (0x007100A0), CAurora16BitTexture::Get16BitTextureDataPtr (0x007107B0),
  CAuroraInterface::Get<Type>(const CExoString&) forwarders (0x0070E8B0..),
  CResMDX::GetMDXSize / OnResourceFreed, CRes4PC::OnResourceFreed,
  CResTPC::Get{TPC,TXI}Data{Ptr,Size}. CSWNameGen::LoadNameTable returns
  int (the code returns 0/1).
- CResHelper's type argument is `int` as in bucket j (the Mac build uses
  unsigned short).

## Round s2 results

Folder MATCH 194/203 -> 231/246. 43 functions attempted: 36 NEW exact,
7 NONMATCH kept, plus 1 old NONMATCH promoted (0x0070CBD0
CAuroraInterface::ReleaseResource). Every callee, vtable, global and string
of the matched code was checked by address with a relocation dump.

Files: src/o/{aurora_texture_packs,video_modes,sw_gui_quick_panel{,_ctor,
_dtor},sw_gui_quick_or_custom{,_ctor,_dtor},sw_gui_name_chargen{,_ctor,
_dtor}}.cpp; additions to aurora_raster.cpp and res_tga_tpc.cpp. The
char-gen GUI headers (include/o/sw_gui_{dialog,bark_bubble,tutorial_box,
options_in_game,in_game_abilities,skill_info,levelup_chargen,
main_chargen}.h) are copies of bucket l's, L_ macros renamed O_.

- CAuroraInterface: GetResource 0x0070C920, Load/Unload/GetCurrent/
  SetTexturePack 0x0070CF30-0x0070D800, ~CAuroraInterface 0x0070D340 (tail
  call AurTextureFreeLayers), RequestModel 0x0070D8D0, ctor 0x0070D9C0
  (installs the renderer callbacks), GetModelResHelper 0x0070DE20,
  CExoLinkedList<CAuroraModel> ctor 0x0070D2D0, RenderInterface::IsResident
  0x0070D980 (Mac name), FindVideoMode 0x0070D220 (bucket c's name).
- CResLTR::OnResourceServiced 0x00712410 (1.4 KB, one function).
- CSWGuiQuickPanel 0x006F0220-0x006F0700 (7), CSWGuiQuickOrCustomPanel
  0x006F07D0-0x006F0E10 (10), CSWGuiNameChargen SetParentPanel,
  HandleCancelButton, OnPanelAdded, HandleInputEvent, ??_G, dtor (6).
- Missing starts annotated (not in functions.csv): 0x006F0220, 0x006F0310,
  0x006F0800, 0x006F0830, 0x006F09D0, 0x006F09E0.

NONMATCH kept:
- 0x0070D380 / 0x0070D510 / 0x0070D6B0 Get{Compressed,Processed,16Bit}Raster
  (6/18/12): the original zero-initialises four location locals (TPC, DDS,
  4PC, TGA) in every function, even the ones it never queries, and keeps
  the last Exists result in a register. Plain, by-reference, ?: and
  inline-helper forms all drop the unused stores.
- 0x0070D0A0 FindNearestVideoMode (32): nBest lives in ESI around the
  rep movsd; ours spills it. Flag/goto/declaration-order forms all tie.
- 0x006F9BF0 HandleRandomName (18), 0x006F9CD0 HandleDoneButton (20),
  0x006F9E70 CSWGuiNameChargen ctor (38): register picks (this vs the
  message box; &m_btnDone vs &m_btnBack).
- Skipped: CSWNameGen::GetRandomName 0x00711390 (one 1.3 KB x87 function
  with recursion and coalesced uninitialised locals) and 0x007118C0.

## Lessons (s2)

- Raw tag reads (`unsigned char* pTag = ...; type = *(unsigned short*)pTag;
  pTag += 2; helper = *(T**)pTag;`) reproduce the original's `add eax,2`
  steps; this promoted ReleaseResource and matched GetModelResHelper.
- MSVC 7 turns `!strcmp(buf, "MDL")` into a 4-byte repe cmpsb; GetResource
  needed no other trick.
- A bool local assigned in an if/else gives the `setne al; movzx` plus
  `xor al,al; movzx` return pair (IsResident).
- In a ctor, calls to the panel's own virtuals are direct; button events
  through base virtual slots use the vcall thunks (0x00624BB0 slot 0x54,
  0x00644720 slot 0x5c = HandleInputEvent2A).
- Jump-table switch bodies are laid out in source order: write the cases
  in the order of the original's code, not the case values.
- Two conditional temporaries assigned to one string: use if/else, not ?:
  (?: adds the conditional-destruction flags).

## Round s3 results

Folder MATCH 231/246 -> 259/280. 34 functions attempted: 28 NEW exact,
6 NONMATCH kept. Every callee, vtable, thunk, global and string of the
matched code was checked by address with a relocation dump.

Files: src/o/sw_gui_portrait_chargen{,_ctor,_dtor}.cpp,
src/o/sw_gui_feats_chargen{,_ctor}.cpp, include/o/sw_gui_feats_chargen.h;
CSWGuiPortraitCharGen moved from a pad stub to a full layout in
include/o/sw_gui_main_chargen.h (with CExoArrayListM, a CExoArrayList with
the inline ctor/dtor/Allocate/Add/IndexOf/DelIndex/SetSize bodies).

- CSWGuiPortraitCharGen 0x006F89D0-0x006F9A50 (11 of 12): OnPanelAdded,
  SetParentPanel, OnSelectDisplayButton, UpdatePortraitButton, Draw,
  HandleCancelButton, HandleRightButton, HandleInputEvent, ctor, ??_G, dtor.
  Layout 0x1240 ("PORTCUST"). Draw is at 0x006F8C60 (a missing start:
  functions.csv and the Mac map put "Draw" at 0x006F8AD0, which is
  UpdatePortraitButton; the vtable +0x34 slot is 0x006F8C60).
- CSWGuiFeatsCharGen 0x006F2A20-0x006F47D0 (17 of 22): IndexOf COMDAT
  0x006F2A20, dtor, ShowGranted, HandleCancelButton, Draw, DetermineFeat,
  SetDescription, ??_G, OnPanelAdded, OnEnterFeat, BuildButtons,
  OnFeatSelectionChanged, OnFeatPicked, OnDoubleClick, ctor, OnAcceptButton,
  HandleInputEvent. Layout 0x1A1C ("FTCHRGEN").
- Missing starts annotated (not in functions.csv): 0x006F2C40, 0x006F3CF0,
  0x006F89D0, 0x006F8C60.
- Shared o header changes (names/layout only): CSWCCreatureVirtuals slots
  0xec SetPortrait / 0xf4 SetPortraitId, CSWCCreatureStats m_nPortraitId
  (0x11c) and m_lstFeats (CExoArrayList<unsigned short> at 0xb0, replacing
  three loose fields; GetNumFeats casts num), CSWFeat m_nDescriptionStrRef
  (0x0c), CSWGuiPanel slot 0x50 HandleInputEvent27, CAurCamera 0x44
  SetFieldOfView / 0x74 AttachToHook, CAurObject 0x128 SetAnimationBlend,
  CGuiInGame m_pTutorialBox (0xa0), ShowTutorialWindow returns int,
  CExoString::operator>. The o placeholder CSWGuiFeatsCharGen in
  sw_gui_levelup_chargen.h was replaced by the full class.

NONMATCH kept:
- 0x006F90F0 GetPortraitsList (67): register picks (this / table / second
  Add argument). A `const T&` Add gets the frame right (46) but is not the
  original signature.
- 0x006F3290 BuildAvailableList (39): this in EDI (original ESI).
- 0x006F3460 InitiateFeats (45): the original tests the feat count with a
  dword load + `test ax,ax` and keeps the redundant "granted > 0" loop test.
- 0x006F37E0 AddChosenFeat (27): the original keeps the name loop
  unrotated; for/while/for(;;) forms all rotate.
- 0x006F3990 RemoveChosenFeat (39): MSVC turns our recursive tail call
  into a loop; the original keeps the call.
- 0x006F3A80 OnRecommendButton (8): EAX vs EDX for the class argument (an
  int local gets this far; an unsigned char local spills).

## Lessons (s3)

- The natural /Ob2 inline budget reproduces the original's mixed inlining:
  with real inline bodies for IndexOf / Add / Allocate, MSVC inlines three
  IndexOf calls in DetermineFeat and calls the COMDAT copy for the fourth,
  calls Add out of line in BuildAvailableList and inlines Add+Allocate in
  InitiateFeats, exactly as the original. Do not split these by hand.
- A reference local (`int& n = m_list[i];`) gives the original's
  `lea` + later load (UpdatePortraitButton).
- `CExoString(" ")` as an explicit temporary in an operator+ chain makes
  MSVC keep the ctor's return value; the implicit conversion (`a + " " + b`)
  matches.
- Calls whose arguments come from a `GetInGameGui()` chain: a
  `CGuiInGame* pGui = ...;` local gives the original's argument order.
- An inline wrapper `void AddFeat(unsigned short n) { m_lstFeats.AddUnique(n); }`
  gives the argument-first evaluation (OnAcceptButton).
- Compare-chain switches: write the cases in code order (case 2, 1, 0,
  default for GetPortraitsList's class columns); a switch with shared
  bodies (not if/else) gives the original's tail merging (Draw).
- `unsigned char nPage = m_cChart.m_nPage;` reproduces `mov al; movzx`.
- Button setup `CSWGuiButton* pButton = &m_btnX; pButton->m_bNavigable = 0;
  pButton->AddEvent(...)` reused for each button matches the ctors.
- Members with virtual calls (m_lblDescItem.CopyStyle) stay virtual calls.

## Round s4 results

Folder MATCH 259/280 -> 282/313. 33 functions attempted: 23 NEW exact,
10 NONMATCH kept. Every callee, vtable, global and string of the matched
code was checked by address with a relocation dump.

Files: src/o/sw_gui_powers_levelup{,_ctor}.cpp, src/o/sw_gui_skills_chargen
{,_points}.cpp, include/o/sw_gui_powers_levelup.h, sw_gui_skills_chargen.h
(the o placeholders in sw_gui_levelup_chargen.h became forward declarations).

- CSWGuiPowersLevelUp 0x006F0EE0-0x006F28C0 (14 of 20): dtor, OnPanelAdded,
  HandleCancelButton, OnAcceptButton, Draw, DeterminePower, SetDescription,
  ??_G, OnEnterPower, BuildButtons, OnPowerSelectionChanged,
  OnRecommendButton, OnPowerPicked, HandleInputEvent. Layout 0x1A10
  ("pwrlvlup"); lists are CExoArrayListM<unsigned long>.
- CSWGuiSkillsCharGen 0x006F47E0-0x006F6A10 (9 of 13 attempted): dtor,
  OnPanelAdded, HandleCancelButton, OnExitPointsButton, SetDescription
  (0x006F4AB0, descriptive), ConfirmAccept, OnMinusButton, OnAcceptButton,
  HandleInputEvent. Layout 0x49D0 ("SKCHRGEN"). Ctor 0x006F51D0 (4.5 KB)
  and ??_G 0x006F4BD0 not attempted.
- Missing starts annotated (not in functions.csv): 0x006F10C0, 0x006F2110,
  0x006F49A0, 0x006F4A80, 0x006F4DC0.
- Shared o header changes (names/layout only): CExoArrayListM::Insert has
  its inline body (needed for InitiatePowers; no MATCH lost), CSWSpell
  m_nDescriptionStrRef 0x0c / m_anPrereqs[5] / m_lstPrereqSpells 0x174,
  CSWSCreatureStats m_bIsPC 0x6c, CSWCCreatureStats m_pModifiedSkillRanks
  0x84, CSWSkill m_nFlags 0x1c + GetDescriptionText, CSWClass
  GetClassSkillValue, SkillSelectionCompleted (folded 0x006EE5D0),
  CSWGuiCustomPanel::SkillCreationCompleted, a SetSkills overload.

NONMATCH kept:
- 0x006F1730 BuildAvailableList (70), 0x006F1990 InitiatePowers (24),
  0x006F1C90 AddChosenPower (31): the name-sorting loops (CExoString
  temporary compared with operator>) stay unrotated in the original; every
  for/while/goto/named-local form rotates (same tie as AddChosenFeat).
  BuildAvailableList also calls Add out of line where we inline it.
- 0x006F1E40 RemoveChosenPower (11): a bRemoved flag keeps the recursive
  call (no tail-recursion loop); left: loop alignment and EAX/EDX picks.
  Try the same flag on RemoveChosenFeat 0x006F3990.
- 0x006F2110 OnDoubleClick (2): ECX vs EAX for the movzx argument.
- 0x006F2180 CSWGuiPowersLevelUp ctor (104): register picks from the
  m_bIsPC bitfield copy on shift every later temporary.
- 0x006F4B60 IsClassSkill (4): a dead `movzx eax, al` of the class id.
- 0x006F4BF0 OnEnterPointsButton (22), 0x006F4E20 OnRecommendButton (33),
  0x006F6570 OnPlusButton (56): register / frame picks.

## Lessons (s4)

- The original's mixed inlining of CExoArrayList Insert/Add/Allocate
  (inline in InitiatePowers, calls elsewhere) needs a real inline Insert.
- A flag set before a recursive call (`bRemoved = 1; Recurse(); break;`)
  defeats tail-recursion elimination.
- Arrays indexed through a short (`&g_pRules->m_pSkills[nSkill]` with an
  unsigned short parameter) give the original's movzx/shl/add order.
- `ShowControl()`/`HideControl()` (pointer then field) reproduce
  `lea eax,[button]; mov [eax+0x44]`; a plain member store folds the offset.
- Pull a local for a call argument computed from a call chain
  (`CSWSStats* p = ...->m_pStats; m_cChart.SetCreature(p, 1)`) to get the
  original's call-first order.
- A function defined later in a file is inlined into HandleInputEvent, so
  big callees (OnMinus/OnPlus/OnRecommend) live in a separate .cpp.

## Round s5 results

Folder MATCH 282/313 -> 299/331. 23 functions attempted (18 new, 5 old
NONMATCH retries): 16 NEW exact, 2 NEW NONMATCH, 1 NONMATCH promoted
(0x006F3990 RemoveChosenFeat); 0x006F4E20 skills OnRecommendButton now
reproduces the stack-slot class index (still NONMATCH, same size). Every callee, vtable, thunk, global and
string of the matched code was checked by address with a relocation dump.

Files: src/o/sw_gui_abilities_chargen{,_ctor}.cpp,
src/o/sw_gui_skills_chargen_ctor.cpp, include/o/sw_gui_abilities_chargen.h
(the o placeholder in sw_gui_levelup_chargen.h became a forward
declaration).

- CSWGuiAbilitiesCharGen 0x006F6B60-0x006F8880 (15 of 16): refund/cost
  helpers (GetAbilityPointRefund 0x006F6B60 descriptive, GetAbilityPointCost
  0x006F6BB0), dtor, OnPanelAdded, HandleCancelButton, OnExitPointsButton,
  CompletePanel, SetDescription, ??_G, OnEnterPointsButton, OnAcceptButton,
  ctor 0x006F7600 (3.7 KB), OnMinusButton, OnPlusButton, HandleInputEvent.
  Layout 0x3DF4 ("ABCHRGEN"); scores in STR, DEX, CON, WIS, INT, CHA order.
  The +/- button events are the folded 0x0067CB40 / 0x0067CB50
  (OnMinusButtonPushed / OnPlusButtonPushed, declared without a body).
- CSWGuiSkillsCharGen ??_G 0x006F4BD0 (emitted with the ctor).
- Missing starts annotated (not in functions.csv): 0x006F6DB0, 0x006F6E40.
- Shared o header changes (names/layout only): CSWClass m_nPrimaryAbility
  0x17a and m_nRecommended* 0x17b-0x180 (unsigned char, were char
  m_nPackage*), CSWSCreatureStats base scores 0xe9-0xf4 + Get*Base inlines,
  CSWSCreature::GetStats inline, CSWPartyTable m_nControlledNPC 0xf0,
  CSWGuiCustomPanel::AbilityCreationCompleted 0x006EF500, skills
  CSWCLevelUpStatsS::GetClassId inline, OnMinus/PlusButtonPushed on the
  skills panel.

NONMATCH kept:
- 0x006F7390 OnRecommendButton (abilities): this/zero swap ESI/EDI and
  AL/DL; the stack-slot class index is reproduced.
- 0x006F51D0 CSWGuiSkillsCharGen ctor (4.5 KB): temporaries rotate one
  register off from the g_pAppManager loads on; minus m_bNavigable store.
  The rest (InitControl run, descriptions, loop) is structurally identical.
- Retried without success: 0x006F3A80, 0x006F4B60, 0x006F2110 (forms in
  the lessons below did not help).

## Lessons (s5)

- MSVC 7.0 uses what it knows about callees compiled earlier in the same
  TU: a register (EDX) or a member (m_nPoints in EBP) stays live across a
  call to a small leaf function defined above. Keep callers and such leaf
  callees in one file (GetAbilityPointCost / Refund with Plus / Minus).
- The "byte stored to [esp+N], dword reloaded, pushed" class index comes
  from an inline wrapper with an unsigned char parameter
  (GetClassObject(m_nNumClasses - 1) / GetClassId(...)), and the original
  also aligns the frame (and esp,-8) for it.
- `mov reg8,[p+x]; movzx reg,reg8` for the first of a run of field copies
  is an inline accessor returning unsigned char (GetSTRBase()); applying
  it to the server branch also needed CSWSCreature::GetStats() inline.
- An in-class helper for a repeated message-box block was not inlined;
  the original wrote the block out each time.
- Locals for m_nCurrent / the score before the branches reproduce the
  original's early loads (OnMinusButton).
- A bitfield set through SetVisibleBit() compiles to lea/add (x*2) and
  blocks reuse of loaded values; a plain bitfield store gives shl. Pointer
  stores (`CSWGuiControl* p = &m_aPlus[i]; p->m_bNavigable = 0;`) change
  which member the loop's induction pointer is based on.
- A flag before the recursive call (bRemoved) also fixed RemoveChosenFeat.

## Next in-range queue

1. Bucket o game code is nearly exhausted. Left: CSWNameGen::GetRandomName
   0x00711390 (one 1.3 KB x87 function; the lane split at 0x0071143D is
   wrong) and 0x007118C0 (2.2 KB x87).
2. Old NONMATCH retries with the s5 lessons: 0x006F51D0 skills ctor,
   0x006F1E40 RemoveChosenPower, 0x006F4BF0 / 0x006F6570 skills points.
3. CRT leftovers and EH funclets (0x00713E90 on) need no game source.
