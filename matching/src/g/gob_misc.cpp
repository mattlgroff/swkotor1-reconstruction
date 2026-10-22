// Gob members that walk the part tree or the collected light
// list, and the model super-model release.
#define G_EXO_LIST_CONTAINS
#include "g/exo_list.h"

class Part;
class Model;

class PartLight {
public:
    char pad00[0x58];
    float m_fRadius;                        // +0x58
    char pad5c[0x74 - 0x5c];
    int m_nPriority;                        // +0x74
};

class MdlNodeTriMesh;

class MdlNode {
public:
    unsigned short m_nType;                 // +0x00

    void InternalDestructorDispatch();      // 0x004488f0

    MdlNodeTriMesh* AsMdlNodeTriMesh()
    {
        if ((m_nType & 0x021) == 0x021)
            return (MdlNodeTriMesh*)this;
        return 0;
    }
};

class MdlNodeTriMesh : public MdlNode {
public:
    char pad02[0xa8 - 0x02];
    char m_szTexture0[32];                  // +0xa8
};

class Material {
public:
    void SetTexture(int nStage, char* szName);  // 0x0047b890
};

class PartTriMesh {
public:
    void* m_pVtbl;                          // +0x00
    MdlNode* m_pNode;                       // +0x04
    char pad08[0x44 - 0x08];
    Material* m_pMaterial;                  // +0x44
};

class CAurTexture;

void __cdecl findalltrimeshparts(Part* pRoot,
                                 CExoArrayList<PartTriMesh*>* pList);  // 0x004461d0
void __cdecl GetAllNamedTextures(Part* pRoot, CExoArrayList<CAurTexture*>* pList,
                                 char* szName);                        // 0x00446ab0
void __cdecl AurTextureSetAttributes(CAurTexture* pTexture, int nFlags); // 0x00422ce0

void __cdecl findalllightparts(Part* pRoot,
                               CExoArrayList<PartLight*>* pList);  // 0x00445ae0
unsigned char __cdecl TreeHasSingleTexture(Part* pRoot);           // 0x00443b30
unsigned char __cdecl TreeHasGeometry(Part* pRoot);                // 0x004440d0
void __cdecl TreeSetGeometryFlags(Part* pRoot);                    // 0x004443e0
unsigned char __cdecl TreeHasTransparency(Part* pRoot);            // 0x00443d70

class Gob {
public:
    virtual void V000();
    virtual void V004();
    virtual void V008();
    virtual void V00c();
    virtual void V010();
    virtual void V014();
    virtual void V018();
    virtual void V01c();
    virtual void V020();
    virtual void V024();
    virtual void V028();
    virtual void V02c();
    virtual void V030();
    virtual void V034();
    virtual void V038();
    virtual void V03c();
    virtual void V040();
    virtual void V044();
    virtual void V048();
    virtual void V04c();
    virtual void V050();
    virtual void V054();
    virtual void V058();
    virtual void V05c();
    virtual void V060();
    virtual void V064();
    virtual void V068();
    virtual void V06c();
    virtual void V070();
    virtual void V074();
    virtual void V078();
    virtual void V07c();
    virtual void V080();
    virtual void V084();
    virtual void V088();
    virtual void V08c();
    virtual void V090();
    virtual void V094();
    virtual void V098();
    virtual void V09c();
    virtual void V0a0();
    virtual void V0a4();
    virtual void V0a8();
    virtual void V0ac();
    virtual void V0b0();
    virtual void V0b4();
    virtual void V0b8();
    virtual void V0bc();
    virtual void V0c0();
    virtual void V0c4();
    virtual void V0c8();
    virtual void V0cc();
    virtual void V0d0();
    virtual void V0d4();
    virtual void V0d8();
    virtual void V0dc();
    virtual void V0e0();
    virtual void V0e4();
    virtual void V0e8();
    virtual void V0ec();
    virtual void V0f0();
    virtual void V0f4();
    virtual void V0f8();
    virtual void V0fc();
    virtual void V100();
    virtual void V104();
    virtual void V108();
    virtual void V10c();
    virtual void V110();
    virtual void V114();
    virtual void V118();
    virtual void V11c();
    virtual void V120();
    virtual void V124();
    virtual void V128();
    virtual void V12c();
    virtual void V130();
    virtual void V134();
    virtual void V138();
    virtual void V13c();
    virtual void V140();
    virtual void V144();
    virtual void V148();
    virtual void V14c();
    virtual void V150();
    virtual void V154();
    virtual void V158();
    virtual void V15c();
    virtual void V160();
    virtual void V164();
    virtual void V168();
    virtual void V16c();
    virtual void V170();
    virtual void V174();
    virtual void V178();
    virtual void V17c();
    virtual void V180();
    virtual void V184();
    virtual void UpdateRenderState();       // +0x188

    char pad04[0x6c - 0x04];
    Part* m_pRoot;                          // +0x6c
    char pad70[0x164 - 0x70];
    unsigned char m_bSingleTexture;         // +0x164
    unsigned char m_b165;                   // +0x165
    unsigned char m_bHasGeometry;           // +0x166
    unsigned char m_bDirty;                 // +0x167
    int m_n168;                             // +0x168
    char pad16c[0x17f - 0x16c];
    unsigned char m_bTransparent;           // +0x17f

