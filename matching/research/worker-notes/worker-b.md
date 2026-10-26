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

## Round s3 (2026-10-26): VM Get* queries

Folder `kotor-match -q -j 2 matching/src/b`: 558/633 -> 611/679 MATCH.
46 functions annotated, 45 new exact, 1 kept as NONMATCH, and 8 prior
NONMATCHes promoted. No prior MATCH regressed.

### The pop-fail layout rule (solved)

When a handler pops two or more arguments, the original puts the shared
`return -2001` block right after the FIRST pop and later pops jump back to
it. That layout comes from the final push written as

    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;

with each pop in its own `if (...) return ...;`. The ternary
`return Push(x) ? 0 : -2000;` (same neg/sbb bytes at the end) puts the merged
block after the LAST pop instead. One-pop handlers match with either form.
Use `jne` after the first pop plus `je <back>` after the second as the cue.
This promoted AddAvailableNPCByObject 0x0052DC40, AddPartyMember 0x0052DE70,
GetAbilityScore 0x005361B0, SavingThrow 0x005421E0 and EffectSkillDecrease/
Increase 0x00533B90/0x00533D00.

### Other lessons

- `mov eax,[obj+0xa74]; push args; mov ecx,eax; call` means the stats
  pointer is a named local (`CSWSCreatureStats* pStats = ...->m_pStats;`).
  Writing `p->m_pStats->F(x)` loads it straight into ecx. This promoted
  GetAbilityModifier 0x00536310 and matched GetHasFeat/Skill/Spell and
  GetHitDice.
- `mov eax,[x]; mov ecx,[vm]; push eax` (argument computed before the VM
  pointer load) needs a local for the pushed value (GetAreaUnescapable
  0x005367B0, GetGameDifficulty, GetEffectSpellId).
- A call whose argument is another call's result: evaluate the inner call
  into a local first if the original calls it before loading the outer
  `this` (GetGold).
- Two identical virtual calls in an if/else get tail-merged into the form
  the original shows; a ternary on the object pointer does not
  (GetCurrentHitPoints).
- Declaration order decides which of oid/pObject gets the lower slot
  (GetHitDice declares `CGameObject* pObject` first).
- GetGoingToBeAttackedBy 0x00538FC0 swaps the two error codes in the original.

### Header changes

- `sws_creature.h`: `CSWSFaction` methods 0x005BEA80-0x005BF6E0 and its
  +0x14 member cursor; stats +0x78 `m_nFactionId`, +0x124/+0x126 force
  points, GetFort/Will/ReflexSavingThrow, GetFeatRemainingUses, HasSpell,
  GetEffectImmunity; creature +0x520 `m_oidGoingToBeAttackedBy`, +0xa24
  `m_oidEncounter`, `GetMaxForcePoints` 0x004FD490 (bucket a calls it
  GetMaxHitPoints; the VM routine is GetMaxForcePoints); `CSWSObject`
  GetHasSpellEffectApplied/GetHasFeatEffectApplied; module +0x1bc
  `m_nTimeOfDay` (1 day, 2 night, 3 dawn).
- `game_effect.h`: +0x1c `m_nSpellId`, `GetScriptEffectType` 0x00503A70.
- `server_exo_app.h`: `GetServerInfo` 0x004AEC90.
- Vtable slot 0x98 is still named `GetCreatureSize` (used by sws_player.cpp);
  buckets a/k call it GetMaxHitPoints, and the VM uses it that way.

### New files and exact matches

