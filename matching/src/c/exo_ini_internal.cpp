// CExoIniInternal / CFileSection constructors and destructor
// (b005e4_exo_pimpl_ctors).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
};

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedList();                               // 0x005EA4D0 for CFileSection
    ~CExoLinkedList()
    {
        if (m_pcExoLinkedListInternal) {
            delete m_pcExoLinkedListInternal;
        }
    }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
};

class CFileKey {
public:
    CExoString m_sName;                             // 0x00
    CExoString m_sValue;                            // 0x08

    ~CFileKey();                                    // 0x0059EA30
};

class CFileSection {
public:
    CExoString m_sName;                             // 0x00
    CExoLinkedList<CFileKey> m_lstKeys;             // 0x08

    CFileSection();
    ~CFileSection();
};

class CExoIniInternal {
public:
    CExoLinkedList<CFileSection> m_lstSections;     // 0x00
    CExoString m_sFileName;                         // 0x04

    CExoIniInternal();
    void Destroy();
};

// FUNCTION: SWKOTOR 0x005EA540
CExoIniInternal::CExoIniInternal()
{
    m_sFileName = "";
}

// FUNCTION: SWKOTOR 0x005EA5A0
CFileSection::~CFileSection()
{
}

// FUNCTION: SWKOTOR 0x005EA600
CFileSection::CFileSection()
{
}

// FUNCTION: SWKOTOR 0x005EA650
void CExoIniInternal::Destroy()
{
    while (!m_lstSections.IsEmpty()) {
        CFileSection* pSection = m_lstSections.RemoveHead();
        while (!pSection->m_lstKeys.IsEmpty()) {
            CFileKey* pKey = pSection->m_lstKeys.RemoveHead();
            if (pKey) {
                delete pKey;
            }
        }
        delete pSection;
    }
    m_sFileName = "";
}
