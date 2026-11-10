// Per-light bump-mapped PartTriMesh render paths (0x00474EA0-0x00477050).
// Names come from the PartTriMesh::Draw dispatch in renderdraw.cpp.

extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDisable(unsigned int);
extern "C" __declspec(dllimport) unsigned char __stdcall glIsEnabled(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int, unsigned int);
extern "C" __declspec(dllimport) void __stdcall glEnableClientState(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDisableClientState(unsigned int);

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ) { z = fZ; y = fY; x = fX; w = fW; }
};

Quaternion operator-(const Quaternion& q);                  // 0x004A9B50
Vector operator*(const Quaternion& q, const Vector& v);     // 0x004A9BB0
Vector operator*(const Vector& a, const Vector& b);         // 0x004AB1E0
Vector operator/(const Vector& v, float f);                 // 0x00437A90
float Magnitude(const Vector& v);                           // 0x004AB100

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(const CExoArrayList<T>& other);   // 0x0046D250
    ~CExoArrayList() { if (element) delete[] element; }
};

class CAurTexture {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual float GetSpecularIntensity();       // +0x80, descriptive
    virtual void v84();
    virtual void GetSpecularColor(Vector* pColor);  // +0x88, descriptive
    __forceinline Vector SpecularColor() { Vector v; GetSpecularColor(&v); return v; }
    virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0();
    virtual void Bind();                        // +0xb4
};

class Material {
public:
    CAurTexture* m_pTexture0;       // +0x00
    CAurTexture* m_pTexture1;       // +0x04
    char m_pad08[0x10 - 0x08];
    CAurTexture* m_pBumpMap;        // +0x10

    int IsBumpMapBumpTexture();     // 0x0047AF50
};

class MdlNodeTriMesh {
public:
    char m_pad00[0x8c];
    Vector m_vBumpScale;            // +0x8c
};

class TriMeshPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void GetWorldTransform(Vector* pPos, Quaternion* pOri);    // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual TriMeshPart* GetTriMesh();          // +0x2c

    MdlNodeTriMesh* m_pNode;        // +0x04
    char m_pad08[0x44 - 0x08];
    Material* m_pMaterial;          // +0x44
};

class PartTriMesh {
public:
    char m_pad00[8];
    TriMeshPart* m_pObject;         // +0x08
};

class Light {
public:
    char m_pad00[0x4c];
    Vector m_vColor;                // +0x4c
    float m_fRadius;                // +0x58
    char m_pad5C[0x94 - 0x5c];
    Vector m_vPosition;             // +0x94
};

class LightManager {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual void DisableBumpMapLights();        // +0x2c
    virtual void EnableBumpMapLights();         // +0x30

    void* GetBumpMapLights();                   // 0x004A28A0
};

class Scene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4(); virtual void vb8(); virtual void vbc();
    virtual void vc0(); virtual void vc4(); virtual void vc8(); virtual void vcc();
    virtual void vd0(); virtual void vd4(); virtual void vd8(); virtual void vdc();
    virtual void ve0(); virtual void ve4(); virtual void ve8(); virtual void vec();
    virtual void vf0(); virtual void vf4(); virtual void vf8(); virtual void vfc();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual void v120();
    virtual void DoFog();                       // +0x124

    char m_pad04[0x60 - 0x04];
    int m_bFogEnabled;              // +0x60
    float m_fFogStart;              // +0x64
    float m_fFogEnd;                // +0x68
    char m_pad6C[0x80 - 0x6c];
    LightManager* m_pLights;        // +0x80
    char m_pad84[0xa4 - 0x84];
    CAurTexture* m_pTexture;        // +0xa4
};

class RenderCamera {
public:
    char m_pad00[0x7c];
    Vector m_vPosition;             // +0x7c
};

class VertexProgram {
public:
    VertexProgram(const char* szProgram, int nContext);    // 0x004A27D0
    void Bind(void* pMesh, int bLightmap);                  // 0x004A24D0

    char m_data[0x201c];
};

void RenderFlat(PartTriMesh* pPart);            // 0x0046F900
void RenderLightMapped(PartTriMesh* pPart);     // 0x0046FA40
int AurPixelShadersAvailable();                 // 0x0046EDA0
int AurGLCaps710();                             // 0x0045F710
void SetCombinerConstant(int nIndex, float r, float g, float b, float a);   // 0x0044F820
namespace GLRender {
void ActivateTextureStage(int nStage);          // 0x00425250
}

