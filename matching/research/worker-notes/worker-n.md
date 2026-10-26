# Worker N matching notes

Range: 0x00620000–0x0063FFFF. Kotor RE has no worker-n notes file; its
worker-c notes provide the inherited conventions. Use current annotations
and reports to determine ownership.

## Current checkpoint, 2026-10-26

- Prior family: party lookup 0x00635460/0x006346C0/0x006349C0 and message-box
  controls 0x00627130/0x00627160: five exact matches. Party records have
  stride 0x88 at +0x24. Use typed records and extern g_pAppManager.
- Three visual forwards remain NONMATCH with established void signatures:
  0x0063CB50 (4 diffs), 0x0063C0C0 (3), 0x0063D440 (6). Returning uint
  would match, but no independent evidence establishes that public return
  contract. Do not change the type just for bytes.
- Current family: panel lifecycle 0x00624A00/0x00624A40/0x00624AE0 (3/3),
  in-game selection 0x0062ADA0/0x0062B060/0x0062B0B0/0x0062B410 (4/4),
  party follow 0x006359C0/0x006369D0 (2/2): nine new exact matches.
- 0x0062B060 takes one byte index and RET 4, despite a stale two-argument
  caller alias. 0x00624AE0 forwards to text at this+0x14C, not a no-op.
  RecalculateFollowPoint uses an embedded follow-path object at +0x2DC.

## Next family

The TODO scan found 265 candidates, including 111 at most 128 bytes and 43
named candidates at most 256 bytes before subtracting the current work.
Continue a larger related GUI family from that pool. Stronger names include
0x00625200 UpdateControls, 0x00625A00 controller-loss OnAccept,
0x006271A0 SetMessage, 0x00627220 controller-loss OnPanelAdded,
0x0062AD30 GetPlaceableCamera, 0x0062B490 ResetInterfaceFont,
0x0062F420 TutorialReasonWillShowWindow, 0x0062F780/0x0062F7E0 pause
callbacks, 0x0062EEB0 UpdateStatus, 0x0062FAD0 fade-panel visibility, and
0x00630620 InitializeDialogAnimationData. Object candidates include
0x0063C4A0 AnimationDialog, 0x0063D1D0 NewVocalization and 0x0063D270 SetWind.
These are a screening list, not implemented claims; recheck markers, ABI,
declarations and callees before writing them.

## Isolated r1 GUI family, in progress

Prefetched all 256 currently eligible addresses with `todo.py --range
0x620000 0x640000 -n 500 --names matching/mac_names.csv`; screened GUI
lifecycle/control and main-interface methods together against original
instructions and existing declarations. First checkpoint: seven new exact
functions, 0x00624AB0/0x00624A70/0x0062B580/0x0062B5A0/0x0062EF70/
0x0062F0A0/0x0062F0C0. Typed 24-byte CStatusSummary captures flags,
alignment shifts, credits, XP and suppression count; canonical 7/7.

Second checkpoint extends the same GUI family with camera records, dialog
storage, main-interface forwards, font updates and fade visibility. The
original fade threshold at 0x0073D708 is 0.001f (relocation masking does not
validate constant identity). Record count uses signed comparison; camera
records have 0x30 stride. Public AddFloatyText uses const Vector& and RET 16.

Message lifecycle family adds native string-by-value ownership (copy ctor
declaration is required for the direct return-object argument shape),
controller loss/modal warning callbacks, button events, and panel input-class
restoration. Mac hint at 0x00625A00 is incorrect: original body and callers
establish CSWGuiMessageBox::OnPanelRemoved, whereas controller OnAccept is
0x00625A40. OnPanelAdded has a remaining final arithmetic scheduling tie;
control bitfield inline shape also remains under evaluation. No ABI changes
are made for those ties.

