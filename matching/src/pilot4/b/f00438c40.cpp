// Pilot 4 arm B: 0x00438C40, recursive collection of renderable tri-mesh
// parts below a model node (recursion inlined by the compiler).

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    T &operator[](int i) { return element[i]; }
    void Allocate(int n);                       // <Part*>: 0x00437D60
    void Add(T t)                               // <Part*>: 0x00438B40
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

class Part;
class MdlNodeTriMesh;
class MdlNodeSkin;
class MdlNodeLightsaber;

class MdlNode {
public:
    MdlNodeTriMesh *AsMdlNodeTriMesh();         // 0x0043E400
    MdlNodeSkin *AsMdlNodeSkin();               // 0x0043E3F0
    MdlNodeLightsaber *AsMdlNodeLightsaber();   // 0x0043E3A0
    int GetPartsList(CExoArrayList<Part *> *pList);

    char pad_0x0[0x2c];
    CExoArrayList<MdlNode *> m_lChildren;       // +0x2c
};

class MdlNodeTriMesh {
public:
    char pad_0x0[0x189];
    unsigned char m_bRender;                    // +0x189
};

#pragma inline_recursion(on)
// FUNCTION: SWKOTOR 0x00438C40
inline int MdlNode::GetPartsList(CExoArrayList<Part *> *pList)
{
    int nSkins = 0;
    for (int i = 0; i < m_lChildren.num; i++) {
        if (m_lChildren[i]->AsMdlNodeLightsaber()) {
            continue;
        }
        if (m_lChildren[i]->AsMdlNodeTriMesh() && m_lChildren[i]->AsMdlNodeTriMesh()->m_bRender) {
            pList->Add((Part *)m_lChildren[i]->AsMdlNodeTriMesh());
            if (m_lChildren[i]->AsMdlNodeSkin()) {
                nSkins++;
            }
        }
        nSkins += m_lChildren[i]->GetPartsList(pList);
    }
    return nSkins;
}

// Taking the address emits the out-of-line copy of the inline function.
int (MdlNode::*g_pfnGetPartsList)(CExoArrayList<Part *> *) = &MdlNode::GetPartsList;
