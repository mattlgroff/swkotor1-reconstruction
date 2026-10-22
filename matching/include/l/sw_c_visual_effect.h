// Worker L matching headers: client visual effect (CSWCVisualEffectOnObject,
// 0x006A0480-0x006A1880) and the client classes it touches.
#ifndef L_SW_C_VISUAL_EFFECT_H
#define L_SW_C_VISUAL_EFFECT_H

#define OBJECT_INVALID 0x7f000000

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
};

Vector NormalizeVector(const Vector& v);            // 0x004AB180
float GetYaw(const Vector& vDirection);             // 0x004AA0F0

struct Quaternion {
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Quaternion(float fw, float fx, float fy, float fz) { w = fw; x = fx; y = fy; z = fz; }
};

Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);   // 0x004ACAC0

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(int n);                              // 0x005E5BC0
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    char* CStr() const;                             // 0x005E5670
    void Format(const char* szFormat, ...);         // 0x005E5680
};

class CResRef {
public:
    char m_resRef[16];
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CResRef& r);
};

// Aurora model object (NewCAurObject 0x00449CC0).
class CAurObject {
public:
    virtual ~CAurObject();                          // 0x00
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* szAnim, float fSpeed, int a, int b);   // 0x18
    virtual int GetEventTime(const char* szEvent, float* pStart, float* pEnd);   // 0x1c
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetVisible(int bVisible);          // 0x4c
    virtual void AttachToObject(CAurObject* pTo, const char* szNode, int n);  // 0x50
    virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90();
    virtual void SetPosition(const char* szNode, Vector vPosition, Quaternion qOrientation);    // 0x94
    virtual void GetPosition(const char* szNode, Vector* pPosition, Quaternion* pOrientation);  // 0x98
    virtual void v9C(); virtual void vA0();
    virtual void SetTarget(CAurObject* pTo, const char* szNode);  // 0xa4
    virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0();
    virtual void SetDrawMode(int n);                // 0xc4
    virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0();
    virtual void Restart();                         // 0xe4
};

CAurObject* NewCAurObject(const char* szModel, const char* szName, int a, int b);  // 0x00449CC0

void AurEnableSpeedBlur();                          // 0x0044F0A0
void AurDisableSpeedBlur();                         // 0x0044F0B0
void AurSetSpeedBlurRatio(float f);                 // 0x0044F130

class CExoSoundSource {
public:
    virtual ~CExoSoundSource();
    void Stop();                                    // 0x005D5A20
};

class C2DA {
public:
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);  // 0x00414110
};

class CTwoDimArrays {
public:
    char pad_0x0[0x88];
    C2DA* m_pVisualEffectTable;                     // 0x88
    C2DA* GetVisualEffectTable() { return m_pVisualEffectTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCObject;
class CSWCCreature;
class CSWCItem;

class CAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual void RefreshTextures();                 // 0x78
};

class CGuiPanel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64();
    virtual void SetActive(int b);                  // 0x68
};

class CGuiInGame {
public:
    char pad_0x0[0x184];
    CGuiPanel* m_pPanel;                            // 0x184
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CGuiInGame* m_pGuiInGame;                       // 0x48
};

class CCameraBehavior {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual int GetType();                          // 0x10

    char pad_0x4[0x80];
    int m_bLocked;                                  // 0x84
    char pad_0x88[0x88];
    unsigned long m_nMode;                          // 0x110
};

class CSWCModuleCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void SetFOV(float fFOV);                // 0x44
    virtual float GetFOV();                         // 0x48
    virtual void v4C(); virtual void v50();
    virtual void BlendFOV(float fFOV, float fTime, int n);  // 0x54
    virtual void StopFOVBlend(int n);               // 0x58
    virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual CCameraBehavior* GetBehavior(int nIndex);   // 0x80
};

class CClientOptions {
public:
    char pad_0x0[0x72];
    unsigned char m_nGraphicsDetail;                // 0x72
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c

    unsigned long m_idSelf;                         // 0x04
};

class CSWCArea {
public:
    void UpdateShadowingLights();                   // 0x00605410
};

class CSWCObject : public CGameObject {
public:
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CAurObject* GetModelPart(unsigned long nPart, int bRecurse);   // 0x98
    virtual void v9C(); virtual void vA0();
    virtual void SetColorOverlay(const Vector& vColor, int nMode, int a, int b);  // 0xa4
    virtual void SetDrawMode(int nMode);            // 0xa8
    virtual void vAC();
    virtual void SetEnvironmentMap(int a, int b, const CResRef& cEnvMap);   // 0xb0

