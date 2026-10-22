// CSWSpell construction and the prerequisite/master-spell id
// parser (b0059_f2_scriptvar_dialog).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    CExoString &operator=(const char *s);
    char operator[](int n) const;
    int GetLength() const;

    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef(const CResRef &r);
    CResRef operator=(const char *s);
    char m_resRef[16];
};

struct STR_RES {
    CExoString szString;
    CResRef cSoundResRef;
    float fSoundLength;
};

class CTlkTable {
public:
    int GetSimpleString(unsigned long strref, STR_RES *pRes, unsigned char nGender);
};

extern CTlkTable *g_pTlkTable;
extern const unsigned char g_nGender;

class C2DA {
public:
    int GetFLOATEntry(int nRow, int nColumn, float *pValue);
};

class CTwoDimArrays {
public:
    char pad_0x0[0xd8];
    C2DA *m_pForceAdjust;            // +0xd8
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays *m_p2DArrays;      // +0xb8
    CTwoDimArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;
extern int g_nForceAdjustGoodCostColumn;     // 0x007A2184
extern int g_nForceAdjustEvilCostColumn;     // 0x007A2188

class CSWSpell {
public:
    CSWSpell();
    ~CSWSpell();
    void SetPrerequisites(const CExoString &s);
    void SetMasterSpells(const CExoString &s);
    unsigned char GetSpellLevel(unsigned char nClass);
    int GetSubSpell(unsigned char nIndex);
    int GetAdjustedForcePoints(unsigned long nLevel, int bAdjust);
    CExoString GetSpellNameText();

    CExoString m_sName;              // +0x0
    unsigned long m_nName;           // +0x8
    char pad_0xc[0x4];
    CResRef m_cIcon;                 // +0x10
    CExoString m_s20;                // +0x20
    CExoString m_s28;                // +0x28
    unsigned short m_n30;            // +0x30
    char pad_0x32[0x2];
    CExoString m_s34;                // +0x34
    unsigned char m_nLevel3C;        // +0x3c
    unsigned char m_nLevel3D;        // +0x3d
    unsigned char m_nLevel3E;        // +0x3e
    unsigned char m_nLevel3F;        // +0x3f
    unsigned long m_n40;             // +0x40
    char pad_0x44[0x2];
    CResRef m_r46;                   // +0x46
    CResRef m_r56;                   // +0x56
    CResRef m_r66;                   // +0x66
    CResRef m_r76;                   // +0x76
    CResRef m_r86;                   // +0x86
    CResRef m_r96;                   // +0x96
    char pad_0xa6[0x2];
    unsigned long m_nA8;             // +0xa8
    CResRef m_rAC;                   // +0xac
    CResRef m_rBC;                   // +0xbc
    CResRef m_rCC;                   // +0xcc
    CResRef m_rDC;                   // +0xdc
    unsigned long m_nEC;             // +0xec
    unsigned short m_nF0;            // +0xf0
    char pad_0xf2[0x2];
    unsigned long m_nF4;             // +0xf4
    CResRef m_rF8;                   // +0xf8
    unsigned long m_n108;            // +0x108
    unsigned char m_n10C;            // +0x10c
    CResRef m_r10D;                  // +0x10d
    unsigned char m_n11D;            // +0x11d
    unsigned char m_n11E;            // +0x11e
    char pad_0x11f[0x1];
    unsigned long m_n120;            // +0x120
    char pad_0x124[0x8];
    int *m_p12C;                     // +0x12c
    unsigned char m_n130;            // +0x130
    char pad_0x131[0x3];
    unsigned long m_n134;            // +0x134
    unsigned long m_n138;            // +0x138
    unsigned long m_n13C;            // +0x13c
    unsigned char m_n140;            // +0x140
    char pad_0x141[0x3];
    unsigned long m_n144;            // +0x144
    unsigned long m_n148;            // +0x148
    char pad_0x14c[0x2];
    unsigned char m_nForcePoints;    // +0x14e
    char m_cAlignment;               // +0x14f
    CExoString m_sPrerequisites;     // +0x150
    CExoString m_sMasterSpells;      // +0x158
    int m_nPrerequisites[5];         // +0x160
    int m_nMasterSpells[5];          // +0x174
    unsigned long m_n188;            // +0x188
    unsigned long m_n18C;            // +0x18c
    unsigned long m_n190;            // +0x190
    unsigned char m_n194;            // +0x194
};

// FUNCTION: SWKOTOR 0x0059B310
CSWSpell::CSWSpell()
{
    m_n40 = 3000;
    m_r76 = "v_conjure";
    m_r86 = "v_cast";
    m_r96 = "";
    m_rDC = "";
    m_n30 = 0;
    m_s34 = "";
    m_nF4 = 0;
    m_rF8 = "fx_hit";
    m_n108 = 1;
    m_r10D = "fx_imp";
    m_rAC = "";
    m_rBC = "";
    m_rCC = "";
    m_r46 = "";
    m_r56 = "";
    m_r66 = "";
    m_n138 = 0xffffffff;
    m_n13C = 0xffffffff;
    m_n10C = 0;
    m_nA8 = 0;
    m_n11D = 0;
    m_n11E = 0;
    m_n120 = 0;
    m_p12C = 0;
    m_n130 = 0;
    m_n134 = 0;
    m_n140 = 0;
    m_n144 = 0;
    m_n148 = 0;
    m_n188 = 0;
    m_n18C = 0;
    m_n190 = 0;
    m_n194 = 0;
    m_nEC = 0;
    m_nF0 = 0;
}

// FUNCTION: SWKOTOR 0x0059B5B0
CSWSpell::~CSWSpell()
{
    if (m_p12C)
        delete[] m_p12C;
}

// NONMATCH (12): register allocation only; original keeps `this` in ebp (then
// reuses it as the id pointer) and the length in ebx/[esp+0x10], with the
// 5-count in the dead arg slot. Same for SetMasterSpells.
// FUNCTION: SWKOTOR 0x0059B700
void CSWSpell::SetPrerequisites(const CExoString &s)
{
    m_sPrerequisites = s;
    int nLength = s.GetLength();
    int i = 0;
    while (i < nLength) {
        if (s[i] != '_' && s[i] != ':')
            break;
        i++;
    }
    for (int n = 0; n < 5; n++) {
        int nValue = 0;
        m_nPrerequisites[n] = -1;
        if (i < nLength) {
            if (s[i] != '_') {
                for (;;) {
                    if (s[i] == ':')
                        break;
                    nValue = nValue * 10 + (unsigned char)s[i] - '0';
                    i++;
                    if (s[i] == '_')
                        break;
                }
            }
            i++;
            m_nPrerequisites[n] = nValue;
        }
    }
}

// FUNCTION: SWKOTOR 0x0059B7D0
void CSWSpell::SetMasterSpells(const CExoString &s)
{
    m_sMasterSpells = s;
    int nLength = s.GetLength();
    int i = 0;
    while (i < nLength) {
        if (s[i] != '_' && s[i] != ':')
            break;
        i++;
    }
    for (int n = 0; n < 5; n++) {
        int nValue = 0;
        m_nMasterSpells[n] = -1;
        if (i < nLength) {
            if (s[i] != '_') {
                for (;;) {
                    if (s[i] == ':')
                        break;
                    nValue = nValue * 10 + (unsigned char)s[i] - '0';
                    i++;
                    if (s[i] == '_')
                        break;
                }
            }
            i++;
            m_nMasterSpells[n] = nValue;
        }
    }
}

// FUNCTION: SWKOTOR 0x0059B650
unsigned char CSWSpell::GetSpellLevel(unsigned char nClass)
{
    switch (nClass) {
    case 0:
    case 1:
    case 2:
        return 0xff;
    case 3:
        return m_nLevel3C;
    case 4:
        return m_nLevel3D;
    case 5:
        return m_nLevel3E;
    default:
        return m_nLevel3F;
    }
}

// FUNCTION: SWKOTOR 0x0059B6A0
int CSWSpell::GetSubSpell(unsigned char nIndex)
{
    if (nIndex >= m_n130)
        return -1;
    return m_p12C[nIndex];
}

class CSWSpellArray {
public:
    ~CSWSpellArray();
    CSWSpell *GetSpell(int nSpell);

