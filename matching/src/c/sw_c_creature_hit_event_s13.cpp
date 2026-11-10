// "hit" animation-event callback (registered by RegisterAnimationCallbacks),
// worker C s13; missing from functions.csv. Translation-unit-local layout.

#include "c/exo_base.h"
#include <stdlib.h>

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

class Quaternion {
public:
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
};

struct CTwoDimArrays {
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCCombatAttackData {
public:
    char pad_0x0[0x4];
    unsigned long m_oidAttacker;                    // 0x04
    char pad_0x8[0xc - 0x8];
    unsigned char m_nAttackResult;                  // 0x0c
    char pad_0xd[0x14 - 0xd];
    unsigned long m_oidTarget;                      // 0x14
    char pad_0x18[0x1e - 0x18];
    unsigned short m_nDamage;                       // 0x1e (descriptive)
    int m_bHandled;                                 // 0x20 (descriptive)
    int m_bNoVoice;                                 // 0x24 (descriptive)
    char pad_0x28[0x40 - 0x28];
    int m_bAbortTarget;                             // 0x40 (descriptive)
};

class CAurModel {
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
    virtual int GetHookTransform(const char* sHook, Vector* pPosition, Quaternion* pOrientation);  // 0x98 (descriptive)
};

class CAurAnimationBase {
public:
    virtual void v00(); virtual void v04();
    virtual CAurModel* GetModel(unsigned char n);   // 0x08
    virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void PlayOverlayAnimation(unsigned short nAnim, int bLoop);   // 0x44 (descriptive)
};

class CSWSCreature {
public:
    int IsDebilitated();                            // 0x005B4880
};

class CSWCArea {
public:
    int ApplyFireAndForgetVisualEffect(unsigned short nEffect, Vector vPosition);  // 0x00604C90
};

class CSWCCreature;
class CSWCDoor;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
};

struct CSWCCreatureSoundSet {
    char pad_0x0[0x4c];
    short m_nVoiceChance;                           // 0x4c (descriptive)
};

class CSWCCreatureAppearance {
public:
    char pad_0x0[0x18];
    unsigned short m_nAppearanceType;               // 0x18
};

class CSWCObject : public CGameObject {
public:
    unsigned short GetCurrentAnimation();           // 0x0063C060
    CAurAnimationBase* GetAnimationBase();          // 0x0063C070
    int AnimationParry(unsigned short nAnim);       // 0x0063C4F0
    void AbortFireForgetAnimation(int b);           // 0x0063CAD0

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x20 - 0x8];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
};

class CSWCCreature : public CSWCObject {
public:
    int InCombat();                                 // 0x0060D320
    int SpawnHitVisuals(unsigned char bHit, CSWCCombatAttackData* pData, Vector& vDirection,
                        CExoString& sHook);         // 0x0060D730
    CSWSCreature* AsServerCreature();               // 0x0060FB20
    void PlayVoiceChat(unsigned short nIndex, int bSpatial);    // 0x00611470
    void BlurWeapon(int b);                         // 0x006116A0
    void PlayHitSound(CSWCCombatAttackData* pData, int b);     // 0x00617470
    unsigned short GetAppearanceType() { return m_pAppearance->m_nAppearanceType; }

    char pad_0x30[0x218 - 0x30];
    unsigned char m_nCombatAnimState;               // 0x218 (descriptive)
    char pad_0x219[0x21c - 0x219];
    CSWCCreatureAppearance* m_pAppearance;          // 0x21c
    char pad_0x220[0x2f8 - 0x220];
    CSWCCreatureSoundSet* m_pSoundSet;              // 0x2f8 (descriptive)
};

