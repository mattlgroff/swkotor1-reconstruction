// CSWSObject::BroadcastSafeProjectile (0x004CDB00).
#include "i/sws_object.h"

class CSWSPlayer;

class CSWSProjectileMessage {
public:
    int SendServerToPlayerSafeProjectileProjectile(CSWSPlayer* pPlayer, unsigned long oidOriginator,
                                                   unsigned long oidTarget, Vector vOrigin,
                                                   Vector vTarget, unsigned long nDelay,
                                                   unsigned char nProjectileType, int nSpell,
                                                   unsigned char nAttackResult,
                                                   unsigned char nProjectilePath,
                                                   int bFlag);  // 0x0056E5F0
};

extern const float PROJECTILE_BROADCAST_RANGE;     // 0x00747440 (placeholder name)

// FUNCTION: SWKOTOR 0x004CDB00
void CSWSObject::BroadcastSafeProjectile(unsigned long oidOriginator, unsigned long oidTarget,
                                         const Vector& vOrigin, const Vector& vTarget,
                                         unsigned long nDelay, unsigned char nProjectileType,
                                         int nSpell, unsigned char nAttackResult,
                                         unsigned char nProjectilePath, int bFlag)
{
    if (GetAreaInline() && GetAreaInline()->GetCreatureList()) {
        CExoArrayList<unsigned long>* pList = GetArea()->GetCreatureList();
        for (int i = 0; i < pList->num; i++) {
            CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID((*pList)[i]);
            if (pCreature == NULL) {
                continue;
            }
            CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->m_pServerExoApp
                                      ->GetClientObjectByObjectID(pCreature->m_idSelf);
            if (pPlayer == NULL) {
                continue;
            }
            Vector vToOrigin = pCreature->m_vPosition - vOrigin;
            Vector vToTarget = pCreature->m_vPosition - vTarget;
            if (vToOrigin.x * vToOrigin.x + vToOrigin.y * vToOrigin.y +
                        vToOrigin.z * vToOrigin.z <= PROJECTILE_BROADCAST_RANGE * PROJECTILE_BROADCAST_RANGE ||
                vToTarget.x * vToTarget.x + vToTarget.y * vToTarget.y +
                        vToTarget.z * vToTarget.z <= PROJECTILE_BROADCAST_RANGE * PROJECTILE_BROADCAST_RANGE) {
                ((CSWSProjectileMessage*)GetServerExoApp()->GetNWSMessage())
                    ->SendServerToPlayerSafeProjectileProjectile(
                        pPlayer, oidOriginator, oidTarget, vOrigin, vTarget, nDelay,
                        nProjectileType, nSpell, nAttackResult, nProjectilePath, bFlag);
            }
        }
    }
}
