// CSWSCreatureStats random talent pickers (0x005AB9E0-0x005B23A0).
// Names other than GetCreatureTalent / GetCreatureTalentRandomPowerForClass
// are descriptive (NWN-style GetCreatureTalentRandom* family).

extern "C" int __cdecl rand(void);

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int nSize);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

class CExoString {
public:
    CExoString(const char *s);
    ~CExoString();
    char *CStr() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
};

class C2DArrays {
public:
    char pad_0x0[0xf4];
    C2DA *m_pSpellsTable;            // +0xf4 (placeholder name)
    C2DA *m_pFeatTable;              // +0xf8 (placeholder name)
    C2DA *GetFeatTable() { return m_pFeatTable; }
    C2DA *GetSpellsTable() { return m_pSpellsTable; }
};

class CSWFeat {
public:
    char pad_0x0[0x10];
    unsigned long m_nTalentMask;     // +0x10 (placeholder name)
    int m_nTalentWeight;             // +0x14 (placeholder name)
};

class CSWSpell {
public:
    CExoString m_sLabel;             // +0x0
    char pad_0x8[0x124 - 0x8];
    unsigned long m_nTalentMask;     // +0x124 (placeholder name)
    int m_nTalentWeight;             // +0x128 (placeholder name)

    int GetAdjustedForcePoints(unsigned long nLevel, int bAdjust);
};

class CSWSpellArray {
public:
    CSWSpell *GetSpell(int nSpell);
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray *m_pSpellArray;    // +0x8c
    char pad_0x90[0x28];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
    CSWFeat *GetFeat(unsigned short nFeat);
};

extern CSWRules *g_pRules;

class CSWSArea;

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
    CSWSArea *GetArea();

    char pad_0x4[0x9ec];
    unsigned char m_nStateFlags;     // +0x9f0
    char pad_0x9f1[0xa88 - 0x9f1];
    int m_bForceAdjust;              // +0xa88
};

class CSWSMemorizedSpell {
public:
    unsigned long m_nSpell;
    int m_bReady;
    unsigned char m_nClass;
};

class CSWSClassInfo {
public:
    CExoArrayList<unsigned long> m_pKnownSpellList[1];  // +0x0
    char pad_0xc[0x1c];

    unsigned long GetKnownSpell(unsigned char nSpellLevel, unsigned char n)
    {
        if (nSpellLevel >= 1)
            return 0xffffffff;
        if (n >= m_pKnownSpellList[nSpellLevel].num)
            return 0xffffffff;
        return m_pKnownSpellList[nSpellLevel].element[n];
    }
};

class CSWSCreatureStats {
public:
    int CheckTalentMask(unsigned long nMask, unsigned long nWanted);
    int GetCreatureHasTalent(int nType, int nId);
    int HasFeat(unsigned short nFeat);
    unsigned char GetFeatRemainingUses(unsigned short nFeat);
    int GetCreatureTalentRandomSpecialAbility(int nCategory, int nMaxCR, int *pnType, int *pnId,
                                              unsigned char *pnClass, unsigned char *pnMetaType,
                                              int nExclusionMask, int nExclude);
    int GetCreatureTalentRandomPower(int a1, int a2, int a3, int a4, int a5, int a6, int a7);
    int GetCreatureTalentRandomPowerFromItem(int nCategory, int nMaxCR, int *pnType, int *pnId,
                                             unsigned char *pnClass, unsigned long *pnItem,
                                             int *pnProperty, int nExclusionMask, int nExclude);
    int GetCreatureTalentRandomFeat(int nCategory, int nMaxCR, int *pnType, int *pnId,
                                    unsigned char *pnClass, int nExclusionMask, int nExclude);
    int GetCreatureTalentRandomPowerForClass(int nCategory, int nMaxCR, int *pnType, int *pnId,
                                             unsigned char *pnClass, unsigned char nMultiClass,
                                             int nExclusionMask, int nExclude);
    int GetCreatureTalent(int nCategory, int nMaxCR, int *pnType, int *pnId,
                          unsigned char *pnClass, unsigned long *pnItem, int *pnProperty,
                          unsigned char *pnMetaType, int nExclusionMask, int nLastType,
                          int nLastId);

