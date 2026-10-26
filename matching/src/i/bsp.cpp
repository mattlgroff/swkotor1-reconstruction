// BSP tree helpers (bsp.o, 0x004ACED0-0x004AD7E0). Names from the Mac
// debug map where it has them (BSPNode ctor/dtor, BSPFitSphere); the
// rest are descriptive.
#include "i/bsp.h"
#include <math.h>

// FUNCTION: SWKOTOR 0x004ACED0
BSPNode::~BSPNode()
{
    if (m_pFront) {
        delete m_pFront;
    }
    if (m_pBack) {
        delete m_pBack;
    }
}

// FUNCTION: SWKOTOR 0x004ACF60 SYMBOL: ?BSPTraverse@@YAXPAVBSPNode@@PAVVector@@MP6AX01M@Z@Z
void BSPTraverse(BSPNode* pNode, Vector* pCenter, float fRadiusSq,
                 void (*pVisit)(BSPNode*, Vector*, float))
{
    if (pNode == 0) {
        return;
    }
    float fRadius = (float)sqrt(fRadiusSq);
    pVisit(pNode, pCenter, fRadiusSq);
    float fDist = pNode->n.z * pCenter->z + pNode->n.y * pCenter->y +
                  pNode->n.x * pCenter->x + pNode->d;
    if (fDist > -fRadius) {
        BSPTraverse(pNode->m_pBack, pCenter, fRadiusSq, pVisit);
    }
    if (fDist < fRadius) {
        BSPTraverse(pNode->m_pFront, pCenter, fRadiusSq, pVisit);
    }
}

// FUNCTION: SWKOTOR 0x004ACFF0
int RFaceSplitTest(CExoArrayList<Vector>* pFace, Plane* pPlane)
{
    int nSides = 0;
    for (int i = 0; i < pFace->num; i++) {
        Vector* pV = &pFace->element[i];
        nSides |= pV->z * pPlane->n.z + pV->y * pPlane->n.y + pV->x * pPlane->n.x + pPlane->d < g_fZero ? 1 : 2;
        if (nSides == 3) {
            break;
        }
    }
    return nSides;
}

// FUNCTION: SWKOTOR 0x004AD060
BSPNode* BSPFitSphere(BSPNode* pNode, const Vector& vCenter, float fRadius)
{
    if (pNode->m_bLeaf) {
        return pNode;
    }
    float fDist = pNode->n.z * vCenter.z + pNode->n.y * vCenter.y +
                  pNode->n.x * vCenter.x + pNode->d;
    if (fDist <= -fRadius) {
        return BSPFitSphere(pNode->m_pFront, vCenter, fRadius);
    }
    if (fDist >= fRadius) {
        return BSPFitSphere(pNode->m_pBack, vCenter, fRadius);
    }
    return pNode;
}

// FUNCTION: SWKOTOR 0x004AD0D0
BSPNode* BSPFitBox(BSPNode* pNode, Vector* pMin, Vector* pMax)
{
    if (pNode->m_bLeaf) {
        return pNode;
    }
    Plane flipped(-pNode->n, -pNode->d);
    if (FUN_004aaf50(*pMin, *pMax, flipped) >= g_fBoxEpsilon) {
        return BSPFitBox(pNode->m_pFront, pMin, pMax);
    }
    if (FUN_004aaf50(*pMin, *pMax, *pNode) >= g_fBoxEpsilon) {
        return BSPFitBox(pNode->m_pBack, pMin, pMax);
    }
    return pNode;
}

// FUNCTION: SWKOTOR 0x004AD1A0
BSPNode* BSPSearch(BSPNode* pNode, Vector vPoint, int (*pTest)(BSPNode*, void*), void* pContext)
{
    if (pTest(pNode, pContext)) {
        return pNode;
    }
    if (pNode->m_bLeaf) {
        return 0;
    }
    BSPNode* pNear;
    BSPNode* pFar;
    if (vPoint.z * pNode->n.z + vPoint.y * pNode->n.y +
        vPoint.x * pNode->n.x + pNode->d < g_fZero) {
        pNear = pNode->m_pFront;
        pFar = pNode->m_pBack;
    } else {
        pNear = pNode->m_pBack;
        pFar = pNode->m_pFront;
    }
    BSPNode* pFound = BSPSearch(pNear, vPoint, pTest, pContext);
    if (pFound) {
        return pFound;
    }
    return BSPSearch(pFar, vPoint, pTest, pContext);
}

