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
   Mac build defines once (with ctor/dtor variants resolved: GCC's
   complete-object `C1`/`D1`, else `C2`/`D2`; MSVC's `??_G` is GCC's `D0`).
2. **vtable**: vtables both builds name, slot by slot.
3. **strings**: the exact set of string literals a function references,
   when one unpaired function on each side has that set. PowerPC string
   addresses come from tracking `lis`/`addis`/`addi`/`ori` and the
   `bcl`/`mflr` PIC base.
4. **calls**: around each paired function, a single remaining unpaired
   callee or caller on each side, proposed by two paired functions.
5. **calls1**: the same with one proposal.

Given both builds, it pairs each separately. Where both name a function,
they must agree on `Class::method`; otherwise the function is left unnamed
and reported as a conflict.

### Accuracy

`--holdout 0.2` hides a fifth of the name pairs and checks what signals 2
to 5 recover. Two seeds per build:

| Signal | 2004 build | 2009 build |
| --- | --- | --- |
| vtable | 100% | 98–100% |
| strings | 97–100% | 93–99% |
| calls | 96–98% | 95–99% |
| calls1 | 94–96% | 89–90% |

Hiding the same names from both builds and combining, three seeds:

| Combined | Recovered | Correct |
| --- | --- | --- |
| both builds agree | 338–362 | **98.6–99.4%** |
| only one build pairs it | 94–113 | 87–96% |
| builds disagree (left unnamed) | 2–8 | – |

Where both builds pair a function, they agree on 5,008 of 5,059 (99.0%).

## Results

| | Functions |
| --- | --- |
| Game functions in the exe (stock LIBCMT excluded) | 8,910 |
| Named by this project | 6,369 |
| Paired with a Mac build | 5,875 |
| Builds disagree, left unnamed | 51 |
| **Paired and not named here** | **943** |
| of which both builds agree | **739** |
| Neither | 1,598 |

None of the 943 has been attempted in `matching/`. They cover the whole
game: server-side logic, UI, client side, client/server messages,
renderer (`GLRender`, `CAurInternalGL`, `Scene`), NWScript commands and
the minigames (`CSWMiniGame`, `CSWMiniPlayer`, Pazaak), four of which have
no playable `src/` definition yet.

### Source files and link order

The 2009 debug map gives the original object file for the 5,642 Windows
functions paired with that build. In address order these fall into long
runs, one object at a time: the exe is laid out file by file, as MSVC links
`.obj` files. The single-function interruptions are inline helpers that
GCC emitted in a different file than MSVC did.

- 282 runs of at least three functions cover 5,462 of the 5,642.
- Merged, they give an inferred link order of 237 runs over 221 object
  files, beginning `appmanager`, `main`, `exoresman`, `guiingame`, `swgui`,
  `swguiabilitieschargen`, `swgui`, `exoresfile`, `resgff`, `twodimarray`.
  STRUCTURE.md lists the original link order as open work for phase 4.
- 2,239 functions sit inside a run; 870 of them are still unnamed, and the
  run gives their likely source file (57 in `nwscreatureai`, 53 in
  `guiingame`, 39 in `aurglcore`, 33 in `swguipazaak`).

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

`matching/mac_names.csv` and `matching/link_order.csv` hold the results;
`todo.py` reads the first by default:

```
0x004A23B0  16 B  todo  src/units/1470/...-scene3d.cpp  AurVertexProgramMakeCurrent() [name, 2/2] (aurvertexprog.o)
```

Prefer names both builds agree on (`2/2`), and confirm any name against the
code before annotating. Once a function matches, its name is in this
project's own records like any other. To refresh both files from your own
copies of the Mac executables:

```bash
python matching/tools/ppcnames.py --exe swkotor.exe --ppc "<2004 app>/Contents/MacOS/Knights of the Old Republic" "<2009 app>/Contents/MacOS/XKOTOR" --out ../ppcnames-01 --repo
```
