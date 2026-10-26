// Small CSWCObject / CSWCCreature virtual leaves at 0x00616FA0-0x006170C0
// (worker C s3). Only 0x00616FA0 is in functions.csv; the other starts are
// real vtable targets (slot offsets from the playable vtable tables). They
// are declared non-virtual here: only their bodies are compared.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    ~CExoString();                                  // 0x005E5C20
};

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char* s);                         // 0x00406D80
};

class CSWPortrait {
public:
    CResRef GetPortrait(unsigned char nIndex);      // 0x00678EF0
};

class CSWCCreatureStats {
public:
    CExoString GetFullName();                       // 0x00647CE0 (descriptive)
    CResRef GetPortrait(int nIndex);                // 0x00647E80
    char pad_0x0[0x70];
    CSWPortrait m_cPortrait;                        // 0x70
    char pad_0x71[0xab];
    unsigned short m_nPortraitId;                   // 0x11c
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8();
    virtual void SetShadowBlob(int bHidden);        // 0xac
    virtual void HideShadowBlob();                  // 0xb0
};

class CSWCObject {
public:
    void SetId(unsigned long oid);
    void ShowShadowBlob();
    void HideShadowBlob();
    float GetDefaultValueOnePointFive();

    void* m_pVtable;                                // 0x00
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x60];
    CSWCAnimBase* m_pAnimBase;                      // 0x68
};

// FUNCTION: SWKOTOR 0x00616FA0
void CSWCObject::SetId(unsigned long oid)
{
    m_idSelf = oid;
}

// FUNCTION: SWKOTOR 0x00616FB0
void CSWCObject::ShowShadowBlob()
{
    if (m_pAnimBase != 0) {
        m_pAnimBase->SetShadowBlob(0);
    }
}

// FUNCTION: SWKOTOR 0x00616FD0
void CSWCObject::HideShadowBlob()
{
    if (m_pAnimBase != 0) {
        m_pAnimBase->HideShadowBlob();
    }
}

// Descriptive name: vtable slots +0xD8/+0x140 return the constant 1.5f.
// FUNCTION: SWKOTOR 0x00616FE0
float CSWCObject::GetDefaultValueOnePointFive()
{
    return 1.5f;
}

class CSWCCreature : public CSWCObject {
public:
    CResRef GetPortrait(int nIndex);
    CResRef GetPortrait(unsigned char nIndex);
    unsigned short GetPortraitId();
    CExoString GetName();
    int IsHostile();

    char pad_0x6c[0x270];
    unsigned char m_nReputation;                    // 0x2dc (2 = hostile)
    char pad_0x2dd[0x1b];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

// FUNCTION: SWKOTOR 0x00616FF0 SYMBOL: ?GetPortrait@CSWCCreature@@QAE?AVCResRef@@H@Z
CResRef CSWCCreature::GetPortrait(int nIndex)
{
    if (m_pStats != 0) {
        return m_pStats->GetPortrait(nIndex);
    }
    return CResRef("");
}

// FUNCTION: SWKOTOR 0x00617030 SYMBOL: ?GetPortrait@CSWCCreature@@QAE?AVCResRef@@E@Z
CResRef CSWCCreature::GetPortrait(unsigned char nIndex)
{
    if (m_pStats != 0) {
        return m_pStats->m_cPortrait.GetPortrait(nIndex);
    }
    return CResRef("");
}

// FUNCTION: SWKOTOR 0x00617070
unsigned short CSWCCreature::GetPortraitId()
{
    if (m_pStats != 0) {
        return m_pStats->m_nPortraitId;
    }
    return 0xffff;
}

// FUNCTION: SWKOTOR 0x00617090
CExoString CSWCCreature::GetName()
{
    return m_pStats->GetFullName();
}

// FUNCTION: SWKOTOR 0x006170C0
int CSWCCreature::IsHostile()
{
    return m_nReputation == 2;
}
