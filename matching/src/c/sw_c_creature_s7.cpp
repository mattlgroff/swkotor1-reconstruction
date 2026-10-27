// CSWCCreature members, worker C s7 (translation-unit-local layout).

#include "c/exo_base.h"

#include <stdlib.h>

struct Vector {
    Vector() {}
    Vector(float a, float b, float c) : x(a), y(b), z(c) {}
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    float MagnitudeSquared() const { return x * x + y * y + z * z; }
    float x, y, z;
};

class CResRef {
public:
    char* GetResRefStr() const;                     // 0x00405FE0
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const char* s);               // 0x00406290
    CResRef operator=(const CResRef& r);            // 0x00406120
    char m_resRef[16];
};

class CSWSpell {
public:
    char pad_0x0[0x46];
    CResRef m_resConjHeadVisual;                    // 0x46
    CResRef m_resConjHandVisual;                    // 0x56
    CResRef m_resConjGrndVisual;                    // 0x66
    CResRef m_resConjCastModel;                     // 0x76
    CResRef m_resConjCastModelAlt;                  // 0x86 (appearance flag 1)
    CResRef m_resConjCastSound;                     // 0x96 (descriptive)
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

struct CSWRace {
    char pad_0x0[0x20];
    int m_bConjureCastModel;                        // 0x20 (descriptive)
    char pad_0x24[0x34 - 0x24];
};

class CSWCCreature;

class CSWCSpellCastVisual {
public:
    void UnloadConjureVisual();                     // 0x00695D10
    void UnloadCastVisual();                        // 0x00695D90
    void Update(unsigned long nDelta);              // 0x00695E10 (descriptive)
    void LoadConjureVisual(CResRef resHead, CResRef resHand, CResRef resGround, CResRef resCast,
                           CResRef resSound, CSWCCreature* pCreature);   // 0x00695F30 (descriptive)

    char pad_0x0[0x18];
    int m_nConjureTime;                             // 0x18 (descriptive)
    int m_nCastTime;                                // 0x1c (descriptive)
    int m_bActive;                                  // 0x20 (descriptive)

    void SetTimes(int nTime) { m_nConjureTime = nTime; m_nCastTime = nTime; }
};

class CExoSound {
public:
    int PlayOneShotSoundAt(const CResRef& cResRef, Vector vPosition, float fVolume, unsigned char nPriority,
                           unsigned long a, unsigned long b, unsigned long c, float fPitch);   // 0x005D5E10
    void GetListenerPosition(Vector* pPosition);    // 0x005D63C0
    void GetPriorityGroupInfo(unsigned long nGroup, float* pMinDistance, float* pMaxDistance);   // 0x005D64B0
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413270
    int GetINTEntry(int nRow, int nColumn, int* pValue);                              // 0x00413510
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue);                // 0x00413190
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);                          // 0x00413350
};

class CTwoDimArrays {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearance;                            // 0x0c
    char pad_0x10[0x14 - 0x10];
    C2DA* m_pSurfaceMaterial;                       // 0x14
    char pad_0x18[0x58 - 0x18];
    C2DA* m_pFootstepSounds;                        // 0x58
    C2DA* m_pAppearanceSoundSet;                               // 0x5c

    C2DA* GetAppearanceTable() { return m_pAppearance; }
    C2DA* GetFootstepSounds() { return m_pFootstepSounds; }
    C2DA* GetSurfaceMaterial() { return m_pSurfaceMaterial; }
    C2DA* GetAppearanceSoundSet() { return m_pAppearanceSoundSet; }
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray* m_pSpellArray;                   // 0x8c

    CSWSpellArray* GetSpellArray() { return m_pSpellArray; }
    char pad_0x90[0xb0 - 0x90];
    CSWRace* m_pRaces;                              // 0xb0
    char pad_0xb4[0xb8 - 0xb4];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8

    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nAppearanceCol_SoundAppType;           // 0x007A2128
extern int g_nAppearanceCol_FootstepType;           // 0x007A20EC
extern int g_nAppearanceCol_SIZECATEGORY;           // 0x007A2124
extern int g_nFootstepRollingColumn;                // 0x007A219C
extern int g_nFootstepPitchOffsetColumn;            // 0x007A21A0
extern int g_nSurfaceMaterialCol_Name;              // 0x007A21B0
extern int g_nAppearanceCol_HEADTRACK;              // 0x007A2100
extern int g_nAppearanceCol_HEADBONE;               // 0x007A20FC
extern CExoString g_sLookAtBoneOverride;            // 0x008338D0 (descriptive)

float VectorMagnitude(const Vector& v);             // 0x004AB100
Vector VectorSubtract(const Vector& a, const Vector& b);   // 0x00423BC0 (cdecl)

class CSWCArea {
public:
    int GetSurfaceMaterial(Vector vPosition);       // 0x00604BF0
};

class CAurObject;
class CSWCCreature;
struct CCAppearanceInfo;

Vector __cdecl NormalizeVector(const Vector& v);    // 0x004AB180

class CSWCCreatureAppearance {
public:
    int CreateBodyAppearance(CCAppearanceInfo* pInfo);  // 0x00698B10
    void CreateHeadAppearance(CCAppearanceInfo* pInfo); // 0x00697A20
    void UpdateAnimTypeA(CCAppearanceInfo* pInfo);      // 0x00697B00 (descriptive)
    void UpdateAnimTypeB(CCAppearanceInfo* pInfo);      // 0x00697BC0 (descriptive)
    void SetAlignment(short n);                     // 0x00698150
    void ForceAnimate();                            // 0x006978A0

    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
    char pad_0x1a;
    unsigned char m_nCastModelType;                 // 0x1b (descriptive)
    char pad_0x1c[0x60 - 0x1c];
    float m_fWalkRate;                              // 0x60 (descriptive)
    float m_fRunRate;                               // 0x64 (descriptive)
};

class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual char* GetName();                        // 0x0c
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
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114();
    virtual void LookAt(CGob* pTarget, char* sHook, char* sBone, float fHorizontal, float fVertical,
                        float fSpeed, float fRange);   // 0x118 (descriptive)
    virtual void StopLookAt();                      // 0x11c (descriptive)
};

