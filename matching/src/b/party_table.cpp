// CSWPartyTable (0x00563200-0x005667C0).
#include "b/party_table.h"
#include "b/server_exo_app.h"
#include "b/sws_creature.h"
#include "b/sw_rules.h"
#include "b/sws_player.h"
#include <math.h>

struct Quaternion {
    Quaternion(Vector vAxis, float fAngle);                     // 0x004AC960

    float x;
    float y;
    float z;
    float w;
};

float __cdecl GetYaw(const Vector& vOrientation);               // 0x004AA0F0
float __cdecl GetFormationAngle(float fAngle, const Vector& vDirection); // 0x004A96D0
Vector __cdecl GetDirection(const Quaternion& q);               // 0x004A9BB0

inline Vector operator+(const Vector& a, const Vector& b)
{
    Vector v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    return v;
}

struct CSWPartyFormationEntry {
    char pad_0x0[0xc];
    float m_fAngle;                                             // 0xc
};

class CSWPartyFormation {
public:
    CSWPartyFormationEntry* GetEntry(int nIndex);               // 0x00634CC0
};

struct CSWPartySlot {
    Vector m_vOffset;                                           // 0x00
    int m_nFormationEntry;                                      // 0x0c
    Vector m_vDirection;                                        // 0x10
    char pad_0x1c[0x34 - 0x1c];
    Vector m_vPosition;                                         // 0x34
    char pad_0x40[0x88 - 0x40];
};

class CSWParty {
public:
    void ResetPartyGui();                                       // 0x00634A40
    void FollowStateChanged();                                  // 0x006369D0
    void SetLeaderPosition(const Vector& vPosition, float fYaw, int bArg); // 0x00637890

    char pad_0x0[0xd4];
    CSWPartySlot m_aSlots[4];                                   // 0xd4
};

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);                   // 0x0062EEB0
};

class CSWSEvent {
public:
    CSWSEvent();                                                // 0x004D69F0
    void SetInteger(int nIndex, int nValue);                    // 0x004D76B0

    char pad_0x0[0x34];
};

extern const unsigned long OBJECT_ALL_POSSESSOR;                 // 0x0074664C (0x7FFFFFFF)

extern const unsigned short EFFECT_TYPE_RESURRECTION;            // 0x0074668C (4)
extern const unsigned short DURATION_TYPE_INSTANT;              // 0x007473B8 (0)

struct FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
};
extern "C" __declspec(dllimport) void __stdcall GetSystemTimeAsFileTime(FILETIME* pTime);

extern unsigned __int64 g_nSessionStartTime;                    // 0x007A3A20

// FUNCTION: SWKOTOR 0x00563600
void CSWPartyTable::ClearInventory()
{
    if (m_pItems) {
        m_pItems->ClearItems();
    }
    m_bPartyCreated = 0;
}

// FUNCTION: SWKOTOR 0x00563D20
CSWPartyTable::CSWPartyTable()
{
    m_pItems = NULL;
    m_pJournal = NULL;
    ClearTable();
}

// FUNCTION: SWKOTOR 0x00563D40
CSWPartyTable::~CSWPartyTable()
{
    if (m_pItems) {
        delete m_pItems;
        m_pItems = NULL;
    }
    if (m_pJournal) {
        delete m_pJournal;
        m_pJournal = NULL;
    }
}

// FUNCTION: SWKOTOR 0x00563340
CItemRepository* CSWPartyTable::GetPartyItems()
{
    if (!m_pItems) {
        m_pItems = new CItemRepository(g_pAppManager->GetServerExoApp()->GetPlayerCreatureId(), 1);
    }
    return m_pItems;
}

// FUNCTION: SWKOTOR 0x005633C0
CSWSJournal* CSWPartyTable::GetPartyJournal()
{
    if (!m_pJournal) {
        m_pJournal = new CSWSJournal();
    }
    return m_pJournal;
}

// FUNCTION: SWKOTOR 0x00563A80
void CSWPartyTable::SetPartyFollowState(int nState)
{
    m_nFollowState = 0;
    g_pAppManager->GetClientExoApp()->GetSWParty()->FollowStateChanged();
}

