// Gob texture replacement virtuals (vtable 0x741268 +0x34, +0x38,
// +0x48) that walk the part tree's materials.
#include "g/exo_list.h"

extern "C" char* __cdecl strcpy(char* d, const char* s);
#pragma intrinsic(strcpy)

struct _iobuf;
extern "C" int __cdecl fprintf(_iobuf* f, const char* fmt, ...);  // 0x006faf01
extern _iobuf* g_pTextureLog;               // 0x007a8a3c

class CAurTexture {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual unsigned char IsNull();         // +0x10

    void SetIsEnvironmentMapped(int b);     // 0x00421480
};

CAurTexture* __cdecl AurTextureGetReference(char* szName, int n);  // 0x00423a60
void __cdecl AurTextureRelease(CAurTexture* p);                    // 0x00420d70

class SafePointerTexture {
public:
    CAurTexture* m_p;

    SafePointerTexture& operator=(CAurTexture* p);                  // 0x00447fa0
};

class Material {
public:
    CAurTexture* m_pTexture;                // +0x00

    void ReplaceTexture(char* szName, int nStage);                  // 0x0047b890
    void ReplaceTexture(char* szOld, char* szNew, char* szPart, int* pIndices,
                        int nIndices);                              // 0x0047b7e0
};

class Part;

void __cdecl findallmaterials(Part* pRoot, CExoArrayList<Material*>* pList);  // 0x00445ec0
Part* __cdecl FindPart(Part* pRoot, char* szName);                  // 0x0043fe00

class Gob {
public:
    char pad00[0x6c];
    Part* m_pRoot;                          // +0x6c
    char pad70[0x167 - 0x70];
    unsigned char m_bDirty;                 // +0x167
    char pad168[0x1a8 - 0x168];
    SafePointerTexture m_pEnvironment;      // +0x1a8

    int ReplaceAllTextures(char* szName, int nStage);
    int ReplacePartTexture(char* szPart, char* szNew, int nIndices,
                           unsigned short* pIndices);
    int SetEnvironmentMap(char* szName);
    void SetEnvironmentTexture(CAurTexture* pTexture)
    {
        CAurTexture* pOld = m_pEnvironment.m_p;
        m_pEnvironment = pTexture;
        AurTextureRelease(pOld);
    }
};

// FUNCTION: SWKOTOR 0x00446520
// NONMATCH (21): the original zeroes ebx in the empty-name branch and esi in
// the other; ours hoists one `xor ebx, ebx` above the name test (tried `!*s`,
// `s[0] == 0`).
int Gob::ReplaceAllTextures(char* szName, int nStage)
{
    if (szName[0] == '\0') {
        CExoArrayList<Material*> lMaterials;
        findallmaterials(m_pRoot, &lMaterials);
        for (int i = 0; i < lMaterials.num; i++)
            lMaterials.element[i]->ReplaceTexture(0, nStage);
        return 0;
    }
    CExoArrayList<Material*> lMaterials;
    findallmaterials(m_pRoot, &lMaterials);
    for (int i = 0; i < lMaterials.num; i++) {
        lMaterials.element[i]->ReplaceTexture(szName, nStage);
        if (m_pEnvironment.m_p)
            lMaterials.element[i]->m_pTexture->SetIsEnvironmentMapped(1);
    }
    m_bDirty = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00446910
int Gob::ReplacePartTexture(char* szPart, char* szNew, int nIndices,
                            unsigned short* pIndices)
{
    int i;
    if (g_pTextureLog) {
        fprintf(g_pTextureLog, "%s\n", szNew);
        for (i = 0; i < nIndices; i++)
            fprintf(g_pTextureLog, "%i ", pIndices[i]);
        fprintf(g_pTextureLog, "\n");
    }
    int anIndices[256];
    for (i = 0; i < nIndices; i++)
        anIndices[i] = pIndices[i];
    CExoArrayList<Material*> lMaterials;
    Part* pPart = FindPart(m_pRoot, szPart);
    if (!pPart)
        return 0;
    findallmaterials(pPart, &lMaterials);
    for (i = 0; i < lMaterials.num; i++) {
        lMaterials.element[i]->ReplaceTexture(0, szNew, szPart, anIndices, nIndices);
        if (m_pEnvironment.m_p)
            lMaterials.element[i]->m_pTexture->SetIsEnvironmentMapped(1);
    }
    m_bDirty = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00448690
// NONMATCH (18): ebp / edi swapped between the new and the old texture, and
// the original reloads the environment pointer into esi (tried a function
// scope pOld and an inline setter).
int Gob::SetEnvironmentMap(char* szName)
{
    char szBuf[64];
    if (szName && *szName)
        strcpy(szBuf, szName);
    else
        strcpy(szBuf, "NULL");
    CAurTexture* pTexture = AurTextureGetReference(szBuf, 0);
    if (pTexture->IsNull()) {
        AurTextureRelease(pTexture);
        return 0;
    }
    SetEnvironmentTexture(pTexture);
    if (m_pEnvironment.m_p) {
        CExoArrayList<Material*> lMaterials;
        findallmaterials(m_pRoot, &lMaterials);
        for (int i = 0; i < lMaterials.num; i++)
            lMaterials.element[i]->m_pTexture->SetIsEnvironmentMapped(1);
    }
    return 1;
}
