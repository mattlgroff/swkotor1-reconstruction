// MdlNodeTriMesh face planes and edge neighbours (0x004386b0). Name not
// recorded; behavioural reference: src/units/0668/5b9aacef0c-model_postprocess.cpp.
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

class Vector {
public:
    float x, y, z;

    Vector& operator=(const Vector& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector __cdecl NormalOf(const Vector& a, const Vector& b, const Vector& c);  // 0x004aca00

// Mesh face (0x20 bytes): plane and the neighbour across each edge.
class MdlFace {
public:
    Vector m_vNormal;                       // +0x00
    float m_fDistance;                      // +0x0c
    int m_n10;                              // +0x10
    short m_anAdjacent[3];                  // +0x14
    char pad1a[0x20 - 0x1a];
};

// Corner indices of a post-process face (0x14 bytes).
class MdlFaceIndices {
public:
    int* m_pIndices;                        // +0x00
    char pad04[0x14 - 0x04];
};

class MdlNodeTriMesh_PostProcess {
public:
    Vector* m_pVertices;                    // +0x00
    char pad04[0x48 - 0x04];
    MdlFaceIndices* m_pFaces;               // +0x48
};

class MdlNodeTriMesh {
public:
    char pad00[0x58];
    CExoArrayList<MdlFace> m_lFaces;        // +0x58
    char pad64[0x134 - 0x64];
    MdlNodeTriMesh_PostProcess* m_pPostProcess; // +0x134

    void BuildNeighborLists();
};

// FUNCTION: SWKOTOR 0x004386b0
void MdlNodeTriMesh::BuildNeighborLists()
{
    for (int f = 0; f < m_lFaces.num; f++) {
        int* pIdx = m_pPostProcess->m_pFaces[f].m_pIndices;
        Vector* pVerts = m_pPostProcess->m_pVertices;
        m_lFaces[f].m_vNormal = NormalOf(pVerts[pIdx[0]], pVerts[pIdx[1]], pVerts[pIdx[2]]);
        Vector& v0 = m_pPostProcess->m_pVertices[m_pPostProcess->m_pFaces[f].m_pIndices[0]];
        m_lFaces[f].m_fDistance = -(v0 * m_lFaces[f].m_vNormal);
        for (int e = 0; e < 3; e++) {
            int* pFace = m_pPostProcess->m_pFaces[f].m_pIndices;
            int a = pFace[e];
            int b = pFace[(e + 1) % 3];
            m_lFaces[f].m_anAdjacent[e] = -1;
            for (int g = 0; g < m_lFaces.num; g++) {
                if (f != g) {
                    for (int j = 0; j < 3; j++) {
                        m_lFaces[f].m_anAdjacent[e] = -1;
                        if (m_pPostProcess->m_pFaces[g].m_pIndices[j] == a &&
                            (m_pPostProcess->m_pFaces[g].m_pIndices[(j + 1) % 3] == b ||
                             m_pPostProcess->m_pFaces[g].m_pIndices[(j + 2) % 3] == b)) {
                            m_lFaces[f].m_anAdjacent[e] = g;
                            break;
                        }
                    }
                }
                if (m_lFaces[f].m_anAdjacent[e] >= 0)
                    break;
            }
        }
    }
}
