// CExoArrayList::Allocate for the model face / skin weight
// element types, and the face constructors their array news call.
#include "g/mdl_node.h"

extern int g_nTriFaceCount;         // 0x007a8a00

// Skin vertex weight record (0x34 bytes, plain data).
struct SkinVertexWeight {
    int m_a[13];
};

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

// FUNCTION: SWKOTOR 0x0043e0d0
MaxFace_PostProcess::MaxFace_PostProcess()
{
    m_pVerts = 0;
    m_pTVerts = 0;
    m_pNormals = 0;
    m_nVerts = 0;
    m_nTVerts = 0;
}

// FUNCTION: SWKOTOR 0x0043f710
TriFace::TriFace()
{
    m_n00 = 0;
    m_n04 = 0;
    m_n08 = 0;
    g_nTriFaceCount++;
    m_n10 = 0;
}

// FUNCTION: SWKOTOR 0x0043f5e0 SYMBOL: ?Allocate@?$CExoArrayList@VMaxFace_PostProcess@@@@QAEXH@Z
template void CExoArrayList<MaxFace_PostProcess>::Allocate(int n);

// FUNCTION: SWKOTOR 0x004450d0 SYMBOL: ?Allocate@?$CExoArrayList@VTriFace@@@@QAEXH@Z
template void CExoArrayList<TriFace>::Allocate(int n);

// FUNCTION: SWKOTOR 0x0043f6b0 SYMBOL: ?Allocate@?$CExoArrayList@USkinVertexWeight@@@@QAEXH@Z
template void CExoArrayList<SkinVertexWeight>::Allocate(int n);
