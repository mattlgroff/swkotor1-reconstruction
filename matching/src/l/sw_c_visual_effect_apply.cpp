// CSWCVisualEffectOnObject (0x006A3E80-0x006A5F00): colour / texture /
// self-illumination helpers and the hard-coded effect detach path
// (behavioural reference: src/unknown/sub_006a4830_visual_effect_lifecycle.cpp,
// src/unknown/sub_006a_visual_gui_helpers.cpp).

#include "l/sw_c_visual_effect.h"

// FUNCTION: SWKOTOR 0x006A2E10
void CSWCVisualEffectOnObject::ApplySpellVisual(unsigned short nEffect)
{
    nEffect -= 1200;
    switch (nEffect) {
    case 0:
        m_sTargetNode = "headconjure";
        break;
    case 1:
        m_sTargetNode = "handconjure";
        break;
    case 2:
        m_sTargetNode = "impact";
        break;
    }
    if (!m_pModel) {
        return;
    }
    if (nEffect == 2) {
        Quaternion qOrientation;
        Vector vPosition;
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[1]);
        CSWCObject* pSource = !pGameObject ? 0 : pGameObject->AsSWCObject();
        pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        CSWCObject* pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
        m_pModel->GetPosition("root", &vPosition, &qOrientation);
        if (pSource && pTarget) {
            Vector vDirection;
            vDirection = pSource->m_vPosition - pTarget->m_vPosition;
            if (vDirection.z * vDirection.z + vDirection.y * vDirection.y + vDirection.x * vDirection.x <= 0.0001f) {
                vDirection = Vector(0.0f, 0.0f, 0.0f);
            }
            vDirection = NormalizeVector(vDirection);
            qOrientation = EulerToQuaternion(GetYaw(vDirection), 0.0f, 0.0f);
        }
        m_pModel->SetPosition("root", vPosition, qOrientation);
        pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
        m_pModel->AttachToObject(pTarget->GetModelPart(0xff, 1), m_sTargetNode.CStr(), 3);
    } else {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        CSWCObject* pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
        m_pModel->AttachToObject(pTarget->GetModelPart(0xff, 1), m_sTargetNode.CStr(), 0);
    }
    m_pModel->PlayAnimation("impact", 1.0f, 1, 0);
}

// FUNCTION: SWKOTOR 0x006A34B0
void CSWCVisualEffectOnObject::ApplySkinTexture(unsigned short nEffect)
{
    CExoString sTexture;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (!pGameObject) {
        return;
    }
    CSWCObject* pObject = pGameObject->AsSWCObject();
    if (!pObject) {
        return;
    }
    nEffect -= 1000;
    switch (nEffect) {
    case 10:
        sTexture = "vdu_tex_stone";
        pObject->m_bIllumination[0] = 1;
        break;
    case 20:
        sTexture = "vdu_tex_grstone";
        pObject->m_bIllumination[0] = 1;
        break;
    case 30:
        sTexture = "vdu_tex_bark";
        pObject->m_bIllumination[1] = 1;
        break;
    case 40:
        sTexture = "vdu_tex_shade";
        pObject->m_bIllumination[2] = 1;
        break;
    default:
        return;
    }
    pObject->ReplaceTextures("", sTexture, 0);
    CSWCCreature* pCreature = pObject->AsSWCCreature();
    if (!pCreature) {
        return;
    }
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
        pCreature->GetEquippedItemID(1));
    if (!pItem) {
        return;
    }
    pItem->ReplaceTextures("", sTexture, 0);
}

// FUNCTION: SWKOTOR 0x006A3660
void CSWCVisualEffectOnObject::ApplyEnvironmentMap(unsigned short nEffect)
{
    CExoString sEnvMap;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            nEffect -= 100;
            sEnvMap.Format("vdu_envmap%03u", nEffect);
            pObject->ReplaceTextures("", "", 0);
            pObject->SetEnvironmentMap(1, 0, sEnvMap);
        }
    }
}

