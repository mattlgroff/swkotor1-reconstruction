// CSWSEffectListHandler sanctuary, resurrection and teleport handlers.
#include "a/effect_list_handler.h"

// minimal views for this file (placeholder names)
class CSWSArea6 {
public:
    char pad_0x0[0x190];
    CExoArrayList<unsigned long> m_aCreatures;  // 0x190
};

// CSWSCreature::GetFromList920 element (placeholder name)
struct CSWSCreatureVisibility {
    unsigned long m_oid;                        // 0x00
    unsigned char m_nFlags;                     // 0x04
};

// a local list whose storage is released on scope exit
class CObjectIdList6 {
public:
    CObjectIdList6() { num = 0; array_size = 0; element = NULL; }
    ~CObjectIdList6() { delete[] element; }
    void Add(unsigned long oid);                // 0x00671C00 (folded instantiation)

    unsigned long* element;
    int num;
    int array_size;
};

// CServerExoAppInternal2::m_lInvisible with the inline list helpers
class CSanctuaryList {
public:
    void AddInvisible(unsigned long oid);       // 0x006449D0
    int Contains(unsigned long oid)
    {
        int n = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == oid) {
                n++;
            }
        }
        return n;
    }
    void Remove(unsigned long oid)
    {
        int i;
        for (i = 0; i < num; i++) {
            if (element[i] == oid) {
                break;
            }
        }
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }

    unsigned long* element;
    int num;
    int array_size;
};

class CSWSModule6 {
public:
    char pad_0x0[0x13c];
    CSanctuaryList m_lSanctuary;                // 0x13c (placeholder name)
};

class CSWSObject6 : public CSWSObject {
public:
    CSWSArea6* GetArea();                       // 0x004CB120
};

class CSWSCreature6 : public CSWSCreature {
public:
    CSWSCreatureVisibility* GetFromList920(unsigned long oid);  // 0x00517A20
    unsigned char SavingThrowRoll(unsigned char nSaveType, unsigned short nDC,
                                  unsigned short nSaveVsType, unsigned long oidVersus,
                                  int bSendFeedback, unsigned short nFeat, int bFromCombat);  // 0x005B92B0
    void PacifyCreature(unsigned long oid);     // 0x004FDA20
    void OnIdChanged(unsigned long oid);        // 0x00517960
    virtual short GetMaxHitPoints(int bArg);    // 0x98
    short GetMaxForcePoints();                  // 0x004FD490
    void SetCurrentHitPoints(int nHP);          // 0x004CC280
};

class CSWSMessage6 {
public:
    int SendServerToPlayerDestroyDeathGUI(unsigned long nPlayerId);  // 0x0056B320
};

class CSWSCreatureR : public CSWSCreature {
public:
    void SetCurrentHitPoints(int nHP);          // 0x004CC280
    void ClearAllActions(int bArg);             // 0x004CCD80
    void UpdateAmbientAnimationState();         // 0x004F28D0
};

// CServerExoApp::GetClientObjectByObjectID result (placeholder layout)
class CSWSPlayer6 {
public:
    unsigned long GetGameObjectID() { return m_oidPC; }

    char pad_0x0[0x38];
    unsigned long m_oidPC;                      // 0x38 (placeholder name)
};

extern const unsigned char CLASS_TYPE_JEDI_GUARDIAN;    // 0x0074B31C
extern const unsigned char CLASS_TYPE_JEDI_CONSULAR;    // 0x0074B31D
extern const unsigned char CLASS_TYPE_JEDI_SENTINEL;    // 0x0074B31E

class CResRef6 {
public:
    CResRef6();                                 // 0x00405ED0
    CResRef6 operator=(const CExoString& sName);  // 0x004061F0

    char m_resRef[16];
};

// an entry of CSWSCreature::m_lstEffectIcons (placeholder names)
struct CEffectIcon6 {
    short m_nIcon;                              // 0x00
    CResRef6 m_resIcon;                         // 0x02
    int m_bGood;                                // 0x14
    unsigned short m_nPriority;                 // 0x18
    int m_bGoodFound;                           // 0x1c
};

