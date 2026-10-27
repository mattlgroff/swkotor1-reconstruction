// Scene core / CHitInfo / SafePointer host (0x00451C80-0x00458ED0).
#ifndef MATCHING_H_SCENE_H
#define MATCHING_H_SCENE_H

#include "h/scenebsp.h"

class SafePointerBase;

// List of the SafePointers that currently reference the object; the
// destructor clears every one of them.
class SafePointerHost : public CExoArrayList<SafePointerBase*> {
public:
    ~SafePointerHost();                     // 0x00457820
};

// Any SafePointer target: vfptr at +0, host list at +4.
class CAurObject : public SafePointerHost {
public:
    virtual ~CAurObject();
};

class SafePointerBase {
public:
    CAurObject* m_p;

    SafePointerBase() { m_p = 0; }
    ~SafePointerBase()
    {
        if (m_p)
            m_p->RemoveNoThrow(this);
    }
    // operator=(NULL) shape: the host pointer is formed before the push.
    void Reset()
    {
        if (m_p) {
            SafePointerHost* pHost = m_p;
            pHost->Remove(this);
        }
        m_p = 0;
    }
    void Clear()
    {
        if (m_p)
            m_p->Remove(this);
        m_p = 0;
    }
    // operator=(object): unregister with the old host, register with the new.
    void Set(CAurObject* p);
};

__forceinline void SafePointerBase::Set(CAurObject* p)
{
    if (m_p)
        m_p->RemoveInline(this);
    m_p = p;
    if (p)
        p->Add(this);
}

class Vector0 : public Vector {
public:
    Vector0() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector0(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector0& operator=(const Vector& v) { Vector::operator=(v); return *this; }
};

class CHitInfo {
public:
    void* m_p0;                             // +0x00
    void* m_p4;                             // +0x04
    CExoArrayList<void*> m_lHits;           // +0x08
    int m_n14;                              // +0x14
    int m_n18;                              // +0x18
    Vector0 m_v1C;                          // +0x1c
    Vector0 m_v28;                          // +0x28
    int m_n34;                              // +0x34
    SafePointerBase m_pHit;                 // +0x38
    int m_n3C;                              // +0x3c
    Vector0 m_v40;                          // +0x40
    int m_n4C;                              // +0x4c
    int m_n50;                              // +0x50
    int m_n54;                              // +0x54
    int m_n58;                              // +0x58
    unsigned char m_b5C;                    // +0x5c
    unsigned char m_b5D;                    // +0x5d
    unsigned char m_b5E;                    // +0x5e
    unsigned char m_b5F;                    // +0x5f
    unsigned char m_b60;                    // +0x60

    CHitInfo(void* p0, void* p4, int n14);  // 0x004578F0
    ~CHitInfo();                            // 0x00456B30
    void ClearOutput();                     // 0x00456A60
    void SetHit(const Vector& v, int nType) { m_v1C = v; m_n18 = nType; }
    void Reset(void* p0, void* p4, int n14);    // 0x00456AF0
};

class ScenePart {
public:
    virtual void V000();
    virtual void V004();
    virtual void V008();
    virtual void V00C();
    virtual void V010();
    virtual void Animate(int n);            // +0x14
    virtual void V018();
    virtual void V01C();
    virtual void V020();
    virtual void V024();
    virtual void V028();
    virtual void V02C();
    virtual void V030();
    virtual void* GetLight();               // +0x34
    virtual void V038();
    virtual void V03C();
    virtual void V040();
    virtual void V044();
    virtual void V048();
    virtual void V04C();
    virtual void V050();
    virtual void* GetEmitter();             // +0x54
    virtual void V058();
    virtual void V05C();
    virtual void V060();
    virtual void V064();
    virtual void V068();
    virtual void V06C();
    virtual void V070();
    virtual void V074();
    virtual void V078();
    virtual void V07C();
    virtual void V080();
    virtual void V084();
    virtual void V088();
    virtual void V08C();
    virtual void V090();
    virtual void V094();
    virtual void V098();
    virtual void V09C();
    virtual void V0A0();
    virtual void V0A4();
    virtual void V0A8();
    virtual void V0AC();
    virtual void V0B0();
    virtual void V0B4();
    virtual void V0B8();
    virtual void V0BC();
    virtual void V0C0();
    virtual void V0C4();
    virtual void V0C8();
    virtual void V0CC();
    virtual void V0D0();
    virtual void V0D4();
    virtual void V0D8();
    virtual void V0DC();
    virtual void V0E0();
    virtual void V0E4();
    virtual void V0E8();
    virtual void V0EC();
    virtual void V0F0();
    virtual void V0F4();
    virtual void V0F8();
    virtual void V0FC();
    virtual void V100();
    virtual void V104();
    virtual void V108();
    virtual void V10C();
    virtual void V110();
    virtual void V114();
    virtual void V118();
    virtual void V11C();
    virtual void V120();
    virtual void V124();
    virtual void V128();
    virtual void V12C();
    virtual void V130();
    virtual void V134();
    virtual void V138();
    virtual void V13C();
    virtual void V140();
    virtual void V144();
    virtual void V148();
    virtual void V14C();
    virtual void V150();
    virtual void V154();
    virtual void V158();
    virtual void V15C();
    virtual int IsAnimated();               // +0x160
    char pad_0x4[0x12];
    unsigned char m_b16;                    // +0x16
    char pad_0x17[0x41];
    int m_n58;                              // +0x58
    char pad_0x5c[0x10];
    void* m_p6C;                            // +0x6c
};

class WindManager {
public:
    ~WindManager();                         // 0x004A6710
    void SetWind(Vector v);                 // 0x004A5900
    void SetVariation(float a, float b);    // 0x004A57A0
};

// Vector flavour with a user-declared destructor (the room constructor's
// temporary keeps an EH frame without any state stores).
class VectorD {
public:
    float x, y, z;

