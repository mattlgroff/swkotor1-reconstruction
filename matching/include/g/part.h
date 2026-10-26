// the Part instance hierarchy (CAurPart -> Part -> PartTriMesh ->
// PartSkin / PartAABB / PartDanglyMesh / PartLightsaber, PartLight,
// PartCamera, PartReference) plus the CExoArrayList pieces their lifecycles
// use (behavioural reference: src/batches/b0044_part_ctors.cpp,
// b0044_graphics_dtors.cpp, b0044_gob_parts.cpp).
#ifndef G_PART_H
#define G_PART_H

#include "g/exo_list.h"

class Material {
public:
    ~Material();                    // 0x0047b980
};

class CAurObject {
public:
    virtual ~CAurObject();
};

class Part;

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

void __cdecl AurTextureRelease(int nTexture);      // 0x00423f50
extern "C" int __cdecl _stricmp(const char* a, const char* b); // 0x0070acaf
Part* __cdecl FindPart(Part* pRoot, char* szName);   // 0x0043fe00

extern int g_nPartCount;            // 0x007a8a10
extern int g_nPartTriMeshCount;     // 0x007a8a14

class CAurPart {
public:
    virtual ~CAurPart() {}
};

// The model node a part instantiates (MdlNode +0x04 index, +0x08 tree).
class PartModel {
public:
    char pad00[0x4c];
    unsigned char m_nType;          // +0x4c (low 7 bits)
    char pad4d[0xb8 - 0x4d];
    void** m_pNodeData;             // +0xb8

    PartModel* AsModel() { return (m_nType & 0x7f) == 2 ? this : 0; }
};

class MdlNodeAnimMesh;

class PartNode {
public:
    unsigned short m_nType;         // +0x00
    int m_nIndex;                   // +0x04
    PartModel* m_pModel;            // +0x08

    MdlNodeAnimMesh* AsMdlNodeAnimMesh()
    {
        if ((m_nType & 0x0a1) == 0x0a1)
            return (MdlNodeAnimMesh*)this;
        return 0;
    }
};

class Part : public CAurPart {
public:
    PartNode* m_pNode;              // +0x04
    char pad08[0x30 - 0x08];
    CExoArrayList<Part*> m_lChildren;   // +0x30
    int m_n3c;                      // +0x3c
    void* m_p40;                    // +0x40
    Material* m_pMaterial;          // +0x44

    virtual ~Part();
    virtual void V04(); virtual void V08(); virtual void V0c();
    virtual void V10(); virtual void V14(); virtual void V18();
    virtual void V1c();
    virtual int HitCheck(const Vector& vStart, const Vector& vEnd, Vector* pHit,
                         int nUnused, int nFlags);   // +0x20
    virtual void V24();
    virtual void V28(); virtual void V2c(); virtual void V30();
    virtual void V34(); virtual void V38(); virtual void V3c();
    virtual void V40(); virtual void V44(); virtual void V48();
    virtual void V4c(); virtual void V50(); virtual void V54();
    virtual void V58(); virtual void V5c(); virtual void V60();
    virtual void V64(); virtual void V68();
    virtual void Propagate6c(int nArg);     // +0x6c
    virtual Vector SetPartPosition(Vector v);   // +0x70 (no-op, returns 0,0,0)

    void PropagateToChildren(int nArg);
    CExoArrayList<Part*>* GetChildren();
    void* GetP40();
    void* GetNodeData();
};

class PartTriMesh : public Part {
public:
    char pad48[0x6c - 0x48];
    Part* m_pRoot;                  // +0x6c
    CAurObject* m_pObject;          // +0x70
    int m_nTexture0;                // +0x74
    int m_nTexture1;                // +0x78
    unsigned char m_bOwnsTexture0;  // +0x7c
    char pad7d[0x90 - 0x7d];
    void* m_pVertexData;            // +0x90
    float m_f94;                    // +0x94

    virtual ~PartTriMesh();
    float GetF94();
    Part* FindNamedPart(char* szName);
};

class PartAnimMesh : public PartTriMesh {
public:
    virtual int HitCheck(const Vector& vStart, const Vector& vEnd, Vector* pHit,
                         int nUnused, int nFlags);   // +0x20
};

// The animated-mesh geometry test (name not recorded).
int __cdecl HitCheckAnimMesh(Part* pPart, MdlNodeAnimMesh* pNode, Vector vStart,
                             Vector vEnd, Vector* pHit, int nFlags);   // 0x00440cf0

class PartSkin : public PartTriMesh {
public:
    virtual ~PartSkin();
};

class PartAABB : public PartTriMesh {
public:
    PartAABB();
};

class PartDanglyMesh : public PartTriMesh {
public:
    char pad98[0xb4 - 0x98];
    void* m_pConstraints;           // +0xb4
    int m_nDanglyTexture;           // +0xb8

    virtual ~PartDanglyMesh();
};

class PartLightsaber : public PartTriMesh {
public:
    char pad98[0x3b8 - 0x98];
    short m_n3b8;                   // +0x3b8
    short m_n3ba;                   // +0x3ba
    short m_n3bc;                   // +0x3bc

    virtual ~PartLightsaber();
};

class PartCamera : public Part {
public:
    PartCamera();
};

class PartReference : public Part {
public:
    char pad48[0x4c - 0x48];
    CAurObject* m_pReferenced;      // +0x4c

    virtual ~PartReference();
};

class CAurObjectLightContainer {
public:
    CExoArrayList<void*> m_lLights;     // +0x04
    CExoArrayList<void*> m_lPriorities; // +0x10

    virtual ~CAurObjectLightContainer();
};

#endif
