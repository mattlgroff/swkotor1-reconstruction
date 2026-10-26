// GLRender helpers (render pools, client-array setup, small vector
// math), 0x00423a90-0x00427450.
#include "g/glrender.h"

// FUNCTION: SWKOTOR 0x00423a90
void RequestPoolManagerSys::ResizePool(int nIndex, int nSize)
{
    g_nPoolBytes += nSize - m_pSizes[nIndex];
    g_nPoolPages += ((nSize + 0xfff) & ~0xfff) - ((m_pSizes[nIndex] + 0xfff) & ~0xfff);
    void* pOld = m_ppPools[nIndex];
    void* pNew = operator new(nSize);
    memcpy(pNew, pOld, m_pSizes[nIndex]);
    free(pOld);
    m_pSizes[nIndex] = nSize;
    m_ppPools[nIndex] = pNew;
}

// FUNCTION: SWKOTOR 0x00423b40
int RequestPoolManagerSys::GetPoolSize(int nIndex)
{
    return m_pSizes[nIndex];
}

// FUNCTION: SWKOTOR 0x00423b50
int RequestPoolManagerSys::GetBuffer(int nIndex)
{
    if (nIndex == -1)
        return 0;
    return (int)m_ppPools[nIndex];
}

// FUNCTION: SWKOTOR 0x00423b70
float AbsF(float f)
{
    return f < 0.0f ? -f : f;
}

// FUNCTION: SWKOTOR 0x00423b90
Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// FUNCTION: SWKOTOR 0x00423bc0
Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// FUNCTION: SWKOTOR 0x00423bf0 SYMBOL: ??D@YA?AVVector@@ABV0@M@Z
Vector operator*(const Vector& v, float s)
{
    return Vector(v.x * s, v.y * s, v.z * s);
}

// FUNCTION: SWKOTOR 0x00423c20 SYMBOL: ??D@YAMABVVector@@0@Z
float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// FUNCTION: SWKOTOR 0x00423c40
Vector Quaternion::axis() const
{
    Vector v(x, y, z);
    double fAngle = 2.0 * acos(w);
    if (AbsF(fAngle) < 1e-7)
        return Vector(1.0f, 0.0f, 0.0f);
    return v * (float)(1.0 / sin(fAngle * 0.5));
}

// FUNCTION: SWKOTOR 0x00423cd0
void DisableCombiners()
{
    glDisable(0x8522);
    g_nCurrentStage = 1;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glDisable(0x8513);
    glDisable(0xc60);
    glDisable(0xc61);
    glDisable(0xc62);
    glDisable(0xde1);
    g_n827fa0 = 0;
    g_aCurrentStageTID[g_nCurrentStage] = 0;
    g_n827fa4 = 0;
    g_nCurrentStage = 0;
    if (HasMultiTexture())
        glActiveTextureARB(g_nCurrentStage + GL_TEXTURE0_ARB);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
}

// FUNCTION: SWKOTOR 0x00423ee0
void PoolRequest(int nType, int nSize, int nFlags, int nUnused, int* pId)
{
    *pId = g_pRequestPoolManager->Request(nType, nSize, nFlags);
}

// FUNCTION: SWKOTOR 0x00423f10
void* PoolLock(int nId, int nUnused1, int nUnused2, int nFlags)
{
    return g_pRequestPoolManager->Lock(nId, nFlags);
}

// FUNCTION: SWKOTOR 0x00423f30
void PoolUnlock(int nId)
{
    g_pRequestPoolManager->Unlock(nId);
}

// FUNCTION: SWKOTOR 0x00423f50
void PoolRelease(int nId)
{
    if (nId != -1)
        g_pRequestPoolManager->Release(nId);
}

// FUNCTION: SWKOTOR 0x00423f70
void PoolResize(int nId, int nSize)
{
    g_pRequestPoolManager->ResizePool(nId, nSize);
}

// FUNCTION: SWKOTOR 0x00424d70
int GetCompressedContactSheetOffset(int nCount, int nSize, int nFormat)
{
    int nBlock = (nFormat == 4) ? 16 : 8;
    int nBlocks = (nSize + 3) / 4;
    int nTotal = nBlocks * nBlocks * nBlock;
    for (int nDim = nSize; nDim > 1;) {
        nDim >>= 1;
        nBlocks = (nDim + 3) / 4;
        nTotal += nBlocks * nBlocks * nBlock;
    }
    return nTotal * nCount;
}

