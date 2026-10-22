# Contributing byte-identical code

This guide is for people and AI coding agents adding to the byte-identical
decompilation in `matching/`. It is the original-toolchain build described
in [STRUCTURE.md](../STRUCTURE.md); the general rules in
[CONTRIBUTING.md](../CONTRIBUTING.md) apply as well. Commands use forward
slashes, which work in PowerShell, Git Bash, Linux and macOS alike.

## 1. Setup

You need three things that this repository does not ship:

- **Visual Studio .NET 2002 (MSVC 7.0, `cl` 13.00.9466)** from your own
  licensed copy. Use the `Vc7` folder, which contains `BIN/CL.EXE`,
  `INCLUDE` and `LIB`. Later versions (2003 and up) produce different code,
  and no other version will match.
- **Your purchased GOG 1.03 `swkotor.exe`**, SHA-256
  `9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435`.
  Keep it outside the checkout.
- **Python 3 with `pefile`**: `python -m pip install pefile==2024.8.26`.

Point the tools at the first two. Environment variables last only for the
current shell, and AI agents often get a fresh shell per command, so set
them in the same command line as the tool or pass `--msvc`/`--exe`:

```powershell
# PowerShell
$env:KOTOR_MSVC7 = "C:/Tools/msvc7"; $env:KOTOR_ORIGINAL_EXE = "D:/Games/KotOR/swkotor.exe"
```

```bash
# Git Bash, Linux, macOS
export KOTOR_MSVC7="C:/Tools/msvc7" KOTOR_ORIGINAL_EXE="D:/Games/KotOR/swkotor.exe"
```

On Linux or macOS the compiler runs under Wine (`KOTOR_WINE` names the
launcher), and paths are handled for you. If you pass compiler flags
yourself, use the dash form (`-O2`): Git Bash rewrites `/O2` into a path.

**Check the setup.** This compiles every matched file, which takes from
about 10 seconds to a few minutes depending on the machine:

```bash
python matching/tools/matchcmp.py -q matching/src
grep -c ',MATCH,' matching/status.csv
```

The first command's "N/M functions match" must equal the second's count. If
it's lower, the compiler or exe is wrong.

## 2. Pick work

```bash
python matching/tools/todo.py                      # smallest functions with no source yet
python matching/tools/todo.py --range 0x5E0000 0x620000 -n 50
python matching/tools/todo.py --near               # attempts closest to matching
grep -i 0x0062B050 matching/status.csv            # is an address taken already?
```

`status.csv` uses uppercase hex, while `matchcmp` prints lowercase; `grep
-i` handles both.

**Where files go.** Sources live in `matching/src/<bucket>/` and headers in
`matching/include/<bucket>/`. Each bucket is an address range, so that
parallel workers never edit the same files:

| Bucket | Address range | Bucket | Address range |
| --- | --- | --- | --- |
| d | 0x00400000–0x0041FFFF | f | 0x00580000–0x005CFFFF |
| g | 0x00420000–0x0044FFFF | j | 0x005D0000–0x005DFFFF |
| h | 0x00450000–0x0047FFFF | c | 0x005E0000–0x0061FFFF |
| e | 0x00480000–0x0049FFFF | n | 0x00620000–0x0063FFFF |
| i | 0x004A0000–0x004CFFFF | m | 0x00640000–0x0067FFFF |
| a | 0x004D0000–0x0051FFFF | k | 0x00680000–0x0069FFFF |
| b | 0x00520000–0x0057FFFF | l | 0x006A0000–0x006EFFFF |

- **Working alone:** add the function to the existing file for its class if
  there is one, in any bucket (for example `matching/src/c/gui_in_game*.cpp`
  for `CGuiInGame`). Otherwise use the bucket for its address.
- **Working in parallel** (several people or agents at once): each takes
  one bucket and touches only that bucket's folders. Create them if they're
  missing (`mkdir -p matching/src/n matching/include/n`).
- The C runtime at 0x006F0000 and up is mostly the stock `LIBCMT.LIB`
  (`matching/crt_libcmt.csv`). Those need no source, and `todo.py` hides
  them.

Work in class clusters (constructor, destructor and their methods
together), since they share headers.

## 3. Match one function

