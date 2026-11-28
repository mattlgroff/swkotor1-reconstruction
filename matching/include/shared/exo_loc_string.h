// exo_loc_string.h -- CExoLocString, a localised string: per-language text
// plus a TLK string reference
//
// One declaration for every unit (CODE_RULES.md, R1). The members and
// special members are the implementation unit's (c/exo_loc_string_r9.cpp);
// the methods and their parameter types follow the Mac debug map.
#ifndef SHARED_EXO_LOC_STRING_H
#define SHARED_EXO_LOC_STRING_H

class CExoString;
class CExoLocStringInternal;

class CExoLocString {
public:
    CExoLocString();                                                // 0x005E9E10
    CExoLocString(const CExoLocString& lsString);                   // 0x005E9F80
    ~CExoLocString();                                               // 0x005EA000
    CExoLocString& operator=(const CExoLocString& lsString);        // 0x005E9E70
    int operator==(const CExoLocString& lsString) const;            // 0x005EA020
    int operator!=(const CExoLocString& lsString) const;            // 0x005EA030
    int Compare(const CExoLocString& lsString) const;               // 0x005E9F10

    void ClearLocString();                                          // 0x005E9EA0
    unsigned long GetStringCount() const;                           // 0x005E9EF0
    unsigned long GetStringLength(int nIndex);                      // 0x005E9F00
    int GetString(int nIndex, int* pLanguage, CExoString& sString, unsigned char* pGender); // 0x005E9EB0
    int GetString(int nLanguage, CExoString* pString, unsigned char nGender); // 0x005EA050
    int GetStringAndCache(int nLanguage, CExoString* pString, unsigned char nGender); // 0x005EA130
    int GetStringLoc(int nLanguage, CExoString* pString, unsigned char nGender) const; // 0x005EA240
    float GetStringReadTime(int nLanguage, unsigned char nGender);  // 0x005EA270
    int AddString(int nLanguage, const CExoString& sString, unsigned char nGender); // 0x005EA340
    void RemoveString(int nLanguage, unsigned char nGender);
    void SetStrRef(unsigned long nStrRef) { m_dwStringRef = nStrRef; }
    int GetStringInternal(int nId, CExoString* pString) const;      // 0x005EC9C0

    // The internal list keys a string by language and gender.
    static int MakeId(int nLanguage, unsigned char nGender)
    {
        nGender = nLanguage ? nGender : 0;
        return nLanguage * 2 + nGender;
    }
    static int MakeIdRef(int nLanguage, unsigned char& nGender)
    {
        nGender = nLanguage ? nGender : 0;
        return nLanguage * 2 + nGender;
    }

    CExoLocStringInternal* m_pcExoLocStringInternal;                // +0x00
    unsigned long m_dwStringRef;                                    // +0x04
};

#endif
