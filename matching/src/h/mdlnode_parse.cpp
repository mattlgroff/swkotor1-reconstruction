// MdlNode base-class field parser of the ASCII model reader
// (0x00465560) and the light and trimesh node overrides.

#include "h/texctl.h"

extern "C" int __cdecl strncmp(const char*, const char*, unsigned int);

extern unsigned char g_bBinaryModel;        // 0x007B927C

#define MDL_FIELD(name, stmt) \
    if (!_stricmp(name, GetWord(s))) \
        stmt

struct NewController;
class MdlNode;

// Controller builders (cdecl); the first argument is the field keyword.
int AddGenericControllerSingle(char* szName, char* szText,
                               CExoArrayList<NewController>* pControllers,
                               CExoArrayList<float>* pData, int nType, short nRow);  // 0x00464E40
void AddGenericController(char* szName, char* szText,
                          CExoArrayList<NewController>* pControllers,
                          CExoArrayList<float>* pData, int nType, short nRow);       // 0x00465090
void AddBezierController(char* szName, char* szText,
                         CExoArrayList<NewController>* pControllers,
                         CExoArrayList<float>* pData, int nType, short nRow);        // 0x00465320
void ParseQuaternion(char* s, Quaternion* pOut);                                     // 0x00463A30
MdlNode* FindMdlNode(MdlNode* pRoot, char* szName);                                  // 0x00464AF0
void ParseStringList(char* s, CExoArrayList<char*>* pList);                          // 0x00464880
void ParseVectorList(char* s, CExoArrayList<Vector>* pList);                         // 0x00421FF0
void ParseList(char* s, CExoArrayList<int>* pList);                                  // 0x00464630
void ParseUIntPtrList(char* s, CExoArrayList<unsigned int*>* pList);                 // 0x00464A40
struct MaxFace_PostProcess;
void Parse(char* s, CExoArrayList<MaxFace_PostProcess>* pList);                      // 0x00468A00
void Parse(char* s, CExoArrayList<unsigned int>* pList);                             // 0x00468B90
class AurRes;
AurRes* AurResGet(const char* szName, const char* szExt, int* pSize, char bText);    // 0x0044C740
void AurResClose(AurRes* pRes, int n);                                               // 0x0044C8F0
char* AurResGetNextLine();                                                           // 0x0044BFA0

// ParseVector 0x00463570, expanded in this TU.
inline void ParseVectorInline(char* s, float* pOut)
{
    sscanf(s, "%f %f %f", pOut, pOut + 1, pOut + 2);
}

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

// ParseString 0x00463590, expanded in this TU.
inline void ParseStringInline(char* s, char* pOut)
{
    int n = sscanf(s, "%s", pOut);
    if (n == 0)
        strcpy(pOut, (char*)n);
}

// Parse a keyword's single value, key list or bezier key list.
#define MDL_CONTROLLER(name, len, type, row, keyrow) \
    if (!_stricmp(name, GetWord(s))) \
        AddGenericControllerSingle(GetWord(s), s + len, &m_lControllers, &m_lControllerData, type, row); \
    else if (!_stricmp(name "key", GetWord(s))) \
        AddGenericController(GetWord(s), s + len + 3, &m_lControllers, &m_lControllerData, type, keyrow); \
    else if (!_stricmp(name "bezierkey", GetWord(s))) \
        AddBezierController(GetWord(s), s + len + 9, &m_lControllers, &m_lControllerData, type, keyrow);

class MaxTree {
public:
    char pad_0x0[0x28];
    MdlNode* m_pRoot;                           // +0x28
};

class MdlNode {
public:
    virtual void InternalParseField(char* s);
    char pad_0x4[4];
    MaxTree* m_pTree;                           // +0x08
    MdlNode* m_pParent;                         // +0x0c
    Vector m_vPosition;                         // +0x10
    Quaternion m_qOrientation;                  // +0x1c
    CExoArrayList<MdlNode*> m_lChildren;        // +0x2c
    CExoArrayList<NewController> m_lControllers;    // +0x38
    CExoArrayList<float> m_lControllerData;     // +0x44
};