Third checkpoint result: message family 14/15 exact. SetEnabled(int) inline
member accessors reproduce original flags scheduling; direct field
assignments caused hoisted loads. Ten additional GUI lifetimes all exact:
0x006270A0/0x00627080/0x00627930/0x00624920/0x00624900/0x00628500/
0x006284E0/0x00628100/0x006280E0/0x00628600. Implicit compiler-generated
destructors avoid the vptr installation emitted by user-defined empty
virtual destructors; use actual label/button members in destruction order.

Pause/fade extension: UpdateControllerPauseState, EndControllerPauseState,
MarkNoClickEvent and the status UpdateStatus dispatcher match. Original
unsigned pause comparison uses `<= 0`, not equality; explicitly naming the
client pointer reproduces world-time argument scheduling. New retained
attempts: IsMinimapHidden (branch placement), TutorialReasonWillShowWindow
(byte mask temporary), GetFadeOverridePanelVisible (prologue/return
scheduling), IsFadeDone (shared boolean returns), and
InitializeDialogAnimationData (zero register and return scheduling). The
original fade completion uses strict start+duration < current, not <=.

## r1 frozen handoff, 2026-10-26

- Pool screened: 256 eligible candidates prefetched/reviewed at pool level;
  selected a substantial GUI lifecycle/control workset, extending past the
  named shortlist to internal button/text and input-translation members.
- NEW implemented/attempted annotations over 5edb148: **58**.
- NEW canonical exact functions: **52**. NEW retained NONMATCH: **6**.
  Prior NONMATCH promotions: zero; the three animation-forward ABI ties
  remain untouched. No prior exact regression in the whole owned folder.
- Whole-folder canonical command with original inputs and default
  `-O2 -Gy -GX`, `-q -j 4 --report /tmp/worker-n-r1-final.csv matching/src/n`:
  **96/105 exact**, comprising 44 prior exact + 52 new exact; 3 prior misses
  + 6 new misses. Shared reports are unchanged.
- New NONMATCH queue: 0x006244B0 IsFadeDone 4 diffs; 0x006258F0 OnPanelAdded
  13; 0x0062F420 TutorialReasonWillShowWindow 9; 0x0062F910 IsMinimapHidden
  6; 0x0062FAD0 GetFadeOverridePanelVisible 9; 0x00630620
  InitializeDialogAnimationData 4. Keep these grounded implementations;
  avoid further scheduling retries in this batch.
- Adjacent extensions 0x006250F0 message input dispatcher, 0x00624970 input
  translation and 0x006252F0 AddMessageLabel are all exact. AddMessageLabel
  requires stable typed label/list pointers and passes the label directly
  after AddControls (which may modify the stack control-array entry).
- Full compact selected family is exhausted. Larger remaining message-box
  constructor 0x00626DF0 and FixMessageLabel 0x006253A0 need the broader
  manager/load/navigation/border/text layout and deserve the next coherent
  workset with the other GUI constructors; they are unattempted, not matches.
- Validation here is original compiler/executable byte comparison. No V4
  differential claim is made for GUI methods that require the game GUI and
  application graph; no VS2022 sources, builds or tests were touched.

Final callee audit corrected the unmapped 0x00627260 class to a clearly
marked descriptive CSWGuiModalWarningBox (no evidence for a battery name).
Its 0x00625A60 callback uses established SetUnknown1a8(int,int,int):
0x005EDE10 demonstrably stores three integers, so the weaker FadeToBlack
alias and float parameter are inappropriate even though masked calls match.

## Isolated r2 object/control family, in progress

Screened the complete 206-address remaining eligible pool via canonical TODO
and Mac hints (206 >= 100). Selected the coherent CSWCObject animation/table,
queue, model and server-link family; the larger GUI constructors have much
larger exception/layout requirements. First checkpoint: eight new exact
predicates at 0x0063C170/1E0/240/2A0/410/4F0/550/5B0. The short animation
argument stays short through ServerToClientAnimation's virtual +E0 contract;
GetINTEntry receives its zero-extended row. Naming the C2DA pointer before
GetINTEntry reproduces the original table-before-arguments scheduling.

