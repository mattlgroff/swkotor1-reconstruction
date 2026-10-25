#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A6440
Vector* WindManager::GetGlobalPointWind(Vector* out, const Vector* point, float time)
{
    (void)time;
    Vector* result = out;
    PointSourceWindVector(result, *point);
    return result;
}
