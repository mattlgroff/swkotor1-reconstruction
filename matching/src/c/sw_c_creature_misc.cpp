// CSWCCreature members (b0061_client_area_creature_misc): camera reset,
// perception updates, lightsaber / stealth visuals, weapon visibility and
// creature light lookup; plus ComputeZValue, SetItemUnEquippedByMe,
// PerformUseItem and SetDesiredOrientationForLock (b00604).

#include "c/exo_base.h"

#define OBJECT_INVALID 0x7f000000

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

Vector NormalizeVector(const Vector& v);            // 0x004AB180

extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

struct Quaternion {
    float w, x, y, z;
};

class CSWCCreature;
class CSWSCreature;
class CSWCPlaceable;
class CSWCItem;
class CGob;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual int AsSWCDoor();                        // 0x14
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual CSWCItem* AsSWCItem();                  // 0x34
    virtual void v38();
    virtual int AsSWCTrigger();                     // 0x3c
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84();
    virtual void SetOrientation(const Vector& v);   // 0x88
    virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
    virtual void v9C();
    virtual void PlayAnimation(int nAnim, int b);   // 0xa0
    virtual void vA4(); virtual void vA8(); virtual void vAC(); virtual void vB0();
    virtual void ApplyVisualEffect(unsigned short nEffect, int a, unsigned long oidTarget,
                                   unsigned long oidSource, unsigned char nNode, int b,
                                   const Vector& vOffset);  // 0xb4
    virtual void RemoveVisualEffect(unsigned short nEffect);  // 0xb8
    virtual void vBC(); virtual void vC0(); virtual void vC4(); virtual void vC8();
    virtual void vCC(); virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void vDC(); virtual void vE0(); virtual void vE4(); virtual void vE8();
    virtual void vEC(); virtual void vF0(); virtual void vF4(); virtual void vF8();
    virtual void vFC(); virtual void v100(); virtual void v104(); virtual void v108();
    virtual void v10C(); virtual void v110(); virtual void v114(); virtual void v118();
    virtual void v11C(); virtual void v120(); virtual void v124(); virtual void v128();
    virtual void v12C(); virtual void v130(); virtual void v134();
    virtual int IsStealthed();                      // 0x138

    unsigned long m_idSelf;                         // 0x04
};

class CSWCArea;

class CSWCObject : public CGameObject {
public:
    char pad_0x8[0x18];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

class CSWCMessage {
public:
    int SendPlayerToServerInput_UseItem(unsigned long a, int b, int c, int d, int e);  // 0x00677BD0
};

class CSWSAreaHeight;

class CServerExoApp {
public:
    float ComputeHeight(void* pArea, Vector* pPosition, int b);   // 0x004AEA50
};

class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CSWCItem* GetItem();                    // 0x70
    virtual void v74();
    virtual int GetObjectType();                    // 0x78
    virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual int GetNodePosition(const char* sNode, Vector* pPosition,
                                Quaternion* pOrientation);  // 0x98
};

class CAnimationBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual int HasAnimation(unsigned short nAnim); // 0x50
};

class CSWCCamera {
public:
    void SetTargetFollow();                         // 0x006383B0
};

class CSceneHolder {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual CSWCCamera* GetCamera(unsigned long nId);   // 0x80
};

class CSWCModule {
public:
    void RestoreCamera();                           // 0x00641DC0
    CSceneHolder* GetScene() { return m_pScene; }

    char pad_0x0[0x40];
    CSceneHolder* m_pScene;                         // 0x40
};

class CSWParty {
public:
    int IsPartyMember(unsigned long oid);           // 0x006349C0
};

struct CClientOptions {
    char pad_0x0[0x30];
    struct CAurRenderer* m_pRenderer;               // 0x30
    char pad_0x34[0x39];
    unsigned char m_nCameraMode;                    // 0x6d
};

struct CAurRenderer {
    unsigned char CheckFrameBufferModificationsAvailable();  // 0x0045FAB0
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
    CGameObject* GetItemByGameObjectID(unsigned long oid);  // 0x005ED5A0
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);  // 0x005ED5B0
    CClientOptions* GetClientOptions();             // 0x005ED700
    CSWParty* GetSWParty();                         // 0x005ED8B0
    void SetInputClass(int a, int b);               // 0x005EDA60
    int IsCombatSlowdown();                         // 0x005EE0F0
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
extern int g_bCameraResetPending;                   // 0x007A2288

