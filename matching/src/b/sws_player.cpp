// CSWSPlayer (0x00560D00-0x005631E0).
#include "b/sws_player.h"
#include "b/server_exo_app.h"
#include "b/sws_creature.h"
#include "b/party_table.h"
#include <string.h>

class CNetLayer {
public:
    void* GetPlayerInfo(unsigned long nPlayerId);               // 0x005D5170
};

struct CNetLayerPlayerInfo {
    char pad_0x0[0x4];
    CExoString m_sPlayerName;                                   // 0x4
};

class CSWSMessage {
public:
    void CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId, int bArg); // 0x004D68A0
    void WriteINT(int nValue, int nBits);                       // 0x004D6340
    void WriteVOIDPtr(void* pData, unsigned long nSize);        // 0x004D63E0
    void WriteCExoLocStringServer(CExoLocString* pString, int nArg); // 0x005785C0
};

class CExoFile {
public:
    CExoFile(const CExoString& sFileName, unsigned short nType, const CExoString& sMode); // 0x005E68D0
    ~CExoFile();                                                // 0x005E69E0
    int IsOpened();                                             // 0x005E6A10
    unsigned long GetSize();                                    // 0x005E6950
    unsigned long Read(void* pBuffer, unsigned long nSize, unsigned long nCount); // 0x005E6960

    char pad_0x0[0x4];
};

extern const int EFFECT_TYPE_DISGUISE;                          // 0x00746774 (0x3E)
extern const unsigned char OBJECT_TYPE_LAST_NON_GAME;           // 0x00746633 (4)

// FUNCTION: SWKOTOR 0x00561080
CGameObject* CSWSPlayer::GetGameObject()
{
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidNWSObject);
    if (!pObject || pObject->m_nObjectType <= OBJECT_TYPE_LAST_NON_GAME) {
        return NULL;
    }
    return pObject;
}

// FUNCTION: SWKOTOR 0x005610B0
CExoString CSWSPlayer::GetPlayerName()
{
    CNetLayerPlayerInfo* pInfo = (CNetLayerPlayerInfo*)g_pAppManager->GetServerExoApp()->GetNetLayer()->GetPlayerInfo(m_nPlayerID);
    if (pInfo) {
        return pInfo->m_sPlayerName;
    }
    return "";
}

// FUNCTION: SWKOTOR 0x00561100
CSWSPlayerLastUpdateObject* CSWSPlayer::CreateNewPlayerLastUpdateObject()
{
    if (m_pLastUpdatePlayer) {
        delete m_pLastUpdatePlayer;
        m_pLastUpdatePlayer = NULL;
    }
    m_pLastUpdatePlayer = new CSWSPlayerLastUpdateObject();
    m_pLastUpdatePlayer->ClearKnownSpellUsesLeft();
    return m_pLastUpdatePlayer;
}

// FUNCTION: SWKOTOR 0x00561190
int CSWSPlayer::PermittedToDisplayCharacterSheet(unsigned long nId)
{
    if (!g_pAppManager->GetServerExoApp()) {
        return 0;
    }
    CSWSCreature* pOther = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(nId);
    if (!pOther) {
        return 0;
    }
    CSWSCreature* pMine = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidNWSObject);
    if (!pMine) {
        return 0;
    }
    return pMine->m_idSelf == pOther->m_idSelf;
}

// FUNCTION: SWKOTOR 0x005611F0 SYMBOL: ??0?$CExoLinkedList@VCLastUpdateObject@@@@QAE@XZ
template class CExoLinkedList<CLastUpdateObject>;

