"""Regression tests for ppcnames.py on synthetic inputs; no game files needed.

    python -m unittest discover -s matching/tools -p "test_*.py"
"""
import contextlib
import io
import struct
import sys
import tempfile
import unittest
from collections import Counter
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ppcnames as pn  # noqa: E402


def mac_index(*entries):
    """ppc_index from (start, demangled, mangled) entries."""
    return pn.ppc_index({start: (mangled, demangled) for start, demangled, mangled in entries})


def windows(*entries):
    """windows_names-shaped {address: (qual, params, kind, shown)} from (address, demangled, kind)."""
    return {a: (*pn.split_name(d), kind, d) for a, d, kind in entries}


class Signatures(unittest.TestCase):
    def test_unique_name_with_wrong_parameter_kind_is_refused(self):
        pairs, rejects = pn.Pairs(), []
        pn.pair_names(pairs, windows((0x1000, "public: void __thiscall Foo::bar(int)", None)),
                      mac_index((0x10, "Foo::bar(float)", "__ZN3Foo3barEf")), {0x1000}, rejects)
        self.assertEqual(pairs.p, {})
        self.assertEqual(rejects, [(0x1000, [0x10])])

    def test_guessed_integer_and_pointer_types_are_compatible(self):
        pairs = pn.Pairs()
        pn.pair_names(pairs, windows((0x1000, "public: int __thiscall Foo::bar(int,class CExoString *)", None)),
                      mac_index((0x10, "Foo::bar(unsigned long, CExoString const&)", "__ZN3Foo3barEmRK10CExoString")),
                      {0x1000})
        self.assertEqual(pairs.p, {0x1000: 0x10})
        self.assertEqual(pairs.signature[0x1000], "compatible")

    def test_only_the_listed_differences_are_compatible(self):
        for windows, mac in (("Foo*", "Bar*"), ("void(*)(int)", "void(*)(float)"), ("|const", ""),
                             ("unsigned char", "int"), ("Foo*", "Foo**"), ("int", "float"), ("int", "Foo*")):
            with self.subTest(windows=windows, mac=mac):
                self.assertIsNone(pn.signature_match(windows, mac))
        for windows, mac in (("int", "unsigned long"), ("int", "unsigned char"), ("int", "bool"),
                             ("Foo*", "Foo const&"), ("void*", "Foo*"), ("Foo*", "void*"),
                             ("char*", "unsigned char*"), ("CExoKeyTable**", "CExoKeyTable*&"),
                             ("int|const", "unsigned long|const")):
            with self.subTest(windows=windows, mac=mac):
                self.assertEqual(pn.signature_match(windows, mac), "compatible")

    def test_template_arguments_are_not_indirection(self):
        self.assertIsNone(pn.signature_match("Box<int*>", "Box<int&>"))
        self.assertIsNone(pn.signature_match("Box<int*>*", "Box<int&>*"))
        self.assertEqual(pn.signature_match("Box<int>*", "Box<int> const&"), "compatible")
        self.assertEqual(pn.base_type("const volatile Box<int>*"), pn.base_type("Box<int> const volatile*"))

    def test_operators_keep_their_class(self):
        self.assertEqual(pn.split_name("Foo::operator int()"), ("Foo::operator int", ""))
        self.assertEqual(pn.split_name("public: __thiscall Foo::operator int(void)"), ("Foo::operator int", ""))
        self.assertEqual(pn.split_name("public: class CResRef & __thiscall CResRef::operator=(class CResRef const &)"),
                         pn.split_name("CResRef::operator=(CResRef const&)"))
        self.assertEqual(pn.split_name("Foo::operator()(int) const"), ("Foo::operator()", "int|const"))
        self.assertEqual(pn.split_name("Box<unsigned int>::operator bool() const")[0], "Box<unsigned int>::operator bool")
        self.assertEqual(pn.split_name("bool Box<unsigned int>::operator==(Box<unsigned int> const&)")[0],
                         "Box<unsigned int>::operator==")
        self.assertNotEqual(pn.split_name("Box<unsigned int>::operator bool()"),
                            pn.split_name("Other<unsigned int>::operator bool()"))

    def test_parameter_count_must_match(self):
        self.assertIsNone(pn.signature_match("int,int", "int"))
        self.assertIsNone(pn.signature_match("int", ""))
        self.assertIsNone(pn.signature_match("Vector", "Vector*"))
        self.assertIsNone(pn.signature_match("float", "double"))
        self.assertEqual(pn.signature_match(None, "int"), "unknown")
        self.assertEqual(pn.signature_match("CExoString const&", "CExoString const&"), "exact")

    def test_msvc_and_gcc_spellings_normalize_alike(self):
        _, a = pn.split_name("public: void __thiscall Foo::bar(class CExoString const &,char const *)const ")
        _, b = pn.split_name("Foo::bar(CExoString const&, char const*) const")
        self.assertEqual(a, b)
        self.assertEqual(pn.split_name("public: void __thiscall CExoArrayList<unsigned long>::SetSize(int)"),
                         pn.split_name("CExoArrayList<unsigned long>::SetSize(int)"))

    def test_exact_parameters_win_over_compatible_ones(self):
        pairs = pn.Pairs()
        pn.pair_names(pairs, windows((0x1000, "public: void __thiscall Foo::bar(int)", None)),
                      mac_index((0x10, "Foo::bar(unsigned long)", "__ZN3Foo3barEm"),
                                (0x20, "Foo::bar(int)", "__ZN3Foo3barEi")), {0x1000})
        self.assertEqual(pairs.p, {0x1000: 0x20})
        self.assertEqual(pairs.signature[0x1000], "exact")

    def test_parameters_are_checked_before_ctor_variants(self):
        # The complete-object copy (C1) is preferred, but only among
        # constructors whose parameters fit.
        pairs = pn.Pairs()
        pn.pair_names(pairs, windows((0x1000, "public: __thiscall Foo::Foo(int)", "ctor")),
                      mac_index((0x10, "Foo::Foo(float)", "__ZN3FooC1Ef"),
                                (0x20, "Foo::Foo(int)", "__ZN3FooC2Ei")), {0x1000})
        self.assertEqual(pairs.p, {0x1000: 0x20})


