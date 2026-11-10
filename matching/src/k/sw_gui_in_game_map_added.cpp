// CSWGuiInGameMap::OnPanelAdded (swguiingamemap.o): area name, map image,
// map notes and the party-select / return-to-Ebon-Hawk button states.
// Panel offsets as sw_gui_in_game_map.cpp; server classes trimmed.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
    void Format(const char* szFormat, ...);         // 0x005E5680
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    void CopyToString(CExoString& s) const;         // 0x00405F70
    char m_resRef[16];
};

class Vector {
public:
    float x, y, z;
};

extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vGuiDisabledTextColor;              // 0x007A2408
extern Vector g_vUpgradeAvailableColor;             // 0x007A2378
extern Vector g_vGuiDisabledBorderColor;            // 0x007A2384

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
    void SetStrRef(int nStrRef);                    // 0x00415E50
    void SetColor(const Vector& vColor);            // 0x00414E10
    void SetAlignment(int nAlignment);              // 0x00415F20
};

class CSWGuiImageParams {
public:
    void SetImage(const CResRef& cImage, int bForce);   // 0x00415900
    void SetB1(int b);                              // 0x004159C0
};

class CSWCArea {
public:
    char pad_0x0[0x17c];
    CExoString m_sName;                             // 0x17c
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CSWPartyMemberData {
public:
    unsigned long m_oidCreature;                    // 0x00
    char pad_0x4[0x88 - 0x4];
};

class CSWParty {
public:
    int IsPartyTogether();                          // 0x00635350
    int m_nCount;                                   // 0x00
    char pad_0x4[0x20];
    CSWPartyMemberData m_aMembers[3];               // 0x24
};

class CClientOptions {
public:
    char pad_0x0[0x14];
    unsigned long m_nFlags;                         // 0x14
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    CClientOptions* GetClientOptions();             // 0x005ED700
    CSWParty* GetParty();                           // 0x005ED8B0
    int ShowTutorialWindow(int nWindow, unsigned long oid1, unsigned long oid2, int nArg);  // 0x005EDF40
};

class CSWSArea {
public:
    int GetFirstObjectIndiceByX(int* pnIndex, float fX);    // 0x00506F20
    char pad_0x0[0x190];
    unsigned long* m_pObjectIds;                    // 0x190 (descriptive)
    int m_nObjects;                                 // 0x194 (descriptive)
    char pad_0x198[0x2ac - 0x198];
    int m_bUnescapable;                             // 0x2ac
};

struct CSWSCreatureListEntry {
    char pad_0x0[0x4];
    unsigned char m_nFlags;                         // 0x04 (bit 0: seen)
};

class CSWSCreature {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    float GetSpotCheckDistance();                   // 0x004EFC70
    CSWSCreatureListEntry* GetFromList920(unsigned long oid);  // 0x00517A20 (placeholder name)
    int GetReputation(unsigned long oid, int bForce);   // 0x0057CB80
    char pad_0x0[0x4];
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x90 - 0x8];
    Vector m_vPosition;                             // 0x90
};

class CAreaMap {
public:
    char pad_0x0[0x4];
    unsigned long m_nWidth;                         // 0x04 (descriptive)
};

class CSWSModule {
public:
    char pad_0x0[0x30];
    CResRef m_cResRef;                              // 0x30
    char pad_0x40[0x218 - 0x40];
    CAreaMap* m_pAreaMap;                           // 0x218 (descriptive)
};

class CSWPartyTable {
public:
    char pad_0x0[0x100];
    int m_bCanReturn;                               // 0x100 (descriptive)
    unsigned long m_nReturnStrRef;                  // 0x104 (descriptive)
};

class CServerExoApp {
public:
    CSWSModule* GetModule();                        // 0x004AE6B0
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);    // 0x004AE770
    unsigned long ClientToServerObjectID(unsigned long oid);      // 0x004AEA30
    unsigned long GetPlayerCreatureId();            // 0x004AEA40
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiMapHider {
public:
    CExoString InitializeMapNotes(int bFlag);       // 0x00692C70
    char pad_0x0[0x248];
};

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void OnPanelAdded();                    // 0x0040B870
    char pad_0x4[0x64 - 0x4];
};

class CSWGuiInGameMap : public CSWGuiPanel {
public:
    virtual void OnPanelAdded();
    void SetMapNoteText(const CExoString& sNote);   // 0x006929B0

    char pad_0x64[0x28c - 0x64];
    CSWGuiTextParams m_cAreaText;                   // 0x28c (LBL_Area text)
    char pad_0x28d[0x5f4 - 0x28d];
    Vector m_vReturnBorderColor;                    // 0x5f4 (BTN_RETURN border)
    char pad_0x600[0x6d0 - 0x600];
    CSWGuiTextParams m_cReturnText;                 // 0x6d0 (BTN_RETURN text)
    char pad_0x6d1[0x7b8 - 0x6d1];
    Vector m_vPartyBorderColor;                     // 0x7b8 (BTN_PRTYSLCT border)
    char pad_0x7c4[0x894 - 0x7c4];
    CSWGuiTextParams m_cPartyText;                  // 0x894 (BTN_PRTYSLCT text)
    char pad_0x895[0xe38 - 0x895];
    CSWGuiMapHider m_cMapHider;                     // 0xe38
    char pad_0x1080[0x10a8 - 0x1080];
    CSWGuiImageParams m_cMapImage;                  // 0x10a8 (map image params)
    char pad_0x10a9[0x10d4 - 0x10a9];
    unsigned long m_nNoReturnStrRef;                // 0x10d4 (descriptive)
    unsigned long m_bPartySelect : 1;               // 0x10d8 bit 0 (descriptive)
    unsigned long m_bCanReturn : 1;                 //        bit 1 (descriptive)
};

