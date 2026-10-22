// small MaxTree / Model / MdlNode leaves (0x0043e0f0-0x0043e490):
// type queries, checked downcasts, ref counting and parse thunks.
#include "g/mdl_node.h"

extern int g_nTriFaceCount;         // 0x007a8a00
extern int g_bModelNoDelete;        // 0x007a8a04

// FUNCTION: SWKOTOR 0x0043e0f0
MaxFace_PostProcess::~MaxFace_PostProcess()
{
    if (m_pVerts)
        delete[] m_pVerts;
    if (m_pTVerts)
        delete[] m_pTVerts;
    if (m_pNormals)
        delete[] m_pNormals;
    m_nVerts = 0;
    m_nTVerts = 0;
}

// FUNCTION: SWKOTOR 0x0043e130
int MaxTree::GetType()
{
    return m_nType & 0x7f;
}

// FUNCTION: SWKOTOR 0x0043e140
TriFace::~TriFace()
{
    g_nTriFaceCount--;
}

// FUNCTION: SWKOTOR 0x0043e1a0
MdlNodeTrigger::~MdlNodeTrigger()
{
}

// FUNCTION: SWKOTOR 0x0043e1c0
Model* MaxTree::AsModel()
{
    return (m_nType & 0x7f) == 2 ? (Model*)this : 0;
}

// FUNCTION: SWKOTOR 0x0043e1d0
Animation* MaxTree::AsAnimation()
{
    return (m_nType & 0x7f) == 5 ? (Animation*)this : 0;
}

int __cdecl StringCompare(char* a, char* b);    // 0x004aa980

// FUNCTION: SWKOTOR 0x0043e1f0
int __cdecl equal(char* a, char* b)
{
    return StringCompare(a, b) == 0;
}

// FUNCTION: SWKOTOR 0x0043e210
int Model::DelRef(int nUnused)
{
    if (g_bModelNoDelete)
        return m_nRefCount;
    m_nRefCount--;
    int n = m_nRefCount;
    if (m_nRefCount == 0)
        MaxTree::operator delete(this);
    return n;
}

// FUNCTION: SWKOTOR 0x0043e240
int Model::AddRef(int nUnused)
{
    return ++m_nRefCount;
}

// FUNCTION: SWKOTOR 0x0043e340
MdlNodeAABB* MdlNode::AsMdlNodeAABB()
{
    short f = m_nType;
    f &= MDLNODE_AABB | MDLNODE_MESH | MDLNODE_HEADER;
    if (f == (MDLNODE_AABB | MDLNODE_MESH | MDLNODE_HEADER))
        return (MdlNodeAABB*)this;
    return 0;
}