1. **Name it from this project's records, not from guesses.** Look up the
   address and its callees before you write any declaration:
   ```bash
   grep -i 0x0062B050 matching/symbols.csv            # name, if matched code already uses it
   grep -rn "UpdateCombatMode\|CGuiInGame" matching/include | head
   ```
   - `symbols.csv` holds the names already proven by byte-identical code,
     and `matching/include` holds the class layouts.
   - The comments in `src/` sometimes name a function differently. A
     wrong name still compiles to matching bytes, because call targets are
     masked, so the tool can't catch it.
   - If neither file knows the function, choose a descriptive name and say
     so in a comment.
2. **Understand what it does.** `todo.py --address 0x0062B050` prints the
   modern-toolchain definition from `src/`. It's play-tested and cites the
   original instruction addresses, so read it for behaviour, member
   offsets, callees and exception-handling notes. Its raw-offset style
   (`wr32(self, 0x04, 0)`) never compiles to the original bytes, so don't
   paste it. If `todo.py` shows `-` (no `src/` definition), work from the
   disassembly alone.
3. **See the original.** Write a stub with the right name and annotation,
   then run `matchcmp -v`. It prints the original (left) next to yours
   (right), with `!!` on differing instructions:
   ```cpp
   // FUNCTION: SWKOTOR 0x0062B050
   void CGuiInGame::UpdateCombatMode() {}
   ```
   ```bash
   python matching/tools/matchcmp.py -v matching/src/n/gui_in_game_n.cpp
   ```
   `-v` prints nothing extra for a function that already matches.
4. **Write it in 2002 style:**
   - real classes with members at their original offsets, using padding
     arrays (`char pad_0x1c[0x34 - 0x1c];`) where a member is unknown
   - virtual functions in vtable order
   - callees declared but not defined
   - `__thiscall` member functions
   - nothing past C++98: no `nullptr`, `auto`, `<cstdint>` or `constexpr`.
     MSVC 7.0 rejects them.

   **Headers:**
   - Include paths are relative to `matching/include`:
     `#include "n/gui_in_game_n.h"`.
   - Each bucket may declare its own minimal copy of a class with just the
     members its functions use. Copy from other buckets' headers; don't
     include across buckets when working in parallel.
   - Use a classic `#ifndef` include guard named after the bucket and file.
5. **Annotate** each definition with `// FUNCTION: SWKOTOR 0x...` on the line
   above. Add `SYMBOL: <decorated name>` when the name alone is ambiguous
   (templates, overloads, compiler-generated functions).
6. **Iterate** until the tool prints `OK`. Judge progress by the "diff
   insns" count, and see the source-shape notes in [README.md](README.md).
7. **Timebox** each function to about 20 minutes or 15 compiles. If it still
   doesn't match, keep the best attempt annotated (it counts as NONMATCH) and
   move on. Most remaining misses are register-allocation or stack-slot ties
   that a later pass can revisit.

## 4. Before you commit

1. Note the count before your change (`grep -c ',MATCH,' matching/status.csv`),
   then rerun the whole track and refresh the report:
   ```bash
   python matching/tools/matchcmp.py -q --report matching/status.csv matching/src
   grep -c ',MATCH,' matching/status.csv
   ```
   - The count must not go down.
   - Look for lines like `!! 0x0062b050 annotated twice: <file> and <file>`;
     even with `-q` they go to standard output. Fix any before committing.
2. Run `python matching/tools/symbolmap.py`. It refreshes `symbols.csv` and
   prints every symbol that maps to more than one address. A new entry
   there usually means a naming slip in your change.
3. Commit the sources, headers, `matching/status.csv` and
   `matching/symbols.csv` together. Don't commit
   `matching/assets/progress.svg`: CI redraws it after the merge.
4. Never commit `swkotor.exe`, compiler files, object files or anything
   extracted from the game. Don't edit `src/` in a matching change.

## Reviewing a matching change

- The diff touches only `matching/`, and `status.csv` and `symbols.csv` were
  regenerated.
- Re-run `matchcmp.py -q` on the changed folders. Every function the change
  claims as matched must print `OK` with your own exe and compiler.
- No address is annotated twice, the MATCH count did not drop, and
  `symbolmap.py` reports no new multi-address symbols.
