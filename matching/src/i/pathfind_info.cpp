// CPathfindInformation buffer setters and CWalkHitInfo::Initialize
// (0x004BB340-0x004BB570).
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004BB340
void CPathfindInformation::FUN_004bb340(float* p)
{
    if (m_p1ac) {
        delete[] m_p1ac;
        m_p1ac = NULL;
    }
    m_p1ac = p;
}

// FUNCTION: SWKOTOR 0x004BB380
void CPathfindInformation::FUN_004bb380(Vector* pA, Vector* pB)
{
    *pA = m_v1e4;
    *pB = m_v1f0;
}

// FUNCTION: SWKOTOR 0x004BB3C0
void CPathfindInformation::FUN_004bb3c0(const Vector* pA, const Vector* pB)
{
    m_v1e4 = *pA;
    m_v1f0 = *pB;
}

// FUNCTION: SWKOTOR 0x004BB400
void CPathfindInformation::FUN_004bb400(int n, float* p)
{
    m_n1fc = n;
    if (m_p200) {
        delete[] m_p200;
    }
    m_p200 = p;
}

// FUNCTION: SWKOTOR 0x004BB440
void CPathfindInformation::FUN_004bb440(int n, float* p)
{
    m_n204 = n;
    if (m_p208) {
        delete[] m_p208;
    }
    m_p208 = p;
}

// FUNCTION: SWKOTOR 0x004BB480
void CPathfindInformation::FUN_004bb480(float* p)
{
    if ((m_p21c != NULL || p == NULL) && m_p21c != NULL && m_p21c != p) {
        m_n218 = 0;
        delete[] m_p21c;
        m_p21c = NULL;
    }
    m_p21c = p;
}

// FUNCTION: SWKOTOR 0x004BB4D0
void CPathfindInformation::FUN_004bb4d0(float* p)
{
    if ((m_p228 != NULL || p == NULL) && m_p228 != NULL && m_p228 != p) {
        m_n220 = 0;
        delete[] m_p228;
        m_p228 = NULL;
    }
    m_p228 = p;
}

// FUNCTION: SWKOTOR 0x004BB520
void CPathfindInformation::FUN_004bb520(float* p)
{
    if ((m_p22c != NULL || p == NULL) && m_p22c != NULL && m_p22c != p) {
        m_n224 = 0;
        delete[] m_p22c;
        m_p22c = NULL;
    }
    m_p22c = p;
}

// FUNCTION: SWKOTOR 0x004BB570
void CWalkHitInfo::Initialize(int nType, const Vector& v1, const Vector& v2, const Vector& v3,
                              const Vector& v4, int nIndex, const Vector& v5)
{
    m_nType = nType;
    m_v04 = v1;
    m_v10 = v2;
    m_v1c = v3;
    m_v28 = v4;
    m_n44 = nIndex;
    m_v34 = v5;
}
