// CSWTlkTable::Fetch (b005e8_exo_rand_client_misc).

#include "shared/exo_string.h"

class CClientExoApp {
public:
    CExoString GetGUIString(unsigned long nStrRef); // 0x005EDEB0
};

class CAppManager {
public:
    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

#include "shared/globals.h"

class CSWSObject;

class CSWTlkTable {
public:
    char pad_0x0[0x3c];
    unsigned char m_nGender;                        // 0x3c
    char pad_0x3d[0x1b];
    CSWSObject* m_pObject;                          // 0x58
    unsigned long m_bFlag : 1;                      // 0x5c

    unsigned char GetObjectGender();                // 0x00674B40
    CExoString Fetch(unsigned long nStrRef, CSWSObject* pObject, int nUnused, int bFlag);
};

// FUNCTION: SWKOTOR 0x005EE390
CExoString CSWTlkTable::Fetch(unsigned long nStrRef, CSWSObject* pObject, int nUnused, int bFlag)
{
    m_pObject = pObject;
    unsigned char nGender = GetObjectGender();
    m_bFlag = bFlag;
    m_nGender = nGender;
    return g_pAppManager->m_pClientExoApp->GetGUIString(nStrRef);
}
