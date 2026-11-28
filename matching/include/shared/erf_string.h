// erf_string.h -- CERFString
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/erf.h); the other copies' members fall on its members.
#ifndef SHARED_ERF_STRING_H
#define SHARED_ERF_STRING_H

#include "shared/exo_string.h"

class CExoFile;

class CERFString {
public:
    CERFString()
    {
        m_nLanguage = 0;
        m_nLength = 0;
        m_sText = 0;
    }
    ~CERFString();
    void SetText(CExoString& sText);
    int Read(CExoFile* pFile);

    unsigned long m_nLanguage;                      // 0x00
    unsigned long m_nLength;                        // 0x04
    char* m_sText;                                  // 0x08
};

#endif
