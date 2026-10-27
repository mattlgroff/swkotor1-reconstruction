// CPathfindInformation resets / buffer frees and CAvoidCreature path
// builders (0x005CEB70-0x005CFDA0).

#include <string.h>

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator-() const { return Vector(-x, -y, -z); }
    float x, y, z;
};

Vector CrossProduct(const Vector &a, const Vector &b);      // 0x004AB210
Vector Normalize(const Vector &v);                          // 0x004AB180

class CPathfindInformation {
public:
    void DeleteFirstTileFValueAlternatives();
    void FUN_005cec10();
    void FUN_005cec50();
    void FUN_005cec90();
    void Shutdown();
    void ResetGridSearchData();
    void ResetWayPointData();
    void FUN_005cf130();
    void Initialize();

    int m_n00;                       // +0x00
    float m_fStep;                   // +0x04
    float m_f08;                     // +0x08
    int m_n0C;                       // +0x0c
    float m_f10;                     // +0x10
    float m_f14;                     // +0x14
    char pad_0x18[0x4];
    int m_n1C;                       // +0x1c
    int m_n20;                       // +0x20
    int m_n24;                       // +0x24
    int m_n28;                       // +0x28
    unsigned long m_n2C;             // +0x2c
    unsigned long m_n30;             // +0x30
    char pad_0x34[0x4];
    unsigned long m_n38;             // +0x38
    char pad_0x3c[0x4];
    int m_n40;                       // +0x40
    float m_f44;                     // +0x44
    float m_f48;                     // +0x48
    float m_f4C;                     // +0x4c
    float m_f50;                     // +0x50
    int m_n54;                       // +0x54
    int m_n58;                       // +0x58
    float m_f5C;                     // +0x5c
    float m_f60;                     // +0x60
    float m_f64;                     // +0x64
    float m_f68;                     // +0x68
    int m_n6C;                       // +0x6c
    int m_n70;                       // +0x70
    float m_f74;                     // +0x74
    float m_f78;                     // +0x78
    float m_f7C;                     // +0x7c
    int m_n80;                       // +0x80
    int m_n84;                       // +0x84
    int m_n88;                       // +0x88
    int m_n8C;                       // +0x8c
    float *m_p90;                    // +0x90
    int m_n94;                       // +0x94
    float *m_p98;                    // +0x98
    int m_n9C;                       // +0x9c
    int m_nA0;                       // +0xa0
    int m_nA4;                       // +0xa4
    int m_nA8;                       // +0xa8
    int *m_pAC;                      // +0xac
    int *m_pB0;                      // +0xb0
    int m_nB4;                       // +0xb4
    int *m_pB8;                      // +0xb8
    unsigned long m_nBC;             // +0xbc
    int m_nC0;                       // +0xc0
    int *m_pC4;                      // +0xc4
    int *m_pC8;                      // +0xc8
    int m_nCC;                       // +0xcc
    int *m_pD0;                      // +0xd0
    int *m_pD4;                      // +0xd4
    int m_nD8;                       // +0xd8
    int m_nDC;                       // +0xdc
    int m_nE0;                       // +0xe0
    int m_nE4;                       // +0xe4
    int *m_pE8;                      // +0xe8
    int *m_pEC;                      // +0xec
    int **m_pF0;                     // +0xf0
    int m_nF4;                       // +0xf4
    int m_nF8;                       // +0xf8
    char pad_0xfc[0x4];
    unsigned char m_b100;            // +0x100
    char pad_0x101[0x3];
    int m_n104;                      // +0x104
    Vector m_v108;                   // +0x108
    Vector m_v114;                   // +0x114
    char pad_0x120[0x44];
    float m_f164;                    // +0x164
    int m_n168;                      // +0x168
    int *m_p16C;                     // +0x16c
    Vector m_v170;                   // +0x170
    Vector m_v17C;                   // +0x17c
    int m_n188;                      // +0x188
    int *m_p18C;                     // +0x18c
    int m_n190;                      // +0x190
    int m_n194;                      // +0x194
    int m_n198;                      // +0x198
    char pad_0x19c[0x4];
    unsigned char m_b1A0;            // +0x1a0
    char pad_0x1a1[0x3];
    int m_n1A4;                      // +0x1a4
    int m_n1A8;                      // +0x1a8
    int *m_p1AC;                     // +0x1ac
    int m_n1B0;                      // +0x1b0
    int m_n1B4;                      // +0x1b4
    Vector m_v1B8;                   // +0x1b8
    Vector m_v1C4;                   // +0x1c4
    int m_n1D0;                      // +0x1d0
    float m_f1D4;                    // +0x1d4
    int *m_p1D8;                     // +0x1d8
    int m_n1DC;                      // +0x1dc
    int m_n1E0;                      // +0x1e0
    char pad_0x1e4[0x1c];
    int m_n200;                      // +0x200
    char pad_0x204[0x4];
    int m_n208;                      // +0x208
    float m_f20C;                    // +0x20c
    int m_n210;                      // +0x210
    char pad_0x214[0x4];
    int m_n218;                      // +0x218
    int *m_p21C;                     // +0x21c
    int m_n220;                      // +0x220
    int m_n224;                      // +0x224
    int *m_p228;                     // +0x228
    int *m_p22C;                     // +0x22c
    char pad_0x230[0x8];
    int *m_p238;                     // +0x238
    int m_n23C;                      // +0x23c
    int m_n240;                      // +0x240
    int m_n244;                      // +0x244
    char pad_0x248[0xc];
    unsigned long m_n254;            // +0x254
    Vector m_v258;                   // +0x258
    unsigned long m_n264;            // +0x264
    char pad_0x268[0x4];
    int m_n26C;                      // +0x26c
    int m_n270;                      // +0x270
    int m_n274;                      // +0x274
    char pad_0x278[0x0];
};