    CAnimBase* GetAnimationBase();                  // 0x0063C070
    void ReplaceTextures(const CResRef& cTexture, const CResRef& cBump, int n);  // 0x0063C0C0
    void SetAlpha(float fAlpha, unsigned char nMask);   // 0x0063CB50
    void ClearSelfIllumination();                   // 0x0063CE10
    void RefreshSelfIllumination();                 // 0x0063CE30
    void ReplaceTexture(CResRef cTexture, float fTime);  // 0x0063D440
    void RestoreTexture();                          // 0x0063D460
    void StartFlash();                              // 0x0063D540
    void StopFlash();                               // 0x0063D550

    char pad_0x8[0x4];
    int m_bIllumination[3];                         // 0x0c
    char pad_0x18[0x8];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
};

class CSWSCreature {
public:
    int IsDebilitated();                            // 0x005B4880
};

class CSWCCreatureMovement {
public:
    char pad_0x0[0x5c];
    float m_fMaxSpeed;                              // 0x5c
};

class CSWCCreature : public CSWCObject {
public:
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    void Reset();                                   // 0x004015A0
    CSWSCreature* GetServerCreature();              // 0x0060FB20

    char pad_0x30[0x1ec];
    CSWCCreatureMovement* m_pMovement;              // 0x21c
    char pad_0x220[0x190];
    float m_fSpeed;                                 // 0x3b0
};

class CSWCItemHead {
public:
    virtual void v00();
    char pad_0x4[0xc];
};

class CSWCItem : public CSWCItemHead, public CSWCObject {
};

class CSWCMiniGame;

class CClientExoApp {
public:
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWCItem* GetItemByGameObjectID(unsigned long oid);     // 0x005ED5A0
    CSWCMiniGame* GetMiniGame();                    // 0x005EDB00
    CSWCModule* GetModule();                        // 0x005ED530
    CSWCModuleCamera* GetModuleCamera();            // 0x005ED560
    int GetPausedByCombat();                        // 0x005EDC10
    CClientOptions* GetClientOptions();             // 0x005ED700
};

class CAppManager {
public:
    void* m_pUnused;                                // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCVisualEffectOnObject {
public:
    CSWCVisualEffectOnObject();                             // 0x006A09E0
    ~CSWCVisualEffectOnObject();                            // 0x006A1810

    int IsReady();                                  // 0x006A0480
    void SetDefaultColors(unsigned short nEffect);  // 0x006A04C0
    void HideModel(unsigned short nEffect);                    // 0x006A05B0
    void ResetBeamTimer(CAurObject* pBeam);         // 0x006A05E0
    int GetImpactDuration(unsigned short nEffect);  // 0x006A0620
    int LoadBeam(unsigned short nEffect);           // 0x006A0680
    int LoadFailImpact(unsigned short nEffect);     // 0x006A0800
    int LoadDurationModel(unsigned short nEffect);  // 0x006A0860
    void DestroyModel();                            // 0x006A08D0
    unsigned char GetType();                        // 0x006A0920
    unsigned char GetBeamNode(unsigned short nEffect);  // 0x006A0930
    void DestroyModels();                           // 0x006A0BF0
    void Detach();                                  // 0x006A0C70
    void AttachBeam();                              // 0x006A0D30
    void ApplyTexture(unsigned short nEffect);      // 0x006A1220
    void RemoveTexture();                           // 0x006A1470
    void ApplySpeedBlur(unsigned short nEffect);    // 0x006A14E0
    void RemoveSpeedBlur();                         // 0x006A1570
    void ApplyFlash(unsigned short nEffect);        // 0x006A15D0
    void RemoveFlash(unsigned short nEffect);       // 0x006A1610
    void ApplyImpactEffect(unsigned short nEffect); // 0x006A2AF0
    void ApplySpellVisual(unsigned short nEffect);  // 0x006A2E10
    void ApplyBeamEffect(unsigned short nEffect);   // 0x006A30E0
    void ApplySkinTexture(unsigned short nEffect);  // 0x006A34B0
    void ApplyEnvironmentMap(unsigned short nEffect);       // 0x006A3660
    void ApplyColorOverlay(unsigned short nEffect);         // 0x006A3740
    void ApplyDurationEffect(unsigned short nEffect);       // 0x006A38E0
    void ApplyEffect4060(unsigned short nEffect);   // 0x006A4060
    void ApplyEffect4240(unsigned short nEffect);   // 0x006A4240
    void ApplyFOV(unsigned short nEffect);          // 0x006A4580
    void ApplyHardCodedVisualEffect(unsigned short nEffect);    // 0x006A54D0
    void UpdateFOV(unsigned short nEffect, int bUnused);        // 0x006A4E70
    void UpdateSelfIlluminationPulse(unsigned long nDelta);     // 0x006A4A80
    void UpdateAlpha(unsigned long nDelta);         // 0x006A4C00
    void UpdateMotionBlur(unsigned short nUnused);  // 0x006A5920
    void ApplyColorEffect(unsigned short nEffect);  // 0x006A3E80
    void PlayCastAnimation(unsigned short nEffect);            // 0x006A3FD0
    void RefreshTextures(unsigned short nEffect);              // 0x006A4000
    void AttachHook(unsigned short nEffect);        // 0x006A4680
    void RemoveSelfIllumination(unsigned short nEffect);       // 0x006A4720
    void ResetSelfIllumination(unsigned short nEffect);        // 0x006A47B0
    void SetObjectDrawMode(unsigned short nEffect);            // 0x006A47F0
    void RemoveDurationModel(unsigned short nEffect);   // 0x006A4830
    void RemoveColorEffect(unsigned short nEffect); // 0x006A4920
    int LoadLight(unsigned short nEffect);                     // 0x006A4FA0
    void RemoveFOV(unsigned short nEffect, int bForce);  // 0x006A49A0
    int LoadHardCodedVisualEffect(unsigned short nEffect);  // 0x006A5890
    void RemoveHardCodedVisualEffect(unsigned short nEffect);   // 0x006A56F0
    void DetachAll();                               // 0x006A5F00
    void Reset();                                   // 0x006A6950
    unsigned short GetHardCodedEffect()
    {
        if (m_bC6 == 1) {
            return m_nA0;
        }
        if (m_bC6 == 2) {
            return m_nA2;
        }
        if (m_bC6 == 3) {
            return m_nA4;
        }
        return 0xffff;
    }
    void Update(unsigned short nEffect, unsigned long nDelta);  // 0x006A69E0
    void AttachModel(CAurObject* pModel, CAurObject* pTo, CExoString* pNode);  // 0x006A1650

