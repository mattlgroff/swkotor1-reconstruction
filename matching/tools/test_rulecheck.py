"""Rule checker regressions on synthetic source trees only."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rulecheck as rc


TEMPORARY = []


def tearDownModule():
    for tmp in TEMPORARY:
        tmp.cleanup()


def tree(files):
    """A Tree over {relative path: text}, written to a temporary folder."""
    tmp = tempfile.TemporaryDirectory()
    TEMPORARY.append(tmp)
    root = Path(tmp.name)
    for sub in ("include", "src"):
        (root / sub).mkdir()
    for rel, text in files.items():
        path = root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
    return rc.Tree(root)


def checked(t, rel):
    """{address: Function} for a unit, after the whole-tree check."""
    unit = t.units[(t.root / rel)]
    out = {}
    for fn in rc.find_functions(unit):
        t.check(fn)
        out[fn.address] = fn
    return out


def rules(fn):
    return {r for r, _ in fn.blocking}


GOOD = """\
// FUNCTION: SWKOTOR 0x00401000
int Twice(int n)
{
    return n * 2;
}
"""


class Declarations(unittest.TestCase):
    def test_differing_copies_block_r1(self):
        t = tree({
            "src/a/one.cpp": "class CFoo { public: int m_nCount; };\n"
                             "// FUNCTION: SWKOTOR 0x00401000\n"
                             "int Get(CFoo* p) { return p->m_nCount; }\n",
            "src/b/two.cpp": "class CFoo { public: int m_nCount; float m_fScale; };\n"
                             "// FUNCTION: SWKOTOR 0x00402000\n"
                             "float Scale(CFoo* p) { return p->m_fScale; }\n",
        })
        self.assertIn("R1", rules(checked(t, "src/a/one.cpp")[0x401000]))
        self.assertIn("R1", rules(checked(t, "src/b/two.cpp")[0x402000]))

    def test_class_struct_mismatch_blocks_r1(self):
        t = tree({
            "include/foo.h": "struct CFoo { int m_nCount; };\n",
            "src/a/one.cpp": "class CFoo;\n"
                             "// FUNCTION: SWKOTOR 0x00401000\n"
                             "int Get(CFoo* p) { return 0; }\n",
        })
        self.assertIn("R1", rules(checked(t, "src/a/one.cpp")[0x401000]))

    def test_shared_header_and_unit_private_type_pass(self):
        t = tree({
            "include/foo.h": "#ifndef FOO_H\n#define FOO_H\n"
                             "class CFoo { public: int m_nCount; };\n#endif\n",
            "src/a/one.cpp": '#include "foo.h"\n'
                             "struct Entry { int key; };\n"
                             "// FUNCTION: SWKOTOR 0x00401000\n"
                             "int Get(CFoo* p, Entry* e) { return p->m_nCount + e->key; }\n",
            "src/b/two.cpp": '#include "foo.h"\n'
                             "// FUNCTION: SWKOTOR 0x00402000\n"
                             "int Count(CFoo* p) { return p->m_nCount; }\n",
        })
        self.assertEqual(rules(checked(t, "src/a/one.cpp")[0x401000]), set())
        self.assertEqual(rules(checked(t, "src/b/two.cpp")[0x402000]), set())

    def test_conflict_only_blocks_functions_that_use_the_type(self):
        t = tree({
            "src/a/one.cpp": "class CFoo { int a; };\n" + GOOD,
            "src/b/two.cpp": "class CFoo { int b; };\n",
        })
        self.assertEqual(rules(checked(t, "src/a/one.cpp")[0x401000]), set())

    def test_extern_in_unit_blocks_r2(self):
        t = tree({
            "include/app.h": "class CApp;\nextern CApp* g_pApp;\n",
            "src/a/one.cpp": "extern int g_nFrame;\n"
                             "// FUNCTION: SWKOTOR 0x00401000\n"
                             "int Frame() { return g_nFrame; }\n",
            "src/b/two.cpp": '#include "app.h"\n'
                             "// FUNCTION: SWKOTOR 0x00402000\n"
                             "CApp* App() { return g_pApp; }\n",
        })
        self.assertIn("R2", rules(checked(t, "src/a/one.cpp")[0x401000]))
        self.assertNotIn("R2", rules(checked(t, "src/b/two.cpp")[0x402000]))

    def test_global_with_two_types_blocks_r2(self):
        t = tree({
            "include/a.h": "extern int g_nFrame;\n",
            "include/b.h": "extern long g_nFrame;\n",
            "src/a/one.cpp": '#include "a.h"\n' + GOOD,
        })
        self.assertTrue(t.extern_conflicts == {"g_nFrame"})

    def test_padding_member_and_virtual(self):
        t = tree({
            "include/foo.h": "class CFoo { public: virtual void Pad1(); "
                             "int m_nCount; char pad[4]; };\n"
                             "class CBar { public: int m_nCount; char pad_0x4[0x10]; };\n",
            "src/a/one.cpp": '#include "foo.h"\n'
                             "// FUNCTION: SWKOTOR 0x00401000\n"
                             "int Get(CFoo* p) { return p->m_nCount; }\n"
                             "// FUNCTION: SWKOTOR 0x00402000\n"
                             "int Count(CBar* p) { return p->m_nCount; }\n",
        })
        fns = checked(t, "src/a/one.cpp")
        self.assertEqual(rules(fns[0x401000]), {"R13", "R17"})
        self.assertEqual(rules(fns[0x402000]), set())


def body(code, prefix=""):
    t = tree({"src/a/one.cpp": prefix + "// FUNCTION: SWKOTOR 0x00401000\n" + code})
    return checked(t, "src/a/one.cpp")[0x401000]


class Bodies(unittest.TestCase):
    def test_offset_casts_block_r4(self):
        self.assertIn("R4", rules(body("int f(void* p) { return *(int*)((char*)p + 0x10); }\n")))
        self.assertIn("R4", rules(body("int S::f() { return *(int*)((char*)this + 8); }\n",
                                       prefix="struct S { int f(); };\n")))

    def test_parser_byte_arithmetic_passes(self):
        fn = body("int Read(char* pData, int n) { return Parse((char*)pData + 1, n); }\n")
        self.assertNotIn("R4", rules(fn))

    def test_fake_code_blocks_r17(self):
        cases = [
            "void f() { volatile int n = 0; }\n",
            "void f(int a) { if (a) { } g(); }\n",
            "void f(int a) { if (a) g(); else { } }\n",
            "void f(int a) { a = a; }\n",
            "int f(int a) { return a + 0; }\n",
            "int f(int a) { return a * 1; }\n",
            "void f() { do { g(); } while (0); }\n",
            "void f() { g();; }\n",
            "void f() {\n    g(); // probe\n}\n",
        ]
        for code in cases:
            with self.subTest(code=code):
                self.assertIn("R17", rules(body(code)))

    def test_human_idioms_pass(self):
        cases = [
            "void f(char* d, const char* s) { while ((*d++ = *s++) != 0) ; }\n",
            "int f(int n) { int i; for (i = 0; i < n && g(i); i++)\n        ;\n    return i; }\n",
            "int f(int n) { switch (n) { case 1: ; } return n * 10 + 0x10; }\n",
            "void f() { int a[] = {1, 2}; g(a); }\n",
            "void f() {\n    volatile int n = 0; // volatile: shared with the timer IRQ\n}\n",
            "void f(float x) { g(x * 1.5f, x + 0.5f); }\n",
            "void f(int a) { if (a) { g(); } else if (a > 1) { h(); } }\n",
        ]
        for code in cases:
            with self.subTest(code=code):
                self.assertEqual(rules(body(code)), set())

    def test_macro_do_while_passes(self):
        fn = body("void f() { STEP(); }\n",
                  prefix="#define STEP() \\\n    do { g(); } while (0)\n")
        self.assertEqual(rules(fn), set())

    def test_assembly(self):
        fn = body("__declspec(naked) int f() { __asm { ret } }\n")
        self.assertIn("R18", rules(fn))
        fn = body("__declspec(naked) int f() { __asm { ret } }\n", prefix="// ORIGINAL-ASM: MASM object\n")
        self.assertTrue(fn.original_asm)


class Annotations(unittest.TestCase):
    def test_stacked_annotations_have_no_body(self):
        t = tree({"src/a/one.cpp":
                  "class CFoo { public: virtual ~CFoo(); };\n"
                  "// FUNCTION: SWKOTOR 0x00401000 SYMBOL: ??_GCFoo@@UAEPAXI@Z\n"
                  "// FUNCTION: SWKOTOR 0x00401010 SYMBOL: ??_ECFoo@@UAEPAXI@Z\n"
                  "\n"
                  "// FUNCTION: SWKOTOR 0x00401020\n"
                  "CFoo::~CFoo()\n{\n    volatile int n;\n}\n"})
        fns = checked(t, "src/a/one.cpp")
        self.assertFalse(fns[0x401000].has_body)
        self.assertFalse(fns[0x401010].has_body)
        self.assertEqual(rules(fns[0x401000]), set())
        self.assertIn("R17", rules(fns[0x401020]))

    def test_explicit_instantiation_has_no_body(self):
        t = tree({"src/a/one.cpp":
                  "template <class T> struct L { void Clear(); };\n"
                  "// FUNCTION: SWKOTOR 0x00401000 SYMBOL: ?Clear@?$L@H@@QAEXXZ\n"
                  "template void L<int>::Clear();\n"})
        fn = checked(t, "src/a/one.cpp")[0x401000]
        self.assertFalse(fn.has_body)

    def test_header_comment_advisory(self):
        self.assertIn("R33", {r for r, _ in body(GOOD.split("\n", 1)[1]).advisory})
        fn = body(GOOD.split("\n", 1)[1], prefix="// Doubles n.\n")
        self.assertNotIn("R33", {r for r, _ in fn.advisory})


if __name__ == "__main__":
    unittest.main()
