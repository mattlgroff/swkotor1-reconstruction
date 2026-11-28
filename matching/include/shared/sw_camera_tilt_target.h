// sw_camera_tilt_target.h -- CSWCameraTiltTarget
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/n/sw_c_module_control.h); the other copies' members fall on its members.
#ifndef SHARED_SW_CAMERA_TILT_TARGET_H
#define SHARED_SW_CAMERA_TILT_TARGET_H

#include "shared/vector.h"

class CSWCameraTiltTarget {
public:
    char pad_0x00[0x24];
    Vector m_vUp;                                   // +0x24
    float m_f30;                                    // +0x30
    float m_fZoom;                                  // +0x34
    float m_f38;                                    // +0x38
    float m_f3C;                                    // +0x3c
    float m_fTurn;                                  // +0x40
    float m_f44;                                    // +0x44
    float m_fTilt;                                  // +0x48
    char pad_0x4c[0x58 - 0x4c];
    float m_f58;                                    // +0x58
    float m_f5C;                                    // +0x5c
    float m_f60;                                    // +0x60
    float m_f64;                                    // +0x64
    float m_f68;                                    // +0x68
    float m_f6C;                                    // +0x6c
    char pad_0x70[0x90 - 0x70];
    float m_fShakeYaw;                              // +0x90
    float m_fShakePitch;                            // +0x94
};

#endif
