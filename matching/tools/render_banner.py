#!/usr/bin/env python3
"""Render matching/assets/progress.svg, the project's progress tracker.

The tracker shows the two decompilations of swkotor.exe side by side, so
they are never confused:

- Modern-toolchain decompilation (src/, VS2022 + clang-cl): behaviour-equivalent
  C++ built with modern Visual Studio. Counts functions src/ defines under
  their original address (kotor_<cc>_<address>), functions it cites (folded
  into a caller or shared), and C runtime functions that VS2022's library
  supplies. Milestones restate STATUS.md; edit PLAYABLE when it changes.
- Original-toolchain, byte-identical decompilation (matching/, MSVC 7.0): source that compiles
  to the original's exact bytes, from matching/status.csv (matchcmp.py
  --report). Functions identical to the stock LIBCMT.LIB come from
  matching/crt_libcmt.csv.

Both are measured against matching/functions.csv, the exe's function starts
("kind" eh marks compiler-generated EH funclets, which are excluded), plus
any annotated address it misses. Sizes run from each start to the next, so
byte figures include alignment padding. The change badge compares
matching/status.csv with its previous committed revision. CI reruns this
whenever src/ or the matching inputs change.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import json
import re
import subprocess
from html import escape
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / "matching" / "assets" / "progress.svg"
LIBRARY = ROOT / "matching" / "crt_libcmt.csv"   # stock LIBCMT.LIB functions
CRT_BANDS = ((0x006FA390, 0x0070C0A0), (0x00713190, 0x00713E80))   # statically linked C runtime

TEXT_START, TEXT_END = 0x00401000, 0x0073D000   # .text of the 1.0.3.0 exe
CELL_BYTES = 0x1000
MAP_COLUMNS = 46
SOURCE_SUFFIXES = {".cpp", ".h", ".hpp", ".inc"}
DEFINITION = re.compile(
    r"\b[A-Za-z_]\w*?_(00[4-7][0-9a-fA-F]{5})\s*\(([^;{}()]|\([^()]*\))*\)\s*"
    r"(?:const\s*)?(?:noexcept\s*)?(?:->\s*[\w:<>*& ]+)?\{")

# (milestone, state) from STATUS.md; state is done, open or todo.
PLAYABLE = [
    ("Builds", "done"),
    ("Endar Spire", "done"),
    ("Taris", "open"),
    ("Minigames", "todo"),
    ("Rest", "todo"),
]

BG, BORDER, PANEL = "#090909", "#343434", "#111214"
TEXT, MUTED, DIM = "#f4f4f5", "#c9c9c9", "#8a8a8a"
PLAY_COLOR, MATCH_COLOR = "#6ba8ff", "#5be3a8"
LIB_COLOR, NEAR_COLOR = "#9aa5b1", "#c9963a"
TODO_COLOR, CG_COLOR, PENDING = "#272727", "#4a3f6b", "#3a3a3a"
STATE_COLOR = {"done": None, "open": NEAR_COLOR, "todo": PENDING}
FONT = "-apple-system, BlinkMacSystemFont, Segoe UI, Arial, sans-serif"


def read_status(text: str) -> dict[int, str]:
    return {int(r["address"], 16): r["status"] for r in csv.DictReader(text.splitlines())}


def previous_status(report: Path, current: dict[int, str]) -> dict[int, str] | None:
    """The most recent committed revision of the report that differs from now."""
    try:
        relative = report.resolve().relative_to(ROOT).as_posix()
        history = subprocess.run(["git", "log", "-n", "3", "--format=%H", "--", relative],
                                 cwd=ROOT, capture_output=True, text=True, check=True).stdout.split()
        for rev in history:
            text = subprocess.run(["git", "show", f"{rev}:{relative}"], cwd=ROOT,
                                  capture_output=True, text=True, check=True).stdout
            old = read_status(text)
            if old != current:
                return old
    except (OSError, ValueError, KeyError, subprocess.CalledProcessError):
        pass
    return None


CITED = re.compile(r"(?<![0-9A-Fa-fx])(00[4-7][0-9A-Fa-f]{5})(?![0-9A-Fa-f])")


def playable_coverage() -> dict[int, int]:
    """Original address -> 2 if src/ defines a function under that address,
    1 if src/ only cites it (inlined, folded into a caller, or shared with an
    identical function)."""
    coverage: dict[int, int] = {}
    for path in (ROOT / "src").rglob("*"):
        if path.suffix in SOURCE_SUFFIXES:
            text = path.read_text(encoding="utf-8", errors="replace")
            for m in CITED.finditer(text):
                coverage.setdefault(int(m.group(1), 16), 1)
            for m in DEFINITION.finditer(text):
                coverage[int(m.group(1), 16)] = 2
    return coverage


def load_functions(status: dict[int, str], table: Path, playable: dict[int, int]):
    """[(start, size, category, playable coverage)].

    Playable coverage: 2 own definition in src/, 1 cited in src/ (folded into
    a caller or shared), 3 C runtime that VS2022's library supplies, 0 none.

    Category: M byte-identical, L identical to the stock VS.NET 2002
    LIBCMT.LIB, N matching attempt, T no matched source, CG EH funclet.
    """
    library = set()
    if LIBRARY.exists():
        with LIBRARY.open(encoding="utf-8") as source:
            library = {int(r["address"], 16) for r in csv.DictReader(source)}
    with table.open(encoding="utf-8") as source:
        kinds = {int(r["address"], 16): r.get("kind", "") for r in csv.DictReader(source)}
    for address in status:
        kinds.setdefault(address, "game")
    starts = sorted(kinds)
    functions = []
    for index, start in enumerate(starts):
        end = starts[index + 1] if index + 1 < len(starts) else TEXT_END
        state = status.get(start)
        category = ("M" if state == "MATCH" else "CG" if kinds[start] == "eh"
                    else "L" if start in library else "N" if state else "T")
        level = playable.get(start, 0)
        if not level and any(low <= start < high for low, high in CRT_BANDS):
            level = 3
        functions.append((start, max(0, min(end, TEXT_END) - start), category, level))
    return functions


def mix(color_a: str, color_b: str, t: float) -> str:
    a = [int(color_a[i:i + 2], 16) for i in (1, 3, 5)]
    b = [int(color_b[i:i + 2], 16) for i in (1, 3, 5)]
    return "#" + "".join(f"{round(x + (y - x) * t):02x}" for x, y in zip(a, b))


def map_class(category: str, playable: bool) -> str:
    """Map key: byte-identical, stock library, modern toolchain only, none, EH."""
    if category in ("M", "L", "CG"):
        return category
    return "P" if playable else "T"


def cell_color(shares: dict[str, int]) -> str:
    game = sum(v for k, v in shares.items() if k != "CG")
    if game == 0:
        return CG_COLOR if shares.get("CG") else "#161616"
    top = max(("M", "L", "P"), key=lambda k: shares.get(k, 0))
    share = shares.get(top, 0) / game
    if share == 0:
        return TODO_COLOR
    base = {"M": MATCH_COLOR, "L": LIB_COLOR, "P": PLAY_COLOR}[top]
    return mix(TODO_COLOR, base, 0.35 + 0.65 * share)


def bar(add, x0, y0, width, height, shares) -> None:
    add(f'<rect x="{x0}" y="{y0}" width="{width}" height="{height}" rx="{height / 2}" fill="{TODO_COLOR}"/>')
    x = x0
    for fraction, color in shares:
        w = width * fraction
        if w > 0:
            add(f'<rect x="{x:.2f}" y="{y0}" width="{max(w, 1.5):.2f}" height="{height}" fill="{color}"/>')
        x += w
    add(f'<rect x="{x0}" y="{y0}" width="{width}" height="{height}" rx="{height / 2}" '
        f'fill="none" stroke="{PANEL}" stroke-width="2"/>')


def text(add, x, y, value, size=12, fill=MUTED, weight=None, anchor=None) -> None:
    extra = (f' font-weight="{weight}"' if weight else "") + (f' text-anchor="{anchor}"' if anchor else "")
    add(f'<text x="{x}" y="{y}" font-size="{size}" fill="{fill}"{extra}>{escape(value)}</text>')


def steps(add, x0, y0, width, items, color) -> None:
    """A row of milestone dots: [(label, state)] with state done/open/todo."""
    pitch = width / len(items)
    for i, (label, state) in enumerate(items):
        x = x0 + i * pitch
        fill = color if state == "done" else STATE_COLOR[state]
        add(f'<circle cx="{x + 6:.1f}" cy="{y0}" r="6" fill="{fill}"/>')
        mark = "✓" if state == "done" else "…" if state == "open" else ""
        text(add, f"{x + 6:.1f}", y0 + 3.5, mark, 8, BG, 700, "middle")
        text(add, f"{x + 16:.1f}", y0 + 4, label, 10.5, TEXT if state != "todo" else DIM)


def panel(add, x0, y0, w, h, color, kicker, author, title, toolchain, claim) -> None:
    add(f'<rect x="{x0}" y="{y0}" width="{w}" height="{h}" rx="10" fill="{PANEL}" stroke="{BORDER}"/>')
    add(f'<rect x="{x0}" y="{y0}" width="{w}" height="4" rx="2" fill="{color}"/>')
    text(add, x0 + 16, y0 + 26, kicker, 11, color, 700)
    text(add, x0 + w - 16, y0 + 26, author, 11, MUTED, 600, "end")
    text(add, x0 + 16, y0 + 46, title, 15, TEXT, 650)
    text(add, x0 + 16, y0 + 64, toolchain, 11.5, DIM)
    text(add, x0 + 16, y0 + 82, claim, 12, MUTED)


def render(functions, units: int, complete: bool, old_status: dict[int, str] | None) -> str:
    game = [f for f in functions if f[2] != "CG"]
    total = len(game)
    game_bytes = sum(f[1] for f in game) or 1
    eh = len(functions) - total
    count = lambda pred: sum(1 for f in game if pred(f))
    size = lambda pred: sum(f[1] for f in game if pred(f))
    defined = count(lambda f: f[3] == 2)
    folded = count(lambda f: f[3] == 1)
    runtime = count(lambda f: f[3] == 3)
    defined_bytes, folded_bytes = size(lambda f: f[3] == 2), size(lambda f: f[3] == 1)
    runtime_bytes = size(lambda f: f[3] == 3)
    matched, library = count(lambda f: f[2] == "M"), count(lambda f: f[2] == "L")
    near = count(lambda f: f[2] == "N")
    matched_bytes, library_bytes = size(lambda f: f[2] == "M"), size(lambda f: f[2] == "L")
    near_bytes = size(lambda f: f[2] == "N")
    either = count(lambda f: f[3] or f[2] in ("M", "L"))
    played = defined + folded + runtime

    parts: list[str] = []
    add = parts.append

    # Header.
    text(add, 23, 34, "Star Wars: Knights of the Old Republic — two decompilations of "
         "swkotor.exe 1.0.3.0 (GOG)", 15, "#dedede", 600)
    text(add, 23, 54, "Same executable, two toolchains: modern Visual Studio and its libraries, or the "
         "original compiler and libraries (byte-identical). Unofficial.", 12, DIM)
    badge, badge_fg, badge_bg = "Tracking", "#a1a1aa", "#222225"
    if old_status is not None:
        diff = matched - sum(1 for v in old_status.values() if v == "MATCH")
        if diff:
            badge = f"{diff:+,} byte-identical"
            badge_fg, badge_bg = ("#33db73", "#062514") if diff > 0 else ("#fca5a5", "#371517")
        else:
            badge = "No change"
    badge_w = max(112, 30 + len(badge) * 7.6)
    add(f'<rect x="{938 - badge_w:.1f}" y="17" width="{badge_w:.1f}" height="27" rx="14" fill="{badge_bg}"/>')
    text(add, f"{938 - badge_w / 2:.1f}", 35, badge, 12.5, badge_fg, 600, "middle")

    # Two panels.
    top, w, h = 72, 450, 236
    left, right = 23, 489
    panel(add, left, top, w, h, PLAY_COLOR, "① MODERN TOOLCHAIN", "by mattlgroff",
          "Decompiled with Visual Studio 2022",
          f"src/ · VS2022 MSVC + clang-cl · C++17 · {units:,} units",
          "Modern compiler and libraries: same behaviour, different bytes.")
    panel(add, right, top, w, h, MATCH_COLOR, "② ORIGINAL TOOLCHAIN · BYTE-IDENTICAL", "by RayesDiyab",
          "Decompiled with Visual Studio .NET 2002",
          "matching/ · MSVC 7.0 (cl 13.00.9466) + original LIBCMT.LIB",
          "The compiler and libraries BioWare used: identical bytes.")

    y = top + 132
    text(add, left + 16, y, f"{played / total * 100:.1f}%", 34, PLAY_COLOR, 700)
    text(add, left + 150, y - 16, "of functions decompiled" + (" · builds the full game" if complete else ""),
         12.5, MUTED)
    text(add, left + 150, y, f"{played:,} / {total:,} · {total - played:,} not there yet", 11.5, DIM)
    text(add, right + 16, y, f"{matched / total * 100:.1f}%", 34, MATCH_COLOR, 700)
    text(add, right + 150, y - 16, "of functions decompiled byte-identical", 12.5, MUTED)
    text(add, right + 150, y, f"{matched:,} / {total:,} · {matched_bytes / game_bytes * 100:.1f}% of code "
         f"bytes", 11.5, DIM)

    y += 16
    bar(add, left + 16, y, w - 32, 12, [(defined_bytes / game_bytes, PLAY_COLOR),
                                        (folded_bytes / game_bytes, mix(PANEL, PLAY_COLOR, 0.65)),
                                        (runtime_bytes / game_bytes, mix(PANEL, PLAY_COLOR, 0.4))])
    bar(add, right + 16, y, w - 32, 12, [(matched_bytes / game_bytes, MATCH_COLOR),
                                         (library_bytes / game_bytes, LIB_COLOR),
                                         (near_bytes / game_bytes, NEAR_COLOR)])
    text(add, left + 16, y + 30, f"{defined:,} own definitions · {folded:,} folded into callers · "
         f"{runtime:,} runtime (VS2022 library)", 10.5, DIM)
    text(add, right + 16, y + 30, f"+{library:,} identical to stock LIBCMT.LIB (no source needed) · "
         f"{near:,} attempts", 10.5, DIM)

    steps(add, left + 16, top + h - 20, w - 32, PLAYABLE, PLAY_COLOR)
    steps(add, right + 16, top + h - 20, w - 32, [
        ("Toolchain", "done"),
        ("Functions", "open"),
        ("LIBCMT", "done"),
        ("Data, link", "todo"),
        ("Whole .exe", "todo"),
    ], MATCH_COLOR)

    # Address map, both builds in one picture.
    map_top = top + h + 40
    text(add, 23, map_top - 10, "Address map — 4 KiB per cell", 13, "#dedede", 550)
    key = [(MATCH_COLOR, "byte-identical"), (LIB_COLOR, "stock LIBCMT"),
           (PLAY_COLOR, "modern toolchain"), (CG_COLOR, "compiler-generated"),
           (TODO_COLOR, "neither yet")]
    kx = 939
    for color, label in reversed(key):
        kx -= len(label) * 5.9 + 28
        add(f'<rect x="{kx:.1f}" y="{map_top - 19}" width="10" height="10" rx="2" fill="{color}"/>')
        text(add, f"{kx + 14:.1f}", map_top - 10, label, 11, DIM)
    cells = (TEXT_END - TEXT_START) // CELL_BYTES
    rows = (cells + MAP_COLUMNS - 1) // MAP_COLUMNS
    x_map, pitch_x, pitch_y, size_y = 86, 18.6, 12, 10
    starts = [f[0] for f in functions]
    for index in range(cells):
        row, column = divmod(index, MAP_COLUMNS)
        low = TEXT_START + index * CELL_BYTES
        if column == 0:
            add(f'<text x="{x_map - 8}" y="{map_top + row * pitch_y + 9}" font-size="10" '
                f'text-anchor="end" fill="{DIM}" font-family="Consolas, Menlo, monospace">'
                f'0x{low:08X}</text>')
        shares: dict[str, int] = {}
        first = max(0, bisect.bisect_right(starts, low) - 1)
        for start, length, category, playable in functions[first:bisect.bisect_left(starts, low + CELL_BYTES)]:
            overlap = min(start + length, low + CELL_BYTES) - max(start, low)
            if overlap > 0:
                k = map_class(category, playable)
                shares[k] = shares.get(k, 0) + overlap
        add(f'<rect x="{x_map + column * pitch_x:.1f}" y="{map_top + row * pitch_y}" '
            f'width="{pitch_x - 2:.1f}" height="{size_y}" rx="2" fill="{cell_color(shares)}"/>')
    foot = map_top + rows * pitch_y + 22
    text(add, 23, foot, f"Together the two cover {either / total * 100:.1f}% of the {total:,} functions. "
         f"Byte-identical code replaces modern-toolchain code function by function (STRUCTURE.md). "
         f"{eh:,} EH funclets excluded.", 11.5, DIM)
    height = foot + 18

    desc = (f"Decompiled with Visual Studio 2022: "
            f"{played:,} of {total:,} functions ({played / total * 100:.1f}%: {defined:,} with their own "
            f"definition, {folded:,} folded into others, {runtime:,} C runtime from the VS2022 library)"
            f"{', builds the full game' if complete else ''}. "
            f"Decompiled with the original Visual Studio .NET 2002 (MSVC 7.0), byte-identical: "
            f"{matched:,} functions "
            f"({matched / total * 100:.1f}%) compile to the original bytes, plus {library:,} "
            f"identical to the stock LIBCMT.LIB.")
    body = "\n    ".join(parts)
    return f'''<svg xmlns="http://www.w3.org/2000/svg" width="960" height="{height}" viewBox="0 0 960 {height}" role="img" aria-labelledby="card-title card-desc">
  <title id="card-title">KOTOR decompilation progress</title>
  <desc id="card-desc">{escape(desc)}</desc>
  <rect x="1" y="1" width="958" height="{height - 2}" rx="14" fill="{BG}" stroke="{BORDER}" stroke-width="1.5"/>
  <g font-family="{FONT}">
    {body}
  </g>
</svg>
'''


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--report", type=Path, default=ROOT / "matching" / "status.csv",
                        help="matchcmp.py --report CSV")
    parser.add_argument("--function-table", type=Path, default=ROOT / "matching" / "functions.csv",
                        help="CSV of function starts (address, kind)")
    parser.add_argument("--out", type=Path, default=OUTPUT)
    args = parser.parse_args()
    manifest = json.loads((ROOT / "build-manifest.json").read_text(encoding="utf-8"))
    units = len(manifest["targets"]["game"]["units"])
    status = read_status(args.report.read_text(encoding="utf-8"))
    complete = bool(manifest.get("game_complete"))
    functions = load_functions(status, args.function_table, playable_coverage())
    args.out.parent.mkdir(parents=True, exist_ok=True)
    content = render(functions, units, complete, previous_status(args.report, status))
    if not args.out.exists() or args.out.read_bytes() != content.encode("utf-8"):
        args.out.write_bytes(content.encode("utf-8"))  # LF on every OS
    defined = sum(1 for f in functions if f[3] == 2 and f[2] != "CG")
    matched = sum(1 for f in functions if f[2] == "M")
    covered = sum(1 for f in functions if f[3] and f[2] != "CG")
    total = sum(1 for f in functions if f[2] != "CG")
    print(f"Tracker: modern toolchain {covered:,}/{total:,} ({defined:,} own definitions), "
          f"{matched:,} byte-identical -> {args.out}")


if __name__ == "__main__":
    main()
