// CSWCItem (b0060_client_dtors_forwarders, b0062 message leaves).

class CSWSItem;

#include "shared/game_object.h"

class CSWSObject : public CGameObject {
public:
};

#include "shared/swc_object_server_link.h"

class CSWCItem {
public:
    CSWCItem();
    virtual ~CSWCItem();

    CSWSItem* GetServerItem();

    char pad_0x4[0xc];
    CSWCObjectServerLink m_cServerLink;             // 0x10
};

// Defined only to emit the vtable and its scalar deleting destructor.
CSWCItem::CSWCItem()
{
}

// FUNCTION: SWKOTOR 0x006474B0 SYMBOL: ??_GCSWCItem@@UAEPAXI@Z
// CSWCItem::`scalar deleting destructor' (compiler generated).

// FUNCTION: SWKOTOR 0x00646C90
CSWSItem* CSWCItem::GetServerItem()
{
    if (m_cServerLink.GetServerObject()) {
        return m_cServerLink.GetServerObject()->AsSWSItem();
    }
    return 0;
}
