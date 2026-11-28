// CSWCObject (nwcobject.o) lifetime, visual-effect list, fade and position
// family. Self-contained layout for these translation units; the older
// object_control_family.h keeps its own partial view of the same object.
// Vtable 0x007516C8 slot names follow the playable build's vtable table
// (src/units/1461/f4c7ba6a63-w3_client_t3.cpp) and the Mac debug map.
#ifndef N_SW_C_OBJECT_H
#define N_SW_C_OBJECT_H

#include "shared/vector_ops.h"
class Quaternion {
public:
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Vector ToDirection();                           // 0x0043E250
};
Quaternion RotationArc(Vector vFrom, Vector vTo);   // 0x004AB630
float Magnitude(const Vector& v);                   // 0x004AB100
int operator==(const Vector& a, const Vector& b);   // 0x004AA980
#include "shared/exo_string.h"

#include "shared/res_ref.h"

class CSWPortrait {
public:
    char m_data[16];
    CSWPortrait();                                  // 0x00678EC0
    ~CSWPortrait();                                 // 0x004015A0
    CResRef GetPortrait(unsigned char n);           // 0x00678EF0
};

#include "shared/globals.h"
#include "shared/object_id.h"

// Linked list -----------------------------------------------------------


#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"




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

#include "shared/swc_visual_effect_on_object.h"

#include "shared/game_object.h"

class CSWSObject : public CGameObject {
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

#include "shared/app_manager.h"

// Object -----------------------------------------------------------------

struct FireForgetAnimation {
    int active;
    unsigned short animation;
    unsigned short pad_0x6;
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

#include "shared/game_object.h"

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
