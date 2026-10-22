// Material (0x0047ABD0 .. 0x0047BA6F).
// Material::Init (0x0047B110) is matched in matching/src/pilot1/material.cpp.

#include <string.h>
#include <stdio.h>

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }

    void Allocate(int n);       // 0x005E03C0
    void Remove(T value) throw();   // 0x00445080
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
};

class CAurTextureRef;

class CAurTextureBits {
public:
    char pad_0x0[0x14];
    int m_bEnvironment;         // +0x14
};

class CAurTexture {
public:
    virtual void v00();
    virtual void v04();
    virtual CAurTextureBits* GetBits();     // +0x08
    virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18();
    virtual unsigned char IsLoaded();       // +0x1c
    virtual void v20(); virtual void v24();
    virtual unsigned char IsValid();        // +0x28
    virtual void v2c(); virtual void v30(); virtual void v34(); virtual void v38();
    virtual void v3c();
    virtual char* GetName();                // +0x40

    unsigned char IsProcedural();           // 0x00420600
    void SetIsEnvironmentMapped(int b);     // 0x00421480

    CExoArrayList<CAurTextureRef*> m_lRefs; // +0x04
};

class CAurTextureRef {
public:
    CAurTexture* m_p;

    CAurTextureRef() { m_p = 0; }
    ~CAurTextureRef() { if (m_p) m_p->m_lRefs.Remove(this); }
    CAurTextureRef& operator=(CAurTexture* p);  // 0x00447FA0
    void Clear()
    {
        if (m_p)
            m_p->m_lRefs.Remove(this);
        m_p = 0;
    }
};

void AurTextureRelease(CAurTexture* p);                 // 0x00420D70
CAurTexture* AurTextureGetReference(char* szName, int n);   // 0x00423A60
CAurTexture* AurTextureGetNULL();                       // 0x004230C0
int AurTextureReplaceSingleReference(CAurTexture* p, char* a1, char* a2, char* a3,
                                     int a4, int a5);   // 0x004235F0
void AurTextureSetLayered(CAurTexture* p, char* szName, int a4, int a5);   // 0x00421750

char* ParseKeyword(char* szLine);               // 0x00463530
void Parse_cStr(char* szLine, char* szOut);     // 0x00463590
void Parse_bool(char* szLine, unsigned char* pOut);   // 0x00463680
void Parse_int(char* szLine, int* pOut);        // 0x004635E0
void Parse_float(char* szLine, float* pOut);    // 0x004635C0

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd, const char* lpText,
                                                           const char* lpCaption,
                                                           unsigned int uType);

class MdlNodeData {
public:
    char pad_0x0[0xa8];
    char m_szBitmap[0x20];      // +0xa8
    char m_szBitmap2[0x08];     // +0xc8
    char m_szBitmap3[0x20];     // +0xd0
};

class MdlNodeHolder {
public:
    void* vtable;
    MdlNodeData* m_pData;       // +0x04
};

class MaterialSource {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual MdlNodeHolder* AsTriMesh();     // +0x2c
    virtual void v30();
    virtual MdlNodeHolder* AsOther();       // +0x34
};

class Material;
extern CExoArrayList<Material*> g_lMaterials;   // 0x00830298

static inline void ReleaseTexture(CAurTextureRef& r)
{
    CAurTexture* p = r.m_p;
    r.Clear();
    AurTextureRelease(p);
}

static inline bool IsTextureName(char* szName)
{
    return szName && *szName && strcmp("NULL", szName);
}

static inline CAurTexture* GetTextureOrNULL(char* szName)
{
    if (IsTextureName(szName))
        return AurTextureGetReference(szName, 0);
    return AurTextureGetNULL();
}

static inline void ReplaceReference(CAurTextureRef& r, char* szName, int n)
{
    CAurTexture* pOld = r.m_p;
    CAurTexture* pNew = AurTextureGetReference(szName, n);
    r = pNew;
    AurTextureRelease(pOld);
}

class Material {
public:
    CAurTextureRef m_tTexture;      // +0x00
    CAurTextureRef m_tTexture1;     // +0x04
    CAurTextureRef m_tTexture2;     // +0x08
    CAurTextureRef m_tTexture3;     // +0x0c
    CAurTextureRef m_tBumpMap;      // +0x10
    CAurTextureRef m_tEnvMap;       // +0x14
    CAurTextureRef m_tTexture6;     // +0x18
    char* m_szBumpMap;              // +0x1c
    char* m_szBumpyShiny;           // +0x20
    char* m_szEnvMap;               // +0x24
    CExoArrayList<int> m_lSrcBlend; // +0x28
    CExoArrayList<int> m_lDstBlend; // +0x34
    unsigned char m_bDecal;         // +0x40
    int m_nRenderType;              // +0x44
    float m_fWaterAlpha;            // +0x48

