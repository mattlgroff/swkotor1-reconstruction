// Worker M matching headers: mini-game objects (swoop / turret games).
// CSWMiniGameObjectArray (0xFF object slots), CSWMiniGameObject (abstract,
// vtable 0x00752424), CSWTrackFollower (abstract, vtable 0x007524C8),
// CSWMiniPlayer (vtable 0x007525F0), CSWMiniObstacle (vtable 0x0075287C),
// 0x0066BF10-0x0066E8F2 plus the sub-object ctor 0x00673A10.
#ifndef M_SW_MINI_GAME_OBJECT_H
#define M_SW_MINI_GAME_OBJECT_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator*(float f) const
    {
        return Vector(f * x, f * y, f * z);
    }
};

Vector operator/(const Vector& v, float f);         // 0x00437A90
inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Position / velocity pair integrated by the mini game (0x18 bytes).
struct CSWMiniState {
    Vector m_vPos;                                  // 0x00
    Vector m_vVel;                                  // 0x0c

    CSWMiniState() {}
    // 0x00670750 out of line where M_OUTLINE_MINI_STATE is set.
#ifdef M_OUTLINE_MINI_STATE
    CSWMiniState(const Vector& vPos, const Vector& vVel);
#else
    CSWMiniState(const Vector& vPos, const Vector& vVel)
    {
        m_vPos = vPos;
        m_vVel = vVel;
    }
#endif
};

CSWMiniState operator+(const CSWMiniState& a, const CSWMiniState& b);   // 0x006707A0
CSWMiniState operator*(const CSWMiniState& a, float f);   // 0x00670830
CSWMiniState operator/(const CSWMiniState& a, float f);   // 0x006708C0
CSWMiniState MakeMiniState(const Vector& vVel, const Vector& vPos);   // 0x006709C0

struct Quaternion {
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Quaternion(Vector vAxis, float fAngle);         // 0x004AC960
    Vector GetXAxis() const;                        // 0x0044F3D0
    Vector GetYAxis() const;                        // 0x0044F420
    Vector ToDirection() const;                     // 0x0043E250
};

Quaternion operator*(const Quaternion& a, const Quaternion& b);   // 0x004A9AC0
Quaternion DirectionToQuaternion(const Vector& v);  // 0x004ACCA0

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { delete[] element; element = 0; }
    void Allocate(int n);
    void Remove(T t) throw();                       // 0x0066DCC0 (pointer lists)
    void Insert(T t, int nIndex);                   // 0x00616940
    void DelIndex(int i);                           // 0x006857A0
    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                return i;
            }
        }
        return -1;
    }
    void DelIndexInline(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        }
        else {
            Allocate(n);
        }
        num = n;
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
};

// The same list as a local (destructor frees only a non-null buffer).
template <class T>
class CExoLocalList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoLocalList() { num = 0; array_size = 0; element = 0; }
    ~CExoLocalList() { if (element) delete[] element; }
};

// A back-referenced pointer: the target keeps a list of the SafePointers
// that point at it (+0x04) and clears them when it dies.
template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer() { m_p = 0; }
    ~SafePointer()
    {
        T* p = m_p;
        if (p) {
            p->m_lRefs.Remove(this);
        }
    }
    SafePointer<T>& Set(T* p);                      // 0x00447FA0 (folded, out of line)
    SafePointer<T>& operator=(T* p)
    {
        if (m_p) {
            m_p->m_lRefs.Remove(this);
        }
        m_p = p;
        if (p) {
            p->m_lRefs.Add(this);
        }
        return *this;
    }
};

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const CResRef& r);            // 0x00406120
    int IsValid() const;                            // 0x00406030
    int operator==(const char* s) const;            // 0x004060D0
    int operator==(const CResRef& r) const;         // 0x004060B0
    void CopyToString(CExoString& s) const;         // 0x00405F70
    void CopyToString(char* sz) const;              // 0x00405FB0
    char* GetResRef();                              // 0x00641DB0 (identity, folded)
};

class CResStruct {
public:
    unsigned long m_nStructIndex;
};

class CResList {
public:
    char pad_0x0[0x10];
};

class CResGFF {
public:
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x004118C0
    int GetListCount(CResList* pList);              // 0x00411940
    int GetListElement(CResStruct* pOut, CResList* pList, int n);   // 0x00411990
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault);   // 0x00411C90
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel);   // 0x00411A10
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);   // 0x00411C20
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault);   // 0x00411D00
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szField, int& bSuccess,
                             const CResRef& cDefault);   // 0x00411E10
    Vector ReadFieldVector(CResStruct* pStruct, char* szField, int& bSuccess,
                           const Vector& vDefault);  // 0x004122A0
};

class CVirtualMachine {
public:
    int RunScript(CExoString* psFile, unsigned long oidSelf, int bOid);   // 0x005D0FC0
};

extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

inline CVirtualMachine* GetVirtualMachine() { return g_pVirtualMachine; }

