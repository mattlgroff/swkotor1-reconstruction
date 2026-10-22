// CAurTexture references and SafePointer<CAurTexture>
// (construction, release, the NULL texture). This unit sees the body of
// CExoArrayList::Remove (the original knows it cannot throw: the ctor's SEH
// frame has no state stores); the CAurTextureBasic unit does not.
#define G_EXO_ARRAY_LIST_REMOVE
#include "g/aur_texture.h"

// Deletes a texture reference nobody points at any more (inlined in
// ~CAurTexture).
inline void ReleaseTexture(CAurTexture* pTexture)
{
    if (pTexture && pTexture->m_lRefs.num == 0) {
        if (pTexture->AsBasic())
            delete pTexture->AsBasic();
        else
            delete pTexture;
    }
}

inline void CAurTexture::SetTarget(CAurTexture* pTarget)
{
    CAurTexture* pOld = m_pTarget.m_p;
    m_pTarget = pTarget;
    ReleaseTexture(pOld);
}

// FUNCTION: SWKOTOR 0x00420d70
void AurTextureRelease(CAurTexture* pTexture)
{
    ReleaseTexture(pTexture);
}

// FUNCTION: SWKOTOR 0x004204e0 SYMBOL: ??0?$SafePointer@VCAurTexture@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x00422380 SYMBOL: ??1?$SafePointer@VCAurTexture@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x00447fa0 SYMBOL: ??4?$SafePointer@VCAurTexture@@@@QAEAAV0@PAVCAurTexture@@@Z
template class SafePointer<CAurTexture>;

// FUNCTION: SWKOTOR 0x00422d60 SYMBOL: ??1?$SafePointerList@VCAurTexture@@@@QAE@XZ
template class SafePointerList<CAurTexture>;

// FUNCTION: SWKOTOR 0x00422e30
CAurTexture::CAurTexture()
{
    m_pTarget = 0;
    m_n14 = 0;
}

// FUNCTION: SWKOTOR 0x00422e90
CAurTexture::~CAurTexture()
{
    SetTarget(0);
}

// FUNCTION: SWKOTOR 0x00423130 SYMBOL: ??_GCAurTexture@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004230c0
CAurTexture* AurTextureGetNULL()
{
    if (!g_pNullTexture.m_p)
        g_pNullTexture = new CAurTexture;
    return g_pNullTexture.m_p;
}

