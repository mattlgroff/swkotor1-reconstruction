// Cheat console helpers emitted in nwcarea.o (worker C s3): the
// swkotor.ini EnableCheats check, the skill rank cheat and its eight
// per-skill entry points. The skill entry points are real starts missing from
// functions.csv; their names are descriptive.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Clamp macros evaluate their argument twice (the original re-tests).
#define CHEAT_MIN(a, b) (((a) > (b)) ? (b) : (a))
#define CHEAT_MAX(a, b) (((a) < (b)) ? (b) : (a))

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    int AsINT() const;                              // 0x005E5630
    char* CStr();                                   // 0x005E5670
    void Format(const char* sFormat, ...);          // 0x005E5680
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile,
                     const CExoString& sSection, const CExoString& sKey);  // 0x005E67C0
    void* m_pInternal;
};

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char* s);                         // 0x00406D80
};

class CSWPartyTable {
public:
    void DistributeExperience(int nXP, int bUnknown);   // 0x005653A0
    char pad_0x0[0x194];
    int m_bCheatUsed;                               // 0x194
};

class CSWClass {
public:
    static int IsForceUserClass(int nClass);  // 0x005BE4A0
};

class CSWSCreatureStats {
public:
    unsigned char GetClass(int nIndex);             // 0x005A4E90
    unsigned char GetLevel(int bIncludeNegative);   // 0x005A5FD0
    int GetExpNeededForLevelUp();                   // 0x005A6610
    char pad_0x0[0x89];
    unsigned char m_nNumMultiClasses;               // 0x89
    void SetSkillRank(unsigned char nSkill, char nRank);    // 0x005A54C0
    void SetSTRBase(unsigned char n);               // 0x005A9FE0
    void SetDEXBase(unsigned char n);               // 0x005AA020
    void SetCONBase(unsigned char n, int bUnknown); // 0x005AA060
    void SetINTBase(unsigned char n);               // 0x005AA0F0
    void SetWISBase(unsigned char n);               // 0x005AA130
    void SetCHABase(unsigned char n);               // 0x005AA170
    void ModifyAlignment(short nDelta);             // 0x005AE610
};

class CSWSCreature {
public:
    int GetGold();                                  // 0x004EDD60
    void SetGold(int nGold);                        // 0x004EDD90
    char pad_0x0[0x90];
    float m_fPositionX;                             // 0x90
    float m_fPositionY;                             // 0x94
    float m_fPositionZ;                             // 0x98
    char pad_0x9c[0xa74 - 0x9c];
    CSWSCreatureStats* m_pStats;                    // 0xa74
};

struct CSWPlayerObject {
    char pad_0x0[0x38];
    unsigned long m_oidPC;                          // 0x38
};

class CServerExoApp {
public:
    void SetMoveToModuleString(const CExoString& sModule);  // 0x004AECD0
    void SetMoveToModulePending(int bPending);      // 0x004AECC0
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
    unsigned long GetPlayerCreatureId();            // 0x004AEA40
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);    // 0x004AE770
    CSWPlayerObject* GetClientObjectByPlayerID(unsigned long nPlayerId, int b);    // 0x004AE920
};

class CSWCMessage {
public:
    int SendPlayerToServer_70_0F();                 // 0x00677940
    int SendPlayerToServer_0F_04();                 // 0x006779D0
};

class CSWCArea {
public:
    int RestartMiniGame();                          // 0x00605E50
    void ResetColors();                             // 0x00605FE0
    char pad_0x0[0x264];
    void* m_pMiniGame;                              // 0x264
};

class CSWCModule {
public:
    char pad_0x0[0x38];
    CExoString m_sName;                             // 0x38
    char pad_0x40[0x8];
    CSWCArea* m_pArea;                              // 0x48
};

struct CClientOptions {
    char pad_0x0[0x88];
    int m_bUnknown88;                               // 0x88 (cheat toggle)
    int m_bUnknown8C;                               // 0x8c (cheat toggle)
};

struct CGuiInGameX {
    char pad_0x0[0xc20];
    int m_bShowAllMapNotes;                         // 0xc20 (descriptive)
};

class CClientExoApp {
public:
    CGuiInGameX* GetInGameGui();                    // 0x005ED690
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
    CClientOptions* GetClientOptions();             // 0x005ED700
    CSWCModule* GetModule();                        // 0x005ED530
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    int CreatureAcquireItem(unsigned long oidCreature, const CResRef& cResRef);   // 0x005ED7E0
};