typedef void (__cdecl* PFNCOMBINER)(unsigned int, CAurTexture*, CAurTexture*, CAurTexture*, CAurTexture*);
extern PFNCOMBINER g_pfnCombiner;               // 0x007A47E0
typedef void (__cdecl* PFNCOMBINERCOLOR)(Vector*, float);
extern PFNCOMBINERCOLOR g_pfnCombinerColor;     // 0x007A47DC, descriptive
extern PFNCOMBINERCOLOR g_pfnCombinerConst;     // 0x007A6820
typedef void (__stdcall* PFNACTIVETEXTURE)(unsigned int);
extern PFNACTIVETEXTURE glClientActiveTextureARB;     // 0x007BB6B0
typedef void (__stdcall* PFNPROGRAMPARAMETER)(unsigned int, unsigned int, float, float, float, float);
extern PFNPROGRAMPARAMETER g_pfnProgramEnvParameter4fARB;  // 0x007BB850
extern PFNPROGRAMPARAMETER g_pfnProgramParameter4fNV;      // 0x007BB7AC

extern Scene* g_pScene;                         // 0x007B92A0
extern RenderCamera* g_pRenderCamera;           // 0x007BB4E8
extern int g_bARBVertexProgram;                 // 0x0078E5EC
extern unsigned int g_aBlendFactors[];          // 0x0073F270
extern unsigned int g_eGLTexture2D;             // 0x0078D44C
extern unsigned int g_eGLFog;                   // 0x0078D450
extern unsigned int g_eGLTexture3D;             // 0x0078D46C
extern unsigned int g_eTexCoordArray;           // 0x0073F398
extern unsigned int g_eAttribArray5;            // 0x0073F3B4, descriptive
extern unsigned int g_eTangentArray;            // 0x0073F3B8
extern unsigned int g_eBinormalArray;           // 0x0073F3BC
extern unsigned int g_eTexNormalArray;          // 0x0073F3C0
extern unsigned int g_nActiveTextureUnit;       // 0x007A6898
extern float g_fBumpLightScale;                 // 0x0078E648, descriptive
extern char g_szBumpDiffuseVP[];                // 0x0079A600, descriptive
extern int g_nBumpDiffuseVPContext;             // 0x00830558, descriptive
extern char g_szBumpSpecularVP[];               // 0x0079A218, descriptive
extern int g_nBumpSpecularVPContext;            // 0x00830554, descriptive
extern int g_bBumpSpecularMagnitude;            // 0x0078E5F4, descriptive
extern int g_nBumpSpecularMode;                 // 0x0078E634, descriptive
extern float g_fBumpSpecularScale;              // 0x0078E630, descriptive
extern const float g_fZero;                     // 0x0073D700

static inline void SetVertexProgramParameter(unsigned int nIndex, float x, float y, float z, float w)
{
    if (g_bARBVertexProgram)
        g_pfnProgramEnvParameter4fARB(0x8620, nIndex, x, y, z, w);
    else
        g_pfnProgramParameter4fNV(0x8620, nIndex, x, y, z, w);
}

static inline void SetVertexProgramParameter(unsigned int nIndex, const Vector& v, float w)
{
    SetVertexProgramParameter(nIndex, v.x, v.y, v.z, w);
}

static inline void Combiner(unsigned int nMode, CAurTexture* a, CAurTexture* b, CAurTexture* c,
                            CAurTexture* d)
{
    g_pfnCombiner(nMode, a, b, c, d);
}

