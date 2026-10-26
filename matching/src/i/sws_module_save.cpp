// CSWSModule save path: IFO / ERF finish, player list lookup and the
// faction file (0x004C3960-0x004CA680).
#include "i/sws_module.h"

inline void* operator new(size_t, void* p) { return p; }

// FUNCTION: SWKOTOR 0x004C5C70
int CSWSModule::GetPlayerIndexInPlayerList(CSWSPlayer* pPlayer)
{
    int nIndex = 0;
    CExoLinkedListPosition pos = m_lPlayers.m_pList->m_pHead;
    while (pos) {
        CSWSModulePlayerEntry* pEntry = m_lPlayers.GetAtPos(pos);
        if (pEntry->m_sPlayerName == pPlayer->GetPlayerName()) {
            CSWSCreature* pCreature =
                g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pPlayer->m_oidNWSObject);
            if (pEntry->m_sFirstName == pCreature->GetCreatureStats()->m_lsFirstName &&
                pEntry->m_sLastName == pCreature->GetCreatureStats()->m_lsLastName) {
                return nIndex;
            }
        }
        m_lPlayers.m_pList->GetNext(pos);
        nIndex++;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x004C8B90
int CSWSModule::SaveModuleIFOFinish(CResGFF* pRes, CResStruct* pStruct, CERFFile* pERF,
                                    CExoString& sPrefix, CExoArrayList<unsigned long>& aPlayers)
{
    CExoString s1;
    CExoString s2;
    CExoString s3;
    SavePlayers(pRes, pStruct, sPrefix, aPlayers);
    pERF->AddResource("Module", 2014, pRes, 1, -1);
    delete pRes;
    delete pStruct;
    return 1;
}

// FUNCTION: SWKOTOR 0x004CA680
int CSWSModule::SaveModuleFinish(CExoString& sPrefix, CExoString& sPath)
{
    if (m_pSaveERF == NULL) {
        return 0;
    }
    if (!m_bIsNWM) {
        CExoString sType("ARE ");
        SaveStatic(m_pSaveERF, sType, 2012, 1);
    }
    SaveModuleIFOFinish(m_pSaveIFO, m_pSaveIFOStruct, m_pSaveERF, sPrefix, *m_pSavePlayerIDs);
    m_pSaveERF->Finish();
    delete m_pSaveERF;
    m_pSaveERF = NULL;
    delete m_pSavePlayerIDs;
    m_pSavePlayerIDs = NULL;
    return 1;
}

// FUNCTION: SWKOTOR 0x004C3960
int CSWSModule::SaveModuleFAC()
{
    CExoString sFile;
    CFactionManager* pFactions = GetServerExoApp()->GetFactionManager();
    CResGFF* pRes = new CResGFF();
    CResStruct* pTop = new CResStruct;
    int bCreated = pRes->CreateGFFFile(pTop, "FAC ", "V2.0");
    if (bCreated == 1) {
        CResList cList;
        pRes->AddList(&cList, pTop, "FactionList");
        pFactions->SaveFactions(pRes, &cList);
        pRes->AddList(&cList, pTop, "RepList");
        pFactions->SaveReputations(pRes, &cList);
        sFile = "GAMEINPROGRESS:REPUTE";
        {
            CExoString sFileName(sFile.CStr());
            pRes->WriteGFFFile(sFileName, 0x7f6);
        }
        if (pRes) {
            delete pRes;
        }
    } else {
        if (pRes) {
            delete pRes;
        }
    }
    delete pTop;
    return bCreated;
}

// FUNCTION: SWKOTOR 0x004C8960
void CSWSModule::SaveModuleStart(CExoString& sPrefix, CExoString& sPath)
{
    m_bIsSaveGame = 1;
    CExoString sFile = g_pExoBase->m_pcExoAliasList->ResolveFileName(sPath, 0xbc1);
    DeleteFileA(sFile.CStr());
    m_pSaveERF = new CERFFile;
    m_pSavePlayerIDs = new CExoArrayList<unsigned long>;
    m_pSaveERF->Create(sPath);
    m_pSaveERF->SetVersion("MOD V1.0");
    m_pSaveERF->WriteHeader();
    m_pSaveERF->WriteStringTable();
    m_nSaveEntries = 3;
    m_pSaveERF->SetNumEntries(3);
    m_pSaveIFO = new CResGFF();
    m_pSaveIFOStruct = new CResStruct;
    if (m_pSaveIFO->CreateGFFFile(m_pSaveIFOStruct, "IFO ", "V2.0")) {
        SaveModuleIFOStart(m_pSaveIFO, m_pSaveIFOStruct);
        SaveModuleFAC();
    } else {
        delete m_pSaveERF;
        m_pSaveERF = NULL;
        if (m_pSaveIFO) {
            delete m_pSaveIFO;
        }
        m_pSaveIFO = NULL;
        delete m_pSaveIFOStruct;
        m_pSaveIFOStruct = NULL;
    }
}

// FUNCTION: SWKOTOR 0x004C5980
int CSWSModule::SaveStatic(CERFFile* pERF, const CExoString& sType, unsigned short nResType,
                           int bGFF)
{
    int bCreated = 0;
    int bResult = 1;
    CResRef cResRef("Module");
    CRes* pModule = g_pExoResMan->GetResObject(cResRef, 2014);
    CExoStringList* pList = g_pExoResMan->GetResOfType(nResType, pModule);
    if (pList) {
        for (int i = 0; i < pList->GetCount(); i++) {
            CExoString* pName = pList->GetString(i);
            new (&cResRef) CResRef(pName->CStr());
            CRes* pRes = g_pExoResMan->GetResObject(cResRef, nResType);
            if (!pRes) {
                if (bGFF) {
                    CResGFF* pGFF = new CResGFF();
                    char* szType = sType.CStr();
                    strncpy(pGFF->m_pFileType, szType, 4);
                    pRes = pGFF;
                } else {
                    pRes = new CRes();
                }
                {
                    CResRef cKey(pName->CStr());
                    g_pExoResMan->SetResObject(cKey, nResType, pRes);
                }
                bCreated = 1;
            }
            if (pRes) {
                if (pRes->Demand()) {
                    pERF->AddResource(pName->CStr(), nResType, pRes, bGFF, -1);
                    pRes->Release();
                } else {
                    bResult = 0;
                }
                g_pExoResMan->ReleaseResObject(pRes);
            } else {
                bResult = 0;
            }
            if (bCreated) {
                bCreated = 0;
                if (pRes) {
                    delete pRes;
                }
            }
        }
    }
    g_pExoResMan->ReleaseResObject(pModule);
    if (pList) {
        delete pList;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004C3C70
void CSWSModule::SavePrimaryPlayerInfo(CSWSCreature* pCreature, const CExoString& sDirectory)
{
    CSWSPlayer* pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(pCreature->m_idSelf);
    CResGFF* pRes = new CResGFF();
    CResStruct* pTop = new CResStruct;
    if (pRes->CreateGFFFile(pTop, "BIC ", "V2.0")) {
        CExoString sPath;
        pRes->WriteFieldCExoString(pTop, pPlayer->GetPlayerName(), "Mod_CommntyName");
        pRes->WriteFieldBYTE(pTop, pPlayer->GetIsPrimaryPlayer(), "Mod_IsPrimaryPlr");
        pRes->WriteFieldDWORD(pTop, pCreature->m_idSelf, "ObjectId");
        pCreature->SaveCreature(pRes, pTop);
        sPath = sDirectory + "\\" + "Player";
        pRes->WriteGFFFile(sPath, 0x7df);
    }
    if (pRes) {
        delete pRes;
    }
}

// FUNCTION: SWKOTOR 0x004C7050
int CSWSModule::SaveModuleIFOStart(CResGFF* pRes, CResStruct* pStruct)
{
    CExoLocString sUnused;
    CResList cList;
    CResStruct cElement;
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    pRes->WriteFieldVOID(pStruct, m_pModuleID, 0x20, "Mod_ID");
    pRes->WriteFieldINT(pStruct, m_pModuleID->m_nCreatorID, "Mod_Creator_ID");
    pRes->WriteFieldDWORD(pStruct, m_pModuleID->m_nVersion, "Mod_Version");
    pRes->WriteFieldCExoLocString(pStruct, m_cName, "Mod_Name");
    pRes->WriteFieldCExoLocString(pStruct, m_cDescription, "Mod_Description");
    pRes->WriteFieldBYTE(pStruct, m_bIsSaveGame, "Mod_IsSaveGame");
    pRes->WriteFieldBYTE(pStruct, m_bIsNWM, "Mod_IsNWMFile");
    if (m_bIsNWM) {
        pRes->WriteFieldCExoString(pStruct, m_sNWMResName, "Mod_NWMResName");
    }
    {
        CExoString sHak(m_cResRef6c.GetResRefStr());
        pRes->WriteFieldCExoString(pStruct, sHak, "Mod_Hak");
    }
    pRes->WriteFieldDWORD64(pStruct, g_nNextEffectID, "Mod_Effect_NxtId");
    pRes->WriteFieldDWORD(pStruct, pArray->GetNextCharArrayID(0), "Mod_NextCharId0");
    pRes->WriteFieldDWORD(pStruct, pArray->GetNextCharArrayID(1), "Mod_NextCharId1");
    pRes->WriteFieldDWORD(pStruct, pArray->GetNextObjectArrayID(0), "Mod_NextObjId0");
    pRes->WriteFieldDWORD(pStruct, pArray->GetNextObjectArrayID(1), "Mod_NextObjId1");
    pRes->WriteFieldCExoString(pStruct, m_sLowerCaseName, "Mod_Tag");
    pRes->WriteFieldCResRef(pStruct, m_pEntryInfo->m_cArea, "Mod_Entry_Area");
    pRes->WriteFieldFLOAT(pStruct, m_pEntryInfo->m_fX, "Mod_Entry_X");
    pRes->WriteFieldFLOAT(pStruct, m_pEntryInfo->m_fY, "Mod_Entry_Y");
    pRes->WriteFieldFLOAT(pStruct, m_pEntryInfo->m_fZ, "Mod_Entry_Z");
    pRes->WriteFieldFLOAT(pStruct, m_pEntryInfo->m_fDirX, "Mod_Entry_Dir_X");
    pRes->WriteFieldFLOAT(pStruct, m_pEntryInfo->m_fDirY, "Mod_Entry_Dir_Y");
    pRes->WriteFieldBYTE(pStruct, m_n1a0, "Mod_MinPerHour");
    pRes->WriteFieldBYTE(pStruct, m_nDawnHour, "Mod_DawnHour");
    pRes->WriteFieldBYTE(pStruct, m_nDuskHour, "Mod_DuskHour");
    unsigned long nDay;
    unsigned long nTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    unsigned long nHour;
    unsigned long nMinute;
    unsigned long nSecond;
    unsigned long nMillisecond;
    GetServerExoApp()->GetWorldTimer()->FUN_004add90(nTime, &nHour, &nMinute, &nSecond,
                                                     &nMillisecond);
    unsigned long nPauseDay = GetServerExoApp()->GetWorldTimer()->m_nPauseDay;
    unsigned long nPauseTime = GetServerExoApp()->GetWorldTimer()->m_nPauseTime;
    pRes->WriteFieldDWORD(pStruct, m_nYear, "Mod_StartYear");
    pRes->WriteFieldBYTE(pStruct, m_nMonth, "Mod_StartMonth");
    pRes->WriteFieldBYTE(pStruct, m_nDay, "Mod_StartDay");
    pRes->WriteFieldBYTE(pStruct, m_nHour, "Mod_StartHour");
    pRes->WriteFieldDWORD(pStruct, m_nTransitionTime, "Mod_Transition");
    pRes->WriteFieldWORD(pStruct, nMinute, "Mod_StartMinute");
    pRes->WriteFieldWORD(pStruct, nSecond, "Mod_StartSecond");
    pRes->WriteFieldWORD(pStruct, nMillisecond, "Mod_StartMiliSec");
    pRes->WriteFieldDWORD(pStruct, nPauseDay, "Mod_PauseDay");
    pRes->WriteFieldDWORD(pStruct, nPauseTime, "Mod_PauseTime");
    pRes->WriteFieldBYTE(pStruct, m_nXPScale, "Mod_XPScale");
    SaveLimboCreatures(pRes, pStruct);
    pRes->AddList(&cList, pStruct, "Mod_Expan_List");
    CExoLinkedListPosition pos = m_lstExpansions.GetHeadPos();
    while (pos) {
        CSWSModuleExpansion* pExpansion = m_lstExpansions.GetAtPos(pos);
        pRes->AddListElement(&cElement, &cList, 0);
        pRes->WriteFieldCExoLocString(&cElement, pExpansion->m_sName, "Expansion_Name");
        pRes->WriteFieldINT(&cElement, pExpansion->m_nID, "Expansion_ID");
        m_lstExpansions.GetNext(pos);
    }
    pRes->AddList(&cList, pStruct, "Mod_CutSceneList");
    pos = m_lstCutScenes.GetHeadPos();
    while (pos) {
        CSWSModuleCutScene* pCutScene = m_lstCutScenes.GetAtPos(pos);
        pRes->AddListElement(&cElement, &cList, 1);
        pRes->WriteFieldCResRef(&cElement, pCutScene->m_cName, "CutScene_Name");
        pRes->WriteFieldDWORD(&cElement, pCutScene->m_nID, "CutScene_ID");
        m_lstCutScenes.GetNext(pos);
    }
    {
        CResRef cScript(m_sScripts[0]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnHeartbeat");
    }
    {
        CResRef cScript(m_sScripts[1]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnUsrDefined");
    }
    {
        CResRef cScript(m_sScripts[4]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnClientEntr");
    }
    {
        CResRef cScript(m_sScripts[5]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnClientLeav");
    }
    {
        CResRef cScript(m_sScripts[6]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnActvtItem");
    }
    {
        CResRef cScript(m_sScripts[7]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnAcquirItem");
    }
    {
        CResRef cScript(m_sScripts[8]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnUnAqreItem");
    }
    {
        CResRef cScript(m_sScripts[2]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnModLoad");
    }
    {
        CResRef cScript(m_sScripts[3]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnModStart");
    }
    {
        CResRef cScript(m_sScripts[9]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnPlrDeath");
    }
    {
        CResRef cScript(m_sScripts[10]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnPlrDying");
    }
    {
        CResRef cScript(m_sScripts[11]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnSpawnBtnDn");
    }
    {
        CResRef cScript(m_sScripts[12]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnPlrRest");
    }
    {
        CResRef cScript(m_sScripts[13]);
        pRes->WriteFieldCResRef(pStruct, cScript, "Mod_OnPlrLvlUp");
    }
    pRes->AddList(&cList, pStruct, "Mod_Area_list");
    CResRef cAreaRef = GetServerExoApp()->GetAreaByGameObjectID(m_oidArea)->m_cResRef;
    pRes->AddListElement(&cElement, &cList, 6);
    pRes->WriteFieldCResRef(&cElement, cAreaRef, "Area_Name");
    pRes->WriteFieldDWORD(&cElement, m_oidArea, "ObjectId");
    g_pAppManager->m_p14->m_nC = 1;
    pRes->AddList(&cList, pStruct, "Mod_Tokens");
    for (unsigned long i = 0; i < g_pTlkTable->m_nTokens; i++) {
        if (g_pTlkTable->m_pTokens[i].m_nNumber > (unsigned long)MIN_CUSTOM_TOKEN) {
            pRes->AddListElement(&cElement, &cList, 7);
            pRes->WriteFieldDWORD(&cElement, g_pTlkTable->m_pTokens[i].m_nNumber, "Mod_TokensNumber");
            pRes->WriteFieldCExoString(&cElement, g_pTlkTable->m_pTokens[i].m_sValue,
                                       "Mod_TokensValue");
        }
    }
    m_cScriptVarTable.SaveVarTable(pRes, pStruct);
    m_cVarTable.SaveVarTable(pRes, pStruct);
    GetServerExoApp()->GetServerAIMaster()->SaveEventQueue(pRes, pStruct);
    return 1;
}



static inline int FindIndex(CExoArrayList<unsigned long>& aList, unsigned long n)
{
    for (int i = 0; i < aList.num; i++) {
        if (aList[i] == n) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004C7870
int CSWSModule::SavePlayers(CResGFF* pRes, CResStruct* pStruct, const CExoString& sPrefix,
                            CExoArrayList<unsigned long>& aPlayers)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CExoArrayList<unsigned long> aFound;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CResList cList;
    CResStruct cElement;
    pRes->AddList(&cList, pStruct, "Mod_PlayerList");
    CGameObject* pObject;
    for (int i = 0; i < aPlayers.num; i++) {
        if (pArray->GetGameObject(aPlayers[i], &pObject) != GAME_OBJECT_VALID) {
            continue;
        }
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        CSWSPlayer* pPlayer = pServer->GetClientObjectByObjectId(pCreature->m_idSelf);
        if (!pPlayer) {
            pPlayer = pServer->GetClientObjectByObjectId(pServer->ClientToServerObjectId(
                GetClientExoApp()->GetSWParty()->GetCharacter(0)->m_idSelf));
            if (!pPlayer) {
                pCreature->DumpToLog();
                continue;
            }
        }
        pPlayer->SaveServerCharacter();
        int nIndex = GetPlayerIndexInPlayerList(pPlayer);
        if (nIndex != -1) {
            aFound.AddInline(nIndex);
        }
        pRes->AddListElement(&cElement, &cList, 0xbead);
        pRes->WriteFieldCExoString(&cElement, pPlayer->GetPlayerName(), "Mod_CommntyName");
        pRes->WriteFieldBYTE(&cElement, pPlayer->GetIsPrimaryPlayer(), "Mod_IsPrimaryPlr");
        pRes->WriteFieldCExoLocString(&cElement, pCreature->GetCreatureStats()->m_lsFirstName,
                                      "Mod_FirstName");
        pRes->WriteFieldCExoLocString(&cElement, pCreature->GetCreatureStats()->m_lsLastName,
                                      "Mod_LastName");
        pRes->WriteFieldDWORD(&cElement, pCreature->m_idSelf, "ObjectId");
        pCreature->SaveCreature(pRes, &cElement);
    }
    CResGFF* pIFO;
    {
        CResRef cModule("Module");
        pIFO = (CResGFF*)g_pExoResMan->GetResObject(cModule, 2014);
    }
    CExoString sName;
    if (!pIFO) {
        return 0;
    }
    pIFO->Demand();
    CResStruct cTop;
    pIFO->GetTopLevelStruct(&cTop);
    CResList cIFOList;
    if (!pIFO->GetList(&cIFOList, &cTop, "Mod_PlayerList")) {
        pIFO->Release();
        g_pExoResMan->ReleaseResObject(pIFO);
        return 0;
    }
    for (unsigned long j = 0; j < m_lPlayers.m_pList->m_nCount; j++) {
        if (FindIndex(aFound, j)) {
            continue;
        }
        CResStruct cEntry;
        if (!pIFO->GetListElement(&cEntry, &cIFOList, j)) {
            g_pExoResMan->ReleaseResObject(pIFO);
            return 0;
        }
        int bSuccess;
        unsigned long oid = pIFO->ReadFieldDWORD(&cEntry, "ObjectId", bSuccess, 0x7f000000);
        sName = pIFO->ReadFieldCExoString(&cEntry, "Mod_CommntyName", bSuccess, CExoString(-1));
        int nMatches = 0;
        for (int k = 0; k < aPlayers.num; k++) {
            if (aPlayers[k] == oid) {
                nMatches++;
            }
        }
        if (nMatches || oid == 0x7f000000) {
            continue;
        }
        CSWSPlayer* pPlayer = new CSWSPlayer(oid);
        pPlayer->LoadCharacterFromIFO(j, 1, 0);
        CSWSCreature* pCreature = (CSWSCreature*)pPlayer->GetGameObject();
        pRes->AddListElement(&cElement, &cList, 0xbead);
        pRes->WriteFieldCExoString(&cElement, sName, "Mod_CommntyName");
        pRes->WriteFieldBYTE(&cElement, pPlayer->GetIsPrimaryPlayer(), "Mod_IsPrimaryPlr");
        pRes->WriteFieldCExoLocString(&cElement, pCreature->GetCreatureStats()->m_lsFirstName,
                                      "Mod_FirstName");
        pRes->WriteFieldCExoLocString(&cElement, pCreature->GetCreatureStats()->m_lsLastName,
                                      "Mod_LastName");
        pRes->WriteFieldDWORD(&cElement, pCreature->m_idSelf, "ObjectId");
        pCreature->SaveCreature(pRes, &cElement);
        pCreature->RemoveFromArea(0);
        delete pPlayer;
    }
    pIFO->Release();
    g_pExoResMan->ReleaseResObject(pIFO);
    return 1;
}
