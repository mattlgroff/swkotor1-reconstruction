// CSWCCreature members, round 6 (worker c): sound set / position / model
// helpers, skill / give-item / attack / spell forwarders to CSWCMessage,
// feat checks, force shield lookup, equipped-item visibility, way points,
// attack data, shadow blob, head tracking, plus the small V3 FUN_ members
// between 0x0060B330 and 0x0060FB40 (names chosen from their behaviour).

#include "c/exo_base.h"

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

struct Quaternion {
    float w, x, y, z;
    Quaternion() {}
};

extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const int EFFECT_FORCE_SHIELD;               // 0x00746824
extern int g_bUpdateCreatureOnMove;                 // 0x0083395C

class CSWCCreature;
class CSWSCreature;
class CSWCItem;
class CExoSoundSource;
class CGob;

class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24();
    virtual void* AddSource(void* pfn, void* pData, int n);  // 0x28
    virtual void RemoveSource(void* pSource);       // 0x2c
};

class CRes {
public:
    void* Demand();                                 // 0x00409B10
    int Release();                                  // 0x00409B80
};

class CSoundSet {
public:
    void GetStrres(int a, int b);                   // 0x00678820

    char pad_0x0[0x8];
    CRes* m_pRes;                                   // 0x08
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCCreature* AsClientCreature();       // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual CSWCItem* AsSWCItem();                  // 0x34
    virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CSWCCreature* AsCreatureForCallback();  // 0x70
    virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void UpdatePosition();                  // 0x90
    virtual void SetVisible(int b);                 // 0x94
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
    virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8();
    virtual void UpdateShadow();                    // 0xfc
    virtual void RemoveShadow();                    // 0x100
    virtual void v104();
    virtual void RefreshShadow();                   // 0x108 (descriptive)

    void UnloadModel();                             // 0x004015A0

    unsigned long m_idSelf;                         // 0x04
};

class CSWCArea;

class CModel {
public:
    virtual void Destroy(int b);                    // 0x00 (scalar deleting dtor)
    virtual void v04();
    virtual CGob* GetPart(unsigned char nPart);     // 0x08
    virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58();
    virtual void ResetState(int a, int b, int c, int d);  // 0x5c
    virtual void v60(); virtual void v64();
    virtual void SetPosition(const Vector& v);      // 0x68
    virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual void Unload();                          // 0x78
    virtual void SetWeaponModel(unsigned long oid, const CExoString& s, int nSlot, float f);  // 0x7c
    virtual void v80();
    virtual void ShowEquipped();                    // 0x84
    virtual void StartHeadTracking();               // 0x88
    virtual void StopHeadTracking();                // 0x8c
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0();
    virtual CGob* AddShadowBlob(const CExoString& sModel);   // 0xa4 (descriptive)
    virtual void vA8(); virtual void vAC();
    virtual void vB0();
    virtual void SetShadowOrientation(const Quaternion& q);  // 0xb4

    char pad_0x4[0xc2];
    unsigned char m_nLightingFlag;                  // 0xc6
};

class CAnimationBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual int HasAnimation(unsigned short nAnim); // 0x4c

    char pad_0x4[0xc1];
    unsigned char m_nStance;                        // 0xc5
    char pad_0xc6[0x2];
    unsigned char m_bSpecialWalk;                   // 0xc8
    char pad_0xc9[0x3];
    int m_nRunQuality;                              // 0xcc
};

struct CSWCAttackData {
    char pad_0x0[0x14];
    unsigned long m_oidTarget;                      // 0x14
    char pad_0x18[0x2];
    unsigned short m_nReaction;                     // 0x1a
    char pad_0x1c[0x2];
    unsigned short m_nAttackType;                   // 0x1e
    int m_bRanged;                                  // 0x20
};

extern int g_nDebugAttackAnimation;                 // 0x008338BC

class CSWCAreaX {
public:
    void UpdatePosition(const Vector& v);           // 0x006054E0
};

class CSWCObject : public CGameObject {
public:
    char pad_0x8[0x18];
    CSWCAreaX* m_pArea;                             // 0x20
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
    int m_n3C;                                      // 0x3c
    int m_n40;                                      // 0x40
    char pad_0x44[0x4];
    unsigned short m_nAnimation;                    // 0x48
    char pad_0x4a[0x6];
    int m_bNoModelUpdate;                           // 0x50
    char pad_0x54[0x14];
    CModel* m_pModel;                               // 0x68
};

class CSWCMessage {
public:
    int SendPlayerToServerInput_UseItem(unsigned long a, int b, int c, unsigned long d, const Vector* e);  // 0x00677BD0
    void SendPlayerToServerInput_CastSpell(int a, int b, int c, int d, int e, int f, int g);  // 0x006776A0
    void SendPlayerToServerInput_GiveItem(unsigned long a, int b, int c, int d);  // 0x00677870
    void SendPlayerToServerInput_Attack(unsigned long oid, int b);   // 0x00677A90
    void SendPlayerToServerInput_UseSkill(int a, int b, unsigned long oid, const Vector& v);  // 0x00677B10
};

class CGuiInGame {
public:
    void ShowSoloModeQuery(int b);                  // 0x0062E550
};

class CSWParty {
public:
    CSWCCreature* GetPartyMember(int n);            // 0x006346C0

    int m_nSoloMode;                                // 0x00
};

