// Worker L matching headers: the client area-of-effect object
// CSWCAreaOfEffectObject (vtable 0x00757E28, 0x17C bytes, constructor
// 0x006D8310, 0x006D8310-0x006DA520). Its effect instances are an array of
// CSWCAnimBase models in the CSWCObject animation-base pointer (+0x68).
#ifndef L_SW_C_AREA_OF_EFFECT_H
#define L_SW_C_AREA_OF_EFFECT_H

#include "l/sw_c_projectile.h"

// The client visual effect (vtable 0x00754278, 0x130 bytes; names from
// k/sw_c_visual_effect.cpp).
class CSWCVisualEffect : public CSWCObject {
public:
    char pad_0xfc[0x114 - 0xfc];
    int m_bDone;                                    // 0x114
    int m_bHasImpact;                               // 0x118
    int m_bDestroyWhenDone;                         // 0x11c
    char pad_0x120[0x130 - 0x120];

    void SetAnimation(unsigned short nAnim);        // 0x00690510
};

class CExoSound {
public:
    int Play3DOneShotSound(const CResRef& cRes, Vector vPos, float fDelay, unsigned char nGroup,
                           unsigned long nFlags, unsigned char nVolume, float fPitch,
                           float fVariance);        // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CExoSoundSource {
public:
    CExoSoundSource(const CResRef& cResRef);        // 0x005D60E0
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5900
    void Stop();                                    // 0x005D5A20
    void Set3D(int b3D);                            // 0x005D5910
    unsigned long Play();                           // 0x005D5930
    void SetLooping(int bLooping);                  // 0x005D59D0
    void SetPosition(const Vector& vPosition, float fRange);   // 0x005D59E0
    char pad_0x4[4];
};

class CSWCAreaOfEffectObject : public CSWCObject {
public:
    CSWCAreaOfEffectObject();                       // 0x006D8310
    virtual ~CSWCAreaOfEffectObject();              // 0x006D9820
    virtual void AIUpdate();                        // 0x74 (0x006DA9B0)
    virtual void UnloadModel(int nPart);            // 0x7c (0x006D91D0)
    virtual void ReleaseModel();                    // 0x80 (0x006D90D0)
    virtual int SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap);  // 0xac (0x006D96E0)
    virtual int SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap);  // 0xb0 (0x006D9590)

    int SetAnimation(unsigned short nAnim, unsigned char nInstance);   // 0x006D91F0
    void ResetInstanceDuration(int nInstance);      // 0x006D9300
    void StopEffect();                              // 0x006D9350
    void SetMoveTimer(unsigned long nTimer, CAurObject* pModel);   // 0x006D9520
    int PlaceInstance(int nInstance);               // 0x006D98B0 (descriptive)
    void UpdateInstances();                         // 0x006DA330 (descriptive)

    unsigned long* m_pnDurations;                   // 0xfc
    unsigned long* m_pnMoveTimers;                  // 0x100
    unsigned long* m_pnAnimationLengths;            // 0x104
    unsigned long m_anGroupDurations[3];            // 0x108
    int m_anGroupInstances[3];                      // 0x114
    int m_nInstances;                               // 0x120
    unsigned long m_n124;                           // 0x124
    unsigned long m_n128;                           // 0x128
    unsigned long m_n12C;                           // 0x12c
    unsigned long m_nFinishTimer;                   // 0x130 (descriptive)
    int m_bFinished;                                // 0x134 (descriptive)
    int m_bEnding;                                  // 0x138 (descriptive)
    unsigned char m_anGroupFlags[3];                // 0x13c (descriptive)
    unsigned char m_n13F;                           // 0x13f
    int* m_pbPlaced;                                // 0x140
    class CAurScene* m_pScene;                      // 0x144
    unsigned short* m_pnAnimations;                 // 0x148
    CSWCVisualEffect* m_pVisualEffect;              // 0x14c
    int m_n150;                                     // 0x150
    CExoSoundSource* m_pSound;                      // 0x154
    CResRef m_cImpactSound;                         // 0x158 (descriptive)
    int m_nImpactSoundChance;                       // 0x168 (descriptive, percent)
    int m_n16C;                                     // 0x16c
    int m_nEffectRow;                               // 0x170 visualeffects.2da row (descriptive)
    int m_bSoundStarted;                            // 0x174 (descriptive)
    unsigned long m_oidTarget;                      // 0x178 followed object (descriptive)
};

#endif