class CExoSoundSource {
public:
    CExoSoundSource(const CResRef& cRes);           // 0x005D60E0
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char n);         // 0x005D5900
    void Set3D(int b);                              // 0x005D5910
    int IsHardwarePlayingSoundPleaseDontUseThis();  // 0x005D5920
    unsigned long Play();                           // 0x005D5930
    void SetLooping(int b);                         // 0x005D59D0
    void SetResRef(const CResRef& cRes, int b);     // 0x005D61C0
    void SetVolume(unsigned char n);                // 0x005D5950
    void SetPosition(const Vector& v, int n);       // 0x005D59E0
    void Stop();                                    // 0x005D5A20

    void* m_pInternal;                              // 0x04
};

class CExoSound {
public:
    void PlaySoundEffect(const CResRef& cRes, Vector vPos, int a, unsigned char nGroup,
                         int b, int c, int d, int e);   // 0x005D5E10
    int PlayOneShotSound(const CResRef& cRes, unsigned char nGroup, int a, unsigned char nVolume,
                         int b, int c);             // 0x005D5E00
};

extern unsigned char g_nMiniGameAccelVolume;        // 0x0074C5E8
extern Vector g_vMiniHitPoint;                      // 0x007B93A8
extern Vector g_vMiniHitPos;                        // 0x007B93C4
extern Vector g_vMiniHitNormal;                     // 0x007B93E8
extern char* g_szMiniHitPart;                       // 0x007B93F4

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CSWMiniBulletInfo;

// The node returned by CAurObject slot 0x10c.
class CAurNode {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void GetTransform(Vector* pPos, Quaternion* pOri);   // 0x0c
};

class CAurObject;
class CAurPart;

// SafePointer<CAurObject>: operator= is the folded out-of-line
// SafePointer<CAurTexture>::operator= 0x00447FA0, the destructor is inline.
class CAurObjectRef {
public:
    CAurObject* m_p;

    CAurObjectRef() { m_p = 0; }
    CAurObjectRef(CAurObject* p);
    ~CAurObjectRef();
    CAurObjectRef& operator=(CAurObject* p);        // 0x00447FA0
    operator CAurObject*() { return m_p; }
    CAurObject* operator->() { return m_p; }
};

typedef void (*AurEventCallback)(CAurObject* pModel, char* szEvent, void* pUser);

class CAurObject {
public:
    virtual ~CAurObject();                          // 0x00
    virtual void v04();
    virtual void v08();
    virtual char* GetName();                        // 0x0c
    virtual void v10();
    virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int a, int b);  // 0x18
    virtual void v1C();
    virtual void v20(); virtual void v24();
    virtual void AddEventCallback(AurEventCallback pfn, const char* szEvent, void* pUser,
                                  float fTime, int n);   // 0x28
    virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetScene(void* pScene);            // 0x4c
    virtual void AttachToObject(CAurObject* pTo, const char* szNode, int n);    // 0x50
    virtual void v54();
    virtual void SetBehavior(void* pBehavior);      // 0x58
    virtual Vector SetLocalPosition(Vector v);      // 0x5c
    virtual Quaternion SetOrientation(Quaternion q);   // 0x60
    virtual Vector GetPosition();                   // 0x64
    virtual void v68();
    virtual Vector GetHitOrigin();                  // 0x6c
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84();
    virtual int HitTest(CAurNode* pNode, Vector vFrom, Vector vTo, int n, Vector* pOut);   // 0x88
    virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104();
    virtual CSWMiniBulletInfo* GetUserData(int nKey);   // 0x108
    virtual CAurNode* GetNode(const char* szNode);  // 0x10c
    virtual void FindParts(int (*pfnFilter)(CAurPart*), CExoLocalList<CAurPart*>* pList);   // 0x110

    CExoArrayList<CAurObjectRef*> m_lRefs;          // 0x04
};

inline CAurObjectRef::CAurObjectRef(CAurObject* p)
{
    m_p = p;
    if (p) {
        p->m_lRefs.Add(this);
    }
}

inline CAurObjectRef::~CAurObjectRef()
{
    CAurObject* p = m_p;
    if (p) {
        p->m_lRefs.Remove(this);
    }
}

// A part whose slot 0x04 returns its name (gun bank attach callback).
class CAurPart {
public:
    virtual void v00();
    virtual char* GetPartName();                    // 0x04
};

class CSWMiniGameObject;
class CSWTrackFollower;
class CSWMiniPlayer;
class CSWMiniObstacle;
class CSWMiniEnemy;


class CSWMiniEnemyHead {
public:
    char pad_0x0[0x1a0];
    unsigned char m_bActive;                        // 0x1a0
};

class CSWMiniGameObjectArray {
public:
    CSWMiniGameObjectArray();                       // 0x0066BF10
    CSWMiniGameObject* GetObject(unsigned long nId, unsigned char nType);   // 0x0066BF30
    int GetFreeSlot();                              // 0x0066C060