| File | Functions |
| --- | --- |
| vm_commands_faction.cpp | GetFactionAverageGoodEvilAlignment/Level/Reputation/XP, BestAC, Equal, Gold, LeastDamagedMember, Leader, MostDamagedMember, MostFrequentClass, StrongestMember, WeakestMember, WorstAC (0x005380F0-0x00538B10) |
| vm_commands_effect_get.cpp | GetEffectCreator/DurationType/SpellId/SubType/Type (0x00537AA0-0x00537C70) |
| vm_commands_save.cpp | GetFortitude/Reflex/WillSavingThrow (0x00538BE0, 0x0053CE10, 0x0053E540), GetGameDifficulty, GetGoingToBeAttackedBy, GetGold, GetGoldPieceValue |
| vm_commands_has.cpp | GetHasFeat, GetHasFeatEffect, GetHasInventory, GetHasSkill, GetHasSpell, GetHasSpellEffect (0x00539170-0x005394D0) |
| vm_commands_hp.cpp | GetHitDice, GetCurrentHitPoints (+Max), GetCurrent/MaxForcePoints, GetIdentified, GetInventoryDisturbItem/Type, GetIsDawn/Day/Night, GetIsEncounterCreature, GetIsImmune, GetIsInCombat |

`functions.csv` misses several of these starts (0x00538BE0 and 0x00538CA0
sit inside the 0x00538B10 extent); they are annotated anyway.

### Kept NONMATCH

- GetFirstFactionMember 0x005386F0 (12): the original keeps g_pAppManager in
  ecx and reloads the server pointer for GetFactionOfObject; ours keeps the
  server pointer. Helper, inline condition and switch forms all tried.
- Untouched older misses: GetGoodEvilValue 0x005364F0 (11, pStats register
  choice before the switch), GetAC, GetArea.

## Round s4 (2026-10-26)

Ran out at the usage limit before writing notes; its commits (838158c and
earlier "WIP b s4") were merged into main. Folder start of s5: 667/737.

## Round s5 (2026-10-26): small leaf VM handlers 0x005424E0-0x00547EF0

Folder `kotor-match -q -j 2 matching/src/b`: 667/737 -> 753/824 MATCH.
87 functions annotated, 86 new exact, 1 kept as NONMATCH. No prior MATCH
regressed.

### How the work was found

A scratch script lists every handler address from
`src/units/1360/3e4c4bbc2c-vm_action_names.inc` that no `matching/src`
file annotates (about 200 remained at the start of s5). The gap to the next
handler gives the size. All s5 picks were such starts; `functions.csv`
lacks most of them (they sit inside larger csv extents).

### New files and exact matches

| File | Functions |
| --- | --- |
| vm_commands_set_sound.cpp | SetAreaUnescapable, SetCameraMode, SetCommandable, SetLightsaberPowered, SetListening, SetNPCSelectability, SetPlotFlag, SetPlayerRestrictMode, SetReturnStrref, SWMG_SetSpeedBlurEffect, SetForcePowerUnsuccessful, SetXP, ShipBuild, ShowLevelUpGUI, ShowUpgradeScreen, SoundObject* (11), StopRumblePattern, SwitchPlayerCharacter (0x005424E0-0x00544910) |
| vm_commands_set_item.cpp | SetIdentified, SetLocked, SetPlaceableIllumination, GetItemStackSize 0x005465D0, SetItemNonEquippable |
| vm_commands_late.cpp | Get/SetPartyAIStyle, Get/SetNPCAIStyle, ClearAllEffects, GetStandardFaction, Get/SetMinOneHP, SetGlobalFadeIn/Out, SetFormation, GetIsDebilitated, GetSubScreenID, CancelCombat, PauseGame, ShowGalaxyMap, planets (5), ButtonMashCheck (2), SetDialogPlaceableCamera, GetSoloMode, stealth XP (9), GetSubRace, GetBlockingCreature, GetFoundEnemyCreature, SetLockOrientation/HeadFollowInDialog, Enable/DisableVideoEffect, SetTutorialWindowsEnabled, ShowTutorialWindow, IsCreditSequenceInProgress, ResetDialogState, SetGoodEvilValue, GetIsPoisoned, SetSoloMode, CancelPostDialogCharacterSwitch, SetMaxHitPoints, NoClicksFor, HoldWorldFadeInForDialog, SuppressStatusSummaryEntry, GetCheatCode (0x005459A0-0x00547E90) |

