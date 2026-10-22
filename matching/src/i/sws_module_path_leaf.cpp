// CSWSModule inter-area path leaves (successor generation, intra-area
// goal, inter-area plot). Kept apart from their callers in
// sws_module_path.cpp so /Ob2 does not inline them.
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C3EA0
void CSWSModule::SetIntraAreaGoal(CPathfindInformation* pPath)
{
    if (pPath->m_nAreas <= 1) {
        return;
    }
    pPath->m_v44.x = pPath->m_vGoal.x;
    pPath->m_v44.y = pPath->m_vGoal.y;
    pPath->m_v44.z = pPath->m_vGoal.z;
    pPath->m_oid54 = pPath->m_oidTargetArea;
    pPath->m_f50 = pPath->m_f68;
    CGameObject* pObject;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(pPath->m_pTransitions[0], &pObject) !=
        GAME_OBJECT_VALID) {
        return;
    }
    CSWSObject* pTransition;
    Vector vGoal;
    if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
        pTransition = pObject->AsSWSTrigger();
        Vector vTarget(pPath->m_vTarget.x, pPath->m_vTarget.y, pPath->m_vTarget.z);
        vGoal.x = 0.0f;
        vGoal.y = 0.0f;
        vGoal.z = 0.0f;
        if (((CSWSTrigger*)pTransition)->FUN_0058d8c0(&vTarget, &vGoal) != 1) {
            return;
        }
        pPath->m_f68 = 0.0f;
    } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
        pTransition = pObject->AsSWSDoor();
        ((CSWSDoor*)pTransition)->FUN_00589240(
            &vGoal, Vector(pPath->m_vTarget.x, pPath->m_vTarget.y, pPath->m_vTarget.z), 1);
        pPath->m_f68 = pPath->m_f04 + 0.5f;
    } else {
        return;
    }
    pPath->m_vGoal.y = vGoal.y;
    pPath->m_vGoal.z = vGoal.z;
    pPath->m_vGoal.x = vGoal.x;
    pPath->FUN_005cec90();
    pPath->m_oidTargetArea = pPath->m_oidStartArea;
    pPath->m_oidTransition = pTransition->m_idSelf;
}

// FUNCTION: SWKOTOR 0x004C54A0
int CSWSModule::GenerateInterAreaDFSSuccessors(int nDepth, CPathfindInformation* pPath,
                                               unsigned long** ppList)
{
    CSWSArea* pArea = GetArea();
    if (!pArea) {
        return 0;
    }
    int nCount = pArea->CountAreaTransitionTriggers(NULL, pPath->m_nExcluded, pPath->m_pExcluded);
    if (!nCount) {
        return 0;
    }
    *ppList = new unsigned long[nCount];
    return pArea->CountAreaTransitionTriggers(*ppList, pPath->m_nExcluded, pPath->m_pExcluded);
}

// FUNCTION: SWKOTOR 0x004C6E20
int CSWSModule::PlotInterAreaPath(CPathfindInformation* pPath, int nArg)
{
    if (pPath->m_oidTargetArea == pPath->m_oidStartArea) {
        if (!pPath->m_pAreas) {
            pPath->m_bInterAreaPathFound = 1;
            pPath->m_nAreas = 1;
            unsigned long* pAreas = new unsigned long[1];
            pAreas[0] = pPath->m_oidTargetArea;
            pPath->m_pAreas = pAreas;
            pPath->m_pTransitions = NULL;
        }
    } else if (!pPath->m_pAreas) {
        if (!ComputeInterAreaPath(pPath)) {
            return PLOT_PATH_FAIL;
        }
    } else {
        pPath->FUN_005cec50();
    }
    int nResult = PlotPathInArea(pPath, nArg);
    if (nResult == PLOT_PATH_FAIL) {
        if (pPath->m_nAreas > 1) {
            int nExcluded = pPath->m_nExcluded;
            unsigned long* pExcluded = new unsigned long[nExcluded + 1];
            for (int i = 0; i < nExcluded; i++) {
                pExcluded[i] = pPath->m_pExcluded[i];
            }
            pExcluded[nExcluded] = pPath->m_pTransitions[0];
            pPath->m_nExcluded = nExcluded + 1;
            if (pPath->m_pExcluded) {
                delete[] pPath->m_pExcluded;
            }
            pPath->m_pExcluded = pExcluded;
            pPath->ResetInterAreaPathSearchData();
            pPath->FUN_005cec50();
            if (!ComputeInterAreaPath(pPath)) {
                return PLOT_PATH_FAIL;
            }
            nResult = PLOT_PATH_OK;
            pPath->FUN_005cec50();
        }
    } else if (pPath->m_nAreas > 1) {
        pPath->FUN_005cec50();
    }
    return nResult;
}
