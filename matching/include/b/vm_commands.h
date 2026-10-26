// Worker B matching header: CSWVirtualMachineCommands (NWScript engine
// command implementations, 0x0052xxxx-0x0054xxxx) and the CVirtualMachine
// facade it calls (0x005D1000-0x005D1110, worker J).
#ifndef B_VM_COMMANDS_H
#define B_VM_COMMANDS_H

#include "b/exo_string.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"
#include "b/vector.h"

#define VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW  (-2000)
#define VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW (-2001)


class CVirtualMachine {
public:
    int StackPopInteger(int* nValue);                       // 0x005D1000
    int StackPushInteger(int nValue);                       // 0x005D1010
    int StackPopFloat(float* fValue);                       // 0x005D1020
    int StackPushFloat(float fValue);                       // 0x005D1030
    int StackPopVector(Vector* vValue);                   // 0x005D1040
    int StackPushVector(Vector vValue);                   // 0x005D1050
    int StackPopString(CExoString* sValue);                 // 0x005D1080
    int StackPushString(const CExoString& sValue);          // 0x005D1090
    int StackPopEngineStructure(int nType, void** pValue);  // 0x005D10A0
    int StackPushEngineStructure(int nType, void* pValue);  // 0x005D10B0
    int StackPopObject(unsigned long* oidValue);            // 0x005D10C0
    int StackPushObject(unsigned long oidValue);            // 0x005D10D0
    int StackPopCommand(class CVirtualMachineScript** ppScript); // 0x005D10E0
    int RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid); // 0x005D0FC0

    void* m_pInternal;                                      // 0x00
};

extern CVirtualMachine* g_pVirtualMachine;                  // 0x007A3A00

#define ENGINE_STRUCTURE_EFFECT 0
#define ENGINE_STRUCTURE_EVENT  1
#define ENGINE_STRUCTURE_LOCATION 2
#define ENGINE_STRUCTURE_TALENT 3

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C (0)

class CResGFF;
struct CResStruct;

// Abstract command implementer the virtual machine calls back into
// (vtable 0x007450AC: every slot but the destructor is _purecall).
class CVirtualMachineCmdImplementer {
public:
    virtual ~CVirtualMachineCmdImplementer() {}             // 0x00
    virtual void InitializeCommands() = 0;                  // 0x04
    virtual int RunCommand(int nCommandId, int nParameters) = 0; // 0x08
    virtual void RunScriptStarted(CExoString* sScriptFile) = 0; // 0x0c
    virtual void ReportError(CExoString* sScriptFile, int nError) = 0; // 0x10
    virtual void* CreateGameDefinedStructure(int nType) = 0; // 0x14
    virtual void DestroyGameDefinedStructure(int nType, void* pStructure) = 0; // 0x18
    virtual int GetEqualGameDefinedStructure(int nType, void* pStructure1, void* pStructure2) = 0; // 0x1c
    virtual void* CopyGameDefinedStructure(int nType, void* pStructure) = 0; // 0x20
    virtual int SaveGameDefinedStructure(int nType, void* pStructure, CResGFF* pRes, CResStruct* pStruct) = 0; // 0x24
    virtual int LoadGameDefinedStructure(int nType, void** ppStructure, CResGFF* pRes, CResStruct* pStruct) = 0; // 0x28

    int m_bValidObjectRunScript;                            // 0x04
    unsigned long m_oidObjectRunScript;                     // 0x08
};

class __single_inheritance CSWVirtualMachineCommands;
typedef int (CSWVirtualMachineCommands::*PFN_VM_COMMAND)(int nCommandId, int nParameters);

