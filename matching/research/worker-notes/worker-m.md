# Worker M matching notes

Range: 0x00640000–0x0067FFFF. Consult Kotor RE's earlier worker-m/k notes,
then current annotations and symbol records.

## Current checkpoint, 2026-10-26

- Previous accepted family: journal delete handlers 0x00652290/0x006522E0/
  0x00652330, module handlers 0x00652840/0x00652860 and spell-like record
  ctor 0x006475C0: six new exact matches.
- Current exacts: module Type14/end-game 0x00652AD0 and
  CSWTlkTable::GetObjectGender 0x00674B40. The latter also has an older
  folded CTlkTable::GetGender caller alias; do not annotate it twice.
- Constructors 0x00676DA0 and 0x006770C0 remain NONMATCH: field stores
  agree, but the original has an EH registration/member-cleanup frame.
  Do not promote these based on stores alone. Prefer the established
  CSWCQuestJournal Windows name over the weaker CSWCJournal Mac hint.

## Next family

A 272-row TODO scan yielded a coherent CSWCMessage pool. Start with the
following 20 unclaimed handlers, then continue compatible message families
from the wider pool instead of treating each dispatcher as a batch:

- Ambient handlers: 0x00654380/3E0/450/4D0/530/5A0/600/680 and dispatcher
  0x00655510.
- Sound-object Play/Stop/ChangePosition: 0x0064F050/0A0/0F0; Sound dispatcher
  0x00656BA0.
- GuiQuickbar dispatcher 0x006637C0 and SetButton 0x00652B00.
- MapPin dispatcher 0x00663810, Enabled 0x00652D00, ReferenceNumber
  0x00652D60; GuiInfoPopup 0x006631E0 and GuiInventory 0x0064DEA0.

References cluster in `src/units/1737/`, `1738/` and `1739/`; the larger
448/512-byte handlers have behavioral references too. Recheck ownership and
callees before implementation. Names/reader hints are hypotheses until the
binary and canonical comparison support them.

## Active audited handler batch, worker-m-r1

Prefetched 268 eligible TODO addresses (entire range, names plus source index),
then selected the related CSWCMessage family. Screened rows are not attempts.
Canonical x86 confirms module +0x48 -> area +0x1DC ambient and virtual slots
0x00..0x20. First checkpoint: five actual new attempts, five NEW exacts:
0x00654380, 0x006543E0, 0x006544D0, 0x00654530, 0x006545A0.
All five matched on the first canonical MSVC7 compile. Family work continues.

Second checkpoint: 21 actual NEW attempts total, currently 16 exact / 5 misses.
New exacts beyond the initial five: 0x00665510; 0x0064F050/0A0/0F0,
0x00656BA0; 0x006637C0, 0x006631E0, 0x0064DEA0, 0x006514C0;
0x00663810, 0x00652D00. Retained misses: 0x00654450, 0x00654600,
0x00654680 (three source shapes, register/vtable scheduling); 0x00652D60
(EH register reuse), 0x00652B00 (EH/stack alignment). Work continues.

Compiler lessons: spelling each identical return case separately preserves
MSVC7's GuiInfoPopup jump table, whereas grouped labels fold into a simple
comparison. Materializing the area's receiver before GetMapPin preserves
argument-push placement, matching MapPinEnabled exactly.

Expansion checkpoint: 26 actual new attempts, 21 NEW exacts / 5 misses.
The four remaining compact journal add/remove/picture handlers all matched
first compile: 0x00651ED0, 0x00651FB0, 0x006520A0, 0x00652210. Popup GUI
0x00654230 also matched first compile. Retained established Journal_TypeN
names from symbols.csv, and CSWCQuestJournal from Windows call evidence.
SetJournalPicture takes WORD despite this caller passing a ReadDWORD result;
canonical callee uses only the low word and normal C++ matches exactly.

Third expansion checkpoint: 36 actual NEW attempts, currently 28 NEW exacts
and 8 retained misses. Additional exacts: 0x0064EB90 (item update),
0x0064F430 (trigger update), 0x0064D3A0 (door delete), 0x00656C90
(delete dispatcher), 0x006632A0 (quest add), 0x00654700 (character download),
0x0066A4A0 (game object dispatcher). Additional misses: 0x006529F0
(unused-resource EH state), 0x00652380 (full journal flag scheduling),
0x0064BF30 (custom-token loop scheduling, under review).

Preserved the Windows GetQuestJournal return type CSWCQuestJournal and reused
already-established CSWCJournal method symbols through a minimal base interface.
No nonzero base adjustment is introduced. SJournalEntry is copied from the
existing matched F declarations, including the 0x3C by-value ABI and bitfields.
The 512-byte Type6 journal handler matches on the first compile. The 512-byte
character download matches on the second shape: common post-switch return
shares string cleanup and a materialized client receiver preserves evaluation.

The initial item-update draft had a nonpolymorphic CSWCObject placeholder;
canonical comparison rejected the resulting primary-base layout. Corrected
it to the established polymorphic 0x10-byte base before accepting the exact.


