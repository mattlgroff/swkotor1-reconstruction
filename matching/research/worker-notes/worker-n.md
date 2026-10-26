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