Renamed: 0x0053C220 was annotated as `ExecuteCommandGetItemStackSize`
(name guessed); vm_action_names.inc calls it GetNumStackedItems, and the
real GetItemStackSize is 0x005465D0. It is now
`ExecuteCommandGetNumStackedItems` (bytes unchanged, symbols.csv will move).

### Kept NONMATCH

- SetMusicVolume 0x00547EF0 (6): the original puts the `fVolume = 0` block
  after the return and jumps back. Both compares match with
  `0.0f > fVolume` and `!(fVolume < 1.0f)`; if/else, ternary and nested
  forms all keep the block inline.

### Lessons

- `mov esi,[value]` loaded before a call whose result gets the store
  (`push esi; mov esi,[esp+4]; call Get...; mov [eax+off],esi`) means an
  inline setter: write `GetX()->SetY(v)` with `void SetY(int v) { m_y = v; }`
  (SetLocked on door/placeable, SetPartyAIStyle, bitfield
  SetTutorialWindowsEnabled). The argument is evaluated before `this`.
- A `mov [esp+N],0` right after the first pop's `push eax` initialises the
  SECOND popped variable, not the first. Check which slot it hits before
  choosing which local gets `= 0` (SetLocked, SetNPCAIStyle).
- A pop target with no initialiser whose slot is pushed on the failure path
  (`mov eax,[esp]` = the oid slot) is an uninitialised result variable that
  shares the oid's slot (SoundObjectGetVolume, GetSubRace, GetMinOneHP).
- A value kept in esi across the object lookup and copied only inside the
  `if (n >= 0)` block needs a block-local copy (`int nNewXP = nXP;`, SetXP).
- One `GetArea()` call followed by a member read means a local
  `CSWSArea* pArea = p->GetArea(); if (pArea)` (stealth XP handlers).
- Duplicated push paths for early exits (GetStandardFaction) come from
  separate `return Push(x) ? 0 : -2000;` statements per exit.
- Float-to-int argument computed before the `this` chain: put it in a local
  first (NoClicksFor, SoundObjectFadeAndStop).
- Several late handlers return -2000 on pop failure (SetSoloMode,
  SetMaxHitPoints, NoClicksFor, SuppressStatusSummaryEntry, GetCheatCode,
  SetMusicVolume). SetForcePowerUnsuccessful pops its int with
  StackPopObject.
- `sws_item.h` cannot be included together with `sws_message.h` or
  `party_table.h` (duplicate CSWSMessage/CSWSItem); keep item handlers in
  their own TU.

### Header changes

- `sws_creature.h`: stats +0xe8 `m_nSubRace`, +0x13a `m_nAIStyle`; area
  +0x2b4..+0x2c0 stealth XP fields, SetStealthXPMax/Current,
  AwardStealthXP 0x00508770 (name guessed), SetPlayerRestrictMode;
  CSWSObject::SetCurrentHitPoints; creature CancelCombat, SetFormation
  0x00514700 (name guessed), IsDebilitated, +0x9e0 `m_bPoisoned`.
- `server_exo_app.h`: GetSoundObjectByGameObjectID, Get/SetButtonMashCheck,
  client StopRumblePattern (returns int: the VM pushes it),
  SetPausedByCombat, Enable/DisableVideoEffect, ShowTutorialWindow,
  GetCreditSequenceInProgress; CClientOptions +0x14 bitfield.
- `sws_door.h`/`sws_item.h`: inline SetLocked setters, SetLightIsOn.
- `party_table.h`: inline SetPartyAIStyle (+0xe4).

## Round s6 (2026-10-26): remaining leaf and mid-size VM handlers

Folder `kotor-match -q -j 2 matching/src/b`: 753/824 -> 815/892 MATCH.
68 functions annotated, 62 new exact, 6 kept as NONMATCH. No prior MATCH
regressed. Work list: the s5 scratch scan of vm_action_names.inc handler
starts without an annotation (most are missing from `functions.csv`).

### New files and exact matches

