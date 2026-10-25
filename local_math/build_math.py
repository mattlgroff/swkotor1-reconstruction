"""Mandatory local math generation and provider validation; never downloads inputs."""
from pathlib import Path
import json
import sys

INPUTS = ('local_math/build_math.py', 'local_math/gen_l1_asm.py', 'local_math/l1_bytegate.py',
          'local_math/providers.json', 'src/math/kotor_math.h', 'src/math/kotor_x87.h',
          'src/math/kotor_math_x87.asm', 'src/math/kotor_vc7_math_init.cpp')


def build(root, out, game_exe, tools, clang, env, run, digest, pinned, recipe):
    for name in INPUTS:
        if name not in pinned or not (root / name).is_file():
            raise ValueError('Missing pinned mandatory math input: ' + name)
    if not clang or not (tools / 'ml.exe').is_file():
        raise ValueError('Mandatory math requires LLVM and MASM')
    local = out / 'local_math'
    commands = []
    def execute(argv, log):
        run(argv, env, out / log)
        commands.append(argv)
    execute([sys.executable, '-B', str(root/'local_math/gen_l1_asm.py'), str(game_exe), str(local)], 'math-generation.log')
    expected = ('kotor_vc7_math.asm', 'kotor_vc7_pow_tables.inc', 'l1_layout.json')
    if not all((local/n).is_file() and (local/n).stat().st_size for n in expected):
        raise ValueError('Incomplete generated math inputs')
    layout = json.loads((local/'l1_layout.json').read_text())
    if len(layout['instructions']) != 2806 or len(layout['runs']) != 18:
        raise ValueError('Unexpected math instruction coverage')
    objects = []
    for source in (local/'kotor_vc7_math.asm', root/'src/math/kotor_math_x87.asm'):
        obj = local/(source.stem+'.obj')
        execute([str(tools/'ml.exe'), '/nologo','/c','/coff','/safeseh','/W3','/WX',
                 '/I'+str(local), '/Fl'+str(local/(source.stem+'.lst')), '/Fo'+str(obj), str(source)], source.stem+'.log')
        objects.append(obj)
    obj = local/'kotor_vc7_math_init.obj'
    execute([str(clang), *recipe, '/c', str(root/'src/math/kotor_vc7_math_init.cpp'), '/Fo'+str(obj)], 'math-init.log')
    objects.append(obj)
    if any(not p.is_file() or p.read_bytes()[:2] != b'\x4c\x01' for p in objects):
        raise ValueError('Missing x86 math object')
    execute([sys.executable,'-B',str(root/'local_math/l1_bytegate.py'),str(game_exe),str(objects[0]),
             str(local/'kotor_vc7_math.lst'),str(local/'l1_layout.json')], 'math-bytegate.log')
    return objects, {'commands':commands, 'assembler_sha256':digest(tools/'ml.exe'),
                     'files':{p.relative_to(out).as_posix():digest(p) for p in local.iterdir() if p.is_file()}}


def validate_providers(root, out):
    expected = json.loads((root/'local_math/providers.json').read_text())
    rows = {}
    for line in (out/'link.map').read_text(errors='replace').splitlines():
        fields = line.split()
        if len(fields) >= 4 and ':' in fields[0]:
            rows.setdefault(fields[1], []).append(fields[-1])
    for symbol, owner in expected.items():
        hits = rows.get(symbol, [])
        if len(hits) != 1 or Path(hits[0]).name != owner:
            raise ValueError('Missing or incorrect mandatory math provider: ' + symbol)
    return expected
