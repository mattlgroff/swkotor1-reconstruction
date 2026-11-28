// exo_string.h -- CExoString, the engine's heap-allocated string (exostring)
//
// One declaration for every unit (CODE_RULES.md, R1). Every method is an
// out-of-line function of the executable; the Mac symbol tables name each
// one, and c/exo_string_r8.cpp defines them. Parameter types follow those
// definitions where they differ from the Mac names (const char*).
#ifndef SHARED_EXO_STRING_H
#define SHARED_EXO_STRING_H

class CExoString
{
public:
    CExoString();                                           // 0x005B3190
    CExoString(const char* s);                              // 0x005E5A90
    CExoString(const CExoString& s);                        // 0x005E5B00
    CExoString(const char* s, int nLength);                 // 0x005E5B70
    CExoString(int n);                                      // 0x005E5BC0
    ~CExoString();                                          // 0x005E5C20

    CExoString& operator=(const char* s);                   // 0x005E5140
    CExoString& operator=(const CExoString& s);             // 0x005E5C50
    CExoString operator+(const CExoString& s) const;        // 0x005E5D10
    int operator==(const CExoString& s) const;              // 0x005E5200
    int operator==(const char* s) const;                    // 0x005E5290
    int operator!=(const CExoString& s) const;              // 0x005E5310
    int operator!=(const char* s) const;                    // 0x005E5390
    int operator<(const CExoString& s) const;               // 0x005E5410
    int operator>(const CExoString& s) const;               // 0x005E5490
    int operator<=(const CExoString& s) const;              // 0x005E5500
    int operator>=(const CExoString& s) const;              // 0x005E5580
    char operator[](int nIndex) const;                      // 0x005E5600

    int AsINT() const;                                      // 0x005E5630
    float AsFLOAT() const;                                  // 0x005E5650
    char* CStr() const;                                     // 0x005E5670
    void Format(const char* szFormat, ...);                 // 0x005E5680
    int GetLength() const;                                  // 0x005E5790
    void Insert(const CExoString& s, int nPosition);        // 0x005E57B0
    int IsEmpty() const;                                    // 0x005E58A0
    int CompareNoCase(const char* s) const;                 // 0x005E58C0
    int ComparePrefixNoCase(const CExoString& s, int nLength) const; // 0x005E5900
    int StripNonAlphaNumeric(int bFileName, int bSpaces, int bUnderscores); // 0x005E5950
    int Find(const CExoString& s, int nStart) const;        // 0x005E5EB0
    int Find(char c, int nStart) const;                     // 0x005E5F20
    CExoString Left(int nCount) const;                      // 0x005E5F70
    CExoString LowerCase() const;                           // 0x005E6080
    CExoString Right(int nCount) const;                     // 0x005E6170
    CExoString SubString(int nStart, int nCount) const;     // 0x005E6270
    CExoString UpperCase() const;                           // 0x005E6360
    int CompareNoCase(const CExoString& s) const;           // 0x005E6450

    char* m_sString;                                        // +0x00
    unsigned long m_nBufferLength;                          // +0x04
};

#endif