// FUNCTION: SWKOTOR 0x005CEC10
void CPathfindInformation::FUN_005cec10()
{
    float f = m_f5C;
    m_f5C = m_f74;
    m_f74 = f;
    f = m_f60;
    m_f60 = m_f78;
    m_f78 = f;
    f = m_f64;
    m_f64 = m_f7C;
    m_f7C = f;
    int n = m_n70;
    m_n70 = m_n80;
    m_n80 = n;
}

// FUNCTION: SWKOTOR 0x005CEC50
void CPathfindInformation::FUN_005cec50()
{
    float f = m_f5C;
    m_f5C = m_f44;
    m_f44 = f;
    f = m_f60;
    m_f60 = m_f48;
    m_f48 = f;
    f = m_f64;
    m_f64 = m_f4C;
    m_f4C = f;
    f = m_f68;
    m_f68 = m_f50;
    m_f50 = f;
    int n = m_n70;
    m_n70 = m_n54;
    m_n54 = n;
}

// FUNCTION: SWKOTOR 0x005CEC90
void CPathfindInformation::FUN_005cec90()
{
    if (m_fStep != 0.0f)
        m_n6C = (int)(m_f68 / m_fStep);
    else
        m_n6C = (int)m_f68;
}

// FUNCTION: SWKOTOR 0x005CEDE0
void CPathfindInformation::Shutdown()
{
    if (m_p90)
        delete[] m_p90;
    if (m_p98)
        delete[] m_p98;
    if (m_pAC)
        delete[] m_pAC;
    if (m_pB0)
        delete[] m_pB0;
    if (m_pB8)
        delete[] m_pB8;
    if (m_pC4)
        delete[] m_pC4;
    if (m_pC8)
        delete[] m_pC8;
    if (m_pD0)
        delete[] m_pD0;
    if (m_pD4)
        delete[] m_pD4;
    if (m_pF0)
        DeleteFirstTileFValueAlternatives();
    if (m_pE8) {
        delete[] m_pE8;
        m_pE8 = 0;
    }
    if (m_pEC) {
        delete[] m_pEC;
        m_pEC = 0;
    }
    if (m_p16C) {
        delete[] m_p16C;
        m_p16C = 0;
    }
    if (m_p238) {
        delete[] m_p238;
        m_p238 = 0;
    }
    if (m_p1AC) {
        delete[] m_p1AC;
        m_p1AC = 0;
    }
    m_p1AC = 0;
    if (m_p21C) {
        m_n218 = 0;
        delete[] m_p21C;
        m_p21C = 0;
    }
    m_p21C = 0;
    if (m_p228) {
        m_n220 = 0;
        delete[] m_p228;
        m_p228 = 0;
    }
    m_p228 = 0;
    if (m_p22C) {
        m_n224 = 0;
        delete[] m_p22C;
        m_p22C = 0;
    }
    m_p22C = 0;
}

