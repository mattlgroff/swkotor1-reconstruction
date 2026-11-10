// CollapsePartTree (0x00453F10): flattens a part tree into a list, applying
// each parent's placement to its children and instancing reference parts
// into the room's object list. With a Gob, the child named "<model>a" is
// kept as the tree's only child. The name is descriptive.

extern "C" char* __cdecl strcpy(char*, const char*);
extern "C" char* __cdecl strcat(char*, const char*);
#pragma intrinsic(strcpy, strcat)
extern "C" int __cdecl _stricmp(const char*, const char*);
extern "C" int __cdecl rand();
void* operator new(unsigned int);
void operator delete(void*);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);                   // 0x005E03C0 (pointer lists)
    T& operator[](int i) { return element[i]; }
    inline void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    __forceinline void AddCopy(T value)
    {
        if (num == array_size) {
            int n = array_size ? array_size * 2 : 8;
            T* old = element;
            array_size = n;
            element = new T[n];
            for (int i = 0; i < num; i++)
                element[i] = old[i];
            if (old)
                delete[] old;
        }
        element[num++] = value;
    }
    __forceinline void RemoveInline(T value)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == value)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator+(const Vector& o) const { return Vector(x + o.x, y + o.y, z + o.z); }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ) { z = fZ; y = fY; x = fX; w = fW; }
};

Vector operator*(const Quaternion& q, const Vector& v);         // 0x004A9BB0
Quaternion operator*(const Quaternion& a, const Quaternion& b); // 0x004A9AC0

class Model {
public:
    char pad_0x0[8];
    char m_szName[0xb0];                    // +0x08
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
    char pad_0xc[0x44];
    char m_szRefModel[32];                  // +0x50

    // 0x00438A00
    inline char* GetName()
    {
        if (m_pTree->AsModel())
            return m_pTree->AsModel()->m_lStrings[m_nNameIndex];
        return m_pTree->AsAnimation()->m_pModel->m_lStrings[m_nNameIndex];
    }
};

class Scene;
class Gob;
class Part;

class PartReference {
public:
    void* m_vtbl;
    MdlNode* m_pNode;                       // +0x04
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
    const Vector& GetPosition() { return m_vPosition; }
    const Quaternion& GetOrientation() { return m_qOrientation; }
    MdlNode* m_pNode;                       // +0x04
    Vector m_vPosition;                     // +0x08
    Quaternion m_qOrientation;              // +0x14
    char pad_0x24[0x30 - 0x24];
    CExoArrayList<Part*> m_lChildren;       // +0x30
    Part* m_pParent;                        // +0x3c
    Gob* m_pGob;                            // +0x40
};

class Gob {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void PlayAnimation(char* szName, float fSpeed, int nFlags, float fOffset);  // +0x18
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
    virtual Vector SetPosition(Vector v);               // +0x5c
    virtual Quaternion SetOrientation(Quaternion q);    // +0x60
    virtual void V64(); virtual void V68(); virtual void V6C();
    virtual void V70(); virtual void V74(); virtual void V78(); virtual void V7C();
    virtual void V80(); virtual void V84(); virtual void V88(); virtual void V8C();
    virtual void V90(); virtual void V94(); virtual void V98(); virtual void V9C();
    virtual void VA0(); virtual void VA4(); virtual void VA8(); virtual void VAC();
    virtual void VB0(); virtual void VB4(); virtual void VB8(); virtual void VBC();
    virtual void VC0(); virtual void VC4(); virtual void VC8(); virtual void VCC();
    virtual void VD0(); virtual void VD4(); virtual void VD8(); virtual void VDC();
    virtual void VE0(); virtual void VE4(); virtual void VE8(); virtual void VEC();
    virtual void VF0(); virtual void VF4(); virtual void VF8(); virtual void VFC();
    virtual void V100(); virtual void V104(); virtual void V108(); virtual void V10C();
    virtual void V110(); virtual void V114(); virtual void V118(); virtual void V11C();
    virtual void V120(); virtual void V124(); virtual void V128(); virtual void V12C();
    virtual void V130(); virtual void V134(); virtual void V138(); virtual void V13C();
    virtual void V140(); virtual void V144(); virtual void V148(); virtual void V14C();
    virtual void V150(); virtual void V154(); virtual void V158(); virtual void V15C();
    virtual void V160(); virtual void V164(); virtual void V168(); virtual void V16C();
    virtual void V170(); virtual void V174(); virtual void V178(); virtual void V17C();
    virtual void V180();
    virtual void PartsChanged();                        // +0x184, descriptive

