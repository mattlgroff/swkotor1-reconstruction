// CSWSObject::SpellCastAndImpact (0x004CDF50).
#include "i/sws_object.h"

class CSWSpell {
public:
    int GetAdjustedForcePoints(unsigned long nCasterLevel, int bPlayer);  // 0x0059B8A0
    char pad_0x0[0x34];
    CExoString m_sScript;                           // 0x34 (placeholder name)
    char pad_0x3c[0x14e - 0x3c];
    unsigned char m_nCategory;                      // 0x14e (placeholder name)
};

// 0x2c bytes: the spell-impact event record.
struct CSWSSpellImpactEvent {
    CSWSSpellImpactEvent() : m_vTarget(0.0f, 0.0f, 0.0f) {}
    unsigned long m_nSpellId;                       // 0x00
    unsigned long m_oidCaster;                      // 0x04
    unsigned long m_oidTarget;                      // 0x08
    unsigned long m_oidItem;                        // 0x0c
    Vector m_vTarget;                               // 0x10
    CExoString m_sScript;                           // 0x1c
    unsigned long m_oidArea;                        // 0x24
    unsigned long m_n28;                            // 0x28
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent,
                          void* pEventData);        // 0x004B08D0
};

extern const unsigned long EVENT_SPELL_IMPACT;      // 0x007441D4

// FUNCTION: SWKOTOR 0x004CDF50
void CSWSObject::SpellCastAndImpact(unsigned long nSpellId, const Vector& vTargetPosition,
                                    unsigned long oidTarget, unsigned char nMultiClass,
                                    unsigned long oidItem, int bSpellCountered,
                                    int bCounteringSpell, unsigned char nProjectilePathType,
                                    int bInstantSpell)
{
    CExoString sUnused;
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpellId);
    if (pSpell) {
        unsigned char nProjectileType = nProjectilePathType ? 7 : 6;
        if (!m_bSpellImpactQueued) {
            if (!bInstantSpell && AsSWSCreature()) {
                CSWCCMessageData* pData = new CSWCCMessageData();
                pData->SetObjectID(0, m_idSelf);
                pData->SetObjectID(1, oidItem);
                pData->SetObjectID(2, oidTarget);
                pData->SetInteger(0, nSpellId);
                pData->SetInteger(1, pSpell->m_nCategory);
                pData->SetInteger(2, pSpell->GetAdjustedForcePoints(
                                         AsSWSCreature()->m_pStats->m_nCasterLevel,
                                         AsSWSCreature()->m_bPlayerCharacter));
                CSWSCreatureStats* pStats = AsSWSCreature()->m_pStats;
                pData->SetInteger(3, (short)(pStats->m_nForcePointsBonus + pStats->m_nForcePoints));
                BroadcastSpellData(pSpell, pData);
                delete pData;
            }
            CSWSCreature* pTarget =
                g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidTarget);
            if (pTarget) {
                BroadcastSafeProjectile(m_idSelf, oidTarget, m_vPosition, pTarget->m_vPosition,
                                        m_nLastSpellProjectileTime, nProjectileType, nSpellId, 0,
                                        nProjectilePathType, 0, 0);
            } else {
                BroadcastSafeProjectile(m_idSelf, oidTarget, m_vPosition, m_vLastSpellTarget,
                                        m_nLastSpellProjectileTime, nProjectileType, nSpellId, 0,
                                        nProjectilePathType, 0, 0);
            }
            CSWSSpellImpactEvent* pEvent = new CSWSSpellImpactEvent();
            pEvent->m_nSpellId = nSpellId;
            pEvent->m_sScript = pSpell->m_sScript;
            pEvent->m_oidTarget = oidTarget;
            pEvent->m_vTarget = vTargetPosition;
            pEvent->m_oidCaster = m_idSelf;
            pEvent->m_oidItem = oidItem;
            if (GetArea()) {
                pEvent->m_oidArea = GetArea()->GetId();
            } else {
                pEvent->m_oidArea = OBJECT_INVALID_ID;
            }
            g_pAppManager->m_pServerExoApp->GetServerAIMaster()->AddEventDeltaTime(
                0, m_nLastSpellProjectileTime, m_idSelf, m_idSelf, EVENT_SPELL_IMPACT, pEvent);
        }
        m_bSpellImpactQueued = 1;
    }
}
