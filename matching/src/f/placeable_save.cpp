// CSWSPlaceable::SavePlaceable (round s8, 0x00586A70).

class CExoString {
public:
    ~CExoString();
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
    char m_resRef[16];
};

class Vector {
public:
    float x, y, z;
};

float GetFacing(const Vector &vOrientation);

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF;

class CSWSItem {
public:
    int SaveItem(CResGFF *pGFF, CResStruct *pStruct);
    char pad_0x0[0x14];
    unsigned long m_nObjectId;       // +0x14
};

class CItemRepository {
public:
    CSWSItem *ItemListGetItem(unsigned long nIndex);
    char pad_0x0[0x10];
    unsigned long m_nItemCount;      // +0x10
};

class CResGFF {
public:
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pElement, CResList *pList, unsigned long nType);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCExoLocString(CResStruct *pStruct, CExoLocString &sValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldWORD(CResStruct *pStruct, unsigned short nValue, char *szLabel);
    int WriteFieldSHORT(CResStruct *pStruct, short nValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
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
    char pad_0xa8[0x2c];
    int m_nAnimation;                // +0xd4
    char pad_0xd8[0x8];
    short m_nBaseHitPoints;          // +0xe0
    char pad_0xe2[0x16];
    unsigned char m_bPlotObject;     // +0xf8
    char pad_0xf9[0x107];
    int m_bMin1HP;                   // +0x200
    char pad_0x204[0x14];
    int m_bPartyInteract;            // +0x218
    char pad_0x21c[0xc];
};

class CSWSPlaceable : public CSWSObject {
public:
    int SavePlaceable(CResGFF *pGFF, CResStruct *pStruct);
    int GetLightIsOn();

    CExoLocString m_sLocName;        // +0x228
    unsigned char m_nAppearance;     // +0x230
    char pad_0x231[0x3];
    CExoLocString m_sDescription;    // +0x234
    int m_nFactionId;                // +0x23c
    char pad_0x240[0x14];
    int m_bGroundPile;               // +0x254
    unsigned char m_nHardness;       // +0x258
    char pad_0x259[0x3];
    float m_fBearing;                // +0x25c
    int m_bLocked;                   // +0x260
    CExoString m_sKeyName;           // +0x264
    int m_bKeyRequired;              // +0x26c
    int m_bAutoRemoveKey;            // +0x270
    unsigned char m_nOpenLockDC;     // +0x274
    unsigned char m_nCloseLockDC;    // +0x275
    unsigned char m_nTrapDetectDC;   // +0x276
    char pad_0x277[0x1];
    int m_bTrapFlag;                 // +0x278
    unsigned char m_nDisarmDC;       // +0x27c
    char pad_0x27d[0x3];
    int m_bTrapDisarmable;           // +0x280
    int m_bTrapDetectable;           // +0x284
    int m_bTrapOneShot;              // +0x288
    char pad_0x28c[0x4];
    unsigned char m_nTrapType;       // +0x290
    char pad_0x291[0x3];
    CExoString m_sScripts[16];       // +0x294
    unsigned char m_nFortSave;       // +0x314
    unsigned char m_nWillSave;       // +0x315
    unsigned char m_nReflexSave;     // +0x316
    char pad_0x317[0xd];
    int m_bHasInventory;             // +0x324
    int m_bUseable;                  // +0x328
    char pad_0x32c[0x4];
    int m_bLockable;                 // +0x330
    int m_bDieWhenEmpty;             // +0x334
    int m_bOpen;                     // +0x338
    char pad_0x33c[0x30];
    CItemRepository *m_pItemRepository; // +0x36c
    char pad_0x370[0x24];
    unsigned char m_nBodyBag;        // +0x394
    char pad_0x395[0x3];
    int m_bStatic;                   // +0x398
    char pad_0x39c[0xa4];
    int m_bIsBodyBag;                // +0x440
    int m_bIsBodyBagVisible;         // +0x444
    char pad_0x448[0x4];
    int m_bIsCorpse;                 // +0x44c
};

// NONMATCH (70): the bearing float lands in the pGFF argument slot in the
// original and the call's ecx load is scheduled earlier; everything else matches.
// FUNCTION: SWKOTOR 0x00586A70
int CSWSPlaceable::SavePlaceable(CResGFF *pGFF, CResStruct *pStruct)
{
    CResList cList;
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldCExoLocString(pStruct, *GetLocName(), "LocName");
    pGFF->WriteFieldBYTE(pStruct, m_bAutoRemoveKey, "AutoRemoveKey");
    pGFF->WriteFieldDWORD(pStruct, m_nFactionId, "Faction");
    pGFF->WriteFieldBYTE(pStruct, m_bPlotObject, "Plot");
    pGFF->WriteFieldBYTE(pStruct, m_bMin1HP, "Min1HP");
    pGFF->WriteFieldBYTE(pStruct, m_nOpenLockDC, "OpenLockDC");
    pGFF->WriteFieldCExoString(pStruct, m_sKeyName, "KeyName");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapDisarmable, "TrapDisarmable");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapDetectable, "TrapDetectable");
    pGFF->WriteFieldBYTE(pStruct, m_nDisarmDC, "DisarmDC");
    pGFF->WriteFieldBYTE(pStruct, m_nTrapDetectDC, "TrapDetectDC");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapFlag, "TrapFlag");
    pGFF->WriteFieldBYTE(pStruct, m_bTrapOneShot, "TrapOneShot");
    pGFF->WriteFieldBYTE(pStruct, m_nTrapType, "TrapType");
    pGFF->WriteFieldBYTE(pStruct, m_bUseable, "Useable");
    pGFF->WriteFieldBYTE(pStruct, m_bStatic, "Static");
    pGFF->WriteFieldBYTE(pStruct, m_bGroundPile, "GroundPile");
    pGFF->WriteFieldDWORD(pStruct, m_nAppearance, "Appearance");
    pGFF->WriteFieldSHORT(pStruct, m_nBaseHitPoints, "HP");
    pGFF->WriteFieldSHORT(pStruct, GetCurrentHitPoints(1), "CurrentHP");
    pGFF->WriteFieldBYTE(pStruct, m_nHardness, "Hardness");
    pGFF->WriteFieldBYTE(pStruct, m_nFortSave, "Fort");
    pGFF->WriteFieldBYTE(pStruct, m_nWillSave, "Will");
    pGFF->WriteFieldBYTE(pStruct, m_nReflexSave, "Ref");
    pGFF->WriteFieldBYTE(pStruct, m_bLockable, "Lockable");
    pGFF->WriteFieldBYTE(pStruct, m_bLocked, "Locked");
    pGFF->WriteFieldBYTE(pStruct, m_bHasInventory, "HasInventory");
    pGFF->WriteFieldBYTE(pStruct, m_bKeyRequired, "KeyRequired");
    pGFF->WriteFieldBYTE(pStruct, m_nCloseLockDC, "CloseLockDC");
    pGFF->WriteFieldBYTE(pStruct, m_bOpen, "Open");
    pGFF->WriteFieldBYTE(pStruct, m_bPartyInteract, "PartyInteract");
    if (GetPortraitId() == 0xffff)
        pGFF->WriteFieldCResRef(pStruct, GetPortrait(), "Portrait");
    else
        pGFF->WriteFieldWORD(pStruct, GetPortraitId(), "PortraitId");
    pGFF->WriteFieldCResRef(pStruct, GetConversation(), "Conversation");
    pGFF->WriteFieldBYTE(pStruct, m_nBodyBag, "BodyBag");
    pGFF->WriteFieldBYTE(pStruct, m_bDieWhenEmpty, "DieWhenEmpty");
    pGFF->WriteFieldBYTE(pStruct, GetLightIsOn(), "LightState");
    pGFF->WriteFieldCExoLocString(pStruct, m_sDescription, "Description");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[0], "OnClosed");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[1], "OnDamaged");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[2], "OnDeath");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[3], "OnDisarm");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[4], "OnHeartbeat");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[5], "OnInvDisturbed");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[6], "OnLock");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[7], "OnMeleeAttacked");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[8], "OnOpen");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[9], "OnSpellCastAt");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[11], "OnUnlock");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[12], "OnUsed");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[13], "OnUserDefined");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[14], "OnDialog");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[15], "OnEndDialogue");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[10], "OnTrapTriggered");
    pGFF->WriteFieldINT(pStruct, m_nAnimation, "Animation");
    if (m_pItemRepository->m_nItemCount > 0) {
        pGFF->AddList(&cList, pStruct, "ItemList");
        for (unsigned long i = 0; i < m_pItemRepository->m_nItemCount; i++) {
            CResStruct cElement;
            pGFF->AddListElement(&cElement, &cList, 0);
            CSWSItem *pItem = m_pItemRepository->ItemListGetItem(i);
            pGFF->WriteFieldDWORD(&cElement, pItem->m_nObjectId, "ObjectId");
            pItem->SaveItem(pGFF, &cElement);
        }
    }
    float fBearing = GetFacing(m_vOrientation) * 0.017453292f;
    m_fBearing = fBearing;
    pGFF->WriteFieldFLOAT(pStruct, fBearing, "Bearing");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.x, "X");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.y, "Y");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.z, "Z");
    pGFF->WriteFieldBYTE(pStruct, m_bIsBodyBag, "IsBodyBag");
    pGFF->WriteFieldBYTE(pStruct, m_bIsBodyBagVisible, "IsBodyBagVisible");
    pGFF->WriteFieldBYTE(pStruct, m_bIsCorpse, "IsCorpse");
    return 1;
}
