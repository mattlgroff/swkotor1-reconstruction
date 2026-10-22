// MdlNodeAnimMesh controller (0x00485660).
#include "e/mdlanim.h"

class Vector2 {
public:
    float x, y;

    Vector2() {}
    Vector2(float fX, float fY) { x = fX; y = fY; }
    Vector2(const Vector2& v) { x = v.x; y = v.y; }
    Vector2& operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator*(float f) const { return Vector2(x * f, y * f); }
};

// Vertex buffer locks (cdecl, forward through the global render interface).
void* LockBuffer(void* pBuffer, int nOffset, int nSize, int nFlags);   // 0x00423F10
void UnlockBuffer(void* pBuffer);                                       // 0x00423F30

class MeshBuffers {
public:
    char pad_0x0[0x1c];
    void* m_pVertexBuffer;      // +0x1c
    void* m_pUVBuffer;          // +0x20
};

class MeshBufferOwner {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual MeshBuffers* GetBuffers();  // +0x0c
};

class PartTriMesh : public Part {
public:
    char pad_0x44[0x70 - 0x44];
    MeshBufferOwner* m_pBufferOwner;    // +0x70
};

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    float m_fSampleTime;            // +0x19c
    char pad_0x1a0[0x1c4 - 0x1a0];
    float* m_pVertexKeys;           // +0x1c4
    float* m_pUVKeys;               // +0x1c8
    int m_nVertexKeys;              // +0x1cc
    int m_nUVKeys;                  // +0x1d0

    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

// NONMATCH (64 insns): instruction stream nearly identical, but the frame is
// 8 bytes larger (locals at different slots), and the original keeps vKey0.y
// alive for `rest * k1` (fld st(4); fmul st(1); fstp st(2)) where ours
// multiplies in place. Key vectors must be float-built (fld) locals and the
// vertex a copy (z through an int temp); that needed Vector's explicit copy
// ctor / operator= in mdlanim.h.
// FUNCTION: SWKOTOR 0x00485660
void MdlNodeAnimMesh::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    int nKey;
    PartTriMesh* pMesh = (PartTriMesh*)pPart->GetRoot();
    if (m_fSampleTime != 0.0f)
        nKey = (int)(fTime / m_fSampleTime);
    else
        nKey = 0;

    if (nKey >= 0 && nKey < m_nVertexKeys - 1) {
        float fFrac = fTime / m_fSampleTime - nKey;
        MeshBuffers* pBuffers = pMesh->m_pBufferOwner->GetBuffers();
        Vector* pVerts = (Vector*)LockBuffer(pBuffers->m_pVertexBuffer, 0, 0, 1);
        for (int i = 0; i < m_nVertexCount; i++) {
            Vector vVert = pVerts[i];
            int k = (m_nVertexKeys * i + nKey) * 3;
            Vector vKey0(m_pVertexKeys[k], m_pVertexKeys[k + 1], m_pVertexKeys[k + 2]);
            Vector vKey1(m_pVertexKeys[k + 3], m_pVertexKeys[k + 4], m_pVertexKeys[k + 5]);
            pVerts[i] = vVert * (1.0f - fBlend) + (vKey0 * (1.0f - fFrac) + vKey1 * fFrac) * fBlend;
        }
        UnlockBuffer(pBuffers->m_pVertexBuffer);
    }
    else if (m_nVertexKeys != 0) {
        MeshBuffers* pBuffers = pMesh->m_pBufferOwner->GetBuffers();
        Vector* pVerts = (Vector*)LockBuffer(pBuffers->m_pVertexBuffer, 0, 0, 1);
        if (nKey < 0)
            nKey = 0;
        if (nKey > m_nVertexKeys - 1)
            nKey = m_nVertexKeys - 1;
        for (int i = 0; i < m_nVertexCount; i++) {
            Vector vVert = pVerts[i];
            int k = (m_nVertexKeys * i + nKey) * 3;
            Vector vKey(m_pVertexKeys[k], m_pVertexKeys[k + 1], m_pVertexKeys[k + 2]);
            pVerts[i] = vVert * (1.0f - fBlend) + vKey * fBlend;
        }
        UnlockBuffer(pBuffers->m_pVertexBuffer);
    }

    if (m_nUVKeys != 0) {
        MeshBuffers* pBuffers = pMesh->m_pBufferOwner->GetBuffers();
        Vector2* pUVs = (Vector2*)LockBuffer(pBuffers->m_pUVBuffer, 0, 0, 1);
        if (nKey >= 0 && nKey < m_nUVKeys - 1) {
            float fFrac = fTime / m_fSampleTime - nKey;
            for (int i = 0; i < m_nVertexCount; i++) {
                Vector2 vUV = pUVs[i];
                int k = (m_nUVKeys * i + nKey) * 2;
                Vector2 vKey0(m_pUVKeys[k], m_pUVKeys[k + 1]);
                Vector2 vKey1(m_pUVKeys[k + 2], m_pUVKeys[k + 3]);
                pUVs[i] = vUV * (1.0f - fBlend) + (vKey0 * (1.0f - fFrac) + vKey1 * fFrac) * fBlend;
            }
        }
        else if (m_nUVKeys != 0) {
            int nClamped = nKey > 0 ? nKey : 0;
            if (nClamped >= m_nUVKeys - 1)
                nClamped = m_nUVKeys - 1;
            for (int i = 0; i < m_nVertexCount; i++) {
                Vector2 vUV = pUVs[i];
                int k = (m_nUVKeys * i + nClamped) * 2;
                Vector2 vKey(m_pUVKeys[k], m_pUVKeys[k + 1]);
                pUVs[i] = vUV * (1.0f - fBlend) + vKey * fBlend;
            }
        }
        UnlockBuffer(pBuffers->m_pUVBuffer);
    }

    pPart->UpdateTransform();
    MdlNode::InternalControl(pPart, fPrevTime, fTime, fBlend);
}
