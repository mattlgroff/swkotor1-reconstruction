// render buffer pools (RequestPoolManager, RequestPoolManagerSys,
// RequestPoolManagerVBO), 0x0042e240-0x00436470.
#include "g/request_pool.h"

// File statics: never address-taken, so MSVC schedules around their stores.
static int s_nVBOBytesRequested;    // 0x007a6984
static int s_nPoolBytesRequested;   // 0x007a6988

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

template class CExoArrayList<int>;

// Only the constructors inline Add's Allocate call; the other Add callers
// keep it out of line (0x00437d60 / 0x005e03c0), hence inline_depth(1)
// around them.

// FUNCTION: SWKOTOR 0x0042e240 SYMBOL: ??1?$CExoArrayList@H@@QAE@XZ

// FUNCTION: SWKOTOR 0x0042e250
RequestPoolManager::~RequestPoolManager()
{
}

// FUNCTION: SWKOTOR 0x0042e280 SYMBOL: ??_GRequestPoolManager@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0042e2a0
void* RequestPoolManagerVBO::Lock(int nId, int nFlags)
{
    if (nId == -1)
        return 0;
    if (m_aLocked.element[nId] != 0)
        return 0;
    glBindBufferARB(m_aTargets.element[nId], m_aNames.element[nId]);
    m_aFlags.element[nId] = nFlags;
    m_aLocked.element[nId] = 1;
    unsigned int nAccess;
    if (nFlags & 8)
        nAccess = GL_READ_ONLY_ARB;
    else if (nFlags & 0x10)
        nAccess = GL_READ_WRITE_ARB;
    else
        nAccess = g_bVBOWriteOnly ? GL_WRITE_ONLY_ARB : GL_READ_WRITE_ARB;
    return glMapBufferARB(m_aTargets.element[nId], nAccess);
}

// FUNCTION: SWKOTOR 0x0042e340
void RequestPoolManagerVBO::Unlock(int nId)
{
    if (nId != -1) {
        if (m_aLocked.element[nId]) {
            m_aLocked.element[nId] = 0;
            glBindBufferARB(m_aTargets.element[nId], m_aNames.element[nId]);
            glUnmapBufferARB(m_aTargets.element[nId]);
        }
    }
}

// FUNCTION: SWKOTOR 0x0042e390
int RequestPoolManagerVBO::GetBuffer(int nId)
{
    if (nId == -1)
        return 0;
    return m_aNames.element[nId];
}

// FUNCTION: SWKOTOR 0x0042e3b0
int RequestPoolManagerVBO::GetIndices(int nId)
{
    if (nId != -1)
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_aNames.element[nId]);
    return 0;
}

// FUNCTION: SWKOTOR 0x0042e3d0
int RequestPoolManagerVBO::GetPoolSize(int nId)
{
    if (nId == -1)
        return 0;
    glBindBufferARB(m_aTargets.element[nId], m_aNames.element[nId]);
    int nSize;
    glGetBufferParameterivARB(m_aTargets.element[nId], GL_BUFFER_SIZE_ARB, &nSize);
    return nSize;
}

// FUNCTION: SWKOTOR 0x0042e420
void RequestPoolManagerVBO::ResizePool(int nId, int nNewSize)
{
    if (nId == -1)
        return;
    glBindBufferARB(m_aTargets.element[nId], m_aNames.element[nId]);
    int nSize;
    glGetBufferParameterivARB(m_aTargets.element[nId], GL_BUFFER_SIZE_ARB, &nSize);
    void* pCopy = operator new(nSize);
    void* pData = glMapBufferARB(m_aTargets.element[nId], GL_READ_ONLY_ARB);
    memcpy(pCopy, pData, nSize);
    glUnmapBufferARB(m_aTargets.element[nId]);
    unsigned int nName = m_aNames.element[nId];
    glDeleteBuffersARB(1, &nName);
    m_aNames.element[nId] = 0;
    glGenBuffersARB(1, &m_aNames.element[nId]);
    glBindBufferARB(m_aTargets.element[nId], m_aNames.element[nId]);
    glBufferDataARB(m_aTargets.element[nId], nNewSize, 0, GL_DYNAMIC_DRAW_ARB);
    glBufferSubDataARB(m_aTargets.element[nId], 0, nSize, pCopy);
    free(pCopy);
}

// FUNCTION: SWKOTOR 0x0042e540
RequestPoolManagerSys::~RequestPoolManagerSys()
{
    for (unsigned int i = 0; i < m_nCount; i++)
        free(m_ppPools[i]);
    free(m_ppPools);
    free(m_pSizes);
}

// FUNCTION: SWKOTOR 0x0042e5b0
void* RequestPoolManagerSys::Lock(int nId, int nFlags)
{
    if (nId == -1)
        return 0;
    if (m_aLocked.element[nId])
        return 0;
    m_aLocked.element[nId] = 1;
    return m_ppPools[nId];
}

