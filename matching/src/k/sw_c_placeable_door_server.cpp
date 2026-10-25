// Client placeable / door links to their server-side objects.

class CSWSPlaceable;
class CSWSDoor;
class CSWSObject;

class CSWCObjectServerLink {
public:
    CSWSObject* GetServerObject();                        // 0x0063D4B0
};

class CSWCObject : public CSWCObjectServerLink {};

class CSWCPlaceable : public CSWCObject {
public:
    CSWSPlaceable* GetServerPlaceable();
};

class CSWCDoor : public CSWCObject {
public:
    CSWSDoor* GetServerDoor();
};

class CSWSObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14();
    virtual CSWSDoor* AsSWCDoor();                         // +0x18
    virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44();
    virtual CSWSPlaceable* AsSWCPlaceable();                // +0x48
};

// FUNCTION: SWKOTOR 0x00682E00
CSWSPlaceable* CSWCPlaceable::GetServerPlaceable()
{
    if (GetServerObject() != 0)
        return GetServerObject()->AsSWCPlaceable();
    return 0;
}

// FUNCTION: SWKOTOR 0x00683EE0
CSWSDoor* CSWCDoor::GetServerDoor()
{
    if (GetServerObject() != 0)
        return GetServerObject()->AsSWCDoor();
    return 0;
}
