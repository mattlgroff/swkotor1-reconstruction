# Names from the Mac PowerPC builds

Notes on using Aspyr's Mac ports as a naming source for this project: what
the builds contain, how `tools/ppcnames.py` pairs them with the GOG exe, how
accurate that is, and what it turned up. Numbers are from the GOG 1.03 exe
and this repository at the commit that added this note.

## The builds

| Build | CPU | Symbols | Named functions | Source file per function |
| --- | --- | --- | --- | --- |
| GOG Windows 1.03 `swkotor.exe` | x86 | none | none | no |
| Aspyr Mac 1.03 (2004), `Knights of the Old Republic` | PowerPC | 16,178 | ~10,300 | no |
| Aspyr Mac 1.03e (2009), `XKOTOR` | universal: PowerPC, i386 | 79,571 (ppc slice) | ~13,300 | **yes** |
| Aspyr Mac 1.4.0 (Steam), `KOTOR_Exe` | x86_64 | none | none (RTTI type names only) | no |

Executables this note used:

| Build | Size | SHA-256 |
| --- | --- | --- |
| 2004 PowerPC | 5,507,392 | `124ef9b20902b604ec31d313ac142f8397bc5563f3225fd5c9312e74c59d6864` |
| 2009 universal | 15,847,792 | `7b664cbf3f85b0a693f7dc1ba3112038e4725173ca3a56b62d8686c1c84ef28e` |

Current `nm` refuses the 2004 executable (obsolete load command 16,
`LC_PREBOUND_DYLIB`), so `ppcnames.py` reads the `LC_SYMTAB` itself. It reads
a universal binary through its ppc slice.

The 2009 build carries a **debug map**: for each function, the object file
it came from (`N_OSO` then `N_FUN` stabs), with paths in BioWare's source
tree. The map has no type stabs, so no class layouts. Its source files by
directory:

| Directory | Files |
| --- | --- |
| `PC Source/Dev/game/clientcore` | 128 |
| `PC Source/Dev/game/servercore` | 58 |
| `PC Source/Dev/libsource/AURORA` | 35 |
| `PC Source/Dev/libsource/exobase` | 19 |
| `PC Source/Dev/libsource/aurorainterface` | 17 |
| `PC Source/Dev/libsource/exoresman` | 15 |
| `PC Source/Dev/libsource/nwscript` | 8 |
| other `libsource` and `game` directories | 22 |
| Aspyr's `ASL` library and `Mac Source` | 57 |

All three Mac builds compile the game over an Aspyr layer of Windows
stand-ins (`HWND_Mac`, `HDC_Mac`, `HANDLE_Mac`, `IDirectInput_Mac`, and in
2009 `DInput_Mac`, `DSound_Mac`, `MilesWrapper`). The game classes keep the
names this project uses: of the 4,385 `Class::method` names that matched
code defines in `symbols.csv`, 3,064 appear in the 2004 build.

## Method

`ppcnames.py` pairs functions with five signals, strongest first:

1. **name**: a name `status.csv` or `symbols.csv` already holds that the
   Mac build defines once with compatible parameters, exact ones preferred
   (with ctor/dtor variants resolved after that: GCC's complete-object
   `C1`/`D1`, else `C2`/`D2`; MSVC's `??_G` is GCC's `D0`). Compatible
   means the same count and method `const`, and per parameter only these
   listed differences (`param_match`): another spelling or signedness of
   the same integer width; a repo `int` for a narrower integer or `bool`;
   a pointer for a reference to the same type at the same depth, with or
   without `const`; `void*` for any pointer; and `char*`/`unsigned char*`
   for each other. Another class, a callback type, a float or a 64-bit
   value must be equal. An address the CSVs give two different names (a
   body `/OPT:ICF` folded, two `status.csv` rows, or `status.csv` against
   `symbols.csv`) is not paired by name; one name with several parameter
   lists pairs if any of them fits.
2. **vtable**: vtables both builds name, slot by slot, unless a slot
   contradicts a name pair already made.
3. **strings**: the exact set of string literals a function references,
   when one unpaired function on each side has that set. PowerPC string
   addresses come from tracking `lis`/`addis`/`addi`/`ori` and the
   `bcl`/`mflr` PIC base.
4. **calls**: around each paired function, a single remaining unpaired
   callee or caller on each side, proposed by two distinct paired
   functions. On Windows the calls come from decoding each instruction, so
   an `E8` byte inside an operand is not a call. On the PowerPC, register
   values follow the function's branches: where paths meet only the values
   they agree on survive, and calls, update forms and multi-register loads
   change what they change.
5. **calls1**: the same with one proposal.

Given both builds, it pairs each separately. Where both name a function,
they must agree on the full signature, `Class::method` and parameters;
otherwise the function is left unnamed and both candidates are listed.

### Accuracy

`--holdout 0.2` freezes the name pairs first, then hides one seeded fifth
of them (699 of 3,497) from both builds and checks what signals 2 to 5
recover; the pairs not hidden seed them as they are. The hidden functions
are ones the name signal can pair, likely easier than functions no name
reaches, so read these as upper bounds. Three seeds:

| Signal | 2004 build | 2009 build |
| --- | --- | --- |
| vtable | 100% | 100% |
| strings | 97–98% | 94–98% |
| calls | 95–99% | 91–100% |
| calls1 | 92–95% | 86–89% |
| coverage | 52–55% | 46–49% |

