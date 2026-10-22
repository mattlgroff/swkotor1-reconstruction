// CSWRace / CSWFeat / CSWSkill TLK text getters and ctors, CSWRoom
// (b005c_f2_journal_store, 0x005C78D0-0x005C7DC0, 0x005CD700-0x005CD970).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    char *CStr() const;
    int CompareNoCase(const char *s) const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef operator=(const CResRef &cResRef);
    int operator==(const char *s) const;
    void CopyToString(CExoString &s) const;
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class Quaternion {
public:
    Quaternion(float fW, float fX, float fY, float fZ) { z = fZ; y = fY; x = fX; w = fW; }
    float w, x, y, z;
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

class CSWRace {
public:
    unsigned long m_nName;               // 0x00
    unsigned long m_nConverName;         // 0x04
    unsigned long m_nConverNameLower;    // 0x08
    char pad_0xc[0x18];
    unsigned short m_nNumFirstLevelGrantedFeats;   // 0x24
    unsigned short *m_lstFirstLevelGrantedFeats;   // 0x28

    ~CSWRace();
    CExoString GetNameText();
    CExoString GetConverNameText();
    CExoString GetConverNameLowerText();
    int IsFirstLevelGrantedFeat(unsigned short nFeat);
};

// FUNCTION: SWKOTOR 0x005C78D0
CSWRace::~CSWRace()
{
    if (m_lstFirstLevelGrantedFeats) {
        delete[] m_lstFirstLevelGrantedFeats;
        m_lstFirstLevelGrantedFeats = 0;
    }
}

// FUNCTION: SWKOTOR 0x005C78F0
CExoString CSWRace::GetNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005C7990
CExoString CSWRace::GetConverNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nConverName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005C7A30
CExoString CSWRace::GetConverNameLowerText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nConverNameLower, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005C7CC0
int CSWRace::IsFirstLevelGrantedFeat(unsigned short nFeat)
{
    if (m_lstFirstLevelGrantedFeats == 0)
        return 0;
    for (unsigned short i = 0; i < m_nNumFirstLevelGrantedFeats; i++) {
        if (m_lstFirstLevelGrantedFeats[i] == nFeat)
            return 1;
    }
    return 0;
}

class CSWFeat {
public:
    unsigned long m_n00;                 // 0x00
    unsigned long m_n04;                 // 0x04
    unsigned long m_nName;               // 0x08
    unsigned long m_nDescription;        // 0x0C
    char pad_0x10[0x8];
    CResRef m_cIcon;                     // 0x18
    unsigned long m_b0 : 1;              // 0x28
    unsigned long m_b1 : 1;
    unsigned long m_b2 : 1;
    unsigned long m_b3 : 1;
    unsigned long m_b4 : 1;
    unsigned char m_n2c;                 // 0x2C
    unsigned char m_n2d;
    unsigned char m_n2e;
    unsigned char m_n2f;
    unsigned char m_n30;
    unsigned char m_n31;
    unsigned char m_n32;
    unsigned char m_n33;
    char pad_0x34[0x4];
    unsigned short m_n38;                // 0x38
    unsigned short m_n3a;                // 0x3A
    unsigned long m_n3c;                 // 0x3C
    unsigned long m_n40;                 // 0x40
    unsigned short m_n44;                // 0x44
    unsigned short m_n46;                // 0x46

    CSWFeat();
    CExoString GetNameText();
    CExoString GetDescriptionText();
};

// NONMATCH (18): store scheduling only; MSVC hoists our byte stores (+0x2C..+0x33)
// ahead of the dword/word -1 stores, the original keeps source order.
// FUNCTION: SWKOTOR 0x005CD700
CSWFeat::CSWFeat()
{
    m_nName = 0xffffffff;
    m_nDescription = 0xffffffff;
    m_n38 = 0xffff;
    m_n3a = 0xffff;
    m_n46 = 0xffff;
    m_n2c = 0;
    m_n2e = 0;
    m_n2f = 0;
    m_n30 = 0;
    m_n31 = 0;
    m_n44 = m_n40 = m_n3c = 0xffffffff;
    m_n04 = 0xffffffff;
    m_n00 = 0xffffffff;
    m_b0 = 0;
    m_b1 = 0;
    m_b2 = 0;
    m_b3 = 0;
    m_n32 = 0;
    m_n33 = 0;
    m_b4 = 1;
}

// FUNCTION: SWKOTOR 0x005CD760
CExoString CSWFeat::GetNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005CD800
CExoString CSWFeat::GetDescriptionText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nDescription, &strres, g_nGender);
    return strres.szString;
}