// FUNCTION: SWKOTOR 0x00425250
void ActivateTextureStage(int nStage)
{
    if (HasMultiTexture()) {
        glClientActiveTextureARB(nStage + GL_TEXTURE0_ARB);
        g_nCurrentStage = nStage;
    } else {
        g_nCurrentStage = nStage;
    }
}

// NONMATCH (18): the original pops edi/esi before the final stage store and
// pops ebx after it; cl here pops all three before the tail test.
// FUNCTION: SWKOTOR 0x00425280
void SetLightmapTexCoordToStage0(int a, int b, int nId, int nCount, MdlNodeTriMesh* pNode,
                                 int nOffset)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        char* pBase = (char*)nOffset;
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB);
        g_nCurrentStage = 0;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset1);
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB + 1);
        g_nCurrentStage = 1;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset);
    } else {
        char* pBase = (char*)g_pRequestPoolManager->GetBuffer(nId) + nOffset;
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB);
        g_nCurrentStage = 0;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset1);
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB + 1);
        g_nCurrentStage = 1;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset);
    }
    ActivateTextureStage(0);
}

// NONMATCH (18): same epilogue scheduling as 0x00425280.
// FUNCTION: SWKOTOR 0x004253d0
void SetDecalTexCoordToStage0(int a, int b, int nId, int nCount, MdlNodeTriMesh* pNode,
                              int nOffset)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        char* pBase = (char*)nOffset;
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB);
        g_nCurrentStage = 0;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset);
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB + 1);
        g_nCurrentStage = 1;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset1);
    } else {
        char* pBase = (char*)g_pRequestPoolManager->GetBuffer(nId) + nOffset;
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB);
        g_nCurrentStage = 0;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset);
        if (HasMultiTexture())
            glClientActiveTextureARB(GL_TEXTURE0_ARB + 1);
        g_nCurrentStage = 1;
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset1);
    }
    ActivateTextureStage(0);
}

// FUNCTION: SWKOTOR 0x00425900
void SetVertexBuffer(int nStride, char* pOffset, int nId)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        glVertexPointer(3, GL_FLOAT, nStride, pOffset);
    } else {
        char* pBase = (char*)g_pRequestPoolManager->GetBuffer(nId);
        glVertexPointer(3, GL_FLOAT, nStride, pBase + (int)pOffset);
    }
}

// FUNCTION: SWKOTOR 0x00425970
void SetColorBuffer(int nStride, char* pOffset, int nId)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        glColorPointer(4, GL_UNSIGNED_BYTE, nStride, pOffset);
    } else {
        char* pBase = (char*)g_pRequestPoolManager->GetBuffer(nId);
        glColorPointer(4, GL_UNSIGNED_BYTE, nStride, pBase + (int)pOffset);
    }
}

// FUNCTION: SWKOTOR 0x004259e0
void SetNormalBuffer(int nStride, char* pOffset, int nId)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        glNormalPointer(GL_FLOAT, nStride, pOffset);
    } else {
        char* pBase = (char*)g_pRequestPoolManager->GetBuffer(nId);
        glNormalPointer(GL_FLOAT, nStride, pBase + (int)pOffset);
    }
}

// FUNCTION: SWKOTOR 0x00425a40
void SetTexCoordBuffer(int nStride, char* pOffset, int nId)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        glTexCoordPointer(2, GL_FLOAT, nStride, pOffset);
    } else {
        char* pBase = (char*)g_pRequestPoolManager->GetBuffer(nId);
        glTexCoordPointer(2, GL_FLOAT, nStride, pBase + (int)pOffset);
    }
}

// FUNCTION: SWKOTOR 0x00425ab0
void DrawElements(int nMode, int nCount, int nFirst, int nId)
{
    glDrawElements(g_aPrimitiveModes[nMode], nCount, GL_UNSIGNED_SHORT,
                   (unsigned short*)g_pRequestPoolManager->GetIndices(nId) + nFirst);
}