class CSWCObject;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
};

const unsigned long OBJECT_INVALID = 0x7f000000;

class CLIP {
public:
    CLIP();                                         // 0x0070C4D0
    virtual ~CLIP();
    int LoadLip(CExoString sName);                  // 0x0070C590
    int GetNumKeys();                               // 0x005A4C50 (shared accessor)
    float GetLength();                              // 0x0070C280 (descriptive)
    float GetKeyTime(int nKey);                     // 0x0070C2C0 (descriptive)
    unsigned long GetKeyShape(int nKey);            // 0x0070C290 (descriptive)
    void Unload();                                  // 0x005DE450 (shared accessor)

    char pad_0x4[0x30 - 0x4];
};

class CSWCCombatAttackData {
public:
    void CopyFrom(const CSWCCombatAttackData& data);    // 0x0060B6B0
};

struct Quaternion {
    float w, x, y, z;
};

class CAurModel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void PlayOverlayAnimation(unsigned short nAnim, int bLoop);   // 0x44 (descriptive)
    virtual void v48(); virtual void v4C(); virtual void v50();
    virtual void PlayAnimation(unsigned short nAnim, float fSpeed, int nFlags, int b);   // 0x54 (descriptive)
    virtual void v58();
    virtual int SetLipSyncData(float* pTimes, float* pShapes, int nKeys, float fLength);   // 0x5c (descriptive)
    virtual void SetOrientation(Vector* pOrientation);      // 0x64 (descriptive)
    virtual void SetOrientation(Quaternion* pOrientation);  // 0x60 (descriptive)
    virtual void SetPosition(Vector* pPosition);    // 0x68 (descriptive)
    virtual void SetLocalPosition(int b);           // 0x6c (descriptive)
    virtual void SetLocalOrientation(int b);        // 0x70 (descriptive)
};

class CSWSCreature {
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
    virtual void v90();
    virtual int GetIsPossessedOrBusy();             // 0x94 (descriptive)

    int IsDead();                                   // 0x004EF890
    int IsDebilitated();                            // 0x005B4880

    char pad_0x4[0xa8c - 0x4];
    int m_bA8C;                                     // 0xa8c (descriptive)
};

class CSWSObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
};

struct CSWPartyMember {
    char pad_0x0[0x48];
    float m_fFollowSpeed;                           // 0x48 (descriptive)
    Vector m_vFollowDirection;                      // 0x4c (descriptive)
    char pad_0x58[0x88 - 0x58];
};

class CSWParty {
public:
    CSWCCreature* GetPartyMember(int n);            // 0x006346C0
    int IsPartyMember(unsigned long oid);           // 0x006349C0

    char pad_0x0[0x24];
    CSWPartyMember m_aMembers[3];                   // 0x24 (descriptive)
};

class CGuiInGame {
public:
    char pad_0x0[0xb4];
    int m_bB4;                                      // 0xb4 (descriptive)
};

extern const float g_fRunRatio;                        // 0x007A22F4 (descriptive)

struct CClientOptionsX {
    char pad_0x0[0x6d];
    unsigned char m_nCameraMode;                    // 0x6d
};

class CClientExoApp {
public:
    CClientOptionsX* GetClientOptions();            // 0x005ED700
    CSWParty* GetSWParty();                         // 0x005ED8B0
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    void LookUpAndPerformRumbleWithCutOff(int nRumble, int b, Vector* pPosition);   // 0x005EDF60
};

class CClientExoAppX;

class CServerExoApp {
public:
    int IsInjured(unsigned long oid);               // 0x004AEE40
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644

class CSWCObject {
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
    virtual void v90(); virtual void v94();
    virtual CGob* GetPart(unsigned char nPart, int bLoad);  // 0x98 (descriptive)

    char pad_0x4[0x8 - 0x4];
    unsigned char m_nObjectType;                    // 0x08
    char pad_0x9[0x24 - 0x9];
    Vector m_vPosition;                             // 0x24
};

class CSWCCreatureStats {
public:
    char pad_0x0[0x24];
    unsigned short m_nRace;                         // 0x24
    char pad_0x26[0x80 - 0x26];
    unsigned short m_nGoodEvil;                     // 0x80
};

class CSWCCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84();
    virtual void SetOrientation(const Vector& v);   // 0x88
    virtual void SetPosition(const Vector& v);      // 0x8c
    virtual void v90(); virtual void v94();
    virtual CGob* GetPart(unsigned char nPart, int bLoad);  // 0x98 (descriptive)
    virtual void SetAnimation(unsigned short nAnim, int a, int b);  // 0x9c
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void UnloadModelParts();                // 0xe0 (descriptive)
    virtual void ReloadModelParts();                // 0xe4 (descriptive)

