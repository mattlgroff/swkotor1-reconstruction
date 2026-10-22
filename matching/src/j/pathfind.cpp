// CPathfindInformation resets / ctor and CAvoidCreature path
// helpers (0x005D00A0-0x005D0F40).
#include "j/pathfind.h"

// FUNCTION: SWKOTOR 0x005D00A0
void CPathfindInformation::ResetInterTileSearchData()
{
    if (m_nC4) {
        delete[] m_nC4;
    }
    if (m_nC8) {
        delete[] m_nC8;
    }
    if (m_nD0) {
        delete[] m_nD0;
    }
    if (m_nD4) {
        delete[] m_nD4;
    }
    m_nC0 = 0;
    m_nC4 = 0;
    m_nC8 = 0;
    m_nCC = 0;
    m_nD0 = 0;
    m_nD4 = 0;
    if (m_nF0) {
        DeleteFirstTileFValueAlternatives();
    }
    m_nD8 = 0;
    m_nDC = 0;
    m_nE0 = 0;
    if (m_nF4) {
        delete[] m_nF4;
    }
    m_nF4 = 0;
}

// FUNCTION: SWKOTOR 0x005D0690
Vector* CAvoidCreature::FindPath(int* pnPoints)
{
    if (m_n50 == m_n64) {
        Vector* pvPath = new Vector[2];
        pvPath[0] = m_vStart;
        pvPath[1] = m_vEnd;
        *pnPoints = 2;
        return pvPath;
    }
    if (m_bLeft) {
        return FindPath_Left(pnPoints);
    }
    return FindPath_Right(pnPoints);
}

// FUNCTION: SWKOTOR 0x005D0770
int CAvoidCreature::IsPathClear(int nParam, Vector* pvPoints, int nPoints, float* pfDistance)
{
    *pfDistance = 1.7014118e38f;
    Vector vPrev = pvPoints[0];
    Vector vCur;
    for (int i = 1; i < nPoints; i++) {
        vCur = pvPoints[i];
        if (!IsSegmentClear(nParam, &vPrev, &vCur, pfDistance)) {
            return 0;
        }
        vPrev = vCur;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D0CE0
CPathfindInformation::CPathfindInformation()
{
    m_n108 = 0;
    m_n10C = 0;
    m_n110 = 0;
    m_n114 = 0;
    m_n118 = 0;
    m_n11C = 0;
    m_n12C = 0;
    m_n130 = 0;
    m_n134 = 0;
    m_n138 = 0;
    m_n13C = 0;
    m_n140 = 0;
    m_n144 = 0;
    m_n148 = 0;
    m_n14C = 0;
    m_n150 = 0;
    m_n154 = 0;
    m_n158 = 0;
    m_n170 = 0;
    m_n174 = 0;
    m_n178 = 0;
    m_n17C = 0;
    m_n180 = 0;
    m_n184 = 0;
    m_n1B8 = 0;
    m_n1BC = 0;
    m_n1C0 = 0;
    m_n1C4 = 0;
    m_n1C8 = 0;
    m_n1CC = 0;
    m_n1E4 = 0;
    m_n1E8 = 0;
    m_n1EC = 0;
    m_n1F0 = 0;
    m_n1F4 = 0;
    m_n1F8 = 0;
    m_n248 = 0;
    m_n24C = 0;
    m_n250 = 0;
    m_n258 = 0;
    m_n25C = 0;
    m_n260 = 0;
    m_n18C = 0;
    m_n34 = 0x7f000000;
    Initialize();
}

// FUNCTION: SWKOTOR 0x005D0E00
void CPathfindInformation::ResetInterAreaPathSearchData()
{
    if (m_nAC) {
        delete[] m_nAC;
    }
    if (m_nB0) {
        delete[] m_nB0;
    }
    m_nA0 = 0;
    m_nA4 = 0;
    m_nA8 = 0;
    m_nAC = 0;
    m_nB0 = 0;
    ResetGridSearchData();
    ResetWayPointData();
    ResetInterTileSearchData();
    ResetPathData();
}

// FUNCTION: SWKOTOR 0x005D0E70
void CPathfindInformation::ResetInterAreaSearchData()
{
    if (m_nB8) {
        delete[] m_nB8;
    }
    m_n84 = 0;
    m_n1C = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_nB4 = 0;
    m_nB8 = 0;
    m_n88 = 1;
    ResetInterAreaPathSearchData();
}

// FUNCTION: SWKOTOR 0x005D0EC0
void CPathfindInformation::ResetAtEndOfPath()
{
    m_n194 = 0;
    m_n40 = 0;
    m_n44 = 0;
    m_n48 = 0;
    m_n4C = 0;
    m_n54 = 0;
    m_n58 = 0;
    m_n5C = 0;
    m_n60 = 0;
    m_n64 = 0;
    m_n68 = 0;
    m_n70 = 0;
    m_n74 = 0;
    m_n78 = 0;
    m_n7C = 0;
    m_n80 = 0;
    ResetInterAreaSearchData();
}