    int m_nNext;                                    // 0x00
    CSWMiniGameObject* m_pObjects[0xff];            // 0x04
};

// One track point of the mini game (+0x38 list): a name and a position.
struct CSWMiniTrackPoint {
    CResRef m_cName;                                // 0x00
    Vector m_vPos;                                  // 0x10
};

// The 0x20-byte explosion effect (+0x5c list).
class CSWMiniExplosion {
public:
    CSWMiniExplosion();                             // 0x00673080

    CAurObjectRef m_pModel;                         // 0x00
    Vector m_vPos;                                  // 0x04
    Quaternion m_qOri;                              // 0x10
};

class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource();                    // 0x005D6200
    void Play(const CResRef& cRes, int a, int b, int c, int d);   // 0x005D5B10
};

// The camera object at CSWCMiniGame + 0x20.
class CSWMiniCamera {
public:
    virtual void v00();
    virtual CAurObject* GetObject();                // 0x04
    virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void SetClipRange(float fNear, float fFar);   // 0x40
    virtual void SetFieldOfView(float f);           // 0x44
    virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void AttachTo(CAurObject* pTo, const char* szNode, int n);   // 0x74
    virtual void v78();
    virtual void SetBehavior(void* pBehavior);      // 0x7c
};

// CResHelper base (vtable 0x0074767C, destructor 0x00506BD0), 0x1c bytes.
class CResHelperMiniGame {
public:
    virtual ~CResHelperMiniGame();                  // 0x00506BD0

    int m_bAutoRequest;                             // 0x04
    void* m_pRes;                                   // 0x08
    CResRef m_cResRef;                              // 0x0c
};

// An id list with BioWare's inline DelIndex.
class CExoIdList : public CExoArrayList<unsigned long> {
public:
    int IndexOf(unsigned long n)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == n) {
                return i;
            }
        }
        return -1;
    }
    void DelIndexInline(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
};

// Inline Allocate for the track point list (AddTrackPoint 0x006720C0).
template <>
inline void CExoArrayList<CSWMiniTrackPoint*>::Allocate(int n)
{
    CSWMiniTrackPoint** pOld = element;
    array_size = n;
    element = new CSWMiniTrackPoint*[n];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}

// Units that define M_INLINE_ID_ALLOCATE (AddEnemy 0x00671E40) see an inline
// Allocate for id lists; elsewhere it is the folded 0x005E03C0.
#ifdef M_INLINE_ID_ALLOCATE
template <>
inline void CExoArrayList<unsigned long>::Allocate(int n)
{
    unsigned long* pOld = element;
    array_size = n;
    element = new unsigned long[n];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}
#endif

// A room of the mini-game area (CSWCMiniGame + 0xa8).
class CSWMiniRoom {
public:
    char pad_0x0[0x44];
    void* m_pRoomData;                              // 0x44
};

// The area scene at CSWCArea + 0x184 (slot 0x7c selects the visible room).
class CSWMiniSceneObj {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetRoom(void* pRoom);              // 0x7c
};

// The area at CSWCMiniGame + 0x28 (a CSWCArea).
class CSWMiniScene {
public:
    CSWMiniRoom* GetRoomAt(const Vector& v);        // 0x00605FA0 (CSWCArea::GetRoomAt)

    char pad_0x0[0x184];
    CSWMiniSceneObj* m_pScene;                      // 0x184
    CSWMiniSceneObj* GetScene() { return m_pScene; }
};

class CSWCMiniGame : public CResHelperMiniGame {
public:
    ~CSWCMiniGame();                                // 0x00672EB0 (??_G 0x00673060)