// FUNCTION: SWKOTOR 0x00563CF0
void CSWPartyTable::ResetCurrentSessionStartTime()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    g_nSessionStartTime = ((unsigned __int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
}

// FUNCTION: SWKOTOR 0x00565500
void CSWPartyTable::SetSoloMode(int bSolo, int bUnstealth)
{
    if (!bSolo && bUnstealth) {
        UnstealthParty(bSolo);
    }
    m_bSoloMode = bSolo;
}

// FUNCTION: SWKOTOR 0x00565530
void CSWPartyTable::UpdateMembers(int bArg)
{
    for (int i = 0; i < PARTY_NUM_NPCS; i++) {
        SaveMember(i, bArg);
    }
}

// FUNCTION: SWKOTOR 0x005665C0
void CSWPartyTable::Save(int nArg, int bSaveMembers)
{
    if (bSaveMembers) {
        for (int i = 0; i < PARTY_NUM_NPCS; i++) {
            SaveMember(i, 0);
        }
        UpdateInventory(0);
    }
    SaveTable(nArg);
}

// FUNCTION: SWKOTOR 0x00563620
CExoString CSWPartyTable::GetFilename(int nNPC)
{
    CExoString sName;
    sName.Format("AVAILNPC%d", nNPC);
    return sName;
}

// FUNCTION: SWKOTOR 0x00563710
int CSWPartyTable::GetIsMember(int nNPC)
{
    for (int i = 0; i < m_nNumMembers; i++) {
        if (nNPC == m_aMembers[i]) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00563810
int CSWPartyTable::KillNPCObject(int nNPC, int bFade)
{
    if (nNPC < 0 || nNPC >= PARTY_NUM_NPCS) {
        return 0;
    }
    if (GetIsMember(nNPC)) {
        return 0;
    }
    unsigned long oid = m_aNPCObjectIds[nNPC];
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
    m_aNPCObjectIds[nNPC] = INVALID_OBJECT_ID;
    if (!pCreature) {
        return 0;
    }
    if (bFade) {
        CSWCCreature* pClient = pCreature->GetClientCreature();
        if (pClient) {
            pClient->SetFadeState(0);
            pClient->SetDesiredFadeState(0, 1, 0, 0);
            pClient->m_bFadeDestroy = 1;
        }
    }
    delete pCreature;
    return 1;
}

// FUNCTION: SWKOTOR 0x005638D0
int CSWPartyTable::AddGameInProgress()
{
    if (m_nGameInProgress == 0) {
        g_pExoResMan->AddResourceDirectory("GAMEINPROGRESS:");
    }
    return ++m_nGameInProgress;
}

// FUNCTION: SWKOTOR 0x00563950
void CSWPartyTable::RemoveGameInProgress()
{
    if (--m_nGameInProgress == 0) {
        g_pExoResMan->RemoveResourceDirectory("GAMEINPROGRESS:");
    }
}

// FUNCTION: SWKOTOR 0x00563B90
void CSWPartyTable::AutoLevelUpNPCs()
{
    for (int i = 0; i < m_nNumMembers; i++) {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(m_aNPCObjectIds[m_aMembers[i]]);
        if (pCreature && pCreature->m_pStats->CanLevelUp()) {
            pCreature->m_pStats->AutoLevelUp(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x00563BF0
unsigned long CSWPartyTable::GetTotalPlayedSeconds()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    __int64 nDiff = (((unsigned __int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime) - g_nSessionStartTime;
    if (nDiff < 0) {
        nDiff = 0;
    }
    return (unsigned long)(((unsigned __int64)nDiff + 5000000) / 10000000) + m_nPlayedSeconds;
}

// FUNCTION: SWKOTOR 0x00563C60
void CSWPartyTable::UnstealthParty(int nArg)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CGameObject* pPlayer = pServer->GetClientObjectByObjectId(pServer->GetPlayerCreatureId());
    if (pPlayer) {
        unsigned long oid = ((CSWSPlayer*)pPlayer)->m_oid38;
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
        if (pCreature) {
            pCreature->SetActivity(1, 0, nArg);
        }
    }
    for (unsigned char i = 0; i < m_nNumMembers; i++) {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(m_aNPCObjectIds[m_aMembers[i]]);
        if (pCreature) {
            pCreature->SetActivity(1, 0, nArg);
        }
    }
}

// FUNCTION: SWKOTOR 0x00565560
int CSWPartyTable::RemoveMember(int nNPC)
{
    if (m_nControlledNPC != -1 || m_nNumMembers <= 0 || nNPC < 0 || nNPC >= PARTY_NUM_NPCS
        || !m_aNPCAvailable[nNPC]) {
        return 0;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(GetNPCObject(nNPC, 0, 1));
    if (pCreature) {
        pCreature->ClearAllEffects(0);
    }
    for (int i = 0; i < m_nNumMembers; i++) {
        if (nNPC == m_aMembers[i]) {
            for (int j = i + 1; j < m_nNumMembers; j++) {
                m_aMembers[j - 1] = m_aMembers[j];
            }
            if (--m_nNumMembers == 0 && m_bSoloMode) {
                m_bSoloMode = 0;
            }
            return 1;
        }
    }
    return 0;
}

// NONMATCH: register rotation (eax/ecx/edx) in the GetINTEntry argument setup
// and `this` saved before instead of after the 2DA null test.
// FUNCTION: SWKOTOR 0x00566600 SYMBOL: ?GivePlotXP@CSWPartyTable@@QAEXABVCExoString@@H@Z
void CSWPartyTable::GivePlotXP(const CExoString& sLabel, int nPercent)
{
    int nValue = 0;
    C2DA* pPlotXP = g_pRules->m_p2DArrays->m_pPlotXP;
    if (!pPlotXP) {
        return;
    }
    pPlotXP->GetINTEntry(sLabel, "XP", &nValue);
    int nXP = (int)ceil(nValue * nPercent * 0.01f);
    if (nXP > 0) {
        DistributeExperience(nXP, 1);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(2, nXP);
    }
}

// NONMATCH: same register rotation as the CExoString overload.
// FUNCTION: SWKOTOR 0x005666E0 SYMBOL: ?GivePlotXP@CSWPartyTable@@QAEXHH@Z
void CSWPartyTable::GivePlotXP(int nRow, int nPercent)
{
    int nValue = 0;
    C2DA* pPlotXP = g_pRules->m_p2DArrays->m_pPlotXP;
    if (!pPlotXP) {
        return;
    }
    pPlotXP->GetINTEntry(nRow, "XP", &nValue);
    int nXP = (int)ceil(nValue * nPercent * 0.01f);
    if (nXP > 0) {
        DistributeExperience(nXP, 1);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(2, nXP);
    }
}

// FUNCTION: SWKOTOR 0x00563200
void CSWPartyTable::ClearTable()
{
    m_nFC = 0;
    m_nGameInProgress = 0;
    m_nControlledNPC = -1;
    m_nF8 = 0;
    m_n100 = 1;
    m_n104 = 0x7db3;
    m_n108 = 0xa488;
    m_nPlayedSeconds = 0;
    m_n10C = 0;
    int i;
    for (i = 0; i < 16; i++) {
        m_aTokens[i] = 0;
        m_aTokenValues[i] = 0;
    }
    m_nLeaderNPC = -1;
    m_bSoloMode = 0;
    m_n194 = 0;
    if (m_pItems) {
        delete m_pItems;
        m_pItems = NULL;
    }
    m_nNumMembers = 0;
    m_nE4 = 0;
    m_nFollowState = 0;
    m_nEC = -1;
    m_bPartyCreated = 0;
    for (i = 0; i < PARTY_NUM_NPCS; i++) {
        m_aNPCAvailable[i] = 0;
        m_aNPCSelectable[i] = 1;
        m_aNPCObjectIds[i] = INVALID_OBJECT_ID;
    }
    for (i = 0; i < 5; i++) {
        m_a120[i] = 2;
    }
    for (i = 0; i < 13; i++) {
        m_a134[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        m_a168[i] = -1;
    }
    if (m_pJournal) {
        delete m_pJournal;
        m_pJournal = NULL;
    }
}

// FUNCTION: SWKOTOR 0x00563D90
void CSWPartyTable::SaveJournal(CResGFF* pRes, CResStruct* pStruct)
{
    CSWSJournal* pJournal = GetPartyJournal();
    if (!pJournal || pJournal->m_aEntries.num == 0) {
        return;
    }
    pRes->WriteFieldINT(pStruct, g_nJournalSortOrder, "JNL_SortOrder");
    CResList cList;
    pRes->AddList(&cList, pStruct, "JNL_Entries");
    for (int i = 0; i < pJournal->m_aEntries.num; i++) {
        CResStruct cEntry;
        pRes->AddListElement(&cEntry, &cList, 0);
        pRes->WriteFieldCExoString(&cEntry, pJournal->m_aEntries.element[i].m_sPlotID, "JNL_PlotID");
        pRes->WriteFieldINT(&cEntry, pJournal->m_aEntries.element[i].m_nState, "JNL_State");
        pRes->WriteFieldDWORD(&cEntry, pJournal->m_aEntries.element[i].m_nDate, "JNL_Date");
        pRes->WriteFieldDWORD(&cEntry, pJournal->m_aEntries.element[i].m_nTime, "JNL_Time");
    }
}

// NONMATCH: prologue loads nXP into eax before saving `this` (original saves
// this to edi first and tests nXP in ecx).
// FUNCTION: SWKOTOR 0x005653A0
void CSWPartyTable::DistributeExperience(int nXP, int bSignal)
{
    int i;
    CSWSCreature* pCreature;
    if (nXP <= 0) {
        return;
    }
    m_nF8 += nXP;
    for (i = 0; i < m_nNumMembers; i++) {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        pCreature = pServer->GetCreatureByGameObjectID(m_aNPCObjectIds[m_aMembers[i]]);
        if (pCreature) {
            pCreature->m_sTag.CStr();
            pCreature->AddExperience(nXP);
            if ((g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags & 1)
                && pCreature->m_pStats->CanLevelUp()) {
                pCreature->m_pStats->AutoLevelUp(1);
            }
        }
    }
    CSWSPlayer* pPlayer = g_pAppManager->GetServerExoApp()->GetPlayerList()->GetHead()->AsSWSPlayer();
    unsigned long oidPC = pPlayer->m_oid38;
    CSWSCreature* pPC = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidPC);
    if (!pPC) {
        return;
    }
    pPC->AddExperience(nXP);
    if (bSignal) {
        CSWSEvent* pEvent = new CSWSEvent();
        pEvent->SetInteger(0, nXP);
        pPC->SignalEvent(0x8f, pEvent);
    }
}

// FUNCTION: SWKOTOR 0x00565620
int CSWPartyTable::AddMember(int nNPC, unsigned long oidCreature)
{
    if (m_nControlledNPC != -1 || m_nNumMembers == 2 || nNPC < 0 || nNPC >= PARTY_NUM_NPCS
        || !m_aNPCAvailable[nNPC]) {
        return 0;
    }
    if (!g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature)) {
        return 0;
    }
    int nCount = m_nNumMembers;
    int i;
    for (i = 0; i < m_nNumMembers; i++) {
        if (nNPC == m_aMembers[i]) {
            return 0;
        }
    }
    for (i = 0; i < PARTY_NUM_NPCS; i++) {
        if (i != nNPC && m_aNPCObjectIds[i] == oidCreature) {
            return 0;
        }
    }
    m_aMembers[nCount] = nNPC;
    m_aNPCObjectIds[nNPC] = oidCreature;
    CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
    CSWCCreature* pClientCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(oidCreature));
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pClientCreature) {
        pClientCreature->SetPartyMember(1);
    }
    if (pCreature) {
        pCreature->SetInParty(1, 1);
        TransferInventory(pCreature);
        pCreature->OnJoinParty();
    }
    if (m_nNumMembers == 0) {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        CSWSCreature* pPC = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
        if (pPC && pPC->m_bPlayerCharacter == 1) {
            m_bSoloMode = 1;
        }
    }
    m_nNumMembers++;
    return 1;
}

// FUNCTION: SWKOTOR 0x00563430
void CSWPartyTable::LoadJournal(CResGFF* pRes, CResStruct* pStruct)
{
    if (!GetPartyJournal()) {
        return;
    }
    CResList cList;
    if (!pRes->GetList(&cList, pStruct, "JNL_Entries")) {
        return;
    }
    int bSuccess;
    g_nJournalSortOrder = pRes->ReadFieldINT(pStruct, "JNL_SortOrder", bSuccess, g_nJournalSortOrder);
    CExoString sPlotID;
    int nCount = pRes->GetListCount(&cList);
    for (int i = 0; i < nCount; i++) {
        CResStruct cEntry;
        pRes->GetListElement(&cEntry, &cList, i);
        sPlotID = pRes->ReadFieldCExoString(&cEntry, "JNL_PlotID", bSuccess, "");
        int nState = pRes->ReadFieldINT(&cEntry, "JNL_State", bSuccess, 0);
        GetPartyJournal()->SetState(sPlotID, nState, 1);
        unsigned long nDate = pRes->ReadFieldDWORD(&cEntry, "JNL_Date", bSuccess, 0);
        GetPartyJournal()->SetDate(sPlotID, nDate, 1);
        unsigned long nTime = pRes->ReadFieldDWORD(&cEntry, "JNL_Time", bSuccess, 0);
        GetPartyJournal()->SetTime(sPlotID, nTime, 1);
    }
}

// FUNCTION: SWKOTOR 0x00563E80
void CSWPartyTable::SaveMember(int nNPC, int bClearActions)
{
    if (nNPC < 0 || nNPC >= PARTY_NUM_NPCS) {
        return;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_aNPCObjectIds[nNPC]);
    if (!pCreature) {
        return;
    }
    if (bClearActions) {
        pCreature->ClearAllActions(1);
    }
    CExoString sName = GetFilename(nNPC);
    CResGFF* pRes = new CResGFF();
    CResStruct cTop;
    pRes->CreateGFFFile(&cTop, "UTC ", "V2.0");
    pCreature->SaveCreature(pRes, &cTop);
    CExoString sPath = CExoString("GAMEINPROGRESS:") + sName;
    {
        CExoString sFileName(sPath.CStr());
        pRes->WriteGFFFile(sFileName, 0x7eb);
    }
    if (pRes) {
        delete pRes;
    }
}

// FUNCTION: SWKOTOR 0x00564030
void CSWPartyTable::UpdateInventory(int bClear)
{
    if (!m_pItems) {
        return;
    }
    CResGFF* pRes = new CResGFF();
    CResStruct cTop;
    pRes->CreateGFFFile(&cTop, "INV ", "V2.0");
    CResList cList;
    pRes->AddList(&cList, &cTop, "ItemList");
    CResStruct cItem;
    for (unsigned long i = 0; i < m_pItems->m_nItemCount; i++) {
        CSWSItem* pItem = m_pItems->GetItemInRepository(i);
        if (pItem && pRes->AddListElement(&cItem, &cList, 0)) {
            pItem->SaveItem(pRes, &cItem);
        }
    }
    CExoString sFile("GAMEINPROGRESS:INVENTORY");
    {
        CExoString sFileName(sFile.CStr());
        pRes->WriteGFFFile(sFileName, 0);
    }
    if (bClear) {
        m_pItems->ClearItems();
    }
    if (pRes) {
        delete pRes;
    }
}

// FUNCTION: SWKOTOR 0x005641E0
void CSWPartyTable::TransferInventory(CSWSCreature* pCreature)
{
    if (pCreature) {
        CSWSItem* pItem;
        int bInParty = pCreature->m_bInParty;
        pCreature->SetInParty(0, 0);
        m_nFC += pCreature->GetGold();
        CItemRepository* pRepository = pCreature->GetItemRepository(1);
        pCreature->SetInParty(bInParty, 0);
        if (!m_pItems) {
            m_pItems = new CItemRepository(g_pAppManager->GetServerExoApp()->GetPlayerCreatureId(), 1);
        }
        for (unsigned long i = 0; i < pRepository->m_nItemCount; i++) {
            pItem = pRepository->GetItemInRepository(i);
            if (pItem) {
                pItem->SetPossessor(OBJECT_ALL_POSSESSOR, 0, 0, INVALID_OBJECT_ID);
                m_pItems->AddItem(&pItem, 1, 0, 1);
            }
        }
        pRepository->ClearItems();
    }
}

// FUNCTION: SWKOTOR 0x005645F0 SYMBOL: ?AddNPC@CSWPartyTable@@QAEHHABVCExoString@@@Z
int CSWPartyTable::AddNPC(int nNPC, const CExoString& sTemplate)
{
    if (nNPC < 0 || nNPC >= PARTY_NUM_NPCS || m_aNPCAvailable[nNPC]) {
        return 0;
    }
    CSWSCreature* pCreature = new CSWSCreature(INVALID_OBJECT_ID, 0);
    if (!pCreature->LoadFromTemplate(sTemplate, 0)) {
        delete pCreature;
        return 0;
    }
    int nResult = AddNPC(nNPC, pCreature);
    delete pCreature;
    return nResult;
}

// FUNCTION: SWKOTOR 0x00564700
unsigned long CSWPartyTable::GetNPCObject(int nNPC, int bCreate, int bRevive)
{
    if (nNPC < 0 || nNPC >= PARTY_NUM_NPCS || !m_aNPCAvailable[nNPC]) {
        return INVALID_OBJECT_ID;
    }
    if (m_aNPCObjectIds[nNPC] != INVALID_OBJECT_ID) {
        return m_aNPCObjectIds[nNPC];
    }
    if (!bCreate) {
        return INVALID_OBJECT_ID;
    }
    CSWSCreature* pCreature = new CSWSCreature(INVALID_OBJECT_ID, 0);
    AddGameInProgress();
    if (!pCreature->LoadFromTemplate(GetFilename(nNPC), 0)) {
        delete pCreature;
        return INVALID_OBJECT_ID;
    }
    RemoveGameInProgress();
    if (bRevive && pCreature->GetCurrentHitPoints(0) <= 0) {
        pCreature->m_bF0 = 1;
        CGameEffect* pEffect = new CGameEffect(1);
        pEffect->m_nType = EFFECT_TYPE_RESURRECTION;
        pEffect->SetDurationType(DURATION_TYPE_INSTANT);
        pCreature->ApplyEffect(pEffect, 0, 0);
    }
    m_aNPCObjectIds[nNPC] = pCreature->m_idSelf;
    return pCreature->m_idSelf;
}

// NONMATCH: PercentXP temp string / 2DA load order, m_nF8 is fild'ed directly
// (original spills it to a stack slot first); register allocation differs.
// FUNCTION: SWKOTOR 0x00564300 SYMBOL: ?AddNPC@CSWPartyTable@@QAEHHPAVCSWSCreature@@@Z
int CSWPartyTable::AddNPC(int nNPC, CSWSCreature* pCreature)
{
    if (nNPC < 0 || nNPC >= PARTY_NUM_NPCS || !pCreature || m_aNPCAvailable[nNPC]) {
        return 0;
    }
    m_aNPCAvailable[nNPC] = 1;
    TransferInventory(pCreature);
    float fFactor = 1.0f;
    int nPercent = 0;
    C2DA* pPercentXP = g_pRules->m_p2DArrays->m_pPercentXP;
    int bFound = pPercentXP->GetINTEntry(nNPC, "PercentXP", &nPercent);
    if (bFound && nPercent) {
        fFactor = nPercent * 0.01f;
    }
    int nXP = (int)(m_nF8 * fFactor);
    int nNeeded = 0;
    for (int nLevel = 1; nLevel <= pCreature->m_pStats->GetLevel(0); nLevel++) {
        nNeeded += (nLevel - 1) * 1000;
    }
    if (nNeeded > nXP) {
        pCreature->m_nXPDebt = nNeeded - nXP;
    }
    else {
        pCreature->m_nXPDebt = 0;
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPC = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pPC && pPC->GetFaction()) {
        pPC->GetFaction()->AddMember(pCreature->m_idSelf, 0);
    }
    pCreature->m_pStats->SetMovementRate(0);
    CExoString sPath;
    sPath.Format("GAMEINPROGRESS:%s", GetFilename(nNPC));
    CResGFF* pRes = new CResGFF();
    CResStruct cTop;
    pRes->CreateGFFFile(&cTop, "UTC ", "V2.0");
    pCreature->SaveCreature(pRes, &cTop);
    return pRes->WriteGFFFile(CExoString(sPath.CStr()), 0x7eb);
}

// NONMATCH: the creature id is kept in a new stack slot (original reuses the
// bRevive slot), PercentXP test is folded into a bool, and register choice.
// FUNCTION: SWKOTOR 0x00565130
unsigned long CSWPartyTable::SpawnNPC(int nNPC, int bUsePosition, const Vector* pPosition,
                                      const Vector* pOrientation, int bRevive)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    unsigned long oidNPC = GetNPCObject(nNPC, 1, bRevive);
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oidNPC);
    if (pCreature) {
    int bCommandable = pCreature->m_bCommandable;
    pCreature->m_bCommandable = 1;
    pCreature->ClearAllActions(1);
    pCreature->m_bCommandable = bCommandable;
    CSWSArea* pArea = pServer->GetModule()->GetArea();
    Vector vPosition;
    Vector vSafe;
    Vector vOrientation;
    vSafe.x = 0.0f;
    vSafe.y = 0.0f;
    vSafe.z = 0.0f;
    if (bUsePosition) {
        vPosition = *pPosition;
        vOrientation = *pOrientation;
    }
    else {
        vPosition = pCreature->m_vPosition;
        vOrientation = pCreature->m_vOrientation;
    }
    if (pArea->GetRoom(vPosition, 0)) {
        pArea->ComputeSafeLocation(vPosition, 20.0f, pCreature->m_n340, 1, &vSafe);
    }
    pCreature->AddToArea(pArea, vSafe.x, vSafe.y, vSafe.z, 1, 0);
    pCreature->SetOrientation(vOrientation);
    float fFactor = 1.0f;
    int nPercent;
    if (g_pRules->m_p2DArrays->m_pPercentXP->GetINTEntry(nNPC, "PercentXP", &nPercent) && nPercent) {
        fFactor = nPercent * 0.01f;
    }
    int nPartyXP = m_nF8;
    int nRemaining = pCreature->m_pStats->m_nExperience - pCreature->m_nXPDebt;
    if (nRemaining < (int)(nPartyXP * fFactor)) {
        pCreature->AddExperience(nPartyXP - (int)(nRemaining / (float)nPercent * 100.0f));
        if ((g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags & 1)
            && pCreature->m_pStats->CanLevelUp()) {
            pCreature->m_pStats->AutoLevelUp(1);
        }
    }
    }
    return oidNPC;
}

// NONMATCH: stack frame is 0xc bytes smaller than the original (different
// slot sharing between the spawn vectors, CResRef and GFF locals).
// FUNCTION: SWKOTOR 0x00565760
void CSWPartyTable::CreateParty()
{
    if (m_bPartyCreated) {
        return;
    }
    AddGameInProgress();
    CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    pClient->GetSWParty();
    if (m_nControlledNPC == -1) {
        for (unsigned long i = 0; i < m_nNumMembers; i++) {
            Vector vOrientation;
            Vector vPosition;
            vOrientation.x = 0.0f;
            vOrientation.y = 0.0f;
            vOrientation.z = 0.0f;
            vPosition.x = 0.0f;
            vPosition.y = 0.0f;
            vPosition.z = 0.0f;
            {
                unsigned long oidNPC = SpawnNPC(m_aMembers[i], 0, &vPosition, &vOrientation,
                                                !g_pAppManager->GetServerExoApp()->GetLoadFromSaveGame());
                CSWCCreature* pClientCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(oidNPC));
                CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oidNPC);
                if (pClientCreature) {
                    pClientCreature->SetPartyMember(1);
                }
                if (pCreature) {
                    pCreature->SetInParty(1, 1);
                }
            }
        }
    }
    else {
        m_aNPCObjectIds[m_nControlledNPC] = pServer->GetPlayerCreatureId();
    }
    CResGFF* pRes = new CResGFF(0, "INV ", "INVENTORY");
    if (pRes->GetLoaded()) {
        if (!m_pItems) {
            m_pItems = new CItemRepository(pServer->GetPlayerCreatureId(), 1);
        }
        else {
            m_pItems->ClearItems();
            m_bPartyCreated = 0;
        }
        CResStruct cTop;
        CResList cList;
        pRes->GetTopLevelStruct(&cTop);
        pRes->GetList(&cList, &cTop, "ItemList");
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CSWSItem* pItem = NULL;
            CResStruct cItem;
            pRes->GetListElement(&cItem, &cList, i);
            pItem = new CSWSItem(INVALID_OBJECT_ID);
            if (pItem) {
                if (!pItem->LoadItem(pRes, &cItem)) {
                    delete pItem;
                }
                else if (m_pItems->AddItem(&pItem, 1, 0, 0) && pItem) {
                    pItem->SetPossessor(pServer->GetPlayerCreatureId(), 0, 0, INVALID_OBJECT_ID);
                }
            }
        }
    }
    if (pRes) {
        delete pRes;
    }
    RemoveGameInProgress();
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature) {
        TransferInventory(pCreature);
    }
    int k;
    for (k = 0; k < PARTY_NUM_NPCS; k++) {
        pCreature = pServer->GetCreatureByGameObjectID(m_aNPCObjectIds[k]);
        if (pCreature) {
            TransferInventory(pCreature);
        }
    }
    pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature) {
        pCreature->OnJoinParty();
    }
    for (k = 0; k < PARTY_NUM_NPCS; k++) {
        pCreature = pServer->GetCreatureByGameObjectID(m_aNPCObjectIds[k]);
        if (pCreature) {
            pCreature->OnJoinParty();
        }
    }
    m_bPartyCreated = 1;
}

