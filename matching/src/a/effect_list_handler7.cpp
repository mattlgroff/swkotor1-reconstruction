// CSWSEffectListHandler handlers registered by InitializeEffects (0x004E4A10)
// that had no source yet: area of effect, ...
#include <stdlib.h>
#include "a/effect_list_handler.h"

class CSWSRoom;
class CSWSCreatureFollowInfo;

class CSWSArea {
public:
    float ComputeHeight(const Vector& vPosition, int bArg2, CSWSRoom** ppRoom);  // 0x004BC380
    int ComputeSafeLocation(const Vector& vPosition, float fRange, CSWSCreatureFollowInfo* pInfo,
                            int bArg4, Vector* pResult);  // 0x004BE860
};

class CSWSPlaceable {
public:
    void AddToArea(CSWSArea* pArea, const Vector& vPosition, int bRunScripts);  // 0x00587170
};

class CServerExoApp7 {
public:
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);  // 0x004AE780
};

class CSWSObject7 : public CSWSObject {
public:
    CSWSArea* GetArea();                        // 0x004CB120
    Vector GetPosition() { return m_vPosition; }
};

// size 0x290, vtable 0x00749EA8 (only the members used here)
class CSWSAreaOfEffectObject {
public:
    CSWSAreaOfEffectObject(unsigned long oidSelf);  // 0x00594480
    void SetCreator(unsigned long oid);         // 0x00594FF0
    void LoadAreaEffect(int nRow);              // 0x005947B0
    void SetTargetObjID(unsigned long oid);     // 0x005955A0
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts);  // 0x00595F30

    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4(); virtual void vb8();
    virtual void SetEffectSpellId(unsigned long nSpellId);  // 0xbc (0x005945E0)

    unsigned long m_idSelf;                     // 0x04
    char pad_0x8[0x260 - 0x8];
    CExoString m_sScriptOnExit;                 // 0x260
    char pad_0x268[0x270 - 0x268];
    CExoString m_sScriptOnEnter;                // 0x270
    CExoString m_sScriptHeartbeat;              // 0x278
    char pad_0x280[0x290 - 0x280];
};

// FUNCTION: SWKOTOR 0x004DADE0
int CSWSEffectListHandler::OnApplyAreaOfEffect(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 1;
    }
    CSWSAreaOfEffectObject* pAoE = new CSWSAreaOfEffectObject(0x7f000000);
    pAoE->SetCreator(pEffect->GetCreator());
    pAoE->LoadAreaEffect((unsigned char)pEffect->GetInteger(0));
    if (pEffect->GetString(0) != "") {
        pAoE->m_sScriptOnEnter = pEffect->GetString(0);
    }
    if (pEffect->GetString(1) != "") {
        pAoE->m_sScriptOnExit = pEffect->GetString(1);
    }
    if (pEffect->GetString(2) != "") {
        pAoE->m_sScriptHeartbeat = pEffect->GetString(2);
    }
    pAoE->SetEffectSpellId(pEffect->GetSpellId());
    CSWSArea* pArea = ((CSWSObject7*)pCreature)->GetArea();
    pAoE->SetTargetObjID(pCreature->m_idSelf);
    Vector vPosition = ((CSWSObject7*)pCreature)->GetPosition();
    pAoE->AddToArea(pArea, vPosition.x, vPosition.y, vPosition.z, 1);
    pEffect->SetObjectID(0, pAoE->m_idSelf);
    return 0;
}

// FUNCTION: SWKOTOR 0x004DA090
int CSWSEffectListHandler::OnApplyDeaf(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    if (!pObject->AsSWSCreature()) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    CSWSCreature* pCreator = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
    if (pCreature->GetStats()->GetEffectImmunity(8, pCreator)) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetObjectID(0, pObject->GetId());
        if (pCreator) {
            CSWCCMessageData* pCopy = new CSWCCMessageData;
            pData->CopyTo(pCopy);
            pCreator->SendFeedbackData(0x8c, pCopy);
        }
        pObject->AsSWSCreature()->SendFeedbackData(0x8c, pData);
        return 1;
    }
    if (pCreature->m_bF8) {
        return 1;
    }
    CGameEffect* pAttack = new CGameEffect(pEffect);
    CGameEffect* pSkill = new CGameEffect(pEffect);
    pAttack->m_nType = 0x17;
    pAttack->SetInteger(0, -0x11);
    pCreature->ApplyEffect(pAttack, bLoadingGame, 0);
    pSkill->m_nType = 0x19;
    pSkill->SetInteger(0, 0x14);
    pCreature->ApplyEffect(pSkill, bLoadingGame, 0);
    return 0;
}

class CSWSCreature7 : public CSWSCreature {
public:
    void ClearAllActions(int bArg);             // 0x004CCD80
    CSWSCreatureFollowInfo* GetPathfindInfo() { return m_pFollowInfo; }
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg5, int bArg6);  // 0x004FA100
};