// vtable 0x007450E4.
class CSWVirtualMachineCommands : public CVirtualMachineCmdImplementer {
public:
    virtual ~CSWVirtualMachineCommands();                   // 0x0052C070
    virtual void InitializeCommands();                      // 0x0054C960
    virtual int RunCommand(int nCommandId, int nParameters); // 0x0052C0D0
    virtual void RunScriptStarted(CExoString* sScriptFile); // 0x0052C100
    virtual void ReportError(CExoString* sScriptFile, int nError); // 0x005B5E90 (shared empty function)
    virtual void* CreateGameDefinedStructure(int nType);    // 0x0052C580
    virtual void DestroyGameDefinedStructure(int nType, void* pStructure); // 0x00548360
    virtual int GetEqualGameDefinedStructure(int nType, void* pStructure1, void* pStructure2); // 0x0052C510
    virtual void* CopyGameDefinedStructure(int nType, void* pStructure); // 0x0052C140
    virtual int SaveGameDefinedStructure(int nType, void* pStructure, CResGFF* pRes, CResStruct* pStruct); // 0x0052C2D0
    virtual int LoadGameDefinedStructure(int nType, void** ppStructure, CResGFF* pRes, CResStruct* pStruct); // 0x0052C360

    PFN_VM_COMMAND* m_pCommands;                            // 0x0c (indexed by command id, 0x304 entries)