// NONMATCH: structure matches; pOld is kept in ebx instead of being reloaded
// from its stack slot, the !pTarget exit is shared, and registers rotate.
// FUNCTION: SWKOTOR 0x005667C0
int CSWPartyTable::SwitchPlayerCharacter(int nNPC)
{
    CExoString sFileName;
    CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSPlayer* pPlayer = (CSWSPlayer*)pServer->GetPlayerList()->GetHead();
    CSWSCreature* pOld = pServer->GetCreatureByGameObjectID(pPlayer->m_oid38);
    CSWSArea* pArea = pOld->GetArea();
    int bReposition = 1;
    Vector vPosition;
    Vector vOrientation;
    vPosition.x = 0.0f;
    vPosition.y = 0.0f;
    vPosition.z = 0.0f;
    vOrientation.x = 0.0f;
    vOrientation.y = 0.0f;
    vOrientation.z = 0.0f;
    CSWSCreature* pTarget;
    int nNewControlled;
    if (nNPC == -1) {
        if (m_nControlledNPC == -1) {
            return 1;
        }
        CSWSCreature* pPC = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
        pPC->m_pStats->m_bIsPC = 0;
        pPC->m_bIsPC = 0;
        SaveMember(m_nControlledNPC, 0);
        pTarget = new CSWSCreature(INVALID_OBJECT_ID, 0);
        AddGameInProgress();
        if (!pTarget->LoadFromTemplate("PC", 0)) {
            delete pTarget;
            return 0;
        }
        RemoveGameInProgress();
        vPosition = pOld->m_vPosition;
        vOrientation = pOld->m_vOrientation;
        KillNPCObject(m_nControlledNPC, 1);
        nNewControlled = -1;
    }
    else {
        if (nNPC < 0 || nNPC >= PARTY_NUM_NPCS || !m_aNPCAvailable[nNPC]) {
            return 0;
        }
        if (m_nControlledNPC == nNPC) {
            return 1;
        }
        if (m_nControlledNPC == -1) {
            sFileName = "GAMEINPROGRESS:PC";
            CResGFF* pRes = new CResGFF();
            CResStruct cTop;
            pRes->CreateGFFFile(&cTop, "UTC ", "V2.0");
            pOld->SaveCreature(pRes, &cTop);
            if (!pRes->WriteGFFFile(CExoString(sFileName.CStr()), 0x7eb)) {
                return 0;
            }
        }
        else {
            CSWSCreature* pPC = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
            pPC->m_pStats->m_bIsPC = 0;
            pPC->m_bIsPC = 0;
            SaveMember(m_nControlledNPC, 0);
        }
        unsigned long oidTarget;
        if (!m_aNPCAvailable[nNPC] || (oidTarget = m_aNPCObjectIds[nNPC]) == INVALID_OBJECT_ID) {
            oidTarget = INVALID_OBJECT_ID;
        }
        pTarget = pServer->GetCreatureByGameObjectID(oidTarget);
        if (!pTarget) {
            pTarget = pServer->GetCreatureByGameObjectID(GetNPCObject(nNPC, 1, 1));
            vPosition = pOld->m_vPosition;
            vOrientation = pOld->m_vOrientation;
        }
        else {
            bReposition = 0;
        }
        if (m_nControlledNPC == -1) {
            CSWCCreature* pClientCreature = pOld->GetClientCreature();
            if (pClientCreature) {
                pClientCreature->SetFadeState(0);
                pClientCreature->SetDesiredFadeState(0, 1, 0, 0);
                pClientCreature->m_bFadeDestroy = 1;
            }
            delete pOld;
        }
        else {
            KillNPCObject(m_nControlledNPC, 1);
        }
        nNewControlled = nNPC;
    }
    if (!pTarget) {
        return 0;
    }
    pClient->GetSWParty()->ResetPartyGui();
    pTarget->SetInParty(1, 1);
    pTarget->m_pStats->m_bIsPC = 1;
    pTarget->m_bIsPC = 1;
    CItemRepository* pRepository = pTarget->GetItemRepository(1);
    if (pRepository) {
        for (unsigned long i = 0; i < pRepository->m_nItemCount; i++) {
            CSWSItem* pItem = m_pItems->GetItemInRepository(i);
            if (pItem) {
                pItem->SetPossessor(pTarget->m_idSelf, 0, 0, INVALID_OBJECT_ID);
            }
        }
    }
    if (bReposition) {
        pTarget->AddToArea(pArea, vPosition.x, vPosition.y, vPosition.z, 0, 0);
        pTarget->SetOrientation(vOrientation);
    }
    pPlayer->m_oid38 = pTarget->m_idSelf;
    pPlayer->SetGameObject(pTarget);
    for (int i = m_nNumMembers - 1; i >= 0; i--) {
        int nMember = m_aMembers[i];
        RemoveMember(nMember);
        if (nMember >= 0 && nMember < PARTY_NUM_NPCS && !GetIsMember(nMember)) {
            CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_aNPCObjectIds[nMember]);
            m_aNPCObjectIds[nMember] = INVALID_OBJECT_ID;
            if (pCreature) {
                CSWCCreature* pClientCreature = pCreature->GetClientCreature();
                if (pClientCreature) {
                    pClientCreature->SetFadeState(0);
                    pClientCreature->SetDesiredFadeState(0, 1, 0, 0);
                    pClientCreature->m_bFadeDestroy = 1;
                }
                delete pCreature;
            }
        }
    }
    m_nControlledNPC = nNewControlled;
    return 1;
}