| File | Functions |
| --- | --- |
| vm_commands_small.cpp (new) | GetChallengeRating, GetCommandable, GetCreatureSize, MusicBackgroundGetDayTrack, ActionPickUpItem, PrintInteger, RemoveAvailableNPC, RemoveFromParty, GetFirstAttacker, GetDamageDealtByType, GetEncounterActive (4 routines), MusicBattlePlay (3), ActionOpenDoor, MagicalEffect (3), SpawnAvailableNPC, Vector, VectorMagnitude, GetCreatureHasTalent, RemovePartyMember, SetAreaTransitionBMP, SetEncounterActive (4), RevealMap, SetAreaFogColor, SetTime, ShowPartySelectionGUI, ActionSurrenderToEnemies (3), VectorNormalize, GetIsLinkImmune, GetGlobalString, SetGlobalString, ObjectToString, GivePlotXP, MusicBackgroundPlay (5), GetIsLiveContentAvailable, SetCameraFacing, StringToInt/StringToFloat, RemoveEffect |
| vm_commands_journal.cpp (new) | AddJournalWorldEntryStrref, DeleteJournalWorldAllEntries/Entry/EntryStrref, SetMapPinEnabled |
| vm_commands_distance.cpp (new) | GetDistanceBetween(2D), GetDistanceBetweenLocations(2D), GetDistanceToObject(2D), GetFacing, AngleToVector/VectorToAngle, PrintFloat, FaceObjectAwayFromObject |
| vm_commands_effect.cpp | EffectForceResistanceDecrease/Increase, EffectCharmed (0x534340), EffectTemporaryHitpoints |
| vm_commands_set_item.cpp | SignalEvent, ActionUnequipItem |
| vm_commands_action.cpp | DelayCommand, ActionWait |
| vm_commands_last.cpp | GetEnteringObject/GetClickingObject 0x00537EE0 |
| vm_commands_object.cpp | GetJournalEntry |

GetEnteringObject ends in a jump table and 0x00537FE0 (GetFacing) is not
in `functions.csv`; it only counts as MATCH once GetFacing is annotated
(matchcmp then sees the next start).

### Kept NONMATCH

- GiveItem 0x0053E830 (19): register choice (&pItem and the base item
  index in eax in the original, ecx/edx in ours).
- ActionSpeakStringByStrRef 0x00544270 (8): the original materialises the
  GetGameObject result as an int (sub/neg/sbb/inc) and compares it with 1;
  cl folds every `(x == VALID) == 1` form tried (local, inline helper).
- GetClassByPosition 0x00536CE0 (13) and GetIsEnemy 0x00537CE0 (16):
  block placement. The original keeps the pop-failure block right after
  the first pop and the second pop jumps back to it; cl always merges it
  after the second pop (separate ifs and `||` both tried).
- DuplicateHeadAppearance 0x00546F90 (34): the 2DA pointer is loaded into
  eax and the column temporary's address is re-taken with lea; ours loads
  `this` straight into ecx (temporary, named local and inline accessor
  tried).
- ActionMoveAwayFromObject 0x0053F990 (21): esi/edi swap only.

### Lessons

- Two pops: `if (!a || !b) return` puts the failure block at the end of
  the function; two separate `if (!x) return` statements give the
  original's `je fail / jne ok / fail:` layout (PrintInteger,
  SpawnAvailableNPC, AddJournalWorldEntryStrref, MusicBattlePlay).
- A value loaded into a register before an unrelated call (SignalEvent's
  event pointer in ebx) needs a local copy right after the pops.
- A stats method called with arguments read from another object
  (GetHasTalent, GetIsLinkImmune) needs `CSWSCreatureStats* pStats =
  ...->m_pStats;` first, so `this` is loaded before the arguments.
- x87 sums: `y*y + x*x + z*z` compiles to the z, x, y load order seen in
  FaceObjectAwayFromObject. `(float)atan2(y, x) * 57.29578f` keeps the
  dword fmul; without the cast the constant becomes a qword.
- Inline vector subtraction `Vector(a.x - b.x, a.y - b.y, a.z - b.z)`
  computes z, y, x (GetDistanceBetween, GetDistanceBetweenLocations); a 2D
  delta is three member assignments. 0x00423BC0 is the out-of-line copy
  (FaceObjectAwayFromObject calls it), so the TU uses a local helper.
