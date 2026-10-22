// CSWClass 2DA table loaders (b005b_f2_class_loaders, b005b_f2_combat).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    CExoString operator+(const CExoString &s) const;
    int GetLength() const;
    void Format(const char *szFormat, ...);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char *szName);
    CResRef(const CExoString &sName);
    CResRef(const CResRef &cResRef);
};

class C2DA {
public:
    virtual ~C2DA();
    char pad_0x4[0x20];
    int m_nNumRows;               // 0x24
    char pad_0x28[0x2c];
    C2DA(CResRef cResRef, int bPrivate);
    int Load2DArray();
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
    int GetNumRows() { return m_nNumRows; }
};

class CSWFeatList {
public:
    int IsFeatValid(unsigned short nFeat);   // 0x00550C00
};

struct CSWClassFeat {
    unsigned short m_nFeat;          // +0x0
    unsigned char m_nGranted;        // +0x2
    unsigned char m_nList;           // +0x3
    unsigned long m_nRecommended;    // +0x4
};

struct CSWClassSkill {
    unsigned short m_nSkill;         // +0x0
    int m_bClassSkill;               // +0x4
    int m_nRecommended;              // +0x8
};

struct CSWClassSpell {
    unsigned short m_nSpell;         // +0x0
    int m_nDarkRecommended;          // +0x4
    int m_nLightRecommended;         // +0x8
    unsigned char m_nUserType;       // +0xc
};

#pragma pack(push, 1)
class CSWClass {
public:
    void LoadAttackBonusTable(const CExoString &sTable);
    void LoadFeatGain(const CExoString &sPrefix);
    void LoadFeatsTable(const CExoString &sPrefix, CSWFeatList *pFeats);
    void LoadSavingThrowTable(const CExoString &sTable);
    void LoadSkillsTable(const CExoString &sPrefix);
    void LoadSpellGainTable(const CExoString &sColumn);
    void LoadSpellKnownTable(const CExoString &sTable);
    void LoadArmorClassTable(const CExoString &sColumn);
    void LoadSpellsTable();
    unsigned long m_nName;           // +0x0
    unsigned long m_nNameLower;      // +0x4
    unsigned long m_nNamePlural;     // +0x8
    unsigned long m_nDescription;    // +0xc
    CExoString m_sName;              // +0x10
    unsigned char m_lstAttackBonus[60];   // +0x18
    unsigned char m_n54;             // +0x54
    char pad_0x55[0x2];
    unsigned char m_lstFortSave[60];      // +0x57
    unsigned char m_lstRefSave[60];       // +0x93
    unsigned char m_lstWillSave[60];      // +0xcf
    char pad_0x10b;
    unsigned char *m_lstACBonus;     // +0x10c
    unsigned char **m_ppSpellsKnown; // +0x110
    unsigned char m_lstNumSpellLevels[20];  // +0x114
    unsigned char m_lstSpellGain[20];       // +0x128
    unsigned char m_lstBonusFeats[20];    // +0x13c
    unsigned char m_lstRegularFeats[20];  // +0x150
    CSWClassSkill *m_pSkills;        // +0x164
    unsigned short m_nNumSkills;     // +0x168
    char pad_0x16a[0x2];
    CSWClassFeat *m_pFeats;          // +0x16c
    unsigned short m_nNumFeats;      // +0x170
    char pad_0x172[0x2];
    CSWClassSpell *m_pSpells;        // +0x174
    unsigned short m_nNumSpells;     // +0x178
    unsigned char m_n17A;            // +0x17a
    unsigned long m_n17B;            // +0x17b
    unsigned short m_n17F;           // +0x17f
    unsigned char m_n181;            // +0x181
    unsigned char m_n182;            // +0x182
    unsigned char m_a183[20];        // +0x183
    char pad_0x197;
    unsigned long m_n198;            // +0x198
    unsigned long m_n19C;            // +0x19c
};
#pragma pack(pop)

