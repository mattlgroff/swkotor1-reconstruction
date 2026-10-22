// MdlNode -> Part instancing (MdlNode*::InternalCreateInstance,
// 0x00447b20-0x00448120) and the MdlNode ConJoin factories plus the small
// MdlNode subclass constructors (0x0043fd70, 0x00445350, 0x0044a390-
// 0x0044ae60). Behavioural reference: src/batches/b0044_gob_parts.cpp,
// b0044_mdlnode_lifecycle.cpp, b0044_part_ctors.cpp. Self-contained: the
// subclass constructors that the original inlined into the factories are
// written inline here.
#ifndef G_MDL_CREATE_H
#define G_MDL_CREATE_H

#include "g/exo_list.h"

void* __cdecl operator new(size_t n);           // 0x006fa7e6
void __cdecl operator delete(void* p);          // 0x006fa390

class Part;
class Gob;

// ---------------------------------------------------------------- MdlNode
class MdlNode {
public:
    unsigned short m_nType;     // +0x00
    char pad02[0x50 - 0x02];

    MdlNode(char* szName);                      // 0x00445210
    char* GetName();                            // 0x00438a00

    Part* InternalCreateInstance(Gob* pGob);    // 0x00447b20
    MdlNode* ConJoin();                         // 0x00445350
    MdlNode* ConJoinLight();                    // 0x0044a4a0
    MdlNode* ConJoinEmitter();                  // 0x0044a390
    MdlNode* ConJoinReference();                // 0x0044a5e0
    MdlNode* ConJoinTriMesh();                  // 0x0044a850
    MdlNode* ConJoinSkin();                     // 0x0044ad20
    MdlNode* ConJoinAnimMesh();                 // 0x0044ae60
    MdlNode* ConJoinTrigger();                  // 0x0043fd70
};

class MdlNodeTrigger : public MdlNode {
public:
    MdlNodeTrigger(char* szName);               // 0x0049d7b0
};

class MdlNodeEmitter : public MdlNode {
public:
    char pad50[0x130 - 0x50];

    MdlNodeEmitter(char* szName);               // 0x0044a300
};

class MdlNodeLight : public MdlNode {
public:
    char pad50[0x94 - 0x50];
    int m_n94;                  // +0x94
    int m_bAffectDynamic98;     // +0x98
    int m_n9c;                  // +0x9c
    int m_na0;                  // +0xa0
    char pada4[0xac - 0xa4];

    MdlNodeLight(char* szName);                 // 0x0044a3f0
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447b90
};

class MdlNodeCamera : public MdlNode {
public:
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447c80
};

class MdlNodeReference : public MdlNode {
public:
    char pad50[0x74 - 0x50];

    MdlNodeReference(char* szName) : MdlNode(szName) { m_nType = 0x11; }
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447ce0
};

class MdlNodeTriMesh : public MdlNode {
public:
    char pad50[0x19c - 0x50];

    MdlNodeTriMesh(char* szName);               // 0x0044a640
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447d60
};

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    char pad19c[0x1d4 - 0x19c];

    MdlNodeAnimMesh(char* szName);              // 0x0044abc0
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447dd0
};

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    char pad19c[0x200 - 0x19c];

    MdlNodeSkin(char* szName);                  // 0x0044ac70
};

class AABB;

class MdlNodeAABB : public MdlNodeTriMesh {
public:
    AABB* m_pTree;              // +0x19c

    MdlNodeAABB(char* szName);                  // 0x0044a8b0
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447e30
};

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    Part* InternalCreateInstance(Gob* pGob);    // 0x00447e90
};

class MdlNodeLightsaber : public MdlNodeTriMesh {
public:
    Part* InternalCreateInstance(Gob* pGob);    // 0x004480c0
};

// ------------------------------------------------------------------- Part
class Part {
public:
    char pad04[0x48 - 0x04];
    int m_n48;                  // +0x48

    Part(MdlNode* pNode);                       // 0x004457a0
    virtual ~Part();
};

class PartTriMesh : public Part {
public:
    char pad4c[0x98 - 0x4c];

    PartTriMesh(MdlNode* pNode);                // 0x00445840
    virtual ~PartTriMesh();
};

class PartLight : public Part {
public:
    char pad4c[0x5c - 0x4c];
    int m_n5c;                  // +0x5c
    char pad60[0x68 - 0x60];
    int m_n68;                  // +0x68
    int m_n6c;                  // +0x6c
    int m_n70;                  // +0x70
    char pad74[0xa0 - 0x74];

    PartLight(MdlNode* pNode);                  // 0x00445a40
    virtual ~PartLight();
};

class PartCamera : public Part {
public:
    PartCamera(MdlNode* pNode) : Part(pNode) {}
    virtual ~PartCamera();
};

class PartReference : public Part {
public:
    void* m_pReferenced;        // +0x4c
    char pad50[0x54 - 0x50];

    PartReference(MdlNode* pNode) : Part(pNode) { m_pReferenced = 0; }
    virtual ~PartReference();
};

class PartAnimMesh : public PartTriMesh {
public:
    PartAnimMesh(MdlNode* pNode) : PartTriMesh(pNode) {}
    virtual ~PartAnimMesh();
};

class PartAABB : public PartTriMesh {
public:
    PartAABB(MdlNode* pNode) : PartTriMesh(pNode) {}
    virtual ~PartAABB();
};

class PartDanglyMesh : public PartTriMesh {
public:
    char pad98[0xbc - 0x98];

    PartDanglyMesh(MdlNode* pNode);             // 0x00447980
    virtual ~PartDanglyMesh();
};

class PartLightsaber : public PartTriMesh {
public:
    char pad98[0x3cc - 0x98];

    PartLightsaber(MdlNode* pNode);             // 0x004478d0
    virtual ~PartLightsaber();
};

// -------------------------------------------------------------------- Gob
class Gob {
public:
    char pad00[0xbc];
    int m_bHasLights;           // +0xbc
    char padc0[0x19c - 0xc0];
    CExoArrayList<Part*> m_lLights;     // +0x19c
};

#endif