// NONMATCH: x87 sequencing differs (the original adds straight from the
// returned vector and reuses the GetFormationAngle argument slot for the
// Quaternion angle); frame is 0xc bytes larger.
// FUNCTION: SWKOTOR 0x00565B00
void CSWPartyTable::PositionParty()
{
    CSWParty* pParty = g_pAppManager->GetClientExoApp()->GetSWParty();
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPC = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    Vector vOrientation = pPC->m_vOrientation;
    pParty->SetLeaderPosition(pPC->m_vPosition, GetYaw(vOrientation), 0);
    if (!m_n10C) {
        Vector vPosition;
        vPosition.x = 0.0f;
        vPosition.y = 0.0f;
        vPosition.z = 0.0f;
        for (int i = 0; i < m_nNumMembers; i++) {
            CSWPartyFormation* pFormation = (CSWPartyFormation*)((char*)pParty + 0x2dc);
            Vector vAxis;
            vAxis.x = 0.0f;
            vAxis.y = 0.0f;
            vAxis.z = 1.0f;
            CSWPartySlot* pSlot = &pParty->m_aSlots[i];
            Vector vDirection = pSlot->m_vDirection;
            Vector vOffset = pSlot->m_vOffset;
            float fAngle = pFormation->GetEntry(pSlot->m_nFormationEntry)->m_fAngle;
            Vector vRotated = GetDirection(Quaternion(vAxis, GetFormationAngle(fAngle, vDirection)));
            vPosition = vOffset + vRotated;
            unsigned long oidNPC = GetNPCObject(m_aMembers[i], 0, 1);
            g_pAppManager->GetServerExoApp()->ComputeSafeLocation(vPosition, 10.0f, oidNPC, 0, &vPosition);
            CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oidNPC);
            pCreature->SetPosition(vPosition, 1, 1, 0);
            pSlot->m_vPosition = vPosition;
            pCreature->SetOrientation(vOrientation);
        }
        UnstealthParty(0);
        m_bSoloMode = 0;
    }
    m_n10C = 0;
}
