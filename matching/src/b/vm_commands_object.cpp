// CSWVirtualMachineCommands: commands that look up game objects, players
// and journals (saving throws, one-liners, custom tokens, log entries).
#include "b/vm_commands.h"
#include "b/sws_player.h"
#include "b/sws_message.h"
#include "b/party_table.h"
#include "b/global_variable_table.h"
#include <math.h>
#include <stdlib.h>

class CExoDebug {
public:
    void GetCurrentTimestamp(CExoString& sTimestamp);       // 0x005E9410
    void WriteToScriptLogFile(CExoString& sText);           // 0x005E9420
    void FlushScriptLogFile();                              // 0x005E93E0

    CExoString m_sScriptLog;                                // 0x00 (name guessed)
};

class CExoBase {
public:
    void* m_p0;                                             // 0x00
    void* m_pcExoTimers;                                    // 0x04
    CExoDebug* m_pcExoDebug;                                // 0x08
};

extern CExoBase* g_pExoBase;                                // 0x007A39E0
extern int g_bExoDebugDisabled;                             // 0x0083282C (.data)

// Stripped debug output: the arguments are still evaluated.
inline void ExoDebugLog(const char* sText, const char* sTimestamp) {}
inline void ExoDebugLog(const char* sText) {}

class CTlkTable {
public:
    void SetCustomToken(int nToken, const CExoString& sValue); // 0x0041DB50
};

extern CTlkTable* g_pTlkTable;                              // 0x007A3A08
extern const int MIN_CUSTOM_TOKEN;                          // 0x0073ECCC (.rdata)

// The listen-pattern matches kept on the object (+0x1a0).
struct CSWSListenObject {
    char pad_0x0[0x1a0];
    CExoArrayList<CExoString*> m_lstMatchedStrings;         // 0x1a0
};

