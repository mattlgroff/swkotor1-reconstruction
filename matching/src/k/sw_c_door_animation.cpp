// CSWCDoor::UpdateAnimation: fire-and-forget countdown, next queued
// animation, door open / close / destroy sounds and hit-check state.
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
extern int g_nDoorTypesCol_SoundAppType;            // 0x007A224C
extern int g_nGenericDoorsCol_SoundAppType;         // 0x007A2250
extern int g_nPlaceableSoundsCol_Opened;            // 0x007A2268
extern int g_nPlaceableSoundsCol_Closed;            // 0x007A226C
extern int g_nPlaceableSoundsCol_Destroyed;         // 0x007A2270

// FUNCTION: SWKOTOR 0x00684030
void CSWCDoor::UpdateAnimation()
{
    if (AnimationFireAndForget(m_nAnimation) == 1) {
        m_nFireForgetTime -= m_nElapsed;
        if (m_nFireForgetTime < 0) {
            SetAnimationSlot(GetLoopingAnimation(), 0, 0);
            m_nFireForgetTime = 0;
        }
    }
    if (IsAnimationLooping(m_nAnimation) == 1) {
        // The animation slot is read back as a full word (the callee only
        // writes its low half), so it is an int-sized local.
        int nSoundType;
        int animation;
        float speed;
        unsigned long duration;
        void* data;
        if (GetTopFireForgetAnimation((unsigned short*)&animation, &speed, &duration, &data) == 1) {
            m_nAnimation = (unsigned short)animation;
            m_pModel->PlayAnim(animation, speed, 1, 0);
            m_nFireForgetTime = duration;
            RemoveTopFireForgetAnimation();
            if ((unsigned short)animation == 0x14e || (unsigned short)animation == 0x14f
                || (unsigned short)animation == 0x150 || (unsigned short)animation == 0x151
                || (unsigned short)animation == 0x149) {
                CExoString sSound;
                int nRow = m_n124;
                C2DAs* p2DArrays = g_pRules->m_p2DArrays;
                C2DA* pTable;
                if (nRow == 0) {
                    nRow = m_n125;
                    pTable = p2DArrays->m_pGenericDoors;
                    pTable->GetINTEntry(nRow, g_nGenericDoorsCol_SoundAppType, &nSoundType);
                }
                else {
                    pTable = p2DArrays->m_pDoorTypes;
                    pTable->GetINTEntry(nRow, g_nDoorTypesCol_SoundAppType, &nSoundType);
                }
                int nColumn;
                C2DA* pSounds;
                if ((unsigned short)animation == 0x14e || (unsigned short)animation == 0x14f) {
                    p2DArrays = g_pRules->m_p2DArrays;
                    nColumn = g_nPlaceableSoundsCol_Opened;
                    pSounds = p2DArrays->m_pPlaceableSounds;
                    pSounds->GetCExoStringEntry(nSoundType, nColumn, &sSound);
                }
                else if ((unsigned short)animation == 0x150 || (unsigned short)animation == 0x151) {
                    p2DArrays = g_pRules->m_p2DArrays;
                    nColumn = g_nPlaceableSoundsCol_Closed;
                    pSounds = p2DArrays->m_pPlaceableSounds;
                    pSounds->GetCExoStringEntry(nSoundType, nColumn, &sSound);
                }
                else if ((unsigned short)animation == 0x149) {
                    p2DArrays = g_pRules->m_p2DArrays;
                    nColumn = g_nPlaceableSoundsCol_Destroyed;
                    pSounds = p2DArrays->m_pPlaceableSounds;
                    pSounds->GetCExoStringEntry(nSoundType, nColumn, &sSound);
                }
                CResRefSound cSound(sSound);
                g_pExoSound->Play3DOneShotSound(cSound, m_vPosition, 1.5f, 0x16, 0, 0, 0.0f, 0.0f);
            }
        }
        CAurObject* pModel;
        if (m_nAnimation == 0x2726) {
            pModel = GetModel(0xff, 1);
            m_nOpenState = 0;
            if (pModel) {
                pModel->SetIgnoreHitCheck(0);
            }
        }
        else if (m_nAnimation == 0x2742) {
            pModel = GetModel(0xff, 1);
            m_nOpenState = 1;
            if (pModel) {
                pModel->SetIgnoreHitCheck(1);
            }
        }
        else if (m_nAnimation == 0x2743) {
            pModel = GetModel(0xff, 1);
            m_nOpenState = 2;
            if (pModel) {
                pModel->SetIgnoreHitCheck(1);
            }
        }
        else if (m_nAnimation == 0x2758) {
            pModel = GetModel(0xff, 1);
            m_nOpenState = 3;
            if (pModel) {
                pModel->SetIgnoreHitCheck(1);
            }
        }
        if (data) {
            delete data;
        }
    }
}
