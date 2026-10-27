// Stencil-shadow face classification for a trimesh node (0x004741A0).

class Vector {
public:
    float x, y, z;
};

struct FaceAdjacency {
    int n[3];
};

struct MdlFace {
    Vector vNormal;                         // +0x00
    float fDistance;                        // +0x0c
    int nMaterial;                          // +0x10
    FaceAdjacency adjacent;                 // +0x14
};

struct ShadowFace {
    float fFacing;                          // +0x00
    FaceAdjacency adjacent;                 // +0x04
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class MdlNodeTriMesh {
public:
    char pad_0x0[0x58];
    CExoArrayList<MdlFace> m_lFaces;        // +0x58
};

// NONMATCH (5 insns): the original keeps the face counter in the dead
// third-argument slot; ours allocates a frame slot (`push ecx`).
// FUNCTION: SWKOTOR 0x004741A0
void ProcessFaces(Vector* pLight, ShadowFace* pOut, MdlNodeTriMesh* pNode)
{
    for (int i = 0; i < pNode->m_lFaces.num; i++) {
        MdlFace* pFace = &pNode->m_lFaces.element[i];
        pOut[i].fFacing = pFace->vNormal.y * pLight->y + pFace->vNormal.z * pLight->z +
                          pFace->vNormal.x * pLight->x + pFace->fDistance;
        pOut[i].adjacent = pNode->m_lFaces.element[i].adjacent;
    }
}
