// CSWSCreatureStats leaves (b005a_f2_dialog_stats).

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *CStr() const;
    void Format(const char *fmt, ...);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    ~CExoLocString();
    void *m_pInternal;
    unsigned long m_n4;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void DelIndex(int i)
    {
        num = num - 1;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
};

class CGameEffect {
public:
    ~CGameEffect();
};

class CSWSStatsBuffer {
public:
    ~CSWSStatsBuffer() { delete[] m_pData; m_pData = 0; }
    unsigned char *m_pData;
};

class CSWSCombatData {
public:
    ~CSWSCombatData();
};

class CResGFFStruct {
public:
    unsigned long m_nStructIndex;
};

class CResList {
public:
    char m_data[0x14];
};

class CResGFF {
public:
    int GetList(CResList *pList, CResGFFStruct *pStruct, char *szLabel);
    unsigned long GetListCount(CResList *pList);
    int GetListElement(CResGFFStruct *pStruct, CResList *pList, unsigned long n);
    unsigned short ReadFieldWORD(CResGFFStruct *pStruct, char *szLabel, int *bSuccess,
                                 unsigned short nDefault);
};

class CSWModuleInfo {
public:
    char pad_0x0[0x94];
    int m_nMaxLevel;                 // +0x94
};

class CServerExoApp {
public:
    CSWModuleInfo *GetModuleInfo();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWBaseItemArray;

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray *m_pBaseItemArray;     // +0x34
    CSWBaseItemArray *GetBaseItemArray() { return m_pBaseItemArray; }
    unsigned long m_anExperienceTable[50];  // +0x38
};

extern CSWRules *g_pRules;

class CSWSCreature {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual int GetIsPossessed();    // +0x94
    int GetIsInConversation();
    int GetIsDroid();
    int GetDamageBonus(int nType, int b1, int *pnExtra, int b2, int b3, int b4, int b5,
                       int n6, int n7, int n8);

    char pad_0x4[0x4f4];
    int m_nCreatureSize;             // +0x4f8
};

class CSWBaseItem {
public:
    char pad_0x0[0x1c];
    unsigned char m_nNumDice;        // +0x1c
    unsigned char m_nDieToRoll;      // +0x1d
};

class CSWBaseItemArray {
public:
    CSWBaseItem *GetBaseItem(int nBaseItem);
};

class CSWItemProps {
public:
    char pad_0x0[0x1a];
    unsigned char m_nDamageFlags;    // +0x1a
};

class CSWSObject;

class CSWSItem {
public:
    CSWItemProps *GetItemProps();
    char pad_0x0[0xc];
    int m_nBaseItem;                 // +0xc
    int GetBaseItem() { return m_nBaseItem; }
};

class CSWSCreatureStats_ClassInfo {
public:
    ~CSWSCreatureStats_ClassInfo();
    char pad_0x0[0x19];
    unsigned char m_nSpellsLeft;     // +0x19
    unsigned char m_nSpellsPerDay;   // +0x1a
    unsigned char m_nClass;          // +0x1b
    char pad_0x1c[0xc];
};

int GetIsSpellCasterClass(int nClass);

class CSWSCreatureStats {
public:
    ~CSWSCreatureStats();
    void ClearFeats();
    void AddKnownSpell(unsigned char nMultiClass, unsigned long nSpell);
    void ReadSpellsFromGff(CResGFF *pGFF, CResGFFStruct *pStruct);
    unsigned char GetLevel(int bFalse);
    unsigned char ComputeSpellsPerDay(unsigned char nMultiClass, int b);
    unsigned char GetClass(unsigned char nMultiClass)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0xff;
        return m_ClassInfo[nMultiClass].m_nClass;
    }
    int CanLevelUp();
    void AdjustSpellUsesPerDay();
    int GetWeaponFocus(CSWSItem *pWeapon);
    void GetTotalDamageRange(CSWSObject *pTarget, CSWSItem *pWeapon, int *pMin, int *pMax,
                             int nArg, int bEffects);

    CExoArrayList<unsigned short> m_lFeats;        // +0x0
    CExoArrayList<unsigned short> m_lBonusFeats;   // +0xc
    CExoArrayList<unsigned short> m_lFeatUses;     // +0x18
    CSWSCreature *m_pBaseCreature;                 // +0x24
    CExoArrayList<CGameEffect *> m_lEffects;       // +0x28
    CExoLocString m_sFirstName;                    // +0x34
    CExoLocString m_sLastName;                     // +0x3c
    char pad_0x44[0x14];
    CExoLocString m_sDescription;                  // +0x58
    char pad_0x60[0x8];
    unsigned long m_nExperience;     // +0x68
    char pad_0x6c[0x1d];
    unsigned char m_nNumMultiClasses;  // +0x89
    char pad_0x8a[0x2];
    CSWSCreatureStats_ClassInfo m_ClassInfo[2];  // +0x8c
    char pad_0xdc[0x4];
    CExoString m_sSubRace;           // +0xe0
    char pad_0xe8[0x2];
    char m_nDamageBonus;             // +0xea
    char pad_0xeb[0x2d];
    CSWSCombatData *m_pCombatData;   // +0x118
    char pad_0x11c[0xc];
    CSWSStatsBuffer *m_pBuffer;      // +0x128
    char pad_0x12c[0x3c];
    unsigned char *m_p168;           // +0x168
    char pad_0x16c[0x38];
    CExoString m_sDeity;             // +0x1a4
};

