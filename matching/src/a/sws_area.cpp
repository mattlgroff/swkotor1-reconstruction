// CSWSArea leaves, object-list saves and stealth XP
// (0x00506A10-0x00508C4F; behavioural reference:
// src/unknown/sub_0050_area_leaf_cluster.cpp).
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x00506A10
CSWCArea* CSWSArea::GetClientArea()
{
    if (m_pClientArea == NULL) {
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        m_pClientArea = pClient->GetAreaByGameObjectID(pClient->ServerToClientObjectId(m_idSelf));
        if (m_pClientArea != NULL) {
            m_pClientArea->SetServerArea(this);
        }
    }
    return m_pClientArea;
}

// FUNCTION: SWKOTOR 0x00506A60
void CSWSArea::AttachmentFromClientArea(CSWCArea* pArea)
{
    m_pClientArea = pArea;
}

// FUNCTION: SWKOTOR 0x00506A70
void CSWSArea::DetachFromClientArea()
{
    m_pClientArea = NULL;
}

// FUNCTION: SWKOTOR 0x00506A80
void CSWSArea::SetStealthXPMax(unsigned long nMax)
{
    m_nStealthXPMax = nMax;
    if (m_nStealthXPCurrent > nMax) {
        m_nStealthXPCurrent = nMax;
    }
}

// FUNCTION: SWKOTOR 0x00506AA0
void CSWSArea::SetStealthXPCurrent(unsigned long nCurrent)
{
    if (nCurrent > m_nStealthXPMax) {
        m_nStealthXPCurrent = m_nStealthXPMax;
    }
    else {
        m_nStealthXPCurrent = nCurrent;
    }
}

// FUNCTION: SWKOTOR 0x00506AC0
unsigned char CSWSArea::GetNextTransPendingID()
{
    if (++m_nTransPendNextID == 0) {
        m_nTransPendNextID = 1;
    }
    return m_nTransPendNextID;
}

// FUNCTION: SWKOTOR 0x00506AE0
void CSWSArea::SetSunFogColor(const Vector& vColor)
{
    m_nSunFogColor = ((unsigned char)(vColor.z * 255.0f) << 16)
                   | ((unsigned char)(vColor.y * 255.0f) << 8)
                   | (unsigned char)(vColor.x * 255.0f);
}

// FUNCTION: SWKOTOR 0x00506B30
void CSWSArea::SetTransitionPending(int bPending, unsigned char nID)
{
    m_bTransPending = bPending;
    m_nTransPendCurrID = nID;
    if (bPending) {
        g_pAppManager->m_pClientExoApp->SetInputActive(0, 1);
    }
    else {
        g_pAppManager->m_pClientExoApp->SetInputActive(1, 0);
    }
}

// FUNCTION: SWKOTOR 0x00506B90
void CSWSArea::SetPlayerRestrictMode(int nMode)
{
    if (nMode && m_nRestrictMode != nMode) {
        GetServerExoApp()->GetPartyTable()->UnstealthParty(1);
    }
    m_nRestrictMode = nMode;
}

// FUNCTION: SWKOTOR 0x005071B0
int CSWSArea::GetFirstObjectIdInArea(unsigned long* pOid)
{
    m_nObjectIterator = 0;
    if (m_aGameObjects.num == 0) {
        return 0;
    }
    *pOid = m_aGameObjects.element[0];
    return 1;
}

