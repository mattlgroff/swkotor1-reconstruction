// CSWCWaypoint (client waypoint, object type 12, vtable 0x00754598):
// constructor and destructors. Class name from the object type; the map
// note members are descriptive.
#include "k/sw_c_client_object.h"

extern unsigned char g_nObjectTypeWaypoint;                          // 0x0074663B

class CSWCWaypoint : public CSWCObject {
public:
    CSWCWaypoint();
    virtual ~CSWCWaypoint();

    int m_bHasMapNote;                                               // +0xFC, descriptive
    CExoString m_sMapNote;                                           // +0x100, descriptive
    char pad_0x108[0x10c - 0x108];
    int m_bMapNoteEnabled;                                           // +0x10C, descriptive
};

// FUNCTION: SWKOTOR 0x00692540
CSWCWaypoint::CSWCWaypoint() : CSWCObject(g_nObjectTypeWaypoint)
{
    m_nAnimation = 10001;
    m_bHasMapNote = 0;
    m_bMapNoteEnabled = 0;
}

// FUNCTION: SWKOTOR 0x006925B0
CSWCWaypoint::~CSWCWaypoint()
{
}

// FUNCTION: SWKOTOR 0x00692610 SYMBOL: ??_GCSWCWaypoint@@UAEPAXI@Z
// CSWCWaypoint::`scalar deleting destructor' (compiler generated).
