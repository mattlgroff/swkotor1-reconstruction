// InputBinary model relocation (0x004a0060-0x004a15d0;
// behavioural reference: src/batches/b004a0_input_binary_reset.cpp,
// docs/batches/b004a0_input_binary_reset.md). Binary models are loaded as
// one block; every stored pointer is an offset that Reset* turns into an
// address (base = the model block, base2 = the raw .mdx data).
#ifndef I_INPUT_BINARY_H
#define I_INPUT_BINARY_H

typedef unsigned int size_t;

void* operator new(size_t n);
void operator delete(void* p);
extern "C" void* __cdecl memcpy(void* pDst, const void* pSrc, size_t n);
#pragma intrinsic(memcpy)

// CExoArrayList with the inline growth used by the SafePointer back lists.
template <class T>
class CExoArrayList {
public:
    T* element;         // +0x00
    int num;            // +0x04
    int array_size;     // +0x08

    T& operator[](int i) { return element[i]; }
    void Allocate(int n);
    void SetSize(int n);
    void InlineAllocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++)
            element[i] = pOld[i];
        if (pOld)
            delete[] pOld;
    }
    void Add(T t)
    {
        if (num == array_size)
            InlineAllocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void Remove(T t)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == t)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void InlineSetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        } else {
            Allocate(n);
        }
        num = n;
    }
};

template <class T>
class SafePointer {
public:
    T* m_p;

    ~SafePointer();
    SafePointer<T>& operator=(T* p)
    {
        if (m_p)
            m_p->m_lRefs.Remove(this);
        m_p = p;
        if (p)
            p->m_lRefs.Add(this);
        return *this;
    }
};

class CAurTexture {
public:
    virtual ~CAurTexture();
    CExoArrayList<SafePointer<CAurTexture>*> m_lRefs;   // +0x04
};

CAurTexture* AurTextureGetReference(char* szName, int n);  // 0x00423a60

// Render-pool helpers (glrender).
void PoolRequest(int nType, int nSize, int nFlags, int nUnused, int* pId);  // 0x00423ee0
void* PoolLock(int nId, int nUnused1, int nUnused2, int nFlags);           // 0x00423f10
void PoolUnlock(int nId);                                                   // 0x00423f30

// MdlNode type words (.rdata 0x00740a18..0x00740a44).
extern const unsigned short NODE_TYPE_DUMMY;        // 0x00740a18 (0x001)
extern const unsigned short NODE_TYPE_LIGHT;        // 0x00740a1c (0x003)
extern const unsigned short NODE_TYPE_EMITTER;      // 0x00740a20 (0x005)
extern const unsigned short NODE_TYPE_CAMERA;       // 0x00740a24 (0x009)
extern const unsigned short NODE_TYPE_REFERENCE;    // 0x00740a28 (0x011)
extern const unsigned short NODE_TYPE_TRIMESH;      // 0x00740a2c (0x021)
extern const unsigned short NODE_TYPE_SKIN;         // 0x00740a30 (0x061)
extern const unsigned short NODE_TYPE_ANIM;         // 0x00740a34 (0x0a1)
extern const unsigned short NODE_TYPE_DANGLY;       // 0x00740a38 (0x121)
extern const unsigned short NODE_TYPE_AABB;         // 0x00740a3c (0x221)
extern const unsigned short NODE_TYPE_PATCH;        // 0x00740a40 (0x401)
extern const unsigned short NODE_TYPE_SABER;        // 0x00740a44 (0x821)

// Lightsaber vertex-buffer dimensions (.rdata words).
extern const short SABER_NUM_SEGMENTS;              // 0x00740a4c
extern const short SABER_NUM_VERTS;                 // 0x00740a50
extern const short SABER_NUM_STRIPS;                // 0x00740a54

extern int g_nMeshFaces;            // 0x00832c80
extern int g_nMeshVertBytes;        // 0x00832c88
extern int g_nModelMemory;          // 0x007a6990

class MdlNode;
class MaxTreeParser;
typedef void (*MdlNodeFn)(MdlNode* pNode);
typedef void (*MaxTreeFn)(MaxTreeParser* pParser, char* szField);

// Per-node render thunks (0x0043e420..0x0043e490).
void Thunk43d450(MdlNode* p);
void Thunk43c320(MdlNode* p);
void ThunkSkinGenVertices(MdlNode* p);
void Thunk43c1a0(MdlNode* p);
void Thunk43c130(MdlNode* p);
void Thunk43bd40(MdlNode* p);
void Thunk439df0(MdlNode* p);
void Thunk43c090(MdlNode* p);
void AnimationParseField(MaxTreeParser* p, char* szField);  // 0x0044a8e0
void MaxTreeParseField(MaxTreeParser* p, char* szField);    // 0x0043e190
void ModelParseHeader(MaxTreeParser* p, char* szField);     // 0x0044aa60
void ModelParseField(MaxTreeParser* p, char* szField);      // 0x0043e1e0