class Agreement(unittest.TestCase):
    def build(self, names):
        return SimpleNamespace(names={p: ("", d) for p, d in names.items()})

    def pairs(self, mapping, method="name"):
        pairs = pn.Pairs()
        for w, p in mapping.items():
            pairs.add(w, p, method)
        return pairs

    def test_same_method_different_parameters_is_a_conflict(self):
        builds = [self.build({0x10: "Foo::bar(int)"}), self.build({0x30: "Foo::bar(float)"})]
        out = pn.combine({0x1000}, builds, [self.pairs({0x1000: 0x10}), self.pairs({0x1000: 0x30})])
        self.assertEqual(out[0x1000], ("", "", "conflict", "Foo::bar(float) | Foo::bar(int)"))

    def test_conversion_operators_of_two_classes_conflict(self):
        builds = [self.build({0x10: "Foo::operator int()"}), self.build({0x30: "Bar::operator int()"})]
        out = pn.combine({0x1000}, builds, [self.pairs({0x1000: 0x10}), self.pairs({0x1000: 0x30})])
        self.assertEqual(out[0x1000][2], "conflict")

    def test_same_signature_agrees(self):
        builds = [self.build({0x10: "Foo::bar(int)"}), self.build({0x30: "Foo::bar(int)"})]
        out = pn.combine({0x1000}, builds, [self.pairs({0x1000: 0x10}), self.pairs({0x1000: 0x30}, "calls")])
        self.assertEqual(out[0x1000], ("Foo::bar(int)", "name", "2/2", ""))


class Propagation(unittest.TestCase):
    def test_one_anchor_that_is_caller_and_callee_counts_once(self):
        # A (paired) calls X and X calls A: the callee and the caller views
        # both propose X, but only A supports them.
        pairs = pn.Pairs()
        pairs.add(0xA, 0xA0, "name")
        wscan = {0xA: ([0xB], frozenset()), 0xB: ([0xA], frozenset())}
        pscan = {0xA0: ([0xB0], frozenset()), 0xB0: ([0xA0], frozenset())}
        size = {0xA: 32, 0xB: 32, 0xA0: 32, 0xB0: 32}
        pn.pair_calls(pairs, wscan, pscan, {0xA, 0xB}, size, size, 2)
        self.assertNotIn(0xB, pairs.p)
        pn.pair_calls(pairs, wscan, pscan, {0xA, 0xB}, size, size, 1)
        self.assertEqual(pairs.method.get(0xB), "calls1")

    def test_two_independent_anchors_pair(self):
        pairs = pn.Pairs()
        pairs.add(0xA, 0xA0, "name")
        pairs.add(0xC, 0xC0, "name")
        wscan = {0xA: ([0xB], frozenset()), 0xC: ([0xB], frozenset()), 0xB: ([], frozenset())}
        pscan = {0xA0: ([0xB0], frozenset()), 0xC0: ([0xB0], frozenset()), 0xB0: ([], frozenset())}
        size = {x: 32 for x in (0xA, 0xB, 0xC, 0xA0, 0xB0, 0xC0)}
        pn.pair_calls(pairs, wscan, pscan, {0xA, 0xB, 0xC}, size, size, 2)
        self.assertEqual(pairs.method.get(0xB), "calls")

    def vtable(self, known):
        pairs, stats = pn.Pairs(), Counter()
        for w, p in known.items():
            pairs.add(w, p, "name")
        names = {0x10: ("", "Foo::a()"), 0x20: ("", "Foo::b()"), 0x30: ("", "Foo::c()")}
        pn.pair_vtables(pairs, {"Foo": [0x100, 0x200, 0x300]}, {"Foo": [0x10, 0x20, 0x30]},
                        names, {0x100: 32, 0x200: 32, 0x300: 32}, stats)
        return pairs, stats

    def test_vtable_alignment_is_used_when_known_slots_agree(self):
        pairs, stats = self.vtable({0x100: 0x10})
        self.assertEqual(pairs.p, {0x100: 0x10, 0x200: 0x20, 0x300: 0x30})
        self.assertEqual(stats["vtables paired"], 1)

    def test_vtable_alignment_stops_when_a_known_slot_contradicts_it(self):
        pairs, stats = self.vtable({0x100: 0x20})
        self.assertEqual(pairs.p, {0x100: 0x20})
        self.assertEqual(stats["vtables contradicted"], 1)


