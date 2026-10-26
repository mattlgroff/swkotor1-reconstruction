// PartInitMaterial: load each tri-mesh part's textures and build its vertex
// primitive (skin / anim / dangly / plain), recursing over the part tree.
// Behavioural reference: src/units/0667/9129632a82-model_material.cpp.
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class MdlNodeTriMesh {
public:
    char pad00[0x138];
    void* m_p138;                           // +0x138
    char pad13c[0x160 - 0x13c];
    int m_n160;                             // +0x160
};

class MdlNode {
public:
    unsigned char m_nType;                  // +0x00

    __forceinline MdlNodeTriMesh* AsMdlNodeTriMesh()
    {
        return (m_nType & 0x21) != 0x21 ? 0 : (MdlNodeTriMesh*)this;
    }
};

class Part;
class PartTriMesh;

class Material {
public:
    void LoadTextures(Part* pPart);         // 0x0047b770
};

class VertexPrimitive {
public:
    virtual ~VertexPrimitive();
};

class VertexPrimitiveSkin : public VertexPrimitive {
public:
    VertexPrimitiveSkin(void* pSkin);       // 0x004794c0
    char pad04[0x1c - 0x04];
};

class VertexPrimitiveAnim : public VertexPrimitive {
public:
    VertexPrimitiveAnim(PartTriMesh* pMesh);    // 0x004793a0
    char pad04[0x24 - 0x04];
};

class VertexPrimitiveDangly : public VertexPrimitive {
public:
    VertexPrimitiveDangly(PartTriMesh* pMesh);  // 0x00479430
    char pad04[0x20 - 0x04];
};

class VertexPrimitiveMesh : public VertexPrimitive {
public:
    VertexPrimitiveMesh(PartTriMesh* pMesh);    // 0x004792a0
    char pad04[0x1c - 0x04];
};

class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual PartTriMesh* AsTriMesh();       // +0x2c
    virtual void* AsAnimMesh();             // +0x30
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void* AsDanglyMesh();           // +0x4c
    virtual void* AsSkin();                 // +0x50

    MdlNode* m_pNode;                       // +0x04
    char pad08[0x30 - 0x08];
    CExoArrayList<Part*> m_lChildren;       // +0x30
    char pad3c[0x44 - 0x3c];
    Material* m_pMaterial;                  // +0x44
};

class PartTriMesh : public Part {
public:
    char pad48[0x70 - 0x48];
    VertexPrimitive* m_pPrimitive;          // +0x70
};

class Gob {
public:
    char pad00[0x184];
    unsigned char m_bHasAnimatedTexture;    // +0x184
};

#pragma inline_recursion(on)
#pragma inline_depth(1)

// FUNCTION: SWKOTOR 0x00442d70
// NONMATCH: the original inlines its recursion once but still inlines the
// node casts inside the inlined copy (inline_depth(1) leaves them as calls,
// inline_depth(2) recurses a second level), and keeps pPart on the stack.
void __cdecl PartInitMaterial(Gob* pGob, Part* pPart, char* szPrefix)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (pMesh) {
        if (pMesh->m_pNode->AsMdlNodeTriMesh()->m_p138) {
            if (pMesh->m_pNode->AsMdlNodeTriMesh()->m_n160 != -1 && pGob)
                pGob->m_bHasAnimatedTexture = 1;
        }
        if (pMesh->m_pMaterial)
            pMesh->m_pMaterial->LoadTextures(pPart);
        VertexPrimitive* pPrimitive;
        if (pMesh->AsSkin())
            pPrimitive = new VertexPrimitiveSkin(pMesh->AsSkin());
        else if (pMesh->AsAnimMesh())
            pPrimitive = new VertexPrimitiveAnim(pMesh);
        else if (pMesh->AsDanglyMesh())
            pPrimitive = new VertexPrimitiveDangly(pMesh);
        else
            pPrimitive = new VertexPrimitiveMesh(pMesh);
        pMesh->m_pPrimitive = pPrimitive;
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        PartInitMaterial(pGob, pPart->m_lChildren.element[i], szPrefix);
}