class CSWPartyTable {
public:
    char pad_0x0[0x190];
    int m_bSoloMode;                                // 0x190
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

class CSWCModuleX {
public:
    char pad_0x0[0x48];
    void* m_p48;                                    // 0x48
};

struct CClientOptionsX {
    char pad_0x0[0x6c];
    unsigned char m_nShadows;                       // 0x6c
    unsigned char m_nCameraMode;                    // 0x6d
};

class CClientExoApp {
public:
    CSWCModuleX* GetModule();                       // 0x005ED530
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CGameObject* GetItemByGameObjectID(unsigned long oid);  // 0x005ED5A0
    CClientOptionsX* GetClientOptions();            // 0x005ED700
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    CGameObject* GetProjectileByGameObjectID(unsigned long oid);  // 0x005ED5E0
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
    CSWParty* GetSWParty();                         // 0x005ED8B0
    void SetAttackMashTutorialTimer(unsigned long n);  // 0x005EDF90
    unsigned long GetAttackMashTutorialTimer();     // 0x005EDFA0
    int ShowTutorialWindow(int n, unsigned long a, unsigned long b, int c);  // 0x005EDF40
    void* GetMiniGame();                            // 0x005EDB00
};

class CServerExoApp {
public:
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
    int IsInjured(unsigned long oid);               // 0x004AEE40
    unsigned long ClientToServerObjectID(unsigned long oid);  // 0x004AEA30
    int IsActionsPending(unsigned long oid);        // 0x004AEAE0
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

class CGameEffectX {
public:
    int GetForceShield(int b);                      // 0x00503690

    char pad_0x0[0x8];
    unsigned short m_nType;                         // 0x08
};

class CSWSCreatureStats {
public:
    int HasFeat(unsigned short nFeat);              // 0x005A6630
};

class CItemRepository {
public:
    CSWCItem* ItemListGetItem(unsigned long n);     // 0x005560B0

    char pad_0x0[0x10];
    unsigned long m_nItems;                         // 0x10
};

class CSWSArea {
public:
    char pad_0x0[0x2b0];
    int m_bNoItemUse;                               // 0x2b0
};

class CSWSItem;

class CSWCItem {
public:
    CSWSItem* GetServerItem();                      // 0x00646C90
    void SetEquippedByMe(int b);                    // 0x00646600
};

class CSWSCreature {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    CItemRepository* GetItemRepository(int b);      // 0x004EF770

    char pad_0x0[0x124];
    CExoArrayList<CGameEffectX*> m_appliedEffects;  // 0x124
    char pad_0x130[0x944];
    CSWSCreatureStats* m_pStats;                    // 0xa74
};

class CSWCCreatureStats {
public:
    int HasFeat(unsigned short nFeat);              // 0x00649340
    int GetAlignmentCategory();
    unsigned char GetSkillRank(unsigned short nSkill);

    char pad_0x0[0x80];
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0x6];
    unsigned char* m_pSkills;                       // 0x88
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, int nColumn, float* pResult);  // 0x00413350
    int GetINTEntry(int nRow, int nColumn, int* pResult);     // 0x00413510
};

class C2DAs {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
};

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;                     // 0xab
    char pad_0xac[0xc];
    C2DAs* m_p2DArrays;                             // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nHeadTurnRateColumn;                   // 0x007A2118
extern int g_nAppearanceCol_SIZECATEGORY;           // 0x007A2124
extern int g_nHeadBoneColumn;                       // 0x007A20D0
extern int g_nHeadTurnHColumn;                      // 0x007A20F4
extern int g_nHeadTurnVColumn;                      // 0x007A20F8
extern const float g_fDefaultHeadTurnH;             // 0x00740A0C
extern const float g_fDefaultHeadTurnV;             // 0x007455DC

Vector __cdecl NormalizeVector(const Vector& v);    // 0x004AB180

class CPathRequest {
public:
    virtual ~CPathRequest();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void Notify(int n);                     // 0x4c
};

class CCreatureAppearanceX {
public:
    void SetBody(void* p);                          // 0x00698A40
    void SetHead(void* p);                          // 0x00696F60
    void SetA(void* p);                             // 0x006054E0
    void SetB(void* p);                             // 0x006054E0
    void SetAlignment(short n);                     // 0x00698150
    float GetCameraOffset();                        // 0x00697580

    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
};

struct CCreatureAppearance {
    unsigned long m_data[15];
};

class CSWCCreature : public CSWCObject {
public:
    int GetSoundSetStrres(int a, int b);
    void SetPosition(const Vector& v);
    void UnloadModel();
    int RequestAppearance(CCreatureAppearance* pAppearance, unsigned char nFlags);
    void ResolveAttackIndex(unsigned short nAnim, int b, int nExclude);
    int InCreatureCombat(CGameObject* pTarget);
    void PerformSkill(int nSkill, int nSub, unsigned long oidTarget);
    void PerformGiveItem(int a, int b, int c);
    void PerformAttack(unsigned long oid);
    void PerformCastSpell(int a, int b, int c, int d, int e, int f, int g);
    void SetAlignmentGoodEvil(short n);
    CGob* GetModel(unsigned char nPart, int bLoad);
    int GetForceShield();
    void ShowEquippedItems();
    void PartyMenuStealth(int a, int b);
    int CheckFeat(int a, int b, int c);
    void ActionMenuAttack(int a, CSWCCreature* pTarget);
    void GetHostileItems(int a, int b);
    void PartyMenuUseItem(unsigned long oidItem, CGameObject* pTarget);
    void GetUsableItems(int a, int b);
    void UpdateShadowBlob();
    void StripEquippedItems();
    void SetNewLoopingCreatureSound(const CExoString& sSound);
    void SetWayPoints(unsigned short nCount, Vector* pPoints);
    void DoFootstepAudio(unsigned char bSkip);
    void SetAnimationState(int a, int b);
    void AddUsableItem(int a, CSWSItem* pItem, int bEquipped, int b);  // 0x006193A0
    unsigned long GetEquippedItemID(int nSlot);     // 0x0060CA80
    void SetDesiredDirection(const Vector& v);
    void SetDesiredOrientation(const Vector& v);
    void ClearPathRequest();
    void ResetDesiredPositionTimer();
    void GetWayPoints(unsigned short* pCount, Vector** ppPoints, unsigned short* pCurrent);
    void SetLightingFlag(unsigned char n);
    int SelectByMode(int nMode, int a, int b, int c);
    void StopLoopingCreatureSound();
    static void __cdecl GetHeadTurnRateCallback(CGameObject* pObject, void* pUnused, float* pResult);
    static void __cdecl GetHeadBoneCallback(CGameObject* pObject, void* pUnused, float* pResult);
    void SetSpecialWalk(unsigned char b);
    void SetRunQuality(int n);
    void StartHeadTracking();
    void StopHeadTracking();
    float GetHeadTurnHorizontal();
    float GetHeadTurnVertical();
    void DoNothing(int n);
    void ResetAmbientPause();
    float GetCameraOffset();
    int GetBlockedAnimation();
    void SetCombatMode(int nMode);
    void SetAttackPair(int a, int b);
    int GetIsBlocked();
    void SetEngaged(int b);
    int RemoveRumbleSource();
    void ResetModelState();
    CSWSCreature* AsServerCreature();
    void SetPartyTransition(int nMode);
    int IsAnimationBlocked(unsigned short nAnimation);  // 0x0063C4A0
    void SetTransitionMode(int nMode);              // 0x0063D820
    CGob* ApplyShadowBlob();

