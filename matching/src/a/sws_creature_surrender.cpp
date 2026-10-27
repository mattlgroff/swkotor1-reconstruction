// CSWSCreature::SurrenderToEnemies (nwscreatureai.cpp).
#include "a/sws_creature.h"

extern const float PROJECTILE_BROADCAST_RANGE;      // 0x00747440

class CSWSFaction {
public:
    void AddMember(unsigned long oidCreature, int bArg); // 0x005BFA70
};

class CFactionManagerList {
public:
    CSWSFaction* m_pFactions;                       // 0x00
};

class CSWSObject {
public:
    void ClearAllEffects(int bArg);                 // 0x004D0940
};

// FUNCTION: SWKOTOR 0x00518990
void CSWSCreature::SurrenderToEnemies(unsigned long nFlag)
{
    CancelCombat(0);
    reinterpret_cast<CSWSObject*>(this)->ClearAllEffects(nFlag);
    Vector vPosition = m_vPosition;
    float fRange = PROJECTILE_BROADCAST_RANGE;
    float fMaxX = vPosition.x + fRange;
    CSWSArea* pArea = GetArea();
    if (pArea) {
        int nIndex;
        if (pArea->GetFirstObjectIndiceByX(&nIndex, vPosition.x - fRange) == 1) {
            for (; nIndex < pArea->m_lPlayers.num; nIndex++) {
                CSWSCreature* pOther =
                    GetServerExoApp()->GetCreatureByGameObjectID(pArea->m_lPlayers.element[nIndex]);
                if (!pOther || m_idSelf == pOther->m_idSelf) continue;
                if (pOther->m_vPosition.x > fMaxX) break;
                Vector vDelta(pOther->m_vPosition.x - vPosition.x, pOther->m_vPosition.y - vPosition.y,
                              pOther->m_vPosition.z - vPosition.z);
                if (vDelta.MagnitudeSquared() <= fRange * fRange &&
                    GetReputation(pOther->m_idSelf, 0) <= 10) {
                    pOther->CancelCombat(0);
                    reinterpret_cast<CSWSObject*>(pOther)->ClearAllEffects(nFlag);
                }
            }
        }
    }
    if (GetServerExoApp()->GetFactionManager() &&
        reinterpret_cast<CFactionManagerList*>(GetServerExoApp()->GetFactionManager())->m_pFactions) {
        unsigned long oidSelf = m_idSelf;
        reinterpret_cast<CFactionManagerList*>(GetServerExoApp()->GetFactionManager())->m_pFactions->AddMember(oidSelf, 0);
    }
}
