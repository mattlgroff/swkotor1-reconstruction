// CSWSArea room information from the area's .LYT layout.
#include "a/sws_area.h"

class CLYT {
public:
    CLYT();                                         // 0x005DE770
    virtual ~CLYT();
    int LoadLayout(const CResRef& cResRef);         // 0x005DF140
    void UnloadLayout();                            // 0x005DE450
    int GetNumRooms();                              // 0x005A4C50 (folded getter)

    char pad_0x4[0x60 - 0x4];
};

// FUNCTION: SWKOTOR 0x005073D0
int CSWSArea::LoadRoomInfo(CResStruct* pStruct)
{
    m_cValue8 = g_cAreaValue8;
    CLYT* pLayout = new CLYT();
    int bLoaded = pLayout->LoadLayout(m_cResRef);
    if (bLoaded) {
        m_nRooms = pLayout->GetNumRooms();
        m_pRooms = LoadRooms(pLayout);
    }
    pLayout->UnloadLayout();
    if (pLayout) {
        delete pLayout;
    }
    return bLoaded;
}
