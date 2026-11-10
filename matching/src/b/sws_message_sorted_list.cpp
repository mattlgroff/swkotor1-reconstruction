// CSWSPlayerLUOSortedObjectList: one entry of the array that
// SortObjectsForGameObjectUpdate (0x00567850) builds with `new[]`.
#include "b/sws_message.h"

class CSWSPlayerLUOSortedObjectList {
public:
    CSWSPlayerLUOSortedObjectList();

    unsigned long m_oidObject;                              // 0x0
    float m_fDistance;                                      // 0x4
    unsigned long m_nCategory;                              // 0x8
};

// FUNCTION: SWKOTOR 0x00566D80
CSWSPlayerLUOSortedObjectList::CSWSPlayerLUOSortedObjectList()
{
    m_oidObject = INVALID_OBJECT_ID;
    m_fDistance = 0.0f;
    m_nCategory = 0;
}
