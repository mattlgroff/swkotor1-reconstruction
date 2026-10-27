#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A6480
void WindManager::DeregisterWindyEmitter(PartEmitter* pEmitter)
{
    if (m_lEmitters.Contains(pEmitter))
        m_lEmitters.Remove(pEmitter);
}
