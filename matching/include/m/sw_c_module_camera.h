// CSWCModule camera controls (nwcmodule.o) in bucket m. Camera behaviours
// follow n/sw_behavior_camera.h; module field names are descriptive.
#ifndef MATCHING_M_SW_C_MODULE_CAMERA_H
#define MATCHING_M_SW_C_MODULE_CAMERA_H

#define M_CAM_VSLOTS4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Remove(T value);                           // 0x00445080
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector& operator+=(const Vector& v);            // 0x004A9740
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Vector ToDirection();                           // 0x0043E250
};

Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);   // 0x004ACAC0
Quaternion Slerp(Quaternion qFrom, const Quaternion& qTo, float fT);   // 0x004A9E00
Quaternion Normalize(Quaternion q);                                    // 0x004A9D60, descriptive

float GetFacing(Vector* pVector);                   // 0x004AA0F0
float GetPitch(const Quaternion& q);                // 0x004A9F40

class Gob {
public:
    virtual ~Gob();
    M_CAM_VSLOTS4(g04) M_CAM_VSLOTS4(g14) M_CAM_VSLOTS4(g24) M_CAM_VSLOTS4(g34)
    M_CAM_VSLOTS4(g44) virtual void g54(); virtual void g58();
    virtual Vector SetPosition(Vector vPosition);       // +0x5c
    virtual Quaternion SetOrientation(Quaternion q);    // +0x60
    virtual Vector GetPosition();                       // +0x64
    virtual Quaternion GetOrientation();                // +0x68

    CExoArrayList<Gob**> m_lWatchers;               // +0x04
};

class GobRef {
public:
    Gob* m_p;

    GobRef& operator=(Gob* p);                      // 0x00447FA0

    ~GobRef()
    {
        if (m_p) {
            CExoArrayList<Gob**>& l = m_p->m_lWatchers;
            l.Remove(&m_p);
        }
    }
};

// Camera state captured by SaveCameraSettings (see n/sw_behavior_camera.h).
class CSWCameraSettings {
public:
    int m_nValid;                                   // +0x00
    int m_nType;                                    // +0x04
    Gob* m_pCamera;                                 // +0x08
    GobRef m_rTarget;                               // +0x0c
    int m_nStyle;                                   // +0x10
    int m_nFollowBody;                              // +0x14
    float m_fFollowHeight;                          // +0x18
    int m_nVideoEffect;                             // +0x1c

    ~CSWCameraSettings();
};

// Object returned by CAurBehavior slot +0x1c (the stick parameters).
class CSWCameraTiltTarget {
public:
    char pad_0x00[0x24];
    Vector m_vUp;                                   // +0x24
    float m_f30;                                    // +0x30
    float m_fZoom;                                  // +0x34
    char pad_0x38[0x40 - 0x38];
    float m_fTurn;                                  // +0x40
    float m_f44;                                    // +0x44
    float m_fTilt;                                  // +0x48
    char pad_0x4c[0x5c - 0x4c];
    float m_f5C;                                    // +0x5c
    float m_f60;                                    // +0x60
    char pad_0x64[0x68 - 0x64];
    float m_f68;                                    // +0x68
    float m_f6C;                                    // +0x6c
    char pad_0x70[0x90 - 0x70];
    float m_fShakeYaw;                              // +0x90
    float m_fShakePitch;                            // +0x94
};

class CAurBehavior {
public:
    virtual ~CAurBehavior();
    virtual void UpdateScenes();                    // +0x04
    virtual int Control(float fDelta);              // +0x08
    virtual void Message(char* szMsg);              // +0x0c
    virtual int GetType();                          // +0x10
    virtual int GetBehaviorFlag14();                // +0x14
    virtual int GetBehaviorFlag18();                // +0x18
    virtual CSWCameraTiltTarget* GetTiltTarget();   // +0x1c, descriptive

    CExoArrayList<GobRef*> m_lDependencies;         // +0x04
    Gob* m_pGob;                                    // +0x10
};

