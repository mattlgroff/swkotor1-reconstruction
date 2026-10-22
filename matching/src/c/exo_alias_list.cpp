// CExoAliasList facade (b005e3_exo_file_wrappers, b005e4_exo_pimpl_ctors).

#include "c/exo_base.h"

class CExoAliasListInternal {
public:
    char pad_0x0[0xc];

    CExoAliasListInternal();                        // 0x005EB5B0
    ~CExoAliasListInternal();                       // 0x005EC0E0
    CExoString ResolveFileName(const CExoString& sFileName, unsigned short nResType);   // 0x005EB6B0
};

class CExoAliasList {
public:
    CExoAliasListInternal* m_pcExoAliasListInternal;   // 0x00

    CExoAliasList();
    ~CExoAliasList();
    CExoString ResolveFileName(const CExoString& sFileName, unsigned short nResType);
};

// FUNCTION: SWKOTOR 0x005E6810
CExoAliasList::CExoAliasList()
{
    m_pcExoAliasListInternal = new CExoAliasListInternal();
}

// FUNCTION: SWKOTOR 0x005E68A0
CExoString CExoAliasList::ResolveFileName(const CExoString& sFileName, unsigned short nResType)
{
    return m_pcExoAliasListInternal->ResolveFileName(sFileName, nResType);
}

// FUNCTION: SWKOTOR 0x005E69B0
CExoAliasList::~CExoAliasList()
{
    if (m_pcExoAliasListInternal) {
        delete m_pcExoAliasListInternal;
        m_pcExoAliasListInternal = 0;
    }
}
