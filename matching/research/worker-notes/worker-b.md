# Worker B matching notes

Range: 0x00520000–0x0057FFFF. Older history and compiler lessons are in
Kotor RE's `docs/batches/worker-b-notes.md` (read-only); current
annotations and reports take precedence over its queue.

## Round s1 (2026-10-26): CSWVirtualMachineCommands part 3

Folder `kotor-match -q -j 2 matching/src/b`: 457/517 -> 522/590 MATCH.
73 functions annotated, 65 new exact, 8 kept as NONMATCH. No prior MATCH
regressed.

### How the work was found

`todo.py` lists only `functions.csv` starts, and in 0x0052C000-0x0054C960
that table misses most `ExecuteCommand*` bodies (one csv row often spans
5-20 real functions). Three sources fill the gap:

- A scan for 16-byte-aligned starts after `ret`/padding finds the real
  boundaries. Re-check each one in the disassembly, because an aligned
  address after a mid-function `ret` gives a false hit.
- `InitializeCommands` 0x0054C960 stores each handler into
  `m_pCommands[id]`. Tracking its `mov [reg+off], imm` stores maps each
  handler to its NWScript routine ids (off/4).
- `src/units/1360/3e4c4bbc2c-vm_action_names.inc` names every handler
  address (for example `{"ActionJumpToLocation",0x0052CDC0u}`). Use it for
  the `ExecuteCommand<Name>` names. For shared handlers, the first name
  gets the annotation and the header comment lists the others.

### Header changes

- `vm_commands.h`: `CVirtualMachineCmdImplementer` (vtable 0x007450AC, all
  pure) and `CSWVirtualMachineCommands` are now real polymorphic classes
  (vtable 0x007450E4). `m_pCommands` (+0x0c) is a PMF array declared with
  `class __single_inheritance` so the PMF is 4 bytes. Slot names follow
  j/vm.h, except slot 0x1c, now `GetEqualGameDefinedStructure`.
- New `game_effect.h`: the one shared `CGameEffect`. `sws_creature.h` and
  `vm_effect.h` both include it, so VM sources can now include
  `sws_creature.h` and `vm_effect.h` together.
- `CSWSObject` must stay data-free in `sws_creature.h`, because
  `CSWSItem`/`CSWSCreature` declare their members from +0xf4. Moving
  +0xf4..+0x148 into `CSWSObject` regressed 36 CSWSItem/party functions.
  Use a TU-local view struct instead (`CSWSObjectLinkView` in
  vm_commands_apply.cpp).
- Area +0x208 is `CSWSAmbientSound*` (vtable 0x0074C268, no destructor
  slot: PlayMusic..SetAmbientNightTrack, 9 virtuals).
- `CSWRules` +0xab `m_nNumSkills`; `CSWSpellArray` +0 `m_nNumSpells`;
  `CSWSCreatureStats` +0x17e `m_nGoodEvil`; `CGameEffect` +0x0c
  `m_fDuration`, +0x18 `m_oidCreator`.

### New files

`vm_commands_engine.cpp` holds the destructor, `RunCommand`,
`RunScriptStarted` and the Create/Copy/Equal/Save/Load GameDefinedStructure
handlers. `vm_commands_action.cpp` holds the `Action*` handlers, AssignCommand
and the cutscene, feet-to-meters and autosave commands.
`vm_commands_party.cpp` holds the NPC/party/alignment/reputation/faction
and ambient-sound commands. `vm_commands_apply.cpp` holds ApplyEffectAt*.
The new effect builders are in `vm_commands_effect.cpp`.

### Exact matches

