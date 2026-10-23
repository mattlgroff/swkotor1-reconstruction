# Names from the Mac PowerPC build

Notes on using Aspyr's 2004 Mac port as a naming source for this project:
what the build contains, how `tools/ppcnames.py` pairs it with the GOG exe,
how accurate that is, and what it turned up. Numbers are from the GOG 1.03
exe and this repository at the commit that added this note.

## The builds

| Build | CPU | Compiler | Symbols |
| --- | --- | --- | --- |
| GOG Windows 1.03 `swkotor.exe` | x86 | MSVC 7.0 | none |
| Aspyr Mac 1.03 (2004), `Knights of the Old Republic` | PowerPC | Apple GCC | **16,178 symbols** |
| Aspyr Mac 1.4.0 (Steam), `KOTOR_Exe` | x86_64 | Apple clang | none (C++ RTTI type names only) |

The PowerPC executable this note used is 5,507,392 bytes, SHA-256
`124ef9b20902b604ec31d313ac142f8397bc5563f3225fd5c9312e74c59d6864`.
Current `nm` refuses it (obsolete load command 16, `LC_PREBOUND_DYLIB`), so
`ppcnames.py` reads the `LC_SYMTAB` itself.

Both Mac builds compile the same game source as the Windows exe over an
Aspyr platform layer of Windows stand-ins: `HWND_Mac`, `HDC_Mac`,
`HANDLE_Mac`, `HFONT_Mac`, `IDirectInput_Mac` and
`IDirectInputDeviceA_Mac`. The PowerPC build links the real Miles Carbon
library; the 1.4.0 build replaces Miles with an OpenAL layer
(`Miles::DriverOpenAL`). The game classes keep the names this project
uses: of the 4,385 `Class::method` names that matched code defines in
`symbols.csv`, 3,064 also appear in the PowerPC build.

## Method

`ppcnames.py` pairs functions with five signals, strongest first:

1. **name**: a name `status.csv` or `symbols.csv` already holds that the
   PowerPC build defines once (with ctor/dtor variants resolved: GCC's
   complete-object `C1`/`D1`, else `C2`/`D2`; MSVC's `??_G` is GCC's `D0`).
2. **vtable**: the 145 vtables both builds name, slot by slot.
3. **strings**: the exact set of string literals a function references,
   when one unpaired function on each side has that set. PowerPC string
   addresses come from tracking `lis`/`addis`/`addi`/`ori` and the
   `bcl`/`mflr` PIC base.
4. **calls**: around each paired function, a single remaining unpaired
   callee or caller on each side, proposed by two paired functions.
5. **calls1**: the same with one proposal.

### Accuracy

`--holdout 0.2` hides a fifth of the name pairs, runs signals 2 to 5 and
checks what they recover. Three seeds:

| Signal | Recovered | Correct |
| --- | --- | --- |
| vtable | 9–12 | 92–100% |
| strings | 66–73 | 97–100% |
| calls | 79–91 | 96–98% |
| calls1 | 240–249 | 94–96% |

The name pairs themselves agree with the strings: of 439 name pairs where
both functions reference strings, 438 share at least one.

## Results

| | Functions |
| --- | --- |
| Game functions in the exe (stock LIBCMT excluded) | 8,910 |
| Named by this project | 6,369 |
| Paired with the PowerPC build | 5,343 |
| Paired and not named here | 865 |
| Neither | 1,676 |

All 865 newly named functions are ones nobody has attempted in `matching/`;
691 of them have a playable `src/` definition. Spot checks agree with the
playable sources' own file names. The 1,676 left are mostly functions GCC
inlined, Windows-only code and compiler helpers.

Names this project uses but the PowerPC build lacks: 2,424. Examples are
Windows-specific functions (`PumpMessages`, `UpdateWindowTitle`), MSVC
helpers (`` `vector constructor iterator' ``) and inline functions GCC never
emitted out of line.

## Findings for this repository

- **`functions.csv` lacks 600 function starts that `status.csv` or
  `symbols.csv` name**, such as `CSWGuiText::Load` at `0x00416050`. A
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

## Using the names

The paired names come from Aspyr's binary, so `ppcnames.py` writes them
outside the checkout and they are not committed here. Use them as leads:

```bash
python matching/tools/ppcnames.py --exe swkotor.exe --ppc "<mac app>/Contents/MacOS/Knights of the Old Republic" --out ../ppcnames-01
python matching/tools/todo.py --names ../ppcnames-01/pairs.csv
```

Confirm a name against the code before annotating. About one `calls1`
name in twenty is wrong. Once a function matches, its name is in this
project's own records like any other.
