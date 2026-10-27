// CClientExoAppInternal::LookUpAndPerformRumbleWithCutOff, worker C s8
// (clientexoappinternal.o; translation-unit-local layout).

#include "c/exo_base.h"

struct Vector {
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);                   // 0x00413430
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);                       // 0x00413660
    int GetFLOATEntry(const CExoString& sRow, const CExoString& sColumn, float* pValue);     // 0x00413FA0
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);         // 0x00414110
};

class CTwoDimArrays {
public:
    char pad_0x0[0x58];
    C2DA* m_pVisualEffects;                         // 0x58 (descriptive)
    char pad_0x5c[0x88 - 0x5c];
    C2DA* m_pRumbleEffects;                         // 0x88 (descriptive)
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCCreature {
public:
    char pad_0x0[0x24];
    Vector m_vPosition;                             // 0x24
};

class CGuiInGame {
public:
    char pad_0x0[0xb4];
    void* m_pListenerOverride;                      // 0xb4 (descriptive)
};

class CClientExoApp {
public:
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGuiInGame* GetInGameGui();                     // 0x005ED690
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CExoSound {
public:
    void GetListenerPosition(Vector* pPosition);    // 0x005D63C0
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CClientExoAppInternal {
public:
    int PlayRumblePattern(int nPattern);            // 0x005FB490
    void LookUpAndPerformRumbleWithCutOff(int nEffect, unsigned char nType, const Vector& vPosition);
};

// NONMATCH (49 diff insns): stack packing (the original frame is 8 bytes
// larger and shares the label temporary's slot with nRow) and the register
// choice for bByLabel/nRow that follows. An int flag makes MSVC keep 0 in
// ebx for every zero store; the original uses immediates.
// FUNCTION: SWKOTOR 0x005FB960
void CClientExoAppInternal::LookUpAndPerformRumbleWithCutOff(int nEffect, unsigned char nType, const Vector& vPosition)
{
    CExoString sRow;
    C2DA* pTable;
    unsigned char bByLabel;
    int nRow;
    switch (nType) {
    case 0:
        pTable = g_pRules->m_p2DArrays->m_pVisualEffects;
        bByLabel = 0;
        nRow = nEffect;
        break;
    case 1:
        pTable = g_pRules->m_p2DArrays->m_pRumbleEffects;
        bByLabel = 1;
        sRow = CExoString(nEffect);
        break;
    default:
        return;
    }
    if (!pTable) {
        return;
    }
    int nPattern;
    if (bByLabel) {
        if (!pTable->GetINTEntry(sRow, CExoString("RumblePattern"), &nPattern)) {
            return;
        }
    } else {
        if (!pTable->GetINTEntry(nRow, CExoString("RumblePattern"), &nPattern)) {
            return;
        }
    }
    CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
    if (!pPlayer) {
        return;
    }
    float fCutOff;
    unsigned char bMissing;
    if (bByLabel) {
        bMissing = !pTable->GetFLOATEntry(sRow, CExoString("RumbleCutOff"), &fCutOff);
    } else {
        bMissing = !pTable->GetFLOATEntry(nRow, CExoString("RumbleCutOff"), &fCutOff);
    }
    if (bMissing) {
        fCutOff = 10.0f;
    }
    Vector vListener(0.0f, 0.0f, 0.0f);
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pListenerOverride) {
        g_pExoSound->GetListenerPosition(&vListener);
    } else {
        vListener = pPlayer->m_vPosition;
    }
    Vector vDiff = vListener - vPosition;
    if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z <= fCutOff * fCutOff) {
        PlayRumblePattern(nPattern);
    }
}