// FUNCTION: SWKOTOR 0x004D9720
// NONMATCH (25): creature and creator swap edi/ebx (pure register swap).
int CSWSEffectListHandler::OnApplyEntangled(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    if (!pObject->AsSWSCreature()) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    CSWSCreature* pCreator = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
    if (pCreature->GetStats()->GetEffectImmunity(0xa, pCreator)) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetObjectID(0, pObject->GetId());
        if (pCreator) {
            CSWCCMessageData* pCopy = new CSWCCMessageData;
            pData->CopyTo(pCopy);
            pCreator->SendFeedbackData(0x90, pCopy);
        }
        pObject->AsSWSCreature()->SendFeedbackData(0x90, pData);
        return 1;
    }
    if (pCreature->GetF8()) {
        return 1;
    }
    ((CSWSCreature7*)pCreature)->ClearAllActions(1);
    CGameEffect* pHold = new CGameEffect(pEffect);
    CGameEffect* pSpeed = new CGameEffect(pEffect);
    CGameEffect* pAttack = new CGameEffect(pEffect);
    pHold->m_nType = 0x0b;
    pHold->SetInteger(0, 2);
    pHold->SetInteger(1, 0);
    pHold->SetInteger(2, g_pRules->m_nAA);
    pCreature->ApplyEffect(pHold, bLoadingGame, 0);
    pSpeed->m_nType = 0x25;
    pSpeed->SetInteger(0, 1);
    pSpeed->SetInteger(1, 4);
    pCreature->ApplyEffect(pSpeed, bLoadingGame, 0);
    pAttack->m_nType = 0x17;
    pAttack->SetInteger(0, -3);
    pCreature->ApplyEffect(pAttack, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004DD270
int CSWSEffectListHandler::OnRemoveDisappearAppear(CSWSObject* pObject, CGameEffect* pEffect)
{
    Vector vPosition(pEffect->GetFloat(0), pEffect->GetFloat(1), pEffect->GetFloat(2));
    Vector vSafe(0.0f, 0.0f, 0.0f);
    CSWSArea* pArea =
        ((CServerExoApp7*)g_pAppManager->m_pServerExoApp)->GetAreaByGameObjectID(pEffect->GetObjectID(0));
    if (pObject->m_nObjectType == g_nObjectTypeA) {
        CSWSCreature7* pCreature = (CSWSCreature7*)pObject->AsSWSCreature();
        pCreature->ClearAllActions(1);
        pCreature->m_pCombatRound->SetRoundPaused(0, 0x7f000000);
        pCreature->m_pCombatRound->SetPauseTimer(0x7d0, 0);
        pCreature->m_pCombatRound->SetRoundPaused(1, 0x7f000000);
        pCreature->ClearActions4ECFF0();
        if (pArea->ComputeSafeLocation(vPosition, 20.0f, pCreature->GetPathfindInfo(), 0, &vSafe)) {
            vSafe.z = pArea->ComputeHeight(vSafe, 0, NULL);
            pCreature->AddToArea(pArea, vSafe.x, vSafe.y, vSafe.z, 0, 0);
            return 1;
        }
        pEffect->SetFloat(2, pArea->ComputeHeight(vPosition, 0, NULL));
        pCreature->AddToArea(pArea, pEffect->GetFloat(0), pEffect->GetFloat(1), pEffect->GetFloat(2),
                             0, 0);
        return 1;
    }
    if (pObject->m_nObjectType == g_nObjectTypeB) {
        Vector vPlace(pEffect->GetFloat(0), pEffect->GetFloat(1), pEffect->GetFloat(2));
        pObject->AsSWSPlaceable()->AddToArea(pArea, vPlace, 0);
    }
    return 1;
}

class CScriptEvent {
public:
    CScriptEvent();                             // 0x004D7540
    unsigned short m_nType;                     // 0x00
    char pad_0x2[0x34 - 0x2];
};

extern const unsigned long EVENT_0x7441DC;      // 0x007441DC (placeholder name)

// CSWSModule: the CGameObject base sits at +0x1c (only the id is used here)
class CSWSModuleBase7 {
public:
    virtual ~CSWSModuleBase7();
    char pad_0x4[0x1c - 0x4];
};

class CSWSModule7 : public CSWSModuleBase7, public CGameObject {
public:
    char pad_0x24[0x1a0 - 0x24];
    unsigned char m_nMinutesPerHour;            // 0x1a0
};

class CServerExoAppModule7 {
public:
    CSWSModule7* GetModule();                   // 0x004AE6B0
};

class CSWSCreatureTHP : public CSWSCreature {
public:
    void AutoCloseGUIPanels(int bArg);          // 0x004EDED0
    void SetCurrentHitPoints(int nHP);          // 0x004CC280
    short GetTemporaryHitPoints() { return (short)m_nTemporaryHitPoints; }
    unsigned short GetAppearanceType() { return m_nAppearanceType; }
};

// FUNCTION: SWKOTOR 0x004D9320
int CSWSEffectListHandler::OnRemoveTemporaryHitpoints(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
    CSWSCreatureTHP* pCreature = (CSWSCreatureTHP*)pObject->AsSWSCreature();
    int bPossessed = pCreature->GetIsPossessed();
    int bDead = pCreature->GetIsInConversation();
    int bDeadTemp = pCreature->GetDeadTemp();
    if (pEffect) {
        pCreature->m_nTemporaryHitPoints =
            __max(pCreature->GetTemporaryHitPoints() - pEffect->GetInteger(0), 0);
    }
    if (!bPossessed && pCreature->GetIsPossessed() == 1) {
        pCreature->UpdateAmbientAnimationState();
        pCreature->m_nE8 = 0;
        unsigned long oidModule =
            ((CServerExoAppModule7*)GetServerExoApp())->GetModule()->GetId();
        CScriptEvent* pEvent = new CScriptEvent;
        pEvent->m_nType = 0x20;
        GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, pCreature->GetId(), oidModule, EVENT_0x7441DC, pEvent);
        pCreature->AutoCloseGUIPanels(0);
        return 1;
    }
    if ((!bDead && pCreature->GetIsInConversation() == 1) ||
        (!bDeadTemp && pCreature->GetDeadTemp() == 1)) {
        CGameEffect* pDeath = new CGameEffect(1);
        pDeath->m_nType = 0x13;
        pDeath->SetDurationType(g_nDurationInstant);
        pDeath->SetCreator(0x7f000000);
        CExoString sColor;
        {
            CExoString sColumn("BLOODCOLR");
            g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(
                pCreature->m_nAppearanceType, sColumn, &sColor);
        }
        CGameEffect* pBlood = new CGameEffect(1);
        pBlood->m_nType = 0x1e;
        pBlood->SetDurationType(g_nDurationInstant);
        if (sColor == "R") {
            pBlood->SetInteger(0, 0x9e);
        } else if (sColor == "G") {
            pBlood->SetInteger(0, 0x9f);
        } else if (sColor == "Y") {
            pBlood->SetInteger(0, 0xa0);
        }
        pCreature->ApplyEffect(pBlood, 0, 0);
        pDeath->SetInteger(1, 1);
        pCreature->ApplyEffect(pDeath, 0, 0);
    }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DDAC0
int CSWSEffectListHandler::OnRemoveWounding(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreatureTHP* pCreature = (CSWSCreatureTHP*)pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->GetIsPossessed();
        int bDead = pCreature->GetIsInConversation();
        int bDeadTemp = pCreature->GetDeadTemp();
        pCreature->SetCurrentHitPoints(pCreature->GetCurrentHitPoints(1) - 1);
        CExoString sColor;
        {
            CExoString sColumn("BLOODCOLR");
            g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(
                pCreature->m_nAppearanceType, sColumn, &sColor);
        }
        CGameEffect* pBlood = new CGameEffect(1);
        pBlood->m_nType = 0x1e;
        pBlood->SetDurationType(g_nDurationInstant);
        if (sColor == "R") {
            pBlood->SetInteger(0, 0x9e);
        } else if (sColor == "G") {
            pBlood->SetInteger(0, 0x9f);
        } else if (sColor == "Y") {
            pBlood->SetInteger(0, 0xa0);
        }
        pCreature->ApplyEffect(pBlood, 0, 0);
        if ((!bDead && pCreature->GetIsInConversation()) ||
            (!bDeadTemp && pCreature->GetDeadTemp())) {
            CGameEffect* pDeath = new CGameEffect(1);
            pDeath->m_nType = 0x13;
            pDeath->SetDurationType(g_nDurationInstant);
            pDeath->SetCreator(pEffect->GetCreator());
            pDeath->SetInteger(0, 0);
            pDeath->SetInteger(1, 1);
            pCreature->ApplyEffect(pDeath, 0, 0);
        } else {
            CGameEffect* pTick = new CGameEffect(1);
            pTick->SetCreator(pEffect->GetCreator());
            pTick->m_nType = 0x54;
            pTick->SetDurationType(g_nDurationTemporary);
            pTick->m_fDuration = 6.0f;
            pObject->ApplyEffect(pTick, 0, 0);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DC7A0
int CSWSEffectListHandler::OnRemoveHitPointChangeWhenDying(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreatureTHP* pCreature = (CSWSCreatureTHP*)pObject->AsSWSCreature();
    if (pCreature && pCreature->GetStats()->m_b6C) {
        if (pEffect->GetFloat(0) > 0.0f) {
            pCreature->SetCurrentHitPoints(pCreature->GetCurrentHitPoints(1) + 1);
        } else {
            pCreature->SetCurrentHitPoints(pCreature->GetCurrentHitPoints(1) - 1);
            CExoString sColor;
            {
                CExoString sColumn("BLOODCOLR");
                g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(
                    pCreature->m_nAppearanceType, sColumn, &sColor);
            }
            CGameEffect* pBlood = new CGameEffect(1);
            pBlood->m_nType = 0x1e;
            pBlood->SetDurationType(g_nDurationInstant);
            if (sColor == "R") {
                pBlood->SetInteger(0, 0x9e);
            } else if (sColor == "G") {
                pBlood->SetInteger(0, 0x9f);
            } else if (sColor == "Y") {
                pBlood->SetInteger(0, 0xa0);
            }
            pCreature->ApplyEffect(pBlood, 0, 0);
        }
    }
    if (pCreature->GetIsPossessed()) {
        CGameEffect* pNew = new CGameEffect(1);
        pNew->SetCreator(pEffect->GetCreator());
        pNew->m_nType = 0x39;
        pNew->SetDurationType(g_nDurationTemporary);
        pNew->m_fDuration = pEffect->GetDuration();
        pNew->SetFloat(0, pEffect->GetFloat(0));
        pObject->ApplyEffect(pNew, 0, 0);
        return 1;
    }
    if (pCreature->GetIsInConversation() || pCreature->GetDeadTemp()) {
        CGameEffect* pDeath = new CGameEffect(1);
        pDeath->m_nType = 0x13;
        pDeath->SetDurationType(g_nDurationInstant);
        pDeath->SetCreator(pEffect->GetCreator());
        pDeath->SetInteger(0, 0);
        pDeath->SetInteger(1, 1);
        pCreature->ApplyEffect(pDeath, 0, 0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DBE60
int CSWSEffectListHandler::OnApplyCurse(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 0;
    }
    if (pCreature->GetF8()) {
        return 1;
    }
    CSWSCreature* pCreator = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
    if (pCreature->GetStats()->GetEffectImmunity(0x11, pCreator)) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetObjectID(0, pObject->GetId());
        if (pCreator) {
            CSWCCMessageData* pCopy = new CSWCCMessageData;
            pData->CopyTo(pCopy);
            pCreator->SendFeedbackData(0x8d, pCopy);
        }
        pObject->AsSWSCreature()->SendFeedbackData(0x8d, pData);
        return 1;
    }
    CGameEffect* pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x25;
    pNew->SetInteger(0, 0);
    pNew->SetInteger(1, pEffect->GetInteger(0));
    pObject->ApplyEffect(pNew, bLoadingGame, 0);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x25;
    pNew->SetInteger(0, 1);
    pNew->SetInteger(1, pEffect->GetInteger(1));
    pObject->ApplyEffect(pNew, bLoadingGame, 0);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x25;
    pNew->SetInteger(0, 2);
    pNew->SetInteger(1, pEffect->GetInteger(2));
    GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, pCreator->GetId(),
                                                              pObject->GetId(), g_nEvent7441C8, pNew);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x25;
    pNew->SetInteger(0, 3);
    pNew->SetInteger(1, pEffect->GetInteger(3));
    pObject->ApplyEffect(pNew, bLoadingGame, 0);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x25;
    pNew->SetInteger(0, 4);
    pNew->SetInteger(1, pEffect->GetInteger(4));
    pObject->ApplyEffect(pNew, bLoadingGame, 0);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x25;
    pNew->SetInteger(0, 5);
    pNew->SetInteger(1, pEffect->GetInteger(5));
    pObject->ApplyEffect(pNew, bLoadingGame, 0);
    return 0;
}

class CSWSCreatureDisease : public CSWSCreature {
public:
    unsigned char SavingThrowRoll(unsigned char nSaveType, unsigned short nDC,
                                  unsigned short nSaveVsType, unsigned long oidVersus,
                                  int bSendFeedback, unsigned short nFeat, int bFromCombat);  // 0x005B92B0
    void DisplayFloatyDataToSelf(unsigned long nData);  // 0x004EF560
};

extern const unsigned long g_nMillisecondsPerMinute;  // 0x00744178 (60000)

// FUNCTION: SWKOTOR 0x004E2720
int CSWSEffectListHandler::OnApplyDisease(CSWSObject* pObject, CGameEffect* pEffect,
                                          int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
    if (!bLoadingGame) {
        if (pEffect->GetDurationType() == g_nDurationInstant) {
            pEffect->SetDurationType(g_nDurationPermanent);
        }
        C2DA* pDiseaseTable = g_pRules->Get2DArrays()->m_pDiseaseTable;
        CWorldTimer* pTimer = GetServerExoApp()->GetWorldTimer();
        int nRow = pEffect->GetInteger(0);
        if (pObject->AsSWSCreature()->m_bF8) {
            return 1;
        }
        CSWSCreature* pCreator =
            GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
        if (pObject->AsSWSCreature()->GetStats()->GetEffectImmunity(3, pCreator)) {
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, pObject->GetId());
            if (pCreator) {
                CSWCCMessageData* pCopy = new CSWCCMessageData;
                pData->CopyTo(pCopy);
                pCreator->SendFeedbackData(0x7d, pCopy);
            }
            pObject->AsSWSCreature()->SendFeedbackData(0x7d, pData);
            return 1;
        }
        for (int i = 0; i < pObject->m_appliedEffects.num; i++) {
            if (pObject->m_appliedEffects.element[i]->GetType() == 5) {
                return 1;
            }
        }
        int nFirstSave;
        {
            CExoString sColumn("First_Save");
            pDiseaseTable->GetINTEntry(nRow, sColumn, &nFirstSave);
        }
        if (((CSWSCreatureDisease*)pObject->AsSWSCreature())
                ->SavingThrowRoll(1, (unsigned short)nFirstSave, 5, pEffect->GetCreator(), 1, 0, 0)) {
            return 1;
        }
        int nName;
        {
            CExoString sColumn("Name");
            pDiseaseTable->GetINTEntry(nRow, sColumn, &nName);
        }
        if (pObject->AsSWSCreature()) {
            ((CSWSCreatureDisease*)pObject->AsSWSCreature())->DisplayFloatyDataToSelf(0);
        }
        unsigned long nDay;
        unsigned long nTime;
        pTimer->GetWorldTime(&nDay, &nTime);
        int nHours;
        {
            CExoString sColumn("Incu_Hours");
            pDiseaseTable->GetINTEntry(nRow, sColumn, &nHours);
        }
        unsigned long nExpiryDay;
        unsigned long nExpiryTime;
        unsigned long nDelta = ((CServerExoAppModule7*)GetServerExoApp())->GetModule()->m_nMinutesPerHour *
                               g_nMillisecondsPerMinute * nHours;
        pTimer->AddWorldTimes(nDay, nTime, 0, nDelta, &nExpiryDay, &nExpiryTime);
        pEffect->SetInteger(3, nExpiryDay);
        pEffect->SetInteger(4, nExpiryTime);
    }
    CGameEffect* pVisual = new CGameEffect(pEffect);
    pVisual->m_nType = 0x1e;
    pVisual->SetInteger(0, 0x33);
    pObject->ApplyEffect(pVisual, bLoadingGame, 0);
    return 0;
    }
    return 1;
}

class CSWSCreaturePoison : public CSWSCreature {
public:
    unsigned char SavingThrowRoll(unsigned char nSaveType, unsigned short nDC,
                                  unsigned short nSaveVsType, unsigned long oidVersus,
                                  int bSendFeedback, unsigned short nFeat, int bFromCombat);  // 0x005B92B0
    void BroadcastPoisoned(CSWCCMessageData* pData);  // 0x004ECD60
    void BroadcastVoiceChat(unsigned char nChat);   // 0x004F2520
    void ApplyPoisonDamage(int nPoison, CGameEffect* pEffect, int bArg3, float fScale);  // 0x004EE770
    int GetIsPoisoned() { return m_n9E0; }
};

// FUNCTION: SWKOTOR 0x004DB320
// NONMATCH (21): frame 0x20 vs 0x24; the original keeps the operator new
// temporaries in the object argument slot and DC/Name in other slots.
int CSWSEffectListHandler::OnApplyPoison(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    unsigned long nDay;
    unsigned long nTime;
    CSWSCreaturePoison* pCreature = (CSWSCreaturePoison*)pObject->AsSWSCreature();
    if (!pCreature) {
        return 0;
    }
    if (pCreature->m_bF8 || pCreature->GetIsInConversation() || pCreature->GetDeadTemp() ||
        pCreature->GetIsPoisoned() == 1) {
        GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, pEffect->GetCreator(), pCreature->GetId(), g_nEventRemoveEffect, pEffect);
        return 0;
    }
    if (!bLoadingGame) {
        CSWSCreature* pCreator =
            GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
        if (pCreature->GetStats()->GetEffectImmunity(2, pCreator)) {
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, pObject->GetId());
            pData->SetInteger(0, 0);
            pData->SetInteger(1, 0);
            pData->SetInteger(2, 0x2000);
            if (pCreator) {
                CSWCCMessageData* pCopy = new CSWCCMessageData;
                pData->CopyTo(pCopy);
                pCreator->SendFeedbackData(0x3e, pCopy);
            }
            pCreature->SendFeedbackData(0x3e, pData);
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, pEffect->GetCreator(), pCreature->GetId(), g_nEventRemoveEffect, pEffect);
            return 0;
        }
        GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
        pEffect->SetInteger(1, nDay);
        pEffect->SetInteger(2, nTime);
        C2DA* pPoisonTable = g_pRules->Get2DArrays()->m_pPoisonTable;
        if (!pPoisonTable) {
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, pEffect->GetCreator(), pCreature->GetId(), g_nEventRemoveEffect, pEffect);
            return 0;
        }
        int nRow = pEffect->GetInteger(0);
        int nDC;
        {
            CExoString sColumn("DC_SAVE");
            pPoisonTable->GetINTEntry(nRow, sColumn, &nDC);
        }
        if (pCreature->SavingThrowRoll(1, (unsigned short)nDC, 0xc, pEffect->GetCreator(), 1, 0, 0)) {
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, pEffect->GetCreator(), pCreature->GetId(), g_nEventRemoveEffect, pEffect);
            return 0;
        }
        int nName;
        {
            CExoString sColumn("Name");
            pPoisonTable->GetINTEntry(nRow, sColumn, &nName);
        }
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetInteger(0, nName);
        pData->SetObjectID(0, pCreature->GetId());
        pCreature->BroadcastPoisoned(pData);
        delete pData;
        pCreature->BroadcastVoiceChat(0x1c);
        pEffect->SetFloat(0, 2.0f);
        pCreature->ApplyPoisonDamage(nRow, pEffect, 0, 1.0f);
        pEffect->SetInteger(3, nDay);
        pEffect->SetInteger(4, nTime);
        int nDuration;
        {
            CExoString sColumn("DURATION");
            pPoisonTable->GetINTEntry(nRow, sColumn, &nDuration);
        }
        pEffect->SetInteger(5, nDuration);
        pEffect->m_fDuration = nDuration * 1000.0f;
        pEffect->SetDurationType(g_nDurationTemporary);
        int nPeriod;
        {
            CExoString sColumn("PERIOD");
            pPoisonTable->GetINTEntry(nRow, sColumn, &nPeriod);
        }
        pEffect->SetInteger(6, nPeriod);
        pEffect->SetInteger(7, 0);
    }
    CGameEffect* pVisual = new CGameEffect(1);
    pVisual->m_nType = 0x1e;
    pVisual->SetDurationType(g_nDurationInstant);
    pVisual->SetInteger(0, 0x3eb);
    pVisual->SetInteger(2, 0);
    pVisual->SetObjectID(0, pEffect->GetCreator());
    pCreature->ApplyEffect(pVisual, 0, 0);
    pCreature->m_n9E0 = 1;
    pCreature->m_nEffectID9E8 = pEffect->GetID();
    return 0;
}