// Behaviours that carry a shake offset pair (descriptive field names).
class CSWCameraShake2C : public CAurBehavior {
public:
    char pad_0x14[0x2c - 0x14];
    float m_fShakeYaw;                              // +0x2c
    float m_fShakePitch;                            // +0x30
};

class CSWCameraShake70 : public CAurBehavior {
public:
    char pad_0x14[0x70 - 0x14];
    float m_fShakeYaw;                              // +0x70
    float m_fShakePitch;                            // +0x74
};

// vtable 0x0075165C (only the fields used here)
class CSWCameraOnAStick : public CAurBehavior {
public:
    CSWCameraOnAStick(Gob* pGob, Gob* pTarget);             // 0x00639FC0
    CSWCameraOnAStick(CSWCameraSettings* pSettings);        // 0x0063AD60
    void ResetLastPosition(Vector vPosition, Quaternion qOrientation);  // 0x00637C20

    char pad_0x14o[0x60 - 0x14];
    int m_bKeepOrientation;                         // +0x60, descriptive
    char pad_0x64[0x68 - 0x64];
    int m_nFollowBody;                              // +0x68
    float m_fFollowHeight;                          // +0x6c
    char pad_0x70[0x90 - 0x70];
    float m_fShakeYaw;                              // +0x90
    float m_fShakePitch;                            // +0x94

    void SaveCameraSettings(CSWCameraSettings* pSettings);  // 0x0063BA80
    Vector GetCameraPosition();                             // 0x0063F600

    char pad_0x98[0x100 - 0x98];
    float m_f100;                                   // +0x100
    float m_fTiltSpeed;                             // +0x104
    float m_fPitch;                                 // +0x108
    float m_f10C;                                   // +0x10c
    char pad_0x110[0x138 - 0x110];
};

class CSWCameraNavigate : public CAurBehavior {
public:
    float m_fTurn;                                  // +0x14
    float m_fTilt;                                  // +0x18
};

class CSWBehaviorCameraCombat : public CAurBehavior {
public:
    float m_f14;                                    // +0x14
    float m_f18;                                    // +0x18
    char pad_0x1c[0x2c - 0x1c];
    float m_fTurnAccel;                             // +0x2c

    void SetTurn(float fTurn);                      // 0x006D1870, descriptive
    void RestoreSettings();                         // 0x006D22C0, descriptive
};

// Camera object owned by the module (+0x40); behaviours are looked up by
// type (-1 = current).
class CSWCModuleCamera {
public:
    virtual void m00();
    virtual Gob* GetGob();                              // +0x04
    virtual void m08(); virtual void m0c(); virtual void m10(); virtual void m14();
    virtual Vector SetPosition(Vector vPosition);       // +0x18
    virtual void m1c();
    virtual Vector GetPosition();                       // +0x20
    virtual Quaternion GetOrientation();                // +0x24
    virtual void m28(); virtual void m2c(); virtual void m30(); virtual void m34(); virtual void m38();
    virtual void GetClipPlanes(float* pNear, float* pFar);   // +0x3c
    virtual void SetClipPlanes(float fNear, float fFov);     // +0x40
    M_CAM_VSLOTS4(m44)
    virtual void m54(); virtual void m58();
    virtual void SetFollow(Gob* pModel);                // +0x5c
    virtual void m60();
    virtual float GetYaw();                             // +0x64
    virtual float GetPitch();                           // +0x68
    virtual float GetDistance();                        // +0x6c
    virtual void m70(); virtual void m74(); virtual void m78();
    virtual void SetBehavior(CAurBehavior* pBehavior);  // +0x7c
    virtual CAurBehavior* GetBehavior(int nType);       // +0x80
};

Vector Normalize(const Vector& v);                  // 0x004AB180