// FUNCTION: SWKOTOR 0x005071E0
int CSWSArea::GetNextObjectIdInArea(unsigned long* pOid)
{
    if (++m_nObjectIterator < m_aGameObjects.num) {
        *pOid = m_aGameObjects.element[m_nObjectIterator];
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00507340
void CSWSArea::DeactivateAI()
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
    for (int i = 0; i < m_aGameObjects.num; i++) {
        CGameObject* pObject;
        if (pArray->GetGameObject(m_aGameObjects.element[i], &pObject) == g_nGameObjectFound &&
            pObject->m_nObjectType > g_nObjectTypeArea &&
            pObject->AsSWSCreature() &&
            ((CSWSObject*)pObject)->m_nAILevel == 1) {
            pAIMaster->SetAILevel((CSWSObject*)pObject, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507680
void CSWSArea::SaveCreatures(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    GetServerExoApp()->GetPartyTable();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "Creature List");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature->m_bPartyMember == 0) {
                pRes->AddListElement(&cStruct, &cList, 4);
                pRes->WriteFieldDWORD(&cStruct, pCreature->m_idSelf, "ObjectId");
                pCreature->SaveCreature(pRes, &cStruct);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00507750
void CSWSArea::SaveItems(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "List");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSItem* pItem = pObject->AsSWSItem();
            pRes->AddListElement(&cStruct, &cList, 0);
            pRes->WriteFieldDWORD(&cStruct, pItem->m_idSelf, "ObjectId");
            pItem->SaveItem(pRes, &cStruct);
            pItem->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507810
void CSWSArea::SaveDoors(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "Door List");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSDoor* pDoor = pObject->AsSWSDoor();
            pRes->AddListElement(&cStruct, &cList, 8);
            pRes->WriteFieldDWORD(&cStruct, pDoor->m_idSelf, "ObjectId");
            pDoor->SaveDoor(pRes, &cStruct);
            pDoor->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x005078D0
void CSWSArea::SaveTriggers(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "TriggerList");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSTrigger* pTrigger = pObject->AsSWSTrigger();
            pRes->AddListElement(&cStruct, &cList, 1);
            pRes->WriteFieldDWORD(&cStruct, pTrigger->m_idSelf, "ObjectId");
            pTrigger->SaveTrigger(pRes, &cStruct);
            pTrigger->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507990
void CSWSArea::SaveEncounters(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "Encounter List");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSEncounter* pEncounter = pObject->AsSWSEncounter();
            pRes->AddListElement(&cStruct, &cList, 7);
            pRes->WriteFieldDWORD(&cStruct, pEncounter->m_idSelf, "ObjectId");
            pEncounter->SaveEncounter(pRes, &cStruct);
            pEncounter->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507A50
void CSWSArea::SaveWaypoints(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "WaypointList");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
            pRes->AddListElement(&cStruct, &cList, 5);
            pRes->WriteFieldDWORD(&cStruct, pWaypoint->m_idSelf, "ObjectId");
            pWaypoint->SaveWaypoint(pRes, &cStruct);
            pWaypoint->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507B10
void CSWSArea::SaveSounds(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "SoundList");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSSoundObject* pSound = pObject->AsSWSSoundObject();
            pRes->AddListElement(&cStruct, &cList, 6);
            pRes->WriteFieldDWORD(&cStruct, pSound->m_idSelf, "ObjectId");
            pSound->Save(pRes, &cStruct);
            pSound->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507BD0
void CSWSArea::SavePlaceables(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "Placeable List");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
            if (pPlaceable->m_bPartyOwned == 0) {
                pRes->AddListElement(&cStruct, &cList, 9);
                pRes->WriteFieldDWORD(&cStruct, pPlaceable->m_idSelf, "ObjectId");
                pPlaceable->SavePlaceable(pRes, &cStruct);
                pPlaceable->SaveObjectState(pRes, &cStruct);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00507CA0
void CSWSArea::SaveStores(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "StoreList");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSStore* pStore = pObject->AsSWSStore();
            pRes->AddListElement(&cStruct, &cList, 0xb);
            pRes->WriteFieldDWORD(&cStruct, pStore->m_idSelf, "ObjectId");
            pStore->SaveStore(pRes, &cStruct);
            pStore->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00507D60
void CSWSArea::SaveAreaEffects(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CResList cList;
    CResStruct cStruct;
    CGameObject* pObject;
    pRes->AddList(&cList, pStruct, "AreaEffectList");
    for (int i = 0; i < pList->num; i++) {
        if (pArray->GetGameObject(pList->element[i], &pObject) == g_nGameObjectFound) {
            CSWSAreaOfEffectObject* pEffect = pObject->AsSWSAreaOfEffectObject();
            pRes->AddListElement(&cStruct, &cList, 0xd);
            pRes->WriteFieldDWORD(&cStruct, pEffect->m_idSelf, "ObjectId");
            pEffect->SaveEffect(pRes, &cStruct);
            pEffect->SaveObjectState(pRes, &cStruct);
        }
    }
}

// FUNCTION: SWKOTOR 0x00508710
void CSWSArea::DeleteAllObjects()
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    for (int i = m_aGameObjects.num - 1; i >= 0; i--) {
        CGameObject* pObject;
        pArray->GetGameObject(m_aGameObjects.element[i], &pObject);
        if (pObject) {
            delete pObject;
        }
    }
    m_aGameObjects.num = 0;
}

// FUNCTION: SWKOTOR 0x00508770
void CSWSArea::AwardStealthXP(unsigned long oidCreature)
{
    if (GetServerExoApp()->GetCreatureByGameObjectID(oidCreature) &&
        m_bStealthXPEnabled && m_nStealthXPCurrent > 0) {
        if (GetServerExoApp()->GetPartyTable()) {
            GetServerExoApp()->GetPartyTable()->DistributeExperience(m_nStealthXPCurrent, 1);
        }
        if (g_pAppManager->m_pClientExoApp->GetInGameGui()) {
            g_pAppManager->m_pClientExoApp->GetInGameGui()->ShowStealthXP(3, m_nStealthXPCurrent);
        }
        m_nStealthXPCurrent = 0;
        m_bStealthXPEnabled = 0;
    }
}

// FUNCTION: SWKOTOR 0x00508960
unsigned char CSWSArea::GetNumMinesSetByPlayerParty()
{
    unsigned char nMines = 0;
    for (int i = 0; i < m_aTriggers.num; i++) {
        CSWSTrigger* pTrigger = GetServerExoApp()->GetTriggerByGameObjectID(m_aTriggers.element[i]);
        if (pTrigger && pTrigger->m_bIsTrap && pTrigger->m_bCreatedByPlayer) {
            nMines++;
        }
    }
    return nMines;
}

// FUNCTION: SWKOTOR 0x005089D0
int CSWSArea::GetPlayerCanSetMines()
{
    return GetNumMinesSetByPlayerParty() < 15;
}

// FUNCTION: SWKOTOR 0x00508C20
void CSWSArea::IncrementPlayersInArea()
{
    if (m_nPlayersInArea == 0) {
        ActivateAI();
    }
    m_nPlayersInArea++;
}

// FUNCTION: SWKOTOR 0x00508C40
void CSWSArea::DecrementPlayersInArea()
{
    if (--m_nPlayersInArea == 0) {
        DeactivateAI();
    }
}