// FUNCTION: SWKOTOR 0x004DD520
int CSWSEffectListHandler::OnApplyNegativeLevel(CSWSObject* pObject, CGameEffect* pEffect,
                                                int bLoadingGame)
{
    if (pEffect->GetInteger(0) > 100) {
        return 1;
    }
    if (!pObject->AsSWSCreature()) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    CSWSCreature* pCreator = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
    if (pCreature->GetStats()->GetEffectImmunity(0x1d, pCreator)) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetObjectID(0, pObject->GetId());
        if (pCreator) {
            CSWCCMessageData* pCopy = new CSWCCMessageData;
            pData->CopyTo(pCopy);
            pCreator->SendFeedbackData(0x83, pCopy);
        }
        pCreature->SendFeedbackData(0x83, pData);
        return 1;
    }
    CGameEffect* pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x0b;
    pNew->SetInteger(0, pEffect->GetInteger(0));
    pNew->SetInteger(1, 0);
    pNew->SetInteger(2, g_pRules->m_nAA);
    pCreature->ApplyEffect(pNew, bLoadingGame, 0);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x1b;
    pNew->SetInteger(0, pEffect->GetInteger(0));
    pNew->SetInteger(1, 0);
    pNew->SetInteger(3, g_pRules->m_nAA);
    pCreature->ApplyEffect(pNew, bLoadingGame, 0);
    pNew = new CGameEffect(pEffect);
    pNew->m_nType = 0x38;
    pNew->SetInteger(0, 0xff);
    pNew->SetInteger(1, pEffect->GetInteger(0));
    pNew->SetInteger(2, g_pRules->m_nAA);
    pCreature->ApplyEffect(pNew, bLoadingGame, 0);
    int nBestClass = 0;
    int nBestLevel = 0;
    for (int i = 0; i < pCreature->GetStats()->m_nNumMultiClasses; i++) {
        if (pCreature->GetStats()->GetClassLevel(i, 0) > nBestLevel) {
            nBestClass = i;
            nBestLevel = pCreature->GetStats()->GetClassLevel(i, 0);
        }
    }
    if (nBestLevel > 0) {
        int nNegative = pCreature->GetStats()->GetClassNegativeLevels(nBestClass);
        pCreature->GetStats()->SetClassNegativeLevels(nBestClass, pEffect->GetInteger(0) + nNegative);
        pEffect->SetInteger(1, nBestClass);
        if (pCreature->GetStats()->GetLevel(1) <= 0) {
            CGameEffect* pDeath = new CGameEffect(1);
            pDeath->m_nType = 0x13;
            pDeath->SetDurationType(g_nDurationInstant);
            pDeath->SetInteger(0, 1);
            pDeath->SetInteger(1, 1);
            pDeath->SetCreator(pEffect->GetCreator());
            pCreature->ApplyEffect(pDeath, bLoadingGame, 0);
        }
    }
    if (pCreator) {
        CGameEffect* pBonus = new CGameEffect(1);
        pBonus->SetDurationType(g_nDurationPermanent);
        pBonus->m_nType = 0x0f;
        pBonus->SetInteger(0, pEffect->GetInteger(0) * 5);
        pCreator->ApplyEffect(pBonus, bLoadingGame, 0);
    }
    return 0;
}