class X86Scanner(unittest.TestCase):
    def scan(self, code, starts=(), strings=None):
        win = SimpleNamespace(body=lambda s, e: code[:e - s], strings=strings or {})
        return pn.scan_windows(win, set(starts) | {0x1000}, {0x1000: 0x1000 + len(code)}, [0x1000])[0x1000]

    def test_e8_inside_an_operand_is_not_a_call(self):
        # mov eax, 0x00000FFA_E8 ... : the E8 is the MOV's immediate
        calls, _ = self.scan(bytes.fromhex("b8 e8 fa 0f 00 00 90 c3"), starts={0x2000, 0x1FFF})
        self.assertEqual(calls, [])

    def test_real_call_and_string_reference(self):
        code = bytes.fromhex("68 00 30 40 00") + b"\xe8" + struct.pack("<i", 0x2000 - 0x100A) + b"\xc3"
        calls, texts = self.scan(code, starts={0x2000}, strings={0x403000: "hello"})
        self.assertEqual(calls, [0x2000])
        self.assertEqual(texts, {"hello"})

    def test_lengths(self):
        for hexcode, length in (("8b 04 8d 00 10 40 00", 7),  # mov eax, [ecx*4+0x401000]
                                ("66 c7 45 f8 34 12", 6),     # mov word [ebp-8], 0x1234
                                ("0f 84 00 00 00 00", 6),     # jz rel32
                                ("f6 45 08 01", 4),           # test byte [ebp+8], 1
                                ("f7 d8", 2),                 # neg eax
                                ("d9 05 00 10 40 00", 6),     # fld dword [0x401000]
                                ("c2 08 00", 3)):
            code = bytes.fromhex(hexcode)
            self.assertEqual(pn.x86_instruction(code, 0)[0], length, hexcode)

    def test_prefixed_call_counts_and_16_bit_call_does_not(self):
        self.assertEqual(self.scan(bytes.fromhex("2e e8 fa 0f 00 00 c3"), starts={0x2000})[0], [0x2000])
        self.assertEqual(self.scan(bytes.fromhex("66 2e e8 f9 0f c3"), starts={0x1FFE, 0x0FF9F833})[0], [])

    def test_3dnow_ends_in_its_opcode_byte(self):
        # pavgusb mm0, mm0 (0F 0F C0 BF), then call 0x2000
        code = bytes.fromhex("0f 0f c0 bf e8 f7 0f 00 00 c3")
        self.assertEqual(pn.x86_instruction(code, 0)[0], 4)
        self.assertEqual(self.scan(code, starts={0x2000})[0], [0x2000])

    def test_truncated_instruction_ends_the_scan(self):
        self.assertEqual(self.scan(bytes.fromhex("90 e8 00 00"))[0], [])


def ppc(*words):
    return b"".join(struct.pack(">I", w) for w in words)


def lis(rd, hi):
    return (15 << 26) | (rd << 21) | (hi & 0xFFFF)


def addi(rd, ra, imm):
    return (14 << 26) | (rd << 21) | (ra << 16) | (imm & 0xFFFF)


def dform(op, rd, ra, imm):
    return (op << 26) | (rd << 21) | (ra << 16) | (imm & 0xFFFF)


MFLR3 = 0x7C6802A6
BCTRL = 0x4E800421
BL_NEXT = 0x48000005  # bl $+4


def memory(code, table=b"", table_at=0x8000):
    """A Mac build stub: code at 0x1000, a table at table_at."""
    def bytes_at(a, n):
        for start, data in ((0x1000, code), (table_at, table)):
            if start <= a < start + len(data):
                return data[a - start:a - start + n]
        return b""
    word = lambda a: struct.unpack(">I", bytes_at(a, 4))[0] if len(bytes_at(a, 4)) == 4 else None
    return SimpleNamespace(bytes_at=bytes_at, word=word)


MTCTR0 = 0x7C0903A6  # mtctr r0
BCTR = 0x4E800420
BLR = 0x4E800020


def lwzx(rd, ra, rb):
    return (31 << 26) | (rd << 21) | (ra << 16) | (rb << 11) | (23 << 1)


def cmplwi(ra, value):
    return (10 << 26) | (ra << 16) | (value & 0xFFFF)


def slwi(ra, rs, n):
    return (21 << 26) | (rs << 21) | (ra << 16) | (n << 11) | (0 << 6) | ((31 - n) << 1)


def add(rd, ra, rb):
    return (31 << 26) | (rd << 21) | (ra << 16) | (rb << 11) | (266 << 1)


