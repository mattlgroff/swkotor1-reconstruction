struct Vector3 {
    float x;
    float y;
    float z;
};

class CClientExoApp {
public:
    void LookUpAndPerformRumbleWithCutOff(int nEffect, int nFlags, int nPosition);
};

class CAppManager {
public:
    char pad_0x0[4];
    CClientExoApp* m_pClientExoApp; // +0x4
};

extern CAppManager* g_pAppManager; // 0x007A39FC

class CSWBehaviorAttachSound {
public:
    void AnnounceAreaVFX();
    char pad_0x0[0x24];
    Vector3 m_vPosition;            // +0x24
    char pad_0x30[0xd8];
    int m_nEffect;                  // +0x108
};

// FUNCTION: SWKOTOR 0x00690940
void CSWBehaviorAttachSound::AnnounceAreaVFX()
{
    g_pAppManager->m_pClientExoApp->LookUpAndPerformRumbleWithCutOff(
        m_nEffect, 1, (int)&m_vPosition);
}

class CExoLinkedListInternal {
public:
    int m_nCount;                     // +0x0
    void* RemoveHead();               // 0x005E9C80
};

class CClientVFX {
public:
    virtual ~CClientVFX();
};

extern CExoLinkedListInternal* g_lCachedVFX; // 0x00833BC8

// FUNCTION: SWKOTOR 0x00690910
void UnloadCachedVFX()
{
    while (g_lCachedVFX->m_nCount != 0) {
        CClientVFX* pVFX = (CClientVFX*)g_lCachedVFX->RemoveHead();
        if (pVFX)
            delete pVFX;
    }
}

#include <stdlib.h>

class CAreaVFXObject {
public:
    void Destroy();
};

// FUNCTION: SWKOTOR 0x00690960
void* __fastcall area_vfx_scalar_delete_00690960(CAreaVFXObject* pObject, void*, unsigned int nFlags)
{
    pObject->Destroy();
    if (nFlags & 1)
        free(pObject);
    return pObject;
}
