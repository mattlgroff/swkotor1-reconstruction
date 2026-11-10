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
    Vector& operator+=(const Vector& v);            // 0x004A9740
};

Vector NormalizeVector(const Vector& v);            // 0x004AB180
int operator==(const Vector& a, const Vector& b);   // 0x004AA980
float Magnitude(const Vector& v);                   // 0x004AB100
float GetYaw(const Vector& vDirection);             // 0x004AA0F0

struct Quaternion {
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Quaternion(float fw, float fx, float fy, float fz) { z = fz; y = fy; x = fx; w = fw; }
    Quaternion(Vector vAxis, float fAngle);         // 0x004AC960
};

Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);   // 0x004ACAC0
Vector operator*(const Quaternion& q, const Vector& v);  // 0x004A9BB0
Vector CrossProduct(const Vector& a, const Vector& b);   // 0x004AB210

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(int n);                              // 0x005E5BC0
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    void Format(const char* szFormat, ...);         // 0x005E5680
};

class CResRef {
public:
    char m_resRef[16];
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CResRef& r);
    char* GetResRef() const;                        // 0x00641DB0 (returns this)
};

// Aurora model object (NewCAurObject 0x00449CC0).
class CAurObject {
public:
    virtual ~CAurObject();                          // 0x00
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int a, int b);    // 0x18
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
    virtual int GetPosition(const char* szNode, Vector* pPosition, Quaternion* pOrientation);   // 0x98
    virtual void SetNodeTransform(const char* szNode, Vector vPosition, Quaternion qOrientation);  // 0x9c (descriptive)
    virtual int GetNodeTransform(const char* szNode, Vector* pv, Quaternion* pq);  // 0xa0
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
    CExoSoundSource(const CResRef& cResRef);        // 0x005D60E0
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5900
    void Set3D(int b3D);                            // 0x005D5910
    void Play();                                    // 0x005D5930
    void SetLooping(int bLooping);                  // 0x005D59D0

    void* m_pInternal;                              // 0x04 (operator new 8)
    void SetPosition(const Vector& vPosition, float fUnused);   // 0x005D59E0
    void Stop();                                    // 0x005D5A20
};

class CExoSound {
public:
    int Play3DOneShotSound(const CResRef& cRes, Vector vPos, float fDelay, unsigned char nGroup,
                           unsigned long nFlags, unsigned char nVolume, float fPitch,
                           float fVariance);        // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class C2DA {
public:
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);  // 0x00414110
    int GetINTEntry(int nRow, int nColumn, int* pValue);    // 0x00413510
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
    int GetINTEntry(const CExoString& sRow, int nColumn, int* pValue);      // 0x00414260
    int GetFLOATEntry(const CExoString& sRow, int nColumn, float* pValue);  // 0x00414080
    int GetCExoStringEntry(const CExoString& sRow, int nColumn, CExoString* pValue);  // 0x00413EC0
    int GetCExoStringEntry(const CExoString& sRow, const CExoString& sColumn,
                           CExoString* pValue);     // 0x00413DE0
};

extern int g_nAppearanceCol_SIZECATEGORY;           // 0x007A2124
extern int g_nVisualEffectCol_ShakeType;            // 0x007A21D4
extern int g_nVisualEffectCol_ShakeDuration;        // 0x007A21D8
extern int g_nVisualEffectCol_ShakeDelay;           // 0x007A21DC
extern int g_nVisualEffectCol_SoundImpact;          // 0x007A21E0
extern int g_nVisualEffectCol_SoundDuration;        // 0x007A21E4

class CTwoDimArrays {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
    char pad_0x10[0x88 - 0x10];
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
class CSWCDoor;
class CSWCPlaceable;
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

    char pad_0x4[0xc4 - 0x4];
    unsigned char m_nAnimType;                      // 0xc4
};

// Area scene (CSWCArea + 0x184).
class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64();
    virtual void SetPlayerLight(CAurObject* pLight);    // 0x68 (descriptive)
};

class CSWCArea {
public:
    void UpdateShadowingLights();                   // 0x00605410
    char pad_0x0[0x184];
    CAurScene* m_pScene;                            // 0x184
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48

    void ShakeCamera(int nDelay, int nDuration);    // 0x006416F0
    void BumpCamera(int nDuration);                 // 0x00641730
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
    virtual ~CGameObject();                         // 0x00
    virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c

    unsigned long m_idSelf;                         // 0x04
};

class CSWCObject : public CGameObject {
public:
    virtual void v10();
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart, int n);  // 0x78
    virtual void v7C();
    virtual void v80(); virtual void v84();
    virtual void SetOrientation(const Vector& v);   // 0x88
    virtual void SetPosition(const Vector& v);      // 0x8c
    virtual void v90();
    virtual void AttachToScene(CAurScene* pScene);  // 0x94
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
    unsigned long GetProjectileTravelTime(const Vector& vFrom, int n);  // 0x0063D010

    char pad_0x8[0x4];
    int m_bIllumination[3];                         // 0x0c
    char pad_0x18[0x8];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
};

class CSWCPlaceable : public CSWCObject {
public:
    char pad_0x3c[0x144 - 0x3c];
    CExoString m_sModelName;                        // 0x144 (descriptive)
};

class CSWCDoor : public CSWCObject {
public:
    char pad_0x3c[0x130 - 0x3c];
    CExoString m_sModelName;                        // 0x130 (descriptive)
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);   // 0x004D8230
};

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CSWSCreature {
public:
    int IsDebilitated();                            // 0x005B4880
};

