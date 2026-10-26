// Client animation bases of placeables, doors, visual effects and triggers
// (0x006E4E50-0x006E6200). The constructors emit the vtables, so the scalar
// deleting destructors come from this translation unit.
#include "l/sw_c_anim_base_objects.h"

namespace {
const char s_szUnmappedAnimation[] =
    "CSWCAnimBasePlaceable::ServerToClientAnimation(): Failed to map server anim %i to client anim.";
}

// ---------------------------------------------------------------- placeable

// FUNCTION: SWKOTOR 0x006E4E50
CSWCAnimBasePlaceable::CSWCAnimBasePlaceable()
{
    m_nAnimType = 10;
}

// FUNCTION: SWKOTOR 0x006E4E70
CSWCAnimBasePlaceable::~CSWCAnimBasePlaceable()
{
    CSWCAnimBase::UnloadModel();
}

// FUNCTION: SWKOTOR 0x006E4EC0
int CSWCAnimBasePlaceable::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    char szModel[17];
    cResRef.CopyToString(szModel);
    m_pModel = NewCAurObject(szModel, "placeable", 0, 0);
    if (m_pModel == 0) {
        return 0;
    }
    m_cModelResRef = cResRef;
    m_bReplaceTextureIndexed = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x006E4F30
int CSWCAnimBasePlaceable::PlayAnimation(unsigned short nAnim, float fSpeed, int a, int nTransition)
{
    if (m_pModel == 0) {
        return 0;
    }
    CExoString sName = GetAnimationName(nAnim);
    if (fSpeed == 0.0f) {
        fSpeed = 1.0f;
    }
    int bLoop;
    switch (nAnim) {
    default:
        if (nAnim >= 0x3e8 && nAnim <= 0x52f) {
            bLoop = 1;
            break;
        }
        if (nAnim >= 0x578 && nAnim <= 0x6bf) {
            bLoop = 0;
            break;
        }
        m_pModel->PlayAnimation("", 1.0f, 0, nTransition);
        return 1;
    case 0x2710:
    case 0x2758:
    case 0x2759:
    case 0x275a:
    case 0x275b:
    case 0x275c:
        m_pModel->PlayAnimation(sName.CStr(), fSpeed, 0, nTransition);
        return 0;
    case 0x132:
    case 0x138:
    case 0x139:
    case 0x13a:
    case 0x13b:
    case 0x271e:
    case 0x275d:
    case 0x27e5:
    case 0x27e6:
    case 0x27e7:
    case 0x27e8:
        bLoop = 1;
        break;
    case 0x277a:
    case 0x277b:
    case 0x277c:
    case 0x277e:
    case 0x277f:
    case 0x2780:
    case 0x2781:
    case 0x2782:
    case 0x2783:
    case 0x2784:
        bLoop = 0;
        break;
    }
    m_pModel->PlayAnimation(sName.CStr(), fSpeed, bLoop, nTransition);
    return 1;
}

// FUNCTION: SWKOTOR 0x006E5180 SYMBOL: ??_GCSWCAnimBasePlaceable@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006E51A0
CExoString CSWCAnimBasePlaceable::GetAnimationName(unsigned short nAnim)
{
    unsigned short nClient = ServerToClientAnimation(nAnim);
    CExoString sName;
    if (!g_pRules->Get2DArrays()->GetAnimations()->GetCExoStringEntry(nClient, g_nAnimNameColumn, &sName)) {
        sName = "";
    }
    if (sName.GetLength() <= 0) {
        if (nAnim < 0x3e8 || nAnim > 0x6bf) {
            sName = "default";
        } else {
            CExoString sSuffix;
            int nIndex = -1;
            if (nAnim <= 0x467) {
                nIndex = nAnim - 0x3e8;
            } else if (nAnim >= 0x4b0 && nAnim <= 0x52f) {
                sSuffix = "w";
                nIndex = nAnim - 0x4b0;
            } else if (nAnim >= 0x578 && nAnim <= 0x5f7) {
                sSuffix = "l";
                nIndex = nAnim - 0x578;
            } else if (nAnim >= 0x640) {
                sSuffix = "wl";
                nIndex = nAnim - 0x640;
            }
            if (++nIndex != 0) {
                char szNumber[4];
                sprintf(szNumber, "%03d", nIndex);
                sName = CExoString("cut") + CExoString(szNumber) + sSuffix;
            }
        }
    }
    return sName;
}

