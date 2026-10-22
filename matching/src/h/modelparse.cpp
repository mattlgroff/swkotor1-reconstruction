// MdlNode subclass InternalParseField overrides (ASCII model
// reader) 0x00468060-0x0046A3C0.

#include "h/texctl.h"

extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int);

extern unsigned char g_bBinaryModel;        // 0x007B927C

#define MDL_FIELD(name, stmt) \
    if (!_stricmp(name, GetWord(s))) \
        stmt

struct MaxFace_PostProcess;
struct SkinVertexWeight;
struct AABBNode;

void ParseVectorList(char* s, CExoArrayList<Vector>* pList);                // 0x00421FF0
void ParseList(char* s, CExoArrayList<int>* pList);                         // 0x00464630
void ParseSkinWeights(char* s, CExoArrayList<SkinVertexWeight>* pList);     // 0x00468D70
void ParseQuaternions(char* s, CExoArrayList<Quaternion>* pList);           // 0x00468F60
void Parse_AABB(char* s, AABBNode** ppRoot);                                // 0x00463C20

class MdlNode {
public:
    virtual int InternalParseField(char* s);    // 0x00465560
    char pad_0x4[0x4c];
    char m_szRefModel[32];                      // +0x50
    int m_bReattachable;                        // +0x70
};

class MdlNodeDummy : public MdlNode {
public:
    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x00468060 SYMBOL: ?InternalParseField@MdlNodeDummy@@UAEHPAD@Z
int MdlNodeDummy::InternalParseField(char* s)
{
    return MdlNode::InternalParseField(s);
}

class MdlNodeReference : public MdlNode {
public:
    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x00468070 SYMBOL: ?InternalParseField@MdlNodeReference@@UAEHPAD@Z
int MdlNodeReference::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_FIELD("refModel", ParseString(s + 8, m_szRefModel));
        MDL_FIELD("reattachable", ParseInt(s + 12, &m_bReattachable));
    }
    return MdlNode::InternalParseField(s);
}

class MdlNodeTriMesh : public MdlNode {
public:
    virtual int InternalParseField(char* s);    // 0x00469700
    char pad_0x74[0x128];
};

class MdlNodeTriMeshDummy : public MdlNodeTriMesh {
public:
    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x0046A150 SYMBOL: ?InternalParseField@MdlNodeTriMeshDummy@@UAEHPAD@Z
int MdlNodeTriMeshDummy::InternalParseField(char* s)
{
    return MdlNodeTriMesh::InternalParseField(s);
}

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    CExoArrayList<float> m_lConstraints;    // +0x19c
    float m_fDisplacement;                  // +0x1a8
    float m_fTightness;                     // +0x1ac
    float m_fPeriod;                        // +0x1b0

    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x0046A160 SYMBOL: ?InternalParseField@MdlNodeDanglyMesh@@UAEHPAD@Z
int MdlNodeDanglyMesh::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_FIELD("displacement", sscanf(s + 12, "%f", &m_fDisplacement));
        MDL_FIELD("period", sscanf(s + 6, "%f", &m_fPeriod));
        MDL_FIELD("tightness", sscanf(s + 9, "%f", &m_fTightness));
        MDL_FIELD("constraints", ParseFloatList(s + 11, &m_lConstraints));
    }
    return MdlNodeTriMesh::InternalParseField(s);
}

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    float m_fSamplePeriod;                  // +0x19c
    CExoArrayList<Vector> m_lAnimVerts;     // +0x1a0
    CExoArrayList<Vector> m_lAnimTVerts;    // +0x1ac

    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x0046A240 SYMBOL: ?InternalParseField@MdlNodeAnimMesh@@UAEHPAD@Z
int MdlNodeAnimMesh::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_FIELD("sampleperiod", sscanf(s + 12, "%f", &m_fSamplePeriod));
        MDL_FIELD("animverts", ParseVectorList(s + 9, &m_lAnimVerts));
        MDL_FIELD("animtverts", ParseVectorList(s + 10, &m_lAnimTVerts));
    }
    return MdlNodeTriMesh::InternalParseField(s);
}

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    CExoArrayList<SkinVertexWeight> m_lWeights;     // +0x19c
    char pad_0x1a8[0x10];
    CExoArrayList<Quaternion> m_lQBoneRefInv;       // +0x1b8
    CExoArrayList<Vector> m_lTBoneRefInv;           // +0x1c4
    CExoArrayList<int> m_lBoneConstantIndices;      // +0x1d0

    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x0046A2F0 SYMBOL: ?InternalParseField@MdlNodeSkin@@UAEHPAD@Z
