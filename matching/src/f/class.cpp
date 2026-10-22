// CSWClass / CSWBaseItem name text, CSWClass ctor/dtor (b005b_f2_faction).

extern "C" void *memset(void *p, int c, unsigned int n);
#pragma intrinsic(memset)
extern "C" void free(void *p);

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
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

class CSWBaseItem {
public:
    CExoString GetNameText();
    unsigned long m_nName;           // +0x0
};

#pragma pack(push, 1)
class CSWClass {
public:
    CSWClass();
    ~CSWClass();
    CExoString GetNameText();
    CExoString GetNameLowerText();
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
    unsigned char *m_p10C;          // +0x10c
    unsigned short **m_ppFeatTables;           // +0x110
    unsigned long m_a114[5];         // +0x114
    unsigned char m_a128[20];        // +0x128
    unsigned char m_a13C[20];        // +0x13c
    unsigned char m_a150[20];        // +0x150
    unsigned char *m_p164;                    // +0x164
    unsigned short m_n168;           // +0x168
    char pad_0x16a[0x2];
    unsigned char *m_p16C;                    // +0x16c
    unsigned short m_n170;           // +0x170
    char pad_0x172[0x2];
    unsigned char *m_p174;                    // +0x174
    char pad_0x178[0x2];
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

// FUNCTION: SWKOTOR 0x005B4690
CExoString CSWBaseItem::GetNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005BE0E0
CExoString CSWClass::GetNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005BE180
CExoString CSWClass::GetNameLowerText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nNameLower, &strres, g_nGender);
    return strres.szString;
}

// NONMATCH (17): store order/regs only: the original loads `lea edi` before
// `mov ecx,0xf` for the first rep stosd, zeroes +0x17b with a fresh eax and
// the two 20-byte memsets use ecx=0 / eax=base (ours: eax=0 / ecx=base).
// FUNCTION: SWKOTOR 0x005BCAC0
CSWClass::CSWClass()
{
    m_nName = 0xffffffff;
    m_nNamePlural = 0xffffffff;
    m_nDescription = 0xffffffff;
    memset(m_lstAttackBonus, 0, sizeof(m_lstAttackBonus));
    for (int i = 0; i < 20; i++) {
        m_a128[i] = 0;
        m_a183[i] = 0;
        m_a13C[i] = 0;
        m_a150[i] = 0;
    }
    m_n17B = 0;
    m_n17F = 0;
    memset(m_lstFortSave, 0, sizeof(m_lstFortSave));
    memset(m_lstRefSave, 0, sizeof(m_lstRefSave));
    memset(m_lstWillSave, 0, sizeof(m_lstWillSave));
    memset(m_a114, 0, sizeof(m_a114));
    memset(m_a128, 0, sizeof(m_a128));
    m_p164 = 0;
    m_ppFeatTables = 0;
    m_p174 = 0;
    m_p10C = 0;
    m_n54 = 0;
    m_n168 = 0;
    m_n198 = 0;
    m_p16C = 0;
    m_n170 = 0;
    m_n19C = 0;
    m_n17A = 0;
    m_n181 = 0;
    m_n182 = 0;
}

// FUNCTION: SWKOTOR 0x005BCBB0
CSWClass::~CSWClass()
{
    if (m_ppFeatTables) {
        for (int i = 0; i < 20; i++) {
            if (m_ppFeatTables[i]) {
                delete[] m_ppFeatTables[i];
                m_ppFeatTables[i] = 0;
            }
        }
        delete[] m_ppFeatTables;
        m_ppFeatTables = 0;
    }
    if (m_p164) {
        delete[] m_p164;
        m_p164 = 0;
    }
    if (m_p16C) {
        delete[] m_p16C;
        m_p16C = 0;
    }
    if (m_p10C) {
        delete[] m_p10C;
        m_p10C = 0;
    }
    if (m_p174) {
        delete[] m_p174;
        m_p174 = 0;
    }
}
