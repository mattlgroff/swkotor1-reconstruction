#include "i/server_exo_app.h"

#define OBJECT_INVALID 0x7f000000

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                                          // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CScriptEvent {
public:
    CScriptEvent();                                                 // 0x004D7540
    void SetObjectID(int nIndex, unsigned long oid);                // 0x004D7780
    unsigned short m_nType;                                         // +0x00
    char m_pad2[0x34 - 2];
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent,
                          CScriptEvent* pEvent);                    // 0x004B08D0
};

extern const unsigned long EVENT_SIGNAL_EVENT;                      // 0x007441DC

class CPathfindInformation {
public:
    char m_pad0[0x2c];
    unsigned long m_oidCreature;                                    // +0x2c
};

class CSWSWaypoint {
public:
    char m_pad0[0x90];
    Vector m_vPosition;                                             // +0x90
    Vector m_vOrientation;                                          // +0x9c
};

// FUNCTION: SWKOTOR 0x004B3D10
void CServerExoAppInternal::MovePlayerToArea(void* pPlayerArg)
{
    CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayerArg;
    Vector vSafe(0.0f, 0.0f, 0.0f);
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (pPlayer->GetGameObject() == NULL) {
        return;
    }
    CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
    if (m_sMoveToModuleStartWaypoint[0]) {
        unsigned long oidWaypoint = GetModule()->GetWaypoint(m_sMoveToModuleStartWaypoint);
        if (oidWaypoint != OBJECT_INVALID) {
            CSWSWaypoint* pWaypoint = GetServerExoApp()->GetWaypointByGameObjectID(oidWaypoint);
            if (pWaypoint) {
                pCreature->m_vDesiredAreaLocation = pWaypoint->m_vPosition;
                pCreature->SetOrientation(pWaypoint->m_vOrientation);
            }
        }
        m_sMoveToModuleStartWaypoint = "";
    }
    vPosition = pCreature->m_vDesiredAreaLocation;
    CSWSArea* pArea = GetModule()->GetArea();
    pCreature->m_pPathfindInformation->m_oidCreature = pCreature->m_idSelf;
    if (pArea) {
        if (!pArea->ComputeSafeLocation(vPosition, 20.0f, pCreature->m_pPathfindInformation,
                                        (void*)1, &vSafe)) {
            vSafe = vPosition;
        }
        pCreature->UpdateSubareasOnJump(vSafe, pArea->m_oid120, m_bLoadFromSaveGame == 0);
        pCreature->AddToArea(pArea, vSafe.x, vSafe.y, vSafe.z, 0, 0);
    }
    pCreature->UpdateVisibleList(0);
    m_c1b770.PositionParty();
    m_bLoadFromSaveGame = 0;
}
// FUNCTION: SWKOTOR 0x004B3A90
void CServerExoAppInternal::InitiateModuleForPlayer(void* pPlayerArg)
{
    CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayerArg;
    CSWSCreature* pCreature;
    if (pPlayer->GetGameObject() == NULL) {
        pCreature = new CSWSCreature(OBJECT_INVALID, 1);
        unsigned long oidSelf = pCreature->m_idSelf;
        pCreature->m_pPathfindInformation->m_oidCreature = oidSelf;
        pPlayer->SetGameObject(pCreature);
    }
    else {
        pCreature = pPlayer->GetGameObject()->AsSWSCreature();
    }
    int bNewToModule = (int)(bool)!pPlayer->m_bFromSaveGame;
    CSWSArea* pArea;
    if (bNewToModule) {
        Vector vOrientation(0.0f, 0.0f, 0.0f);
        pArea = GetModule()->GetArea();
        pCreature->m_oidDesiredArea = pArea->m_oid120;
        pCreature->m_vDesiredAreaLocation = Vector(GetModule()->m_pEntryInfo->m_vEntryPosition.x,
                                                   GetModule()->m_pEntryInfo->m_vEntryPosition.y,
                                                   GetModule()->m_pEntryInfo->m_vEntryPosition.z);
        pCreature->m_bDesiredAreaUpdateComplete = 0;
        vOrientation.x = GetModule()->m_pEntryInfo->m_fEntryDirectionX;
        vOrientation.y = GetModule()->m_pEntryInfo->m_fEntryDirectionY;
        vOrientation.z = 0.0f;
        pCreature->SetOrientation(vOrientation);
    }
    else {
        pArea = pCreature->GetArea();
        if (pArea) {
            pCreature->m_oidDesiredArea = pArea->m_oid120;
            pCreature->m_vDesiredAreaLocation = pCreature->m_vPosition;
            pCreature->m_bDesiredAreaUpdateComplete = 0;
        }
        else {
            pArea = GetServerExoApp()->GetAreaByGameObjectID(pCreature->m_oidDesiredArea);
        }
    }
    if (pArea == NULL) {
        return;
    }
    pCreature->SetArea(NULL);
    CSWSMessage* pMessage = GetServerExoApp()->FUN_004aedb0();
    if (pMessage == NULL) {
        return;
    }
    {
        CExoString sBMP("");
        pPlayer->SetAreaTransitionBMP(pArea->m_nLoadScreenID, sBMP);
    }
    pMessage->SendServerToPlayerArea_ClientArea(pPlayer, pArea, pCreature->m_vDesiredAreaLocation.x,
                                                pCreature->m_vDesiredAreaLocation.y,
                                                pCreature->m_vDesiredAreaLocation.z,
                                                pCreature->m_vOrientation, bNewToModule);
    pMessage->SendServerToPlayerSetCustomTokenList(pPlayer->m_nPlayerID);
}
// FUNCTION: SWKOTOR 0x004B69C0
void CServerExoAppInternal::Shutdown(int bExit, int bShutdownNetwork)
{
    int bMultiplayer = m_pServerInfo->m_n98;
    m_lShutdownPlayers.Clear();
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    if (bMultiplayer && bExit) {
        while (pos) {
            CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
            if (pPlayer) {
                m_pServerMessage->SendServerToPlayerShutDownServer(
                    pPlayer->m_nPlayerID, (int)(m_nShutdownDelay / 100000));
                m_lShutdownPlayers.Add(pPlayer->m_nPlayerID);
            }
            m_pPlayerList->GetNext(pos);
        }
    }
    else {
        int nGuard = 0x100;
        while (pos) {
            if (--nGuard <= 0) {
                break;
            }
            CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
            if (pPlayer) {
                if (bShutdownNetwork) {
                    m_lShutdownPlayers.Add(pPlayer->m_nPlayerID);
                    m_pPlayerList->GetNext(pos);
                }
                else {
                    pos = m_pPlayerList->GetHeadPos();
                }
            }
            else {
                m_pPlayerList->GetNext(pos);
            }
        }
    }
    pos = m_pSysAdminList->GetHeadPos();
    int nGuard = 0x100;
    while (pos) {
        if (--nGuard <= 0) {
            break;
        }
        CSWSPlayer* pAdmin = m_pSysAdminList->GetAtPos(pos);
        if (pAdmin) {
            unsigned long nPlayerId = pAdmin->m_nPlayerID;
            if (!m_lShutdownPlayers.Contains(nPlayerId)) {
                pos = m_pSysAdminList->GetHeadPos();
            }
            else {
                m_pSysAdminList->GetNext(pos);
            }
        }
        else {
            m_pSysAdminList->GetNext(pos);
        }
    }
    if (bMultiplayer && bExit && m_lShutdownPlayers.num) {
        m_nShutdownLast = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
        m_nShutdownRemaining = 180000000;
    }
    else {
        m_nShutdownLast = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
        m_nShutdownRemaining = 1000000;
    }
}
// FUNCTION: SWKOTOR 0x004B5C50
int CServerExoAppInternal::LoadCharacterFinish(CSWSPlayer* pPlayer, int bLoadFromIFO)
{
    CSWSModule* pModule = g_pAppManager->m_pServerExoApp->GetModule();
    CScriptEvent* pEvent = new CScriptEvent;
    CScriptEvent* pDeathEvent = NULL;
    if (pPlayer->m_bPendingAreaMove != 1) {
        pPlayer->m_bPendingAreaMove = 1;
        m_pServerMessage->SendServerToPlayerLogin_Confirm(pPlayer->m_nPlayerID);
        m_pServerMessage->SendServerToPlayerModule_Info(pPlayer->m_nPlayerID);
    }
    if (bLoadFromIFO && pPlayer->m_nPrimaryPlayerIndex != -1) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject) {
            delete pObject;
        }
        pPlayer->LoadCharacterIFO(pPlayer->m_nPrimaryPlayerIndex, 1, 1);
    }
    pEvent->m_nType = 14;
    if (pPlayer->GetGameObject()) {
        if (pPlayer->GetGameObject()->AsSWSCreature()->GetIsDead() == 1 ||
            pPlayer->GetGameObject()->AsSWSCreature()->GetDeadTemp() == 1) {
            pDeathEvent = new CScriptEvent;
            pDeathEvent->m_nType = 10;
        }
        pEvent->SetObjectID(0, pPlayer->GetGameObject()->m_idSelf);
        CSWSMessage* pMessage = g_pAppManager->m_pServerExoApp->FUN_004aedb0();
        CExoLinkedList<CSWSPlayer>* pList = g_pAppManager->m_pServerExoApp->FUN_004aed90();
        CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
        CExoLinkedListPosition pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pOther = pList->GetAtPos(pos);
            if (pOther && pOther->m_bCharacterListed && pMessage) {
                pMessage->SendServerToPlayerPlayModuleCharacterListResponse(
                    pOther->m_nPlayerID, pCreature->m_idSelf, 1);
            }
            pList->GetNext(pos);
        }
    }
    else {
        pEvent->SetObjectID(0, OBJECT_INVALID);
    }
    if (pModule) {
        GetServerExoApp()->FUN_004aed80()->AddEventDeltaTime(
            0, 0, pPlayer->m_nPlayerID, pModule->m_oidModule, EVENT_SIGNAL_EVENT, pEvent);
        if (pDeathEvent) {
            GetServerExoApp()->FUN_004aed80()->AddEventDeltaTime(
                0, 0, pPlayer->GetGameObject()->m_idSelf, pModule->m_oidModule, EVENT_SIGNAL_EVENT,
                pDeathEvent);
        }
        CSWSCreature* pPC = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pPlayer->m_oidPCObject);
        if (pPC && &pPC->m_cAppearance) {
            int nNormalHead;
            {
                CExoString sColumn("NORMALHEAD");
                g_pRules->m_p2DArrays->m_pAppearanceTable->GetINTEntry(
                    pPC->m_cAppearance.m_nAppearanceType, sColumn, &nNormalHead);
            }
            pModule->GetArea()->EnforceNoPCHeadCloning(nNormalHead);
        }
    }
    return 1;
}
