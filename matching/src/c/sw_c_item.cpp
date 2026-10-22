// CSWCItem (b0060_client_dtors_forwarders, b0062 message leaves).

class CSWSItem;

class CSWSObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual CSWSItem* AsSWSItem();                  // 0x38
};

class CSWCObjectServerLink {
public:
    CSWSObject* GetServerObject();                  // 0x0063D4B0
};

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
