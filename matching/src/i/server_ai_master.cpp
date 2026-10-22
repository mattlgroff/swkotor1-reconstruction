// CServerAIMaster (0x004AF0F0-0x004B1200): handler forwarders,
// AI level lists, the event queue and its save/load, plus the small helper
// classes defined alongside it.
#include "i/server_ai_master.h"

// FUNCTION: SWKOTOR 0x004AF0F0
void CServerAIMaster::OnEffectApplied(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    m_pEffectListHandler->OnApplyEffect(pObject, pEffect, bLoadingGame);
}

// FUNCTION: SWKOTOR 0x004AF100
void CServerAIMaster::OnEffectRemoved(CSWSObject* pObject, CGameEffect* pEffect)
{
    m_pEffectListHandler->OnRemoveEffect(pObject, pEffect);
}

// FUNCTION: SWKOTOR 0x004AF110
void CServerAIMaster::OnItemPropertyApplied(CSWSItem* pItem, CSWItemProperty* pProperty,
                                            CSWSCreature* pCreature, unsigned long nSlot, int bArg)
{
    m_pItemPropertyHandler->OnApplyItemProperty(pItem, pProperty, pCreature, nSlot, bArg);
}

// FUNCTION: SWKOTOR 0x004AF120
void CServerAIMaster::OnItemPropertyRemoved(CSWSItem* pItem, CSWItemProperty* pProperty,
                                            CSWSCreature* pCreature, unsigned long nSlot)
{
    m_pItemPropertyHandler->OnRemoveItemProperty(pItem, pProperty, pCreature, nSlot);
}

// FUNCTION: SWKOTOR 0x004AF130
CSWSSpellScriptData::CSWSSpellScriptData()
    : m_n10(0), m_n14(0), m_n18(0)
{
}

// FUNCTION: SWKOTOR 0x004AF150
CSWSSpellScriptData::~CSWSSpellScriptData()
{
}

// FUNCTION: SWKOTOR 0x004AF160
int CServerAIBodyBagInfo::SaveBodyBag(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldDWORD(pStruct, m_oidBodyBag, "BodyBagId");
    pGFF->WriteFieldFLOAT(pStruct, m_fX, "PositionX");
    pGFF->WriteFieldFLOAT(pStruct, m_fY, "PositionY");
    pGFF->WriteFieldFLOAT(pStruct, m_fZ, "PositionZ");
    return 1;
}

// FUNCTION: SWKOTOR 0x004AF1C0
int CServerAIBodyBagInfo::LoadBodyBag(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    m_oidBodyBag = pGFF->ReadFieldDWORD(pStruct, "BodyBagId", bSuccess, 0);
    m_fX = pGFF->ReadFieldFLOAT(pStruct, "PositionX", bSuccess, 0.0f);
    m_fY = pGFF->ReadFieldFLOAT(pStruct, "PositionY", bSuccess, 0.0f);
    m_fZ = pGFF->ReadFieldFLOAT(pStruct, "PositionZ", bSuccess, 0.0f);
    return 1;
}

// FUNCTION: SWKOTOR 0x004AF240 SYMBOL: ?count@?$CExoArrayList@K@@QAEHK@Z
template int CExoArrayList<unsigned long>::count(unsigned long t);

// FUNCTION: SWKOTOR 0x004AF260 SYMBOL: ??0?$CExoLinkedList@UCServerAIEventNode@@@@QAE@XZ
template class CExoLinkedList<CServerAIEventNode>;

// FUNCTION: SWKOTOR 0x004AF2D0
CGameEffectApplierRemover::~CGameEffectApplierRemover()
{
}

// FUNCTION: SWKOTOR 0x004AF2E0 SYMBOL: ??_GCGameEffectApplierRemover@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004AF300
CItemPropertyApplierRemover::~CItemPropertyApplierRemover()
{
}