Second r2 checkpoint: 14 additional attempts, 13 exact, cumulative 21/22.
Looping/fire-forget/play-out-of-place/dialog predicates and control members
are exact. RemoveTopFireForgetAnimation uses a typed 28-byte queue entry and
copies each member (preserving the untouched 16-bit padding); defining it in
the same translation unit as ClearFireForgetAnimations reproduces MSVC's
knowledge that ECX survives the call. GetServerObject is retained NONMATCH
at four instruction diffs: only EAX versus EDX for the pre-conversion object
ID, after explicit server temporaries resolved the earlier control-flow tie.

Third r2 checkpoint extends model, portrait, vocalization and animation queue:
10 new attempts / 6 exact, cumulative 27/32. Exact: 0x0063D1D0 NewVocalization,
0x0063D140 SetPortraitId, 0x0063D070 RemoveShadowBlob, 0x0063F570 SetArea,
0x0063BFD0 queue-entry constructor, 0x0063C760 AddFireForgetAnimation.
CSWPortrait is 16 bytes and slot +EC takes only const CResRef&; the zero
pushed before GetPortrait is its unsigned-char argument, not a second setter
argument. GetTopFireForgetAnimation is retained with seven x87 scheduling
diffs after explicit client pointers resolve all timer argument scheduling.
SetOrientation (five diffs), SetWind (six) and AcquireAnimationBase (two)
retain honest ABI and source, with register scheduling ties. Existing
UnregisterAnimationCallbacks and CExoArrayList<unsigned long>::AddUnique /
Remove names are reused for the established callees.

Fourth r2 checkpoint completes the compact event/control extension: eight
new attempts / five exact, cumulative 32/40. New exact addresses:
0x0063D3F0, 0x0063D850, 0x0063F5E0, 0x0063E510, 0x0063CFD0.
The event/portrait helper names without independent public records are
explicitly descriptive. SetPortraitRef needs a named assignment-result
CResRef before its virtual call; nesting the assignment in the argument
hoists the vtable read. Environment-map defaults at 0x0073E304 are verified
as "DEFAULT"; both SetEnvMap helpers retain stack-slot/register NONMATCH
attempts (26/28 diffs) after three source shapes. OnAnimationEvent retains
six register/argument-scheduling diffs with established int animation-name
slot argument; no short-argument ABI was invented to remove them.

## r2 frozen handoff, 2026-10-26

- Branch: `codex/worker-n-r2`, based on published `ee977a4`.
- Screened: **206** remaining eligible addresses; selected one CSWCObject
  animation/control/queue/model/server-link family rather than revisiting
  the six r1 scheduling misses or three unresolved void-return forwards.
- NEW annotations/attempts: **40**. NEW exact: **32**. NEW NONMATCH: **8**.
  Prior NONMATCH promotions: **zero**. Existing exact regressions: **zero**.
- Canonical whole-folder run, original inputs/default MSVC 7 flags,
  `-q -j 4 --report /tmp/worker-n-r2-final.csv matching/src/n`:
  **128/145 exact** = 96 prior exact + 32 new; 9 prior misses + 8 new.
  Shared status/symbol reports were not changed. `git diff --check` passed.
- New exact addresses: 0x0063BFD0, 0x0063C0A0, 0x0063C100,
  0x0063C170, 0x0063C1E0, 0x0063C240, 0x0063C2A0, 0x0063C300,
  0x0063C380, 0x0063C410, 0x0063C470, 0x0063C4A0, 0x0063C4F0,
  0x0063C550, 0x0063C5B0, 0x0063C760, 0x0063C9C0, 0x0063CAB0,
  0x0063CAD0, 0x0063CE40, 0x0063CFD0, 0x0063D070, 0x0063D140,
  0x0063D1D0, 0x0063D2D0, 0x0063D300, 0x0063D3F0, 0x0063D830,
  0x0063D850, 0x0063E510, 0x0063F570, 0x0063F5E0.
