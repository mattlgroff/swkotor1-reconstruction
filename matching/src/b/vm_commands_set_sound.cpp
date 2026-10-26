// CSWVirtualMachineCommands: Set*/Show*/SoundObject* handlers
// (0x005424E0-0x00544910).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_message.h"
#include "b/party_table.h"

extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633
extern int g_bShipBuild;                                    // 0x00832834 (name guessed)

void AurEnableSpeedBlur();                                  // 0x0044F0A0
void AurDisableSpeedBlur();                                 // 0x0044F0B0
void AurSetSpeedBlurRatio(float fRatio);                    // 0x0044F130

class CSWSSoundObject {
public:
    void Play();                                            // 0x005C8BD0
    void Stop();                                            // 0x005C8C70
    void FadeAndStop(int nMilliseconds);                    // 0x005C8D10
    void ChangePitchVariation(float fVariation);            // 0x005C8D50
    void ChangeFixedVariance(float fVariance);              // 0x005C8DA0
    void ChangeVolume(int nVolume);                         // 0x005C8DF0
    void ChangePosition(Vector vPosition);                  // 0x005C8E50

    char pad_0x0[0x234];
    int m_nVolume;                                          // 0x234
    char pad_0x238[0x240 - 0x238];
    float m_fPitchVariation;                                // 0x240
    char pad_0x244[0x248 - 0x244];
    float m_fFixedVariance;                                 // 0x248
};

class CSWGuiInGame {
public:
    int ShowLevelUpGUI(int bShow);                          // 0x0062DC00 (name guessed)
    void ShowUpgradeScreen(unsigned long oidItem);          // 0x0062E760
};

class CSWCCreatureLightsaber {
public:
    void SetLightsaberPowered(int bPowered, int bInstant, int bSound); // 0x0060FB80 (name guessed)
};

class CSWSCreatureStatsXP {
public:
    void SetExperience(int nXP, int bSignal);               // 0x005AF480 (name guessed)
};

struct CSWSObjectListeningView {
    char pad_0x0[0x19c];
    int m_bListening;                                       // 0x19c
};

struct CSWSObjectPlotView {
    char pad_0x0[0xf8];
    int m_bPlotFlag;                                        // 0xf8
};

