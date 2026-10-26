// Part world/local transforms: walk the parent chain (+0x3c) and finish
// with the owning Gob (+0x40). Behavioural reference:
// src/units/0732/4526b4e7fa-f1_part_core.cpp.
#include "g/gob.h"

Quaternion operator-(const Quaternion& q);                       // 0x004a9b50
Quaternion operator*(const Quaternion& a, const Quaternion& b);  // 0x004a9ac0
Vector operator*(const Quaternion& q, const Vector& v);          // 0x004a9bb0

static inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// FUNCTION: SWKOTOR 0x0043e4a0
void Part::GetWorldTransform(Vector* pPos, Quaternion* pOrient)
{
    Part* p = this;
    *pPos = Vector(0.0f, 0.0f, 0.0f);
    *pOrient = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    Gob* pGob = m_pGob;
    if (!pGob) {
        *pPos = m_vPosition;
        *pOrient = m_qOrientation;
        return;
    }
    do {
        *pPos *= p->m_fScale;
        *pPos = p->m_qOrientation * *pPos;
        *pPos = p->m_vPosition + *pPos;
        *pOrient = p->m_qOrientation * *pOrient;
        p = p->m_pParent;
    } while (p);
    if (pGob->m_fScale != 1.0f)
        *pPos *= pGob->m_fScale;
    *pPos = pGob->m_qOrientation * *pPos;
    *pPos = pGob->m_vPosition + *pPos;
    *pOrient = pGob->m_qOrientation * *pOrient;
}

// FUNCTION: SWKOTOR 0x0043e6a0
void Part::GetLocalTransform(Vector* pPos, Quaternion* pOrient)
{
    float fScale = 1.0f;
    if (m_pParent) {
        m_pParent->GetLocalTransform(pPos, pOrient);
        fScale = m_pParent->m_fScale;
    } else if (m_pGob) {
        *pOrient = -m_pGob->m_qOrientation * *pOrient;
        *pPos = -m_pGob->m_qOrientation * Vector(pPos->x - m_pGob->m_vPosition.x,
                                                 pPos->y - m_pGob->m_vPosition.y,
                                                 pPos->z - m_pGob->m_vPosition.z);
        fScale = m_pGob->m_fScale;
    }
    *pOrient = -m_qOrientation * *pOrient;
    *pPos = -m_qOrientation * (*pPos - m_vPosition);
    *pPos *= 1.0f / fScale;
}