// FUNCTION: SWKOTOR 0x00465560
void MdlNode::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_FIELD("position", ParseVectorInline(s + 8, (float*)&m_vPosition));
        MDL_FIELD("orientation", ParseQuaternion(s + 11, &m_qOrientation));
        MDL_CONTROLLER("position", 8, 8, -1, 0x10)
        MDL_CONTROLLER("orientation", 11, 0x14, -1, 0x1c)
        Vector vWireColor(0.0f, 0.0f, 0.0f);
        MDL_FIELD("wirecolor", ParseVectorInline(s + 9, (float*)&vWireColor));
        MDL_CONTROLLER("scale", 5, 0x24, -1, -1)
    }
    if (!strncmp("parent", s, 6)) {
        char szName[64];
        sscanf(s + 6, "%s", szName);
        if (!_stricmp(szName, "NULL")) {
            m_pParent = 0;
            m_pTree->m_pRoot = this;
            return;
        }
        MaxTree* pTree = m_pTree;
        MdlNode* pParent = (MdlNode*)pTree;
        if (pTree)
            pParent = FindMdlNode(pTree->m_pRoot, szName);
        m_pParent = pParent;
        if (pParent)
            m_pParent->m_lChildren.AddCall(this);
    }
}

class MdlNodeLight : public MdlNode {
public:
    float m_fFlareRadius;                       // +0x50
    char pad_0x54[0xc];
    CExoArrayList<float> m_lFlareSizes;         // +0x60
    CExoArrayList<float> m_lFlarePositions;     // +0x6c
    CExoArrayList<Vector> m_lFlareColorShifts;  // +0x78
    CExoArrayList<char*> m_lTextureNames;       // +0x84
    int m_nLightPriority;                       // +0x90
    int m_bAmbientOnly;                         // +0x94
    int m_nDynamicType;                         // +0x98
    int m_bAffectDynamic;                       // +0x9c
    int m_bShadow;                              // +0xa0
    int m_bGenerateFlare;                       // +0xa4
    int m_bFadingLight;                         // +0xa8

    virtual void InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x00469150
void MdlNodeLight::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_CONTROLLER("color", 5, 0x4c, -1, -1)
        MDL_CONTROLLER("radius", 6, 0x58, -1, -1)
        MDL_CONTROLLER("shadowradius", 12, 0x60, -1, -1)
        MDL_CONTROLLER("verticaldisplacement", 20, 0x64, -1, -1)
        MDL_CONTROLLER("multiplier", 10, 0x8c, -1, -1)
        MDL_FIELD("ambientonly", ParseInt(s + 11, &m_bAmbientOnly));
        MDL_FIELD("shadow", ParseInt(s + 6, &m_bShadow));
        MDL_FIELD("isdynamic", ParseInt(s + 9, &m_nDynamicType));
        MDL_FIELD("nDynamicType", ParseInt(s + 12, &m_nDynamicType));
        MDL_FIELD("affectdynamic", ParseInt(s + 13, &m_bAffectDynamic));
        MDL_FIELD("lightpriority", ParseInt(s + 13, &m_nLightPriority));
        MDL_FIELD("generateflare", ParseInt(s + 13, &m_bGenerateFlare));
        MDL_FIELD("texturenames", ParseStringList(s + 12, &m_lTextureNames));
        MDL_FIELD("flaresizes", ParseFloatList(s + 10, &m_lFlareSizes));
        MDL_FIELD("flarepositions", ParseFloatList(s + 14, &m_lFlarePositions));
        MDL_FIELD("flarecolorshifts", ParseVectorList(s + 16, &m_lFlareColorShifts));
        MDL_FIELD("flareradius", sscanf(s + 11, "%f", &m_fFlareRadius));
        MDL_FIELD("fadingLight", ParseInt(s + 11, &m_bFadingLight));
    }
    MdlNode::InternalParseField(s);
}