    int GetHookNode();                              // 0x00670C10
    void GetClipRange(float* pNear, float* pFar);   // 0x00670B30
    void SetClipRange(float fA, float fB);          // 0x00670B50
    Vector GetAxis(Quaternion* pOri);               // 0x00670BB0
    void UpdateAxis(Vector* pVec, float fDelta);    // 0x00670FB0
    void UpdateAxisLinear(Vector* pVec, float fDelta);   // 0x00670A10
    void UpdateAxisRK4(Vector* pVec, float fDelta);  // 0x00670C30
    Vector GetAccel(const Vector& vA, const Vector& vB);   // 0x00670A90
    void Rotate(float fA, float fB, float fScale);  // 0x00671020
    void Go();                                      // 0x006714E0
    CSWMiniGameObject* GetEnemy(int i);             // 0x00671570
    CSWTrackFollower* RemoveObject(CSWTrackFollower* p);   // 0x006715B0
    CSWMiniTrackPoint* GetTrackPoint(int i);        // 0x00671610
    CSWMiniGameObject* GetObstacle(int i);          // 0x00671630
    CSWMiniCamera* SetCamera(CSWMiniCamera* p);     // 0x00671670
    CSWMiniPlayer* SetPlayer(CSWMiniPlayer* p);     // 0x006717F0
    int SetLateralAxis(int n);                      // 0x00671BC0
    int SetVerticalAxis(int n);                     // 0x00671BE0
    int ClearEnemies();                             // 0x00672040
    int ClearTrackPoints();                         // 0x006721F0
    int ClearObstacles();                           // 0x00672350
    void AddBullet(CAurObject* pBullet);            // 0x00672FA0
    CSWMiniTrackPoint* AddTrackPoint(const CResRef& cName, const Vector& vPos);   // 0x006720C0
    CSWMiniObstacle* AddObstacle(const CResRef& cModel, const Vector& vPos);   // 0x00672240
    CSWMiniEnemy* AddEnemy(CExoArrayList<CResRef>* pModels, CExoArrayList<unsigned char>* pAttach,
                           const CResRef& cTrack);   // 0x00671E40
    CSWMiniPlayer* CreatePlayer(CExoArrayList<CResRef>* pModels,
                                CExoArrayList<unsigned char>* pAttach,
                                const CResRef& cTrack);   // 0x00671820
    CSWMiniPlayer* CreatePlayerWithHook(CExoArrayList<CResRef>* pModels,
                                        CExoArrayList<unsigned char>* pAttach,
                                        const CResRef& cTrack, const CResRef& cHook,
                                        unsigned char bAttach);   // 0x006719D0
    int HitCheckBullet(CAurObject* pBullet, CAurObject* pHook, CSWMiniGameObject* p);   // 0x006730A0
    void DoHitCheck();                              // 0x006732F0
    void Update(float fDelta);                      // 0x006735D0
    __forceinline void RemoveBullet(int i)
    {
        delete m_lstBullets.element[i];
        m_lstBullets.DelIndexInline(i);
    }
    CSWMiniPlayer* GetPlayer() { return m_pPlayer; }
    CSWMiniTrackPoint* FindTrackPoint(const CResRef& cName)
    {
        int nCount = m_lstTrackPoints.num;
        for (int i = 0; i < nCount; i++) {
            CSWMiniTrackPoint* pPoint = m_lstTrackPoints.element[i];
            if (cName == pPoint->m_cName) {
                return pPoint;
            }
        }
        return 0;
    }

    CExoStreamingSoundSource* m_pMusic;             // 0x1c
    CSWMiniCamera* m_pCamera;                       // 0x20
    CSWMiniPlayer* m_pPlayer;                       // 0x24
    CSWMiniScene* m_pScene;                         // 0x28
    CExoIdList m_lstEnemies;                        // 0x2c
    CExoArrayList<CSWMiniTrackPoint*> m_lstTrackPoints;   // 0x38
    CExoIdList m_lstObstacles;                      // 0x44
    CExoArrayList<CAurObjectRef*> m_lstBullets;     // 0x50
    CExoArrayList<CSWMiniExplosion*> m_lstEffects;  // 0x5c
    float m_fNear;                                  // 0x68
    float m_fFar;                                   // 0x6c
    float m_f70;                                    // 0x70
    float m_f74;                                    // 0x74
    int m_nAxisType;                                // 0x78
    int m_n7C;                                      // 0x7c
    int m_nMode;                                    // 0x80
    int m_nLateralAxis;                             // 0x84
    int m_nVerticalAxis;                            // 0x88
    char pad_0x8c[0x94 - 0x8c];
    unsigned char m_bActive;                        // 0x94
    unsigned char m_b95;                            // 0x95
    CResRef m_cMusic;                               // 0x96
    char pad_0xa6[0xa8 - 0xa6];
    CSWMiniRoom* m_pRoom;                           // 0xa8
    int m_nAxis;                                    // 0xac
    Vector m_vB0;                                   // 0xb0
    float m_fBC;                                    // 0xbc
    CSWMiniState m_sC0;                             // 0xc0
    CSWMiniState m_sD8;                             // 0xd8
};

class CClientExoApp {
public:
    CSWMiniGameObjectArray* GetMiniGameObjects();   // 0x005EDAF0
    CSWCMiniGame* GetMiniGame();                    // 0x005EDB00
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

// The 0x28-byte block at CSWMiniGameObject + 0x1c.
class CSWMiniHitInfo {
public:
    // 0x00673A10 out of line; the units that define
    // M_INLINE_MINI_HIT_INFO (CSWMGGunBank's constructor) see the same body
    // as an inline constructor.
#ifdef M_INLINE_MINI_HIT_INFO
    CSWMiniHitInfo()
    {
        m_nDamage = 0;
        m_nTargetMask = 0;
        m_fSpeed = 0.0f;
        m_fRateOfFire = 0.0f;
        m_fLifespan = 0.0f;
    }
#else
    CSWMiniHitInfo();
#endif

