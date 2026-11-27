# Code rules for the matching track

This file is the project profile for the `decomp-code-rules` catalogue
(rules R1–R37). The catalogue applies to `matching/` as written, except
where this file tightens or relaxes a rule and says why. Rule numbers are
the catalogue's and never change.

## Why

A matching decompilation exists so people can understand the original
program. Identical bytes prove the source is faithful, but they are not the
goal: source that matches and that nobody can read has failed at the job.
Every rule serves one test:

> Would a BioWare programmer in 2003, writing C++ for this engine with
> Visual Studio .NET 2002, have typed this? Can a reader see what it does,
> and why, without opening the disassembly?

Two laws follow:

1. **The bytes win.** A style change must not change what the compiler
   emits. When a rule and the bytes disagree, keep the bytes and write
   `// RULE-EXCEPTION: <rule>, <reason the bytes give>` at the code. An
   exception bends a style rule. It never licenses fake code.
2. **Identical bytes alone are not a match.** A function counts as matched
   only when `matchcmp.py` prints `OK` *and* its source passes every
   [match] rule: R1, R2 (one declaration per type and per global), R4 (no
   offset casts), R13 (real vtables), R17 (no fake code) and R18 (no
   assembly bodies). A byte-identical function that fails one is
   `MATCH-PENDING`. It stays in `status.csv` as `MATCH`, and it is reported
   separately, not counted as matched under the rules.

## Checking

```bash
python matching/tools/rulecheck.py                       # whole track
python matching/tools/rulecheck.py matching/src/n        # one bucket
python matching/tools/rulecheck.py -v matching/src/n/gui_in_game_n.cpp
python matching/tools/rulecheck.py --report /tmp/rules.csv
python -m unittest discover -s matching/tools -p "test_rulecheck.py"
```

`rulecheck.py` reads source only. It needs neither MSVC nor `swkotor.exe`,
takes about ten seconds for the whole track, and changes nothing. It prints
two numbers, **bytes matched** (from `status.csv`) and **matched under the
rules**, then the MATCH-PENDING count per rule, the advisory counts and the
types with the most conflicting definitions. `--report` writes one row per
annotated function: address, name, byte status, rule status, blocking and
advisory findings.

What each [match] rule's check fires on (and nothing else):

| rule | the checker reports |
| --- | --- |
| R1 | a class, struct, union or enum defined in two files with different bodies, or as `class` in one place and `struct` in another (forward declarations included). It blocks each function that names the type, in a file that defines it or includes a header that does. |
| R2 | an `extern` global declared in a `.cpp` rather than a header; one global declared with two different types |
| R4 | `(char*)`/`(int)` arithmetic with a constant offset that is then cast to a typed pointer; the same arithmetic on `this`; `this` cast and indexed as an array |
| R13 | a virtual named `pad*`/`dummy*` |
| R17 | `volatile` locals, and `volatile` members without a `// volatile: <evidence>` note; empty `if`/`else` bodies; bare `;` statements (empty loop bodies and `case X: ;` are allowed); `do { } while (0)` outside a macro; self-assignment; `+ 0` and `* 1`; padding members named `pad`, `padN`, `m_padN` or `*_pad`; lines marked `// probe` |
| R18 | `__asm`/`_emit` in a body or a `__declspec(naked)` function, unless it is marked `ORIGINAL-ASM` (see below) |

Not yet checked mechanically, so for review only: locals unused in every
branch (R17), layout views read through a cast (R1), and vtable slot counts
against the binary (R13; the track has no vtable table yet).

Advisory findings never block a match: placeholder names (R7), functions
without a header comment (R33), and addresses, scores or campaign notes in
function comments (R21).

## Baseline

`rulecheck.py` on `ac462c8`, before any source was changed:

| | functions |
| --- | ---: |
| annotated | 9,538 |
| bytes matched (`status.csv` MATCH) | 8,470 |
| **matched under the rules** | **871** |
| MATCH-PENDING | 7,599 |
| blocked by R1 | 7,476 (6,102 by R1 alone) |
| blocked by R2 | 1,046 (110 by R2 alone) |
| blocked by R17 | 501 (4 by R17 alone) |
| blocked by R18 | 11 (7 by R18 alone) |
| blocked by R4 | 4 |