// FUNCTION: SWKOTOR 0x0043e360
MdlNodeAnimMesh* MdlNode::AsMdlNodeAnimMesh()
{
    return (m_nType & (MDLNODE_ANIM | MDLNODE_MESH | MDLNODE_HEADER)) ==
                   (MDLNODE_ANIM | MDLNODE_MESH | MDLNODE_HEADER)
               ? (MdlNodeAnimMesh*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e370
MdlNodeCamera* MdlNode::AsMdlNodeCamera()
{
    return (m_nType & (MDLNODE_CAMERA | MDLNODE_HEADER)) ==
                   (MDLNODE_CAMERA | MDLNODE_HEADER)
               ? (MdlNodeCamera*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e380
MdlNodeDanglyMesh* MdlNode::AsMdlNodeDanglyMesh()
{
    short f = m_nType;
    f &= MDLNODE_DANGLY | MDLNODE_MESH | MDLNODE_HEADER;
    if (f == (MDLNODE_DANGLY | MDLNODE_MESH | MDLNODE_HEADER))
        return (MdlNodeDanglyMesh*)this;
    return 0;
}

// FUNCTION: SWKOTOR 0x0043e3a0
MdlNodeLightsaber* MdlNode::AsMdlNodeLightsaber()
{
    short f = m_nType;
    f &= MDLNODE_SABER | MDLNODE_MESH | MDLNODE_HEADER;
    if (f == (MDLNODE_SABER | MDLNODE_MESH | MDLNODE_HEADER))
        return (MdlNodeLightsaber*)this;
    return 0;
}

// FUNCTION: SWKOTOR 0x0043e3c0
MdlNodeEmitter* MdlNode::AsMdlNodeEmitter()
{
    return (m_nType & (MDLNODE_EMITTER | MDLNODE_HEADER)) ==
                   (MDLNODE_EMITTER | MDLNODE_HEADER)
               ? (MdlNodeEmitter*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e3d0
MdlNodeLight* MdlNode::AsMdlNodeLight()
{
    return (m_nType & (MDLNODE_LIGHT | MDLNODE_HEADER)) ==
                   (MDLNODE_LIGHT | MDLNODE_HEADER)
               ? (MdlNodeLight*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e3e0
MdlNodeReference* MdlNode::AsMdlNodeReference()
{
    return (m_nType & (MDLNODE_REFERENCE | MDLNODE_HEADER)) ==
                   (MDLNODE_REFERENCE | MDLNODE_HEADER)
               ? (MdlNodeReference*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e3f0
MdlNodeSkin* MdlNode::AsMdlNodeSkin()
{
    return (m_nType & (MDLNODE_SKIN | MDLNODE_MESH | MDLNODE_HEADER)) ==
                   (MDLNODE_SKIN | MDLNODE_MESH | MDLNODE_HEADER)
               ? (MdlNodeSkin*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e400
MdlNodeTriMesh* MdlNode::AsMdlNodeTriMesh()
{
    return (m_nType & (MDLNODE_MESH | MDLNODE_HEADER)) ==
                   (MDLNODE_MESH | MDLNODE_HEADER)
               ? (MdlNodeTriMesh*)this
               : 0;
}

// FUNCTION: SWKOTOR 0x0043e410
unsigned char MdlNodeTriMesh::HasTangentBases()
{
    return m_nTangentSpace != -1;
}

// FUNCTION: SWKOTOR 0x0043e2a0 SYMBOL: ??_GAABB@@QAEPAXI@Z
void DeleteAABB(AABB* p)
{
    delete p;
}

// --- hidden leaves between the functions.csv rows ---------------------------

class CCursorTracker {
public:
    char pad00[0x50];
    int m_nChanges;             // +0x50
};

class CCursorOwner {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0c();
    virtual void V10();
    virtual void SetScale(int nScale);          // +0x14
    char pad04[0x68 - 4];
    CCursorTracker* m_pTracker; // +0x68

    void SetCursorScale(int nScale);
};

// FUNCTION: SWKOTOR 0x0043e150
void CCursorOwner::SetCursorScale(int nScale)
{
    if (m_pTracker) {
        m_pTracker->m_nChanges++;
        SetScale(nScale);
        m_pTracker->m_nChanges++;
    } else {
        SetScale(nScale);
    }
}

class MaxTreeParser {
public:
    void InternalParseField(char* szField);     // 0x00468870
    void ModelParseField(char* szField);        // 0x004688a0
};

// FUNCTION: SWKOTOR 0x0043e190
void __cdecl MaxTreeParseField(MaxTreeParser* p, char* szField)
{
    p->InternalParseField(szField);
}

// FUNCTION: SWKOTOR 0x0043e1e0
void __cdecl ModelParseField(MaxTreeParser* p, char* szField)
{
    p->ModelParseField(szField);
}

// FUNCTION: SWKOTOR 0x0043e1b0
void* __cdecl AurNew(size_t n)
{
    return operator new(n);
}

class CAurPart {
public:
    virtual ~CAurPart();
};

// FUNCTION: SWKOTOR 0x0043e2e0
CAurPart::~CAurPart()
{
}

class CAurMeshBuffer {
public:
    virtual ~CAurMeshBuffer();
};

// FUNCTION: SWKOTOR 0x0043e310
CAurMeshBuffer::~CAurMeshBuffer()
{
}

// FUNCTION: SWKOTOR 0x0043e2f0 SYMBOL: ??_GCAurPart@@UAEPAXI@Z
void DeleteAurPart(CAurPart* p) { delete p; }
// FUNCTION: SWKOTOR 0x0043e320 SYMBOL: ??_GCAurMeshBuffer@@UAEPAXI@Z
void DeleteAurMeshBuffer(CAurMeshBuffer* p) { delete p; }

// FUNCTION: SWKOTOR 0x0043e250
Vector Quaternion::ToDirection()
{
    return Vector((x * z + w * y) * 2.0f, (w * z - x * y) * 2.0f,
                  1.0f - (y * y + z * z) * 2.0f);
}

// --- static per-node-type dispatch thunks (function tables at +0x50/+0x54
// of the MdlNode class descriptors) ------------------------------------------

class MdlNodeThunkTarget {
public:
    void Fn43d450();
    void Fn43c320();
    void InternalGenVertices();     // 0x004885e0 MdlNodeSkin
    void Fn43c1a0();
    void Fn43c130();
    void Fn43bd40();
    void Fn439df0();
    void Fn43c090();
};

// FUNCTION: SWKOTOR 0x0043e420
void __cdecl Thunk43d450(MdlNodeThunkTarget* p) { p->Fn43d450(); }
// FUNCTION: SWKOTOR 0x0043e430
void __cdecl Thunk43c320(MdlNodeThunkTarget* p) { p->Fn43c320(); }
// FUNCTION: SWKOTOR 0x0043e440
void __cdecl ThunkSkinGenVertices(MdlNodeThunkTarget* p) { p->InternalGenVertices(); }
// FUNCTION: SWKOTOR 0x0043e450
void __cdecl Thunk43c1a0(MdlNodeThunkTarget* p) { p->Fn43c1a0(); }
// FUNCTION: SWKOTOR 0x0043e460
void __cdecl Thunk43c130(MdlNodeThunkTarget* p) { p->Fn43c130(); }
// FUNCTION: SWKOTOR 0x0043e470
void __cdecl Thunk43bd40(MdlNodeThunkTarget* p) { p->Fn43bd40(); }
// FUNCTION: SWKOTOR 0x0043e480
void __cdecl Thunk439df0(MdlNodeThunkTarget* p) { p->Fn439df0(); }
// FUNCTION: SWKOTOR 0x0043e490
void __cdecl Thunk43c090(MdlNodeThunkTarget* p) { p->Fn43c090(); }
