// CSWCPlaceable::UpdateAnimation: fire-and-forget countdown, next queued
// animation (with the out-of-place model reset), placeable sounds.
#include "k/sw_c_placeable_door.h"

class CResRefSound {
public:
    char m_resRef[16];
    CResRefSound(const CExoString&);                // 0x00406D60 (CResRef::CResRef)
};

class CExoSound {
public:
    int Play3DOneShotSound(const CResRefSound&, Vector, float, unsigned char, unsigned long,
                           unsigned char, float, float);  // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

// Note for 0x006832A0: the original loads the sound table (+0x70) before
// pushing, so its arms merge before any push; ours push first. One
// store-order swap in the out-of-place reset. Tried table / arrays locals,
// one and two inline accessors, named and implicit column strings. The
// zero / one register hoisting (ebx / edi) is fragile: it needs the two
// inline accessors (or the pointer locals) to appear.
// FUNCTION: SWKOTOR 0x006832A0
// NONMATCH: 21 diff insns (928 / 912 bytes). See the note above: the three
// sound-column arms push their arguments before the tail merge.
void CSWCPlaceable::UpdateAnimation()
{
    if (AnimationFireAndForget(m_nAnimation) == 1) {
        m_nFireForgetTime -= m_nElapsed;
        if (m_nFireForgetTime < 0) {
            SetAnimationSlot(GetLoopingAnimation(), 0, 0);
        }
    }
    if (IsAnimationLooping(m_nAnimation) == 1) {
        int animation;
        int nSoundType;
        float speed;
        unsigned long duration;
        void* data;
        if (GetTopFireForgetAnimation((unsigned short*)&animation, &speed, &duration, &data) == 1) {
            m_nAnimation = (unsigned short)animation;
            if (m_bOutOfPlace) {
                Vector vPosition = m_vPosition;
                Vector vOrientation = m_vOrientation;
                m_bOutOfPlace = 0;
                m_pModel->SetPosition(vPosition);
                m_pModel->SetFacing(vOrientation);
                m_pModel->SetFlag130(0);
                m_pModel->SetFlag138(0);
                AbortFireForgetAnimation(0);
            }
            if (AnimationPlayOutOfPlace(animation)) {
                m_bOutOfPlace = 1;
                Vector vOrigin;
                Quaternion qIdentity;
                m_pModel->SetPosition(vOrigin);
                m_pModel->SetOrientation(qIdentity);
                m_pModel->SetFlag130(1);
                m_pModel->SetFlag138(1);
                m_n54 = 1;
            }
            m_pModel->PlayAnim(animation, speed, 1, 0);
            m_nFireForgetTime = duration;
            RemoveTopFireForgetAnimation();
            if ((unsigned short)animation == 0x138 || (unsigned short)animation == 0x139
                || (unsigned short)animation == 0x132 || (unsigned short)animation == 0x13b
                || (unsigned short)animation == 0x13a) {
                CExoString sSound;
                int nAppearance = m_nAppearance;
                int bFound;
                {
                    CExoString sColumn("SoundAppType");
                    bFound = g_pRules->Get2DArrays()->GetPlaceables()->GetINTEntry(nAppearance, sColumn, &nSoundType);
                }
                CSWSPlaceable* pServer = ServerInline();
                CSWCArea* pArea = m_pArea;
                if (pServer && pServer->m_n440 && !pServer->m_n444 && pArea && pArea->ParentCorpseExists(m_oid)) {
                    nSoundType = 0x35;
                    bFound = 1;
                }
                if (bFound && (unsigned short)animation != 0x138) {
                    if ((unsigned short)animation == 0x139) {
                        CExoString sColumn("Closed");
                        bFound = g_pRules->Get2DArrays()->GetPlaceableSounds()->GetCExoStringEntry(nSoundType, sColumn, &sSound);
                    }
                    else if ((unsigned short)animation == 0x132) {
                        CExoString sColumn("Destroyed");
                        bFound = g_pRules->Get2DArrays()->GetPlaceableSounds()->GetCExoStringEntry(nSoundType, sColumn, &sSound);
                    }
                    else if ((unsigned short)animation == 0x13b || (unsigned short)animation == 0x13a) {
                        CExoString sColumn("Used");
                        bFound = g_pRules->Get2DArrays()->GetPlaceableSounds()->GetCExoStringEntry(nSoundType, sColumn, &sSound);
                    }
                    else {
                        bFound = 0;
                    }
                    if (bFound) {
                        CResRefSound cSound(sSound);
                        g_pExoSound->Play3DOneShotSound(cSound, m_vPosition, 0.0f, 0x16, 0, 0, 0.0f, 0.0f);
                    }
                }
            }
        }
    }
}