extern int g_bCheatToggle7A267C;                    // 0x007A267C (descriptive)

class CAppManager {
public:
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }

    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

int GetCheatsTurnedOn();
void AurPostString(const char* sText, int nX, int nY, float fDuration);  // 0x0044D490
void CheatSetSkillRank(unsigned char nSkill, char* sArgs);

// FUNCTION: SWKOTOR 0x0060A300
int GetCheatsTurnedOn()
{
    int nValue = 0;
    CExoIni cIni;
    CExoString sValue;
    if (g_pAppManager->m_pServerExoApp == 0) {
        return 0;
    }
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Game Options", "EnableCheats")) {
        nValue = sValue.AsINT();
        if (nValue) {
            g_pAppManager->m_pServerExoApp->GetPartyTable()->m_bCheatUsed = 1;
        }
    }
    return nValue == 1;
}

// Descriptive name: cheat entry point that sends the client message 70/0F
// (real start missing from functions.csv).
// FUNCTION: SWKOTOR 0x0060A460
void CheatSendMessage70_0F()
{
    if (GetCheatsTurnedOn()) {
        g_pAppManager->m_pClientExoApp->GetSWCMessage()->SendPlayerToServer_70_0F();
    }
}

// Descriptive name: sets one of the player's skill ranks (clamped 0..99).
// FUNCTION: SWKOTOR 0x0060A480
void CheatSetSkillRank(unsigned char nSkill, char* sArgs)
{
    int nRank = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nRank);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nRank < 0) {
        nRank = 0;
    } else if (nRank > 99) {
        nRank = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature) {
        pCreature->m_pStats->SetSkillRank(nSkill, (char)nRank);
    }
}

