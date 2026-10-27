// CSWSEffectListHandler::OnApplyKnockdown. Separate from effect_list_handler7.cpp
// because the original calls the out-of-line Vector subtraction here, where
// the jump/push handlers there inline it.
#include "a/effect_list_handler.h"

Vector operator-(const Vector& a, const Vector& b);  // 0x00423BC0
Vector Normalize(const Vector& v);              // 0x004AB180

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// FUNCTION: SWKOTOR 0x004D99B0
// NONMATCH (14): the facing dot product is summed y, z, x here and z, y, x
// in the original (source order of the terms makes no difference).
int CSWSEffectListHandler::OnApplyKnockdown(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    unsigned long oidCreator = pEffect->GetCreator();
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
    CSWSCreature* pCreator = GetServerExoApp()->GetCreatureByGameObjectID(oidCreator);
    if (pObject->AsSWSCreature()->GetStats()->GetEffectImmunity(0x1c, pCreator)) {
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetObjectID(0, pObject->GetId());
        if (pCreator) {
            CSWCCMessageData* pCopy = new CSWCCMessageData;
            pData->CopyTo(pCopy);
            pCreator->SendFeedbackData(0x81, pCopy);
        }
        pObject->AsSWSCreature()->SendFeedbackData(0x81, pData);
        return 1;
    }
    if (pCreature->GetF8()) {
        return 1;
    }
    if (pCreature->GetIsInConversation()) {
        return 0;
    }
    if (pCreature->GetIsPossessed()) {
        return 0;
    }
    if (pCreature->GetDeadTemp()) {
        return 0;
    }
    pCreature->m_pCombatRound->SetRoundPaused(1, oidCreator);
    pCreature->m_pCombatRound->SetPauseTimer(0, 1);
    CGameEffect* pNew = new CGameEffect(pEffect);
    pNew->m_fDuration = pEffect->GetDuration() + 1.5f;
    pNew->m_nType = 0x17;
    pNew->SetInteger(0, -0x101);
    pObject->ApplyEffect(pNew, bLoadingGame, 0);
    pObject->m_nE8 = 0;
    CGameObject* pFound;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreator, &pFound) ==
        g_nGameObjectFound) {
        CSWSCreature* pAttacker = pFound->AsSWSCreature();
        Vector vDirection;
        vDirection = Normalize(pAttacker->m_vPosition - pCreature->m_vPosition);
        if (!(vDirection * Normalize(pCreature->m_vOrientation) >= 0.707)) {
            pCreature->SendFeedbackMessage(0x2715);
            return 0;
        }
    }
    pCreature->SendFeedbackMessage(0x2717);
    }
    return 0;
}

class CSWParty {
public:
    int IsPartyMember(unsigned long oid);       // 0x006349C0

    int m_nMembers;                             // 0x00 (placeholder name)
};

class CClientExoApp {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);  // 0x005EDA50
    unsigned long GetLastTarget();              // 0x005EDD80
    CSWParty* GetSWParty();                     // 0x005ED8B0
    void AddFloatyText(unsigned long oid, unsigned char nType, int nValue);  // 0x005EDEA0
};

class CSWSCreatureHeal : public CSWSCreature {
public:
    // the virtual 0x98 slot of CSWSObject (CSWSCreature hides the name)
    short GetMaxHitPointsV(int bArg) { CSWSObject* pThis = this; return pThis->GetMaxHitPoints(bArg); }
    short GetMaxForcePoints();                  // 0x004FD490
    void SetCurrentHitPoints(int nHP);          // 0x004CC280
};

