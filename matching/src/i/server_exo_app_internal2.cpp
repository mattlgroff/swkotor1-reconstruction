#include "i/server_exo_app.h"

#define OBJECT_INVALID 0x7f000000

class CClientOptions {
public:
    char m_pad0[8];
    unsigned long m_nFlags;                                         // +0x08 (bit 2: autosave)
};

class CClientExoApp {
public:
    CClientOptions* GetOptions();                                   // 0x005ED700
};

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                                 // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC

extern const unsigned long PLAYER_ID_ALL_PLAYERS;                   // 0x0074C52C
extern const unsigned long PLAYER_ID_SERVER_ADMIN;                  // 0x0074C534 (placeholder name)

#pragma pack(push, 4)
class CWorldTimer {
public:
    void SetSnapshotTime();                                         // 0x004ADBD0
    unsigned __int64 GetSnapshotTime();                             // 0x004ADC70
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    void FUN_004adef0(const CWorldTimer* pOther);                   // 0x004ADEF0
    void PauseWorldTimer();                                         // 0x004ADFF0
    void UnpauseWorldTimer();                                       // 0x004AE030

    void ResetLastSnapshotTime() { m_nLastSnapshotTime = GetSnapshotTime(); }

    char m_pad0[0x14];
    unsigned __int64 m_nLastSnapshotTime;                           // +0x14
};
#pragma pack(pop)

static inline int PauseState(CServerExoAppInternal* pApp, unsigned char nMask)
{
    return (unsigned char)(pApp->m_nPauseState & nMask) > 0;
}

static inline unsigned char ActivePauseState(CServerExoAppInternal* pApp)
{
    if ((pApp->m_nPauseState & 2u) > 0) {
        return 2;
    }
    if ((unsigned char)(pApp->m_nPauseState & 1) > 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B4960
unsigned long CServerExoAppInternal::ResolvePlayerByFirstName(const CExoString& sName)
{
    CExoString sFirstName;
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject) {
                CSWSCreature* pCreature = pObject->AsSWSCreature();
                if (pCreature) {
                    unsigned long nPlayerId = pPlayer->m_nPlayerID;
                    CSWSMessage::GetLocStringServer(nPlayerId, *pCreature->GetFirstName(),
                                                    CExoLocString(), sFirstName, 0);
                    if (sFirstName == sName) {
                        return pPlayer->m_nPlayerID;
                    }
                }
            }
        }
        m_pPlayerList->GetNext(pos);
    }
    return OBJECT_INVALID_ID;
}
// FUNCTION: SWKOTOR 0x004B4AB0
int CServerExoAppInternal::UpdateShutdownTimer(unsigned __int64 nNow)
{
    if (m_nShutdownRemaining == 0) {
        return 0;
    }
    if (nNow - m_nShutdownLast > m_nShutdownRemaining) {
        m_nShutdownRemaining = 0;
        return 1;
    }
    if ((m_nShutdownRemaining > 60000000 && m_nShutdownRemaining - (nNow - m_nShutdownLast) < 60000000) ||
        (m_nShutdownRemaining > 30000000 && m_nShutdownRemaining - (nNow - m_nShutdownLast) < 30000000)) {
        for (int i = 0; i < m_lShutdownPlayers.num; i++) {
            m_pServerMessage->SendServerToPlayerShutDownServer(
                m_lShutdownPlayers.element[i],
                (int)((m_nShutdownRemaining - (nNow - m_nShutdownLast)) / 1000000));
        }
    }
    m_nShutdownRemaining = m_nShutdownRemaining - (nNow - m_nShutdownLast);
    m_nShutdownLast = nNow;
    return 0;
}
// FUNCTION: SWKOTOR 0x004B5050
int CServerExoAppInternal::AutoSaveOnEnteringModule(const CExoString& sModule, C2DA* p2DA)
{
    CExoString sValue;
    bool bMissing;
    {
        CExoString sColumn("AutoSaveOnEnter");
        bMissing = !p2DA->GetCExoStringEntry(sModule, sColumn, &sValue);
    }
    if (bMissing) {
        m_nAutoSaveElapsed = 0;
        m_bAutoSaveDue = 0;
        return 1;
    }
    int bAutoSave;
    if (sValue.CompareNoCase("force")) {
        bAutoSave = 1;
    }
    else if (sValue.CompareNoCase("no")) {
        bAutoSave = 0;
    }
    else {
        if (!m_bAutoSaveDue) {
            return 0;
        }
        if (sValue.CompareNoCase("yes")) {
            bAutoSave = (g_pAppManager->m_pClientExoApp->GetOptions()->m_nFlags >> 2) & 1;
        }
        else {
            bAutoSave = 1;
        }
    }
    if (bAutoSave) {
        m_nAutoSaveElapsed = 0;
        m_bAutoSaveDue = 0;
    }
    return bAutoSave;
}
// FUNCTION: SWKOTOR 0x004B6950
void CServerExoAppInternal::UpdateClientGameObjects(int nFlags)
{
    unsigned __int64 nTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
        UpdateClientGameObjectsForPlayer(pPlayer, nFlags, nTime);
        m_pPlayerList->GetNext(pos);
    }
}
// FUNCTION: SWKOTOR 0x004B6C40
CWorldTimer* CServerExoAppInternal::GetActiveTimer(unsigned long oid)
{
    if (oid != OBJECT_INVALID && (PauseState(this, 2) || PauseState(this, 1))) {
        unsigned char nActive = ActivePauseState(this);
        if (nActive == 1) {
            if (IsOnActiveExclusionList(oid) == 1) {
                return m_pTimestopTimer;
            }
        }
        else if (nActive == 2) {
            if (IsOnActiveExclusionList(oid) == 1) {
                return m_pPauseTimer;
            }
        }
        else {
            return NULL;
        }
    }
    return m_pWorldTimer;
}

