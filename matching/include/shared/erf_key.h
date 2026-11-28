// erf_key.h -- CERFKey
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/erf.h); the other copies' members fall on its members.
#ifndef SHARED_ERF_KEY_H
#define SHARED_ERF_KEY_H


class CExoFile;

class CERFKey {
public:
    CERFKey()
    {
        memset(m_cResRef, 0, 16);
        memset(&m_nUnused, 0, 2);
        m_nResID = 0;
        m_nResType = 0xFFFF;
    }
    ~CERFKey();
    int Write(CExoFile* pFile);

    char m_cResRef[16];                             // 0x00
    unsigned long m_nResID;                         // 0x10
    unsigned short m_nResType;                      // 0x14
    unsigned short m_nUnused;                       // 0x16
};

#endif