class MdlNodeEmitter : public MdlNode {
public:
    float m_fDeadSpace;                         // +0x50
    float m_fBlastRadius;                       // +0x54
    float m_fBlastLength;                       // +0x58
    int m_nBranches;                            // +0x5c
    int m_nControlPtSmoothing;                  // +0x60
    int m_nXGrid;                               // +0x64
    int m_nYGrid;                               // +0x68
    int m_nSpawnType;                           // +0x6c
    char m_szUpdate[32];                        // +0x70
    char m_szRender[32];                        // +0x90
    char m_szBlend[32];                         // +0xb0
    char m_szTexture[32];                       // +0xd0
    char m_szChunkName[16];                     // +0xf0
    int m_bTwoSidedTex;                         // +0x100
    int m_bLoop;                                // +0x104
    short m_nRenderOrder;                       // +0x108
    unsigned char m_bFrameBlending;             // +0x10a
    char m_szDepthTextureName[33];              // +0x10b
    int m_nFlags;                               // +0x12c

    virtual void InternalParseField(char* s);
    void EnablePointToPoint();                  // 0x0048D790
    void EnablePointToPointSel();               // 0x0048D7A0
    void EnableAffectedByWind();                // 0x0048D7B0
    void EnableTinted();                        // 0x0048D7C0
    void EnableBouncing();                      // 0x0048D7D0
    void EnableRandom();                        // 0x0048D7E0
    void EnableRandomOrder();                   // 0x0048D7F0
    void EnableInheriting();                    // 0x0048D800
    void EnableInheritingVelocity();            // 0x0048D810
    void EnableInheritingLocal();               // 0x0048D820
    void EnableSplat();                         // 0x0048D830
    void EnableInheritingPart();                // 0x0048D840
    void EnableDepthTexture();                  // 0x0048D850
};

