// CSWCObject fade state, alpha and position members (nwcobject.o).
#include "n/sw_c_object.h"

static __forceinline int IsPlayerCreature(unsigned long oid)
{
    return oid == g_pAppManager->m_pClientExoApp->GetPlayerCreatureId();
}

// FUNCTION: SWKOTOR 0x0063CE60
void CSWCObject::SetDesiredFadeState(unsigned char nState, int bTimed, unsigned long nDuration,
                                     unsigned long nDelay)
{
    if (this && m_bNoServerLink == 1) {
        return;
    }
    m_nDesiredFadeState = nState;
    m_bTimedFade = bTimed;
    m_nFadeDuration = nDuration;
    m_nFadeDelay = nDelay;
    if (bTimed) {
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->GetWorldTimer()->GetWorldTime(&m_nFadeStartDay, &m_nFadeStartTime);
    }
    unsigned char nShadows = g_pAppManager->m_pClientExoApp->GetOptions()->m_nShadows;
    if (nState == 1) {
        if (m_pAnimationBase) {
            if (nShadows == 2) {
                TurnOnShadows();
                HideShadowBlob();
            }
            else if (nShadows == 1) {
                if (IsPlayerCreature(m_idSelf)) {
                    TurnOnShadows();
                    HideShadowBlob();
                }
                else {
                    TurnOffShadows();
                    ShowShadowBlob();
                }
            }
            else {
                TurnOffShadows();
                ShowShadowBlob();
            }
        }
        m_bShadowBlob = 1;
    }
    else {
        if (m_pAnimationBase) {
            if (nShadows == 2) {
                TurnOffShadows();
                HideShadowBlob();
            }
            else if (nShadows == 1) {
                if (IsPlayerCreature(m_idSelf)) {
                    TurnOffShadows();
                    HideShadowBlob();
                }
                else {
                    TurnOffShadows();
                    HideShadowBlob();
                }
            }
            else {
                TurnOffShadows();
                ShowShadowBlob();
            }
        }
        m_bShadowBlob = 0;
    }
}

// FUNCTION: SWKOTOR 0x0063D010
unsigned long CSWCObject::GetProjectileTravelTime(const Vector& vTarget, int bFast)
{
    float fDistance = Magnitude(vTarget - m_vPosition);
    float fSpeed;
    if (bFast) {
        fSpeed = 37.0f;
    }
    else {
        fSpeed = 42.0f;
    }
    return (unsigned long)(fDistance / fSpeed * 1000.0f);
}

// FUNCTION: SWKOTOR 0x0063D780
float CSWCObject::GetStickyMouseThreshhold()
{
    float fHeight = g_nScreenHeight * 0.1f;
    float fWidth = g_nScreenWidth * 0.1f;
    return fWidth * fWidth + fHeight * fHeight;
}

// FUNCTION: SWKOTOR 0x0063DEE0
float CSWCObject::GetFinalFadeAlpha()
{
    if (AsSWCCreature()) {
        CSWCPlayerCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
        if (pPlayer) {
            for (int i = 0; i < pPlayer->m_lObjectRecords.num; i++) {
                if (pPlayer->m_lObjectRecords.element[i]->m_oidObject == m_idSelf) {
                    return g_fOne;
                }
            }
            if (m_nDesiredFadeState != 1) {
                return g_fZero;
            }
        }
    }
    else if (m_nDesiredFadeState != 1) {
        return g_fZero;
    }
    float fAlpha = GetVisualEffectAlpha();
    if (fAlpha == -1.0f) {
        return g_fOne;
    }
    return fAlpha;
}

// FUNCTION: SWKOTOR 0x0063DF60
int CSWCObject::FadeObject()
{
    if (m_nFadeState != m_nDesiredFadeState) {
        unsigned long nDelta = (unsigned long)(
            g_pAppManager->m_pClientExoApp->GetWorldTimer()->GetSnapshotTimeDifference() / 1000);
        if (m_nDesiredFadeState == 1) {
            if (GetFinalFadeAlpha() <= m_fFadeAlpha) {
                m_fFadeAlpha = GetFinalFadeAlpha();
                m_nFadeState = 1;
            }
            else {
                m_fFadeAlpha += nDelta * 0.0005f;
                m_nFadeState = 2;
            }
        }
        else if (m_nDesiredFadeState == 0) {
            if (m_nFadeDelay) {
                unsigned long nDay;
                unsigned long nTime;
                unsigned long nDiffDay;
                unsigned long nDiffTime;
                CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                pClient->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
                pClient = g_pAppManager->m_pClientExoApp;
                pClient->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nFadeStartDay,
                                                             m_nFadeStartTime, &nDiffDay,
                                                             &nDiffTime);
                if (nDiffTime <= m_nFadeDelay) {
                    return 0;
                }
            }
            CSWCCreature* pCreature = AsSWCCreature();
            if (pCreature && pCreature->m_p3A4) {
                pCreature->SetPartyMember(0);
            }
            if (GetFinalFadeAlpha() >= m_fFadeAlpha) {
                m_fFadeAlpha = GetFinalFadeAlpha();
                m_nFadeState = 0;
                if (m_bTimedFade) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    pClient->GetWorldTimer()->GetWorldTime(&m_nFadeStartDay, &m_nFadeStartTime);
                    m_pAnimationBase->AttachToScene(0);
                }
                pCreature = AsSWCCreature();
                if (pCreature && pCreature->m_p3A4) {
                    pCreature->SetPartyMember(0);
                }
            }
            else {
                m_fFadeAlpha -= nDelta * 0.0005f;
                m_nFadeState = 3;
            }
        }

        if (m_fFadeAlpha < 0.0f) {
            m_fFadeAlpha = 0.0f;
        }
        else if (m_fFadeAlpha > 1.0f) {
            m_fFadeAlpha = 1.0f;
        }
        if (m_pAnimationBase) {
            m_pAnimationBase->SetAlpha(m_fFadeAlpha, 0xff);
        }
        return 0;
    }

    if (m_bTimedFade) {
        unsigned long nDay;
        unsigned long nTime;
        unsigned long nDiffDay;
        unsigned long nDiffTime;
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
        pClient = g_pAppManager->m_pClientExoApp;
        pClient->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nFadeStartDay,
                                                     m_nFadeStartTime, &nDiffDay, &nDiffTime);
        if (nDiffTime >= m_nFadeDelay + m_nFadeDuration) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0063E220