    int m_nDamage;                                  // 0x00
    float m_fSpeed;                                 // 0x04
    float m_fRateOfFire;                            // 0x08
    float m_fLifespan;                              // 0x0c
    int m_nTargetMask;                              // 0x10
    int m_n14;                                      // 0x14
    CResRef m_cSound;                               // 0x18
};

// The bullet user data (CAurObject slot 0x108 with 0xAAAB).
class CSWMiniBulletInfo {
public:
    char pad_0x0[0x14];
    CSWMiniHitInfo m_cHit;                          // 0x14
    CSWTrackFollower* m_pOwner;                     // 0x3c
};

// Target of CSWMiniGameObject::m_pRef18 (keeps its SafePointers at +4).
class CSWMiniRefTarget {
public:
    void* m_p00;
    CExoArrayList<SafePointer<CSWMiniRefTarget>*> m_lRefs;   // 0x04
};

class CSWMiniGameObject {
public:
    CSWMiniGameObject(int nType);                   // 0x0066C540
    ~CSWMiniGameObject();                           // 0x0066C610

    virtual void PlayAnimation(const char* szAnim, float fSpeed, int nFlags) = 0;   // 0x00
    virtual CExoString GetModelName() = 0;          // 0x04
    virtual void LoadScripts(CResGFF* pGFF, int nUnused, CResStruct* pStruct);   // 0x08
    virtual CResRef GetScript(int nScript) = 0;     // 0x0c
    virtual void SetScript(int nScript, const CResRef& cScript) = 0;   // 0x10
    virtual CSWTrackFollower* AsTrackFollower();    // 0x14
    virtual CSWMiniPlayer* AsMiniPlayer();          // 0x18
    virtual CSWMiniEnemyHead* AsMiniEnemy();        // 0x1c
    virtual CSWMiniObstacle* AsMiniObstacle();      // 0x20
    virtual void RunHeartbeatScript(float fDelta);  // 0x24

    unsigned long GetId() { return m_nId; }
    void RunHitBulletScript(int bHit);              // 0x0066C190
    void SetHitInfo(const CSWMiniHitInfo& cHit);    // 0x00671460
    void RunHitFollowerScript(int bHit);            // 0x0066C2D0
    void RunCreateScript();                         // 0x0066C370
    void ValidateTarget4C();                        // 0x0066C130
    void ValidateTarget50();                        // 0x0066C160

    int m_nObjectType;                              // 0x04
    unsigned long m_nId;                            // 0x08
    unsigned char m_n0C;                            // 0x0c
    CExoString m_sTag;                              // 0x10
    SafePointer<CSWMiniRefTarget> m_pRef18;         // 0x18
    CSWMiniHitInfo m_cHit;                          // 0x1c
    CExoString m_s44;                               // 0x44
    unsigned long m_nTarget4C;                      // 0x4c
    unsigned long m_nTarget50;                      // 0x50
    Vector m_vSoundPos;                             // 0x54
};

struct CSWMiniHookEntry {
    CAurObject* m_pModel;                           // 0x00
    unsigned char m_bAttach;                        // 0x04
};

class CSWMiniHookList : public CExoArrayList<CSWMiniHookEntry> {
public:
    __forceinline void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
};

// The four targetting limits read by CSWMiniEnemy::LoadGunBank.
class CSWMGTargettingInfo {
public:
    // 0x00673A30 out of line; inlined where M_INLINE_TARGETTING_INFO is set.
#ifdef M_INLINE_TARGETTING_INFO
    CSWMGTargettingInfo()
    {
        m_fHorizSpread = 0.0f;
        m_fVertSpread = 0.0f;
        m_fSensingRadius = 0.0f;
        m_fInaccuracy = 0.0f;
    }
#else
    CSWMGTargettingInfo();
#endif

    float m_fHorizSpread;                           // 0x00
    float m_fSensingRadius;                         // 0x04
    float m_fVertSpread;                            // 0x08
    float m_fInaccuracy;                            // 0x0c
};

// What CAurObject slot 0x108 (GetUserData) returns for a gun: its emitter
// behaviour (type 0xAAAD plain, 0xAAAA targetting).
class CAurEmitter {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual int GetType();                          // 0x10
    virtual void v14(); virtual void v18(); virtual void v1C(); virtual void v20();
    virtual void Restart();                         // 0x24
    void SetBirthrateA(float f);                    // 0x006DAF60 (type 0xAAAD)
    void SetBirthrateB(float f);                    // 0x006DAFA0 (type 0xAAAA)

    char pad_0x4[0x14 - 4];
    CAurObject* m_pModel;                           // 0x14
    CAurPart* m_pPart;                              // 0x18
    char pad_0x1c[0x3c - 0x1c];
};

class CAurGunEmitter : public CAurEmitter {
public:
    CAurGunEmitter(CAurObject* pGun, CAurObject* pModel, CAurPart* pPart);   // 0x006DADF0

    CResRef m_cSound;                               // 0x3c
    float m_fRate;                                  // 0x4c
    char pad_0x50[0x54 - 0x50];
};

class CAurTargettingEmitter : public CAurEmitter {
public:
    CAurTargettingEmitter(CAurObject* pGun, CAurObject* pModel, CAurPart* pPart,
                          CAurObject* pTarget);     // 0x006DB590

