// CSWCVisualEffectOnObject (0x006A0480-0x006A1880): client-side beam / impact /
// duration visual effects (behavioural reference: src/unknown/
// sub_0069d8b0_visual_effect_helpers.cpp, sub_006a*.cpp).

#include "l/sw_c_visual_effect.h"

extern short g_nSpeedBlurCount;                     // 0x00833BCC

// FUNCTION: SWKOTOR 0x006A0480
int CSWCVisualEffectOnObject::IsReady()
{
    if (m_pAC && m_pB0 && m_pB4 && m_pC0 && !m_bC6) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006A04C0
void CSWCVisualEffectOnObject::SetDefaultColors(unsigned short nEffect)
{
    switch ((unsigned short)(nEffect - 0x1f4)) {
    case 0:
        m_vColor1 = Vector(1.0f, 0.0f, 0.0f);
        m_vColor2 = Vector(1.0f, 1.0f, 1.0f);
        break;
    case 1:
        m_vColor1 = Vector(0.0f, 0.0f, 1.0f);
        m_vColor2 = Vector(1.0f, 1.0f, 1.0f);
        break;
    }
}

// FUNCTION: SWKOTOR 0x006A05B0
void CSWCVisualEffectOnObject::HideModel(unsigned short nEffect)
{
    if (m_pModel) {
        m_pModel->AttachToObject(0, 0, 0);
        m_pModel->SetVisible(0);
    }
}

// FUNCTION: SWKOTOR 0x006A05E0
void CSWCVisualEffectOnObject::ResetBeamTimer(CAurObject* pBeam)
{
    if (pBeam == m_pBeams[0]) {
        m_nBeamTimer[0] = 3000;
    } else if (pBeam == m_pBeams[1]) {
        m_nBeamTimer[1] = 3000;
    } else if (pBeam == m_pBeams[2]) {
        m_nBeamTimer[2] = 3000;
    }
}

// FUNCTION: SWKOTOR 0x006A0620
int CSWCVisualEffectOnObject::GetImpactDuration(unsigned short nEffect)
{
    if (nEffect == 0xffff) {
        return 0;
    }
    if (m_pModel && nEffect == 0x4b1) {
        float fStart;
        float fEnd;
        m_pModel->GetEventTime("impact", &fStart, &fEnd);
        return (int)(fEnd * 1000.0f);
    }
    return 1000;
}

// FUNCTION: SWKOTOR 0x006A0680
int CSWCVisualEffectOnObject::LoadBeam(unsigned short nEffect)
{
    CExoString sModel;
    nEffect -= 600;
    switch (nEffect) {
    case 8: sModel = "v_lightns_dur"; break;
    case 9: sModel = "v_lightnx_dur"; break;
    case 10: sModel = "v_drddisab_dur"; break;
    case 11: sModel = "v_drdkill_dur"; break;
    case 12: sModel = "v_deathfld_dur"; break;
    case 13: sModel = "v_drain_dur"; break;
    case 14: sModel = "v_flame_dur"; break;
    case 15: sModel = "v_stunray_dur"; break;
    case 16: sModel = "v_coldray_dur"; break;
    case 17: sModel = "v_ionray01_dur"; break;
    case 18: sModel = "v_ionray02_dur"; break;
    case 19: sModel = "v_fstorm_dur"; break;
    case 20: sModel = "v_drdstun_dur"; break;
    case 21: sModel = "v_fshock_dur"; break;
    }
    m_pModel = NewCAurObject(sModel.CStr(), "fxbeam", 0, 0);
    if (!m_pModel) {
        return 0;
    }
    m_pModel->SetDrawMode(3);
    m_pModel->Restart();
    return 1;
}

// FUNCTION: SWKOTOR 0x006A0800
int CSWCVisualEffectOnObject::LoadFailImpact(unsigned short nEffect)
{
    unsigned short nIndex = nEffect - 0x4b0;
    switch (nIndex) {
    case 0:
    case 1:
        m_pModel = NewCAurObject("v_fizzle_imp", "fxfail", 0, 0);
        break;
    case 2:
        m_pModel = NewCAurObject("v_fresist_imp", "fxresist", 0, 0);
        break;
    default:
        return 0;
    }
    return m_pModel != 0;
}

// FUNCTION: SWKOTOR 0x006A0860
int CSWCVisualEffectOnObject::LoadDurationModel(unsigned short nEffect)
{
    switch ((unsigned short)(nEffect - 0x6a4)) {
    case 0:
        m_pModel = NewCAurObject("v_medal_dur", "fxmedal", 0, 0);
        break;
    case 1:
        m_pModel = NewCAurObject("v_revmask1_dur", "fxrevanmask1", 0, 0);
        break;
    case 2:
        m_pModel = NewCAurObject("v_revmask2_dur", "fxrevanmask2", 0, 0);
        break;
    }
    return m_pModel != 0;
}

// FUNCTION: SWKOTOR 0x006A08D0
void CSWCVisualEffectOnObject::DestroyModel()
{
    if (m_pModel) {
        m_pModel->AttachToObject(0, 0, 0);
        m_pModel->SetVisible(0);
        if (m_pModel) {
            delete m_pModel;
        }
        m_pModel = 0;
    }
}

// FUNCTION: SWKOTOR 0x006A0920
unsigned char CSWCVisualEffectOnObject::GetType()
{
    return m_nType;
}

// FUNCTION: SWKOTOR 0x006A0930
unsigned char CSWCVisualEffectOnObject::GetBeamNode(unsigned short nEffect)
{
    if (nEffect < 200) return 2;
    if (nEffect < 300) return 13;
    if (nEffect < 400) return 4;
    if (nEffect < 500) return 13;
    if (nEffect < 600) return 13;
    if (nEffect < 700) return 7;
    if (nEffect < 800) return 8;
    if (nEffect < 900) return 9;
    if (nEffect < 1000) return 10;
    if (nEffect < 1100) return 1;
    if (nEffect < 1200) return 11;
    if (nEffect < 1300) return 12;
    if (nEffect < 0x72) return 14;
    if (nEffect < 1600) return 15;
    if (nEffect < 0x74) return 16;
    return 0;
}

// FUNCTION: SWKOTOR 0x006A0BF0
void CSWCVisualEffectOnObject::DestroyModels()
{
    DestroyModel();
    for (int i = 0; i < 3; i++) {
        if (m_pBeams[i]) {
            delete m_pBeams[i];
            m_pBeams[i] = 0;
        }
        if (m_pImpacts[i]) {
            delete m_pImpacts[i];
            m_pImpacts[i] = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x006A0C70
void CSWCVisualEffectOnObject::Detach()
{
    m_nE0 = 0;
    for (int i = 0; i < 3; i++) {
        if (m_pBeams[i]) {
            m_pBeams[i]->AttachToObject(0, 0, 0);
            m_pBeams[i]->SetVisible(0);
        }
    }
    if (m_pModel) {
        if (m_nType == 4) {
            CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
            if (pGameObject) {
                CSWCObject* pObject = pGameObject->AsSWCObject();
                if (pObject) {
                    CSWCCreature* pCreature = pObject->AsSWCCreature();
                    if (pCreature && pCreature == g_pAppManager->GetClientExoApp()->GetPlayerCreature()
                        && pCreature->m_pArea) {
                        pCreature->m_pArea->UpdateShadowingLights();
                    }
                }
            }
        }
        m_pModel->AttachToObject(0, 0, 0);
        m_pModel->SetVisible(0);
    }
}

// FUNCTION: SWKOTOR 0x006A0D30
void CSWCVisualEffectOnObject::AttachBeam()
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    CSWCObject* pSource = !pGameObject ? 0 : pGameObject->AsSWCObject();
    pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[1]);
    CSWCObject* pTarget = !pGameObject ? 0 : pGameObject->AsSWCObject();
    if (pSource && pTarget && m_pModel) {
        m_pModel->AttachToObject(pTarget->GetModelPart(0xff, 1), m_sSourceNode.CStr(), 0);
        m_pModel->SetTarget(pSource->GetModelPart(0xff, 1), m_sTargetNode.CStr());
    }
}

// FUNCTION: SWKOTOR 0x006A1470
void CSWCVisualEffectOnObject::RemoveTexture()
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            pObject->RestoreTexture();
            CSWCCreature* pCreature = pObject->AsSWCCreature();
            if (pCreature) {
                CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
                    pCreature->GetEquippedItemID(1));
                if (pItem) {
                    pItem->RestoreTexture();
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A14E0
void CSWCVisualEffectOnObject::ApplySpeedBlur(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            // NONMATCH: the original tests the cases with a dec chain.
            switch ((unsigned short)(nEffect - 0x640)) {
            case 1:
            case 2:
                g_nSpeedBlurCount++;
                CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
                if (pPlayer && pPlayer->m_idSelf != pObject->m_idSelf) {
                    break;
                }
                if (g_nSpeedBlurCount == 1 && !g_pAppManager->GetClientExoApp()->GetMiniGame()) {
                    AurSetSpeedBlurRatio(0.75f);
                    AurEnableSpeedBlur();
                }
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A1570
void CSWCVisualEffectOnObject::RemoveSpeedBlur()
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject && pGameObject->AsSWCObject()) {
        if (--g_nSpeedBlurCount < 0) {
            g_nSpeedBlurCount = 0;
        } else if (g_nSpeedBlurCount != 0) {
            return;
        }
        if (!g_pAppManager->GetClientExoApp()->GetMiniGame()) {
            AurDisableSpeedBlur();
        }
    }
}

// FUNCTION: SWKOTOR 0x006A15D0
void CSWCVisualEffectOnObject::ApplyFlash(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            switch ((unsigned short)(nEffect - 0x708)) {
            case 0:
                pObject->StartFlash();
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A1610
void CSWCVisualEffectOnObject::RemoveFlash(unsigned short nEffect)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            switch ((unsigned short)(nEffect - 0x708)) {
            case 0:
                pObject->StopFlash();
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006A1810
CSWCVisualEffectOnObject::~CSWCVisualEffectOnObject()
{
    if (m_pSound) {
        delete m_pSound;
    }
    DestroyModels();
}

// FUNCTION: SWKOTOR 0x006A09E0
CSWCVisualEffectOnObject::CSWCVisualEffectOnObject()
{
    m_n34 = 0;
    m_n38 = 0;
    m_n3C = 0;
    m_sSourceNode = "";
    m_oidTargets[0] = OBJECT_INVALID;
    m_oidTargets[1] = OBJECT_INVALID;
    m_oidTargets[2] = OBJECT_INVALID;
    for (int i = 0; i < 3; i++) {
        m_pBeams[i] = 0;
        m_pImpacts[i] = 0;
        m_n4C[i] = 0;
        m_n58[i] = 0;
        m_n60[i] = 0;
        m_n6C[i] = 0;
    }
    m_vColor1 = Vector(0.0f, 0.0f, 0.0f);
    m_vColor2 = Vector(0.0f, 0.0f, 0.0f);
    m_nA0 = 0;
    m_nA2 = 0;
    m_nA4 = 0;
    m_nA8 = 0;
    m_nEffect = 0;
    m_pAC = 0;
    m_pB0 = 0;
    m_pB4 = 0;
    m_pB8 = 0;
    m_nBeamTimer[0] = 0;
    m_nBeamTimer[1] = 0;
    m_nBeamTimer[2] = 0;
    m_nBeamTimer[3] = 0;
    m_bC6 = 0;
    m_bC7 = 0;
    m_pModel = 0;
    m_pBC = 0;
    m_pC0 = 0;
    m_vOffset = Vector(0.0f, 0.0f, 0.0f);
    m_fD0 = 0.0f;
    m_nD4 = 0;
    m_nD8 = 0;
    m_nType = 0;
    m_nDC = 0;
    m_nE0 = 0;
    m_fE4 = 0.0f;
    m_nE8 = 0;
    m_pSound = 0;
    m_nF0 = 0;
    m_nF4 = 0;
    m_fCC = 1.0f;
}

// FUNCTION: SWKOTOR 0x006A1220
void CSWCVisualEffectOnObject::ApplyTexture(unsigned short nEffect)
{
    CExoString sTexture;
    m_nD8 = 0x14;
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (!pGameObject) {
        return;
    }
    CSWCObject* pObject = pGameObject->AsSWCObject();
    if (!pObject) {
        return;
    }
    nEffect -= 0x578;
    switch (nEffect) {
    case 1: sTexture = "fx_tex_01"; break;
    case 2: sTexture = "fx_tex_02"; break;
    case 3: sTexture = "fx_tex_03"; break;
    case 4: sTexture = "fx_tex_04"; break;
    case 5: sTexture = "fx_tex_05"; break;
    case 6: sTexture = "fx_tex_06"; break;
    case 7: sTexture = "fx_tex_07"; break;
    case 8: sTexture = "fx_tex_08"; break;
    case 9: sTexture = "fx_tex_09"; break;
    case 10: sTexture = "fx_tex_10"; break;
    case 11: sTexture = "fx_tex_11"; break;
    case 12: sTexture = "fx_tex_12"; break;
    case 13: sTexture = "fx_tex_14"; break;
    case 14: sTexture = "fx_tex_15"; break;
    case 15: sTexture = "fx_tex_16"; break;
    case 16: sTexture = "fx_tex_17"; break;
    case 17: sTexture = "fx_tex_18"; break;
    case 18: sTexture = "fx_tex_19"; break;
    case 19: sTexture = "fx_tex_20"; break;
    case 20: sTexture = "fx_tex_21"; break;
    case 21: sTexture = "fx_tex_22"; break;
    case 22: sTexture = "fx_tex_23"; break;
    case 23: sTexture = "fx_tex_24"; break;
    case 24: sTexture = "fx_tex_25"; break;
    case 25: sTexture = "fx_tex_26"; break;
    case 26: sTexture = "fx_tex_stealth"; break;
    default:
        return;
    }
    pObject->ReplaceTexture(CResRef(sTexture), 0.02f);
    CSWCCreature* pCreature = pObject->AsSWCCreature();
    if (!pCreature) {
        return;
    }
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
        pCreature->GetEquippedItemID(1));
    if (!pItem) {
        return;
    }
    pItem->ReplaceTexture(CResRef(sTexture), 0.02f);
}

// FUNCTION: SWKOTOR 0x006A1650
// NONMATCH: the two CExoString temporaries get swapped stack slots.
void CSWCVisualEffectOnObject::AttachModel(CAurObject* pModel, CAurObject* pTo, CExoString* pNode)
{
    int bOrientationOff;
    if (!g_pRules->Get2DArrays()->GetVisualEffectTable()->GetINTEntry(
            m_nEffect, "OrientationOff", &bOrientationOff)) {
        bOrientationOff = 0;
    }
    if (bOrientationOff) {
        Quaternion qOrientation;
        Vector vPosition;
        pModel->AttachToObject(pTo, pNode->CStr(), 3);
        pModel->GetPosition("root", &vPosition, &qOrientation);
        pModel->SetPosition("root", vPosition, Quaternion(1.0f, 0.0f, 0.0f, 0.0f));
        return;
    }
    pModel->AttachToObject(pTo, pNode->CStr(), 0);
}
