// Minimal client object layer for bucket k: CExoString, Vector, the Aurora
// object / animation-base vtables and CSWCObject (0xFC bytes). Offsets agree
// with include/k/sw_c_placeable_door.h and include/n/object_control_family.h.
#ifndef K_SW_C_CLIENT_OBJECT_H
#define K_SW_C_CLIENT_OBJECT_H

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();
    CExoString(const CExoString&);
    ~CExoString();
};

class CResRef {
public:
    char m_resRef[16];
    char* GetResRefStr() const;                                      // 0x00405FE0
};

class Vector {
public:
    float x, y, z;
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
};

class CSWCObject;
class PartTrigger;
class Scene;

class CAurObject {
public:
    virtual ~CAurObject();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void SetClientObject(CSWCObject*);     // +0x74
    virtual void v78();
    virtual void SetObjectType(int);               // +0x7C
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0();
    virtual void SetRenderMode(int);               // +0xC4, descriptive
    virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void EnableRenderOrder();              // +0xDC, descriptive
};

class CSWCAnimBase {
public:
    virtual ~CSWCAnimBase();
    virtual void v04();
    virtual CAurObject* GetModel(unsigned char);                     // +0x08
    virtual int LoadModel(const CResRef&, unsigned char);            // +0x0C
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50();
    virtual int PlayAnim(int, float, int, int);                      // +0x54
    virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual void UnloadModel();                                      // +0x78
};

class CSWSArea {
public:
    int LineSegmentsIntersect(float, float, float, float, float, float, float, float,
                              float*, float*);                       // 0x0058F840
};

class CGameObject {
public:
    virtual ~CGameObject();
};

class CClientObject : public CGameObject {
public:
    virtual ~CClientObject();
};

class CSWCObject : public CClientObject {
public:
    CSWCObject(unsigned char);                                       // 0x0063E530
    virtual ~CSWCObject();                                           // 0x0063E800
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CExoString GetName();                                    // +0x70
    virtual void AIUpdate();                                         // +0x74
    virtual int LoadModel(const CResRef&, unsigned char, int);       // +0x78
    virtual void v7C(); virtual void v80(); virtual void v84(); virtual void v88();
    virtual void SetPosition(const Vector&);                         // +0x8C (base 0x0063F340)
    virtual void v90();
    virtual void SetDisplayed(int);                                  // +0x94, descriptive
    virtual void v98(); virtual void v9C(); virtual void vA0();
    virtual int Hilite(Vector*, int, int, int);                      // +0xA4 (0x0063CB00)
    virtual int UnHilite(int);                                       // +0xA8 (0x0063CB30)

    unsigned long m_oid;                                             // +0x04
    char pad_0x8[0x20 - 0x8];
    CSWSArea* m_pArea;                                               // +0x20 (as used by trigger edge tests)
    Vector m_vPosition;                                              // +0x24
    Vector m_vOrientation;                                           // +0x30
    unsigned long m_nDay;                                            // +0x3C
    unsigned long m_nTick;                                           // +0x40
    unsigned long m_nElapsed;                                        // +0x44
    unsigned short m_nAnimation;                                     // +0x48
    char pad_0x4a[0x64 - 0x4a];
    int m_nFireForgetTime;                                           // +0x64
    CSWCAnimBase* m_pModel;                                          // +0x68
    char pad_0x6c[0xe4 - 0x6c];
    int m_nE4;                                                       // +0xE4
    char pad_0xe8[0xfc - 0xe8];

    float GetAnimationLength(unsigned long);                         // 0x0063C0E0
    int AnimationLooping(unsigned short);                            // 0x0063C300
    int AnimationFireAndForget(unsigned short);                      // 0x0063C380
    int GetTopFireForgetAnimation(unsigned short*, float*, unsigned long*, void**); // 0x0063C610
    void AddFireForgetAnimation(unsigned short, unsigned long, float, void*, unsigned long); // 0x0063C760
    void RemoveTopFireForgetAnimation();                             // 0x0063C9C0
};

typedef char KClientObjectSize[(sizeof(CSWCObject) == 0xfc) ? 1 : -1];

class CSWCArea;
class CGameObjectArray {
public:
    unsigned char AddInternalObject(unsigned long*, CGameObject*, int); // 0x004D80E0
    unsigned char Delete(unsigned long);                              // 0x004D8290
};

class CClientAIMaster {
public:
    int AddObject(CClientObject*);                                   // 0x00604A20
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long*, unsigned long*);               // 0x004ADE40
    int SubtractWorldTimes(unsigned long, unsigned long, unsigned long, unsigned long,
                           unsigned long*, unsigned long*);          // 0x004AE460
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                                               // +0x48
    CSWCArea* GetArea() { return m_pArea; }
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                                    // 0x005ED4F0
    CSWCModule* GetModule();                                         // 0x005ED530
    CGameObjectArray* GetGameObjectArray();                          // 0x005ED570
    CClientAIMaster* GetClientAIMaster();                            // 0x005ED6B0
};

class CAppManager {
public:
    void* vptr;
    CClientExoApp* m_pClientExoApp;                                  // +0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                                   // 0x007A39FC

#endif
