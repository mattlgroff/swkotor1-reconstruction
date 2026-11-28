// CExoIniInternal / CFileSection constructors and destructor
// (b005e4_exo_pimpl_ctors).

#include "shared/exo_string.h"

#include "shared/exo_linked_list.h"



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
