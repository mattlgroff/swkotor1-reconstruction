"""Build the reconstruction from a reviewed relative-path source manifest."""
from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent
GAME_SHA256 = "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435"
OVERRIDES = {"CL", "_CL_", "LINK", "_LINK_", "CCC_OVERRIDE_OPTIONS"}
GENERATED_LINK_INPUTS = {"resources/swkotor-original.res", "imports/mss32.lib", "imports/binkw32.lib"}
REVIEWED_LINK_ALIASES = {
    "__kotor_m15a_yaw_004aa0f0": "_kotor_m15a_yaw_004aa0f0",
    "?restore_camera_00641dc0@kotor_input_required@@YIXPAX0@Z": "@kotor_fastcall_00641dc0@8",
    "?camera_restore_007a2288@kotor_input_cells_required@@3HC": "_recovered_camera_reset_cell_007a2288",
    "@kotor_fastcall_0062b1e0@16": "@load_screen_update_created_in_game_gui_0062b1e0@16",
    "__live_free": "_live_free",
    "__live_operator_new": "_live_operator_new",
    "??0StartupString@kotor_reconstruction@@QAE@PBD@Z": "??0StartupString@kotor_reconstruction@@QAE@RBD@Z",
}
REVIEWED_LIBRARIES = {"libcmt.lib", "libvcruntime.lib", "oldnames.lib", "libucrt.lib", "kernel32.lib",
                      "user32.lib", "gdi32.lib", "libcpmt.lib", "uuid.lib", "opengl32.lib", "glu32.lib",
                      "ole32.lib", "version.lib", "imm32.lib", "dinput8.lib", "dxguid.lib"}


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def reject_overrides(env: dict[str, str]) -> None:
    if any(key.upper() in OVERRIDES and value for key, value in env.items()):
        raise ValueError("Remove inherited compiler/linker option overrides before building.")


def inside(root: Path, relative: str) -> Path:
    path = Path(relative)
    if path.is_absolute() or path.drive or ".." in path.parts:
        raise ValueError(f"Manifest path must be relative: {relative}")
    result = (root / path).resolve()
    if not result.is_relative_to(root.resolve()):
        raise ValueError("Manifest path escapes its root.")
    return result


def setup_msvc(vs_root: Path) -> dict[str, str]:
    setup = vs_root / "VC/Auxiliary/Build/vcvarsall.bat"
    if not setup.is_file():
        raise ValueError("Visual Studio C++ Build Tools not found; pass --vs-root.")
    # The trusted installed setup script needs cmd; source/compiler commands do not.
    if any(c in str(setup) for c in '"%!&|<>^\r\n'):
        raise ValueError("Unsupported shell metacharacter in Visual Studio path.")
    reject_overrides(dict(os.environ))
    setup_env = {key: value for key, value in os.environ.items()
                 if key.upper() not in {"INCLUDE", "LIB", "LIBPATH"}}
    with tempfile.TemporaryDirectory(prefix="kotor-build-env-") as temporary:
        wrapper = Path(temporary) / "setup.cmd"
        wrapper.write_text(f'@echo off\ncall "{setup}" x64_x86 10.0.26100.0 -vcvars_ver=14.44 >nul\nif errorlevel 1 exit /b 91\nset\n', encoding="utf-8")
        result = subprocess.run(
            ["cmd.exe", "/d", "/c", str(wrapper)],
            env=setup_env, capture_output=True, text=True, check=True, timeout=60,
        )
    env = dict(os.environ)
    for line in result.stdout.splitlines():
        key, sep, value = line.partition("=")
        if sep and key:
            env[key] = value
    reject_overrides(env)
    if not env.get("VCToolsVersion", "").startswith("14.44."):
        raise ValueError("This manifest requires MSVC 14.44.")
    if env.get("WindowsSDKVersion", "").rstrip("\\/") != "10.0.26100.0":
        raise ValueError("This manifest requires Windows SDK 10.0.26100.0.")
    return env


def run(argv: list[str], env: dict[str, str], log: Path) -> None:
    result = subprocess.run(argv, cwd=ROOT, env=env, capture_output=True, text=True, timeout=300)
    log.write_text(result.stdout + result.stderr, encoding="utf-8")
    if result.returncode:
        raise RuntimeError(f"Command failed ({result.returncode}); see {log}")


RUNTIME_DIRECTORIES = ("Override", "Temp", "Logs", "LocalVault", "DMVault", "ServerVault",
                       "Tempclient", "saves", "currentgame", "gameinprogress")


