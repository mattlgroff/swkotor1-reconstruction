// 0x00690940 (CSWCVisualEffect::Announce) lives in sw_c_visual_effect.cpp.

#include "shared/exo_linked_list.h"

class CClientVFX {
public:
    virtual ~CClientVFX();
};

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x00690910
void UnloadCachedVFX()
{
    while (g_lCachedVFX->pHead) {
        CClientVFX* pVFX = (CClientVFX*)g_lCachedVFX->RemoveHead();
        if (pVFX)
            delete pVFX;
    }
}

// 0x00690960, the area-of-effect object's scalar deleting destructor, is
// annotated in sw_c_visual_effect.cpp.