- New retained NONMATCH queue: 0x0063C110 SetOrientation 5;
  0x0063C610 GetTopFireForgetAnimation 7; 0x0063CB70 SetEnvMap 26;
  0x0063CCC0 SetEnvMapPart 28; 0x0063D270 SetWind 6;
  0x0063D330 OnAnimationEvent 6; 0x0063D4B0 GetServerObject 4;
  0x0063D700 AcquireAnimationBase 2. Do not burn another batch on these
  known register/stack/x87 ties without new compiler evidence.
- Final callee audit uses established int-returning
  CWorldTimer::AddWorldTimes, CSWCCreature::UnregisterAnimationCallbacks,
  CExoArrayList<unsigned long>::AddUnique/Remove and CResRef/CExoString
  contracts. Every new global is an extern declaration. No absolute-address
  loads, assembly, fake callee labels or ABI changes for matching were used.
- The complete local object layout is 0xFC bytes; extending it did not alter
  any preexisting header or derived layout. Matching validation is canonical
  compiler/executable byte comparison, with no V4 runtime promotion claim.
  Engine object graphs, timers, strings, GUI state and model vtables remain
  external dependencies; no gameplay sources/builds/tests were touched.
- Next coherent object workset: constructor/destructor 0x0063E530/0x0063E800
  with their complete base/member layout, or fade/visibility lifecycle
  0x0063CE60/0x0063DE50/0x0063DEE0/0x0063DF60/0x0063E220 and area-fade
  0x0063E350. These are unattempted, not matches. GUI constructor/FixMessageLabel
  queue from r1 remains available for a fresh context.

## s1 camera-behaviour family, 2026-10-26

- Branch `claude/worker-n-s1` from `c47d071`. Screened the 171-address
  eligible pool (`todo.py --range 0x620000 0x640000 --names
  matching/mac_names.csv`); selected the swbehaviorcamera.o family
  (CAurBehavior-derived navigate/on-a-stick/free-look/death cameras, vtables
  0x007515F4/0x0075165C/0x0075167C/0x0075169C) plus the CSWCModule tilt
  controls that drive them. New shared header `include/n/sw_behavior_camera.h`.
- Folder result (`kotor-match -q -j 2 matching/src/n`): **128/145 -> 165/184**.
  39 new annotations: **37 new exact**, 2 NONMATCH. The two old raw-offset
  annotations 0x006386D0 (navigate dtor) and 0x006383B0 (ClearTarget) were
  rewritten into the class files (still exact, not counted as new).
- New exact: death 0x00638540 0x0063A730 0x0063A760 0x0063A770 0x0063BBD0
  0x0063BC90(??_G); navigate 0x00638670 0x006386A0 0x006386B0(??_G)
  0x006386E0; on-a-stick 0x00637A30 0x00637AF0 0x00637B80 0x00637C20
  0x00638070 0x00638E80 0x00638F60 0x00638FE0 0x00639760 0x00639C30
  0x00639FC0 0x0063A310 0x0063A320 0x0063A360 0x0063A380 0x0063AD40(??_G)
  0x0063BA80 0x0063F600 0x0063F630; free look 0x006383C0 0x0063A5D0
  0x0063A6B0 0x0063A6C0 0x0063BAD0(??_G); CSWCModule 0x0063FCC0 0x0063FDB0;
  CSWCArea::GetEnvMap 0x0063BFA0 (descriptive).
- NONMATCH (register-numbering ties, stop retrying): 0x0063BAF0 free-look
  settings ctor 9 (eax vs ecx for the first settings load after the clip
  ctor; reference/const/local-temp/statement-order variants all equal);
  0x0063AD60 on-a-stick settings ctor 47 (same tie plus quaternion copy
  scheduling).
