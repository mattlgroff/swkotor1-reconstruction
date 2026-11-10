// CSWCVisualEffectOnObject lifecycle: start / stop / show / re-attach and the
// per-frame update (0x006A5000-0x006A6D70, nwcvisualeffectonobject.o).

#include "l/sw_c_visual_effect.h"

// FUNCTION: SWKOTOR 0x006A6590
void CSWCVisualEffectOnObject::StopVisualEffect(int bStop)
{
    if (m_bBC) {
        m_bC0 = bStop;
        return;
    }
    m_bBC = 1;
    m_bC0 = bStop;
    int bPlayed = 0;
    float fLength;
    float fStart;
    for (int i = 0; i < 3; i++) {
        m_n4C[i] = 0;
        if (m_pBeams[i]) {
            m_pBeams[i]->GetEventTime("cessation", &fStart, &fLength);
            if (m_pBeams[i]->PlayAnimation("cessation", 1.0f, 1, 0)) {
                m_n58[i] = 3;
                m_n4C[i] = (unsigned long)(fLength * 1000.0f);
                m_nBeamTimer[0] = 3000 - (int)(fLength * -1000.0f);
                bPlayed = 1;
            } else {
                m_bAC = 1;
            }
        }
    }
    if (bStop && !bPlayed) {
        m_bB8 = 1;
    }
    if (m_bC6 == 2) {
        if (!(m_nA2 < 400 && m_nA2 >= 300 && m_nA4 < 400 && m_nA4 >= 300)) {
            RemoveHardCodedVisualEffect(m_nA2);
            if (m_pModel) {
                m_pModel->AttachToObject(0, 0, 0);
                m_pModel->SetVisible(0);
                if (m_pModel) {
                    delete m_pModel;
                }
                m_pModel = 0;
            }
        }
        if (m_nA4 == 0xffff) {
            m_bC6 = 0;
        } else {
            m_bC6 = 3;
            m_nType = GetBeamNode(m_nA4);
            m_nBeamTimer[3] = 1000;
        }
    } else if (m_bC6 == 1) {
        RemoveHardCodedVisualEffect(m_nA0);
        if (m_pModel) {
            m_pModel->AttachToObject(0, 0, 0);
            m_pModel->SetVisible(0);
            if (m_pModel) {
                delete m_pModel;
            }
            m_pModel = 0;
        }
    }
    if (m_nA4 != 0xffff) {
        if (!(m_nA2 < 400 && m_nA2 >= 300 && m_nA4 < 400 && m_nA4 >= 300)) {
            LoadHardCodedVisualEffect(m_nA4);
        }
        ApplyHardCodedVisualEffect(m_nA4);
    }
    CExoString sSound;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pObject = !pGameObject ? 0 : pGameObject->AsSWCObject();
    CExoString sRow(m_nEffect);
    if (!g_pRules->Get2DArrays()->GetVisualEffectTable()->GetCExoStringEntry(sRow, "SoundCessation", &sSound)) {
        sSound = "";
    }
    if (pObject && sSound.GetLength() > 0) {
        g_pExoSound->Play3DOneShotSound(sSound, pObject->m_vPosition, 1.5f, 13, 0, 0, 0.0f, 0.0f);
    }
    if (m_pSound) {
        m_pSound->Stop();
        if (m_pSound) {
            delete m_pSound;
        }
        m_pSound = 0;
    }
    if (!m_nE0) {
        Detach();
    }
}