// FUNCTION: SWKOTOR 0x005A6810
int CSWSCreatureStats::CanLevelUp()
{
    CServerExoApp *pApp = g_pAppManager->GetServerExoApp();
    if (GetLevel(0) < pApp->GetModuleInfo()->m_nMaxLevel &&
        m_nExperience >= g_pRules->m_anExperienceTable[GetLevel(0)] &&
        !m_pBaseCreature->GetIsPossessed() && !m_pBaseCreature->GetIsInConversation())
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005A6D80
void CSWSCreatureStats::AdjustSpellUsesPerDay()
{
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        if (GetIsSpellCasterClass(GetClass(i))) {
            unsigned char nOld = m_ClassInfo[i].m_nSpellsPerDay;
            unsigned char nNew = ComputeSpellsPerDay(i, 0);
            char nDiff = nNew - nOld;
            m_ClassInfo[i].m_nSpellsPerDay = nNew;
            if (nDiff < 0) {
                if (nDiff < m_ClassInfo[i].m_nSpellsLeft)
                    m_ClassInfo[i].m_nSpellsLeft += nDiff;
                else
                    m_ClassInfo[i].m_nSpellsLeft = 0;
            }
        }
    }
}

// NONMATCH (25): original keeps `this` in ebp and stores the creature size
// into the dead pWeapon arg slot before the first GetIsDroid call (dead store
// we cannot reproduce); also loads m_pBaseCreature into ebp before the pushes.
// FUNCTION: SWKOTOR 0x005A9E10
void CSWSCreatureStats::GetTotalDamageRange(CSWSObject *pTarget, CSWSItem *pWeapon, int *pMin,
                                            int *pMax, int nArg, int bEffects)
{
    int nMin, nMax;
    if (pTarget && pWeapon) {
        nMin = g_pRules->m_pBaseItemArray->GetBaseItem(pWeapon->GetBaseItem())->m_nNumDice;
        nMax = g_pRules->m_pBaseItemArray->GetBaseItem(pWeapon->GetBaseItem())->m_nDieToRoll * nMin;
        if (pWeapon->GetItemProps()->m_nDamageFlags <= 0) {
            nMin += m_nDamageBonus;
            nMax += m_nDamageBonus;
        }
        if (GetWeaponFocus(pWeapon)) {
            nMin += 2;
            nMax += 2;
        }
    } else {
        *(int *)&pWeapon = m_pBaseCreature->m_nCreatureSize;
        m_pBaseCreature->GetIsDroid();
        int nSize = m_pBaseCreature->m_nCreatureSize;
        nMin = 1;
        nMax = (unsigned char)(m_pBaseCreature->GetIsDroid() == 1 ? (nSize > 2) * 2 + 4
                                                                  : (nSize <= 2) + 1);
        if (pTarget) {
            nMin = m_nDamageBonus + 1;
            nMax += m_nDamageBonus;
        }
    }
    if (bEffects) {
        int nExtra = 0;
        int nBonus = m_pBaseCreature->GetDamageBonus(2, 0, &nExtra, 0, 1, 0, 0, 0xff, 0xff, nArg);
        nMin += nBonus + nExtra;
        nMax += nBonus;
    }
    *pMin = nMin;
    *pMax = nMax;
    if (*pMin < 1)
        *pMin = 1;
    if (*pMax < 1)
        *pMax = 1;
}

// FUNCTION: SWKOTOR 0x005ACF40
CSWSCreatureStats::~CSWSCreatureStats()
{
    if (m_p168) {
        delete[] m_p168;
        m_p168 = 0;
    }
    ClearFeats();
    int nEffects = m_lEffects.num;
    for (int i = nEffects - 1; i >= 0; i--) {
        CGameEffect *pEffect = m_lEffects.element[i];
        m_lEffects.DelIndex(i);
        if (pEffect)
            delete pEffect;
    }
    if (m_pBuffer) {
        delete m_pBuffer;
        m_pBuffer = 0;
    }
    if (m_pCombatData) {
        delete m_pCombatData;
        m_pCombatData = 0;
    }
}

// FUNCTION: SWKOTOR 0x005AEB30
void CSWSCreatureStats::ReadSpellsFromGff(CResGFF *pGFF, CResGFFStruct *pStruct)
{
    CResList cClassList;
    if (pGFF->GetList(&cClassList, pStruct, "ClassList")) {
        for (int i = 0; i < m_nNumMultiClasses; i++) {
            CResGFFStruct cClass;
            if (pGFF->GetListElement(&cClass, &cClassList, i)) {
                CExoString sLabel;
                unsigned short n = 0;
                sLabel.Format("%s%d", "KnownList", 0);
                CResList cSpellList;
                if (pGFF->GetList(&cSpellList, &cClass, sLabel.CStr())) {
                    for (n = 0; n < pGFF->GetListCount(&cSpellList); n++) {
                        CResGFFStruct cSpell;
                        int bSuccess;
                        if (pGFF->GetListElement(&cSpell, &cSpellList, n)) {
                            int nSpell = pGFF->ReadFieldWORD(&cSpell, "Spell", &bSuccess, 0xffff);
                            if (nSpell != -1)
                                AddKnownSpell(i, nSpell);
                        }
                    }
                }
            }
        }
    }
}