    int SetAppearance(CCAppearanceInfo* pInfo, unsigned char nFlags, int bReload);
    void AnimationUpdate();

    CSWSObject* GetServerObject();                  // 0x0063D4B0 (CSWCObjectServerLink)
    // __forceinline: with a plain inline MSVC 7.0 stops expanding this at the
    // third use in FollowAnimationUpdate (the original expands all three).
    __forceinline CSWSCreature* GetServerCreature()
    {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }
    int AnimationPause(unsigned short nAnim);       // 0x0063C1E0 (CSWCObject)
    void SetDesiredOrientationForLock();            // 0x0060BA10
    void TurnToDirection(Vector* pDirection);       // 0x00611850
    void MoveCreatureToPosition(Vector* pPosition);
    void AnimateFireAndForget();
    void FollowAnimationUpdate();
    int LipSync(const CResRef& cResRef);
    int IsInjured();                                // 0x00614B60
    int GetPauseReadyAnimation();                   // 0x0060F020
    int IsAnimationPlaying(unsigned short nAnim);   // 0x0063C4A0
    float GetAnimationLength(unsigned long nAnim);  // 0x0063C0E0 (CSWCObject)
    int AnimationFireAndForget(unsigned short nAnim);   // 0x0063C380 (CSWCObject)
    int AnimationOverlay(unsigned short nAnim);     // 0x0063C410 (CSWCObject)
    int AnimationPlayOutOfPlace(unsigned short nAnim);  // 0x0063C470 (CSWCObject)
    int AnimationAttack(unsigned short nAnim);      // 0x0063C550 (CSWCObject)
    int AnimationHideEquippedItems(unsigned short nAnim);   // 0x0063C5B0 (CSWCObject)
    int GetTopFireForgetAnimation(unsigned short* pAnim, float* pSpeed, unsigned long* pDuration,
                                  CSWCCombatAttackData** ppData);   // 0x0063C610 (CSWCObject)
    void RemoveTopFireForgetAnimation();            // 0x0063C9C0 (CSWCObject)
    void OnAnimationChanged(unsigned long nOld, unsigned short nNew);   // 0x0063D850 (CSWCObject)
    void HideEquippedItems(const CResRef& resA, const CResRef& resB);   // 0x00614630
    void PlayHitSound(CSWCCombatAttackData* pData, int b);  // 0x00617470
    void PlayFootstep();
    void HandleConjuring();
    int LookAt(unsigned long oidTarget, float fRange);
    float GetHeadTurnHorizontal();                  // 0x0060E250
    float GetHeadTurnVertical();                    // 0x0060E2A0
    void EmitFootstepDust(int bDirt, int bSplash);  // 0x00619E10 (descriptive)
    void DoFootstepAudio(unsigned char bSkip);      // 0x0060DED0

    // callees
    unsigned short GetLoopingAnimation();           // 0x0063C100 (CSWCObject)
    unsigned short GetCurrentAnimation();           // 0x0063C060 (CSWCObject)
    void SetLoopingAnimation(unsigned short nAnim); // 0x0063D830 (CSWCObject)
    void AbortFireForgetAnimation(int b);           // 0x0063CAD0 (CSWCObject)
    int IsAnimationLooping(unsigned short nAnim);   // 0x0063C300 (CSWCObject)
    int AnimationStationary(unsigned short nAnim);  // 0x0063C170 (CSWCObject)
    void UpdateShadowBlob();                        // 0x0060E5B0
    int LoadRollingFootstepSound();                 // 0x006105B0

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x20 - 0x8];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
    char pad_0x3c[0x44 - 0x3c];
    unsigned long m_nDeltaTime;                     // 0x44
    unsigned short m_nAnimation;                    // 0x48
    char pad_0x4a[0x4c - 0x4a];
    float m_fAnimationSpeed;                        // 0x4c (descriptive)
    int m_bOutOfPlace;                              // 0x50 (descriptive)
    int m_n54;                                      // 0x54
    char pad_0x58[0x64 - 0x58];
    int m_nFireForgetTime;                          // 0x64 (descriptive)
    CAurModel* m_pModel;                            // 0x68
    char pad_0x6c[0x138 - 0x6c];
    int m_n138;                                     // 0x138
    char pad_0x13c[0x140 - 0x13c];
    int m_n140;                                     // 0x140
    char pad_0x144[0x194 - 0x144];
    int m_bFootstepRowZero;                         // 0x194 (descriptive)
    char pad_0x198[0x1c4 - 0x198];
    Vector m_vDesiredDirection;                     // 0x1c4
    Vector m_vDesiredOrientation;                   // 0x1d0
    Vector m_vDesiredPosition;                      // 0x1dc
    unsigned long m_nMoveTimeLeft;                  // 0x1e8 (descriptive)
    char pad_0x1ec[0x210 - 0x1ec];
    float m_fMovementRate;                          // 0x210 (descriptive)
    char pad_0x214[0x21c - 0x214];
    CSWCCreatureAppearance* m_pAppearanceInfo;      // 0x21c
    char pad_0x220[0x2b8 - 0x220];
    CSWCCombatAttackData* m_pAttackData;            // 0x2b8 (descriptive)
    CSWCCombatAttackData* m_pDefendData;            // 0x2bc (descriptive)
    int m_bConjuring;                               // 0x2c0
    int m_nConjureSpell;                            // 0x2c4
    char pad_0x2c8[0x2d2 - 0x2c8];
    unsigned char m_nCombatMode;                    // 0x2d2 (descriptive)
    char pad_0x2d3;
    unsigned char m_nWalkFlags;                     // 0x2d4 (descriptive)
    char pad_0x2d5[0x2f0 - 0x2d5];
    CSWCSpellCastVisual* m_pCastVisual;             // 0x2f0
    unsigned long m_oidLockTarget;                  // 0x2f4 (descriptive)
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    char pad_0x2fc[0x300 - 0x2fc];
    int m_bInWater;                                 // 0x300 (descriptive)
    char pad_0x304[0x3a4 - 0x304];
    int m_bCanLimp;                                 // 0x3a4 (descriptive)
    char pad_0x3a8[0x3bc - 0x3a8];
    unsigned long m_nFollowAnimation;               // 0x3bc (descriptive)
    char pad_0x3c0[0x3e4 - 0x3c0];
    unsigned long m_oidLookAtTarget;                // 0x3e4

    unsigned short GetAppearanceType() { return m_pAppearanceInfo->m_nAppearanceType; }
};