    VectorD(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    ~VectorD() {}
};

class BSPNodeBase {
public:
    BSPNodeBase(VectorD vNormal, float fDist);  // 0x004AD630
    ~BSPNodeBase();                             // 0x004ACED0
    char pad_0x0[0x1c];
    Vector m_vMin;                              // +0x1c
    Vector m_vMax;                              // +0x28
    char pad_0x34[0x5c - 0x34];
};

class RoomObject {
public:
    virtual ~RoomObject();
};

class CAurRoom : public BSPNodeBase {
public:
    CExoArrayList<CAurRoom*> m_lVisible;        // +0x5c
    CExoArrayList<RoomObject*> m_lObjects;      // +0x68
    RoomObject* m_pModel;                       // +0x74
    char m_szName[0x20];                        // +0x78

    CAurRoom(char* szName);                     // 0x004548A0
    ~CAurRoom();                                // 0x00452A60
};

class Scene;

class FlameGob {
public:
    virtual ~FlameGob();
    char pad_0x4[0x74];
    Vector m_vPosition;                         // +0x78
    Quaternion m_qOrientation;                  // +0x84

    void AttachToScene(Scene* pScene);          // 0x0043F200
    void GetParts(CExoArrayList<ScenePart*>* pList);    // 0x00447180
};

struct FlameData {
    char pad_0x0[8];
    Vector m_vPosition;                         // +0x08
    Quaternion m_qOrientation;                  // +0x14
};

class CAurScene : public SafePointerHost {
public:
    virtual ~CAurScene();                   // 0x00457990
};

// SafePointer whose destructor expands the host's Remove (Scene +0xa4).
class SafePointerInl {
public:
    CAurObject* m_p;

    ~SafePointerInl()
    {
        if (m_p)
            m_p->RemoveInline((SafePointerBase*)this);
    }
    void Reset()
    {
        if (m_p) {
            SafePointerHost* pHost = m_p;
            pHost->Remove((SafePointerBase*)this);
        }
        m_p = 0;
    }
};

class IntList;

class LightManager {
public:
    virtual void V00();
    virtual void BeginFrame();              // +0x04
    ~LightManager();                        // 0x004A2BC0
    IntList* GetLights(int nType);          // 0x004A4B10
    void GetNearestLights(Scene* pScene, Vector vPos, float fRadius, int n);    // 0x004A3770
    char pad_0x4[0xa0];
    void* m_pSun;                           // +0xa4
};

class SceneGob {
public:
    ~SceneGob();                            // 0x0044B2F0
    void AttachToScene(Scene* pScene);      // 0x0043F200
    char pad_0x0[0x1cc];
};

class Scene : public CAurScene {
public:
    char pad_0x10[0x44];
    CExoArrayList<ScenePart*> m_lChildren;  // +0x54
    int m_bFogEnabled;                      // +0x60
    float m_fFogStart;                      // +0x64
    float m_fFogEnd;                        // +0x68
    int m_nFogMode;                         // +0x6c
    float m_fFogDensity;                    // +0x70
    Vector m_vFogColor;                     // +0x74
    LightManager* m_pLights;                // +0x80
    WindManager* m_pWind;                   // +0x84
    char pad_0x88[0xc];
    CExoArrayList<void*> m_l94;             // +0x94
    BSPNodeBase* m_pBSP;                    // +0xa0
    SafePointerInl m_pTexture;              // +0xa4
    char pad_0xa8[0x14];
    CExoArrayList<void*> m_lBC;             // +0xbc
    CExoArrayList<CAurRoom*> m_lRooms;      // +0xc8
    CAurRoom* m_pCurrentRoom;               // +0xd4
    char pad_0xd8[0xc];
    void* m_pE4;                            // +0xe4
    void* m_pE8;                            // +0xe8
    CExoArrayList<ScenePart*> m_lAnimated;  // +0xec
    SceneGob m_gob;                         // +0xf8

    Scene(char* szName);                    // 0x00458790
    virtual ~Scene();                       // 0x00458BE0
    void Animate(int n);                    // 0x00451C80
    void ChildAdd(ScenePart* pPart);        // 0x00455670
    void ChildRemove(ScenePart* pPart);     // 0x00453900
    void ListSpecialPartsR(void* p);        // 0x004531A0
    void UnlistSpecialParts(void* p);       // 0x00453550
    void SetVisibility(CAurRoom* pRoom, CAurRoom* pOther, int bVisible);  // 0x00454940
    void SetSingleTileFlame(FlameGob* pFlame, FlameGob** ppCurrent, FlameData* pData);   // 0x00455370
    void DoCapBucket();                     // 0x00455A40
    void RenderOnlyGobs();                  // 0x00450D60
    void SetupFog();                        // 0x00450DE0
    float GetFogStart() { return m_fFogStart; }
    float GetFogEnd() { return m_fFogEnd; }
    float GetFogDensity() { return m_fFogDensity; }
    CAurRoom* FindRoom(char* szName);       // 0x00452B10
    int HitCheck(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo);       // 0x00456B60
    CAurObject* HitCheckGob(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo);    // 0x0044FE00
    int WriteVisibility(char* szDir, char* szName); // 0x00452B70
    CAurRoom* GetRoom(int i);               // 0x00452C60
    void SetCurrentRoom(int i);             // 0x00452C90
    void DoStaticMeshShadows();             // 0x00456250
};

#endif
