// Part / node tree walkers used when spawning tiles (0x00452000,
// 0x00452500). The original inlines each recursion several levels deep.

extern "C" int __cdecl strcmp(const char*, const char*);
#pragma intrinsic(strcmp)

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

class Model {
public:
    char pad_0x0[0xb8];
    CExoArrayList<char*> m_lStrings;        // +0xb8
};

class Animation {
public:
    char pad_0x0[0x84];
    Model* m_pModel;                        // +0x84
};

class MaxTree {
public:
    Model* AsModel();                       // 0x0043E1C0
    Animation* AsAnimation();               // 0x0043E1D0
};

class MdlNode {
public:
    char pad_0x0[4];
    int m_nNameIndex;                       // +0x04
    MaxTree* m_pTree;                       // +0x08
    char pad_0xc[0x20];
    CExoArrayList<MdlNode*> m_lChildren;    // +0x2c

    // 0x00438A00
    inline char* GetName()
    {
        if (m_pTree->AsModel())
            return m_pTree->AsModel()->m_lStrings[m_nNameIndex];
        return m_pTree->AsAnimation()->m_pModel->m_lStrings[m_nNameIndex];
    }
};

class PartTriMeshNode {
public:
    char pad_0x0[0x185];
    unsigned char m_bRotateTexture;         // +0x185
};

class PartTriMesh {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual void V2C();
    virtual void V30();
    virtual void V34();
    virtual void V38();
    virtual void V3C();
    virtual void V40();
    virtual void V44();
    virtual void V48();
    virtual void V4C();
    virtual void V50();
    virtual void V54();
    virtual void V58();
    virtual void V5C();
    virtual void V60();
    virtual void V64();
    virtual void V68();
    virtual void V6C();
    virtual void V70();
    virtual void V74();
    virtual void V78();
    virtual void SetTextureRotation(float fDegrees);    // +0x7c
    PartTriMeshNode* m_pNode;               // +0x04
};

class Part {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual PartTriMesh* AsTriMesh();       // +0x2c
    char pad_0x4[0x2c];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

#pragma inline_recursion(on)
#pragma inline_depth(9)

// FUNCTION: SWKOTOR 0x00452000
MdlNode* findnode(MdlNode* pNode, char* szName)
{
    if (!strcmp(pNode->GetName(), szName))
        return pNode;
    MdlNode* pFound = 0;
    for (int i = 0; i < pNode->m_lChildren.num; i++) {
        pFound = findnode(pNode->m_lChildren.element[i], szName);
        if (pFound)
            break;
    }
    return pFound;
}

// FUNCTION: SWKOTOR 0x00452500
void SetPartTextureRotate(Part* pPart, int nRotation)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (pMesh && pMesh->m_pNode->m_bRotateTexture)
        pMesh->SetTextureRotation(nRotation * 90.0f);
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        SetPartTextureRotate(pPart->m_lChildren.element[i], nRotation);
}
