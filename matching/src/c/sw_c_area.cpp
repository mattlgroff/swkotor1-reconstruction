// CSWCArea::AddCorpse / SetAsDay (b005f3_client_internal_lists).

#include "c/client_exo_app.h"

struct Vector {
    float x, y, z;
};

extern float g_fFogDensity;                         // 0x0078E6A0
extern float g_fAmbientScale;                       // 0x0078E408

class CSWCArea {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual void SetSunColor(int b, Vector v);      // 0x14
    virtual void SetMoonColor(int b, Vector v);     // 0x18
    virtual void v1C();
    virtual void SetFog(unsigned long a, unsigned long b, Vector* pColor);  // 0x20
    virtual ~CSWCArea();                            // slot unknown

    CSWCArea();

    void AddCorpse(unsigned long oid);
    void SetAsDay();
    void SetAsNight();                              // 0x006059C0

    char pad_0x4[0x8c];
    unsigned long m_nFogA;                          // 0x90
    unsigned long m_nFogB;                          // 0x94
    char pad_0x98[0x8];
    int m_bIsNight;                                 // 0xa0
    int m_bDayNightPending;                         // 0xa4
    char pad_0xa8[0xc];
    unsigned char m_nAmbientScale;                  // 0xb4
    char pad_0xb5[0x97];
    Vector m_vMoonColor;                            // 0x14c
    Vector m_vSunColor;                             // 0x158
    Vector m_vFogColor;                             // 0x164
    char pad_0x170[0x18];
    int m_bLightingDirty;                           // 0x188
    char pad_0x18c[0x28];
    int m_nUnknown1b4;                              // 0x1b4
    int m_nUnknown1b8;                              // 0x1b8
    char pad_0x1bc[0x4];
    int m_bUnknown1c0;                              // 0x1c0
    char pad_0x1c4[0xa4];
    unsigned long m_aCorpses[4];                    // 0x268
    unsigned char m_nCorpseRead;                    // 0x278
    unsigned char m_nCorpseWrite;                   // 0x279
};

// FUNCTION: SWKOTOR 0x006056B0
void CSWCArea::AddCorpse(unsigned long oid)
{
    m_aCorpses[m_nCorpseWrite] = oid;
    if (++m_nCorpseWrite >= 4) {
        m_nCorpseWrite = 0;
    }
    if (m_nCorpseRead == m_nCorpseWrite) {
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_aCorpses[m_nCorpseRead]);
        if (pCreature) {
            CSWCPlaceable* pPlaceable = g_pAppManager->m_pClientExoApp->GetPlaceableByGameObjectID(pCreature->m_oidCorpseContainer);
            if (pPlaceable) {
                CSWCPlaceableAppearance* pAppearance = pPlaceable->GetAppearance();
                if (pAppearance) {
                    pAppearance->m_bCorpseFaded = 1;
                }
                pPlaceable->PlayAnimation(1, 0, 0, 0);
            }
            pCreature->SetCorpseState(1);
            pCreature->PlayAnimation(0, 1, 45000, 1000);
            pCreature->m_bCorpse = 1;
        }
        m_aCorpses[m_nCorpseRead] = OBJECT_INVALID;
        if (++m_nCorpseRead >= 4) {
            m_nCorpseRead = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x006058C0
void CSWCArea::SetAsDay()
{
    if (!m_bIsNight && m_bDayNightPending) {
        SetAsNight();
        return;
    }
    g_fFogDensity = 0.05f;
    g_fAmbientScale = (float)m_nAmbientScale * 0.01f;
    m_bUnknown1c0 = 1;
    m_nUnknown1b4 = 0;
    m_nUnknown1b8 = 0;
    m_bDayNightPending = 0;
    SetFog(m_nFogA, m_nFogB, &m_vFogColor);
    SetMoonColor(1, m_vMoonColor);
    SetSunColor(1, m_vSunColor);
    if (m_bLightingDirty) {
        CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
        if (pPlayer) {
            pPlayer->GetCreatureLight(1);
        }
    }
}

// Defined only to emit the vtable and its scalar deleting destructor.
CSWCArea::CSWCArea()
{
}

// FUNCTION: SWKOTOR 0x0060A2E0 SYMBOL: ??_GCSWCArea@@UAEPAXI@Z
// CSWCArea::`scalar deleting destructor' (compiler generated).