    unsigned short GetFeat(unsigned short n)
    {
        if (n >= m_lFeats.num)
            return 0xffff;
        return m_lFeats.element[n];
    }
    unsigned short GetNumberKnownSpells(unsigned char nMultiClass, unsigned char nSpellLevel)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0;
        return m_ClassInfo[nMultiClass].m_pKnownSpellList[nSpellLevel].num;
    }
    unsigned long GetKnownSpell(unsigned char nMultiClass, unsigned char nSpellLevel,
                                unsigned char n)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0xffffffff;
        return m_ClassInfo[nMultiClass].GetKnownSpell(nSpellLevel, n);
    }

    CExoArrayList<unsigned short> m_lFeats;            // +0x0
    char pad_0xc[0x24 - 0xc];
    CSWSCreature *m_pBaseCreature;                     // +0x24
    char pad_0x28[0x89 - 0x28];
    unsigned char m_nNumMultiClasses;  // +0x89
    char pad_0x8a[0x2];
    CSWSClassInfo m_ClassInfo[2];    // +0x8c
    char pad_0xdc[0x124 - 0xdc];
    short m_nForcePointsBonus;       // +0x124
    short m_nForcePoints;            // +0x126
    CExoArrayList<CSWSMemorizedSpell> *m_pMemorizedSpells;  // +0x128
    char pad_0x12c[0x17e - 0x12c];
    short m_nGoodEvil;               // +0x17e
};

// The pickers expand GetTalentAllowed (0x005A5DF0) in place: the original
// numbers the feat and spell CExoString scopes of both branches.
#define TALENT_ALLOWED_EXPAND(bAllowed, nType, nId, nExclusionMask)                        \
    {                                                                                      \
        bAllowed = 1;                                                                      \
        int nExclusion = 0;                                                                \
        bool bFound = false;                                                               \
        if ((nType) == 1) {                                                                \
            CExoString sColumn("Exclusion");                                               \
            bFound = g_pRules->Get2DArrays()->GetFeatTable()->GetINTEntry(nId, sColumn,    \
                                                                          &nExclusion) != 0; \
        } else if ((nType) == 0) {                                                         \
            CExoString sColumn("Exclusion");                                               \
            bFound = g_pRules->Get2DArrays()->GetSpellsTable()->GetINTEntry(nId, sColumn,  \
                                                                            &nExclusion) != 0; \
        }                                                                                  \
        if (bFound && nExclusion && (nExclusion | (nExclusionMask)))                       \
            bAllowed = 0;                                                                  \
    }

// NONMATCH (67): register allocation - the original keeps this in ebx (plus a stack
// copy), a zero register in ebp and the loop index as a stack word; cl spills this.
// FUNCTION: SWKOTOR 0x005AC370
int CSWSCreatureStats::GetCreatureTalentRandomFeat(int nCategory, int nMaxCR, int *pnType,
                                                   int *pnId, unsigned char *pnClass,
                                                   int nExclusionMask, int nExclude)
{
    int bResult = 0;
    CExoArrayList<unsigned long> *pFeats = new CExoArrayList<unsigned long>;
    int nBestWeight = -100;
    for (unsigned short i = 0; i < m_lFeats.num; i++) {
        unsigned short nFeat = m_lFeats.element[i];
        CSWFeat *pFeat = g_pRules->GetFeat(nFeat);
        if (pFeat && CheckTalentMask(pFeat->m_nTalentMask, nCategory)) {
            int bAllowed;
            TALENT_ALLOWED_EXPAND(bAllowed, 1, nFeat, nExclusionMask);
            if (bAllowed &&
                nExclude != nFeat && nFeat != -1 &&
                ((m_pBaseCreature->m_nStateFlags & 6) == 6 || !m_pBaseCreature->GetDead()) &&
                !m_pBaseCreature->GetIsInCombatAnimation() && !m_pBaseCreature->GetIsDying() &&
                HasFeat(nFeat) && GetFeatRemainingUses(nFeat)) {
                if (nMaxCR != -1) {
                    if (pFeat->m_nTalentWeight > nBestWeight) {
                        delete pFeats;
                        pFeats = new CExoArrayList<unsigned long>;
                        pFeats->Add(nFeat);
                        nBestWeight = pFeat->m_nTalentWeight;
                    } else if (pFeat->m_nTalentWeight == nBestWeight) {
                        pFeats->Add(nFeat);
                    }
                } else {
                    pFeats->Add(nFeat);
                }
            }
        }
    }
    int nCount = pFeats->num;
    if (nCount > 0) {
        *pnType = 1;
        int n = rand() % nCount;
        bResult = 1;
        *pnId = pFeats->element[n];
    }
    delete pFeats;
    return bResult;
}

