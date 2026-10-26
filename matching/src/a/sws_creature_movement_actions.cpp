#include <stdlib.h>
#include "a/sws_creature.h"
extern const unsigned long OBJECT_INVALID; // 0x00746644

// NONMATCH: 4 diff: escaped local stack-slot allocation.
// FUNCTION: SWKOTOR 0x005136B0
int CSWSCreature::AIActionCheckForceFollowObject(CSWSObjectActionNode* pNode)
{
    Vector vPosition = m_vPosition;
    unsigned long oid = pNode->m_n38;
    unsigned long nRun = pNode->m_n3C;
    unsigned long nRange = pNode->m_n40;
    unsigned long nRadius = pNode->m_n44;
    vPosition.z = 0.0f;
    unsigned long nZero = 0;
    AddActionToFront(0x3a, 0xffff, 3, &oid, 1, &nRun, 2, &nRange, 2, &nRadius);
    AddActionToFront(0x37, 0xffff, 3, &oid, 1, &nRun, 2, &nRange, 2, &nRadius,
                     1, &nZero, 2, &vPosition.x, 2, &vPosition.y);
    return AI_ACTION_RESULT_005140E0;
}

// NONMATCH: 2 diff: initial count compare is folded into a memory operand.
// FUNCTION: SWKOTOR 0x0051AFD0
void CSWSCreature::SetVisibilityList(CExoArrayList<CSWSCreatureListEntry*>* pList)
{
    CExoArrayList<CSWSCreatureListEntry*>* pOwn = &m_l920;
    if (pOwn->element) {
        delete[] pOwn->element;
        pOwn->element = 0;
        pOwn->array_size = 0;
    }
    pOwn->num = 0;
    if (!pList) return;
    int nCount = pList->num;
    for (int i = 0; i < nCount; i++, nCount = pList->num) pOwn->Add(pList->element[i]);
}

// NONMATCH: 11 diff: vector stack-slot reuse and x87 scheduling.
// FUNCTION: SWKOTOR 0x00510670
int CSWSCreature::AIActionCheckMoveToPoint(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    if (!(m_nPartyFollowFlags & 2)) return AI_ACTION_RESULT_005140D0;
    float x = pNode->m_f38, y = pNode->m_f3C, z = pNode->m_f40;
    unsigned long oidArea = pNode->m_n44, oidTarget = pNode->m_n48;
    int bRun = pNode->m_n4C;
    CGameObject* pObject;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(oidArea, &pObject) != GAME_OBJECT_VALID)
        return AI_ACTION_RESULT_005140D0;
    float fRange = m_pFollowInfo->m_fHitRadius;
    int bOtherArea = GetArea() && oidArea != GetArea()->m_idSelf;
    Vector vPoint(x,y,z);
    Vector vDelta = VectorSubtract(m_vPosition, vPoint);
    if (bOtherArea || (fRange + fRange)*(fRange + fRange) <=
        vDelta.z*vDelta.z + vDelta.y*vDelta.y + vDelta.x*vDelta.x) {
    AddActionToFront(0xa, pNode->m_nGroupId, 2, &x, 2, &y, 2, &z,
        3, &oidArea, 3, &oidTarget, 1, reinterpret_cast<void*>(bRun));
    Vector vZero(0.0f,0.0f,0.0f);
    AddMoveToPointActionToFront(pNode->m_nGroupId, Vector(x,y,z), oidArea, oidTarget, bRun,
        0.0f,0.0f,0,0,0,0,0,0,vZero,0,0,0);
    AddPathfindingWaitActionToFront(pNode->m_nGroupId);
    }
    return AI_ACTION_RESULT_005140E0;
}
// NONMATCH: 29 diff: vector materialization and area/argument scheduling.
// FUNCTION: SWKOTOR 0x005108A0
int CSWSCreature::AIActionCheckMoveToPointRadius(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    float x = pNode->m_f38, y = pNode->m_f3C, z = pNode->m_f40;
    unsigned long oidArea = pNode->m_n44, oidTarget = pNode->m_n48;
    int bRun = pNode->m_n4C;
    float fRadius = pNode->m_f50;
    int nFlag = pNode->m_n54;
    CGameObject* pObject;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(oidArea, &pObject) != GAME_OBJECT_VALID)
        return AI_ACTION_RESULT_005140D0;
    int bOtherArea = GetArea() && oidArea != GetArea()->m_idSelf;
    Vector vDifference(m_vPosition.x-x, m_vPosition.y-y, 0.0f);
    if (bOtherArea || (fRadius+0.01f)*(fRadius+0.01f) < vDifference.y*vDifference.y+vDifference.x*vDifference.x) {
    AddActionToFront(0x12, pNode->m_nGroupId, 2, &x, 2, &y, 2, &z, 3, &oidArea,
        3, &oidTarget, 1, &bRun, 2, &fRadius, 1, &nFlag);
    Vector vZero(0.0f,0.0f,0.0f);
    AddMoveToPointActionToFront(pNode->m_nGroupId, Vector(x,y,z), oidArea, oidTarget, bRun,
        fRadius,0.0f,0,0,0,0,0,0,vZero,0,0,nFlag);
    AddPathfindingWaitActionToFront(pNode->m_nGroupId);
    }
    return AI_ACTION_RESULT_005140E0;
}
// NONMATCH: 23 diff: x87 compare order and area/party register scheduling.
// FUNCTION: SWKOTOR 0x00510AB0
int CSWSCreature::AIActionCheckMoveToFollowRadius(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    float x = pNode->m_f38, y = pNode->m_f3C, z = pNode->m_f40;
    unsigned long oidArea = pNode->m_n44;
    float fRadius = pNode->m_f50;
    CGameObject* pObject;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(oidArea, &pObject) != GAME_OBJECT_VALID)
        return AI_ACTION_RESULT_005140D0;
    int bOtherArea = GetArea() && oidArea != GetArea()->m_idSelf;
    float dx = m_vPosition.x - x, dy = m_vPosition.y - y, dz = m_vPosition.z - z;
    if (bOtherArea || (fRadius+0.01f)*(fRadius+0.01f) < dz*dz+dy*dy+dx*dx) {
    unsigned long oidClient = GetClientExoApp()->ServerToClientObjectId(m_idSelf);
    int nIndex = GetClientExoApp()->GetSWParty()->IsPartyMember(oidClient);
    if (nIndex >= 0) GetClientExoApp()->GetSWParty()->m_aMovement[nIndex].m_nState = 5;
    }
    return AI_ACTION_RESULT_005140E0;
}

