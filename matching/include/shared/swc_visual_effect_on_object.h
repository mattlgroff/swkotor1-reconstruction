// swc_visual_effect_on_object.h -- CSWCVisualEffectOnObject
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/l/sw_c_visual_effect.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_VISUAL_EFFECT_ON_OBJECT_H
#define SHARED_SWC_VISUAL_EFFECT_ON_OBJECT_H

#include "shared/exo_string.h"
#include "shared/vector.h"

class AttachHook;
class CAurObject;
class CExoSoundSource;

class CSWCVisualEffectOnObject {
public:
    CSWCVisualEffectOnObject();                             // 0x006A09E0
    ~CSWCVisualEffectOnObject();                            // 0x006A1810

    int IsReady();                                  // 0x006A0480
    void SetDefaultColors(unsigned short nEffect);  // 0x006A04C0
    void HideModel(unsigned short nEffect);                    // 0x006A05B0
    void ResetBeamTimer(CAurObject* pBeam);         // 0x006A05E0
    int GetImpactDuration(unsigned short nEffect);  // 0x006A0620
    int LoadBeam(unsigned short nEffect);           // 0x006A0680
    int LoadFailImpact(unsigned short nEffect);     // 0x006A0800
    int LoadDurationModel(unsigned short nEffect);  // 0x006A0860
    void DestroyModel();                            // 0x006A08D0
    unsigned char GetType();                        // 0x006A0920
    unsigned char GetBeamNode(unsigned short nEffect);  // 0x006A0930
    void DestroyModels();                           // 0x006A0BF0
    void Detach();                                  // 0x006A0C70
    void AttachBeam();                              // 0x006A0D30
    void AttachLight();                             // 0x006A0DF0 (descriptive)
    void ApplyTexture(unsigned short nEffect);      // 0x006A1220
    void RemoveTexture();                           // 0x006A1470
    void ApplySpeedBlur(unsigned short nEffect);    // 0x006A14E0
    void RemoveSpeedBlur();                         // 0x006A1570
    void ApplyFlash(unsigned short nEffect);        // 0x006A15D0
    void RemoveFlash(unsigned short nEffect);       // 0x006A1610
    void ApplyCheat(unsigned short nEffect);        // 0x006A2AF0
    void ApplySpellVisual(unsigned short nEffect);  // 0x006A2E10
    void ApplyLightsaberThrow(unsigned short nEffect);  // 0x006A30E0 (descriptive)
    void ApplySkinTexture(unsigned short nEffect);  // 0x006A34B0
    void ApplyEnvironmentMap(unsigned short nEffect);       // 0x006A3660
    void ApplyColorOverlay(unsigned short nEffect);         // 0x006A3740
    void ApplyLightSource(unsigned short nEffect);  // 0x006A38E0
    void ApplyChunk(unsigned short nEffect);        // 0x006A4060 (descriptive)
    void ApplyMirv(unsigned short nEffect);         // 0x006A4240
    void ApplyFOV(unsigned short nEffect);          // 0x006A4580
    void ApplyHardCodedVisualEffect(unsigned short nEffect);    // 0x006A54D0
    void UpdateFOV(unsigned short nEffect, int bUnused);        // 0x006A4E70
    void UpdateSelfIlluminationPulse(unsigned long nDelta);     // 0x006A4A80
    void UpdateAlpha(unsigned long nDelta);         // 0x006A4C00
    void UpdateMotionBlur(unsigned short nUnused);  // 0x006A5920
    void ApplyColorEffect(unsigned short nEffect);  // 0x006A3E80
    void PlayCastAnimation(unsigned short nEffect);            // 0x006A3FD0
    void RefreshTextures(unsigned short nEffect);              // 0x006A4000
    void AttachHook(unsigned short nEffect);        // 0x006A4680
    void RemoveSelfIllumination(unsigned short nEffect);       // 0x006A4720
    void ResetSelfIllumination(unsigned short nEffect);        // 0x006A47B0
    void SetObjectDrawMode(unsigned short nEffect);            // 0x006A47F0
    void RemoveDurationModel(unsigned short nEffect);   // 0x006A4830
    void RemoveColorEffect(unsigned short nEffect); // 0x006A4920
    int LoadLight(unsigned short nEffect);                     // 0x006A4FA0
    void RemoveFOV(unsigned short nEffect, int bForce);  // 0x006A49A0
    int LoadHardCodedVisualEffect(unsigned short nEffect);  // 0x006A5890
    void RemoveHardCodedVisualEffect(unsigned short nEffect);   // 0x006A56F0
    void DetachAll();                               // 0x006A5F00
    void Reset();                                   // 0x006A6950
    void ReAttachVisualEffect();                    // 0x006A5000
    void StopVisualEffect(int bStop);               // 0x006A6590
    int ShowVisualEffect(int bForce);               // 0x006A5A40
    void StartVisualEffect();                       // 0x006A5FC0
    void Update(unsigned short nEffect, unsigned long nDelta);  // 0x006A69E0
    void Update(unsigned long nDelta);              // 0x006A6A50
    void AttachVFXModelToTargetModel(CAurObject* pModel, CAurObject* pTo,
                                     const CExoString& sNode);   // 0x006A1650

