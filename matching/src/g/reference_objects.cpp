// CreateReferenceObjects: create and attach the objects that reference
// nodes of a part tree name. The original inlines its recursion several
// levels deep. Behavioural reference:
// src/units/0666/bc5557a6fd-model_lifetime.cpp.
extern "C" int __cdecl rand();

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

class MdlNodeReference {
public:
    char pad00[0x50];
    char m_szRefModel[0x20];                    // +0x50
};

class MdlNode {
public:
    unsigned char m_nType;                      // +0x00

    MdlNodeReference* AsMdlNodeReference()
    {
        return (MdlNodeReference*)((m_nType & 0x11) == 0x11 ? this : 0);
    }
};

class Gob {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00c();
    virtual void v010(); virtual void v014();
    virtual void PlayAnimation(char* szName, float fSpeed, int nFlags, float fOffset);  // +0x018
    virtual void v01c();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04c();
    virtual void* AttachToObject(Gob* pObject, char* szPart, int nMode);   // +0x050
};

Gob* __cdecl NewAurObject(char* szModel, char* szName, void* p2, void* p3);   // 0x00449cc0

class PartReference;

class Part {
public:
    virtual void v00();
    virtual char* GetName();                    // +0x04
    virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34();
    virtual PartReference* AsReference();       // +0x38

    MdlNode* m_pNode;                           // +0x04
    char pad08[0x30 - 0x08];
    CExoArrayList<Part*> m_lChildren;           // +0x30
};

class PartReference : public Part {
public:
    char pad3c[0x4c - 0x3c];
    Gob* m_pObject;                             // +0x4c
};

#pragma inline_recursion(on)
#pragma inline_depth(4)

// FUNCTION: SWKOTOR 0x004448c0
// NONMATCH: same four inlined levels, but the original still inlines the
// node cast in the innermost copy (here it is a call) and computes its mask
// into ecx (`and ecx, eax`); see the notes (PartInitMaterial has the same tie).
void __cdecl CreateReferenceObjects(Gob* pGob, Part* pPart)
{
    PartReference* pRef = pPart->AsReference();
    if (pRef) {
        pRef->m_pObject = NewAurObject(pRef->m_pNode->AsMdlNodeReference()->m_szRefModel, 0, 0, 0);
        if (pRef->m_pObject) {
            pRef->m_pObject->AttachToObject(pGob, pRef->GetName(), 0);
            float fSpeed = (rand() % 100 - 50.0f) * 0.001f + 1.0f;
            float fOffset = rand() % 100 * 0.01f;
            pRef->m_pObject->PlayAnimation("default", fSpeed, 0, fOffset);
        }
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        CreateReferenceObjects(pGob, pPart->m_lChildren.element[i]);
}
