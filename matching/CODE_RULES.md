# Code rules for the matching track

Identical bytes prove that a source file is faithful. They are not the goal:
the decompilation exists so people can read and understand the original
program. Every rule here serves one test, the **reader test**:

> Would a programmer on the original team, writing in this codebase's style
> and dialect, have typed this? And can a reader see what it does, and why,
> without opening the disassembly?

Source that produces the right bytes but fails the test is a **fakematch**:
a `volatile` that only forces a load, a raw offset cast, a pasted assembly
body, a padding member that pushes known members into place. A fakematch is
a clue to the real source, not a match.

Rule numbers (R1, R2, ...) are stable identifiers shared with the general
decompilation code rules this file follows. This file is the **project
profile**: it fixes what is particular to KotOR and MSVC 7.0, and it says
which rules this project tightens or relaxes, and why.

## Two laws

1. **The bytes win.** A change made for style must not change what the
   compiler emits. When a rule and the bytes disagree, keep the bytes and
   write `// RULE-EXCEPTION: <rule>, <reason the bytes give>` at the code. An
   exception bends a style rule. It never licenses fake code.
2. **Identical bytes alone are not a match.** A function counts as matched
   only when its bytes match *and* it passes every [match] rule below.
   Byte-identical but failing is `MATCH-PENDING`: it is reported, and it is
   not counted under the rules. Placeholder names (R7) are open work, not a
   failed match.

`matchcmp.py` checks the first law. `tools/rulecheck.py` checks the second.

## The [match] rules

A [match] rule blocks a match exactly where its checker row fires. Each row
is deliberately narrow so that genuine original code passes.

| rule | what it requires | how `rulecheck.py` tests it |
| --- | --- | --- |
| R1 | Declare each type once, in a shared header every user includes. | a class or struct defined in two files with different bodies; a name declared both `class` and `struct` (MSVC mangles the two differently) |
| R2 | Declare each global once, as `extern` in a header, with one type. | an `extern` data declaration in a `.cpp`; a global declared with different types in different files; a global read through a cast of its image address (`*(int*)0x007A2370`) |
| R4 | Use typed members, never fixed offsets. | `(T*)((char*)p + N)`; `this` in byte-pointer arithmetic |
| R13 | Vtables come from evidence: every declared virtual is a slot. | virtuals named `pad` or `dummy` (slot placeholders such as `v40()` are open naming work, not violations) |
| R17 | No fake code. | `volatile` on locals, or on globals and members without a `// volatile: <evidence>` note; scalar locals never used again, unless labelled R36; empty `if`/`else` blocks, unless labelled R36; bare `;` statements; `do { } while (0)` in a body; self-assignment; `+ 0`, `* 1`; members named `pad` or `_pad` |
| R18 | Assembly is not source. | `__asm`, `_asm`, `__emit` in a function body. Calls to a listed helper are allowed (below). |

An R36 label is a `// NOTE:` or `// BUG:` comment on the line, or inside the
block. It claims the original had this code, so it needs bytes that simpler
source can't explain: the call whose result is discarded, the compare whose
branch does nothing.

## Project profile

### Compiler and dialect

- Visual Studio .NET 2002, MSVC 7.0 (`cl` 13.00.9466), `-O2 -Gy -GX`.
- C++98 as MSVC 7.0 accepts it: no `nullptr`, `auto`, `constexpr`,
  `enum class` or `<cstdint>`. A variable declared in a `for` header stays
  visible after the loop, as in VC6 (`/Zc:forScope` is off by default), and
  7.0 also accepts a second `for (int i ...)` in the same scope.
- `bool` is one byte; BioWare used `int` (`BOOL`-style) for most flags. Use
  `bool` only where the bytes show a byte-wide boolean.
- The retail executable carries no line information and the matched sources
  use no `__LINE__`, `__FILE__` or `assert`. Layout changes are therefore
  free here (R23), but run the byte gate once per formatting batch anyway.

### Annotations and parking

- `// FUNCTION: SWKOTOR 0x<address>` directly above each definition;
  `SYMBOL: <decorated name>` where the name alone is ambiguous
  (README.md, "Annotations").
- An unfinished attempt stays annotated and is reported `NONMATCH`. Put a
  one-line `// NONMATCH (<n> diff insns): <what differs>` note above it. This
  is the project's parking mechanism; it needs no `#ifdef`.
- Functions with no body at the annotation (explicit template
  instantiations, compiler-generated deleting destructors) are reported
  `UNCHECKED` by `rulecheck.py`: their source is a template or a class
  elsewhere.

### Listed inline-assembly helpers (R18)

| helper | header | evidence |
| --- | --- | --- |
| `FloatToInt(int* pOut, float f)` | `shared/float_to_int.h` | the same `fld` / `frndint` / `fistp [ptr]` sequence is inlined in functions across the engine (combat, effects, emitters, images, renderer, texture controllers) |

Add a helper here only with evidence that the original had it, and give it
one shared header. A function whose whole body is assembly is never a match:
the `__declspec(naked)` bodies in `d/exo_string_copy.cpp`,
`d/resman_cache_resource.cpp`, `d/res_gff_misc.cpp`, `g/console.cpp`,
`g/console_reg.cpp` reproduce compiler output (one pushes the EH handler
address `0x71428f` by hand) and stay `MATCH-PENDING` until rewritten as C++.

### Naming style

The game is BioWare's Aurora engine (shared with Neverwinter Nights), and the
Mac ports kept its symbol tables, so most names have direct evidence
(`mac_names.csv`, `research/mac-ppc-names.md`).

