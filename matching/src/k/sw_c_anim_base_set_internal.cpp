// CSWCAnimBase::PlayAnimOnModel (vtable slot 0xf0): plays an animation by
// id on a model; fire-and-forget ids go through PlayAnimSetOnModel, the
// "reverse" ids force a negative speed.
#include "k/sw_c_anim_base.h"

// FUNCTION: SWKOTOR 0x0069E8D0
// NONMATCH: 36 diff insns, register allocation only (original: result edi,
// nAnim ebp, pModel ebx; ours: result ebx, nAnim edi, pModel ebp). Layout,
// both jump tables and size agree. Tried declaration order, early returns,
// id / model / CStr locals, copy-init forms.
int CSWCAnimBase::PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b)
{
    int bResult = 0;
    CExoString sAnim = GetAnimationName(nAnim);
    if (pModel) {
        if (fSpeed == 0.0f) {
            fSpeed = 1.0f;
        }
        switch ((unsigned short)nAnim) {
        case 0x2713:
        case 0x2761:
        case 0x2763:
        case 0x27e4:
        case 0x27fe:
        case 0x2810:
        case 0x2812:
        case 0x281f:
            if (fSpeed >= 0.0f) {
                fSpeed = -fSpeed;
            }
        default:
            pModel->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
            break;
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
        case 0x2791:
        case 0x2792:
        case 0x2798:
        case 0x27aa:
        case 0x27ab:
        case 0x283e:
            PlayAnimSetOnModel(pModel, nAnim, 1);
            break;
        }
        bResult = 1;
    }
    return bResult;
}
