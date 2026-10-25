// CSWSCreatureStats talent queries (b005a_f2_dialog_stats).

class CSWSObject;
class CSWSpell;

class CSWSpellArray {
public:
    CSWSpell *GetSpell(int nSpell);
};

class CSWSkill {
public:
    char pad_0x0[0x1c];
    unsigned char m_nFlags;          // +0x1c
    char pad_0x1d[0x3];
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray *m_pSpellArray;    // +0x8c
    char pad_0x90[0x24];
    CSWSkill *m_pSkills;             // +0xb4
};

extern CSWRules *g_pRules;

int GetIsSpellCasterClass(int nClass);

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();           // +0x94

    int GetIsInCombatAnimation();
    int GetIsDying();

    char pad_0x4[0x9ec];
    unsigned char m_nStateFlags;     // +0x9f0
};

class CSWSCreatureStats {
public:
    int GetCreatureHasTalent(int nType, int nId);
    unsigned char GetClass(unsigned char nMultiClass);
    int HasFeat(unsigned short nFeat);
    unsigned char GetFeatRemainingUses(unsigned short nFeat);
    int GetSpellKnown(unsigned char nMultiClass, unsigned long nSpell, int bAny);
    unsigned char GetSpellUsesLeft(unsigned long nSpell, unsigned char nMultiClass, int a, int b);
    int GetHasSpellFromFeat(unsigned long nSpell);
    int GetHasPowerFromItem(int nPower);
    char GetSkillRank(unsigned char nSkill, CSWSObject *pVersus, int bBaseOnly);

    char pad_0x0[0x24];
    CSWSCreature *m_pBaseCreature;   // +0x24
    char pad_0x28[0x61];
    unsigned char m_nNumMultiClasses;  // +0x89
    int GetWeaponSpecialization(class CSWItem* pItem);
};

class CSWBaseItem {
public:
    char pad_0x0[0xb0];
    unsigned short m_nWeaponSpecialization;
};

class CSWItem {
public:
    virtual ~CSWItem();
    CSWBaseItem* GetBaseItem();
};

// FUNCTION: SWKOTOR 0x005AAA30
int CSWSCreatureStats::GetCreatureHasTalent(int nType, int nId)
{
    if (nType == -1 || nId == -1)
        return 0;
    if (((m_pBaseCreature->m_nStateFlags & 6) != 6 && m_pBaseCreature->GetDead()) ||
        m_pBaseCreature->GetIsInCombatAnimation() || m_pBaseCreature->GetIsDying()) {
        if (nType == 0)
            g_pRules->m_pSpellArray->GetSpell(nId);
        return 0;
    }
    if (nType == 0) {
        if (!g_pRules->m_pSpellArray->GetSpell(nId))
            return 0;
        int bFound = 0;
        for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
            if (GetIsSpellCasterClass(GetClass(i))) {
                if (GetSpellKnown(0, nId, 1) && GetSpellUsesLeft(nId, i, 0, 0))
                    bFound = 1;
            } else if (GetHasSpellFromFeat(nId) ||
                       (GetSpellKnown(0, nId, 1) && GetSpellUsesLeft(nId, i, 0, 0)) ||
                       GetHasPowerFromItem(nId)) {
                bFound = 1;
            }
        }
        if (bFound && (m_pBaseCreature->m_nStateFlags & 8))
            return 1;
        return 0;
    }
    if (nType == 1) {
        if (HasFeat(nId) && GetFeatRemainingUses(nId))
            return 1;
        return 0;
    }
    if (nType == 2) {
        if (GetSkillRank(nId, 0, 0) > 0)
            return 1;
        if (!(g_pRules->m_pSkills[(unsigned short)nId].m_nFlags & 2))
            return 0;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A67D0
int CSWSCreatureStats::GetWeaponSpecialization(CSWItem* pItem)
{
    if (pItem != 0) {
        if (pItem->GetBaseItem() != 0) {
            CSWBaseItem* pBaseItem = pItem->GetBaseItem();
            union {
                unsigned long nFull;
                unsigned short nWord;
            } nFeat;
            nFeat.nFull = pBaseItem->m_nWeaponSpecialization;
            if (nFeat.nWord != 0)
                return HasFeat(nFeat.nWord);
        }
    }
    return 0;
}
