// CSWCItem (client item, 0x1A8 bytes): CSWItem at +0x00 (vtable 0x00751BA0)
// and CSWCObject at +0x10 (vtable 0x00751A58). Self-contained layout for the
// item lifetime / model / power family (0x00646230-0x006474B0). The CSWCObject
// overrides take the CSWCObject sub-object as `this`, like the original.
// Slot names follow the playable build's vtable table
// (src/units/1460/6af36d5f93-w3_client_player_model.cpp) and bucket k's
// CSWCObject/CSWCAnimBase views.
#ifndef M_SW_C_ITEM_OBJECT_H
#define M_SW_C_ITEM_OBJECT_H

extern "C" void __cdecl free(void* p);
extern "C" void* __cdecl memset(void* p, int c, unsigned int n);
#pragma intrinsic(memset)

#define OBJECT_INVALID 0x7f000000

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;  // 0x005E5D10
    int GetLength() const;                          // 0x005E5790
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef(const CExoString& s);                   // 0x00406D60
};

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

struct Quaternion {
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};

float GetFacing(const Vector& v);                                   // 0x004AA0F0
Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);  // 0x004ACAC0
Quaternion RotationArc(Vector vFrom, Vector vTo);                   // 0x004AB630
Quaternion operator*(const Quaternion& a, const Quaternion& b);     // 0x004A9AC0

extern const float g_fZero;                         // 0x0073D700
extern unsigned char g_nGender;                     // 0x0073ECD0
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char g_nObjectArraySuccess;   // 0x0074666C
extern int g_bItemGroundAlign;                      // 0x00833A98 (descriptive)
extern int g_bHitStatic;                            // 0x007FBF4C

struct __declspec(align(8)) STR_RES {
    CExoString m_sText;                             // 0x00
    CResRef m_cSound;                               // 0x08
    float m_fSoundLength;                           // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& res, unsigned char nGender);  // 0x0041E550
};
extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

// Collaborators ----------------------------------------------------------

class CSWCObject;
class CSWItem;

class CAurObject {
public:
    virtual ~CAurObject();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* sName, float fSpeed, int nFlags, int nUnused);  // +0x18
    virtual int GetAnimationTime(const char* sName, float* pTime, int nUnused);  // +0x1c (descriptive)
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetVisible(int bVisible);                                   // +0x4c
    virtual void AttachToObject(CAurObject* pParent, const char* sNode, int n);  // +0x50
    virtual void v54(); virtual void v58(); virtual void v5C(); virtual void v60();
    virtual Vector GetPosition();                                            // +0x64
    virtual void v68(); virtual void v6C(); virtual void v70();
    virtual void SetClientObject(void* pObject);                             // +0x74
    virtual void v78();
    virtual void SetObjectType(int nType);                                   // +0x7c
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0();
    virtual void SetPickMode(int nMode);                                     // +0xc4 (descriptive)
};

class CSWCAnimBase {
public:
    virtual ~CSWCAnimBase();
    virtual void v04();
    virtual CAurObject* GetModel(unsigned char nPart);                      // +0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);     // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50();
    virtual int PlayAnim(int nAnimation, float fSpeed, int nFlags, int n);  // +0x54
    virtual void v58(); virtual void v5C();
    virtual void SetOrientation(const Quaternion& q);                       // +0x60
    virtual void v64(); virtual void v68(); virtual void v6C(); virtual void v70();
    virtual void UnloadPart(unsigned char nPart);                           // +0x74 (descriptive)
    virtual void UnloadModel();                                             // +0x78

    CSWCAnimBase();                                 // 0x0069DFB0
    char pad_0x4[0xf0 - 0x4];
};

class CExoSoundSource {
public:
    CExoSoundSource(const CResRef& cRes);           // 0x005D60E0
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char n);         // 0x005D5900
    void Set3D(int b);                              // 0x005D5910
    unsigned long Play();                           // 0x005D5930
    void SetLooping(int b);                         // 0x005D59D0
    void SetPosition(const Vector& v, int n);       // 0x005D59E0
    void Stop();                                    // 0x005D5A20

    void* m_pInternal;                              // 0x04
};

