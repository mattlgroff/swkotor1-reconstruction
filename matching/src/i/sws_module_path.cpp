// CSWSModule inter-area depth-first search, path computation and
// PlotPath (callers of sws_module_path_leaf.cpp).
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C5510
int CSWSModule::InterAreaDFS(int nDepth, int nMaxDepth, CPathfindInformation* pPath)
{
    if (pPath->m_pAreas[nDepth] == pPath->m_oidTargetArea) {
        pPath->m_nAreas = nDepth + 1;
        return 1;
    }
    if (nDepth >= nMaxDepth) {
        return 0;
    }
    int i;
    char szIndent[32];
    for (i = 0; i < nDepth; i++) {
        szIndent[i] = szIndent[i];
    }
    unsigned long* pSuccessors = NULL;
    int nCount = GenerateInterAreaDFSSuccessors(nDepth, pPath, &pSuccessors);
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    for (i = 0; i < nCount; i++) {
        pPath->m_pTransitions[nDepth] = pSuccessors[i];
        CGameObject* pObject;
        if (pArray->GetGameObject(pSuccessors[i], &pObject) == GAME_OBJECT_VALID) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                CSWSTrigger* pTrigger = pObject->AsSWSTrigger();
                if (pTrigger->m_bAreaTransition) {
                    if (pArray->GetGameObject(pTrigger->GetTargetArea(), &pObject) ==
                        GAME_OBJECT_VALID) {
                        CSWSArea* pArea = pObject->AsSWSArea();
                        if (pArea) {
                            pPath->m_pAreas[nDepth + 1] = pArea->m_oid120;
                            if (InterAreaDFS(nDepth + 1, nMaxDepth, pPath) == 1) {
                                if (pSuccessors) {
                                    delete[] pSuccessors;
                                }
                                return 1;
                            }
                        }
                    }
                }
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                CSWSDoor* pDoor = pObject->AsSWSDoor();
                if (pDoor && pDoor->FUN_005890d0()) {
                    CSWSObject* pLinked = pDoor->GetLinkedObject();
                    if (pLinked) {
                        CSWSArea* pArea = pLinked->GetArea();
                        if (pArea) {
                            pPath->m_pAreas[nDepth + 1] = pArea->m_oid120;
                            if (InterAreaDFS(nDepth + 1, nMaxDepth, pPath) == 1) {
                                if (pSuccessors) {
                                    delete[] pSuccessors;
                                }
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if (pSuccessors) {
        delete[] pSuccessors;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004C6BD0
int CSWSModule::ComputeInterAreaPath(CPathfindInformation* pPath)
{
    pPath->m_nAreas = 10;
    pPath->m_pAreas = new unsigned long[10];
    pPath->m_pTransitions = new unsigned long[9];
    pPath->m_bInterAreaPathFound = 0;
    for (int nMax = 1; nMax < 10; nMax++) {
        if (pPath->m_bInterAreaPathFound) {
            break;
        }
        pPath->m_pAreas[0] = pPath->m_oidStartArea;
        CSWSArea* pArea = GetServerExoApp()->GetAreaByGameObjectID(m_oidArea);
        if (pArea) {
            pArea->m_n1e0 = 0;
        }
        int nResult = InterAreaDFS(0, nMax, pPath);
        if (nResult == 1) {
            pPath->m_bInterAreaPathFound = nResult;
        }
    }
    if (!pPath->m_bInterAreaPathFound) {
        return 0;
    }
    SetIntraAreaGoal(pPath);
    return 1;
}

// FUNCTION: SWKOTOR 0x004C6F70
int CSWSModule::PlotPath(CPathfindInformation* pPath, int nArg)
{
    if (!pPath) {
        return PLOT_PATH_FAIL;
    }
    if (pPath->m_nState == 0 && pPath->m_n88 == 1) {
        return PLOT_PATH_DONE;
    }
    if (pPath->m_nState == 1) {
        if (pPath->m_nA4 == 1) {
            pPath->ResetInterTileSearchData();
            pPath->ResetGridSearchData();
            pPath->m_nA4 = 0;
        } else {
            int n84 = pPath->m_n84;
            pPath->ResetInterAreaSearchData();
            pPath->m_n84 = n84;
        }
        pPath->m_n3c = 0;
        pPath->m_n88 = 0;
        pPath->m_nState = 0;
    }
    GetServerExoApp()->GetServerAIMaster()->m_n68 = 1;
    int nResult = PlotInterAreaPath(pPath, nArg);
    if (g_bWalkDebug) {
        unsigned long oidCreature = pPath->m_oidCreature;
        CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
        if (nResult == PLOT_PATH_DONE) {
            pCreature->FUN_005111c0();
            pCreature->FUN_005111c0();
        }
    }
    return nResult;
}