// FUNCTION: SWKOTOR 0x005CEB70
void CPathfindInformation::DeleteFirstTileFValueAlternatives()
{
    if (m_pF0) {
        for (int i = 0; i < m_nE4; i++) {
            if (m_pF0[i]) {
                delete[] m_pF0[i];
                m_pF0[i] = 0;
            }
        }
        if (m_pE8) {
            delete[] m_pE8;
            m_pE8 = 0;
        }
        if (m_pEC) {
            delete[] m_pEC;
            m_pEC = 0;
        }
        m_nE4 = 0;
        delete[] m_pF0;
        m_pF0 = 0;
    }
}

// FUNCTION: SWKOTOR 0x005CEFA0
void CPathfindInformation::ResetGridSearchData()
{
    m_v170 = Vector(0.0f, 0.0f, 0.0f);
    m_v17C = Vector(0.0f, 0.0f, 0.0f);
    m_f164 = 700.0f;
    m_n168 = 0;
    m_n188 = 0;
    if (m_p18C)
        delete[] m_p18C;
    m_p18C = 0;
    m_n190 = 700;
    m_f1D4 = 100000.0f;
}

// FUNCTION: SWKOTOR 0x005CFDA0
void CPathfindInformation::Initialize()
{
    m_fStep = 0.6f;
    m_f08 = 0.6f;
    m_f10 = 0.6f;
    m_n00 = 0;
    m_f14 = 0.5f;
    m_n1C = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_n194 = 0;
    m_n2C = 0x7f000000;
    m_n30 = 0x7f000000;
    m_n40 = 0;
    m_f44 = 0;
    m_f48 = 0;
    m_f4C = 0;
    m_n54 = 0;
    m_n58 = 0;
    m_f5C = 0;
    m_f60 = 0;
    m_f64 = 0;
    m_f68 = 0;
    m_n6C = 0;
    m_n70 = 0;
    m_f74 = 0;
    m_f78 = 0;
    m_f7C = 0;
    m_n80 = 0;
    m_n84 = 0;
    m_n88 = 1;
    m_n8C = 0;
    m_p90 = 0;
    m_n94 = 0;
    m_p98 = 0;
    m_n9C = 0;
    m_nA0 = 0;
    m_nA4 = 0;
    m_nA8 = 0;
    m_pAC = 0;
    m_pB0 = 0;
    m_nB4 = 0;
    m_pB8 = 0;
    m_nC0 = 0;
    m_pC4 = 0;
    m_pC8 = 0;
    m_nCC = 0;
    m_pD0 = 0;
    m_pD4 = 0;
    m_nD8 = 0;
    m_nDC = 0;
    m_nE0 = 0;
    m_nE4 = 0;
    m_pF0 = 0;
    m_nF4 = 0;
    m_pE8 = 0;
    m_pEC = 0;
    m_n28 = 0;
    m_nF8 = 0;
    m_b100 = 0;
    m_n104 = 0;
    m_v108 = Vector(0.0f, 0.0f, 0.0f);
    m_v114 = Vector(1.0f, 0.0f, 0.0f);
    m_v170 = Vector(0.0f, 0.0f, 0.0f);
    m_f164 = 700.0f;
    m_n168 = 0;
    m_p16C = 0;
    m_v17C = Vector(0.0f, 0.0f, 0.0f);
    m_n188 = 0;
    if (m_p18C)
        delete[] m_p18C;
    m_n1A4 = -1;
    m_n1A8 = -1;
    m_p18C = 0;
    m_nBC = 0x7f000000;
    m_p1AC = 0;
    m_n1B0 = 0;
    m_n1B4 = 0;
    m_n1D0 = 0;
    m_n1DC = 0;
    m_p1D8 = 0;
    m_n194 = 0;
    m_n198 = 0;
    m_b1A0 = 0;
    m_n210 = 0;
    m_p238 = 0;
    m_n200 = 0;
    m_n208 = 0;
    m_n23C = 0;
    m_n240 = 0;
    m_n244 = 0;
    m_n190 = 700;
    m_f1D4 = 100000.0f;
    m_n1E0 = 1;
    m_n254 = 0x7f000000;
    m_v258 = Vector(0.0f, 0.0f, 0.0f);
    m_n264 = 0x7f000000;
    m_n218 = 0;
    m_p21C = 0;
    m_n220 = 0;
    m_n224 = 0;
    m_p228 = 0;
    m_p22C = 0;
    m_n0C = 0;
    m_n38 = 0x7f000000;
    m_n26C = 0;
    m_n270 = 0;
    m_n274 = 0;
}

