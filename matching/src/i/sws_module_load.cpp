// CSWSModule load steps (0x004C5720-0x004C8C70).
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C5880
int CSWSModule::LoadModuleFinish()
{
    CResStruct cTop;
    m_pRes->GetTopLevelStruct(&cTop);
    CAppManager14* pLoad = g_pAppManager->m_p14;
    pLoad->m_n8 = pLoad->m_nC;
    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerLoadBar_UpdateStallEvent(pLoad->m_nC, -1);
    CScriptEvent* pEvent = new CScriptEvent;
    pEvent->m_nType = 0x11;
    GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, m_idSelf, m_idSelf,
                                                              EVENT_SIGNAL_EVENT, pEvent);
    m_pRes->Release();
    m_bLoaded = 1;
    if (g_nModuleFrameLock) {
        SetFramesPerSecond("30");
        g_bConsoleOverlay = 0;
        g_fFrameLock = (float)g_nModuleFrameLock;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004C5720
int CSWSModule::LoadModuleInProgress(int nAreaIndex, int nArg)
{
    unsigned long oid = m_bIsSaveGame ? m_oidArea : nAreaIndex;
    CSWSArea* pArea;
    if (m_bIsSaveGame) {
        pArea = new CSWSArea(m_cAreaResRef, 0, oid);
    } else {
        pArea = new CSWSArea(m_cAreaResRef, 0, 0x7f000000);
    }
    if (!pArea) {
        m_pRes->Release();
        RemoveModuleResources(m_sModuleName);
        return 3;
    }
    if (!pArea->LoadArea(m_bIsSaveGame)) {
        m_pRes->Release();
        RemoveModuleResources(m_sModuleName);
        delete pArea;
        return 4;
    }
    if (!m_bIsSaveGame) {
        m_oidArea = pArea->m_oid120;
    }
    g_pAppManager->m_p14->m_n8 = nAreaIndex + 1;
    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerLoadBar_UpdateStallEvent(nAreaIndex + 1, -1);
    return 0;
}

// FUNCTION: SWKOTOR 0x004C8E90
void CSWSModule::PackPlayerCharacterListIntoMessage(CSWSPlayer* pPlayer,
                                                    CExoArrayList<NWPlayerCharacterList_st*>& aList)
{
    int nCount = 0;
    CResRef cResRef;
    CExoString sNumber;
    CExoLinkedListPosition pos = m_lPlayers.m_pList->m_pHead;
    while (pos) {
        CSWSModulePlayerEntry* pEntry = (CSWSModulePlayerEntry*)m_lPlayers.m_pList->GetAtPos(pos);
        NWPlayerCharacterList_st* p = new NWPlayerCharacterList_st;
        p->m_sFirstName = pEntry->m_sFirstName;
        p->m_sLastName = pEntry->m_sLastName;
        p->m_nType = 0xe;
        sNumber.Format("%i", (unsigned short)nCount);
        cResRef = sNumber;
        p->m_cResRef = cResRef;
        if (!aList.contains(p)) {
            aList.Add(p);
        } else {
            delete p;
        }
        m_lPlayers.m_pList->GetNext(pos);
        nCount++;
    }
}

// FUNCTION: SWKOTOR 0x004C8C70
int CSWSModule::LoadLimboCreatures(CResGFF* pRes, CResStruct* pStruct, int bLoadFromSave)
{
    CResList cList;
    CResStruct cStruct;
    int bSuccess;
    if (pRes->GetList(&cList, pStruct, "Creature List")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            if (pRes->GetListElement(&cStruct, &cList, i) && pRes->GetStructID(&cStruct) == 4) {
                unsigned long oid = pRes->ReadFieldDWORD(&cStruct, "ObjectId", bSuccess, 0x7f000000);
                CSWSCreature* pCreature = new CSWSCreature(oid, 0);
                if (pCreature) {
                    if (!pCreature->LoadCreature(pRes, &cStruct, bLoadFromSave)) {
                        delete pCreature;
                    } else {
                        if (bLoadFromSave) {
                            pCreature->m_bLoadedFromSave = 1;
                        }
                        pRes->ReadFieldFLOAT(&cStruct, "XPosition", bSuccess, 0.0f);
                        pRes->ReadFieldFLOAT(&cStruct, "YPosition", bSuccess, 0.0f);
                        pRes->ReadFieldFLOAT(&cStruct, "ZPosition", bSuccess, 0.0f);
                        pCreature->SetOrientation(
                            Vector(pRes->ReadFieldFLOAT(&cStruct, "XOrientation", bSuccess, 0.0f),
                                   pRes->ReadFieldFLOAT(&cStruct, "YOrientation", bSuccess, 0.0f),
                                   pRes->ReadFieldFLOAT(&cStruct, "ZOrientation", bSuccess, 0.0f)));
                        AddObjectToLimbo(pCreature->m_idSelf);
                    }
                }
            }
        }
    }
    return 1;
}