## Completed substantial handler batch, 2026-10-26

Final NEW work: **36 actual attempts, 29 NEW exact matches, 7 retained
NONMATCHs**. The original 20-handler screened pool is fully attempted; the
batch expanded by 16 compatible journal, popup, object-update, download,
custom-token and top-level game-object handlers. No screened-only rows are
counted as attempts. No prior NONMATCH promotions are claimed.

NEW exact addresses:
0x0064BF30, 0x0064D3A0, 0x0064DEA0, 0x0064EB90, 0x0064F050,
0x0064F0A0, 0x0064F0F0, 0x0064F430, 0x006514C0, 0x00651ED0,
0x00651FB0, 0x006520A0, 0x00652210, 0x00652D00, 0x00654230,
0x00654380, 0x006543E0, 0x006544D0, 0x00654530, 0x006545A0,
0x00654700, 0x00656BA0, 0x00656C90, 0x006631E0, 0x006632A0,
0x006637C0, 0x00663810, 0x00665510, 0x0066A4A0.

Retained NEW NONMATCHs:
- 0x00652380: full-journal flags/copy scheduling, best 46 diff insns;
  two source shapes, 880 compiled bytes vs 864-byte canonical extent.
- 0x006529F0: unused-resource EH state/stack slot, 5 diff insns;
  two shapes, 208 vs 224 bytes.
- 0x00652B00: quickbar common-case ECX/push scheduling, best 17 diff
  insns including switch-data differences; two shapes, 512/512 bytes.
- 0x00652D60: allocation/EH zero/one register reuse, best 27 diff insns;
  three shapes, 448/448 bytes.
- 0x00654450 / 0x00654600 / 0x00654680: ambient branch allocation and
  receiver/vtable hoisting, best 10 / 10 / 6 diff insns; three shapes each.

The custom-token loop matched on its second shape: separate the stream count
from a for-loop countdown local to place the entry test before its EBX copy.

Callee-name audit corrected preliminary map/sound semantic labels before
handoff: use the established CSWCArea::FindObject/AddObject with
CSWCAreaObject and CSWCSoundObject::SetPositionElevated. All direct callees
were cross-checked against canonical call targets and existing declarations.
CExoLocString::AddString retains the existing int-language signature even
though its prior implementation remains NONMATCH. The CSWCJournal /
CSWCQuestJournal naming split predates this batch; the minimal zero-offset
interface retains both existing symbol families pending hierarchy reconciliation.
CGuiInGame::ClosePopUpPanel is a descriptive label for folded empty 0x004015A0;
its original specialized name remains unknown.

Whole owned folder canonical MSVC7 result: **259/284 exact**, up from baseline
230/248; the 18 old misses remain and all 29 new exacts pass. No duplicate
annotations. Only owned source/header/notes files changed; no shared reports,
VS2022 sources, status graphics, proprietary artifacts or pushes.

Next coherent queue: larger CSWCMessage object lifecycle/update handlers
(0x0064CC90, 0x0064C3C0, 0x0064F960, 0x00654B10), inventory/container protocol
(0x0064DEE0, 0x0064FEF0), and module/camera loading (0x0064B8E0,
0x0064DCF0). The active-item property handler 0x00651560 has a native out-of-frame
read documented by the playable reference; defer layout/ownership decisions
until its stack contract is reconciled. Do not revisit constructor EH misses
0x00676DA0 / 0x006770C0 as store-only "matches".

## Active next-family batch, worker-m-r2

Screened 180 of the 232 current eligible M TODO rows, then the 38-row current
CSWCMessage pool. Screening is separate from implementation. First five NEW
attempts: 0x0064CC90, 0x0064F960, 0x00654B10, 0x0064FEF0, 0x0064DCF0.
Current result: three NEW exacts (CC90, 54B10, DCF0), two retained misses.
Prior seven source-shape misses remain untouched.

Creature deletion's complete 512-byte EH path matches, including uninitialized
2DA output slots and dead-creature corpse handling. Use existing inline
Get2DArrays / typed table accessors to preserve the original evaluation tree.
ItemScript_Add (400 bytes) matches after materializing the AI receiver and base
item index. Extended item layouts stay local to this new family interface.

The load-progress handler (432 bytes) matches with nested float casts within
one SetLoadBarProgress expression. This places its second argument before the
five reader calls and forces each original float materialization. Preserve the
established GetTutorialFlag(int) symbol at 0x005EDFF0 even though this caller's
semantic reference labels the five returned bytes as load steps. The native
RunFrame name at 0x00401C10 and SendPlayerToServer_04_03 are preserved.

Bounded retained misses: TriggerUpdate_Delete 0x0064F960 (39 diff insns,
496/448 bytes; three shapes, common-success EH epilogue cloned by MSVC7),
GuiContainer 0x0064FEF0 (4 diff insns, 448/448 bytes; two shapes, one EBP pop
scheduled across the second coordinate store). Both retain complete typed C++.

