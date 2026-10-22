// Worker J matching headers: CExoString (callees only, 0x005E5xxx).
#ifndef J_EXO_STRING_H
#define J_EXO_STRING_H

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    CExoString(const char* s, int nLength);         // 0x005E5B70
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    int operator!=(const char* s) const;            // 0x005E5290
    int operator==(const CExoString& s) const;      // 0x005E5200
    int IsEmpty() const;                            // 0x005E58A0
    int CompareNoCase(const char* s) const;         // 0x005E58C0
    char* CStr() const;                             // 0x005E5670
    char operator[](int nIndex) const;              // 0x005E5600
    int GetLength() const;                          // 0x005E5790
    int Find(const CExoString& s, int nStart) const;    // 0x005E5EB0
    CExoString Left(int nCount) const;              // 0x005E5F70
    CExoString Right(int nCount) const;             // 0x005E6170
    void Format(const char* szFormat, ...);         // 0x005E5680

    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04
};

#endif