// FUNCTION: SWKOTOR 0x004658B0
void MdlNodeEmitter::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        {   // flags applied through the Enable* setters
            unsigned char bP2P = 0;
            int nP2PSel = 0;
            unsigned char bAffectedByWind = 0;
            unsigned char bTinted = 0;
            unsigned char bBounce = 0;
            int nRandom = 0;
            unsigned char bInherit = 0;
            unsigned char bInheritVel = 0;
            unsigned char bInheritLocal = 0;
            unsigned char bSplat = 0;
            unsigned char bInheritPart = 0;
            unsigned char bDepthTexture = 0;
            MDL_FIELD("p2p", ParseBool(s + 3, &bP2P));
            MDL_FIELD("p2p_sel", ParseInt(s + 7, &nP2PSel));
            MDL_FIELD("affectedByWind", ParseBool(s + 14, &bAffectedByWind));
            MDL_FIELD("m_isTinted", ParseBool(s + 10, &bTinted));
            MDL_FIELD("bounce", ParseBool(s + 6, &bBounce));
            MDL_FIELD("random", ParseInt(s + 6, &nRandom));
            MDL_FIELD("inherit", ParseBool(s + 7, &bInherit));
            MDL_FIELD("inheritvel", ParseBool(s + 10, &bInheritVel));
            MDL_FIELD("inherit_local", ParseBool(s + 13, &bInheritLocal));
            MDL_FIELD("splat", ParseBool(s + 5, &bSplat));
            MDL_FIELD("inherit_part", ParseBool(s + 12, &bInheritPart));
            MDL_FIELD("depth_texture", ParseBool(s + 13, &bDepthTexture));
            if (bP2P)
                EnablePointToPoint();
            if (nP2PSel == 1)
                EnablePointToPointSel();
            if (bAffectedByWind)
                EnableAffectedByWind();
            if (bTinted)
                EnableTinted();
            if (bBounce)
                EnableBouncing();
            if (nRandom & 1)
                EnableRandom();
            if (nRandom & 2)
                EnableRandomOrder();
            if (bInherit)
                EnableInheriting();
            if (bInheritVel)
                EnableInheritingVelocity();
            if (bInheritLocal)
                EnableInheritingLocal();
            if (bSplat)
                EnableSplat();
            if (bInheritPart)
                EnableInheritingPart();
            if (bDepthTexture)
                EnableDepthTexture();
        }
        if (!_stricmp("renderorder", GetWord(s))) {
            int nOrder;
            sscanf(s + 11, "%d", &nOrder);
            m_nRenderOrder = (short)nOrder;
        }
        MDL_FIELD("spawntype", ParseInt(s + 9, &m_nSpawnType));
        MDL_FIELD("update", ParseString(s + 6, m_szUpdate));
        MDL_FIELD("render", ParseString(s + 6, m_szRender));
        MDL_FIELD("blend", ParseString(s + 5, m_szBlend));
        MDL_FIELD("texture", ParseString(s + 7, m_szTexture));
        MDL_FIELD("chunkName", ParseString(s + 9, m_szChunkName));
        MDL_FIELD("xgrid", ParseInt(s + 5, &m_nXGrid));
        MDL_FIELD("ygrid", ParseInt(s + 5, &m_nYGrid));
        MDL_FIELD("loop", ParseInt(s + 4, &m_bLoop));
        MDL_FIELD("deadspace", sscanf(s + 9, "%f", &m_fDeadSpace));
        MDL_FIELD("twosidedtex", ParseInt(s + 11, &m_bTwoSidedTex));
        MDL_FIELD("blastRadius", sscanf(s + 11, "%f", &m_fBlastRadius));
        MDL_FIELD("blastLength", sscanf(s + 11, "%f", &m_fBlastLength));
        MDL_FIELD("m_bFrameBlending", ParseBool(s + 16, &m_bFrameBlending));
        MDL_FIELD("m_sDepthTextureName", ParseString(s + 19, m_szDepthTextureName));
        MDL_FIELD("numBranches", ParseInt(s + 11, &m_nBranches));
        MDL_FIELD("controlptsmoothing", ParseInt(s + 18, &m_nControlPtSmoothing));
        MDL_CONTROLLER("blurlength", 10, 0xb4, -1, -1)
        MDL_CONTROLLER("detonate", 8, 0x1f6, -1, -1)
        MDL_CONTROLLER("velocity", 8, 0xa8, -1, -1)
        MDL_CONTROLLER("randvel", 7, 0x8c, -1, -1)
        MDL_CONTROLLER("birthrate", 9, 0x58, -1, -1)
        MDL_CONTROLLER("m_fRandomBirthRate", 18, 0xf0, -1, -1)
        MDL_CONTROLLER("colorStart", 10, 0x188, -1, -1)
        MDL_CONTROLLER("colorMid", 8, 0x11c, -1, -1)
        MDL_CONTROLLER("colorEnd", 8, 0x17c, -1, -1)
        MDL_CONTROLLER("percentStart", 12, 0xdc, -1, -1)
        MDL_CONTROLLER("percentMid", 10, 0xe0, -1, -1)
        MDL_CONTROLLER("percentEnd", 10, 0xe4, -1, -1)
        MDL_CONTROLLER("alphaStart", 10, 0x54, -1, -1)
        MDL_CONTROLLER("alphaMid", 8, 0xd8, -1, -1)
        MDL_CONTROLLER("alphaEnd", 8, 0x50, -1, -1)
        MDL_CONTROLLER("sizeStart", 9, 0x90, -1, -1)
        MDL_CONTROLLER("sizeMid", 7, 0xe8, -1, -1)
        MDL_CONTROLLER("sizeEnd", 7, 0x94, -1, -1)
        MDL_CONTROLLER("sizeStart_y", 11, 0x98, -1, -1)
        MDL_CONTROLLER("sizeMid_y", 9, 0xec, -1, -1)
        MDL_CONTROLLER("sizeEnd_y", 9, 0x9c, -1, -1)
        MDL_CONTROLLER("frameStart", 10, 0x70, -1, -1)
        MDL_CONTROLLER("frameEnd", 8, 0x6c, -1, -1)
        MDL_CONTROLLER("lifeExp", 7, 0x78, -1, -1)
        MDL_CONTROLLER("mass", 4, 0x7c, -1, -1)
        MDL_CONTROLLER("spread", 6, 0xa0, -1, -1)
        MDL_CONTROLLER("particleRot", 11, 0x88, -1, -1)
        MDL_CONTROLLER("fps", 3, 0x68, -1, -1)
        MDL_CONTROLLER("xsize", 5, 0xac, -1, -1)
        MDL_CONTROLLER("ysize", 5, 0xb0, -1, -1)
        MDL_CONTROLLER("bounce_co", 9, 0x5c, -1, -1)
        MDL_CONTROLLER("lightningDelay", 14, 0xb8, -1, -1)
        MDL_CONTROLLER("lightningRadius", 15, 0xbc, -1, -1)
        MDL_CONTROLLER("lightningScale", 14, 0xc0, -1, -1)
        MDL_CONTROLLER("lightningSubDiv", 15, 0xc4, -1, -1)
        MDL_CONTROLLER("lightningZigzag", 15, 0xc8, -1, -1)
        MDL_CONTROLLER("p2p_bezier2", 11, 0x80, -1, -1)
        MDL_CONTROLLER("p2p_bezier3", 11, 0x84, -1, -1)
        MDL_CONTROLLER("combinetime", 11, 0x60, -1, -1)
        MDL_CONTROLLER("drag", 4, 0x64, -1, -1)
        MDL_CONTROLLER("grav", 4, 0x74, -1, -1)
        MDL_CONTROLLER("threshold", 9, 0xa4, -1, -1)
        MDL_CONTROLLER("targetsize", 10, 0xfc, -1, -1)
        MDL_CONTROLLER("numcontrolpts", 13, 0x100, -1, -1)
        MDL_CONTROLLER("controlptradius", 15, 0x104, -1, -1)
        MDL_CONTROLLER("controlptdelay", 14, 0x108, -1, -1)
        MDL_CONTROLLER("tangentspread", 13, 0x10c, -1, -1)
        MDL_CONTROLLER("tangentlength", 13, 0x110, -1, -1)
    }
    MdlNode::InternalParseField(s);
}

