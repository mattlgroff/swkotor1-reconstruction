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

## Still declared in units

- **Type conflicts (13).** Units disagree on the type: signedness or
  width (`unsigned short` against `int` for script constants, which changes
  the load), or a different class for the same object (`LoadingModel*` and
  `Model*`, `TriggerPart*` and `Part*`, `CExoArrayList<Part*>` and
  `CExoArrayList<Gob*>`). The class cases are R1 layout views; the width
  cases need the byte gate to pick the type.
- **Function pointers and typedef-typed globals**: the GL and WGL extension
  entry points, declared with different typedefs or calling-convention
  spellings in different units.
- **Names that are also a `#define` somewhere** (for example
  `OBJECT_INVALID`), **declarator lists** (`extern T a, b;`) and
  **declarations inside functions**.

Conflicting names: `ABILITY_CHARISMA`, `ABILITY_CONSTITUTION`, `ABILITY_DEXTERITY`, `ABILITY_INTELLIGENCE`, `ABILITY_STRENGTH`, `ABILITY_WISDOM`, `ACTION_COMPLETE`, `ACTION_FAILED`, `ACTION_IN_PROGRESS`, `AC_VS_DAMAGE_TYPE_ALL`, `DAMAGE_FLAG_00`, `DURATION_TYPE_INSTANT`, `EFFECT_TYPE_ABILITY_DECREASE`, `EFFECT_TYPE_ABILITY_INCREASE`, `EFFECT_TYPE_AC_DECREASE`, `EFFECT_TYPE_AC_INCREASE`, `EFFECT_TYPE_ATTACK_DECREASE`, `EFFECT_TYPE_ATTACK_INCREASE`, `EFFECT_TYPE_CONCEALMENT`, `EFFECT_TYPE_DAMAGE`, `EFFECT_TYPE_DAMAGE_DECREASE`, `EFFECT_TYPE_DAMAGE_IMMUNITY`, `EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE`, `EFFECT_TYPE_DAMAGE_INCREASE`, `EFFECT_TYPE_DAMAGE_REDUCTION`, `EFFECT_TYPE_DAMAGE_RESISTANCE`, `EFFECT_TYPE_DAMAGE_SHIELD`, `EFFECT_TYPE_DEATH`, `EFFECT_TYPE_DISGUISE`, `EFFECT_TYPE_FORCE_JUMP`, `EFFECT_TYPE_FORCE_RESISTANCE_DECREASE`, `EFFECT_TYPE_FORCE_RESISTANCE_INCREASE`, `EFFECT_TYPE_ICON`, `EFFECT_TYPE_IMMUNITY`, `EFFECT_TYPE_LINK`, `EFFECT_TYPE_POISON`, `EFFECT_TYPE_REGENERATE`, `EFFECT_TYPE_SAVING_THROW_DECREASE`, `EFFECT_TYPE_SAVING_THROW_INCREASE`, `EFFECT_TYPE_SETSTATE`, `EFFECT_TYPE_SKILL_DECREASE`, `EFFECT_TYPE_SKILL_INCREASE`, `EFFECT_TYPE_SPELL_IMMUNITY`, `EFFECT_TYPE_TEMPORARY_HITPOINTS`, `EFFECT_TYPE_TIMESTOP`, `EFFECT_TYPE_TRUESEEING`, `EFFECT_TYPE_VISUALEFFECT`, `EVENT_DESTROY_OBJECT`, `EVENT_SIGNAL_EVENT`, `IMMUNITY_TYPE_04`, `IMMUNITY_TYPE_CRITICAL_HIT`, `IMMUNITY_TYPE_ENTANGLE`, `IMMUNITY_TYPE_PARALYSIS`, `IMMUNITY_TYPE_SLOW`, `g_aGLBlendFactors`, `g_bVBOWriteOnly`, `g_fFrameTime`, `g_fOne`, `g_lActiveBins`, `g_lModels` ...