struct CSWSAttackData {
    char pad_0x0[0x12d];
    unsigned char m_nAttackResult;              // 0x12d (placeholder name)
    char pad_0x12e[0x13c - 0x12e];
    int m_bEffectFeedback;                      // 0x13c (placeholder name)
};

class CSWSCreatureState : public CSWSCreature {
public:
    void ClearAllActions(int bArg);             // 0x004CCD80
    void SetAttackSummaryFeedbackEffectData(int nState, unsigned long oidTarget);  // 0x005B60F0
};

// FUNCTION: SWKOTOR 0x004DA330
int CSWSEffectListHandler::OnApplySetStateInternal(CSWSObject* pObject, CGameEffect* pEffect,
                                                   int bLoadingGame)
{
    CSWSCreatureState* pCreature = (CSWSCreatureState*)pObject->AsSWSCreature();
    int bCleared = 0;
    int nState;
    if (pCreature && pEffect) {
        nState = pEffect->GetInteger(0);
        pCreature->m_n350 = 0;
        pCreature->m_n354 = 1;
        bCleared = 0;
        if (nState != 9) {
            pCreature->m_nE8 = 1;
            pCreature->ClearAllActions(1);
            pCreature->m_pCombatRound->RemoveAllActions();
            pCreature->m_nE8 = 0;
            bCleared = 1;
        }
        switch (nState) {
        case 4: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -3);
            unsigned long nDay;
            unsigned long nTime;
            pEffect->GetExpiryTime(&nDay, &nTime);
            pNew->SetExpiryTime(nDay, nTime);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x1e;
            pNew->SetInteger(0, 0x7d2);
            pNew->SetExpiryTime(nDay, nTime);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 7: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -3);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 9: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -3);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 2: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x1b;
            pNew->SetInteger(0, 2);
            pNew->SetInteger(1, 0);
            pNew->SetInteger(3, g_pRules->m_nAA);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            break;
        }
        case 3: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x1e;
            pNew->SetInteger(0, 0x3ef);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x1b;
            pNew->SetInteger(0, 2);
            pNew->SetInteger(1, 0);
            pNew->SetInteger(3, g_pRules->m_nAA);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -3);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 5: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -0xf);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 6: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -0x15f);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 10: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -3);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        case 8: {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x17;
            pNew->SetInteger(0, -3);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->UpdateAmbientAnimationState();
            break;
        }
        }
    }
    CSWSCreatureState* pCreator =
        (CSWSCreatureState*)GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
    if (!bCleared && pCreator) {
        CSWSAttackData* pAttack =
            pCreator->m_pCombatRound->GetAttack(pCreator->m_pCombatRound->m_nCurrentAttack);
        if (pAttack) {
            pAttack->m_nAttackResult = (unsigned char)nState;
            pAttack->m_bEffectFeedback = 1;
            pCreator->SetAttackSummaryFeedbackEffectData(nState, pCreature->m_idSelf);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E4A10
void CSWSEffectListHandler::InitializeEffects()
{
    m_pApplyFunctions = new EffectApplyFn[0x6e];
    m_pRemoveFunctions = new EffectRemoveFn[0x6e];
    for (int i = 0; i < 0x6e; i++) {
        m_pApplyFunctions[i] = 0;
        m_pRemoveFunctions[i] = 0;
    }
    m_pApplyFunctions[0x26] = &CSWSEffectListHandler::OnApplyDamage;
    m_pApplyFunctions[0x27] = &CSWSEffectListHandler::OnApplyHeal;
    m_pApplyFunctions[0x13] = &CSWSEffectListHandler::OnApplyDeath;
    m_pApplyFunctions[0x4] = &CSWSEffectListHandler::OnApplyResurrection;
    m_pApplyFunctions[0x24] = &CSWSEffectListHandler::OnApplyAbilityIncrease;
    m_pRemoveFunctions[0x24] = &CSWSEffectListHandler::OnRemoveAbilityIncrease;
    m_pApplyFunctions[0x25] = &CSWSEffectListHandler::OnApplyAbilityDecrease;
    m_pRemoveFunctions[0x25] = &CSWSEffectListHandler::OnRemoveAbilityDecrease;
    m_pApplyFunctions[0x6] = &CSWSEffectListHandler::OnApplySummonCreature;
    m_pRemoveFunctions[0x6] = &CSWSEffectListHandler::OnRemoveSummonCreature;
    m_pApplyFunctions[0x2] = &CSWSEffectListHandler::OnApplyNothing;
    m_pApplyFunctions[0x1a] = &CSWSEffectListHandler::OnApplySavingThrowIncrease;
    m_pRemoveFunctions[0x1a] = &CSWSEffectListHandler::OnRemoveFlag344;
    m_pApplyFunctions[0x1b] = &CSWSEffectListHandler::OnApplySavingThrowDecrease;
    m_pRemoveFunctions[0x1b] = &CSWSEffectListHandler::OnRemoveFlag344Type1B;
    m_pApplyFunctions[0x30] = &CSWSEffectListHandler::OnApplyACIncrease;
    m_pRemoveFunctions[0x30] = &CSWSEffectListHandler::OnRemoveACIncrease;
    m_pApplyFunctions[0x31] = &CSWSEffectListHandler::OnApplyACDecrease;
    m_pRemoveFunctions[0x31] = &CSWSEffectListHandler::OnRemoveACDecrease;
    m_pApplyFunctions[0xa] = &CSWSEffectListHandler::OnApplyAttackIncrease;
    m_pRemoveFunctions[0xa] = &CSWSEffectListHandler::OnRemoveFlag344Type0A;
    m_pApplyFunctions[0xb] = &CSWSEffectListHandler::OnApplyAttackDecrease;
    m_pRemoveFunctions[0xb] = &CSWSEffectListHandler::OnRemoveFlag344Type0B;
    m_pApplyFunctions[0xd] = &CSWSEffectListHandler::OnApplyFlag344;
    m_pRemoveFunctions[0xd] = &CSWSEffectListHandler::OnRemoveFlag344Type0D;
    m_pApplyFunctions[0xe] = &CSWSEffectListHandler::OnApplyDamageDecrease;
    m_pRemoveFunctions[0xe] = &CSWSEffectListHandler::OnRemoveFlag344Type0E;
    m_pApplyFunctions[0xf] = &CSWSEffectListHandler::OnApplyTemporaryHitpoints;
    m_pRemoveFunctions[0xf] = &CSWSEffectListHandler::OnRemoveTemporaryHitpoints;
    m_pApplyFunctions[0x10] = &CSWSEffectListHandler::OnApplyDamageImmunityIncrease;
    m_pRemoveFunctions[0x10] = &CSWSEffectListHandler::OnRemoveDamageImmunityIncrease;
    m_pApplyFunctions[0x11] = &CSWSEffectListHandler::OnApplyDamageImmunityDecrease;
    m_pRemoveFunctions[0x11] = &CSWSEffectListHandler::OnRemoveDamageImmunityDecrease;
    m_pApplyFunctions[0x12] = &CSWSEffectListHandler::OnApplyEntangled;
    m_pRemoveFunctions[0x12] = &CSWSEffectListHandler::OnRemoveEntangled;
    m_pApplyFunctions[0x14] = &CSWSEffectListHandler::OnApplyKnockdown;
    m_pRemoveFunctions[0x14] = &CSWSEffectListHandler::OnRemoveCombatPause;
    m_pApplyFunctions[0x57] = &CSWSEffectListHandler::OnApplyDisarm;
    m_pRemoveFunctions[0x57] = &CSWSEffectListHandler::OnRemoveNothing;
    m_pApplyFunctions[0x17] = &CSWSEffectListHandler::OnApplySetAIState;
    m_pRemoveFunctions[0x17] = &CSWSEffectListHandler::OnRemoveSetAIState;
    m_pApplyFunctions[0x15] = &CSWSEffectListHandler::OnApplyDeaf;
    m_pApplyFunctions[0x18] = &CSWSEffectListHandler::OnApplyEnemyAttackBonus;
    m_pRemoveFunctions[0x18] = &CSWSEffectListHandler::OnRemoveEnemyAttackBonus;
    m_pApplyFunctions[0x8] = &CSWSEffectListHandler::OnApplySetState;
    m_pRemoveFunctions[0x8] = &CSWSEffectListHandler::OnRemoveSetState;
    m_pApplyFunctions[0x52] = &CSWSEffectListHandler::OnApplyNegativeLevel;
    m_pRemoveFunctions[0x52] = &CSWSEffectListHandler::OnRemoveNegativeLevel;
    m_pApplyFunctions[0x3d] = &CSWSEffectListHandler::OnApplyNothingType3D;
    m_pRemoveFunctions[0x3d] = &CSWSEffectListHandler::OnRemoveNothingType3D;
    m_pApplyFunctions[0x7] = &CSWSEffectListHandler::OnApplyRegenerate;
    m_pApplyFunctions[0x5] = &CSWSEffectListHandler::OnApplyDisease;
    m_pRemoveFunctions[0x5] = &CSWSEffectListHandler::OnRemove9E4;
    m_pApplyFunctions[0x23] = &CSWSEffectListHandler::OnApplyPoison;
    m_pRemoveFunctions[0x23] = &CSWSEffectListHandler::OnRemove9E0;
    m_pApplyFunctions[0xc] = &CSWSEffectListHandler::OnApplyNothingType0C;
    m_pApplyFunctions[0x1c] = &CSWSEffectListHandler::OnApplyMovementSpeedIncrease;
    m_pRemoveFunctions[0x1c] = &CSWSEffectListHandler::OnRemoveMovementSpeedChange;
    m_pApplyFunctions[0x1d] = &CSWSEffectListHandler::OnApplyMovementSpeedDecrease;
    m_pRemoveFunctions[0x1d] = &CSWSEffectListHandler::OnRemoveMovementSpeedChangeType1D;
    m_pApplyFunctions[0x1e] = &CSWSEffectListHandler::OnApplyVisualEffect;
    m_pRemoveFunctions[0x1e] = &CSWSEffectListHandler::OnRemoveAIValue;
    m_pApplyFunctions[0x1f] = &CSWSEffectListHandler::OnApplyAreaOfEffect;
    m_pRemoveFunctions[0x1f] = &CSWSEffectListHandler::OnRemoveAreaOfEffect;
    m_pApplyFunctions[0x20] = &CSWSEffectListHandler::OnApplyLinkedVisual;
    m_pRemoveFunctions[0x20] = &CSWSEffectListHandler::OnRemoveNothingType20;
    m_pApplyFunctions[0x21] = &CSWSEffectListHandler::OnApplyForceResistanceIncrease;
    m_pRemoveFunctions[0x21] = &CSWSEffectListHandler::OnRemoveForceResistanceIncrease;
    m_pApplyFunctions[0x22] = &CSWSEffectListHandler::OnApplyForceResistanceDecrease;
    m_pRemoveFunctions[0x22] = &CSWSEffectListHandler::OnRemoveForceResistanceDecrease;
    m_pApplyFunctions[0x41] = &CSWSEffectListHandler::OnApplyNothingType41;
    m_pRemoveFunctions[0x41] = &CSWSEffectListHandler::OnRemoveNothingType41;
    m_pApplyFunctions[0x32] = &CSWSEffectListHandler::OnApplyNothingType32;
    m_pRemoveFunctions[0x32] = &CSWSEffectListHandler::OnRemoveNothingType32;
    m_pApplyFunctions[0x1] = &CSWSEffectListHandler::OnApplyHasteOrSlow;
    m_pRemoveFunctions[0x1] = &CSWSEffectListHandler::OnRemoveHasteOrSlow;
    m_pApplyFunctions[0x3] = &CSWSEffectListHandler::OnApplyHasteOrSlow;
    m_pRemoveFunctions[0x3] = &CSWSEffectListHandler::OnRemoveHasteOrSlow;
    m_pApplyFunctions[0x29] = &CSWSEffectListHandler::OnApplyPair1C30;
    m_pRemoveFunctions[0x29] = &CSWSEffectListHandler::OnRemove8CC;
    m_pApplyFunctions[0x2a] = &CSWSEffectListHandler::OnApplySlow;
    m_pRemoveFunctions[0x2a] = &CSWSEffectListHandler::OnRemove8D0;
    m_pApplyFunctions[0x28] = &CSWSEffectListHandler::OnApplyLink;
    m_pApplyFunctions[0x2c] = &CSWSEffectListHandler::OnApply9A4;
    m_pRemoveFunctions[0x2c] = &CSWSEffectListHandler::OnRemove9A4;
    m_pApplyFunctions[0x2d] = &CSWSEffectListHandler::OnApplyCurse;
    m_pRemoveFunctions[0x2d] = &CSWSEffectListHandler::OnRemoveNothingType2D;
    m_pApplyFunctions[0x2e] = &CSWSEffectListHandler::OnApplyImmunityFeedback;
    m_pRemoveFunctions[0x2e] = &CSWSEffectListHandler::OnRemove8C8;
    m_pApplyFunctions[0x2f] = &CSWSEffectListHandler::OnApplyInvisibility;
    m_pRemoveFunctions[0x2f] = &CSWSEffectListHandler::OnRemoveInvisibility;
    m_pApplyFunctions[0x3f] = &CSWSEffectListHandler::OnApplySanctuary;
    m_pRemoveFunctions[0x3f] = &CSWSEffectListHandler::OnRemoveSanctuary;
    m_pApplyFunctions[0x46] = &CSWSEffectListHandler::OnApply8EC;
    m_pRemoveFunctions[0x46] = &CSWSEffectListHandler::OnRemove8ECBit0;
    m_pApplyFunctions[0x47] = &CSWSEffectListHandler::OnApplyUltraVision;
    m_pRemoveFunctions[0x47] = &CSWSEffectListHandler::OnRemoveUltraVision;
    m_pApplyFunctions[0x48] = &CSWSEffectListHandler::OnApplyTrueSeeing;
    m_pRemoveFunctions[0x48] = &CSWSEffectListHandler::OnRemoveTrueSeeing;
    m_pApplyFunctions[0x49] = &CSWSEffectListHandler::OnApplyBlindness;
    m_pRemoveFunctions[0x49] = &CSWSEffectListHandler::OnRemoveBlindness;
    m_pApplyFunctions[0x4a] = &CSWSEffectListHandler::OnApplyPair2F49;
    m_pRemoveFunctions[0x4a] = &CSWSEffectListHandler::OnRemoveNothingType4A;
    m_pApplyFunctions[0x4b] = &CSWSEffectListHandler::OnApplyMissChance;
    m_pRemoveFunctions[0x4b] = &CSWSEffectListHandler::OnRemoveNothingType4B;
    m_pApplyFunctions[0x4c] = &CSWSEffectListHandler::OnApplyMissChanceType4C;
    m_pRemoveFunctions[0x4c] = &CSWSEffectListHandler::OnRemoveNothingType4C;
    m_pApplyFunctions[0x4f] = &CSWSEffectListHandler::OnApplyArea;
    m_pRemoveFunctions[0x4f] = &CSWSEffectListHandler::OnRemoveDisappearAppear;
    m_pApplyFunctions[0x50] = &CSWSEffectListHandler::OnApplyCommandable;
    m_pApplyFunctions[0x51] = &CSWSEffectListHandler::OnApplyCommandable2;
    m_pApplyFunctions[0x4e] = &CSWSEffectListHandler::OnApplyNothingType4E;
    m_pRemoveFunctions[0x4e] = &CSWSEffectListHandler::OnRemoveNothingType4E;
    m_pApplyFunctions[0x16] = &CSWSEffectListHandler::OnApplyCreatureOnly;
    m_pRemoveFunctions[0x16] = &CSWSEffectListHandler::OnRemoveNothingType16;
    m_pApplyFunctions[0x9] = &CSWSEffectListHandler::OnApplySetStateInternal;
    m_pRemoveFunctions[0x9] = &CSWSEffectListHandler::OnRemoveSetStateInternal;
    m_pApplyFunctions[0x19] = &CSWSEffectListHandler::OnApplyStats11C;
    m_pRemoveFunctions[0x19] = &CSWSEffectListHandler::OnRemoveArcaneSpellFailure;
    m_pApplyFunctions[0x33] = &CSWSEffectListHandler::OnApplyDispelAllMagic;
    m_pApplyFunctions[0x34] = &CSWSEffectListHandler::OnApplyDispelBestMagic;
    m_pApplyFunctions[0x35] = &CSWSEffectListHandler::OnApplyPair3119;
    m_pRemoveFunctions[0x35] = &CSWSEffectListHandler::OnRemove8E4;
    m_pApplyFunctions[0x36] = &CSWSEffectListHandler::OnApplyVisualCopy;
    m_pRemoveFunctions[0x36] = &CSWSEffectListHandler::OnRemoveNothingType36;
    m_pApplyFunctions[0x37] = &CSWSEffectListHandler::OnApplySkillIncrease;
    m_pRemoveFunctions[0x37] = &CSWSEffectListHandler::OnRemoveNothingType37;
    m_pApplyFunctions[0x38] = &CSWSEffectListHandler::OnApplySkillDecrease;
    m_pRemoveFunctions[0x38] = &CSWSEffectListHandler::OnRemoveNothingType38;
    m_pApplyFunctions[0x39] = &CSWSEffectListHandler::OnApplyFloat39;
    m_pRemoveFunctions[0x39] = &CSWSEffectListHandler::OnRemoveHitPointChangeWhenDying;
    m_pApplyFunctions[0x3a] = &CSWSEffectListHandler::OnApplyA10;
    m_pRemoveFunctions[0x3a] = &CSWSEffectListHandler::OnRemoveSpecialWalkAnimation;
    m_pApplyFunctions[0x3b] = &CSWSEffectListHandler::OnApply8E8;
    m_pRemoveFunctions[0x3b] = &CSWSEffectListHandler::OnRemove8E8;
    m_pApplyFunctions[0x43] = &CSWSEffectListHandler::OnApplyEffectIcon;
    m_pRemoveFunctions[0x43] = &CSWSEffectListHandler::OnRemoveEffectIcon;
    m_pApplyFunctions[0x3e] = &CSWSEffectListHandler::OnApplyDisguise;
    m_pRemoveFunctions[0x3e] = &CSWSEffectListHandler::OnRemove4EE150;
    m_pApplyFunctions[0x40] = &CSWSEffectListHandler::OnApplyTimestop;
    m_pRemoveFunctions[0x40] = &CSWSEffectListHandler::OnRemoveServerFlags;
    m_pApplyFunctions[0x44] = &CSWSEffectListHandler::OnApplyRulesEntry;
    m_pRemoveFunctions[0x44] = &CSWSEffectListHandler::OnRemoveNothingType44;
    m_pApplyFunctions[0x45] = &CSWSEffectListHandler::OnApplyVision;
    m_pRemoveFunctions[0x45] = &CSWSEffectListHandler::OnRemoveNothingType45;
    m_pApplyFunctions[0x4d] = &CSWSEffectListHandler::OnApplyStats6C;
    m_pRemoveFunctions[0x4d] = &CSWSEffectListHandler::OnRemoveNothingType4D;
    m_pApplyFunctions[0x58] = &CSWSEffectListHandler::OnApplyStats6C;
    m_pRemoveFunctions[0x58] = &CSWSEffectListHandler::OnRemoveNothingType4D;
    m_pApplyFunctions[0x53] = &CSWSEffectListHandler::OnApplyBonusFeat;
    m_pRemoveFunctions[0x53] = &CSWSEffectListHandler::OnRemoveBonusFeat;
    m_pApplyFunctions[0x54] = &CSWSEffectListHandler::OnApplyTemporary54;
    m_pRemoveFunctions[0x54] = &CSWSEffectListHandler::OnRemoveWounding;
    m_pApplyFunctions[0x56] = &CSWSEffectListHandler::OnApplyVampiricRegeneration;
    m_pApplyFunctions[0x59] = &CSWSEffectListHandler::OnApplySummonParty;
    m_pRemoveFunctions[0x59] = &CSWSEffectListHandler::OnRemoveSummonCreatureType59;
    m_pApplyFunctions[0x5a] = &CSWSEffectListHandler::OnApplyHitPoints124;
    m_pApplyFunctions[0x5b] = &CSWSEffectListHandler::OnApplyHitPoints126;
    m_pRemoveFunctions[0x5b] = &CSWSEffectListHandler::OnRemoveHitPoints126;
    m_pApplyFunctions[0x2b] = &CSWSEffectListHandler::OnApplyCreatureOnlyType2B;
    m_pRemoveFunctions[0x2b] = &CSWSEffectListHandler::OnRemoveCreatureOnly;
    m_pApplyFunctions[0x3c] = &CSWSEffectListHandler::OnApplyForcePush;
    m_pRemoveFunctions[0x3c] = &CSWSEffectListHandler::OnRemoveApplyVisual17;
    m_pApplyFunctions[0x42] = &CSWSEffectListHandler::OnApplyCreatureOnlyType42;
    m_pRemoveFunctions[0x42] = &CSWSEffectListHandler::OnRemoveCreatureOnlyType42;
    m_pApplyFunctions[0x5c] = &CSWSEffectListHandler::OnApplyCreatureOnlyType5C;
    m_pRemoveFunctions[0x5c] = &CSWSEffectListHandler::OnRemoveCreatureOnlyType5C;
    m_pApplyFunctions[0x5d] = &CSWSEffectListHandler::OnApplyCreatureOnlyType5D;
    m_pRemoveFunctions[0x5d] = &CSWSEffectListHandler::OnRemoveCreatureOnlyType5D;
    m_pApplyFunctions[0x5e] = &CSWSEffectListHandler::OnApplyCreatureOnlyType5E;
    m_pRemoveFunctions[0x5e] = &CSWSEffectListHandler::OnRemoveCreatureOnlyType5E;
    m_pApplyFunctions[0x5f] = &CSWSEffectListHandler::OnApplyDamage124;
    m_pApplyFunctions[0x60] = &CSWSEffectListHandler::OnApplyHeal124;
    m_pApplyFunctions[0x61] = &CSWSEffectListHandler::OnApplyCreatureOnlyType61;
    m_pRemoveFunctions[0x61] = &CSWSEffectListHandler::OnRemoveCreatureOnlyType61;
    m_pApplyFunctions[0x62] = &CSWSEffectListHandler::OnApplyA9C;
    m_pRemoveFunctions[0x62] = &CSWSEffectListHandler::OnRemoveA9C;
    m_pApplyFunctions[0x63] = &CSWSEffectListHandler::OnApplyCreatureOnlyType63;
    m_pRemoveFunctions[0x63] = &CSWSEffectListHandler::OnRemoveCreatureOnlyType63;
    m_pApplyFunctions[0x64] = &CSWSEffectListHandler::OnApplyLightsaberThrow;
    m_pRemoveFunctions[0x64] = &CSWSEffectListHandler::OnRemoveA04;
    m_pApplyFunctions[0x65] = &CSWSEffectListHandler::OnApply8D4;
    m_pRemoveFunctions[0x65] = &CSWSEffectListHandler::OnRemove8D4;
    m_pApplyFunctions[0x66] = &CSWSEffectListHandler::OnApplyTargetEvent;
    m_pRemoveFunctions[0x66] = &CSWSEffectListHandler::OnRemoveNothingType66;
    m_pApplyFunctions[0x67] = &CSWSEffectListHandler::OnApplyForceJumpDelayed;
    m_pRemoveFunctions[0x67] = &CSWSEffectListHandler::OnRemoveNothingType67;
    m_pApplyFunctions[0x68] = &CSWSEffectListHandler::OnApplyAssuredDeflection;
    m_pRemoveFunctions[0x68] = &CSWSEffectListHandler::OnRemoveAssuredDeflection;
    m_pApplyFunctions[0x69] = &CSWSEffectListHandler::OnApplyForceResisted;
    m_pApplyFunctions[0x6a] = &CSWSEffectListHandler::OnApplyForceFizzle;
    m_pApplyFunctions[0x6b] = &CSWSEffectListHandler::OnApplyForceShield;
    m_pRemoveFunctions[0x6b] = &CSWSEffectListHandler::OnRemoveNothingType6B;
    m_pApplyFunctions[0x6c] = &CSWSEffectListHandler::OnApplyPureGoodPowers;
    m_pRemoveFunctions[0x6c] = &CSWSEffectListHandler::OnRemoveNothingType6C;
    m_pApplyFunctions[0x6d] = &CSWSEffectListHandler::OnApplyPureEvilPowers;
    m_pRemoveFunctions[0x6d] = &CSWSEffectListHandler::OnRemoveNothingType6D;
}

// CPathfindInformation (only the members used here)
class CSWSCreatureFollowInfo {
public:
    char pad_0x0[0x4];
    float m_fRadius;                            // 0x04 (placeholder name)
    char pad_0x8[0x14 - 0x8];
    float m_fRange;                             // 0x14 (placeholder name)
    char pad_0x18[0x8c - 0x18];
    int m_nWayPoints;                           // 0x8c
    float* m_pWayPoints;                        // 0x90
    char pad_0x94[0x9c - 0x94];
    int m_nCurrentWayPoint;                     // 0x9c (placeholder name)
};

inline Vector operator-(const Vector& v)
{
    return Vector(-v.x, -v.y, -v.z);
}

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

inline Vector operator*(float f, const Vector& v)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector Normalize(const Vector& v);              // 0x004AB180

class CResRef {
public:
    char m_resRef[16];
};

// CSWSArea + 0x100 (CResHelper base); its resref sits at +0x0c
class CAreaResHelper7 {
public:
    virtual ~CAreaResHelper7();
    char pad_0x4[0xc - 0x4];
    CResRef m_cResRef;                          // 0x0c
    CResRef& GetResRef() { return m_cResRef; }
};

class CSWSAreaBase7 {
public:
    virtual ~CSWSAreaBase7();
    char pad_0x4[0x100 - 0x4];
};

class CSWSAreaPush : public CSWSAreaBase7, public CAreaResHelper7 {
};

// line-walk result (0x48 bytes; the stop point at +4)
struct CWalkHitInfo {
    CWalkHitInfo();                             // 0x004B14B0
    unsigned long m_n00;
    Vector m_vPoint;                            // 0x04
    char pad_0x10[0x48 - 0x10];
};

class CServerExoAppPush {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
    int TestDirectLine(unsigned long oidSelf, CResRef& cResRef, const Vector& vStart,
                       const Vector& vEnd, float* pRadius, float fRange, int bArg7,
                       CWalkHitInfo* pHit);     // 0x004AEE80
};

class CSWSCreaturePush : public CSWSCreature {
public:
    void SetOrientation(const Vector& vOrientation);  // 0x004CD170
    CSWSAreaPush* GetArea();                    // 0x004CB120
    void WalkUpdateLocation_QuickWalk(int bArg);  // 0x0051D9C0
};

// FUNCTION: SWKOTOR 0x004E3800
// NONMATCH (22): only the area resref argument differs: the original adds
// 0x100 (CResHelper base) and 0x0c in two steps; cl folds them into 0x10c.
int CSWSEffectListHandler::OnApplyForcePush(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    CSWSCreaturePush* pCreature = (CSWSCreaturePush*)pObject->AsSWSCreature();
    if (pCreature) {
    CGameObject* pSource =
        ((CServerExoAppPush*)GetServerExoApp())->GetGameObject(pEffect->GetCreator());
    if (pSource && pSource->AsSWSObject()) {
    Vector vPoint;
    int bUsePoint = pEffect->GetInteger(0);
    int bDirect = 0;
    if (bUsePoint) {
        vPoint.x = pEffect->GetFloat(0);
        vPoint.y = pEffect->GetFloat(1);
        vPoint.z = pEffect->GetFloat(2);
        bDirect = pEffect->GetInteger(1);
    } else {
        vPoint = pSource->AsSWSObject()->m_vPosition;
    }
    float fDistance = pEffect->GetFloat(3);
    if (fDistance == 0.0f) {
        fDistance = 5.0f;
    }
    Vector* pPosition = &pCreature->m_vPosition;
    Vector vDirection = Normalize(*pPosition - vPoint);
    Vector vTarget = vDirection * fDistance + *pPosition;
    pCreature->SetOrientation(-vDirection);
    if (!bDirect) {
        CWalkHitInfo cHit;
        CSWSCreatureFollowInfo* pInfo = pCreature->m_pFollowInfo;
        float fRadius = pInfo->m_fRadius;
        float fRange = pInfo->m_fRange;
        CSWSAreaPush* pArea = pCreature->GetArea();
        int nResult = ((CServerExoAppPush*)GetServerExoApp())
                          ->TestDirectLine(pCreature->GetId(), pArea->GetResRef(),
                                           *pPosition, vTarget, &fRadius, fRange, 1, &cHit);
        if (nResult != 1) {
            if (nResult == 0) {
                vTarget = *pPosition;
            } else {
                vTarget = cHit.m_vPoint;
            }
        }
    }
    float* pPath = new float[4];
    pPath[0] = pPosition->x;
    pPath[1] = pCreature->m_vPosition.y;
    pPath[2] = vTarget.x;
    pPath[3] = vTarget.y;
    if (pCreature->m_pFollowInfo->m_nWayPoints) {
        delete[] pCreature->m_pFollowInfo->m_pWayPoints;
        pCreature->m_pFollowInfo->m_pWayPoints = NULL;
    }
    pCreature->m_pFollowInfo->m_nWayPoints = 4;
    pCreature->m_pFollowInfo->m_pWayPoints = pPath;
    pCreature->m_pFollowInfo->m_nCurrentWayPoint = 0;
    pCreature->m_nAIState = 4;
    pCreature->WalkUpdateLocation_QuickWalk(1);
    CGameEffect* pNew = new CGameEffect(pEffect);
    pNew->m_nType = 8;
    pNew->SetDurationType(g_nDurationTemporary);
    pNew->SetInteger(0, 9);
    pCreature->ApplyEffect(pNew, 0, 0);
    }
    }
    return 0;
}

class CServerExoAppJump {
public:
    int TestSafeLocation(unsigned long oidArea, const Vector& vPosition);  // 0x004AEB50
};

class CSWSCombatRoundJump {
public:
    char pad_0x0[0x9b8];
    int m_nAttackRange9B8;                      // 0x9b8 (placeholder name)
};

class CSWSCreatureJump : public CSWSCreature {
public:
    void SetLockOrientationToObject(unsigned long oidTarget, int bArg);  // 0x004F34A0
    float DesiredAttackRange(unsigned long oidTarget, int nArg);  // 0x004F1310
    int AddMoveToPointActionToFront(unsigned short nGroup, const Vector& vTarget,
                                    unsigned long oidArea, unsigned long oidTarget, int bRun,
                                    float fRange, float fTolerance, int a7, int a8, int a9,
                                    int a10, int a11, int a12, const Vector& vOffset,
                                    unsigned long a14, unsigned long a15, int a16);  // 0x004F8A50
    void WalkUpdateLocation_QuickWalk(int bArg);  // 0x0051D9C0
};

// FUNCTION: SWKOTOR 0x004DE0A0
// NONMATCH (104): same flow and x87 sequence; caster/target-position swap
// ebx/edi, the scratch registers rotate and the push-vector multiply order differs.
int CSWSEffectListHandler::OnApplyForceJumpDelayed(CSWSObject* pObject, CGameEffect* pEffect,
                                                   int bLoadingGame)
{
    CSWSCreatureJump* pCaster = (CSWSCreatureJump*)pObject->AsSWSCreature();
    if (pCaster) {
    CSWSCreatureJump* pTarget = (CSWSCreatureJump*)GetServerExoApp()->GetCreatureByGameObjectID(
        pEffect->GetObjectID(0));
    if (pTarget) {
    pCaster->SetLockOrientationToObject(pTarget->m_idSelf, 0);
    Vector* pTargetPos = &pTarget->m_vPosition;
    Vector vDirection = Normalize(*pTargetPos - pCaster->m_vPosition);
    Vector vLand = *pTargetPos - vDirection * pCaster->DesiredAttackRange(pTarget->m_idSelf, 1);
    if (!((CServerExoAppJump*)g_pAppManager->m_pServerExoApp)->TestSafeLocation(pTarget->m_idSelf, vLand)) {
        float fRange = pCaster->DesiredAttackRange(
            pTarget->m_idSelf, ((CSWSCombatRoundJump*)pCaster->m_pCombatRound)->m_nAttackRange9B8);
        vLand = *pTargetPos;
        Vector vPush = fRange * vDirection + *pTargetPos;
        if (pTarget->m_pFollowInfo->m_nWayPoints) {
            delete[] pTarget->m_pFollowInfo->m_pWayPoints;
            pTarget->m_pFollowInfo->m_pWayPoints = NULL;
        }
        float* pPath = new float[4];
        pPath[0] = pTargetPos->x;
        pPath[1] = pTarget->m_vPosition.y;
        pPath[2] = vPush.x;
        pPath[3] = vPush.y;
        Vector vZero(0.0f, 0.0f, 0.0f);
        pTarget->AddMoveToPointActionToFront(0xffff, vPush, pTarget->m_oidArea, 0x7f000000, 0,
                                             0.0f, 0.0f, 0, 0, 0, 0, 0, 0, vZero, 0, 0, 0);
        pTarget->m_nAIState = 6;
        pTarget->m_pFollowInfo->m_nWayPoints = 4;
        pTarget->m_pFollowInfo->m_pWayPoints = pPath;
        pTarget->m_pFollowInfo->m_nCurrentWayPoint = 0;
        pTarget->WalkUpdateLocation_QuickWalk(1);
        CGameEffect* pNew = new CGameEffect(pEffect);
        pNew->m_nType = 8;
        pNew->SetDurationType(g_nDurationTemporary);
        pNew->SetInteger(0, 0xd);
        pTarget->ApplyEffect(pNew, 0, 0);
    }
    if (pCaster->m_pFollowInfo->m_nWayPoints) {
        delete[] pCaster->m_pFollowInfo->m_pWayPoints;
        pCaster->m_pFollowInfo->m_pWayPoints = NULL;
    }
    float* pPath = new float[4];
    pPath[0] = pCaster->m_vPosition.x;
    pPath[1] = pCaster->m_vPosition.y;
    pPath[2] = vLand.x;
    pPath[3] = vLand.y;
    pCaster->m_pFollowInfo->m_nWayPoints = 4;
    pCaster->m_pFollowInfo->m_pWayPoints = pPath;
    pCaster->m_pFollowInfo->m_nCurrentWayPoint = 0;
    pCaster->m_nAIState = 5;
    pCaster->WalkUpdateLocation_QuickWalk(1);
    }
    }
    return 0;
}