// FUNCTION: SWKOTOR 0x004AD260
int BSPHitCheck(BSPNode* pNode, Vector vStart, Vector vEnd, Vector* pHit,
                int (*pHitTest)(BSPNode*, Vector, Vector, Vector*))
{
    int nResult = 0;
    if (pNode == 0) {
        return 0;
    }
    if (pHitTest) {
        nResult = pHitTest(pNode, vStart, vEnd, &vEnd);
        if (nResult) {
            *pHit = vEnd;
            nResult = 1;
        }
    }
    if (pNode->m_bLeaf) {
        if (pHitTest == 0 && pNode->m_bLeaf == 1) {
            if (pHit) {
                *pHit = vStart;
            }
            return 1;
        }
        return nResult;
    }
    int bStartFront = vStart.z * pNode->n.z + vStart.y * pNode->n.y + vStart.x * pNode->n.x + pNode->d > 0.0f;
    int bEndFront = vEnd.z * pNode->n.z + vEnd.y * pNode->n.y + vEnd.x * pNode->n.x + pNode->d > 0.0f;
    if (!bStartFront && !bEndFront) {
        return BSPHitCheck(pNode->m_pFront, vStart, vEnd, pHit, pHitTest) | nResult;
    }
    if (bStartFront == 1 && bEndFront == 1) {
        return BSPHitCheck(pNode->m_pBack, vStart, vEnd, pHit, pHitTest) | nResult;
    }
    Vector vSplit = planelineintersection(pNode->n, pNode->d, vStart, vEnd);
    if (!bStartFront) {
        if (BSPHitCheck(pNode->m_pFront, vStart, vSplit, pHit, pHitTest)) {
            return 1;
        }
        return BSPHitCheck(pNode->m_pBack, vSplit, vEnd, pHit, pHitTest) | nResult;
    }
    if (BSPHitCheck(pNode->m_pBack, vStart, vSplit, pHit, pHitTest)) {
        return 1;
    }
    return BSPHitCheck(pNode->m_pFront, vSplit, vEnd, pHit, pHitTest) | nResult;
}

// FUNCTION: SWKOTOR 0x004AD560
void BSPSphereCheck(BSPNode* pNode, Vector* pCenter, float fRadius, CExoArrayList<BSPNode*>* pOut)
{
    if (pNode->m_bLeaf) {
        pOut->Add(pNode);
        return;
    }
    float fDist = pNode->n.z * pCenter->z + pNode->n.y * pCenter->y +
                  pNode->n.x * pCenter->x + pNode->d;
    if (fDist <= -fRadius) {
        BSPSphereCheck(pNode->m_pFront, pCenter, fRadius, pOut);
        return;
    }
    if (!(fDist >= fRadius)) {
        BSPSphereCheck(pNode->m_pFront, pCenter, fRadius, pOut);
    }
    BSPSphereCheck(pNode->m_pBack, pCenter, fRadius, pOut);
}

// FUNCTION: SWKOTOR 0x004AD630
BSPNode::BSPNode(Vector vNormal, float fDist)
    : Plane(vNormal, fDist), m_vMin(0.0f, 0.0f, 0.0f), m_vMax(0.0f, 0.0f, 0.0f),
      m_lPlanes34(0), m_lPlanes40(0), m_lPoints(0)
{
    m_pFront = 0;
    m_pBack = 0;
    m_bLeaf = 0;
    m_pData = 0;
}

