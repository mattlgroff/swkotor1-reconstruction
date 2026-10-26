// CSWCObject (nwcobject.o) lifetime, visual-effect list, fade and position
// family. Self-contained layout for these translation units; the older
// object_control_family.h keeps its own partial view of the same object.
// Vtable 0x007516C8 slot names follow the playable build's vtable table
// (src/units/1461/f4c7ba6a63-w3_client_t3.cpp) and the Mac debug map.
#ifndef N_SW_C_OBJECT_H
#define N_SW_C_OBJECT_H

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-() const { return Vector(-x, -y, -z); }
};
inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
class Quaternion {
public:
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Vector ToDirection();                           // 0x0043E250
};
Quaternion RotationArc(Vector vFrom, Vector vTo);   // 0x004AB630
float Magnitude(const Vector& v);                   // 0x004AB100
int operator==(const Vector& a, const Vector& b);   // 0x004AA980
inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
};

class CResRef {
public:
    char m_resRef[16];
    CResRef operator=(const CResRef& r);            // 0x00406120
};

class CSWPortrait {
public:
    char m_data[16];
    CSWPortrait();                                  // 0x00678EC0
    ~CSWPortrait();                                 // 0x004015A0
    CResRef GetPortrait(unsigned char n);           // 0x00678EF0
};

extern const char g_sEmpty[];                       // 0x0073D71C
extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern int g_bUpdateCreatureOnMove;                 // 0x0083395C
extern int g_nScreenWidth;                          // 0x0078D1D4
extern int g_nScreenHeight;                         // 0x0078D1D8
extern const float g_fZero;                         // 0x0073D700
extern const float g_fOne;                          // 0x0073D764

// Linked list -----------------------------------------------------------

typedef void* CExoLinkedListPosition;

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

    void* GetHead() { void* p = pHead; if (pHead) p = pHead->pObject; return p; }
    void* GetTail() { void* p = pTail; if (pTail) p = pTail->pObject; return p; }
    CExoLinkedListInternal();                       // 0x005E9AA0
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* GetPrev(CExoLinkedListPosition& pos);     // 0x005E9BD0
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    void* RemoveHead();                             // 0x005E9C80
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;

    CExoLinkedList();
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    CExoLinkedListPosition GetTailPos() { return m_pcExoLinkedListInternal->pTail; }
    T* GetHead() { return (T*)m_pcExoLinkedListInternal->GetHead(); }
    T* GetTail() { return (T*)m_pcExoLinkedListInternal->GetTail(); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* GetPrev(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetPrev(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListPosition AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

template <class T>
CExoLinkedList<T>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal;
}

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

// Collaborators ----------------------------------------------------------

class CAurObject;
class CAurScene;
class CSWCArea;
class CSWCObject;
class CSWSObject;

class CAurPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void GetWorldTransform(Vector& vPosition, Quaternion& qOrientation);  // +0x0C (placeholder name)
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108();
    virtual CAurPart* FindPart(const char* sNode);  // +0x10C (placeholder name)
};

class CSWCModuleCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                   // +0x20 (placeholder name)
    virtual Quaternion GetOrientation();            // +0x24 (placeholder name)
    virtual Vector WorldToScreen(Vector v, int b);  // +0x28 (placeholder name)
};

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nScreenWidth;                           // 0x6c (placeholder name)
    short m_nScreenHeight;                          // 0x6e (placeholder name)
};