int MdlNodeSkin::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_FIELD("weights", ParseSkinWeights(s + 7, &m_lWeights));
        MDL_FIELD("qbone_ref_inv", ParseQuaternions(s + 13, &m_lQBoneRefInv));
        MDL_FIELD("tbone_ref_inv", ParseVectorList(s + 13, &m_lTBoneRefInv));
        MDL_FIELD("boneconstantindices", ParseList(s + 19, &m_lBoneConstantIndices));
    }
    return MdlNodeTriMesh::InternalParseField(s);
}

class MdlNodeAABB : public MdlNodeTriMesh {
public:
    AABBNode* m_pAABB;                      // +0x19c

    virtual int InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x0046A3C0 SYMBOL: ?InternalParseField@MdlNodeAABB@@UAEHPAD@Z
int MdlNodeAABB::InternalParseField(char* s)
{
    if (!g_bBinaryModel) {
        MDL_FIELD("aabb", Parse_AABB(s + 4, &m_pAABB));
    }
    return MdlNodeTriMesh::InternalParseField(s);
}

class MaxTree;
void ParseNode(char* s, MaxTree* pTree);    // 0x004680E0

class MaxTree {
public:
    virtual void InternalParseField(char* s);
    char pad_0x4[0x4c];
};

// FUNCTION: SWKOTOR 0x00468870 SYMBOL: ?InternalParseField@MaxTree@@UAEXPAD@Z
void MaxTree::InternalParseField(char* s)
{
    if (!_strnicmp(s, "node", 4))
        ParseNode(s + 4, this);
}

// ParseVector 0x00463570, expanded in this TU.
inline void ParseVectorInline(char* s, float* pOut)
{
    sscanf(s, "%f %f %f", pOut, pOut + 1, pOut + 2);
}

class Model : public MaxTree {
public:
    char pad_0x50[0x18];
    Vector m_vBMin;                         // +0x68
    Vector m_vBMax;                         // +0x74
    float m_fRadius;                        // +0x80

    virtual void InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x004688A0 SYMBOL: ?InternalParseField@Model@@UAEXPAD@Z
void Model::InternalParseField(char* s)
{
    MDL_FIELD("radius", sscanf(s + 6, "%f", &m_fRadius));
    MDL_FIELD("bmin", ParseVectorInline(s + 4, (float*)&m_vBMin));
    MDL_FIELD("bmax", ParseVectorInline(s + 4, (float*)&m_vBMax));
    MaxTree::InternalParseField(s);
}

// ParseString 0x00463590, expanded in this TU.
inline void ParseStringInline(char* s, char* pOut)
{
    int n = sscanf(s, "%s", pOut);
    if (n == 0)
        strcpy(pOut, (char*)n);
}

void ParseKeyString(char* s, AurKeyString* pKey);   // 0x004638F0

class Animation : public MaxTree {
public:
    float m_fLength;                        // +0x50
    float m_fTransTime;                     // +0x54
    char m_szAnimRoot[32];                  // +0x58
    CExoArrayList<AurKeyString> m_lEvents;  // +0x78

    virtual void InternalParseField(char* s);
};

// FUNCTION: SWKOTOR 0x0046A410 SYMBOL: ?InternalParseField@Animation@@UAEXPAD@Z
void Animation::InternalParseField(char* s)
{
    char szName[128];
    if (!_strnicmp("animlength", s, 10))
        sscanf(s + 10, "%s %f", szName, &m_fLength);
    if (!_strnicmp("event", s, 5)) {
        AurKeyString ev;
        ev.szValue[0] = 0;
        ParseKeyString(s + 5, &ev);
        int i;
        for (i = 0; i < m_lEvents.num && ev.fTime > m_lEvents.element[i].fTime; i++)
            ;
        m_lEvents.Insert(ev, i);
    }
    MDL_FIELD("length", sscanf(s + 6, "%f", &m_fLength));
    MDL_FIELD("transtime", sscanf(s + 9, "%f", &m_fTransTime));
    MDL_FIELD("animroot", ParseStringInline(s + 8, m_szAnimRoot));
    MaxTree::InternalParseField(s);
}