Combined over the same hidden functions:

| Combined | Recovered | Correct |
| --- | --- | --- |
| both builds agree | 279–290 | **97.9–98.9%** |
| only one build pairs it | 85–105 | 87–93% |
| builds disagree (left unnamed) | 4–10 | – |
| coverage | 53–55% of the hidden | |

Where both builds pair a function, they agree on the full signature for
4,655 of 4,731 (98.4%).

## Results

| | Functions |
| --- | --- |
| Game functions in the exe (stock LIBCMT excluded) | 8,910 |
| Named by this project | 6,369 |
| Paired with a Mac build | 5,476 |
| Builds disagree, left unnamed | 76 |
| **Paired and not named here** | **909** |
| of which both builds agree | **705** |
| Neither | 1,632 |

A name pair also records how its parameters agree with the repo's: 2,972
exact, 282 compatible, 221 unknown (a `status.csv` name without
parameters). 282 and 294 repo names (2004, 2009 build) were refused for
their parameters (`signature_rejects.csv`), and 610 addresses the CSVs name
differently are not paired by name. 30 of the 2009 build's vtables were
dropped because a known pair contradicted their alignment: that build adds
a virtual (`AsNavigable`) to the `CSWGuiControl` family, shifting every
later slot.

None of the 909 has been attempted in `matching/`. They cover the whole
game: server-side logic, UI, client side, client/server messages,
renderer (`GLRender`, `CAurInternalGL`, `Scene`), NWScript commands and
the minigames (`CSWMiniGame`, `CSWMiniPlayer`, Pazaak), four of which have
no playable `src/` definition yet.

### Source files and link order

The 2009 debug map gives the original object file for the 5,200 Windows
functions paired with that build. In address order these fall into long
runs, one object at a time: the exe is laid out file by file, as MSVC links
`.obj` files. The single-function interruptions are inline helpers that
GCC emitted in a different file than MSVC did.

- 266 runs of at least three functions cover 5,031 of the 5,200.
- Merged, they give an inferred link order of 226 runs over 216 object
  files, beginning `appmanager`, `main`, `exomovieplayerinternal`,
  `exoresman`, `guiingame`, `swgui`, `swguiabilitieschargen`, `swgui`,
  `exoresfile`, `resgff`.
  STRUCTURE.md lists the original link order as open work for phase 4.
- 2,593 functions sit inside a run; 860 of them are still unnamed, and the
  run gives their likely source file (56 in `nwscreatureai`, 53 in
  `guiingame`, 39 in `aurglcore`, 35 in `nwccreature`). `mac_names.csv`
  marks these `neighbours`, apart from the `debug map` ones.

## Findings for this repository

- **`functions.csv` lacks 600 function starts** that `status.csv` or
  `symbols.csv` name, such as `CSWGuiText::Load` at `0x00416050`. A
  tool that takes extents from `functions.csv` alone sees the function
  before each one run on into it. `todo.py` and `ppcnames.py` add the
  `status.csv` addresses; `ppcnames.py` also adds the `symbols.csv`
  addresses and the exe's direct call targets. To list the gap:
  ```bash
  python -c "import csv; rows=lambda n: list(csv.DictReader(open('matching/'+n))); f={int(r['address'],16) for r in rows('functions.csv')}; n={int(r['address'],16) for r in rows('status.csv')+[r for r in rows('symbols.csv') if r['role']!='data']}; print(len(n-f), ['0x%08X' % a for a in sorted(n-f)])"
  ```
- **MSVC lays out overloaded virtual functions in reverse.** In
  `CSWCAnimBase` and `CSWCAnimBaseTW`, `ReplaceTexture(CResRef const&, ...)`
  and `ReplaceTexture(unsigned char, ...)` occupy swapped slots in the two
  builds. Declaring a class's virtual functions "in vtable order" for
  matching means declaring an overload set in the reverse of its slot order.
- **`/OPT:ICF` folds tiny bodies across classes.** A vtable slot can point
  to a body another class also uses (the 4th slot of `CSWGuiPanel` lands on
  the body named `CSWCCreature::DoNothing`), so a slot target is not proof
  of a name.
- **The exe follows BioWare's source files**, so `matching/src/` could
  group functions by original file instead of only by address bucket.

## Using the names

`matching/mac_names.csv` and `matching/link_order.csv` hold the results,
as hints kept apart from `symbols.csv`; `todo.py` shows the first only
when asked:

```
python matching/tools/todo.py --names matching/mac_names.csv
0x004A0030  16 B  todo  src/units/0658/...-behavior_tables.cpp  (near aurbehavior.o)
0x004A23B0  16 B  todo  src/units/1470/...-scene3d.cpp  AurVertexProgramMakeCurrent() [name, 2/2, exact] (aurvertexprog.o)
```

`near` marks a source file inferred from the neighbouring functions rather
than from the function's own pair.

Prefer names both builds agree on (`2/2`), and confirm any name against the
code before annotating. Once a function matches, its name is in this
project's own records like any other. To refresh both files from your own
copies of the Mac executables:

```bash
python matching/tools/ppcnames.py --exe swkotor.exe --ppc "<2004 app>/Contents/MacOS/Knights of the Old Republic" "<2009 app>/Contents/MacOS/XKOTOR" --out ../ppcnames-01 --repo
```