// FUNCTION: SWKOTOR 0x005BCDA0
void CSWClass::LoadAttackBonusTable(const CExoString &sTable)
{
    int nValue;
    CExoString sUnused;
    C2DA *pTable = new C2DA(sTable, 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadAttackBonusTable: Can't load") + sTable;
        sError = sError + " 2DA file";
    }
    for (int i = 0; i < 60; i++) {
        int bOk = pTable->GetINTEntry(i, "BAB", &nValue);
        m_lstAttackBonus[i] = bOk ? nValue : 0;
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BCF70
void CSWClass::LoadFeatGain(const CExoString &sPrefix)
{
    int nValue;
    C2DA *pTable = new C2DA("featgain", 0);
    if (!pTable->Load2DArray()) {
        CExoString sError("CSWClass::LoadFeatGain: can't load featgain.2da");
    }
    CExoString sRegular = sPrefix + "_REG";
    CExoString sBonus = sPrefix + "_BON";
    for (int i = 0; i < 20; i++) {
        if (pTable->GetINTEntry(i, sRegular, &nValue))
            m_lstRegularFeats[i] = (unsigned char)nValue;
        if (pTable->GetINTEntry(i, sBonus, &nValue))
            m_lstBonusFeats[i] = (unsigned char)nValue;
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BD0F0
void CSWClass::LoadFeatsTable(const CExoString &sPrefix, CSWFeatList *pFeats)
{
    int nValue;
    C2DA *pTable = new C2DA("Feat", 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadFeatTable: Can't load feat.2da");
        sError = sError + " 2DA file";
    }
    if (m_pFeats) {
        delete[] m_pFeats;
        m_pFeats = 0;
    }
    m_nNumFeats = (unsigned short)pTable->GetNumRows();
    m_pFeats = new CSWClassFeat[m_nNumFeats];
    int nCount = 0;
    for (int i = 0; i < m_nNumFeats; i++) {
        m_pFeats[i].m_nFeat = 0;
        m_pFeats[i].m_nGranted = 0;
        m_pFeats[i].m_nList = 0;
        m_pFeats[nCount].m_nFeat = 0;
        m_pFeats[nCount].m_nGranted = 0;
        m_pFeats[nCount].m_nList = 0;
        m_pFeats[nCount].m_nFeat = (unsigned short)i;
        if (!pFeats->IsFeatValid(m_pFeats[nCount].m_nFeat))
            continue;
        pTable->GetINTEntry(i, sPrefix + "_List", &nValue);
        switch ((unsigned char)nValue) {
        case 0:
            m_pFeats[nCount].m_nList = 1;
            break;
        case 1:
            m_pFeats[nCount].m_nList = 3;
            break;
        case 2:
            m_pFeats[nCount].m_nList = 2;
            break;
        case 3:
            m_pFeats[nCount].m_nList = 0;
            break;
        case 4:
            continue;
        }
        pTable->GetINTEntry(i, sPrefix + "_Granted", &nValue);
        m_pFeats[nCount].m_nGranted = (unsigned char)nValue;
        pTable->GetINTEntry(i, sPrefix + "_Recom", &nValue);
        m_pFeats[nCount].m_nRecommended = nValue;
        nCount++;
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BD480
void CSWClass::LoadSavingThrowTable(const CExoString &sTable)
{
    int nValue;
    C2DA *pTable = new C2DA(sTable, 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadSavingThrowTable: Can't load") + sTable;
        sError = sError + " 2DA file";
    }
    for (int i = 0; i < 60; i++) {
        pTable->GetINTEntry(i, "FortSave", &nValue);
        m_lstFortSave[i] = (unsigned char)nValue;
        pTable->GetINTEntry(i, "WillSave", &nValue);
        m_lstRefSave[i] = (unsigned char)nValue;
        pTable->GetINTEntry(i, "RefSave", &nValue);
        m_lstWillSave[i] = (unsigned char)nValue;
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BD6C0
void CSWClass::LoadSkillsTable(const CExoString &sPrefix)
{
    C2DA *pTable = new C2DA("skills", 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadSkillsTable: Can't load skills.2da");
    }
    if (m_pSkills) {
        delete[] m_pSkills;
        m_pSkills = 0;
    }
    m_nNumSkills = (unsigned short)pTable->GetNumRows();
    m_pSkills = new CSWClassSkill[m_nNumSkills];
    for (int i = 0; i < m_nNumSkills; i++) {
        m_pSkills[i].m_nSkill = (unsigned short)i;
        int nValue;
        pTable->GetINTEntry(i, sPrefix + "_Class", &nValue);
        m_pSkills[i].m_bClassSkill = nValue;
        int bOk = pTable->GetINTEntry(i, sPrefix + "_Reco", &nValue);
        if (bOk)
            m_pSkills[i].m_nRecommended = nValue;
        else
            m_pSkills[i].m_nRecommended = -1;
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BD900
void CSWClass::LoadSpellGainTable(const CExoString &sColumn)
{
    int nValue;
    if (sColumn.GetLength() < 1)
        return;
    C2DA *pLevels = new C2DA("CLS_SPGN_JEDI", 0);
    if (!pLevels->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadSpellGainTable: Can't load CLS_SPGN_JEDI");
        sError = sError + " 2DA file";
    }
    C2DA *pGain = new C2DA("ClassPowerGain", 0);
    if (!pGain->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadSpellGainTable: Can't load ClassPowerGain");
        sError = sError + " 2DA file";
    }
    for (int i = 0; i < 20; i++) {
        pLevels->GetINTEntry(i, "NumSpellLevels", &nValue);
        m_lstNumSpellLevels[i] = (unsigned char)nValue;
        if (m_lstNumSpellLevels[i]) {
            for (int j = 0; j < m_lstNumSpellLevels[i]; j++) {
                if (pGain->GetINTEntry(i, sColumn, &nValue))
                    m_lstSpellGain[i] = (unsigned char)nValue;
                else
                    m_lstSpellGain[i] = 0xff;
            }
        }
    }
    delete pGain;
    delete pLevels;
}

// FUNCTION: SWKOTOR 0x005BDBE0
void CSWClass::LoadSpellKnownTable(const CExoString &sTable)
{
    int nValue;
    CExoString sColumn;
    C2DA *pTable = new C2DA(sTable, 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadSpellKnownTable: Can't load") + sTable;
        sError = sError + " 2DA file";
    }
    if (m_ppSpellsKnown) {
        for (int i = 0; i < 20; i++) {
            if (m_ppSpellsKnown[i]) {
                delete[] m_ppSpellsKnown[i];
                m_ppSpellsKnown[i] = 0;
            }
        }
        delete[] m_ppSpellsKnown;
        m_ppSpellsKnown = 0;
    }
    m_ppSpellsKnown = new unsigned char *[20];
    for (int i = 0; i < 20; i++) {
        if (m_lstNumSpellLevels[i]) {
            m_ppSpellsKnown[i] = new unsigned char[m_lstNumSpellLevels[i]];
            for (int j = 0; j < m_lstNumSpellLevels[i]; j++) {
                sColumn.Format("SpellLevel%d", j);
                if (pTable->GetINTEntry(i, sColumn, &nValue))
                    m_ppSpellsKnown[i][j] = (unsigned char)nValue;
                else
                    m_ppSpellsKnown[i][j] = 0;
            }
        }
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BDE80
void CSWClass::LoadArmorClassTable(const CExoString &sColumn)
{
    int nValue;
    C2DA *pTable = new C2DA("ACBonus", 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadArmorClassTable: Can't load ACBonus");
        sError = sError + " 2DA file";
    }
    if (m_lstACBonus) {
        delete[] m_lstACBonus;
        m_lstACBonus = 0;
    }
    m_lstACBonus = new unsigned char[21];
    for (int i = 0; i <= 20; i++) {
        if (pTable->GetINTEntry(i, sColumn, &nValue))
            m_lstACBonus[i] = (unsigned char)nValue;
        else
            m_lstACBonus[i] = 0;
    }
    delete pTable;
}

// FUNCTION: SWKOTOR 0x005BE4C0
void CSWClass::LoadSpellsTable()
{
    C2DA *pTable = new C2DA("spells", 0);
    if (!pTable->Load2DArray()) {
        CExoString sError;
        sError = CExoString("CSWClass::LoadSpellsTable: Can't load spells.2da");
    }
    if (m_pSpells) {
        delete[] m_pSpells;
        m_pSpells = 0;
    }
    m_nNumSpells = (unsigned short)pTable->GetNumRows();
    m_pSpells = new CSWClassSpell[m_nNumSpells];
    for (int i = 0; i < m_nNumSpells; i++) {
        m_pSpells[i].m_nSpell = (unsigned short)i;
        int nValue;
        pTable->GetINTEntry(i, "Dark_Recom", &nValue);
        m_pSpells[i].m_nDarkRecommended = nValue;
        pTable->GetINTEntry(i, "Light_Recom", &nValue);
        m_pSpells[i].m_nLightRecommended = nValue;
        pTable->GetINTEntry(i, "UserType", &nValue);
        m_pSpells[i].m_nUserType = (unsigned char)nValue;
    }
    delete pTable;
}