    Vector m_vColor1;                               // 0x00
    Vector m_vColor2;                               // 0x0c
    Vector m_vOffset;                               // 0x18
    CExoString m_sSourceNode;                       // 0x24
    CExoString m_sTargetNode;                       // 0x2c
    Vector m_vTargetPosition;                       // 0x34 (descriptive)
    unsigned long m_oidTargets[3];                  // 0x40
    unsigned long m_n4C[3];                         // 0x4c
    unsigned short m_n58[3];                        // 0x58
    unsigned short pad_0x5e;
    unsigned long m_n60[3];                         // 0x60
    unsigned short m_n6C[3];                        // 0x6c
    unsigned short pad_0x72;
    CAurObject* m_pBeams[3];                        // 0x74
    CAurObject* m_pImpacts[3];                      // 0x80
    CAurObject* m_pModel;                           // 0x8c
    unsigned long m_nBeamTimer[4];                  // 0x90
    unsigned short m_nA0;                           // 0xa0
    unsigned short m_nA2;                           // 0xa2
    unsigned short m_nA4;                           // 0xa4
    unsigned short pad_0xa6;
    unsigned long m_nA8;                            // 0xa8
    int m_bAC;                                       // 0xac
    int m_bB0;                                       // 0xb0
    int m_bB4;                                       // 0xb4
    int m_bB8;                                       // 0xb8
    int m_bBC;                                       // 0xbc
    int m_bC0;                                       // 0xc0
    unsigned short m_nEffect;                       // 0xc4
    unsigned char m_bC6;                            // 0xc6
    unsigned char m_bC7;                            // 0xc7
    unsigned char m_bHasDuration;                   // 0xc8 (descriptive)
    unsigned char pad_0xc9[3];
    float m_fCC;                                    // 0xcc
    float m_fD0;                                    // 0xd0
    unsigned long m_nD4;                            // 0xd4
    unsigned char m_nD8;                            // 0xd8
    unsigned char m_nType;                          // 0xd9
    unsigned short pad_0xda;
    unsigned long m_nDC;                            // 0xdc
    unsigned long m_nE0;                            // 0xe0
    float m_fE4;                                    // 0xe4
    unsigned long m_nE8;                            // 0xe8
    CExoSoundSource* m_pSound;                      // 0xec
    unsigned long m_nF0;                            // 0xf0
    unsigned long m_nF4;                            // 0xf4

    // declared by other copies
    int Load(unsigned short nEffect, int nDuration, unsigned long oidSelf, unsigned long nTarget, unsigned long nNode, unsigned char nA, unsigned char nB, Vector vOffset);
};

#endif
