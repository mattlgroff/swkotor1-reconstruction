// recursive part-tree collectors (light parts, materials, trimesh
// parts, named textures). The original inlines each recursion seven levels.
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
    void Allocate(int n);                       // 0x005e03c0
    void AddUnique(T t);                        // 0x0046bd10
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
};

extern "C" int __cdecl _stricmp(const char* a, const char* b);    // 0x0070acaf

class PartLight;
class PartTriMesh;
class Material;

class CAurTexture {
public:
    virtual void v00();
    virtual CAurTexture* AsBasic();         // +0x04
    virtual CAurTexture* GetTarget();       // +0x08
    virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18();
    virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2c(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3c();
    virtual char* GetName();                // +0x40
};

class Material {
public:
    CAurTexture* m_pTexture;                // +0x00
};

class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20();
    virtual PartLight* AsLight();           // +0x24
    virtual void v28();
    virtual PartTriMesh* AsTriMesh();       // +0x2c

    char pad04[0x30 - 0x04];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

class PartTriMesh {
public:
    char pad00[0x44];
    Material* m_pMaterial;                  // +0x44
};

class PartWithMaterial {
public:
    char pad00[0x30];
    CExoArrayList<Part*> m_lChildren;       // +0x30
    char pad3c[0x44 - 0x3c];
    Material* m_pMaterial;                  // +0x44
};

#pragma inline_recursion(on)
#pragma inline_depth(10)

// FUNCTION: SWKOTOR 0x00445ae0
void __cdecl findalllightparts(Part* pPart, CExoArrayList<PartLight*>* pList)
{
    PartLight* pLight = pPart->AsLight();
    if (pLight)
        pList->Add(pLight);
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        findalllightparts(pPart->m_lChildren[i], pList);
}

// FUNCTION: SWKOTOR 0x00445ec0
void __cdecl findallmaterials(Part* pPart, CExoArrayList<Material*>* pList)
{
    PartWithMaterial* p = (PartWithMaterial*)pPart;
    if (p->m_pMaterial)
        pList->AddUnique(p->m_pMaterial);
    for (int i = 0; i < p->m_lChildren.num; i++)
        findallmaterials(p->m_lChildren[i], pList);
}

// FUNCTION: SWKOTOR 0x004461d0
void __cdecl findalltrimeshparts(Part* pPart, CExoArrayList<PartTriMesh*>* pList)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (pMesh)
        pList->Add(pMesh);
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        findalltrimeshparts(pPart->m_lChildren[i], pList);
}

#pragma inline_depth(6)

// FUNCTION: SWKOTOR 0x00446ab0
void __cdecl GetAllNamedTextures(Part* pPart, CExoArrayList<CAurTexture*>* pList,
                                 char* szName)
{
    PartTriMesh* pMesh = pPart->AsTriMesh();
    if (pMesh) {
        CAurTexture* pTexture = pMesh->m_pMaterial->m_pTexture->GetTarget();
        if (pTexture->AsBasic() && !_stricmp(pTexture->GetName(), szName))
            pList->AddUnique(pTexture);
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        GetAllNamedTextures(pPart->m_lChildren.element[i], pList, szName);
}
