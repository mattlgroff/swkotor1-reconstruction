// Pilot 4 arm B: shared declarations.
#ifndef PILOT4_B_COMMON_H
#define PILOT4_B_COMMON_H

class CExoString {
public:
    CExoString();
    CExoString(const char *s);                  // 0x005E5A90
    CExoString(const CExoString &s);            // 0x005E5B00
    ~CExoString();                              // 0x005E5C20
    int operator==(const CExoString &s) const;  // 0x005E5200
    char *CStr() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

#endif
