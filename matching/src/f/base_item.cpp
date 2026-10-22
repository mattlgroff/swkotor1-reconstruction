// CSWBaseItem / CSWBaseItemArray / CSWItem leaves (0x005B2EF0-0x005B47B0); the
// CExoString default ctor 0x005B3190 lives in exo_string_ctor.cpp so it is not inlined here.

#include <stdio.h>
#include <string.h>

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef(const char *szName);
    CResRef(const CResRef &cResRef);
    char m_resRef[16];
};

class CSWBaseItem {
public:
    CSWBaseItem();
    ~CSWBaseItem();
    CResRef GetIconResRef(short nIcon, char cVariant);
    CResRef GetModelResRef(short nIcon, char cVariant);
    unsigned short GetRequiredFeat(unsigned char nIndex);
    int GetIsRangedSlot();
    int GetIsMeleeSlot();

    int m_nName;                     // +0x00
    int m_n04;                       // +0x04
    unsigned char m_nEquipableSlots; // +0x08
    char pad_0x9[0xf];
    unsigned char m_b18;             // +0x18
    char pad_0x19[0x23];
    char m_sItemClass[0xc];          // +0x3c
    int m_n48;                       // +0x48
    CResRef m_cDefaultIcon;          // +0x4c
    CResRef m_cDefaultModel;         // +0x5c
    char pad_0x6c[0x8];
    int m_nModelType;                // +0x74
    unsigned char m_b78;             // +0x78
    char pad_0x79[0x3];
    unsigned short *m_pRequiredFeats; // +0x7c
    unsigned char m_nRequiredFeats;  // +0x80
    char pad_0x81[0x7];
    unsigned char m_b88;             // +0x88
    char pad_0x89[0xb];
    CExoString m_s94;                // +0x94
    CExoString m_s9c;                // +0x9c
    CExoString m_sa4;                // +0xa4
    char pad_0xac[0x1];
    unsigned char m_nad;             // +0xad
    char pad_0xae[0x1];
    unsigned char m_baf;             // +0xaf
    unsigned short m_nb0;            // +0xb0
    unsigned short m_nb2;            // +0xb2
    char pad_0xb4[0x4];
    int m_nb8;                       // +0xb8
    CExoString m_sbc;                // +0xbc
    float m_fc4;                     // +0xc4
};

class CSWBaseItemArray {
public:
    CSWBaseItem *GetBaseItem(int nBaseItem);

    int m_nNumBaseItems;             // +0x0
    CSWBaseItem *m_pBaseItems;       // +0x4
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray *m_pBaseItemArray; // +0x34
};

extern CSWRules *g_pRules;

class CSWItem {
public:
    CSWItem();
    virtual ~CSWItem();
    CSWBaseItem *GetBaseItem();

    unsigned char m_a4[6];           // +0x4
    char pad_0xa[0x2];
    int m_nBaseItem;                 // +0xc
};

// FUNCTION: SWKOTOR 0x005B2EF0
CSWBaseItem::CSWBaseItem()
{
    m_n04 = 0;
    m_b18 = 0;
    m_nName = -1;
    memset(m_sItemClass, 0, sizeof(m_sItemClass));
    m_n48 = 0;
    m_b78 = 0;
    m_pRequiredFeats = 0;
    m_nRequiredFeats = 0;
    m_b88 = 0;
    m_nb0 = 0;
    m_nb2 = 0;
    m_nb8 = 0;
    m_baf = 1;
    m_nad = 0xff;
}

// FUNCTION: SWKOTOR 0x005B2FC0
CSWBaseItem::~CSWBaseItem()
{
    if (m_pRequiredFeats)
        delete[] m_pRequiredFeats;
}

// FUNCTION: SWKOTOR 0x005B3050
CResRef CSWBaseItem::GetIconResRef(short nIcon, char cVariant)
{
    char szName[20];
    if (m_nModelType && cVariant)
        sprintf(szName, "i%s_%c_%03d", m_sItemClass, cVariant, nIcon);
    else
        sprintf(szName, "i%s_%03d", m_sItemClass, nIcon);
    return CResRef(szName);
}

// FUNCTION: SWKOTOR 0x005B30C0
unsigned short CSWBaseItem::GetRequiredFeat(unsigned char nIndex)
{
    if (nIndex >= m_nRequiredFeats)
        return 0xffff;
    return m_pRequiredFeats[nIndex];
}

// FUNCTION: SWKOTOR 0x005B30E0
CResRef CSWBaseItem::GetModelResRef(short nIcon, char cVariant)
{
    char szName[20];
    if (m_nModelType && cVariant)
        sprintf(szName, "%s_%c_%03d", m_sItemClass, cVariant, nIcon);
    else
        sprintf(szName, "%s_%03d", m_sItemClass, nIcon);
    return CResRef(szName);
}

// FUNCTION: SWKOTOR 0x005B3150
int CSWBaseItem::GetIsRangedSlot()
{
    if (m_nEquipableSlots == 3 || m_nEquipableSlots == 5 || m_nEquipableSlots == 6)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005B3170
int CSWBaseItem::GetIsMeleeSlot()
{
    if (m_nEquipableSlots == 1 || m_nEquipableSlots == 2 || m_nEquipableSlots == 4)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005B31A0
CSWBaseItem *CSWBaseItemArray::GetBaseItem(int nBaseItem)
{
    if (m_pBaseItems && nBaseItem >= 0 && nBaseItem < m_nNumBaseItems)
        return &m_pBaseItems[nBaseItem];
    return 0;
}

// FUNCTION: SWKOTOR 0x005B4660
CSWItem::CSWItem()
{
    m_nBaseItem = 30;
    memset(m_a4, 0, sizeof(m_a4));
}

// FUNCTION: SWKOTOR 0x005B4680
CSWItem::~CSWItem()
{
}

// FUNCTION: SWKOTOR 0x005B4790
CSWBaseItem *CSWItem::GetBaseItem()
{
    return g_pRules->m_pBaseItemArray->GetBaseItem(m_nBaseItem);
}