Second local checkpoint: ten actual NEW attempts, seven NEW exacts, three
retained misses. Additional exacts: Camera 0x0064B8E0 (464 bytes), SetGuiSkillRank
0x0064CE90 (160 bytes), GenericObjectUpdate_Update 0x00651880 (624 bytes),
AreaOfEffectUpdate_Add 0x00651680 (304 bytes). ItemUpdate_Add 0x0064E7B0 is
retained NONMATCH after three shapes (36 diff insns, 784/784 bytes; receiver /
area EBX/EBP allocation plus output-slot scheduling). Full typed reuse/create
behavior is retained; no assembly or raw absolute global loads were added.

Camera matched on its first source compile, including the native uninitialized
camera local on the absent-module path. Generic updates matched after removing
invented initializers from conditionally read portrait/speed/animation fields
and preserving two separate mask-4 tests around the float and WORD readers.
SetGuiSkillRank needs plain char ranks and statement-form bounds checking;
changing signed char or a ternary accessor alters the inlined register tree.
AoE allocation/registration matches directly with the 0x17C typed object,
virtual SetObjectID/SetArea slots and established game-object array interface.

Third local checkpoint: fifteen actual NEW attempts, nine NEW exacts / six
retained misses. Inventory 0x0064DEE0 (1,264 bytes) and token-part helper
0x006500B0 (256 bytes) are additional exacts. Per-case returns preserve the
inventory switch's local success epilogues. Token handling requires separate
CTlkTable/CSWTlkTable globals (7A3A08 / 7A3A0C) and a scoped integer-string
local, avoiding reliance on a constructor's EAX result.

New bounded misses: ServerStatus 0x0064F280 (41 diff insns, 432/432 bytes;
three complete source shapes, common EH success placement), LoadBar 0x0064EDD0
(37 diff insns, 640/640; three shapes, inline field accessor/register trees),
and CreatureUpdate_VisEffect 0x0064C3C0 (261 diff insns, 1744/1840; two shapes,
large EH frame/register and temporary scheduling). The latter retains all
three native dummy allocation paths and the first path's inverted array-status
check. Its extended 0x44C creature declaration is isolated in that translation
unit. Existing GAME_OBJECT_VALID, object-type globals, canonical immediate
OBJECT_INVALID and typed base-class conversions are preserved.

Status format data was read directly from canonical .rdata: "%c%s.%s", 's',
"Module", "Run". The original caller uses established GetGameOver/SetGameOver
symbols even though its behavioral reference labels the state "downloaded".
No invented state names replace those proven symbols.

Fourth local checkpoint / recovery state: **21 actual NEW attempts, 12 NEW
exacts, 9 retained NEW NONMATCHs; no prior NONMATCH promotions.** This expands
well past compact dispatchers into complete object creation and inventory
model/sound paths. Two full add handlers are bounded misses: DoorUpdate_Add
0x0064CF30 (29 diff insns, 1120/1136 bytes; three shapes, locstring temporary
slots and register scheduling), TriggerUpdate_Add 0x0064F4B0 (81 diff insns,
1200/1200; three shapes, zero-vector frame/temporary scheduling). Their best
complete sources are retained. Native strings "Model", "VisibleModel",
"ModelName", "v_mnfrag", "EMPTY" were directly checked in canonical .rdata.

The compatible inventory item expansion adds NEW exact PlayDropSound
0x006466D0, AttachAuxiliaryModel 0x00646830 and SetSpecialAttackModel
0x00646880. The latter two names are descriptive; these addresses had no
established symbols. SetEquippedModelFlags 0x00646BA0 is a complete bounded
NONMATCH (9 diff insns, 224/240 bytes; two source shapes, owner-ID evaluation
and model-override load hoisting). All use typed fields and virtual slots.

Canonical whole M-folder check: **271/305 exact**, compared with accepted
baseline **259/284**. All 259 prior exacts survive, the prior 25 misses retain
their prior results, and all twelve claimed new exacts pass. No duplicate
annotations. The six new files/headers pending this checkpoint remain inside
owned M paths; no reports, non-M sources, VS2022 files or pushes are involved.

NEW exact addresses to date:
0x006466D0, 0x00646830, 0x00646880, 0x0064B8E0, 0x0064CC90,
0x0064CE90, 0x0064DCF0, 0x0064DEE0, 0x006500B0, 0x00651680,
0x00651880, 0x00654B10.
NEW NONMATCH addresses to date:
0x00646BA0, 0x0064C3C0, 0x0064CF30, 0x0064E7B0, 0x0064EDD0,
0x0064F280, 0x0064F4B0, 0x0064F960, 0x0064FEF0.

Next compatible queue: item render orientation 0x00646A30 and secondary
SetArea receiver 0x00646930 (the latter needs a defensible secondary-base
class declaration before attempting); CSWCMessage creature add 0x0064C0A0,
item creation 0x0064B540 / active properties 0x0064E3D0, placeable add
0x0064D500, and the local-character upload overload 0x00678080. The GUI
handler at 0x0064EC00 has two fixed stack arrays with an unbounded byte count;
its native stack contract needs reconciliation before a safe typed attempt.
Do not count disassembly-only screening of these queue addresses as attempts.