class CSWCCreature {
public:
    virtual void c00(); virtual void c04(); virtual void c08(); virtual void c0c();
    M_CAM_VSLOTS4(c10) M_CAM_VSLOTS4(c20) M_CAM_VSLOTS4(c30) M_CAM_VSLOTS4(c40)
    M_CAM_VSLOTS4(c50) M_CAM_VSLOTS4(c60) M_CAM_VSLOTS4(c70)
    virtual void c80(); virtual void c84();
    virtual void SetOrientation(const Vector& vDirection);  // +0x88
    virtual void c8c();
    M_CAM_VSLOTS4(c90) M_CAM_VSLOTS4(ca0) M_CAM_VSLOTS4(cb0) M_CAM_VSLOTS4(cc0)
    virtual Gob* GetGob();                          // +0xd0
    virtual int GetFollowBody();                    // +0xd4, descriptive
    virtual float GetHeadHeight();                  // +0xd8, descriptive

    int GetFreeLookVideoEffect();                   // 0x00610490, descriptive
    float GetCameraOffset();                        // 0x0060F010, descriptive

    unsigned short GetCurrentAnimation();           // 0x0063C060

    char pad_0x04[0x24 - 0x04];
    Vector m_vPosition;                             // +0x24
    Vector m_vOrientation;                          // +0x30
    char pad_0x3c[0x138 - 0x3c];
    int m_bIgnoreCameraClamp;                       // +0x138, descriptive
    char pad_0x13c[0x448 - 0x13c];
    float m_fCameraTurnAccel;                       // +0x448
};

class CSWCObject {
public:
    M_CAM_VSLOTS4(o00) M_CAM_VSLOTS4(o10) M_CAM_VSLOTS4(o20) M_CAM_VSLOTS4(o30)
    M_CAM_VSLOTS4(o40) M_CAM_VSLOTS4(o50) M_CAM_VSLOTS4(o60) M_CAM_VSLOTS4(o70)
    M_CAM_VSLOTS4(o80) virtual void o90(); virtual void o94();
    virtual Gob* GetModel(int nPart, int bLoad);    // +0x98, descriptive
};

class CGameObject {
public:
    virtual void g00(); virtual void g04(); virtual void g08();
    virtual CSWCObject* AsSWCObject();              // +0x0c
    M_CAM_VSLOTS4(g10)
    virtual void g20(); virtual void g24(); virtual void g28();
    virtual CSWCCreature* AsSWCCreature();          // +0x2c
    virtual void g30(); virtual void g34(); virtual void g38(); virtual void g3c();
};

struct CClientOptions {
    void SetCameraMode(unsigned char nMode);        // 0x0061B6F0

    unsigned char m_nFlags;                         // +0x00

    char pad_0x01[0x6d - 0x01];
    unsigned char m_nCameraMode;                    // +0x6d
};

class CSWParty {
public:
    CSWCCreature* GetCharacter(int nIndex);         // 0x006346C0
};

class CClientExoApp {
public:
    CSWParty* GetParty();                           // 0x005ED8B0
    int GetCombatMode();                            // 0x005EDE70
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CClientOptions* GetClientOptions();             // 0x005ED700
};

void* operator new(unsigned int nSize);
void operator delete(void* p);

// Behaviour constructors used when switching camera modes.
class CSWBehaviorCameraDialog : public CAurBehavior {
public:
    CSWBehaviorCameraDialog(Gob* pGob, unsigned long nShot);   // 0x006BB670
    char pad_0x14[0x9c - 0x14];
};

class CSWBehaviorCameraCombatMode : public CAurBehavior {
public:
    CSWBehaviorCameraCombatMode(Gob* pGob);         // 0x006D12B0
    char pad_0x14[0x94 - 0x14];
};

class CSWCameraNavigateMode : public CAurBehavior {
public:
    CSWCameraNavigateMode(Gob* pGob);               // 0x00638670
    char pad_0x14[0x30 - 0x14];
};

class CSWCameraFreeLook : public CAurBehavior {
public:
    CSWCameraFreeLook(Gob* pGob, Gob* pTarget, int nVideoEffect);  // 0x0063A5D0
    CSWCameraFreeLook(CSWCameraSettings* pSettings);               // 0x0063BAF0
    char pad_0x14[0xc4 - 0x14];
};