// FUNCTION: SWKOTOR 0x005CF060
void CPathfindInformation::ResetWayPointData()
{
    if (m_p90)
        delete[] m_p90;
    if (m_p98)
        delete[] m_p98;
    m_n8C = 0;
    m_p90 = 0;
    m_n94 = 0;
    m_p98 = 0;
    m_n9C = 0;
    m_nF8 = 0;
    m_n104 = 0;
    m_v108 = Vector(0.0f, 0.0f, 0.0f);
    m_v114 = Vector(1.0f, 0.0f, 0.0f);
}

// FUNCTION: SWKOTOR 0x005CF130
void CPathfindInformation::FUN_005cf130()
{
    m_v1B8 = Vector(0.0f, 0.0f, 0.0f);
    m_v1C4 = Vector(0.0f, 0.0f, 0.0f);
    m_n1D0 = 0;
    m_n1DC = 0;
    if (m_p1D8)
        delete[] m_p1D8;
    m_p1D8 = 0;
    m_f20C = 700.0f;
    m_f1D4 = 3.402823466e+38F;
}

// ---------------------------------------------------------------------------
// CAvoidCreature

class CAvoidCreature {
public:
    int InsertPathSegmentBetween(int nStart, int nEnd, Vector *pPoints, int nPoints,
                                 CPathfindInformation *pInfo);
    Vector *FindPath_Left(int *pnPoints);
    Vector *FindPath_Right(int *pnPoints);
    int IsPointInHex(const Vector &vPoint);
    int GetClosestPointTo(const Vector &vPoint);

    char pad_0x0[0x4];
    Vector m_avCorners[6];           // +0x04
    char pad_0x4c[0x4];
    int m_nStartCorner;              // +0x50
    Vector m_vStart;                 // +0x54
    char pad_0x60[0x4];
    int m_nEndCorner;                // +0x64
    Vector m_vEnd;                   // +0x68
};