- Conditional format strings (PrintFloat) are four Format calls in
  if/else; cl merges the tails. A ternary gives mov/cmov-style code.
- A temporary passed to a push whose result is tested after the
  destructor (ObjectToString) needs a scoped named copy and a bool.
- `delete p` on a location after a pop failure: write `if (p) { delete p; }`
  to get `test; je; mov esi`.
- Unused uninitialised locals share a dead slot: GetCommandable's result
  shares pObject's slot, GetEncounterActive's shares nParameters.

### Header changes

- `sws_creature.h`: CGameObject +0x8c `m_oidArea`; stats +0x84
  `m_fChallengeRating`, GetHasTalent, GetIsLinkImmune, byte-argument
  GetClass overload, GetClassLevel; CSWSFaction::RemoveMember; creature
  AddPickUpItemActions, GetFirst/NextAttacker, Surrender,
  AddUnequipActions; object AddOpenDoorAction; area SetSunFogColor,
  GetClientArea, +0x1d4 `m_oidEnteringObject`; module +0x14c
  `m_oidEnteringObject`, +0x218 `m_pAreaMap`, world journal methods,
  UpdateTime.
- `server_exo_app.h`: GetEncounterByGameObjectID 0x004AE7F0,
  GetWaypointByGameObjectID 0x004AE7E0, CWorldTimer::AdvanceToTime.
- `sw_rules.h`: CTwoDimArrays +0xc `m_pAppearanceTable`.
- vm_commands_last.cpp local views: door/placeable/AoE/trigger/0x5c
  entering-object fields, placeable +0x340 `m_bHostile` (name guessed).

## Next in-range queue (verify starts in the disassembly first)

Unannotated handler starts from vm_action_names.inc (s6 scan, size from
the gap to the next handler):

- Small/medium: 0x00541A30 RandomName (272, EH, 0x711290 class),
  0x00549040 GetCurrentAction (320, action-queue loop), 0x00542DC0
  SetIsDestroyable (336), 0x00530170 DisplayFeedBackText, 0x00546630
  SetItemStackSize, 0x00548900 BarkString, 0x00536AD0 GetCasterLevel,
  0x0053F220 GetItemActivated* and 0x0053F800 GetLastItemEquipped* (switch
  layouts), 0x00546340 SurrenderByFaction, 0x00536000 FloatToString (like
  PrintFloat plus a second Format), 0x0053FE00 ActionMoveToLocation,
  0x00541320 PopUpGUIPanel, 0x00548640 AddJournalWorldEntry.
- Larger: 0x005475A0 SetEffectIcon, 0x0054A090 GetNearestTrapToObject,
  0x0053CC30 GetReflexAdjustedDamage, 0x00544090 SpeakString,
  0x00545A70 SetPartyLeader, 0x00549180 GetFirst/NextEffect, 0x00540300
  OpenStore, 0x00542830 SetFacing, 0x00547F60 CreateItemOnFloor,
  0x0053A390 GetItemPossessedBy, 0x0052FF20 DestroyObject, and the EH
  handlers listed under s3.
- Known open NONMATCHes: see the s5 and s6 lists above, plus
  GetFirstFactionMember, GetGoodEvilValue, GetAC, GetArea,
  GetLastHostileActor, SetMusicVolume.

## s7 recovery after usage interruption

Recovered the two implemented mid-size handlers and one fully implemented
SetIsDestroyable handler from the interrupted B worktree. Discarded the
unimplemented placeholder stubs. Three functions attempted: 0x00541A30
RandomName and 0x00542DC0 SetIsDestroyable are MATCH; 0x00549040
GetCurrentAction remains NONMATCH (10 differing instructions: list head
register assignment). Existing B baseline: 815 MATCH / 892 annotated.