// Model hit-ground event callback: plays the fall sound for the surface
// the creature lands on.
// FUNCTION: SWKOTOR 0x0060B400
void HitGroundEvent(CAurObject* pObject, char* sEvent, void* pUserData)
{
    CExoString sSound;
    CSWCCreature* pCreature = (CSWCCreature*)pUserData;
    if (!pCreature) {
        return;
    }
    int nRow;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(pCreature->GetAppearanceType(), g_nAppearanceCol_SoundAppType, &nRow);
    int nSurface = 0;
    if (pCreature->m_pArea) {
        nSurface = pCreature->m_pArea->GetSurfaceMaterial(pCreature->m_vPosition);
    }
    switch (nSurface) {
    case 1:
    case 3:
    case 14:
        g_pRules->Get2DArrays()->GetAppearanceSoundSet()->GetCExoStringEntry(nRow, "FallDirt", &sSound);
        break;
    case 10:
        g_pRules->Get2DArrays()->GetAppearanceSoundSet()->GetCExoStringEntry(nRow, "FallMetal", &sSound);
        break;
    case 4:
    case 5:
    case 9:
    case 15:
        g_pRules->Get2DArrays()->GetAppearanceSoundSet()->GetCExoStringEntry(nRow, "FallHard", &sSound);
        break;
    case 6:
    case 11:
    case 12:
    case 13:
        g_pRules->Get2DArrays()->GetAppearanceSoundSet()->GetCExoStringEntry(nRow, "FallWater", &sSound);
        break;
    }
    g_pExoSound->PlayOneShotSoundAt(sSound.CStr(), pCreature->m_vPosition, 0.0f, 0, 0, 0, 0, 0.0f);
}

// Returns 0 when the body model cannot be built (the caller-declared symbol
// says void; the original sets eax on both exits).
// NONMATCH (19 diff insns): the original does not jump-thread the
// `!(body || head)` path past the `if (bBody)` test (it reloads the unset
// current-animation slot there); every other instruction is identical.
// FUNCTION: SWKOTOR 0x006134C0
int CSWCCreature::SetAppearance(CCAppearanceInfo* pInfo, unsigned char nFlags, int bReload)
{
    UnloadModelParts();
    unsigned short nLooping;
    unsigned short nCurrent;
    unsigned char bBody = nFlags & 1;
    if (bBody || (nFlags & 2)) {
        nLooping = GetLoopingAnimation();
        nCurrent = GetCurrentAnimation();
    }
    if (bBody) {
        Vector vPosition = m_vPosition;
        Vector vDesiredPosition = m_vDesiredPosition;
        Vector vOrientation = m_vOrientation;
        Vector vDesiredDirection = m_vDesiredDirection;
        Vector vDesiredOrientation = m_vDesiredOrientation;
        if (!m_pAppearanceInfo->CreateBodyAppearance(pInfo)) {
            return 0;
        }
        SetPosition(vPosition);
        m_vDesiredPosition = vDesiredPosition;
        SetOrientation(vOrientation);
        m_vDesiredDirection = NormalizeVector(vDesiredDirection);
        m_vDesiredOrientation = NormalizeVector(vDesiredOrientation);
    }
    if (nFlags & 2) {
        m_pAppearanceInfo->CreateHeadAppearance(pInfo);
    }
    if (nFlags & 4) {
        m_pAppearanceInfo->UpdateAnimTypeA(pInfo);
    }
    if (nFlags & 8) {
        m_pAppearanceInfo->UpdateAnimTypeB(pInfo);
    }
    if (bBody || (nFlags & 2)) {
        SetLoopingAnimation(nLooping);
        AbortFireForgetAnimation(0);
        if (IsAnimationLooping(nCurrent) != 1 && AnimationStationary(nCurrent)) {
            SetAnimation(nLooping, 0, 0);
        } else {
            SetAnimation(nCurrent, 0, 0);
        }
    }
    UpdateShadowBlob();
    if (bReload) {
        ReloadModelParts();
    }
    LoadRollingFootstepSound();
    m_pAppearanceInfo->SetAlignment(m_pStats->m_nGoodEvil);
    m_pAppearanceInfo->ForceAnimate();
    return 1;
}

