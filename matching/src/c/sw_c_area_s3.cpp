// CSWCArea members (nwcarea.o, worker C s3) with a translation-unit local
// area layout.

struct Vector {
    float x, y, z;
};

class CSWCVisualEffect {
public:
    virtual ~CSWCVisualEffect();                    // 0x00
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88();
    virtual void SetPosition(const Vector& vPosition);  // 0x8c
    virtual void v90();
    virtual void AttachToScene(void* pScene);       // 0x94 (name unresolved)

    CSWCVisualEffect(int bFireAndForget);           // 0x0068FE70
    int LoadEffect(unsigned long nEffect);          // 0x00690010 (descriptive)
    void SetAnimation(unsigned short nAnimation);   // 0x00690510
    void Announce();                                // 0x00690940 (descriptive)

    char pad_0x4[0x114];
    int m_bUnknown118;                              // 0x118
    int m_bUnknown11c;                              // 0x11c
    char pad_0x120[0x10];
};

class CSWCArea {
public:
    int ApplyFireAndForgetVisualEffect(unsigned short nEffect, Vector vPosition);

    char pad_0x0[0x184];
    void* m_pScene;                                 // 0x184
};

// FUNCTION: SWKOTOR 0x00604C90
int CSWCArea::ApplyFireAndForgetVisualEffect(unsigned short nEffect, Vector vPosition)
{
    CSWCVisualEffect* pEffect = new CSWCVisualEffect(1);
    if (pEffect == 0) {
        return 0;
    }
    if (!pEffect->LoadEffect(nEffect)) {
        delete pEffect;
        return 0;
    }
    pEffect->AttachToScene(m_pScene);
    pEffect->m_bUnknown11c = 1;
    pEffect->m_bUnknown118 = 1;
    pEffect->SetPosition(vPosition);
    pEffect->SetAnimation(1);
    pEffect->Announce();
    return 1;
}
