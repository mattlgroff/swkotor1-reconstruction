// Creature animation/appearance/state family. Layout stays local: extending
// the shared partial creature would change the offsets of existing subclasses.
#include "c/exo_base.h"
#include "c/sw_c_creature_ability_list.h"

class CResRef {
public:
    char m_resref[16];
    CResRef();
    CResRef(const CExoString& s);
    int operator==(const char* s) const;
    CResRef operator=(const CResRef& s);
    int operator!=(const CResRef& s) const;
};
class CSWCCreature;
class CSWSCreature;
class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();
};
class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24();
    virtual unsigned long RegisterCallback(void (__cdecl *pfn)(void*, char*, void*),
        const char* sEvent, void* pUser, float fPeriod, int b);
    virtual void UnregisterCallback(unsigned long nHandle); // +2c
};
class CAnimationBase {
public:
    virtual void v00(); virtual void v04(); virtual CGob* GetModel(unsigned long nPart);
    virtual void v0C(); virtual void v10(); virtual void v14(); virtual void v18();
    virtual void v1C(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2C(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3C(); virtual void v40(); virtual void v44(); virtual void v48();
    virtual void v4C(); virtual void v50(); virtual void v54(); virtual void v58();
    virtual void v5C(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6C(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7C(); virtual void HideEquippedItems(const CResRef&, const CResRef&);
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4();
    virtual void ApplyVisualStateA();
    virtual void ApplyVisualStateB();
    char pad_0x4[0xc1];
    unsigned char m_nWeaponClass;
    unsigned char m_nUnusedC6;
    unsigned char m_nWieldType;
};
class CSWSCreature {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual int GetIsDead();
    int GetDeadTemp(); // 0x004ef890
    char pad_0x4[0xab0];
    float m_fRate; // +ab4; semantic name unresolved.
};
class C2DA {
public:
    int GetCExoStringEntry(int, const CExoString&, CExoString*);
    int GetCExoStringEntry(int, int, CExoString*);
    int GetINTEntry(int, int, int*);
    int GetFLOATEntry(int, int, float*);
};
class C2DAs {
public:
    char pad_0x0[0xc]; C2DA* m_pAppearanceTable;
    char pad_0x10[0x48]; C2DA* m_pFootstepTable;
};
extern int g_nAppearanceCol_FootstepType; // 0x007a20ec
extern int g_nFootstepRollingColumn; // 0x007a219c
extern int g_nAppearanceCol_SIZECATEGORY; // 0x007a2124
extern int g_nFootstepPitchOffsetColumn; // 0x007a21a0
struct Vector { float x, y, z; };
class CExoSoundSource {
public:
    virtual ~CExoSoundSource();
    void* m_pInternal;
    CExoSoundSource(const CResRef& s);
    void Set3D(int b);
    void SetPriorityGroup(unsigned char n);
    void SetLooping(int b);
    void SetPosition(const Vector& v, float f);
    int SetFixedVariance(float f);
    int Play();
    void Stop();
};
class CExoStreamingSoundSource {
public:
    void* m_pInternal;
    CExoStreamingSoundSource();
    ~CExoStreamingSoundSource();
    void Set3D(int b);
    void SetPriorityGroup(unsigned char n);
    void SetPosition(const Vector& v);
    int Play(const CResRef& s, int n, int a, int b, int c);
};
struct STR_RES { CExoString m_sText; CResRef m_sSound; float m_fSoundLength; };
class CSoundSet {
public:
    virtual ~CSoundSet();
    char pad_0x4[0x18];
    CSoundSet(const CResRef& s);
};
class CSoundSetList {
public:
    CResRef GetSoundSetResRef(unsigned short nId); // 0x006788d0, descriptive name.
};
class CSWCCreaturePartyStatus {
public:
    char pad_0x0[4];
    int m_bActive, m_bEnabled, m_nState;
    char pad_0x10[4];
    unsigned long m_oidCreature;
    CExoString m_sName;
    CSWCCreaturePartyStatus();
};
class CSWBaseItem {
public: char pad_0x0[8]; unsigned char m_nWeaponClass, m_nWieldType;
};
class CSWBaseItemArray { public: CSWBaseItem* GetBaseItem(int nBaseItem); };
class CSWRules {
public: char pad_0x0[0x34]; CSWBaseItemArray* m_pBaseItems;
    char pad_0x38[0x80]; C2DAs* m_p2DArrays;
};
extern CSWRules* g_pRules;
class CSceneHolder {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void* GetCamera(unsigned long nId);
};
class CSWCArea {
public:
    char pad_0x0[0x104]; unsigned long m_idSelf;
    char pad_0x108[0x7c]; void* m_pScene;
};
extern const unsigned long OBJECT_INVALID_ID;
class CSWCCreatureStats {
public:
    char pad_0x0[0x70]; CResRef m_sPortrait;
    void SetPortraitId(unsigned short nId);
};
class CSWCModule {
public:
    char pad_0x0[0x40];
    CSceneHolder* m_pSceneHolder;
    char pad_0x44[4];
    CSWCArea* m_pArea;
    void SetMapCharacter(unsigned long oid, int n);
};
struct CPerceptionRecord {
    unsigned long m_oidCreature;
    unsigned char m_nFlags;
};
class CSpellLikeAbilityList {
public:
    CSWSStats_SpellLikeAbility* element;
    int num, array_size;
    CSpellLikeAbilityList() { num = 0; array_size = 0; element = 0; }
    ~CSpellLikeAbilityList() { delete[] element; element = 0; }
    void SetSize(int n) {
        if (!n && element) { delete[] element; element = 0; array_size = 0; }
        num = n;
    }
};
class CSWPartyClient {
public:
    void SetPlayerCreature(unsigned long oid);
    int RemovePartyMember(unsigned long oid); // 0x006349f0, descriptive callee name.
    void SetPC(int nIndex);
};
class CItemVisualState {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4();
    virtual void vB8();
    virtual void vBC();
    virtual void vC0();
    virtual void vC4();
    virtual void vC8();
    virtual void vCC();
    virtual void vD0();
    virtual void vD4();
    virtual void vD8();
    virtual void vDC();
    virtual void vE0();
    virtual void vE4();
    virtual void vE8();
    virtual void vEC();
    virtual void vF0();
    virtual void vF4();
    virtual void vF8();
    virtual void vFC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void ApplyVisualStateA();
    virtual void ApplyVisualStateB();
};
class CSWCItem : public CGameObject {
public:
    char pad_0x4[8];
    int m_nBaseItem;
    CItemVisualState m_visualState; // +10, nested visual interface.
};
extern void __cdecl QueuedAnimationEvent(void*, char*, void*); // 0x0063e510, descriptive callback name.
class CCreatureAppearance {
public:
    void* GetCameraFollowPart(); // 0x00697460
    int GetFollowingBody(); // 0x006974a0
    float GetHeadHeightOffset(); // 0x006974d0
    int GetUnavailableProperty(); // folded constant-zero helper at 0x0063e7f0.
    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;
    char pad_0x1a[0x3e];
    float m_fDriveAcceleration;
};
class CClientExoApp {
public:
    int GetPausedByCombat(); // 0x005edc10
    CSWCModule* GetModule();
    unsigned long GetPlayerCreatureId();
    CSoundSetList* GetSoundSetList();
    CGameObject* GetItemByGameObjectID(unsigned long oid);
    class CClientPlayer* GetPlayerByGameObjectID(unsigned long oid);
    CSWCCreature* GetPlayerCreature();
    CSWPartyClient* GetSWParty();
    int IsPlayerCreature(CGameObject* p);
};
class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;

class CSWCCreature {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual CSWCCreature* AsClientCreature();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void AttachToScene(void* pScene);
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4();
    virtual void vB8();
    virtual void vBC();
    virtual void vC0();
    virtual void vC4();
    virtual void vC8();
    virtual void vCC();
    virtual void vD0();
    virtual void vD4();
    virtual void vD8();
    virtual void vDC();
    virtual void vE0();
    virtual void vE4();
    virtual void vE8();
    virtual void vEC();
    virtual void vF0();
    virtual void vF4();
    virtual void vF8();
    virtual void vFC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12C();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13C();
    virtual void v140();
    virtual void v144();
    virtual CResRef GetPortrait(int nVariant);
    void UnregisterAnimationCallbacks(); // descriptive name; +3ec array of 17 handles.
    float GetDriveAcceleration();
    float GetServerRate(); // descriptive name; reads server creature +ab4.
    void PlayDrawAnimation(unsigned short nAnimation);
    void ClearVisibilityList();
    void HideEquippedItems(const CResRef& a, const CResRef& b);
    unsigned short GetAppearanceType() { return m_pAppearance->m_nAppearanceType; }
    void SetProjectileObjectId(unsigned long oid); // descriptive +3c8 setter name.
    void UpdateAreaId();
    void* GetCameraFollowPart();
    int GetFollowingBody();
    float GetHeadHeightOffset();
    void SetPortrait(const CResRef& sPortrait);
    void SetPortraitId(unsigned short nId);
    void DoNothing(const CResRef& s); // 0x0060e760, folded RET 4.
    unsigned long GetEquippedItemID(unsigned long nSlot); // 0x0060ca80
    unsigned long GetHandItemId(unsigned long nHand); // descriptive name.
    void UpdateWeaponClass();
    void PlayVoiceChat(unsigned short nIndex, int bSpatial);
    int GetSoundSetStrres(unsigned short nIndex, STR_RES* pResource);
    void SetQueuedAnimationCallback(); // descriptive source spelling normalized.
    void UpdateEquippedVisualStateA(); // descriptive forwarding names.
    void UpdateEquippedVisualStateB();
    int LoadRollingFootstepSound();
    void PlayRollingFootstepSound(int bRun);
    void UpdateRollingFootstepSound();
    void SetSoundSet(unsigned short nId);
    unsigned short GetCurrentAnimation(); // CSWCObject equivalent receiver, 0x0063c060
    int InCombat();
    void LoadModelA();
    void LoadModelB();
    void CreateSpellLikeAbilityList();
    void DeleteSpellLikeAbilityList();
    void SetPartyMember(int b);
    int GetAppearanceProperty(); // descriptive wrapper name.
    void SetAppearance(void* pAppearance, unsigned char nFlags, int b);
    CGameObject* GetServerObject(); // 0x0063d4b0
    CAnimationBase* GetAnimationBase(); // 0x0063c070
    void PlayAnimation(unsigned short nAnimation, int nDuration, float fSpeed, int a, int b); // 0x0063c760
    CSWSCreature* GetServerCreature() {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }
    unsigned long m_idSelf;
    char pad_0x8[0x18];
    CSWCArea* m_pArea;
    Vector m_vPosition;
    char pad_0x30[0x38];
    CAnimationBase* m_pAnimationBase;
    char pad_0x6c[0x60];
    CExoStreamingSoundSource* m_pVoiceSound;
    int m_nVoiceState;
    char pad_0xd4[0x20];
    unsigned long m_nQueuedAnimationCallback;
    char pad_0xf8[0x124];
    CCreatureAppearance* m_pAppearance;
    unsigned long m_oidVisualItem;
    char pad_0x224[0xc];
    unsigned long m_oidRightWeapon, m_oidLeftWeapon;
    char pad_0x238[0x14];
    unsigned long m_oidHand3, m_oidHand4, m_oidHand5;
    char pad_0x258[0x7c];
    unsigned char m_nMovementFlags;
    char pad_0x2d5[0x23];
    CSWCCreatureStats* m_pStats;
    unsigned short m_nSoundSet;
    char pad_0x2fe[0x12];
    int m_bAppearancePending;
    int m_bCameraDetached;
    int m_nCameraMode;
    char m_aAppearance[0x3c];
    char pad_0x358[8];
    CExoArrayList<CPerceptionRecord*> m_lstVisibility;
    CSoundSet* m_pSoundSet;
    char pad_0x370[0x1c];
    unsigned short m_nOriginalAppearance;
    char pad_0x38e[0xe];
    unsigned long m_oidArea;
    CSpellLikeAbilityList* m_pSpellLikeAbilities;
    int m_bPartyMember;
    char pad_0x3a8[0x20];
    unsigned long m_oidProjectile;
    int m_bEquippedItemsHidden;
    char pad_0x3d0[0x1c];
    unsigned long m_nAnimationCallbacks[17];
    CExoSoundSource* m_pRollingSound;
    float m_fRollingPitch;
    int m_bRollingSoundPlaying;
    int m_bRollingSoundRun;
    char pad_0x440[4];
    unsigned short m_nDrawAnimation;
};

// FUNCTION: SWKOTOR 0x00610550
void CSWCCreature::UnregisterAnimationCallbacks()
{
    CGob* pModel = m_pAnimationBase->GetModel(0xff);
    if (pModel) {
        for (int i = 0; i < 17; ++i) {
            if (m_nAnimationCallbacks[i]) pModel->UnregisterCallback(m_nAnimationCallbacks[i]);
        }
    }
}

// FUNCTION: SWKOTOR 0x00610590
float CSWCCreature::GetDriveAcceleration()
{
    if (m_nMovementFlags & 1) return 15.0f;
    return m_pAppearance->m_fDriveAcceleration;
}

// FUNCTION: SWKOTOR 0x006109B0
float CSWCCreature::GetServerRate()
{
    if (GetServerObject() && GetServerObject()->AsSWSCreature()) {
        return GetServerCreature()->m_fRate;
    }
    return 37.0f;
}

// FUNCTION: SWKOTOR 0x00610A70
void CSWCCreature::PlayDrawAnimation(unsigned short nAnimation)
{
    if (g_pAppManager->m_pClientExoApp->GetPausedByCombat()) {
        m_nDrawAnimation = nAnimation;
    } else {
        PlayAnimation(nAnimation, 1500, 1.0f, 0, 0);
        m_nDrawAnimation = 0xffff;
    }
}

// FUNCTION: SWKOTOR 0x006145D0
void CSWCCreature::ClearVisibilityList()
{
    for (int i = m_lstVisibility.num - 1; i >= 0; --i) {
        CPerceptionRecord* p = m_lstVisibility[i];
        m_lstVisibility.DelIndex(i);
        if (p) delete p;
    }
}

// FUNCTION: SWKOTOR 0x00614630
void CSWCCreature::HideEquippedItems(const CResRef& a, const CResRef& b)
{
    if (m_pAnimationBase) {
        GetAnimationBase();
        m_pAnimationBase->HideEquippedItems(a, b);
        m_bEquippedItemsHidden = 1;
    }
}

// FUNCTION: SWKOTOR 0x0060D320
// Existing verified symbol is InCombat; this implementation checks MODELTYPE.
// NONMATCH: appearance/rules load ordering and ESI reuse differ (12 instructions).
int CSWCCreature::InCombat()
{
    int bResult = 0;
    CExoString sType;
    CExoString sColumn("MODELTYPE");
    unsigned short nAppearance = GetAppearanceType();
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
    pTable->GetCExoStringEntry(nAppearance, sColumn, &sType);
    char c = sType[0];
    if (c == 'S' || c == 'L' || c == 's' || c == 'l') bResult = 1;
    return bResult;
}

// FUNCTION: SWKOTOR 0x00614A30
// Existing verified call symbol LoadModelA (semantic hint ForceAppearanceUpdate).
void CSWCCreature::LoadModelA()
{
    m_bAppearancePending = 0;
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    m_pAppearance->m_nAppearanceType = m_nOriginalAppearance;
    SetAppearance(m_aAppearance, 0xef, 1);
    if (pModule && pModule->m_pArea) AttachToScene(pModule->m_pArea->m_pScene);
}

// FUNCTION: SWKOTOR 0x0060E6C0
// Existing verified call symbol LoadModelB (semantic hint ReAttachCamera).
void CSWCCreature::LoadModelB()
{
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    if (!m_bAppearancePending && this == g_pAppManager->m_pClientExoApp->GetPlayerCreature() &&
        m_bCameraDetached && pModule && pModule->m_pSceneHolder &&
        !pModule->m_pSceneHolder->GetCamera(0x106d) && !pModule->m_pSceneHolder->GetCamera(0x106e)) {
        pModule->SetMapCharacter(m_idSelf, m_nCameraMode);
        m_bCameraDetached = 0;
        m_nCameraMode = 0;
    }
}

// FUNCTION: SWKOTOR 0x00617A20
// NONMATCH: original retains a new-expression EH frame; this inline list ctor removes it.
void CSWCCreature::CreateSpellLikeAbilityList()
{
    m_pSpellLikeAbilities = new CSpellLikeAbilityList();
}

// FUNCTION: SWKOTOR 0x00617A70
void CSWCCreature::DeleteSpellLikeAbilityList()
{
    if (m_pSpellLikeAbilities) {
        m_pSpellLikeAbilities->SetSize(0);
        delete m_pSpellLikeAbilities;
        m_pSpellLikeAbilities = 0;
    }
}

// FUNCTION: SWKOTOR 0x0060EF50
// NONMATCH: established void ABI retained; the original leaves EAX=1 on the unchanged path.
void CSWCCreature::SetPartyMember(int b)
{
    if (m_bPartyMember != b) {
        m_bPartyMember = b;
        if (b) {
            unsigned long oid = m_idSelf;
            return g_pAppManager->m_pClientExoApp->GetSWParty()->SetPlayerCreature(oid);
        }
        if (g_pAppManager->m_pClientExoApp->IsPlayerCreature((CGameObject*)this)) {
            g_pAppManager->m_pClientExoApp->GetSWParty()->SetPC(-3);
        }
        unsigned long oid = m_idSelf;
        g_pAppManager->m_pClientExoApp->GetSWParty()->RemovePartyMember(oid);
        return;
    }
    return;
}

// FUNCTION: SWKOTOR 0x0060D540
int CSWCCreature::GetAppearanceProperty()
{
    return m_pAppearance ? m_pAppearance->GetUnavailableProperty() : 0;
}

// FUNCTION: SWKOTOR 0x0060ED80
void CSWCCreature::SetProjectileObjectId(unsigned long oid)
{
    m_oidProjectile = oid;
}

// FUNCTION: SWKOTOR 0x0060ED90
void CSWCCreature::UpdateAreaId()
{
    if (m_pArea) m_oidArea = m_pArea->m_idSelf;
    else m_oidArea = OBJECT_INVALID_ID;
}

// FUNCTION: SWKOTOR 0x0060EFE0
void* CSWCCreature::GetCameraFollowPart()
{
    return m_pAppearance->GetCameraFollowPart();
}

// FUNCTION: SWKOTOR 0x0060EFF0
int CSWCCreature::GetFollowingBody()
{
    return m_pAppearance->GetFollowingBody();
}

// FUNCTION: SWKOTOR 0x0060F000
float CSWCCreature::GetHeadHeightOffset()
{
    return m_pAppearance->GetHeadHeightOffset();
}

// FUNCTION: SWKOTOR 0x0060D560
void CSWCCreature::SetPortrait(const CResRef& sPortrait)
{
    if (GetPortrait(0) != sPortrait) {
        m_pStats->m_sPortrait = sPortrait;
        DoNothing(GetPortrait(1));
    }
}

// FUNCTION: SWKOTOR 0x0060D5C0
void CSWCCreature::SetPortraitId(unsigned short nId)
{
    if (m_pStats) {
        m_pStats->SetPortraitId(nId);
        DoNothing(GetPortrait(1));
    }
}

// FUNCTION: SWKOTOR 0x0060D070
unsigned long CSWCCreature::GetHandItemId(unsigned long nHand)
{
    switch (nHand) {
    case 1: case 6: return GetEquippedItemID(0x10);
    case 2: return GetEquippedItemID(0x20);
    case 3: return m_oidHand3;
    case 4: return m_oidHand4;
    case 5: return m_oidHand5;
    default: return 0x7f000000;
    }
}

// FUNCTION: SWKOTOR 0x006107C0
void CSWCCreature::PlayRollingFootstepSound(int bRun)
{
    if (m_pRollingSound) {
        m_pRollingSound->SetPosition(m_vPosition, 0.0f);
        if (!m_bRollingSoundPlaying || m_bRollingSoundRun != bRun) {
            if (bRun) m_pRollingSound->SetFixedVariance(m_fRollingPitch + 1.0f);
            else m_pRollingSound->SetFixedVariance(1.0f);
            m_pRollingSound->Play();
            m_bRollingSoundPlaying = 1;
            m_bRollingSoundRun = bRun;
        }
    }
}

// FUNCTION: SWKOTOR 0x00610840
void CSWCCreature::UpdateRollingFootstepSound()
{
    if (!m_pRollingSound) return;
    switch (GetCurrentAnimation()) {
    case 0x2712: case 0x2713: case 0x276d: case 0x2795:
        PlayRollingFootstepSound(0); return;
    case 0x2714: case 0x276e:
        PlayRollingFootstepSound(1); return;
    }
    if (m_pRollingSound && m_bRollingSoundPlaying) {
        m_pRollingSound->Stop();
        m_bRollingSoundPlaying = 0;
        m_bRollingSoundRun = 0;
    }
}

// FUNCTION: SWKOTOR 0x006105B0
// NONMATCH: load scheduling, cold return placement and CResRef argument materialization.
int CSWCCreature::LoadRollingFootstepSound()
{
    CExoString sSound;
    if (m_pRollingSound) {
        delete m_pRollingSound;
        m_pRollingSound = 0;
    }
    int nFootstep;
    unsigned short nAppearance = m_pAppearance->m_nAppearanceType;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
    if (pTable->GetINTEntry(nAppearance, g_nAppearanceCol_FootstepType, &nFootstep) &&
        g_pRules->m_p2DArrays->m_pFootstepTable->GetCExoStringEntry(
            nFootstep, g_nFootstepRollingColumn, &sSound)) {
    } else return 0;
    m_pRollingSound = new CExoSoundSource(CResRef(sSound));
    m_pRollingSound->Set3D(1);
    unsigned char nPriority;
    if (m_idSelf == g_pAppManager->m_pClientExoApp->GetPlayerCreatureId()) {
        nPriority = 0x13;
    } else {
        int nSize;
        nAppearance = m_pAppearance->m_nAppearanceType;
        pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
        pTable->GetINTEntry(nAppearance, g_nAppearanceCol_SIZECATEGORY, &nSize);
        if (nSize == 1 || nSize == 2 || nSize == 3) nPriority = 0x14;
        else nPriority = 0x12;
    }
    m_pRollingSound->SetPriorityGroup(nPriority);
    m_pRollingSound->SetLooping(1);
    m_pRollingSound->SetPosition(m_vPosition, 1.5f);
    g_pRules->m_p2DArrays->m_pFootstepTable->GetFLOATEntry(
        nFootstep, g_nFootstepPitchOffsetColumn, &m_fRollingPitch);
    m_bRollingSoundPlaying = 0;
    m_bRollingSoundRun = 0;
    return m_pRollingSound != 0;
}

// FUNCTION: SWKOTOR 0x0060B7C0
void CSWCCreature::SetSoundSet(unsigned short nId)
{
    if (nId != m_nSoundSet) {
        m_nSoundSet = nId;
        if (m_pSoundSet) {
            delete m_pSoundSet;
            m_pSoundSet = 0;
        }
        if (m_nSoundSet != 0xffff) {
            CSoundSetList* pList = g_pAppManager->m_pClientExoApp->GetSoundSetList();
            if (pList) {
                CResRef sRef = pList->GetSoundSetResRef(m_nSoundSet);
                if (!(sRef == "")) m_pSoundSet = new CSoundSet(sRef);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0060B750
// NONMATCH: zero/EH-state register choice before the scalar initializers.
CSWCCreaturePartyStatus::CSWCCreaturePartyStatus()
{
    m_nState = 0;
    m_bEnabled = m_bActive = 1;
    m_oidCreature = 0x7f000000;
    m_sName = "";
}

// FUNCTION: SWKOTOR 0x0060F150
void CSWCCreature::SetQueuedAnimationCallback()
{
    if (!m_nQueuedAnimationCallback) {
        CGob* pModel = FindPart(0xff, 1);
        m_nQueuedAnimationCallback = pModel->RegisterCallback(
            QueuedAnimationEvent, "*", AsClientCreature(), 9999.0f, 0);
    }
}

// FUNCTION: SWKOTOR 0x0060E490
void CSWCCreature::UpdateEquippedVisualStateA()
{
    CSWCItem* pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidRightWeapon);
    if (pItem) pItem->m_visualState.ApplyVisualStateA();
    pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidLeftWeapon);
    if (pItem) pItem->m_visualState.ApplyVisualStateA();
    pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidVisualItem);
    if (pItem) pItem->m_visualState.ApplyVisualStateA();
    if (m_pAnimationBase) m_pAnimationBase->ApplyVisualStateA();
}

// FUNCTION: SWKOTOR 0x0060E520
void CSWCCreature::UpdateEquippedVisualStateB()
{
    CSWCItem* pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidRightWeapon);
    if (pItem) pItem->m_visualState.ApplyVisualStateB();
    pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidLeftWeapon);
    if (pItem) pItem->m_visualState.ApplyVisualStateB();
    pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidVisualItem);
    if (pItem) pItem->m_visualState.ApplyVisualStateB();
    if (m_pAnimationBase) m_pAnimationBase->ApplyVisualStateB();
}

// FUNCTION: SWKOTOR 0x00613710
// NONMATCH: nClass/nWield register and stack assignment, branch placement (62 insns).
void CSWCCreature::UpdateWeaponClass()
{
    unsigned long oidRight = m_oidRightWeapon;
    unsigned long oidLeft = m_oidLeftWeapon;
    unsigned char nClass = 0;
    unsigned char nWield = 0;
    if (oidRight == 0x7f000000) {
        if (oidLeft == oidRight) {
            nClass = 8;
            GetAnimationBase()->m_nWieldType = 2;
        } else if (g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(oidLeft)) {
            nClass = 0;
        }
    } else {
        CSWCItem* pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(oidRight);
        if (pItem) {
            int nBaseItem = pItem->m_nBaseItem;
            CSWBaseItemArray* pBaseItems = g_pRules->m_pBaseItems;
            CSWBaseItem* pBase = pBaseItems->GetBaseItem(nBaseItem);
            int bSetWield = 1;
            if (oidLeft == 0x7f000000) {
                switch (pBase->m_nWeaponClass) {
                case 1: nClass = 1; break;
                case 2: nClass = 2; break;
                case 3: nClass = 3; break;
                case 4: nClass = 5; break;
                case 5: nClass = 7; break;
                case 6: nClass = 9; break;
                }
            } else {
                pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(oidLeft);
                if (pItem) {
                    nBaseItem = pItem->m_nBaseItem;
                    pBaseItems = g_pRules->m_pBaseItems;
                    pBase = pBaseItems->GetBaseItem(nBaseItem);
                    if (pBase->m_nWeaponClass == 2) nClass = 4;
                    else if (pBase->m_nWeaponClass == 4) nClass = 6;
                } else bSetWield = 0;
            }
            if (bSetWield) {
                nWield = pBase->m_nWieldType;
                GetAnimationBase()->m_nWieldType = nWield;
            }
        }
    }
    GetAnimationBase()->m_nWeaponClass = nClass;
}

// FUNCTION: SWKOTOR 0x00611470
// NONMATCH: query loads, retained zero register and outgoing argument scheduling (29 insns).
void CSWCCreature::PlayVoiceChat(unsigned short nIndex, int bSpatial)
{
    STR_RES resource;
    if (!GetSoundSetStrres(nIndex, &resource)) return;
    if (GetServerObject() && GetServerObject()->AsSWSCreature()) {
        if (GetServerCreature()->GetIsDead() || (GetServerObject() ? GetServerObject()->AsSWSCreature() : 0)->GetDeadTemp()) {
            if (nIndex != 0x10) return;
        }
    }
    unsigned char nPriority;
    if (g_pAppManager->m_pClientExoApp->GetPlayerCreature() == this) nPriority = 8;
    else if (g_pAppManager->m_pClientExoApp->GetPlayerByGameObjectID(m_idSelf)) nPriority = 9;
    else {
        int nSize;
        g_pRules->m_p2DArrays->m_pAppearanceTable->GetINTEntry(
            m_pAppearance->m_nAppearanceType, g_nAppearanceCol_SIZECATEGORY, &nSize);
        nPriority = nSize <= 3 ? 0x11 : 0x10;
    }
    m_nVoiceState = 0;
    if (m_pVoiceSound) {
        delete m_pVoiceSound;
        m_pVoiceSound = 0;
    }
    m_pVoiceSound = new CExoStreamingSoundSource;
    if (m_pVoiceSound) {
        m_pVoiceSound->Set3D(bSpatial);
        m_pVoiceSound->SetPriorityGroup(nPriority);
        if (bSpatial) m_pVoiceSound->SetPosition(m_vPosition);
        if (!m_pVoiceSound->Play(resource.m_sSound, 4, 0, 0, 0)) {
            delete m_pVoiceSound;
            m_pVoiceSound = 0;
        }
    }
}