// FUNCTION: SWKOTOR 0x0061A980
void CSWCCreature::AnimationUpdate()
{
    if (!AnimationStationary(m_nAnimation) && GetServerObject() && GetServerObject()->AsSWSCreature()) {
        if (!GetServerCreature()->m_bA8C) {
            return;
        }
    }
    if (m_oidLockTarget != OBJECT_INVALID_ID) {
        SetDesiredOrientationForLock();
    }
    if (AnimationStationary(m_nAnimation)) {
        TurnToDirection(&m_vDesiredOrientation);
    } else {
        TurnToDirection(&m_vDesiredDirection);
    }
    MoveCreatureToPosition(&m_vDesiredPosition);
    if (!AnimationStationary(m_nAnimation)) {
        int nLooping = GetLoopingAnimation();
        SetAnimation(nLooping, 0, 0);
    }
    AnimateFireAndForget();
    if (m_nAnimation == 10001 && m_nCombatMode != 2 && m_nCombatMode != 3 && m_nCombatMode != 14 && m_n138 != 1) {
        SetAnimation(10000, 0, 0);
    }
    if (AnimationPause(m_nAnimation)) {
        if (m_nCombatMode == 2 || m_nCombatMode == 3 || m_nCombatMode == 14) {
            SetAnimation(10001, 0, 0);
        }
    }
    if (!AnimationPause(m_nAnimation) || m_nAnimation == 10150) {
        return;
    }
    unsigned long oid = m_idSelf;
    if (!g_pAppManager->GetServerExoApp()->IsInjured(oid)) {
        return;
    }
    if (g_pAppManager->GetClientExoApp()->GetClientOptions()->m_nCameraMode == 5 &&
        this == g_pAppManager->GetClientExoApp()->GetSWParty()->GetPartyMember(0)) {
        return;
    }
    SetAnimation(10092, 0, 0);
}

// FUNCTION: SWKOTOR 0x0061A2D0
void CSWCCreature::PlayFootstep()
{
    CExoString sColumn;
    CExoString sSound;
    float fPitch = 0.0f;
    CSWCArea* pArea = m_pArea;
    if (!pArea) {
        return;
    }
    int nRow;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(GetAppearanceType(), g_nAppearanceCol_FootstepType, &nRow);
    if (g_pRules->Get2DArrays()->GetFootstepSounds()->GetCExoStringEntry(nRow, g_nFootstepRollingColumn, &sSound)) {
        return;
    }
    if (m_bFootstepRowZero == 1) {
        nRow = 0;
    }
    int nSurface = pArea->GetSurfaceMaterial(m_vPosition);
    g_pRules->Get2DArrays()->GetSurfaceMaterial()->GetCExoStringEntry(nSurface, g_nSurfaceMaterialCol_Name, &sColumn);
    if (sColumn != "") {
        sColumn = sColumn + CExoString(rand() % 3);
    }
    g_pRules->Get2DArrays()->GetFootstepSounds()->GetCExoStringEntry(nRow, sColumn, &sSound);
    g_pRules->Get2DArrays()->GetFootstepSounds()->GetFLOATEntry(nRow, g_nFootstepPitchOffsetColumn, &fPitch);
    unsigned char nGroup;
    unsigned long oid = m_idSelf;
    if (oid == g_pAppManager->GetClientExoApp()->GetPlayerCreatureId()) {
        nGroup = 19;
    } else {
        int nSize;
        g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(GetAppearanceType(), g_nAppearanceCol_SIZECATEGORY, &nSize);
        if (nSize == 1 || nSize == 2 || nSize == 3) {
            nGroup = 20;
        } else {
            nGroup = 18;
        }
    }
    float fMinDistance;
    float fMaxDistance;
    Vector vListener(0.0f, 0.0f, 0.0f);
    g_pExoSound->GetPriorityGroupInfo(nGroup, &fMinDistance, &fMaxDistance);
    g_pExoSound->GetListenerPosition(&vListener);
    float fDistance = VectorMagnitude(m_vPosition - vListener);
    if (fDistance > fMaxDistance + 2.0f) {
        return;
    }
    g_pExoSound->PlayOneShotSoundAt(sSound.CStr(), m_vPosition, 0.0f, nGroup, 0, 0, 0, fPitch);
    if (nRow != 6 && nRow != 7) {
        if (nSurface == 1) {
            EmitFootstepDust(nSurface, 0);
            m_bInWater = 0;
        } else if (nSurface == 6 || nSurface == 11 || nSurface == 12 || nSurface == 13) {
            EmitFootstepDust(0, m_bInWater == 0);
            m_bInWater = 1;
        } else if (nSurface == 5) {
            DoFootstepAudio(0);
        } else {
            m_bInWater = 0;
        }
    }
    g_pAppManager->GetClientExoApp()->LookUpAndPerformRumbleWithCutOff(nRow, 0, &m_vPosition);
}