class Model;

class MaxTree {
public:
    MaxTreeFn m_pfnParse;       // +0x00
    MaxTreeFn m_pfnField;       // +0x04
    char pad08[0x28 - 0x08];
    MdlNode* m_pRoot;           // +0x28

    int GetType();                      // 0x0043e130
    Model* AsModel();                   // 0x0043e1c0
    void AddToFreeTokenList(char* p);   // 0x00445190
    void ResetFreeLists();              // 0x004451d0
};

class Animation : public MaxTree {
public:
    char pad2c[0x78 - 0x2c];
    CExoArrayList<int> m_lEvents;       // +0x78
    Model* m_pModel;                    // +0x84
};

class Model : public MaxTree {
public:
    char pad2c[0x48 - 0x2c];
    void* m_pResource;                  // +0x48
    unsigned char m_nType;              // +0x4c
    char pad4d[0x58 - 0x4d];
    CExoArrayList<Animation*> m_lAnimations;    // +0x58
    Model* m_pSuperModel;               // +0x64
    char pad68[0x88 - 0x68];
    char m_szSuperModel[0x20];          // +0x88
    char* m_pa8;                        // +0xa8
    int m_nVertexPool;                  // +0xac (raw offset until uploaded)
    int m_nVertexPoolSize;              // +0xb0
    void* m_pRawResource;               // +0xb4
    CExoArrayList<char*> m_lNames;      // +0xb8

    int AddRef(Model* pRef);            // 0x0043e240
};

Model* FindModel(char* szName);         // 0x00464110
void InsertModel(Model* pModel);        // 0x0046a5f0
void ResolveAnimationNodes(MdlNode* pRoot, int n);  // 0x00437db0

class MdlNode {
public:
    unsigned short m_nType;             // +0x00
    char pad02[0x08 - 0x02];
    MaxTree* m_pTree;                   // +0x08
    MdlNode* m_pParent;                 // +0x0c
    char pad10[0x2c - 0x10];
    CExoArrayList<MdlNode*> m_lChildren;    // +0x2c
    CExoArrayList<char> m_lControllerKeys;  // +0x38
    CExoArrayList<char> m_lControllerData;  // +0x44

    void AddToFreeList(void* p);        // 0x004452d0
    void AddToFreeTokenList(char* p);   // 0x00445310
};

class MdlNodeLight : public MdlNode {
public:
    char pad50[0x54 - 0x50];
    CExoArrayList<SafePointer<CAurTexture> > m_lTextures;    // +0x54
    CExoArrayList<float> m_lFlareSizes;     // +0x60
    CExoArrayList<float> m_lFlarePositions; // +0x6c
    CExoArrayList<char> m_lFlareColors;     // +0x78
    CExoArrayList<char*> m_lTextureNames;   // +0x84
};

class MdlNodeTriMesh : public MdlNode {
public:
    MdlNodeFn m_pfnRender;              // +0x50
    MdlNodeFn m_pfnPrepare;             // +0x54
    CExoArrayList<char> m_lFaces;       // +0x58
    char pad64[0xe8 - 0x64];
    CExoArrayList<int*> m_lIndexLists;  // +0xe8
    CExoArrayList<int> m_lIndexOffsets; // +0xf4
    CExoArrayList<int> m_lIndexCounts;  // +0x100
    CExoArrayList<int> m_lIndexData;    // +0x10c (raw offsets)
    CExoArrayList<int> m_lIndexPools;   // +0x118
    int m_nVertexData;                  // +0x124 (raw offset)
    int m_nVertexPool;                  // +0x128
    int m_nVertexCount;                 // +0x12c
    char pad130[0x14c - 0x130];
    int m_nVertexSize;                  // +0x14c
    char pad150[0x180 - 0x150];
    short m_nVertices;                  // +0x180
    char pad182[0x198 - 0x182];
    char* m_pVertices;                  // +0x198
};

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    char pad19c[0x1a0 - 0x19c];
    char* m_pAnimVerts;                 // +0x1a0
    int m_nAnimVerts;                   // +0x1a4
    char pad1a8[0x1ac - 0x1a8];
    char* m_pAnimTVerts;                // +0x1ac
    int m_nAnimTVerts;                  // +0x1b0
    char pad1b4[0x1b8 - 0x1b4];
    char* m_pAnimNormals;               // +0x1b8
    int m_nAnimNormals;                 // +0x1bc
    char pad1c0[0x1c4 - 0x1c0];
    char* m_pVertexSets;                // +0x1c4
    char* m_pTextureSets;               // +0x1c8
    int m_nVertexSets;                  // +0x1cc
    int m_nTextureSets;                 // +0x1d0
};

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    CExoArrayList<char> m_lConstraints; // +0x19c
    char pad1a8[0x1b4 - 0x1a8];
    char* m_pVertexData;                // +0x1b4
};

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    char pad19c[0x1b0 - 0x19c];
    char* m_pBoneMap;                   // +0x1b0
    int m_nBoneMap;                     // +0x1b4
    char* m_pBoneQuats;                 // +0x1b8
    int m_nBoneQuats;                   // +0x1bc
    char pad1c0[0x1c4 - 0x1c0];
    char* m_pBoneTrans;                 // +0x1c4
    int m_nBoneTrans;                   // +0x1c8
    char pad1cc[0x1d0 - 0x1cc];
    char* m_pBoneConst;                 // +0x1d0
    int m_nBoneConst;                   // +0x1d4
};