// FUNCTION: SWKOTOR 0x00474EA0
void RenderLightMappedBumpDiffuse(PartTriMesh* pPart)
{
    if (!g_pScene) {
        RenderFlat(pPart);
        return;
    }
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    CExoArrayList<Light*> lLights(*(CExoArrayList<Light*>*)g_pScene->m_pLights->GetBumpMapLights());
    if (lLights.num == 0) {
        RenderLightMapped(pPart);
    } else if (AurPixelShadersAvailable()) {
        glDisable(g_eGLFog);
        TriMeshPart* pMesh = pPart->m_pObject->GetTriMesh();
        MdlNodeTriMesh* pNode = 0;
        if (pMesh)
            pNode = pMesh->m_pNode;
        Vector vScale(0.0f, 0.0f, 0.0f);
        static VertexProgram* s_pProgram = new VertexProgram(g_szBumpDiffuseVP, g_nBumpDiffuseVPContext);
        glEnable(g_eGLTexture3D);
        if (pNode)
            vScale = pNode->m_vBumpScale;
        int bAlphaTest = glIsEnabled(g_eGLTexture2D);
        glDisable(g_eGLTexture2D);
        GLRender::ActivateTextureStage(1);
        glEnableClientState(g_eTexCoordArray);
        glEnableClientState(g_eTangentArray);
        glEnableClientState(g_eBinormalArray);
        glEnableClientState(g_eTexNormalArray);
        g_pfnCombiner(8, pMesh->m_pMaterial->m_pBumpMap, g_pScene->m_pTexture,
                      pMesh->m_pMaterial->m_pTexture1, pMesh->m_pMaterial->m_pTexture0);
        SetCombinerConstant(0x10, g_pScene->m_fFogStart, g_pScene->m_fFogEnd,
                            g_pScene->m_bFogEnabled ? 1.0f : 0.0f, 0.5f);
        SetCombinerConstant(0x11, 1.0f, -1.0f, 0.0f, 0.0f);
        Quaternion qOri(1.0f, 0.0f, 0.0f, 0.0f);
        Vector vPos(0.0f, 0.0f, 0.0f);
        pMesh->GetWorldTransform(&vPos, &qOri);
        Vector vEye = -qOri * (g_pRenderCamera->m_vPosition - vPos);
        for (int i = 0; i < lLights.num; i++) {
            g_pScene->m_pLights->DisableBumpMapLights();
            s_pProgram->Bind(pMesh, 1);
            Vector vLightPos = lLights.element[i]->m_vPosition;
            Vector vLight;
            vLight = -qOri * (vLightPos - vPos);
            const Vector& vLightColor = lLights.element[i]->m_vColor;
            Vector vColor = vLightColor * (vScale * g_fBumpLightScale);
            g_pfnCombinerColor(&vColor, 1.0f);
            SetVertexProgramParameter(0xc, vLight, 1.0f);
            SetVertexProgramParameter(0xd, vEye, 1.0f);
            float fRadius = lLights.element[i]->m_fRadius;
            SetVertexProgramParameter(0xe, 1.0f, 0.0f, 9.0f / (fRadius * fRadius * 2.0f), 1.0f);
            RenderFlat(pPart);
            if (bAlphaTest)
                glEnable(g_eGLTexture2D);
            g_pScene->m_pLights->EnableBumpMapLights();
        }
        if (AurGLCaps710())
            glClientActiveTextureARB(0x84c1);
        g_nActiveTextureUnit = 1;
        glDisableClientState(g_eTexCoordArray);
        if (AurGLCaps710())
            glClientActiveTextureARB(0x84c0);
        g_nActiveTextureUnit = 0;
        glDisableClientState(g_eTangentArray);
        glDisableClientState(g_eBinormalArray);
        glDisableClientState(g_eTexNormalArray);
        g_pScene->DoFog();
        g_pfnCombiner(0x20, 0, 0, 0, 0);
        g_pfnCombiner(0xd, 0, 0, 0, 0);
        g_pfnCombiner(0, 0, 0, 0, 0);
        glDisable(g_eGLTexture3D);
        pMesh->m_pMaterial->m_pTexture0->Bind();
    }
}

