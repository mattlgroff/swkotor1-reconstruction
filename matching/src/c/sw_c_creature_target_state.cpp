class CClientExoApp {
public:
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    void SetCombatMode(int bCombat);                // 0x005EDE80
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCCreature {
public:
    char pad_0x0[4];
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x438];
    unsigned long m_nUnknown440;                    // 0x440
    void SetAttackTarget(int b);
};

// FUNCTION: SWKOTOR 0x00610A10
void CSWCCreature::SetAttackTarget(int b)
{
    m_nUnknown440 = m_nUnknown440 ^ ((m_nUnknown440 ^ (unsigned long)b) & 1);
    if (m_idSelf != g_pAppManager->GetClientExoApp()->GetPlayerCreatureId()) return;
    g_pAppManager->GetClientExoApp()->SetCombatMode(m_nUnknown440 & 1);
}
