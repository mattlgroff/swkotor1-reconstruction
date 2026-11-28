# R18: the assembly bodies, rewritten as C++

Nine functions had `__declspec(naked)` or `__asm` bodies. Each reproduced
compiler output, so each had C++ behind it.

| address | function | what the assembly hid | now |
| --- | --- | --- | --- |
| 0x00406C70 | `CExoResMan::CacheResource(const CExoString&, unsigned short, int&)` | a by-value `CExoString` return: hidden return slot, the "return value constructed" flag, a copy into a temporary and a copy into the slot (no NRVO in MSVC 7.0). It was called `CopyString`; the Mac symbol tables name it (exoresman.o). | match |
| 0x004085F0 | `CExoResMan::CacheResource(const CExoString&, unsigned short)` | the two-argument overload passing a local `int` that MSVC keeps in the dead `nType` slot | match |
| 0x00410560 | `ComputeCRC` | a CRC-32 loop that computes the table index, advances, then folds the entry in | match |
| 0x0044BF30 | `AurResReadData` | `static`: only this unit calls it | match |
| 0x0044BE20 | `AurResGetNextLineFile` | `static`; MSVC passes `pRes` in `eax` | match |
| 0x0044BE70 | `AurResGetNextLineData` | `static`; `pRes` in `eax` | match |
| 0x0044BFA0 | `AurResGetNextLine` | tail-calls the two readers with `pRes = 0` in `eax` | match |
| 0x0044C680 | `AurResGetResource` | `static`; `szExt`/`szName` in `ecx`/`edx`, `pnSize` on a caller-cleaned stack (a `__fastcall` would end `ret 4`) | match |
| 0x00418670 | `CSWGuiEditbox::IsSelectable` | reads the flag byte at `+0x44` with `movzx`, where every other access in the unit fits `unsigned long` bit-fields | NONMATCH (2 instructions) |

MSVC 7.0 at `-O2` gives a `static` function whose callers are all in the unit
its own argument registers and stack clean-up. A body that takes an argument
in `eax`, or reads `ecx`/`edx` and returns with a plain `ret`, is the first
sign of one.

The scaffolding the assembly needed went with it: `AurResOperatorNew`,
`AurResResourceOperatorNew`, `AurResStackAllocateHelper` (a `this` cast to
`CExoArrayList`) and the placement-new helpers of `CopyString`.

`IsSelectable` is the one loss. Declaring the flags at `+0x44` as
`unsigned char` bit-fields gives its byte load but breaks 16 other functions,
which differ only in register choice. Its real declaration needs the whole
`CSWGuiControl` layout (R1).
