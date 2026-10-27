// Reference-node expansion for a freshly instanced part tree (0x00454470).
// The original inlines the recursion seven levels deep; only the outermost
// level expands the child-list Add.

extern "C" int __cdecl sprintf(char*, const char*, ...);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);                   // 0x005E03C0 (pointer lists)
    inline void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
};

class MdlNode {
public:
    char pad_0x0[0x50];
    char m_szRefModel[32];                  // +0x50

    char* GetName();                        // 0x00438A00
};

class Model {
public:
    char pad_0x0[0x28];
    MdlNode* m_pRoot;                       // +0x28
};

class Gob;
class Part;

class PartReference {
public:
    void* m_vtbl;
    MdlNode* m_pNode;                       // +0x04
    char pad_0x8[0x28];
    CExoArrayList<Part*> m_lChildren;       // +0x30
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
    virtual void V2C();
    virtual void V30();
    virtual void V34();
    virtual PartReference* AsReference();   // +0x38
    MdlNode* m_pNode;                       // +0x04
    char pad_0x8[0x28];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

Model* FindModel(char* szName);                                     // 0x00464110
Part* __cdecl CreateInstanceTree(MdlNode* pNode, Gob* pGob);        // 0x00449410
void __cdecl PartInitMaterial(Gob* pGob, Part* pPart, char* szPrefix);  // 0x00442D70

#pragma inline_recursion(on)
#pragma inline_depth(7)

// FUNCTION: SWKOTOR 0x00454470
void ExpandReferenceParts(Part* pPart)
{
    PartReference* pRef = pPart->AsReference();
    if (pRef) {
        MdlNode* pNode = pRef->m_pNode;
        Model* pModel = FindModel(pNode->m_szRefModel);
        pRef->m_lChildren.Add(CreateInstanceTree(pModel->m_pRoot, 0));
        char szPrefix[32];
        sprintf(szPrefix, "%s", pNode->GetName());
        PartInitMaterial(0, pPart, szPrefix);
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        ExpandReferenceParts(pPart->m_lChildren.element[i]);
}