// NONMATCH (51 diff insns): register assignment only; the original keeps
// `this` in esi and the body part / target in edi, ours swaps them
// (nesting the guards or splitting the target declaration does not help).
// FUNCTION: SWKOTOR 0x006146E0
int CSWCCreature::LookAt(unsigned long oidTarget, float fRange)
{
    CExoString sHook;
    CExoString sBone;
    if (m_n140 || m_oidLookAtTarget == oidTarget || m_n54 || !m_pAppearanceInfo) {
        return 0;
    }
    CGob* pHead = GetPart(0xfe, 0);
    CGob* pBody = GetPart(0xff, 0);
    if (!pHead && !pBody) {
        return 0;
    }
    int bHeadTrack;
    if (!g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(GetAppearanceType(), g_nAppearanceCol_HEADTRACK, &bHeadTrack)) {
        bHeadTrack = 0;
    }
    if (bHeadTrack && oidTarget != OBJECT_INVALID) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidTarget);
        if (!pGameObject) {
            return 0;
        }
        CSWCObject* pTarget = pGameObject->AsSWCObject();
        if (!pTarget || !pTarget->GetPart(0xff, 0)) {
            return 0;
        }
        Vector vDiff = VectorSubtract(pTarget->m_vPosition, m_vPosition);
        if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z > fRange * fRange) {
            return 0;
        }
        if (g_sLookAtBoneOverride != "") {
            sBone = g_sLookAtBoneOverride;
        } else if (!g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(GetAppearanceType(), g_nAppearanceCol_HEADBONE, &sBone)) {
            sBone = "hturn_g";
        }
        float fHorizontal = GetHeadTurnHorizontal();
        float fVertical = GetHeadTurnVertical();
        if (pTarget->m_nObjectType != 5) {
            sHook = pTarget->GetPart(0xff, 1)->GetName();
        } else {
            sHook = "CAMERAHOOK";
        }
        m_oidLookAtTarget = oidTarget;
        if (pHead) {
            pHead->LookAt(pTarget->GetPart(0xff, 1), sHook.CStr(), sBone.CStr(), fHorizontal, fVertical, 0.25f, fRange);
        }
        if (pBody) {
            pBody->LookAt(pTarget->GetPart(0xff, 1), sHook.CStr(), sBone.CStr(), fHorizontal, fVertical, 0.25f, fRange);
        }
        return 1;
    }
    m_oidLookAtTarget = OBJECT_INVALID;
    if (pHead) {
        pHead->StopLookAt();
    }
    if (pBody) {
        pBody->StopLookAt();
    }
    return 0;
}

// Descriptive name: starts, keeps and ends the spell conjure/cast visuals for
// the current animation.
// FUNCTION: SWKOTOR 0x0060BB20
void CSWCCreature::HandleConjuring()
{
    int bConjuring = 0;
    CSWSpell* pSpell;
    unsigned short nAnimation = m_nAnimation;
    if (nAnimation == 10015 || nAnimation == 10016 || nAnimation == 11000 || nAnimation == 10017 ||
        nAnimation == 10018 || nAnimation == 10019 || nAnimation == 10020 || nAnimation == 10061 ||
        nAnimation == 10250 || nAnimation == 10251 || nAnimation == 10252 || nAnimation == 10253 ||
        nAnimation == 10261 || nAnimation == 10401) {
        int nSpell = m_nConjureSpell;
        bConjuring = 1;
        pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
    }
    nAnimation = m_nAnimation;
    if (nAnimation == 10017 || nAnimation == 10018 || nAnimation == 10019 || nAnimation == 10020 ||
        nAnimation == 10061 || nAnimation == 10250 || nAnimation == 10251 || nAnimation == 10252 ||
        nAnimation == 10253 || nAnimation == 10261 || nAnimation == 10401) {
        if (!m_bConjuring) {
            m_pCastVisual->SetTimes(5000);
            m_pCastVisual->m_bActive = 0;
            m_bConjuring = 1;
        }
    }
    if (bConjuring == 1) {
        if (!m_bConjuring && pSpell) {
            CResRef resCast;
            resCast = "";
            if (g_pRules->m_pRaces[m_pStats->m_nRace].m_bConjureCastModel) {
                if (m_pAppearanceInfo->m_nCastModelType == 1) {
                    resCast = pSpell->m_resConjCastModelAlt;
                } else {
                    resCast = pSpell->m_resConjCastModel;
                }
            }
            CResRef resSound;
            resSound = pSpell->m_resConjCastSound;
            m_pCastVisual->UnloadConjureVisual();
            m_pCastVisual->UnloadCastVisual();
            m_pCastVisual->LoadConjureVisual(pSpell->m_resConjHeadVisual, pSpell->m_resConjHandVisual,
                                             pSpell->m_resConjGrndVisual, resCast, resSound, this);
            m_pCastVisual->SetTimes(5000);
            m_pCastVisual->m_bActive = 0;
        }
    } else if (!bConjuring && m_bConjuring == 1) {
        if (!m_pCastVisual->m_bActive) {
            m_pCastVisual->UnloadConjureVisual();
            m_pCastVisual->UnloadCastVisual();
        }
        m_nConjureSpell = -1;
    }
    m_pCastVisual->Update(m_nDeltaTime);
    m_bConjuring = bConjuring;
}