// FUNCTION: SWKOTOR 0x005421E0
int CSWVirtualMachineCommands::ExecuteCommandSavingThrow(int nCommandId, int nParameters)
{
    unsigned long oidTarget = INVALID_OBJECT_ID;
    unsigned long oidSaveVersus = m_oidObjectRunScript;
    int nDC;
    int nSaveType;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget) ||
        !g_pVirtualMachine->StackPopInteger(&nDC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nSaveType) ||
            !g_pVirtualMachine->StackPopObject(&oidSaveVersus)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType == 5) {
        int nSave;
        switch (nCommandId) {
        case 0x6c:
            nSave = 1;
            break;
        case 0x6d:
            nSave = 2;
            break;
        case 0x6e:
            nSave = 3;
            break;
        }
        nResult = pObject->AsSWSCreature()->SavingThrowRoll(nSave, nDC, nSaveType, oidSaveVersus, 1, 0, 0);
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00543F70
int CSWVirtualMachineCommands::ExecuteCommandSpeakOneLinerConversation(int nCommandId, int nParameters)
{
    CExoString sResRef("");
    unsigned long oidTalkTo = INVALID_OBJECT_ID;
    CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if ((nParameters >= 1 && !g_pVirtualMachine->StackPopString(&sResRef)) ||
        (nParameters >= 2 && !g_pVirtualMachine->StackPopObject(&oidTalkTo))) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        pArray->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSObject()) {
        pObject->AsSWSObject()->SpeakOneLinerConversation(sResRef, oidTalkTo);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00541690
int CSWVirtualMachineCommands::ExecuteCommandWriteTimestampedLogEntry(int nCommandId, int nParameters)
{
    CExoString sLogEntry;
    if (!g_pVirtualMachine->StackPopString(&sLogEntry)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sTimestamp;
    g_pExoBase->m_pcExoDebug->GetCurrentTimestamp(sTimestamp);
    if (!g_bExoDebugDisabled) {
        ExoDebugLog(sLogEntry.CStr(), sTimestamp.CStr());
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00541770
int CSWVirtualMachineCommands::ExecuteCommandPopObjectAndInteger(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0054AF60
int CSWVirtualMachineCommands::ExecuteCommandRemoveJournalQuestEntry(int nCommandId, int nParameters)
{
    CExoString sPlotID;
    if (!g_pVirtualMachine->StackPopString(&sPlotID)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (!pList) {
        return 1;
    }
    void* pos = pList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
        if (pPlayer && pPlayer->GetGameObject()) {
            CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
            if (pCreature) {
                pCreature->GetJournal()->RemoveEntry(sPlotID);
            }
        }
        pList->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0054C830
int CSWVirtualMachineCommands::ExecuteCommandGetMatchedSubstring(int nCommandId, int nParameters)
{
    CExoString sResult;
    int nString;
    if (!g_pVirtualMachine->StackPopInteger(&nString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        CExoArrayList<CExoString*> lstMatches = ((CSWSListenObject*)pObject)->m_lstMatchedStrings;
        if (nString >= 0 && nString < lstMatches.num) {
            sResult = *lstMatches[nString];
        }
    }
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0054B090
int CSWVirtualMachineCommands::ExecuteCommandSetCustomToken(int nCommandId, int nParameters)
{
    int nToken;
    CExoString sValue;
    if (!g_pVirtualMachine->StackPopInteger(&nToken)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nToken > MIN_CUSTOM_TOKEN) {
        g_pTlkTable->SetCustomToken(nToken, sValue);
        CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
        CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
        if (pMessage && pList) {
            void* pos = pList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
                if (pPlayer && pPlayer->GetGameObject()) {
                    pMessage->SendServerToPlayerSetCustomToken(pPlayer->GetPlayerId(), nToken, sValue);
                }
                pList->GetNext(pos);
            }
        }
    }
    return 0;
}

// Door action used by 0x00548310 (name guessed).
class CSWSDoor {
public:
    void DoorAction(int a1, int a2);                        // 0x00589600 (name guessed)
};

extern const unsigned char OBJECT_TYPE_MODULE;              // 0x00746632 (3)
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)

// The generic object's local variable table (+0x110).
class CSWSObjectVarTableView : public CGameObject {
public:
    char pad_0xf4[0x110 - 0xf4];
    CSWVarTable m_VarTable;                                 // 0x110
};

// FUNCTION: SWKOTOR 0x005355F0
int CSWVirtualMachineCommands::ExecuteCommandActionEquipItem(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    int nInventorySlot;
    int bInstant = 0;
    unsigned long oidSelf = m_oidObjectRunScript;
    CGameObject* pObject;
    CGameObject* pItemObject;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nInventorySlot)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&bInstant)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nInventorySlot >= 0 && nInventorySlot < 0x12) {
        nInventorySlot = (int)pow(2.0, nInventorySlot);
        if (m_bValidObjectRunScript == 1 &&
            g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidSelf, &pObject) == GAME_OBJECT_VALID) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature) {
                if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidItem, &pItemObject) == GAME_OBJECT_VALID) {
                    CSWSItem* pItem = pItemObject->AsSWSItem();
                    if (pItem) {
                        pCreature->AddEquipActions(pItem, nInventorySlot, 0, 0, bInstant, 0, 0);
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053C010
int CSWVirtualMachineCommands::ExecuteCommandGetCreatureStat194(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    int nResult = 0;
    if (pCreature && pCreature->m_pStats) {
        nResult = pCreature->m_pStats->m_n194;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053C1B0
int CSWVirtualMachineCommands::ExecuteCommandGetAreaValue(int nCommandId, int nParameters)
{
    unsigned long oidArea;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        nResult = pArea->m_pProperties->m_n10;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00541280
int CSWVirtualMachineCommands::ExecuteCommandPlayAreaSoundAtLocation(int nCommandId, int nParameters)
{
    CScriptLocation* pLocation;
    int nSound;
    if (!g_pVirtualMachine->StackPopInteger(&nSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature) {
        CSWSArea* pArea = pCreature->GetArea();
        if (pArea) {
            pArea->PlaySoundAtLocation(nSound, pLocation);
        }
        if (pLocation) {
            delete pLocation;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00541970
int CSWVirtualMachineCommands::ExecuteCommandSelfObjectAction(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->AddObjectAction(oidTarget, &pCreature->m_vPosition, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543130
int CSWVirtualMachineCommands::ExecuteCommandSetLocal(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nIndex;
    int nValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nIndex)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        CSWVarTable* pVarTable;
        int nObjectType = pObject->m_nObjectType;
        if (nObjectType == OBJECT_TYPE_AREA) {
            pVarTable = &pObject->AsSWSArea()->m_VarTable;
        } else if (nObjectType == OBJECT_TYPE_MODULE) {
            pVarTable = &pObject->AsSWSModule()->m_VarTable;
        } else {
            pVarTable = &((CSWSObjectVarTableView*)pObject)->m_VarTable;
        }
        if (pVarTable) {
            switch (nCommandId) {
            case 0x2a8:
                pVarTable->SetLocalBoolean(nIndex, nValue);
                break;
            case 0x2aa:
                if (nValue >= -128 && nValue <= 127) {
                    pVarTable->SetLocalNumber(nIndex, nValue);
                } else {
                    ExoDebugLog(pObject->m_sTag.CStr(), pObject->m_sTag.CStr());
                }
                break;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00548310
int CSWVirtualMachineCommands::ExecuteCommandDoorAction(int nCommandId, int nParameters)
{
    unsigned long oidDoor;
    if (!g_pVirtualMachine->StackPopObject(&oidDoor)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSDoor* pDoor = g_pAppManager->GetServerExoApp()->GetDoorByGameObjectID(oidDoor);
    if (pDoor) {
        pDoor->DoorAction(0, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00541170
int CSWVirtualMachineCommands::ExecuteCommandPlaySound(int nCommandId, int nParameters)
{
    CExoString sSoundName;
    if (!g_pVirtualMachine->StackPopString(&sSoundName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_bCommandable) {
        ((CSWSObject*)pObject)->AddAction(0x17, 0xffff, 4, &sSoundName);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053BEC0
int CSWVirtualMachineCommands::ExecuteCommandGetModuleName(int nCommandId, int nParameters)
{
    CExoString sName;
    switch (nCommandId) {
    case 0x231: {
        CExoLocString lsName(g_pAppManager->GetServerExoApp()->GetModule()->m_lsModuleName);
        int nLanguage = g_pAppManager->GetServerExoApp()->GetModuleLanguage();
        lsName.GetStringLoc(nLanguage, sName, 0);
        break;
    }
    case 0xd2: {
        sName = g_pAppManager->GetServerExoApp()->GetModule()->m_sModuleName;
        int nColon = sName.Find(':', 0);
        if (nColon != -1) {
            sName = sName.Right(sName.GetLength() - nColon - 1);
        }
        break;
    }
    }
    if (!g_pVirtualMachine->StackPushString(sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543030
int CSWVirtualMachineCommands::ExecuteCommandSetListenPattern(int nCommandId, int nParameters)
{
    CExoString sPattern;
    unsigned long oidListener;
    int nNumber;
    if (!g_pVirtualMachine->StackPopObject(&oidListener) ||
        !g_pVirtualMachine->StackPopString(&sPattern) ||
        !g_pVirtualMachine->StackPopInteger(&nNumber)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidListener, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        ((CSWSObject*)pObject)->SetListenExpression(sPattern, nNumber);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053C080
int CSWVirtualMachineCommands::ExecuteCommandGetName(int nCommandId, int nParameters)
{
    CExoString sName;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        CSWSMessage::GetLocStringServer(-1, *pObject->AsSWSObject()->GetFirstName(),
                                        *pObject->AsSWSObject()->GetLastName(), sName, 0);
    }
    if (!g_pVirtualMachine->StackPushString(sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005417C0
int CSWVirtualMachineCommands::ExecuteCommandPrintString(int nCommandId, int nParameters)
{
    int bPrefix;
    CExoString sString;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&bPrefix)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else {
        bPrefix = 0;
    }
    if (bPrefix == 1) {
        if (!g_bExoDebugDisabled) {
            ExoDebugLog(sString.CStr());
        }
        g_pExoBase->m_pcExoDebug->m_sScriptLog.Format("PRINTSTRING: %s\n", sString.CStr());
    } else {
        if (!g_bExoDebugDisabled) {
            ExoDebugLog(sString.CStr());
        }
        g_pExoBase->m_pcExoDebug->m_sScriptLog.Format("%s\n", sString.CStr());
    }
    if (!g_bExoDebugDisabled) {
        g_pExoBase->m_pcExoDebug->WriteToScriptLogFile(g_pExoBase->m_pcExoDebug->m_sScriptLog);
        g_pExoBase->m_pcExoDebug->FlushScriptLogFile();
    }
    return 0;
}

class CSWCCMessageData {
public:
    CSWCCMessageData();                                     // 0x004D69F0
    ~CSWCCMessageData();                                    // 0x004D6A40
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0
    void SetString(int nIndex, const CExoString& sValue);   // 0x004D6AE0
    void SetObjectID(int nIndex, unsigned long oid);        // 0x004D7780

    char pad_0x0[0x34];
};

extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634 (5)

// FUNCTION: SWKOTOR 0x00535DB0
int CSWVirtualMachineCommands::ExecuteCommandFloatingText(int nCommandId, int nParameters)
{
    CExoString sString;
    int nStrRef;
    int nValue;
    unsigned long oidCreature;
    int bBroadcastToFaction = 1;
    if (nCommandId == 0x20d) {
        if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        nStrRef = nValue;
    } else if (nCommandId == 0x20e) {
        if (!g_pVirtualMachine->StackPopString(&sString)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&bBroadcastToFaction)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidCreature);
    if (pObject != NULL && pObject->m_nObjectType == OBJECT_TYPE_CREATURE && pObject->AsSWSObject() != NULL) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        if (pData) {
            pData->SetObjectID(0, oidCreature);
            if (nCommandId == 0x20d) {
                pData->SetInteger(9, 0x5d);
                pData->SetInteger(0, nStrRef);
            } else if (nCommandId == 0x20e) {
                pData->SetInteger(9, 0x5e);
                pData->SetString(0, sString);
            }
            if (!bBroadcastToFaction) {
                CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidCreature);
                if (pPlayer) {
                    g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->GetPlayerId(), 0xb, pData, 0);
                }
            }
            delete pData;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005483F0
int CSWVirtualMachineCommands::ExecuteCommandAddJournalQuestEntry(int nCommandId, int nParameters)
{
    CExoString sPlotID;
    int nState;
    int bAllowOverrideHigher = 0;
    if (!g_pVirtualMachine->StackPopString(&sPlotID)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nState)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&bAllowOverrideHigher)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nState < 0) {
        nState = abs(nState);
    }
    unsigned long nDate;
    unsigned long nTime;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDate, &nTime);
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (!pList) {
        return 1;
    }
    int nXP = 0;
    void* pos = pList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
        if (pPlayer && pPlayer->GetGameObject()) {
            CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
            if (pCreature) {
                if (bAllowOverrideHigher || pCreature->GetJournal()->GetState(sPlotID) < nState) {
                    pCreature->GetJournal()->SetDate(sPlotID, nDate, 0);
                    pCreature->GetJournal()->SetTime(sPlotID, nTime, 0);
                    pCreature->GetJournal()->SetState(sPlotID, nState, 0);
                    nXP = pCreature->GetJournal()->GetQuestEntryXP(sPlotID);
                }
            }
        }
        pList->GetNext(pos);
    }
    if (nXP && g_pAppManager->GetServerExoApp()->GetPartyTable()) {
        g_pAppManager->GetServerExoApp()->GetPartyTable()->DistributeExperience(nXP, 1);
    }
    return 0;
}