- Lessons: Vector must be POD here (no user copy ctor/assignment) for the
  `lea ecx,[esi+off]` member-copy shape and by-value pushes; Quaternion
  ZAxis/YAxis inline helpers in ctor form evaluate x first when returned.
  `const Vector&` temporaries do not reproduce `fld [eax]` direct use; write
  `Vector v = a->GetPosition() - b->GetPosition(); v.z = 0;` (dead z
  computation dropped). Split `g_pRules->m_p2DArrays` and
  `GetModule()->m_pArea` into named locals when the original loads the
  first pointer before the prologue pushes. Death Message/ctor order: the
  original interleaves assignments (write them in the scheduled order).
  `m_q = Euler(...); m_pGob->SetOrientation(m_q);` forwards from the call
  result; a named Quaternion temp adds 16 bytes of frame. GobRef dtor needs
  the `CExoArrayList<Gob**>& l = m_p->m_lWatchers; l.Remove(&m_p);` form for
  the free-look dtor scheduling. `vResult = vHead - vOffset` via a named
  result reproduces the temp-then-copy return in ComputeDesiredPosition.
- Next in-range queue (unattempted): camera Control/large helpers 0x00637CC0,
  0x006391A0, 0x006398F0, 0x00639D00 (free-look Control), 0x0063A810 (death
  Control), 0x0063B050 (hit check), 0x0063BCB0 (stick Control); CSWCModule
  TurnCamera 0x0063FE10 and SetCameraFollow 0x0063F9D0 (use the header's
  CSWCModuleCamera::GetBehavior slot +0x80). Then the GUI constructor queue
  from r1 and the CSWCObject ctor/dtor/fade family from r2.

## s3 CSWCObject lifetime / visual-effect / fade family, 2026-10-26

- Branch `claude/worker-n-s3` from `cb11729`. Folder result
  (`kotor-match -q -j 2 matching/src/n`): **194/228 -> 227/269**.
  41 new annotations: **33 new exact**, 8 NONMATCH. No prior exact lost.
- New self-contained header `include/n/sw_c_object.h`: CSWCObject derives
  from CGameObject (out-of-line ctor 0x004D7D60, inline empty virtual dtor),
  full vtable 0x007516C8 through +0x144 with slot names from the playable
  build's vtable table (src/units/1461/f4c7ba6a63-w3_client_t3.cpp), and the
  complete 0xFC layout (+0x6C CExoLinkedList<CSWCVisualEffectOnObject>,
  +0x70 CExoArrayList, +0xA8 CSWPortrait, +0xE8 CExoString). It does not
  replace object_control_family.h (older files keep their partial view).
- functions.csv misses many starts here; annotated anyway: 0x0063C080,
  0x0063CB00, 0x0063CB30, 0x0063D0A0, 0x0063D780, 0x0063D970, 0x0063DAA0,
  0x0063DB20, 0x0063DB90, 0x0063DDF0, 0x0063E450, 0x0063E4B0, 0x0063E730,
  0x0063E760, 0x0063E770, 0x0063E790, 0x0063E7B0, 0x0063E7D0, 0x0063E7E0,
  0x0063F400, 0x0063F490 (ctor 0x0063E530 needed the E730..E7E0 ones).
- New exact: 0x0063C000 (CSWGuiInterfaceAction::Reset, placeholder name),
  0x0063C080, 0x0063CB00, 0x0063CB30, 0x0063CE60, 0x0063D010, 0x0063D0A0,
  0x0063D560, 0x0063D780, 0x0063D7B0 (CExoLinkedList<VFX> ctor), 0x0063D8C0,
  0x0063D970, 0x0063DAE0, 0x0063DDF0, 0x0063DF60, 0x0063E220, 0x0063E350,
  0x0063E450, 0x0063E4B0, 0x0063E530 (ctor), 0x0063E730, 0x0063E760,
  0x0063E770, 0x0063E790, 0x0063E7A0, 0x0063E7B0, 0x0063E7C0, 0x0063E7D0,
  0x0063E7E0, 0x0063E7F0, 0x0063E800 (dtor), 0x0063F340, 0x0063F5C0 (??_G).
