// recursive part-tree queries (FindPart, single texture,
// transparency, geometry flags). Each recursion is inlined into itself; the
// per-function inline_depth reproduces the original unrolling.
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

extern "C" int __cdecl _stricmp(const char* a, const char* b);    // 0x0070acaf

class CAurTexture;
class Gob;

class Material {
public:
    CAurTexture* m_pTexture;                // +0x00
};

class MdlNode {
public:
    char* GetName();                        // 0x00438a00
};

class PartTriMesh {
public:
    char pad00[0x44];
    Material* m_pMaterial;                  // +0x44
    char pad48[0x84 - 0x48];
    float m_fAlpha;                         // +0x84
};

class GobTextures {
public:
    char pad00[0x168];
    Material* m_pSingleMaterial;            // +0x168
};

class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual PartTriMesh* AsTriMesh();       // +0x2c

    MdlNode* m_pNode;                       // +0x04
    char pad08[0x2c - 0x08];
    unsigned char m_bHasGeometry;           // +0x2c
    char pad2d[0x30 - 0x2d];
    CExoArrayList<Part*> m_lChildren;       // +0x30
    char pad3c[0x40 - 0x3c];
    GobTextures* m_pGob;                    // +0x40
};

#pragma inline_recursion(on)
#pragma inline_depth(7)

// FUNCTION: SWKOTOR 0x0043fe00
Part* __cdecl FindPart(Part* pPart, char* szName)
{
    if (!pPart)
        return 0;
    if (!szName)
        return 0;
    if (!_stricmp(pPart->m_pNode->GetName(), szName))
        return pPart;
    for (int i = 0; i < pPart->m_lChildren.num; i++) {
        Part* pFound = FindPart(pPart->m_lChildren.element[i], szName);
        if (pFound)
            return pFound;
    }
    return 0;
}

#pragma inline_depth(4)

// FUNCTION: SWKOTOR 0x00443b30
// NONMATCH: the original compares the textures straight from memory
// (`cmp [mat], 0` first) and unrolls five levels in 576 bytes; ours hoists
// the single-material texture load (tried an inline SameTexture helper and
// a ternary).
unsigned char __cdecl TreeHasSingleTexture(Part* pPart)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    GobTextures* pGob = pPart->m_pGob;
    if (!pGob)
        return 0;
    if (pMesh) {
        Material* pMaterial = pMesh->m_pMaterial;
        if (!pGob->m_pSingleMaterial) {
            pGob->m_pSingleMaterial = pMaterial;
        } else if (pMaterial->m_pTexture) {
            if (pMaterial->m_pTexture != pGob->m_pSingleMaterial->m_pTexture)
                return 0;
        } else if (pGob->m_pSingleMaterial->m_pTexture) {
            return 0;
        }
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++) {
        if (!TreeHasSingleTexture(pPart->m_lChildren.element[i]))
            return 0;
    }
    return 1;
}

#pragma inline_depth(8)

// FUNCTION: SWKOTOR 0x00443d70
unsigned char __cdecl TreeHasTransparency(Part* pPart)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (!pPart->m_pGob)
        return 0;
    if (pMesh && pMesh->m_fAlpha < 1.0f)
        return 1;
    for (int i = 0; i < pPart->m_lChildren.num; i++) {
        if (TreeHasTransparency(pPart->m_lChildren.element[i]))
            return 1;
    }
    return 0;
}

#pragma inline_depth(9)

// FUNCTION: SWKOTOR 0x004440d0
unsigned char __cdecl TreeHasGeometry(Part* pPart)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (!pPart->m_pGob)
        return 0;
    if (pMesh)
        return 1;
    for (int i = 0; i < pPart->m_lChildren.num; i++) {
        if (TreeHasGeometry(pPart->m_lChildren.element[i]))
            return 1;
    }
    return 0;
}

#pragma inline_depth(10)

// FUNCTION: SWKOTOR 0x004443e0
unsigned char __cdecl TreeSetGeometryFlags(Part* pPart)
{
    pPart->m_bHasGeometry = 0;
    for (int i = 0; i < pPart->m_lChildren.num; i++) {
        if (TreeSetGeometryFlags(pPart->m_lChildren[i]))
            pPart->m_bHasGeometry = 1;
    }
    if (pPart->AsTriMesh())
        pPart->m_bHasGeometry = 1;
    return pPart->m_bHasGeometry;
}