// FUNCTION: SWKOTOR 0x00693650
// NONMATCH: register allocation (original: this edi, server ebp, loop
// index ebx); size, calls, EH states and control flow agree.
void CSWGuiInGameMap::OnPanelAdded()
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CExoString sAreaName(g_pAppManager->GetClientExoApp()->GetModule()->m_pArea->m_sName);
    m_cAreaText.SetText(sAreaName);
    CResRef cModule = pServer->GetModule()->m_cResRef;
    CExoString sResRef;
    CExoString sMapName;
    cModule.CopyToString(sResRef);
    sMapName.Format("lbl_map%s", sResRef.CStr());
    int bHasMap = 0;
    if (pServer->GetModule()->m_pAreaMap->m_nWidth > 0) {
        m_cMapImage.SetImage(sMapName, 0);
        bHasMap = 1;
    }
    else {
        m_cMapImage.SetImage("lbl_mapm28aa", 0);
    }
    m_cMapImage.SetB1(0);
    SetMapNoteText(m_cMapHider.InitializeMapNotes(bHasMap));
    CSWSArea* pArea = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId())->GetArea();
    if (pArea->m_bUnescapable) {
        m_nNoReturnStrRef = 0x9633;
        m_bPartySelect = 0;
    }
    else {
        m_nNoReturnStrRef = 0;
        m_bPartySelect = 1;
    }
    if (m_bPartySelect) {
        if (!g_pAppManager->GetClientExoApp()->GetParty()->IsPartyTogether()) {
            m_nNoReturnStrRef = 0x9634;
            m_bPartySelect = 0;
        }
        if (m_bPartySelect) {
            float fBest = 3.402823466e+38F;
            CSWSCreature* pLeader = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
                g_pAppManager->m_pServerExoApp->ClientToServerObjectID(
                    g_pAppManager->GetClientExoApp()->GetPlayerCreatureId()));
            int i;
            for (i = 0; i < g_pAppManager->GetClientExoApp()->GetParty()->m_nCount; i++) {
                CSWSCreature* pMember = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
                    g_pAppManager->m_pServerExoApp->ClientToServerObjectID(
                        g_pAppManager->GetClientExoApp()->GetParty()->m_aMembers[i].m_oidCreature));
                if (pMember && pMember->m_vPosition.x - pMember->GetSpotCheckDistance() < fBest) {
                    fBest = pMember->m_vPosition.x - pMember->GetSpotCheckDistance();
                }
            }
            CSWSArea* pLeaderArea = pLeader->GetArea();
            int nIndex;
            pLeaderArea->GetFirstObjectIndiceByX(&nIndex, fBest);
            while (m_bPartySelect && nIndex < pLeaderArea->m_nObjects) {
                CSWSCreature* pObject = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
                    pLeaderArea->m_pObjectIds[nIndex]);
                if (pObject) {
                    for (int j = 0; j < g_pAppManager->GetClientExoApp()->GetParty()->m_nCount; j++) {
                        CSWSCreature* pMember = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
                            g_pAppManager->m_pServerExoApp->ClientToServerObjectID(
                                g_pAppManager->GetClientExoApp()->GetParty()->m_aMembers[j].m_oidCreature));
                        if (pMember && pMember->GetReputation(pObject->m_idSelf, 0) <= 10) {
                            CSWSCreatureListEntry* pEntry = pObject->GetFromList920(pMember->m_idSelf);
                            if (pEntry && (pEntry->m_nFlags & 1)) {
                                m_bPartySelect = 0;
                                m_nNoReturnStrRef = 0x963e;
                            }
                        }
                    }
                }
                nIndex++;
            }
        }
    }
    m_bCanReturn = (m_bPartySelect && pServer->GetPartyTable()->m_bCanReturn) ? 1 : 0;
    m_cReturnText.SetStrRef(pServer->GetPartyTable()->m_nReturnStrRef);
    m_cReturnText.SetColor(m_bCanReturn ? g_vDifficultyExitColor : g_vGuiDisabledTextColor);
    m_vReturnBorderColor = m_bCanReturn ? g_vUpgradeAvailableColor : g_vGuiDisabledBorderColor;
    m_cPartyText.SetColor(m_bPartySelect ? g_vDifficultyExitColor : g_vGuiDisabledTextColor);
    m_vPartyBorderColor = m_bPartySelect ? g_vUpgradeAvailableColor : g_vGuiDisabledBorderColor;
    if (g_pAppManager->GetClientExoApp()->GetClientOptions()->m_nFlags & 0x100) {
        m_cPartyText.SetAlignment(0x12);
        m_cReturnText.SetAlignment(0x12);
    }
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0xd, 0x7f000000, 0x7f000000, 0);
}
