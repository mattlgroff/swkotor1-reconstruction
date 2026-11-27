// CSWSAreaMap: the server-side explored-area bitmap of an area
// (b0057_area_map_room). The map is 440 x 256 pixels; the explored grid is
// m_nGridWidth x m_nGridHeight cells, one bit per cell.
#ifndef B_SWS_AREA_MAP_H
#define B_SWS_AREA_MAP_H

#include "b/vector.h"

// 0x004AA0F0: yaw (degrees) of a direction vector.
float __cdecl GetVectorYaw(const Vector& v);

#include "shared/sws_area_map.h"

#endif