class CSWCAnimBase {
public:
    virtual ~CSWCAnimBase();
    virtual void AttachToScene(CAurScene* pScene);  // +0x04
    virtual CAurObject* GetModel(unsigned char n);  // +0x08
    virtual void v0C(); virtual void v10(); virtual void v14(); virtual void v18();
    virtual void v1C(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2C(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3C(); virtual void v40(); virtual void v44(); virtual void v48();
    virtual void v4C(); virtual void v50(); virtual void v54(); virtual void v58();
    virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void SetPosition(const Vector& v);      // +0x68 (placeholder name)
    virtual void v6C(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7C(); virtual void v80(); virtual void v84(); virtual void v88();
    virtual void v8C(); virtual void v90();
    virtual void SetAlpha(float fAlpha, unsigned char nPart);  // +0x94
    virtual void v98(); virtual void v9C(); virtual void vA0(); virtual void vA4();
    virtual void vA8(); virtual void vAC(); virtual void vB0();
    virtual void SetGroundNormal(const Quaternion& q);  // +0xB4 (placeholder name)
    virtual void vB8(); virtual void vBC(); virtual void vC0(); virtual void vC4();
    virtual void vC8(); virtual void vCC(); virtual void vD0(); virtual void vD4();
    virtual void vD8();
    virtual unsigned char GetReticlePoint(Vector& v);   // +0xDC (placeholder name)

    int Hilite(int a, int b, int c);                // 0x006A03E0 (placeholder name)
    int UnHilite(int a);                            // 0x006A0440 (placeholder name)
};

class CSWCVisualEffectOnObject {
public:
    CSWCVisualEffectOnObject();                     // 0x006A09E0
    ~CSWCVisualEffectOnObject();                    // 0x006A1810
    int IsReady();                                  // 0x006A0480
    unsigned char GetType();                        // 0x006A0920
    void DestroyModels();                           // 0x006A0BF0
    void Detach();                                  // 0x006A0C70
    int Load(unsigned short nEffect, int nDuration, unsigned long oidSelf,
             unsigned long nTarget, unsigned long nNode, unsigned char nA,
             unsigned char nB, Vector vOffset);     // 0x006A1880 (placeholder name)
    void ReAttachVisualEffect();                    // 0x006A5000
    void DetachAll();                               // 0x006A5F00
    void StartVisualEffect();                       // 0x006A5FC0
    void ShowVisualEffect(int bShow);               // 0x006A5A40
    void StopVisualEffect(int bForce);              // 0x006A6590
    void Reset();                                   // 0x006A6950
    void Update(unsigned long nDelta);              // 0x006A6A50

    char pad_0x0[0xa8];
    unsigned long m_nA8;                            // 0xa8
    char pad_0xac[0xbc - 0xac];
    void* m_pBC;                                    // 0xbc
    unsigned long m_bC0;                            // 0xc0
    unsigned short m_nEffect;                       // 0xc4
    unsigned char m_bC6;                            // 0xc6
    unsigned char m_bC7;                            // 0xc7
    unsigned char m_bC8;                            // 0xc8
    char pad_0xc9[0xcc - 0xc9];
    float m_fAlpha;                                 // 0xcc
    char pad_0xd0[0xd8 - 0xd0];
    unsigned char m_nPriority;                      // 0xd8
    char pad_0xd9[0xdc - 0xd9];
    int m_bDC;                                      // 0xdc
    int m_bAttached;                                // 0xe0
    char pad_0xe4[0xf8 - 0xe4];
};

class CSWSObject {
public:
    void DetachFromClientObject();                  // 0x004CC320
};

class CExoSoundSource {
public:
    virtual ~CExoSoundSource();
};

class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource();                    // 0x005D6200
    void* m_pInternal;
};

class CSWCArea {
public:
    int GetIsDark();                                // 0x00605440
    int GetObjectsAtPositionVisible(const Vector& v);   // 0x006054E0
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CWorldTimer {
public:
    unsigned __int64 GetSnapshotTimeDifference();   // 0x004ADC80
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);  // 0x004ADE40
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1,
                           unsigned long nDay2, unsigned long nTime2,
                           unsigned long* pDay, unsigned long* pTime);  // 0x004AE460
};

class CClientAIMaster {
public:
    int RemoveObject(CSWCObject* pObject);          // 0x00604A30
};

class CSWCCreature;
struct CSWCObjectRecord {
    unsigned long m_oidObject;                      // 0x00 (placeholder name)
};
class CSWCPlayerCreature {
public:
    char pad_0x0[0x360];
    CExoArrayList<CSWCObjectRecord*> m_lObjectRecords;  // 0x360 (placeholder name)
};

class CSWCOptions {
public:
    char pad_0x0[0x6c];
    unsigned char m_nShadows;                       // 0x6c (placeholder name)
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CSWCModule* GetModule();                        // 0x005ED530
    CSWCPlayerCreature* GetPlayerCreature();        // 0x005ED540
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
    CSWCModuleCamera* GetModuleCamera();            // 0x005ED560
    CSWCOptions* GetOptions();                      // 0x005ED700 (placeholder name)
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
    void RemoveObjectFromHitcheckIgnoreList(unsigned long oid);  // 0x005ED760
};

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

// Object -----------------------------------------------------------------

struct FireForgetAnimation {
    int active;
    unsigned short animation;
    unsigned short pad;
    unsigned long day, tick, duration;
    float speed;
    void* data;
    FireForgetAnimation()
    {
        active = 0;
        animation = 10000;
        day = 0;
        tick = 0;
        duration = 500;
        speed = 1.0f;
        data = 0;
    }
};

class CSWSPlaceable {
public:
    char pad_0x0[0x440];
    int m_n440;                                     // 0x440 (placeholder name)
    int m_n444;                                     // 0x444 (placeholder name)
};
class CSWCDoor {
public:
    void PlayAnimationAction(unsigned short nAnim, void* pData, int n);   // 0x00683E00
};
class CSWCPlaceable {
public:
    void PlayAnimationAction(unsigned short nAnim, void* pData, int n);   // 0x00682620
    CSWSPlaceable* GetServerPlaceable();            // 0x00682E00
};
class CSWCTrigger {
public:
    void PlayAnimation(unsigned short nAnim, void* pData, int n);         // 0x006911B0
};

class CGameObject {
public:
    CGameObject(unsigned char nObjectType, unsigned long oid);  // 0x004D7D60
    virtual ~CGameObject() {}
    virtual void SetId(unsigned long oid);          // 0x04
    virtual void ResetUpdateTimes(unsigned long nDay, unsigned long nTime);  // 0x08
    virtual CSWCObject* AsSWCObject();              // 0x0c
    virtual void* AsSWSObject();                    // 0x10
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void* AsSWSDoor();                      // 0x18
    virtual void* AsSWCModule();                    // 0x1c
    virtual void* AsSWSModule();                    // 0x20
    virtual void* AsSWCArea();                      // 0x24
    virtual void* AsSWSArea();                      // 0x28
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void* AsSWSCreature();                  // 0x30
    virtual void* AsSWCItem();                      // 0x34
    virtual void* AsSWSItem();                      // 0x38
    virtual CSWCTrigger* AsSWCTrigger();            // 0x3c
    virtual void* AsSWSTrigger();                   // 0x40
    virtual void* AsSWCProjectile();                // 0x44
    virtual void* AsSWSPlaceable();                 // 0x48
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c
    virtual void* As50(); virtual void* As54(); virtual void* As58();
    virtual void* As5C(); virtual void* As60(); virtual void* As64();
    virtual void* As68(); virtual void* As6C();
    virtual CExoString GetName();                   // 0x70

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCObject : public CGameObject {
public:
    CSWCObject(unsigned char nObjectType);
    virtual ~CSWCObject();
    virtual CExoString GetName();                   // 0x70
    virtual void AIUpdate() = 0;                    // 0x74
    virtual int LoadModel(int a, int b, int c) = 0; // 0x78
    virtual void UnloadModel(int a) = 0;            // 0x7c
    virtual void Unknown80() = 0;                   // 0x80
    virtual void SetArea(CSWCArea* pArea);          // 0x84
    virtual void SetOrientation(const Vector& v);   // 0x88
    virtual void SetPosition(const Vector& v);      // 0x8c
    virtual void Unknown90();                       // 0x90 (empty)
    virtual void AttachToScene(CAurScene* pScene);  // 0x94
    virtual CAurObject* GetModel(unsigned char nPart, int b);  // 0x98
    virtual void SetAnimation(unsigned short nAnim, int a, int b);  // 0x9c
    virtual void AddFireForgetAnimation(unsigned short nAnim, void* pData);  // 0xa0
    virtual int Hilite(int a, int b, int c, int d); // 0xa4
    virtual int UnHilite(int a);                    // 0xa8
    virtual int SetEnvMapPart(unsigned char n, int a, int b, const CResRef& r);  // 0xac
    virtual int SetEnvMap(int a, int b, const CResRef& r);  // 0xb0
    virtual int AddVisualEffect(unsigned short nEffect, int nDuration,
                                unsigned long nTarget, unsigned long nNode,
                                unsigned char nA, unsigned char nB,
                                const Vector& vOffset);  // 0xb4
    virtual int DeleteVisualEffect(unsigned short nEffect);  // 0xb8
    virtual void KillAllVisualEffects();            // 0xbc
    virtual int RefreshVisualEffects(int a);        // 0xc0
    virtual void UpdateVisualEffectsList();         // 0xc4
    virtual void StopAllVisualEffects();            // 0xc8
    virtual int HasVisualEffectApplied(int nEffect);  // 0xcc
    virtual CAurObject* GetCameraFollowPart();      // 0xd0
    virtual int UnknownD4();                        // 0xd4 (return 1)
    virtual float UnknownD8();                      // 0xd8
    virtual void ActivateHardCodedVisualWithHighestPriority(unsigned char nType,
                                                    CSWCVisualEffectOnObject* pEffect);  // 0xdc
    virtual void DetachVisualEffects();             // 0xe0
    virtual void ReAttachVisualEffects();           // 0xe4
    virtual void UnknownE8();                       // 0xe8 (empty)
    virtual void SetPortrait(const CResRef& r);     // 0xec
    virtual CResRef GetPortrait(unsigned char n);   // 0xf0
    virtual void SetPortraitId(unsigned short n);   // 0xf4
    virtual unsigned short GetPortraitId();         // 0xf8
    virtual int UnknownFC();                        // 0xfc (return 1)
    virtual void RemoveShadowBlob();                // 0x100
    virtual void ShowShadowBlob();                  // 0x104
    virtual void HideShadowBlob();                  // 0x108
    virtual void SetGroundNormal(const Vector& v);  // 0x10c
    virtual void TurnOnShadows();                   // 0x110
    virtual void TurnOffShadows();                  // 0x114
    virtual void Unknown118();                      // 0x118 (empty)
    virtual int Unknown11C();                       // 0x11c (return 0)
    virtual void BarkString(int a, int b, int c, int d);  // 0x120
    virtual void BarkBubble(int a, int b, int c, int d);  // 0x124
    virtual int Unknown128(int a);                  // 0x128 (return 0)
    virtual void ClientServerSyncHandler_Position(const Vector& v, int a, int b);  // 0x12c
    virtual void ClientServerSyncHandler_Orientation(const Vector& v);  // 0x130
    virtual void ClientServerSyncHandler_Animation(unsigned long a, int b);  // 0x134
    virtual int IsHostile();                        // 0x138
    virtual unsigned char GetReticlePoint(Vector& v);  // 0x13c
    virtual float Unknown140();                     // 0x140
    virtual float GetStickyMouseThreshhold();       // 0x144

    void AddFireForgetAnimation(unsigned short nAnim, unsigned long nDuration, float fSpeed,
                                void* pData, unsigned long nDelay);  // 0x0063C760
    unsigned long GetProjectileTravelTime(const Vector& vTarget, int bFast);  // 0x0063D010 (placeholder name)
    void DetachVisualEffectOfSameType(CSWCVisualEffectOnObject* pEffect);  // 0x0063DD50 (placeholder name)
    float GetVisualEffectAlpha();
    float GetFinalFadeAlpha();
    int FadeObject();
    void SetFadeState(unsigned char nState);
    void SetDesiredFadeState(unsigned char nState, int bTimed, unsigned long nDuration, unsigned long nDelay);
    void SetObjectVisibilityOnSetArea(CSWCArea* pArea);

    unsigned long m_n0C;                            // 0x0c
    unsigned long m_n10;                            // 0x10
    unsigned long m_n14;                            // 0x14
    unsigned long m_n18;                            // 0x18
    unsigned long m_n1C;                            // 0x1c
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
    char pad_0x3c[0x44 - 0x3c];
    unsigned long m_nDeltaTime;                     // 0x44 (placeholder name)
    unsigned short m_nCurrentAnimation;             // 0x48
    unsigned short m_nPendingAnimation;             // 0x4a
    float m_fAnimationSpeed;                        // 0x4c
    int m_n50;                                      // 0x50
    int m_n54;                                      // 0x54
    unsigned short m_nLoopingAnimation;             // 0x58
    unsigned short m_nAnimationCapacity;            // 0x5a
    FireForgetAnimation* m_pAnimationQueue;         // 0x5c
    unsigned char m_nAnimationCount;                // 0x60
    int m_n64;                                      // 0x64
    CSWCAnimBase* m_pAnimationBase;                 // 0x68
    CExoLinkedList<CSWCVisualEffectOnObject> m_lVisualEffects;  // 0x6c
    CExoArrayList<unsigned long> m_lstAnimationCallbacks;       // 0x70 (placeholder name)
    int m_n7C;                                      // 0x7c
    unsigned long m_nFadeStartDay;                  // 0x80
    unsigned long m_nFadeStartTime;                 // 0x84
    unsigned long m_nFadeDuration;                  // 0x88
    unsigned long m_nFadeDelay;                     // 0x8c
    unsigned char m_nFadeState;                     // 0x90
    unsigned char m_nDesiredFadeState;              // 0x91
    int m_bTimedFade;                               // 0x94
    float m_fFadeAlpha;                             // 0x98
    int m_n9C;                                      // 0x9c
    int m_nA0;                                      // 0xa0
    float m_fA4;                                    // 0xa4
    CSWPortrait m_cPortrait;                        // 0xa8
    unsigned short m_nPortraitId;                   // 0xb8
    unsigned short m_nBA;                           // 0xba
    unsigned long m_oidBC;                          // 0xbc
    int m_nC0;                                      // 0xc0
    int m_nC4;                                      // 0xc4
    CExoSoundSource* m_pVocalization;               // 0xc8
    CExoStreamingSoundSource* m_pStreamingVocalization;  // 0xcc
    int m_nVocalizationType;                        // 0xd0
    int m_bShadowBlob;                              // 0xd4
    Vector m_vGroundNormal;                         // 0xd8
    int m_bNoServerLink;                            // 0xe4
    CExoString m_sNameInfo;                         // 0xe8
    int m_nNameInfoTime;                            // 0xf0
    int m_nF4;                                      // 0xf4
    CSWSObject* m_pServerObject;                    // 0xf8
};

class CSWCCreature : public CSWCObject {
public:
    char pad_0xfc[0x3a4 - 0xfc];
    void* m_p3A4;                                   // 0x3a4 (placeholder name)
    void SetPartyMember(int bMember);               // 0x0060EF50
};

#endif
