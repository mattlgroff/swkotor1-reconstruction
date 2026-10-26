#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x0066C190
void CSWMiniGameObject::RunHitBulletScript(int bHit)
{
    if (!bHit) {
        CResRef cScript = GetScript(1);
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            unsigned long nId = m_nId;
            g_pVirtualMachine->RunScript(&sScript, nId, 1);
            return;
        }
    }
    CSWTrackFollower* follower = AsTrackFollower();
    if (follower && (m_cHit.m_nTargetMask & follower->GetTargetMask())) {
        follower->SetN19C(-m_cHit.m_nDamage);
    }
    if (m_cHit.m_cSound.IsValid()) {
        unsigned char nGroup = 0x17;
        if (!AsMiniPlayer()) nGroup = 0x18;
        CResRef cSound(m_cHit.m_cSound.GetResRef());
        g_pExoSound->Play3DOneShotSound(cSound, m_vSoundPos, 0.0f, nGroup, 0, 0, 0.0f, 0.0f);
    }
}