    void ProcessGob();                      // 0x00444840
    void SetLightPriority(int nPriority);   // 0x00445e30
    void CollectLights(CExoArrayList<PartLight*>* pList);  // 0x00447180
    float GetMaximumLightRadius();          // 0x00447f00
    int RestoreTexture();                   // 0x00446660
    int SetTextureAttributes(char* szName, int nFlags);    // 0x00446e10
};

// FUNCTION: SWKOTOR 0x00444840
void Gob::ProcessGob()
{
    if (!m_bDirty)
        return;
    m_n168 = 0;
    m_bSingleTexture = TreeHasSingleTexture(m_pRoot);
    if (!m_bSingleTexture)
        m_n168 = 0;
    if (!m_n168)
        m_bSingleTexture = 0;
    m_bHasGeometry = TreeHasGeometry(m_pRoot);
    TreeSetGeometryFlags(m_pRoot);
    m_bTransparent = TreeHasTransparency(m_pRoot);
    UpdateRenderState();
    m_bDirty = 0;
}

// FUNCTION: SWKOTOR 0x00445e30
void Gob::SetLightPriority(int nPriority)
{
    CExoArrayList<PartLight*> lLights;
    findalllightparts(m_pRoot, &lLights);
    for (int i = 0; i < lLights.num; i++)
        lLights.element[i]->m_nPriority = nPriority;
}

// FUNCTION: SWKOTOR 0x00447f00
float Gob::GetMaximumLightRadius()
{
    float fMax = 0.0f;
    CExoArrayList<PartLight*> lLights;
    CollectLights(&lLights);
    for (int i = 0; i < lLights.num; i++) {
        if (lLights.element[i]->m_fRadius > fMax)
            fMax = lLights.element[i]->m_fRadius;
    }
    return fMax;
}

// FUNCTION: SWKOTOR 0x00446660
int Gob::RestoreTexture()
{
    CExoArrayList<PartTriMesh*> lParts;
    findalltrimeshparts(m_pRoot, &lParts);
    for (int i = 0; i < lParts.num; i++) {
        PartTriMesh* pPart = lParts.element[i];
        pPart->m_pMaterial->SetTexture(
            0, pPart->m_pNode->AsMdlNodeTriMesh()->m_szTexture0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00446e10
int Gob::SetTextureAttributes(char* szName, int nFlags)
{
    CExoArrayList<CAurTexture*> lTextures;
    GetAllNamedTextures(m_pRoot, &lTextures, szName);
    if (lTextures.num > 0) {
        for (int i = 0; i < lTextures.num; i++)
            AurTextureSetAttributes(lTextures.element[i], nFlags);
        return 1;
    }
    return 0;
}

// ------------------------------------------------------------------ Model
extern int g_bKeepModels;                   // 0x007a8a04

class MaxTree;
typedef void (*MaxTreeFn)(MaxTree*);

class MaxTree {
public:
    MaxTreeFn m_pfnDestroy;                 // +0x00
    char pad04[0x28 - 0x04];
    MdlNode* m_pRoot;                       // +0x28
    char pad2c[0x54 - 0x2c];
    int m_nRefCount;                        // +0x54

    static void operator delete(void* p);   // 0x0044aec0
};

class Animation : public MaxTree {
public:
    char pad58[0x7c - 0x58];
    int m_nKeys;                            // +0x7c
    char pad80[0x88 - 0x80];
};

void __cdecl MdlNodeRemoveAnimation(MdlNode* pRoot, int nIndex);   // 0x00437db0

extern CExoArrayList<Model*> g_lModels;     // 0x007a8a64
extern int g_nModelMemory;                  // 0x007a6990

class Model : public MaxTree {
public:
    CExoArrayList<Animation*> m_lAnimations; // +0x58
    Model* m_pSuperModel;                   // +0x64

    void ReleaseSuperModel();               // 0x0044b2c0
    void InternalDestructor();              // 0x00449f60
};

// FUNCTION: SWKOTOR 0x0044b2c0
void Model::ReleaseSuperModel()
{
    Model* pSuper = m_pSuperModel;
    if (pSuper && !g_bKeepModels) {
        if (--pSuper->m_nRefCount == 0)
            MaxTree::operator delete(pSuper);
    }
    m_pSuperModel = 0;
}

// FUNCTION: SWKOTOR 0x00449f60
void Model::InternalDestructor()
{
    if (g_lModels.Contains(this))
        g_lModels.Remove(this);
    Model* pSuper = m_pSuperModel;
    if (pSuper) {
        if (!g_bKeepModels && --pSuper->m_nRefCount == 0)
            MaxTree::operator delete(pSuper);
        m_pSuperModel = 0;
    }
    int i = m_lAnimations.num;
    g_nModelMemory += -i * (int)sizeof(Animation*);
    g_nModelMemory += m_lAnimations.num * -(int)sizeof(Animation);
    while (i) {
        Animation* pAnim = m_lAnimations.element[--i];
        g_nModelMemory += pAnim->m_nKeys * -0x24;
        MdlNodeRemoveAnimation(m_lAnimations.element[i]->m_pRoot, -1);
        m_lAnimations.element[i]->m_pfnDestroy(m_lAnimations.element[i]);
    }
    if (m_pRoot)
        m_pRoot->InternalDestructorDispatch();
}