class CSWBaseItem {
public:
    char pad_0x0[0xac];
    unsigned char m_nWeaponType;                    // 0xac
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItems;                 // 0x34
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCItem : public CGameObject {
public:
    void SetEquippedByMe(int b);                    // 0x00646600

    CSWBaseItem* GetBaseItem();                     // 0x005B4790
    void SetPowered(int a, int b, int c, int d);    // 0x00647130

    char pad_0x8[0x4];
    int m_nBaseItem;                                // 0x0c
    char pad_0x10[0x188];
    int m_bPowered;                                 // 0x198
};

class CSWSPlaceable {
public:
    char pad_0x0[0x324];
    int m_bUseable;                                 // 0x324
};

class CSWCPlaceable : public CGameObject {
public:
    CSWSPlaceable* GetServerPlaceable();            // 0x00682E00
};

class CSWSArea {
public:
    char pad_0x0[0x2b0];
    int m_bStealthDisabled;                         // 0x2b0
};

class CSWSCreature {
public:
    CSWSArea* GetArea();                            // 0x004CB120
};

class CSWCArea {
public:
    void ApplyFireAndForgetVisualEffect(int nEffect, Vector vPosition);   // 0x00604C90
    int IsPositionVisible(Vector* pPosition);      // 0x006054E0
};

class CSWCCreatureStats {
public:
    int GetCanUseSkill(unsigned char nSkill);       // 0x006477E0
};

class CGameEffect {
public:
    unsigned char GetHardCodedEffectClass();        // 0x006A0920

    char pad_0x0[0x8c];
    unsigned long m_nLightColor;                    // 0x8c
    char pad_0x90[0x30];
    int m_bExpired;                                 // 0xc0
    char pad_0xc4[0x10];
    int m_bDynamic;                                 // 0xd4
    char pad_0xd8[0x8];
    int m_bActive;                                  // 0xe0
};

struct CPerceptionRecord {
    unsigned long m_oidCreature;                    // 0x00
    unsigned char m_bSeen : 1;                      // 0x04 bit 0
    unsigned char m_bHeard : 1;                     // bit 1
    unsigned char m_bBit2 : 1;
    unsigned char m_bBit3 : 1;
    unsigned char m_bVanished : 1;                  // bit 4
};

class CSWCCreature : public CSWCObject {
public:
    void ResetCameraToFollow();
    void UpdateRelativePerception(CPerceptionRecord* pRecord);
    void UpdateRelativePerception(unsigned long oid);
    int ShowLightSaberContactVisual(int nUnused);
    void SetOverrideLightsaberState(int bState, int a, int b);
    char GetTargetInterfaceTargetType(CGameObject* pTarget);
    int IsStealthCapable();
    void BlurWeapon(int bBlur);
    void UpdateVisibleWeapons(unsigned long nSlot, CSWCItem* pItem, int bFlag);
    void SetStealthState(int nState, int bQuiet);
    unsigned long GetCreatureLight(int bDynamic);
    void ComputeZValue(char* pArea, Vector* pPosition, int a, int b);
    void SetItemUnEquippedByMe(unsigned long oid);
    int PerformUseItem(unsigned long a, long b, int c, int d);
    void SetDesiredOrientationForLock();
    void SetDesiredOrientation(const Vector& v);    // 0x0060B3C0

    // callees
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    CGameObject* GetServerObject();                 // 0x0063D4B0
    CAnimationBase* GetAnimationBase();             // 0x0063C070
    void PlayDrawAnimation(unsigned short nAnim);   // 0x00610A70
    void PlaySoundSetEntry(int nEntry, int b);      // 0x00611470
    static void __cdecl SwitchWeaponEvent(int a, int b, CSWCCreature* p);  // 0x00610F40

    CSWSCreature* GetServerCreature()
    {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }

