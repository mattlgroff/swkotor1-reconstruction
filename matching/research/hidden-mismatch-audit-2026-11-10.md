# Hidden-mismatch audit, 2026-11-10

`tools/matchcmp.py` masks every relocation before comparing bytes. A
function can therefore report MATCH while it calls the wrong function, loads
the wrong global, or uses the wrong float constant or string literal. This
note records an audit of every MATCH function for those errors, the fixes,
and what is still open.

## Method

A checker compiled every annotated source with the canonical flags and
walked each MATCH function's relocations. For every relocation it read the
original's real target at the same offset in `swkotor.exe` and judged it in
one of two ways:

- **Data defined in our object** (strings, `__real@` constants, static
  data, switch tables): the bytes were compared, following nested
  relocations.
- **External symbols:** the address was compared against independent ground
  truth. The ground truth was the annotation addresses under `matching/src`,
  `crt_libcmt.csv`, the import table, and `// 0xADDR` comments in headers.
  `symbols.csv` was not used, because it is generated from the same MATCH
  relocations.

The checker was validated with five deliberately injected errors: a float
constant, a second constant, a string, a global, and a CRT call. matchcmp
still reported all five functions as OK. The checker flagged exactly those
five.

## Results

The first run was on local main 580f5d54, with 8,452 MATCH. It found 368
disagreements in 202 MATCH functions:

| Kind | Count | Typical causes |
| --- | --- | --- |
| callee | 241 | scalar vs array new/delete or free, wrong template instance, destructor of another class, swapped switch cases, `_strnicmp` instead of `_strncmp`, `__aulldiv` instead of `__alldiv`, `operator!=` instead of `operator==` |
| string | 80 | invented GFF labels and error texts, swapped string pairs |
| global | 36 | swapped globals, misnamed constant globals |
| constant | 11 | wrong float literals, e.g. 0.42857143f instead of 0.4285715f, 400 instead of 900, deg/rad swapped |

All buckets were fixed in one pass, and no MATCH was lost. A re-run on the
fixed tree leaves **44 rows**, all of them callee rows. No string, global or
constant rows remain.

## Lessons for new work

- Check every new MATCH's call targets, globals, constants and strings
  against the original. A clean matchcmp run proves none of them.
- MSVC 7.0 lowers `new T[n]` and `delete[]` to plain `??2`/`??3` unless
  `operator new[]` or `operator delete[]` is declared in the translation
  unit. Declare them only where the original really calls `??_U`/`??_V`.
- Read GFF field names, error messages and other string literals from the
  exe, not from memory or other projects.
- Swapped switch cases with identical bodies show up only in the jump table.
  The order of the case bodies in the source decides which block is which.
- Implicit destructors stay out of line in cl 13.00. Where the original
  inlines one, declare an explicit inline `~X() {}`.

## Open items (44 remaining rows)

1. **Identical-code folding (about 28 rows).** The original linker folded
   functions with identical bytes, so one address serves several template
   instances. For example, the `CExoArrayList` destructor at 0x0056B6F0
   serves several element types. The checker assumed one symbol, one
   address. It should accept a call when our compiled callee's bytes equal
   the original target's bytes.
   - This affects the list-destructor rows in a, c, d, f and m.
   - It also affects the pointer-list `Allocate`/`Add` rows in g and pilot4.
2. **Re-check bucket g's list retypes.** The audit fix retyped
   `aur_array.cpp` 0x00437C40/0x00437D60/0x00438B40 to
   `CExoArrayList<MdlNodeTriMesh*>`, and 0x0042E240 to `<unsigned int>`,
   assuming one body per element type. Under folding that assumption does
   not hold. The bytes are unchanged, but the type names may be wrong. This
   is also why `pilot4` `MdlNode::GetPartsList` now conflicts.
3. **COMDAT copies from another unit (about 15 rows, g, h and i).** The
   original kept another translation unit's out-of-line copy of a template
   function. That copy was built with the other new/delete pair. One source
   file cannot express this without losing inlining the original also does.
   Left as is.
4. **Real disagreements to fix (4):**
   - `pilot1` 0x0040F990 calls `CResRef::operator=(const char*)`. The
     original calls the `char[16]` overload at 0x00406170.
   - `pilot3` 0x00698EC0 calls the `CSWCAnimBase` constructor. The original
     calls `CSWCAnimBaseTW` at 0x0069CBD0, so the base class is probably
     wrong.
   - `k` 0x00648BE0 `SetWillSavingThrow` versus bucket f's
     `class_getters.cpp` names. The Mac debug map names 0x005BCCF0
     `GetRefSaveBonus` and 0x005BCD10 `GetWillSaveBonus`. The f loader stores
     the "WillSave" column at +0x93, which is named `m_lstRefSave`. Confirm
     which function the original saving-throw code calls, then fix f's
     names. f's 0x005BE010/0x005BE070 are `IsSkillClassSkill` and
     `IsSkillUseable` in the debug map.
   - `c` 0x005FBFE0: three lists share one constructor, 0x005F4F40, so
     they share one element type. That type is still unknown.
