// CSWVirtualMachineCommands: globals, scripts, tags, messages and other
// small commands.
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/global_variable_table.h"
#include "b/party_table.h"
#include <stdlib.h>

// Callee classes used only here.
struct STR_RES {
    CExoString sString;                                     // 0x00
    CResRef cSoundResRef;                                   // 0x08
    float fSoundLength;                                     // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                              // 0x007A3A08
extern unsigned char g_nGender;                             // 0x0073ECD0

class CSWCCMessageData {
public:
    CSWCCMessageData();                                     // 0x004D69F0
    void SetString(int nIndex, const CExoString& sValue);   // 0x004D6AE0
    char pad_0x0[0x34];
};

class CSWSCreature {
public:
    void SendFeedbackMessage(unsigned short nFeedback, CSWCCMessageData* pData); // 0x004EDE10
};

class CSWSModule {
public:
    unsigned long FindObjectByTagOrdinal(const CExoString& sTag, int nNth); // 0x004C5FE0 (name guessed)
};

class CSWCRoom {
public:
    void PlayAnimation(int nAnimation);                     // 0x00646180 (name guessed)
};

class CSWCArea {
public:
    CSWCRoom* GetRoom(const CExoString& sRoomName);         // 0x00605510 (name guessed)
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                                      // 0x48
};

extern int g_nScriptVar;                                    // 0x00832828 (.data)

// FUNCTION: SWKOTOR 0x00535B70
int CSWVirtualMachineCommands::ExecuteCommandExecuteScript(int nCommandId, int nParameters)
{
    CExoString sScript;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopString(&sScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3) {
        int nScriptVar = -1;
        if (!g_pVirtualMachine->StackPopInteger(&nScriptVar)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        g_nScriptVar = nScriptVar;
    }
    if (m_bValidObjectRunScript == 1) {
        g_pVirtualMachine->RunScript(&sScript, oidTarget, 1);
    } else {
        g_pVirtualMachine->RunScript(&sScript, INVALID_OBJECT_ID, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00535C70
int CSWVirtualMachineCommands::ExecuteCommandExportAllCharacters(int nCommandId, int nParameters)
{
    g_pAppManager->GetServerExoApp()->SetExportAllCharactersPending(1);
    return 0;
}

// FUNCTION: SWKOTOR 0x00538CD0
int CSWVirtualMachineCommands::ExecuteCommandGetGlobalLocation(int nCommandId, int nParameters)
{
    CExoString sName;
    CScriptLocation cLocation;
    if (!g_pVirtualMachine->StackPopString(&sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->GetValueLocation(sName, &cLocation);
    if (!g_pVirtualMachine->StackPushEngineStructure(2, &cLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538DC0
int CSWVirtualMachineCommands::ExecuteCommandGetGlobalNumber(int nCommandId, int nParameters)
{
    CExoString sName;
    char nValue = 0;
    int bValue;
    if (!g_pVirtualMachine->StackPopString(&sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    switch (nCommandId) {
    case 0x242: {
        g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->GetValueBoolean(sName, &bValue);
        nValue = bValue != 0;
        break;
    }
    case 0x244:
        g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->GetValueNumber(sName, (unsigned char*)&nValue);
        break;
    }
    if (!g_pVirtualMachine->StackPushInteger(nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053C280
int CSWVirtualMachineCommands::ExecuteCommandGetObjectByTag(int nCommandId, int nParameters)
{
    CExoString sTag;
    int nNth;
    if (!g_pVirtualMachine->StackPopString(&sTag) ||
        !g_pVirtualMachine->StackPopInteger(&nNth)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidResult = g_pAppManager->GetServerExoApp()->GetModule()->FindObjectByTagOrdinal(sTag.LowerCase(), nNth);
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053DCA0
int CSWVirtualMachineCommands::ExecuteCommandGetStrRefSoundDuration(int nCommandId, int nParameters)
{
    int nStrRef;
    STR_RES strRes;
    if (!g_pVirtualMachine->StackPopInteger(&nStrRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nStrRef < 0 || !g_pTlkTable->Fetch(nStrRef, strRes, g_nGender)) {
        strRes.fSoundLength = 0.0f;
    }
    if (!g_pVirtualMachine->StackPushFloat(strRes.fSoundLength)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053EAA0
int CSWVirtualMachineCommands::ExecuteCommandIntToFloat(int nCommandId, int nParameters)
{
    int nInteger;
    if (!g_pVirtualMachine->StackPopInteger(&nInteger)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    float fValue = (float)nInteger;
    return g_pVirtualMachine->StackPushFloat(fValue) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00540DD0
int CSWVirtualMachineCommands::ExecuteCommandPlayRoomAnimation(int nCommandId, int nParameters)
{
    CExoString sRoom;
    int nAnimation;
    if (!g_pVirtualMachine->StackPopString(&sRoom)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAnimation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
    if (pModule) {
        CSWCArea* pArea = pModule->m_pArea;
        if (pArea) {
            CSWCRoom* pRoom = pArea->GetRoom(sRoom.CStr());
            if (pRoom) {
                pRoom->PlayAnimation(nAnimation);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542300
int CSWVirtualMachineCommands::ExecuteCommandSendMessageToPC(int nCommandId, int nParameters)
{
    unsigned long oidPlayer;
    CExoString sMessage;
    if (nParameters != 2) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidPlayer)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sMessage)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidPlayer);
    if (pCreature) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetString(0, sMessage);
        pCreature->SendFeedbackMessage(0xcc, pData);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542A50
int CSWVirtualMachineCommands::ExecuteCommandSetGlobalLocation(int nCommandId, int nParameters)
{
    CExoString sName;
    CScriptLocation* pLocation;
    if (!g_pVirtualMachine->StackPopString(&sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pLocation) {
        g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->SetValueLocation(sName, pLocation);
        delete pLocation;
        pLocation = NULL;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542B60
int CSWVirtualMachineCommands::ExecuteCommandSetGlobalNumber(int nCommandId, int nParameters)
{
    CExoString sName;
    int nValue;
    if (!g_pVirtualMachine->StackPopString(&sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    switch (nCommandId) {
    case 0x243: {
        int bValue = nValue != 0;
        g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->SetValueBoolean(sName, bValue);
        break;
    }
    case 0x245:
        g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->SetValueNumber(sName, nValue);
        break;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005473E0
int CSWVirtualMachineCommands::ExecuteCommandStartCreditSequence(int nCommandId, int nParameters)
{
    int bTransparentBackground;
    CExoString sMovie;
    if (!g_pVirtualMachine->StackPopInteger(&bTransparentBackground)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1) {
        if (!g_pVirtualMachine->StackPopString(&sMovie)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    if (bTransparentBackground) {
        Vector vColor(-1.0f, -1.0f, -1.0f);
        g_pAppManager->GetClientExoApp()->StartCreditSequence(vColor, sMovie);
    } else {
        Vector vColor(0.0f, 0.0f, 0.0f);
        g_pAppManager->GetClientExoApp()->StartCreditSequence(vColor, sMovie);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00535570
int CSWVirtualMachineCommands::ExecuteCommandEndGame(int nCommandId, int nParameters)
{
    int bShowEndGameGui = 1;
    if (nParameters >= 1 && !g_pVirtualMachine->StackPopInteger(&bShowEndGameGui)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    float fDelay = bShowEndGameGui ? 5.0f : 0.0f;
    g_pAppManager->GetClientExoApp()->FadeToBlack(1, fDelay, bShowEndGameGui);
    if (bShowEndGameGui) {
        g_pAppManager->GetClientExoApp()->SetGameOver(1);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00535FB0
int CSWVirtualMachineCommands::ExecuteCommandFloatToInt(int nCommandId, int nParameters)
{
    float fValue;
    if (!g_pVirtualMachine->StackPopFloat(&fValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return g_pVirtualMachine->StackPushInteger((int)fValue) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00541910
int CSWVirtualMachineCommands::ExecuteCommandPopVectorAndInteger(int nCommandId, int nParameters)
{
    int nValue;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopVector(&vPosition)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005419D0
int CSWVirtualMachineCommands::ExecuteCommandRandom(int nCommandId, int nParameters)
{
    int nMaxInteger;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nMaxInteger)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nMaxInteger) {
        nResult = rand() % nMaxInteger;
        if (nMaxInteger < 0) {
            nResult = -nResult;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00544E50
int CSWVirtualMachineCommands::ExecuteCommandTimeToSeconds(int nCommandId, int nParameters)
{
    float fSeconds;
    int nValue = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    switch (nCommandId) {
    case 0x79:
        fSeconds = nValue * 3.0f;
        break;
    case 0x7a: {
        int nMinutesPerHour = g_pAppManager->GetServerExoApp()->GetWorldTimer()->m_nMinutesPerHour;
        fSeconds = (float)nMinutesPerHour * nValue * 60.0f;
        break;
    }
    case 0x7b:
        fSeconds = nValue * 30.0f;
        break;
    }
    return g_pVirtualMachine->StackPushFloat(fSeconds) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00548180
int CSWVirtualMachineCommands::ExecuteCommandSetNPCObject(int nCommandId, int nParameters)
{
    int nNPC;
    unsigned long oidNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetNPCObjectId(nNPC, oidNPC);
    return 0;
}

// FUNCTION: SWKOTOR 0x005481E0
int CSWVirtualMachineCommands::ExecuteCommandIsMoviePlaying(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetClientExoApp()->IsMoviePlaying()) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005482D0
int CSWVirtualMachineCommands::ExecuteCommandQueueMovieById(int nCommandId, int nParameters)
{
    int nMovie;
    if (!g_pVirtualMachine->StackPopInteger(&nMovie)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->QueueMovie(nMovie);
    return 0;
}

// FUNCTION: SWKOTOR 0x00548210
int CSWVirtualMachineCommands::ExecuteCommandQueueMovie(int nCommandId, int nParameters)
{
    CExoString sMovie;
    int bSkippable;
    if (!g_pVirtualMachine->StackPopString(&sMovie)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bSkippable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->AddMovieToMovieQueue(sMovie, bSkippable);
    return 0;
}

class CExoInput {
public:
    void ClearEvents();                                     // 0x005DF4E0
};

class CExoSound {
public:
    void SetSoundMode(int nMode);                           // 0x005D5E80
};

extern CExoInput* g_pExoInput;                              // 0x007A39E4
extern CExoSound* g_pExoSound;                              // 0x007A39EC
extern const int SOUND_MODE_NORMAL;                         // 0x0074C5EC (0)
extern const int SOUND_MODE_MOVIE;                          // 0x0074C5F8 (3)

// FUNCTION: SWKOTOR 0x00540ED0
int CSWVirtualMachineCommands::ExecuteCommandPlayMovie(int nCommandId, int nParameters)
{
    CExoString sMovie;
    if (!g_pVirtualMachine->StackPopString(&sMovie)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pExoInput->ClearEvents();
    g_pAppManager->GetClientExoApp()->ResetDriveAcceleration();
    g_pExoSound->SetSoundMode(SOUND_MODE_MOVIE);
    g_pAppManager->GetClientExoApp()->HideLoadScreen(0, 1);
    int nResult;
    g_pAppManager->GetClientExoApp()->PlayMovie(sMovie, &nResult, 0, 0);
    g_pExoSound->SetSoundMode(SOUND_MODE_NORMAL);
    return 0;
}

// FUNCTION: SWKOTOR 0x00540FD0
int CSWVirtualMachineCommands::ExecuteCommandStartPazaakGame(int nCommandId, int nParameters)
{
    int nOpponentDeck;
    if (!g_pVirtualMachine->StackPopInteger(&nOpponentDeck)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sEndScript;
    int nMaxWager;
    if (!g_pVirtualMachine->StackPopString(&sEndScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nMaxWager)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bShowTutorial = 0;
    if (nParameters >= 4 && !g_pVirtualMachine->StackPopInteger(&bShowTutorial)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidOpponent = INVALID_OBJECT_ID;
    if (nParameters >= 5 && !g_pVirtualMachine->StackPopObject(&oidOpponent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->StartPazaakGame(nOpponentDeck, sEndScript, nMaxWager,
                                                     bShowTutorial != 0, oidOpponent);
    return 0;
}

// FUNCTION: SWKOTOR 0x00541120
int CSWVirtualMachineCommands::ExecuteCommandPlayRumblePattern(int nCommandId, int nParameters)
{
    int nPattern;
    if (!g_pVirtualMachine->StackPopInteger(&nPattern)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetClientExoApp()->PlayRumblePattern(nPattern)) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