// FUNCTION: SWKOTOR 0x005424E0
int CSWVirtualMachineCommands::ExecuteCommandSetAreaUnescapable(int nCommandId, int nParameters)
{
    int bUnescapable;
    if (!g_pVirtualMachine->StackPopInteger(&bUnescapable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        CSWSArea* pArea = pModule->GetArea();
        if (pArea) {
            pArea->m_bUnescapable = bUnescapable;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542640
int CSWVirtualMachineCommands::ExecuteCommandSetCameraMode(int nCommandId, int nParameters)
{
    unsigned long oidPlayer;
    int nMode;
    if (!g_pVirtualMachine->StackPopObject(&oidPlayer)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nMode)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
    if (pPlayer) {
        CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
        if (pMessage) {
            pMessage->SendServerToPlayerCamera_SetMode(pPlayer, (unsigned char)nMode);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005426C0
int CSWVirtualMachineCommands::ExecuteCommandSetCommandable(int nCommandId, int nParameters)
{
    unsigned long oidTarget = m_oidObjectRunScript;
    CGameObject* pObject;
    int bCommandable;
    if (!g_pVirtualMachine->StackPopInteger(&bCommandable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->m_nObjectType > OBJECT_TYPE_AREA) {
            pObject->m_bCommandable = bCommandable != 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542F10
int CSWVirtualMachineCommands::ExecuteCommandSetLightsaberPowered(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int bSound;
    int bInstant;
    int bPowered;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bPowered)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bInstant)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
    CSWCCreatureLightsaber* pCreature =
        (CSWCCreatureLightsaber*)pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(oidCreature));
    if (pCreature) {
        pCreature->SetLightsaberPowered(bPowered, bInstant, bSound);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542FB0
int CSWVirtualMachineCommands::ExecuteCommandSetListening(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    CGameObject* pObject;
    int bListening;
    if (!g_pVirtualMachine->StackPopObject(&oidObject) ||
        !g_pVirtualMachine->StackPopInteger(&bListening)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        ((CSWSObjectListeningView*)pObject)->m_bListening = bListening;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543300
int CSWVirtualMachineCommands::ExecuteCommandSetNPCSelectability(int nCommandId, int nParameters)
{
    int nSelectability;
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nSelectability)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetNPCSelectable(nNPC, (unsigned char)nSelectability);
    return 0;
}

// FUNCTION: SWKOTOR 0x005433D0
int CSWVirtualMachineCommands::ExecuteCommandSetPlotFlag(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    int bPlotFlag;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bPlotFlag)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject && pObject->AsSWSObject()) {
        ((CSWSObjectPlotView*)pObject->AsSWSObject())->m_bPlotFlag = bPlotFlag;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543450
int CSWVirtualMachineCommands::ExecuteCommandSetPlayerRestrictMode(int nCommandId, int nParameters)
{
    int bRestrict;
    if (!g_pVirtualMachine->StackPopInteger(&bRestrict)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetModule() &&
        g_pAppManager->GetServerExoApp()->GetModule()->GetArea()) {
        g_pAppManager->GetServerExoApp()->GetModule()->GetArea()->SetPlayerRestrictMode(bRestrict);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005434C0
int CSWVirtualMachineCommands::ExecuteCommandSetReturnStrref(int nCommandId, int nParameters)
{
    int nReturnQueryStrRef;
    int nStringRef;
    int bShow;
    nReturnQueryStrRef = 0;
    if (!g_pVirtualMachine->StackPopInteger(&bShow)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nStringRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 2 && !g_pVirtualMachine->StackPopInteger(&nReturnQueryStrRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetPartyValues(bShow, nStringRef, nReturnQueryStrRef);
    return 0;
}

// FUNCTION: SWKOTOR 0x00543550
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetSpeedBlurEffect(int nCommandId, int nParameters)
{
    int bEnabled;
    if (!g_pVirtualMachine->StackPopInteger(&bEnabled)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2) {
        float fRatio = 0.75f;
        if (!g_pVirtualMachine->StackPopFloat(&fRatio)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        AurSetSpeedBlurRatio(fRatio);
    }
    if (bEnabled) {
        AurEnableSpeedBlur();
    } else {
        AurDisableSpeedBlur();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543740
int CSWVirtualMachineCommands::ExecuteCommandSetForcePowerUnsuccessful(int nCommandId, int nParameters)
{
    unsigned long nResult = 0;
    unsigned long oidCreature = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&nResult) ||
        !g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        pCreature->m_bLastForcePowerSuccessful = nResult;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005437B0
int CSWVirtualMachineCommands::ExecuteCommandSetXP(int nCommandId, int nParameters)
{
    int nXP;
    unsigned long oidCreature;
    CGameObject* pObject;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nXP)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nXP >= 0) {
        int nNewXP = nXP;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
            pObject) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature) {
                ((CSWSCreatureStatsXP*)pCreature->m_pStats)->SetExperience(nNewXP, 1);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543840
int CSWVirtualMachineCommands::ExecuteCommandShipBuild(int nCommandId, int nParameters)
{
    g_bShipBuild = 1;
    return g_pVirtualMachine->StackPushInteger(1) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00543870
int CSWVirtualMachineCommands::ExecuteCommandShowLevelUpGUI(int nCommandId, int nParameters)
{
    int bShown = g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowLevelUpGUI(1);
    return g_pVirtualMachine->StackPushInteger(bShown) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00543990
int CSWVirtualMachineCommands::ExecuteCommandShowUpgradeScreen(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowUpgradeScreen(oidItem);
    return 0;
}

// FUNCTION: SWKOTOR 0x00543AA0
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectGetFixedVariance(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    float fVariance;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        fVariance = pSound->m_fFixedVariance;
    }
    return g_pVirtualMachine->StackPushFloat(fVariance) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00543B10
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectGetPitchVariance(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    float fVariance;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        fVariance = pSound->m_fPitchVariation;
    }
    return g_pVirtualMachine->StackPushFloat(fVariance) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00543B80
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectGetVolume(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    int nVolume;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        nVolume = pSound->m_nVolume;
    }
    return g_pVirtualMachine->StackPushInteger(nVolume) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00543BE0
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectPlay(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->Play();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543C30
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectSetFixedVariance(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    float fVariance;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fVariance)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->ChangeFixedVariance(fVariance);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543CA0
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectSetPitchVariance(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    float fVariance;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fVariance)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->ChangePitchVariation(fVariance);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543D10
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectSetPosition(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    Vector vPosition;
    vPosition.x = 0.0f;
    vPosition.y = 0.0f;
    vPosition.z = 0.0f;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopVector(&vPosition)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->ChangePosition(vPosition);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543DA0
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectSetVolume(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    int nVolume;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nVolume)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->ChangeVolume(nVolume);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543E10
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectStop(int nCommandId, int nParameters)
{
    unsigned long oidSound;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->Stop();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543E60
int CSWVirtualMachineCommands::ExecuteCommandSoundObjectFadeAndStop(int nCommandId, int nParameters)
{
    float fSeconds;
    unsigned long oidSound;
    if (!g_pVirtualMachine->StackPopObject(&oidSound)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fSeconds)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nMilliseconds = (int)(fSeconds * 1000.0f);
    CSWSSoundObject* pSound = g_pAppManager->GetServerExoApp()->GetSoundObjectByGameObjectID(oidSound);
    if (pSound) {
        pSound->FadeAndStop(nMilliseconds);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005446C0
int CSWVirtualMachineCommands::ExecuteCommandStopRumblePattern(int nCommandId, int nParameters)
{
    int nPattern;
    if (!g_pVirtualMachine->StackPopInteger(&nPattern)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = g_pAppManager->GetClientExoApp()->StopRumblePattern(nPattern);
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00544910
int CSWVirtualMachineCommands::ExecuteCommandSwitchPlayerCharacter(int nCommandId, int nParameters)
{
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bResult = g_pAppManager->GetServerExoApp()->GetPartyTable()->SwitchPlayerCharacter(nNPC);
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