    int ExecuteCommandGetStringLength(int nCommandId, int nParameters);
    int ExecuteCommandGetStringLowerCase(int nCommandId, int nParameters);
    int ExecuteCommandGetStringUpperCase(int nCommandId, int nParameters);
    int ExecuteCommandGetStringLeft(int nCommandId, int nParameters);
    int ExecuteCommandGetStringRight(int nCommandId, int nParameters);
    int ExecuteCommandGetSubString(int nCommandId, int nParameters);
    int ExecuteCommandInsertString(int nCommandId, int nParameters);
    int ExecuteCommandFindSubString(int nCommandId, int nParameters);
    int ExecuteCommandIntToHexString(int nCommandId, int nParameters);
    int ExecuteCommandTestStringAgainstPattern(int nCommandId, int nParameters);
    int ExecuteCommandGetMatchedSubstring(int nCommandId, int nParameters);
    int ExecuteCommandEffectDroidStun(int nCommandId, int nParameters);
    int ExecuteCommandEffectChoke(int nCommandId, int nParameters);
    int ExecuteCommandEffectBodyFuel(int nCommandId, int nParameters);
    int ExecuteCommandEffectPsychicStatic(int nCommandId, int nParameters);
    int ExecuteCommandEffectForcePushed(int nCommandId, int nParameters);
    int ExecuteCommandEffectTrueSeeing(int nCommandId, int nParameters);
    int ExecuteCommandEventConversation(int nCommandId, int nParameters);
    int ExecuteCommandGetDifficultyModifier(int nCommandId, int nParameters);
    int ExecuteCommandEventUserDefined(int nCommandId, int nParameters);
    int ExecuteCommandEffectResurrection(int nCommandId, int nParameters);
    int ExecuteCommandEffectForceJump(int nCommandId, int nParameters);
    int ExecuteCommandEventSpellCastAt(int nCommandId, int nParameters);
    int ExecuteCommandRollDice(int nCommandId, int nParameters);
    int ExecuteCommandActionEquipItem(int nCommandId, int nParameters); // 0x005355F0 (name guessed)
    int ExecuteCommandFloatToInt(int nCommandId, int nParameters); // 0x00535FB0 (name guessed)
    int ExecuteCommandGetCreatureStat194(int nCommandId, int nParameters); // 0x0053C010 (name guessed)
    int ExecuteCommandGetAreaValue(int nCommandId, int nParameters); // 0x0053C1B0 (name guessed)
    int ExecuteCommandGetItemStackSize(int nCommandId, int nParameters); // 0x0053C220 (name guessed)
    int ExecuteCommandPlayAreaSoundAtLocation(int nCommandId, int nParameters); // 0x00541280 (name guessed)
    int ExecuteCommandPopVectorAndInteger(int nCommandId, int nParameters); // 0x00541910 (name guessed)
    int ExecuteCommandSelfObjectAction(int nCommandId, int nParameters); // 0x00541970 (name guessed)
    int ExecuteCommandRandom(int nCommandId, int nParameters); // 0x005419D0 (name guessed)
    int ExecuteCommandSetLocal(int nCommandId, int nParameters); // 0x00543130 (name guessed)
    int ExecuteCommandQueueMovieById(int nCommandId, int nParameters); // 0x005482D0 (name guessed)
    int ExecuteCommandDoorAction(int nCommandId, int nParameters); // 0x00548310 (name guessed)
    int ExecuteCommandEffectType2F(int nCommandId, int nParameters); // 0x00532E40 (name guessed)
    int ExecuteCommandEffectType5C(int nCommandId, int nParameters); // 0x00534CD0 (name guessed)
    int ExecuteCommandEffectType5D(int nCommandId, int nParameters); // 0x00534DB0 (name guessed)
    int ExecuteCommandEffectType5F(int nCommandId, int nParameters); // 0x00534F40 (name guessed)
    int ExecuteCommandEffectType60(int nCommandId, int nParameters); // 0x00535020 (name guessed)
    int ExecuteCommandEffectType5B(int nCommandId, int nParameters); // 0x00535100 (name guessed)
    int ExecuteCommandGetTalentValue(int nCommandId, int nParameters); // 0x00544D70 (name guessed)
    int ExecuteCommandTimeToSeconds(int nCommandId, int nParameters); // 0x00544E50 (name guessed)
    int ExecuteCommandSetNPCObject(int nCommandId, int nParameters); // 0x00548180 (name guessed)
    int ExecuteCommandIsMoviePlaying(int nCommandId, int nParameters); // 0x005481E0 (name guessed)
    int ExecuteCommandIntToFloat(int nCommandId, int nParameters);     // 0x0053EAA0
    int ExecuteCommandExportAllCharacters(int nCommandId, int nParameters); // 0x00535C70 (name guessed)
    int ExecuteCommandExecuteScript(int nCommandId, int nParameters);
    int ExecuteCommandGetGlobalNumber(int nCommandId, int nParameters);
    int ExecuteCommandSetGlobalNumber(int nCommandId, int nParameters);
    int ExecuteCommandGetGlobalLocation(int nCommandId, int nParameters);
    int ExecuteCommandSetGlobalLocation(int nCommandId, int nParameters);
    int ExecuteCommandSendMessageToPC(int nCommandId, int nParameters);
    int ExecuteCommandGetStrRefSoundDuration(int nCommandId, int nParameters);
    int ExecuteCommandGetObjectByTag(int nCommandId, int nParameters);
    int ExecuteCommandStartCreditSequence(int nCommandId, int nParameters);
    int ExecuteCommandPlayRoomAnimation(int nCommandId, int nParameters);
    int ExecuteCommandEffectSavingThrowDecrease(int nCommandId, int nParameters);
    int ExecuteCommandEffectSavingThrowIncrease(int nCommandId, int nParameters);
    int ExecuteCommandEffectBeam(int nCommandId, int nParameters);
    int ExecuteCommandEffectForcePushTargeted(int nCommandId, int nParameters);
    int ExecuteCommandSaveNPCState(int nCommandId, int nParameters);   // 0x005421A0 (name guessed)
    int ExecuteCommandEffectVisualEffect(int nCommandId, int nParameters);
    int ExecuteCommandEffectDamageImmunity(int nCommandId, int nParameters);
    int ExecuteCommandSavingThrow(int nCommandId, int nParameters);
    int ExecuteCommandPopObjectAndInteger(int nCommandId, int nParameters); // 0x00541770 (name guessed)
    int ExecuteCommandStartPazaakGame(int nCommandId, int nParameters);  // 0x00540FD0 (name guessed)
    int ExecuteCommandPlayRumblePattern(int nCommandId, int nParameters); // 0x00541120 (name guessed)
    int ExecuteCommandSpeakOneLinerConversation(int nCommandId, int nParameters);
    int ExecuteCommandWriteTimestampedLogEntry(int nCommandId, int nParameters);
    int ExecuteCommandRemoveJournalQuestEntry(int nCommandId, int nParameters);
    int ExecuteCommandChangeItemCost(int nCommandId, int nParameters);
    int ExecuteCommandSetCustomToken(int nCommandId, int nParameters);
    int ExecuteCommandEffectLightsaberThrow(int nCommandId, int nParameters);
    int ExecuteCommandEndGame(int nCommandId, int nParameters);
    int ExecuteCommandEventActivateItem(int nCommandId, int nParameters);
    int ExecuteCommandPlaySound(int nCommandId, int nParameters);
    int ExecuteCommandGetModuleName(int nCommandId, int nParameters);
    int ExecuteCommandQueueMovie(int nCommandId, int nParameters);
    int ExecuteCommandGetName(int nCommandId, int nParameters);
    int ExecuteCommandSetListenPattern(int nCommandId, int nParameters);
    int ExecuteCommandFloatingText(int nCommandId, int nParameters);
    int ExecuteCommandAddJournalQuestEntry(int nCommandId, int nParameters);
    int ExecuteCommandEffectWhirlWind(int nCommandId, int nParameters);
    int ExecuteCommandPrintString(int nCommandId, int nParameters);
    int ExecuteCommandEffectImmunity(int nCommandId, int nParameters);
    int ExecuteCommandTalent(int nCommandId, int nParameters);
    int ExecuteCommandPlayMovie(int nCommandId, int nParameters);
    int ExecuteCommandCreateItemOnObject(int nCommandId, int nParameters);
    int ExecuteCommandCreateItemOnFloor(int nCommandId, int nParameters);
    int ExecuteCommandGetItemPossessedBy(int nCommandId, int nParameters);
    int ExecuteCommandSpeakString(int nCommandId, int nParameters);
    int ExecuteCommandGetSpellTargetLocation(int nCommandId, int nParameters);
    int ExecuteCommandEffectHorrified(int nCommandId, int nParameters);
    int ExecuteCommandPopUpGUIPanel(int nCommandId, int nParameters);
    int ExecuteCommandSetJournalQuestEntryPicture(int nCommandId, int nParameters);
    int ExecuteCommandStartNewModule(int nCommandId, int nParameters);
    int ExecuteCommandGetLastConversation(int nCommandId, int nParameters);
    int ExecuteCommandGetTag(int nCommandId, int nParameters);
    int ExecuteCommandSetGlobalString(int nCommandId, int nParameters);
    int ExecuteCommandSetEffectIcon(int nCommandId, int nParameters);
    int ExecuteCommandEffectDamage(int nCommandId, int nParameters);
    int ExecuteCommandIntToString(int nCommandId, int nParameters);
    int ExecuteCommandGetIsLiveContentAvailable(int nCommandId, int nParameters);
    int ExecuteCommandStringConversions(int nCommandId, int nParameters);
    int ExecuteCommandFloatToString(int nCommandId, int nParameters);
    int ExecuteCommandGetIsBusy(int nCommandId, int nParameters);       // 0x0053ECB0 (name guessed)
    int ExecuteCommandGetNPCValue(int nCommandId, int nParameters);     // 0x0053ED70 (name guessed)
    int ExecuteCommandGetIsEffectValid(int nCommandId, int nParameters); // 0x0053EE00 (name guessed)
    int ExecuteCommandGetSameObject54(int nCommandId, int nParameters); // 0x0053EEC0 (name guessed)
    int ExecuteCommandGetGuiValueB4(int nCommandId, int nParameters);   // 0x0053EF70 (name guessed)
    int ExecuteCommandGetCreatureStat194Or1(int nCommandId, int nParameters); // 0x0053EFA0 (name guessed)
    int ExecuteCommandGetTimeHour(int nCommandId, int nParameters);     // 0x0053E020 (name guessed)
    int ExecuteCommandGetTimeMillisecond(int nCommandId, int nParameters); // 0x0053E060 (name guessed)
    int ExecuteCommandGetTimeMinute(int nCommandId, int nParameters);   // 0x0053E0A0 (name guessed)
    int ExecuteCommandGetTimeSecond(int nCommandId, int nParameters);   // 0x0053E0E0 (name guessed)
    int ExecuteCommandGetEffectCount(int nCommandId, int nParameters);  // 0x0053E120 (name guessed)
    int ExecuteCommandGetCreatureValue(int nCommandId, int nParameters); // 0x0053E1B0 (name guessed)
    int ExecuteCommandGetObjectValue(int nCommandId, int nParameters);  // 0x0053E220 (name guessed)
    int ExecuteCommandActionBarkString(int nCommandId, int nParameters); // 0x0052C6A0
    int ExecuteCommandActionDoCommand(int nCommandId, int nParameters); // 0x0052C740
    int ExecuteCommandActionEquipMostDamagingMelee(int nCommandId, int nParameters); // 0x0052C7B0
    int ExecuteCommandActionEquipMostDamagingRanged(int nCommandId, int nParameters); // 0x0052C820
    int ExecuteCommandActionEquipMostEffectiveArmor(int nCommandId, int nParameters); // 0x0052C880
    int ExecuteCommandActionGiveItem(int nCommandId, int nParameters);  // 0x0052C8B0 (also ActionTakeItem)
    int ExecuteCommandActionFollowLeader(int nCommandId, int nParameters); // 0x0052CBE0
    int ExecuteCommandActionInteractObject(int nCommandId, int nParameters); // 0x0052CC70
    int ExecuteCommandActionJumpToObject(int nCommandId, int nParameters); // 0x0052CCE0 (also JumpToObject)
    int ExecuteCommandActionJumpToLocation(int nCommandId, int nParameters); // 0x0052CDC0 (also JumpToLocation)
    int ExecuteCommandActionLockObject(int nCommandId, int nParameters); // 0x0052CFF0 (also ActionUnlockObject)
    int ExecuteCommandActionMoveAwayFromLocation(int nCommandId, int nParameters); // 0x0052D090
    int ExecuteCommandActionPauseConversation(int nCommandId, int nParameters); // 0x0052D330
    int ExecuteCommandActionRandomWalk(int nCommandId, int nParameters); // 0x0052D440
    int ExecuteCommandActionResumeConversation(int nCommandId, int nParameters); // 0x0052D520
    int ExecuteCommandActionUseFeat(int nCommandId, int nParameters);   // 0x0052D8F0
    int ExecuteCommandActionUseSkill(int nCommandId, int nParameters);  // 0x0052D9B0
    int ExecuteCommandActionUseTalent(int nCommandId, int nParameters); // 0x0052DAF0 (OnObject / AtLocation)
    int ExecuteCommandAddAvailableNPCByObject(int nCommandId, int nParameters); // 0x0052DC40
    int ExecuteCommandAddAvailableNPCByTemplate(int nCommandId, int nParameters); // 0x0052DCD0
    int ExecuteCommandAddMultiClass(int nCommandId, int nParameters);   // 0x0052DDC0
    int ExecuteCommandAddPartyMember(int nCommandId, int nParameters);  // 0x0052DE70
    int ExecuteCommandAddToParty(int nCommandId, int nParameters);      // 0x0052DF40
    int ExecuteCommandAdjustAlignment(int nCommandId, int nParameters); // 0x0052DFE0
    int ExecuteCommandAdjustReputation(int nCommandId, int nParameters); // 0x0052E1A0
    int ExecuteCommandAmbientSound(int nCommandId, int nParameters);    // 0x0052E270 (Play/Stop/ChangeDay/ChangeNight/SetDayVolume/SetNightVolume)
    int ExecuteCommandApplyEffectAtLocation(int nCommandId, int nParameters); // 0x0052E430
    int ExecuteCommandApplyEffectToObject(int nCommandId, int nParameters); // 0x0052E570
    int ExecuteCommandAssignCommand(int nCommandId, int nParameters);  // 0x0052E720
    int ExecuteCommandActionAttack(int nCommandId, int nParameters);   // 0x0052E7F0
    int ExecuteCommandCutsceneAttack(int nCommandId, int nParameters); // 0x0052E890
    int ExecuteCommandCutsceneMove(int nCommandId, int nParameters);   // 0x0052E950
    int ExecuteCommandChangeFaction(int nCommandId, int nParameters);  // 0x0052F2F0 (also ChangeToStandardFaction)
    int ExecuteCommandClearAllActions(int nCommandId, int nParameters); // 0x0052F4A0
    int ExecuteCommandActionCloseDoor(int nCommandId, int nParameters); // 0x0052F4F0
    int ExecuteCommandFeetToMeters(int nCommandId, int nParameters);   // 0x005302F0 (also YardsToMeters)
    int ExecuteCommandDoSinglePlayerAutoSave(int nCommandId, int nParameters); // 0x00530660
    int ExecuteCommandEffectAbilityDecrease(int nCommandId, int nParameters); // 0x00530680
    int ExecuteCommandEffectAbilityIncrease(int nCommandId, int nParameters); // 0x005307F0
    int ExecuteCommandEffectACDecrease(int nCommandId, int nParameters); // 0x00530960
    int ExecuteCommandEffectACIncrease(int nCommandId, int nParameters); // 0x00530B20
    int ExecuteCommandEffectAttackDecrease(int nCommandId, int nParameters); // 0x00530F60
    int ExecuteCommandEffectAttackIncrease(int nCommandId, int nParameters); // 0x005310E0
    int ExecuteCommandEffectConcealment(int nCommandId, int nParameters); // 0x005319A0
    int ExecuteCommandEffectDamageDecrease(int nCommandId, int nParameters); // 0x00531D50
    int ExecuteCommandEffectDamageImmunityIncrease(int nCommandId, int nParameters); // 0x00532050
    int ExecuteCommandEffectDamageIncrease(int nCommandId, int nParameters); // 0x005321B0
    int ExecuteCommandEffectDamageReduction(int nCommandId, int nParameters); // 0x00532350
    int ExecuteCommandEffectDamageResistance(int nCommandId, int nParameters); // 0x00532500
    int ExecuteCommandEffectForceShield(int nCommandId, int nParameters); // 0x00532690
    int ExecuteCommandEffectDamageShield(int nCommandId, int nParameters); // 0x00532790
    int ExecuteCommandEffectForceDrain(int nCommandId, int nParameters); // 0x00532920
    int ExecuteCommandEffectHeal(int nCommandId, int nParameters);     // 0x00532A70
    int ExecuteCommandEffectHitPointChangeWhenDying(int nCommandId, int nParameters); // 0x00532BD0
    int ExecuteCommandEffectLinkEffects(int nCommandId, int nParameters); // 0x00532F80
    int ExecuteCommandEffectMissChance(int nCommandId, int nParameters); // 0x00533090
    int ExecuteCommandEffectModifyAttacks(int nCommandId, int nParameters); // 0x005331C0
    int ExecuteCommandEffectMovementSpeedDecrease(int nCommandId, int nParameters); // 0x005332E0
    int ExecuteCommandEffectMovementSpeedIncrease(int nCommandId, int nParameters); // 0x00533420
    int ExecuteCommandEffectPoison(int nCommandId, int nParameters);   // 0x00533560
    int ExecuteCommandEffectSeeInvisible(int nCommandId, int nParameters); // 0x00533AC0
    int ExecuteCommandEffectSkillDecrease(int nCommandId, int nParameters); // 0x00533B90
    int ExecuteCommandEffectSkillIncrease(int nCommandId, int nParameters); // 0x00533D00
    int ExecuteCommandEffectSpellImmunity(int nCommandId, int nParameters); // 0x00533E70
    int ExecuteCommandEffectSpellLevelAbsorption(int nCommandId, int nParameters); // 0x00533FA0
    int ExecuteCommandGetLastAttacker(int nCommandId, int nParameters); // 0x0053A880
    int ExecuteCommandGetLastHostileTarget(int nCommandId, int nParameters); // 0x0053A900
    int ExecuteCommandGetLastAttackAction(int nCommandId, int nParameters); // 0x0053A970
    int ExecuteCommandGetLastAttackMode(int nCommandId, int nParameters); // 0x0053A9E0
    int ExecuteCommandGetLastAttackResult(int nCommandId, int nParameters); // 0x0053AAD0
    int ExecuteCommandGetWasForcePowerSuccessful(int nCommandId, int nParameters); // 0x0053AB40
    int ExecuteCommandGetLastAttackType(int nCommandId, int nParameters); // 0x0053ABB0
    int ExecuteCommandGetLastClosedBy(int nCommandId, int nParameters); // 0x0053AC60
    int ExecuteCommandGetLastCombatFeatUsed(int nCommandId, int nParameters); // 0x0053ACD0
    int ExecuteCommandGetLastDamager(int nCommandId, int nParameters); // 0x0053AD40
    int ExecuteCommandGetLastDisarmed(int nCommandId, int nParameters); // 0x0053ADB0
    int ExecuteCommandGetLastDisturbed(int nCommandId, int nParameters); // 0x0053AE50
    int ExecuteCommandGetLastForcePowerUsed(int nCommandId, int nParameters); // 0x0053AED0
    int ExecuteCommandGetLastHostileActor(int nCommandId, int nParameters); // 0x0053AF40
    int ExecuteCommandGetLastKiller(int nCommandId, int nParameters); // 0x0053B080
    int ExecuteCommandGetLastLocked(int nCommandId, int nParameters); // 0x0053B0E0
    int ExecuteCommandGetLastOpenedBy(int nCommandId, int nParameters); // 0x0053B170
    int ExecuteCommandGetLastPazaakResult(int nCommandId, int nParameters); // 0x0053B1E0
    int ExecuteCommandGetLastPerceived(int nCommandId, int nParameters); // 0x0053B230
    int ExecuteCommandGetLastPerception(int nCommandId, int nParameters); // 0x0053B2A0 (Heard/Inaudible/Seen/Vanished)
    int ExecuteCommandGetLastSpeaker(int nCommandId, int nParameters); // 0x0053B410
    int ExecuteCommandGetLastTrapDetected(int nCommandId, int nParameters); // 0x0053B480
    int ExecuteCommandGetLastUnlocked(int nCommandId, int nParameters); // 0x0053B4E0
    int ExecuteCommandGetLastUsedBy(int nCommandId, int nParameters); // 0x0053B570
    int ExecuteCommandGetLastWeaponUsed(int nCommandId, int nParameters); // 0x0053B5E0
    int ExecuteCommandGetExitingObject(int nCommandId, int nParameters); // 0x0053B640
    int ExecuteCommandGetLoadFromSaveGame(int nCommandId, int nParameters); // 0x0053B730
    int ExecuteCommandGetAbilityScore(int nCommandId, int nParameters); // 0x005361B0
    int ExecuteCommandGetAbilityModifier(int nCommandId, int nParameters); // 0x00536310
    int ExecuteCommandGetAC(int nCommandId, int nParameters);           // 0x00536410
    int ExecuteCommandGetGoodEvilValue(int nCommandId, int nParameters); // 0x005364F0 (also GetAlignmentGoodEvil)
    int ExecuteCommandGetAreaOfEffectCreator(int nCommandId, int nParameters); // 0x005365A0
    int ExecuteCommandGetAppearanceType(int nCommandId, int nParameters); // 0x00536630
    int ExecuteCommandGetArea(int nCommandId, int nParameters);         // 0x005366A0
    int ExecuteCommandGetAreaUnescapable(int nCommandId, int nParameters); // 0x005367B0
    int ExecuteCommandGetAttackTarget(int nCommandId, int nParameters); // 0x00536800
    int ExecuteCommandGetAttemptedAttackTarget(int nCommandId, int nParameters); // 0x00536860
    int ExecuteCommandGetAttemptedSpellTarget(int nCommandId, int nParameters); // 0x005368B0
    int ExecuteCommandGetAttemptedMovementTarget(int nCommandId, int nParameters); // 0x00536900
    int ExecuteCommandGetSpellTarget(int nCommandId, int nParameters);  // 0x00536950
    int ExecuteCommandGetBaseItemType(int nCommandId, int nParameters); // 0x005369B0
    int ExecuteCommandMusicBackgroundGetBattleTrack(int nCommandId, int nParameters); // 0x00536A10
    int ExecuteCommandGetBlockingDoor(int nCommandId, int nParameters); // 0x00536A80
};

#endif