class CSWParty {
public:
    CSWCCreature* GetPlayerCharacter();             // 0x00635460
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWParty* GetParty();                           // 0x005ED8B0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

// FUNCTION: SWKOTOR 0x00617EB0
// NONMATCH (13 diff insns, 1 byte): scheduling only - the appearance-table
// pointer chain before the ABORTONPARRY arguments, ecx=this before the
// overlay-animation pushes, and eax (not ecx) for g_pAppManager in GetParty.
// The original dereferences the attacker without a null check when pData is 0.
void HitEvent(void*, char*, CSWCCombatAttackData* pData)
{
    CSWCObject* pAttackerObject = 0;
    CSWCObject* pTarget = 0;
    if (pData) {
        pTarget = (CSWCObject*)g_pAppManager->GetClientExoApp()->GetGameObject(pData->m_oidTarget);
        pAttackerObject = (CSWCObject*)g_pAppManager->GetClientExoApp()->GetGameObject(pData->m_oidAttacker);
        if (!pAttackerObject || !pTarget) {
            return;
        }
    }
    if (!pAttackerObject->AsSWCCreature()) {
        return;
    }
    CSWCCreature* pAttacker = pAttackerObject->AsSWCCreature();
    if (pData->m_bHandled) {
        return;
    }
    unsigned char nResult = pData->m_nAttackResult;
    if (nResult == 1 || nResult == 2 || nResult == 3 || nResult == 5) {
        pAttacker->PlayHitSound(pData, 0);
        if (pTarget->AsSWCCreature()) {
            CSWCCreature* pCreature = pTarget->AsSWCCreature();
            if (pCreature->InCombat()) {
                CSWSCreature* pServer = pCreature->AsServerCreature();
                if (!pServer || !pServer->IsDebilitated()) {
                    if (pData->m_nDamage != 0 || pCreature->m_nCombatAnimState != 1) {
                        pTarget->GetAnimationBase()->PlayOverlayAnimation(0x283e, 1);
                    }
                }
            }
        }
        if (pTarget->AsSWCCreature() && pTarget->AsSWCCreature()->m_pSoundSet->m_nVoiceChance > 0 &&
            !pData->m_bNoVoice) {
            CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetParty()->GetPlayerCharacter();
            unsigned long oid = pTarget->m_idSelf;
            if ((pPlayer && oid == pPlayer->m_idSelf) || rand() % 5 == 0) {
                unsigned short nVoice = rand() % 2 ? 13 : 14;
                pTarget->AsSWCCreature()->PlayVoiceChat(nVoice, 1);
            }
        }
    } else if ((nResult == 8 && pTarget->AnimationParry(pTarget->GetCurrentAnimation())) ||
               (pData->m_nAttackResult == 4 && pTarget->AnimationParry(pTarget->GetCurrentAnimation()))) {
        pAttacker->PlayHitSound(pData, 0);
        int bAbort;
        {
            CExoString sColumn("ABORTONPARRY");
            unsigned short nRow = pAttacker->GetAppearanceType();
            C2DA* pTable = g_pRules->m_p2DArrays->m_pAppearanceTable;
            pTable->GetINTEntry(nRow, sColumn, &bAbort);
        }
        if (bAbort) {
            pAttacker->BlurWeapon(0);
            pAttacker->AbortFireForgetAnimation(0);
        }
        if (pData->m_bAbortTarget) {
            pTarget->AbortFireForgetAnimation(0);
        }
    }
    nResult = pData->m_nAttackResult;
    if (nResult == 1 || nResult == 2 || nResult == 3 || nResult == 5) {
        if (pTarget->AsSWCCreature()) {
            CExoString sHook("impact");
            Vector vDirection = pAttacker->m_vPosition - pTarget->m_vPosition;
            pTarget->AsSWCCreature()->SpawnHitVisuals(1, pData, vDirection, sHook);
        } else if (pTarget->AsSWCDoor() && pTarget->m_pArea) {
            Vector vPosition;
            Quaternion qOrientation;
            pTarget->GetAnimationBase()->GetModel(0xff)->GetHookTransform("lookathook", &vPosition, &qOrientation);
            pTarget->m_pArea->ApplyFireAndForgetVisualEffect(0xfa3, vPosition);
            pTarget->m_pArea->ApplyFireAndForgetVisualEffect(0xfab, vPosition);
        }
    }
}
