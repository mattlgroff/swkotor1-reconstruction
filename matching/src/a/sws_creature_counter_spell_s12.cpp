// CSWSCreature::AIActionCounterSpell: walks into range of the target and
// queues the counter-spell actions (sibling of AddCounterSpellActions).
#include "a/sws_creature.h"

extern const int AI_ACTION_IN_PROGRESS;              // 0x00745DBC

// FUNCTION: SWKOTOR 0x00514270
int CSWSCreature::AIActionCounterSpell(CSWSObjectActionNode* pNode)
{
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if ((GetDead() == 1 && GetIsPossessed() == 1) || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    ActivityManager(4);
    unsigned long oidTarget = pNode->m_n38;
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (!pTarget || pTarget->GetDead() || pTarget->GetIsPossessed() || pTarget->GetDeadTemp())
        return AI_ACTION_RESULT_005140D0;
    CSWSArea* pArea = pTarget->GetArea();
    if (!pArea)
        return AI_ACTION_IN_PROGRESS;
    vPosition = pTarget->m_vPosition;
    float fRange = g_pRules->m_fRangeLong;
    int bRun = 1;
    int bOne = 1;
    {
    Vector vDelta = VectorSubtract(m_vPosition, vPosition);
    unsigned long oidArea = pArea->GetId();
    if (oidArea == GetArea()->GetId() &&
        !(vDelta.x * vDelta.x + vDelta.y * vDelta.y + vDelta.z * vDelta.z > fRange * fRange)) {
        BroadcastCombatStateToParty(oidTarget);
        UpdateFlag4DC();
        unsigned char nMode = m_nCombatMode;
        if (nMode != 4) {
            m_nCombatMode = 4;
            if (nMode != 1) {
                if (nMode != 2) {
                    if (nMode != 3) {
                        SetActivity(0x800, 1, 0);
                    } else {
                        SetActivity(0x200, 0, 0);
                    }
                } else {
                    SetActivity(0x100, 0, 0);
                }
            } else {
                SetActivity(0x400, 0, 0);
            }
        }
        if (m_oidCounterSpellTarget != oidTarget)
            m_oidCounterSpellTarget = oidTarget;
        if (GetLockOrientationToObject() != oidTarget)
            SetFacingTarget4F34A0(oidTarget, 0);
        return AI_ACTION_IN_PROGRESS;
    }
    }
    if (!(m_nPartyFollowFlags & 2)) {
        SendFeedbackMessage(7, NULL);
        return AI_ACTION_RESULT_005140D0;
    }
    AddActionToFront(0x32, pNode->m_nGroupId, 3, &oidTarget);
    AddActionToFront(0x11, pNode->m_nGroupId, 3, &oidTarget, 1, &bRun, 2, &fRange, 2, &fRange, 1, &bOne);
    AddMoveToPointActionToFront(pNode->m_nGroupId, vPosition, pArea->GetId(), oidTarget, bRun, fRange, 0.0f,
                                0, 0, 0, 0, 0, 0, Vector(0.0f, 0.0f, 0.0f), 0, 0, 0);
    AddPathfindingWaitActionToFront(pNode->m_nGroupId);
    return AI_ACTION_RESULT_005140E0;
}