Name audit: the vtable slot +0x94 in the B CGameObject view had been named
GetIsInConversation without sufficient evidence. Other recovered creature
uses identify it as a dead-state query, while the slot is shared across
object types, so it is now neutral GetObjectState94. Native SetIsDestroyable
uses that slot for the dead creature test. Confirmed the original's event
constant at 0x007441E0 is 11 and object invalid value is 0x7F000000.

The former GetIsActionTimed at 0x0057A240 is now GetAcceptableAction, the
Mac debug map's 2/2 name; this changes the decorated symbol and should be
accounted for in the coordinator's symbol audit. RandomName's callees at
0x00711340 and 0x00711090 are descriptively named LoadNameTable and
UnloadNameTable; original caller addresses and the literal "names" were
checked against the executable. This recovered batch contains no guessed
return-zero stubs. The next queue is the s6 list above, excluding these
three addresses; recheck current status before annotating.

## Follow-up VM journal and format handlers

Screened 100 current todo candidates (the report is based on the baseline
status and therefore still includes the already recovered RandomName).
Attempted two related VM handlers: 0x00548640 AddJournalWorldEntry MATCH,
0x00536000 FloatToString NONMATCH (17 instruction differences, frame and
register allocation). Native float format literals were read from the exe:
"%%%1d.%02df\\n" and "%%%2d.%02df\\n". Separate early returns in the
journal pop checks reproduced the native failure block and exact code.
Both functions retain their normal stack underflow and overflow contracts.

## Expanded VM context and item attempts

Four further candidates retained after two or three compiler shapes each:
0x0053F220 GetItemActivated NONMATCH 22 (register and branch placement),
0x0053F800 GetLastItemEquipped shared module handler NONMATCH 32 (branch
and location-copy register choice), 0x0053FE00 ActionMoveToLocation
NONMATCH 27 (stack and call block placement), and 0x00546630
SetItemStackSize NONMATCH 17 (base item/size register choice). Implemented
native behaviours including the module-null paths and movement's intentional
location ownership, preserving its script-object-validity check.

Corrected the B declaration of AddMoveToPointAction at 0x004F8B60 to the
signature in the existing native reconstructed definition: int return and
two float parameters after the run flag. The prior B declaration incorrectly
typed the second float as an int. The old movement caller and the new caller
were rechecked against native code; coordinator should treat the decorated
symbol replacement as intentional when auditing symbols.

A fifth candidate, 0x00541320 PopUpGUIPanel / PopUpDeathGUIPanel,
MATCHes exactly (432 bytes). The original initializes panel, respawn,
wait and string-reference locals in that order; the matching source uses
that declaration order. Both command IDs preserve their original pop and
message behavior. SetItemStackSize's best compiler shape initializes the
size local to zero, as the native stack stores do.

## VM effects, bark, feedback and faction

Four further handlers attempted: 0x005475A0 SetEffectIcon MATCH (448 bytes),
0x00546340 SurrenderByFaction NONMATCH 10 (callee-saved register timing),
0x00548900 BarkString NONMATCH 11 (GUI state scratch register), and
0x00530170 DisplayFeedBackText NONMATCH 18 (table-call register ordering).
SetEffectIcon matches after saving the popped source effect pointer before
the integer pop. Original effect type data at 0x00746788 is 67.

Corrected the guessed B declaration at 0x0050EE80 to
SetCombatMode(unsigned char,int), Mac debug map 2/2, and its existing call
site. This is distinct from SetCombatState(int,unsigned char) at 0x004F2610.
Surrender uses an out-of-line GetFactionManager declaration because its
original call targets 0x004AEFC0; a local preprocessor guard preserves
existing inline usage in the rest of B. BarkString's 0x0062AB30 callee
uses the two CExoString/CResRef references indicated by Mac names (2/2);
existing source in C has provisional integer parameters and needs later
cross-range signature reconciliation. Compiler byte matching alone masks
these call relocations.

## Journal array and server-admin strings

