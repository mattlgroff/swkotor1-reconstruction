// CSWCRoom (client room, 0x48 bytes, vtable 0x00751A34): the compiler-made
// vector deleting destructor emitted in nwcarea.o, where CSWCArea allocates
// its room array. The destructor itself (0x006460B0) is worker n's address;
// it is defined here without an annotation only to emit the vtable.

class CSWRoom {
public:
    CSWRoom();
    virtual ~CSWRoom();                             // 0x005C7D00
    char pad_0x4[0x44];
};

class CSWCRoom : public CSWRoom {
public:
    CSWCRoom();
    virtual ~CSWCRoom();
};

CSWCRoom::CSWCRoom()
{
}

CSWCRoom::~CSWCRoom()
{
}

// Unannotated: an array new makes cl put the vector deleting destructor in
// the vtable (CSWCArea allocates its rooms this way).
CSWCRoom* NewClientRooms(int nRooms)
{
    return new CSWCRoom[nRooms];
}

// FUNCTION: SWKOTOR 0x00604B60 SYMBOL: ??_ECSWCRoom@@UAEPAXI@Z
