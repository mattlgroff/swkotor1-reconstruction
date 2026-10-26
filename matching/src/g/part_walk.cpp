// Recursive part-tree setters: fog flag and material down-sampling. The
// original inlines each recursion several levels deep.
// Behavioural reference: src/units/0803/85446d96e4-m47_gob1.cpp.
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

class MdlNode {
public:
    unsigned char m_nType;                  // +0x00
    char pad01[0xa8 - 0x01];
    char m_szBitmap[0x20];                  // +0xa8 (tri-mesh texture name)
};

class Material {
public:
    void SetDownSampling(int b);           // 0x0047af10
};

class PartTriMesh {
public:
    char pad00[0x7d];
    unsigned char m_bFog;                   // +0x7d
};

class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual PartTriMesh* AsTriMesh();       // +0x2c

    MdlNode* m_pNode;                       // +0x04
    char pad08[0x30 - 0x08];
    CExoArrayList<Part*> m_lChildren;       // +0x30
    char pad3c[0x44 - 0x3c];
    Material* m_pMaterial;                  // +0x44
};

#pragma inline_recursion(on)
#pragma inline_depth(10)

// FUNCTION: SWKOTOR 0x00440550
void __cdecl PartSetFog(Part* pPart, unsigned char bOn)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (pMesh)
        pMesh->m_bFog = bOn;
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        PartSetFog(pPart->m_lChildren[i], bOn);
}

#pragma inline_depth(5)

// FUNCTION: SWKOTOR 0x004434a0
int __cdecl PartSetCanDownSample(Part* pPart, char b)
{
    int nCount = 0;
    MdlNode* pNode = pPart->m_pNode;
    if ((pNode->m_nType & 0x21) == 0x21 && pNode->m_szBitmap[0] &&
        strcmp("NULL", pNode->m_szBitmap)) {
        pPart->m_pMaterial->SetDownSampling(b);
        nCount = 1;
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        nCount += PartSetCanDownSample(pPart->m_lChildren.element[i], b);
    return nCount;
}
