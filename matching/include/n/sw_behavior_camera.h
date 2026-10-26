// Client camera behaviours (swbehaviorcamera.o): the CAurBehavior-derived
// navigate, on-a-stick, free-look and death cameras.
// Layouts come from the original constructors at 0x00638670 (navigate),
// 0x00639FC0 (on a stick), 0x0063A5D0 (free look) and 0x0063BBD0 (death).
#ifndef MATCHING_N_SW_BEHAVIOR_CAMERA_H
#define MATCHING_N_SW_BEHAVIOR_CAMERA_H

extern "C" int __cdecl _stricmp(const char* a, const char* b);

char* MsgGetWord(char* szMsg);                       // 0x00463530
void MsgParseFloat(char* szText, float* pOut);       // 0x004635C0

#define N_CAM_VSLOTS4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Remove(T value) throw();   // 0x00445080
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator-() const { return Vector(-x, -y, -z); }
    void Normalize();                               // 0x004AB130
    Vector& operator+=(const Vector& v);            // 0x004A9740
    Vector& operator-=(const Vector& v);            // 0x004A9770
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }

    // Rotated +Y and +Z axes (inline in the camera code).
    Vector YAxis() const
    {
        return Vector(2.0f * (y * x - z * w),
                      1.0f - 2.0f * (x * x + z * z),
                      2.0f * (y * z + x * w));
    }
    Vector ZAxis() const
    {
        return Vector(2.0f * (x * z + y * w),
                      2.0f * (y * z - w * x),
                      1.0f - 2.0f * (x * x + y * y));
    }
};

Vector operator*(const Quaternion& q, const Vector& v);                 // 0x004A9BB0
Vector Normalize(const Vector& v);                                      // 0x004AB180
float GetFacing(Vector* pVector);                                      // 0x004AA0F0
float Pitch(const Vector& v);                                           // 0x004AA130
Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);    // 0x004ACAC0
float GetRoll(const Quaternion& q);                                     // 0x004AA020
float GetPitch(const Quaternion& q);                                    // 0x004A9F40
float GetEulerRoll(Quaternion q);                                       // 0x004ACB90, descriptive

// View object returned by Gob slot +0x04 for camera objects.
class CAurCameraView {
public:
    N_CAM_VSLOTS4(c0a) N_CAM_VSLOTS4(c0b) N_CAM_VSLOTS4(c0c) N_CAM_VSLOTS4(c0d)
    virtual void c1a0();
    virtual void SetViewAngle(float fAngle);                            // +0x44
};

class Gob {
public:
    virtual ~Gob();
    virtual CAurCameraView* GetCameraView();                            // +0x04
    virtual void g0a1(); virtual void g0a2(); virtual void g0a3();
    N_CAM_VSLOTS4(g0b) N_CAM_VSLOTS4(g0c) N_CAM_VSLOTS4(g0d)
    N_CAM_VSLOTS4(g1a)
    virtual void g1b0(); virtual void g1b1();
    virtual Vector SetPosition(Vector vPosition);                       // +0x5c
    virtual Quaternion SetOrientation(Quaternion qOrientation);         // +0x60
    virtual Vector GetPosition();                                       // +0x64
    virtual Quaternion GetOrientation();                                // +0x68

    CExoArrayList<Gob**> m_lWatchers;                                   // +0x04
};

// Back-referenced Gob pointer: registers its slot with the Gob's watcher
// list so the Gob can clear it when it dies.
class GobRef {
public:
    Gob* m_p;

    GobRef() { m_p = 0; }
    ~GobRef()
    {
        if (m_p) {
            CExoArrayList<Gob**>& l = m_p->m_lWatchers;
            l.Remove(&m_p);
        }
    }
    GobRef& operator=(Gob* p);                      // 0x00447FA0
    GobRef& operator=(const GobRef& r) { return *this = r.m_p; }
    void Clear()
    {
        if (m_p)
            m_p->m_lWatchers.Remove(&m_p);
        m_p = 0;
    }
    operator Gob*() const { return m_p; }
    Gob* operator->() const { return m_p; }
};

// Object returned by CAurBehavior slot +0x1C; only its accumulated tilt
// at +0x48 is used here (descriptive names).
class CSWCameraTiltTarget {
public:
    char pad_0x00[0x48];
    float m_fTilt;                              // +0x48
};

class CAurBehavior {
public:
    CExoArrayList<GobRef*> m_lDependencies;     // +0x04
    Gob* m_pGob;                                // +0x10

    CAurBehavior(Gob* pGob);                    // 0x0049F110
    virtual ~CAurBehavior();                    // 0x0049FA70
    virtual void UpdateScenes();                // +0x04
    virtual int Control(float fDelta);          // +0x08
    virtual void Message(char* szMsg);          // +0x0c
    virtual int GetType();                      // +0x10
    virtual int GetBehaviorFlag14();            // +0x14
    virtual int GetBehaviorFlag18();            // +0x18
    virtual CSWCameraTiltTarget* GetTiltTarget();   // +0x1c, descriptive

    void AddDependency(Gob* pGob);              // 0x0049F130
    void RemoveDependency(Gob* pGob);           // 0x0049F1F0
};