// NONMATCH (13): the original keeps afAttribs in the frame (0x30 bytes) and
// reuses only the nIndex / nVertices argument slots (nTexture, hPbuffer);
// MSVC here moves afAttribs into a dead argument slot (frame 0x2c) whatever
// the declaration order, array form or initialiser.
// FUNCTION: SWKOTOR 0x00425af0
int ImposterManager::AllocateImposter(int nIndex, int nVertices, const void* pVertices)
{
    void* hOldRC = g_hGLContext;
    void* hOldDC = g_hGLDC;
    Imposter* pImposter = &m_pImposters[nIndex];
    pImposter->m_nVertices = nVertices;
    pImposter->m_bUsed = 1;
    pImposter->m_pVertices = operator new(nVertices * 12);
    memcpy(pImposter->m_pVertices, pVertices, nVertices * 12);
    unsigned int nTexture;
    void* hPbuffer;
    glGenTextures(1, &nTexture);
    pImposter->m_nTexture = nTexture;
    glBindTexture(0xde1, nTexture);
    g_aCurrentStageTID[g_nCurrentStage] = 0;
    int aiAttribs[5];
    float afAttribs[1];
    int nFormat;
    unsigned int nFormats;
    int aiBufferAttribs[3];
    aiAttribs[0] = 0x202d;
    aiAttribs[1] = 1;
    aiAttribs[2] = 0x2010;
    aiAttribs[3] = 1;
    aiAttribs[4] = 0;
    afAttribs[0] = 0;
    wglChoosePixelFormatARB(hOldDC, aiAttribs, afAttribs, 1, &nFormat, &nFormats);
    aiBufferAttribs[0] = 0x2033;
    aiBufferAttribs[1] = 1;
    aiBufferAttribs[2] = 0;
    hPbuffer = wglCreatePbufferARB(hOldDC, nFormat, 256, 256, aiBufferAttribs);
    void* hDC = wglGetPbufferDCARB(hPbuffer);
    void* hRC = wglCreateContext(hDC);
    wglShareLists(hOldRC, hRC);
    wglMakeCurrent(hDC, hRC);
    pImposter->m_hRC = hRC;
    pImposter->m_hDC = hDC;
    pImposter->m_hPbuffer = hPbuffer;
    return wglMakeCurrent(hOldDC, hOldRC);
}

// FUNCTION: SWKOTOR 0x00425c30
void InitRenderSystem()
{
    g_hGLContext = wglGetCurrentContext();
    g_hGLDC = wglGetCurrentDC();
    memset(g_aCurrentStageTID, 0, sizeof(g_aCurrentStageTID));
    g_nCurrentStage = 0;
}

// FUNCTION: SWKOTOR 0x00425c60
int AllocateZeroPair()
{
    int nId = g_pRequestPoolManager->Request(0, 8, 2);
    int* p = (int*)g_pRequestPoolManager->Lock(nId, 1);
    RequestPoolManager* pManager = g_pRequestPoolManager;
    p[0] = 0;
    p[1] = 0;
    pManager->Unlock(nId);
    return nId;
}

// FUNCTION: SWKOTOR 0x00425ca0
void SetViewTransform(const Vector* pPosition, const Quaternion* pOrientation)
{
    glMatrixMode(0x1700);
    glLoadIdentity();
    glMatrixMode(0x1700);
    if (g_bUseViewMatrix) {
        Matrix m = MakeMatrix(Vector(0.0f, 0.0f, 0.0f), -*pOrientation);
        glMultMatrixf(m.m);
    } else {
        Vector v = pOrientation->axis();
        glRotatef((float)(2.0 * acos(pOrientation->w)) * 57.29578f, v.x, v.y, v.z);
    }
    glTranslatef(-pPosition->x, -pPosition->y, -pPosition->z);
}

struct SquareVertex {
    float v[3];
    unsigned char c[4];
};

// FUNCTION: SWKOTOR 0x00426000
void RenderSquares(int nId)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        glVertexPointer(3, GL_FLOAT, 0x10, 0);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0x10, (void*)0xc);
        glDrawArrays(2, 0, 4);
    } else {
        SquareVertex* p = (SquareVertex*)g_pRequestPoolManager->GetBuffer(nId);
        glBegin(2);
        glColor4ubv(p->c);
        int i = 4;
        do {
            glVertex3fv(p->v);
            p++;
        } while (--i);
        glEnd();
    }
}