class CSWSObject {
public:
    CSWSArea* GetArea(); // 0x004CB120
};
// FUNCTION: SWKOTOR 0x005101A0
int CSWSCreature::AIActionCheckMoveToObject(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    unsigned long oid = pNode->m_n38;
    int bRun = pNode->m_n3C;
    Vector vPosition(0.0f,0.0f,0.0f);
    float fRange = 0.0f;
    CGameObject* pObject;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(oid, &pObject) != GAME_OBJECT_VALID)
        return AI_ACTION_RESULT_005140D0;
    CSWSArea* pArea = reinterpret_cast<CSWSObject*>(pObject)->GetArea();
    GetUseRange(oid, vPosition, fRange, 0);
    if (!pArea) {
        if (!pObject->AsSWSCreature()) return AI_ACTION_RESULT_005140D0;
        pArea = GetServerExoApp()->GetModule6B0()->GetArea();
        vPosition = pObject->AsSWSCreature()->m_v314;
        if (!pArea) return AI_ACTION_RESULT_005140D0;
    }
    if (pArea != GetArea() || !GetIsInUseRange(oid, 0.0f, 0)) {
        unsigned long oidArea = pArea->m_idSelf;
        AddActionToFront(2, pNode->m_nGroupId, 3, &oid, 1, &bRun);
        Vector vZero(0.0f,0.0f,0.0f);
        AddMoveToPointActionToFront(pNode->m_nGroupId, vPosition, oidArea, oid,
            bRun,fRange,0.0f,0,0,0,0,0,0,vZero,0,0,0);
        AddPathfindingWaitActionToFront(pNode->m_nGroupId);
    }
    SetFacingTarget4F34A0(OBJECT_INVALID, 0);
    return AI_ACTION_RESULT_005140E0;
}

// NONMATCH (41): x87 scheduling and this/node register allocation.
// FUNCTION: SWKOTOR 0x0050FD50
int CSWSCreature::AIActionCheckMoveAwayFromLocation(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    Vector vFrom(0.0f, 0.0f, 0.0f);
    CSWSArea* pArea = GetArea();
    vFrom.x = pNode->m_f38;
    vFrom.y = pNode->m_f3C;
    vFrom.z = pNode->m_f40;
    unsigned long nRun = pNode->m_n44;
    float fDistance = pNode->m_f48;
    unsigned long nArg = pNode->m_n4C;
    float dx = m_vPosition.x - vFrom.x;
    float dy = m_vPosition.y - vFrom.y;
    float dz = m_vPosition.z - vFrom.z;
    if (fDistance * fDistance >= dz * dz + dy * dy + dx * dx) {
        Vector vAway = pArea->ComputeAwayVector(m_vPosition, vFrom, fDistance, m_pFollowInfo);
        unsigned long oidArea = pArea->m_idSelf;
        AddActionToFront(0x2c, pNode->m_nGroupId, 2, &vFrom.x, 2, &vFrom.y, 2, &vFrom.z,
                         1, &nRun, 2, &fDistance, 1, &nArg);
        Vector vZero(0.0f, 0.0f, 0.0f);
        AddMoveToPointActionToFront(pNode->m_nGroupId, vAway, oidArea, 0x7f000000, nRun,
                                    0.0f, 0.0f, 0, 0, 0, 0, 1, 0, vZero, 0, 0, 0);
        AddPathfindingWaitActionToFront(pNode->m_nGroupId);
    }
    return AI_ACTION_RESULT_005140E0;
}
