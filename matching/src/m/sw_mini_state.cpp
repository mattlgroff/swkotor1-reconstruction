// CSWMiniState (position / velocity pair) arithmetic used by the mini-game
// integrators 0x00670C30 and 0x00679870 (0x006707A0-0x00670A10). Vector + is a free
// function and Vector * float a member; both build the result with the
// three-float constructor.

#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x006707A0 SYMBOL: ??H@YA?AUCSWMiniState@@ABU0@0@Z
CSWMiniState operator+(const CSWMiniState& a, const CSWMiniState& b)
{
    return CSWMiniState(a.m_vPos + b.m_vPos, a.m_vVel + b.m_vVel);
}

// FUNCTION: SWKOTOR 0x00670830 SYMBOL: ??D@YA?AUCSWMiniState@@ABU0@M@Z
CSWMiniState operator*(const CSWMiniState& a, float f)
{
    return CSWMiniState(a.m_vPos * f, a.m_vVel * f);
}

// FUNCTION: SWKOTOR 0x006708C0 SYMBOL: ??K@YA?AUCSWMiniState@@ABU0@M@Z
CSWMiniState operator/(const CSWMiniState& a, float f)
{
    if (f != 0.0f) {
        return CSWMiniState(a.m_vPos / f, a.m_vVel / f);
    }
    return CSWMiniState(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f));
}

// FUNCTION: SWKOTOR 0x006709C0
CSWMiniState MakeMiniState(const Vector& vVel, const Vector& vPos)
{
    return CSWMiniState(vPos, vVel);
}

// FUNCTION: SWKOTOR 0x00670A10
void CSWCMiniGame::UpdateAxisLinear(Vector* pVec, float fDelta)
{
    m_sD8.m_vVel = m_sC0.m_vVel + (*pVec * m_f74) * fDelta;
}

// FUNCTION: SWKOTOR 0x00670A90
Vector CSWCMiniGame::GetAccel(const Vector& vA, const Vector& vB)
{
    return vB * m_fBC - vA * (m_fBC / m_f74);
}

// FUNCTION: SWKOTOR 0x00670C30
// NONMATCH (RK4 step): the original copies each k = state * fDelta result
// into its named local with rep movsd (no return-value elision) and keeps
// the frame at 0x120; ours constructs k1..k4 in place. Declaring the k's
// first and assigning them adds the CSWMiniState zero-initialisation.
void CSWCMiniGame::UpdateAxisRK4(Vector* pVec, float fDelta)
{
    Vector vMid = (m_vB0 + *pVec) * 0.5f;
    CSWMiniState s(GetAccel(m_sC0.m_vPos, m_vB0), m_sC0.m_vPos);
    CSWMiniState k1 = s * fDelta;
    s = m_sC0 + k1 * 0.5f;
    CSWMiniState k2 = CSWMiniState(GetAccel(s.m_vPos, vMid), s.m_vPos) * fDelta;
    s = m_sC0 + k2 * 0.5f;
    CSWMiniState k3 = CSWMiniState(GetAccel(s.m_vPos, vMid), s.m_vPos) * fDelta;
    s = m_sC0 + k3;
    CSWMiniState k4 = CSWMiniState(GetAccel(s.m_vPos, *pVec), s.m_vPos) * fDelta;
    m_sD8 = m_sC0 + k1 / 6.0f + k2 / 3.0f + k3 / 3.0f + k4 / 6.0f;
    m_vB0 = *pVec;
}

// FUNCTION: SWKOTOR 0x00671020
void CSWCMiniGame::Rotate(float fA, float fB, float fScale)
{
    float f = fScale * m_f74;
    fA = fA * f;
    fB = f * fB;
    if (m_nLateralAxis < 0) {
        fA = -fA;
    }
    switch (abs(m_nLateralAxis)) {
    case 1:
        m_sD8.m_vVel.x -= fA;
        break;
    case 2:
        m_sD8.m_vVel.y -= fA;
        break;
    case 3:
        m_sD8.m_vVel.z -= fA;
        break;
    }
    if (m_nVerticalAxis < 0) {
        fB = -fB;
    }
    switch (abs(m_nVerticalAxis)) {
    case 1:
        m_sD8.m_vVel.x -= fB;
        break;
    case 2:
        m_sD8.m_vVel.y -= fB;
        break;
    case 3:
        m_sD8.m_vVel.z -= fB;
        break;
    }
}