// NONMATCH (6): only the order of the two exit blocks (the original places the
// `return 1` block before the give-up block); for(;;), while and goto shapes all
// give the same layout.
// FUNCTION: SWKOTOR 0x005B23A0
int CSWSCreatureStats::GetCreatureTalent(int nCategory, int nMaxCR, int *pnType, int *pnId,
                                         unsigned char *pnClass, unsigned long *pnItem,
                                         int *pnProperty, unsigned char *pnMetaType,
                                         int nExclusionMask, int nLastType, int nLastId)
{
    *pnType = -1;
    *pnId = -1;
    *pnClass = 0xff;
    *pnItem = 0x7f000000;
    *pnProperty = -1;
    *pnMetaType = 0xff;
    int bSpellsTried = 0;
    int bFeatsTried = 0;
    while (!bSpellsTried || !bFeatsTried) {
        int nRoll = rand() % 2;
        if (nRoll == 0) {
            if (!bSpellsTried) {
                int nExclude = nLastType == 0 ? nLastId : -1;
                if (GetCreatureTalentRandomSpecialAbility(nCategory, nMaxCR, pnType, pnId, pnClass,
                                                          pnMetaType, nExclusionMask, nExclude) ||
                    GetCreatureTalentRandomPower(nCategory, nMaxCR, (int)pnType, (int)pnId,
                                                 (int)pnClass, nExclusionMask, nExclude) ||
                    GetCreatureTalentRandomPowerFromItem(nCategory, nMaxCR, pnType, pnId, pnClass,
                                                         pnItem, pnProperty, nExclusionMask,
                                                         nExclude))
                    return 1;
                bSpellsTried = 1;
            }
        } else if (nRoll == 1) {
            if (!bFeatsTried) {
                int nExclude;
                if (nLastType == 1) {
                    nExclude = nLastId;
                    if (nLastId == -1)
                        bSpellsTried = 1;
                } else {
                    nExclude = -1;
                }
                if (GetCreatureTalentRandomFeat(nCategory, nMaxCR, pnType, pnId, pnClass,
                                                nExclusionMask, nExclude))
                    return 1;
                bFeatsTried = 1;
            }
        }
    }
    *pnType = -1;
    *pnId = -1;
    return 0;
}