// FUNCTION: SWKOTOR 0x006A5A40
int CSWCVisualEffectOnObject::ShowVisualEffect(int bForce)
{
    if (!bForce && m_nA8) {
        return 1;
    }
    CGameObject* pGameObject;
    CGameObjectArray* pArray = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
    if (!pArray) {
        return 0;
    }
    if (pArray->GetGameObject(m_oidTargets[0], &pGameObject) != GAME_OBJECT_VALID) {
        return 0;
    }
    CSWCObject* pObject = pGameObject->AsSWCObject();
    CAurObject* pTarget = pObject->GetModelPart(0xff, 1);
    CExoString sNode;
    CSWCPlaceable* pPlaceable = pObject->AsSWCPlaceable();
    CSWCDoor* pDoor = pObject->AsSWCDoor();
    if (m_pBeams[0]) {
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_ground";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "grnd";
        } else {
            sNode = "root";
        }
        AttachVFXModelToTargetModel(m_pBeams[0], pTarget, sNode);
    }
    if (m_pBeams[1]) {
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_impact";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "impc";
        } else {
            sNode = "impact";
        }
        AttachVFXModelToTargetModel(m_pBeams[1], pTarget, sNode);
    }
    if (m_pBeams[2]) {
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_head_hit";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "hhit";
        } else {
            sNode = "talkdummy";
        }
        CAurObject* pHeadTarget;
        if (pObject->GetAnimationBase() && pObject->GetAnimationBase()->m_nAnimType == 3) {
            pHeadTarget = pObject->GetModelPart(0xfe, 1);
        } else {
            pHeadTarget = pTarget;
        }
        AttachVFXModelToTargetModel(m_pBeams[2], pHeadTarget, sNode);
    }
    if (m_pModel) {
        pPlaceable = pObject->AsSWCPlaceable();
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_ground";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "grnd";
        } else {
            sNode = "root";
        }
        m_pModel->AttachToObject(pTarget, sNode.CStr(), 0);
    }
    m_nA8 = 1;
    m_nE0 = 1;
    if (m_bC6 == 1) {
        if (m_nA0 != 0xffff) {
            ApplyHardCodedVisualEffect(m_nA0);
        }
    } else if (m_bC6 == 2) {
        if (m_nA2 != 0xffff) {
            ApplyHardCodedVisualEffect(m_nA2);
        }
    } else if (m_bC6 == 3) {
        if (m_nA4 != 0xffff) {
            ApplyHardCodedVisualEffect(m_nA4);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006A5000
// NONMATCH: 88 insns, an esi/edi swap of this and the placeable / effect
// register (with an out-of-line GetHardCodedEffect call the registers agree).
void CSWCVisualEffectOnObject::ReAttachVisualEffect()
{
    CExoString sNode;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (!pGameObject) {
        return;
    }
    CSWCObject* pObject = pGameObject->AsSWCObject();
    if (!pObject) {
        return;
    }
    CAurObject* pTarget = pObject->GetModelPart(0xff, 1);
    if (!pTarget) {
        return;
    }
    m_nE0 = 1;
    CSWCPlaceable* pPlaceable = pObject->AsSWCPlaceable();
    CSWCDoor* pDoor = pObject->AsSWCDoor();
    if (m_pBeams[0]) {
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_ground";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "grnd";
        } else {
            sNode = "root";
        }
        AttachVFXModelToTargetModel(m_pBeams[0], pTarget, sNode);
    }
    if (m_pBeams[1]) {
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_impact";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "impc";
        } else {
            sNode = "impact";
        }
        AttachVFXModelToTargetModel(m_pBeams[1], pTarget, sNode);
    }
    if (m_pBeams[2]) {
        if (pPlaceable) {
            sNode = pPlaceable->m_sModelName + "_head_hit";
        } else if (pDoor) {
            sNode = pDoor->m_sModelName + "hhit";
        } else {
            sNode = "talkdummy";
        }
        CAurObject* pHeadTarget;
        if (pObject->GetAnimationBase() && pObject->GetAnimationBase()->m_nAnimType == 3) {
            pHeadTarget = pObject->GetModelPart(0xfe, 1);
        } else {
            pHeadTarget = pTarget;
        }
        AttachVFXModelToTargetModel(m_pBeams[2], pHeadTarget, sNode);
    }
    unsigned short nEffect;
    if (m_bC6 == 1) {
        nEffect = m_nA0;
    } else if (m_bC6 == 2) {
        nEffect = m_nA2;
    } else if (m_bC6 == 3) {
        nEffect = m_nA4;
    } else {
        nEffect = 0xffff;
    }
    if (m_pModel) {
        if (nEffect >= 600 && nEffect < 700) {
            AttachBeam();
        } else if (nEffect >= 300 && nEffect < 400) {
            AttachLight();
        } else if (nEffect >= 1200 && nEffect < 1300) {
            ApplySpellVisual(nEffect);
        } else {
            m_pModel->AttachToObject(pTarget, m_sTargetNode.CStr(), 0);
        }
    }
    if (nEffect >= 100 && nEffect < 200) {
        ApplyEnvironmentMap(nEffect);
    } else if (nEffect >= 200 && nEffect < 300) {
        ApplyColorOverlay(nEffect);
    } else if (nEffect >= 400 && nEffect < 500) {
        switch ((unsigned short)(nEffect - 400)) {
        case 1:
        case 4:
            pObject->SetAlpha(m_fCC, 0xff);
            break;
        default:
            ApplyColorEffect(nEffect);
            break;
        }
    } else if (nEffect >= 500 && nEffect < 600) {
        pObject->SetColorOverlay(m_vOffset, 3, 0, 1);
    } else if (nEffect >= 1000 && nEffect < 1100) {
        ApplySkinTexture(nEffect);
    } else if (nEffect >= 1400 && nEffect < 1500) {
        ApplyTexture(nEffect);
    } else if (nEffect >= 1800 && nEffect < 1900) {
        ApplyFlash(nEffect);
    }
}