Nearly all of the gap is one design decision: CONTRIBUTING.md asked each
bucket to declare its own minimal copy of every class. `Vector` has 98
different definitions in 244 files, `CExoString` 165 in 216, `CAppManager`
73 in 213 and `CClientExoApp` 144 in 154. The copies disagree in ways that
change code (an inline constructor in one copy and not another, `class`
here and `struct` there, a virtual destructor or not), so each copy is a
claim about the original that the others contradict. The migration plan
below retires them.

## Project profile

### Compiler and dialect (R22)

- Visual Studio .NET 2002, `cl` 13.00.9466, `-O2 -Gy -GX`, linked
  2004-02-12 (Rich header). C++98 with that compiler's extensions.
- Rejected: `nullptr`, `auto` as a type, `constexpr`, `enum class`,
  `<cstdint>`, `static_assert`, `override`, variadic templates.
- `for`-scope: MSVC 7.0 keeps a `for`-header variable in scope after the
  loop unless `/Zc:forScope` is passed, and `matchcmp.py` does not pass it.
  A second `for (int i ...)` in the same scope is an error.
- `bool` is one byte and mangles as `_N`; BioWare's own code mostly uses
  `int` flags (`int bActive`). Use `bool` only where the mangled name or
  the code shows it (R6).
- Structs default to 8-byte packing. `#pragma pack(push, 4)` is genuine
  where an `__int64` member sits at a 4-aligned offset.

### Annotations

- `// FUNCTION: SWKOTOR 0x<address>` directly above a definition, with
  `SYMBOL: <decorated name>` where the name alone is ambiguous.
- An annotation with no definition below it (a blank line, another
  annotation or an explicit instantiation follows) is a compiler-generated
  function. It is judged by the types its symbol names.
- `// ORIGINAL-ASM: <evidence>` within the eight lines above an annotation
  marks a function the evidence shows BioWare wrote in assembly (the Rich
  header records MASM objects, so some exist). Such a function is listed
  separately and counted neither way (R18). A naked
  function that replays MSVC's own prologue, EH frame and calls is
  compiler output, not original assembly, and must not carry the mark.
- `// ASM-HELPER: <evidence>` on an `__asm` line inside an otherwise-C++
  function marks an inline-assembly helper the original had (R18).
- Parking: the best failed attempt stays annotated and compiles as
  `NONMATCH`. There is no `#ifdef NON_MATCHING`; R37 applies in full.

### Naming (R7, R8, R29)

The Mac PowerPC builds kept BioWare's symbol names
([research/mac-ppc-names.md](research/mac-ppc-names.md)), so the style is
recorded, not chosen:

| entity | style | examples |
| --- | --- | --- |
| class | `C` + PascalCase; subsystem infix `SWS` server, `SWC` client, `SWGui`, `Exo`, `Aur` | `CSWSCreature`, `CSWCCreature`, `CExoString`, `CAurObject` |
| plain struct | PascalCase, often without `C` | `Vector`, `Quaternion`, `AurRes` |
| method | PascalCase, verb first; recorded lower-case names stay verbatim | `GetObjectTableManager`, `setCaptureEventRate` |
| member | `m_` + Hungarian type prefix + PascalCase | `m_nCount`, `m_bPaused`, `m_pClientExoApp`, `m_sName`, `m_fScale`, `m_oidTarget`, `m_cResRef`, `m_lEvents` |
| parameter, local | Hungarian prefix, no `m_` | `nSize`, `bActive`, `pObject`, `sName`, `cResRef`, `fDelta`, `oidTarget` |
| global | `g_` + Hungarian | `g_pAppManager`, `g_pRules`, `g_fStartupDelay` |
| free function | PascalCase; Aurora layer `Aur` prefix | `AurStartupWait`, `ComputeCRC` |
| enum value, constant | `UPPER_SNAKE` with a shared prefix | `EFFECT_TYPE_DAMAGE_REDUCTION` |

Hungarian prefixes: `n` integer, `b` boolean `int`, `p` pointer, `s`
`CExoString`, `c` object by value or reference (`CResRef`), `f` float,
`l` list, `oid` object id, `sz` C string, `a` array.

