// CSWSPlayerLUOSortedObjectList: one entry of the array that
// SortObjectsForGameObjectUpdate (0x00567850) builds with `new[]`.
#include "b/sws_message.h"

#include "shared/sws_player_luo_sorted_object_list.h"

// FUNCTION: SWKOTOR 0x00566D80
CSWSPlayerLUOSortedObjectList::CSWSPlayerLUOSortedObjectList()
{
    m_oidObject = INVALID_OBJECT_ID;
    m_fDistance = 0.0f;
    m_nCategory = 0;
}
