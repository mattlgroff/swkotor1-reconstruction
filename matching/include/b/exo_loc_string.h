// CExoLocString (localised string: list of language strings plus a StrRef).
#ifndef B_EXO_LOC_STRING_H
#define B_EXO_LOC_STRING_H

#include "b/exo_string.h"

class CExoLocString {
public:
    CExoLocString();                                            // 0x005E9E10
    CExoLocString(const CExoLocString& lsString);               // 0x005E9F80
    ~CExoLocString();                                           // 0x005EA000
    CExoLocString& operator=(const CExoLocString& lsString);    // 0x005E9E70
    int operator!=(const CExoLocString& lsString) const;        // 0x005EA030
    int GetString(int nLanguage, CExoString* pString, unsigned char nGender); // 0x005EA130
    int GetStringLoc(int nLanguage, CExoString& sString, unsigned char nGender); // 0x005EA240

    void* m_pList;                                              // 0x0
    unsigned long m_nStrRef;                                    // 0x4
};

#endif
