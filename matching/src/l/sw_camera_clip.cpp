// The camera spring integrators CSWCameraClipBase (vtable 0x00757BD4) and
// CSWCameraClipHelper (vtable 0x00757BD8): {position, velocity} pulled
// towards the input point with gain / mass, stepped with Runge-Kutta.
#include "l/sw_behavior_camera_combat.h"

// FUNCTION: SWKOTOR 0x006D0A40
CSWCameraClipState operator/(const CSWCameraClipState& s, float f)
{
    return f != 0.0f ? CSWCameraClipState(s.m_vPosition / f, s.m_vVelocity / f)
                     : CSWCameraClipState(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f));
}

// FUNCTION: SWKOTOR 0x006D0B20
// NONMATCH: the original keeps two zero Vector temporaries live while it
// re-zeroes the states and interleaves the stores (16 insns).
CSWCameraClipBase::CSWCameraClipBase()
    : m_vLastInput(0.0f, 0.0f, 0.0f)
{
    m_fMass = 100.0f;
    m_fGain = 100.0f;
}

// FUNCTION: SWKOTOR 0x006D0FC0
Vector CSWCameraClipBase::GetAccelerationVector(const Vector& vPosition, const Vector& vInput)
{
    return vInput * m_fGain - vPosition * (m_fGain / m_fMass);
}

// FUNCTION: SWKOTOR 0x006D1030
void CSWCameraClipBase::SetVelocity(float fVelocity)
{
    m_cState = m_cNewState;
    m_cNewState.m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
    m_cNewState.m_vPosition = Vector(0.0f, fVelocity, 0.0f);
}

// FUNCTION: SWKOTOR 0x006D10B0
CSWCameraClipHelper::CSWCameraClipHelper()
{
    m_fReleaseGain = 100.0f;
}

// FUNCTION: SWKOTOR 0x006D10D0
Vector CSWCameraClipHelper::GetAccelerationVector(const Vector& vPosition, const Vector& vInput)
{
    if (vInput.x == 0.0f && vInput.y == 0.0f && vInput.z == 0.0f) {
        return vInput * m_fReleaseGain - vPosition * (m_fReleaseGain / m_fMass);
    }
    return CSWCameraClipBase::GetAccelerationVector(vPosition, vInput);
}

// FUNCTION: SWKOTOR 0x006D1190
CSWCameraClipState CSWCameraClipBase::Update(Vector vInput, float fStep, int bClearVelocity,
                                             int bClearPosition, int bClearNewVelocity,
                                             int bClearNewPosition)
{
    if (bClearNewVelocity) {
        m_cNewState.m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
    }
    if (bClearNewPosition) {
        m_cNewState.m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    }
    m_cState = m_cNewState;
    if (bClearVelocity) {
        m_cState.m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
    }
    if (bClearPosition) {
        m_cState.m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    }
    Accelerate(vInput, fStep);
    return m_cNewState;
}
