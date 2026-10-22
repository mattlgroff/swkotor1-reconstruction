// MaxTree / Model / MdlNode / Part declarations for the model
// lifecycle cluster (0x0043e0f0-0x0044b9e0; behavioural reference:
// src/batches/b0044_mdlnode_lifecycle.cpp, b0044_gob_parts.cpp,
// b0044_part_ctors.cpp). Only the members the matched bodies touch are
// spelled out; the rest is padding.
#ifndef G_MDL_NODE_H
#define G_MDL_NODE_H

#include "g/exo_list.h"

// MdlNode type flags (first word of every MdlNode).
enum {
    MDLNODE_HEADER = 0x0001,
    MDLNODE_LIGHT = 0x0002,
    MDLNODE_EMITTER = 0x0004,
    MDLNODE_CAMERA = 0x0008,
    MDLNODE_REFERENCE = 0x0010,
    MDLNODE_MESH = 0x0020,
    MDLNODE_SKIN = 0x0040,
    MDLNODE_ANIM = 0x0080,
    MDLNODE_DANGLY = 0x0100,
    MDLNODE_AABB = 0x0200,
    MDLNODE_SABER = 0x0800
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Quaternion {
public:
    float x, y, z, w;

    Vector ToDirection();
};

// One face of the MDL post-process face list (0x14 bytes).
class MaxFace_PostProcess {
public:
    int* m_pVerts;          // +0x00
    int* m_pTVerts;         // +0x04
    int m_nVerts;           // +0x08
    int m_nTVerts;          // +0x0c
    int* m_pNormals;        // +0x10

    MaxFace_PostProcess();                      // 0x0043e0d0
    ~MaxFace_PostProcess();
};

// Triangle face (0x20 bytes); its destructor only drops a live count.
class TriFace {
public:
    int m_n00;              // +0x00
    int m_n04;              // +0x04
    int m_n08;              // +0x08
    int m_n0c;              // +0x0c
    int m_n10;              // +0x10
    char pad14[0x20 - 0x14];

    TriFace();                                  // 0x0043f710
    ~TriFace();
};

class MdlNode;
class Model;
class Animation;

class MaxTree {
public:
    char pad00[0x30];
    CExoArrayList<void*> m_lFreeList;       // +0x30
    CExoArrayList<char*> m_lFreeTokenList;  // +0x3c
    char pad48[0x4c - 0x48];
    unsigned char m_nType;  // +0x4c (low 7 bits)
    char pad4d[0x54 - 0x4d];
    int m_nRefCount;        // +0x54

    int GetType();
    Model* AsModel();
    Animation* AsAnimation();
    static void operator delete(void* p);       // 0x0044aec0
    void AddToFreeTokenList(char* p);
    void ResetFreeLists();
};

class Model : public MaxTree {
public:
    int DelRef(int nUnused);
    int AddRef(int nUnused);
};

class Animation : public MaxTree {
};

class MdlNodeAABB;
class MdlNodeAnimMesh;
class MdlNodeCamera;
class MdlNodeDanglyMesh;
class MdlNodeLightsaber;
class MdlNodeEmitter;
class MdlNodeLight;
class MdlNodeReference;
class MdlNodeSkin;
class MdlNodeTriMesh;

class MdlNode {
public:
    unsigned short m_nType;     // +0x00
    int m_nIndex;               // +0x04
    MaxTree* m_pTree;           // +0x08

    void AddToFreeList(void* p);
    void AddToFreeTokenList(char* p);

    ~MdlNode();                 // 0x0044a040
    MdlNodeAABB* AsMdlNodeAABB();
    MdlNodeAnimMesh* AsMdlNodeAnimMesh();
    MdlNodeCamera* AsMdlNodeCamera();
    MdlNodeDanglyMesh* AsMdlNodeDanglyMesh();
    MdlNodeLightsaber* AsMdlNodeLightsaber();
    MdlNodeEmitter* AsMdlNodeEmitter();
    MdlNodeLight* AsMdlNodeLight();
    MdlNodeReference* AsMdlNodeReference();
    MdlNodeSkin* AsMdlNodeSkin();
    MdlNodeTriMesh* AsMdlNodeTriMesh();
};

class MdlNodeTrigger : public MdlNode {
public:
    ~MdlNodeTrigger();
};

class MdlNodeAABB : public MdlNode {};
class MdlNodeAnimMesh : public MdlNode {};
class MdlNodeCamera : public MdlNode {};
class MdlNodeDanglyMesh : public MdlNode {};
class MdlNodeLightsaber : public MdlNode {};
class MdlNodeEmitter : public MdlNode {};
class MdlNodeLight : public MdlNode {};
class MdlNodeReference : public MdlNode {};
class MdlNodeSkin : public MdlNode {};

class MdlNodeTriMesh : public MdlNode {
public:
    char pad0c[0x170 - 0xc];
    int m_nTangentSpace;        // +0x170
    unsigned char HasTangentBases();
};

// Axis-aligned bounding box tree node.
class AABB {
public:
    char pad00[0x18];
    AABB* m_pLeft;              // +0x18
    AABB* m_pRight;             // +0x1c

    ~AABB()
    {
        if (m_pLeft)
            delete m_pLeft;
        if (m_pRight)
            delete m_pRight;
    }
};

#endif
