// Worker L matching headers: the client projectile CSWCProjectile
// (nwcprojectile.o, vtable 0x00757CC8, 0x006D2C30-0x006D7BC0) and the
// client classes it touches. The CGameObject/CSWCObject view follows
// worker N's n/sw_c_object.h; only the members used here are named.
// Projectile member and behaviour-entry names are descriptive unless a
// comment cites a symbol.
#ifndef L_SW_C_PROJECTILE_H
#define L_SW_C_PROJECTILE_H

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
};

struct Quaternion {
    float w, x, y, z;
};

Vector NormalizeVector(const Vector& v);            // 0x004AB180
float GetYaw(const Vector& vDirection);             // 0x004AA0F0
float Pitch(const Vector& vDirection);              // 0x004AA130
Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);  // 0x004ACAC0

inline float MagnitudeSquared(const Vector& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

extern "C" int __cdecl rand(void);

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    int operator==(const char* s) const;            // 0x005E5290
    int IsEmpty() const;                            // 0x005E58A0
    char* CStr() const;                             // 0x005E5670
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef operator=(const CResRef& r);            // 0x00406120
    CResRef operator=(const char* s);               // 0x00406290
    void CopyToString(char* pStr) const;            // 0x00405FB0
};

extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern const unsigned char OBJECT_TYPE_PROJECTILE;  // 0x00746637
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

// Linked list ---------------------------------------------------------------

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;
    CExoLinkedListNode* pTail;
    unsigned long nCount;

    CExoLinkedListInternal();                       // 0x005E9AA0
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80
    void* AddTail(void* p);                         // 0x005E9DC0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;

    CExoLinkedList();
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    void* AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

template <class T>
CExoLinkedList<T>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal;
}

// Collaborators -------------------------------------------------------------

class CSWCObject;

class CAurObject {
public:
    virtual ~CAurObject();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int a, int b);   // 0x18
    virtual void v1C(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2C(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3C(); virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetVisible(int bVisible);          // 0x4c
    virtual void AttachToObject(CAurObject* pTo, const char* szNode, int n);  // 0x50
    virtual void v54(); virtual void v58(); virtual void v5C();
    virtual Quaternion SetOrientation(Quaternion q);    // 0x60
    virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void SetClientObject(CSWCObject* pObject);  // 0x74 (descriptive)
    virtual void v78();
    virtual void SetObjectType(int nType);              // 0x7c (descriptive)
};

CAurObject* NewCAurObject(const char* szModel, const char* szName, int a, int b);  // 0x00449CC0

// Client animation base (vtable 0x00754F60), 0xF0 bytes; layout as in
// l/sw_c_anim_base_objects.h.
class CSWCAnimBase {
public:
    CSWCAnimBase();                                 // 0x0069DFB0
    virtual ~CSWCAnimBase();                        // 0x0069E140
    virtual void SetVisible(int bVisible);          // 0x04
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50();
    virtual int PlayAnimation(unsigned short nAnim, float fSpeed, int a, int nTransition);  // 0x54
    virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual int SetFacing(const Vector& vDirection);    // 0x64 (0x0069D5D0)
    virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void UnloadModelPart(int nPart);        // 0x74 (descriptive)
    virtual void UnloadModel();                     // 0x78 (0x0069D740)
    virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8();
    virtual void AttachShadow();                    // 0xbc (descriptive)

    char pad_0x4[0x2c - 0x4];
    CResRef m_cModelResRef;                         // 0x2c
    int m_bReplaceTextureIndexed;                   // 0x3c
    char pad_0x40[0xb8 - 0x40];
    CAurObject* m_pModel;                           // 0xb8
    char pad_0xbc[0xc4 - 0xbc];
    unsigned char m_nAnimType;                      // 0xc4
    char pad_0xc5[0xf0 - 0xc5];
};

// Projectile model animation base (vtable 0x00759170).
class CSWCAnimBaseProjectile : public CSWCAnimBase {
public:
    CSWCAnimBaseProjectile();                       // 0x006EA470
    virtual ~CSWCAnimBaseProjectile();              // 0x006EA490
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c (0x006EA4E0)
    virtual int PlayAnimation(unsigned short nAnim, float fSpeed, int a, int nTransition);  // 0x54 (0x006EA550)
    virtual int SetFacing(const Vector& vDirection);    // 0x64 (0x006EA6A0)
};

class CSWSpell {
public:
    char pad_0x0[0x10d];
    CResRef m_cProjectileModel;                     // 0x10d (descriptive)
    unsigned char m_n11D;                           // 0x11d (descriptive)
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray* m_pSpellArray;                   // 0x8c
};
extern CSWRules* g_pRules;                          // 0x007A3A28

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);  // 0x004ADE40
};

