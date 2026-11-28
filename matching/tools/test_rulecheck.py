"""Rule-checker regressions on synthetic sources only."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rulecheck as rc


def findings(body, head="void C::F()"):
    """Rules found in one function body given as source text."""
    return sorted({rule for rule, _ in rc.body_findings(rc.blank(body), head, body)})


class BodyRules(unittest.TestCase):
    def test_clean_body_passes(self):
        self.assertEqual(findings("{\n    int n = m_nCount;\n    Use(n);\n}"), [])

    def test_offset_cast_is_r4(self):
        self.assertEqual(findings("{\n    return *(int*)((char*)pObject + 0x1c);\n}"), ["R4"])
        self.assertEqual(findings("{\n    m_n = *(int*)((char*)this + 8);\n}"), ["R4"])

    def test_buffer_arithmetic_is_not_r4(self):
        # parsers walk raw buffers; no typed member is involved
        self.assertEqual(findings("{\n    m_pData = (char*)m_pResource + 8;\n}"), [])

    def test_absolute_address_is_r2(self):
        self.assertEqual(findings("{\n    return *(int*)0x007A2370;\n}"), ["R2"])
        self.assertEqual(findings("{\n    return n & 0x007A2370;\n}"), [])

    def test_volatile_local_is_r17(self):
        self.assertEqual(findings("{\n    volatile int n = m_n;\n    return n;\n}"), ["R17"])

    def test_asm_body_is_r18_but_helper_call_is_not(self):
        self.assertEqual(findings("{\n    __asm { mov eax, 1 }\n}"), ["R18"])
        self.assertEqual(findings("{\n    int n;\n    FloatToInt(&n, f);\n    return n;\n}"), [])

    def test_empty_if_needs_a_label(self):
        self.assertEqual(findings("{\n    if (a) {\n    } else {\n        b();\n    }\n}"), ["R17"])
        labelled = "{\n    if (a) {\n        // NOTE: the original tests a and does nothing\n    } else {\n        b();\n    }\n}"
        self.assertEqual(findings(labelled), [])

    def test_unused_local_needs_a_label(self):
        self.assertEqual(findings("{\n    int nSkill = GetInteger(0);\n    return 1;\n}"), ["R17"])
        self.assertEqual(findings("{\n    int nSkill = GetInteger(0);  // NOTE: unused in the original\n    return 1;\n}"), [])

    def test_fake_statements(self):
        self.assertEqual(findings("{\n    s = s;\n}"), ["R17"])
        self.assertEqual(findings("{\n    return n + 0;\n}"), ["R17"])
        self.assertEqual(findings("{\n    do { a(); } while (0);\n}"), ["R17"])
        self.assertEqual(findings("{\n    a();\n    ;\n}"), ["R17"])

    def test_type_toggled_by_a_unit_macro(self):
        with tempfile.TemporaryDirectory() as d:
            h = Path(d) / "list.h"
            h.write_text("template <class T> class CList {\npublic:\n#ifdef LIST_ADD_INLINE\n"
                         "    __forceinline\n#endif\n    void Add(T t);\n};\n#ifdef SOME_GUARD_H\n#endif\n")
            c = Path(d) / "a.cpp"
            c.write_text('#define LIST_ADD_INLINE\n#include "list.h"\n')
            b = Path(d) / "b.cpp"
            b.write_text('#include "list.h"\n')
            toggles = rc.toggled_types([rc.Unit(h), rc.Unit(c), rc.Unit(b)])
        self.assertEqual(dict(toggles), {"CList": {"LIST_ADD_INLINE"}})

    def test_original_self_assignment_needs_a_label(self):
        # the retail code calls operator=(const&) on one object with itself
        labelled = "{\n    // NOTE: the original assigns s to itself\n    s = s;\n}"
        self.assertEqual(findings(labelled), [])
        self.assertEqual(findings("{\n    s = s;  // BUG: no-op in the original\n}"), [])
        two_lines = "{\n    // NOTE: the original assigns s\n    // to itself\n    s = s;\n}"
        self.assertEqual(findings(two_lines), [])
        self.assertEqual(findings("{\n    // NOTE: unrelated\n    a();\n    s = s;\n}"), ["R17"])

    def test_genuine_forms_pass(self):
        self.assertEqual(findings("{\n    while (*p++) ;\n}"), [])
        self.assertEqual(findings("{\n    int a[] = {1, 2};\n    return a[0] * 10 + 0x0;\n}"), [])
        self.assertEqual(findings("{\n    for (;;) {\n        break;\n    }\n}"), [])

    def test_comments_and_strings_are_ignored(self):
        self.assertEqual(findings('{\n    // volatile, __asm, (char*)this + 4\n    Print("x + 0;");\n}'), [])


class Declarations(unittest.TestCase):
    def test_extern_data_skips_functions(self):
        text = rc.blank("extern int g_n;          // 0x1\n"
                        "extern \"C\" double __cdecl sin(double);\n"
                        "extern void (__cdecl *g_pfn)(int);\n"
                        "extern const float g_af[32];\n")
        self.assertEqual([n for n, _, _ in rc.extern_data(text)], ["g_n", "g_pfn", "g_af"])

    def test_extern_types_compare_array_bounds_equal(self):
        a = rc.extern_data(rc.blank("extern float g_a[];"))[0][1]
        b = rc.extern_data(rc.blank("extern float g_a[4];"))[0][1]
        self.assertEqual(a, b)

    def test_function_bodies_skip_instantiations(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "unit.cpp"
            path.write_text("// FUNCTION: SWKOTOR 0x00401000 SYMBOL: ?F@@YAXXZ\n"
                            "template void CList<int>::Clear();\n\n"
                            "// FUNCTION: SWKOTOR 0x00401010 SYMBOL: ??_GC@@UAEPAXI@Z\n"
                            "class C {\npublic:\n    virtual ~C();\n};\n\n"
                            "// FUNCTION: SWKOTOR 0x00401020\n"
                            "int C::Get() const\n{\n    return m_n;\n}\n")
            got = [(a, n, s is not None) for a, n, _, s, _ in rc.function_bodies(rc.Unit(path))]
        self.assertEqual(got, [(0x401000, "?", False), (0x401010, "?", False),
                               (0x401020, "C::Get", True)])

    def test_type_copies_with_different_bodies(self):
        with tempfile.TemporaryDirectory() as tmp:
            a, b, c = Path(tmp) / "a.h", Path(tmp) / "b.h", Path(tmp) / "c.h"
            a.write_text("class CList { public: int* m_pArray; int m_nCount; };\n")
            b.write_text("class CList { public: int* element; int num; };\n")
            c.write_text("struct CList;\n")
            bodies, keywords = rc.collect_types([rc.Unit(a), rc.Unit(b), rc.Unit(c)])
        self.assertEqual(len(bodies["CList"]), 2)
        self.assertEqual(set(keywords["CList"]), {"class", "struct"})


if __name__ == "__main__":
    unittest.main()
