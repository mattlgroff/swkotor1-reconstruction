// CSWClass per-level table getters and feat / skill table lookups
// (0x005BCCB0-0x005BE770).

class CSWSkill {
public:
    char pad_0x0[0x1c];
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

struct CSWClassSkill {
    unsigned short m_nSkill;         // +0x0
    char pad_0x2[0x2];
    int m_nValue;                    // +0x4
    int m_nLevel;                    // +0x8
};

struct CSWClassFeat {
    unsigned short m_nFeat;          // +0x0
    unsigned char m_nLevel;          // +0x2
    unsigned char m_nFlags;          // +0x3
    int m_nGrantedLevel;             // +0x4
};

#pragma pack(push, 1)
class CSWClass {
public:
    unsigned char GetAttackBonus(unsigned char nLevel);
    unsigned char GetFortSaveBonus(unsigned char nLevel);
    unsigned char GetWillSaveBonus(unsigned char nLevel);
    unsigned char GetReflexSaveBonus(unsigned char nLevel);
    unsigned char GetSpellsPerDayLevel(unsigned char nLevel, unsigned char nSpellLevel);
    unsigned char GetSpellGain(unsigned char nLevel, unsigned char nSpellLevel, int a3, int a4,
                               int a5);
    int GetClassSkillValue(unsigned short nSkill);
    int IsClassSkill(unsigned short nSkill);
    unsigned char GetFeatGainLevel(unsigned short nFeat);
    int GetFeatFlag1(unsigned short nFeat);
    int GetFeatFlag0(unsigned short nFeat);
    int GetFeatLevel(unsigned short nFeat, unsigned char *pnLevel);
    int GetGrantedFeat(int nLevel);
    int GetGrantedSkill(int nLevel);
    static int IsForceUserClass(int nClass);
    unsigned char GetArmorBonus(int nLevel);