class AABB {
public:
    char pad00[0x18];
    AABB* m_pLeft;                      // +0x18
    AABB* m_pRight;                     // +0x1c
};

class MdlNodeAABB : public MdlNodeTriMesh {
public:
    AABB* m_pTree;                      // +0x19c
};

class MdlNodeLightsaber : public MdlNodeTriMesh {
public:
    char* m_p19c;                       // +0x19c
    char* m_p1a0;                       // +0x1a0
    char* m_p1a4;                       // +0x1a4
    int m_nVertexPool;                  // +0x1a8
    int m_nTexPool;                     // +0x1ac
};

class AurRes;

char* AurResGetDataBytes(int nBytes, AurRes* pRes);     // 0x0044BD10
void AurResFreeDataBytes(AurRes* pRes, char* pData);    // 0x0044BD50
void AurResRemove(AurRes* pRes);                        // 0x0044BDC0
void AurResClose(AurRes* pRes, int n);                  // 0x0044C8F0

class InputBinary {
public:
    MaxTree* Read(AurRes* pMdl, AurRes* pMdx, bool bLite);   // 0x004A1260

    void ResetAnim(MdlNodeAnimMesh* pNode, int nBase, int nRaw);
    void ResetDangly(MdlNodeDanglyMesh* pNode, int nBase, int nRaw);
    void ResetMdlNodePartsLite(MdlNode* pNode, int nBase, int nRaw);
    // Bodies identical to ResetMdlNodePartsLite; the linker folded them
    // into 0x004a0160, but the dispatcher still sees distinct targets.
    void ResetCameraLite(MdlNode* pNode, int nBase, int nRaw);
    void ResetEmitterLite(MdlNode* pNode, int nBase, int nRaw);
    void ResetReferenceLite(MdlNode* pNode, int nBase, int nRaw);
    void ResetPatchLite(MdlNode* pNode, int nBase, int nRaw);
    void ResetSkin(MdlNodeSkin* pNode, int nBase, int nRaw);
    void ResetTriMesh(MdlNodeTriMesh* pNode, int nBase, int nRaw);
    void ResetAABBTree(AABB* pAABB, int nBase, int nRaw);
    void ResetLightsaber(MdlNodeLightsaber* pNode, int nBase, int nRaw);
    void ResetLightsaberLite(MdlNodeLightsaber* pNode, int nBase, int nRaw);
    void ResetLight(MdlNodeLight* pNode, int nBase, int nRaw);
    void ResetLightLite(MdlNodeLight* pNode, int nBase, int nRaw);
    void ResetMdlNode(MdlNode* pNode, int nBase, int nRaw);
    void ResetMdlNodeLite(MdlNode* pNode, int nBase, int nRaw);
    void ResetMdlNodeParts(MdlNode* pNode, int nBase, int nRaw);
    void ResetTriMeshParts(MdlNodeTriMesh* pNode, int nBase, int nRaw);
    void ResetTriMeshPartsLite(MdlNodeTriMesh* pNode, int nBase, int nRaw);
    void ResetAnimation(Animation* pAnim, int nBase, int nRaw);
    Model* Reset(Model* pModel, int nRaw, void* pResource);
    Model* ResetLite(Model* pModel, int nRaw, void* pResource);

    unsigned int m_nMdlSize;            // +0x00
    unsigned int m_nMdxSize;            // +0x04
};

#endif
