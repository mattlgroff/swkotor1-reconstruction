#include "i/wind_manager.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x004A6510
void WindManager::AddPointSourceWind(Vector position, unsigned int a,
                                    unsigned int b, unsigned int c)
{
    PointSourceWind wind;
    memcpy(wind.words, &position, 12);
    wind.words[3] = a;
    wind.words[4] = b;
    wind.words[5] = c;
    m_lPointSourceWinds.Add(wind);
}
