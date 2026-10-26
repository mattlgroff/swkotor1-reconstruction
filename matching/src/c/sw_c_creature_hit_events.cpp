#include "c/client_exo_app.h"

struct CHitEventPart {
    char pad_0x0[4];
    unsigned long m_oidOwner;                         // 0x04
};

// FUNCTION: SWKOTOR 0x00610E10
void HitClashEvent(void*, char*, CHitEventPart* pUser)
{
    CExoString sEvent("Clash");
    if (pUser == 0) return;
    CAppManager* pManager = g_pAppManager;
    CSWCCreature* pTarget = pManager->GetClientExoApp()->GetCreatureByGameObjectID(pUser->m_oidOwner);
    if (pTarget != 0) {
        pTarget->PlaySwingSound(sEvent, (CSWCCombatAttackData*)pUser, 2);
    }
}

// FUNCTION: SWKOTOR 0x00610E90
void HitContactEvent(void*, char*, CHitEventPart* pUser)
{
    if (pUser == 0) return;
    CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pUser->m_oidOwner);
    if (pTarget != 0) pTarget->ShowLightSaberContactVisual((int)pUser);
}

// FUNCTION: SWKOTOR 0x00610EC0
void HitParryEvent(void*, char*, CHitEventPart* pUser)
{
    CExoString sEvent("Parry");
    if (pUser == 0) return;
    CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pUser->m_oidOwner);
    if (pTarget != 0) {
        pTarget->PlaySwingSound(sEvent, (CSWCCombatAttackData*)pUser, 2);
    }
}

// FUNCTION: SWKOTOR 0x00610C90
void SwingShortEvent(void*, char*, CHitEventPart* pUser)
{
    CExoString sEvent("SwingShort");
    if (pUser == 0) return;
    CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pUser->m_oidOwner);
    if (pTarget != 0) pTarget->PlaySwingSound(sEvent, (CSWCCombatAttackData*)pUser, 3);
}

// FUNCTION: SWKOTOR 0x00610D10
void SwingLongEvent(void*, char*, CHitEventPart* pUser)
{
    CExoString sEvent("SwingLong");
    if (pUser == 0) return;
    CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pUser->m_oidOwner);
    if (pTarget != 0) pTarget->PlaySwingSound(sEvent, (CSWCCombatAttackData*)pUser, 3);
}

// FUNCTION: SWKOTOR 0x00610D90
void SwingTwirlEvent(void*, char*, CHitEventPart* pUser)
{
    CExoString sEvent("SwingTwirl");
    if (pUser == 0) return;
    CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pUser->m_oidOwner);
    if (pTarget != 0) pTarget->PlaySwingSound(sEvent, (CSWCCombatAttackData*)pUser, 3);
}