// FUNCTION: SWKOTOR 0x004AD6D0 SYMBOL: ?BSPTraverse@@YAXPAVBSPNode@@VVector@@P6AX0PAXPAV?$CExoArrayList@VPlane@@@@@Z2@Z
void BSPTraverse(BSPNode* pNode, Vector vPoint, void (*pVisit)(BSPNode*, void*, CExoArrayList<Plane>*),
                 void* pContext)
{
    if (pNode == 0) {
        return;
    }
    CExoArrayList<Plane> planes(0);
    pVisit(pNode, pContext, &planes);
    float fDist = vPoint.z * pNode->n.z + vPoint.y * pNode->n.y +
                  vPoint.x * pNode->n.x + pNode->d;
    BSPNode* pSecond = fDist > g_fZero ? pNode->m_pBack : pNode->m_pFront;
    BSPNode* pFirst = fDist > g_fZero ? pNode->m_pFront : pNode->m_pBack;
    BSPTraverse(pFirst, vPoint, pVisit, pContext);
    BSPTraverse(pSecond, vPoint, pVisit, pContext);
}

// NONMATCH: cl tail-merges the split==1 Add into the split==2 one and the
// plane pointer lands in ecx instead of edx; flag slots/regs match.
// FUNCTION: SWKOTOR 0x004AD7E0
void BSPTraverseVolumeApprox(BSPNode* pNode, Vector* pPoint, CExoArrayList<Plane>* pPlanes,
                             void (*pVisit)(BSPNode*, void*, CExoArrayList<Plane>*), void* pContext)
{
    if (pNode == 0) {
        return;
    }
    pVisit(pNode, pContext, pPlanes);
    if (pNode->m_bLeaf) {
        return;
    }
    pNode->m_lPlanes34.num = 0;
    pNode->m_lPlanes40.num = 0;
    unsigned char bFrontDone = 0;
    unsigned char bBackDone = 0;
    for (int i = pPlanes->num; i != 0;) {
        i--;
        int nSplit = RFaceSplitTest(&pNode->m_lPoints, &pPlanes->element[i]);
        if (nSplit == 3) {
            pNode->m_lPlanes40.Add(pPlanes->element[i]);
            pNode->m_lPlanes34.Add(pPlanes->element[i]);
        }
        Plane* pPlane = &pPlanes->element[i];
        bool bFacing = pPlane->n.y * pNode->n.y + pPlane->n.z * pNode->n.z + pNode->n.x * pPlane->n.x > 0.0f;
        if (nSplit == 1) {
            if (bFacing) {
                if (!bFrontDone) {
                    pNode->m_lPlanes40.Add(*pPlane);
                    continue;
                }
            } else if (!bBackDone) {
                pNode->m_lPlanes34.Add(*pPlane);
            }
        } else if (nSplit == 2) {
            if (bFacing) {
                bFrontDone = 1;
                if (bBackDone) {
                    return;
                }
                pNode->m_lPlanes34.Add(*pPlane);
                continue;
            }
            bBackDone = 1;
            if (bFrontDone) {
                return;
            }
            pNode->m_lPlanes40.Add(*pPlane);
            continue;
        }
        if (bFrontDone && bBackDone) {
            return;
        }
    }
    float fDist = pNode->n.z * pPoint->z + pNode->n.y * pPoint->y + pNode->n.x * pPoint->x + pNode->d;
    BSPNode* pNear = fDist > 0.0f ? pNode->m_pBack : pNode->m_pFront;
    BSPNode* pFar = fDist > 0.0f ? pNode->m_pFront : pNode->m_pBack;
    int nNear = fDist > 0.0f ? pNode->m_lPlanes40.num : pNode->m_lPlanes34.num;
    int nFar = fDist > 0.0f ? pNode->m_lPlanes34.num : pNode->m_lPlanes40.num;
    unsigned char bNearDone = fDist > 0.0f ? bFrontDone : bBackDone;
    unsigned char bFarDone = fDist > 0.0f ? bBackDone : bFrontDone;
    if (!bNearDone) {
        if (nNear) {
            BSPTraverseVolumeApprox(pNear, pPoint, fDist > 0.0f ? &pNode->m_lPlanes40 : &pNode->m_lPlanes34,
                                    pVisit, pContext);
        } else {
            BSPTraverse(pNear, *pPoint, pVisit, pContext);
        }
    }
    if (bFarDone) {
        return;
    }
    if (nFar) {
        BSPTraverseVolumeApprox(pFar, pPoint, fDist > 0.0f ? &pNode->m_lPlanes34 : &pNode->m_lPlanes40,
                                pVisit, pContext);
    } else {
        BSPTraverse(pFar, *pPoint, pVisit, pContext);
    }
}
