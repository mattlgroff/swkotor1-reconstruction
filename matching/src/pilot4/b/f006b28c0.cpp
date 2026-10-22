// Pilot 4 arm B: 0x006B28C0, GUI character panel refresh (portrait, level, HP).

class CExoString {
public:
    CExoString();                               // 0x005B3190
    CExoString(int n);                          // 0x005E5BC0
    ~CExoString();                              // 0x005E5C20
    void Format(const char *format, ...);       // 0x005E5680
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
};

class CSWCCreatureStats {
public:
    short GetLevel(int n);                      // 0x00647720
    short GetMaxHitPoints();                    // 0x00647A80
    char pad_0x0[0x4c];
    short m_nCurrentHitPoints;                  // 0x4c
};

class CSWCCreature {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00C();
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01C();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03C();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04C();
    virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05C();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06C();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07C();
    virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08C();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09C();
    virtual void v0A0(); virtual void v0A4(); virtual void v0A8(); virtual void v0AC();
    virtual void v0B0(); virtual void v0B4(); virtual void v0B8(); virtual void v0BC();
    virtual void v0C0(); virtual void v0C4(); virtual void v0C8(); virtual void v0CC();
    virtual void v0D0(); virtual void v0D4(); virtual void v0D8(); virtual void v0DC();
    virtual void v0E0(); virtual void v0E4(); virtual void v0E8(); virtual void v0EC();
    virtual void v0F0(); virtual void v0F4(); virtual void v0F8(); virtual void v0FC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13C();
    virtual void v140(); virtual void v144();
    virtual CResRef GetPortrait(int bLarge);    // 0x148

    char pad_0x4[0x2f4];
    CSWCCreatureStats *m_pStats;                // 0x2f8
};

class CSWParty {
public:
    CSWCCreature *GetPC();                      // 0x00635460
};

class CClientExoApp {
public:
    CSWParty *GetParty();                       // 0x005ED8B0
    CSWCCreature *GetCreatureByGameObjectID(unsigned long oid);  // 0x005ED5B0
    unsigned long ServerToClientObjectId(unsigned long oid);     // 0x005EDA50
};

class CSWPartyTable {
public:
    unsigned long GetNPCObject(int nNPC, int a, int b);  // 0x00564700
};

struct CServerInfo {
    char pad_0x0[0x104];
    unsigned char m_nFlags;                     // 0x104
};

class CServerExoApp {
public:
    CServerInfo *GetServerInfo();               // 0x004AEC90
    CSWPartyTable *GetPartyTable();             // 0x004AEE70
};

class CAppManager {
public:
    void *m_pUnknown00;
    CClientExoApp *m_pClientExoApp;             // 0x04
    CServerExoApp *m_pServerExoApp;             // 0x08
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;              // 0x007A39FC

class CSWGuiBorderParams {
public:
    void SetFillImage(const CResRef &cResRef, int nArg);  // 0x00414C00
    void SetFillImage(const CResRef &cResRef) { SetFillImage(cResRef, 0); }
};

class CSWGuiTextParams {
public:
    void SetText(const CExoString &s);          // 0x00415E00
};

class CSWGuiLabel {
public:
    virtual void v00();
    virtual void v04();
    virtual int GetTextHeight();                // +0x08
    int GetFontHeight();                        // 0x00414EE0
    char pad_0x4[0x14];
    CSWGuiTextParams m_text;                    // +0x18
};

class CSWGuiCharacterPanel {
public:
    void UpdateCharacter();
    char pad_0x0[0xe14];
    CSWGuiBorderParams m_portraitBorder;        // 0xe14
    char pad_0xe15[0x19f];
    CSWGuiLabel m_lblHitPoints;                 // 0xfb4 (text 0xfcc)
    char pad_0xfd0[0x13c];
    CSWGuiTextParams m_lblLevelText;            // 0x110c
    char pad_0x110d[0xcd3];
    signed char m_nNPC;                         // 0x1de0
};

// FUNCTION: SWKOTOR 0x006B28C0
void CSWGuiCharacterPanel::UpdateCharacter()
{
    CSWCCreature *pCreature;
    if (m_nNPC != -1 && (g_pAppManager->GetServerExoApp()->GetServerInfo()->m_nFlags & 1)) {
        pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(
            g_pAppManager->GetClientExoApp()->ServerToClientObjectId(
            g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCObject(m_nNPC, 0, 1)));
    }
    else {
        pCreature = g_pAppManager->GetClientExoApp()->GetParty()->GetPC();
    }
    if (pCreature) {
        m_portraitBorder.SetFillImage(pCreature->GetPortrait(1));
        {
            CExoString sLevel(pCreature->m_pStats->GetLevel(0));
            m_lblLevelText.SetText(sLevel);
        }
        CExoString sHP;
        short nCurrent = pCreature->m_pStats->m_nCurrentHitPoints;
        sHP.Format("%d/%d", nCurrent, pCreature->m_pStats->GetMaxHitPoints());
        m_lblHitPoints.m_text.SetText(sHP);
        if (m_lblHitPoints.GetTextHeight() > m_lblHitPoints.GetFontHeight()) {
            short nCurrent2 = pCreature->m_pStats->m_nCurrentHitPoints;
            sHP.Format("%d/ %d", nCurrent2, pCreature->m_pStats->GetMaxHitPoints());
            m_lblHitPoints.m_text.SetText(sHP);
        }
    }
}
