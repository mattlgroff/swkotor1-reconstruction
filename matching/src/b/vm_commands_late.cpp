// CSWVirtualMachineCommands: late K1 handlers (party AI style, fades,
// planets, stealth XP, dialog camera and tutorial windows),
// 0x005459A0-0x00547EF0.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/party_table.h"

extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634

class CExoSound {
public:
    void SetMusicVolume(float fVolume, int bArg);           // 0x005D5C50
};
extern CExoSound* g_pExoSound;                              // 0x007A39EC

class CSWGuiInGame {
public:
    void StartGlobalFade(int bFadeIn, float fWait, float fLength, Vector* pColor); // 0x0062ABF0
    void ShowGalaxyMapGui(int nPlanet);                     // 0x0062D040
    void SetDialogPlaceableCamera(int nCameraId, int nArg); // 0x0062E900 (name guessed)
    void SetGlobalDialogState(int nState);                  // 0x0062EC60
    void SuppressStatusSummary(int bSuppress);              // 0x0062F0C0
    void SetLockObjectOrientationInDialog(unsigned long oid, int bLock); // 0x0062F160
    void SetLockHeadFollowInDialog(unsigned long oid, int bLock); // 0x0062F2B0
    void MarkNoClickEvent(unsigned long nMilliseconds);     // 0x0062F960

    char pad_0x0[0x2c];
    int m_nSubScreen;                                       // 0x2c
    void* m_pSubScreen;                                     // 0x30
    char pad_0x34[0xb98 - 0x34];
    int m_bHoldWorldFadeIn;                                 // 0xb98
    char pad_0xb9c[0xba4 - 0xb9c];
    unsigned long m_oidPostDialogCharacter;                 // 0xba4
    char pad_0xba8[0xc20 - 0xba8];
    int m_nCheatCode;                                       // 0xc20
};

// CSWSObject members past CGameObject (CSWSObject itself stays data-free).
struct CSWSObjectHitPointsView {
    char pad_0x0[0xe0];
    int m_nMaxHitPoints;                                    // 0xe0
    char pad_0xe4[0x200 - 0xe4];
    int m_bMinOneHP;                                        // 0x200
};

class CPathfindInformation {
public:
    char pad_0x0[0x254];
    unsigned long m_oidBlockingCreature;                    // 0x254
    char pad_0x258[0x264 - 0x258];
    unsigned long m_oidFoundEnemyCreature;                  // 0x264
};

struct CSWSCreaturePathfindView {
    char pad_0x0[0x340];
    CPathfindInformation* m_pPathfindInformation;           // 0x340
};

// FUNCTION: SWKOTOR 0x005459A0
int CSWVirtualMachineCommands::ExecuteCommandGetPartyAIStyle(int nCommandId, int nParameters)
{
    int nStyle = g_pAppManager->GetServerExoApp()->GetPartyTable()->m_nE4;
    return g_pVirtualMachine->StackPushInteger(nStyle) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005459D0
int CSWVirtualMachineCommands::ExecuteCommandGetNPCAIStyle(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    unsigned long oidCreature;
    int nStyle = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        nStyle = pObject->AsSWSCreature()->m_pStats->m_nAIStyle;
    }
    return g_pVirtualMachine->StackPushInteger(nStyle) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00545C50
int CSWVirtualMachineCommands::ExecuteCommandSetPartyAIStyle(int nCommandId, int nParameters)
{
    int nStyle = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nStyle)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetPartyAIStyle(nStyle);
    return 0;
}