    Material();
    ~Material();
    void ParseField(char* szLine);
    void Init(char* szName);        // 0x0047B110
    void ClearTextures();
    void InitializeTextures(char* szTex0, char* szTex1, char* szTex2, char* szTex3);
    void LoadTextures(MaterialSource* pSource);
    void ReplaceTexture(char* a1, char* szName, char* szReplace, int a4, int a5);
    void ReplaceTexture(char* szName, int n);
    bool HasBumpMap();
};

// FUNCTION: SWKOTOR 0x0047ABD0
void Material::ParseField(char* szLine)
{
    char szBlending[32];
    char szBumpMap[32];
    char szBumpyShiny[32];
    char szEnvMap[32];

    szBumpMap[0] = 0;
    szBumpyShiny[0] = 0;
    szEnvMap[0] = 0;
    szBlending[0] = 0;

    if (!_stricmp("bumpmaptexture", ParseKeyword(szLine)))
        Parse_cStr(szLine + 14, szBumpMap);
    if (!_stricmp("bumpyshinytexture", ParseKeyword(szLine)))
        Parse_cStr(szLine + 17, szBumpyShiny);
    if (!_stricmp("envmaptexture", ParseKeyword(szLine)))
        Parse_cStr(szLine + 13, szEnvMap);
    if (!_stricmp("blending", ParseKeyword(szLine)))
        Parse_cStr(szLine + 8, szBlending);
    if (!_stricmp("decal", ParseKeyword(szLine)))
        Parse_bool(szLine + 5, &m_bDecal);
    if (!_stricmp("renderbmlmtype", ParseKeyword(szLine)))
        Parse_int(szLine + 14, &m_nRenderType);
    if (!_stricmp("wateralpha", ParseKeyword(szLine)))
        Parse_float(szLine + 10, &m_fWaterAlpha);

    if (szBumpMap[0]) {
        m_szBumpMap = new char[32];
        strcpy(m_szBumpMap, szBumpMap);
    }
    if (szBumpyShiny[0]) {
        m_szBumpyShiny = new char[32];
        strcpy(m_szBumpyShiny, szBumpyShiny);
    }
    if (szEnvMap[0]) {
        m_szEnvMap = new char[32];
        strcpy(m_szEnvMap, szEnvMap);
    }
    if (szBlending[0]) {
        if (!strcmp(szBlending, "additive")) {
            m_lSrcBlend.element[0] = 0;
            m_lDstBlend.element[0] = 2;
        }
        else if (!strcmp(szBlending, "punchthrough")) {
            m_lSrcBlend.element[0] = 2;
            m_lDstBlend.element[0] = 3;
        }
    }
}

// FUNCTION: SWKOTOR 0x0047B290
Material::Material()
{
    m_szBumpMap = 0;
    m_szEnvMap = 0;
    m_szBumpyShiny = 0;
    m_tTexture.Clear();
    m_tTexture1.Clear();
    m_tTexture2.Clear();
    m_tTexture3.Clear();
    m_tBumpMap.Clear();
    m_tTexture6.Clear();
    m_lSrcBlend.Allocate(2);
    m_lDstBlend.Allocate(2);
    m_lSrcBlend.Add(0);
    m_lDstBlend.Add(1);
    m_bDecal = 0;
    m_nRenderType = 1;
    g_lMaterials.Add(this);
    m_fWaterAlpha = 1.0f;
}

// NOTE: NONMATCH - same shape; original keeps this in edi and pushes ebx after slot 0.
// FUNCTION: SWKOTOR 0x0047B440
void Material::ClearTextures()
{
    if (m_tTexture.m_p)
        ReleaseTexture(m_tTexture);
    if (m_tTexture1.m_p)
        ReleaseTexture(m_tTexture1);
    if (m_tTexture2.m_p)
        ReleaseTexture(m_tTexture2);
    if (m_tTexture3.m_p)
        ReleaseTexture(m_tTexture3);
    if (m_tBumpMap.m_p)
        ReleaseTexture(m_tBumpMap);
    if (m_tEnvMap.m_p)
        ReleaseTexture(m_tEnvMap);
    if (m_tTexture6.m_p)
        ReleaseTexture(m_tTexture6);
}

