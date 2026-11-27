// sw_camera_clip_state.h -- CSWCameraClipState
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/l/sw_behavior_camera_combat.h); the other copies' members fall on its members.
#ifndef SHARED_SW_CAMERA_CLIP_STATE_H
#define SHARED_SW_CAMERA_CLIP_STATE_H

#include "shared/vector.h"

struct CSWCameraClipState {
    CSWCameraClipState() : m_vPosition(0.0f, 0.0f, 0.0f), m_vVelocity(0.0f, 0.0f, 0.0f)
    {
        m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
        m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    }
    CSWCameraClipState(const Vector& vPosition, const Vector& vVelocity)
        : m_vPosition(vPosition), m_vVelocity(vVelocity)
    {
    }
    Vector m_vPosition;                         // +0x00
    Vector m_vVelocity;                         // +0x0c
};

#endif