class CExoSound {
public:
    void PlaySoundEffect(const CResRef& cRes, Vector vPos, int a, unsigned char nGroup,
                         int b, int c, int d, int e);   // 0x005D5E10
};
extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CSWBaseItem {
public:
    unsigned long m_nNameStrRef;                    // 0x00
    char pad_0x4[0x88 - 0x4];
    unsigned char m_nModelType;                     // 0x88 (placeholder name)
    char pad_0x89[0x90 - 0x89];
    int m_bPowered;                                 // 0x90 (placeholder name)
    CExoString m_sPoweredSound;                     // 0x94 (placeholder name)
    CExoString m_sPowerUpSound;                     // 0x9c (placeholder name)
    CExoString m_sPowerDownSound;                   // 0xa4 (placeholder name)
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34
};
extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCArea {
public:
    char pad_0x0[0x10c];
    char m_cSurface[0x264 - 0x10c];                 // 0x10c (placeholder name)
    int m_bLoading;                                 // 0x264 (placeholder name)
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CSWCCreature {
public:
    char pad_0x0[0x3d8];
    int m_n3d8;                                     // 0x3d8 (placeholder name)
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTick);   // 0x004ADE40
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTick1,
                           unsigned long nDay2, unsigned long nTick2,
                           unsigned long* pDay, unsigned long* pTick);   // 0x004AE460
};

class CGameObject;
class CGameObjectArray {
public:
    unsigned char Delete(unsigned long oid, CGameObject** ppObject);  // 0x004D81A0
};

class CClientAIList {
public:
    int RemoveObject(CSWCObject* pObject);          // 0x00604810
};

class CClientAIMaster {
public:
    void* m_pVtable;
    CClientAIList m_List;                           // 0x04
    int RemoveObject(CSWCObject* pObject);          // 0x00604A30
    CClientAIList* GetAIList() { return &m_List; }
};

class CGuiInGame {
public:
    int IsGlobalFading();                           // 0x0062AC60
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);  // 0x005ED5B0
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
    int GetAreaNotReadyToDisplay();                 // 0x005EDAD0
};

class CServerExoApp {
public:
    float ComputeHeight(void* pSurface, Vector* pPosition, int nFlags);  // 0x004AEA50
};

class CAppManager {
public:
    void* m_pVtable;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

// Item classes -----------------------------------------------------------

class CSWItem {
public:
    CSWItem();                                      // 0x005B4660
    virtual ~CSWItem();                             // 0x005B4680
    CSWBaseItem* GetBaseItem()
    {
        int nBaseItem = m_nBaseItem;
        return g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
    }

    unsigned char m_a4[6];                          // 0x04
    char pad_0xa[2];
    int m_nBaseItem;                                // 0x0c
};

class CSWCObject {
public:
    CSWCObject(unsigned char nObjectType);          // 0x0063E530
    virtual ~CSWCObject();                          // 0x0063E800
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30();
    virtual CSWItem* AsSWItem();                                            // +0x34
    virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void UpdateAI();                                                // +0x74
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart, int n);  // +0x78
    virtual void UnloadModel();                                             // +0x7c
    virtual void v80();
    virtual void SetArea(CSWCArea* pArea);                                  // +0x84
    virtual void SetOrientation(const Vector& v);                           // +0x88
    virtual void SetPosition(const Vector& v);                              // +0x8c
    virtual void v90(); virtual void v94();
    virtual CAurObject* GetModel(unsigned char nPart, int b);               // +0x98
    virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4();
    virtual void UpdatePopUpBubble();                                       // +0xe8

