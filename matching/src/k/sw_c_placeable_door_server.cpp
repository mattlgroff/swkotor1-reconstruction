// Client placeable / door links to their server-side objects.

class CSWSPlaceable;
class CSWSDoor;
class CSWSObject;

#include "shared/swc_object_server_link.h"

class CSWCObject : public CSWCObjectServerLink {};

class CSWCPlaceable : public CSWCObject {
public:
    CSWSPlaceable* GetServerPlaceable();
};

class CSWCDoor : public CSWCObject {
public:
    CSWSDoor* GetServerDoor();
};

#include "shared/game_object.h"

class CSWSObject : public CGameObject {
public:
};

// FUNCTION: SWKOTOR 0x00682E00
CSWSPlaceable* CSWCPlaceable::GetServerPlaceable()
{
    if (GetServerObject() != 0)
        return GetServerObject()->AsSWSPlaceable();
    return 0;
}

// FUNCTION: SWKOTOR 0x00683EE0
CSWSDoor* CSWCDoor::GetServerDoor()
{
    if (GetServerObject() != 0)
        return GetServerObject()->AsSWSDoor();
    return 0;
}
