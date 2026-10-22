// CServerExoAppInternal small members (0x004B14F0-0x004B4CF0):
// module/area caches, the Get*ByGameObjectID family, pause state, player
// list walks and timers.
#include "i/server_exo_app.h"

extern "C" __declspec(dllimport) int __stdcall CopyFileA(const char* sExisting, const char* sNew, int bFailIfExists);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesA(const char* sFile);
extern "C" __declspec(dllimport) int __stdcall SetFileAttributesA(const char* sFile, unsigned long nAttributes);

class CWorldTimer {
public:
    unsigned __int64 FUN_004adc70();                // 0x004ADC70
};

struct CAppManager14 {
    int m_n0;
};

struct CClientPartyMember {
    char m_pad0[0x24];
    Vector m_vPosition;                             // +0x24
    Vector m_vOrientation;                          // +0x30
};

class CClientPartyList {
public:
    CClientPartyMember* GetMember(int nIndex);      // 0x006346C0 (unnamed)
    int m_nCount;
};

class CClientExoApp {
public:
    CClientPartyList* GetPartyList();               // 0x005ED8B0 (unnamed)
};

class CVirtualMachine {
public:
    int RunScript(CExoString* sScript, unsigned long oidSelf, int nScriptType);  // 0x005D0FC0
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

class CSWSPlayerCharacterQuery;

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                 // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08
    char m_pad0c[0x14 - 0x0c];
    CAppManager14* m_p14;                           // +0x14

    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

// FUNCTION: SWKOTOR 0x004B14F0
CSWSModule* CServerExoAppInternal::GetModule()
{
    if (m_oidLastModule != m_oidModule) {
        CGameObject* pObject = 0;
        m_pGameObjArray->GetGameObject(m_oidModule, &pObject);
        if (pObject) {
            m_oidLastModule = m_oidModule;
            m_pLastModule = pObject->AsSWSModule();
            return m_pLastModule;
        }
        m_oidLastModule = 0x7f000000;
        m_pLastModule = 0;
    }
    return m_pLastModule;
}

// FUNCTION: SWKOTOR 0x004B1560
void CServerExoAppInternal::ClearLastModuleFound()
{
    m_oidLastModule = 0x7f000000;
    m_pLastModule = 0;
}

// FUNCTION: SWKOTOR 0x004B1580
CExoString CServerExoAppInternal::GetModuleName()
{
    if (GetModule()) {
        return m_pServerInfo->m_sModuleName;
    }
    return CExoString();
}

// FUNCTION: SWKOTOR 0x004B1650
int CServerExoAppInternal::Initialize()
{
    CResRef cResRef("globalcat");
    m_cGlobalVariableTable.ReadCatalogue(cResRef);
    return 1;
}

// FUNCTION: SWKOTOR 0x004B1680
int CServerExoAppInternal::CopyModuleToCurrentGame(CExoString& sSource, CExoString& sDest, int nUnused)
{
    if (!CopyFileA(sSource.CStr(), sDest.CStr(), 0)) {
        return 0;
    }
    if (GetFileAttributesA(sDest.CStr()) & 1) {
        SetFileAttributesA(sDest.CStr(), 0x80);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004B1700
CGameObject* CServerExoAppInternal::GetGameObject(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) != GAME_OBJECT_VALID) {
        return 0;
    }
    return pObject;
}

// FUNCTION: SWKOTOR 0x004B1740
CSWSItem* CServerExoAppInternal::GetItemByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_ITEM) {
        return pObject->AsSWSItem();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1790
CSWSCreature* CServerExoAppInternal::GetCreatureByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
        return pObject->AsSWSCreature();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B17E0
CSWSArea* CServerExoAppInternal::GetAreaByGameObjectID(unsigned long oid)
{
    if (m_oidLastArea != oid) {
        m_pLastArea = 0;
        m_oidLastArea = 0x7f000000;
        CGameObject* pObject = 0;
        if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
            pObject->m_nObjectType == OBJECT_TYPE_AREA) {
            m_oidLastArea = oid;
            m_pLastArea = pObject->AsSWSArea();
        }
    }
    return m_pLastArea;
}

