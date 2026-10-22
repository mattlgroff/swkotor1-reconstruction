// CExoString (engine string), shared by worker B sources.
#ifndef B_EXO_STRING_H
#define B_EXO_STRING_H

#include <stddef.h>

// 0x004015A0: a bare `ret` shared by many empty functions (stripped debug
// output in release builds); called cdecl with an id argument.
void __cdecl ExoDebugNoOp(int nId);

class CExoString {
public:
    CExoString();                                       // 0x005B3190
    CExoString(const char* s);                          // 0x005E5A90
    CExoString(const CExoString& s);                    // 0x005E5B00
    CExoString(const char* s, int nLength);             // 0x005E5B70
    CExoString(int n);                                  // 0x005E5BC0
    ~CExoString();                                      // 0x005E5C20
    CExoString& operator=(const CExoString& s);         // 0x005E5C50
    CExoString& operator=(const char* s);
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    int operator==(const CExoString& s) const;          // 0x005E5200
    int operator==(const char* s) const;                // 0x005E5290
    CExoString LowerCase() const;                       // 0x005E6080
    char operator[](int n) const;                       // 0x005E5600
    char* CStr() const;                                 // 0x005E5670
    void Format(char* format, ...);                     // 0x005E5680
    int GetLength() const;                              // 0x005E5790
    int IsEmpty() const;                                // 0x005E58A0
    int CompareNoCase(const CExoString& s) const;       // 0x005E58C0
    int CompareNoCase(const char* s) const;             // 0x005E58C0 (char* view)
    int Find(const CExoString& s, int nStart) const;    // 0x005E5EB0
    int Find(char c, int nStart) const;                 // 0x005E5F20
    CExoString Left(int nCount) const;                  // 0x005E5F70
    CExoString Right(int nCount) const;                 // 0x005E6170
    CExoString SubString(int nStart, int nCount) const; // 0x005E6270
    CExoString UpperCase() const;                       // 0x005E6360
    void Insert(const CExoString& s, int nPosition);    // 0x005E57B0
    int operator!=(const char* s) const;                // 0x005E5390
    int operator!=(const CExoString& s) const;          // 0x005E5310
    int operator<(const CExoString& s) const;           // 0x005E5410

    char* m_sString;
    unsigned long m_nBufferLength;
};

#endif