void CSWCObject::SetFadeState(unsigned char nState)
{
    if (this && m_bNoServerLink == 1) {
        return;
    }
    m_nFadeState = nState;
    unsigned char nShadows = g_pAppManager->m_pClientExoApp->GetOptions()->m_nShadows;
    if (nState == 1) {
        if (m_pAnimationBase) {
            if (nShadows == 2) {
                TurnOnShadows();
                HideShadowBlob();
            }
            else if (nShadows == 1) {
                if (IsPlayerCreature(m_idSelf)) {
                    TurnOnShadows();
                    HideShadowBlob();
                }
                else {
                    TurnOffShadows();
                    ShowShadowBlob();
                }
            }
            else {
                TurnOffShadows();
                ShowShadowBlob();
            }
        }
        m_bShadowBlob = 1;
        m_nDesiredFadeState = 1;
        m_fFadeAlpha = GetFinalFadeAlpha();
    }
    else {
        if (m_pAnimationBase) {
            if (nShadows == 2) {
                TurnOffShadows();
                HideShadowBlob();
            }
            else if (nShadows == 1) {
                if (IsPlayerCreature(m_idSelf)) {
                    TurnOffShadows();
                    HideShadowBlob();
                }
                else {
                    TurnOffShadows();
                    HideShadowBlob();
                }
            }
            else {
                TurnOffShadows();
                ShowShadowBlob();
            }
        }
        m_bShadowBlob = 0;
        m_nDesiredFadeState = 0;
        m_fFadeAlpha = GetFinalFadeAlpha();
    }
    if (m_pAnimationBase) {
        m_pAnimationBase->SetAlpha(m_fFadeAlpha, 0xff);
    }
}

// FUNCTION: SWKOTOR 0x0063E350
void CSWCObject::SetObjectVisibilityOnSetArea(CSWCArea* pArea)
{
    Vector vPosition;
    if (m_nObjectType != OBJECT_TYPE_PLACEABLE && m_nObjectType != OBJECT_TYPE_DOOR &&
        m_nObjectType != OBJECT_TYPE_ITEM) {
        return;
    }
    if (m_nObjectType == OBJECT_TYPE_PLACEABLE) {
        CSWSPlaceable* pPlaceable = AsSWCPlaceable()->GetServerPlaceable();
        if (pPlaceable && pPlaceable->m_n440 && !pPlaceable->m_n444) {
            return;
        }
    }
    vPosition = m_vPosition;
    if (!(m_vPosition == Vector()) && pArea->GetObjectsAtPositionVisible(vPosition)) {
        SetFadeState(1);
        SetDesiredFadeState(1, 0, 0, 0);
        return;
    }
    SetFadeState(0);
    SetDesiredFadeState(0, 0, 0, 0);
}

// FUNCTION: SWKOTOR 0x0063F340
void CSWCObject::SetPosition(const Vector& v)
{
    m_vPosition = v;
    if (g_bUpdateCreatureOnMove) {
        AsSWCCreature();
    }
    if (m_nObjectType == OBJECT_TYPE_PLACEABLE || m_nObjectType == OBJECT_TYPE_DOOR ||
        m_nObjectType == OBJECT_TYPE_ITEM) {
        int bSkip = 0;
        if (m_nObjectType == OBJECT_TYPE_PLACEABLE) {
            CSWSPlaceable* pPlaceable = AsSWCPlaceable()->GetServerPlaceable();
            if (pPlaceable && pPlaceable->m_n440 && !pPlaceable->m_n444) {
                bSkip = 1;
            }
        }
        if (!bSkip && m_pArea && m_pArea->GetObjectsAtPositionVisible(v)) {
            SetFadeState(1);
            SetDesiredFadeState(1, 0, 0, 0);
        }
    }
    if (m_pAnimationBase && !m_n50) {
        m_pAnimationBase->SetPosition(v);
    }
    Unknown90();
}

// FUNCTION: SWKOTOR 0x0063D560
unsigned char CSWCObject::GetReticlePoint(Vector& v)
{
    int bResult = 0;
    if (m_pAnimationBase) {
        CAurObject* pModel = m_pAnimationBase->GetModel(0xff);
        if (pModel) {
            Quaternion qOrientation;
            CAurPart* pPart = pModel->FindPart("lookathook");
            if (pPart) {
                pPart->GetWorldTransform(v, qOrientation);
                CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
                Vector vBack = -pCamera->GetOrientation().ToDirection();
                int nResult;
                if (vBack * v > vBack * pCamera->GetPosition()) {
                    nResult = 1;
                }
                else {
                    nResult = 2;
                }
                v = pCamera->WorldToScreen(v, 1);
                v.x *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenWidth;
                v.y *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenHeight;
                return (unsigned char)nResult;
            }
        }
        bResult = m_pAnimationBase->GetReticlePoint(v);
    }
    return (unsigned char)bResult;
}