class PPCScanner(unittest.TestCase):
    def texts(self, code, strings, table=b""):
        return pn.scan_ppc(memory(code, table), {0x1000: 0x1000 + len(code)}, strings)[0x1000][1]

    def test_bctrl_forgets_volatile_registers(self):
        code = ppc(lis(3, 1), BCTRL, addi(5, 3, 0x20))
        self.assertEqual(self.texts(code, {0x10020: "stale"}), frozenset())

    def test_bl_forgets_the_link_register(self):
        # bcl 20,31,$+4 sets the PIC base; a later bl changes LR, so mflr after it is unknown
        code = ppc(0x429F0005, BL_NEXT, MFLR3, addi(4, 3, 0x10))
        self.assertEqual(self.texts(code, {0x1004 + 0x10: "pic", 0x1008 + 0x10: "after bl"}), frozenset())
        code = ppc(0x429F0005, MFLR3, addi(4, 3, 0x10))
        self.assertEqual(self.texts(code, {0x1004 + 0x10: "pic"}), {"pic"})

    def test_update_form_moves_the_base_register(self):
        # lis r3,1; stwu r4,16(r3); addi r5,r3,32 -> r3 is 0x10010, so 0x10030
        code = ppc(lis(3, 1), dform(37, 4, 3, 16), addi(5, 3, 32))
        self.assertEqual(self.texts(code, {0x10020: "stale", 0x10030: "right"}), {"right"})
        code = ppc(lis(3, 1), dform(33, 4, 3, 16), addi(5, 3, 32))  # lwzu
        self.assertEqual(self.texts(code, {0x10020: "stale", 0x10030: "right"}), {"right"})

    def test_code_a_branch_skips_does_not_feed_the_target(self):
        # b +8 skips lis r3,1; at addi r3 is unknown on the only path that reaches it
        code = ppc(0x48000008, lis(3, 1), addi(5, 3, 32))
        self.assertEqual(self.texts(code, {0x10020: "false"}), frozenset())

    def test_paths_that_agree_keep_their_value(self):
        # lis r3,1; beq +8; li r4,0; addi r5,r3,32: r3 is the same on both paths
        code = ppc(lis(3, 1), 0x41820008, addi(4, 0, 0), addi(5, 3, 32))
        self.assertEqual(self.texts(code, {0x10020: "kept"}), {"kept"})

    def scan(self, code, ends=None):
        return pn.scan_ppc(memory(code), ends or {0x1000: 0x1000 + len(code)}, {})[0x1000]

    def switch(self, relative, entries=None, count=2, between=(), guard="bgt", after_guard=()):
        """lis r31,1; cmplwi r3,count-1; guard; after_guard; r9 = 0x8000 (the
        table); slwi r0,r3,2; lwzx r0,r9,r0; [add r0,r0,r9;] [between;]
        mtctr r0; bctr; then case 1: addi r4,r31,0x20; blr; a block only the
        guard reaches: lis r31,3; addi r7,r31,0x50; blr; case 2: lis r31,2;
        addi r6,r31,0x40; blr. The guard is bgt out, bgtlr, or ble past a
        b out. Returns (code, table, bctr index, case indices)."""
        guards = {"bgt": [None], "bgtlr": [0x4D810020], "ble": [0x40810008, None]}
        body = [lis(31, 1), cmplwi(3, count - 1)] + guards[guard] + list(after_guard)
        body += [lis(9, 1), addi(9, 9, -0x8000), slwi(0, 3, 2), lwzx(0, 9, 0)]
        body += [add(0, 0, 9)] if relative else []
        body += list(between) + [MTCTR0, BCTR]
        bctr = len(body) - 1
        case1, out, case2 = bctr + 1, bctr + 3, bctr + 6
        body += [addi(4, 31, 0x20), BLR, lis(31, 3), addi(7, 31, 0x50), BLR, lis(31, 2), addi(6, 31, 0x40), BLR]
        if None in body:
            at = body.index(None)
            body[at] = (0x41810000 if guard == "bgt" else 0x48000000) | ((out - at) * 4)  # bgt out / b out
        targets = entries or [0x1000 + 4 * case1, 0x1000 + 4 * case2]
        table = b"".join(struct.pack(">i", e - 0x8000 if relative else e) for e in targets)
        return ppc(*body), table, bctr, [case1, case2]

    def analyse(self, code, table):
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        return words, pn.ppc_analyse(words, 0x1000, memory(code, table).word)

    def test_jump_table_is_recovered(self):
        for relative in (False, True):
            with self.subTest(relative=relative):
                code, table, bctr, cases = self.switch(relative)
                words, (before, succ) = self.analyse(code, table)
                self.assertEqual(pn.ppc_jump_table(words, 0x1000, before, bctr, memory(code, table).word), cases)
                self.assertEqual(succ[bctr], cases)  # not every block: the bgt-only block is left out
                self.assertEqual(self.texts(code, {0x10020: "case 1", 0x20040: "case 2", 0x30050: "out"}, table),
                                 {"case 1", "case 2", "out"})

    def recovered(self, code, table, bctr):
        words, (before, _) = self.analyse(code, table)
        return pn.ppc_jump_table(words, 0x1000, before, bctr, memory(code, table).word)

    def test_real_guard_shapes_prove_the_bound(self):
        for guard in ("bgt", "bgtlr", "ble"):
            with self.subTest(guard=guard):
                code, table, bctr, cases = self.switch(False, guard=guard)
                self.assertEqual(self.recovered(code, table, bctr), cases)

    def test_guard_after_the_scale(self):
        # cmplwi r3,1; slwi r0,r3,2; bgt out; table load ... (0xb0230 in the 2009 build)
        code, table, bctr, cases = self.switch(False)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        bgt, scale = words[2], words.index(slwi(0, 3, 2))
        moved = words[:2] + words[3:scale + 1] + [0x41810000 | ((bgt & 0xFFFC) - 4 * (scale - 2))] + words[scale + 1:]
        self.assertEqual(self.recovered(ppc(*moved), table, bctr), cases)

    def test_another_way_into_the_switch_is_not_proven(self):
        # a branch from before the guard straight to the table code skips the check
        code, table, bctr, _ = self.switch(False)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        words[0] = 0x41820000 | (3 * 4)  # beq to the instruction after the guard
        self.assertIsNone(self.recovered(ppc(*words), table, bctr))

    def test_skipping_the_compare_or_the_load_is_not_proven(self):
        code, table, bctr, _ = self.switch(False, count=1)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        mtctr = words.index(MTCTR0)
        for label, first in (("b over the cmplwi to the guard", 0x48000008),
                             ("beq past the load to the mtctr", 0x41820000 | (mtctr * 4))):
            with self.subTest(label):
                self.assertIsNone(self.recovered(ppc(first, *words[1:]), table, bctr))

    def test_a_branch_to_the_next_instruction_is_no_guard(self):
        code, table, bctr, _ = self.switch(False, count=1)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        self.assertIsNone(self.recovered(ppc(*words[:2], 0x40810004, *words[3:]), table, bctr))  # ble $+4

    def test_the_chain_stops_at_the_function_entry(self):
        # the compare is reached only by a back-edge; the first call runs without it
        words = [lis(9, 1), addi(9, 9, -0x8000), slwi(0, 3, 2), lwzx(0, 9, 0), MTCTR0, BCTR, BLR,
                 cmplwi(3, 0), 0x41810008, 0x4BFFFFDC, BLR]
        table = struct.pack(">II", 0x1018, 0x1028)
        self.assertIsNone(self.recovered(ppc(*words), table, 5))

    def test_a_bound_no_branch_enforces_is_not_proven(self):
        code, table, bctr, _ = self.switch(False, count=1)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        bgt = words[2]
        for label, change in (("no branch", {2: 0x60000000}),
                              ("blt", {2: 0x41800000 | (bgt & 0xFFFF)}),
                              ("bgt on cr1", {2: 0x41850000 | (bgt & 0xFFFF)}),
                              ("cr0 overwritten", {0: cmplwi(3, 0), 1: cmplwi(4, 0)})):
            with self.subTest(label):
                changed = [change.get(i, w) for i, w in enumerate(words)]
                self.assertIsNone(self.recovered(ppc(*changed), table, bctr))

    def test_index_changed_after_the_guard_is_not_proven(self):
        ori = (24 << 26) | (3 << 21) | (3 << 16) | 1  # ori r3,r3,1
        self.assertEqual(pn.ppc_writes(ori), {3})
        code, table, bctr, _ = self.switch(False, after_guard=[ori])
        self.assertIsNone(self.recovered(code, table, bctr))

    def test_writes_cover_what_a_step_changes(self):
        import random
        rng = random.Random(4)
        for _ in range(20000):
            w = rng.getrandbits(32)
            regs = {r: rng.getrandbits(32) for r in range(32)}
            after, _ = pn.ppc_step(w, 0x1000, regs, None, lambda a: None)
            changed = {r for r in range(32) if after.get(r) != regs[r]}
            self.assertLessEqual(changed, pn.ppc_writes(w), f"{w:08x}")

    def test_mulhw_has_no_overflow_form(self):
        self.assertNotIn(75 + 512, pn.X31_WRITE_RD)
        self.assertNotIn(11 + 512, pn.X31_WRITE_RD)

    def test_jump_table_reads_only_the_checked_entries(self):
        # one case checked; the next word looks like a case but is not one
        code, table, bctr, cases = self.switch(False, count=1)
        words, (before, succ) = self.analyse(code, table)
        self.assertEqual(succ[bctr], [cases[0]])

    def test_jump_table_beyond_max_cases_is_not_proven(self):
        code, table, bctr, _ = self.switch(False, count=pn.MAX_CASES + 2)
        table = table[:4] * (pn.MAX_CASES + 2)
        words, (before, _) = self.analyse(code, table)
        self.assertIsNone(pn.ppc_jump_table(words, 0x1000, before, bctr, memory(code, table).word))

    def test_jump_table_target_overwritten_is_not_proven(self):
        code, table, bctr, _ = self.switch(False, between=[addi(0, 0, 0x1020)])  # li r0,0x1020
        words, (before, succ) = self.analyse(code, table)
        self.assertIsNone(pn.ppc_jump_table(words, 0x1000, before, bctr, memory(code, table).word))

    def test_jump_table_no_longer_proven_falls_back(self):
        # case 2 changes the table register and jumps back to the lwzx
        code, table, bctr, cases = self.switch(False)
        words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
        lwzx_at = next(i for i, w in enumerate(words) if w == lwzx(0, 9, 0))
        back = cases[1] + 1
        words[back] = lis(9, 2)
        words[back + 1] = 0x48000000 | (((lwzx_at - 1 - (back + 1)) * 4) & 0x03FFFFFC)  # b slwi
        code = ppc(*words)
        _, (before, succ) = self.analyse(code, table)
        self.assertIsNone(pn.ppc_jump_table(words, 0x1000, before, bctr, memory(code, table).word))
        _, leaders, _ = pn.ppc_flow(words, 0x1000)
        self.assertEqual(succ[bctr], leaders)

    def test_unreached_code_does_not_erase_reached_values(self):
        # lis r3,1; b +8; nop (nothing reaches it); addi r4,r3,32; blr
        code = ppc(lis(3, 1), 0x48000008, 0x60000000, addi(4, 3, 32), 0x4E800020)
        self.assertEqual(self.texts(code, {0x10020: "real"}), {"real"})

    def test_landing_pad_keeps_what_calls_keep(self):
        # lis r31,1; lis r3,2; bl $+4... ; blr; then an unreached pad using r31 and r3
        code = ppc(lis(31, 1), lis(3, 2), BL_NEXT, 0x4E800020, addi(4, 31, 32), addi(5, 3, 32))
        self.assertEqual(self.texts(code, {0x10020: "kept", 0x20020: "volatile"}), {"kept"})

    def test_bctr_may_reach_a_case_that_is_also_reached_directly(self):
        # lis r3,1; beq case; lis r3,2; bctr; case: addi r4,r3,32; blr
        code = ppc(0x3C600001, 0x4182000C, 0x3C600002, 0x4E800420, 0x38830020, 0x4E800020)
        self.assertEqual(self.texts(code, {0x10020: "one path", 0x20020: "other path"}), frozenset())

    def test_dead_code_in_a_function_without_calls_gives_nothing(self):
        code = ppc(0x4800000C, lis(3, 1), addi(5, 3, 32), BLR)  # b +12 over lis/addi
        self.assertEqual(self.texts(code, {0x10020: "dead"}), frozenset())

    def test_body_without_a_symbol_after_a_return(self):
        # li r3,1; blr; then another body: bcl 20,31,$+4; mflr r31; addi r4,r31,16
        code = ppc(addi(3, 0, 1), BLR, 0x429F0005, 0x7FE802A6, addi(4, 31, 16), BLR)
        self.assertEqual(self.texts(code, {0x100C + 16: "own body"}), {"own body"})

    def test_landing_pad_after_pic_setup(self):
        # bcl 20,31,$+4; mflr r31; bl elsewhere; blr; pad: addi r4,r31,16
        mflr31 = 0x7FE802A6
        code = ppc(0x429F0005, mflr31, 0x48001001, BLR, addi(4, 31, 16))
        self.assertEqual(self.texts(code, {0x1004 + 16: "pad"}), {"pad"})

    def test_mr_to_itself_keeps_the_value(self):
        mr31 = (31 << 26) | (31 << 21) | (31 << 16) | (31 << 11) | (444 << 1)
        code = ppc(lis(31, 1), mr31, addi(4, 31, 32))
        self.assertEqual(self.texts(code, {0x10020: "real"}), {"real"})

    def test_bctr_does_not_undo_the_pic_base(self):
        # bcl 20,31,$+4; mflr r3; ... bctr; case: addi r4,r3,0x10
        code = ppc(0x429F0005, MFLR3, 0x41820008, 0x4E800420, addi(4, 3, 0x10), 0x4E800020)
        self.assertEqual(self.texts(code, {0x1004 + 0x10: "pic"}), {"pic"})

    def test_mr_keeps_its_source(self):
        # lis r31,1; loop: addi r4,r31,32; mr r3,r31; b loop
        mr = (31 << 26) | (31 << 21) | (3 << 16) | (31 << 11) | (444 << 1)
        code = ppc(lis(31, 1), addi(4, 31, 32), mr, 0x4BFFFFF8)
        self.assertEqual(self.texts(code, {0x10020: "real"}), {"real"})

    def test_conditional_tail_call_is_a_callee(self):
        code = ppc(0x41821000, 0x4E800020)  # beq 0x2000; blr
        self.assertEqual(self.scan(code, {0x1000: 0x1008, 0x2000: 0x2004})[0], [0x2000])

    def test_empty_body(self):
        self.assertEqual(self.scan(b"", {0x1000: 0x1000}), ([], frozenset()))

    def test_lswi_forgets_every_register_it_loads(self):
        lswi = (31 << 26) | (4 << 21) | (1 << 16) | (8 << 11) | (597 << 1)  # r4, r5
        code = ppc(lis(5, 1), lswi, addi(6, 5, 32))
        self.assertEqual(self.texts(code, {0x10020: "false"}), frozenset())

    def test_lmw_forgets_what_it_loads(self):
        code = ppc(lis(30, 1), dform(46, 29, 1, 8), addi(5, 30, 0x20))
        self.assertEqual(self.texts(code, {0x10020: "stale"}), frozenset())


