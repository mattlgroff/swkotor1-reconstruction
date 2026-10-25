#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A57A0
void WindManager::SetMaximumDeviation(float fMinimum, float fMaximum)
{
    m_fMinimumDeviation = fMinimum;
    m_fMaximumDeviation = fMaximum;
}
