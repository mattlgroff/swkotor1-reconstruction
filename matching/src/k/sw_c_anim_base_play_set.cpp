// CSWCAnimBase::PlayAnimSetOnModel: plays one animation of the set on a
// model, choosing the loop / blend flags from the animation id.
#include "k/sw_c_anim_base.h"
#include <stdio.h>

class C2DA {
public:
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue);  // 0x00413190
};

class C2DArrays {
public:
    char pad_0x0[0x3c];
    C2DA* m_pAnimations;                            // 0x3c
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays* m_p2DArrays;                         // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nAnimNameColumn;                       // 0x007A2174

// FUNCTION: SWKOTOR 0x0069E620
// NONMATCH: register allocation only (the original keeps nAnim in edi and
// the cut index in esi and never spills this; ours spills this and holds
// the index in eax). Size, temporaries, EH states and flow agree; nRow /
// nIndex scoping variants tried.
CExoString CSWCAnimBase::GetAnimationName(unsigned short nAnim)
{
    unsigned short nRow = ServerToClientAnimation(nAnim);
    CExoString sName;
    if (!g_pRules->m_p2DArrays->m_pAnimations->GetCExoStringEntry(nRow, g_nAnimNameColumn, &sName)) {
        sName = "";
    }
    if (sName.GetLength() > 0) {
        return sName;
    }
    CExoString sSuffix;
    int nIndex = -1;
    if (nAnim >= 1000 && nAnim <= 1127) {
        nIndex = nAnim - 1000;
    }
    else if (nAnim >= 1200 && nAnim <= 1327) {
        sSuffix = "w";
        nIndex = nAnim - 1200;
    }
    else if (nAnim >= 1400 && nAnim <= 1527) {
        sSuffix = "l";
        nIndex = nAnim - 1400;
    }
    else if (nAnim >= 1600 && nAnim <= 1727) {
        sSuffix = "wl";
        nIndex = nAnim - 1600;
    }
    if (++nIndex != 0) {
        char szNum[4];
        sprintf(szNum, "%03d", nIndex);
        CExoString sNum(szNum);
        sName = CExoString("cut") + sNum + sSuffix;
    }
    if (sName.IsEmpty() && nAnim != 10098) {
        nRow = ServerToClientAnimation(10000);
        if (!g_pRules->m_p2DArrays->m_pAnimations->GetCExoStringEntry(nRow, g_nAnimNameColumn, &sName)) {
            sName = "";
        }
        if (sName.GetLength() > 0) {
            return sName;
        }
    }
    return sName;
}

// FUNCTION: SWKOTOR 0x0069E420
int CSWCAnimBase::PlayAnimSetOnModel(CAurObject* pModel, int nAnim, int bLoop)
{
    CExoString sAnim;
    if (!pModel) {
        return 0;
    }
    unsigned short nFlags;
    if (!bLoop) {
        switch ((unsigned short)nAnim) {
        case 0x272d:
        case 0x272e:
        case 0x2732:
        case 0x2736:
        case 0x2737:
        case 0x2738:
        case 0x2739:
        case 0x273a:
        case 0x2756:
        case 0x2757:
        case 0x2798:
        case 0x27a1:
        case 0x27aa:
        case 0x27ab:
            nFlags = 0x400;
            break;
        default:
            nFlags = 0x80;
            break;
        }
    }
    else {
        nFlags = 8;
    }
    float fSpeed = 1.0f;
    sAnim = GetAnimationName(nAnim);
    switch ((unsigned short)nAnim) {
    case 0x272e:
    case 0x2736:
    case 0x2737:
    case 0x2738:
    case 0x2739:
    case 0x273a:
    case 0x27aa:
    case 0x27ab:
        break;
    case 0x175:
        fSpeed = -1.0f;
    default:
        nFlags |= 1;
        break;
    }
    if (sAnim == "pause1") {
        return 0;
    }
    return pModel->PlayAnimation(sAnim.CStr(), fSpeed, nFlags, 0);
}