// Creature block at +0x21c (appearance / movement data).
class CSWCCreatureMovement {
public:
    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
    char pad_0x1a[0x5c - 0x1a];
    float m_fMaxSpeed;                              // 0x5c
};

class CSWCCreature : public CSWCObject {
public:
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    void SetProjectileObjectId(unsigned long oid);  // 0x0060ED80
    float GetServerRate();                          // 0x006109B0
    void Reset();                                   // 0x004015A0
    CSWSCreature* GetServerCreature();              // 0x0060FB20

    char pad_0x3c[0x21c - 0x3c];
    CSWCCreatureMovement* m_pMovement;              // 0x21c
    unsigned short GetAppearanceType() { return m_pMovement->m_nAppearanceType; }
    char pad_0x220[0x190];
    float m_fSpeed;                                 // 0x3b0
};

class CSWCItemHead {
public:
    virtual void v00();
    char pad_0x4[0xc];
};

class CSWCItem : public CSWCItemHead, public CSWCObject {
public:
    char pad_0x4c[0x128 - 0x4c];
    CResRef m_cModelResRef;                         // 0x128 (descriptive)
};

// Client projectile (nwcprojectile.o, 0x1C8 bytes); only what ApplyMirv uses.
class CSWCProjectile : public CSWCObject {
public:
    CSWCProjectile();                               // 0x006D57B0
    void SetDuration(unsigned long nDuration);      // 0x006D2C30
    void SetSpellId(int nSpell);                    // 0x006D41B0
    int PlayAnimation(unsigned short nAnim);        // 0x006D4250
    void SetTarget(unsigned long oidTarget, const CExoString& sNode);  // 0x006D42D0
    void SetMode(int nMode);                        // 0x006D6EE0
    void SetTargetPosition(const Vector& vPosition);    // 0x0064B4D0

    char pad_0x3c[0x114 - 0x3c];
    Vector m_vTargetPosition;                       // 0x114
    char pad_0x120[0x158 - 0x120];
    unsigned char m_n158;                           // 0x158
    char pad_0x159[0x1a1 - 0x159];
    unsigned char m_n1A1;                           // 0x1a1
    char pad_0x1a2[0x1c4 - 0x1a2];
    unsigned long m_oidSource;                      // 0x1c4
};

class CSWCMiniGame;

class CClientExoApp {
public:
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    void LookUpAndPerformRumbleWithCutOff(int nEffect, int b, Vector* pPosition);  // 0x005EDF60
    CSWCItem* GetItemByGameObjectID(unsigned long oid);     // 0x005ED5A0
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x005ED5B0
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
    void AttachLight();                             // 0x006A0DF0 (descriptive)
    void ApplyTexture(unsigned short nEffect);      // 0x006A1220
    void RemoveTexture();                           // 0x006A1470
    void ApplySpeedBlur(unsigned short nEffect);    // 0x006A14E0
    void RemoveSpeedBlur();                         // 0x006A1570
    void ApplyFlash(unsigned short nEffect);        // 0x006A15D0
    void RemoveFlash(unsigned short nEffect);       // 0x006A1610
    void ApplyCheat(unsigned short nEffect);        // 0x006A2AF0
    void ApplySpellVisual(unsigned short nEffect);  // 0x006A2E10
    void ApplyLightsaberThrow(unsigned short nEffect);  // 0x006A30E0 (descriptive)
    void ApplySkinTexture(unsigned short nEffect);  // 0x006A34B0
    void ApplyEnvironmentMap(unsigned short nEffect);       // 0x006A3660
    void ApplyColorOverlay(unsigned short nEffect);         // 0x006A3740
    void ApplyLightSource(unsigned short nEffect);  // 0x006A38E0
    void ApplyChunk(unsigned short nEffect);        // 0x006A4060 (descriptive)
    void ApplyMirv(unsigned short nEffect);         // 0x006A4240
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
    void ReAttachVisualEffect();                    // 0x006A5000
    void StopVisualEffect(int bStop);               // 0x006A6590
    int ShowVisualEffect(int bForce);               // 0x006A5A40
    void StartVisualEffect();                       // 0x006A5FC0
    void Update(unsigned short nEffect, unsigned long nDelta);  // 0x006A69E0
    void Update(unsigned long nDelta);              // 0x006A6A50
    void AttachVFXModelToTargetModel(CAurObject* pModel, CAurObject* pTo,
                                     const CExoString& sNode);   // 0x006A1650

    Vector m_vColor1;                               // 0x00
    Vector m_vColor2;                               // 0x0c
    Vector m_vOffset;                               // 0x18
    CExoString m_sSourceNode;                       // 0x24
    CExoString m_sTargetNode;                       // 0x2c
    Vector m_vTargetPosition;                       // 0x34 (descriptive)
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
    int m_bAC;                                       // 0xac
    int m_bB0;                                       // 0xb0
    int m_bB4;                                       // 0xb4
    int m_bB8;                                       // 0xb8
    int m_bBC;                                       // 0xbc
    int m_bC0;                                       // 0xc0
    unsigned short m_nEffect;                       // 0xc4
    unsigned char m_bC6;                            // 0xc6
    unsigned char m_bC7;                            // 0xc7
    unsigned char m_bHasDuration;                   // 0xc8 (descriptive)
    unsigned char pad_0xc9[3];
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
