// CSWSArea::UnloadArea (0x0050C0E0): releases the area's walk data
// buffers, the name list, the tag lookup entry and the room array.
#include "a/sws_area.h"

class CSWSRoom {
public:
    virtual ~CSWSRoom();                            // 0x005794B0

    char pad_0x4[0x4c - 0x4];
};

class CSWSModule;
class CSWSModuleTags {
public:
    void RemoveObjectFromLookupTable(CExoString* pTag, unsigned long oid); // 0x004C5EF0
};

// FUNCTION: SWKOTOR 0x0050C0E0
void CSWSArea::UnloadArea()
{
    delete[] m_pAreaData34;
    m_pAreaData34 = NULL;
    delete[] m_pAreaData40;
    m_pAreaData40 = NULL;
    delete[] m_pAreaData1c;
    m_pAreaData1c = NULL;
    delete[] m_pAreaData20;
    m_pAreaData20 = NULL;
    delete[] m_pAreaData24;
    m_pAreaData24 = NULL;
    delete[] m_pAreaData28;
    m_pAreaData28 = NULL;
    while (!m_lNames.IsEmpty()) {
        CExoLocString* pName = m_lNames.RemoveHead();
        delete pName;
    }
    delete[] m_pAreaData160;
    m_pAreaData160 = NULL;
    CSWSModule* pModule = GetServerExoApp()->GetModule();
    if (pModule) {
        ((CSWSModuleTags*)pModule)->RemoveObjectFromLookupTable(&m_sTag, m_idSelf);
    }
    delete[] m_pRooms;
    m_pRooms = NULL;
}