    char pad_0x3c[0x30];
    CExoLinkedList<CGameEffect> m_lstEffects;       // 0x6c
    char pad_0x70[0x20];
    unsigned char m_bDrawWeapon;                    // 0x90
    char pad_0x91[0xbb];
    int m_bAnimationBusy;                           // 0x14c
    char pad_0x150[0x44];
    int m_nStealthState;                            // 0x194
    char pad_0x198[0x18];
    int m_bPerceptionEffect;                        // 0x1b0
    char pad_0x1b4[0x94];
    unsigned long m_oidEquipped248;                 // 0x248
    char pad_0x24c[0xc];
    unsigned long m_oidEquipped258;                 // 0x258
    char pad_0x25c[0x70];
    int m_bBlurWeapon;                              // 0x2cc
    char pad_0x2d0[0x24];
    unsigned long m_oidLockTarget;                  // 0x2f4
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    char pad_0x2fc[0x64];
    CExoArrayList<CPerceptionRecord*> m_lstPerception;  // 0x360
    char pad_0x36c[0x24];
    int m_n390;                                     // 0x390
    char pad_0x394[0x4];
    int m_n398;                                     // 0x398
    char pad_0x39c[0x3c];
    int m_bOverrideLightsaber;                      // 0x3d8
};

extern char g_sLightsaberContactNode[];             // 0x0074F368

// FUNCTION: SWKOTOR 0x0060B920
void CSWCCreature::ResetCameraToFollow()
{
    if (g_pAppManager->m_pClientExoApp->GetSWParty()->IsPartyMember(m_idSelf)) {
        return;
    }
    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode == 5) {
        g_bCameraResetPending = 1;
        g_pAppManager->m_pClientExoApp->GetModule()->RestoreCamera();
        g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
    }
    CSWCCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModule()->GetScene()->GetCamera(0x106a);
    if (pCamera) {
        pCamera->SetTargetFollow();
    }
}