// FUNCTION: SWKOTOR 0x0060A510
void CheatSetComputerUse(char* sArgs)
{
    CheatSetSkillRank(0, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A520
void CheatSetDemolitions(char* sArgs)
{
    CheatSetSkillRank(1, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A530
void CheatSetStealth(char* sArgs)
{
    CheatSetSkillRank(2, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A540
void CheatSetAwareness(char* sArgs)
{
    CheatSetSkillRank(3, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A550
void CheatSetPersuade(char* sArgs)
{
    CheatSetSkillRank(4, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A560
void CheatSetRepair(char* sArgs)
{
    CheatSetSkillRank(5, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A570
void CheatSetSecurity(char* sArgs)
{
    CheatSetSkillRank(6, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A580
void CheatSetTreatInjury(char* sArgs)
{
    CheatSetSkillRank(7, sArgs);
}

// Descriptive name: sets one of the player's six base abilities (0..99).
// FUNCTION: SWKOTOR 0x0060A590
void CheatSetAbility(int nAbility, char* sArgs)
{
    int nValue = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nValue);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > 99) {
        nValue = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature && pCreature->m_pStats) {
        switch (nAbility) {
        case 0:
            pCreature->m_pStats->SetSTRBase((unsigned char)nValue);
            break;
        case 1:
            pCreature->m_pStats->SetDEXBase((unsigned char)nValue);
            break;
        case 2:
            pCreature->m_pStats->SetCONBase((unsigned char)nValue, 1);
            break;
        case 3:
            pCreature->m_pStats->SetINTBase((unsigned char)nValue);
            break;
        case 4:
            pCreature->m_pStats->SetWISBase((unsigned char)nValue);
            break;
        case 5:
            pCreature->m_pStats->SetCHABase((unsigned char)nValue);
            break;
        }
    }
}

// FUNCTION: SWKOTOR 0x0060A690
void CheatSetStrength(char* sArgs)
{
    CheatSetAbility(0, sArgs);
}

// FUNCTION: SWKOTOR 0x0060A6A0
void CheatSetDexterity(char* sArgs)
{
    int nValue = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nValue);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > 99) {
        nValue = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature && pCreature->m_pStats) {
        pCreature->m_pStats->SetDEXBase((unsigned char)nValue);
    }
}

// FUNCTION: SWKOTOR 0x0060A730
void CheatSetConstitution(char* sArgs)
{
    int nValue = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nValue);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > 99) {
        nValue = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature && pCreature->m_pStats) {
        pCreature->m_pStats->SetCONBase((unsigned char)nValue, 1);
    }
}

// FUNCTION: SWKOTOR 0x0060A7D0
void CheatSetIntelligence(char* sArgs)
{
    int nValue = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nValue);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > 99) {
        nValue = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature && pCreature->m_pStats) {
        pCreature->m_pStats->SetINTBase((unsigned char)nValue);
    }
}

// FUNCTION: SWKOTOR 0x0060A860
void CheatSetWisdom(char* sArgs)
{
    int nValue = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nValue);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > 99) {
        nValue = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature && pCreature->m_pStats) {
        pCreature->m_pStats->SetWISBase((unsigned char)nValue);
    }
}

// FUNCTION: SWKOTOR 0x0060A8F0
void CheatSetCharisma(char* sArgs)
{
    int nValue = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nValue);
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > 99) {
        nValue = 99;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature && pCreature->m_pStats) {
        pCreature->m_pStats->SetCHABase((unsigned char)nValue);
    }
}

// FUNCTION: SWKOTOR 0x0060A980
void CheatGiveExperience(char* sArgs)
{
    int nXP = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nXP);
    if (nXP > 0) {
        CSWPartyTable* pParty = g_pAppManager->m_pServerExoApp->GetPartyTable();
        if (pParty) {
            pParty->DistributeExperience(nXP, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0060A9E0
void CheatToggleOption88()
{
    if (GetCheatsTurnedOn()) {
        int bValue = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_bUnknown88;
        g_pAppManager->m_pClientExoApp->GetClientOptions()->m_bUnknown88 = !bValue;
    }
}

// FUNCTION: SWKOTOR 0x0060AA20
void CheatSendMessage0F_04()
{
    if (GetCheatsTurnedOn()) {
        g_pAppManager->m_pClientExoApp->GetSWCMessage()->SendPlayerToServer_0F_04();
    }
}

// FUNCTION: SWKOTOR 0x0060AA40
void CheatRestartMiniGame()
{
    if (GetCheatsTurnedOn() && g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pMiniGame) {
        g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->RestartMiniGame();
    }
}

// FUNCTION: SWKOTOR 0x0060AA80
void CheatResetAreaColors()
{
    if (GetCheatsTurnedOn() && g_pAppManager && g_pAppManager->m_pClientExoApp) {
        CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
        if (pModule && pModule->m_pArea) {
            pModule->m_pArea->ResetColors();
        }
    }
}

// FUNCTION: SWKOTOR 0x0060AAC0
void CheatModifyAlignment(int nDelta)
{
    if (!GetCheatsTurnedOn() || !g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature) {
        pCreature->m_pStats->ModifyAlignment((short)CHEAT_MAX(CHEAT_MIN(nDelta, 100), -100));
    }
}

// FUNCTION: SWKOTOR 0x0060AB30
void CheatModifyAlignmentInverse(int nDelta)
{
    if (!GetCheatsTurnedOn() || !g_pAppManager->m_pServerExoApp->GetPartyTable()) {
        return;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (pCreature) {
        pCreature->m_pStats->ModifyAlignment((short)CHEAT_MIN(CHEAT_MAX(-nDelta, -100), 100));
    }
}

// FUNCTION: SWKOTOR 0x0060ABB0
void CheatToggleGlobal7A267C()
{
    if (GetCheatsTurnedOn()) {
        g_bCheatToggle7A267C = !g_bCheatToggle7A267C;
    }
}

// FUNCTION: SWKOTOR 0x0060ABD0
void CheatAddGold(char* sArgs)
{
    int nGold = 0;
    sscanf(sArgs, "%d", &nGold);
    if (GetCheatsTurnedOn() && nGold > 0) {
        CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
        if (pCreature) {
            pCreature->SetGold(pCreature->GetGold() + __max(nGold, 99));
        }
    }
}

// FUNCTION: SWKOTOR 0x0060AC50
void CheatGiveItem(char* sArgs)
{
    char sItem[0x100];
    if (!GetCheatsTurnedOn()) {
        return;
    }
    char* pSpace = strchr(sArgs, ' ');
    if (pSpace) {
        *pSpace = 0;
        strcpy(sItem, sArgs);
    } else {
        strcpy(sItem, sArgs);
    }
    CResRef cItem(sItem);
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cItem);
}

// FUNCTION: SWKOTOR 0x0060ACD0
void CheatGiveMedicalEquipment(char* sArgs)
{
    int nCount = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nCount);
    if (nCount == 0 || nCount > 99) {
        nCount = 99;
    }
    for (int i = 0; i < nCount; i++) {
        CResRef cItem("g_i_medeqpmnt02");
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cItem);
    }
}

// FUNCTION: SWKOTOR 0x0060AD60
void CheatGiveRepairEquipment(char* sArgs)
{
    int nCount = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nCount);
    if (nCount == 0 || nCount > 99) {
        nCount = 99;
    }
    for (int i = 0; i < nCount; i++) {
        CResRef cItem("g_i_drdrepeqp002");
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cItem);
    }
}

// FUNCTION: SWKOTOR 0x0060ADF0
void CheatGiveComputerSpikes(char* sArgs)
{
    int nCount = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nCount);
    if (nCount == 0 || nCount > 99) {
        nCount = 99;
    }
    for (int i = 0; i < nCount; i++) {
        CResRef cItem("g_i_progspike01");
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cItem);
    }
}