struct MaxFace_PostProcess {
    int* pVerts;                                // +0x00
    int* pTVerts;                               // +0x04
    int nSmoothGroup;                           // +0x08
    int nMaterial;                              // +0x0c
    int* pNormals;                              // +0x10
};

struct TriFace {
    char pad_0x0[0x10];
    int nMaterial;                              // +0x10
    char pad_0x14[0xc];
};

// Raw ASCII mesh data kept until the trimesh is post-processed.
class MdlNodeTriMesh_PostProcess {
public:
    CExoArrayList<Vector> m_lVerts;             // +0x00
    CExoArrayList<Vector> m_lTVerts0;           // +0x0c
    CExoArrayList<Vector> m_lTVerts1;           // +0x18
    CExoArrayList<Vector> m_lTVerts2;           // +0x24
    CExoArrayList<Vector> m_lTVerts3;           // +0x30
    CExoArrayList<Vector> m_lColors;            // +0x3c
    CExoArrayList<MaxFace_PostProcess> m_lFaces;    // +0x48
    char pad_0x54[0x18];

    MdlNodeTriMesh_PostProcess();               // 0x00445720
};

// Texture index lists of the mesh being read (0x007A8A88 / 7C / 70).
extern CExoArrayList<Vector> g_lTexIndices1;
extern CExoArrayList<Vector> g_lTexIndices2;
extern CExoArrayList<Vector> g_lTexIndices3;