class CSWSkill {
public:
    unsigned long m_nName;               // 0x00
    unsigned long m_nDescription;        // 0x04
    char pad_0x8[0x8];
    CExoString m_sLabel;                 // 0x10
    char pad_0x18[0x4];
    unsigned long m_b0 : 1;              // 0x1C
    unsigned long m_b1 : 1;
    unsigned long m_b2 : 1;
    unsigned long m_b3 : 1;
    unsigned long m_b4 : 1;
    unsigned long m_b5 : 1;

    CSWSkill();
    CExoString GetNameText();
    CExoString GetDescriptionText();
};

// FUNCTION: SWKOTOR 0x005CD8A0
CSWSkill::CSWSkill()
{
    m_b0 = 0;
    m_b1 = 0;
    m_b2 = 0;
    m_b3 = 1;
    m_b4 = 1;
    m_b5 = 1;
    m_nName = 0xffffffff;
    m_nDescription = 0xffffffff;
}

// FUNCTION: SWKOTOR 0x005CD8D0
CExoString CSWSkill::GetNameText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nName, &strres, g_nGender);
    return strres.szString;
}

// FUNCTION: SWKOTOR 0x005CD970
CExoString CSWSkill::GetDescriptionText()
{
    STR_RES strres;
    g_pTlkTable->GetSimpleString(m_nDescription, &strres, g_nGender);
    return strres.szString;
}

class CSWRoom {
public:
    virtual ~CSWRoom();
    Vector m_vPosition;                  // 0x04
    Quaternion m_qOrientation;           // 0x10
    CResRef m_cResRef;                   // 0x20
    unsigned long m_nEnvAudio;           // 0x30
    float m_fAmbientScale;               // 0x34
    int m_bNoRoom;                       // 0x38

    CSWRoom();
    void SetRoomInfo(const CResRef &cResRef, const Vector &vPosition);
    void SetRoomEnvAudioProps(int nCount, CExoString *pNames, unsigned long *pEnvAudio,
                              float *pAmbientScale);
};

// FUNCTION: SWKOTOR 0x005C7D10
CSWRoom::CSWRoom()
    : m_vPosition(0.0f, 0.0f, 0.0f), m_qOrientation(1.0f, 0.0f, 0.0f, 0.0f)
{
    m_nEnvAudio = 0;
    m_bNoRoom = 0;
    m_fAmbientScale = 1.0f;
}

// FUNCTION: SWKOTOR 0x005C7D00
CSWRoom::~CSWRoom()
{
}

// FUNCTION: SWKOTOR 0x005C7D70
void CSWRoom::SetRoomInfo(const CResRef &cResRef, const Vector &vPosition)
{
    m_bNoRoom = cResRef == "****";
    m_cResRef = cResRef;
    m_vPosition = vPosition;
}

// FUNCTION: SWKOTOR 0x005C7DC0
void CSWRoom::SetRoomEnvAudioProps(int nCount, CExoString *pNames, unsigned long *pEnvAudio,
                                   float *pAmbientScale)
{
    if (m_bNoRoom != 0)
        return;
    CExoString sName;
    m_cResRef.CopyToString(sName);
    int i;
    for (i = 0; i < nCount; i++) {
        if (sName.CompareNoCase(pNames[i].CStr()))
            break;
    }
    if (i == nCount)
        return;
    m_nEnvAudio = pEnvAudio[i];
    m_fAmbientScale = pAmbientScale[i];
}