- NONMATCH (stop retrying): visual-effect list walkers 0x0063DAA0 (1, jump
  threading), 0x0063DB20 (4), 0x0063DB90 (13), 0x0063DD50 (7), 0x0063DE50
  (4), 0x0063F400 (5), 0x0063F490 (6): all the same register-coalescing tie
  (original computes GetHead() in a scratch reg then `mov esi,reg`; ours
  coalesces). Tried five Internal::GetHead bodies and four list-level ones;
  the `void* p = pHead; if (pHead) p = pHead->pObject;` form (kept) gives the
  right non-CSE'd pHead reload and matches DetachVisualEffects/ReAttach.
  0x0063DEE0 GetFinalFadeAlpha (4, epilogue tail-merge choice; three shapes).
- Lessons:
  - Quaternion needs a user ctor (non-POD) so a by-value return binds to a
    `const Quaternion&` argument without an extra 16-byte copy.
  - `return CExoString(g_sEmpty)` reproduces the GetName EH-flag shape.
  - Dtor: name the GetClientAIMaster() result before RemoveObject(this).
  - Ctor store order: put m_bShadowBlob = 1 after the OBJECT_INVALID load.
  - Shadow-option code: nested `if (n == 2) {..} else if (n == 1) { if
    (IsPlayer(m_idSelf)) {..} else {..} } else {..}` with a __forceinline
    helper taking the id by value (keeps m_idSelf in edi across the call).
    Identical arms reproduce the dead compare in the fade-out branch.
  - FadeObject: write the `!=` main path first; use `<=` / `>=` with the
    assign branch first; scope the day/time locals per block (slot reuse).
  - SetFadeState: assign m_fFadeAlpha = GetFinalFadeAlpha() in both branches
    (tail-merged) so SetAlpha reloads the member instead of a temp.
  - GetReticlePoint: `int bResult = 0` accumulator (xor eax,eax + movzx of
    the anim-base result) and an int nResult with if/else on `>`.
  - CSWGuiInterfaceAction::Reset: +0x0C is a 16-byte struct with a ctor
    {0,0,0,-1} assigned from a temporary after m_n08 = -1.
- Next in-range queue (unattempted): CSWCObject::SetAnimation 0x0063E930
  (2576 B, vtable +9C); camera Control helpers from s1 (0x00637CC0,
  0x006391A0, 0x006398F0, 0x00639D00, 0x0063A810, 0x0063B050, 0x0063BCB0);
  CSWCModule TurnCamera 0x0063FE10 / SetCameraFollow 0x0063F9D0 /
  LoadModule 0x0063F660; GUI constructor queue from r1.

## s4 GUI constructors / CGuiInGame / client input family, 2026-10-26

- Branch `claude/worker-n-s4` from `3b77c3b` (local only). Folder result
  (`kotor-match -q -j 2 matching/src/n`): **227/269 -> 275/320**.
  51 new annotations: **48 new exact**, 3 NONMATCH. No prior exact lost.
  (The first half of the round hit a usage limit; the resumed worker fixed
  the half-written gui_message_box_ctor.cpp and continued.)
- New exact, resumed part: 0x006253A0 CSWGuiMessageBox::FixMessageLabel,
  0x00626DF0 ctor (layout fix), 0x006200E0 CClientExoAppInternal::
  SetInputClass, 0x00620320 DismissInGameGUI, 0x00620350
  HandleMouseClickInWorld, 0x006222F0 HandleMouseOver, 0x00620480
  CExoArrayList<CExoInputEvent>::Insert, 0x0063CFC0 Part::Project (empty
  virtual, ret 0x10), 0x006345E0 CSWPartyMemberData::SetCombatMessage
  (placeholder class name), 0x00630560 CGuiInGame::AppendToDialogBuffer
  (CExoString,oid), 0x0062ACA0 CGuiInGame::SetPlaceableCamera (placeholder
  name), 0x00632720 InitializeGlobal, 0x0062F5F0 ResetInterfaceForSize,
  0x006303B0 ~CGuiInGame, 0x0062C9B0 ShowSWInGameGui, 0x0062E900
  SetShotFromScript, 0x0062ADD0 DialogNodeSpanningTextParse.