    // callees
    int InCombat();                                 // 0x0060D320
    int ComputeWeaponOverlays(int bCheck, int b);
    int UpdateAttackData(unsigned short* pAnimation, CSWCAttackData* pData);
    CAnimationBase* GetAnimationBase();             // 0x0063C070
    int ResolveRangedAnimation(int nAttackType, int nStance);   // 0x00614110
    int ResolveMeleeAnimation(unsigned short nAnimation, int nAttackType, int bCombat, int nStance);  // 0x00613DA0
    unsigned short ResolveReaction(int nAnimation, unsigned short nReaction, int nStance);  // 0x0060D140
    void SetAttackAnimationData(int nAttackType, int nStance, int bRanged);  // 0x006142F0
    void LoadModelA();                              // 0x00614A30
    void LoadModelB();                              // 0x0060E6C0
    void SetAttackTarget(int b);                    // 0x00610A10
    void AddHostileItem(int a, CSWCItem* pItem, int b);  // 0x006196A0
    CGameObject* GetServerObject();                 // 0x0063D4B0

    CSWSCreature* GetServerCreature()
    {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }

    char pad_0x6c[0x68];
    int m_bNoShadowRefresh;                         // 0xd4 (descriptive)
    Vector m_vShadowNormal;                         // 0xd8
    char pad_0xe4[0x10];
    void* m_pRumbleSource;                          // 0xf4
    char pad_0xf8[0x40];
    int m_n138;                                     // 0x138
    int m_bEngaged;                                 // 0x13c
    char pad_0x140[0x30];
    CPathRequest* m_pPathRequest;                   // 0x170
    int m_n174;                                     // 0x174
    int m_n178;                                     // 0x178
    CExoSoundSource* m_pLoopingSound;               // 0x17c
    char pad_0x180[0x3c];
    Vector* m_pWayPoints;                           // 0x1bc
    Vector* m_pCurWayPoint;                         // 0x1c0
    Vector m_vDesiredDirection;                     // 0x1c4
    Vector m_vDesiredOrientation;                   // 0x1d0
    char pad_0x1dc[0xc];
    int m_nDesiredPositionTimer;                    // 0x1e8
    unsigned short m_nWayPoints;                    // 0x1ec
    char pad_0x1ee[0x2a];
    unsigned char m_nLightingFlag;                  // 0x218
    char pad_0x219[0x3];
    CCreatureAppearanceX* m_pAppearance;            // 0x21c
    char pad_0x220[0x1c];
    unsigned long m_oidEquipped23C;                 // 0x23c
    unsigned long m_oidEquipped240;                 // 0x240
    char pad_0x244[0x4];
    unsigned long m_oidEquipped248;                 // 0x248
    char pad_0x24c[0x84];
    unsigned short m_n2D0;                          // 0x2d0
    char pad_0x2d2[0x26];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    unsigned short m_nSoundSet;                     // 0x2fc
    char pad_0x2fe[0x12];
    unsigned long m_nPartsLoaded;                   // 0x310
    char pad_0x314[0x8];
    CCreatureAppearance m_appearance;               // 0x31c
    char pad_0x358[0x14];
    CSoundSet* m_pSoundSet;                         // 0x36c
    char pad_0x370[0x20];
    int m_n390;                                     // 0x390
    char pad_0x394[0x4];
    int m_n398;                                     // 0x398
    char pad_0x39c[0x8];
    int m_bInMiniGame;                              // 0x3a4
    int m_nCombatMode;                              // 0x3a8
    char pad_0x3ac[0x4];
    int m_n3B0;                                     // 0x3b0
    int m_n3B4;                                     // 0x3b4
    char pad_0x3b8[0x8];
    int m_nAttackIndex;                             // 0x3c0
    int m_nLastAttackIndex;                         // 0x3c4
    unsigned long m_oidProjectile;                  // 0x3c8
    int m_n3CC;                                     // 0x3cc
    int m_bHeadTracking;                            // 0x3d0
    char pad_0x3d4[0x14];
    int m_nForcedAttackAnimation;                   // 0x3e8
};

int __cdecl rand();
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memcpy)

Quaternion __cdecl RotationArc(Vector a, Vector b);     // 0x004AB630

class CResRef {
public:
    CResRef(const CExoString& s);                   // 0x00406D60