def prepare_runtime(runtime: Path, game_dir: Path) -> None:
    """Create empty runtime directories without copying assets or launching the game."""
    runtime = runtime.resolve(strict=True)
    game_dir = game_dir.resolve(strict=True)
    if not runtime.is_dir() or not game_dir.is_dir():
        raise ValueError("Runtime and original installation must be existing directories.")
    for protected in (ROOT, game_dir):
        if runtime.is_relative_to(protected) or protected.is_relative_to(runtime):
            raise ValueError("Runtime must be outside and separate from source and original installation.")
    paths = [runtime / name for name in RUNTIME_DIRECTORIES]
    # Check every destination before making changes; never follow redirected directories.
    for path in paths:
        if path.is_symlink() or path.is_junction() or path.resolve().parent != runtime:
            raise ValueError(f"Runtime directory must not be a link or junction: {path.name}")
        if path.exists() and not path.is_dir():
            raise ValueError(f"Runtime directory name is occupied by a file: {path.name}")
    for path in paths:
        path.mkdir(exist_ok=True)
    print("Runtime directories ready; existing contents preserved. No game launched.")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", choices=["game"], default="game")
    parser.add_argument("--game-dir", type=Path)
    parser.add_argument("--out", type=Path, help="New directory for this build")
    parser.add_argument("--prepare-runtime", type=Path, help="Create required empty directories in an existing runtime copy; do not build or launch")
    parser.add_argument("--vs-root", type=Path, default=Path(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")) / "Microsoft Visual Studio/2022/BuildTools")
    parser.add_argument("--clang-cl", type=Path)
    parser.add_argument("--jobs", type=int, default=1, help="Concurrent compiler processes (1–16)")
    args = parser.parse_args()
    if args.prepare_runtime is not None:
        if args.game_dir is None or args.out is not None:
            raise ValueError("Use --prepare-runtime with --game-dir and without --out.")
        prepare_runtime(args.prepare_runtime, args.game_dir)
        return
    if args.out is None:
        raise ValueError("Building requires --out.")
    if not 1 <= args.jobs <= 16:
        raise ValueError("--jobs must be between 1 and 16.")
    manifest = json.loads((ROOT / "build-manifest.json").read_text(encoding="utf-8"))
    if args.target == "game" and not manifest.get("game_complete"):
        raise ValueError("Full game source manifest is incomplete; game build is unavailable.")
    if args.target == "game":
        if not args.game_dir:
            raise ValueError("Pass --game-dir for your separately purchased GOG installation.")
        if digest(args.game_dir / "swkotor.exe") != GAME_SHA256:
            raise ValueError("The original executable does not match the supported GOG release.")
    out = args.out.resolve()
    if out.is_relative_to(ROOT) or ROOT.is_relative_to(out):
        raise ValueError("Build output must be outside and separate from the source tree.")
    if args.game_dir and (out.is_relative_to(args.game_dir.resolve()) or args.game_dir.resolve().is_relative_to(out)):
        raise ValueError("Build output must be separate from the original installation.")
    if out.exists():
        raise ValueError("Choose a new output directory; existing outputs are not reused.")
    selected = manifest["targets"][args.target]
    pinned_paths = {entry["path"] for entry in selected["files"]}
    for entry in selected["files"]:
        path = inside(ROOT, entry["path"])
        if digest(path) != entry["sha256"]:
            raise ValueError(f"Source hash mismatch: {entry['path']}")
    env = setup_msvc(args.vs_root.resolve())
    tool_dir = Path(env["VCToolsInstallDir"]) / "bin/Hostx64/x86"
    compiler = tool_dir / "cl.exe"
    linker = tool_dir / "link.exe"
    if any(unit.get("compiler") == "clang-cl" for unit in selected["units"]):
        if not args.clang_cl or not args.clang_cl.is_file():
            raise ValueError("Pass --clang-cl for LLVM 23.1.2.")
        version = subprocess.run([str(args.clang_cl), "--version"], env=env,
                                 capture_output=True, text=True, check=True, timeout=30).stdout
        if not version.startswith("clang version 23.1.2 ") or "85ac560262434c9ccfc0c183ec22d4138ed647fb" not in version:
            raise ValueError("This manifest requires the tested LLVM 23.1.2 release.")
    out.mkdir(parents=True)
    (out / ".gitignore").write_text("*\n", encoding="ascii")
    receipt = {"target": args.target, "compiler_sha256": digest(compiler), "linker_sha256": digest(linker), "sources": selected["files"], "commands": []}
    generated = out / "kotor_generated"
    if selected.get("generation"):
        if args.target != "game" or not args.game_dir:
            raise ValueError("Original-data generation requires the game target and --game-dir.")
        generator_files = [ROOT / "generate_local_resources.py", *sorted((ROOT / "local_generators").glob("*"))]
        for path in generator_files:
            if path.is_file() and path.relative_to(ROOT).as_posix() not in pinned_paths:
                raise ValueError("Every generator and metadata file must be hash pinned.")
        rules_relative = selected["generation"]["rules_src"]
        rules = inside(ROOT, rules_relative)
        if not rules.is_dir():
            raise ValueError("Rules source directory is missing.")
        for path in rules.iterdir():
            if path.suffix in {".cpp", ".h"} and path.relative_to(ROOT).as_posix() not in pinned_paths:
                raise ValueError("Every rules generation input must be hash pinned.")
        command = [sys.executable, str(ROOT / "generate_local_resources.py"),
                   "--game-dir", str(args.game_dir.resolve()), "--out", str(generated),
                   "--rules-src", str(rules), "--lib-exe", str(tool_dir / "lib.exe")]
        run(command, env, out / "generation.log")
        print("Generated local resource inputs.", flush=True)
        receipt["commands"].append(command)
        receipt["generated_files"] = {p.relative_to(generated).as_posix(): digest(p)
                                      for p in generated.rglob("*") if p.is_file()}
    from local_math import build_math
    math_recipe = next(u['flags'] for u in selected['units'] if u['source'].endswith('claude_w5_movement_actions.cpp'))
    math_objects, math_receipt = build_math.build(ROOT, out, args.game_dir.resolve() / 'swkotor.exe',
        tool_dir, args.clang_cl, env, run, digest, pinned_paths, math_recipe)
    receipt['local_math'] = math_receipt
    objects = []
    def compile_unit(index_unit):
        index, unit = index_unit
        obj = out / f"{index:04d}.obj"
        source_kind = unit.get("source_kind", "source")
        if source_kind not in {"source", "generated"}:
            raise ValueError("Unknown source kind.")
        source = inside(generated if source_kind == "generated" else ROOT, unit["source"])
        if source_kind == "generated" and unit["source"] not in receipt.get("generated_files", {}):
            raise ValueError("Generated source was not produced by this build.")
        if source_kind == "source" and unit["source"] not in pinned_paths:
            raise ValueError("Unit source is not in the hash-pinned file list.")
        if unit.get("compiler", "msvc") not in {"msvc", "clang-cl"}:
            raise ValueError("Unknown compiler selector.")
        tool = compiler if unit.get("compiler", "msvc") == "msvc" else args.clang_cl
        if not tool or not tool.is_file():
            raise ValueError("Pass --clang-cl with the required installed LLVM compiler.")
        includes = ["/I" + str(inside(ROOT, p)) for p in unit.get("includes", [])]
        if selected.get("generation"):
            includes.append("/I" + str(out))
        aliases = unit.get("alias_headers", {})
        if aliases:
            alias_root = out / "includes" / f"{index:04d}"
            alias_root.mkdir(parents=True)
            for name, relative in aliases.items():
                if not re.fullmatch(r"[A-Za-z0-9_.-]+\.(?:h|hpp)", name) or relative not in pinned_paths:
                    raise ValueError("Invalid or unpinned alias header.")
                target = inside(ROOT, relative)
                if any(c in str(target) for c in '\r\n"'):
                    raise ValueError("Invalid alias target path.")
                (alias_root / name).write_text('#include "' + target.as_posix() + '"\n', encoding="utf-8")
            includes.append("/I" + str(alias_root))
        for relative in unit.get("generated_headers", []):
            if relative not in receipt.get("generated_files", {}):
                raise ValueError("Missing generated header.")
        flags = unit["flags"]
        fixed_flags = {"/nologo", "/O1", "/O2", "/Od", "/Ob0", "/Ob2", "/Oy", "/Oy-", "/GS", "/EHsc", "/EHs", "/EHs-c-", "/MT", "/W3", "/W4", "/WX", "/showIncludes", "/std:c++17", "/std:c++20", "/fp:strict", "/fp:precise", "/arch:IA32", "/arch:SSE2", "/Brepro", "/Zc:sizedDealloc-", "--target=i686-pc-windows-msvc", "--no-default-config", "/clang:-fms-compatibility-version=19.44", "/clang:-Wno-invalid offsetof".replace(" ", "-"), "/clang:-Wno-nontrivial-memcall", "/clang:-Wno-uninitialized-const-pointer", "/clang:-Wno-unused-function", "/clang:-ffp-contract=off", "/clang:-mfpmath=387", "/clang:-mno-sse", "/clang:-march=i686"}
        for flag in flags:
            if flag not in fixed_flags and not re.fullmatch(r"/D[A-Za-z_][A-Za-z0-9_]*(?:=[A-Za-z0-9_+-]+)?", flag):
                raise ValueError(f"Unreviewed compiler flag: {flag}")
        command = [str(tool), *flags, *includes, "/c", str(source), "/Fo" + str(obj), "/Fd" + str(out / f"{index:04d}.pdb")]
        run(command, env, out / f"{index:04d}.log")
        if not obj.is_file() or not obj.stat().st_size:
            raise RuntimeError("Compiler did not produce a fresh object.")
        if obj.read_bytes()[:2] != b"\x4c\x01":
            raise RuntimeError("Compiler output is not an x86 COFF object.")
        return obj, {"argv": command, "compiler_sha256": digest(tool)}
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        for obj, command_receipt in pool.map(compile_unit, enumerate(selected["units"])):
            objects.append(obj)
            receipt["commands"].append(command_receipt)
            if len(objects) % 100 == 0 or len(objects) == len(selected["units"]):
                print(f"Compiled {len(objects)}/{len(selected['units'])} units.", flush=True)
    executable = inside(out, selected["executable"])
    libraries = selected.get("libraries", [])
    if any(lib.lower() not in REVIEWED_LIBRARIES for lib in libraries):
        raise ValueError("Library names must resolve from the installed toolchain.")
    link_flags = selected.get("link_flags", [])
    if any(flag not in {"/SUBSYSTEM:CONSOLE", "/SUBSYSTEM:WINDOWS", "/OPT:NOREF", "/OPT:NOICF", "/DYNAMICBASE:NO", "/MANIFESTUAC:level='asInvoker'", "/MANIFEST:EMBED"} for flag in link_flags):
        raise ValueError("Unreviewed linker flag.")
    aliases = selected.get("link_aliases", {})
    if any(REVIEWED_LINK_ALIASES.get(symbol) != target for symbol, target in aliases.items()):
        raise ValueError("Unreviewed linker alias.")
    alias_flags = [f"/alternatename:{symbol}={target}" for symbol, target in aliases.items()]
    generated_inputs = []
    for relative in selected.get("generated_link_inputs", []):
        if relative not in GENERATED_LINK_INPUTS or relative not in receipt.get("generated_files", {}):
            raise ValueError("Unreviewed or missing generated link input.")
        generated_inputs.append(str(inside(generated, relative)))
    # A complete game has too many objects for the Windows command-line limit.
    executable.parent.mkdir(parents=True, exist_ok=True)
    link_arguments = ["/nologo", "/MACHINE:X86", "/INCREMENTAL:NO", "/OUT:" + str(executable),
                      "/MAP:" + str(out / "link.map"), *link_flags, *alias_flags,
                      *map(str, objects), *map(str, math_objects), *libraries, *generated_inputs]
    response = out / "link.rsp"
    if any(any(c in argument for c in '\r\n"') for argument in link_arguments):
        raise ValueError("Unsupported character in linker argument.")
    response.write_text("\n".join('"' + argument + '"' for argument in link_arguments) + "\n", encoding="utf-8")
    command = [str(linker), "@" + str(response)]
    run(command, env, out / "link.log")
    receipt["commands"].append(command)
    receipt['math_providers'] = build_math.validate_providers(ROOT, out)
    for relative, expected in math_receipt['files'].items():
        if digest(inside(out, relative)) != expected:
            raise ValueError('Local math input changed during build')
    receipt["executable_sha256"] = digest(executable)
    for entry in selected["files"]:
        if digest(inside(ROOT, entry["path"])) != entry["sha256"]:
            raise ValueError("Source changed during build.")
    for relative, expected in receipt.get("generated_files", {}).items():
        if digest(inside(generated, relative)) != expected:
            raise ValueError("Generated input changed during build.")
    if args.target == "game" and digest(args.game_dir / "swkotor.exe") != GAME_SHA256:
        raise ValueError("Original executable changed during build.")
    (out / "build-receipt.json").write_text(json.dumps(receipt, indent=2), encoding="utf-8")
    print(f"Built {executable}")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, RuntimeError, OSError, subprocess.SubprocessError) as exc:
        print(f"Build stopped: {exc}", file=sys.stderr)
        sys.exit(1)