- Earlier s4 part (before the limit): in-game menu ctor/callbacks, in-game
  messages ctor/dtor/??_G/input, CSWGuiFade family, tooltip panel, status
  summary ctor, message box ctor, vcall thunks ??_9@$BFA/$BFE/$BFI@AE.
- NONMATCH (stop retrying): 0x006247A0 CSWGuiFade::ResetFade (9),
  0x00626920 AddFeedbackMessages (10), 0x00626B10 AddDialogMessages (5).
- Lessons:
  - `g_pAppManager->GetClientExoApp()->X()` through an inline accessor (not
    `->m_pClientExoApp->`) changes the load order of the app pointer vs
    argument loads; it fixed both HandleMouseClickInWorld and
    SetCombatMessage. Same for `GetModule()->GetScene()->GetCamera()`: the
    inline accessor lets cl load the scene straight into ecx.
  - SetInputClass: the case-1 fallback is `SetInputClass(nOld,1)` (cl turns
    the tail recursion into the loop at +0x20); case bodies in source order
    0,3,2,1. Name the object id before the ServerToClientObjectId call.
  - Pointer-to-member with an MI target class is 8 bytes and 8-aligned;
    wrap the struct in `#pragma pack(push,4)` when the original has it at
    +0x0C.
  - HandleMouseOver: define CClientExoAppInternal::SetMouseCursor (matched
    in c/) unannotated in the same file so cl inlines it at most sites and
    still calls it for (0x2D,0,0); write `if (!(bDefault || ...)) {..} else
    {..}` with `nCursor = 1; pFirst = 0;` initialised before
    GetDefaultActions().
  - Float copies into call arguments go through fld/fstp only when they come
    from a float local (`float fFov = cam.m_fFov;`); store order of
    m_bShotSet/m_vShotPosition decides the interleave.
  - Message box: the list box scroll bar sits at +0x104 of CSWGuiListBox;
    `const CSWGuiExtent& ok = m_ok.m_extent;` reproduces the CSE'd ebx.
  - DialogNodeSpanningTextParse: `bool bFound` in a block with a named
    "##" CExoString, ternary for the piece length, and the final return
    written like the pending path (`m_nPending = m_nCount; return
    m_as[m_nCount - m_nPending--];`).
- Next in-range queue (unattempted): CGuiInGame 0x0062C030
  (SetComputerCameraShot), 0x0062BC70, 0x0062C6F0, 0x00633660, 0x006332B0,
  0x006340E0, 0x00634340, 0x00631D80, 0x006313A0 (dialogue client);
  0x0062FBE0, 0x0062FED0, 0x00633C50, 0x006339C0, 0x006323A0; party
  0x006350F0, 0x00634E80, 0x00635480, 0x00636170, 0x006364C0, 0x00637050;
  CSWCModule 0x0063F660 / 0x0063F9D0 / 0x0063FE10; client input 0x006224E0,
  0x00620620 GetDefaultActions, 0x00621210, 0x006227E0; camera helpers
  from s1; CSWCObject::SetAnimation 0x0063E930.

## s5 CSWCModule turn/follow + CGuiInGame dialog reply/close family, 2026-10-26

- Branch `claude/worker-n-s5` from `0e1c578` (local only). Folder result
  (`kotor-match -q -j 2 matching/src/n`): **275/320 -> 280/331**.
  11 new annotations: **5 new exact**, 6 NONMATCH. No prior exact lost.
  Only 39 todo functions are left in range and all are 600 B+, so the batch
  is small.
- New exact: 0x0063FE10 CSWCModule::TurnCamera, 0x0063F9D0
  CSWCModule::SetCameraFollow (sw_c_module_turn.cpp, new header
  include/n/sw_c_module_control.h copied from m/sw_c_module_camera.h),
  0x0062BC70 CGuiInGame::InitializeRepliesData, 0x0062C030
  SetComputerCameraShot (standalone layout like gui_in_game_set_shot.cpp),
  0x0062C6F0 UnloadStuntModelsForDialog.
