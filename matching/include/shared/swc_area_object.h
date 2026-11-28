// swc_area_object.h -- CSWCAreaObject
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_c_message_handlers.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_AREA_OBJECT_H
#define SHARED_SWC_AREA_OBJECT_H

#include "shared/exo_string.h"

class Vector;

class CSWCAreaObject {
public:
    virtual ~CSWCAreaObject();
    virtual void Unknown04();
    virtual void Unknown08();
    virtual void Unknown0C();
    virtual void Unknown10();
    virtual void Unknown14();
    virtual void Unknown18();
    virtual void Unknown1C();
    virtual void Unknown20();
    virtual void Unknown24();
    virtual void Unknown28();
    virtual void Unknown2C();
    virtual void Unknown30();
    virtual void Unknown34();
    virtual void Unknown38();
    virtual void Unknown3C();
    virtual void Unknown40();
    virtual void Unknown44();
    virtual void Unknown48();
    virtual void Unknown4C();
    virtual void Unknown50();
    virtual void Unknown54();
    virtual void Unknown58();
    virtual void Unknown5C();
    virtual void Unknown60();
    virtual void Unknown64();
    virtual void Unknown68();
    virtual void Unknown6C();
    virtual void Unknown70();
    virtual void Unknown74();
    virtual void Unknown78();
    virtual void Unknown7C();
    virtual void Unknown80();
    virtual void Unknown84();
    virtual void Unknown88();
    virtual void SetPosition(const Vector&);
    char pad_004[0xfc-4];
    int m_bEnabled;
    CExoString m_sNote;
    unsigned long m_nReference;
    int m_bReference;
    CSWCAreaObject();
};

#endif