// FUNCTION: SWKOTOR 0x00561820
CLastUpdateObject* CSWSPlayer::GetLastUpdateObject(unsigned long nId)
{
    void* pPos = m_pLastUpdateObjects->GetHeadPos();
    while (pPos) {
        CLastUpdateObject* pObject = m_pLastUpdateObjects->GetAtPos(pPos);
        if (pObject->m_nId == nId) {
            return pObject;
        }
        m_pLastUpdateObjects->GetNext(pPos);
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x00561C40
void CSWSPlayer::SetGameObject(CGameObject* pObject)
{
    unsigned long oidOld = m_oidNWSObject;
    CSWSArea* pOldArea = NULL;
    CSWSArea* pNewArea = NULL;
    if (g_pAppManager->GetServerExoApp()) {
        CGameObject* pOld = g_pAppManager->GetServerExoApp()->GetGameObject(oidOld);
        if (pOld && pOld->m_nObjectType > OBJECT_TYPE_LAST_NON_GAME) {
            pOldArea = ((CSWSObject*)pOld)->GetArea();
        }
    }
    if (pObject) {
        unsigned long oidNew = pObject->m_idSelf;
        m_oidNWSObject = oidNew;
        if (g_pAppManager->GetServerExoApp()) {
            CGameObject* pNew = g_pAppManager->GetServerExoApp()->GetGameObject(oidNew);
            if (pNew && pNew->m_nObjectType > OBJECT_TYPE_LAST_NON_GAME) {
                pNewArea = ((CSWSObject*)pNew)->GetArea();
            }
        }
        if (oidOld != oidNew) {
            m_pClientInfo->m_bDirty = 1;
            if (m_pLastUpdatePlayer) {
                m_pLastUpdatePlayer->ClearActionQueue();
                m_pLastUpdatePlayer->ClearKnownSpells();
                m_pLastUpdatePlayer->ClearKnownSpellUsesLeft();
                m_pLastUpdatePlayer->ClearSpellAddDeleteLists();
                m_pLastUpdatePlayer->ClearFeats();
            }
        }
        if (g_pAppManager->GetServerExoApp() && g_pAppManager->GetServerExoApp()->GetPartyTable()
            && g_pAppManager->GetServerExoApp()->GetPartyTable()->GetPartyItems()) {
            g_pAppManager->GetServerExoApp()->GetPartyTable()->GetPartyItems()->SetParentId(m_oidNWSObject);
        }
    }
    else {
        m_oidNWSObject = INVALID_OBJECT_ID;
    }
    if (pOldArea != pNewArea) {
        if (pNewArea) {
            pNewArea->IncrementPlayersInArea();
        }
        if (pOldArea) {
            pOldArea->DecrementPlayersInArea();
        }
    }
}

// FUNCTION: SWKOTOR 0x00562C70
CSWSPlayer::CSWSPlayer(unsigned long nPlayerId)
    : CSWSClient(nPlayerId)
{
    m_n20 = 0;
    m_n24 = 0;
    m_oidNWSObject = OBJECT_INVALID_ID;
    m_n2C = 0;
    m_n30 = 0;
    m_oid34 = OBJECT_INVALID_ID;
    m_pLastUpdateObjects = new CExoLinkedList<CLastUpdateObject>;
    m_pList10 = new CExoLinkedList<CLastUpdateObject>;
    m_pInventoryGUI = new CSWSPlayerInventoryGUI;
    m_n14 = -1;
    m_s18 = "";
    m_pClientInfo = new CSWSPlayerClientInfo;
    m_pStoreGUI = new CSWSPlayerStoreGUI;
    m_pObjectRef = new CSWSPlayerObjectRef;
    m_n3C = 1;
    m_n40 = 0;
    m_n44 = -1;
    m_n60 = 1;
    m_n64 = 0;
    m_cResRef = "";
    m_n7C = 0;
    m_oid38 = OBJECT_INVALID_ID;
    m_pLastUpdatePlayer = NULL;
    m_n48 = 0;
    m_n80 = 0;
}

// NONMATCH: the drain loop's head test uses eax/ecx/edx rotated by one
// against the original (same instructions otherwise).
// FUNCTION: SWKOTOR 0x00562E40
void CSWSPlayer::ClearPlayerOnDestroyGame()
{
    if (m_pStoreGUI->m_pStore) {
        m_pStoreGUI->m_pStore->Clear();
    }
    if (m_pList10) {
        delete m_pList10;
        m_pList10 = new CExoLinkedList<CLastUpdateObject>;
    }
    if (m_pLastUpdateObjects) {
        CLastUpdateObject* pObject;
        while (m_pLastUpdateObjects->GetHeadPos()) {
            pObject = m_pLastUpdateObjects->RemoveHead();
            delete pObject;
        }
    }
    if (m_pInventoryGUI) {
        delete m_pInventoryGUI;
        m_pInventoryGUI = new CSWSPlayerInventoryGUI;
    }
    if (m_pObjectRef) {
        delete m_pObjectRef;
        m_pObjectRef = new CSWSPlayerObjectRef;
    }
    if (m_pClientInfo) {
        delete m_pClientInfo;
        m_pClientInfo = new CSWSPlayerClientInfo;
    }
    if (m_pStoreGUI) {
        delete m_pStoreGUI;
        m_pStoreGUI = new CSWSPlayerStoreGUI;
    }
}

// FUNCTION: SWKOTOR 0x00563040
CSWSPlayer::~CSWSPlayer()
{
    delete m_pList10;
    m_pList10 = NULL;
    if (m_pLastUpdateObjects) {
        while (m_pLastUpdateObjects->GetHeadPos()) {
            CLastUpdateObject* pObject = m_pLastUpdateObjects->RemoveHead();
            delete pObject;
        }
        delete m_pLastUpdateObjects;
        m_pLastUpdateObjects = NULL;
    }
    delete m_pInventoryGUI;
    m_pInventoryGUI = NULL;
    delete m_pClientInfo;
    m_pClientInfo = NULL;
    delete m_pStoreGUI;
    m_pStoreGUI = NULL;
    delete m_pObjectRef;
    m_pObjectRef = NULL;
    delete m_pLastUpdatePlayer;
    m_pLastUpdatePlayer = NULL;
}

// FUNCTION: SWKOTOR 0x005631E0 SYMBOL: ??_GCSWSPlayer@@UAEPAXI@Z

// NONMATCH: the GetList/GetListElement failure block is laid out at the end
// (original: inline, falling into the shared `return 0`); args rotate eax/ecx/edx.
// FUNCTION: SWKOTOR 0x00560D00
int CSWSPlayer::GetCharacterInfoFromIFO(unsigned char* pClassCount, int* pClasses,
                                        unsigned char* pLevels, unsigned long* pExperience)
{
    CResRef cResRef("Module");
    CResGFF* pRes = (CResGFF*)g_pExoResMan->GetResObject(cResRef, 0x7de);
    if (!pRes) {
        return 0;
    }
    pRes->Demand();
    CResStruct cTop;
    CResList cPlayerList;
    CResStruct cPlayer;
    pRes->GetTopLevelStruct(&cTop);
    if (!pRes->GetList(&cPlayerList, &cTop, "Mod_PlayerList")
        || !pRes->GetListElement(&cPlayer, &cPlayerList, m_n44)) {
        g_pExoResMan->ReleaseResObject(pRes);
        return 0;
    }
    int bSuccess;
    CResList cClassList;
    CResStruct cClass;
    *pExperience = pRes->ReadFieldDWORD(&cPlayer, "Experience", bSuccess, 0);
    if (pRes->GetList(&cClassList, &cPlayer, "ClassList") == 1) {
        *pClassCount = (unsigned char)pRes->GetListCount(&cClassList);
        for (unsigned long i = 0; i < *pClassCount; i++) {
            pRes->GetListElement(&cClass, &cClassList, i);
            pClasses[i] = pRes->ReadFieldINT(&cClass, "Class", bSuccess, -1);
            pLevels[i] = (unsigned char)pRes->ReadFieldSHORT(&cClass, "ClassLevel", bSuccess, 0);
        }
    }
    pRes->Release();
    g_pExoResMan->ReleaseResObject(pRes);
    return 1;
}

// FUNCTION: SWKOTOR 0x00560E60
int CSWSPlayer::LoadCreatureData(CResRef cResRef, CSWSCreature* pCreature)
{
    CResGFF* pRes = new CResGFF(0x7df, "BIC ", cResRef);
    if (!pRes->m_bLoaded) {
        pRes->Dump(0);
        delete pRes;
        return 0;
    }
    m_cResRef = cResRef;
    CResStruct cTop;
    int bSuccess;
    pRes->GetTopLevelStruct(&cTop);
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    int nResult = pStats->ReadStatsFromGff(pRes, &cTop, &pCreature->m_cAppearance);
    if (nResult) {
        pRes->Dump(0);
        delete pRes;
        return nResult;
    }
    pCreature->SetSize(pCreature->GetCreatureSize(1));
    pRes->ReadFieldBYTE(&cTop, "DetectMode", bSuccess, 0);
    pCreature->SetActivity(2, 1, 0);
    int bStealth = pRes->ReadFieldBYTE(&cTop, "StealthMode", bSuccess, 0);
    pCreature->SetActivity(1, bStealth, 0);
    pCreature->m_nCreatureSize = pRes->ReadFieldINT(&cTop, "CreatureSize", bSuccess, 3);
    pCreature->m_bDisguised = pRes->ReadFieldBYTE(&cTop, "PM_IsDisguised", bSuccess, 0);
    if (pCreature->m_bDisguised) {
        pCreature->m_nDisguiseAppearance = pRes->ReadFieldWORD(&cTop, "PM_Appearance", bSuccess, 0);
    }
    pCreature->ReadScriptsFromGff(pRes, &cTop);
    pCreature->LoadFromGff(pRes, &cTop, 0, 1, 0);
    pCreature->m_pStats->ReadSpellsFromGff(pRes, &cTop);
    pCreature->PostLoad();
    pRes->Dump(0);
    delete pRes;
    return 0;
}

// FUNCTION: SWKOTOR 0x00561D70
int CSWSPlayer::LoadLocalCharacter()
{
    CSWSCreature* pCreature = new CSWSCreature(INVALID_OBJECT_ID, 1);
    SetGameObject(pCreature);
    m_oid38 = pCreature->m_idSelf;
    m_n64 = 1;
    int nResult = LoadCreatureData("TEMP_CHAR", pCreature);
    pCreature->SetInParty(1, 1);
    if (nResult) {
        SetGameObject(NULL);
        m_oid38 = INVALID_OBJECT_ID;
        delete pCreature;
    }
    return nResult;
}

// NONMATCH: resman/temp-string load order swapped between the two resource
// directory calls, and the CResRef argument copy rotates eax/ecx/edx.
// FUNCTION: SWKOTOR 0x00562380
int CSWSPlayer::LoadServerCharacter(CResRef cResRef)
{
    CSWSCreature* pCreature = new CSWSCreature(INVALID_OBJECT_ID, 1);
    SetGameObject(pCreature);
    m_oid38 = pCreature->GetId();
    m_n64 = 3;
    g_pExoResMan->RemoveResourceDirectory("SERVERVAULT:");
    g_pExoResMan->AddResourceDirectory("SERVERVAULT:");
    int nResult = LoadCreatureData(cResRef, pCreature);
    if (nResult) {
        SetGameObject(NULL);
        m_oid38 = INVALID_OBJECT_ID;
        delete pCreature;
    }
    return nResult;
}

// NONMATCH: the six zero stores to the position/orientation vectors are
// eliminated here (kept in the original), shifting the frame by 4 bytes.
// FUNCTION: SWKOTOR 0x00561E30
int CSWSPlayer::LoadCharacterFromIFO(int nIndex, int bArg, int bFinish)
{
    Vector vPosition;
    Vector vOrientation;
    vPosition.x = 0.0f;
    vPosition.y = 0.0f;
    vPosition.z = 0.0f;
    vOrientation.x = 0.0f;
    vOrientation.y = 0.0f;
    vOrientation.z = 0.0f;
    int bOwnsRes = 0;
    int bVault = 0;
    CResGFF* pRes;
    if (nIndex == -1) {
        g_pExoResMan->AddResourceDirectory("TEMP:");
        nIndex = m_n80;
        bVault = 1;
        CResRef cResRef("pifo");
        pRes = (CResGFF*)g_pExoResMan->GetResObject(cResRef, 0x7de);
        if (!pRes) {
            pRes = new CResGFF();
            g_pExoResMan->SetResObject("pifo", 0x7de, pRes);
            strncpy(pRes->m_pFileType, "IFO ", 4);
            bOwnsRes = 1;
        }
    }
    else {
        CResRef cResRef("Module");
        pRes = (CResGFF*)g_pExoResMan->GetResObject(cResRef, 0x7de);
    }
    if (!pRes) {
        return 0;
    }
    pRes->Demand();
    CResStruct cTop;
    CResList cPlayerList;
    CResStruct cPlayer;
    pRes->GetTopLevelStruct(&cTop);
    if (!pRes->GetList(&cPlayerList, &cTop, "Mod_PlayerList")
        || !pRes->GetListElement(&cPlayer, &cPlayerList, nIndex)) {
        pRes->Release();
        g_pExoResMan->ReleaseResObject(pRes);
        return 0;
    }
    int bSuccess;
    unsigned long oidCreature = pRes->ReadFieldDWORD(&cPlayer, "ObjectId", bSuccess, OBJECT_INVALID_ID);
    CSWSCreature* pCreature = new CSWSCreature(oidCreature, 1);
    SetGameObject(pCreature);
    m_oid38 = pCreature->m_idSelf;
    unsigned long oidArea = pRes->ReadFieldDWORD(&cPlayer, "AreaId", bSuccess, 0);
    pCreature->SetInParty(1, 1);
    if (pCreature->GetStats()->ReadStatsFromGff(pRes, &cPlayer, &pCreature->m_cAppearance)) {
        g_pExoResMan->ReleaseResObject(pRes);
        delete pCreature;
        return 0;
    }
    pCreature->LoadCreatureState(pRes, &cPlayer);
    pRes->ReadFieldBYTE(&cPlayer, "DetectMode", bSuccess, 0);
    pCreature->SetActivity(2, 1, 0);
    int bStealth = pRes->ReadFieldBYTE(&cPlayer, "StealthMode", bSuccess, 0);
    pCreature->SetActivity(1, bStealth, 0);
    pCreature->m_nCreatureSize = pRes->ReadFieldINT(&cPlayer, "CreatureSize", bSuccess, 3);
    pCreature->LoadFromGff(pRes, &cPlayer, !bVault, 1, 0);
    pCreature->LoadObjectState(pRes, &cPlayer);
    pCreature->GetStats()->ReadSpellsFromGff(pRes, &cPlayer);
    pCreature->m_bDisguised = pRes->ReadFieldBYTE(&cPlayer, "PM_IsDisguised", bSuccess, 0);
    if (pCreature->m_bDisguised) {
        pCreature->m_nDisguiseAppearance = pRes->ReadFieldWORD(&cPlayer, "PM_Appearance", bSuccess, 0);
    }
    pCreature->ReadScriptsFromGff(pRes, &cPlayer);
    vPosition.x = pRes->ReadFieldFLOAT(&cPlayer, "XPosition", bSuccess, 0.0f);
    vPosition.y = pRes->ReadFieldFLOAT(&cPlayer, "YPosition", bSuccess, 0.0f);
    vPosition.z = pRes->ReadFieldFLOAT(&cPlayer, "ZPosition", bSuccess, 0.0f);
    vOrientation.x = pRes->ReadFieldFLOAT(&cPlayer, "XOrientation", bSuccess, 0.0f);
    vOrientation.y = pRes->ReadFieldFLOAT(&cPlayer, "YOrientation", bSuccess, 0.0f);
    vOrientation.z = pRes->ReadFieldFLOAT(&cPlayer, "ZOrientation", bSuccess, 0.0f);
    pCreature->SetOrientation(vOrientation);
    m_n40 = !bVault && bArg;
    if (bFinish) {
        pCreature->PostLoad();
    }
    pRes->Release();
    g_pExoResMan->ReleaseResObject(pRes);
    if (bOwnsRes == 1 && pRes) {
        delete pRes;
    }
    if (bVault) {
        g_pExoResMan->RemoveResourceDirectory("TEMP:");
    }
    else {
        pCreature->m_vLastPosition.x = vPosition.x;
        pCreature->m_oidLastArea = oidArea;
        pCreature->m_vLastPosition.y = vPosition.y;
        pCreature->m_vLastPosition.z = vPosition.z;
        pCreature->m_n320 = 0;
    }
    return 1;
}

// NONMATCH: the 64-bit effect id is loaded high dword first (original: low
// first), which rotates the registers of everything after it.
// FUNCTION: SWKOTOR 0x00561870
int CSWSPlayer::PackCreatureIntoMessage()
{
    int bResult = 1;
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CResGFF* pRes = new CResGFF();
    CResStruct* pTop = new CResStruct;
    if (pRes->CreateGFFFile(pTop, "BIC ", "V2.0")) {
        CExoString sFileName;
        CExoString sUnused;
        CSWSCreature* pCreature = GetGameObject()->AsSWSCreature();
        if (pCreature) {
            if (pCreature->m_bDisguised == 1) {
                for (int i = 0; i < pCreature->m_nAppliedEffects; i++) {
                    if (pCreature->m_pAppliedEffects[i]->m_nType == EFFECT_TYPE_DISGUISE) {
                        pCreature->m_pAppliedEffects[i]->SetInteger(2, 1);
                        pCreature->RemoveEffectById(pCreature->m_pAppliedEffects[i]->m_nID);
                        break;
                    }
                }
                pCreature->m_pStats->UpdateCombatInformation();
            }
            pMessage->CreateWriteMessage(0x1000, 0xffffffff, 1);
            pCreature->SaveCreature(pRes, pTop);
            sFileName = "TEMP:temp_charfile";
            pRes->WriteGFFFile(sFileName, 0x7df);
            g_pExoResMan->AddResourceDirectory("TEMP:");
            CExoFile* pFile = new CExoFile(sFileName, 0x7df, "rb");
            if (pFile->IsOpened()) {
                unsigned long nSize = pFile->GetSize();
                unsigned char* pData = new unsigned char[nSize];
                pFile->Read(pData, 1, nSize);
                delete pFile;
                pMessage->WriteCExoLocStringServer(pCreature->GetFirstName(), 0);
                pMessage->WriteCExoLocStringServer(pCreature->GetLastName(), 0);
                pMessage->WriteINT(nSize, 0x20);
                pMessage->WriteVOIDPtr(pData, nSize);
                g_pExoResMan->RemoveResourceDirectory("TEMP:");
                g_pExoResMan->RemoveFile(sFileName, 0x7df);
                delete[] pData;
            }
            else {
                bResult = 0;
                delete pFile;
            }
        }
        else {
            bResult = 0;
        }
    }
    else {
        bResult = 0;
    }
    delete pTop;
    if (pRes) {
        delete pRes;
    }
    return bResult;
}
