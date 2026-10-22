// CSWSModule leaves (area cache, time, limbo, journal, update) and
// the CResIFO / CExoArrayList / CWorldJournalEntry helpers around them
// (0x004C30C0-0x004C8940).
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C30C0
CResIFO::CResIFO()
{
    strncpy(m_pFileType, "IFO ", 4);
}

// FUNCTION: SWKOTOR 0x004C30F0
CSWSArea* CSWSModule::GetArea()
{
    if (m_oidCachedArea != m_oidArea) {
        m_pCachedArea = GetServerExoApp()->GetAreaByGameObjectID(m_oidArea);
    }
    if (m_pCachedArea) {
        m_oidCachedArea = m_oidArea;
    } else {
        ClearAreaCache();
    }
    return m_pCachedArea;
}

// FUNCTION: SWKOTOR 0x004C3150 SYMBOL: ?ClearAreaCache@CSWSModule@@QAEXXZ

// FUNCTION: SWKOTOR 0x004C3FE0 SYMBOL: ??0CWorldJournalEntry@@QAE@XZ
// FUNCTION: SWKOTOR 0x004C4030 SYMBOL: ??1CWorldJournalEntry@@QAE@XZ
// FUNCTION: SWKOTOR 0x004C4080 SYMBOL: ??0CWorldJournalEntry@@QAE@ABV0@@Z
CWorldJournalEntry* NewWorldJournalEntries(int n)
{
    return new CWorldJournalEntry[n];
}

CWorldJournalEntry* CopyWorldJournalEntry(const CWorldJournalEntry& e)
{
    return new CWorldJournalEntry(e);
}

// FUNCTION: SWKOTOR 0x004C4100
void CSWSModule::GetTime(unsigned long* pYear, unsigned long* pMonth, unsigned long* pDay,
                         unsigned long* pHour, unsigned char* pPhase, unsigned long* pTime)
{
    *pYear = m_nYear;
    *pMonth = m_nMonth;
    *pDay = m_nDay;
    *pHour = m_nHour;
    *pPhase = m_nPhase;
    *pTime = m_nTransitionTime;
}

// FUNCTION: SWKOTOR 0x004C4E90 SYMBOL: ?DelIndex@?$CExoArrayList@UCSWSTagNode@@@@QAEXH@Z
template void CExoArrayList<CSWSTagNode>::DelIndex(int nIndex);