Three exact functions recovered: 0x00527490
CExoArrayList<CSWSPlayerJournalQuestUpdates>::Allocate (208 bytes),
0x00579920 SendServerToServerAdminServerStatus (352 bytes), and
0x00579A80 SendServerToServerAdminPlayerList (208 bytes). The array's
generated element constructor was separately compiled and compared to its
native relocation target 0x0041D910: 16/16 bytes MATCH; its destructor is
the existing 0x0059EA20. Native admin string pointers 0x007A1F14,
0x007A1F1C, 0x007A1F20, 0x007A1F24, and 0x007A1F3C hold ServerStatus,
ModuleLoaded, Idle, ModuleRunning, and PlayerList, respectively. The format
strings and GetModuleName / GetPlayerListString callees were checked against
the executable and Mac debug names.

## Quick chat and bark signature reconciliation

0x00571EC0 SendServerToPlayerQuickChatMessage is implemented as NONMATCH
(24 instruction differences, register and stack allocation); the previous
QuickChatMessage name was replaced with the Mac debug name (2/2) in the B
header and caller. The hearing-range constant at 0x00747444 is 1000.0f.

The BarkString caller now uses the CGuiInGame class name for 0x0062AB30
and the Mac-supported ShowBarkBubble(unsigned long, CExoString const&,
CResRef const&, int) arguments. The C-track auditor independently confirmed
that direct callers do not establish a non-void return, so this call uses
void. Its compile result remains 11 differences.

0x0056AC20 is a localized-text validity check with player, default, module,
and first-entry language fallbacks. It remains NONMATCH (34 differences,
register and branch placement). HasValidLocalizedString is a descriptive
name, since no Mac name is available; the callee types follow existing
matched definitions and the player-language Mac debug name. Do not treat its
public symbol or gender parameter type as independently established.

## Rules cleanup

0x00551440 CSWRules::~CSWRules MATCH (416 bytes). A TU-local partial
class preserves the existing B shared-header layouts while representing the
native cleanup pointers through +0xb8. It uses typed deletes and delete[]
in native order. The generated CSWSkill destructor was independently
compared at its native 0x0059D7C0 relocation target and MATCHes 16 bytes.
The feat destructor is declared external at 0x004015A0, a bare ret; class,
race, spell-array, base-item-array, and 2DA destructor callees use the
existing native symbols/Mac names. The data pointers +0x94..+0xa0 have
neutral names pending type evidence.

## Store and remaining B pool, s8

Screened 107 remaining indexed functions plus seven previously unindexed VM
entry points; the remaining candidates are predominantly 368 bytes or larger.
0x00540300 ExecuteCommandOpenStore MATCH, 496/496 bytes. The caller pops the
store and player IDs and two optional modifiers, clamps both to [-100,100],
opens the GUI, and queues event 0x16 with the original 10-tick delta. Mac
symbols support GetClientObjectByObjectId, ShowStoreGui, and
AddEventDeltaTime; the event code and delta were independently checked.
Compiler lesson: success-gated short circuit places the shared pop-error
return last; expressing the clamp as `if (x <= 100) { if (x < -100) ... }
else ...` produces the native upper-branch placement. This is one new exact
attempt; no existing functions were counted as new.

0x00547F60 CreateItemOnFloor retained NONMATCH (30 differences); implemented
the native location-pop, item creation, area insertion and push failure paths.
Frame-slot and failure-block placement remain different after three shapes.
Corrected the previously provisional second argument of
CSWSMessage::SendServerToPlayerItemScriptAdd at 0x0056B670 from integer to
CSWSArea*, as supported by the Mac debug map (2/2) and native call at
0x005480E6. No other B call sites used the old declaration.

0x00553200 MATCH, 16/16 bytes, previously absent from the coarse function
index. Native vtable 0x00748CC0 has this entry at +0x8C (the GetFirstName
slot). Its receiver is the CSWSItem's secondary CSWSObject base at +0x10;
the native +0x270 pointer therefore addresses item +0x280, the existing
localized-name field. CSWSItemSecondaryView and GetLocalizedName are
explicitly descriptive source names for that secondary-base view, not Mac
symbol claims. Candidate found by scanning aligned executable pointers in
read-only data after screening the indexed pool.