// NONMATCH (24): the VBO path matches; in the client-memory path cl swaps
// ebx/ebp (nQuads vs. the cached glTexCoordPointer import).
// FUNCTION: SWKOTOR 0x00426230
void DrawLightmappedGrass(char* pVertices, int nStride, char* pUnused, int nAttribStride,
                          int nFirst, int nQuads)
{
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glVertexPointer(3, GL_FLOAT, nStride, pVertices);
        pVertices += nQuads * 0x30;
        glNormalPointer(GL_FLOAT, nAttribStride, pVertices);
        glTexCoordPointer(2, GL_FLOAT, nAttribStride, pVertices + 0x0c);
        ActivateTextureStage(1);
        glTexCoordPointer(2, GL_FLOAT, nAttribStride, pVertices + 0x14);
        ActivateTextureStage(0);
        glDrawArrays(GL_QUADS, nFirst, nQuads * 4);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        return;
    }
    glVertexPointer(3, GL_FLOAT, nStride, pVertices);
    pVertices += nQuads * 0x30;
    glNormalPointer(GL_FLOAT, nAttribStride, pVertices);
    glTexCoordPointer(2, GL_FLOAT, nAttribStride, pVertices + 0x0c);
    ActivateTextureStage(1);
    glTexCoordPointer(2, GL_FLOAT, nAttribStride, pVertices + 0x14);
    ActivateTextureStage(0);
    glDrawArrays(GL_QUADS, nFirst, nQuads * 4);
}

// FUNCTION: SWKOTOR 0x00426560
void* CreatePBuffer(int nWidth, int nHeight)
{
    void* hDC = g_hGLDC;
    int aiAttribs[17];
    float afAttribs[1];
    int nFormat;
    unsigned int nFormats;
    int aiBufferAttribs[6];
    aiAttribs[0] = 0x2010;
    aiAttribs[1] = 1;
    aiAttribs[2] = 0x202d;
    aiAttribs[3] = 1;
    aiAttribs[4] = 0x2071;
    aiAttribs[5] = 1;
    aiAttribs[6] = 0x2015;
    aiAttribs[7] = 8;
    aiAttribs[8] = 0x2017;
    aiAttribs[9] = 8;
    aiAttribs[10] = 0x2019;
    aiAttribs[11] = 8;
    aiAttribs[12] = 0x201b;
    aiAttribs[13] = 8;
    aiAttribs[14] = 0x2011;
    aiAttribs[15] = 0;
    aiAttribs[16] = 0;
    afAttribs[0] = 0;
    wglChoosePixelFormatARB(hDC, aiAttribs, afAttribs, 1, &nFormat, &nFormats);
    aiBufferAttribs[0] = 0x2072;
    aiBufferAttribs[1] = 0x2076;
    aiBufferAttribs[2] = 0x2073;
    aiBufferAttribs[3] = 0x207a;
    aiBufferAttribs[4] = 0;
    aiBufferAttribs[5] = 0;
    return wglCreatePbufferARB(hDC, nFormat, nWidth, nHeight, aiBufferAttribs);
}

// FUNCTION: SWKOTOR 0x00426550
void SetCurrentStageTID(int nStage, int nTID)
{
    g_aCurrentStageTID[nStage] = nTID;
}

// FUNCTION: SWKOTOR 0x00426650
void SetFilmNoiseTexture(int nTexture)
{
    g_nFilmNoiseTexture = nTexture;
}

// FUNCTION: SWKOTOR 0x004273e0
int HighestBit(unsigned int x)
{
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x >>= 1;
    x = x - ((x >> 1) & 0x55555555);
    x = ((x >> 2) & 0x33333333) + (x & 0x33333333);
    x = ((x >> 4) + x) & 0x0f0f0f0f;
    x += x >> 8;
    x += x >> 16;
    return x & 0x3f;
}

// FUNCTION: SWKOTOR 0x00427450
void FindSmallestPowerOf2WidthHeight(int nWidth, int nHeight, int* pWidth, int* pHeight)
{
    *pWidth = 1 << (HighestBit(nWidth) + 1);
    *pHeight = 1 << (HighestBit(nHeight) + 1);
}

// FUNCTION: SWKOTOR 0x00432090
// Takes the first free imposter, doubling the table when all are used.
int ImposterManager::SetUp(int nVertices, const void* pVertices)
{
    int nCount = m_nImposters;
    int i;
    for (i = 0; i < m_nImposters; i++) {
        if (!m_pImposters[i].m_bUsed) {
            AllocateImposter(i, nVertices, pVertices);
            return i;
        }
    }
    Imposter* pOld = m_pImposters;
    m_pImposters = new Imposter[nCount * 2];
    memset(m_pImposters, 0, m_nImposters * 2);
    memcpy(m_pImposters, pOld, m_nImposters * sizeof(Imposter));
    delete[] pOld;
    AllocateImposter(i, nVertices, pVertices);
    m_nImposters *= 2;
    return m_nImposters >> 1;
}

// FUNCTION: SWKOTOR 0x00432160
void SetUpImposters(int a, int b)
{
    g_ImposterManager.SetUp(a, (const void*)b);
}