// NONMATCH (61): register allocation of the two result lists and the loop index.
// FUNCTION: SWKOTOR 0x005AB9E0
int CSWSCreatureStats::GetCreatureTalentRandomSpecialAbility(int nCategory, int nMaxCR,
                                                             int *pnType, int *pnId,
                                                             unsigned char *pnClass,
                                                             unsigned char *pnMetaType,
                                                             int nExclusionMask, int nExclude)
{
    int bResult = 0;
    CExoArrayList<unsigned long> *pSpells = new CExoArrayList<unsigned long>;
    CExoArrayList<unsigned long> *pClasses = new CExoArrayList<unsigned long>;
    int nBestWeight = -100;
    for (int i = 0; i < m_pMemorizedSpells->num; i++) {
        unsigned long nSpell = m_pMemorizedSpells->element[i].m_nSpell;
        if (!m_pMemorizedSpells->element[i].m_bReady)
            continue;
        unsigned char nClass = m_pMemorizedSpells->element[i].m_nClass;
        CSWSpell *pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
        if (!pSpell || nSpell == 0xffffffff || !GetCreatureHasTalent(0, nSpell))
            continue;
        if (m_pBaseCreature->GetArea() && !CheckTalentMask(pSpell->m_nTalentMask, nCategory))
            continue;
        int bAllowed;
        TALENT_ALLOWED_EXPAND(bAllowed, 0, nSpell, nExclusionMask);
        if (!bAllowed)
            continue;
        if (nMaxCR != -1) {
            if (pSpell->m_nTalentWeight > nBestWeight) {
                delete pSpells;
                pSpells = new CExoArrayList<unsigned long>;
                pSpells->Add(nSpell);
                delete pClasses;
                pClasses = new CExoArrayList<unsigned long>;
                pClasses->Add(nClass);
                pSpell->m_sLabel.CStr();
                nBestWeight = pSpell->m_nTalentWeight;
                continue;
            }
            if (pSpell->m_nTalentWeight != nBestWeight)
                continue;
        }
        pSpells->Add(nSpell);
        pClasses->Add(nClass);
        pSpell->m_sLabel.CStr();
    }
    int nCount = pSpells->num;
    if (nCount > 0) {
        int n = rand() % nCount;
        *pnType = 0;
        bResult = 1;
        *pnId = pSpells->element[n];
        *pnMetaType = (unsigned char)pClasses->element[n];
        *pnClass = 0xfe;
    }
    delete pSpells;
    delete pClasses;
    return bResult;
}

// NONMATCH (116, 96 bytes short): first draft; list handling and the weight
// comparison chain differ.
// FUNCTION: SWKOTOR 0x005AC5D0
int CSWSCreatureStats::GetCreatureTalentRandomPowerForClass(int nCategory, int nMaxCR,
                                                            int *pnType, int *pnId,
                                                            unsigned char *pnClass,
                                                            unsigned char nMultiClass,
                                                            int nExclusionMask, int nExclude)
{
    CExoArrayList<unsigned long> *pSpells = new CExoArrayList<unsigned long>;
    CExoArrayList<unsigned char> *pClasses = new CExoArrayList<unsigned char>;
    int bResult = 0;
    for (unsigned char i = 0; (unsigned short)i < GetNumberKnownSpells(nMultiClass, 0); i++) {
        unsigned long nSpell = GetKnownSpell(nMultiClass, 0, i);
        CSWSpell *pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
        if (!pSpell)
            continue;
        if (pSpell->GetAdjustedForcePoints(m_nGoodEvil, m_pBaseCreature->m_bForceAdjust) >
            (short)(m_nForcePoints + m_nForcePointsBonus))
            continue;
        if (nSpell == nExclude || !GetCreatureHasTalent(0, nSpell))
            continue;
        if (m_pBaseCreature->GetArea() && !CheckTalentMask(pSpell->m_nTalentMask, nCategory))
            continue;
        if (nMaxCR == -1) {
            pSpells->Add(nSpell);
            pClasses->Add(nMultiClass);
        } else if (pSpell->m_nTalentWeight > -100) {
            delete pSpells;
            delete pClasses;
            pSpells = new CExoArrayList<unsigned long>;
            pClasses = new CExoArrayList<unsigned char>;
            pSpells->Add(nSpell);
            pClasses->Add(nMultiClass);
        } else if (pSpell->m_nTalentWeight == -100) {
            pSpells->Add(nSpell);
            pClasses->Add(nMultiClass);
        } else {
            continue;
        }
        pSpell->m_sLabel.CStr();
    }
    int nCount = pSpells->num;
    if (nCount > 0) {
        int n = rand() % nCount;
        *pnType = 0;
        bResult = 1;
        *pnId = pSpells->element[n];
        *pnClass = pClasses->element[n];
    }
    delete pSpells;
    delete pClasses;
    return bResult;
}