- NONMATCH (stop retrying, all register/slot ties): 0x006340E0
  ShowDialogReplies (12), 0x00634340 HandleDialogReplies (29, loop IV
  regs), 0x006339C0 UpdateDialog (33), 0x00633660 HandleDialogSelection
  (allocation; most diffs a 1-byte shift), 0x006332B0 CloseDialog (34),
  0x00636170 CSWParty::CSWParty (14, constant-register tie 1.0f vs 1.5f).
- Header changes: gui_in_game_dialog.h now has the per-reply arrays
  (+0x114..+0x15C, types from SetReplyData's mangled name), the zeroing
  `Vector()` (needed for `new Vector[n]`; no other user there),
  CSWGuiDialogPanel (+0x3C dialog vtable: +0x08, +0x74 SetReplies, +0x7C,
  fields +0x44/+0x64/+0x19C0), CSWCMessage with ctor/dtor and 0x50 size,
  CSWGuiLetterbox +0x88 with inline StopFade. sw_party.h: CSWPartyMember
  inline ctor (Reset() 0x004EAE80 + 4 stores), Quaternion default ctor, and
  `#ifdef N_SW_PARTY_ZERO_VECTOR` zeroing Vector() only for the ctor TU
  (sw_party_lifecycle.cpp has an uninitialised `Vector vSafe`).
- Lessons:
  - Clamp `MAX(-30, MIN(30, x))` with MIN(a,b) ((a)<(b)?(a):(b)) and
    MAX(a,b) ((a)>(b)?(a):(b)), written inline in the expression (not via a
    named temp), reproduces the double-evaluated re-test. Product order:
    `-(fTurn * (fRate * fScale))` with `float fRate = member;`.
  - `q = EulerToQuaternion(GetPitch(q) + a, GetRoll(q), GetEulerRoll(q))`
    reusing the DirectionToQuaternion local gives the 0x20 frame.
  - Inline-defined GetCameraPosition (0x0063F600 matched elsewhere) in the
    caller's TU reproduces the direct-into-local Vector return;
    `m_vPosition = Vector(vPosition)` gives the loads-then-stores copy.
  - Hoisting `CClientExoApp* pApp = ...` after the combat check fixed
    SetCameraFollow's register assignment; a trailing call wanted the inline
    GetClientExoApp() accessor (load app before the argument).
  - Accessor rule of thumb seen again: outer server calls whose argument is a
    call result want GetServerExoApp() (object load before `push`), inner
    ones want the direct member; mixing both in one nested expression lets
    cl CSE the server pointer.
  - `if (nEffect == -1) nEffect = 0; if (nEffect != -2) Enable(nEffect);`
    gives the threaded single call; `for (i = 0; i < n; i++) if (..) {
    bFound = 1; break; }` gives the rotated search loop.
  - UnloadStuntModelsForDialog's `and esp, -8` frame came out naturally
    (CExoString returned by value from a virtual); reading the server
    placeable's animation as a dword needs a union view.
  - Early-return-at-end shape (`if (m_bInitialized) {...} return 0;`) and
    inline helper methods (`Letterbox->StopFade()` keeping `this` in ecx)
    fixed most of CloseDialog.
- Next in-range queue (unattempted): 0x00631D80 HandleDialogEntry (23 args,
  like HandleDialogReplies), 0x006313A0 SetDialogAnimations, 0x0062FBE0
  CopyQuickSaveGameToFutureGame (string EH), 0x006323A0, 0x00633C50,
  0x0062FED0, 0x00632860 ingame init, 0x006306E0 UpdateShotCamera,
  0x006288F0; party 0x006364C0 AddCharacter, 0x006350F0, 0x00634E80,
  0x00635480 SetLeader (no src), 0x00636A30, 0x00635AF0, 0x00637050;
  CSWCModule::LoadModule 0x0063F660 (EH, unknown callees); client input
  0x006224E0, 0x00620620, 0x00621210, 0x006227E0; camera helpers from s1;
  CSWCObject::SetAnimation 0x0063E930.
