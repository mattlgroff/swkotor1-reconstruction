// CAurGunEmitter (vtable 0x00757F70, 0x006DADF0-0x006DAF52): the plain
// mini-game gun behaviour; Fire re-arms the reload timer, plays the gun's
// "fire"/"ready" animations and the fire sound.
#include "l/sw_mg_behavior.h"

// FUNCTION: SWKOTOR 0x006DADF0
CAurGunEmitter::CAurGunEmitter(CAurObject* pGun, CAurObject* pModel, CAurPart* pPart)
    : CAurBehaviorAttach(pGun, pModel, pPart)
{
    m_fTimer = 0.0f;
}

// FUNCTION: SWKOTOR 0x006DAE60
int CAurGunEmitter::GetType()
{
    return 0xaaad;
}

// FUNCTION: SWKOTOR 0x006DAE70
int CAurGunEmitter::Control(float fDelta)
{
    if (!CAurBehaviorAttach::Control(fDelta)) {
        return 0;
    }
    if (m_fTimer > 0.0) {
        float fTimer = m_fTimer - fDelta;
        m_fTimer = 0.0f > fTimer ? 0.0f : fTimer;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006DAEC0
int CAurGunEmitter::Fire()
{
    if (m_fTimer == 0.0) {
        m_fTimer = m_fRate;
        m_pObject->PlayAnimation("fire", 1.0f, 1, 0.0f);
        m_pObject->PlayAnimation("ready", 1.0f, 0x20, 0.0f);
        if (m_cFireSound.IsValid()) {
            CResRef cSound(m_cFireSound.GetResRef());
            g_pExoSound->PlayOneShotSound(cSound, 0x17, 0, 0, 0, 0);
        }
        return 1;
    }
    return 0;
}