// FUNCTION: SWKOTOR 0x0042e5e0
void RequestPoolManagerSys::Unlock(int nId)
{
    if (nId != -1) {
        int* pLocked = &m_aLocked.element[nId];
        if (*pLocked)
            *pLocked = 0;
    }
}

#pragma inline_depth(1)
// FUNCTION: SWKOTOR 0x00434cc0
void RequestPoolManagerVBO::LoseBuffers()
{
    for (int i = 0; i < m_aNames.num; i++) {
        unsigned int nName = m_aNames.element[i];
        if (nName == 0) {
            m_aSaved.Add(0);
            m_aSavedSizes.Add(0);
        } else {
            glBindBufferARB(m_aTargets.element[i], nName);
            int nSize;
            glGetBufferParameterivARB(m_aTargets.element[i], GL_BUFFER_SIZE_ARB, &nSize);
            m_aSavedSizes.Add(nSize);
            m_aSaved.Add(operator new(nSize));
            void* pData = glMapBufferARB(m_aTargets.element[i], GL_READ_ONLY_ARB);
            memcpy(m_aSaved.element[i], pData, m_aSavedSizes.element[i]);
            glUnmapBufferARB(m_aTargets.element[i]);
            glDeleteBuffersARB(1, &m_aNames.element[i]);
            m_aNames.element[i] = 0;
        }
    }
}
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x00434e70
void RequestPoolManagerVBO::RestoreBuffers()
{
    for (int i = 0; i < m_aSaved.num; i++) {
        if (m_aSaved.element[i]) {
            unsigned int nName;
            glGenBuffersARB(1, &nName);
            m_aNames.element[i] = nName;
            glBindBufferARB(m_aTargets.element[i], nName);
            glBufferDataARB(m_aTargets.element[i], m_aSavedSizes.element[i],
                            m_aSaved.element[i], GL_DYNAMIC_DRAW_ARB);
            free(m_aSaved.element[i]);
        }
    }
    m_aSaved.num = 0;
    m_aSavedSizes.num = 0;
    m_aSaved.SetSize(0);
    m_aSavedSizes.SetSize(0);
}

// FUNCTION: SWKOTOR 0x004351c0
RequestPoolManagerVBO::~RequestPoolManagerVBO()
{
    for (unsigned int i = 0; i < m_nCount; i++) {
        if (m_aNames.element[i]) {
            glDeleteBuffersARB(1, &m_aNames.element[i]);
            m_aNames.element[i] = 0;
        }
    }
    for (int j = 0; j < m_aSaved.num; j++)
        free(m_aSaved.element[j]);
    m_aSaved.SetSize(0);
    m_aSavedSizes.SetSize(0);
    m_aTargets.SetSize(0);
}

#pragma inline_depth(1)
// FUNCTION: SWKOTOR 0x00435310
int RequestPoolManagerVBO::Request(int nType, int nSize, int nFlags)
{
    if (nSize == 0)
        return -1;
    s_nVBOBytesRequested += nSize;
    if (m_aFree.num) {
        int nId = m_aFree.element[m_aFree.num - 1];
        m_aFree.DelIndex(m_aFree.num - 1);
        m_aLocked.element[nId] = 0;
        m_aFlags.element[nId] = nFlags;
        m_aNames.element[nId] = 0;
        glGenBuffersARB(1, &m_aNames.element[nId]);
        if (!nType) {
            m_aTargets.element[nId] = GL_ARRAY_BUFFER_ARB;
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_aNames.element[nId]);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, nSize, 0, GL_DYNAMIC_DRAW_ARB);
            return nId;
        } else {
            m_aTargets.element[nId] = GL_ELEMENT_ARRAY_BUFFER_ARB;
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_aNames.element[nId]);
            glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nSize, 0, GL_DYNAMIC_DRAW_ARB);
        }
        return nId;
    }
    m_aLocked.Add(0);
    m_aFlags.Add(0);
    m_aNames.Add(0);
    m_aTargets.Add(GL_ARRAY_BUFFER_ARB);
    for (unsigned int i = m_nCount + 1; i < m_nCount * 2; i++) {
        m_aFree.Add(i);
        m_aLocked.Add(0);
        m_aFlags.Add(0);
        m_aNames.Add(0);
        m_aTargets.Add(GL_ARRAY_BUFFER_ARB);
    }
    glGenBuffersARB(1, &m_aNames.element[m_nCount]);
    if (nType == 0) {
        m_aTargets.element[m_nCount] = GL_ARRAY_BUFFER_ARB;
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_aNames.element[m_nCount]);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, nSize, 0, GL_DYNAMIC_DRAW_ARB);
    } else {
        m_aTargets.element[m_nCount] = GL_ELEMENT_ARRAY_BUFFER_ARB;
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_aNames.element[m_nCount]);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nSize, 0, GL_DYNAMIC_DRAW_ARB);
    }
    int nId = m_nCount;
    m_nCount = nId * 2;
    return nId;
}
#pragma inline_depth()