// FUNCTION: SWKOTOR 0x004E0750
// NONMATCH (21): the applied-effect scan loads the type word into dx; the
// original compares it in memory twice (ja/jne). Scratch registers rotate.
int CSWSEffectListHandler::OnApplyHeal(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    CSWSCreatureHeal* pCreature = (CSWSCreatureHeal*)pObject->AsSWSCreature();
    if (!pEffect) {
        return 1;
    }
    int nAmount = pEffect->GetInteger(0);
    if (pEffect->GetInteger(1) == 0x36) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        if (!pStats) {
            return 1;
        }
        int nCurrent = pStats->GetCurrentHitPoints();
        int nMax = pCreature->GetMaxForcePoints();
        if (nCurrent + nAmount > nMax) {
            nAmount = nMax - nCurrent;
        }
        pCreature->GetStats()->m_nHitPoints124 = (short)(nAmount + nCurrent);
        CSWCCMessageData* pData = new CSWCCMessageData;
        pData->SetObjectID(0, pCreature->m_idSelf);
        pData->SetInteger(0, nAmount);
        pCreature->SendFeedbackData(0xe4, pData);
        return 1;
    }
    if (pCreature->GetIsInConversation()) {
        return 1;
    }
    if (pCreature->GetDeadTemp()) {
        return 1;
    }
    int bPossessed = pCreature->GetIsPossessed();
    int nNewHP = pCreature->GetCurrentHitPoints(1) + nAmount;
    if (nNewHP > pCreature->GetMaxHitPointsV(1)) {
        nAmount = pCreature->GetMaxHitPointsV(1) - pCreature->GetCurrentHitPoints(1);
    }
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    unsigned long oidClient = pClient->ServerToClientObjectId(pCreature->m_idSelf);
    if (pClient->GetLastTarget() == oidClient ||
        pClient->GetSWParty()->IsPartyMember(oidClient) != -1) {
        pClient->AddFloatyText(oidClient, 1, nAmount);
    }
    pCreature->SetCurrentHitPoints(pCreature->GetCurrentHitPoints(1) + nAmount);
    CSWCCMessageData* pData = new CSWCCMessageData;
    CSWCCMessageData* pCopy = new CSWCCMessageData;
    pData->SetObjectID(0, pCreature->m_idSelf);
    pData->SetInteger(0, nAmount);
    pData->CopyTo(pCopy);
    pCreature->SendFeedbackData(0x97, pData);
    CSWSCreature* pHealer = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetCreator());
    if (pHealer && pHealer != pCreature) {
        pHealer->SendFeedbackData(0x97, pCopy);
    } else {
        delete pCopy;
    }
    int i = 0;
    while (i < pCreature->m_appliedEffects.num) {
        CGameEffect* pApplied = pCreature->m_appliedEffects.element[i];
        if (pApplied->GetType() > 0x54) {
            break;
        }
        if (pApplied->GetType() == 0x54 && pApplied->GetDurationType() == g_nDurationTemporary) {
            pCreature->RemoveEffectById(pApplied->GetID());
            i = 0;
        } else {
            i++;
        }
    }
    if (bPossessed == 1 && !pCreature->GetIsPossessed()) {
        CGameEffect* pNew = new CGameEffect(1);
        pNew->m_nType = 4;
        pNew->SetDurationType(g_nDurationInstant);
        pCreature->ApplyEffect(pNew, bLoadingGame, 0);
    }
    return 1;
}

class CResRef {
public:
    CResRef(const CExoString& sName);           // 0x00406D60

    char m_resRef[16];
};

class CSWSArea;

class CSWSAreaSummon {
public:
    void ApplyEffect(CGameEffect* pEffect, Vector* pPosition, Vector* pOrientation);  // 0x0050C6B0
    float ComputeHeight(const Vector& vPosition, int bArg2, class CSWSRoom** ppRoom);  // 0x004BC380
    int ComputeSafeLocation(const Vector& vPosition, float fRange, CSWSCreatureFollowInfo* pInfo,
                            int bArg4, Vector* pResult);  // 0x004BE860
};

class CServerExoAppSummon {
public:
    CSWSAreaSummon* GetAreaByGameObjectID(unsigned long oid);  // 0x004AE780
};

class CVirtualMachine {
public:
    int RunScript(CExoString* psFileName, unsigned long oidSelf, int bArg);  // 0x005D0FC0
};
extern CVirtualMachine* g_pVirtualMachine;      // 0x007A3A00

// a whole server creature (size 0xac8)
class CSWSCreatureSummon : public CSWSCreature {
public:
    CSWSCreatureSummon(unsigned long oidSelf, int bArg);  // 0x004F7A10
    void SetInParty(int bInParty, int bArg);    // 0x004FDB20
    int LoadFromTemplate(const CResRef& cResRef, int bArg);  // 0x005026D0
    void AddToArea(CSWSAreaSummon* pArea, float fX, float fY, float fZ, int bArg5, int bArg6);  // 0x004FA100
    CSWSCreatureFollowInfo* GetPathfindInfo() { return m_pFollowInfo; }

    char pad_0xaa0[0xac8 - 0xaa0];
};