    CSWMGTargettingInfo m_cInfo;                    // 0x3c
    float m_f4C;                                    // 0x4c
    float m_fBump;                                  // 0x50
    char pad_0x54[0x58 - 0x54];
    Vector m_vOffset;                               // 0x58
    CResRef m_cSound;                               // 0x64
    char pad_0x74[0x7c - 0x74];
};

class CAurBehaviorAttach {
public:
    CAurBehaviorAttach(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart);   // 0x0049FB10

    char pad_0x0[0x3c];
};

CAurObject* NewCAurObject(const char* szModel, const char* szName, int a, int b);   // 0x00449CC0

class CSWMGGunBank {
public:
    CSWMGGunBank(CSWTrackFollower* pOwner, CResRef* pBulletModel, CResRef* pGunModel,
                 CSWMiniHitInfo* pHit);             // 0x006746D0
    virtual CAurObject* AttachGun(CAurObject* pModel, CAurPart* pPart);   // 0x00 0x006743B0
    virtual void Die();                             // 0x04 0x00674990
    virtual int IsTargetting();                     // 0x08
    ~CSWMGGunBank();                                // 0x00674800
    void Reset();                                   // 0x00673FD0
    CAurObject* RemoveGun(CAurObject* pGun);        // 0x00674020
    void LoadResources();                           // 0x00674090
    void SetGunModel(CResRef* pGunModel);           // 0x006740D0
    int ClearGuns();                                // 0x006744D0
    void OnFire();                                  // 0x006738D0
    float SetSpeed(float f);                        // 0x00673840
    float SetLifespan(float f);                     // 0x00673860
    int SetTargetMask(int n);                       // 0x00673880
    float SetRateOfFire(float f);                   // 0x00673F50

    CResRef m_cBulletModel;                         // 0x04
    CResRef m_cGunModel;                            // 0x14
    CResRef m_cFireSound;                           // 0x24
    CExoArrayList<CAurObject*> m_lstGuns;           // 0x34
    CSWMiniHitInfo m_cHit;                          // 0x40
    CSWTrackFollower* m_pOwner;                     // 0x68
};

struct CSWMiniSoundInfo {
    int m_nVolume;                                  // 0x00
    int m_n04;                                      // 0x04
    int m_n08;                                      // 0x08
    CSWMiniSoundInfo() { m_nVolume = 0; m_n04 = 0; m_n08 = 0; }
};

class CSWTrackFollower : public CSWMiniGameObject {
public:
    CSWTrackFollower(CSWCMiniGame* pGame, int nType);   // 0x0066DEE0

    void PlayAnimation(const char* szAnim, float fSpeed, int nFlags);   // 0x0066D550
    CExoString GetModelName();                      // 0x0066CAD0
    void LoadScripts(CResGFF* pGFF, int nUnused, CResStruct* pStruct);   // 0x0066C740
    CResRef GetScript(int nScript);                 // 0x0066C970
    void SetScript(int nScript, const CResRef& cScript);   // 0x0066C9C0
    CSWTrackFollower* AsTrackFollower();
    void RunHeartbeatScript(float fDelta);          // 0x0066E130
    virtual ~CSWTrackFollower();                    // 0x28 (0x0066E000, ??_G 0x0066F7C0)
    virtual int Load(CResGFF* pGFF, CResStruct* pStruct);   // 0x2c 0x0066FFF0
    virtual int LoadGunBank(CResGFF* pGFF, CResStruct* pStruct) = 0;   // 0x30
    virtual CAurObject* AddModel(CAurObject* p, unsigned char bAttach);   // 0x34 0x0066E1F0
    virtual void SetAxisModeV(int n) = 0;           // 0x38
    virtual int GetTargetMask() = 0;                // 0x3c

    CAurObject* SetModel(CAurObject* pModel, int n);   // 0x0066D2F0
    void ApplyDamage(int bSkipScript);              // 0x0066E7A0
    void OnDeath(int bSkipScript);                  // 0x0066E2A0
    int AttachGunParts(CAurObject* pModel);         // 0x0066E490
    void LoadSounds(CResGFF* pGFF, int nUnused, CResStruct* pStruct);   // 0x0066F7E0
    void OnHitObstacle(int bSkipScript);            // 0x0066E5C0
    int DoObstacleHitCheck(CSWMiniObstacle* pObstacle);   // 0x0066E6B0
    CExoSoundSource* SetSound(int n, const CResRef& cRes);   // 0x0066E900
    int LoadFromGFF(CResGFF* pGFF, CResStruct* pStruct);   // 0x0066D0B0
    void Reset();                                   // 0x0066D490
    int HitCheckBullet(CAurObject* pBullet);        // 0x0066D4E0
    Vector GetHookPosition();                       // 0x0066D5D0
    __forceinline Vector GetHookPositionInline()
    {
        Vector v;
        if (m_lstHooked.num) {
            v = m_lstHooked.element[0].m_pModel->GetPosition();
        }
        return v;
    }
    void LoadGunBanks();                            // 0x0066DC50
    void Go();                                      // 0x006701C0
    void RemoveGunBank(CSWMGGunBank* p);            // 0x0066E110