    int m_nNumSpells;                // +0x0
    CSWSpell *m_pSpells;             // +0x4
};

// FUNCTION: SWKOTOR 0x0059B6D0
CSWSpell *CSWSpellArray::GetSpell(int nSpell)
{
    if (m_pSpells && nSpell >= 0 && nSpell < m_nNumSpells)
        return &m_pSpells[nSpell];
    return 0;
}

// NONMATCH (15): original keeps nPoints in esi (plus a stack copy for fild),
// puts fMultiplier in the dead nLevel arg slot and duplicates the early return.
// FUNCTION: SWKOTOR 0x0059B8A0
int CSWSpell::GetAdjustedForcePoints(unsigned long nLevel, int bAdjust)
{
    int nRow = nLevel / 10;
    int nPoints = m_nForcePoints;
    float fMultiplier;
    if (bAdjust) {
        fMultiplier = 1.0f;
        if (m_cAlignment == 'G')
            g_pRules->Get2DArrays()->m_pForceAdjust->GetFLOATEntry(nRow, g_nForceAdjustGoodCostColumn, &fMultiplier);
        else if (m_cAlignment == 'E')
            g_pRules->Get2DArrays()->m_pForceAdjust->GetFLOATEntry(nRow, g_nForceAdjustEvilCostColumn, &fMultiplier);
        return (int)(nPoints * fMultiplier);
    }
    return nPoints;
}

// FUNCTION: SWKOTOR 0x0059B940
CExoString CSWSpell::GetSpellNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x0059B9E0
CSWSpellArray::~CSWSpellArray()
{
    if (m_pSpells) {
        delete[] m_pSpells;
        m_pSpells = 0;
        m_nNumSpells = 0;
    }
}
