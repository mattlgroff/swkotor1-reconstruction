// CSWCVisualEffectOnObject::ApplyCheat (0x006A2AF0): fires the
// "vpr_kngpow" projectile from in front of the source toward the target.

#include <math.h>
#include "l/sw_c_visual_effect.h"

// FUNCTION: SWKOTOR 0x006A2AF0
// NONMATCH: 46 insns, all a rotated eax/ecx/edx choice from the zero-Vector
// compare on (the same rotation as ApplyMirv).
void CSWCVisualEffectOnObject::ApplyCheat(unsigned short nEffect)
{
    CExoString sModel;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
    pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[1]);
    CSWCObject* pSource = !pGameObject ? 0 : pGameObject->AsSWCObject();
    if (!pTarget || !pSource) {
        return;
    }
    CSWCProjectile* pProjectile = new CSWCProjectile;
    switch (nEffect) {
    case 1100:
        sModel = "vpr_kngpow";
        break;
    }
    if (!pProjectile->LoadModel(sModel, 0xff, 0)) {
        delete pProjectile;
        return;
    }
    Vector vPosition = pSource->m_vPosition + pSource->m_vOrientation * 4.0f;
    vPosition.z += 1.0f;
    pProjectile->SetPosition(vPosition);
    pProjectile->SetOrientation(pSource->m_vOrientation);
    CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
    if (pModule && pModule->m_pArea) {
        pProjectile->AttachToScene(pModule->m_pArea->m_pScene);
    }
    pProjectile->PlayAnimation(1);
    pProjectile->SetSpellId(0x2af8);
    if (m_vTargetPosition == Vector(0.0f, 0.0f, 0.0f)) {
        pProjectile->m_vTargetPosition = pTarget->m_vPosition;
        pProjectile->SetTarget(pTarget->m_idSelf, "");
    } else {
        pProjectile->m_vTargetPosition = m_vTargetPosition;
    }
    switch (nEffect) {
    case 1100: {
        float fDistance = Magnitude(pTarget->m_vPosition - pSource->m_vPosition);
        pProjectile->SetDuration((unsigned long)(fDistance / (logf(fDistance) * 3.0f + 2.0f) * 1000.0f));
        pProjectile->m_n1A1 = 2;
        pProjectile->SetMode(4);
        g_pExoSound->Play3DOneShotSound("c_cow_atk1", pSource->m_vPosition, 1.5f, 13, 0, 0, 0.0f, 0.0f);
        break;
    }
    }
}
