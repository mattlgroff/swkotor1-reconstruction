// PartAABB::HitCheck (vtable 0x741048 +0x20): publish the walk state and
// descend the walkmesh AABB tree. Behavioural reference:
// src/units/0732/4526b4e7fa-f1_part_core.cpp.
#include "g/part.h"

class AABBNode;

class MdlNodeAABB {
public:
    char pad00[0x19c];
    AABBNode* m_pAABBRoot;              // +0x19c
};

extern MdlNodeTriMesh* g_pHitAABBMesh;  // 0x007a69b0
extern CHitInfo* g_pHitAABBInfo;        // 0x007a69b4
extern int g_nHitAABBDirection;         // 0x007a8a24

int HitCheckAABBNode(AABBNode* pNode, const Vector* pStart, const Vector* pEnd,
                     float fScale);     // 0x00441570

// NONMATCH (7): the original hoists the first fld/fcomp above the two
// global stores and keeps the mask in edi (`or edi, eax`), storing it after
// the AABB argument pushes; tried expression / statement forms and moving
// the stores.
// FUNCTION: SWKOTOR 0x004428f0
int PartAABB::HitCheck(const Vector& vStart, const Vector& vEnd, Vector* pHit,
                       CHitInfo* pInfo, float fScale)
{
    PartNode* pNode = m_pNode;
    g_pHitAABBMesh = pNode->AsMdlNodeTriMesh();
    g_pHitAABBInfo = pInfo;
    int nDirection = 1 << (vStart.x < vEnd.x ? 0 : 3);
    nDirection |= 1 << (vStart.y < vEnd.y ? 1 : 4);
    nDirection |= 1 << (vStart.z < vEnd.z ? 2 : 5);
    g_nHitAABBDirection = nDirection;
    int nResult = HitCheckAABBNode(pNode->AsMdlNodeAABB()->m_pAABBRoot, &vStart, &vEnd, fScale);
    if (nResult && pHit)
        *pHit = vEnd;
    return nResult;
}
