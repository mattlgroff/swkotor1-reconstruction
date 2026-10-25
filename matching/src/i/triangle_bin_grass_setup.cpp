#include "i/triangle_bin.h"

// FUNCTION: SWKOTOR 0x004A9630
void CAurTriangleBin::BuildGrassPolys()
{
    const unsigned char flags = this->flags;
    if ((flags & 4) != 0)
        return;
    if (polygon_count == 0)
        return;

    bool coloured = false;
    bool lightmapped = false;
    if ((flags & 8) != 0)
        lightmapped = true;
    else if ((flags & 0x10) != 0)
        coloured = true;
    vertex_format = 0x20;
    if (lightmapped)
        vertex_format = 0x28;
    else if (coloured)
        vertex_format = 0x24;
    CreateArrays();
}
