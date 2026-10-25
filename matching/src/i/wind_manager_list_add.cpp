#include "i/wind_manager.h"

// FUNCTION: SWKOTOR 0x004A64C0
void PointSourceWindList::Add(PointSourceWind value)
{
    if (num == array_size) {
        int capacity = array_size;
        if (capacity != 0)
            capacity += capacity;
        else
            capacity = 8;
        Allocate(capacity);
    }
    element[num] = value;
    ++num;
}