class CClientOptions;
class CSWCModule;
class CSWCameraTiltTarget;
class CClientExoApp {
public:
    void EnableVideoEffect(int nEffect);        // 0x005EDF10
    void DisableVideoEffect();                  // 0x005EDF20
    CSWCModule* GetModule();                    // 0x005ED530
    CClientOptions* GetClientOptions();         // 0x005ED700
};
class CAppManager {
public:
    void* pad;
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;

// Camera state captured by SaveCameraSettings for a later restore.
class CSWCameraSettings {
public:
    int m_nValid;                               // +0x00
    int m_nType;                                // +0x04
    Gob* m_pCamera;                             // +0x08
    GobRef m_rTarget;                           // +0x0c
    int m_nStyle;                               // +0x10
    int m_n14;                                  // +0x14
    float m_f18;                                // +0x18
    int m_nVideoEffect;                         // +0x1c
};

// vtable 0x007515F4
class CSWCameraNavigate : public CAurBehavior {
public:
    float m_fTurn;                              // +0x14
    float m_fTilt;                              // +0x18
    float m_fSpeed;                             // +0x1c
    float m_fRaise;                             // +0x20
    float m_fStrafe;                            // +0x24
    float m_fRoll;                              // +0x28
    float m_f2C;                                // +0x2c

    CSWCameraNavigate(Gob* pGob);
    virtual ~CSWCameraNavigate();
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();
};

// vtable 0x0075169C
class CSWCameraDeath : public CAurBehavior {
public:
    GobRef m_rTarget;                           // +0x14
    float m_yaw;                                // +0x18
    float m_speedYaw;                           // +0x1c
    float m_desiredPitch;                       // +0x20
    float m_desiredDistance;                    // +0x24
    float m_logYaw;                             // +0x28
    float m_logPitch;                           // +0x2c
    float m_logDistance;                        // +0x30
    float m_f34;                                // +0x34
    float m_f38;                                // +0x38
    float m_f3C;                                // +0x3c

    CSWCameraDeath(Gob* pGob, Gob* pTarget);
    virtual ~CSWCameraDeath();
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();

    void SetTarget(Gob* pTarget);
};

class CClientOptions {
public:
    float GetFarClip();                         // 0x0061D5E0
    float GetFarClip2();                        // 0x0061D610
    float GetFarClip3();                        // 0x0061D640
};

// Collision/clip helpers embedded in the cameras (descriptive names): the
// base at free-look +0x7C (constructor 0x006D0B20) and the derived helper at
// on-a-stick +0xB4 (constructor 0x006D10B0, vtable 0x00757BD8).
class CSWCameraClipBase {
public:
    CSWCameraClipBase();                        // 0x006D0B20
    virtual void v00();

    char pad_0x04[0x40 - 0x04];
};

class CSWCameraClipHelper : public CSWCameraClipBase {
public:
    CSWCameraClipHelper();                      // 0x006D10B0

    float m_fFarClip;                           // +0x40
    float m_fFarClip2;                          // +0x44
    float m_fFarClip3;                          // +0x48
};

extern const int g_nCameraSettingsTypeOnAStick;     // 0x0075182C
extern int g_nPendingCameraStyle;                   // 0x007A242C

// Client object: only its id and position are used by the cameras.
class CSWCObject {
public:
    virtual void v00();

    unsigned long m_oidSelf;                    // +0x04
    char pad_0x08[0x24 - 0x08];
    Vector m_vPosition;                         // +0x24
};

extern int g_bSwapMouseButtons;                     // 0x008338F0
extern float g_fCameraTiltUpLimit;                  // 0x007A2420
extern float g_fCameraTiltDownLimit;                // 0x007A2424
extern float g_fCameraRotation;                     // 0x007A2428

// vtable 0x0075165C
class CSWCameraOnAStick : public CAurBehavior {
public:
    GobRef m_rTarget;                           // +0x14
    GobRef m_rCombatTarget;                     // +0x18
    Gob* m_pCamera;                             // +0x1c
    int m_n20;                                  // +0x20
    int m_n24;                                  // +0x24
    Quaternion m_qOrientation;                  // +0x28
    Vector m_vLastPosition;                     // +0x38
    Vector m_vDesiredPosition;                  // +0x44
    Vector m_vPosition;                         // +0x50
    int m_nStyle;                               // +0x5c
    int m_n60;                                  // +0x60
    int m_n64;                                  // +0x64
    int m_n68;                                  // +0x68
    float m_f6C;                                // +0x6c
    int m_n70;                                  // +0x70
    Vector m_v74;                               // +0x74
    unsigned long m_oidTarget;                  // +0x80
    float m_f84;                                // +0x84
    float m_f88;                                // +0x88
    float m_f8C;                                // +0x8c
    float m_fYawOffset;                         // +0x90
    float m_f94;                                // +0x94
    float m_f98;                                // +0x98
    float m_f9C;                                // +0x9c
    float m_fA0;                                // +0xa0
    float m_fA4;                                // +0xa4
    float m_fA8;                                // +0xa8
    float m_fAC;                                // +0xac
    int m_bSwapMouseButtons;                    // +0xb0
    CSWCameraClipHelper m_cClip;                // +0xb4
    float m_f100;                               // +0x100
    float m_fTiltSpeed;                         // +0x104
    float m_fPitch;                             // +0x108
    float m_f10C;                               // +0x10c
    float m_fDist;                              // +0x110
    float m_fSpeed;                             // +0x114
    float m_f118;                               // +0x118
    float m_fPresetPitch;                       // +0x11c
    float m_fHeight;                            // +0x120
    float m_f124;                               // +0x124
    Vector m_v128;                              // +0x128
    float m_fTiltRate;                          // +0x134