    Vector m_vColor1;                               // 0x00
    Vector m_vColor2;                               // 0x0c
    Vector m_vOffset;                               // 0x18
    CExoString m_sSourceNode;                       // 0x24
    CExoString m_sTargetNode;                       // 0x2c
    unsigned long m_n34;                            // 0x34
    unsigned long m_n38;                            // 0x38
    unsigned long m_n3C;                            // 0x3c
    unsigned long m_oidTargets[3];                  // 0x40
    unsigned long m_n4C[3];                         // 0x4c
    unsigned short m_n58[3];                        // 0x58
    unsigned short pad_0x5e;
    unsigned long m_n60[3];                         // 0x60
    unsigned short m_n6C[3];                        // 0x6c
    unsigned short pad_0x72;
    CAurObject* m_pBeams[3];                        // 0x74
    CAurObject* m_pImpacts[3];                      // 0x80
    CAurObject* m_pModel;                           // 0x8c
    unsigned long m_nBeamTimer[4];                  // 0x90
    unsigned short m_nA0;                           // 0xa0
    unsigned short m_nA2;                           // 0xa2
    unsigned short m_nA4;                           // 0xa4
    unsigned short pad_0xa6;
    unsigned long m_nA8;                            // 0xa8
    void* m_pAC;                                    // 0xac
    void* m_pB0;                                    // 0xb0
    void* m_pB4;                                    // 0xb4
    void* m_pB8;                                    // 0xb8
    void* m_pBC;                                    // 0xbc
    void* m_pC0;                                    // 0xc0
    unsigned short m_nEffect;                       // 0xc4
    unsigned char m_bC6;                            // 0xc6
    unsigned char m_bC7;                            // 0xc7
    unsigned long m_nC8;                            // 0xc8
    float m_fCC;                                    // 0xcc
    float m_fD0;                                    // 0xd0
    unsigned long m_nD4;                            // 0xd4
    unsigned char m_nD8;                            // 0xd8
    unsigned char m_nType;                          // 0xd9
    unsigned short pad_0xda;
    unsigned long m_nDC;                            // 0xdc
    unsigned long m_nE0;                            // 0xe0
    float m_fE4;                                    // 0xe4
    unsigned long m_nE8;                            // 0xe8
    CExoSoundSource* m_pSound;                      // 0xec
    unsigned long m_nF0;                            // 0xf0
    unsigned long m_nF4;                            // 0xf4
};

#endif
