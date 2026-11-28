// CGameObject destructor pair, the NWPlayerCharacterList_st
// constructor and a CExoLinkedList constructor emitted in the CSWSModule
// translation unit (0x004C3030-0x004C689A). Self-contained declarations.
#include <stddef.h>

class CSWSObject;
class CSWSDoor;
class CSWSCreature;

// vtable 0x007457A0: destructor, two empty setters (0x0060E760 RET 4,
// 0x005B5E90 RET 8), 25 "As" casts folded onto 0x0063E7F0 (return 0),
// then 0x00506EF0 and 0x00406490.
#include "shared/game_object.h"

// ~CGameObject (0x004C3030) and its deleting destructor (0x004C3040) are
// inline in shared/game_object.h; i/sws_module_life.cpp emits them, for
// CSWSModule's constructor.

#include "shared/exo_loc_string.h"

#include "shared/res_ref.h"

struct NWPlayerCharacterList_st {
    NWPlayerCharacterList_st();                     // 0x004C6830
    CExoLocString m_sFirstName;                     // +0x00
    CExoLocString m_sLastName;                      // +0x08
    unsigned char m_n10;                            // +0x10
    CResRef m_cResRef11;                            // +0x11
    unsigned short m_n22;                           // +0x22
    CResRef m_cResRef24;                            // +0x24
    unsigned char* m_pData;                         // +0x34
    unsigned long m_n38;                            // +0x38
    unsigned long m_n3c;                            // +0x3c
};

// FUNCTION: SWKOTOR 0x004C6830
NWPlayerCharacterList_st::NWPlayerCharacterList_st()
{
    m_n38 = 0;
    m_n3c = 0;
    m_pData = NULL;
    m_n10 = 0;
    m_n22 = 0xffff;
}

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                       // 0x005E9AA0
    void* m_pHead;
    void* m_pTail;
    unsigned long m_nCount;
};

template <class T> class CExoLinkedList {
public:
    CExoLinkedList();
    CExoLinkedListInternal* m_pcExoLinkedListInternal;
};

template <class T> CExoLinkedList<T>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal;
}

struct CSWSModulePlayerEntry;

// FUNCTION: SWKOTOR 0x004C4E20 SYMBOL: ??0?$CExoLinkedList@UCSWSModulePlayerEntry@@@@QAE@XZ
template CExoLinkedList<CSWSModulePlayerEntry>::CExoLinkedList();
