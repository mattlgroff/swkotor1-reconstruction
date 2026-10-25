"""Symbol mapping regressions using synthetic COFF and linked x86 code only."""
import struct
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import symbolmap as sm


def fixture(root, addends, call_addend=0, data_is_code=False):
    """Three absolute MOV operands and one relative CALL, with a leading pad."""
    address, image_base = 0x401000, 0x400000
    array_base, callee = 0x8305CC, 0x401800
    unlinked, linked = bytearray(), bytearray()
    for addend in addends:
        unlinked += b"\xB8" + struct.pack("<I", addend & 0xFFFFFFFF)
        linked += b"\xB8" + struct.pack("<I", (array_base + addend) & 0xFFFFFFFF)
    call_offset = len(unlinked) + 1
    unlinked += b"\xE8" + struct.pack("<I", call_addend & 0xFFFFFFFF) + b"\xC3"
    linked += b"\xE8" + struct.pack("<I", (callee + call_addend - address - call_offset - 4) & 0xFFFFFFFF) + b"\xC3"

    # A nonzero function-symbol value checks section-relative addend reads.
    prefix = 4
    code = b"\x90" * prefix + unlinked
    raw_ptr = 60
    rel_ptr = raw_ptr + len(code)
    relocations = b"".join(struct.pack("<IIH", prefix + 1 + i * 5, 1, sm.DIR32)
                           for i in range(len(addends)))
    relocations += struct.pack("<IIH", prefix + call_offset, 2, sm.REL32)
    sym_ptr = rel_ptr + len(relocations)
    header = struct.pack("<HHIIIHH", 0x14C, 1, 0, sym_ptr, 3, 0, 0)
    section = struct.pack("<8sIIIIIIHHI", b".text\0\0\0", 0, 0, len(code), raw_ptr,
                          rel_ptr, 0, len(addends) + 1, 0, 0x60000020)

    def symbol(name, value, section_number, kind):
        return struct.pack("<8sIhHBB", name.ljust(8, b"\0"), value, section_number, kind, 2, 0)

    symbols = (symbol(b"_f", prefix, 1, 0x20)
               + symbol(b"_array", 0, 0, 0x20 if data_is_code else 0)
               + symbol(b"_call", 0, 0, 0x20))
    obj = root / "fixture.obj"
    obj.write_bytes(header + section + code + relocations + symbols + struct.pack("<I", 4))
    source = root / "fixture.cpp"
    source.write_text('// FUNCTION: SWKOTOR 0x00401000\nextern "C" void f() {}\n')
    pe = SimpleNamespace(
        OPTIONAL_HEADER=SimpleNamespace(ImageBase=image_base),
        get_data=lambda rva, size: linked[rva - (address - image_base):rva - (address - image_base) + size])
    return source, obj, pe, address, array_base, callee


class SymbolAddends(unittest.TestCase):
    def scan_fixture(self, addends, call_addend=0, data_is_code=False):
        with tempfile.TemporaryDirectory() as tmp:
            source, obj, pe, address, array_base, callee = fixture(
                Path(tmp), addends, call_addend, data_is_code)
            with mock.patch.object(sm.matchcmp, "compile_source", return_value=obj), \
                    mock.patch.object(sm.matchcmp, "demangle", return_value={"_f": "f"}):
                found = sm.scan(source, sm.matchcmp.DEFAULT_FLAGS, {address}, pe)
        return found, address, array_base, callee

    def test_array_members_resolve_to_one_global_base(self):
        found, address, array_base, callee = self.scan_fixture((0, 4, 8))
        self.assertEqual(found, [("_f", address, "defines")]
                         + [("_array", array_base, "data")] * 3
                         + [("_call", callee, "calls")])

    def test_negative_absolute_and_nonzero_relative_addends(self):
        found, _, array_base, callee = self.scan_fixture((-4, 0, 4), call_addend=4)
        self.assertEqual({a for s, a, _ in found if s == "_array"}, {array_base})
        self.assertEqual([a for s, a, _ in found if s == "_call"], [callee])

    def test_absolute_code_pointers_keep_the_call_role(self):
        found, _, array_base, _ = self.scan_fixture((0, 4, 8), data_is_code=True)
        self.assertEqual([(a, role) for s, a, role in found if s == "_array"],
                         [(array_base, "calls")] * 3)


if __name__ == "__main__":
    unittest.main()
