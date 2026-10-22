// NodeVertex / NodeVertexDangly / NodeVertexAnim and the
// VertexPrimitive Flat / Skinned / Dangly / Anim classes
// (0x00478650-0x004794C0).

#include "h/vertex.h"

// FUNCTION: SWKOTOR 0x00478650
NodeVertexAnim* NodeVertex::AsAnim()
{
    return m_nType == 1 ? (NodeVertexAnim*)this : 0;
}

// FUNCTION: SWKOTOR 0x00478660
NodeVertexDangly* NodeVertex::AsDangly()
{
    return m_nType == 2 ? (NodeVertexDangly*)this : 0;
}

// FUNCTION: SWKOTOR 0x00478670 SYMBOL: ??1VertexPrimitive@@UAE@XZ

// FUNCTION: SWKOTOR 0x00478680
NodeVertex::NodeVertex(Vector* pPosition, Vector* pNormal, Vector* pV18,
                       Vector* pV24, Vector* pV30, Vector* pV3C,
                       Vector* pColor)
{
    m_nType = 0;
    m_nFlags = 0;
    m_vColor = Vector(1.0f, 1.0f, 1.0f);
    m_vPosition = *pPosition;
    m_v3C = *pV3C;
    if (pNormal) {
        m_vNormal = *pNormal;
        m_nFlags = 1;
    }
    if (pV18) {
        m_v18 = *pV18;
        m_nFlags |= 2;
    }
    if (pV24) {
        m_v24 = *pV24;
        m_nFlags |= 4;
    }
    if (pV30) {
        m_v30 = *pV30;
        m_nFlags |= 8;
    }
    if (pColor)
        m_vColor = *pColor;
    g_nNodeVertexBytes += 8;
}

// FUNCTION: SWKOTOR 0x004787E0
NodeVertexDangly::NodeVertexDangly(Vector* pPosition, Vector* pNormal,
                                   Vector* pV18, Vector* pV24, Vector* pV30,
                                   Vector* pV3C, Vector* pColor,
                                   float fDisplacement)
    : NodeVertex(pPosition, pNormal, pV18, pV24, pV30, pV3C, pColor)
{
    m_nType = 2;
    m_fDisplacement = fDisplacement;
}

// FUNCTION: SWKOTOR 0x00478820
bool NodeVertex::operator==(NodeVertex& o)
{
    if (!(o.m_vPosition == m_vPosition) || !(o.m_vNormal == m_vNormal) ||
        !(o.m_v18 == m_v18) || !(o.m_v24 == m_v24) || !(o.m_v30 == m_v30) ||
        !(o.m_v3C == m_v3C) || !(o.m_vColor == m_vColor))
        return false;
    return true;
}

// FUNCTION: SWKOTOR 0x004788C0
bool NodeVertexDangly::operator==(NodeVertexDangly& o)
{
    if (!(o.m_vPosition == m_vPosition) || !(o.m_vNormal == m_vNormal) ||
        !(o.m_v18 == m_v18) || !(o.m_v24 == m_v24) || !(o.m_v30 == m_v30) ||
        !(o.m_v3C == m_v3C) || !(o.m_vColor == m_vColor) ||
        o.m_fDisplacement != m_fDisplacement)
        return false;
    return true;
}

// FUNCTION: SWKOTOR 0x00478970 SYMBOL: ??_GVertexPrimitive@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00478990
VertexPrimitiveFlat::~VertexPrimitiveFlat()
{
    if (m_hBumpProgram != -1) {
        AurVARRelease(m_hBumpProgram);
        m_hBumpProgram = -1;
    }
}

// FUNCTION: SWKOTOR 0x004789F0 SYMBOL: ??_GVertexPrimitiveFlat@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00478A10 SYMBOL: ??1VertexPrimitiveSkinned@@UAE@XZ

// FUNCTION: SWKOTOR 0x00478A20 SYMBOL: ??_GVertexPrimitiveSkinned@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00478A40
VertexPrimitiveDangly::~VertexPrimitiveDangly()
{
    delete m_pVertices;
}

// FUNCTION: SWKOTOR 0x00478AA0 SYMBOL: ??_GVertexPrimitiveDangly@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00478AC0
VertexPrimitiveAnim::~VertexPrimitiveAnim()
{
    int h = m_hNormals;
    AurVARRelease(h);
    m_hNormals = -1;
    h = m_hTexCoords;
    AurVARRelease(h);
    m_hTexCoords = -1;
}

// FUNCTION: SWKOTOR 0x00478B30 SYMBOL: ??_GVertexPrimitiveAnim@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00478B50
void VertexPrimitiveFlat::BuildVertexArrays()
{
    if (!m_pNode->m_n138)
        return;
    PartTriMesh* pSource = m_pOwner->GetSource();
    float* pDst = (float*)AurVARLock(pSource->m_hVertexArray, 0, 0, 1);
    char* pSrc = (char*)AurVARLock(m_pNode->GetVertexBuffer(), 0, 0, 8);
    for (int i = 0; i < m_pNode->m_nNumVertices; i++) {
        float* pUV = (float*)(pSrc + m_pNode->m_nVertexStride * i + m_pNode->m_n194 + m_pNode->m_nUVOffset);
        pDst[i * 2] = pUV[0];
        pDst[i * 2 + 1] = pUV[1];
    }
    AurVARUnlock(pSource->m_hVertexArray);
    int hBuffer = m_pNode->m_pOwner->GetData()->m_hVertexBuffer;
    AurVARUnlock(hBuffer);
}