// FUNCTION: SWKOTOR 0x004D8A40
int CSWSEffectListHandler::OnApplySummonParty(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    if (g_pAppManager->m_pClientExoApp->GetSWParty()->m_nMembers >= 3) {
        return 1;
    }
    CSWSCreature* pTarget = pObject->AsSWSCreature();
    if (pEffect && pTarget) {
        CSWSCreatureSummon* pNew = new CSWSCreatureSummon(0x7f000000, 0);
        pNew->SetInParty(1, 1);
        CResRef cResRef(pEffect->GetString(0));
        pNew->LoadFromTemplate(cResRef, 0);
        if (pEffect->GetObjectID(0) == 0x7f000000) {
            pEffect->SetObjectID(0, pObject->m_oidArea);
            pEffect->SetFloat(0, pObject->m_vPosition.x);
            pEffect->SetFloat(1, pObject->m_vPosition.y);
            pEffect->SetFloat(2, pObject->m_vPosition.z);
        }
        CSWSAreaSummon* pArea = ((CServerExoAppSummon*)GetServerExoApp())
                                    ->GetAreaByGameObjectID(pEffect->GetObjectID(0));
        if (pArea) {
        float fX = pEffect->GetFloat(0);
        float fY = pEffect->GetFloat(1);
        Vector vPosition(fX, fY, pEffect->GetFloat(2));
        Vector vSafe(0.0f, 0.0f, 0.0f);
        float fZ;
        if (pArea->ComputeSafeLocation(vPosition, 10.0f, pNew->GetPathfindInfo(), 0, &vSafe) == 1) {
            fX = vSafe.x;
            fY = vSafe.y;
            fZ = pArea->ComputeHeight(vSafe, 1, NULL);
        } else {
            fX = vPosition.x;
            fY = vPosition.y;
            fZ = vPosition.z;
        }
        pNew->AddToArea(pArea, fX, fY, fZ, 0, 0);
        pNew->m_bSpawned34C = 1;
        g_pVirtualMachine->RunScript(&pNew->m_sScriptOnSpawn, pNew->m_idSelf, 1);
        pEffect->SetObjectID(1, pNew->m_idSelf);
        } else if (pNew) {
            delete pNew;
        }
    }
    return 0;
}

class CScriptEvent {
public:
    CScriptEvent();                             // 0x004D7540
    void SetFloat(int nIndex, float fValue);    // 0x004D7720
    void SetObjectID(int nIndex, unsigned long oid);  // 0x004D7780

    char pad_0x0[0x34];
};

extern const unsigned long EVENT_SUMMON_CREATURE;  // 0x00744214 (0x18, placeholder name)

// FUNCTION: SWKOTOR 0x004D8680
// NONMATCH (74): same flow; the original keeps 0 in ebp and -1 in edi and
// holds the object in ebx, which cl allocates differently (16 bytes longer).
int CSWSEffectListHandler::OnApplySummonCreature(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    if (bLoadingGame != 1) {
    CSWSCreature* pTarget = pObject->AsSWSCreature();
    if (pEffect && pTarget) {
        CSWSCreatureSummon* pNew = new CSWSCreatureSummon(0x7f000000, 0);
        if (pEffect->GetObjectID(0) == 0x7f000000) {
            pEffect->SetObjectID(0, pObject->m_oidArea);
            pEffect->SetFloat(0, pObject->m_vPosition.x);
            pEffect->SetFloat(1, pObject->m_vPosition.y);
            pEffect->SetFloat(2, pObject->m_vPosition.z);
        }
        CSWSAreaSummon* pArea = ((CServerExoAppSummon*)GetServerExoApp())
                                    ->GetAreaByGameObjectID(pEffect->GetObjectID(0));
        if (pArea) {
            float fX = pEffect->GetFloat(0);
            float fY = pEffect->GetFloat(1);
            float fZ = pEffect->GetFloat(2);
            float fDelay = 0.0f;
            if (pEffect->GetFloat(3) > 0.0f) {
                fDelay = pEffect->GetFloat(3);
            }
            CResRef cResRef(pEffect->GetString(0));
            pNew->LoadFromTemplate(cResRef, 0);
            Vector vPosition(fX, fY, fZ);
            Vector vSafe(0.0f, 0.0f, 0.0f);
            if (pArea->ComputeSafeLocation(vPosition, 20.0f, pNew->m_pFollowInfo, 0, &vSafe) == 1) {
                fX = vSafe.x;
                fY = vSafe.y;
                fZ = pArea->ComputeHeight(vSafe, 0, NULL);
            } else {
                fX = vPosition.x;
                fY = vPosition.y;
                fZ = vPosition.z;
            }
            int nVisual = pEffect->GetInteger(0);
            if (nVisual != -1) {
                CGameEffect* pVisual = new CGameEffect(1);
                pVisual->m_nType = 0x1e;
                pVisual->SetDurationType(g_nDurationInstant);
                pVisual->SetInteger(0, nVisual);
                Vector vVisual(fX, fY, fZ);
                pArea->ApplyEffect(pVisual, &vVisual, &pTarget->m_vOrientation);
            }
            unsigned long oidNew = pNew->m_idSelf;
            CScriptEvent* pEvent = new CScriptEvent;
            pEvent->SetFloat(0, fX);
            pEvent->SetFloat(1, fY);
            pEvent->SetFloat(2, fZ);
            pEvent->SetObjectID(0, oidNew);
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, (int)fDelay * 1000, pObject->m_idSelf, pObject->m_idSelf, EVENT_SUMMON_CREATURE,
                pEvent);
            pEffect->SetObjectID(1, oidNew);
        } else if (pNew) {
            delete pNew;
        }
    }
    }
    return 0;
}