    char pad_0x0[0x18];
    unsigned char m_lstAttackBonus[60];   // +0x18
    char pad_0x54[0x3];
    unsigned char m_lstFortSave[60];      // +0x57
    unsigned char m_lstRefSave[60];       // +0x93
    unsigned char m_lstWillSave[60];      // +0xcf
    char pad_0x10b;
    unsigned char *m_pArmorBonus;         // +0x10c
    unsigned char **m_ppSpellGain;        // +0x110
    unsigned char m_anNumSpellLevels[20]; // +0x114
    unsigned char m_anSpellsPerDay[20];   // +0x128
    char pad_0x13c[0x28];
    CSWClassSkill *m_pSkills;             // +0x164
    unsigned short m_nNumSkills;          // +0x168
    char pad_0x16a[0x2];
    CSWClassFeat *m_pFeats;               // +0x16c
    unsigned short m_nNumFeats;           // +0x170
};
#pragma pack(pop)

// FUNCTION: SWKOTOR 0x005BCCB0
unsigned char CSWClass::GetAttackBonus(unsigned char nLevel)
{
    if (nLevel >= 1 && nLevel <= 60)
        return m_lstAttackBonus[nLevel - 1];
    return 0;
}

// FUNCTION: SWKOTOR 0x005BCCD0
unsigned char CSWClass::GetFortSaveBonus(unsigned char nLevel)
{
    if (nLevel >= 1 && nLevel <= 60)
        return m_lstFortSave[nLevel - 1];
    return 0;
}

// FUNCTION: SWKOTOR 0x005BCCF0
unsigned char CSWClass::GetWillSaveBonus(unsigned char nLevel)
{
    if (nLevel >= 1 && nLevel <= 60)
        return m_lstWillSave[nLevel - 1];
    return 0;
}

// FUNCTION: SWKOTOR 0x005BCD10
unsigned char CSWClass::GetReflexSaveBonus(unsigned char nLevel)
{
    if (nLevel >= 1 && nLevel <= 60)
        return m_lstRefSave[nLevel - 1];
    return 0;
}

// FUNCTION: SWKOTOR 0x005BCD30
unsigned char CSWClass::GetSpellsPerDayLevel(unsigned char nLevel, unsigned char nSpellLevel)
{
    if (nLevel >= 1 && nLevel <= 20 && nSpellLevel < m_anNumSpellLevels[nLevel - 1])
        return m_anSpellsPerDay[nLevel - 1];
    return 0xff;
}

// FUNCTION: SWKOTOR 0x005BCD60
unsigned char CSWClass::GetSpellGain(unsigned char nLevel, unsigned char nSpellLevel, int a3,
                                     int a4, int a5)
{
    if (nLevel >= 1 && nLevel <= 20 && nSpellLevel < m_anNumSpellLevels[nLevel - 1] &&
        m_ppSpellGain)
        return m_ppSpellGain[nLevel - 1][nSpellLevel];
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE010
int CSWClass::GetClassSkillValue(unsigned short nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills)
        return 0;
    for (unsigned short i = 0; i < m_nNumSkills; i++) {
        if (m_pSkills[i].m_nSkill == nSkill)
            return m_pSkills[i].m_nValue;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE070
int CSWClass::IsClassSkill(unsigned short nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills)
        return 0;
    CSWSkill *pSkill = &g_pRules->m_pSkills[nSkill];
    if (!pSkill)
        return 0;
    if (pSkill->m_nFlags & 8)
        return 1;
    for (unsigned short i = 0; i < m_nNumSkills; i++) {
        if (m_pSkills[i].m_nSkill == nSkill)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE220
unsigned char CSWClass::GetFeatGainLevel(unsigned short nFeat)
{
    for (unsigned short i = 0; i < m_nNumFeats; i++) {
        if (m_pFeats[i].m_nFeat == nFeat && m_pFeats[i].m_nFlags == 0)
            return m_pFeats[i].m_nLevel;
    }
    return 0xff;
}

// FUNCTION: SWKOTOR 0x005BE270
int CSWClass::GetFeatFlag1(unsigned short nFeat)
{
    for (unsigned short i = 0; i < m_nNumFeats; i++) {
        if (m_pFeats[i].m_nFeat == nFeat)
            return (m_pFeats[i].m_nFlags >> 1) & 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE2C0
int CSWClass::GetFeatFlag0(unsigned short nFeat)
{
    for (unsigned short i = 0; i < m_nNumFeats; i++) {
        if (m_pFeats[i].m_nFeat == nFeat)
            return m_pFeats[i].m_nFlags & 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE310
int CSWClass::GetFeatLevel(unsigned short nFeat, unsigned char *pnLevel)
{
    *pnLevel = 0xff;
    for (unsigned short i = 0; i < m_nNumFeats; i++) {
        if (m_pFeats[i].m_nFeat == nFeat) {
            if (m_pFeats[i].m_nFlags)
                return 0;
            *pnLevel = m_pFeats[i].m_nLevel;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE3C0
int CSWClass::GetGrantedFeat(int nLevel)
{
    int nResult = -1;
    int nCount = -1;
    for (unsigned short i = 0; i < m_nNumFeats; i++) {
        if (m_pFeats[i].m_nGrantedLevel == nLevel)
            return m_pFeats[i].m_nFeat;
        if (m_pFeats[i].m_nGrantedLevel == 0 && nCount != nLevel) {
            nCount++;
            nResult = m_pFeats[i].m_nFeat;
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005BE430
int CSWClass::GetGrantedSkill(int nLevel)
{
    int nResult = -1;
    int nCount = -1;
    for (unsigned short i = 0; i < m_nNumSkills; i++) {
        if (m_pSkills[i].m_nLevel == nLevel)
            return m_pSkills[i].m_nSkill;
        if (m_pSkills[i].m_nLevel == 0 && nCount != nLevel) {
            nCount++;
            nResult = m_pSkills[i].m_nSkill;
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005BE4A0
int CSWClass::IsForceUserClass(int nClass)
{
    if (nClass == 3 || nClass == 4 || nClass == 5)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE770
unsigned char CSWClass::GetArmorBonus(int nLevel)
{
    return m_pArmorBonus[nLevel];
}
