// Gob segment hit tests (vtable 0x741268 +0x88 / +0x8c): the segment is
// moved into the gob's frame, culled by the bounding sphere and handed to
// the part tree. Behavioural reference:
// src/units/0723/10e9440d73-m46_gob_hit.cpp.
#include "g/gob.h"

class CHitInfo {
public:
    char pad00[0x1c];
    Vector m_vHit;                      // +0x1c
    char pad28[0x40 - 0x28];
    Vector m_vNormal;                   // +0x40
    char pad4c[0x5d - 0x4c];
    unsigned char m_bFadeTest;          // +0x5d
    unsigned char m_bSkipNoClip;        // +0x5e
};

extern CHitInfo g_DefaultHitInfo;       // 0x007b93a8
extern float g_fHitMinAlpha;            // 0x0078e858

Quaternion operator-(const Quaternion& q);                     // 0x004a9b50
Vector operator*(const Quaternion& q, const Vector& v);        // 0x004a9bb0
Vector operator+(const Vector& a, const Vector& b);            // 0x00423b90
int SphereIntersect(Vector* pStart, Vector* pEnd, Vector* pCenter, float fRadius);  // 0x004abdd0

static inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// NONMATCH: the original materialises the bit-8 test with `mov cl, ah;
// and cl, 1` and a dead byte store into the pInfo argument slot (same quirk
// as Scene::HitCheckGob); tried a local flag and a bool getter.
// FUNCTION: SWKOTOR 0x0043e930
Gob* Gob::HitCheckPart(Part* pPart, Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo)
{
    if (!pInfo)
        pInfo = &g_DefaultHitInfo;
    if (!m_pModel)
        return 0;
    if (m_pModel->GetNoHitCheck())
        return 0;
    if (pInfo->m_bSkipNoClip && m_bNoClip)
        return 0;
    if (pInfo->m_bFadeTest) {
        if (m_pModel->GetUseAlphaDC()) {
            if (m_fAlphaDC <= g_fHitMinAlpha)
                return 0;
        } else {
            if (m_fAlphaD8 <= g_fHitMinAlpha)
                return 0;
        }
    }
    int bHit = 0;
    vStart = -m_qOrientation * (vStart - m_vPosition);
    vEnd = -m_qOrientation * (vEnd - m_vPosition);
    if (!SphereIntersect(&vStart, &vEnd, &m_vCenter, m_fRadius))
        return 0;
    if (pPart && pPart->Intersect(vStart, vEnd, &vEnd, pInfo, 1.0f)) {
        bHit = 1;
        vEnd = m_vPosition + m_qOrientation * vEnd;
        pInfo->m_vHit = vEnd;
        pInfo->m_vNormal = m_qOrientation * pInfo->m_vNormal;
        if (pHit)
            *pHit = vEnd;
    }
    return bHit ? this : 0;
}

// FUNCTION: SWKOTOR 0x0043eb80
Gob* Gob::IntersectLine(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo)
{
    if (!pInfo)
        pInfo = &g_DefaultHitInfo;
    if (!m_pModel)
        return 0;
    if (m_pModel->GetNoHitCheck())
        return 0;
    if (pInfo->m_bSkipNoClip && m_bNoClip)
        return 0;
    if (pInfo->m_bFadeTest) {
        if (m_pModel->GetUseAlphaDC()) {
            if (m_fAlphaDC <= g_fHitMinAlpha)
                return 0;
        } else {
            if (m_fAlphaD8 <= g_fHitMinAlpha)
                return 0;
        }
    }
    int bHit = 0;
    vStart = -m_qOrientation * (vStart - m_vPosition);
    vEnd = -m_qOrientation * (vEnd - m_vPosition);
    if (!SphereIntersect(&vStart, &vEnd, &m_vCenter, m_fRadius))
        return 0;
    if (m_pRoot && m_pRoot->Intersect(vStart, vEnd, &vEnd, pInfo, m_fScale)) {
        bHit = 1;
        vEnd = m_vPosition + m_qOrientation * vEnd;
        pInfo->m_vHit = vEnd;
        pInfo->m_vNormal = m_qOrientation * pInfo->m_vNormal;
        if (pHit)
            *pHit = vEnd;
    }
    return bHit ? this : 0;
}