| Range | Functions |
| --- | --- |
| 0x0052C070-0x0052C580 | dtor, RunCommand, RunScriptStarted, Copy/Save/Load/Equal/Create structure |
| 0x0052C6A0-0x0052D520 | ActionBarkString, DoCommand, EquipMostDamagingMelee/Ranged, EquipMostEffectiveArmor, GiveItem/TakeItem, FollowLeader, InteractObject, JumpToObject, JumpToLocation, Lock/UnlockObject, PauseConversation, RandomWalk, ResumeConversation |
| 0x0052D8F0-0x0052E1A0 | ActionUseFeat, ActionUseSkill, AddAvailableNPCByTemplate, AddMultiClass, AddToParty, AdjustAlignment, AdjustReputation |
| 0x0052E270-0x0052F4F0 | AmbientSound*, AssignCommand, ActionAttack, CutsceneAttack, CutsceneMove, ChangeFaction/ChangeToStandardFaction, ClearAllActions, ActionCloseDoor |
| 0x005302F0, 0x00530660 | FeetToMeters/YardsToMeters, DoSinglePlayerAutoSave |
| 0x00530680-0x005310E0 | EffectAbility, AC and Attack Decrease/Increase |
| 0x005319A0-0x00533FA0 | EffectConcealment, DamageDecrease/Increase, DamageImmunityIncrease, DamageReduction, DamageResistance, ForceShield, DamageShield, ForceDrain, Heal, HitPointChangeWhenDying, LinkEffects, MissChance, ModifyAttacks, MovementSpeedDecrease/Increase, Poison, SeeInvisible, SpellImmunity, SpellLevelAbsorption |

### Kept NONMATCH (do not retry immediately)

- AddAvailableNPCByObject 0x0052DC40 (9), AddPartyMember 0x0052DE70 (10),
  EffectSkillDecrease 0x00533B90 (15), EffectSkillIncrease 0x00533D00 (15).
  The shared pop-fail block sits after the FIRST pop in the original, and
  the second pop jumps back to it. Separate ifs, `||`, scoped locals,
  nesting and even a `goto` into the first block all put it after the
  second pop. This is the same tie as SavingThrow 0x005421E0. Most two-pop
  handlers do match with separate ifs, so something else decides it.
- ActionMoveAwayFromLocation 0x0052D090 (27): the original shares vDiff's
  slot with vFace (0x44 frame) and keeps pObject in edi; ours keeps the
  constant 1 in edi. An inline `MagnitudeSquared()` fixes the frame but is
  not inlined (it emits a call).
- ActionUseTalent 0x0052DAF0 (41): pure register permutation (pTalent and
  the byte fields).
- ApplyEffectAtLocation 0x0052E430 (23): effect and location stack slots
  are swapped. All 24 declaration orders give the same result.
- ApplyEffectToObject 0x0052E570 (40): the original loads pEffect straight
  into esi. Typed pops reload it from the stack every time and copy the
  float with fld/fstp.

### Lessons

- A dead `p = NULL;` after `delete p` survives when `&p` was passed to a
  pop, and it decides stack-slot sharing (ActionJumpToLocation).
- Fail paths that `delete` with no destructor call mean the pointer was
  `void*` (ApplyEffectAtLocation). Popping into `void*` and casting
  afterwards gives the integer copy of float members.
- Two calls that differ only in one argument get tail-merged.
  `if (c) f(0, 3); else f(0, 2);` gives the `push 3 / push 2` form, where a
  ternary gives setg.
- If `nParameters != 1` jumps to the fail block, write
  `if (nParameters != 1) return ERR;` as a separate if before the pop
  (EffectHeal, MovementSpeed*).
- EffectLinkEffects returns the overflow code (-2000) on pop failure.
- After moving members between shared classes, always run the whole folder
  before committing.

## Next in-range queue (verify starts in the disassembly first)

These handler starts are still unannotated. A csv extent may hide several
of them.

- Large or EH handlers: 0x0052D5B0 ActionStartConversation,
  0x0052E9F0 BeginConversation,
  0x0052EE50 ActionCastSpellAtObject/AtLocation/Fake*,
  0x0052F820 CreateObject, 0x0052FE30 DelayCommand,
  0x0052FF20 DestroyObject, 0x00530170 DisplayFeedBackText,
  0x00530360 DoDoorAction/DoPlaceableObjectAction,
  0x00530CE0 EffectAreaOfEffect, 0x005313F0 (one handler for about 20
  argument-less effects), 0x00531AE0 EffectDamage.
- 0x00534000-0x0054C960: about 300 aligned starts that `status.csv` lacks.
  Use the routine-id map and vm_action_names.inc. The Effect*/Get*/Set*
  leaves continue the patterns above. Known open items from earlier rounds:
  CreateItemOnFloor 0x00547F60, GetItemPossessedBy 0x0053A390,
  SpeakString 0x00544090, StartNewModule 0x00544390,
  SetGlobalString 0x00542C60, SetEffectIcon 0x005475A0.
- Revisit: SavingThrow 0x005421E0 and the four pop-layout misses above,
  all together, once the layout rule is understood.
