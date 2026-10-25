// CSWCDoor transition flag and destructor helpers.

class CSWCDoor {
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
    virtual void v90(); virtual void v94();
    virtual class CSWCDoorTransition* GetTransitionData(int, int); // +0x98
    virtual void v9c(); virtual void va0(); virtual void va4(); virtual void va8();
    virtual void vac(); virtual void vb0();
    char pad_0x4[0x128];
    int m_bIsAreaTransition;                              // +0x12c
    void SetIsAreaTransition(int b);
};

class CSWCDoorTransition {
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
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vB0(const char*);
};

// FUNCTION: SWKOTOR 0x00683CC0
void CSWCDoor::SetIsAreaTransition(int b)
{
    if (b == 0) {
        CSWCDoorTransition* pTransition = GetTransitionData(0xff, 1);
        if (pTransition != 0)
            pTransition->vB0((const char*)0x00753aa8);
    }
    m_bIsAreaTransition = b;
}
