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
class CGameObject {
public:
    virtual ~CGameObject();                         // 0x004C3030
    virtual void SetId(unsigned long oid) {}        // 0x04 (placeholder name)
    virtual void ResetUpdateTimes(unsigned long nDay, unsigned long nTime) {}  // 0x08 (placeholder name)
    virtual void* As0c() { return NULL; }
    virtual CSWSObject* AsSWSObject() { return NULL; }
    virtual void* As14() { return NULL; }
    virtual CSWSDoor* AsSWSDoor() { return NULL; }
    virtual void* As1c() { return NULL; }
    virtual void* As20() { return NULL; }
    virtual void* As24() { return NULL; }
    virtual void* As28() { return NULL; }
    virtual void* As2c() { return NULL; }
    virtual CSWSCreature* AsSWSCreature() { return NULL; }
    virtual void* As34() { return NULL; }
    virtual void* As38() { return NULL; }
    virtual void* As3c() { return NULL; }
    virtual void* As40() { return NULL; }
    virtual void* As44() { return NULL; }
    virtual void* As48() { return NULL; }
    virtual void* As4c() { return NULL; }
    virtual void* As50() { return NULL; }
    virtual void* As54() { return NULL; }
    virtual void* As58() { return NULL; }
    virtual void* As5c() { return NULL; }
    virtual void* As60() { return NULL; }
    virtual void* As64() { return NULL; }
    virtual void* As68() { return NULL; }
    virtual void v6c();                             // 0x00506EF0
    virtual void v70();                             // 0x00406490

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

// FUNCTION: SWKOTOR 0x004C3030
CGameObject::~CGameObject()
{
}

// FUNCTION: SWKOTOR 0x004C3040 SYMBOL: ??_GCGameObject@@UAEPAXI@Z

class CExoLocString {
public:
    CExoLocString();                                // 0x005E9E10
    ~CExoLocString();                               // 0x005EA000
    void* m_pInternal;
    unsigned long m_n4;
};

struct CResRef {
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

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
