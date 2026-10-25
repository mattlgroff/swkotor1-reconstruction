// CSWCObject animation/model forwarding leaves, checked against the exact
// decorated symbols and binary vtable offsets.
class CResRef {
public:
    char m_sResRef[16];
    CResRef(const CResRef& cResRef);
};

class CAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void ReplaceTextures(const CResRef& cTexture,
                                 const CResRef& cBump, int nStore); // +0x18
    virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void ReplaceTexture(const CResRef& cTexture,
                                float fTime); // +0x28
    virtual void v2C(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3C(); virtual void v40(); virtual void v44(); virtual void v48();
    virtual void v4C(); virtual void v50(); virtual void v54(); virtual void v58();
    virtual void v5C(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6C(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7C(); virtual void v80(); virtual void v84(); virtual void v88();
    virtual void v8C(); virtual void v90();
    virtual void SetAlpha(float fAlpha, unsigned char nMask); // +0x94
};

class CSWCObject {
public:
    char pad_0x0[0x68];
    CAnimBase* m_pAnimation;                          // +0x68

    void SetAlpha(float fAlpha, unsigned char nMask);
    void ReplaceTextures(const CResRef& cTexture, const CResRef& cBump,
                         int nStore);
    void ReplaceTexture(CResRef cTexture, float fTime);
};

// FUNCTION: SWKOTOR 0x0063CB50
void CSWCObject::SetAlpha(float fAlpha, unsigned char nMask)
{
    if (m_pAnimation)
        m_pAnimation->SetAlpha(fAlpha, nMask);
}

// FUNCTION: SWKOTOR 0x0063C0C0
void CSWCObject::ReplaceTextures(const CResRef& cTexture,
                                 const CResRef& cBump, int nStore)
{
    if (m_pAnimation)
        m_pAnimation->ReplaceTextures(cTexture, cBump, nStore);
}

// FUNCTION: SWKOTOR 0x0063D440
void CSWCObject::ReplaceTexture(CResRef cTexture, float fTime)
{
    if (m_pAnimation)
        m_pAnimation->ReplaceTexture(cTexture, fTime);
}