// FUNCTION: SWKOTOR 0x0060C980 SYMBOL: ?UpdateRelativePerception@CSWCCreature@@QAEXPAUCPerceptionRecord@@@Z
// NONMATCH: this/bAdd land in EBX/EBP swapped (7 insns).
void CSWCCreature::UpdateRelativePerception(CPerceptionRecord* pRecord)
{
    int bAdd = 0;
    int bRemove = 0;
    CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(pRecord->m_oidCreature);
    if (pCreature) {
        if ((pRecord->m_bSeen == 0 && pRecord->m_bHeard) || pRecord->m_bVanished == 1) {
            bAdd = 1;
        } else if (pRecord->m_bSeen == 1 && pRecord->m_bVanished == 0) {
            bRemove = 1;
        }
        if (m_pArea->IsPositionVisible(&pCreature->m_vPosition) == 1) {
            if (bAdd == 1 && !pCreature->m_bPerceptionEffect) {
                pCreature->ApplyVisualEffect(0x7d1, 0, OBJECT_INVALID, OBJECT_INVALID, 0, 0, Vector(0.0f, 0.0f, 0.0f));
                pCreature->m_bPerceptionEffect = 1;
            } else if (bRemove == 1 && pCreature->m_bPerceptionEffect == 1) {
                pCreature->RemoveVisualEffect(0x7d1);
                pCreature->m_bPerceptionEffect = 0;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0060DE20
int CSWCCreature::ShowLightSaberContactVisual(int nUnused)
{
    Vector vPosition(0.0f, 0.0f, 0.0f);
    Quaternion qOrientation;
    qOrientation.z = 0.0f;
    qOrientation.y = 0.0f;
    qOrientation.x = 0.0f;
    qOrientation.w = 1.0f;
    if (!FindPart(0xff, 1)->GetNodePosition(g_sLightsaberContactNode, &vPosition, &qOrientation)) {
        return 0;
    }
    m_pArea->ApplyFireAndForgetVisualEffect(0xfab, vPosition);
    return 1;
}

// FUNCTION: SWKOTOR 0x0060FC00
// NONMATCH: the original returns int-sized constants on every path except
// the last (`mov al, bl` from EBX = -1 with EAX == 0); not reproduced.
char CSWCCreature::GetTargetInterfaceTargetType(CGameObject* pTarget)
{
    char nType = -1;
    if (!pTarget) {
        return nType;
    }
    if (pTarget->AsSWCCreature()) {
        if (pTarget->AsSWCCreature()->IsStealthed() || g_pAppManager->m_pClientExoApp->IsCombatSlowdown()) {
            nType = 3;
        } else {
            nType = 2;
        }
    } else if (pTarget->AsSWCDoor()) {
        nType = 0;
    } else if (pTarget->AsSWCPlaceable()) {
        nType = 2;
        CSWCPlaceable* pPlaceable = pTarget->AsSWCPlaceable();
        if (pPlaceable) {
            CSWSPlaceable* pServer = pPlaceable->GetServerPlaceable();
            if (pServer && pServer->m_bUseable) {
                nType = 0;
            }
        }
    } else if (pTarget->AsSWCTrigger()) {
        nType = 1;
    }
    return nType;
}

// FUNCTION: SWKOTOR 0x00610AC0
// NONMATCH: second item lookup loads the oid into EDI (this is dead) and the
// tail keeps `test; je; mov eax, 1` where we get setne.
int CSWCCreature::IsStealthCapable()
{
    if (!GetServerObject() || !GetServerObject()->AsSWSCreature()) {
        return 0;
    }
    if (GetServerCreature()->GetArea()->m_bStealthDisabled) {
        return 0;
    }
    if (!m_pStats->GetCanUseSkill(2)) {
        return 0;
    }
    int bCapable = 0;
    CSWCItem* pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidEquipped248);
    if (pItem && pItem->GetBaseItem()) {
        bCapable = pItem->GetBaseItem()->m_nWeaponType == 0x2c;
    }
    pItem = (CSWCItem*)g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(m_oidEquipped258);
    if (pItem && pItem->GetBaseItem()) {
        bCapable |= pItem->GetBaseItem()->m_nWeaponType == 0x2c;
    }
    if (!bCapable) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00613850
// NONMATCH: the original moves the nSlot == 0x10 block after the epilogue.
void CSWCCreature::UpdateVisibleWeapons(unsigned long nSlot, CSWCItem* pItem, int bFlag)
{
    unsigned long nPart = 0;
    if (nSlot != 0x10) {
        if (nSlot == 0x20) {
            nPart = 0xfb;
        }
    } else {
        nPart = 0xf7;
    }
    if (nPart && !bFlag) {
        if (GetAnimationBase()->HasAnimation(0x2806) == 1) {
            int bBusy = m_bAnimationBusy;
            CSWCItem* pCurrent = 0;
            CGob* pModel = FindPart(nPart, 1);
            if (pModel && pModel->GetObjectType() == 6) {
                pCurrent = pModel->GetItem();
            }
            if (pItem != pCurrent && !bBusy) {
                GetAnimationBase();
                m_n390 = 0;
                m_n398 = 0;
                if (m_bDrawWeapon) {
                    PlayDrawAnimation(0x2806);
                }
                if (pCurrent) {
                    pCurrent->SetPowered(0, 0, 1, 0);
                }
            }
        }
    }
    SwitchWeaponEvent(0, 0, this);
}

// FUNCTION: SWKOTOR 0x00613930
void CSWCCreature::SetStealthState(int nState, int bQuiet)
{
    if (nState == 1) {
        if (!m_nStealthState) {
            PlaySoundSetEntry(0x15, nState);
            CClientOptions* pOptions = g_pAppManager->m_pClientExoApp->GetClientOptions();
            if (pOptions->m_pRenderer && pOptions->m_pRenderer->CheckFrameBufferModificationsAvailable()) {
                ApplyVisualEffect(0x1f40, 0, OBJECT_INVALID, OBJECT_INVALID, 0, 0, Vector(0.0f, 0.0f, 0.0f));
            } else {
                ApplyVisualEffect(0x1f42, 0, OBJECT_INVALID, OBJECT_INVALID, 0, 0, Vector(0.0f, 0.0f, 0.0f));
            }
            if (!bQuiet) {
                PlayAnimation(0x2798, 0);
            }
        }
    } else if (nState == 0 && m_nStealthState == 1) {
        CClientOptions* pOptions = g_pAppManager->m_pClientExoApp->GetClientOptions();
        if (pOptions->m_pRenderer && pOptions->m_pRenderer->CheckFrameBufferModificationsAvailable()) {
            RemoveVisualEffect(0x1f40);
        } else {
            RemoveVisualEffect(0x1f42);
        }
        ApplyVisualEffect(0x1f41, 1, OBJECT_INVALID, OBJECT_INVALID, 0, 0, Vector(0.0f, 0.0f, 0.0f));
        if (!bQuiet) {
            PlayAnimation(0x2798, 0);
        }
    }
    m_nStealthState = nState;
}

// FUNCTION: SWKOTOR 0x00613A70 SYMBOL: ?UpdateRelativePerception@CSWCCreature@@QAEXK@Z
// NONMATCH: register choice around the IsPositionVisible / ApplyVisualEffect
// calls (7 insns); the record variant 0x0060C980 swaps EBX/EBP.
void CSWCCreature::UpdateRelativePerception(unsigned long oid)
{
    int bAdd = 0;
    int bRemove = 0;
    for (int i = 0; i < m_lstPerception.num; i++) {
        CPerceptionRecord* pRecord = m_lstPerception.element[i];
        if (pRecord->m_oidCreature == oid) {
            CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(oid);
            if (!pCreature) {
                return;
            }
            if ((pRecord->m_bSeen == 0 && pRecord->m_bHeard) || pRecord->m_bVanished == 1) {
                bAdd = 1;
            } else if (pRecord->m_bSeen == 1 && pRecord->m_bVanished == 0) {
                bRemove = 1;
            }
            if (m_pArea->IsPositionVisible(&pCreature->m_vPosition) != 1) {
                return;
            }
            if (bAdd == 1 && !pCreature->m_bPerceptionEffect) {
                pCreature->ApplyVisualEffect(0x7d1, 0, OBJECT_INVALID, OBJECT_INVALID, 0, 0, Vector(0.0f, 0.0f, 0.0f));
                pCreature->m_bPerceptionEffect = 1;
                return;
            }
            if (bRemove == 1 && pCreature->m_bPerceptionEffect == 1) {
                pCreature->RemoveVisualEffect(0x7d1);
                pCreature->m_bPerceptionEffect = 0;
            }
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x00614AA0
// NONMATCH: the original builds the head object in EAX and copies it to ESI.
unsigned long CSWCCreature::GetCreatureLight(int bDynamic)
{
    CExoLinkedListPosition pos = m_lstEffects.GetHeadPos();
    CGameEffect* pEffect = m_lstEffects.GetHead();
    while (pos) {
        if (pEffect && pEffect->GetHardCodedEffectClass() == 4 && !pEffect->m_bExpired &&
            pEffect->m_bActive && (!bDynamic || pEffect->m_bDynamic)) {
            return pEffect->m_nLightColor;
        }
        pEffect = m_lstEffects.GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0060BF20
void CSWCCreature::ComputeZValue(char* pArea, Vector* pPosition, int a, int b)
{
    if (g_pAppManager->m_pServerExoApp) {
        pPosition->z = 0.0f;
        pPosition->z = g_pAppManager->GetServerExoApp()->ComputeHeight(pArea + 0x10c, pPosition, 1);
    }
}

// FUNCTION: SWKOTOR 0x0060D020
void CSWCCreature::SetItemUnEquippedByMe(unsigned long oid)
{
    if (oid == OBJECT_INVALID_ID) {
        return;
    }
    CGameObject* pObject;
    CGameObjectArray* pArray = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
    if (pArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
        CSWCItem* pItem = pObject->AsSWCItem();
        pItem->SetEquippedByMe(0);
    }
}

// FUNCTION: SWKOTOR 0x0060EE60
int CSWCCreature::PerformUseItem(unsigned long a, long b, int c, int d)
{
    CSWCMessage* pMessage = g_pAppManager->GetClientExoApp()->GetSWCMessage();
    int nResult = (int)pMessage;
    if (pMessage) {
        nResult = pMessage->SendPlayerToServerInput_UseItem(a, b, b >> 16, c, d);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x0060BA10
// NONMATCH: GetGameObject argument registers swapped and the squared length
// is summed z-first (the original keeps z on the FPU stack and adds it last).
void CSWCCreature::SetDesiredOrientationForLock()
{
    CGameObjectArray* pArray = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
    if (!pArray) {
        return;
    }
    CGameObject* pTarget;
    if (pArray->GetGameObject(m_oidLockTarget, &pTarget) == GAME_OBJECT_VALID) {
        if (pTarget) {
            Vector vDiff;
            vDiff.x = ((CSWCObject*)pTarget)->m_vPosition.x - m_vPosition.x;
            vDiff.y = ((CSWCObject*)pTarget)->m_vPosition.y - m_vPosition.y;
            vDiff.z = ((CSWCObject*)pTarget)->m_vPosition.z - m_vPosition.z;
            if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z > 0.00001f) {
                vDiff.z = 0.0f;
                SetDesiredOrientation(NormalizeVector(vDiff));
            }
        }
    } else {
        SetOrientation(m_vOrientation);
    }
}
