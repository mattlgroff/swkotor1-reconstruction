// Small CSWCObject visual-effect helpers whose call slots are corroborated by
// the playable reconstruction and the adjacent worker-L class declarations.
class CAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual unsigned int v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual unsigned int vA0();
    virtual void vA4(); virtual void vA8(); virtual void vAC(); virtual void vB0();
    virtual void vB4(); virtual void vB8(); virtual void vBC(); virtual void vC0();
    virtual void vC4(); virtual void vC8(); virtual void vCC(); virtual void vD0();
    virtual void vD4(); virtual void vD8(); virtual void vDC(); virtual void vE0();
    virtual void vE4(); virtual void vE8();
};

class CSWCObject {
public:
    char pad_0x0[0x68];
    CAnimBase* m_pAnimation;
    unsigned int ClearSelfIllumination();
    unsigned int RefreshSelfIllumination();
    void StartFlash();
    void StopFlash();
};

// FUNCTION: SWKOTOR 0x0063CE10
unsigned int CSWCObject::ClearSelfIllumination()
{
    if (!m_pAnimation)
        return 0;
    return m_pAnimation->vA0();
}

// FUNCTION: SWKOTOR 0x0063CE30
unsigned int CSWCObject::RefreshSelfIllumination()
{
    if (!m_pAnimation)
        return 0;
    return m_pAnimation->v24();
}

// FUNCTION: SWKOTOR 0x0063D540
void CSWCObject::StartFlash()
{
    if (m_pAnimation) {
        m_pAnimation->vE4();
    }
}

// FUNCTION: SWKOTOR 0x0063D550
void CSWCObject::StopFlash()
{
    if (m_pAnimation) {
        m_pAnimation->vE8();
    }
}