    CSWCameraOnAStick(Gob* pGob, Gob* pTarget);
    CSWCameraOnAStick(CSWCameraSettings* pSettings);
    virtual ~CSWCameraOnAStick();
    virtual int Control(float fDelta);
    virtual void Message(char* szMsg);
    virtual int GetType();

    Vector ComputeDesiredPosition(Vector vHead);
    float AccelerateInterp(float fCurrent, float fMin, float fMax, Vector vFrom, Vector vTo, float fDelta);
    void ResetLastPosition(Vector vPosition, Quaternion qOrientation);
    void SetCameraStyle(int nStyle);                // 0x00638070
    void ClearTarget();
    void OrientTowardTarget();
    void SetCameraInitialState();
    Quaternion ComputeOrientation(float fDelta);
    void ChangeTarget(Gob* pTarget, int bUnused);
    void SetLookAtTarget(CSWCObject* pObject);
    Vector GetCameraPosition();
    void SetCameraPosition(Vector vPosition);
    void UpdateClipDistances();
    void ClearCombatTarget();
    void SaveCameraSettings(CSWCameraSettings* pSettings);
};

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString(const char* szSource);           // 0x005E5A90
    ~CExoString();                              // 0x005E5C20
};

class C2DA {
public:
    char pad_0x00[0x24];
    int m_nNumRows;                             // +0x24

    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);    // 0x00413430
};
class CTwoDimArrays {
public:
    char pad_0x00[0x74];
    C2DA* m_pCameraStyles;                      // +0x74
};
class CSWRules {
public:
    char pad_0x00[0xb8];
    CTwoDimArrays* m_p2DArrays;                 // +0xb8
};
extern CSWRules* g_pRules;                      // 0x007A3A28

class CSWCArea {
public:
    char pad_0x00[0xb8];
    int m_nCameraStyle;                         // +0xb8
};
// Camera object owned by the module (+0x40); behaviours are looked up by
// type (-1 = current).
class CSWCModuleCamera {
public:
    N_CAM_VSLOTS4(m0a) N_CAM_VSLOTS4(m0b) N_CAM_VSLOTS4(m0c) N_CAM_VSLOTS4(m0d)
    N_CAM_VSLOTS4(m1a) N_CAM_VSLOTS4(m1b) N_CAM_VSLOTS4(m1c) N_CAM_VSLOTS4(m1d)
    virtual CAurBehavior* GetBehavior(int nType);   // +0x80
};

class CSWCModule {
public:
    char pad_0x00[0x0c];
    int m_nCameraMode;                          // +0x0c
    char pad_0x10[0x40 - 0x10];
    CSWCModuleCamera* m_pCamera;                // +0x40
    char pad_0x44[0x48 - 0x44];
    CSWCArea* m_pArea;                          // +0x48
    char pad_0x4c[0x80 - 0x4c];
    unsigned long m_nCameraFlags;               // +0x80
    char pad_0x84[0x94 - 0x84];
    float m_fCameraTilt;                        // +0x94

    void TiltCamera(float fAmount);
    void AcclTiltCamera(float fAmount);
};

// vtable 0x0075167C
class CSWCameraFreeLook : public CAurBehavior {
public:
    GobRef m_rTarget;                           // +0x14
    Gob* m_pCamera;                             // +0x18
    int m_n1C;                                  // +0x1c
    float m_f20;                                // +0x20
    float m_f24;                                // +0x24
    float m_fRotateSpeed;                       // +0x28
    float m_fTiltSpeed;                         // +0x2c
    float m_fLookUp;                            // +0x30
    float m_fLookDown;                          // +0x34
    float m_f38;                                // +0x38
    Quaternion m_qOrientation;                  // +0x3c
    Vector m_v4C;                               // +0x4c
    Vector m_v58;                               // +0x58
    Vector m_v64;                               // +0x64
    float m_f70;                                // +0x70
    float m_f74;                                // +0x74
    int m_nVideoEffect;                         // +0x78
    CSWCameraClipBase m_cClip;                  // +0x7c
    float m_fLookLimit;                         // +0xbc
    float m_fC0;                                // +0xc0

    CSWCameraFreeLook(Gob* pGob, Gob* pTarget, int nVideoEffect);
    CSWCameraFreeLook(CSWCameraSettings* pSettings);
    virtual ~CSWCameraFreeLook();
    virtual int Control(float fDelta);
    virtual int GetType();

    void UpdateCamraStyle();
};

#endif