    float SetSpeed(float f);                        // 0x0066C870
    float SetF84(float f);                          // 0x0066C8D0
    int SetN8C(int n);                              // 0x0066C900
    float SetFA0(float f);                          // 0x0066C920
    int SetN90(int n);                              // 0x0066C950
    int GetSound15C(int n);                         // 0x0066C9F0
    void SetSound15C(int n, int v);                 // 0x0066CA10
    int GetSound160(int n);                         // 0x0066CA30
    void SetSound160(int n, int v);                 // 0x0066CA50
    int GetSoundVolume(int n);                      // 0x0066CA70
    void SetSoundVolume(int n, int v);              // 0x0066CA90
    void ValidateTarget170();                       // 0x0066CB10
    void SetAxisMode(int n);                        // 0x0066D090
    CSWMGGunBank* GetGunBank(int n);                // 0x0066D240
    CAurObject* GetHookedModel(int n);              // 0x0066D260
    CAurObject* RemoveHookedModel(CAurObject* p);   // 0x0066D280
    void StopTrackAnimation();                      // 0x0066D440
    CAurObject* SetModelIfChanged(CAurObject* pModel, int n);   // 0x0066E280
    void SetN19C(int n);                            // 0x0066E8E0
    void SetHitInfo2(const CSWMiniHitInfo& cHit);   // 0x006737E0

    CAurObject* GetModel() { return m_pModel; }
    float GetF84() { return m_f84; }
    int GetTrackAnimFlags() { return m_nAxisMode == 1 ? 0x11 : 0x14; }

    CSWCMiniGame* m_pMiniGame;                      // 0x60
    CAurObject* m_pModel;                           // 0x64
    CSWMiniHookList m_lstHooked;                    // 0x68
    CExoArrayList<CSWMGGunBank*> m_lstGunBanks;     // 0x74
    int m_nAxisMode;                                // 0x80
    float m_f84;                                    // 0x84
    union {
        unsigned long m_nFlags;                     // 0x88
        struct {
            unsigned long m_bTrackActive : 1;
            unsigned long m_bDead : 1;
        };
    };
    int m_n8C;                                      // 0x8c
    int m_n90;                                      // 0x90
    int m_n94;                                      // 0x94
    float m_fSpeed;                                 // 0x98
    float m_f9C;                                    // 0x9c
    float m_fA0;                                    // 0xa0
    CResRef m_aScripts[10];                         // 0xa4
    CExoSoundSource* m_apSources[1];                // 0x144
    CResRef m_acSoundNames[1];                      // 0x148 (index n - 1)
    CSWMiniSoundInfo m_aSounds[2];                  // 0x158
    unsigned long m_nTarget170;                     // 0x170
    CSWMiniHitInfo m_cHit2;                         // 0x174
    int m_n19C;                                     // 0x19c
};

// One queued hook event (0x24 bytes), see HookEventCallback 0x0066DD60.
struct CSWMiniHookEvent {
    void* m_p00;                                    // 0x00
    Vector m_vPos;                                  // 0x04
    Quaternion m_qOri;                              // 0x10
    unsigned char m_bHasPos;                        // 0x20
    unsigned char m_bHasOri;                        // 0x21
};

class CSWMiniPlayer : public CSWTrackFollower {
public:
    CSWMiniPlayer(CSWCMiniGame* pGame);             // 0x0066EB50
    ~CSWMiniPlayer();                               // 0x0066ECF0 (??_G 0x0066F870)

    void SetAxisModeV(int n);                       // 0x0066CF20
    int DoFollowerHitCheck(CExoIdList* pEnemies, int bFirst);   // 0x0066EDA0
    int GetTargetMask();                            // 0x0063E7A0 (shared)
    int LoadGunBank(CResGFF* pGFF, CResStruct* pStruct);   // 0x0066F890
    int Load(CResGFF* pGFF, CResStruct* pStruct);   // 0x006702F0
    CSWMGGunBank* CreateGunBank(int nBank, CResRef* pBulletModel, CResRef* pGunModel,
                                CSWMiniHitInfo* pHit, CResRef* pFireSound);   // 0x0066F440

    void ClampPosition(Vector* pPos);               // 0x0066CB40
    void SetBank(int n, int bRelative);             // 0x0066CE60
    Vector SetPosition(const Vector& v);            // 0x0066CCF0
    float SetF1D8(float f);                         // 0x0066CF40
    float SetF1DC(float f);                         // 0x0066CF70
    float SetF1E0(float f);                         // 0x0066CFA0
    Vector SetOffset(const Vector& v);              // 0x0066CFD0
    void SetHook(CAurObject* pHook, unsigned char bAttach);   // 0x0066D030

