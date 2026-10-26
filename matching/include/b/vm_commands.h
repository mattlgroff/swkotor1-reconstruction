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
    int ExecuteCommandGetNumStackedItems(int nCommandId, int nParameters); // 0x0053C220
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
    int ExecuteCommandGetFactionAverageGoodEvilAlignment(int nCommandId, int nParameters); // 0x005380F0
    int ExecuteCommandGetFactionAverageLevel(int nCommandId, int nParameters); // 0x00538190
    int ExecuteCommandGetFactionAverageReputation(int nCommandId, int nParameters); // 0x00538230
    int ExecuteCommandGetFactionAverageXP(int nCommandId, int nParameters); // 0x005382E0
    int ExecuteCommandGetFactionBestAC(int nCommandId, int nParameters); // 0x00538380
    int ExecuteCommandGetFactionEqual(int nCommandId, int nParameters); // 0x00538450
    int ExecuteCommandGetFactionGold(int nCommandId, int nParameters); // 0x00538500
    int ExecuteCommandGetFactionLeastDamagedMember(int nCommandId, int nParameters); // 0x005385A0
    int ExecuteCommandGetFactionLeader(int nCommandId, int nParameters); // 0x00538670
    int ExecuteCommandGetFirstFactionMember(int nCommandId, int nParameters); // 0x005386F0 (also GetNextFactionMember)
    int ExecuteCommandGetFactionMostDamagedMember(int nCommandId, int nParameters); // 0x00538800
    int ExecuteCommandGetFactionMostFrequentClass(int nCommandId, int nParameters); // 0x005388D0
    int ExecuteCommandGetFactionStrongestMember(int nCommandId, int nParameters); // 0x00538970
    int ExecuteCommandGetFactionWeakestMember(int nCommandId, int nParameters); // 0x00538A40
    int ExecuteCommandGetFactionWorstAC(int nCommandId, int nParameters); // 0x00538B10
    int ExecuteCommandGetEffectCreator(int nCommandId, int nParameters); // 0x00537AA0
    int ExecuteCommandGetEffectDurationType(int nCommandId, int nParameters); // 0x00537B20
    int ExecuteCommandGetEffectSpellId(int nCommandId, int nParameters); // 0x00537B90
    int ExecuteCommandGetEffectSubType(int nCommandId, int nParameters); // 0x00537C00
    int ExecuteCommandGetEffectType(int nCommandId, int nParameters);   // 0x00537C70
    int ExecuteCommandGetFortitudeSavingThrow(int nCommandId, int nParameters); // 0x00538BE0
    int ExecuteCommandGetGameDifficulty(int nCommandId, int nParameters); // 0x00538CA0
    int ExecuteCommandGetGoingToBeAttackedBy(int nCommandId, int nParameters); // 0x00538FC0
    int ExecuteCommandGetGold(int nCommandId, int nParameters);         // 0x00539030
    int ExecuteCommandGetGoldPieceValue(int nCommandId, int nParameters); // 0x005390E0
    int ExecuteCommandGetReflexSavingThrow(int nCommandId, int nParameters); // 0x0053CE10
    int ExecuteCommandGetWillSavingThrow(int nCommandId, int nParameters); // 0x0053E540
    int ExecuteCommandGetHasFeat(int nCommandId, int nParameters);      // 0x00539170
    int ExecuteCommandGetHasFeatEffect(int nCommandId, int nParameters); // 0x00539230
    int ExecuteCommandGetHasInventory(int nCommandId, int nParameters); // 0x005392D0
    int ExecuteCommandGetHasSkill(int nCommandId, int nParameters);     // 0x00539380
    int ExecuteCommandGetHasSpell(int nCommandId, int nParameters);     // 0x00539440
    int ExecuteCommandGetHasSpellEffect(int nCommandId, int nParameters); // 0x005394D0
    int ExecuteCommandGetHitDice(int nCommandId, int nParameters);      // 0x00539570
    int ExecuteCommandGetCurrentHitPoints(int nCommandId, int nParameters); // 0x00539610 (also GetMaxHitPoints)
    int ExecuteCommandGetCurrentForcePoints(int nCommandId, int nParameters); // 0x005396F0
    int ExecuteCommandGetMaxForcePoints(int nCommandId, int nParameters); // 0x00539760
    int ExecuteCommandGetIdentified(int nCommandId, int nParameters);   // 0x005397C0
    int ExecuteCommandGetInventoryDisturbItem(int nCommandId, int nParameters); // 0x00539820
    int ExecuteCommandGetInventoryDisturbType(int nCommandId, int nParameters); // 0x00539870
    int ExecuteCommandGetIsDawn(int nCommandId, int nParameters);       // 0x005398C0
    int ExecuteCommandGetIsDay(int nCommandId, int nParameters);        // 0x00539900
    int ExecuteCommandGetIsNight(int nCommandId, int nParameters);      // 0x00539DE0
    int ExecuteCommandGetIsEncounterCreature(int nCommandId, int nParameters); // 0x00539C10
    int ExecuteCommandGetIsImmune(int nCommandId, int nParameters);     // 0x00539C80
    int ExecuteCommandGetIsInCombat(int nCommandId, int nParameters);   // 0x00539D80
    int ExecuteCommandGetIsDoorActionPossible(int nCommandId, int nParameters); // 0x00539940
    int ExecuteCommandGetIsDusk(int nCommandId, int nParameters); // 0x00539BD0
    int ExecuteCommandGetIsOpen(int nCommandId, int nParameters); // 0x00539E20
    int ExecuteCommandGetIsPlayableRacialType(int nCommandId, int nParameters); // 0x00539ED0
    int ExecuteCommandGetIsTalentValid(int nCommandId, int nParameters); // 0x00539F90
    int ExecuteCommandGetIsTrapped(int nCommandId, int nParameters); // 0x0053A000
    int ExecuteCommandGetIsWeaponEffective(int nCommandId, int nParameters); // 0x0053A0A0
    int ExecuteCommandGetItemACValue(int nCommandId, int nParameters); // 0x0053A130
    int ExecuteCommandGetItemHasItemProperty(int nCommandId, int nParameters); // 0x0053A1D0
    int ExecuteCommandGetItemInSlot(int nCommandId, int nParameters); // 0x0053A2C0
    int ExecuteCommandGetItemPossessor(int nCommandId, int nParameters); // 0x0053A5C0
    int ExecuteCommandGetJournalQuestExperience(int nCommandId, int nParameters); // 0x0053A680
    int ExecuteCommandGetLocalBoolean(int nCommandId, int nParameters); // 0x0053B760
    int ExecuteCommandGetLocation(int nCommandId, int nParameters); // 0x0053B840
    int ExecuteCommandGetLocked(int nCommandId, int nParameters); // 0x0053B960
    int ExecuteCommandGetLockKeyRequired(int nCommandId, int nParameters); // 0x0053BA10
    int ExecuteCommandGetMatchedSubstringsCount(int nCommandId, int nParameters); // 0x0053BCC0
    int ExecuteCommandGetListenPatternNumber(int nCommandId, int nParameters); // 0x0053BD20
    int ExecuteCommandGetMetaMagicFeat(int nCommandId, int nParameters); // 0x0053BD80
    int ExecuteCommandGetModule(int nCommandId, int nParameters); // 0x0053BDF0
    int ExecuteCommandGetModuleItemAcquired(int nCommandId, int nParameters); // 0x0053BE30
    int ExecuteCommandGetFirstObjectInArea(int nCommandId, int nParameters); // 0x0053C390
    int ExecuteCommandGetObjectType(int nCommandId, int nParameters); // 0x0053C610
    int ExecuteCommandGetIsObjectValid(int nCommandId, int nParameters); // 0x0053C720
    int ExecuteCommandGetObjectSeen(int nCommandId, int nParameters); // 0x0053C780
    int ExecuteCommandGetPartyMemberByIndex(int nCommandId, int nParameters); // 0x0053C870
    int ExecuteCommandGetPartyMemberCount(int nCommandId, int nParameters); // 0x0053C8F0
    int ExecuteCommandGetFirstPC(int nCommandId, int nParameters); // 0x0053C920
    int ExecuteCommandGetPCLevellingUp(int nCommandId, int nParameters); // 0x0053C980
    int ExecuteCommandGetPCSpeaker(int nCommandId, int nParameters); // 0x0053C9C0
    int ExecuteCommandGetPlaceableIllumination(int nCommandId, int nParameters); // 0x0053CA00
    int ExecuteCommandGetPlotFlag(int nCommandId, int nParameters); // 0x0053CA60
    int ExecuteCommandGetPosition(int nCommandId, int nParameters); // 0x0053CAE0
    int ExecuteCommandGetRacialType(int nCommandId, int nParameters); // 0x0053CBA0
    int ExecuteCommandGetReputation(int nCommandId, int nParameters); // 0x0053CED0
    int ExecuteCommandGetPlayerRestrictMode(int nCommandId, int nParameters); // 0x0053CF70
    int ExecuteCommandGetRunScriptVar(int nCommandId, int nParameters); // 0x0053D020
    int ExecuteCommandGetSkillRank(int nCommandId, int nParameters); // 0x0053D050
    int ExecuteCommandGetLastSpellCaster(int nCommandId, int nParameters); // 0x0053D110
    int ExecuteCommandGetSpellCastItem(int nCommandId, int nParameters); // 0x0053D250
    int ExecuteCommandGetSpellId(int nCommandId, int nParameters); // 0x0053D2B0
    int ExecuteCommandGetSpellTargetObject(int nCommandId, int nParameters); // 0x0053D510
    int ExecuteCommandGetGender(int nCommandId, int nParameters); // 0x0053D5A0
    int ExecuteCommandGetSpellSaveDC(int nCommandId, int nParameters); // 0x0053D320
    int ExecuteCommandGetStringByStrRef(int nCommandId, int nParameters); // 0x0053D610
    int ExecuteCommandGetWaypointByTag(int nCommandId, int nParameters); // 0x0053E2B0
    int ExecuteCommandGetTransitionTarget(int nCommandId, int nParameters); // 0x0053E3C0
    int ExecuteCommandGetWeaponRanged(int nCommandId, int nParameters); // 0x0053E4C0
    int ExecuteCommandGetXP(int nCommandId, int nParameters); // 0x0053E600
    int ExecuteCommandGiveGoldToCreature(int nCommandId, int nParameters); // 0x0053E690
    int ExecuteCommandGiveXPToCreature(int nCommandId, int nParameters); // 0x0053E750
    int ExecuteCommandGetIsListening(int nCommandId, int nParameters); // 0x0053F080
    int ExecuteCommandIsNPCPartyMember(int nCommandId, int nParameters); // 0x0053F100
    int ExecuteCommandIsObjectPartyMember(int nCommandId, int nParameters); // 0x0053F160
    int ExecuteCommandGetIsPC(int nCommandId, int nParameters); // 0x0053F1C0
    int ExecuteCommandLocation(int nCommandId, int nParameters); // 0x0053F3B0
    int ExecuteCommandGetFacingFromLocation(int nCommandId, int nParameters); // 0x0053F500
    int ExecuteCommandFabs(int nCommandId, int nParameters); // 0x0053F5C0
    int ExecuteCommandSetAreaUnescapable(int nCommandId, int nParameters); // 0x005424E0
    int ExecuteCommandSetCameraMode(int nCommandId, int nParameters); // 0x00542640
    int ExecuteCommandSetCommandable(int nCommandId, int nParameters); // 0x005426C0
    int ExecuteCommandSetIdentified(int nCommandId, int nParameters); // 0x00542D50
    int ExecuteCommandSetLightsaberPowered(int nCommandId, int nParameters); // 0x00542F10
    int ExecuteCommandSetListening(int nCommandId, int nParameters); // 0x00542FB0
    int ExecuteCommandSetLocked(int nCommandId, int nParameters); // 0x00543240
    int ExecuteCommandSetNPCSelectability(int nCommandId, int nParameters); // 0x00543300
    int ExecuteCommandSetPlaceableIllumination(int nCommandId, int nParameters); // 0x00543360
    int ExecuteCommandSetPlotFlag(int nCommandId, int nParameters); // 0x005433D0
    int ExecuteCommandSetPlayerRestrictMode(int nCommandId, int nParameters); // 0x00543450
    int ExecuteCommandSetReturnStrref(int nCommandId, int nParameters); // 0x005434C0
    int ExecuteCommandSWMG_SetSpeedBlurEffect(int nCommandId, int nParameters); // 0x00543550
    int ExecuteCommandSetForcePowerUnsuccessful(int nCommandId, int nParameters); // 0x00543740
    int ExecuteCommandSetXP(int nCommandId, int nParameters); // 0x005437B0
    int ExecuteCommandShipBuild(int nCommandId, int nParameters); // 0x00543840
    int ExecuteCommandShowLevelUpGUI(int nCommandId, int nParameters); // 0x00543870
    int ExecuteCommandShowUpgradeScreen(int nCommandId, int nParameters); // 0x00543990
    int ExecuteCommandSoundObjectGetFixedVariance(int nCommandId, int nParameters); // 0x00543AA0
    int ExecuteCommandSoundObjectGetPitchVariance(int nCommandId, int nParameters); // 0x00543B10
    int ExecuteCommandSoundObjectGetVolume(int nCommandId, int nParameters); // 0x00543B80
    int ExecuteCommandSoundObjectPlay(int nCommandId, int nParameters); // 0x00543BE0
    int ExecuteCommandSoundObjectSetFixedVariance(int nCommandId, int nParameters); // 0x00543C30
    int ExecuteCommandSoundObjectSetPitchVariance(int nCommandId, int nParameters); // 0x00543CA0
    int ExecuteCommandSoundObjectSetPosition(int nCommandId, int nParameters); // 0x00543D10
    int ExecuteCommandSoundObjectSetVolume(int nCommandId, int nParameters); // 0x00543DA0
    int ExecuteCommandSoundObjectStop(int nCommandId, int nParameters); // 0x00543E10
    int ExecuteCommandSoundObjectFadeAndStop(int nCommandId, int nParameters); // 0x00543E60
    int ExecuteCommandStopRumblePattern(int nCommandId, int nParameters); // 0x005446C0
    int ExecuteCommandSwitchPlayerCharacter(int nCommandId, int nParameters); // 0x00544910
    int ExecuteCommandGetItemStackSize(int nCommandId, int nParameters); // 0x005465D0
    int ExecuteCommandGetPartyAIStyle(int nCommandId, int nParameters); // 0x005459A0
    int ExecuteCommandGetNPCAIStyle(int nCommandId, int nParameters); // 0x005459D0
    int ExecuteCommandSetPartyAIStyle(int nCommandId, int nParameters); // 0x00545C50
    int ExecuteCommandSetNPCAIStyle(int nCommandId, int nParameters); // 0x00545CA0
    int ExecuteCommandClearAllEffects(int nCommandId, int nParameters); // 0x00545D40
    int ExecuteCommandGetStandardFaction(int nCommandId, int nParameters); // 0x00545D80
    int ExecuteCommandGetMinOneHP(int nCommandId, int nParameters); // 0x00545F10
    int ExecuteCommandSetMinOneHP(int nCommandId, int nParameters); // 0x00545F90
    int ExecuteCommandSetGlobalFadeIn(int nCommandId, int nParameters); // 0x00546010
    int ExecuteCommandSetGlobalFadeOut(int nCommandId, int nParameters); // 0x005460D0
    int ExecuteCommandSetFormation(int nCommandId, int nParameters); // 0x00546230
    int ExecuteCommandGetIsDebilitated(int nCommandId, int nParameters); // 0x005462D0
    int ExecuteCommandGetSubScreenID(int nCommandId, int nParameters); // 0x005464D0
    int ExecuteCommandCancelCombat(int nCommandId, int nParameters); // 0x00546510
    int ExecuteCommandPauseGame(int nCommandId, int nParameters); // 0x00546590
    int ExecuteCommandShowGalaxyMap(int nCommandId, int nParameters); // 0x005467B0
    int ExecuteCommandSetPlanetAvailable(int nCommandId, int nParameters); // 0x005467F0
    int ExecuteCommandGetPlanetAvailable(int nCommandId, int nParameters); // 0x00546850
    int ExecuteCommandSetPlanetSelectable(int nCommandId, int nParameters); // 0x005468B0
    int ExecuteCommandGetPlanetSelectable(int nCommandId, int nParameters); // 0x00546910
    int ExecuteCommandGetSelectedPlanet(int nCommandId, int nParameters); // 0x00546970
    int ExecuteCommandSetItemNonEquippable(int nCommandId, int nParameters); // 0x005469A0
    int ExecuteCommandGetButtonMashCheck(int nCommandId, int nParameters); // 0x00546A10
    int ExecuteCommandSetButtonMashCheck(int nCommandId, int nParameters); // 0x00546A40
    int ExecuteCommandSetDialogPlaceableCamera(int nCommandId, int nParameters); // 0x00546A80
    int ExecuteCommandGetSoloMode(int nCommandId, int nParameters); // 0x00546AF0
    int ExecuteCommandGetMaxStealthXP(int nCommandId, int nParameters); // 0x00546B20
    int ExecuteCommandSetMaxStealthXP(int nCommandId, int nParameters); // 0x00546B70
    int ExecuteCommandGetCurrentStealthXP(int nCommandId, int nParameters); // 0x00546BD0
    int ExecuteCommandSetCurrentStealthXP(int nCommandId, int nParameters); // 0x00546C20
    int ExecuteCommandAwardStealthXP(int nCommandId, int nParameters); // 0x00546C80
    int ExecuteCommandGetStealthXPEnabled(int nCommandId, int nParameters); // 0x00546CE0
    int ExecuteCommandSetStealthXPEnabled(int nCommandId, int nParameters); // 0x00546D30
    int ExecuteCommandGetSubRace(int nCommandId, int nParameters); // 0x00546D90
    int ExecuteCommandGetBlockingCreature(int nCommandId, int nParameters); // 0x00546E00
    int ExecuteCommandGetFoundEnemyCreature(int nCommandId, int nParameters); // 0x00546E70
    int ExecuteCommandGetStealthXPDecrement(int nCommandId, int nParameters); // 0x00546EE0
    int ExecuteCommandSetStealthXPDecrement(int nCommandId, int nParameters); // 0x00546F30
    int ExecuteCommandSetLockOrientationInDialog(int nCommandId, int nParameters); // 0x005470E0
    int ExecuteCommandSetLockHeadFollowInDialog(int nCommandId, int nParameters); // 0x00547160
    int ExecuteCommandEnableVideoEffect(int nCommandId, int nParameters); // 0x005471E0
    int ExecuteCommandDisableVideoEffect(int nCommandId, int nParameters); // 0x00547220
    int ExecuteCommandSetTutorialWindowsEnabled(int nCommandId, int nParameters); // 0x005472F0
    int ExecuteCommandShowTutorialWindow(int nCommandId, int nParameters); // 0x00547340
    int ExecuteCommandIsCreditSequenceInProgress(int nCommandId, int nParameters); // 0x005473B0
    int ExecuteCommandResetDialogState(int nCommandId, int nParameters); // 0x00547BC0
    int ExecuteCommandSetGoodEvilValue(int nCommandId, int nParameters); // 0x00547BE0
    int ExecuteCommandGetIsPoisoned(int nCommandId, int nParameters); // 0x00547C50
    int ExecuteCommandSetSoloMode(int nCommandId, int nParameters); // 0x00547CE0
    int ExecuteCommandCancelPostDialogCharacterSwitch(int nCommandId, int nParameters); // 0x00547D20
    int ExecuteCommandSetMaxHitPoints(int nCommandId, int nParameters); // 0x00547D40
    int ExecuteCommandNoClicksFor(int nCommandId, int nParameters); // 0x00547DC0
    int ExecuteCommandHoldWorldFadeInForDialog(int nCommandId, int nParameters); // 0x00547E30
    int ExecuteCommandSuppressStatusSummaryEntry(int nCommandId, int nParameters); // 0x00547E50
    int ExecuteCommandGetCheatCode(int nCommandId, int nParameters); // 0x00547E90
    int ExecuteCommandSetMusicVolume(int nCommandId, int nParameters); // 0x00547EF0
    int ExecuteCommandPrintInteger(int nCommandId, int nParameters); // 0x00541640
    int ExecuteCommandGetCreatureSize(int nCommandId, int nParameters); // 0x00536F80
    int ExecuteCommandActionPickUpItem(int nCommandId, int nParameters); // 0x005404F0
    int ExecuteCommandRemoveAvailableNPC(int nCommandId, int nParameters); // 0x00541B40
    int ExecuteCommandRemoveFromParty(int nCommandId, int nParameters); // 0x00541BA0
    int ExecuteCommandMusicBackgroundGetDayTrack(int nCommandId, int nParameters); // 0x00537300
    int ExecuteCommandVectorMagnitude(int nCommandId, int nParameters); // 0x00545320
    int ExecuteCommandGetChallengeRating(int nCommandId, int nParameters); // 0x00536C60
    int ExecuteCommandGetCommandable(int nCommandId, int nParameters); // 0x00536E40
    int ExecuteCommandActionOpenDoor(int nCommandId, int nParameters); // 0x00540260
    int ExecuteCommandSpawnAvailableNPC(int nCommandId, int nParameters); // 0x00543ED0
    int ExecuteCommandVector(int nCommandId, int nParameters); // 0x00545120
    int ExecuteCommandGetFirstAttacker(int nCommandId, int nParameters); // 0x00546190
    int ExecuteCommandDeleteJournalWorldAllEntries(int nCommandId, int nParameters); // 0x00548A80
    int ExecuteCommandGetDamageDealtByType(int nCommandId, int nParameters); // 0x00537250
    int ExecuteCommandGetEncounterActive(int nCommandId, int nParameters); // 0x00537E30
    int ExecuteCommandMusicBattlePlay(int nCommandId, int nParameters); // 0x005400C0
    int ExecuteCommandMagicalEffect(int nCommandId, int nParameters); // 0x005435C0
    int ExecuteCommandRevealMap(int nCommandId, int nParameters); // 0x00547240
    int ExecuteCommandGetCreatureHasTalent(int nCommandId, int nParameters); // 0x00536EC0
    int ExecuteCommandRemovePartyMember(int nCommandId, int nParameters); // 0x00541C00
    int ExecuteCommandSetAreaFogColor(int nCommandId, int nParameters); // 0x005478B0
    int ExecuteCommandDeleteJournalWorldEntry(int nCommandId, int nParameters); // 0x00548B20
    int ExecuteCommandDeleteJournalWorldEntryStrref(int nCommandId, int nParameters); // 0x00548BE0
    int ExecuteCommandSetAreaTransitionBMP(int nCommandId, int nParameters); // 0x00542410
    int ExecuteCommandSetEncounterActive(int nCommandId, int nParameters); // 0x00542760
    int ExecuteCommandSetTime(int nCommandId, int nParameters); // 0x00543670
    int ExecuteCommandSignalEvent(int nCommandId, int nParameters); // 0x005439D0
    int ExecuteCommandActionUnequipItem(int nCommandId, int nParameters); // 0x00545050
    int ExecuteCommandActionWait(int nCommandId, int nParameters); // 0x00545800
    int ExecuteCommandGetIsLinkImmune(int nCommandId, int nParameters); // 0x005458D0
    int ExecuteCommandShowPartySelectionGUI(int nCommandId, int nParameters); // 0x005438B0
    int ExecuteCommandActionSurrenderToEnemies(int nCommandId, int nParameters); // 0x00544830
    int ExecuteCommandVectorNormalize(int nCommandId, int nParameters); // 0x00545390
    int ExecuteCommandDelayCommand(int nCommandId, int nParameters); // 0x0052FE30
    int ExecuteCommandGetGlobalString(int nCommandId, int nParameters); // 0x00538ED0
    int ExecuteCommandObjectToString(int nCommandId, int nParameters); // 0x00540170
    int ExecuteCommandGivePlotXP(int nCommandId, int nParameters); // 0x00545E20
    int ExecuteCommandSetMapPinEnabled(int nCommandId, int nParameters); // 0x0054B460
    int ExecuteCommandGetDistanceBetween(int nCommandId, int nParameters); // 0x00537370
    int ExecuteCommandGetEnteringObject(int nCommandId, int nParameters); // 0x00537EE0
    int ExecuteCommandEffectForceResistanceDecrease(int nCommandId, int nParameters); // 0x00534120
    int ExecuteCommandEffectForceResistanceIncrease(int nCommandId, int nParameters); // 0x00534230
    int ExecuteCommandGetFacing(int nCommandId, int nParameters); // 0x00537FE0
    int ExecuteCommandMusicBackgroundPlay(int nCommandId, int nParameters); // 0x0053FFB0
    int ExecuteCommandRandomName(int nCommandId, int nParameters); // 0x00541A30
    int ExecuteCommandSetCameraFacing(int nCommandId, int nParameters); // 0x00542530
    int ExecuteCommandAddJournalWorldEntryStrref(int nCommandId, int nParameters); // 0x005487F0
    int ExecuteCommandGetDistanceBetween2D(int nCommandId, int nParameters); // 0x00537470
    int ExecuteCommandActionSpeakStringByStrRef(int nCommandId, int nParameters); // 0x00544270
    int ExecuteCommandStringToInt(int nCommandId, int nParameters); // 0x00544710
    int ExecuteCommandRemoveEffect(int nCommandId, int nParameters); // 0x0054AE40
    int ExecuteCommandGetDistanceBetweenLocations(int nCommandId, int nParameters); // 0x00537590
    int ExecuteCommandGiveItem(int nCommandId, int nParameters); // 0x0053E830
    int ExecuteCommandGetDistanceToObject(int nCommandId, int nParameters); // 0x00537820
    int ExecuteCommandGetDistanceToObject2D(int nCommandId, int nParameters); // 0x00537960
    int ExecuteCommandGetCurrentAction(int nCommandId, int nParameters); // 0x00549040
    int ExecuteCommandEffectCharmed(int nCommandId, int nParameters); // 0x00534340
    int ExecuteCommandEffectTemporaryHitpoints(int nCommandId, int nParameters); // 0x00534490
    int ExecuteCommandGetIsEnemy(int nCommandId, int nParameters); // 0x00537CE0
    int ExecuteCommandSetIsDestroyable(int nCommandId, int nParameters); // 0x00542DC0
    int ExecuteCommandDuplicateHeadAppearance(int nCommandId, int nParameters); // 0x00546F90
    int ExecuteCommandFaceObjectAwayFromObject(int nCommandId, int nParameters); // 0x00547760
    int ExecuteCommandGetClassByPosition(int nCommandId, int nParameters); // 0x00536CE0
    int ExecuteCommandGetDistanceBetweenLocations2D(int nCommandId, int nParameters); // 0x005376C0
    int ExecuteCommandAngleToVector(int nCommandId, int nParameters); // 0x005451C0
    int ExecuteCommandGetJournalEntry(int nCommandId, int nParameters); // 0x00549F30
    int ExecuteCommandActionMoveAwayFromObject(int nCommandId, int nParameters); // 0x0053F990
    int ExecuteCommandPrintFloat(int nCommandId, int nParameters); // 0x005414D0
    int ExecuteCommandDisplayFeedBackText(int nCommandId, int nParameters); // 0x00530170
    int ExecuteCommandSetItemStackSize(int nCommandId, int nParameters); // 0x00546630
    int ExecuteCommandBarkString(int nCommandId, int nParameters); // 0x00548900
    int ExecuteCommandGetCasterLevel(int nCommandId, int nParameters); // 0x00536AD0
    int ExecuteCommandGetItemActivated(int nCommandId, int nParameters); // 0x0053F220
    int ExecuteCommandGetLastItemEquipped(int nCommandId, int nParameters); // 0x0053F800
    int ExecuteCommandSurrenderByFaction(int nCommandId, int nParameters); // 0x00546340
    int ExecuteCommandActionMoveToLocation(int nCommandId, int nParameters); // 0x0053FE00
    int ExecuteCommandAddJournalWorldEntry(int nCommandId, int nParameters); // 0x00548640
    int ExecuteCommandGetNearestTrapToObject(int nCommandId, int nParameters); // 0x0054A090
    int ExecuteCommandGetReflexAdjustedDamage(int nCommandId, int nParameters); // 0x0053CC30
    int ExecuteCommandActionSpeakString(int nCommandId, int nParameters); // 0x00544090
    int ExecuteCommandSetPartyLeader(int nCommandId, int nParameters); // 0x00545A70
    int ExecuteCommandGetFirstEffect(int nCommandId, int nParameters); // 0x00549180
    int ExecuteCommandOpenStore(int nCommandId, int nParameters); // 0x00540300
};

#endif