class CServerExoApp6 {
public:
    CSWSModule6* GetModule();                   // 0x004AE6B0
    CSWSMessage6* GetNWSMessage();              // 0x004AEDB0
    CSWSPlayer6* GetClientObjectByObjectID(unsigned long oid);  // 0x004AE910
};

// FUNCTION: SWKOTOR 0x004E3D10
int CSWSEffectListHandler::OnApplySanctuary(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CObjectIdList6 lUnused;
        CSWSCreature6* pCreature = (CSWSCreature6*)pObject->AsSWSCreature();
        CSWSArea6* pArea = ((CSWSObject6*)pCreature)->GetArea();
        if (pArea) {
            unsigned char nDC = (unsigned char)pEffect->GetInteger(4);
            for (int i = 0; i < pArea->m_aCreatures.num; i++) {
                CSWSCreature6* pOther = (CSWSCreature6*)GetServerExoApp()->GetCreatureByGameObjectID(
                    pArea->m_aCreatures.element[i]);
                if (pOther && pOther->GetId() != pCreature->GetId()) {
                    CSWSCreatureVisibility* pVis = pOther->GetFromList920(pCreature->GetId());
                    if (pVis) {
                        if (!pOther->SavingThrowRoll(3, nDC, 0, pCreature->GetId(), 1, 0, 0)) {
                            pCreature->PacifyCreature(pOther->GetId());
                            pVis->m_nFlags = (pVis->m_nFlags & ~8) | 4;
                        } else {
                            pVis->m_nFlags = (pVis->m_nFlags & ~4) | 8;
                        }
                    }
                }
            }
            CServerExoAppInternal2* pInternal = GetServerExoApp()->GetInternal6B0();
            pInternal->m_lInvisible.AddInvisible(pCreature->GetId());
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E3E70
// NONMATCH: the original keeps the creature in its own stack slot and walks
// the effect list through the pObject argument slot (frame 0x1c, 0x14 here).
int CSWSEffectListHandler::OnRemoveSanctuary(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        int bOtherEffect = 0;
        int bOtherSanctuary = 0;
        CSWSCreature6* pCreature = (CSWSCreature6*)pObject->AsSWSCreature();
        CSWSArea6* pArea = ((CSWSObject6*)pCreature)->GetArea();
        if (pArea) {
            CObjectIdList6 lPacified;
            for (int i = 0; i < pArea->m_aCreatures.num; i++) {
                CSWSCreature6* pOther =
                    (CSWSCreature6*)GetServerExoApp()->GetCreatureByGameObjectID(
                        pArea->m_aCreatures.element[i]);
                if (pOther) {
                    CSWSCreatureVisibility* pVis = pOther->GetFromList920(pCreature->GetId());
                    if (pVis) {
                        unsigned char nState = (pVis->m_nFlags >> 2) & 3;
                        if (nState == 1) {
                            lPacified.Add(pOther->GetId());
                        } else if (nState == 2) {
                            pVis->m_nFlags &= 0xf3;
                        }
                    }
                }
            }
            int nCount = pCreature->m_appliedEffects.num;
            for (int j = pCreature->m_pStats->m_nInvisibilityStart; j < nCount; j++) {
                CGameEffect* pOther = pCreature->m_appliedEffects.element[j];
                unsigned short nType = pOther->m_nType;
                if (nType > 0x3f) {
                    break;
                }
                if ((nType == 0x2f || nType == 0x3f) && pEffect->m_nID != pOther->m_nID) {
                    bOtherEffect = 1;
                    if (nType == 0x3f) {
                        bOtherSanctuary = 1;
                    }
                }
            }
            if (!bOtherEffect) {
                CSWSModule6* pModule = ((CServerExoApp6*)GetServerExoApp())->GetModule();
                unsigned long oid = pCreature->GetId();
                if (pModule->m_lSanctuary.Contains(oid)) {
                    pModule->m_lSanctuary.Remove(oid);
                }
            }
            if (!bOtherSanctuary) {
                for (int k = 0; k < lPacified.num; k++) {
                    CSWSCreature6* pOther =
                        (CSWSCreature6*)GetServerExoApp()->GetCreatureByGameObjectID(
                            lPacified.element[k]);
                    if (pOther) {
                        pOther->OnIdChanged(pCreature->GetId());
                    }
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E13C0
int CSWSEffectListHandler::OnApplyResurrection(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    if (!pObject->m_bF0) {
        return 1;
    }
    if (pObject->AsSWSCreature()) {
        CSWSCreatureR* pCreature = (CSWSCreatureR*)pObject->AsSWSCreature();
        if (pCreature->GetCurrentHitPoints(0) < 1) {
            pCreature->SetCurrentHitPoints(1);
        }
        pCreature->ClearAllActions(1);
        pCreature->m_b19C = 1;
        pCreature->m_nAIStateFlags = 0xffff;
        pCreature->m_nE8 = 1;
        pCreature->UpdateAmbientAnimationState();
        CSWSClient* pClient = GetServerExoApp()->GetClientObjectByObjectID(pObject->GetId());
        if (pClient) {
            CSWSMessage6* pMessage = ((CServerExoApp6*)GetServerExoApp())->GetNWSMessage();
            pMessage->SendServerToPlayerDestroyDeathGUI(pClient->m_nPlayerId);
        }
        for (int i = 0; i < pCreature->m_appliedEffects.num; i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x39) {
                break;
            }
            // re-indexing the list (instead of pOther) keeps 0x39 in a register
            if (pCreature->m_appliedEffects.element[i]->m_nType == 0x39 &&
                pOther->GetDurationType() == g_nDurationTemporary) {
                pCreature->RemoveEffectById(pOther->GetID());
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DEE90
int CSWSEffectListHandler::OnApplyPureGoodPowers(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    CSWSCreature6* pCreature = (CSWSCreature6*)pObject->AsSWSCreature();
    if (!pCreature) {
        return 1;
    }
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    if (!pStats) {
        return 1;
    }
    CSWSPlayer6* pPlayer =
        ((CServerExoApp6*)GetServerExoApp())->GetClientObjectByObjectID(pCreature->GetId());
    if (pPlayer && pPlayer->GetGameObjectID() == pCreature->GetId()) {
        for (unsigned char i = 0; i < pStats->m_nNumMultiClasses; i++) {
            unsigned char nClass = pStats->GetClass(i);
            if (nClass == CLASS_TYPE_JEDI_CONSULAR) {
                int nBefore = pCreature->GetMaxForcePoints();
                CGameEffect* pNew = new CGameEffect(pEffect);
                pNew->m_nType = 0x24;
                pNew->SetInteger(0, 5);
                pNew->SetInteger(1, 3);
                pCreature->ApplyEffect(pNew, 0, 0);
                int nAfter = pCreature->GetMaxForcePoints();
                pStats->m_nHitPoints124 += nAfter - nBefore;
            } else if (nClass == CLASS_TYPE_JEDI_GUARDIAN) {
                CGameEffect* pNew = new CGameEffect(pEffect);
                pNew->m_nType = 0x24;
                pNew->SetInteger(0, 0);
                pNew->SetInteger(1, 3);
                pCreature->ApplyEffect(pNew, 0, 0);
            } else if (nClass == CLASS_TYPE_JEDI_SENTINEL) {
                short nBefore = pCreature->GetMaxHitPoints(0);
                CGameEffect* pNew = new CGameEffect(pEffect);
                pNew->m_nType = 0x24;
                pNew->SetInteger(0, 2);
                pNew->SetInteger(1, 3);
                pCreature->ApplyEffect(pNew, 0, 0);
                short nGain = pCreature->GetMaxHitPoints(0) - nBefore;
                pCreature->SetCurrentHitPoints(pCreature->GetCurrentHitPoints(1) + nGain);
            }
        }
    }
    CGameEffect* pIcon = new CGameEffect(pEffect);
    pIcon->m_nType = 0x43;
    pIcon->SetInteger(0, 0x3c);
    pCreature->ApplyEffect(pIcon, 0, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004DF160
int CSWSEffectListHandler::OnApplyPureEvilPowers(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    CSWSCreature6* pCreature = (CSWSCreature6*)pObject->AsSWSCreature();
    if (!pCreature) {
        return 1;
    }
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    if (!pStats) {
        return 1;
    }
    CSWSPlayer6* pPlayer =
        ((CServerExoApp6*)GetServerExoApp())->GetClientObjectByObjectID(pCreature->GetId());
    if (pPlayer && pPlayer->GetGameObjectID() == pCreature->GetId()) {
        for (unsigned char i = 0; i < pStats->m_nNumMultiClasses; i++) {
            unsigned char nClass = pStats->GetClass(i);
            if (nClass == CLASS_TYPE_JEDI_CONSULAR) {
                pStats->m_nHitPoints124 += 0x32;
            } else if (nClass == CLASS_TYPE_JEDI_GUARDIAN) {
                CGameEffect* pNew = new CGameEffect(pEffect);
                pNew->m_nType = 0xd;
                pNew->SetInteger(0, 8);
                pNew->SetInteger(1, 3);
                pNew->SetInteger(2, g_pRules->m_nAA);
                pNew->SetInteger(5, 1);
                pCreature->ApplyEffect(pNew, 0, 0);
                pNew = new CGameEffect(pEffect);
                pNew->m_nType = 0xd;
                pNew->SetInteger(0, 8);
                pNew->SetInteger(1, 3);
                pNew->SetInteger(2, g_pRules->m_nAA);
                pNew->SetInteger(5, 2);
                pCreature->ApplyEffect(pNew, 0, 0);
            } else if (nClass == CLASS_TYPE_JEDI_SENTINEL) {
                CGameEffect* pNew = new CGameEffect(pEffect);
                pNew->m_nType = 0x16;
                pNew->SetInteger(0, 2);
                pNew->SetInteger(1, g_pRules->m_nAA);
                pCreature->ApplyEffect(pNew, 0, 0);
            }
        }
    }
    CGameEffect* pIcon = new CGameEffect(pEffect);
    pIcon->m_nType = 0x43;
    pIcon->SetInteger(0, 0x3d);
    pCreature->ApplyEffect(pIcon, 0, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E4790
int CSWSEffectListHandler::OnApplyEffectIcon(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (!pObject || !pObject->AsSWSCreature()) {
        return 1;
    }
    int nIcon = pEffect->GetInteger(0);
    if (pEffect->GetDurationType() == g_nDurationInstant) {
        return 1;
    }
    CExoArrayList<CEffectIcon6*>* pIcons =
        (CExoArrayList<CEffectIcon6*>*)&pObject->AsSWSCreature()->m_lstEffectIcons;
    if (!pIcons) {
        return 0;
    }
    int i;
    for (i = 0; i < pIcons->num; i++) {
        if (pIcons->element[i]->m_nIcon == (short)nIcon) {
            return 0;
        }
    }
    CEffectIcon6* pIcon = new CEffectIcon6;
    if (!pIcon) {
        return 1;
    }
    CExoString sResRef;
    int nPriority;
    bool bFailed = !g_pRules->m_p2DArrays->m_pEffectIconTable->GetCExoStringEntry(
        (short)nIcon, CExoString("IconResRef"), &sResRef);
    if (!bFailed) {
        bFailed = !g_pRules->m_p2DArrays->m_pEffectIconTable->GetINTEntry(
            (short)nIcon, CExoString("Priority"), &nPriority);
    }
    if (bFailed) {
        delete pIcon;
        return 1;
    }
    int bGood;
    pIcon->m_bGoodFound =
        g_pRules->m_p2DArrays->m_pEffectIconTable->GetINTEntry((short)nIcon, CExoString("Good"), &bGood);
    pIcon->m_nIcon = (short)nIcon;
    pIcon->m_resIcon = sResRef;
    pIcon->m_bGood = bGood != 0;
    pIcon->m_nPriority = (unsigned short)nPriority;
    for (i = 0; i < pIcons->num; i++) {
        if (pIcons->element[i]->m_nPriority > nPriority) {
            break;
        }
    }
    pIcons->Insert(pIcon, i);
    return 0;
}