// FUNCTION: SWKOTOR 0x004B1860
void CServerExoAppInternal::ClearAreaCache()
{
    m_pLastArea = 0;
    m_oidLastArea = 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004B1880
CSWSTrigger* CServerExoAppInternal::GetTriggerByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
        return pObject->AsSWSTrigger();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B18D0
CSWSPlaceable* CServerExoAppInternal::GetPlaceableByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
        return pObject->AsSWSPlaceable();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1920
CSWSDoor* CServerExoAppInternal::GetDoorByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
        return pObject->AsSWSDoor();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1970
CSWSAreaOfEffectObject* CServerExoAppInternal::GetAreaOfEffectByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_AREA_OF_EFFECT) {
        return pObject->AsSWSAreaOfEffectObject();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B19C0
CSWSWaypoint* CServerExoAppInternal::GetWaypointByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_WAYPOINT) {
        return pObject->AsSWSWaypoint();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1A10
CSWSEncounter* CServerExoAppInternal::GetEncounterByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_ENCOUNTER) {
        return pObject->AsSWSEncounter();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1A60
CSWSSoundObject* CServerExoAppInternal::GetSoundObjectByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_SOUND) {
        return pObject->AsSWSSoundObject();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1AB0
CSWSStore* CServerExoAppInternal::GetStoreByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_STORE) {
        return pObject->AsSWSStore();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1C40
void CServerExoAppInternal::DumpServerOptionsToLog()
{
    CExoString sTimestamp;
    g_pExoBase->m_pcExoDebug->GetCurrentTimestamp(sTimestamp);
    sTimestamp.CStr();
}

// FUNCTION: SWKOTOR 0x004B1CB0
void CServerExoAppInternal::UpdatePlayerPosition(unsigned long oid, const Vector& vPosition,
                                                 const Vector& vOrientation)
{
    CSWSCreature* pCreature = GetCreatureByGameObjectID(oid);
    if (pCreature) {
        pCreature->SetPosition(vPosition, 1, 1, 0);
        pCreature->SetOrientation(vOrientation);
    }
}

// FUNCTION: SWKOTOR 0x004B1D20
int CServerExoAppInternal::IsInjured(unsigned long oid)
{
    CSWSCreature* pCreature = GetCreatureByGameObjectID(oid);
    if (pCreature) {
        return pCreature->IsInjured();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1D80
int CServerExoAppInternal::GetPauseState(unsigned char nMask)
{
    return (unsigned char)(m_nPauseState & nMask) > 0;
}

// FUNCTION: SWKOTOR 0x004B1DA0
unsigned char CServerExoAppInternal::GetActivePauseState()
{
    if ((m_nPauseState & 2u) > 0) {
        return 2;
    }
    return (m_nPauseState & 1u) > 0;
}

// NONMATCH (3 insns): the original tests bit 1 with `and al, 1; test al, al`
// and lays the bit-0 return before the bit-1 return; tried >0u compares,
// nested <=0 (layout matches, 4 insns), a uchar local, bitfields, calling
// GetActivePauseState twice, and a switch on it.
// FUNCTION: SWKOTOR 0x004B1DC0
CExoLinkedList<unsigned long>* CServerExoAppInternal::GetActiveExclusionList()
{
    if ((m_nPauseState & 2u) > 0) {
        return m_pExclusionList2;
    }
    if ((unsigned char)(m_nPauseState & 1) > 0) {
        return m_pExclusionList1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1DF0
int CServerExoAppInternal::GetDifficultyOption(int nOption)
{
    int nDifficulty = m_pServerInfo->m_nDifficulty;
    return g_pRules->GetDifficultyOption(nOption, nDifficulty);
}

// FUNCTION: SWKOTOR 0x004B1E10
int CServerExoAppInternal::FUN_004b1e10(unsigned __int64 nTime)
{
    if (!m_nThrottleLast) {
        m_nThrottleLast = nTime;
        m_nThrottleRemaining = 10000000;
        return 0;
    }
    if (nTime - m_nThrottleLast >= m_nThrottleRemaining) {
        m_nThrottleRemaining = 10000000;
        m_nThrottleLast = nTime;
        return 1;
    }
    m_nThrottleRemaining -= nTime - m_nThrottleLast;
    m_nThrottleLast = nTime;
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1EE0
int CServerExoAppInternal::UpdateAutoSaveTimer()
{
    m_pPauseTimer->FUN_004adc70();
    unsigned long nNow = g_pExoBase->m_pcExoTimers->GetLowResolutionTimer();
    if (m_bAutoSaveDue) {
        m_nAutoSaveLastTime = nNow;
        return 1;
    }
    if (!m_nAutoSaveLastTime) {
        m_nAutoSaveLastTime = nNow;
    }
    unsigned long nLast = m_nAutoSaveLastTime;
    m_nAutoSaveLastTime = nNow;
    m_nAutoSaveElapsed += nNow - nLast;
    if (m_nAutoSaveElapsed > 900000) {
        m_nAutoSaveElapsed = 0;
        m_bAutoSaveDue = 1;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B1F70
unsigned long CServerExoAppInternal::ClientToServerObjectID(unsigned long oid)
{
    if (oid != 0x7f000000) {
        return oid & ~CLIENT_OBJECT_MASK;
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004B1F90
void CServerExoAppInternal::SkipPastDialog(unsigned long oid)
{
    CGameObject* pObject = GetGameObject(oid);
    if (pObject && pObject->AsSWSObject()) {
        pObject->AsSWSObject()->m_bSkipDialog = 1;
    }
}

// FUNCTION: SWKOTOR 0x004B2200
int CServerExoAppInternal::ShrinkLineByCreatures(Vector* a, Vector* b, unsigned long c)
{
    return GetModule()->GetArea()->ShrinkLineByCreatures(a, b, c);
}

// FUNCTION: SWKOTOR 0x004B22D0
void CServerExoAppInternal::SetMoveToWorldTime(unsigned long n0, unsigned char n1, unsigned char n2,
                                               unsigned char n3, unsigned short n4, unsigned short n5,
                                               unsigned short n6)
{
    m_nMoveToTime0 = n0;
    m_nMoveToTime1 = n1;
    m_nMoveToTime2 = n2;
    m_nMoveToTime3 = n3;
    m_nMoveToTime4 = n4;
    m_nMoveToTime5 = n5;
    m_nMoveToTime6 = n6;
}

// FUNCTION: SWKOTOR 0x004B2320
void CServerExoAppInternal::GetMoveToWorldTime(unsigned long* p0, unsigned char* p1, unsigned char* p2,
                                               unsigned char* p3, unsigned short* p4, unsigned short* p5,
                                               unsigned short* p6)
{
    *p0 = m_nMoveToTime0;
    *p1 = m_nMoveToTime1;
    *p2 = m_nMoveToTime2;
    *p3 = m_nMoveToTime3;
    *p4 = m_nMoveToTime4;
    *p5 = m_nMoveToTime5;
    *p6 = m_nMoveToTime6;
}

// FUNCTION: SWKOTOR 0x004B2A50
CSWSClient* CServerExoAppInternal::GetClientObjectByObjectID(unsigned long oid)
{
    if (oid == 0x7f000000) {
        return 0;
    }
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
        if (pPlayer && pPlayer->GetGameObject() && pPlayer->GetGameObject()->m_idSelf == oid) {
            return pPlayer;
        }
        m_pPlayerList->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B2AE0
CSWSClient* CServerExoAppInternal::GetClientObjectByPlayerID(unsigned long nPlayerId, unsigned char nType)
{
    CExoLinkedListPosition pos;
    if (nType == 1 || nType == 0) {
        pos = m_pPlayerList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
            if (pPlayer->m_nPlayerID == nPlayerId) {
                return pPlayer;
            }
            m_pPlayerList->GetNext(pos);
        }
    }
    if (nType == 2 || nType == 0) {
        pos = m_pSysAdminList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = m_pSysAdminList->GetAtPos(pos);
            if (pPlayer->m_nPlayerID == nPlayerId) {
                return pPlayer;
            }
            m_pSysAdminList->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B4030
int CServerExoAppInternal::FUN_004b4030()
{
    return g_pAppManager->m_p14->m_n0 == 1;
}

// FUNCTION: SWKOTOR 0x004B4050
int CServerExoAppInternal::GetPlayerLanguage(unsigned long nPlayerId)
{
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
        if (pPlayer && pPlayer->m_nPlayerID == nPlayerId) {
            return pPlayer->m_nLanguage;
        }
        m_pPlayerList->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B4C50
unsigned long CServerExoAppInternal::GetFirstPCObject()
{
    m_posPC = m_pPlayerList->GetHeadPos();
    if (m_posPC) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(m_posPC);
        if (pPlayer) {
            return pPlayer->m_oidPCObject;
        }
    }
    return OBJECT_INVALID_ID;
}

// FUNCTION: SWKOTOR 0x004B4C80
unsigned long CServerExoAppInternal::GetNextPCObject()
{
    if (m_posPC) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetNext(m_posPC);
        if (pPlayer) {
            return pPlayer->m_oidPCObject;
        }
    }
    return OBJECT_INVALID_ID;
}

// FUNCTION: SWKOTOR 0x004B4CB0
unsigned long CServerExoAppInternal::GetPlayerCreatureID()
{
    CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByPlayerID(0, 1);
    if (pPlayer && pPlayer->GetGameObject()) {
        return pPlayer->GetGameObject()->m_idSelf;
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004B4CF0
int CServerExoAppInternal::IsActionsPending(unsigned long oid)
{
    CSWSCreature* pCreature = GetCreatureByGameObjectID(ClientToServerObjectID(oid));
    if (pCreature) {
        return pCreature->m_lActions.m_pList->m_pHead != 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B3000
int CServerExoAppInternal::ExportAllPlayers()
{
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
        switch (pPlayer->m_nCharacterType) {
        case 1:
            m_pServerMessage->SendServerToPlayerModule_ExportReply(pPlayer);
            break;
        case 3:
        case 4:
            pPlayer->FUN_005624c0();
            break;
        }
        m_pPlayerList->GetNext(pos);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004B40C0
int CServerExoAppInternal::GetFactionOfObject(unsigned long oid, int* pFaction)
{
    CGameObject* pObject = GetGameObject(oid);
    if (pObject) {
        if (pObject->AsSWSCreature()) {
            *pFaction = pObject->AsSWSCreature()->m_pStats->m_nFactionID;
            return 1;
        }
        if (pObject->AsSWSDoor()) {
            *pFaction = pObject->AsSWSDoor()->m_nFactionID;
            return 1;
        }
        if (pObject->AsSWSEncounter()) {
            *pFaction = pObject->AsSWSEncounter()->m_nFactionID;
            return 1;
        }
        if (pObject->AsSWSPlaceable()) {
            *pFaction = pObject->AsSWSPlaceable()->m_nFactionID;
            return 1;
        }
        if (pObject->AsSWSTrigger()) {
            *pFaction = pObject->AsSWSTrigger()->m_nFactionID;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B4730
void CServerExoAppInternal::AddToExclusionList(unsigned long oid, unsigned char nList)
{
    if (nList == 0) {
        return;
    }
    CExoLinkedListPosition pos;
    if (nList & 2) {
        pos = m_pExclusionList2->GetHeadPos();
        while (pos) {
            if ((unsigned long)m_pExclusionList2->GetAtPos(pos) == oid) {
                return;
            }
            m_pExclusionList2->GetNext(pos);
        }
        m_pExclusionList2->AddTail((unsigned long*)oid);
    }
    else if (nList & 1) {
        pos = m_pExclusionList1->GetHeadPos();
        while (pos) {
            if ((unsigned long)m_pExclusionList1->GetAtPos(pos) == oid) {
                return;
            }
            m_pExclusionList1->GetNext(pos);
        }
        m_pExclusionList1->AddTail((unsigned long*)oid);
    }
}

// FUNCTION: SWKOTOR 0x004B4800
void CServerExoAppInternal::RemoveFromExclusionList(unsigned long oid, unsigned char nList)
{
    if (nList == 0) {
        return;
    }
    CExoLinkedListPosition pos;
    if (nList & 1) {
        pos = m_pExclusionList1->GetHeadPos();
        while (pos) {
            if ((unsigned long)m_pExclusionList1->GetAtPos(pos) == oid) {
                m_pExclusionList1->Remove(pos);
                return;
            }
            m_pExclusionList1->GetNext(pos);
        }
    }
    else if (nList & 2) {
        pos = m_pExclusionList2->GetHeadPos();
        while (pos) {
            if ((unsigned long)m_pExclusionList2->GetAtPos(pos) == oid) {
                m_pExclusionList2->Remove(pos);
                return;
            }
            m_pExclusionList2->GetNext(pos);
        }
    }
}

// NONMATCH (6 insns): the inlined GetActiveExclusionList has the same
// bit-test/layout difference as 0x004B1DC0 above; the loop matches.
// FUNCTION: SWKOTOR 0x004B48E0
int CServerExoAppInternal::IsOnActiveExclusionList(unsigned long oid)
{
    CExoLinkedList<unsigned long>* pList = GetActiveExclusionList();
    if (!pList) {
        return 1;
    }
    CExoLinkedListPosition pos = pList->GetHeadPos();
    while (pos) {
        if ((unsigned long)pList->GetAtPos(pos) == oid) {
            return 1;
        }
        pList->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B4D70
int CServerExoAppInternal::TestSafeLocation(unsigned long oidCreature, const Vector& v)
{
    CSWSCreature* pCreature = GetCreatureByGameObjectID(oidCreature);
    if (pCreature == NULL) {
        return 0;
    }
    return GetAreaByGameObjectID(pCreature->m_oidArea)
        ->TestSafeLocationPoint(v, pCreature->m_pPathfindInformation);
}

// FUNCTION: SWKOTOR 0x004B4DF0
int CServerExoAppInternal::ComputeSafeLocation(const Vector& v, float f, unsigned long oidCreature,
                                               void* p, Vector* out)
{
    CSWSCreature* pCreature = GetCreatureByGameObjectID(oidCreature);
    if (pCreature == NULL) {
        return 0;
    }
    return GetAreaByGameObjectID(pCreature->m_oidArea)
        ->ComputeSafeLocation(v, f, pCreature->m_pPathfindInformation, p, out);
}

// FUNCTION: SWKOTOR 0x004B4FF0
Vector CServerExoAppInternal::GetNormalOfPoint(const Vector& vPoint)
{
    int nFace;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    CSWSRoom* pRoom = pModule->GetArea()->GetRoom(vPoint, &nFace);
    if (pRoom) {
        return pRoom->GetNormal(nFace);
    }
    return Vector(0.0f, 0.0f, 1.0f);
}

// FUNCTION: SWKOTOR 0x004B5F50
int CServerExoAppInternal::LoadPrimaryPlayer(CSWSPlayer* pPlayer)
{
    int nIndex = g_pAppManager->GetServerExoApp()->GetModule()->GetPrimaryPlayerIndex();
    if (nIndex == -1) {
        return 0;
    }
    pPlayer->m_nPrimaryPlayerIndex = nIndex;
    LoadCharacterFinish(pPlayer, 1);
    return 1;
}

// FUNCTION: SWKOTOR 0x004B2520
int CServerExoAppInternal::HasEnoughDiskSpaceForSaveGame()
{
    unsigned __int64 nFreeBytes = 0;
    {
        CExoString sPath("SAVES:");
        g_pExoResMan->GetFreeDiskSpace(sPath, &nFreeBytes);
    }
    if ((__int64)(nFreeBytes >> 14) > 0x640) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B4E80
void CServerExoAppInternal::UpdateMapData()
{
    CClientPartyList* pParty = g_pAppManager->m_pClientExoApp->GetPartyList();
    CSWSModule* pModule = GetModule();
    if (pParty && pModule) {
        CSWSAreaMap* pMap = pModule->m_pAreaMap;
        if (pMap) {
            for (int i = 0; i < pParty->m_nCount; i++) {
                CClientPartyMember* pMember = pParty->GetMember(i);
                if (pMember) {
                    pMap->SetWorldPointExplored(pMember->m_vPosition, 1);
                    pMap->SetPartyMemberWorldLocation(i, pMember->m_vPosition);
                    pMap->SetPartyMemberWorldOrientation(i, pMember->m_vOrientation);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004B4F50
void CServerExoAppInternal::ForceRunAreaOnExitScripts()
{
    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos)->AsSWSPlayer();
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject) {
                CSWSCreature* pCreature = pObject->AsSWSCreature();
                if (pCreature) {
                    CSWSArea* pArea = pCreature->GetArea();
                    if (pArea) {
                        pArea->m_oidLastExited = pCreature->m_idSelf;
                        g_pVirtualMachine->RunScript(&pArea->m_sScriptOnExit, pArea->m_oid120, 1);
                    }
                }
            }
        }
        m_pPlayerList->GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x004B1FE0
int CServerExoAppInternal::CreatureAcquireItem(unsigned long oidCreature, const CResRef& cTemplate)
{
    CGameObject* pObject = 0;
    if (m_pGameObjArray->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->AsSWSCreature()) {
        CSWSItem* pItem = new CSWSItem(0x7f000000);
        pItem->LoadFromTemplate(cTemplate);
        pItem->m_nItemFlags &= ~4;
        return pObject->AsSWSCreature()->AcquireItem(&pItem, 0x7f000000, 0x7f000000, 1);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004B3070
int CServerExoAppInternal::SendCharacterQuery(CSWSPlayer* pPlayer)
{
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->FUN_004aedb0();
    unsigned char nCount;
    unsigned __int64 n1;
    struct { unsigned long n2; unsigned long n3; } s;    // adjacent pair (stack layout)
    pPlayer->GetCharacterInfoFromIFO(&nCount, &n1, &s.n2, &s.n3);
    pMessage->SendServerToPlayerLogin_CharacterQuery(pPlayer, &nCount, &n1, &s.n2, &s.n3);
    return 1;
}

// FUNCTION: SWKOTOR 0x004B1B00
void CServerExoAppInternal::GetServerInfoFromIniFile()
{
    CExoIni cIni;
    CExoString s1;
    CExoString s2;
    CExoString s3;
    {
        CExoString sMode("Server");
        m_pNetLayer->StartServerMode(sMode, 0x10);
    }
    m_pServerInfo->m_nMaxPlayers = 0x10;
    m_pServerInfo->FUN_0054ea70(2);
    m_pServerInfo->m_nc4 = 1;
    m_pServerInfo->m_ne0 = 0;
    m_pServerInfo->m_nec = 0;
    m_nAutoSaveElapsed = 0;
    m_pServerInfo->m_n98 = 1;
    m_pServerInfo->m_nc8 = 1;
}