class CGameObject;

class CGameObjectArray {
public:
    unsigned char AddInternalObject(unsigned long* pId, CGameObject* pObject, int b);  // 0x004D80E0
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);   // 0x004D8230
    unsigned char Delete(unsigned long oid);        // 0x004D8290
};

class CClientAIMaster {
public:
    int AddObject(CSWCObject* pObject);             // 0x00604A20
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
};

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCDoor;
class CSWCPlaceable;

class CGameObject {
public:
    virtual ~CGameObject() {}
    virtual void SetId(unsigned long oid);          // 0x04
    virtual void v08(); virtual void v0C(); virtual void v10();
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CExoString GetName();                   // 0x70

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCObject : public CGameObject {
public:
    CSWCObject(unsigned char nObjectType);          // 0x0063E530
    virtual ~CSWCObject();                          // 0x0063E800
    virtual CExoString GetName();                   // 0x70
    virtual void AIUpdate() = 0;                    // 0x74
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart, int n) = 0;  // 0x78
    virtual void UnloadModel(int nPart) = 0;        // 0x7c
    virtual void ReleaseModel() = 0;                // 0x80 (descriptive)
    virtual void SetArea(void* pArea);              // 0x84
    virtual void SetOrientation(const Vector& v);   // 0x88
    virtual void SetPosition(const Vector& v);      // 0x8c

    char pad_0xc[0x24 - 0xc];
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
    unsigned long m_nCreateDay;                     // 0x3c (descriptive)
    unsigned long m_nCreateTime;                    // 0x40 (descriptive)
    unsigned long m_nDeltaTime;                     // 0x44
    unsigned short m_nCurrentAnimation;             // 0x48
    char pad_0x4a[0x68 - 0x4a];
    CSWCAnimBase* m_pAnimationBase;                 // 0x68
    char pad_0x6c[0xfc - 0x6c];
};

class CSWCPlaceable : public CSWCObject {
public:
    char pad_0xfc[0x144 - 0xfc];
    CExoString m_sModelName;                        // 0x144 (descriptive)
};

class CSWCDoor : public CSWCObject {
public:
    char pad_0xfc[0x130 - 0xfc];
    CExoString m_sModelName;                        // 0x130 (descriptive)
};

// Projectile -----------------------------------------------------------------

// One leg of the projectile's flight path (operator new 0x34).
struct CSWCProjectileBehavior {
    Vector m_vStart;                                // 0x00
    Vector m_vEnd;                                  // 0x0c
    unsigned long m_oidTarget;                      // 0x18
    int m_nFlags;                                // 0x1c
    unsigned long m_nDuration;                      // 0x20
    unsigned long m_nDelay;                         // 0x24
    float m_fSpeed;                                 // 0x28
    unsigned char m_nState;                         // 0x2c
    unsigned char m_nType;                          // 0x2d
    char pad_0x2e[2];
    unsigned long m_n30;                            // 0x30
};