// FUNCTION: SWKOTOR 0x00545CA0
int CSWVirtualMachineCommands::ExecuteCommandSetNPCAIStyle(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    int nStyle = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nStyle)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_pStats->m_nAIStyle = (unsigned short)nStyle;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00545D40
int CSWVirtualMachineCommands::ExecuteCommandClearAllEffects(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        static_cast<CSWSObject*>(pObject)->ClearAllEffects(0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00545D80
int CSWVirtualMachineCommands::ExecuteCommandGetStandardFaction(int nCommandId, int nParameters)
{
    int nFaction = -1;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    if (pServer->m_pInternal == NULL) {
        return g_pVirtualMachine->StackPushInteger(nFaction) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (pServer->m_pInternal->m_pFactionManager == NULL) {
        return g_pVirtualMachine->StackPushInteger(nFaction) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    pServer->GetFactionOfObject(oidObject, &nFaction);
    if (nFaction < 1 && nFaction > 6) {
        nFaction = -1;
    }
    return g_pVirtualMachine->StackPushInteger(nFaction) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00545F10
int CSWVirtualMachineCommands::ExecuteCommandGetMinOneHP(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int bMinOneHP;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject && pObject->AsSWSObject()) {
        bMinOneHP = ((CSWSObjectHitPointsView*)pObject->AsSWSObject())->m_bMinOneHP;
    }
    return g_pVirtualMachine->StackPushInteger(bMinOneHP) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00545F90
int CSWVirtualMachineCommands::ExecuteCommandSetMinOneHP(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int bMinOneHP;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bMinOneHP)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject && pObject->AsSWSObject()) {
        CSWSObjectHitPointsView* pView = (CSWSObjectHitPointsView*)pObject->AsSWSObject();
        pView->m_bMinOneHP = bMinOneHP != 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546010
int CSWVirtualMachineCommands::ExecuteCommandSetGlobalFadeIn(int nCommandId, int nParameters)
{
    float fRed;
    float fGreen;
    float fBlue;
    float fLength;
    float fWait;
    if (!g_pVirtualMachine->StackPopFloat(&fWait)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fLength)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fRed)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fGreen)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fBlue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    Vector vColor;
    vColor.x = fRed;
    vColor.y = fGreen;
    vColor.z = fBlue;
    g_pAppManager->GetClientExoApp()->GetInGameGui()->StartGlobalFade(1, fWait, fLength, &vColor);
    return 0;
}

// FUNCTION: SWKOTOR 0x005460D0
int CSWVirtualMachineCommands::ExecuteCommandSetGlobalFadeOut(int nCommandId, int nParameters)
{
    float fRed;
    float fGreen;
    float fBlue;
    float fLength;
    float fWait;
    if (!g_pVirtualMachine->StackPopFloat(&fWait)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fLength)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fRed)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fGreen)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fBlue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    Vector vColor;
    vColor.x = fRed;
    vColor.y = fGreen;
    vColor.z = fBlue;
    g_pAppManager->GetClientExoApp()->GetInGameGui()->StartGlobalFade(0, fWait, fLength, &vColor);
    return 0;
}

// FUNCTION: SWKOTOR 0x00546230
int CSWVirtualMachineCommands::ExecuteCommandSetFormation(int nCommandId, int nParameters)
{
    unsigned long oidAnchor = INVALID_OBJECT_ID;
    unsigned long oidCreature = INVALID_OBJECT_ID;
    int nPosition;
    int nFormation;
    if (!g_pVirtualMachine->StackPopObject(&oidAnchor)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nFormation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nPosition)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        pCreature->SetFormation(oidAnchor, nFormation, nPosition);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005462D0
int CSWVirtualMachineCommands::ExecuteCommandGetIsDebilitated(int nCommandId, int nParameters)
{
    unsigned long oidCreature = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bDebilitated = 0;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        bDebilitated = pCreature->IsDebilitated();
    }
    return g_pVirtualMachine->StackPushInteger(bDebilitated) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005464D0
int CSWVirtualMachineCommands::ExecuteCommandGetSubScreenID(int nCommandId, int nParameters)
{
    CSWGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
    int nSubScreen = 0;
    if (pGui && pGui->m_pSubScreen) {
        nSubScreen = pGui->m_nSubScreen + 1;
    }
    return g_pVirtualMachine->StackPushInteger(nSubScreen) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546510
int CSWVirtualMachineCommands::ExecuteCommandCancelCombat(int nCommandId, int nParameters)
{
    unsigned long oidCreature = INVALID_OBJECT_ID;
    int bArg = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bArg)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        pCreature->CancelCombat(bArg);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546590
int CSWVirtualMachineCommands::ExecuteCommandPauseGame(int nCommandId, int nParameters)
{
    int bPause;
    if (!g_pVirtualMachine->StackPopInteger(&bPause)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->SetPausedByCombat(bPause, 0, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x005467B0
int CSWVirtualMachineCommands::ExecuteCommandShowGalaxyMap(int nCommandId, int nParameters)
{
    int nPlanet;
    if (!g_pVirtualMachine->StackPopInteger(&nPlanet)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowGalaxyMapGui(nPlanet);
    return 0;
}

// FUNCTION: SWKOTOR 0x005467F0
int CSWVirtualMachineCommands::ExecuteCommandSetPlanetAvailable(int nCommandId, int nParameters)
{
    int bAvailable;
    int nPlanet;
    if (!g_pVirtualMachine->StackPopInteger(&nPlanet)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bAvailable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetToken(nPlanet, bAvailable);
    return 0;
}

// FUNCTION: SWKOTOR 0x00546850
int CSWVirtualMachineCommands::ExecuteCommandGetPlanetAvailable(int nCommandId, int nParameters)
{
    int nPlanet;
    if (!g_pVirtualMachine->StackPopInteger(&nPlanet)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bAvailable = g_pAppManager->GetServerExoApp()->GetPartyTable()->GetToken(nPlanet);
    return g_pVirtualMachine->StackPushInteger(bAvailable) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005468B0
int CSWVirtualMachineCommands::ExecuteCommandSetPlanetSelectable(int nCommandId, int nParameters)
{
    int bSelectable;
    int nPlanet;
    if (!g_pVirtualMachine->StackPopInteger(&nPlanet)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bSelectable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetTokenValue(nPlanet, bSelectable);
    return 0;
}

// FUNCTION: SWKOTOR 0x00546910
int CSWVirtualMachineCommands::ExecuteCommandGetPlanetSelectable(int nCommandId, int nParameters)
{
    int nPlanet;
    if (!g_pVirtualMachine->StackPopInteger(&nPlanet)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bSelectable = g_pAppManager->GetServerExoApp()->GetPartyTable()->GetTokenValue(nPlanet);
    return g_pVirtualMachine->StackPushInteger(bSelectable) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546970
int CSWVirtualMachineCommands::ExecuteCommandGetSelectedPlanet(int nCommandId, int nParameters)
{
    int nPlanet = g_pAppManager->GetServerExoApp()->GetPartyTable()->m_nLeaderNPC;
    return g_pVirtualMachine->StackPushInteger(nPlanet) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546A10
int CSWVirtualMachineCommands::ExecuteCommandGetButtonMashCheck(int nCommandId, int nParameters)
{
    int bCheck = g_pAppManager->GetServerExoApp()->GetButtonMashCheck();
    return g_pVirtualMachine->StackPushInteger(bCheck) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546A40
int CSWVirtualMachineCommands::ExecuteCommandSetButtonMashCheck(int nCommandId, int nParameters)
{
    int bCheck = 0;
    if (!g_pVirtualMachine->StackPopInteger(&bCheck)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->SetButtonMashCheck(bCheck);
    return 0;
}

// FUNCTION: SWKOTOR 0x00546A80
int CSWVirtualMachineCommands::ExecuteCommandSetDialogPlaceableCamera(int nCommandId, int nParameters)
{
    int nArg;
    int nCameraId;
    if (!g_pVirtualMachine->StackPopInteger(&nCameraId)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&nArg)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    nArg = -1;
    g_pAppManager->GetClientExoApp()->GetInGameGui()->SetDialogPlaceableCamera(nCameraId, nArg);
    return 0;
}

// FUNCTION: SWKOTOR 0x00546AF0
int CSWVirtualMachineCommands::ExecuteCommandGetSoloMode(int nCommandId, int nParameters)
{
    int bSoloMode = g_pAppManager->GetServerExoApp()->GetPartyTable()->m_bSoloMode;
    return g_pVirtualMachine->StackPushInteger(bSoloMode) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546B20
int CSWVirtualMachineCommands::ExecuteCommandGetMaxStealthXP(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    unsigned long nMax = 0;
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            nMax = pArea->m_nStealthXPMax;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nMax) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546B70
int CSWVirtualMachineCommands::ExecuteCommandSetMaxStealthXP(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    int nMax;
    if (!g_pVirtualMachine->StackPopInteger(&nMax)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            pArea->SetStealthXPMax(nMax);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546BD0
int CSWVirtualMachineCommands::ExecuteCommandGetCurrentStealthXP(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    unsigned long nCurrent = 0;
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            nCurrent = pArea->m_nStealthXPCurrent;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nCurrent) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546C20
int CSWVirtualMachineCommands::ExecuteCommandSetCurrentStealthXP(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    int nCurrent;
    if (!g_pVirtualMachine->StackPopInteger(&nCurrent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            pArea->SetStealthXPCurrent(nCurrent);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546C80
int CSWVirtualMachineCommands::ExecuteCommandAwardStealthXP(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        CSWSArea* pArea = pCreature->GetArea();
        if (pArea) {
            pArea->AwardStealthXP(pCreature->m_idSelf);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546CE0
int CSWVirtualMachineCommands::ExecuteCommandGetStealthXPEnabled(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    int bEnabled = 0;
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            bEnabled = pArea->m_bStealthXPEnabled;
        }
    }
    return g_pVirtualMachine->StackPushInteger(bEnabled) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546D30
int CSWVirtualMachineCommands::ExecuteCommandSetStealthXPEnabled(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    int bEnabled;
    if (!g_pVirtualMachine->StackPopInteger(&bEnabled)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            pArea->m_bStealthXPEnabled = bEnabled;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546D90
int CSWVirtualMachineCommands::ExecuteCommandGetSubRace(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nSubRace;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature && pCreature->m_pStats) {
        nSubRace = pCreature->m_pStats->m_nSubRace;
    }
    return g_pVirtualMachine->StackPushInteger(nSubRace) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546E00
int CSWVirtualMachineCommands::ExecuteCommandGetBlockingCreature(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    unsigned long oidBlocking = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        oidBlocking = ((CSWSCreaturePathfindView*)pCreature)->m_pPathfindInformation->m_oidBlockingCreature;
    }
    return g_pVirtualMachine->StackPushObject(oidBlocking) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546E70
int CSWVirtualMachineCommands::ExecuteCommandGetFoundEnemyCreature(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    unsigned long oidEnemy = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        oidEnemy = ((CSWSCreaturePathfindView*)pCreature)->m_pPathfindInformation->m_oidFoundEnemyCreature;
    }
    return g_pVirtualMachine->StackPushObject(oidEnemy) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546EE0
int CSWVirtualMachineCommands::ExecuteCommandGetStealthXPDecrement(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    unsigned long nLoss = 0;
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            nLoss = pArea->m_nStealthXPLoss;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nLoss) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00546F30
int CSWVirtualMachineCommands::ExecuteCommandSetStealthXPDecrement(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    int nLoss;
    if (!g_pVirtualMachine->StackPopInteger(&nLoss)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pPlayer) {
        CSWSArea* pArea = pPlayer->GetArea();
        if (pArea) {
            pArea->m_nStealthXPLoss = nLoss;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005470E0
int CSWVirtualMachineCommands::ExecuteCommandSetLockOrientationInDialog(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int bLock;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bLock)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (oidObject != INVALID_OBJECT_ID) {
        unsigned long oidClient = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidObject);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SetLockObjectOrientationInDialog(oidClient, bLock);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00547160
int CSWVirtualMachineCommands::ExecuteCommandSetLockHeadFollowInDialog(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int bLock;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bLock)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (oidObject != INVALID_OBJECT_ID) {
        unsigned long oidClient = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidObject);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SetLockHeadFollowInDialog(oidClient, bLock);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005471E0
int CSWVirtualMachineCommands::ExecuteCommandEnableVideoEffect(int nCommandId, int nParameters)
{
    int nEffect;
    if (!g_pVirtualMachine->StackPopInteger(&nEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->EnableVideoEffect(nEffect);
    return 0;
}

// FUNCTION: SWKOTOR 0x00547220
int CSWVirtualMachineCommands::ExecuteCommandDisableVideoEffect(int nCommandId, int nParameters)
{
    g_pAppManager->GetClientExoApp()->DisableVideoEffect();
    return 0;
}

// FUNCTION: SWKOTOR 0x005472F0
int CSWVirtualMachineCommands::ExecuteCommandSetTutorialWindowsEnabled(int nCommandId, int nParameters)
{
    int bEnabled;
    if (!g_pVirtualMachine->StackPopInteger(&bEnabled)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetClientExoApp()->GetOptions()->SetTutorialWindowsEnabled(bEnabled);
    return 0;
}

// FUNCTION: SWKOTOR 0x00547340
int CSWVirtualMachineCommands::ExecuteCommandShowTutorialWindow(int nCommandId, int nParameters)
{
    int nWindow;
    if (!g_pVirtualMachine->StackPopInteger(&nWindow)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    switch (nWindow) {
    case 0:
        nWindow = 9;
        break;
    case 1:
        nWindow = 40;
        break;
    case 2:
        nWindow = 42;
        break;
    default:
        return 0;
    }
    g_pAppManager->GetClientExoApp()->ShowTutorialWindow(nWindow, INVALID_OBJECT_ID, INVALID_OBJECT_ID, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x005473B0
int CSWVirtualMachineCommands::ExecuteCommandIsCreditSequenceInProgress(int nCommandId, int nParameters)
{
    int bInProgress = g_pAppManager->GetClientExoApp()->GetCreditSequenceInProgress();
    return g_pVirtualMachine->StackPushInteger(bInProgress) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00547BC0
int CSWVirtualMachineCommands::ExecuteCommandResetDialogState(int nCommandId, int nParameters)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->SetGlobalDialogState(0);
    return 0;
}

// FUNCTION: SWKOTOR 0x00547BE0
int CSWVirtualMachineCommands::ExecuteCommandSetGoodEvilValue(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nValue;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        pStats->ModifyAlignment((short)((unsigned short)nValue - pStats->m_nGoodEvil));
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00547C50
int CSWVirtualMachineCommands::ExecuteCommandGetIsPoisoned(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bPoisoned = 0;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject && pObject->AsSWSObject()) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        if (pSWSObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
            bPoisoned = pSWSObject->AsSWSCreature()->m_bPoisoned;
        }
    }
    return g_pVirtualMachine->StackPushInteger(bPoisoned) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// The original returns the overflow code when the argument pop fails.
// FUNCTION: SWKOTOR 0x00547CE0
int CSWVirtualMachineCommands::ExecuteCommandSetSoloMode(int nCommandId, int nParameters)
{
    int bSoloMode;
    if (!g_pVirtualMachine->StackPopInteger(&bSoloMode)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetSoloMode(bSoloMode, 1);
    return 0;
}

// FUNCTION: SWKOTOR 0x00547D20
int CSWVirtualMachineCommands::ExecuteCommandCancelPostDialogCharacterSwitch(int nCommandId, int nParameters)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_oidPostDialogCharacter = INVALID_OBJECT_ID;
    return 0;
}

// FUNCTION: SWKOTOR 0x00547D40
int CSWVirtualMachineCommands::ExecuteCommandSetMaxHitPoints(int nCommandId, int nParameters)
{
    int nMaxHitPoints;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nMaxHitPoints)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        if (pSWSObject) {
            ((CSWSObjectHitPointsView*)pSWSObject)->m_nMaxHitPoints = nMaxHitPoints;
            pSWSObject->SetCurrentHitPoints(nMaxHitPoints);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00547DC0
int CSWVirtualMachineCommands::ExecuteCommandNoClicksFor(int nCommandId, int nParameters)
{
    float fDuration = 0.0f;
    if (!g_pVirtualMachine->StackPopFloat(&fDuration)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (fDuration > 0.0f) {
        unsigned long nMilliseconds = (int)(fDuration * 1000.0f);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->MarkNoClickEvent(nMilliseconds);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00547E30
int CSWVirtualMachineCommands::ExecuteCommandHoldWorldFadeInForDialog(int nCommandId, int nParameters)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bHoldWorldFadeIn = 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x00547E50
int CSWVirtualMachineCommands::ExecuteCommandSuppressStatusSummaryEntry(int nCommandId, int nParameters)
{
    int nEntries;
    if (!g_pVirtualMachine->StackPopInteger(&nEntries)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui()->SuppressStatusSummary(nEntries);
    return 0;
}

// FUNCTION: SWKOTOR 0x00547E90
int CSWVirtualMachineCommands::ExecuteCommandGetCheatCode(int nCommandId, int nParameters)
{
    int nCode;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nCode)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nCode == 0) {
        nResult = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nCheatCode;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00547EF0
int CSWVirtualMachineCommands::ExecuteCommandSetMusicVolume(int nCommandId, int nParameters)
{
    float fVolume;
    if (!g_pVirtualMachine->StackPopFloat(&fVolume)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!(0.0f > fVolume)) {
        if (!(fVolume < 1.0f)) {
            fVolume = 1.0f;
        }
    } else {
        fVolume = 0.0f;
    }
    g_pExoSound->SetMusicVolume(fVolume, 1);
    return 0;
}