Placeholders, reported as open naming work (never as a failed match):
`FUN_`, `DAT_`, `sub_`, `unk_`, `local_`, `param_`, `field_0x`, `iVar3`,
`uStack`, `var3`; address-derived names (`Leaf9460`, `GetFromList920`,
`CClientUnknown700`); offset-named members (`m_n04`, `m_s18`); vtable slot
placeholders (`v00()`, `v0c()`); offset padding (`pad_0x1c`). A name taken
from the Mac symbols is evidence and is kept verbatim. A name we invent
says so in a comment.

### Layout (R23, R27, R32, R33)

The original's layout is unknown (no source survives). This records the
track's existing style so it stays consistent:

- 4-space indent, no tabs. Function bodies open on their own line (Allman);
  control statements, classes and enums open on the same line.
- Braces on every `if`/`else`/loop body except an empty loop body, which is
  `;` on its own line.
- Lines up to 120 columns. 471 lines are longer today; wrap them in
  formatting-only commits.
- One member per line in classes, with the offset in a trailing comment
  (`int m_nCount; // 0x1c`). An unknown range is `char pad_0x1c[0x34 - 0x1c];`:
  that spelling is this project's R27 offset placeholder and is allowed
  only for a range no recovered code touches. Bare `pad`, `padN` and
  `m_padN` are R17 findings: rename them with their offset.
- Unit opening (R32): `// <unit> -- <what this unit does>`, includes, unit
  data, functions in address order.
- Function header (R33): a `//` comment directly above the annotation that
  says what the function does when that isn't obvious, its contract, and
  anything surprising. When the purpose is unknown, write the name only,
  or a `guess:` line, never a confident invented description.

### Line information (R23)

The retail exe has no debug information, and no matching source uses
`assert`, `__LINE__` or `__FILE__`. Layout and comment changes are
therefore byte-free today. A unit that adds any of those makes its layout
changes [checked].

### Evidence sources (R7, R12, R15)

In order of strength:

1. Byte-identical code: `symbols.csv` (proved names), `status.csv`.
2. The Mac PowerPC symbol tables: names and parameter types for about
   13,300 functions, and the 2009 debug map's original object file for
   each (263 files, `source` column of `mac_names.csv`).
3. RTTI, vtables and strings in `swkotor.exe` (error strings that start
   with a function name, 2DA column names, GFF field labels).
4. The VS2022 reconstruction in `src/`: behaviour, callees and offsets, not
   names or types.

### Commands

| check | command |
| --- | --- |
| per function | `python matching/tools/matchcmp.py -v <file>` |
| byte gate | `python matching/tools/matchcmp.py -q --report matching/status.csv matching/src`; the MATCH count must not drop |
| symbol gate | `python matching/tools/symbolmap.py`; no new multi-address symbols |
| rule checker | `python matching/tools/rulecheck.py`; matched under the rules must not drop |

## Rule changes for this project

- **R21, relaxed for declarations.** An address comment on a callee or
  global declaration (`void Draw(float fDelta); // 0x0040cc50`) may stay:
  it records which original symbol the declaration binds to until the
  declaration moves to its shared header and `symbols.csv` covers it.
  Addresses, scores and campaign notes (`NONMATCH (16)`, `14 insns`) in
  function header and body comments are still R21 findings; they belong in
  the worker notes and in version control.
- **R15, review only.** The bucket folders are address ranges, not
  original object files, so units do not yet follow the original's
  boundaries. Name new units after the Mac debug map's object file, and
  never split a unit only to break a register-allocation tie (see
  README.md, "Source-shape notes").
- No other rule is relaxed. In particular R1 is not: a byte-identical
  function that depends on a disagreeing local class copy is MATCH-PENDING.

## Migration plan: one declaration per type (R1, R2)

The copies exist so parallel workers never edit the same file. They can be
retired one class at a time without losing that property:

1. **Shared headers.** Each class gets one header under
   `matching/include/common/`, named after the original object file in the
   Mac debug map (`CAppManager` in `appmanager.h`, `CClientExoApp` in
   `clientexoapp.h`). The coordinator owns `common/`; workers propose
   member additions in their notes or a header-only change, and the
   coordinator merges them with a full byte gate.