// FUNCTION: SWKOTOR 0x004B6CC0
void CServerExoAppInternal::UpdateSubareasOnMoveTo(unsigned long oid, const Vector& vFrom, const Vector& vTo)
{
    CGameObject* pObject = NULL;
    m_pGameObjArray->GetGameObject(oid != OBJECT_INVALID ? oid & ~CLIENT_OBJECT_MASK : OBJECT_INVALID, &pObject);
    if (pObject) {
        CSWSCreature* pCreature = pObject->AsSWSObject()->AsSWSCreature();
        if (pCreature) {
            CSWSArea* pArea = pCreature->GetArea();
            if (pArea) {
                CExoArrayList<unsigned long> lSubAreas;
                pArea->IntersectingLineSegment(vFrom, vTo, &lSubAreas);
                pCreature->UpdateSubareasOnMoveTo(vFrom, vTo, 1, &lSubAreas, 0);
            }
        }
    }
}
// FUNCTION: SWKOTOR 0x004B8020
void CServerExoAppInternal::SetWorldTimeOnList(unsigned char nList)
{
    CExoLinkedList<unsigned long>* pList = nList == 2 ? m_pExclusionList2 : m_pExclusionList1;
    CExoLinkedListPosition pos = pList->GetHeadPos();
    while (pos) {
        unsigned long oid = (unsigned long)pList->GetAtPos(pos);
        unsigned long nDay;
        unsigned long nTime;
        GetActiveTimer(oid)->GetWorldTime(&nDay, &nTime);
        CGameObject* pObject;
        m_pGameObjArray->GetGameObject(oid, &pObject);
        if (pObject) {
            pObject->SetWorldTime(nDay, nTime);
        }
        pList->GetNext(pos);
    }
}
// FUNCTION: SWKOTOR 0x004B8110
void CServerExoAppInternal::SetPauseState(unsigned char nState, int bPause)
{
    CSWSMessage* pMessage = m_pServerMessage;
    if (((unsigned char)(m_nPauseState & nState) > 0) == bPause) {
        return;
    }
    if (bPause == 1) {
        m_nPauseState |= nState;
        m_pWorldTimer->PauseWorldTimer();
        if ((m_nPauseState & 2) > 0) {
            m_pTimestopTimer->PauseWorldTimer();
        }
        pMessage->SendServerToPlayerModule_SetPauseState(nState, 1);
        return;
    }
    m_nPauseState &= ~nState;
    if (nState == 2) {
        m_pPauseTimer->PauseWorldTimer();
        SetWorldTimeOnList(2);
        m_pPauseTimer->FUN_004adef0(m_pTimestopTimer);
        m_pPauseTimer->SetSnapshotTime();
        m_pTimestopTimer->SetSnapshotTime();
        m_pPauseTimer->ResetLastSnapshotTime();
        m_pTimestopTimer->ResetLastSnapshotTime();
        if (ActivePauseState(this) != 1) {
            m_pWorldTimer->SetSnapshotTime();
            m_pWorldTimer->ResetLastSnapshotTime();
            m_pWorldTimer->UnpauseWorldTimer();
        }
        m_pTimestopTimer->UnpauseWorldTimer();
        m_pPauseTimer->UnpauseWorldTimer();
    }
    else if (nState == 1) {
        m_pTimestopTimer->PauseWorldTimer();
        m_pPauseTimer->PauseWorldTimer();
        SetWorldTimeOnList(1);
        m_pTimestopTimer->FUN_004adef0(m_pWorldTimer);
        m_pPauseTimer->FUN_004adef0(m_pWorldTimer);
        m_pWorldTimer->SetSnapshotTime();
        m_pTimestopTimer->SetSnapshotTime();
        m_pPauseTimer->SetSnapshotTime();
        m_pPauseTimer->ResetLastSnapshotTime();
        m_pTimestopTimer->ResetLastSnapshotTime();
        m_pWorldTimer->ResetLastSnapshotTime();
        m_pWorldTimer->UnpauseWorldTimer();
        m_pTimestopTimer->UnpauseWorldTimer();
        m_pPauseTimer->UnpauseWorldTimer();
    }
    pMessage->SendServerToPlayerModule_SetPauseState(nState, bPause);
}
// FUNCTION: SWKOTOR 0x004B2380
void CServerExoAppInternal::DoModuleEnterSaveCleanup(const CExoString& sModule, C2DA* p2DA)
{
    int nSaveGroup;
    int bFound;
    {
        CExoString sColumn("DeleteSaveGroupOnEnter");
        bFound = p2DA->GetINTEntry(sModule, sColumn, &nSaveGroup);
    }
    if (bFound) {
        CExoString sPrefix("GAMEINPROGRESS:");
        CExoString sModuleName;
        for (int i = 0; i < p2DA->m_nNumRows; i++) {
            int nGroup;
            {
                CExoString sColumn("SaveGroup");
                bFound = p2DA->GetINTEntry(i, sColumn, &nGroup);
            }
            if (bFound && nGroup == nSaveGroup) {
                {
                    CExoString sColumn("ModuleName");
                    bFound = p2DA->GetCExoStringEntry(i, sColumn, &sModuleName);
                }
                if (bFound) {
                    g_pExoResMan->RemoveFile(sPrefix + sModuleName, 0x809);
                }
            }
        }
    }
}
// FUNCTION: SWKOTOR 0x004B6270
int CServerExoAppInternal::RunModule()
{
    CExoString sUnused;
    if (m_nServerMode == 1) {
        m_nServerMode = 2;
        m_pServerMessage->SendServerToPlayerServerStatus_Status(PLAYER_ID_ALL_PLAYERS);
        m_pServerMessage->SendServerToPlayerServerAdmin_Status(PLAYER_ID_SERVER_ADMIN);
        CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
            if (pPlayer->m_bPendingAreaMove) {
                MovePlayerToArea(pPlayer);
            }
            m_pPlayerList->GetNext(pos);
        }
        if (GetModule()->m_bIsSaveGame && m_bDoingStartNewModule != 1) {
            pos = m_pPlayerList->GetHeadPos();
            LoadPrimaryPlayer(m_pPlayerList->GetAtPos(pos));
            m_c1b770.CreateParty();
        }
        if (m_bDoingStartNewModule == 1) {
            CSWSMessage* pMessage = g_pAppManager->m_pServerExoApp->FUN_004aedb0();
            pMessage->SendServerToPlayerModule_EndStartNewModule(1);
            m_bDoingStartNewModule = 0;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B2E70
int CServerExoAppInternal::StoreCurrentModule()
{
    CSWSModule* pModule = GetModule();
    if (pModule == NULL) {
        return 1;
    }
    int bInclude = IncludeModuleInSave(pModule->GetModuleShortName());
    if (!bInclude) {
        return 1;
    }
    CExoString sPrefix("GAMEINPROGRESS:");
    CExoString sPath(pModule->m_sModuleName);
    int nColon = sPath.Find(':', 0);
    if (nColon == -1) {
        sPath = sPrefix + sPath;
    }
    else {
        sPath = sPrefix + sPath.Right(sPath.GetLength() - nColon - 1);
    }
    pModule->SaveModuleStart(sPrefix, sPath);
    pModule->LoadTemporaryArea();
    pModule->SaveModuleFinish(sPrefix, sPath);
    return 1;
}
