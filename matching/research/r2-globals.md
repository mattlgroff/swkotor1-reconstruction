# One declaration per global (R2)

Units declared the globals they used themselves: 3,612 `extern` declarations
of 1,977 names, 1,642 of them declared inside a `.cpp`. This log records the
move to `include/shared/globals.h` and what is still declared elsewhere.

## Method

- Every top-level `extern` data declaration whose type is a built-in, a class
  or a class template moved into `shared/globals.h`, one per name, sorted by
  address. The header forward-declares the classes and templates it names,
  with the keyword most definitions use, and includes `shared/vector.h`.
- Each file that lost a declaration includes the header where its first
  declaration was.
- Names are not merged by address. The linker folds identical read-only
  constants (`/OPT:ICF`), so different script constants can share an
  address; each keeps its own name.

Result: 1779 globals from 2694 declarations in 500 files.
Byte gate 8,474 / 9,538, the same as before the move. (An intermediate state
matched `CSWCSortedJournalArrayList::SortList` too, but only through the order
of unrelated declarations in its unit, which decides its stack slots; it is
unmatched again, as it was before this work.)
Matched under the rules: 659 (was 579).

## Type conflicts the gate settled

Units disagreed on the type of 83 globals. 70 now have one declaration:

- **Script constants** (`EFFECT_TYPE_*`, `DURATION_TYPE_*`, `IMMUNITY_TYPE_*`,
  `ABILITY_*`, `DAMAGE_*`) are `const int`. Some units declared them
  `unsigned short`, because the code there loads only the low word; MSVC 7.0
  narrows the load of an `int` whose value is truncated to 16 bits at once,
  so the `int` declaration reproduces both kinds of unit. The majority type
  (`unsigned short`) lost nine matches; `const int` loses none.
- `g_fFrameTime` is a plain `float`: the `volatile` one unit gave it was a
  steering trick (`LightManager::ControlLightsFade`, a NONMATCH attempt, goes
  from 26 to 29 differing instructions).
- The rest take the type most units used, with no lost match.

## The held-back globals, resolved

A second pass gave the rest one declaration each. R2-blocked byte matches
went from 259 to 3; the byte gate lost nothing.

- **Layout views renamed to the real class**, where the Mac symbol tables
  name it: `CExoSoundInternal` -> `CExoSound` (`g_pExoSound`; the call is
  `CExoSound::Play3DOneShotSound`, 0x005D5E10), `CExoBaseTimers` ->
  `CExoBase` (`g_pExoBase`; `CExoBase::WcsToMbs`, 0x005E6690),
  `LmScene` -> `Scene`, `LoadingModel` -> `Model`, `RenderCamera` ->
  `Camera`, `LightBin` -> `Bin`, `TriggerPart` -> `Part`.
- **Width and signedness, settled by the code's own uses**: the bloom ramps
  are GL texture names (`unsigned int`, the reference type `DeleteTexture`
  takes); `g_nPowerWidth`/`Height` are `unsigned int`, as the ATI
  frame-buffer passes' conversions need (declaring them `int` loses seven
  matches), so `FindSmallestPowerOf2WidthHeight` writes through
  `unsigned int*`.
- **Render bucket lists**: `scenebsp.cpp` only resets their counts, so their
  element types are the ones `scene_buckets.cpp` reads (`Gob*`,
  `PartEmitter*`, `TriangleBin*`, `Part*`).
- **GL and WGL extension pointers** (`shared/gl_ext.h`): each is declared
  once with a glext.h-style `PFN<NAME>PROC` type, and the loader casts each
  `wglGetProcAddress` result. Twenty unit-local aliases that shared an
  address with a loader global (`g_pfnBindProgramARB`, `g_pglVertexAttribPointerNV`,
  `gl_program_string_arb_007bb580`, ...) now use the loader's name.
- **Combiner pointers** (`shared/combiners.h`): three pointers had six names
  and four typedefs; they take the loader's names and the signatures of the
  functions stored in them (`EnableFragmentShaders(int, CAurTexture* x4)`,
  `Set*Constant(const Vector&, float)`). These stay out of `globals.h`:
  adding them there moved the stack slots of
  `CSWCSortedJournalArrayList::SortList`, an unrelated unit, and lost its match.
- **Resource callbacks** (`shared/aurora_callbacks.h`), typed as the
  `CAuroraInterface` functions the constructor stores in them.
- **`OBJECT_INVALID`** (`shared/object_id.h`): units whose bytes load it from
  memory include the header; units whose bytes use the immediate keep a
  macro and never see the header.
- **Declarator lists** (`extern int a, b, c;`) are one declaration per line.
- `g_hWnd` and `g_hInstance` are `void*`, as the units' own `HWND` and
  `HINSTANCE` typedefs already made them.
- `g_lRequestedModels` (`CExoLinkedList<CAuroraModel>`) moved to `globals.h`
  once `CExoLinkedList` was merged; no byte match is blocked by R2 now.

## Still open

- **Aliases the checker cannot see.** Writable globals at one address are one
  variable (ICF folds only functions and constants), but two reconstructions
  name some of them differently: the raster callbacks at
  0x0078D404-0x0078D424 and 0x007A46F8-FC are `g_pfnGetRaster` ... in
  `aurora_callbacks.h` and `g_pfnLoadImageRaw` ... `g_pfnImageHook` in
  `d/aur_texture.cpp` and `pilot3/aurtexture.cpp`, with different pointer
  types. Unifying them changes those units' argument types; their users are
  NONMATCH today.
- `g_pTlkTableAlt` is `CTlkTable*` with a cast where `CSWTlkTable::Fetch`
  (0x005EE390) is called through it; the original was probably
  `CSWTlkTable*`, which needs `CSWTlkTable` complete in its other units (R1).
