#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A6470
void WindManager::RegisterWindyEmitter(PartEmitter* pEmitter)
{
    m_lEmitters.AddUnique(pEmitter);
}