// Descriptive name: steps the creature toward the desired position over the
// remaining move time, snapping to pPosition when done or too far away.
// NONMATCH (32 diff insns): the original places the shared snap block between
// the two distance tests (ours puts it last) and reuses the result vector's
// z slot for the spilled delta z (frame 0x1c vs 0x20). The interpolation
// itself (three Vector temporaries) is instruction-identical.
// FUNCTION: SWKOTOR 0x0060BDF0
void CSWCCreature::MoveCreatureToPosition(Vector* pPosition)
{
    unsigned long nDelta = m_nDeltaTime;
    if (nDelta < m_nMoveTimeLeft) {
        m_nMoveTimeLeft -= nDelta;
        float fX = m_vDesiredPosition.x - m_vPosition.x;
        float fY = m_vDesiredPosition.y - m_vPosition.y;
        float fZ = m_vDesiredPosition.z - m_vPosition.z;
        float fYY = fY * fY;
        float fXX = fX * fX;
        if (!(fZ * fZ + fXX + fYY > 0.00001f) || !(fYY + fXX < 36.0f)) {
            m_nMoveTimeLeft = 0;
            SetPosition(*pPosition);
        } else {
            float fT = (float)m_nMoveTimeLeft / (float)(m_nMoveTimeLeft + nDelta);
            Vector vDiff;
            vDiff.x = m_vPosition.x - pPosition->x;
            vDiff.y = m_vPosition.y - pPosition->y;
            vDiff.z = m_vPosition.z - pPosition->z;
            Vector vScaled;
            vScaled.x = vDiff.x * fT;
            vScaled.y = vDiff.y * fT;
            vScaled.z = vDiff.z * fT;
            Vector vPosition;
            vPosition.x = vScaled.x + pPosition->x;
            vPosition.y = vScaled.y + pPosition->y;
            vPosition.z = vScaled.z + pPosition->z;
            SetPosition(vPosition);
        }
    } else {
        m_nMoveTimeLeft = 0;
        SetPosition(*pPosition);
    }
}

// Descriptive name: advances the fire-and-forget animation queue.
// FUNCTION: SWKOTOR 0x0061A670
void CSWCCreature::AnimateFireAndForget()
{
    if (AnimationFireAndForget(m_nAnimation) == 1) {
        m_nFireForgetTime -= m_nDeltaTime;
        if (m_nFireForgetTime < 0) {
            if (AnimationOverlay(m_nAnimation)) {
                m_pModel->PlayOverlayAnimation(m_nAnimation, 0);
            }
            m_nFireForgetTime = 0;
            SetAnimation(GetLoopingAnimation(), 0, 0);
        }
    }
    if (IsAnimationLooping(m_nAnimation) != 1) {
        return;
    }
    unsigned short nAnim;
    float fSpeed;
    unsigned long nDuration;
    CSWCCombatAttackData* pData;
    if (GetTopFireForgetAnimation(&nAnim, &fSpeed, &nDuration, &pData) != 1) {
        return;
    }
    CSWCCombatAttackData* pAttackData = 0;
    CSWCCombatAttackData* pDefendData = 0;
    if (AnimationAttack(nAnim) || nAnim == 10001) {
        pAttackData = pData;
        if (pData) {
            m_pAttackData->CopyFrom(*pData);
        }
    }
    if (nAnim == 10024 || nAnim == 10026 || nAnim == 10027 || nAnim == 10025) {
        PlayFootstep();
    }
    if (nAnim == 10017 || nAnim == 10018 || nAnim == 10019 || nAnim == 10020 || nAnim == 10061 ||
        nAnim == 10401) {
        pDefendData = pData;
        if (pData) {
            m_pDefendData->CopyFrom(*pData);
        }
    }
    OnAnimationChanged(m_nAnimation, nAnim);
    m_nAnimation = nAnim;
    if (AnimationHideEquippedItems(nAnim)) {
        HideEquippedItems(CResRef(), CResRef());
    }
    int bOutOfPlace = 0;
    if (m_bOutOfPlace) {
        m_bOutOfPlace = 0;
        Vector vPosition = m_vPosition;
        Vector vOrientation = m_vOrientation;
        m_pModel->SetPosition(&vPosition);
        m_pModel->SetOrientation(&vOrientation);
        m_pModel->SetLocalPosition(0);
        m_pModel->SetLocalOrientation(0);
        AbortFireForgetAnimation(0);
    }
    if (AnimationPlayOutOfPlace(nAnim)) {
        bOutOfPlace = 1;
        m_bOutOfPlace = 1;
        Vector vPosition;
        vPosition.x = 0.0f;
        vPosition.y = 0.0f;
        vPosition.z = 0.0f;
        Quaternion qOrientation;
        qOrientation.z = 0.0f;
        qOrientation.y = 0.0f;
        qOrientation.x = 0.0f;
        qOrientation.w = 1.0f;
        m_pModel->SetPosition(&vPosition);
        m_pModel->SetOrientation(&qOrientation);
        m_pModel->SetLocalPosition(1);
        m_pModel->SetLocalOrientation(1);
        m_n54 = 1;
    }
    if (AnimationOverlay(nAnim)) {
        m_pModel->PlayOverlayAnimation(nAnim, 1);
    } else if (!bOutOfPlace) {
        m_pModel->PlayAnimation(nAnim, fSpeed, 1, 0);
    } else {
        m_pModel->PlayAnimation(nAnim, fSpeed, 3, 0);
    }
    m_nFireForgetTime = nDuration;
    if (pData) {
        PlayHitSound(pData, 1);
    }
    RemoveTopFireForgetAnimation();
    if (pAttackData) {
        delete pAttackData;
    }
    if (pDefendData) {
        delete pDefendData;
    }
}

