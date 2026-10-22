// the sorted global Gob list (element 0x7a8a58, num 0x7a8a5c) and
// the instance-tree entry point.
#include "g/exo_list.h"

extern "C" int __cdecl _stricmp(const char* a, const char* b);    // 0x0070acaf

class Gob {
public:
    char pad00[0x38];
    char m_szName[0x20];                    // +0x38
};

extern CExoArrayList<Gob*> g_lGobs;         // 0x007a8a58

// FUNCTION: SWKOTOR 0x00440410
// NONMATCH (3): the original loads szName into ebp in the prologue, ahead of
// the `num - 1 < 0` guard; ours sinks it into the loop preheader (also tried:
// hi before lo, explicit guard + do-while).
int __cdecl GobListFindIndex(char* szName)
{
    int lo = 0;
    int hi = g_lGobs.num - 1;
    while (lo <= hi) {
        int mid = (hi + lo) >> 1;
        int c = _stricmp(g_lGobs.element[mid]->m_szName, szName);
        if (c > 0)
            hi = mid - 1;
        else if (c < 0)
            lo = mid + 1;
        else
            return mid;
    }
    return lo;
}

// FUNCTION: SWKOTOR 0x00440460
Gob* __cdecl GobListFind(char* szName)
{
    int i = GobListFindIndex(szName);
    if (i < g_lGobs.num && !_stricmp(g_lGobs.element[i]->m_szName, szName))
        return g_lGobs.element[i];
    return 0;
}

class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68();
    virtual void Finalize(Part* pRoot);     // +0x6c
};

class MdlNode;

Part* __cdecl CreateInstanceTreeR(MdlNode* pNode, Gob* pGob);     // 0x00449200

// FUNCTION: SWKOTOR 0x00449410
Part* __cdecl CreateInstanceTree(MdlNode* pNode, Gob* pGob)
{
    Part* pRoot = CreateInstanceTreeR(pNode, pGob);
    pRoot->Finalize(pRoot);
    return pRoot;
}
