// Part hierarchy destructors and their scalar deleting
// destructors (0x00440140-0x00447b00).
#include "g/part.h"

// FUNCTION: SWKOTOR 0x00440140
Part::~Part()
{
    int i = m_lChildren.num;
    while (i) {
        Part* p = m_lChildren.element[i - 1];
        i--;
        if (p)
            delete p;
        m_lChildren.num = i;
    }
    if (m_pMaterial)
        delete m_pMaterial;
    g_nPartCount--;
}

// FUNCTION: SWKOTOR 0x00440230
PartTriMesh::~PartTriMesh()
{
    if (m_pObject)
        delete m_pObject;
    if (m_bOwnsTexture0 == 1) {
        int nTexture = m_nTexture0;
        m_bOwnsTexture0 = 0;
        AurTextureRelease(nTexture);
        m_nTexture0 = -1;
    }
    if (m_nTexture1 != -1) {
        AurTextureRelease(m_nTexture1);
        m_nTexture1 = -1;
    }
    g_nPartTriMeshCount--;
    if (m_pVertexData) {
        delete m_pVertexData;
        m_pVertexData = 0;
    }
}


// FUNCTION: SWKOTOR 0x00445820 SYMBOL: ??_GPart@@UAEPAXI@Z
void DeletePart(Part* p) { delete p; }
// FUNCTION: SWKOTOR 0x00445a20 SYMBOL: ??_GPartTriMesh@@UAEPAXI@Z
void DeletePartTriMesh(PartTriMesh* p) { delete p; }

// FUNCTION: SWKOTOR 0x00440110
void Part::PropagateToChildren(int nArg)
{
    for (int i = 0; i < m_lChildren.num; i++)
        m_lChildren.element[i]->Propagate6c(nArg);
}

// FUNCTION: SWKOTOR 0x004401e0
CExoArrayList<Part*>* Part::GetChildren()
{
    return &m_lChildren;
}

// FUNCTION: SWKOTOR 0x004401f0
void* Part::GetP40()
{
    return m_p40;
}

// FUNCTION: SWKOTOR 0x00440200
void* Part::GetNodeData()
{
    return m_pNode->m_pModel->AsModel()->m_pNodeData[m_pNode->m_nIndex];
}

// FUNCTION: SWKOTOR 0x004402e0
float PartTriMesh::GetF94()
{
    return m_f94;
}

// FUNCTION: SWKOTOR 0x004402f0
Part* PartTriMesh::FindNamedPart(char* szName)
{
    if (_stricmp("", szName) == 0)
        return m_pRoot;
    return FindPart(m_pRoot, szName);
}

// FUNCTION: SWKOTOR 0x0043ef70
// Not in functions.csv; the base hit test finds nothing.
int Part::HitCheck(const Vector& vStart, const Vector& vEnd, Vector* pHit,
                   CHitInfo* pInfo, float fScale)
{
    return 0;
}

// FUNCTION: SWKOTOR 0x00441510
// Not in functions.csv.
int PartAnimMesh::HitCheck(const Vector& vStart, const Vector& vEnd, Vector* pHit,
                           CHitInfo* pInfo, float fScale)
{
    return HitCheckAnimMesh(this, m_pNode->AsMdlNodeAnimMesh(), vStart, vEnd, pHit, fScale);
}

// FUNCTION: SWKOTOR 0x0043f5c0
// Not in functions.csv.
Vector Part::SetPartPosition(Vector v)
{
    return Vector(0, 0, 0);
}

// Callback for the part tree walkers: does the part carry a material?
// FUNCTION: SWKOTOR 0x0043ee40
int PartHasMaterial(Part* pPart)
{
    return pPart->m_pMaterial != 0;
}
