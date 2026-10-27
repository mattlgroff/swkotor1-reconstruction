// CSWSTrigger::SaveTrigger (round s8, 0x0058E660).

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
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    float x, y, z;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pElement, CResList *pList, unsigned long nType);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCExoLocString(CResStruct *pStruct, CExoLocString &sValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldWORD(CResStruct *pStruct, unsigned short nValue, char *szLabel);
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
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
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
    virtual CResRef GetPortrait();                 // +0xc8
    virtual void vcc();
    virtual unsigned short GetPortraitId();        // +0xd0

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
    char pad_0xa8[0x180];
};

class CSWSTrigger : public CSWSObject {
public:
    int SaveTrigger(CResGFF *pGFF, CResStruct *pStruct);
    float GetHighlightHeight() { return m_fHighlightHeight; }

    CExoLocString m_sLocName;        // +0x228
    CExoString m_sLinkedTo;          // +0x230
    CExoString m_sLinkedToModule;    // +0x238
    unsigned char m_nLinkedToFlags;  // +0x240
    char pad_0x241[0x3];
    CExoString m_sScripts[7];        // +0x244
    CExoString m_sKeyName;           // +0x27c
    int m_nVertices;                 // +0x284
    Vector *m_pvVertices;            // +0x288
    char pad_0x28c[0x28];
    int m_bIsTransition;             // +0x2b4
    int m_nFactionId;                // +0x2b8
    int m_bCircular;                 // +0x2bc
    int m_bTrapDetectable;           // +0x2c0
    int m_bTrapDisarmable;           // +0x2c4
    char pad_0x2c8[0x4];
    int m_bAutoRemoveKey;            // +0x2cc
    int m_bTrapOneShot;              // +0x2d0
    unsigned char m_nTrapType;       // +0x2d4
    char pad_0x2d5[0xb];
    float m_fHighlightHeight;        // +0x2e0
    unsigned long m_oidCreator;      // +0x2e4
    char pad_0x2e8[0x14];
    unsigned char m_nCursor;         // +0x2fc
    char pad_0x2fd[0xb];
    unsigned short m_nLoadScreenId;  // +0x308
    char pad_0x30a[0x2];
    CExoLocString m_sTransitionDestination; // +0x30c
    int m_bSetByPlayerParty;         // +0x314
};

// FUNCTION: SWKOTOR 0x0058E660
int CSWSTrigger::SaveTrigger(CResGFF *pGFF, CResStruct *pStruct)
{
    CResStruct cElement;
    CResList cList;
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[0], "ScriptHeartbeat");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[1], "ScriptOnEnter");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[2], "ScriptOnExit");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[3], "ScriptUserDefine");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[4], "OnTrapTriggered");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[5], "OnDisarm");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[6], "OnClick");
    pGFF->WriteFieldBYTE(pStruct, m_nTrapType, "TrapType");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapOneShot, "TrapOneShot");
    pGFF->WriteFieldDWORD(pStruct, m_oidCreator, "CreatorId");
    pGFF->WriteFieldCExoString(pStruct, m_sLinkedTo, "LinkedTo");
    pGFF->WriteFieldBYTE(pStruct, m_nLinkedToFlags, "LinkedToFlags");
    CResRef cModule(m_sLinkedToModule.CStr());
    pGFF->WriteFieldCResRef(pStruct, cModule, "LinkedToModule");
    pGFF->WriteFieldBYTE(pStruct, m_bAutoRemoveKey, "AutoRemoveKey");
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldCExoLocString(pStruct, m_sLocName, "LocalizedName");
    pGFF->WriteFieldDWORD(pStruct, m_nFactionId, "Faction");
    pGFF->WriteFieldBYTE(pStruct, m_nCursor, "Cursor");
    pGFF->WriteFieldCExoString(pStruct, m_sKeyName, "KeyName");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapDisarmable, "TrapDisarmable");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapDetectable, "TrapDetectable");
    if (GetPortraitId() == 0xffff)
        pGFF->WriteFieldCResRef(pStruct, GetPortrait(), "Portrait");
    else
        pGFF->WriteFieldWORD(pStruct, GetPortraitId(), "PortraitId");
    int nType = 0;
    if (m_bIsTransition)
        nType = 1;
    else if (m_bCircular)
        nType = 2;
    pGFF->WriteFieldINT(pStruct, nType, "Type");
    pGFF->WriteFieldFLOAT(pStruct, GetHighlightHeight(), "HighlightHeight");
    Vector v = m_vPosition;
    pGFF->WriteFieldFLOAT(pStruct, v.x, "XPosition");
    pGFF->WriteFieldFLOAT(pStruct, v.y, "YPosition");
    pGFF->WriteFieldFLOAT(pStruct, v.z, "ZPosition");
    v = m_vOrientation;
    pGFF->WriteFieldFLOAT(pStruct, v.x, "XOrientation");
    pGFF->WriteFieldFLOAT(pStruct, v.y, "YOrientation");
    pGFF->WriteFieldFLOAT(pStruct, v.z, "ZOrientation");
    Vector vOrigin = m_vPosition;
    pGFF->AddList(&cList, pStruct, "Geometry");
    for (int i = 0; i < m_nVertices; i++) {
        pGFF->AddListElement(&cElement, &cList, 3);
        Vector vPoint = m_pvVertices[i] - vOrigin;
        pGFF->WriteFieldFLOAT(&cElement, vPoint.x, "PointX");
        pGFF->WriteFieldFLOAT(&cElement, vPoint.y, "PointY");
        pGFF->WriteFieldFLOAT(&cElement, vPoint.z, "PointZ");
    }
    pGFF->WriteFieldWORD(pStruct, m_nLoadScreenId, "LoadScreenID");
    pGFF->WriteFieldCExoLocString(pStruct, m_sTransitionDestination, "TransitionDestination");
    pGFF->WriteFieldBYTE(pStruct, m_bSetByPlayerParty, "SetByPlayerParty");
    return 1;
}