// vtable 0x00757CC8, size 0x1C8.
class CSWCProjectile : public CSWCObject {
public:
    CSWCProjectile();                               // 0x006D57B0
    virtual ~CSWCProjectile();                      // 0x006D5A40
    virtual void AIUpdate();                        // 0x74 (0x006D7BC0)
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart, int n);  // 0x78 (0x006D2C50)
    virtual void UnloadModel(int nPart);            // 0x7c (0x006D2DB0)
    virtual void ReleaseModel();                    // 0x80 (0x006D2D30)

    void SetDuration(unsigned long nDuration);      // 0x006D2C30
    void SetProjectileVelAndAccel(const Vector& v); // 0x006D33C0
    void SetSpellId(int nSpell);                    // 0x006D41B0
    int PlayAnimation(unsigned short nAnim);        // 0x006D4250
    void SetImpactInfo(unsigned char a, unsigned long b, unsigned short c);  // 0x006D42A0
    void SetTarget(unsigned long oidTarget, const CExoString& sNode);  // 0x006D42D0
    void AttachMuzzleFlash(const CExoString& sModel, CAurObject* pTo,
                           const CExoString& sNode); // 0x006D4440
    float GetSourceSpeed() { return m_fSourceSpeed; }  // descriptive
    void ClearBehaviorList();                       // 0x006D4520
    void SetBehaviorListType1();                    // 0x006D4550
    void SetBehaviorListType2();                    // 0x006D4640
    void SetBehaviorListType3();                    // 0x006D4730
    void SetBehaviorListType4();                    // 0x006D4930
    void SetBehaviorListType9();                    // 0x006D4A20
    void SetBehaviorListType5();                    // 0x006D4B10
    void SetBehaviorListSpiral();                   // 0x006D4C00
    void SetBehaviorListType7();                    // 0x006D4FE0
    void SetBehaviorListType14();                   // 0x006D50F0
    void SetBehaviorListType15();                   // 0x006D5250
    void SetBehaviorListType16();                   // 0x006D53A0
    void SetBehaviorListType10();                   // 0x006D5500
    void SetBehaviorListType50();                   // 0x006D55F0
    void SetBehaviorListType11(int b);              // 0x006D5B10
    void SetBehaviorListType8();                    // 0x006D64F0
    void SetBehaviorListType13();                   // 0x006D6A90
    void SetMode(int nMode);                        // 0x006D6EE0

    unsigned char m_nImpactA;                       // 0xfc
    char pad_0xfd[3];
    Vector m_vVelocity;                             // 0x100
    unsigned short m_nImpactC;                      // 0x10c
    char pad_0x10e[2];
    unsigned long m_nImpactB;                       // 0x110
    Vector m_vTargetPosition;                       // 0x114
    unsigned long m_nTotalDuration;                 // 0x120
    unsigned long m_oidTarget;                      // 0x124
    CExoString m_sTargetNode;                       // 0x128
    CResRef m_cModel;                               // 0x130
    CResRef m_cImpactModel;                         // 0x140
    int m_n150;                                     // 0x150
    int m_n154;                                     // 0x154
    unsigned char m_n158;                           // 0x158
    char pad_0x159[3];
    int m_n15C;                                     // 0x15c
    int m_n160;                                     // 0x160
    int m_n164;                                     // 0x164
    int m_n168;                                     // 0x168
    int m_nMode;                                    // 0x16c
    int m_n170;                                     // 0x170
    int m_nSpellId;                                 // 0x174
    Vector m_v178;                                  // 0x178
    Vector m_v184;                                  // 0x184
    unsigned long m_nDuration;                      // 0x190
    CExoLinkedList<CSWCProjectileBehavior> m_lBehaviors;  // 0x194
    CSWCProjectileBehavior* m_pBehavior;            // 0x198
    CAurObject* m_pGob;                             // 0x19c
    unsigned char m_nSide;                          // 0x1a0
    unsigned char m_n1A1;                           // 0x1a1
    char pad_0x1a2[2];
    int m_n1A4;                                     // 0x1a4
    int m_n1A8;                                     // 0x1a8
    CAurObject* m_pMuzzleFlash;                     // 0x1ac
    Vector m_vSourcePosition;                       // 0x1b0
    unsigned long m_nSourceDuration;                // 0x1bc
    float m_fSourceSpeed;                           // 0x1c0
    unsigned long m_oidSource;                      // 0x1c4
};

#endif