    char pad_0x4[0x58 - 0x4];
    Model* m_pModel;                        // +0x58
    char pad_0x5c[0x6c - 0x5c];
    Part* m_pRootPart;                      // +0x6c

    void AttachToScene(Scene* pScene);      // 0x0043F200
    void SetProcessFlag();                  // 0x0043F270
};

class CAurRoom {
public:
    char pad_0x0[0x68];
    CExoArrayList<Gob*> m_lObjects;         // +0x68
};

Gob* NewCAurObject(const char* szModel, const char* szName, int a, int b);  // 0x00449CC0

// NONMATCH: only the two by-value argument copies (SetPosition /
// SetOrientation) colour differently: the original has the vtable in eax,
// source in ecx, destination in edx; cl gives eax/ecx to the copy first.
// FUNCTION: SWKOTOR 0x00453F10
void CollapsePartTree(Gob* pGob, Part* pTree, CExoArrayList<Part*>* pParts, Vector vPosition,
                      Quaternion qOrientation, Scene* pScene, CAurRoom* pRoom)
{
    char szSpecial[0x40];
    Vector vSavedPosition(0.0f, 0.0f, 0.0f);
    Quaternion qSavedOrientation(1.0f, 0.0f, 0.0f, 0.0f);
    Part* pSpecial = 0;
    if (pGob) {
        strcpy(szSpecial, pGob->m_pModel->m_szName);
        strcat(szSpecial, "a");
        vSavedPosition = pTree->m_vPosition;
        qSavedOrientation = pTree->m_qOrientation;
    }
    pTree->m_vPosition = vPosition + qOrientation * pTree->m_vPosition;
    pTree->m_qOrientation = qOrientation * pTree->m_qOrientation;
    pParts->Add(pTree);
    for (int i = 0; i < pParts->num; i++) {
        Part* pPart = pParts->element[i];
        PartReference* pRef = pPart->AsReference();
        if (pRef && pRoom && pScene) {
            Gob* pObject = NewCAurObject(pRef->m_pNode->m_szRefModel, "", 0, 0);
            if (pObject) {
                pObject->SetPosition(pPart->GetPosition());
                pObject->SetOrientation(pPart->GetOrientation());
                float fSpeed = (rand() % 100 - 50.0f) * 0.001f + 1.0f;
                float fOffset = (rand() % 100) * 0.01f;
                pObject->PlayAnimation("default", fSpeed, 0, fOffset);
                pObject->AttachToScene(pScene);
                pRoom->m_lObjects.Add(pObject);
            }
        }
        for (int j = 0; j < pPart->m_lChildren.num; j++) {
            Part* pChild = pPart->m_lChildren.element[j];
            if (pGob && !_stricmp(pChild->m_pNode->GetName(), szSpecial)) {
                pSpecial = pChild;
                continue;
            }
            pChild->m_vPosition = pPart->m_qOrientation * pChild->m_vPosition + pPart->m_vPosition;
            pChild->m_qOrientation = pPart->m_qOrientation * pChild->m_qOrientation;
            pParts->AddCopy(pChild);
        }
        pPart->m_lChildren.num = 0;
        pPart->m_pParent = 0;
        if (pPart->m_pGob && pPart->m_pGob->m_pRootPart == pPart)
            pPart->m_pGob->m_pRootPart = 0;
        pPart->m_pGob = 0;
    }
    if (pGob) {
        pParts->RemoveInline(pTree);
        pTree->m_lChildren.Add(pSpecial);
        pTree->m_pGob = pGob;
        pGob->m_pRootPart = pTree;
        pTree->m_vPosition = vSavedPosition;
        pTree->m_qOrientation = qSavedOrientation;
        pGob->PartsChanged();
        pGob->SetProcessFlag();
    }
}
