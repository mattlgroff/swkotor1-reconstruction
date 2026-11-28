// erf_res.h -- CERFRes
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/erf.h); the other copies' members fall on its members.
#ifndef SHARED_ERF_RES_H
#define SHARED_ERF_RES_H


class CExoFile;

class CERFRes {
public:
    CERFRes()
    {
        m_nOffset = 0;
        m_nSize = 0;
        m_pData = 0;
    }
    ~CERFRes()
    {
        if (m_pData) {
            delete[] m_pData;
            m_pData = 0;
        }
        m_nOffset = 0;
        m_nSize = 0;
    }
    int CopyToFile(CExoFile* pSrc, CExoFile* pDst, int nUnused);
    int Write(CExoFile* pFile, unsigned long nEntryOffset);

    unsigned long m_nOffset;                        // 0x00
    unsigned long m_nSize;                          // 0x04
    char* m_pData;                                  // 0x08
};

#endif