class MdlNodeTriMesh : public MdlNode {
public:
    char pad_0x50[8];
    CExoArrayList<TriFace> m_lFaces;            // +0x58
    Vector m_vBMin;                             // +0x64
    Vector m_vBMax;                             // +0x70
    char pad_0x7c[0x10];
    Vector m_vDiffuse;                          // +0x8c
    Vector m_vAmbient;                          // +0x98
    int m_nTransparencyHint;                    // +0xa4
    char m_szTexture0[32];                      // +0xa8
    char m_szTexture1[32];                      // +0xc8
    CExoArrayList<unsigned int*> m_lVertexIndices;  // +0xe8
    CExoArrayList<unsigned int> m_lLeftOverFaces;   // +0xf4
    CExoArrayList<int> m_lVertexIndicesCount;   // +0x100
    char pad_0x10c[0x28];
    MdlNodeTriMesh_PostProcess* m_pPostProcess; // +0x134
    int m_bAnimateUV;                           // +0x138
    float m_fUVDirectionX;                      // +0x13c
    float m_fUVDirectionY;                      // +0x140
    float m_fUVJitter;                          // +0x144
    float m_fUVJitterSpeed;                     // +0x148
    char pad_0x14c[0x38];
    unsigned char m_bLightMapped;               // +0x184
    unsigned char m_bRotateTexture;             // +0x185
    unsigned char m_bIsBackgroundGeometry;      // +0x186
    unsigned char m_bShadow;                    // +0x187
    unsigned char m_bBeaming;                   // +0x188
    unsigned char m_bRender;                    // +0x189
    char pad_0x18a[0x12];

    virtual void InternalParseField(char* s);
};

