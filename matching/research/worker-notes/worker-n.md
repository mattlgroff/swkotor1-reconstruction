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
