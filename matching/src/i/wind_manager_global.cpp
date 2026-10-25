#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A5900
void WindManager::SetGlobalWind(Vector vWind)
{
    m_vGlobalWind = vWind;

    for (int i = 0; i < m_lEmitters.num; i++) {
        m_lEmitters.element[i]->CalculateRadius();
    }
}
