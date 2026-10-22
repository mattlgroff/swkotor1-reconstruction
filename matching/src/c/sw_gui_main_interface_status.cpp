// CSWGuiMainInterfaceStatus (b0065 client virtual leaves).

class CGuiInGame {
public:
    void ShowTutorial(int n, int b);                // 0x0062EEB0
};

class CClientExoApp {
public:
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
};

class CAppManager {
public:
    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiMainInterfaceStatus {
public:
    void Update(float fDelta);

    char pad_0x0[0x44];
    unsigned long m_nFlags;                         // 0x44
    char pad_0x48[0xf8];
    float m_fTimeLeft;                              // 0x140
    unsigned long m_nStatusFlags;                   // 0x144
    unsigned char m_bShowTutorial;                  // 0x148
};

// FUNCTION: SWKOTOR 0x00686A20
void CSWGuiMainInterfaceStatus::Update(float fDelta)
{
    if (m_nFlags & 2) {
        if ((m_nStatusFlags & 1) && fDelta > 0.0f) {
            if (m_bShowTutorial) {
                g_pAppManager->GetClientExoApp()->GetGuiInGame()->ShowTutorial(10, 0);
            }
            m_nStatusFlags &= ~1;
        }
        m_fTimeLeft -= fDelta;
        if (m_fTimeLeft <= 0.0f) {
            m_nFlags &= ~2;
        }
    }
}