// FUNCTION: SWKOTOR 0x006E53F0
unsigned short CSWCAnimBasePlaceable::ServerToClientAnimation(unsigned short nAnim)
{
    switch (nAnim) {
    case 0x2710:
        nAnim = 0x130;
        break;
    case 0x2758:
        nAnim = 0x133;
        break;
    case 0x2759:
        nAnim = 0x134;
        break;
    case 0x275a:
        nAnim = 0x135;
        break;
    case 0x271e:
    case 0x275d:
    case 0x27e5:
    case 0x27e6:
    case 0x27e7:
    case 0x27e8:
        nAnim = 0x131;
        break;
    case 0x275b:
        nAnim = 0x136;
        break;
    case 0x275c:
        nAnim = 0x137;
        break;
    case 0x277a:
        nAnim = 0x13c;
        break;
    case 0x277b:
        nAnim = 0x13d;
        break;
    case 0x277c:
        nAnim = 0x13e;
        break;
    case 0x277e:
        nAnim = 0x13f;
        break;
    case 0x277f:
        nAnim = 0x140;
        break;
    case 0x2780:
        nAnim = 0x141;
        break;
    case 0x2781:
        nAnim = 0x142;
        break;
    case 0x2782:
        nAnim = 0x143;
        break;
    case 0x2783:
        nAnim = 0x144;
        break;
    case 0x2784:
        nAnim = 0x145;
        break;
    default:
        if (nAnim > 0x270f) {
            char szMessage[256];
            sprintf(szMessage, s_szUnmappedAnimation, nAnim);
        }
        break;
    }
    return nAnim;
}

// --------------------------------------------------------------------- door

// FUNCTION: SWKOTOR 0x006E56A0
CSWCAnimBaseDoor::CSWCAnimBaseDoor()
{
    m_nAnimType = 5;
}

// FUNCTION: SWKOTOR 0x006E56C0
CSWCAnimBaseDoor::~CSWCAnimBaseDoor()
{
    CSWCAnimBase::UnloadModel();
}

// FUNCTION: SWKOTOR 0x006E5710
int CSWCAnimBaseDoor::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    if (nPart != 0xff) {
        return 0;
    }
    char szModel[17];
    cResRef.CopyToString(szModel);
    m_pModel = NewCAurObject(szModel, "door", 0, 0);
    if (m_pModel == 0) {
        char szMessage[128];
        sprintf(szMessage, "Cannot load door model '%s'.", szModel);
        return 0;
    }
    m_cModelResRef = cResRef;
    m_bReplaceTextureIndexed = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x006E57B0