// FUNCTION: SWKOTOR 0x0047B560
void Material::InitializeTextures(char* szTex0, char* szTex1, char* szTex2, char* szTex3)
{
    m_tTexture = GetTextureOrNULL(szTex0);
    if (IsTextureName(szTex0))
        Init(szTex0);
    m_tTexture1 = GetTextureOrNULL(szTex1);
    m_tTexture2 = GetTextureOrNULL(szTex2);
    m_tTexture3 = GetTextureOrNULL(szTex3);

    if (m_szBumpMap) {
        if (*m_szBumpMap && strcmp("NULL", m_szBumpMap))
            m_tBumpMap = AurTextureGetReference(m_szBumpMap, 0);
        delete[] m_szBumpMap;
        m_szBumpMap = 0;
    }
    if (m_szBumpyShiny) {
        if (*m_szBumpyShiny && strcmp("NULL", m_szBumpyShiny)) {
            m_tEnvMap = AurTextureGetReference(m_szBumpyShiny, 0);
            m_tEnvMap.m_p->GetBits()->m_bEnvironment = 1;
        }
        delete[] m_szBumpyShiny;
        m_szBumpyShiny = 0;
    }
    if (m_szEnvMap) {
        if (*m_szEnvMap && strcmp("NULL", m_szEnvMap)) {
            m_tEnvMap = AurTextureGetReference(m_szEnvMap, 0);
            m_tEnvMap.m_p->GetBits()->m_bEnvironment = 1;
        }
        m_tTexture.m_p->SetIsEnvironmentMapped(1);
        delete[] m_szEnvMap;
        m_szEnvMap = 0;
    }
}

// FUNCTION: SWKOTOR 0x0047B770
void Material::LoadTextures(MaterialSource* pSource)
{
    char* szTex0;
    char* szTex1 = 0;
    MdlNodeHolder* pMesh = pSource->AsTriMesh();
    MdlNodeHolder* pOther = pSource->AsOther();
    if (pMesh) {
        szTex0 = pMesh->m_pData->m_szBitmap;
        szTex1 = pMesh->m_pData->m_szBitmap2;
    }
    else if (pOther) {
        szTex0 = pOther->m_pData->m_szBitmap3;
    }
    else {
        return;
    }
    InitializeTextures(szTex0, szTex1, 0, 0);
}

// FUNCTION: SWKOTOR 0x0047B7E0 SYMBOL: ?ReplaceTexture@Material@@QAEXPAD00HH@Z
void Material::ReplaceTexture(char* a1, char* szName, char* szReplace, int a4, int a5)
{
    if (m_tTexture.m_p->IsProcedural()) {
        if (!szReplace)
            return;
        ReplaceReference(m_tTexture, szReplace, 0);
    }
    if (szName && strlen(szName)) {
        CAurTexture* pTex = m_tTexture.m_p;
        if (AurTextureReplaceSingleReference(pTex, a1, szName, szName, a4, a5))
            AurTextureSetLayered(m_tTexture.m_p, szName, a4, a5);
    }
    else {
        AurTextureSetLayered(m_tTexture.m_p, szName, a4, a5);
    }
}

// FUNCTION: SWKOTOR 0x0047B890 SYMBOL: ?ReplaceTexture@Material@@QAEXPADH@Z
void Material::ReplaceTexture(char* szName, int n)
{
    if (m_tTexture.m_p && !m_tTexture.m_p->IsProcedural()) {
        AurTextureReplaceSingleReference(m_tTexture.m_p, szName, (char*)n, 0, 0, 0);
        return;
    }
    if (n) {
        CAurTexture* pOld = m_tTexture.m_p;
        m_tTexture = AurTextureGetReference(szName, n);
        AurTextureRelease(pOld);
    }
}

// NOTE: NONMATCH - original's release keeps a redundant copy (mov eax,edi / test eax,eax).
// FUNCTION: SWKOTOR 0x0047B8F0
bool Material::HasBumpMap()
{
    if (m_tBumpMap.m_p) {
        if (!m_tBumpMap.m_p->IsValid() || m_tBumpMap.m_p->IsLoaded())
            return true;
        char szMessage[256];
        sprintf(szMessage, "Invalid bumpmap: %s.tga", m_tBumpMap.m_p->GetName());
        MessageBoxA(0, szMessage, "Invalid Bumpmap!", 0x1030);
        ReleaseTexture(m_tBumpMap);
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0047B980
Material::~Material()
{
    ClearTextures();
    g_lMaterials.Remove(this);
}