// FUNCTION: SWKOTOR 0x004AF310 SYMBOL: ??_GCItemPropertyApplierRemover@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004AF330
int CServerAIList::RemoveObject(unsigned long oid)
{
    if (m_aoGameObjects.count(oid)) {
        int i = m_aoGameObjects.IndexOf(oid);
        if (i != -1) {
            m_aoGameObjects.DelIndex(i);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004AF390 SYMBOL: ??_GCSWSEffectListHandler@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004AF3B0 SYMBOL: ??_GCSWSItemPropertyHandler@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004AF3D0
int CServerAIMaster::RemoveObject(CSWSObject* pObject)
{
    int nLevel = pObject->m_nAILevel;
    if (nLevel > 4 || nLevel < 0) {
        return 0;
    }
    int nResult = m_aGameAIList[nLevel].RemoveObject(pObject->m_idSelf);
    if (nResult == 1) {
        pObject->m_nAILevel = -1;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x004B0780
CServerAIMaster::CServerAIMaster()
{
    m_pEffectListHandler = new CSWSEffectListHandler;
    m_pEffectListHandler->InitializeEffects();
    m_pItemPropertyHandler = new CSWSItemPropertyHandler;
    m_pItemPropertyHandler->InitializeItemProperties();
    m_pBlock80 = NULL;
    m_pBlock84 = NULL;
    m_pBlock88 = NULL;
    m_pBlock8c = NULL;
}

// FUNCTION: SWKOTOR 0x004B0850
int CServerAIMaster::AddObject(CSWSObject* pObject, int nLevel)
{
    if (nLevel > 4 || nLevel < 0 || pObject == NULL) {
        return 0;
    }
    if (pObject->m_nAILevel != -1) {
        RemoveObject(pObject);
    }
    m_aGameAIList[nLevel].m_aoGameObjects.Add(pObject->m_idSelf);
    pObject->m_nAILevel = nLevel;
    return 1;
}

// FUNCTION: SWKOTOR 0x004B08D0
int CServerAIMaster::AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long nCaller,
                                       unsigned long nObject, unsigned long nEventId, void* pEventData)
{
    unsigned long nCalendarDay;
    unsigned long nTimeOfDay;
    unsigned long nEventDay;
    unsigned long nEventTime;
    g_pAppManager->m_pServerExoApp->GetActivePauseState();
    m_pServer->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    if (!m_pServer->GetWorldTimer()->AddWorldTimes(nCalendarDay, nTimeOfDay, nDays, nTime,
                                                 &nEventDay, &nEventTime)) {
        return AddEventAbsoluteTime(nEventDay, nEventTime, nCaller, nObject, nEventId, pEventData);
    }
    operator delete(pEventData);
    return 0;
}

// FUNCTION: SWKOTOR 0x004B0970
int CServerAIMaster::SaveEventQueue(CResGFF* pGFF, CResStruct* pStruct)
{
    CResList cList;
    CResStruct cElement;
    pGFF->AddList(&cList, pStruct, "EventQueue");
    if (m_lEventQueue.GetCount() > 0) {
        CExoLinkedListPosition pos = m_lEventQueue.GetHeadPos();
        while (pos) {
            CServerAIEventNode* pNode = m_lEventQueue.GetAtPos(pos);
            pGFF->AddListElement(&cElement, &cList, 0xabcd);
            pNode->SaveNode(pGFF, &cElement);
            m_lEventQueue.GetNext(pos);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004B0A00
int CServerAIMaster::LoadEventQueue(CResGFF* pGFF, CResStruct* pStruct)
{
    CResList cList;
    CResStruct cElement;
    if (pGFF->GetList(&cList, pStruct, "EventQueue")) {
        for (unsigned long i = 0; i < pGFF->GetListCount(&cList); i++) {
            CServerAIEventNode* pNode = new CServerAIEventNode;
            pGFF->GetListElement(&cElement, &cList, i);
            if (pNode->LoadNode(pGFF, &cElement)) {
                m_lEventQueue.AddTail(pNode);
            } else {
                delete pNode;
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004B0AB0
void CServerAIMaster::DeleteEventData(unsigned long nEventId, void* pEventData)
{
    if (pEventData == NULL) {
        return;
    }
    switch (nEventId) {
    case 21:
        delete (CSWSCombatAttackData*)pEventData;
        break;
    case 22:
        delete (CUnknown004d6a40*)pEventData;
        break;
    case 8:
    case 19:
        delete (CSWSSpellScriptData*)pEventData;
        break;
    case 1:
        g_pVirtualMachine->DeleteScriptSituation(pEventData);
        break;
    case 10:
    case 24:
    case 26:
        delete (CScriptEvent*)pEventData;
        break;
    }
}

// FUNCTION: SWKOTOR 0x004B11C0
void CServerAIMaster::ClearEventQueue()
{
    while (!m_lEventQueue.IsEmpty()) {
        CServerAIEventNode* pNode = m_lEventQueue.RemoveHead();
        DeleteEventData(pNode->m_nEventId, pNode->m_pEventData);
        delete pNode;
    }
}

// FUNCTION: SWKOTOR 0x004B1200
CServerAIMaster::~CServerAIMaster()
{
    ClearEventQueue();
    if (m_pEffectListHandler) {
        delete m_pEffectListHandler;
        m_pEffectListHandler = NULL;
    }
    if (m_pItemPropertyHandler) {
        delete m_pItemPropertyHandler;
        m_pItemPropertyHandler = NULL;
    }
    if (m_pBlock80) {
        delete m_pBlock80;
        m_pBlock80 = NULL;
    }
    if (m_pBlock84) {
        delete m_pBlock84;
        m_pBlock84 = NULL;
    }
    if (m_pBlock88) {
        delete m_pBlock88;
        m_pBlock88 = NULL;
    }
    if (m_pBlock8c) {
        delete m_pBlock8c;
        m_pBlock8c = NULL;
    }
}

// FUNCTION: SWKOTOR 0x004B08A0
int CServerAIMaster::SetAILevel(CSWSObject* pObject, int nLevel)
{
    if (pObject->m_nAILevel == nLevel) {
        return 1;
    }
    if (nLevel > 4 || nLevel < 0) {
        return 0;
    }
    return AddObject(pObject, nLevel);
}

// FUNCTION: SWKOTOR 0x004AFDB0
int CServerAIMaster::AddEventAbsoluteTime(unsigned long nDay, unsigned long nTime, unsigned long nCaller,
                                          unsigned long nObject, unsigned long nEventId, void* pEventData)
{
    if (g_bLogEvents) {
        LogEventAdd(nCaller, nObject, pEventData);
    }
    CServerAIEventNode* pNew = new CServerAIEventNode;
    pNew->m_nDay = nDay;
    pNew->m_nTime = nTime;
    pNew->m_nCallerObjectId = nCaller;
    pNew->m_nObjectId = nObject;
    pNew->m_nEventId = nEventId;
    pNew->m_pEventData = pEventData;
    int bInserted = 0;
    CExoLinkedListPosition pos = m_lEventQueue.GetHeadPos();
    while (pos && !bInserted) {
        CServerAIEventNode* pNode = m_lEventQueue.GetAtPos(pos);
        if (m_pServer->GetWorldTimer()->CompareWorldTimes(pNode->m_nDay, pNode->m_nTime, nDay, nTime) == 1) {
            m_lEventQueue.AddBefore(pNew, pos);
            bInserted = 1;
        } else {
            m_lEventQueue.GetNext(pos);
        }
    }
    if (!bInserted) {
        m_lEventQueue.AddTail(pNew);
    }
    return 1;
}