def macho(symbols=(), text=b"\0" * 16, symtab=True, cpu=18, magic=0xFEEDFACE):
    """A minimal PowerPC Mach-O: __TEXT,__text and, with symtab, nlist entries
    (name, type, section, value)."""
    strtab = b"\0"
    nlist = b""
    for name, ntype, sect, value in symbols:
        nlist += struct.pack(">IBBhI", len(strtab), ntype, sect, 0, value)
        strtab += name.encode() + b"\0"
    seg_size, symtab_size = 56 + 68, 24 if symtab else 0
    text_off = 28 + seg_size + symtab_size
    sym_off = text_off + len(text)
    str_off = sym_off + len(nlist)
    header = struct.pack(">7I", magic, cpu, 0, 2, 2 if symtab else 1, seg_size + symtab_size, 0)
    seg = struct.pack(">2I16s8I", 1, seg_size, b"__TEXT", 0x1000, len(text), text_off, len(text), 7, 5, 1, 0)
    seg += struct.pack(">16s16s9I", b"__text", b"__TEXT", 0x1000, len(text), text_off, 2, 0, 0, 0, 0, 0)
    cmds = seg + (struct.pack(">6I", 2, 24, sym_off, len(symbols), str_off, len(strtab)) if symtab else b"")
    return header + cmds + text + nlist + strtab