// FUNCTION: SWKOTOR 0x00615E80
void CSWCCreature::FollowAnimationUpdate()
{
    if (GetServerObject() && GetServerObject()->AsSWSCreature()) {
        if (GetServerCreature()->IsDead()) {
            return;
        }
        if (GetServerCreature()->GetIsPossessedOrBusy()) {
            return;
        }
        if (GetServerCreature()->IsDebilitated()) {
            return;
        }
    }
    int bSetAnimation = 1;
    unsigned long oid = m_idSelf;
    int nMember = g_pAppManager->GetClientExoApp()->GetSWParty()->IsPartyMember(oid);
    if (nMember != 1 && nMember != 2) {
        return;
    }
    CSWPartyMember* pMember = &g_pAppManager->GetClientExoApp()->GetSWParty()->m_aMembers[nMember];
    float fSpeed = pMember->m_fFollowSpeed;
    float fRunThreshold = m_fMovementRate;
    fRunThreshold *= g_fRunRatio;
    float fWalk = m_pAppearanceInfo->m_fWalkRate * 1000.0f;
    float fRun = m_pAppearanceInfo->m_fRunRate * 1000.0f;
    float fRate = 0.0f;
    unsigned long nAnimation;
    if (fSpeed <= 0.0001f) {
        if (g_pAppManager->GetClientExoApp()->GetGuiInGame()->m_bB4 && IsAnimationPlaying(m_nAnimation)) {
            bSetAnimation = 0;
        } else {
            nAnimation = GetPauseReadyAnimation();
            fRate = 1.0f;
        }
    } else if (fSpeed >= fRunThreshold) {
        if (m_bCanLimp && IsInjured() &&
            (g_pAppManager->GetClientExoApp()->GetClientOptions()->m_nCameraMode != 5 ||
             this != g_pAppManager->GetClientExoApp()->GetSWParty()->GetPartyMember(0))) {
            nAnimation = 10094;
        } else {
            nAnimation = (m_nWalkFlags & 1) ? 10133 : 10004;
        }
        fRate = fSpeed / (fRun / (GetAnimationLength(nAnimation) * 0.001f));
    } else {
        if (m_bCanLimp && IsInjured() &&
            (g_pAppManager->GetClientExoApp()->GetClientOptions()->m_nCameraMode != 5 ||
             this != g_pAppManager->GetClientExoApp()->GetSWParty()->GetPartyMember(0))) {
            nAnimation = 10093;
        } else {
            nAnimation = (m_nWalkFlags & 1) ? 10133 : 10002;
        }
        fRate = fSpeed / (fWalk / (GetAnimationLength(nAnimation) * 0.001f));
    }
    m_fAnimationSpeed = fRate;
    m_nFollowAnimation = nAnimation;
    if (bSetAnimation) {
        SetAnimation(nAnimation, 0, 0);
    }
    TurnToDirection(&pMember->m_vFollowDirection);
}

// NONMATCH (13 diff insns): stack-slot assignment only. The original puts
// pTimes in the dead `new CLIP` temporary slot, pShapes at 0x14 and fLength at
// 0x10; ours rotates the three (declaration order/scope does not change it).
// FUNCTION: SWKOTOR 0x00616310
int CSWCCreature::LipSync(const CResRef& cResRef)
{
    CExoString sName(cResRef.GetResRefStr());
    CLIP* pLip = new CLIP;
    if (pLip) {
        if (pLip->LoadLip(sName)) {
            int nKeys = pLip->GetNumKeys();
            float fLength = pLip->GetLength();
            float* pTimes = new float[nKeys];
            float* pShapes = new float[nKeys];
            int nResult = 0;
            if (nKeys > 0) {
                float fScale = 1.0f / fLength;
                for (int i = 0; i < nKeys; i++) {
                    pTimes[i] = pLip->GetKeyTime(i) * fScale;
                    pShapes[i] = ((float)pLip->GetKeyShape(i) + 1.0f) * 0.0625f;
                }
            }
            if (pTimes[0] == 0.0f && !(pTimes[nKeys - 1] >= 1.00001f) && !(pTimes[nKeys - 1] <= 0.99999f)) {
                pTimes[nKeys - 1] = 1.0f;
                m_pModel->SetLipSyncData(0, 0, 0, 0.0f);
                nResult = m_pModel->SetLipSyncData(pTimes, pShapes, nKeys, fLength);
            }
            pLip->Unload();
            delete pLip;
            delete[] pTimes;
            if (pShapes) {
                delete[] pShapes;
            }
            return nResult;
        }
        delete pLip;
    }
    return 0;
}
