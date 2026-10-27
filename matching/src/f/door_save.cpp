// CSWSDoor::SaveDoor (round s8, 0x00588AD0).

class CExoString {
public:
    ~CExoString();
    char *CStr() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    ~CExoLocString();
    void *m_pList;
    void *m_pPad;
};

class CResRef {
public:
    CResRef(const CExoString &s);
    CResRef(const char *s);
    char m_resRef[16];
};

class Vector {
public:
    float x, y, z;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCExoLocString(CResStruct *pStruct, CExoLocString &sValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldWORD(CResStruct *pStruct, unsigned short nValue, char *szLabel);
    int WriteFieldSHORT(CResStruct *pStruct, short nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
};

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual CResRef GetConversation();           // +0x80
    virtual void v84();
    virtual void v88();
    virtual CExoLocString *GetLocName();         // +0x8c
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual short GetCurrentHitPoints(int bIncludeTemp); // +0x9c
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual CResRef GetPortrait();               // +0xc8
    virtual void vcc();
    virtual unsigned short GetPortraitId();      // +0xd0

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSObject : public CGameObject {
public:
    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    char pad_0xa8[0x38];
    short m_nBaseHitPoints;          // +0xe0
    char pad_0xe2[0x16];
    unsigned char m_bPlotObject;     // +0xf8
    char pad_0xf9[0x107];
    int m_bMin1HP;                   // +0x200
    char pad_0x204[0x24];
};

class CSWSDoor : public CSWSObject {
public:
    int SaveDoor(CResGFF *pGFF, CResStruct *pStruct);
    float GetBearing() { return m_fBearing; }

    CExoString m_sScripts[15];       // +0x228
    unsigned char m_nAppearance;     // +0x2a0
    unsigned char m_nGenericType;    // +0x2a1
    char pad_0x2a2[0x12];
    float m_fBearing;                // +0x2b4
    int m_nFactionId;                // +0x2b8
    unsigned char m_nFortSave;       // +0x2bc
    unsigned char m_nReflexSave;     // +0x2bd
    unsigned char m_nWillSave;       // +0x2be
    unsigned char m_nOpenLockDC;     // +0x2bf
    unsigned char m_nCloseLockDC;    // +0x2c0
    unsigned char m_nSecretDoorDC;   // +0x2c1
    unsigned char m_nHardness;       // +0x2c2
    char pad_0x2c3[0x1];
    int m_bLocked;                   // +0x2c4
    int m_bLockable;                 // +0x2c8
    unsigned char m_nOpenState;      // +0x2cc
    char pad_0x2cd[0x3];
    CExoString m_sKeyName;           // +0x2d0
    int m_bKeyRequired;              // +0x2d8
    char pad_0x2dc[0xc];
    int m_bTrapFlag;                 // +0x2e8
    unsigned char m_nDisarmDC;       // +0x2ec
    unsigned char m_nTrapDetectDC;   // +0x2ed
    char pad_0x2ee[0x2];
    int m_bAutoRemoveKey;            // +0x2f0
    char pad_0x2f4[0x4];
    unsigned char m_nTrapType;       // +0x2f8
    char pad_0x2f9[0x3];
    int m_bTrapDisarmable;           // +0x2fc
    int m_bTrapDetectable;           // +0x300
    int m_bTrapOneShot;              // +0x304
    char pad_0x308[0x7c];
    unsigned char m_nLinkedToFlags;  // +0x384
    char pad_0x385[0x3];
    CExoString m_sLinkedTo;          // +0x388
    CExoString m_sLinkedToModule;    // +0x390
    char pad_0x398[0xc];
    CExoLocString m_sDescription;    // +0x3a4
    unsigned short m_nLoadScreenId;  // +0x3ac
    char pad_0x3ae[0x12];
    int m_bStatic;                   // +0x3c0
    char pad_0x3c4[0x4];
    CExoLocString m_sTransitionDestination; // +0x3c8
};

// FUNCTION: SWKOTOR 0x00588AD0
int CSWSDoor::SaveDoor(CResGFF *pGFF, CResStruct *pStruct)
{
    pGFF->WriteFieldDWORD(pStruct, m_nAppearance, "Appearance");
    pGFF->WriteFieldBYTE(pStruct, m_nGenericType, "GenericType");
    pGFF->WriteFieldBYTE(pStruct, m_nOpenState, "OpenState");
    pGFF->WriteFieldBYTE(pStruct, m_bAutoRemoveKey, "AutoRemoveKey");
    pGFF->WriteFieldFLOAT(pStruct, GetBearing(), "Bearing");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.x, "X");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.y, "Y");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.z, "Z");
    pGFF->WriteFieldDWORD(pStruct, m_nFactionId, "Faction");
    pGFF->WriteFieldBYTE(pStruct, m_nFortSave, "Fort");
    pGFF->WriteFieldBYTE(pStruct, m_nWillSave, "Will");
    pGFF->WriteFieldBYTE(pStruct, m_nReflexSave, "Ref");
    pGFF->WriteFieldSHORT(pStruct, m_nBaseHitPoints, "HP");
    pGFF->WriteFieldSHORT(pStruct, GetCurrentHitPoints(0), "CurrentHP");
    pGFF->WriteFieldBYTE(pStruct, m_bPlotObject, "Plot");
    pGFF->WriteFieldBYTE(pStruct, m_bMin1HP, "Min1HP");
    pGFF->WriteFieldCExoString(pStruct, m_sKeyName, "KeyName");
    pGFF->WriteFieldBYTE(pStruct, m_bKeyRequired, "KeyRequired");
    pGFF->WriteFieldBYTE(pStruct, m_nOpenLockDC, "OpenLockDC");
    pGFF->WriteFieldBYTE(pStruct, m_nCloseLockDC, "CloseLockDC");
    pGFF->WriteFieldBYTE(pStruct, m_nSecretDoorDC, "SecretDoorDC");
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldCResRef(pStruct, GetConversation(), "Conversation");
    if (GetPortraitId() == -1)
        pGFF->WriteFieldCResRef(pStruct, GetPortrait(), "Portrait");
    else
        pGFF->WriteFieldWORD(pStruct, GetPortraitId(), "PortraitId");
    pGFF->WriteFieldBYTE(pStruct, m_nHardness, "Hardness");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[1], "OnClosed");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[2], "OnDamaged");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[3], "OnDeath");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[4], "OnDisarm");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[5], "OnHeartbeat");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[6], "OnLock");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[7], "OnMeleeAttacked");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[0], "OnOpen");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[8], "OnSpellCastAt");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[9], "OnTrapTriggered");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[10], "OnUnlock");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[11], "OnUserDefined");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[12], "OnClick");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[14], "OnFailToOpen");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[13], "OnDialog");
    pGFF->WriteFieldBYTE(pStruct, m_nTrapType, "TrapType");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapDisarmable, "TrapDisarmable");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapDetectable, "TrapDetectable");
    pGFF->WriteFieldBYTE(pStruct, m_nDisarmDC, "DisarmDC");
    pGFF->WriteFieldBYTE(pStruct, m_nTrapDetectDC, "TrapDetectDC");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapFlag, "TrapFlag");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapOneShot, "TrapOneShot");
    pGFF->WriteFieldBYTE(pStruct, m_bLocked, "Locked");
    pGFF->WriteFieldBYTE(pStruct, m_bLockable, "Lockable");
    pGFF->WriteFieldBYTE(pStruct, m_nLinkedToFlags, "LinkedToFlags");
    pGFF->WriteFieldCExoString(pStruct, m_sLinkedTo, "LinkedTo");
    CResRef cModule(m_sLinkedToModule.CStr());
    pGFF->WriteFieldCResRef(pStruct, cModule, "LinkedToModule");
    pGFF->WriteFieldWORD(pStruct, m_nLoadScreenId, "LoadScreenID");
    pGFF->WriteFieldCExoLocString(pStruct, *GetLocName(), "LocName");
    pGFF->WriteFieldCExoLocString(pStruct, m_sDescription, "Description");
    pGFF->WriteFieldBYTE(pStruct, m_bStatic, "Static");
    pGFF->WriteFieldCExoLocString(pStruct, m_sTransitionDestination, "TransitionDestination");
    return 1;
}
