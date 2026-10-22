// The out-of-line CSWMiniState constructor (inlined everywhere else).

#define M_OUTLINE_MINI_STATE
#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x00670750
CSWMiniState::CSWMiniState(const Vector& vPos, const Vector& vVel)
{
    m_vPos = vPos;
    m_vVel = vVel;
}
