// CSWSCreature combat leaves (b0058_f_lifetime_leaves).

struct CSWSAttackData {
    char pad_0x0[0x10];
    short m_nAttackDelay;      // +0x10
    char pad_0x12[0x2];
    short m_nReactionDelay;    // +0x14
};

struct CSWSCombatRoundAction {
    char pad_0x0[0x38];
    unsigned long m_oidTarget; // +0x38
};

class CSWSCombatRound {
public:
    void ClearAllSpecialAttacks();
    void UpdateAttackTargetForAllActions(unsigned long oidTarget);
    CSWSAttackData *GetAttack(int nAttack);
};

class CSWSItem {
public:
    int IsLightsaber();
};

class CSWInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation);   // +0x7c

    void SetLockOrientationToObject(unsigned long oidObject, int bLock);
    void SetCombatMode(unsigned char nMode, int bForce);

    void CancelAllCombatModes(int bUnused);
    void InitializeNewAttackTarget(unsigned long oidTarget, CSWSCombatRoundAction *pAction);
    void SetCombatReactionTime(int nAttack, short nReaction, short nDelay);
    int IsLightsaberEquipped();

    char pad_0x4[0x80];
    unsigned long m_oidAttackTarget;          // +0x84
    char pad_0x88[0x460];
    int m_bAttackTargetChanged;               // +0x4e8
    char pad_0x4ec[0x20];
    unsigned long m_oidLastAttackTarget;      // +0x50c
    char pad_0x510[0x4b8];
    CSWSCombatRound *m_pcCombatRound;         // +0x9c8
    char pad_0x9cc[0x60];
    CSWInventory *m_pInventory;               // +0xa2c
};

// FUNCTION: SWKOTOR 0x005B48B0
void CSWSCreature::CancelAllCombatModes(int bUnused)
{
    SetLockOrientationToObject(0x7f000000, 0);
    SetAnimation(10000);
    SetCombatMode(0, 1);
    m_pcCombatRound->ClearAllSpecialAttacks();
}

// FUNCTION: SWKOTOR 0x005B4970
void CSWSCreature::InitializeNewAttackTarget(unsigned long oidTarget, CSWSCombatRoundAction *pAction)
{
    if (pAction)
        pAction->m_oidTarget = oidTarget;
    SetLockOrientationToObject(oidTarget, 0);
    m_oidAttackTarget = oidTarget;
    m_oidLastAttackTarget = oidTarget;
    m_pcCombatRound->UpdateAttackTargetForAllActions(oidTarget);
    m_bAttackTargetChanged = 1;
}

// FUNCTION: SWKOTOR 0x005B5600
void CSWSCreature::SetCombatReactionTime(int nAttack, short nReaction, short nDelay)
{
    m_pcCombatRound->GetAttack(nAttack)->m_nReactionDelay = nReaction;
    m_pcCombatRound->GetAttack(nAttack)->m_nAttackDelay = nDelay;
}

// FUNCTION: SWKOTOR 0x005B5690
int CSWSCreature::IsLightsaberEquipped()
{
    int bEquipped = 0;
    CSWSItem *pItem = m_pInventory->GetItemInSlot(0x10);
    if (pItem && pItem->IsLightsaber())
        bEquipped = 1;
    pItem = m_pInventory->GetItemInSlot(0x20);
    if (pItem && pItem->IsLightsaber())
        bEquipped = 1;
    return bEquipped;
}

// ---------------------------------------------------------------------------
// CSWClass

class CSWRules {
public:
    int IsFeatUseable(unsigned short nFeat);
};
extern CSWRules *g_pRules;

struct CSWClassFeat {
    unsigned short m_nFeat;
    char pad_0x2[6];
};

class CSWClass {
public:
    int IsFeatUseable(unsigned short nFeat);

    char pad_0x0[0x16c];
    CSWClassFeat *m_pFeats;       // +0x16c
    unsigned short m_nNumFeats;   // +0x170
};

// FUNCTION: SWKOTOR 0x005BE370
int CSWClass::IsFeatUseable(unsigned short nFeat)
{
    if (!g_pRules->IsFeatUseable(nFeat))
        return 0;
    for (unsigned short i = 0; i < m_nNumFeats; i++) {
        if (m_pFeats[i].m_nFeat == nFeat)
            return 1;
    }
    return 0;
}
