// Worker L matching headers: the combat camera behaviour
// CSWBehaviorCameraCombat (swbehaviorcameracombat.o, vtable 0x00757BDC,
// 0x94 bytes, GetType 0x1070, 0x006D12B0-0x006D22C0) and its clip helper.
#ifndef L_SW_BEHAVIOR_CAMERA_COMBAT_H
#define L_SW_BEHAVIOR_CAMERA_COMBAT_H

#include "l/sw_behavior_camera_dialog.h"

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString(const char* szSource);           // 0x005E5A90
    ~CExoString();                              // 0x005E5C20
};

class C2DA {
public:
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

class CClientOptions {
public:
    char pad_0x00[0x08];
    unsigned char m_nMouseFlags;                // +0x08 (bit 1: invert turning, descriptive)

    float GetBrightness();                      // 0x0061D5B0
    float GetFarClip();                         // 0x0061D5E0
    float GetFarClip2();                        // 0x0061D610
    float GetFarClip3();                        // 0x0061D640
};

class CSWCObject {
public:
    char pad_0x00[0x24];
    Vector m_vPosition;                         // +0x24
};

class CSWCCreatureAppearance {
public:
    float GetHeadNodeHeight();                  // 0x006973B0
    float GetHeadHeightOffset();                // 0x006974D0
};

class CSWCCreature : public CSWCObject {
public:
    char pad_0x30[0x21c - 0x30];
    CSWCCreatureAppearance* m_pAppearance;      // +0x21c
};

class CSWCModuleCamera {
public:
    virtual void m00(); virtual void m04(); virtual void m08(); virtual void m0c();
    virtual void m10(); virtual void m14(); virtual void m18(); virtual void m1c();
    virtual void m20();
    virtual Quaternion GetOrientation();        // +0x24
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();          // +0x0c
};

class CSWParty {
public:
    CSWCCreature* GetPlayerCharacter();         // 0x00635460
};

class CClientExoApp {
public:
    CClientOptions* GetClientOptions();         // 0x005ED700
    CSWParty* GetParty();                       // 0x005ED8B0
    unsigned long GetLastTarget();              // 0x005EDD80
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWCModuleCamera* GetModuleCamera();        // 0x005ED560
    int GetInputClass();                        // 0x005EDA70
    void HideMouse(int nFlags);                 // 0x005EE170
    void ShowMouse(int nFlags);                 // 0x005EE180
};

float GetFacing(Vector* pVector);               // 0x004AA0F0
float GetPitch(const Quaternion& q);            // 0x004A9F40
Vector Normalize(const Vector& v);              // 0x004AB180
float Magnitude(const Vector& v);               // 0x004AB100
Quaternion operator*(const Quaternion& a, const Quaternion& b);   // 0x004A9AC0
Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);    // 0x004ACAC0
class CAppManager {
public:
    void* pad;
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;              // 0x007A39FC

extern int g_bSwapMouseButtons;                 // 0x008338F0
extern int g_bRenderQA;                         // 0x008327EC

class CSWGuiManager {
public:
    void SaveMousePosition();                   // 0x0040A3D0
    void RestoreOldMousePosition();             // 0x0040C7D0
};
extern CSWGuiManager* g_pGuiManager;            // 0x007A39F4

// Spring integrators embedded in the cameras (descriptive names; the
// playable source calls them CSWRK4Acceleration / CSWRK4SplitAcceleration):
// a 4th-order Runge-Kutta step of {position, velocity} pulled towards an
// input point. Vtable 0x00757BD4 (base) / 0x00757BD8 (split).
struct CSWCameraClipState {
    CSWCameraClipState() : m_vPosition(0.0f, 0.0f, 0.0f), m_vVelocity(0.0f, 0.0f, 0.0f)
    {
        m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
        m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    }
    CSWCameraClipState(const Vector& vPosition, const Vector& vVelocity)
        : m_vPosition(vPosition), m_vVelocity(vVelocity)
    {
    }
    Vector m_vPosition;                         // +0x00
    Vector m_vVelocity;                         // +0x0c
};

CSWCameraClipState operator/(const CSWCameraClipState& s, float f);    // 0x006D0A40
Vector operator/(const Vector& v, float f);    // 0x00437A90

class CSWCameraClipBase {
public:
    CSWCameraClipBase();                        // 0x006D0B20
    virtual Vector GetAccelerationVector(const Vector& vPosition, const Vector& vInput);  // 0x006D0FC0

    void Accelerate(const Vector& vInput, float fStep);     // 0x006D0BD0
    CSWCameraClipState Update(Vector vInput, float fStep, int bClearVelocity, int bClearPosition,
                              int bClearNewVelocity, int bClearNewPosition);   // 0x006D1190
    void SetVelocity(float fVelocity);          // 0x006D1030

    Vector m_vLastInput;                        // +0x04
    CSWCameraClipState m_cState;                // +0x10
    CSWCameraClipState m_cNewState;             // +0x28
    float m_fMass;                              // +0x40
    float m_fGain;                              // +0x44
};

class CSWCameraClipHelper : public CSWCameraClipBase {
public:
    CSWCameraClipHelper();                      // 0x006D10B0
    virtual Vector GetAccelerationVector(const Vector& vPosition, const Vector& vInput);  // 0x006D10D0

    float m_fReleaseGain;                       // +0x48
};

class CSWBehaviorCameraCombat : public CAurBehavior {
public:
    CSWBehaviorCameraCombat(Gob* pGob);         // 0x006D12B0
    virtual int Control(float fDelta);          // +0x08 (0x006D1FC0)
    virtual int GetType();                      // +0x10 (0x006D14D0)

    Vector CalculateDesiredLocation(CSWCCreature* pCreature, CSWCObject* pTarget,
                                    float fDelta, float* pfYaw);   // 0x006D14E0
    float GetNextYaw(const Vector& vTarget, float fYaw, float fCurrent,
                     float* pfTurn);            // 0x006D16F0
    Vector GetNextPosition(const Vector& vDesired, float fDuration,
                           float fDelta);       // 0x006D1760
    void SetTurn(float fTurn);                  // 0x006D1870, descriptive
    void RestoreSettings();                     // 0x006D22C0, descriptive
    Vector CheckPosition(CSWCObject* pCreature, const Vector& vDesired,
                         const Vector& vFrom);  // 0x006D18E0, descriptive

    float m_f14;                                // +0x14
    float m_f18;                                // +0x18
    Vector m_v1C;                               // +0x1c
    float m_fTurn;                              // +0x28
    float m_fTurnAccel;                         // +0x2c
    float m_fPitch;                             // +0x30
    float m_fRotation;                          // +0x34
    float m_fDistance;                          // +0x38
    float m_fSpeed;                             // +0x3c
    float m_fHeight;                            // +0x40
    CSWCameraClipHelper m_cClip;                // +0x44
    int m_bSwapMouseButtons;                    // +0x90
};

#endif