template <class T>
int CExoArrayList<T>::contains(T t)
{
    for (int i = 0; i < num; i++) {
        if (*element[i] == *t) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004C4ED0 SYMBOL: ?contains@?$CExoArrayList@PAUNWPlayerCharacterList_st@@@@QAEHPAUNWPlayerCharacterList_st@@@Z
template int CExoArrayList<NWPlayerCharacterList_st*>::contains(NWPlayerCharacterList_st* t);

// FUNCTION: SWKOTOR 0x004C4F30 SYMBOL: ??_G?$CResHelper@VCResIFO@@$0HNO@@@UAEPAXI@Z
CResHelper<CResIFO, 2014>* NewResHelperIFO()
{
    return new CResHelper<CResIFO, 2014>;
}

// FUNCTION: SWKOTOR 0x004C4F50 SYMBOL: ?Allocate@?$CExoArrayList@UCSWSModulePair@@@@QAEXH@Z
template void CExoArrayList<CSWSModulePair>::Allocate(int n);

// FUNCTION: SWKOTOR 0x004C4FA0 SYMBOL: ?Allocate@?$CExoArrayList@UCSWSTagNode@@@@QAEXH@Z
template void CExoArrayList<CSWSTagNode>::Allocate(int n);

// FUNCTION: SWKOTOR 0x004C5BB0
void CSWSModule::SaveLimboCreatures(CResGFF* pRes, CResStruct* pStruct)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CGameObject* pObject;
    CResStruct cStruct;
    pRes->AddList(&cList, pStruct, "Creature List");
    for (int i = 0; i < m_aLimbo.num; i++) {
        if (pArray->GetGameObject(m_aLimbo[i], &pObject) == GAME_OBJECT_VALID) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            pRes->AddListElement(&cStruct, &cList, 4);
            pRes->WriteFieldDWORD(&cStruct, pCreature->m_idSelf, "ObjectId");
            pCreature->SaveCreature(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x004C5D90
int CSWSModule::GetPrimaryPlayerIndex()
{
    int nResult = -1;
    int nIndex = 0;
    CExoLinkedListPosition pos = m_lPlayers.m_pList->m_pHead;
    while (pos) {
        CSWSModulePlayerEntry* pEntry = (CSWSModulePlayerEntry*)m_lPlayers.m_pList->GetAtPos(pos);
        if (pEntry->m_bPrimary == 1) {
            nResult = nIndex;
            break;
        }
        m_lPlayers.m_pList->GetNext(pos);
        nIndex++;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x004C5E60
int CSWSModule::FindTagNode(const char* szTag)
{
    int nLow = 0;
    int nHigh = m_aTagNodes.num - 1;
    while (nLow < nHigh || nLow == m_aTagNodes.num - 1) {
        int nMid = (nLow + nHigh) >> 1;
        if (strcmp(m_aTagNodes[nMid].m_szTag, szTag) >= 0) {
            if (nMid == nHigh) {
                return nMid;
            }
            nHigh = nMid;
        } else {
            nLow = nMid + 1;
        }
    }
    return nLow;
}

// FUNCTION: SWKOTOR 0x004C61D0
int CSWSModule::GetNextWorldJournalEntryID()
{
    int nID = 1;
    for (int i = 0; i < m_aWorldJournal.num; i++) {
        if (!m_aWorldJournal[i].m_bStrref && m_aWorldJournal[i].m_nID == nID) {
            nID++;
            i = -1;
        }
    }
    return nID;
}

// FUNCTION: SWKOTOR 0x004C63F0
void CSWSModule::RemoveObjectFromLimbo(unsigned long oid)
{
    for (int i = m_aLimbo.num - 1; i >= 0; i--) {
        if (oid == m_aLimbo[i]) {
            m_aLimbo.DelIndex(i);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004C6450
int CSWSModule::IsObjectInLimbo(unsigned long oid)
{
    return m_aLimbo.Contains(oid);
}

// FUNCTION: SWKOTOR 0x004C6480
void CSWSModule::TimeStopSanityCheck()
{
    int bFound = 0;
    CExoLinkedList<unsigned long>* pList = GetServerExoApp()->GetActiveExclusionList();
    CExoLinkedListPosition pos = pList->GetHeadPos();
    while (pos && !bFound) {
        unsigned long oid = (unsigned long)pList->GetAtPos(pos);
        CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(oid);
        if (pCreature) {
            for (int i = 0; i < pCreature->m_appliedEffects.num; i++) {
                if (pCreature->m_appliedEffects[i]->m_nType == EFFECT_TYPE_TIMESTOP) {
                    bFound = 1;
                }
            }
        }
        pList->GetNext(pos);
    }
    if (!bFound) {
        GetServerExoApp()->TogglePauseState(1);
    }
}

// FUNCTION: SWKOTOR 0x004C6520 SYMBOL: ?Insert@?$CExoArrayList@UCSWSTagNode@@@@QAEXUCSWSTagNode@@H@Z
template void CExoArrayList<CSWSTagNode>::Insert(CSWSTagNode t, int nIndex);

// FUNCTION: SWKOTOR 0x004C65B0 SYMBOL: ??1?$CExoArrayList@VCWorldJournalEntry@@@@QAE@XZ
template CExoArrayList<CWorldJournalEntry>::~CExoArrayList();

// FUNCTION: SWKOTOR 0x004C66B0 SYMBOL: ?DelIndex@?$CExoArrayList@VCWorldJournalEntry@@@@QAEXH@Z
template void CExoArrayList<CWorldJournalEntry>::DelIndex(int nIndex);

// FUNCTION: SWKOTOR 0x004C6C90
int CSWSModule::AIUpdate()
{
    unsigned long nDay;
    unsigned long nTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    unsigned long nDiffDays;
    unsigned long nDiffTime;
    GetServerExoApp()->GetWorldTimer()->FUN_004ae460(nDay, nTime, m_nLastHeartbeatDay,
                                                     m_nLastHeartbeatTime, &nDiffDays,
                                                     &nDiffTime);
    if (nDiffTime >= 6000) {
        if (m_nLastHeartbeatTime != 0) {
            g_pVirtualMachine->RunScript(&m_sScripts[0], m_idSelf, 1);
        }
        m_nLastHeartbeatDay = nDay;
        m_nLastHeartbeatTime = nTime;
        return 1;
    }
    GetServerExoApp()->GetWorldTimer()->FUN_004ae460(nDay, nTime, m_nLastUpdateDay,
                                                     m_nLastUpdateTime, &nDiffDays,
                                                     &nDiffTime);
    UpdateTime(nDay, nTime, nDiffTime);
    m_nLastUpdateDay = nDay;
    m_nLastUpdateTime = nTime;
    if (GetServerExoApp()->GetActivePauseState() == 1) {
        TimeStopSanityCheck();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004C6DC0
void CSWSModule::DoUpdate()
{
    if (m_bAIUpdateToggle) {
        m_bAIUpdateToggle = 0;
        AIUpdate();
        return;
    }
    CSWSArea* pArea = GetServerExoApp()->GetAreaByGameObjectID(m_oidArea);
    if (pArea) {
        pArea->FUN_00508a30();
    }
    m_bAIUpdateToggle = 1;
}

// FUNCTION: SWKOTOR 0x004C6E00
unsigned long CSWSModule::GetWaypoint(const CExoString& sTag)
{
    return GetObjectByTagAndType(sTag, OBJECT_TYPE_WAYPOINT, 0);
}

// FUNCTION: SWKOTOR 0x004C8170
void CSWSModule::DeleteWorldJournalEntry(unsigned long nID)
{
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    for (int i = m_aWorldJournal.num; i >= 0; i--) {
        if (!m_aWorldJournal[i].m_bStrref && m_aWorldJournal[i].m_nID == nID) {
            m_aWorldJournal.DelIndex(i);
            break;
        }
    }
    if (pMessage) {
        CExoLinkedList<CSWSPlayer>* pList = GetServerExoApp()->GetPlayerList();
        if (pList) {
            CExoLinkedListPosition pos = pList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pPlayer = pList->GetAtPos(pos);
                if (pPlayer && pPlayer->GetGameObject()) {
                    pMessage->SendServerToPlayerJournalDeleteWorld(pPlayer, nID);
                }
                pList->GetNext(pos);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004C8230
void CSWSModule::DeleteWorldJournalEntryStrref(unsigned long nStrref)
{
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    for (int i = m_aWorldJournal.num; i >= 0; i--) {
        if (m_aWorldJournal[i].m_bStrref && m_aWorldJournal[i].m_nID == nStrref) {
            m_aWorldJournal.DelIndex(i);
            break;
        }
    }
    CExoLinkedList<CSWSPlayer>* pList = GetServerExoApp()->GetPlayerList();
    if (pList) {
        CExoLinkedListPosition pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalDeleteWorldStrref(pPlayer, nStrref);
            }
            pList->GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x004C82F0
void CSWSModule::DeleteWorldJournalAllEntries()
{
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    m_aWorldJournal.Clear();
    CExoLinkedList<CSWSPlayer>* pList = GetServerExoApp()->GetPlayerList();
    if (pList) {
        CExoLinkedListPosition pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalDeleteWorldAll(pPlayer);
            }
            pList->GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x004C83B0
void CSWSModule::AddObjectToLimbo(unsigned long oid)
{
    for (int i = 0; i < m_aLimbo.num; i++) {
        if (oid == m_aLimbo[i]) {
            return;
        }
    }
    m_aLimbo.Add(oid);
}

// FUNCTION: SWKOTOR 0x004C8940 SYMBOL: ??_GCSWSModule@@UAEPAXI@Z
CSWSModule* NewModule()
{
    return new CSWSModule;
}