static inline void VertexAttribPointer(unsigned int nIndex, int nSize,
                                       unsigned int eType, const void* p)
{
    if (g_bARBVertexProgram)
        g_pglVertexAttribPointerARB(nIndex, nSize, eType, 0, 0, p);
    else if (g_bNVVertexProgram && AurVertexProgramsAvailable())
        g_pglVertexAttribPointerNV(nIndex, nSize, eType, 0, p);
}

// FUNCTION: SWKOTOR 0x00478F50
char* VertexPrimitiveSkinned::ReconnectArraysVAR(char* p)
{
    char* pNext = VertexPrimitiveFlat::ReconnectArraysVAR(p);
    if (!AurVertexProgramsAvailable())
        return pNext;
    int nVerts = m_pNode->m_nNumVertices;
    VertexAttribPointer(1, 4, 0x1406, pNext);
    pNext += nVerts * 16;
    VertexAttribPointer(4, 4, 0x1402, pNext);
    return pNext + nVerts * 4;
}

// FUNCTION: SWKOTOR 0x00479020
void VertexPrimitiveFlat::SetupVertexArrays()
{
    VertexPrimitiveAnim* pAnim = AsAnim();
    VertexPrimitiveDangly* pDangly = AsDangly();
    PartTriMesh* pSource = m_pOwner->GetSource();
    MdlNodeTriMesh* pNode = m_pNode;
    if (pSource->m_b7c == 1)
        AurVARBindIndices(4, 0, pSource->m_h74, pNode->m_nNumVertices + 1);
    int n194 = pNode->m_n194;
    int nVerts = pNode->m_nNumVertices + 1;
    int hBuffer = m_pNode->m_pOwner->GetData()->m_hVertexBuffer;
    AurVARBindVertices(m_pNode->m_nVertexStride, m_pNode->m_n150, hBuffer,
                       nVerts, pNode, n194);
    if (pAnim) {
        AurVARBindTexCoords(8, 0, pAnim->m_hTexCoords, pNode->m_nNumVertices);
        AurVARBindNormals(0xc, 0, pAnim->m_hNormals, m_pNode->m_nNumVertices);
    }
}

// FUNCTION: SWKOTOR 0x004790E0
void VertexPrimitive::SetupVertexArraysStandard()
{
    if (!m_pOwner->GetSource()->m_b7c)
        glDisableClientState(g_eTexCoordArray);
    else if (g_bDisableTexCoordArray)
        glDisableClientState(g_eTexCoordArray);
    else
        glEnableClientState(g_eTexCoordArray);
    SetupVertexArrays();
    m_bArraysSetup = 0;
}

// FUNCTION: SWKOTOR 0x00479150 SYMBOL: ??4?$CExoArrayList@VVector@@@@QAEAAV0@ABV0@@Z
template CExoArrayList<Vector>& CExoArrayList<Vector>::operator=(const CExoArrayList<Vector>&);

// FUNCTION: SWKOTOR 0x004791E0
NodeVertexAnim::NodeVertexAnim(Vector* pPosition, Vector* pNormal,
                               Vector* pV18, Vector* pV24, Vector* pV30,
                               Vector* pV3C, Vector* pColor,
                               CExoArrayList<Vector>* pA,
                               CExoArrayList<Vector>* pB,
                               CExoArrayList<Vector>* pC)
    : NodeVertex(pPosition, pNormal, pV18, pV24, pV30, pV3C, pColor)
{
    m_nType = 1;
    m_l58 = *pA;
    m_l64 = *pB;
    m_l70 = *pC;
}

// FUNCTION: SWKOTOR 0x004792A0
VertexPrimitiveFlat::VertexPrimitiveFlat(PartTriMesh* pOwner)
    : VertexPrimitive(pOwner)
{
    m_pNode = pOwner->m_pNode;
    if (m_pNode->m_nNumVertices == 0) {
        m_nNumArrays = 1;
        m_hBumpProgram = -1;
        return;
    }
    if (m_pNode->m_n138) {
        AurVARCreate(0, m_pNode->m_nNumVertices * 8, 2, 0, &pOwner->m_hVertexArray);
        m_nNumArrays += 2;
    }
    VertexPrimitiveFlat::BuildVertexArrays();
    m_nNumArrays += 6;
    m_nNumArrays += m_pNode->m_n182 * 2;
    if (pOwner->m_pMaterial->HasBumpMap() && m_pNode->HasBumpmap() &&
        AurBumpMappingAvailable()) {
        m_hBumpProgram = AurBumpProgramCreate();
        m_nNumArrays += 9;
    } else {
        m_hBumpProgram = -1;
    }
}

// FUNCTION: SWKOTOR 0x004793A0
VertexPrimitiveAnim::VertexPrimitiveAnim(PartTriMesh* pOwner)
    : VertexPrimitiveFlat(pOwner)
{
    AurVARCreate(0, m_pNode->m_nNumVertices * 12, 2, 0, &m_hNormals);
    AurVARCreate(0, m_pNode->m_nNumVertices * 8, 2, 0, &m_hTexCoords);
    m_n10 = 0;
}

// FUNCTION: SWKOTOR 0x00479430
VertexPrimitiveDangly::VertexPrimitiveDangly(PartTriMesh* pOwner)
    : VertexPrimitiveFlat(pOwner)
{
    m_pVertices = new Vector[m_pNode->m_nNumVertices];
    m_n10 = 0;
}

// FUNCTION: SWKOTOR 0x004794C0
VertexPrimitiveSkinned::VertexPrimitiveSkinned(PartTriMesh* pOwner)
    : VertexPrimitiveFlat(pOwner)
{
    m_nNumArrays += 6;
}