    char m_resRef[16];
};

class CExoSoundSource {
public:
    virtual ~CExoSoundSource();
    void Stop();                                    // 0x005D5A20

    char pad_0x4[0x4];

    CExoSoundSource(const CResRef& r);              // 0x005D60E0
    void SetPriorityGroup(unsigned char n);         // 0x005D5900
    void SetLooping(int b);                         // 0x005D59D0
};

class CExoSoundInternal {
public:
    void PlaySoundAt(const CResRef& r, Vector v, int a, int b, int c, int d, int e, int f);  // 0x005D5E10
};

extern CExoSoundInternal* g_pExoSound;              // 0x007A39EC
extern char g_sFootstepPrefix[];                    // 0x0074F378


// FUNCTION: SWKOTOR 0x0060B8A0
int CSWCCreature::GetSoundSetStrres(int a, int b)
{
    if (m_nSoundSet == 0xffff) {
        return 0;
    }
    if (!m_pSoundSet) {
        return 0;
    }
    if (!m_pSoundSet->m_pRes->Demand()) {
        return 0;
    }
    m_pSoundSet->GetStrres(a, b);
    m_pSoundSet->m_pRes->Release();
    return 1;
}

// FUNCTION: SWKOTOR 0x0060B9B0
void CSWCCreature::SetPosition(const Vector& v)
{
    m_vPosition = v;
    if (m_pArea) {
        m_pArea->UpdatePosition(v);
    }
    if (g_bUpdateCreatureOnMove) {
        AsSWCCreature();
    }
    if (m_pModel && !m_bNoModelUpdate) {
        m_pModel->SetPosition(v);
    }
    UpdatePosition();
}

// FUNCTION: SWKOTOR 0x0060C8E0
void CSWCCreature::UnloadModel()
{
    CGameObject::UnloadModel();
    if (m_pModel) {
        m_pModel->Unload();
        if (m_pModel) {
            m_pModel->Destroy(1);
        }
        m_pModel = 0;
    }
}

// FUNCTION: SWKOTOR 0x0060C910
int CSWCCreature::RequestAppearance(CCreatureAppearance* pAppearance, unsigned char nFlags)
{
    if (nFlags & 1) {
        m_pAppearance->SetBody(pAppearance);
    }
    if (nFlags & 2) {
        m_pAppearance->SetHead(pAppearance);
    }
    if (nFlags & 4) {
        m_pAppearance->SetA(pAppearance);
    }
    if (nFlags & 8) {
        m_pAppearance->SetB(pAppearance);
    }
    m_appearance = *pAppearance;
    return 1;
}

// FUNCTION: SWKOTOR 0x0060D0E0
// NONMATCH: the original pushes EDI in the prologue and keeps the loop value in EAX; ours shrink-wraps EDI into the loop branch (11 insns).
void CSWCCreature::ResolveAttackIndex(unsigned short nAnim, int b, int nExclude)
{
    if (nAnim == 0x277d && b == 0) {
        for (b = m_nLastAttackIndex; b == m_nLastAttackIndex || b == nExclude; b = rand() % 5) {
        }
    } else {
        b = rand() % 2;
    }
    m_nAttackIndex = b;
    m_nLastAttackIndex = b;
}