// FUNCTION: SWKOTOR 0x006A0DF0
void CSWCVisualEffectOnObject::AttachLight()
{
    Quaternion qOrientation;
    Vector vPosition;
    CExoString sNode;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (!pGameObject) {
        return;
    }
    CSWCObject* pObject = pGameObject->AsSWCObject();
    if (!pObject) {
        return;
    }
    if (!m_pModel) {
        return;
    }
    CAurObject* pTarget = pObject->GetModelPart(0xff, 1);
    if (!pTarget) {
        return;
    }
    CSWCPlaceable* pPlaceable = pObject->AsSWCPlaceable();
    CSWCDoor* pDoor = pObject->AsSWCDoor();
    if (pObject->AsSWCCreature()) {
        pTarget->GetNodeTransform("headconjure", &vPosition, &qOrientation);
        vPosition.y += 0.75f;
        vPosition.z += 0.5f;
    } else if (pPlaceable) {
        CExoString sHook;
        sHook = pPlaceable->m_sModelName + "_head_hit";
        pTarget->GetNodeTransform(sHook.CStr(), &vPosition, &qOrientation);
        vPosition.y += 0.375f;
        vPosition.z += 0.25f;
    } else if (pDoor) {
        CExoString sHook;
        sHook = pDoor->m_sModelName + "hhit";
        pTarget->GetNodeTransform(sHook.CStr(), &vPosition, &qOrientation);
        vPosition.y += 0.375f;
        vPosition.z += 0.25f;
    } else {
        qOrientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
        vPosition = Vector(0.0f, 0.0f, 0.0f);
    }
    m_pModel->SetNodeTransform("root", vPosition, qOrientation);
    if (pPlaceable) {
        sNode = pPlaceable->m_sModelName + "_ground";
    } else if (pDoor) {
        sNode = pDoor->m_sModelName + "grnd";
    } else {
        sNode = "root";
    }
    m_pModel->AttachToObject(pTarget, sNode.CStr(), 0);
    CSWCCreature* pCreature = pObject->AsSWCCreature();
    if (pCreature && pCreature == g_pAppManager->GetClientExoApp()->GetPlayerCreature()
        && pCreature->m_pArea) {
        pCreature->m_pArea->UpdateShadowingLights();
    }
}