int CSWCAnimBaseDoor::PlayAnimOnModel(CAurObject* pModel, unsigned short nAnim, float fSpeed, int a,
                                      int nTransition)
{
    if (pModel) {
        CExoString sName = GetAnimationName(nAnim);
        if (fSpeed == 0.0f) {
            fSpeed = 1.0f;
        }
        switch (nAnim) {
        case 0x2726:
        case 0x2742:
        case 0x2743:
        case 0x2758:
        case 0x27a9:
            pModel->PlayAnimation(sName.CStr(), fSpeed, 2, nTransition);
            break;
        case 0x149:
        case 0x14e:
        case 0x14f:
        case 0x150:
        case 0x151:
        case 0x271e:
        case 0x275d:
        case 0x27e5:
        case 0x27e6:
        case 0x27e7:
        case 0x27e8:
            pModel->PlayAnimation(sName.CStr(), fSpeed, 3, nTransition);
            break;
        default:
            pModel->PlayAnimation("", 1.0f, 0, nTransition);
            break;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006E59A0 SYMBOL: ??_GCSWCAnimBaseDoor@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006E59C0
CExoString CSWCAnimBaseDoor::GetAnimationName(unsigned short nAnim)
{
    unsigned short nClient = ServerToClientAnimation(nAnim);
    CExoString sName;
    if (!g_pRules->Get2DArrays()->GetAnimations()->GetCExoStringEntry(nClient, g_nAnimNameColumn, &sName)) {
        sName = "";
    }
    if (sName.GetLength() <= 0) {
        CExoString sMessage;
        sMessage.Format("CSWCAnimBaseDoor::GetAnimationName(): No name for server animation %d", nAnim);
    }
    return sName;
}

// FUNCTION: SWKOTOR 0x006E5AC0
unsigned short CSWCAnimBaseDoor::ServerToClientAnimation(unsigned short nAnim)
{
    switch (nAnim) {
    case 0x2742:
        nAnim = 0x14b;
        break;
    case 0x2743:
        nAnim = 0x14c;
        break;
    case 0x2726:
        nAnim = 0x14d;
        break;
    case 0x2758:
        nAnim = m_nD0 ? 0x14a : 0x147;
        break;
    case 0x271e:
    case 0x275d:
    case 0x27e5:
    case 0x27e6:
    case 0x27e7:
    case 0x27e8:
        nAnim = 0x148;
        break;
    case 0x281d:
        nAnim = 0x158;
        break;
    case 0x27a9:
        nAnim = 0x16e;
        break;
    default:
        if (nAnim > 0x270f) {
            char szMessage[256];
            sprintf(szMessage, s_szUnmappedAnimation, nAnim);
        }
        break;
    }
    return nAnim;
}

// ------------------------------------------------------------ visual effect

// FUNCTION: SWKOTOR 0x006E5CE0
CSWCAnimBaseVisualEffect::CSWCAnimBaseVisualEffect()
{
    m_nAnimType = 8;
}

// FUNCTION: SWKOTOR 0x006E5D00
CSWCAnimBaseVisualEffect::~CSWCAnimBaseVisualEffect()
{
    CSWCAnimBase::UnloadModel();
}

// FUNCTION: SWKOTOR 0x006E5D50
int CSWCAnimBaseVisualEffect::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    char szModel[17];
    cResRef.CopyToString(szModel);
    m_pModel = NewCAurObject(szModel, "visualeffect", 0, 0);
    if (m_pModel == 0) {
        return 0;
    }
    m_cModelResRef = cResRef;
    m_bReplaceTextureIndexed = 0;
    m_pModel->SetDefaultAnimation();
    return 1;
}

// FUNCTION: SWKOTOR 0x006E5DC0
CExoString CSWCAnimBaseVisualEffect::GetAnimationName(unsigned short nAnim)
{
    switch (nAnim) {
    case 0:
        return CExoString("default");
    case 1:
        return CExoString("impact");
    case 2:
        return CExoString("duration");
    case 3:
        return CExoString("cessation");
    }
    return CExoString("");
}

// FUNCTION: SWKOTOR 0x006E5E60
int CSWCAnimBaseVisualEffect::PlayAnimOnModel(CAurObject* pModel, unsigned short nAnim, float fSpeed,
                                              int a, int nTransition)
{
    CExoString sName;
    if (pModel == 0) {
        return 0;
    }
    sName = GetAnimationName(nAnim);
    if (pModel->PlayAnimation(sName.CStr(), fSpeed, a, nTransition) == 0) {
        return 0;
    }
    return 1;
}

// ------------------------------------------------------------------ trigger

// FUNCTION: SWKOTOR 0x006E5F40
CSWCAnimBaseTrigger::CSWCAnimBaseTrigger()
{
    m_nAnimType = 9;
}

// FUNCTION: SWKOTOR 0x006E5F60
CSWCAnimBaseTrigger::~CSWCAnimBaseTrigger()
{
    CSWCAnimBase::UnloadModel();
}

// FUNCTION: SWKOTOR 0x006E5FB0
int CSWCAnimBaseTrigger::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    char szModel[17];
    cResRef.CopyToString(szModel);
    m_pModel = NewCAurObject(szModel, "trigger", 0, 0);
    if (m_pModel == 0) {
        return 0;
    }
    m_cModelResRef = cResRef;
    m_bReplaceTextureIndexed = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x006E6020 SYMBOL: ??_GCSWCAnimBaseTrigger@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006E6040
CExoString CSWCAnimBaseTrigger::GetAnimationName(unsigned short nAnim)
{
    unsigned short nClient = ServerToClientAnimation(nAnim);
    CExoString sName;
    if (!g_pRules->Get2DArrays()->GetAnimations()->GetCExoStringEntry(nClient, g_nAnimNameColumn, &sName)) {
        sName = "";
    }
    return sName;
}

// FUNCTION: SWKOTOR 0x006E6100
unsigned short CSWCAnimBaseTrigger::ServerToClientAnimation(unsigned short nAnim)
{
    switch (nAnim) {
    case 0x279f:
        nAnim = 0x15b;
        break;
    case 0x27a0:
        nAnim = 0x15e;
        break;
    default:
        if (nAnim > 0x270f) {
            char szMessage[256];
            sprintf(szMessage, s_szUnmappedAnimation, nAnim);
        }
        break;
    }
    return nAnim;
}