class MachOInput(unittest.TestCase):
    def load(self, data):
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "exe"
            path.write_bytes(data)
            return pn.MachO(path)

    def test_valid(self):
        mac = self.load(macho([("_main", 0x0F, 1, 0x1000)]))
        self.assertEqual(mac.symbols(), [(0x1000, 1, "_main")])

    def test_malformed_files_are_refused_cleanly(self):
        good = macho([("_main", 0x0F, 1, 0x1000)])
        for label, data in (("empty", b""),
                            ("wrong magic", macho(magic=0xFEEDFACF)),
                            ("x86", macho(cpu=7)),
                            ("no symtab", macho(symtab=False)),
                            ("truncated", good[:60]),
                            ("universal without ppc", struct.pack(">2I5I", 0xCAFEBABE, 1, 7, 3, 28, len(good), 12) + good),
                            ("universal slice past end", struct.pack(">2I5I", 0xCAFEBABE, 1, 18, 0, 28, 10 ** 6, 12))):
            with self.subTest(label), self.assertRaises(SystemExit):
                self.load(data)

    def test_load_commands_must_fill_sizeofcmds(self):
        data = bytearray(macho([("_main", 0x0F, 1, 0x1000)]))
        struct.pack_into(">I", data, 20, 0)
        with self.assertRaises(SystemExit):
            self.load(bytes(data))

    def test_unterminated_symbol_name(self):
        data = macho([("_main", 0x0F, 1, 0x1000)])
        with self.assertRaises(SystemExit):
            self.load(data[:-1] + b"X").symbols()

    def test_name_index_outside_the_string_table(self):
        data = bytearray(macho([("_main", 0x0F, 1, 0x1000)]))
        sym_off = 28 + 56 + 68 + 24 + 16
        struct.pack_into(">I", data, sym_off, 10 ** 6)
        with self.assertRaises(SystemExit):
            self.load(bytes(data)).symbols()

    def test_debug_map_resets_between_compile_units(self):
        mac = self.load(macho([("", pn.N_SO, 0, 0), ("/obj/a.o", pn.N_OSO, 0, 0), ("_f", pn.N_FUN, 1, 0x1000),
                               ("", pn.N_SO, 0, 0), ("_g", pn.N_FUN, 1, 0x1004),
                               ("/obj/b.o", pn.N_OSO, 0, 0), ("_h", pn.N_FUN, 1, 0x1008)]))
        self.assertEqual(mac.objects(), {0x1000: "a.o", 0x1008: "b.o"})