// NONMATCH: same frame and statement order; the original keeps `s` in esi
// and `this` in ebp (spilled around the lightmap block), ours swaps them,
// and the lightmap offset loop is strength-reduced differently.
// FUNCTION: SWKOTOR 0x00469700
void MdlNodeTriMesh::InternalParseField(char* s)
{
    MDL_FIELD("texture0", ParseStringInline(s + 8, m_szTexture0));
    MDL_FIELD("texture1", ParseStringInline(s + 8, m_szTexture1));
    char szBitmap[64];
    szBitmap[0] = 0;
    MDL_FIELD("bitmap", ParseStringInline(s + 6, szBitmap));
    if (szBitmap[0])
        strcpy(m_szTexture0, szBitmap);
    if (!g_bBinaryModel) {
        if (!m_pPostProcess)
            m_pPostProcess = new MdlNodeTriMesh_PostProcess;
        MdlNodeTriMesh_PostProcess* pp = m_pPostProcess;
        CExoArrayList<Vector>* pVerts = &pp->m_lVerts;
        CExoArrayList<Vector>* pTVerts0 = &pp->m_lTVerts0;
        CExoArrayList<Vector>* pTVerts1 = &pp->m_lTVerts1;
        CExoArrayList<Vector>* pTVerts2 = &pp->m_lTVerts2;
        CExoArrayList<Vector>* pTVerts3 = &pp->m_lTVerts3;
        CExoArrayList<Vector>* pColors = &pp->m_lColors;
        CExoArrayList<MaxFace_PostProcess>* pFaces = &pp->m_lFaces;
        MDL_FIELD("verts", ParseVectorList(s + 5, pVerts));
        MDL_FIELD("tverts", ParseVectorList(s + 6, pTVerts0));
        int bAnimateUV = 0;
        MDL_FIELD("animateuv", ParseInt(s + 9, &bAnimateUV));
        if (bAnimateUV)
            m_bAnimateUV = 1;
        if (m_bAnimateUV) {
            MDL_FIELD("uvdirectionx", sscanf(s + 12, "%f", &m_fUVDirectionX));
            MDL_FIELD("uvdirectiony", sscanf(s + 12, "%f", &m_fUVDirectionY));
            MDL_FIELD("uvjitter", sscanf(s + 8, "%f", &m_fUVJitter));
            MDL_FIELD("uvjitterspeed", sscanf(s + 13, "%f", &m_fUVJitterSpeed));
        }
        MDL_FIELD("tverts1", ParseVectorList(s + 7, pTVerts1));
        MDL_FIELD("tverts2", ParseVectorList(s + 7, pTVerts2));
        MDL_FIELD("tverts3", ParseVectorList(s + 7, pTVerts3));
        MDL_FIELD("colors", ParseVectorList(s + 6, pColors));
        MDL_FIELD("texindices1", ParseVectorList(s + 11, &g_lTexIndices1));
        MDL_FIELD("texindices2", ParseVectorList(s + 11, &g_lTexIndices2));
        MDL_FIELD("texindices3", ParseVectorList(s + 11, &g_lTexIndices3));
        if (m_szTexture1[0] && pTVerts1->num > 0) {
            char szLightMap[64];
            strcpy(szLightMap, m_szTexture1);
            AurRes* pRes = AurResGet(m_szTexture1, ".lmt", 0, 1);
            if (pRes) {
                CExoArrayList<int> lOffsets;
                char* line;
                while ((line = AurResGetNextLine()) != 0) {
                    while (*line == ' ' || *line == '\t')
                        line++;
                    if (!_stricmp("tverts1offset", GetWord(line)))
                        ParseList(line + 13, &lOffsets);
                    if (!_stricmp("texture1", GetWord(line)))
                        ParseString(line + 8, m_szTexture1);
                }
                AurResClose(pRes, 0);
                if (pTVerts1->num * 4 == lOffsets.num) {
                    for (int i = 0; i < pTVerts1->num; i++) {
                        pTVerts1->element[i] = pTVerts1->element[i] * (float)lOffsets.element[i * 4];
                        pTVerts1->element[i].x += lOffsets.element[i * 4 + 1];
                        pTVerts1->element[i].y += lOffsets.element[i * 4 + 2];
                        pTVerts1->element[i] /= (float)lOffsets.element[i * 4 + 3];
                    }
                }
            }
        }
        if (pFaces->num == 0)
            MDL_FIELD("faces", Parse(s + 5, pFaces));
        if (pFaces->num > 0) {
            m_lFaces.SetSizeCall(pFaces->num);
            for (int i = 0; i < pFaces->num; i++)
                m_lFaces.element[i].nMaterial = pFaces->element[i].nMaterial;
        }
        MDL_FIELD("diffuse", ParseVectorInline(s + 7, (float*)&m_vDiffuse));
        MDL_FIELD("ambient", ParseVectorInline(s + 7, (float*)&m_vAmbient));
        MDL_FIELD("shadow", ParseBool(s + 6, &m_bShadow));
        MDL_FIELD("transparencyhint", ParseInt(s + 16, &m_nTransparencyHint));
        MDL_FIELD("beaming", ParseBool(s + 7, &m_bBeaming));
        MDL_FIELD("render", ParseBool(s + 6, &m_bRender));
        MDL_FIELD("rotatetexture", ParseBool(s + 13, &m_bRotateTexture));
        MDL_FIELD("bmin", ParseVectorInline(s + 4, (float*)&m_vBMin));
        MDL_FIELD("bmax", ParseVectorInline(s + 4, (float*)&m_vBMax));
        MDL_FIELD("vertexindicescount", ParseList(s + 18, &m_lVertexIndicesCount));
        MDL_FIELD("leftoverfaces", Parse(s + 13, &m_lLeftOverFaces));
        MDL_FIELD("vertexindices", ParseUIntPtrList(s + 13, &m_lVertexIndices));
        MDL_FIELD("lightmapped", ParseBool(s + 11, &m_bLightMapped));
        MDL_FIELD("m_bIsBackgroundGeometry", ParseBool(s + 23, &m_bIsBackgroundGeometry));
        MDL_CONTROLLER("selfillumcolor", 14, 0x64, -1, -1)
        MDL_CONTROLLER("alpha", 5, 0x84, -1, -1)
    }
    MdlNode::InternalParseField(s);
}
