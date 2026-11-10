// CSWCVisualEffectOnObject effects that launch a client projectile and use the
// out-of-line Vector operator- (ApplyCheat inlines it): ApplyLightsaberThrow
// (0x006A30E0) and ApplyMirv (0x006A4240).

#include <math.h>
#include "l/sw_c_visual_effect.h"

Vector operator-(const Vector& a, const Vector& b); // 0x00423BC0

// FUNCTION: SWKOTOR 0x006A4240
// NONMATCH: 56 insns; the original spills nEffect to its home slot and keeps
// the target in ebx, mine spills the target (frame +4).
void CSWCVisualEffectOnObject::ApplyMirv(unsigned short nEffect)
{
    Quaternion qOrientation;
    Vector vPosition;
    CExoString sModel;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
    pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[1]);
    CSWCObject* pSource = !pGameObject ? 0 : pGameObject->AsSWCObject();
    if (!pTarget || !pSource) {
        return;
    }
    CSWCProjectile* pProjectile = new CSWCProjectile;
    pProjectile->SetSpellId(0x154);
    nEffect -= 900;
    switch (nEffect) {
    case 0:
        sModel = "vpr_magmisl";
        break;
    case 1:
        sModel = "vpr_arofire";
        pProjectile->m_n1A1 = 2;
        break;
    case 2:
        sModel = "vpr_aroacid";
        pProjectile->m_n1A1 = 2;
        break;
    }
    if (!pProjectile->LoadModel(sModel, 0xff, 0)) {
        delete pProjectile;
        return;
    }
    if (!pSource->GetModelPart(0xff, 1)->GetPosition("handconjure", &vPosition, &qOrientation)) {
        vPosition = pSource->m_vPosition;
    }
    pProjectile->SetPosition(vPosition);
    pProjectile->SetOrientation(pSource->m_vOrientation);
    CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
    if (pModule && pModule->m_pArea) {
        pProjectile->AttachToScene(pModule->m_pArea->m_pScene);
    }
    pProjectile->PlayAnimation(1);
    if (m_vTargetPosition == Vector(0.0f, 0.0f, 0.0f)) {
        pProjectile->m_vTargetPosition = pTarget->m_vPosition;
        pProjectile->SetTarget(pTarget->m_idSelf, "");
    } else {
        pProjectile->m_vTargetPosition = m_vTargetPosition;
    }
    switch (nEffect) {
    case 2:
        pProjectile->SetDuration(pProjectile->GetProjectileTravelTime(pTarget->m_vPosition, 0) * 2);
        pProjectile->SetMode(1);
        break;
    case 1: {
        float fDistance = Magnitude(::operator-(pTarget->m_vPosition, pSource->m_vPosition));
        pProjectile->SetDuration((unsigned long)(fDistance / (logf(fDistance) * 3.0f + 2.0f) * 1000.0f));
        pProjectile->SetMode(4);
        break;
    }
    case 0: {
        float fDistance = Magnitude(::operator-(pTarget->m_vPosition, pSource->m_vPosition));
        pProjectile->SetDuration((unsigned long)(fDistance / (logf(fDistance) * 3.0f + 2.0f) * 1000.0f));
        pProjectile->SetMode(4);
        break;
    }
    }
}

// FUNCTION: SWKOTOR 0x006A30E0
// NONMATCH: 89 insns; register assignment of the three objects differs, the
// GetPosition calls merge, and the ?: CExoString temporary uses flag bits.
void CSWCVisualEffectOnObject::ApplyLightsaberThrow(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
    pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[1]);
    CSWCObject* pSource = !pGameObject ? 0 : pGameObject->AsSWCObject();
    pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[2]);
    CSWCObject* pThrower = !pGameObject ? 0 : pGameObject->AsSWCObject();
    float fRate = 37.0f;
    if (pThrower && pThrower->AsSWCCreature()) {
        fRate = pThrower->AsSWCCreature()->GetServerRate();
    }
    if (!pTarget || !pSource || !pThrower || !pThrower->AsSWCCreature()) {
        return;
    }
    CSWCCreature* pCreature = pThrower->AsSWCCreature();
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
        pCreature->GetEquippedItemID(0x10));
    if (!pItem) {
        return;
    }
    CSWCProjectile* pProjectile = new CSWCProjectile;
    pProjectile->SetSpellId(0x31);
    pProjectile->m_oidSource = pThrower->m_idSelf;
    pProjectile->LoadModel(pItem->m_cModelResRef.GetResRef(), 0xff, 0);
    pProjectile->GetModelPart(0xff, 1)->vE0();
    Vector vPosition;
    Quaternion qOrientation;
    if (pSource->m_idSelf == pThrower->m_idSelf) {
        pCreature->GetModelPart(0xff, 1)->GetPosition("rhand", &vPosition, &qOrientation);
    } else {
        pSource->GetModelPart(0xff, 1)->GetPosition("impact", &vPosition, &qOrientation);
    }
    pProjectile->SetPosition(vPosition);
    pCreature->SetProjectileObjectId(pProjectile->m_idSelf);
    Vector vOrientation = pSource->m_vOrientation;
    Quaternion qRotation(CrossProduct(vOrientation, Vector(0.0f, 0.0f, 1.0f)), 1.2217305f);
    pProjectile->SetOrientation(qRotation * vOrientation);
    pProjectile->SetTargetPosition(pTarget->m_vPosition);
    pProjectile->SetTarget(pTarget->m_idSelf, pThrower == pTarget ? CExoString("rhand") : CExoString(""));
    float fDistance = Magnitude(::operator-(pTarget->m_vPosition, pSource->m_vPosition));
    pProjectile->SetDuration((unsigned long)(fDistance / fRate * 2000.0f));
    pProjectile->PlayAnimation(3);
    pProjectile->SetMode(1);
    pProjectile->m_n158 = 0;
    CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
    if (pModule && pModule->m_pArea) {
        pProjectile->AttachToScene(pModule->m_pArea->m_pScene);
    }
}