// FUNCTION: SWKOTOR 0x0060D3E0
// NONMATCH: the original keeps a zero result in EDI with two exits; ours folds the constant (5 insns).
int CSWCCreature::InCreatureCombat(CGameObject* pTarget)
{
    int bResult = 0;
    if (pTarget) {
        if (InCombat() || (pTarget->AsSWCCreature() && pTarget->AsSWCCreature()->InCombat())) {
            return 1;
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x0060EE00
void CSWCCreature::PerformSkill(int nSkill, int nSub, unsigned long oidTarget)
{
    if (oidTarget != OBJECT_INVALID_ID) {
        CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
        if (pMessage) {
            Vector v(0.0f, 0.0f, 0.0f);
            pMessage->SendPlayerToServerInput_UseSkill(nSkill, nSub, oidTarget, v);
        }
    }
}

// FUNCTION: SWKOTOR 0x0060EEA0
void CSWCCreature::PerformGiveItem(int a, int b, int c)
{
    CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
    if (pMessage) {
        pMessage->SendPlayerToServerInput_GiveItem(m_idSelf, a, b, c);
    }
}

// FUNCTION: SWKOTOR 0x0060EEE0
void CSWCCreature::PerformAttack(unsigned long oid)
{
    CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
    if (pMessage) {
        pMessage->SendPlayerToServerInput_Attack(oid, 0);
    }
}

// FUNCTION: SWKOTOR 0x0060EF10
void CSWCCreature::PerformCastSpell(int a, int b, int c, int d, int e, int f, int g)
{
    CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
    if (pMessage) {
        pMessage->SendPlayerToServerInput_CastSpell(a, d, e, f, g, c, b);
    }
}

// FUNCTION: SWKOTOR 0x0060F780
void CSWCCreature::SetAlignmentGoodEvil(short n)
{
    if (m_pStats) {
        int v;
        if (n >= 0) {
            v = n;
            if (v > 100) {
                v = 100;
            }
        } else {
            v = 0;
        }
        m_pStats->m_nGoodEvil = v;
    }
    if (m_pAppearance) {
        m_pAppearance->SetAlignment(n);
    }
}

// FUNCTION: SWKOTOR 0x006179C0
CGob* CSWCCreature::GetModel(unsigned char nPart, int bLoad)
{
    if (!m_pModel) {
        return 0;
    }
    if (bLoad && m_nPartsLoaded > 0 && nPart == 0xff && !m_pModel->GetPart(0xff)) {
        LoadModelA();
        LoadModelB();
    }
    return m_pModel->GetPart(nPart);
}

// FUNCTION: SWKOTOR 0x00616890
// NONMATCH: same block layout, register allocation differs (ECX/EDX, EBX/EDI, ESI/ECX; 17 insns).
int CSWCCreature::GetForceShield()
{
    CSWSCreature* pServer = GetServerCreature();
    int nResult = 0;
    if (pServer) {
        for (int i = 0; i < pServer->m_appliedEffects.num; i++) {
            int nType = pServer->m_appliedEffects.element[i]->m_nType;
            if (nType == EFFECT_FORCE_SHIELD) {
                nResult = pServer->m_appliedEffects.element[i]->GetForceShield(0);
                break;
            }
            if (nType > EFFECT_FORCE_SHIELD) {
                break;
            }
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x00614670
void CSWCCreature::ShowEquippedItems()
{
    if (m_pModel) {
        ComputeWeaponOverlays(1, 1);
        m_pModel->ShowEquipped();
        m_n3CC = 0;
        if (m_oidProjectile != OBJECT_INVALID_ID) {
            CGameObject* pProjectile = g_pAppManager->m_pClientExoApp->GetProjectileByGameObjectID(m_oidProjectile);
            if (pProjectile) {
                pProjectile->SetVisible(0);
            }
            m_oidProjectile = OBJECT_INVALID_ID;
        }
    }
}

// FUNCTION: SWKOTOR 0x0060F4B0
void CSWCCreature::PartyMenuStealth(int a, int b)
{
    if (!g_pAppManager->m_pServerExoApp->GetPartyTable()->m_bSoloMode &&
        g_pAppManager->m_pClientExoApp->GetSWParty()->m_nSoloMode != 1) {
        g_pAppManager->m_pClientExoApp->GetGuiInGame()->ShowSoloModeQuery(1);
        return;
    }
    PerformSkill(2, 0, m_idSelf);
}

// FUNCTION: SWKOTOR 0x00616140
// NONMATCH: tail merging of the `return c` exits differs; the original reuses ESI for c on the server path (7 insns).
int CSWCCreature::CheckFeat(int a, int b, int c)
{
    int nResult = -1;
    CSWSCreature* pServer = GetServerCreature();
    if (pServer && pServer->m_pStats) {
        if (pServer->m_pStats->HasFeat(a)) {
            nResult = a;
        } else if (pServer->m_pStats->HasFeat(b)) {
            nResult = b;
        } else if (pServer->m_pStats->HasFeat(c)) {
            nResult = c;
        }
    } else {
        if (m_pStats->HasFeat(a)) {
            nResult = a;
        } else if (m_pStats->HasFeat(b)) {
            nResult = b;
        } else if (m_pStats->HasFeat(c)) {
            nResult = c;
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x00616800
// NONMATCH: EAX/EDX swapped for the two object ids and the app pointer register (14 insns).
void CSWCCreature::ActionMenuAttack(int a, CSWCCreature* pTarget)
{
    if (g_pAppManager->m_pClientExoApp->GetAttackMashTutorialTimer() > 0 &&
        g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x23, pTarget->m_idSelf, m_idSelf, 0)) {
        return;
    }
    if (g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x22, pTarget->m_idSelf, m_idSelf, 0)) {
        return;
    }
    pTarget->SetAttackTarget(1);
    g_pAppManager->m_pClientExoApp->SetAttackMashTutorialTimer(3000);
    g_pAppManager->m_pClientExoApp->GetSWCMessage()->SendPlayerToServerInput_Attack(m_idSelf, 0);
}

// FUNCTION: SWKOTOR 0x006198E0
void CSWCCreature::GetHostileItems(int a, int b)
{
    CItemRepository* pRepository = GetServerCreature()->GetItemRepository(1);
    for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
        CSWCItem* pItem = pRepository->ItemListGetItem(i);
        AddHostileItem(a, pItem, b);
    }
}

// FUNCTION: SWKOTOR 0x0060B900 SYMBOL: ?SetAnimationState@CSWCCreature@@QAEXHH@Z
void CSWCCreature::SetAnimationState(int a, int b)
{
    m_n3C = a;
    m_n40 = b;
}

// FUNCTION: SWKOTOR 0x0060F590
// NONMATCH: the original cross-jumps both SendPlayerToServerInput_UseItem calls and keeps the item id in EBX (31 insns).
void CSWCCreature::PartyMenuUseItem(unsigned long oidItem, CGameObject* pTarget)
{
    Vector v;
    unsigned long oid = oidItem & 0xbfffffff;
    if (pTarget->m_idSelf != m_idSelf) {
        CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
        if (pMessage) {
            pMessage->SendPlayerToServerInput_UseItem(oid, 0, 0, 0x7f000000, &m_vPosition);
        }
    } else {
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        CSWCMessage* pMessage = g_pAppManager->m_pClientExoApp->GetSWCMessage();
        if (pMessage) {
            pMessage->SendPlayerToServerInput_UseItem(oid, 0, 0, m_idSelf, &v);
        }
    }
}

// FUNCTION: SWKOTOR 0x006197D0
// NONMATCH: register allocation of this/a/b (ESI/EBP/EDI vs EDI/ESI/EBP; 37 insns).
void CSWCCreature::GetUsableItems(int a, int b)
{
    CSWSCreature* pServer = GetServerCreature();
    if (pServer) {
        if (!pServer->GetArea()->m_bNoItemUse) {
            CSWCItem* pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidEquipped23C);
            if (pItem) {
                CSWSItem* pServerItem = pItem->GetServerItem();
                AddUsableItem(a, pServerItem, 1, b);
            }
            pItem = (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidEquipped240);
            if (pItem) {
                CSWSItem* pServerItem = pItem->GetServerItem();
                AddUsableItem(a, pServerItem, 1, b);
            }
            pItem = (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidEquipped248);
            if (pItem) {
                CSWSItem* pServerItem = pItem->GetServerItem();
                AddUsableItem(a, pServerItem, 1, b);
            }
        }
        CItemRepository* pRepository = pServer->GetItemRepository(1);
        for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
            CSWSItem* pServerItem = (CSWSItem*)pRepository->ItemListGetItem(i);
            AddUsableItem(a, pServerItem, 0, b);
        }
    }
}

// FUNCTION: SWKOTOR 0x0060E5B0
void CSWCCreature::UpdateShadowBlob()
{
    unsigned long oidSelf;
    unsigned char nShadows = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nShadows;
    if (nShadows == 2) {
        RemoveShadow();
    } else if (nShadows == 1) {
        oidSelf = m_idSelf;
        if (oidSelf == g_pAppManager->m_pClientExoApp->GetPlayerCreatureId()) {
            RemoveShadow();
            if (g_pAppManager->m_pClientExoApp->GetModule() && g_pAppManager->m_pClientExoApp->GetModule()->m_p48) {
                g_pAppManager->m_pClientExoApp->GetModule();
            }
        } else {
            RemoveShadow();
            UpdateShadow();
        }
    } else {
        RemoveShadow();
        UpdateShadow();
    }
    if (m_pModel) {
        Vector vUp(0.0f, 0.0f, 1.0f);
        m_pModel->SetShadowOrientation(RotationArc(vUp, m_vShadowNormal));
    }
}

// FUNCTION: SWKOTOR 0x0060E010
// NONMATCH: ESI/EDI swap only (this vs item id; 18 insns).
void CSWCCreature::StripEquippedItems()
{
    CGameObject* pObject;
    for (unsigned short i = 0; i < 0x12; i++) {
        unsigned long oid = GetEquippedItemID(i);
        if (oid != OBJECT_INVALID_ID) {
            CGameObjectArray* pArray = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
            if (pArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
                CSWCItem* pItem = pObject->AsSWCItem();
                pItem->SetEquippedByMe(0);
            }
        }
    }
    if (m_pModel) {
        m_pModel->SetWeaponModel(OBJECT_INVALID_ID, "", 0, 1.0f);
        m_pModel->SetWeaponModel(OBJECT_INVALID_ID, "", 1, 1.0f);
    }
}

// FUNCTION: SWKOTOR 0x0060D460
void CSWCCreature::SetNewLoopingCreatureSound(const CExoString& sSound)
{
    m_pLoopingSound = new CExoSoundSource(sSound);
    if (g_pAppManager->m_pClientExoApp->GetPlayerCreature() == this) {
        m_pLoopingSound->SetPriorityGroup(6);
    } else {
        m_pLoopingSound->SetPriorityGroup(7);
    }
    m_pLoopingSound->SetLooping(1);
}

// FUNCTION: SWKOTOR 0x0060C790
void CSWCCreature::SetWayPoints(unsigned short nCount, Vector* pPoints)
{
    if (m_nWayPoints == nCount) {
        memcpy(m_pWayPoints, pPoints, nCount * sizeof(Vector));
        m_pCurWayPoint = m_pWayPoints;
        return;
    }
    m_nWayPoints = nCount;
    if (m_pWayPoints) {
        delete[] m_pWayPoints;
    }
    if (nCount == 0) {
        m_pWayPoints = 0;
        m_pCurWayPoint = 0;
        return;
    }
    m_pWayPoints = new Vector[nCount];
    m_pCurWayPoint = m_pWayPoints;
    if (m_pWayPoints) {
        memcpy(m_pWayPoints, pPoints, nCount * sizeof(Vector));
    }
}

// FUNCTION: SWKOTOR 0x0060DED0
void CSWCCreature::DoFootstepAudio(unsigned char bSkip)
{
    if (bSkip) {
        return;
    }
    unsigned long oidSelf = m_idSelf;
    if (g_pAppManager->m_pClientExoApp->GetPlayerCreatureId() == oidSelf && rand() % 100 < 5) {
        CExoString sName;
        sName = g_sFootstepPrefix;
        sName = sName + CExoString(rand() % 3 + 1);
        CResRef cResRef(sName);
        g_pExoSound->PlaySoundAt(cResRef, m_vPosition, 0, 0x13, 0, 0, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x00611700
int CSWCCreature::ComputeWeaponOverlays(int bCheck, int b)
{
    if (!GetAnimationBase()) {
        return 0;
    }
    if (bCheck == 1 &&
        GetAnimationBase()->HasAnimation(0x272e) != 1 &&
        GetAnimationBase()->HasAnimation(0x27ab) != 1 &&
        GetAnimationBase()->HasAnimation(0x2736) != 1 &&
        GetAnimationBase()->HasAnimation(0x27aa) != 1 &&
        GetAnimationBase()->HasAnimation(0x2737) != 1 &&
        GetAnimationBase()->HasAnimation(0x2738) != 1 &&
        GetAnimationBase()->HasAnimation(0x2739) != 1 &&
        GetAnimationBase()->HasAnimation(0x273a) != 1 &&
        !m_n398 && m_n2D0 != 0) {
        g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(GetEquippedItemID(0x20));
        if (m_n138 != 1) {
            GetAnimationBase();
            g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(GetEquippedItemID(0x10));
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00617330
int CSWCCreature::UpdateAttackData(unsigned short* pAnimation, CSWCAttackData* pData)
{
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(pData->m_oidTarget);
    if (!pObject) {
        *pAnimation = 0xffff;
        pData->m_nReaction = 0xffff;
        return 0;
    }
    unsigned char nStance = GetAnimationBase()->m_nStance;
    CSWCCreature* pTarget = pObject->AsClientCreature();
    int nAnimation = m_nForcedAttackAnimation;
    if (nAnimation) {
        m_nForcedAttackAnimation = 0;
    } else {
        nAnimation = g_nDebugAttackAnimation;
        if (!nAnimation || m_idSelf != 0xffffffff) {
            if (pData->m_bRanged) {
                nAnimation = ResolveRangedAnimation(pData->m_nAttackType, nStance);
            } else {
                int bCombat = InCreatureCombat(pTarget);
                nAnimation = ResolveMeleeAnimation(*pAnimation, pData->m_nAttackType, bCombat, nStance);
            }
        }
    }
    unsigned char nTargetStance = pTarget->GetAnimationBase()->m_nStance;
    if (pTarget && pTarget->AsSWCCreature() && pTarget->AsSWCCreature()->InCombat()) {
        nTargetStance = 0;
    }
    int nReaction = ResolveReaction(nAnimation, pData->m_nReaction, nTargetStance);
    SetAttackAnimationData(pData->m_nAttackType, nStance, pData->m_bRanged);
    *pAnimation = (unsigned short)nAnimation;
    pData->m_nReaction = nReaction;
    return 1;
}

// FUNCTION: SWKOTOR 0x0060B330
// NONMATCH: the original keeps the short in CX and builds the result in EAX (7 insns).
int CSWCCreatureStats::GetAlignmentCategory()
{
    short n = m_nGoodEvil;
    if (n <= 40) {
        return 3;
    }
    if (n < 60) {
        return 1;
    }
    return 2;
}

// FUNCTION: SWKOTOR 0x0060B350
unsigned char CSWCCreatureStats::GetSkillRank(unsigned short nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills) {
        return 0;
    }
    return m_pSkills[nSkill];
}

// FUNCTION: SWKOTOR 0x0060B380
void CSWCCreature::SetDesiredDirection(const Vector& v)
{
    m_vDesiredDirection = NormalizeVector(v);
}

// FUNCTION: SWKOTOR 0x0060B3C0
void CSWCCreature::SetDesiredOrientation(const Vector& v)
{
    m_vDesiredOrientation = NormalizeVector(v);
}

// FUNCTION: SWKOTOR 0x0060BAD0
void CSWCCreature::ClearPathRequest()
{
    if (m_pPathRequest) {
        m_pPathRequest->Notify(0);
        delete m_pPathRequest;
        m_pPathRequest = 0;
        m_n174 = 0;
        m_n178 = 0;
    }
}

// FUNCTION: SWKOTOR 0x0060BF10
void CSWCCreature::ResetDesiredPositionTimer()
{
    m_nDesiredPositionTimer = 0x200;
}

// FUNCTION: SWKOTOR 0x0060C4E0
void CSWCCreature::GetWayPoints(unsigned short* pCount, Vector** ppPoints, unsigned short* pCurrent)
{
    *pCount = m_nWayPoints;
    *ppPoints = m_pWayPoints;
    for (int i = 0; i < m_nWayPoints; i++) {
        if (&m_pWayPoints[i] == m_pCurWayPoint) {
            *pCurrent = i;
            return;
        }
    }
    *pCurrent = 0;
}

// FUNCTION: SWKOTOR 0x0060C8C0
void CSWCCreature::SetLightingFlag(unsigned char n)
{
    m_nLightingFlag = n;
    m_pModel->m_nLightingFlag = n;
}

// FUNCTION: SWKOTOR 0x0060D430
int CSWCCreature::SelectByMode(int nMode, int a, int b, int c)
{
    int nResult = 0;
    switch (nMode) {
    case 4:
        nResult = c;
        break;
    case 3:
        nResult = b;
        break;
    case 2:
        nResult = a;
        break;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x0060D510
void CSWCCreature::StopLoopingCreatureSound()
{
    m_pLoopingSound->Stop();
    delete m_pLoopingSound;
    m_pLoopingSound = 0;
}

// FUNCTION: SWKOTOR 0x0060E120
void __cdecl CSWCCreature::GetHeadTurnRateCallback(CGameObject* pObject, void* pUnused, float* pResult)
{
    float f = 0.0f;
    CSWCCreature* pCreature = pObject->AsCreatureForCallback();
    if (pCreature) {
        g_pRules->m_p2DArrays->m_pAppearanceTable->GetFLOATEntry(pCreature->m_pAppearance->m_nAppearanceType, g_nHeadTurnRateColumn, &f);
        *pResult = f;
    }
}

// FUNCTION: SWKOTOR 0x0060E170
void __cdecl CSWCCreature::GetHeadBoneCallback(CGameObject* pObject, void* pUnused, float* pResult)
{
    float f = 0.0f;
    CSWCCreature* pCreature = pObject->AsCreatureForCallback();
    if (pCreature) {
        g_pRules->m_p2DArrays->m_pAppearanceTable->GetFLOATEntry(pCreature->m_pAppearance->m_nAppearanceType, g_nHeadBoneColumn, &f);
        *pResult = f;
    }
}

// FUNCTION: SWKOTOR 0x0060E1D0
void CSWCCreature::SetSpecialWalk(unsigned char b)
{
    GetAnimationBase()->m_bSpecialWalk = b;
}

// FUNCTION: SWKOTOR 0x0060E1F0
void CSWCCreature::SetRunQuality(int n)
{
    GetAnimationBase()->m_nRunQuality = n;
}

// FUNCTION: SWKOTOR 0x0060E210
void CSWCCreature::StartHeadTracking()
{
    if (m_pModel) {
        m_pModel->StartHeadTracking();
        m_bHeadTracking = 1;
    }
}

// FUNCTION: SWKOTOR 0x0060E230
void CSWCCreature::StopHeadTracking()
{
    if (m_pModel) {
        m_pModel->StopHeadTracking();
        m_bHeadTracking = 0;
    }
}

// FUNCTION: SWKOTOR 0x0060E250
// NONMATCH: the original loads m_pAppearance before the 2DA chain without a copy of this (5 insns).
float CSWCCreature::GetHeadTurnHorizontal()
{
    float f;
    CCreatureAppearanceX* pAppearance = m_pAppearance;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
    unsigned short nAppearance = pAppearance->m_nAppearanceType;
    if (!pTable->GetFLOATEntry(nAppearance, g_nHeadTurnHColumn, &f)) {
        return g_fDefaultHeadTurnH;
    }
    return f;
}

// FUNCTION: SWKOTOR 0x0060E2A0
// NONMATCH: the original loads m_pAppearance before the 2DA chain without a copy of this (5 insns).
float CSWCCreature::GetHeadTurnVertical()
{
    float f;
    CCreatureAppearanceX* pAppearance = m_pAppearance;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
    unsigned short nAppearance = pAppearance->m_nAppearanceType;
    if (!pTable->GetFLOATEntry(nAppearance, g_nHeadTurnVColumn, &f)) {
        return g_fDefaultHeadTurnV;
    }
    return f;
}

// FUNCTION: SWKOTOR 0x0060E760
void CSWCCreature::DoNothing(int n)
{
}

// FUNCTION: SWKOTOR 0x0060E7C0
void CSWCCreature::ResetAmbientPause()
{
    m_n390 = 0;
    m_n398 = 0;
}

// FUNCTION: SWKOTOR 0x0060F010
float CSWCCreature::GetCameraOffset()
{
    return m_pAppearance->GetCameraOffset();
}

// FUNCTION: SWKOTOR 0x0060F020
// NONMATCH: EAX/EDX and ECX/EAX register choice around the two app reloads (8 insns).
int CSWCCreature::GetBlockedAnimation()
{
    if (m_n138) {
        return 0x2711;
    }
    if (g_pAppManager->GetServerExoApp() && g_pAppManager->GetServerExoApp()->IsInjured(m_idSelf)) {
        if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode != 5 ||
            this != g_pAppManager->GetClientExoApp()->GetSWParty()->GetPartyMember(0)) {
            return 0x276c;
        }
    }
    return 0x2710;
}

// FUNCTION: SWKOTOR 0x0060F090
void CSWCCreature::SetCombatMode(int nMode)
{
    switch (nMode) {
    case 2:
        m_nCombatMode = 2;
        break;
    case 1:
        m_nCombatMode = 1;
        break;
    case 0:
        m_nCombatMode = 0;
        break;
    }
}

// FUNCTION: SWKOTOR 0x0060F0D0
void CSWCCreature::SetAttackPair(int a, int b)
{
    m_n3B4 = b;
    m_n3B0 = a;
}

// FUNCTION: SWKOTOR 0x0060F0F0
int CSWCCreature::GetIsBlocked()
{
    int bBlocked = m_bEngaged;
    unsigned long oid = g_pAppManager->m_pServerExoApp->ClientToServerObjectID(m_idSelf);
    if (g_pAppManager->m_pServerExoApp->IsActionsPending(oid)) {
        bBlocked = 1;
    }
    return IsAnimationBlocked(m_nAnimation) ? 1 : bBlocked;
}

// FUNCTION: SWKOTOR 0x0060F140
void CSWCCreature::SetEngaged(int b)
{
    m_bEngaged = b;
}

// FUNCTION: SWKOTOR 0x0060F1A0
int CSWCCreature::RemoveRumbleSource()
{
    CGob* pPart = FindPart(0xff, 1);
    if (m_pRumbleSource && pPart) {
        pPart->RemoveSource(m_pRumbleSource);
        m_pRumbleSource = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0060F540
void CSWCCreature::ResetModelState()
{
    m_pModel->ResetState(0, 0, 0, 0);
}

// FUNCTION: SWKOTOR 0x0060FB20
CSWSCreature* CSWCCreature::AsServerCreature()
{
    return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
}

// FUNCTION: SWKOTOR 0x0060FB40
void CSWCCreature::SetPartyTransition(int nMode)
{
    if (m_bInMiniGame && g_pAppManager->m_pClientExoApp->GetMiniGame()) {
        SetTransitionMode(0);
        return;
    }
    SetTransitionMode(nMode);
}

// NONMATCH (5 real diff insns; the rest is the jump table): the original
// loads the appearance row before the 2DA chain; locals in either order, the
// inline appearance getter and a direct call expression all schedule the
// table chain first.
// FUNCTION: SWKOTOR 0x0060E2F0
CGob* CSWCCreature::ApplyShadowBlob()
{
    CExoString sModel;
    if (!m_pModel) {
        return 0;
    }
    int nSize;
    unsigned short nAppearance = m_pAppearance->m_nAppearanceType;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
    if (!pTable->GetINTEntry(nAppearance, g_nAppearanceCol_SIZECATEGORY, &nSize)) {
        nSize = 3;
    }
    switch (nSize) {
    case 1:
    case 2:
        sModel = "Fx_Shadow_S";
        break;
    case 4:
    case 5:
        sModel = "Fx_Shadow_L";
        break;
    default:
        sModel = "Fx_Shadow_M";
        break;
    }
    CGob* pShadow = m_pModel->AddShadowBlob(sModel);
    Vector vUp(0.0f, 0.0f, 1.0f);
    m_pModel->SetShadowOrientation(RotationArc(vUp, m_vShadowNormal));
    if (!m_bNoShadowRefresh) {
        RefreshShadow();
    }
    return pShadow;
}