    CExoArrayList<CSWMiniHookEvent> m_lstEvents1A0; // 0x1a0
    CExoArrayList<CSWMiniHookEvent> m_lstEvents1AC; // 0x1ac
    CExoArrayList<unsigned long> m_lst1B8;          // 0x1b8
    Vector m_vPosition;                             // 0x1c4
    int m_nBank;                                    // 0x1d0
    unsigned char m_b1D4;                           // 0x1d4
    float m_f1D8;                                   // 0x1d8
    float m_f1DC;                                   // 0x1dc
    float m_f1E0;                                   // 0x1e0
    Vector m_vMin;                                  // 0x1e4
    Vector m_vMax;                                  // 0x1f0
    Vector m_vClamp;                                // 0x1fc
    Vector m_vOffset;                               // 0x208
    Vector m_v214;                                  // 0x214
    CAurObject* m_pHook;                            // 0x220
    unsigned char m_bHookAttached;                  // 0x224
    CAurObject* GetHook() { return m_pHook; }
    char pad_0x225[0x228 - 0x225];
    Vector m_v228;                                  // 0x228
    Vector m_vStartTrack;                           // 0x234
    Quaternion m_q240;                              // 0x240
};

class CSWMGTargettingGunBank : public CSWMGGunBank {
public:
    CSWMGTargettingGunBank(CSWTrackFollower* pOwner, CResRef* pBulletModel, CResRef* pGunModel,
                           CSWMiniHitInfo* pHit, CSWMGTargettingInfo* pInfo,
                           CSWTrackFollower* pTarget);   // 0x00674860
    CAurObject* AttachGun(CAurObject* pModel, CAurPart* pPart);   // 0x00 0x00674530
    void Die();                                     // 0x04 0x00674A10
    int IsTargetting();                             // 0x08

    float SetSensingRadius(float f);                // 0x00673970
    float SetVertSpread(float f);                   // 0x00673990
    float SetHorizSpread(float f);                  // 0x006739C0
    float SetInaccuracy(float f);                   // 0x006739F0

    CSWMGTargettingInfo m_cInfo;                    // 0x6c
    CSWTrackFollower* m_pTarget;                    // 0x7c
};

class CSWMiniEnemy : public CSWTrackFollower {
public:
    // Implicit virtual destructor: 0x006714D0 (a jmp to ~CSWTrackFollower),
    // ??_G 0x006714B0. The constructor is inlined into CSWCMiniGame.
    CSWMiniEnemy(CSWCMiniGame* pGame) : CSWTrackFollower(pGame, 2) {}
    int Load(CResGFF* pGFF, CResStruct* pStruct);   // 0x006705F0 (worker c)
    int LoadGunBank(CResGFF* pGFF, CResStruct* pStruct);   // 0x0066FB20
    void SetAxisModeV(int n);                       // 0x0066D090 (shared)
    int GetTargetMask();                            // 0x00670B10
    CSWMGGunBank* CreateGunBank(int nBank, CResRef* pBulletModel, CResRef* pGunModel,
                                CSWMiniHitInfo* pHit, CSWMGTargettingInfo* pInfo,
                                CResRef* pFireSound);   // 0x0066F5F0

    unsigned char m_b1A0;                           // 0x1a0
};

class CSWMiniObstacle : public CSWMiniGameObject {
public:
    CSWMiniObstacle(CSWCMiniGame* pGame, CAurObject* pModel);   // 0x00670640
    ~CSWMiniObstacle();                             // 0x006706E0
    void PlayAnimation(const char* szAnim, float fSpeed, int nFlags);   // 0x006706C0
    CExoString GetModelName();                      // 0x0066DC80
    void LoadScripts(CResGFF* pGFF, int nUnused, CResStruct* pStruct);   // 0x0066D180
    CResRef GetScript(int nScript);                 // 0x0066D100
    void SetScript(int nScript, const CResRef& cScript);   // 0x0066D150

    CAurObjectRef m_pModel;                         // 0x60
    CResRef m_aScripts[5];                          // 0x64
};

void TrackFollowerPartDie(CAurObject* pPart, void* pUnused, CSWTrackFollower* pFollower);   // 0x0066DE50
void StartTrackCallback(CAurObject* pModel, char* szEvent, void* pUser);   // 0x0066C6A0
int IsGunBankPart(CAurPart* pPart);                 // 0x0066C720
void TrackFollowerAnimEvent(CAurObject* pModel, char* szEvent, CSWTrackFollower* pFollower);   // 0x0066FE50
void GunEventCallback(CAurObject* pModel, char* szEvent, void* pUser);   // 0x00673A40
void GunDie(CAurObject* pModel, char* szEvent, CSWMGGunBank* pBank);   // 0x00674930
void ExplosionDoneCallback(CAurObject* pModel, char* szEvent, void* pUser);   // 0x00670B20

#endif
