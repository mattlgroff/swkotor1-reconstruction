// CFactionManager (0x0052B280-0x0052BF7F).
// SaveReputations (0x0052B830) lives in matching/src/pilot3/faction.cpp.

#include "b/faction_manager.h"
#include "b/sw_rules.h"

// FUNCTION: SWKOTOR 0x0052B280
int CFactionManager::IsValidFactionId(int nFactionId)
{
    if (nFactionId >= 1 && nFactionId <= m_nNumFactions) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052B2A0
void CFactionManager::ResolveFactionName(const CExoString& sName, CFaction* pFaction)
{
    if (sName.CompareNoCase("player")) {
        m_pPlayer = pFaction;
    }
    else if (sName.CompareNoCase("hostile_1")) {
        m_pHostile1 = pFaction;
    }
    else if (sName.CompareNoCase("friendly_1")) {
        m_pFriendly1 = pFaction;
    }
    else if (sName.CompareNoCase("hostile_2")) {
        m_pHostile2 = pFaction;
    }
    else if (sName.CompareNoCase("friendly_2")) {
        m_pFriendly2 = pFaction;
    }
    else if (sName.CompareNoCase("neutral")) {
        m_pNeutral = pFaction;
    }
    else if (sName.CompareNoCase("insane")) {
        m_pInsane = pFaction;
    }
}

// FUNCTION: SWKOTOR 0x0052B370
void CFactionManager::DeleteFaction(int nFactionId)
{
    if (m_pFactionList) {
        CFaction* pFaction = m_pFactionList->element[nFactionId];
        if (pFaction) {
            delete pFaction;
            m_pFactionList->element[nFactionId] = NULL;
        }
    }
}

// FUNCTION: SWKOTOR 0x0052B3B0
CFaction* CFactionManager::GetFaction(int nFactionId)
{
    if (nFactionId >= 0 && nFactionId < m_pFactionList->num) {
        return m_pFactionList->element[nFactionId];
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x0052B3D0
void CFactionManager::SetNPCReputation(int nFactionId1, int nFactionId2, int nReputation)
{
    if (nFactionId2 >= 1 && nFactionId2 <= m_nNumFactions &&
        nFactionId1 >= 0 && nFactionId1 <= m_nNumFactions) {
        if (nReputation > 100) {
            nReputation = 100;
        }
        else if (nReputation < 0) {
            nReputation = 0;
        }
        int nIndex = m_nNumFactions * nFactionId1 + nFactionId2 - 1;
        if (nIndex < m_pReputations->num) {
            m_pReputations->element[nIndex] = nReputation;
        }
    }
}

// FUNCTION: SWKOTOR 0x0052B420
int CFactionManager::GetNPCReputation(int nFactionId1, int nFactionId2)
{
    if (!IsValidFactionId(nFactionId1) && !IsValidFactionId(nFactionId2)) {
        return 50;
    }
    if (!IsValidFactionId(nFactionId1) && IsValidFactionId(nFactionId2)) {
        return GetNPCReputation(nFactionId2, nFactionId1);
    }
    int nIndex = m_nNumFactions * nFactionId2 + nFactionId1 - 1;
    if (nIndex >= 0 && nIndex < m_pReputations->num) {
        return m_pReputations->element[nIndex];
    }
    return 50;
}

// FUNCTION: SWKOTOR 0x0052B490
// NONMATCH: register allocation only: the original keeps the repute 2DA in
// ebx and spills `this`; ours spills the 2DA and keeps a zero in ebx.
int CFactionManager::LoadFactions(int bAppend)
{
    CExoString sLabel;
    C2DA* pRepute = g_pRules->m_p2DArrays->m_pRepute;
    int nRows = pRepute->m_nNumRows;
    int nRow = 0;
    if (bAppend) {
        nRow = m_pFactionList->num;
    }
    for (; nRow < nRows; nRow++) {
        pRepute->GetCExoStringEntry(nRow, "LABEL", &sLabel);
        CFaction* pFaction = new CFaction();
        pFaction->m_sFactionName = sLabel;
        pFaction->m_bGlobal = 1;
        pFaction->m_nParentFactionId = -1;
        ResolveFactionName(sLabel, pFaction);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0052B5C0
int CFactionManager::LoadFactionsFromSaveGame(CResGFF* pRes, CResList* pList)
{
    CExoString sName;
    CResStruct cElement;
    int bSuccess = 1;
    int nCount = pRes->GetListCount(pList);
    for (int i = 0; i < nCount && bSuccess; i++) {
        pRes->GetListElement(&cElement, pList, i);
        sName = pRes->ReadFieldCExoString(&cElement, "FactionName", bSuccess, "");
        int nParent = pRes->ReadFieldDWORD(&cElement, "FactionParentID", bSuccess, 0);
        int bGlobal = pRes->ReadFieldWORD(&cElement, "FactionGlobal", bSuccess, 0);
        if (!bSuccess) {
            bGlobal = 1;
            bSuccess = 1;
        }
        CFaction* pFaction = new CFaction();
        pFaction->m_sFactionName = sName;
        pFaction->m_bGlobal = bGlobal;
        pFaction->m_nParentFactionId = nParent;
        ResolveFactionName(sName, pFaction);
    }
    int bLoaded = LoadFactions(1);
    return bSuccess && bLoaded;
}

// FUNCTION: SWKOTOR 0x0052B790
int CFactionManager::SaveFactions(CResGFF* pRes, CResList* pList)
{
    for (int i = 0; i < m_nNumFactions + 1; i++) {
        CFaction* pFaction = GetFaction(i);
        CResStruct cStruct;
        pRes->AddListElement(&cStruct, pList, i);
        pRes->WriteFieldCExoString(&cStruct, pFaction->m_sFactionName, "FactionName");
        pRes->WriteFieldDWORD(&cStruct, pFaction->m_nParentFactionId, "FactionParentID");
        pRes->WriteFieldWORD(&cStruct, (unsigned short)pFaction->m_bGlobal, "FactionGlobal");
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0052B8E0
CFactionManager::CFactionManager()
{
    m_pPlayer = NULL;
    m_pHostile1 = NULL;
    m_pHostile2 = NULL;
    m_pFriendly1 = NULL;
    m_pFriendly2 = NULL;
    m_pInsane = NULL;
    m_pNeutral = NULL;
    m_pFactionList = new CExoArrayList<CFaction*>;
    m_nNumFactions = 0;
    m_pReputations = NULL;
}

// FUNCTION: SWKOTOR 0x0052B950
CFactionManager::~CFactionManager()
{
    if (m_pFactionList) {
        for (int i = 0; i < m_pFactionList->num; i++) {
            if (m_pFactionList->element[i]) {
                delete m_pFactionList->element[i];
            }
        }
        delete m_pFactionList;
    }
    if (m_pReputations) {
        delete m_pReputations;
        m_pReputations = NULL;
    }
}

// FUNCTION: SWKOTOR 0x0052B9E0
int CFactionManager::LoadReputations()
{
    int nEntries = (m_nNumFactions + 1) * m_nNumFactions;
    int bSuccess = 1;
    if (m_pReputations) {
        delete m_pReputations;
    }
    m_pReputations = new CExoArrayList<int>;
    for (int k = 0; k < nEntries; k++) {
        m_pReputations->Add(100);
    }
    m_pReputations->SetSize(m_pReputations->num);

    C2DA* pRepute = g_pRules->m_p2DArrays->m_pRepute;
    int nRows = pRepute->m_nNumRows;
    int nColumns = pRepute->m_nNumColumns;
    for (int i = 0; bSuccess; i++) {
        if (i > nRows) {
            break;
        }
        for (int j = 0; bSuccess && j < nColumns; j++) {
            int nValue;
            bSuccess = pRepute->GetINTEntry(i, j, &nValue);
            SetNPCReputation(i, j, nValue);
        }
    }
    return bSuccess;
}

// FUNCTION: SWKOTOR 0x0052BBE0
int CFactionManager::LoadReputationsFromSaveGame(CResGFF* pRes, CResList* pList)
{
    CResStruct cElement;
    int bSuccess;
    LoadReputations();
    bSuccess = 1;
    int nCount = pRes->GetListCount(pList);
    int i = 0;
    while (bSuccess && i <= nCount) {
        pRes->GetListElement(&cElement, pList, i);
        int nFactionId1 = pRes->ReadFieldDWORD(&cElement, "FactionID1", bSuccess, 0);
        int nFactionId2 = pRes->ReadFieldDWORD(&cElement, "FactionID2", bSuccess, 0);
        int nReputation = pRes->ReadFieldDWORD(&cElement, "FactionRep", bSuccess, 0);
        SetNPCReputation(nFactionId1, nFactionId2, nReputation);
        i++;
    }
    return bSuccess;
}

// FUNCTION: SWKOTOR 0x0052BCE0
void CFactionManager::CreateDefaultFactions()
{
    m_pPlayer = new CFaction();
    m_pHostile1 = new CFaction();
    m_pHostile2 = new CFaction();
    m_pFriendly1 = new CFaction();
    m_pFriendly2 = new CFaction();
    m_pNeutral = new CFaction();
    m_pInsane = new CFaction();
    int nEntries = (m_nNumFactions + 1) * m_nNumFactions;
    m_pReputations = new CExoArrayList<int>;
    m_pReputations->Add(0);
    for (int k = 1; k < nEntries; k++) {
        m_pReputations->Add(100);
    }
    m_pReputations->SetSize(m_pReputations->num);
}
