// CSWSCreatureStats skill rank / talents (b005a_f2_dialog_stats).

class CSWSObject;

class CSWSArea {
public:
    char pad_0x0[0x43c];
    int m_nStealthBonus;             // +0x43c
};

class CServerExoApp {
public:
    CSWSArea *GetAreaByGameObjectID(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSkill {
public:
    char pad_0x0[0x18];
    int m_nKeyAbility;               // +0x18
    unsigned char m_nFlags;          // +0x1c
    char pad_0x1d[0x3];
};

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;      // +0xab
    char pad_0xac[0x8];
    CSWSkill *m_pSkills;             // +0xb4
};

extern CSWRules *g_pRules;

extern const int ABILITY_STRENGTH;
extern const int ABILITY_DEXTERITY;
extern const int ABILITY_CONSTITUTION;
extern const int ABILITY_INTELLIGENCE;
extern const int ABILITY_WISDOM;
extern const int ABILITY_CHARISMA;

class CSWSCreature {
public:
    int GetTotalEffectBonus(unsigned char nType, CSWSObject *pVersus, int *pnOut, int a, int b,
                            int c, int d, unsigned char nSkill, unsigned char nSave, int e);
    int GetArmorCheckPenalty();
    int GetMaxDexCapped();
    char pad_0x0[0x54];
    unsigned long m_oidArea;         // +0x54
};

class CSWSCreatureStats {
public:
    char GetSkillRank(unsigned char nSkill, CSWSObject *pVersus, int bBaseOnly);
    int HasFeat(unsigned short nFeat);
    __forceinline int GetDEXMod()
    {
        if (m_pBaseCreature->GetMaxDexCapped()) {
            int nMod = m_nDexMod;
            return nMod >= 0 ? 0 : nMod;
        }
        return m_nDexMod;
    }

    char pad_0x0[0x24];
    CSWSCreature *m_pBaseCreature;   // +0x24
    char pad_0x28[0xc2];
    char m_nStrMod;                  // +0xea
    char pad_0xeb;
    char m_nDexMod;                  // +0xec
    char pad_0xed;
    char m_nConMod;                  // +0xee
    char pad_0xef;
    char m_nIntMod;                  // +0xf0
    char pad_0xf1;
    char m_nWisMod;                  // +0xf2
    char pad_0xf3;
    char m_nChaMod;                  // +0xf4
    char pad_0xf5[0x73];
    char *m_pSkillRanks;             // +0x168
    char m_nArmorSkillPenalty;       // +0x16c
    char m_nShieldSkillPenalty;      // +0x16d
};

// NONMATCH (95, all one register swap): the original keeps the rank in edi
// and skill*0x20 in ebx (ours swapped), and folds `add dl,[esi+0x16c]` for the
// armour/shield penalty sum (ours loads both bytes).
// FUNCTION: SWKOTOR 0x005AA570
char CSWSCreatureStats::GetSkillRank(unsigned char nSkill, CSWSObject *pVersus, int bBaseOnly)
{
    if (nSkill >= g_pRules->m_nNumSkills)
        return 0;
    int nRank = m_pSkillRanks[nSkill];
    if (!bBaseOnly) {
        if (!(g_pRules->m_pSkills[nSkill].m_nFlags & 2) && nRank == 0)
            return 0;
        int nExtra = 0;
        nRank += m_pBaseCreature->GetTotalEffectBonus(5, pVersus, &nExtra, 0, 0, 0, 0, nSkill, 0xff, 0);
        int nAbility = g_pRules->m_pSkills[nSkill].m_nKeyAbility;
        if (nAbility == ABILITY_STRENGTH) {
            nRank += m_nStrMod;
            if (m_pBaseCreature->GetArmorCheckPenalty())
                nRank -= 4;
        } else if (nAbility == ABILITY_DEXTERITY) {
            nRank += (char)GetDEXMod();
            if (m_pBaseCreature->GetArmorCheckPenalty())
                nRank -= 4;
        } else if (nAbility == ABILITY_CONSTITUTION) {
            nRank += m_nConMod;
        } else if (nAbility == ABILITY_INTELLIGENCE) {
            nRank += m_nIntMod;
        } else if (nAbility == ABILITY_WISDOM) {
            nRank += m_nWisMod;
        } else if (nAbility == ABILITY_CHARISMA) {
            nRank += m_nChaMod;
        }
        if (g_pRules->m_pSkills[nSkill].m_nFlags & 4)
        {
            char nPenalty = m_nShieldSkillPenalty;
            nPenalty += m_nArmorSkillPenalty;
            nRank += nPenalty;
        }
        unsigned long oidArea = m_pBaseCreature->m_oidArea;
        CSWSArea *pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
        switch (nSkill) {
        case 0:
            if (HasFeat(0x78))
                nRank += 3;
            else if (HasFeat(0x77))
                nRank += 2;
            else if (HasFeat(0xc))
                nRank += 1;
            if (pArea)
                nRank += pArea->m_nStealthBonus;
            break;
        case 1:
        case 2:
            if (HasFeat(0x76))
                nRank += 3;
            else if (HasFeat(0x75))
                nRank += 2;
            else if (HasFeat(7))
                nRank += 1;
            break;
        case 5:
        case 6:
            if (HasFeat(0x78))
                nRank += 3;
            else if (HasFeat(0x77))
                nRank += 2;
            else if (HasFeat(0xc))
                nRank += 1;
            break;
        case 3:
        case 4:
        case 7:
            if (HasFeat(0x7a))
                nRank += 3;
            else if (HasFeat(0x79))
                nRank += 2;
            else if (HasFeat(0xa))
                nRank += 1;
            break;
        }
    }
    if (nRank >= 0x7f)
        return 0x7f;
    if (nRank <= -0x7f)
        return -0x7f;
    return nRank;
}