2. **Order.** Start with small, layout-stable types that many units copy:
   `Vector`, `Quaternion`, `CResRef`, `CExoString`, `CExoArrayList<T>`,
   then the large owners (`CAppManager`, `CClientExoApp`,
   `CServerExoApp`, `CSWSCreature`). `rulecheck.py` lists the worst types
   first.
3. **One definition.** Write the union of the proven members at their
   offsets, `pad_0x..` only for ranges no unit touches, virtuals in vtable
   order, and inline methods only where the binary shows them inlined.
   Where copies disagree, the bytes decide: an inline constructor that one
   unit needs and another can't tolerate is evidence that something else is
   wrong (a missing `#pragma auto_inline`, a different header in the
   original, a wrong type).
4. **Switch units in batches.** Replace a batch of local copies with the
   include, rerun `matchcmp.py` on those units and then the full byte gate.
   A function that regresses keeps its unit's copy for now and stays
   MATCH-PENDING; record why in the worker notes. Never keep a regression.
5. **Globals.** Move each `extern` from the `.cpp` files into the header of
   the unit that owns the address. Declarations don't change code, so this
   is cheap, but run the byte gate: a changed type can change codegen.
6. **Report both numbers** after each batch. Progress is the rise in
   matched under the rules with no drop in bytes matched.

## Fakematches found

These byte-identical functions match through code no BioWare programmer
wrote. Each is a clue to the real source:

| function | finding | likely real source |
| --- | --- | --- |
| `CopyString` 0x00406C70 | naked `__asm` body that replays MSVC's EH frame (`push 0x71428f`, `fs:[0]`), which only the compiler emits (R18) | the C++ function with a local object that has a destructor |
| `CExoResMan::CacheResource` 0x004085F0, `ComputeCRC` 0x00410560, `AurResGetNextLineFile`, `AurResGetNextLineData`, `AurResReadData`, `AurResGetNextLine` (0x0044BE20–0x0044BFA0), `AurResGetResource` 0x0044C680 | naked `__asm` bodies transcribed from the disassembly (R18) | the C++ that compiles to them. Nothing yet shows BioWare wrote these in assembly; if evidence turns up, mark them `ORIGINAL-ASM` |
| `CSWGuiEditbox::IsSelectable` 0x00418670 | `__asm` to read a flag byte, because "the bitfield accessor compiles as a dword load" (R18) | a bitfield declared in an `unsigned char`-sized unit, or a byte member, instead of the current type (R6) |
| `FUN_004aadf0`, `FUN_004aae30` | `fld`/`frndint`/`fistp` in inline asm (R18) | probably genuine: MSVC 7.0 compiles a float-to-int cast as a call to `_ftol`, so the rounding sequence was likely an original inline-asm helper. Confirm it (the same sequence elsewhere in the binary), then mark the lines `ASM-HELPER:` |
| `CSWSCreatureStats::GetUnarmedDamageDice` 0x005A4D40 | `volatile int nSize` carrier (R17) | a call whose result the original kept |
| `CSWGuiStore::DeleteItemEntries` 0x006C1330, `CSWGuiSaveNamePanel::HandleOkButton` 0x006C9D50 | `(char*)this + 0x1760` casts and a `volatile` loop bound (R4, R17) | the panels' real members |
| `CGuiInGame::IsCameraDialog` 0x0062ABD0, `CSWGuiLevelUpCharGen::CancelLevelUp` 0x006E8E20 | offset casts (R4) | typed members |
| `ParseEventCaptureArgs` 0x00401700, `Camera::Animate` 0x0045D220, `CSWVirtualMachineCommands::ExecuteCommandVersusAlignmentEffect` 0x00545470 | empty `if`/`else if` bodies (R17) | a debug or logging macro that compiles to nothing in retail |
| `CSWSCreature::SetPortraitId` 0x004ED4C0 | `sResRef = sResRef;` (R17) | probably a conversion or an assignment through another type |
| `CSWCItem::GetEquippedBy` 0x00646630 and three other `CSWCItem` methods | `volatile` member `m_nNumActiveProperties` (R17) | the real access path to the property count |

`rulecheck.py -v` lists them all with line numbers.
