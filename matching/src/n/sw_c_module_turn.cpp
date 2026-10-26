// CSWCModule::TurnCamera and SetCameraFollow (nwcmodule.o).

#include "n/sw_c_module_control.h"

#define N_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define N_MAX(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: SWKOTOR 0x0063FE10
void CSWCModule::TurnCamera(float fAmount, float fScale)
{
    if (g_pAppManager->m_pClientExoApp->GetCombatMode()) {
        CSWBehaviorCameraCombat* pCombat = (CSWBehaviorCameraCombat*)m_pCamera->GetBehavior(0x1070);
        if (pCombat) {
            pCombat->m_fTurnAccel = 0.0f;
            pCombat->SetTurn(-fAmount);
        }
        return;
    }
    if (m_nCameraMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick) {
            pStick->m_f100 = fAmount;
            pStick->m_f10C = 0.0f;
        }
    }
    else if (m_nCameraMode == 5) {
        CSWCameraFreeLook* pFreeLook = (CSWCameraFreeLook*)m_pCamera->GetBehavior(0x106e);
        if (pFreeLook) {
            float fTurn = N_MAX(-30.0f, N_MIN(30.0f, fAmount * 0.5f));
            float fRate = pFreeLook->m_fRotateSpeed;
            fAmount = -(fTurn * (fRate * fScale));
            CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
            if (pCreature) {
                pCreature->m_fCameraTurnAccel = 0.0f;
                Quaternion q = DirectionToQuaternion(pCreature->m_vOrientation);
                q = EulerToQuaternion(GetPitch(q) + fAmount, GetRoll(q), GetEulerRoll(q));
                pCreature->SetOrientation(q.ToDirection());
                if (pCreature->AsServerCreature())
                    pCreature->AsServerCreature()->SetOrientation(q.ToDirection());
            }
        }
    }
    else if (m_nCameraMode == 7) {
        CSWCameraNavigate* pNavigate = (CSWCameraNavigate*)m_pCamera->GetBehavior(0x1071);
        if (pNavigate)
            pNavigate->m_fTurn = fAmount * -60.0f;
    }
    else {
        if (m_pCamera) {
            CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
            if (pBehavior) {
                CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
                if (pTarget)
                    m_fCameraTurn = pTarget->m_fTurn += fAmount;
            }
        }
        if (fAmount != 0.0f) {
            m_bTurnChanged = 1;
            m_nCameraFlags |= 1;
        }
    }
}

// Matched at 0x0063F600 (sw_behavior_camera_stick_position.cpp); defined
// here unannotated so cl inlines it into SetCameraFollow.
Vector CSWCameraOnAStick::GetCameraPosition()
{
    if (m_pCamera)
        return m_pCamera->GetPosition();
    return Vector(0.0f, 0.0f, 0.0f);
}

// FUNCTION: SWKOTOR 0x0063F9D0
void CSWCModule::SetCameraFollow(unsigned long oidFollow, int bKeepPosition)
{
    if (g_pAppManager->m_pClientExoApp->GetCombatMode())
        return;
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    if (m_nCameraMode == 3) {
        CSWCCreature* pCreature = pApp->GetCreatureByGameObjectID(oidFollow);
        if (!pCreature)
            return;
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (!pStick) {
            pStick = new CSWCameraOnAStick(m_pCamera->GetGob(), pCreature->GetGob());
            m_pCamera->SetBehavior(pStick);
        }
        Vector vPosition = pStick->GetCameraPosition();
        pStick->m_nFollowBody = pCreature->GetFollowBody();
        pStick->m_fFollowHeight = pCreature->GetHeadHeight() + pCreature->GetCameraOffset();
        pStick->ChangeTarget(pCreature->GetGob(), bKeepPosition);
        m_oidFollow = oidFollow;
        if (!bKeepPosition) {
            pStick->SetCameraPosition(vPosition);
            pStick->m_vPosition = Vector(vPosition);
        }
        return;
    }
    CGameObject* pGameObject = pApp->GetGameObject(oidFollow);
    if (!pGameObject)
        return;
    CSWCObject* pObject = pGameObject->AsSWCObject();
    if (!pObject)
        return;
    Gob* pModel = pObject->GetModel(0xff, 1);
    if (!pModel)
        return;
    unsigned long oidPrevious = m_oidFollow;
    m_oidFollow = oidFollow;
    m_bClearViewRequest = bKeepPosition;
    if (!m_pCamera)
        return;
    m_pCamera->SetFollow(pModel);
    m_vCameraTarget = pModel->GetPosition();
    CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
    if (pBehavior) {
        CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
        if (pTarget) {
            pTarget->m_f44 = g_fCameraFollowPitch;
            pTarget->m_f38 = 750.0f;
            if (lockcamera(0)) {
                pTarget->m_f58 = 1.0f;
                pTarget->m_f64 = 1.0f;
            }
            pTarget->m_f5C = g_fCameraFollowC;
            pTarget->m_f60 = g_fCameraFollowPitch;
            pTarget->m_f68 = g_fCameraFollowA;
            pTarget->m_f6C = g_fCameraFollowB;
            if (m_fSavedDistance != 0.0f) {
                pTarget->m_f30 = m_fSavedDistance;
                pTarget->m_f3C = m_fSavedYaw;
                pTarget->m_f44 = m_fSavedPitch;
            }
            pTarget->m_vUp = m_vCameraUp;
        }
    }
    CSWCCreature* pPrevious = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(oidPrevious);
    if (pPrevious)
        pPrevious->UpdateShadowBlob();
    if (pObject->AsSWCCreature())
        pObject->AsSWCCreature()->UpdateShadowBlob();
}
