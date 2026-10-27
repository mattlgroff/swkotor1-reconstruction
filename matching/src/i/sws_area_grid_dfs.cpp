// CSWSArea::GridDFSearch: the depth-first grid path search.
#include <stdlib.h>
#include <string.h>

#include "i/sws_area.h"

// NONMATCH: the original reuses the nDepth/nZ argument slots for the
// transposition-table outputs and the successor array (8-byte frame) and
// walks the successors with a pointer; register allocation differs.
// FUNCTION: SWKOTOR 0x004C0930
int CSWSArea::GridDFSearch(int nDepth, int nCost, int nLimit, int nX, int nY, int nZ)
{
    int* pPath = m_pPathfindInformation->m_pGridPath;
    int nNextDepth = nDepth + 1;
    m_pPathfindInformation->m_nGridPathLength = nNextDepth;
    pPath[nDepth * 2 + 1] = nY;
    pPath[nDepth * 2] = nX;
    int nAbsX = abs(nX);
    int nAbsY = abs(nY);
    if (m_pPathfindInformation->m_nBestGridDistance > nAbsY + nAbsX) {
        m_pPathfindInformation->m_nBestGridDistance = nAbsY + nAbsX;
        memcpy(m_pPathfindInformation->m_pBestGridPath, m_pPathfindInformation->m_pGridPath,
               nDepth * 8 + 8);
        m_pPathfindInformation->m_nBestGridPathLength = nNextDepth;
    }
    if (nX == 0 && nY == 0 && nZ == 0) {
        return 1;
    }
    int nTolerance = m_pPathfindInformation->m_nGridStepTolerance;
    if (nX * nX + nY * nY + nZ * nZ <= nTolerance * nTolerance && nDepth > 0) {
        return 1;
    }
    if (nLimit < nAbsY - nTolerance + nAbsX) {
        return 0;
    }
    int nStoredCost;
    int nStoredLimit;
    if (GridDFSTransTableGet(nX, nY, &nStoredCost, &nStoredLimit)) {
        if (nCost > nStoredCost || nStoredLimit >= nLimit) {
            return 0;
        }
    }
    if (++m_nGridDFSNodes >= 0x40) {
        m_bGridDFSAbort = 1;
    }
    int* pSuccessors;
    int nCount = GridDFSGenerateSuccessors(nCost, nLimit, nX, nY, nZ, &pSuccessors);
    int bFound = 0;
    for (int i = 0; i < nCount; i++) {
        if (bFound || m_bGridDFSAbort) {
            break;
        }
        int* pEntry = &pSuccessors[i * 4];
        bFound = GridDFSearch(nNextDepth, nCost + pEntry[0], nLimit - pEntry[0], pEntry[1],
                              pEntry[2], pEntry[3]);
    }
    delete pSuccessors;
    GridDFSTransTablePut(nX, nY, nCost, nLimit);
    return bFound;
}
