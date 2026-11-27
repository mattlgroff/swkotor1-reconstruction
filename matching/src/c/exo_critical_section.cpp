// CExoCriticalSection facade destructor (b005e0_exo_foundation_leaves).

#include "shared/exo_critical_section_internal.h"

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    CExoCriticalSection();
    ~CExoCriticalSection();
};

// FUNCTION: SWKOTOR 0x005E9290
CExoCriticalSection::CExoCriticalSection()
{
    m_pcExoCriticalSectionInternal = new CExoCriticalSectionInternal();
}

// FUNCTION: SWKOTOR 0x005E9320
CExoCriticalSection::~CExoCriticalSection()
{
    if (m_pcExoCriticalSectionInternal) {
        delete m_pcExoCriticalSectionInternal;
    }
    m_pcExoCriticalSectionInternal = 0;
}
