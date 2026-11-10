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
    unsigned long m_bCombatMode : 1;                // 0x440 bit 0 (descriptive)
    unsigned long m_nFlags440 : 31;
    void SetAttackTarget(int b);
};

// Name kept from the callers' records; it stores the creature's combat-mode
// bit and forwards it to the client app for the player creature.
// FUNCTION: SWKOTOR 0x00610A10
void CSWCCreature::SetAttackTarget(int b)
{
    m_bCombatMode = b;
    unsigned long oidSelf = m_idSelf;
    if (oidSelf == g_pAppManager->GetClientExoApp()->GetPlayerCreatureId()) {
        g_pAppManager->GetClientExoApp()->SetCombatMode(m_bCombatMode);
    }
}