| entity | style | examples |
| --- | --- | --- |
| class | `C` + PascalCase; `CExo` for the engine base, `CSW` for the game, `CSWS`/`CSWC` for server/client sides, `CAur` for the renderer | `CExoString`, `CSWSCreature`, `CSWCArea`, `CAurTexture` |
| renderer classes from the older Aurora core | plain PascalCase | `Gob`, `Scene`, `LightManager` |
| method | PascalCase, verb first | `GetEquippedBy`, `SetPortraitId`, `ClearAllActions` |
| member | `m_` + Hungarian prefix + PascalCase | `m_nCount`, `m_pArea`, `m_bHostile`, `m_fAlpha`, `m_sTag`, `m_cTextParams`, `m_lstEntries`, `m_oidSelf`, `m_vColor` |
| global | `g_` + Hungarian prefix | `g_pAppManager`, `g_pRules`, `g_vKeyMapLockedTextColor` |
| local, parameter | Hungarian prefix + PascalCase | `nIndex`, `pCreature`, `sFile`, `bLoadingGame` |
| constant | upper snake, as the game's script constants | `EFFECT_TYPE_DAMAGE`, `DURATION_TYPE_INSTANT` |

Hungarian prefixes: `n` integer, `p` pointer, `b` boolean, `f` float, `s`
`CExoString`, `c` embedded object, `l`/`lst` list, `a` array, `v` vector,
`oid` object id, `sz` C string.

**Placeholders**, reported as open naming work (R7): `FUN_<address>`,
`DAT_<address>`, `sub_`, `unk`, `field_0x`, address-derived names
(`g_vertex_program_size_008305d0`), offset-named members (`m_n48`,
`m_p94`), and offset-named unknown ranges (`char pad_0x1c[0x34 - 0x1c];`).
An offset-named range that no recovered code touches is honest (R27). A
bare `pad` is not: name the range by its offset.

Names invented here carry `descriptive` or `name guessed` in their comment,
as the headers already do. Never write a confident name the evidence
doesn't support.

### Layout style

- Function braces on their own line; K&R braces inside a function
  (`if (x) {`); four-space indent; `} else {` on one line.
- One member per line in classes, with the offset in a trailing comment
  (`// +0x44` or `// 0x44`).
- Layout changes go in their own commits, never mixed with code changes.

### Evidence sources

In order of strength: `symbols.csv` (names proven by byte-identical code),
the Mac symbol tables (`mac_names.csv`, `link_order.csv`), RTTI and vtables
in the executable, error strings that start with a function name
(`"CSWCAnimBase::LoadModel(): ..."`), the game's 2DA column and script
constant names, and the playable reconstruction in `src/` for behaviour.

### Commands

```bash
python matching/tools/matchcmp.py -q --report matching/status.csv matching/src   # byte gate
python matching/tools/rulecheck.py --report matching/rules.csv                   # rule gate
python matching/tools/rulecheck.py --list R4 R17                                 # findings
python -m unittest discover -s matching/tools -p "test_*.py"                     # tool tests
python matching/tools/mergetype.py TYPE shared/type.h                           # R1 merge; then --compile
```

`rulecheck.py` needs no compiler or game files. It reads `status.csv`, so
run it after the byte gate.

## Rule changes for this project

- **R2 is tightened.** A pointer cast of an image address
  (`*(Vector*)0x007A2408`) counts as an undeclared global. Declare the global
  and use its name: `symbols.csv` usually already has it.
- **R1 and R2 are not relaxed, but most of the tree predates them.** Each
  address bucket was given its own copy of the classes it uses, so that
  parallel workers never touched the same files. Those copies disagree:
  `CExoArrayList` names its members `element`/`num` in
  `l/sw_gui_class_selection.h` and `m_pArray`/`m_nCount` in
  `l/sw_gui_inventory_equip.h`, and `CGuiInGame` has a different member at
  `+0x14` in `l/sw_gui_dialog.h` and `n/gui_in_game_dialog.h`. Until they
  are merged into one declaration per type, the functions that use them are
  `MATCH-PENDING` under R1. New work must not add copies (CONTRIBUTING.md,
  step 4). Merged types live in `include/shared/`: `tools/mergetype.py` does
  the mechanical part, and [research/r1-merges.md](research/r1-merges.md)
  records each merge's evidence and gate results. Merged so far: `Vector`,
  `CExoString`, `CAppManager`, `CSWReentrantServerStats`,
  `CSWVirtualMachineCommands`, `CVirtualMachineCmdImplementer`.
  Globals have one declaration each in `include/shared/globals.h`, apart from
  the cases [research/r2-globals.md](research/r2-globals.md) lists.
- **R7 stays advisory.** Placeholder names never block a match.

## Fakematch patterns seen in this tree

Each is a clue to what the original programmer wrote, not an answer.

| pattern | usually hides | what to try |
| --- | --- | --- |
| `volatile` on a local, member or global | a missing inline accessor, an aliasing pointer, or nothing at all: the two removed so far (`CSWCItem::m_nNumActiveProperties`, the vertex-program globals) were dead | remove it and re-run; then the accessor |
| `*(T*)((char*)this + N)` | a member the local class copy lacks | the member, from the bucket's fuller header |
| `*(T*)0x007XXXXX` | a global with no declaration | the global's name from `symbols.csv` |
| a pasted `__asm` body | compiler output | the C++ that compiles to it |
| an empty `if` branch | a branch the original really tested and left empty | keep it, with a `// NOTE:` |
| an unused local initialised by a call | a value the original fetched and never used | keep it, with a `// NOTE:` |
| a padding member that pushes known members into place | unknown members | an offset-named range (`pad_0x1c`) |