// FUNCTION: SWKOTOR 0x005CECC0
int CAvoidCreature::InsertPathSegmentBetween(int nStart, int nEnd, Vector *pPoints, int nPoints,
                                             CPathfindInformation *pInfo)
{
    float *pOld = pInfo->m_p90;
    int nOut = 0;
    int nSkip;
    int nSize;
    if (nStart == nEnd) {
        nSize = pInfo->m_n8C + nPoints * 2 - 2;
        nSkip = 1;
    } else {
        nSize = pInfo->m_n8C + (nStart - nEnd + nPoints) * 2;
        nSkip = 0;
    }
    float *pNew = new float[nSize];
    int i;
    for (i = 0; i < nStart; i++) {
        pNew[nOut++] = pOld[i * 2];
        pNew[nOut++] = pOld[i * 2 + 1];
    }
    for (i = 0; i < nPoints; i++) {
        pNew[nOut++] = pPoints[i].x;
        pNew[nOut++] = pPoints[i].y;
    }
    for (i = nEnd + nSkip; i < pInfo->m_n8C / 2; i++) {
        pNew[nOut++] = pOld[i * 2];
        pNew[nOut++] = pOld[i * 2 + 1];
    }
    pInfo->m_p90 = 0;
    pInfo->m_n8C = 0;
    delete[] pOld;
    pInfo->m_p90 = pNew;
    pInfo->m_n8C = nSize;
    return 1;
}

// FUNCTION: SWKOTOR 0x005CF650
Vector *CAvoidCreature::FindPath_Left(int *pnPoints)
{
    Vector avPath[10];
    *pnPoints = 0;
    int nCorner = m_nStartCorner;
    avPath[(*pnPoints)++] = m_vStart;
    avPath[(*pnPoints)++] = m_avCorners[nCorner];
    while (nCorner != (m_nEndCorner + 1) % 6) {
        nCorner = (nCorner - 1) % 6;
        if (nCorner < 0)
            nCorner = 5;
        avPath[(*pnPoints)++] = m_avCorners[nCorner];
    }
    avPath[(*pnPoints)++] = m_vEnd;
    Vector *pPath = new Vector[*pnPoints];
    memcpy(pPath, avPath, *pnPoints * sizeof(Vector));
    return pPath;
}

// FUNCTION: SWKOTOR 0x005CF7A0
Vector *CAvoidCreature::FindPath_Right(int *pnPoints)
{
    Vector avPath[10];
    *pnPoints = 0;
    int nCorner = (m_nStartCorner + 1) % 6;
    avPath[(*pnPoints)++] = m_vStart;
    avPath[(*pnPoints)++] = m_avCorners[nCorner];
    while (nCorner != m_nEndCorner) {
        nCorner = (nCorner + 1) % 6;
        avPath[(*pnPoints)++] = m_avCorners[nCorner];
    }
    avPath[(*pnPoints)++] = m_vEnd;
    Vector *pPath = new Vector[*pnPoints];
    memcpy(pPath, avPath, *pnPoints * sizeof(Vector));
    return pPath;
}

// FUNCTION: SWKOTOR 0x005CFC70
int CAvoidCreature::IsPointInHex(const Vector &vPoint)
{
    for (int i = 0; i < 6; i++) {
        int j = (i + 1) % 6;
        Vector vUp(0.0f, 0.0f, 1.0f);
        Vector vEdge = m_avCorners[j] - m_avCorners[i];
        Vector vNormal = Normalize(-CrossProduct(vUp, vEdge));
        Vector vDir = Normalize(vPoint - m_avCorners[i]);
        if (vNormal.x * vDir.x + vNormal.y * vDir.y + vNormal.z * vDir.z < 0.0f)
            return 0;
    }
    return 1;
}

// NONMATCH (4): x87 stack scheduling of the last unrolled iteration only (the
// original compares against st(4) before popping; cl pops first).
// FUNCTION: SWKOTOR 0x005CFAE0
int CAvoidCreature::GetClosestPointTo(const Vector &vPoint)
{
    float fBest = 3.402823466e+38F;
    int nBest = -1;
    for (int i = 0; i < 6; i++) {
        Vector v = m_avCorners[i] - vPoint;
        float fDist = v.y * v.y + v.x * v.x + v.z * v.z;
        if (fDist < fBest) {
            fBest = fDist;
            nBest = i;
        }
    }
    return nBest;
}
