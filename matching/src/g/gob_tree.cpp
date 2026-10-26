// Gob members that collect parts through the EnumParts virtual and
// rewrite their materials.
#include "g/exo_list.h"

class CAurTexture;

template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer<T>& operator=(T* p)
    {
        if (m_p)
            m_p->RemoveRef(this);
        m_p = p;
        if (p)
            p->AddRef(this);
        return *this;
    }
    operator T*() { return m_p; }
};

template <class T>
class SafePointerTarget {
public:
    CExoArrayList<SafePointer<T>*> m_lRefs;

    void AddRef(SafePointer<T>* p) { m_lRefs.Add(p); }
    void RemoveRef(SafePointer<T>* p) { m_lRefs.Remove(p); }
};

class CAurTexture : public SafePointerTarget<CAurTexture> {
public:
    virtual ~CAurTexture();
};


void __cdecl AurTextureRelease(CAurTexture* p);         // 0x00420d70

class Material {
public:
    char pad00[0x0c];
    SafePointer<CAurTexture> m_tBumpMap;                // +0x0c
};

class Part {
public:
    char pad00[0x44];
    Material* m_pMaterial;                              // +0x44
};

typedef int (__cdecl *PartFilter)(Part* pPart);

int __cdecl PartHasMaterial(Part* pPart);               // 0x0043ee40

class Gob {
public:
    virtual void V000(); virtual void V004(); virtual void V008(); virtual void V00c();
    virtual void V010(); virtual void V014(); virtual void V018(); virtual void V01c();
    virtual void V020(); virtual void V024(); virtual void V028(); virtual void V02c();
    virtual void V030(); virtual void V034(); virtual void V038(); virtual void V03c();
    virtual void V040(); virtual void V044(); virtual void V048(); virtual void V04c();
    virtual void V050(); virtual void V054(); virtual void V058(); virtual void V05c();
    virtual void V060(); virtual void V064(); virtual void V068(); virtual void V06c();
    virtual void V070(); virtual void V074(); virtual void V078(); virtual void V07c();
    virtual void V080(); virtual void V084(); virtual void V088(); virtual void V08c();
    virtual void V090(); virtual void V094(); virtual void V098(); virtual void V09c();
    virtual void V0a0(); virtual void V0a4(); virtual void V0a8(); virtual void V0ac();
    virtual void V0b0(); virtual void V0b4(); virtual void V0b8(); virtual void V0bc();
    virtual void V0c0(); virtual void V0c4(); virtual void V0c8(); virtual void V0cc();
    virtual void V0d0(); virtual void V0d4(); virtual void V0d8(); virtual void V0dc();
    virtual void V0e0(); virtual void V0e4(); virtual void V0e8(); virtual void V0ec();
    virtual void V0f0(); virtual void V0f4(); virtual void V0f8(); virtual void V0fc();
    virtual void V100(); virtual void V104(); virtual void V108(); virtual void V10c();
    virtual void EnumParts(PartFilter pfnFilter, CExoArrayList<Part*>* pList);  // +0x110

    char pad04[0x15 - 0x04];
    unsigned char m_bRenderBumped;                      // +0x15
    char pad16[0x11c - 0x16];
    int m_n11c;                                         // +0x11c

    void DisableRenderBumpedOut();
};

// FUNCTION: SWKOTOR 0x004485c0
void Gob::DisableRenderBumpedOut()
{
    if (!m_bRenderBumped)
        return;
    m_bRenderBumped = 0;
    CExoArrayList<Part*> lParts;
    EnumParts(PartHasMaterial, &lParts);
    for (int i = 0; i < lParts.num; i++) {
        Material* pMaterial = lParts.element[i]->m_pMaterial;
        CAurTexture* pTexture = pMaterial->m_tBumpMap;
        pMaterial->m_tBumpMap = 0;
        AurTextureRelease(pTexture);
    }
    m_n11c = 0;
}