class RepoNames(unittest.TestCase):
    def names(self, symbols, status):
        tables = {"symbols.csv": symbols, "status.csv": status}
        with mock.patch.object(pn, "read_csv", lambda name: tables[name]):
            return pn.windows_names()

    def test_conflicting_rows_are_not_paired_by_name(self):
        row = lambda s, d, a: {"symbol": s, "demangled": d, "address": a, "role": "defines"}
        names, ambiguous = self.names(
            [row("?a@Foo@@QAEXXZ", "public: void __thiscall Foo::a(void)", "0x1000"),
             row("?b@Bar@@QAEXXZ", "public: void __thiscall Bar::b(void)", "0x1000"),
             row("?c@Foo@@QAEXXZ", "public: void __thiscall Foo::c(void)", "0x2000"),
             row("?d@Foo@@QAEXXZ", "public: void __thiscall Foo::d(void)", "0x3000"),
             row("?f@Foo@@QAEXH@Z", "public: void __thiscall Foo::f(int)", "0x5000"),
             row("?f@Foo@@QAEXPAH@Z", "public: void __thiscall Foo::f(int *)", "0x5000")],
            [{"address": "0x2000", "name": "Foo::other", "status": "todo"},
             {"address": "0x3000", "name": "Foo::d", "status": "MATCH"},
             {"address": "0x4000", "name": "Foo::e", "status": "todo"}])
        self.assertEqual(ambiguous, {0x1000, 0x2000})
        self.assertEqual(names[0x1000][0], "Foo::a")
        self.assertEqual(names[0x4000][:2], ("Foo::e", None))
        self.assertEqual(names[0x5000][:2], ("Foo::f", ("int", "int*")))  # one name, two parameter lists

    def test_status_rows_that_disagree_with_each_other_or_the_symbol(self):
        row = {"symbol": "?a@Foo@@QAEXXZ", "demangled": "public: void __thiscall Foo::a(void)",
               "address": "0x1000", "role": "defines"}
        _, ambiguous = self.names(
            [row], [{"address": "0x1000", "name": "?zz@Other@@QAEXXZ", "status": "todo"},
                    {"address": "0x2000", "name": "Foo::a", "status": "todo"},
                    {"address": "0x2000", "name": "Foo::b", "status": "todo"},
                    {"address": "0x3000", "name": "?a@Foo@@QAEXXZ", "status": "todo"}])
        self.assertEqual(ambiguous, {0x1000, 0x2000})

    def test_several_parameter_lists_do_not_admit_a_third(self):
        row = lambda d: {"symbol": "?f", "demangled": d, "address": "0x1000", "role": "defines"}
        names, _ = self.names([row("public: void __thiscall Foo::f(int)"),
                               row("public: void __thiscall Foo::f(float)")], [])
        pairs = pn.Pairs()
        pn.pair_names(pairs, names, mac_index((0x10, "Foo::f(double)", "__ZN3Foo1fEd")), {0x1000})
        self.assertEqual(pairs.p, {})
        pn.pair_names(pairs, names, mac_index((0x10, "Foo::f(float)", "__ZN3Foo1fEf")), {0x1000})
        self.assertEqual(pairs.p, {0x1000: 0x10})