class CAppManager {
public:
    void* pad;
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

extern float g_fCameraShakePeriod;                  // 0x007A246C
extern float g_fCameraShakeDegrees;                 // 0x007A2470
extern float g_fCameraFollowA;                      // 0x007A2450
extern float g_fCameraFollowB;                      // 0x007A2454
extern float g_fCameraFollowC;                      // 0x007A2458
extern float g_fCameraFollowPitch;                  // 0x007A245C

class Scene;
class CSWCArea {
public:
    char pad_0x00[0x184];
    Scene* m_pScene;                                // +0x184
};

class CSWCModule {
public:
    char pad_0x00[0x0c];
    int m_nCameraMode;                              // +0x0c
    char pad_0x10[0x40 - 0x10];
    CSWCModuleCamera* m_pCamera;                    // +0x40
    char pad_0x44[0x48 - 0x44];
    CSWCArea* m_pArea;                              // +0x48
    Vector m_vFollowPosition;                       // +0x4c
    Vector m_vCameraTarget;                         // +0x58
    Vector m_vCameraUp;                             // +0x64
    unsigned long m_oidFollow;                      // +0x70
    float m_fSavedDistance;                         // +0x74
    float m_fSavedYaw;                              // +0x78
    float m_fSavedPitch;                            // +0x7c
    unsigned long m_nCameraFlags;                   // +0x80
    int m_n84;                                      // +0x84
    int m_n88;                                      // +0x88
    int m_bClearViewRequest;                        // +0x8c
    unsigned char m_n90;                            // +0x90
    char pad_0x91[0x94 - 0x91];
    float m_fCameraTilt;                            // +0x94
    float m_fCameraTurn;                            // +0x98
    float m_fCameraZoom;                            // +0x9c
    char pad_0xa0[0xd5 - 0xa0];
    unsigned char m_nLastCameraOption;              // +0xd5
    char pad_0xd6[0xd8 - 0xd6];
    int m_bResetCameraParams;                       // +0xd8
    int m_nShakeRemaining;                          // +0xdc
    int m_nShakeE0;                                 // +0xe0
    int m_nShakeDelay;                              // +0xe4
    int m_nShakeDuration;                           // +0xe8
    int m_bShakeChanged;                            // +0xec
    unsigned long m_nBump;                          // +0xf0
    int m_nBumpDuration;                            // +0xf4
    int m_bBumpChanged;                             // +0xf8
    int m_bFollowValid;                             // +0xfc
    int m_bTurnChanged;                             // +0x100
    int m_bZoomChanged;                             // +0x104
    char pad_0x108[0x10c - 0x108];
    CSWCameraSettings m_cSavedCamera;               // +0x10c
    CAurBehavior* m_pCameraModel;                   // +0x12c

    void InitializeShotCamera(unsigned long nShot);
    int SetFreeLookCamera(unsigned long oidTarget);
    int EnterCombatMode();
    int RestoreCamera();
    int GetNextCameraPosition(float fDelta, Vector* pOut);
    int SetNavigateCamera();
    void AcclTurnCamera(float fAmount, float fUnused);
    void ZoomCamera(float fAmount);
    void ScrollCamera(Vector vDelta);
    void UpdateCameraModeChaseCam();                // 0x00640700
    void UpdateCameraModeBG();
    void UpdateCameraModeEQ(float fDelta);          // 0x00640B90
    void UpdateCameraFollow(float fDelta);
    void ShakeCamera(int nDelay, int nDuration);
    void BumpCamera(int nDuration);
    void UpdateCameraBumpAndShake(float fDelta);
    float UpdateCameraCollision(float fDelta, float fZoom);
    float UpdateCameraCollisionDefault(float fDelta, float fZoom);  // 0x006422E0
    float UpdateCameraCollisionEQ(float fDelta, float fZoom);       // 0x006433B0
};

#endif