// FUNCTION: SWKOTOR 0x0060AE80
void CheatGiveSecuritySpikes(char* sArgs)
{
    int nCount = 0;
    if (!GetCheatsTurnedOn()) {
        return;
    }
    sscanf(sArgs, "%d", &nCount);
    if (nCount == 0 || nCount > 99) {
        nCount = 99;
    }
    for (int i = 0; i < nCount; i++) {
        CResRef cItem("g_i_secspike01");
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cItem);
    }
}

// FUNCTION: SWKOTOR 0x0060AF10
void CheatGiveSithArmor()
{
    if (GetCheatsTurnedOn()) {
        CResRef cItem("ptar_sitharmor");
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cItem);
    }
}

// FUNCTION: SWKOTOR 0x0060AF50
void CheatWarp(char* sArgs)
{
    char sModule[0x100];
    if (!GetCheatsTurnedOn()) {
        return;
    }
    char* pSpace = strchr(sArgs, ' ');
    if (pSpace) {
        *pSpace = 0;
        strcpy(sModule, sArgs);
    } else {
        strcpy(sModule, sArgs);
    }
    g_pAppManager->GetServerExoApp()->SetMoveToModuleString(sModule);
    g_pAppManager->m_pServerExoApp->SetMoveToModulePending(1);
}

// FUNCTION: SWKOTOR 0x0060B020
void CheatToggleOption8C()
{
    if (GetCheatsTurnedOn()) {
        int bValue = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_bUnknown8C;
        g_pAppManager->m_pClientExoApp->GetClientOptions()->m_bUnknown8C = !bValue;
    }
}

// FUNCTION: SWKOTOR 0x0060B060
void CheatWhereAmI()
{
    if (!GetCheatsTurnedOn()) {
        return;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    if (!pCreature) {
        return;
    }
    CExoString sText;
    if (g_pAppManager->m_pClientExoApp->GetModule()) {
        sText.Format("Module: %s", g_pAppManager->m_pClientExoApp->GetModule()->m_sName.CStr());
        AurPostString(sText.CStr(), 3, 3, 10.0f);
    }
    sText.Format("Position: (%3.2f, %3.2f, %3.2f)", pCreature->m_fPositionX, pCreature->m_fPositionY,
                 pCreature->m_fPositionZ);
    AurPostString(sText.CStr(), 3, 4, 10.0f);
}

// FUNCTION: SWKOTOR 0x0060B190
void CheatLevelUp()
{
    if (!GetCheatsTurnedOn()) {
        return;
    }
    int i;
    int bForceUser = 0;
    CSWPlayerObject* pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByPlayerID(0, 1);
    if (!pPlayer) {
        return;
    }
    unsigned long oidPC = pPlayer->m_oidPC;
    CSWSCreature* pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidPC);
    if (!pCreature) {
        return;
    }
    for (i = 0; i < pCreature->m_pStats->m_nNumMultiClasses; i++) {
        if (CSWClass::IsForceUserClass(pCreature->m_pStats->GetClass(i))) {
            bForceUser = 1;
        }
    }
    if (!bForceUser && pCreature->m_pStats->GetLevel(0) == 15) {
        return;
    }
    if (pCreature->m_pStats->GetLevel(0) == 20) {
        return;
    }
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    pServer->GetPartyTable()->DistributeExperience(pStats->GetExpNeededForLevelUp(), 1);
}

// FUNCTION: SWKOTOR 0x0060B270
void CheatShowAllMapNotes()
{
    if (GetCheatsTurnedOn()) {
        g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bShowAllMapNotes = 1;
    }
}
