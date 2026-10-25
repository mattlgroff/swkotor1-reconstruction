# Building on Windows

The build produces a 32-bit Windows executable. Use a separately purchased GOG Windows 1.03 installation, with executable version 1.0.3.0. Keep that installation outside this checkout.

## Prerequisites

Install these separately from their official sources:

- [Python for Windows](https://www.python.org/downloads/windows/). The local checks use Python 3.13.5.
- [Visual Studio 2022 C++ Build Tools](https://aka.ms/vs/17/release/vs_buildtools.exe), with the Desktop development with C++ components, MSVC v143 14.44 and [Windows SDK](https://learn.microsoft.com/en-us/windows/apps/windows-sdk/) 10.0.26100.0. The script selects these versions explicitly; newer defaults alone are insufficient.
- [LLVM 23.1.2](https://github.com/llvm/llvm-project/releases/tag/llvmorg-23.1.2), using the `clang+llvm-23.1.2-x86_64-pc-windows-msvc` distribution. Pass its `bin\clang-cl.exe` path. The runner checks version 23.1.2 and LLVM revision `85ac560262434c9ccfc0c183ec22d4138ed647fb`.
- [pefile 2024.8.26](https://pypi.org/project/pefile/2024.8.26/) in the Python environment used to run the build:

```powershell
python -m pip install pefile==2024.8.26 capstone==5.0.7
```

The c67 local math generator also requires [Capstone 5.0.7](https://pypi.org/project/capstone/5.0.7/) to decode the supported executable.

Toolchains and Python packages retain their own licenses. The build does not download or install them, or download game files.

## Original game inputs

`--game-dir` points to the installation directory containing `swkotor.exe`, `Mss32.dll`, `binkw32.dll` and the game's resources. The generator reads the original executable and middleware export tables locally. It does not run the original executable or DLLs.

The supported input hashes are SHA-256:

| File | SHA-256 |
| --- | --- |
| `swkotor.exe` (4,042,752 bytes) | `9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435` |
| `Mss32.dll` | `858d6066bab4445f56c2d1713f16bfa742fb43a149c6afcb83460efe5bdec7da` |
| `binkw32.dll` | `2d0ae23a6175dc7b635c402a5e7e9542e923c0d1c376a8c5ef876ca0d5959d23` |

These identify the inputs; they do not identify a GOG installer build. Do not place original files or extracted data in the source checkout.

## Local original-derived math dependency

The c67 build combines authored reconstruction with a local dependency generated from the supported original executable. Its L1 math generator decodes the original VC7 CRT implementation of `pow`, `acos`, `asin`, `floor`, `ceil` and their support routines, and extracts the lookup tables they require. This is original-derived code, not an independently reconstructed source implementation. The authored x87 intrinsic adapters and generator are distributed; generated assembly, original byte encodings, tables and compiled objects are not.

Generation reads only the hash-validated installation and writes into the fresh private build output. Keep those outputs out of Git and do not redistribute them. A checkout alone cannot supply this dependency. The existing reconstructed acos/asin/pow wrappers retain their own callers; the L1 addition does not mean every game math call has moved away from the modern CRT. See [STATUS.md](STATUS.md) for coverage and verification limits.

## Build

Run from the checkout root in PowerShell. Replace the example paths with your installation and toolchain locations. Choose a new output directory outside both the checkout and the game installation; an existing output directory is rejected.

```powershell
python .\build.py --target game --game-dir "D:\Games\Star Wars - KotOR" --clang-cl "C:\Tools\LLVM-23.1.2\bin\clang-cl.exe" --out "D:\KotorBuilds\game-01" --jobs 4
```

If Build Tools is installed elsewhere, add `--vs-root "C:\Path\To\VisualStudio2022\BuildTools"`. `--jobs` accepts 1 through 16. The script configures the x86 compiler environment itself.

The build validates the source manifest, generates required original-backed data under `kotor_generated`, and automatically generates the L1 math dependency under `local_math` in the output directory using the same `--game-dir`. No separate generation command is needed. It assembles the math code with the installed MSVC MASM, compiles the authored sources, and links `swkotor-reconstructed.exe`. Before accepting the result it checks the generated math code/data against the original and verifies the required linked math providers. Missing inputs or incorrect providers cause a build failure rather than acceptance of a modern-CRT fallback. The output directory also contains object files, compiler logs, `link.log`, `link.map`, and a `build-receipt.json` after successful completion. A failed build leaves its logs for diagnosis; use another new output directory for the next attempt. There is no reused math cache: every build uses a fresh output directory, regenerates the dependency, and records its file hashes in the build receipt.

Keep the entire output directory private. Its generated original-derived math code and tables, constants, resource data and import libraries are local build inputs, not distributable project source. The output directory receives a `.gitignore`; do not upload it or its executable as part of a source contribution. The game build does not launch the resulting game.

See [STATUS.md](STATUS.md) for build and runtime results and their limits.

## Prepare a runtime copy

After a successful build, create a fresh runtime directory outside the checkout, installation and build output. Copy these directories from your purchased game: `data`, `lips`, `miles`, `modules`, `movies`, `Override`, `rims`, `streammusic`, `streamsounds`, `streamwaves`, and `TexturePacks`. Also copy `chitin.key`, `dialog.tlk`, `patch.erf`, `Mss32.dll` and `binkw32.dll`. The `miles` directory contains audio plugins; video uses `binkw32.dll`.

Use real copies, not junctions or symbolic links. Leave the original executable, saves, installers and configuration in the installation. Copy the newly built `swkotor-reconstructed.exe` into the runtime directory as `swkotor.exe`. Keep a `.gitignore` containing `*` in that directory.

For example, after copying the listed assets, use these PowerShell commands with your paths:

```powershell
$buildDir = 'D:\KotorBuilds\game-01'
$runtimeDir = 'D:\KotorRuntime\run-01'
$builtExe = Join-Path $buildDir 'swkotor-reconstructed.exe'
$receipt = Get-Content -LiteralPath (Join-Path $buildDir 'build-receipt.json') -Raw | ConvertFrom-Json
if ((Get-FileHash -LiteralPath $builtExe -Algorithm SHA256).Hash -ne $receipt.executable_sha256) {
    throw 'Executable differs from its build receipt.'
}
if (Test-Path -LiteralPath (Join-Path $runtimeDir 'swkotor.exe')) {
    throw 'Use a fresh runtime copy without an existing executable.'
}
Copy-Item -LiteralPath $builtExe -Destination (Join-Path $runtimeDir 'swkotor.exe')
Set-Content -LiteralPath (Join-Path $runtimeDir '.gitignore') -Value '*' -Encoding ASCII
```

Keep empty directories when copying assets. Run the setup command below before launch: the menu queries free space using the `saves` directory, and a missing directory can produce a misleading insufficient-space warning.

Create a fresh `swkotor.ini` in the runtime directory with these relative asset and output paths. Both graphics options below are needed to request windowed mode:

```ini
[Alias]
HD0=.\
OVERRIDE=.\Override
TEMP=.\Temp
MODULES=.\Modules
LOGS=.\Logs
LOCALVAULT=.\LocalVault
DMVAULT=.\DMVault
SERVERVAULT=.\ServerVault
STREAMMUSIC=.\StreamMusic
TEMPCLIENT=.\Tempclient
SAVES=.\saves
CURRENTGAME=.\currentgame
GAMEINPROGRESS=.\gameinprogress
TEXTUREPACKS=.\texturepacks
STREAMWAVES=.\StreamWaves
LIPS=.\lips

[Graphics Options]
AllowWindowedMode=1
FullScreen=0
```

The `LIPS` alias points to the copied lip-animation resources; missing resources can leave mouths still while voiced dialogue continues. For fullscreen instead, set `FullScreen=1` in the same `[Graphics Options]` section. Keep these changes in the runtime copy, not the original installation.

From the source checkout, create the required empty runtime directories:

```powershell
python .\build.py --prepare-runtime $runtimeDir --game-dir "D:\Games\Star Wars - KotOR"
```

This setup command creates `Override`, `Temp`, `Logs`, `LocalVault`, `DMVault`, `ServerVault`, `Tempclient`, `saves`, `currentgame` and `gameinprogress`. It is safe to repeat: existing directories and their contents are preserved. It rejects source/install destinations and linked runtime subdirectories. It does not copy assets, change the INI, build, or launch the game. Use the relative aliases shown above.

Launch from that directory so relative resources, configuration and saves belong to the copy:

```powershell
Push-Location -LiteralPath $runtimeDir
try { & .\swkotor.exe } finally { Pop-Location }
```

Keep the runtime copy private: it contains original game assets and middleware. These steps describe preparation and launch; observed runtime behavior is recorded in [STATUS.md](STATUS.md).
