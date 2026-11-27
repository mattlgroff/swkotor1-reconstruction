"""Merge the copies of one type into its shared header (CODE_RULES.md, R1).

    python matching/tools/mergetype.py Vector shared/vector.h
    python matching/tools/mergetype.py Vector shared/vector.h --dry
    python matching/tools/mergetype.py --compile            # compile every unit

The first form replaces every top-level definition of the type in
matching/include and matching/src with an include of the shared header
(matching/include/<header>), and gives forward declarations and elaborated
uses the header's keyword (`--keyword`, default class). Definitions nested in
another scope, or preceded by `typedef` or `template`, are listed and left.
`--record FILE` saves each replaced body, keyed by file, for diagnosing what
a unit relied on. Files listed with `--keep` are left alone.

`--compile` compiles every matching unit with MSVC 7.0 (KOTOR_MSVC7) and
prints the errors, so the merge can be finished file by file. Then run the
byte gate (matchcmp.py) and compare every function with the last report:
a merge must not lose a match.
"""
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MATCHING = ROOT / "matching"
sys.path.insert(0, str(Path(__file__).resolve().parent))
import rulecheck as rc


def first_branch_only(text):
    """Text with #else/#elif branches blanked, so braces are counted once."""
    out, stack = [], []
    for line in text.split("\n"):
        st = line.strip()
        if re.match(r"#\s*if", st):
            stack.append(False)
        elif re.match(r"#\s*(else|elif)", st) and stack:
            stack[-1] = True
        elif re.match(r"#\s*endif", st) and stack:
            stack.pop()
        out.append(" " * len(line) if any(stack) else line)
    return "\n".join(out)


def replace(name, header, keyword, keep, dry):
    inc = f'#include "{header}"'
    canon = (MATCHING / "include" / header).resolve()
    other = "struct" if keyword == "class" else "class"
    elaborated = re.compile(r"\b" + other + r"(\s+)" + re.escape(name) + r"\b(?!\s*[:{])")
    record, skipped, fixed = {}, [], 0
    files = sorted(list((MATCHING / "include").rglob("*.h")) + list((MATCHING / "src").rglob("*.cpp")))
    for p in files:
        rel = p.relative_to(ROOT).as_posix()
        if p.resolve() == canon or rel in keep:
            continue
        raw = open(p, newline="").read()
        t = rc.blank(raw)
        tb = first_branch_only(t)
        spans = []
        for m in rc.TYPE_DEF.finditer(t):
            if m.group(2) != name:
                continue
            depth = tb[:m.start()].count("{") - tb[:m.start()].count("}")
            close = rc.match_close(t, m.end() - 1, "{", "}")
            semi = re.match(r"\s*;", t[close + 1:])
            lead = re.search(r"(typedef\s+|template\s*<[^>]*>\s*)$", t[:m.start()])
            if depth or not semi or lead:
                skipped.append((rel, rc.line_of(t, m.start())))
                continue
            spans.append((m.start(), close + 1 + semi.end()))
        new = raw
        if spans:
            out, last = [], 0
            for i, (s, e) in enumerate(spans):
                out.append(raw[last:s])
                if i == 0:
                    out.append(("" if s == 0 or raw[:s].endswith("\n") else "\n") + inc)
                last = e
            out.append(raw[last:])
            new = "".join(out)
            record[rel] = [raw[s:e] for s, e in spans]
        new = elaborated.sub(keyword + r"\1" + name, new)
        if new != raw:
            fixed += not spans
            if not dry:
                open(p, "w", newline="\n").write(new)
    print(f"{name}: replaced in {len(record)} files; keyword fixed in {fixed} more; "
          f"{len(skipped)} left")
    for s in skipped:
        print("  left", *s)
    return record


def compile_all(jobs):
    msvc = Path(os.environ["KOTOR_MSVC7"])
    env = dict(os.environ, PATH=str(msvc / "BIN") + os.pathsep + os.environ["PATH"],
               INCLUDE=str(msvc / "INCLUDE"))
    out = Path(tempfile.mkdtemp())
    files = sorted((MATCHING / "src").rglob("*.cpp"))

    def one(item):
        i, p = item
        run = subprocess.run([str(msvc / "BIN" / "CL.EXE"), "/nologo", "/c", "-O2", "-Gy", "-GX",
                              "/I", str(MATCHING / "include"), f"/Fo{out / f'{i}.obj'}", str(p)],
                             capture_output=True, text=True, env=env)
        return p, run.returncode, run.stdout + run.stderr

    bad = 0
    with ThreadPoolExecutor(jobs) as ex:
        for p, rcode, text in ex.map(one, enumerate(files)):
            if rcode:
                bad += 1
                print(f"FAIL {p.relative_to(ROOT).as_posix()}")
                for line in text.splitlines():
                    if " error " in line:
                        print("   ", re.sub(r"^.*?matching[\/]", "", line))
    print(f"{len(files) - bad}/{len(files)} compiled")
    return bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("type", nargs="?")
    ap.add_argument("header", nargs="?")
    ap.add_argument("--keyword", default="class", choices=["class", "struct"])
    ap.add_argument("--keep", action="append", default=[], help="repo-relative file to leave alone")
    ap.add_argument("--record", help="write the replaced bodies here (JSON)")
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--compile", action="store_true")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()
    if args.compile:
        return 1 if compile_all(args.jobs) else 0
    if not args.type or not args.header:
        ap.error("give TYPE HEADER, or --compile")
    record = replace(args.type, args.header, args.keyword, set(args.keep), args.dry)
    if args.record:
        json.dump(record, open(args.record, "w"), indent=1)


if __name__ == "__main__":
    sys.exit(main())