    CSWCAnimBase* GetAnimationBase();               // 0x0063C070
    void DestroyAnimationBase()                     // inline (descriptive)
    {
        if (m_pModel) {
            m_pModel->UnloadModel();
            delete m_pModel;
            m_pModel = 0;
        }
    }
    int FadeObject();                               // 0x0063DF60

    unsigned long m_oid;                            // 0x04
    char pad_0x8[0x20 - 0x8];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
    unsigned long m_nDay;                           // 0x3c
    unsigned long m_nTick;                          // 0x40
    unsigned long m_nElapsed;                       // 0x44
    char pad_0x48[0x68 - 0x48];
    CSWCAnimBase* m_pModel;                         // 0x68
    char pad_0x6c[0xa4 - 0x6c];
    float m_fCanSeeHeight;                          // 0xa4
    char pad_0xa8[0xfc - 0xa8];
};

class CSWCItem : public CSWItem, public CSWCObject {
public:
    CSWCItem(unsigned long nBaseItem);
    virtual ~CSWCItem();

    virtual CSWItem* AsSWItem();
    virtual void UpdateAI();
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart, int n);
    virtual void SetArea(CSWCArea* pArea);
    virtual void SetOrientation(const Vector& v);
    virtual void SetPosition(const Vector& v);

    CSWCCreature* GetEquippedBy()
    {
        if (m_oidEquippedBy == OBJECT_INVALID) {
            return 0;
        }
        return g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(m_oidEquippedBy);
    }
    float GetGroundHeight(Vector* pPosition, float fDefault)   // inline (descriptive)
    {
        if (m_pArea) {
            return g_pAppManager->m_pServerExoApp->ComputeHeight(m_pArea->m_cSurface, pPosition, 1);
        }
        return fDefault;
    }
    void AttachAuxiliaryModel();                    // 0x00646830
    void AlignToNormal(const Vector& vNormal);      // 0x00646A30 (descriptive)
    void SetEquippedModelFlags(int bEnabled);       // 0x00646BA0
    CExoString GetName(int bFullName);
    void PowerItem(int bPowered, int bForce, int bAnimate, int bQuiet);

    int m_n10c;                                     // 0x10c
    unsigned char m_nArmorType;                     // 0x110
    char pad_0x111[3];
    int m_bIdentified;                              // 0x114
    char pad_0x118[4];
    unsigned char m_n11c;                           // 0x11c
    unsigned char m_n11d;                           // 0x11d
    char pad_0x11e[2];
    unsigned long m_nNumActiveProperties;           // 0x120
    void* m_pActiveProperties;                      // 0x124
    CResRef m_cModel;                               // 0x128
    CResRef m_cIcon;                                // 0x138
    int m_n148;                                     // 0x148
    int m_n14c;                                     // 0x14c
    int m_n150;                                     // 0x150
    int m_n154;                                     // 0x154
    int m_n158;                                     // 0x158
    unsigned char m_n15c;                           // 0x15c
    unsigned char m_n15d;                           // 0x15d
    unsigned char m_n15e;                           // 0x15e
    unsigned char m_n15f;                           // 0x15f
    unsigned long m_oidEquippedBy;                  // 0x160
    unsigned long m_oid164;                         // 0x164
    int m_n168;                                     // 0x168
    unsigned short m_nPropertyUses[8];              // 0x16c
    unsigned char m_nActivePropertyIndex;           // 0x17c
    char pad_0x17d[7];
    int m_nStackSize;                               // 0x184
    int m_n188;                                     // 0x188
    CExoString m_sDescription;                      // 0x18c
    CAurObject* m_pSpecialAttack;                   // 0x194
    int m_n198;                                     // 0x198
    int m_bPowered;                                 // 0x19c
    int m_n1a0;                                     // 0x1a0
    CExoSoundSource* m_pPowerSound;                 // 0x1a4
};

typedef char MItemObjectSize[(sizeof(CSWCObject) == 0xfc) ? 1 : -1];
typedef char MItemSize[(sizeof(CSWCItem) == 0x1a8) ? 1 : -1];

#endif
