// CSWCCreature talk action-menu callback (worker c, s12). A real start missing
// from functions.csv (it sat inside PartyMenuUseItem's extent); the default
// action table stores it as the "i_dialog" handler. Name descriptive.

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ) {}
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
};

class CSWSObject {
public:
    void ClearAllActions(int bClearCombat);         // 0x004CCD80
    void SetOrientation(const Vector& vOrientation);    // 0x004CD170

    char pad_0x0[0xe8];
    int m_bCommandable;                             // 0xe8
    char pad_0xec[0x21c - 0xec];
    int m_n21C;                                     // 0x21c (placeholder name)
};

class CSWCCreature;

class CSWParty {
public:
    CSWCCreature* GetCharacter(int nIndex);         // 0x006346C0
};

class CGuiInGame {
public:
    int CanPerformAction();                         // 0x0062F9A0
    void SetGlobalDialogState(int bState);          // 0x0062EC60

    char pad_0x0[0xb4];
    int m_nB4;                                      // 0xb4
};

class CSWCMessage {
public:
    int SendPlayerToServer_06_08(unsigned long oidTarget);  // 0x00677EA0
};

class CClientExoApp {
public:
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    CSWCMessage* GetMessage();                      // 0x005ED6F0
    CSWParty* GetParty();                           // 0x005ED8B0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCObject {
public:
    virtual void v00();
    CSWSObject* GetServerObject();                  // 0x0063D4B0

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x24 - 0x8];
    Vector m_vPosition;                             // 0x24
};

class CSWCCreature : public CSWCObject {
public:
    void ActionMenuTalk(int nAction, CSWCCreature* pActor);

    char pad_0x30[0x138 - 0x30];
    int m_n138;                                     // 0x138
};

// FUNCTION: SWKOTOR 0x0060F620
void CSWCCreature::ActionMenuTalk(int nAction, CSWCCreature* pActor)
{
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->CanPerformAction()) {
        return;
    }
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_nB4) {
        return;
    }
    if (m_n138) {
        return;
    }
    if (!GetServerObject()) {
        return;
    }
    GetServerObject()->ClearAllActions(1);
    CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetParty()->GetCharacter(0);
    if (pPlayer && GetServerObject()->m_n21C && GetServerObject()->m_bCommandable) {
        Vector vPlayer = pPlayer->m_vPosition;
        Vector vSelf = m_vPosition;
        Vector vFacing = vPlayer - vSelf;
        if (GetServerObject()) {
            GetServerObject()->SetOrientation(vFacing);
        }
    }
    unsigned long oid = m_idSelf;
    g_pAppManager->m_pClientExoApp->GetMessage()->SendPlayerToServer_06_08(oid);
    g_pAppManager->m_pClientExoApp->GetInGameGui()->SetGlobalDialogState(1);
}
