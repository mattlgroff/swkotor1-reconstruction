// Worker A round s9: CSWSArea::AsSWSArea (CGameObject vtable 0x00747B88 slot 0x28). The game
// object base sits at +0x11c behind the CSWArea (+0) and CResHelper (+0x100) bases; this unit
// declares only that shape. No functions.csv row.
class CSWSArea;

class CSWAreaBase {
public:
    virtual void v00();
    char pad_0x4[0x100 - 0x4];
};

class CResHelperAreaBase {
public:
    virtual void v00();
    char pad_0x4[0x1c - 0x4];
};

#include "shared/game_object.h"

class CSWSArea : public CSWAreaBase, public CResHelperAreaBase, public CGameObject {
public:
    virtual CSWSArea* AsSWSArea();
};

// FUNCTION: SWKOTOR 0x0050D340
CSWSArea* CSWSArea::AsSWSArea()
{
    return this;
}
