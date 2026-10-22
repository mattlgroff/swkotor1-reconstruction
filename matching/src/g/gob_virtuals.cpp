// Gob virtuals that forward to the part tree, the bounding volume
// updates and the part transform accessors (vtable 0x741268).
#include "g/gob.h"

static inline Vector operator+(const Vector& a, const Vector& b)
{
    Vector r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

static inline Vector operator*(const Vector& v, float f)
{
    Vector r;
    r.x = f * v.x;
    r.y = f * v.y;
    r.z = f * v.z;
    return r;
}

// FUNCTION: SWKOTOR 0x0043eee0
void Gob::UpdateBoundingBox()
{
    m_vBoxMin = Vector(10000.0f, 10000.0f, 10000.0f);
    m_vBoxMax = Vector(-10000.0f, -10000.0f, -10000.0f);
    m_pRoot->GetBoundingBox(&m_vBoxMin, &m_vBoxMax, m_fScale);
}

// FUNCTION: SWKOTOR 0x0043f010
// NONMATCH (4): the original schedules the next scale load ahead of the
// previous component store (tried member / free operators, operand order,
// a user operator=).
void Gob::UpdateBoundingSphere()
{
    if (!m_pRoot)
        return;
    m_pRoot->GetBoundingSphere(&m_vCenter, &m_fRadius);
    m_vCenter = m_vCenter + m_pRoot->m_vPosition;
    m_vCenter = m_vCenter * m_fScale;
    m_fRadius = m_fRadius * m_fScale * 1.35f;
}

// FUNCTION: SWKOTOR 0x0043f170
Gob* Gob::IntersectSegment(Vector vStart, Vector vEnd, Vector* pHit)
{
    return IntersectLine(vStart, vEnd, pHit, 0);
}

// FUNCTION: SWKOTOR 0x0043f430
int Gob::GetPartWorldTransform(char* szName, Vector* pPos, Quaternion* pOrient)
{
    Part* p = FindPart(szName);
    if (!p)
        return 0;
    Vector v = p->m_vPosition;
    Quaternion q = p->m_qOrientation;
    p->GetWorldTransform(&v, &q);
    if (pPos)
        *pPos = v;
    if (pOrient)
        *pOrient = q;
    return 1;
}

// FUNCTION: SWKOTOR 0x0043f4e0
int Gob::SetPartTransform(char* szName, Vector v, Quaternion q)
{
    Part* p = FindPart(szName);
    if (!p)
        return 0;
    p->m_vPosition = v;
    p->m_qOrientation = q;
    p->m_bDirty = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x0043f540
int Gob::GetPartTransform(char* szName, Vector* pPos, Quaternion* pOrient)
{
    Part* p = FindPart(szName);
    if (!p)
        return 0;
    if (pPos)
        *pPos = p->m_vPosition;
    if (pOrient)
        *pOrient = p->m_qOrientation;
    return 1;
}

// FUNCTION: SWKOTOR 0x004404d0
Vector Gob::GetHeadPosition()
{
    Part* p = ::FindPart(m_pRoot, "cutscenedummy");
    if (p) {
        Vector v(0.0f, 0.0f, 0.0f);
        Quaternion q;
        q.z = 0.0f;
        q.y = 0.0f;
        q.x = 0.0f;
        q.w = 1.0f;
        p->GetWorldTransform(&v, &q);
        return v;
    }
    return Vector(0.0f, 0.0f, 0.0f);
}

// FUNCTION: SWKOTOR 0x004437f0
int Gob::SetCanDownSample(unsigned char b)
{
    if (m_bCanDownSample == b)
        return 0;
    m_bCanDownSample = b;
    return PartSetCanDownSample(m_pRoot, b);
}

// FUNCTION: SWKOTOR 0x00444d90
unsigned char Gob::SetScale(float f, unsigned char bBase)
{
    if (!m_pModel)
        return 0;
    if (bBase == 1) {
        m_fScale = m_fScale / m_fBaseScale;
        m_fBaseScale = f;
        m_fScale = f * m_fScale;
    } else {
        m_fScale = f * m_fBaseScale;
    }
    for (int i = 0; i < m_lChildren.num; i++)
        m_lChildren.element[i]->SetScale(m_fScale, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004477b0
int Gob::EnumParts(int a, int b)
{
    return EnumParts(m_pRoot, a, b);
}

// FUNCTION: SWKOTOR 0x0044f7c0
int Gob::SetSceneAndReport(Scene* pScene)
{
    SetScene(pScene);
    return 1;
}

// FUNCTION: SWKOTOR 0x00449b10
// NONMATCH (31): the original loads the ref count (edx) before the model
// root and the rest follows with rotated scratch registers; ours loads the
// root first and increments in edi (tried x++ / ++x).
int Gob::AttachModel(char* szModel, char* szPart)
{
    Part* pParent = ::FindPart(m_pRoot, szPart);
    if (!pParent)
        return 0;
    Model* pModel = FindModel(szModel);
    if (!pModel)
        return 0;
    ++pModel->m_nRefCount;
    Part* pPart = CreateInstanceTreeR(pModel->m_pRoot, this);
    pPart->Finalize(pPart);
    pPart->m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    pPart->m_qOrientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    m_lAttachedModels.Add(pModel);
    m_lAttachedParts.Add(pPart);
    pParent->m_lChildren.Add(pPart);
    pPart->m_pParent = pParent;
    PartInitMaterial(this, pPart, "");
    m_bDirty = 1;
    PartAttachUpdate(this, pPart);
    DoEvent("AttachModel", 0);
    return 1;
}
