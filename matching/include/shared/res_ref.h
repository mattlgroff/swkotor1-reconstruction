// res_ref.h -- CResRef, a 16-character resource name
//
// One declaration for every unit (CODE_RULES.md, R1). The methods are the
// Mac debug map's, all out of line. There is no user-written copy
// constructor (the map lists none): a CResRef copies as its 16 bytes, the
// way the getters that return one by value copy it. Callers that pass a
// string where a CResRef is taken by value let the conversion build the
// argument in place (`new C2DA("GENDER", 0)`).
#ifndef SHARED_RES_REF_H
#define SHARED_RES_REF_H

class CExoString;

class CResRef {
public:
    char m_resRef[16];

    CResRef();                                      // 0x00405ED0
    CResRef(const char* pResRef, unsigned char nLength); // 0x00405EF0
    CResRef(const CExoString& sResRef);             // 0x00406D60
    CResRef(const char* szResRef);                  // 0x00406D80
    CResRef(unsigned char* pResRef);                // 0x00406DA0

    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    CResRef operator=(unsigned char* pResRef);      // 0x00406170
    CResRef operator=(const CExoString& sResRef);   // 0x004061F0
    CResRef operator=(const char* szResRef);        // 0x00406290
    CResRef operator+=(const CExoString& sString);  // 0x00406300

    int operator==(const CResRef& cResRef) const;   // 0x004060B0
    int operator==(const char* szResRef) const;     // 0x004060D0
    int operator!=(const char* szResRef) const;     // 0x00406060
    int operator!=(const CResRef& cResRef) const;   // 0x00406DC0
    int operator!=(const CExoString& sResRef) const; // 0x00406DE0

    void CopyToString(CExoString& sStr) const;      // 0x00405F70
    void CopyToString(char* pStr) const;            // 0x00405FB0
    char* GetResRefStr() const;                     // 0x00405FE0
    int IsValid() const;                            // 0x00406030
    unsigned char GetLength() const;                // 0x00406040
    char* GetResRef() const;                        // 0x00641DB0
};

#endif