// FUNCTION: SWKOTOR 0x006A3740
void CSWCVisualEffectOnObject::ApplyColorOverlay(unsigned short nEffect)
{
    Vector vColor;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            m_nD8 = 0x14;
            switch ((unsigned short)(nEffect - 200)) {
            case 0:
                vColor = Vector(1.0f, 0.0f, 0.0f);
                break;
            case 1:
                vColor = Vector(0.0f, 1.0f, 0.0f);
                break;
            case 2:
                vColor = Vector(0.0f, 0.0f, 1.0f);
                break;
            case 3:
                vColor = Vector(1.0f, 0.0f, 1.0f);
                break;
            case 4:
                vColor = Vector(1.0f, 1.0f, 0.0f);
                break;
            case 5:
                vColor = Vector(1.0f, 1.0f, 1.0f);
                break;
            default:
                return;
            }
            pObject->SetColorOverlay(vColor, 3, 0, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x006A3E80
void CSWCVisualEffectOnObject::ApplyColorEffect(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            m_nD8 = 1;
            switch ((unsigned short)(nEffect - 400)) {
            case 0:
                pObject->SetAlpha(m_fD0, 0xff);
                m_fCC = m_fD0;
                break;
            case 2:
                pObject->SetAlpha(m_fD0, 0xff);
                m_fCC = m_fD0;
                pObject->SetColorOverlay(Vector(0.25f, 0.25f, 1.0f), 3, 0, 1);
                break;
            case 3:
                pObject->SetAlpha(m_fD0, 0xff);
                m_fCC = m_fD0;
                pObject->SetColorOverlay(Vector(0.5f, 0.0f, 1.0f), 3, 0, 1);
                break;
            case 1:
            case 4:
                m_bC7 = 0;
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A3FD0
void CSWCVisualEffectOnObject::PlayCastAnimation(unsigned short nEffect)
{
    AttachBeam();
    m_pModel->PlayAnimation("cast01", 1.0f, 0, 0);
}

// FUNCTION: SWKOTOR 0x006A4000
void CSWCVisualEffectOnObject::RefreshTextures(unsigned short nEffect)
{
    if (g_pAppManager->GetClientExoApp()->GetClientOptions()->m_nGraphicsDetail >= 2) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        if (pGameObject) {
            CSWCObject* pObject = pGameObject->AsSWCObject();
            if (pObject) {
                CSWCCreature* pCreature = pObject->AsSWCCreature();
                if (pCreature) {
                    pCreature->Reset();
                }
                pObject->GetAnimationBase()->RefreshTextures();
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A4680
void CSWCVisualEffectOnObject::AttachHook(unsigned short nEffect)
{
    switch ((unsigned short)(nEffect - 0x6a4)) {
    case 0:
        m_sTargetNode = "medalhook";
        break;
    case 1:
        m_sTargetNode = "revmask1hook";
        break;
    case 2:
        m_sTargetNode = "revmask2hook";
        break;
    }
    if (m_pModel) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        CSWCObject* pObject = !pGameObject ? 0 : pGameObject->AsSWCObject();
        m_pModel->AttachToObject(pObject->GetModelPart(0xff, 1), m_sTargetNode.CStr(), 0);
    }
}

// FUNCTION: SWKOTOR 0x006A4720
void CSWCVisualEffectOnObject::RemoveSelfIllumination(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            if (pObject->m_bIllumination[0] == 1) {
                pObject->m_bIllumination[0] = 0;
            }
            if (pObject->m_bIllumination[1] == 1) {
                pObject->m_bIllumination[1] = 0;
            }
            if (pObject->m_bIllumination[2] == 1) {
                pObject->m_bIllumination[2] = 0;
            }
            pObject->RefreshSelfIllumination();
            CSWCCreature* pCreature = pObject->AsSWCCreature();
            if (pCreature) {
                CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
                    pCreature->GetEquippedItemID(1));
                if (pItem) {
                    pItem->RefreshSelfIllumination();
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A47B0
void CSWCVisualEffectOnObject::ResetSelfIllumination(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            pObject->ClearSelfIllumination();
            pObject->RefreshSelfIllumination();
        }
    }
}

// FUNCTION: SWKOTOR 0x006A47F0
void CSWCVisualEffectOnObject::SetObjectDrawMode(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            pObject->SetDrawMode(3);
        }
    }
}

// FUNCTION: SWKOTOR 0x006A4830
void CSWCVisualEffectOnObject::RemoveDurationModel(unsigned short nEffect)
{
    if (m_pModel) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        CSWCObject* pObject = (CSWCObject*)pGameObject;
        if (pGameObject) {
            pObject = pGameObject->AsSWCObject();
        }
        nEffect -= 300;
        if (m_nE0 && pObject) {
            CSWCCreature* pCreature = pObject->AsSWCCreature();
            if (pCreature && pCreature == g_pAppManager->GetClientExoApp()->GetPlayerCreature()
                && pCreature->m_pArea) {
                pCreature->m_pArea->UpdateShadowingLights();
            }
        }
        if (nEffect == 33 && g_pAppManager->GetClientExoApp()->GetPlayerCreature()
            && m_oidTargets[0] == g_pAppManager->GetClientExoApp()->GetPlayerCreature()->m_idSelf) {
            CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
            if (pModule && pModule->m_pGuiInGame) {
                pModule->m_pGuiInGame->m_pPanel->SetActive(0);
            }
        }
        m_pModel->AttachToObject(0, 0, 0);
        m_pModel->SetVisible(0);
    }
}

// FUNCTION: SWKOTOR 0x006A4920
void CSWCVisualEffectOnObject::RemoveColorEffect(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            switch ((unsigned short)(nEffect - 400)) {
            case 2:
            case 3:
                pObject->SetDrawMode(3);
            case 0:
            case 1:
            case 4:
                pObject->SetAlpha(1.0f, 0xff);
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A4FA0
// NONMATCH: the shared `return 0` block is laid out after the model test.
int CSWCVisualEffectOnObject::LoadLight(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject && pGameObject->AsSWCObject()) {
        m_pModel = NewCAurObject("fx_light_clr", "fx_light", 0, 0);
        if (!m_pModel) {
            return 0;
        }
        m_pModel->SetDrawMode(3);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006A5F00
void CSWCVisualEffectOnObject::DetachAll()
{
    if (m_nA8) {
        for (int i = 0; i < 3; i++) {
            if (m_pBeams[i]) {
                m_pBeams[i]->AttachToObject(0, 0, 0);
                m_pBeams[i]->SetVisible(0);
            }
        }
        if (m_pModel) {
            m_pModel->AttachToObject(0, 0, 0);
            m_pModel->SetVisible(0);
        }
        if (m_bC6 == 1) {
            if (m_nA0 != 0xffff) {
                RemoveHardCodedVisualEffect(m_nA0);
            }
        } else if (m_bC6 == 2) {
            if (m_nA2 != 0xffff) {
                RemoveHardCodedVisualEffect(m_nA2);
            }
        } else if (m_bC6 == 3) {
            if (m_nA4 != 0xffff) {
                RemoveHardCodedVisualEffect(m_nA4);
            }
        }
        m_nA8 = 0;
        m_nE0 = 0;
    }
}

// FUNCTION: SWKOTOR 0x006A56F0
void CSWCVisualEffectOnObject::RemoveHardCodedVisualEffect(unsigned short nEffect)
{
    if (m_nA8 && m_nF4) {
        if (nEffect >= 100 && nEffect < 200) {
            ResetSelfIllumination(nEffect);
        } else if (nEffect >= 200 && nEffect < 300) {
            SetObjectDrawMode(nEffect);
        } else if (nEffect >= 300 && nEffect < 400) {
            RemoveDurationModel(nEffect);
        } else if (nEffect >= 400 && nEffect < 500) {
            RemoveColorEffect(nEffect);
        } else if (nEffect >= 500 && nEffect < 600) {
            SetObjectDrawMode(nEffect);
        } else if (nEffect >= 600 && nEffect < 700) {
            HideModel(nEffect);
        } else if (nEffect >= 1000 && nEffect < 1100) {
            RemoveSelfIllumination(nEffect);
        } else if (nEffect >= 1400 && nEffect < 1500) {
            RemoveTexture();
        } else if (nEffect >= 1500 && nEffect < 1600) {
            RemoveFOV(nEffect, 0);
        } else if (nEffect >= 1600 && nEffect < 1700) {
            RemoveSpeedBlur();
        } else if (nEffect >= 1800 && nEffect < 1900) {
            RemoveFlash(nEffect);
        }
        m_nF4 = 0;
    }
}

// FUNCTION: SWKOTOR 0x006A5890
int CSWCVisualEffectOnObject::LoadHardCodedVisualEffect(unsigned short nEffect)
{
    if (nEffect >= 300 && nEffect < 400) {
        return LoadLight(nEffect);
    }
    if (nEffect >= 400 && nEffect < 500) {
        switch ((unsigned short)(nEffect - 400)) {
        case 0:
        case 2:
        case 3:
            m_fD0 = 0.5f;
            break;
        }
        return 1;
    }
    if (nEffect >= 600 && nEffect < 700) {
        return LoadBeam(nEffect);
    }
    if (nEffect >= 1200 && nEffect < 1300) {
        return LoadFailImpact(nEffect);
    }
    if (nEffect >= 1700 && nEffect < 1800) {
        return LoadDurationModel(nEffect);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006A54D0
void CSWCVisualEffectOnObject::ApplyHardCodedVisualEffect(unsigned short nEffect)
{
    if (m_nA8 && !m_nF4) {
        if (nEffect < 200) {
            ApplyEnvironmentMap(nEffect);
        } else if (nEffect < 300) {
            ApplyColorOverlay(nEffect);
        } else if (nEffect < 400) {
            ApplyDurationEffect(nEffect);
        } else if (nEffect < 500) {
            ApplyColorEffect(nEffect);
        } else if (nEffect < 600) {
            SetDefaultColors(nEffect);
        } else if (nEffect < 700) {
            PlayCastAnimation(nEffect);
        } else if (nEffect < 800) {
            RefreshTextures(nEffect);
        } else if (nEffect < 900) {
            ApplyEffect4060(nEffect);
        } else if (nEffect < 1000) {
            ApplyEffect4240(nEffect);
        } else if (nEffect < 1100) {
            ApplySkinTexture(nEffect);
        } else if (nEffect < 1200) {
            ApplyImpactEffect(nEffect);
        } else if (nEffect < 1300) {
            ApplySpellVisual(nEffect);
        } else if (nEffect < 1400) {
            ApplyBeamEffect(nEffect);
        } else if (nEffect < 1500) {
            ApplyTexture(nEffect);
        } else if (nEffect < 1600) {
            ApplyFOV(nEffect);
        } else if (nEffect < 1700) {
            ApplySpeedBlur(nEffect);
        } else if (nEffect < 1800) {
            AttachHook(nEffect);
        } else if (nEffect < 1900) {
            ApplyFlash(nEffect);
        }
        m_nF4 = 1;
    }
}

// FUNCTION: SWKOTOR 0x006A4580
void CSWCVisualEffectOnObject::ApplyFOV(unsigned short nEffect)
{
    CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pObject = (CSWCObject*)pGameObject;
    if (pGameObject) {
        pObject = pGameObject->AsSWCObject();
    }
    if (pPlayer && pObject && pPlayer->m_idSelf == pObject->m_idSelf) {
        CSWCModuleCamera* pCamera = g_pAppManager->GetClientExoApp()->GetModuleCamera();
        if (pCamera) {
            switch ((unsigned short)(nEffect - 1500)) {
            case 0:
                m_fE4 = pCamera->GetFOV();
                CCameraBehavior* pBehavior = g_pAppManager->GetClientExoApp()->GetModuleCamera()->GetBehavior(-1);
                if (pBehavior) {
                    if (pBehavior->GetType() == 0x106a) {
                        m_nE8 = pBehavior->m_nMode;
                        pBehavior->m_bLocked = 1;
                    }
                    pBehavior->GetType();
                }
                pCamera->BlendFOV(135.0f, 0.75f, 0);
                m_bC7 = 0;
                m_nBeamTimer[3] = 4750;
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A49A0
void CSWCVisualEffectOnObject::RemoveFOV(unsigned short nEffect, int bForce)
{
    CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pObject = (CSWCObject*)pGameObject;
    if (pGameObject) {
        pObject = pGameObject->AsSWCObject();
    }
    if (pPlayer && pObject && (bForce || pPlayer->m_idSelf == pObject->m_idSelf)) {
        CSWCModuleCamera* pCamera = g_pAppManager->GetClientExoApp()->GetModuleCamera();
        if (pCamera) {
            switch ((unsigned short)(nEffect - 1500)) {
            case 0:
                pCamera->StopFOVBlend(0);
                pCamera->SetFOV(m_fE4);
                CCameraBehavior* pBehavior = g_pAppManager->GetClientExoApp()->GetModuleCamera()->GetBehavior(-1);
                if (pBehavior) {
                    if (pBehavior->GetType() == 0x106a) {
                        pBehavior->m_nMode = m_nE8;
                        pBehavior->m_bLocked = 0;
                    }
                    pBehavior->GetType();
                }
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A4E70
void CSWCVisualEffectOnObject::UpdateFOV(unsigned short nEffect, int bUnused)
{
    CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pObject = (CSWCObject*)pGameObject;
    if (pGameObject) {
        pObject = pGameObject->AsSWCObject();
    }
    if (pPlayer && pObject && pObject->m_idSelf == pPlayer->m_idSelf
        && !g_pAppManager->GetClientExoApp()->GetPausedByCombat()) {
        if (!m_bC7) {
            CSWCModuleCamera* pCamera = g_pAppManager->GetClientExoApp()->GetModuleCamera();
            if (pCamera && !m_bC7 && (float)m_nBeamTimer[3] <= 4000.0f) {
                switch ((unsigned short)(nEffect - 1500)) {
                case 0:
                    pCamera->StopFOVBlend(0);
                    pCamera->BlendFOV(m_fE4, (float)m_nBeamTimer[3] * 0.001f, 0);
                    break;
                }
                m_bC7 = 1;
            }
        }
    } else {
        RemoveFOV(nEffect, 1);
    }
}

// FUNCTION: SWKOTOR 0x006A5920
void CSWCVisualEffectOnObject::UpdateMotionBlur(unsigned short nUnused)
{
    CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
    if (pPlayer) {
        unsigned short nEffect;
        if (m_bC6 == 1) {
            nEffect = m_nA0;
        } else if (m_bC6 == 3) {
            nEffect = m_nA4;
        } else if (m_bC6 == 2) {
            nEffect = m_nA2;
        }
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
        if (pGameObject) {
            CSWCObject* pObject = pGameObject->AsSWCObject();
            if (pObject) {
                int bNotPlayer = 0;
                if (pObject->m_idSelf != pPlayer->m_idSelf) {
                    bNotPlayer = 1;
                }
                CSWCCreature* pCreature = pObject->AsSWCCreature();
                CSWSCreature* pServerCreature;
                if (bNotPlayer || (pCreature && (pServerCreature = pCreature->GetServerCreature()) != 0
                                   && pServerCreature->IsDebilitated()
                                   && pObject->m_idSelf == pPlayer->m_idSelf)) {
                    RemoveHardCodedVisualEffect(nEffect);
                    return;
                }
            }
        }
        ApplyHardCodedVisualEffect(nEffect);
        float fRatio = pPlayer->m_fSpeed / pPlayer->m_pMovement->m_fMaxSpeed;
        if (fRatio > 1.0f) {
            fRatio = 1.0f;
        }
        AurSetSpeedBlurRatio(fRatio * 0.75f);
    }
}

// FUNCTION: SWKOTOR 0x006A6950
void CSWCVisualEffectOnObject::Reset()
{
    unsigned short nEffect = GetHardCodedEffect();
    if (nEffect != 0xffff) {
        RemoveHardCodedVisualEffect(nEffect);
    }
    DestroyModels();
    m_bC6 = 0;
    m_nType = 0;
    if (m_pSound) {
        m_pSound->Stop();
        if (m_pSound) {
            delete m_pSound;
        }
        m_pSound = 0;
    }
}

// FUNCTION: SWKOTOR 0x006A69E0
void CSWCVisualEffectOnObject::Update(unsigned short nEffect, unsigned long nDelta)
{
    if (nEffect >= 400 && nEffect < 500) {
        UpdateAlpha(nDelta);
    } else if (nEffect >= 500 && nEffect < 600) {
        UpdateSelfIlluminationPulse(nDelta);
    } else if (nEffect >= 1500 && nEffect < 1600) {
        UpdateFOV(nEffect, nDelta);
    } else if (nEffect >= 1600 && nEffect < 1700) {
        UpdateMotionBlur(nDelta);
    }
}
