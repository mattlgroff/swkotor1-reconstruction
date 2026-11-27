// Pilot 4 arm A: CClientExoAppInternal capture-file subset.
#ifndef P4A_CLIENT_INTERNAL_H
#define P4A_CLIENT_INTERNAL_H

#include "shared/exo_string.h"

class CExoFile {
public:
    CExoFile(const CExoString& sFileName, unsigned short nResType, const CExoString& sMode); // 0x005E68D0
    ~CExoFile();                                        // 0x005E69E0
    int IsOpen();                                       // 0x005E6A10
    int Write(const CExoString& s);                     // 0x005E6A70

    void* m_pInternal;
};

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();          // 0x005E9260
};

class CExoBase {
public:
    void* m_pUnknown00;
    CExoTimers* m_pcExoTimers;                          // 0x04
};

extern CExoBase* g_pExoBase;                            // 0x007A39E0

class CClientExoAppInternal {
public:
    void CreateCaptureEventFile(const CExoString& sName);   // 0x005F5720

    char pad_0x0[0x88];
    CExoFile* m_pCaptureFile;                           // 0x88
    int m_bCapturing;                                   // 0x8c
    int m_n90;                                          // 0x90
};

#endif
