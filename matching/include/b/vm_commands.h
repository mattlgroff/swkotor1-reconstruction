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
    int RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid); // 0x005D0FC0

    void* m_pInternal;                                      // 0x00
};

extern CVirtualMachine* g_pVirtualMachine;                  // 0x007A3A00

#define ENGINE_STRUCTURE_EFFECT 0
#define ENGINE_STRUCTURE_EVENT  1

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C (0)

class CSWVirtualMachineCommands {
public:
    void* m_pVtable;                                        // 0x00
    int m_bValidObjectRunScript;                            // 0x04
    unsigned long m_oidObjectRunScript;                     // 0x08

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
    void DestroyGameDefinedStructure(int nType, void* pStructure); // 0x00548360 (name guessed)
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
};

#endif
