// aur_model.h -- CAurModel
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/sw_c_creature_s7.cpp); the other copies' members fall on its members.
#ifndef SHARED_AUR_MODEL_H
#define SHARED_AUR_MODEL_H

#include "shared/vector.h"

class Quaternion;

class CAurModel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void PlayOverlayAnimation(unsigned short nAnim, int bLoop);   // 0x44 (descriptive)
    virtual void v48(); virtual void v4C(); virtual void v50();
    virtual void PlayAnimation(unsigned short nAnim, float fSpeed, int nFlags, int b);   // 0x54 (descriptive)
    virtual void v58();
    virtual int SetLipSyncData(float* pTimes, float* pShapes, int nKeys, float fLength);   // 0x5c (descriptive)
    virtual void SetOrientation(Vector* pOrientation);      // 0x64 (descriptive)
    virtual void SetOrientation(Quaternion* pOrientation);  // 0x60 (descriptive)
    virtual void SetPosition(Vector* pPosition);    // 0x68 (descriptive)
    virtual void SetLocalPosition(int b);           // 0x6c (descriptive)
    virtual void SetLocalOrientation(int b);        // 0x70 (descriptive)
    virtual void V74();                                          // +0x74
    virtual void V78();                                          // +0x78
    virtual void V7C();                                          // +0x7c
    virtual void V80();                                          // +0x80
    virtual void V84();                                          // +0x84
    virtual void V88();                                          // +0x88
    virtual void V8C();                                          // +0x8c
    virtual void V90();                                          // +0x90
    virtual void V94();                                          // +0x94
    virtual int GetHookTransform(const char* sHook, Vector* pPosition, Quaternion* pOrientation); // +0x98
};

#endif
