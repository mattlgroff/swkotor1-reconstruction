# Project structure: one source, two builds

This project reconstructs the GOG Windows 1.03 `swkotor.exe` in two ways
that check each other:

| | Playable build | Matching build |
| --- | --- | --- |
| Work of | [mattlgroff](https://github.com/mattlgroff) | [RayesDiyab](https://github.com/RayesDiyab) |
| Goal | The game runs and can be modded | The source is the original, byte for byte |
| Toolchain | VS2022 MSVC + clang-cl, C++17 | Visual Studio .NET 2002 (MSVC 7.0, `cl` 13.00.9466), as named in the exe's Rich header |
| Where | `src/`, `build.py`, `build-manifest.json` | `matching/` |
| Proof | Manual play (see [STATUS.md](STATUS.md)) | Each function compiles to the original's bytes |
| Output | `swkotor-reconstructed.exe` | Per-function comparison now; the identical `swkotor.exe` later |

Today the modern-toolchain build covers 95.3% of the 9,252 functions and
builds the full game:
- 7,232 have their own definition
- 1,323 are folded into other functions
- 264 C runtime functions come from VS2022's library

5,252 (56.8%) are byte-identical. The tracker in
the README shows both, clearly separated.

Neither proof covers the other. Matching proves each function exact but
can't show the game works as a whole. Play shows the game works but can't
show any function is exact. The plan is to move functions from the playable
build to matched source one at a time, without ever breaking the playable
build.

## Credit

The modern-toolchain decompilation (`src/`, `build.py`, the generators, the
build manifest and STATUS.md) is mattlgroff's work. The byte-identical
decompilation (`matching/`: the matched sources and headers, the tools,
the LIBCMT and symbol-map findings, and the tracker) is RayesDiyab's work.
Both are co-maintained here. Contributions keep their authors' credit
through git history.

## Layout

```
src/, build.py, build-manifest.json, local_generators/   playable build (unchanged)
matching/
  src/<bucket>/*.cpp        matched sources; buckets are address ranges, see below
  include/<bucket>/*.h      2002-style class declarations at original offsets
  status.csv                result per annotated function (MATCH / NONMATCH)
  functions.csv             the exe's 20,397 function starts; kind "eh" = EH funclet
  crt_libcmt.csv            functions identical to the stock LIBCMT.LIB
  symbols.csv               symbol -> original address, the bridge (phase 2)
  assets/progress.svg       progress tracker, refreshed by CI
  tools/matchcmp.py         compile with MSVC 7.0 and compare each function
  tools/libmatch.py         find stock library functions in the exe
  tools/symbolmap.py        build symbols.csv from matched objects
  tools/ppcnames.py         pair functions with the Mac PowerPC builds' names
  tools/render_banner.py    render the tracker
.github/workflows/update-progress-tracker.yml
```

Buckets `a`..`n` are address ranges of the exe. They let parallel work
proceed without touching the same files, and say nothing about subsystems.

## Ownership

- **`src/` and the playable build** stay as they are until phase 2 is
  agreed. Matching changes do not edit `src/`.
- **`matching/`** holds matching work. A matching change adds or edits files
  there and regenerates `status.csv`. CI then redraws the tracker.
- Both follow [CONTRIBUTING.md](CONTRIBUTING.md): source and docs only, no
  original executables, assets, compiler binaries or generated original data.
  The tools take the user's own `swkotor.exe` and MSVC 7.0 install through
  `--exe`/`--msvc` or `KOTOR_ORIGINAL_EXE`/`KOTOR_MSVC7`.

## Phases

### 1. Side by side (this change)

The matching track lives next to the playable build. Today:
- **5,252** functions are byte-identical and **504** are attempts not yet
  identical, out of about 9,250 functions. EH funclets are excluded; the
  compiler emits those from their parent functions.
- **The C runtime is the stock VS.NET 2002 `LIBCMT.LIB`.** In the CRT band,
  395 of 447 functions (97% of the bytes) are identical to that library.
  Those need no source: the matching build links the library, as the
  original did. The tracker shows them as "stock LIBCMT".

### 2. The bridge: matched code inside the playable build

Every playable-build function is named after its original address
(`kotor_fastcall_005b2ef0`, `kotor_cdecl_...`). Matched code uses the
original C++ names (`CSWBaseItem::CSWBaseItem`). `matching/tools/symbolmap.py`
connects the two automatically. Each relocation in a byte-identical
function sits where the original holds the real address, so reading the
original there tells you which address each symbol stands for.
`matching/symbols.csv` currently maps 9,510 symbols:
- 5,232 functions defined by matched code
- 1,962 functions it calls
- 2,316 data symbols: globals, vtables and strings

With that map, `build.py` could gain an opt-in switch (proposed, not
implemented) that, for every matched function:
1. compiles the matched `.cpp` with the playable toolchain;
2. generates a `kotor_<cc>_<address>` wrapper that calls the matched
   implementation, and drops the playable definition for that address;
3. generates definitions for the symbols the matched code calls or reads,
   forwarding to the playable build's existing `kotor_<cc>_<address>`
   functions and globals.

Play-testing that build then tests matched code inside the real game. Open
questions before it can work:
- Matched code must also compile under clang-cl. 2002-era C++ mostly does;
  x87 inline asm and MSVC 7 pragmas need guards.
- Globals have to map to the playable build's data by address.
- 110 symbols resolve to more than one address. These are identical copies
  in the original, such as per-object string literals and duplicated
  inline bodies, or naming slips in matched headers. They need a rule.

### 3. Replace

When every function in a playable unit is matched, bridged and play-tested,
the unit's playable version can be retired, and the matched source becomes
the only source for that code.

### 4. Whole executable

Beyond functions: data, strings and vtables in source; the original link
order; linking `LIBCMT.LIB`; and comparing the whole linked `swkotor.exe`
with the original.

## Working on the matching track

See [matching/README.md](matching/README.md) for the annotation format, how
to run the tools, and the source-shape patterns that make functions match.

## Reviewing this change

1. **Scope:** `git diff main --stat` touches only `matching/`,
   `STRUCTURE.md`, `.github/workflows/update-progress-tracker.yml` and four
   lines of `README.md`. Nothing in `src/`, `build.py` or the build
   manifest changes, so the playable build is unaffected.
2. **Reproduce the numbers** (needs MSVC 7.0 and your exe; see
   [matching/CONTRIBUTING.md](matching/CONTRIBUTING.md)):
   ```powershell
   python .\matching\tools\matchcmp.py -q matching\src                 # 5,252 / 5,756
   python .\matching\tools\libmatch.py "<msvc7>\LIB\LIBCMT.LIB" "<swkotor.exe>" out.csv
   python .\matching\tools\render_banner.py                            # tracker, no compiler needed
   ```
3. **Contributing afterwards:** people and AI agents follow
   [matching/CONTRIBUTING.md](matching/CONTRIBUTING.md).