#pragma inline_depth(1)
// FUNCTION: SWKOTOR 0x00435670
void RequestPoolManagerVBO::Release(int nId)
{
    if (nId != -1) {
        m_aFree.Add(nId);
        m_aLocked.element[nId] = 0;
        unsigned int nName = m_aNames.element[nId];
        glDeleteBuffersARB(1, &nName);
        m_aNames.element[nId] = 0;
    }
}
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x00435870 SYMBOL: ??_GRequestPoolManagerSys@@UAEPAXI@Z

#pragma inline_depth(1)
// FUNCTION: SWKOTOR 0x00435890
int RequestPoolManagerSys::Request(int nType, int nSize, int nFlags)
{
    g_nPoolBytes += nSize;
    g_nPoolPages += (nSize + 0xfff) & ~0xfff;
    if (nSize == 0)
        return -1;
    s_nPoolBytesRequested += nSize;
    if (m_aFree.num) {
        void* p = operator new(nSize);
        int nId = m_aFree.element[m_aFree.num - 1];
        m_aFree.DelIndex(m_aFree.num - 1);
        m_ppPools[nId] = p;
        m_pSizes[nId] = nSize;
        return nId;
    }
    m_aLocked.Add(0);
    for (unsigned int i = m_nCount + 1; i < m_nCount * 2; i++) {
        m_aFree.Add(i);
        m_aLocked.Add(0);
    }
    void** ppPools = (void**)operator new(m_nCount * 8);
    int* pSizes = (int*)operator new(m_nCount * 8);
    memset(ppPools, 0, m_nCount * 8);
    memcpy(ppPools, m_ppPools, m_nCount * 4);
    free(m_ppPools);
    memset(pSizes, 0, m_nCount * 8);
    memcpy(pSizes, m_pSizes, m_nCount * 4);
    free(m_pSizes);
    m_ppPools = ppPools;
    m_pSizes = pSizes;
    m_ppPools[m_nCount] = operator new(nSize);
    m_pSizes[m_nCount] = nSize;
    int nId = m_nCount;
    m_nCount = nId * 2;
    return nId;
}
#pragma inline_depth()

#pragma inline_depth(1)
// FUNCTION: SWKOTOR 0x00435a90
void RequestPoolManagerSys::Release(int nId)
{
    if (nId != -1) {
        m_aFree.Add(nId);
        g_nPoolBytes -= m_pSizes[nId];
        g_nPoolPages -= (m_pSizes[nId] + 0xfff) & ~0xfff;
        m_aLocked.element[nId] = 0;
        if (m_ppPools[nId])
            free(m_ppPools[nId]);
        m_ppPools[nId] = 0;
        m_pSizes[nId] = 0;
    }
}
#pragma inline_depth()

extern CExoArrayList<int> g_aRenderSquares;   // 0x007a693c

#pragma inline_depth(1)
// FUNCTION: SWKOTOR 0x00435b40
void AddSquareToRender(int nId)
{
    if (nId != -1)
        g_aRenderSquares.Add(nId);
}
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x00435b90
void RemoveSquareFromRender(int nId)
{
    if (nId != -1) {
        int nFound = 0;
        for (int i = 0; i < g_aRenderSquares.num; i++) {
            if (g_aRenderSquares.element[i] == nId)
                nFound++;
        }
        if (nFound)
            g_aRenderSquares.Remove(nId);
    }
}

// FUNCTION: SWKOTOR 0x00436390
void CreateRequestPoolManagerSys()
{
    if (!g_pRequestPoolManager)
        g_pRequestPoolManager = new RequestPoolManagerSys();
}

// FUNCTION: SWKOTOR 0x00436400
void CreateRequestPoolManagerVBO()
{
    if (!g_pRequestPoolManager)
        g_pRequestPoolManager = new RequestPoolManagerVBO();
}

// FUNCTION: SWKOTOR 0x00436470 SYMBOL: ??_GRequestPoolManagerVBO@@UAEPAXI@Z

// NONMATCH (8, both ctors): the original numbers the EH states with the
// base members separately (base done = state 1, VBO members up to 6); ours
// gives the inlined base one state (0, then 5). An explicit empty inline
// base ctor and a base ctor with a body did not change it.
// FUNCTION: SWKOTOR 0x00434f30
RequestPoolManagerVBO::RequestPoolManagerVBO()
{
    m_nCount = 100;
    for (unsigned int i = 0; i < m_nCount; i++) {
        m_aFree.Add(i);
        m_aFlags.Add(0);
        m_aLocked.Add(0);
        m_aNames.Add(0);
        m_aTargets.Add(GL_ARRAY_BUFFER_ARB);
    }
}

// FUNCTION: SWKOTOR 0x004356e0
RequestPoolManagerSys::RequestPoolManagerSys()
{
    m_nCount = 100;
    m_ppPools = (void**)operator new(400);
    m_pSizes = (int*)operator new(m_nCount * 4);
    memset(m_pSizes, 0, m_nCount * 4);
    memset(m_ppPools, 0, m_nCount * 4);
    for (int i = 0; i < 100; i++) {
        m_aFree.Add(i);
        m_aLocked.Add(0);
    }
}
