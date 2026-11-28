// CExoInputClass: one input class of CExoInputInternal (+0x08 + n * 0x30),
// constructed/destroyed by the eh vector iterators of CExoInputInternal.
// Self-contained (exo_input.h's containers must not see ctors/dtors).

#include "shared/exo_string.h"

#include "shared/exo_linked_list.h"



template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

class CExoInputClass {
public:
    CExoInputClass();
    ~CExoInputClass();

    CExoString m_sName;                             // 0x00
    CExoLinkedList<unsigned long> m_lstAnySlot;     // 0x08
    CExoLinkedList<unsigned long> m_lstSlots[6];    // 0x0c
    CExoArrayList<unsigned long> m_aBits;           // 0x24
};

// FUNCTION: SWKOTOR 0x005E0A80
CExoInputClass::CExoInputClass()
{
}

// FUNCTION: SWKOTOR 0x005E0AF0
CExoInputClass::~CExoInputClass()
{
}
