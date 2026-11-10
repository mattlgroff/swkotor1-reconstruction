// Worker O matching headers: CSWNameGen, the random name generator over a
// CResLTR letter table (0x00711090-0x00712190).
#ifndef O_SW_NAME_GEN_H
#define O_SW_NAME_GEN_H

#include "o/aurora_res_helper.h"

// vtable 0x0075FC10
class CSWNameGen : public CResHelper<CResLTR, 2036> {
public:
    CSWNameGen();
    virtual ~CSWNameGen();
    void UnloadNameTable();
    int LoadNameTable(const CExoString& sTable);
    CExoString GetRandomName(unsigned char nMaxLength);      // 0x00711390

    int m_bLoaded;                                  // 0x1c
    unsigned short m_n20;                           // 0x20
    unsigned char m_n22;                            // 0x22
};

#endif
