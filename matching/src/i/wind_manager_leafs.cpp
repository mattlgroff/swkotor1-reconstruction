#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A6480
void WindManager::DeregisterWindyEmitter(PartEmitter* pEmitter)
{
    int matches = 0;
    for (int i = 0; i < m_lEmitters.num; i++) {
        if (m_lEmitters.element[i] == pEmitter)
            matches++;
    }
    if (matches != 0)
        m_lEmitters.Remove(pEmitter);
}