// FUNCTION: SWKOTOR 0x00475450
void RenderLightMappedBumpSpecular(PartTriMesh* pPart)
{
    if (!g_pScene) {
        RenderFlat(pPart);
        return;
    }
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    CExoArrayList<Light*> lLights(*(CExoArrayList<Light*>*)g_pScene->m_pLights->GetBumpMapLights());
    if (lLights.num == 0) {
        RenderLightMapped(pPart);
    } else {
        int bAlphaTest = glIsEnabled(g_eGLTexture2D);
        glDisable(g_eGLTexture2D);
        if (AurPixelShadersAvailable()) {
            TriMeshPart* pMesh = pPart->m_pObject->GetTriMesh();
            glDisable(g_eGLFog);
            MdlNodeTriMesh* pNode = 0;
            if (pMesh)
                pNode = pMesh->m_pNode;
            Vector vSpecular(0.0f, 0.0f, 0.0f);
            Vector vScale(0.0f, 0.0f, 0.0f);
            static VertexProgram* s_pProgram = new VertexProgram(g_szBumpSpecularVP, g_nBumpSpecularVPContext);
            glEnable(g_eGLTexture3D);
            if (pNode) {
                vSpecular = pMesh->m_pMaterial->m_pBumpMap->SpecularColor();
                vScale = pNode->m_vBumpScale;
                float fScale = Magnitude(vScale);
                vScale = vScale * fScale /
                         ((g_bBumpSpecularMagnitude && pMesh->m_pMaterial->IsBumpMapBumpTexture()
                               ? Magnitude(vSpecular) : g_fZero) + fScale);
            }
            bAlphaTest = glIsEnabled(g_eGLTexture2D);
            glDisable(g_eGLTexture2D);
            GLRender::ActivateTextureStage(1);
            glEnableClientState(g_eTexCoordArray);
            glEnableClientState(g_eTangentArray);
            glEnableClientState(g_eBinormalArray);
            glEnableClientState(g_eTexNormalArray);
            if (g_nBumpSpecularMode == 1)
                Combiner(9, pMesh->m_pMaterial->m_pBumpMap, g_pScene->m_pTexture,
                         pMesh->m_pMaterial->m_pTexture1, pMesh->m_pMaterial->m_pTexture0);
            else if (g_nBumpSpecularMode == 2)
                Combiner(0xa, pMesh->m_pMaterial->m_pBumpMap, g_pScene->m_pTexture,
                         pMesh->m_pMaterial->m_pTexture1, pMesh->m_pMaterial->m_pTexture0);
            else if (g_nBumpSpecularMode == 4)
                Combiner(0xb, pMesh->m_pMaterial->m_pBumpMap, g_pScene->m_pTexture,
                         pMesh->m_pMaterial->m_pTexture1, pMesh->m_pMaterial->m_pTexture0);
            else if (g_nBumpSpecularMode == 8)
                Combiner(0xc, pMesh->m_pMaterial->m_pBumpMap, g_pScene->m_pTexture,
                         pMesh->m_pMaterial->m_pTexture1, pMesh->m_pMaterial->m_pTexture0);
            SetCombinerConstant(0x10, g_pScene->m_fFogStart, g_pScene->m_fFogEnd,
                                g_pScene->m_bFogEnabled ? 1.0f : 0.0f, 0.5f);
            SetCombinerConstant(0x11, 1.0f, -1.0f, 0.0f, 0.0f);
            Quaternion qOri(1.0f, 0.0f, 0.0f, 0.0f);
            Vector vPos(0.0f, 0.0f, 0.0f);
            pMesh->GetWorldTransform(&vPos, &qOri);
            Vector vEye = -qOri * (g_pRenderCamera->m_vPosition - vPos);
            for (int i = 0; i < lLights.num; i++) {
                s_pProgram->Bind(pMesh, 1);
                Vector vLightPos = lLights.element[i]->m_vPosition;
                Vector vLight;
                vLight = -qOri * (vLightPos - vPos);
                Vector vTint = vSpecular * g_fBumpSpecularScale *
                               pMesh->m_pMaterial->m_pBumpMap->GetSpecularIntensity();
                const Vector& vLightColor = lLights.element[i]->m_vColor;
                Vector vColor = vLightColor * vTint;
                g_pfnCombinerConst(&vColor, 1.0f);
                SetVertexProgramParameter(0xc, vLight, 1.0f);
                SetVertexProgramParameter(0xd, vEye, 1.0f);
                float fRadius = lLights.element[i]->m_fRadius;
                SetVertexProgramParameter(0xe, 1.0f, 0.0f, 9.0f / (fRadius * fRadius * 2.0f), 1.0f);
                RenderFlat(pPart);
                if (bAlphaTest)
                    glEnable(g_eGLTexture2D);
            }
            glDisableClientState(g_eAttribArray5);
            glDisableClientState(g_eTangentArray);
            glDisableClientState(g_eBinormalArray);
            glDisableClientState(g_eTexNormalArray);
            if (AurGLCaps710())
                glClientActiveTextureARB(0x84c1);
            g_nActiveTextureUnit = 1;
            glDisableClientState(g_eTexCoordArray);
            if (AurGLCaps710())
                glClientActiveTextureARB(0x84c0);
            g_nActiveTextureUnit = 0;
            g_pScene->DoFog();
            g_pfnCombiner(0x20, 0, 0, 0, 0);
            g_pfnCombiner(0xd, 0, 0, 0, 0);
            g_pfnCombiner(0, 0, 0, 0, 0);
            glDisable(g_eGLTexture3D);
        }
    }
}