class Outputs(unittest.TestCase):
    def test_source_labels(self):
        self.assertEqual(pn.source_how(1, {1: "a.o"}, {}), "debug map")
        self.assertEqual(pn.source_how(2, {1: "a.o"}, {2: "a.o"}), "neighbours")
        self.assertEqual(pn.source_how(3, {}, {}), "")


class Seeds(unittest.TestCase):
    def test_a_hidden_seed_is_not_paired_again_through_another_name(self):
        # 1 and 2 are both Foo::f; with every name known only 1 pairs. Hiding
        # 1 must not let 2 take its Mac partner.
        w = SimpleNamespace(names=windows((1, "public: void __thiscall Foo::f(int)", None),
                                          (2, "public: void __thiscall Foo::f(unsigned int)", None)),
                            tables={}, scan={}, game={1, 2}, size={})
        mac = SimpleNamespace(index=mac_index((101, "Foo::f(int)", "__ZN3Foo1fEi")), tables={}, names={},
                              scan={}, size={})
        reference = pn.Pairs()
        pn.pair_names(reference, w.names, mac.index, {1, 2})
        self.assertEqual(reference.p, {1: 101})
        self.assertEqual(pn.run(w, mac, {2}, Counter(), seeds=reference).p, {})


class Holdout(unittest.TestCase):
    def world(self):
        """Ten Windows functions named Foo::fN, each calling the next, and
        two Mac builds with the same functions and calls."""
        n = 10
        ws = [0x1000 + 0x40 * i for i in range(n)]
        w = SimpleNamespace(
            names={a: (f"Foo::f{i}", "", None, f"Foo::f{i}()") for i, a in enumerate(ws)},
            game=set(ws), ambiguous=set(), tables={},
            scan={a: ([ws[i + 1]] if i + 1 < n else [], frozenset()) for i, a in enumerate(ws)},
            size={a: 0x40 for a in ws})
        w.named = set(ws)
        builds = []
        for label, base in (("one", 0x10000), ("two", 0x20000)):
            ps = [base + 0x40 * i for i in range(n)]
            names = {p: (f"__ZN3Foo2f{i}Ev", f"Foo::f{i}()") for i, p in enumerate(ps)}
            builds.append(SimpleNamespace(label=label, names=names, index=pn.ppc_index(names), tables={},
                                          scan={p: ([ps[i + 1]] if i + 1 < n else [], frozenset())
                                                for i, p in enumerate(ps)},
                                          size={p: 0x40 for p in ps}))
        return w, builds

    def report(self, seed):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            pn.holdout(*self.world(), 0.3, seed)
        return out.getvalue()

    def test_reproducible_with_the_same_hidden_targets_and_a_combined_report(self):
        first = self.report(7)
        self.assertEqual(first, self.report(7))
        self.assertIn("3 of 10 name pairs hidden", first)
        self.assertEqual(first.count("3 of them paired by name in this build"), 2)
        self.assertIn("combined:", first)
        self.assertIn("coverage", first)


if __name__ == "__main__":
    unittest.main()
