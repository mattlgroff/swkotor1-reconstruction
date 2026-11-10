// CSWCCreature::SetCombatState (nwccreature.o), worker C s11: the client
// creature's combat flag. Entering or leaving combat powers the equipped
// right/left hand items (slots 0x10/0x20) up or down, and entering combat
// clears the look-at target. Name from the server caller (0x004F2610); the
// Mac map's AddFireForgetAnimation name for this address is a neighbour slip.

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90();
    virtual int GetIsDead();                        // +0x94
    int GetDeadTemp();                              // 0x004EF890
};

class CSWCItem {
public:
    void SetPowered(int bPowered, int a2, int a3, int a4); // 0x00647130
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();          // +0x30
    virtual CSWCItem* AsSWCItem();                  // +0x34
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
};

class CAppManager {
public:
    void* m_pServerExoApp;                          // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CAnimationBase;

class CSWCCreature {
public:
    void SetCombatState(int bCombat);
    unsigned long GetEquippedItemID(unsigned long nSlot); // 0x0060CA80
    CGameObject* GetServerObject();                 // 0x0063D4B0
    CAnimationBase* GetAnimationBase();             // 0x0063C070
    void LookAt(unsigned long oidTarget, float fRange); // 0x006146E0

    char pad_0x0[0x138];
    int m_bCombatState;                             // 0x138
    char pad_0x13c[0x4];
    int m_bLookAtLocked;                            // 0x140 (blocks LookAt)
};

// FUNCTION: SWKOTOR 0x006182A0
void CSWCCreature::SetCombatState(int bCombat)
{
    int bOldCombat = m_bCombatState;
    m_bCombatState = bCombat;
    if (bCombat) {
        GetAnimationBase();
    }
    if (bOldCombat != bCombat && GetServerObject()) {
        CSWSCreature* pServer = GetServerObject()->AsSWSCreature();
        if (pServer && !pServer->GetIsDead() && !pServer->GetDeadTemp()) {
            unsigned long oidItem = GetEquippedItemID(0x10);
            if (oidItem != 0x7f000000) {
                CGameObject* pItem = g_pAppManager->GetClientExoApp()->GetGameObject(oidItem);
                if (pItem && pItem->AsSWCItem()) {
                    pItem->AsSWCItem()->SetPowered(bCombat, 0, 1, 0);
                }
            }
            oidItem = GetEquippedItemID(0x20);
            if (oidItem != 0x7f000000) {
                CGameObject* pItem = g_pAppManager->GetClientExoApp()->GetGameObject(oidItem);
                if (pItem && pItem->AsSWCItem()) {
                    pItem->AsSWCItem()->SetPowered(bCombat, 0, 1, 0);
                }
            }
        }
    }
    if (bCombat) {
        LookAt(0x7f000000, 0.0f);
        m_bLookAtLocked = 1;
    } else {
        m_bLookAtLocked = 0;
    }
}
