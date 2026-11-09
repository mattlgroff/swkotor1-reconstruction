// CClientExoAppInternal::LoadPlayBackEvents (clientinput.o), worker C s10.
// Parses the text event-capture file into the play-back event list. The
// name is descriptive (no project record); the record member names are too.

#include <stdio.h>
#include "c/client_exo_app.h"

class CExoFile {
public:
    unsigned long Read(void* pBuffer, unsigned long nSize, unsigned long nCount);   // 0x005E6960

    void* m_pcExoFileInternal;                      // 0x00
};

// 0x30-byte captured input event. The constructor (out-of-line copy at
// 0x0061F760) leaves m_nKeyState alone.
struct CClientInputRecord {
    unsigned long m_nTime;                          // 0x00
    char m_cType;                                   // 0x04
    char pad_0x5[0x1];
    unsigned short m_nKey;                          // 0x06
    int m_nParam1;                                  // 0x08
    int m_nParam2;                                  // 0x0c
    int m_nMessage;                                 // 0x10
    int m_nMessageParam;                            // 0x14
    char* m_pData;                                  // 0x18
    unsigned long m_nDataSize;                      // 0x1c
    float m_fX;                                     // 0x20
    float m_fY;                                     // 0x24
    float m_fZ;                                     // 0x28
    unsigned long m_nKeyState;                      // 0x2c

    CClientInputRecord()
    {
        m_nTime = 0;
        m_cType = 0;
        m_nKey = 0;
        m_nParam1 = 0;
        m_nParam2 = 0;
        m_nMessage = 0;
        m_nMessageParam = 0;
        m_pData = 0;
        m_nDataSize = 0;
        m_fX = 0;
        m_fY = 0;
        m_fZ = 0;
    }
};

// FUNCTION: SWKOTOR 0x0061FBA0
void CClientExoAppInternal::LoadPlayBackEvents()
{
    char sLine[80];
    unsigned long nBaseTime;
    unsigned long nTime;
    char cType;
    float fX;
    float fY;
    float fZ;
    int nParam1;
    int nParam2;
    int nCount = 0;
    int bFirstMessage = 0;
    int nPos = 0;

    do {
        if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[nPos], 1, 1)) {
            return;
        }
        if (sLine[nPos] == '\n') {
            nCount++;
        }
        nPos++;
    } while (nCount < 1);
    sscanf(&sLine[7], "%d", &nBaseTime, &cType);

    for (;;) {
        int i;

        nCount = 0;
        i = 0;
        do {
            if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[i], 1, 1)) {
                return;
            }
            if (sLine[i] == ' ') {
                nCount++;
            }
            i++;
            nPos++;
        } while (nCount < 2);
        sscanf(sLine, "%d %c", &nTime, &cType);

        if (cType == 'K' || cType == 'C' || cType == 'I' || cType == 'Z') {
            nCount = 0;
            i = 0;
            do {
                if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[i], 1, 1)) {
                    return;
                }
                if (sLine[i] == '\n') {
                    nCount++;
                }
                i++;
                nPos++;
            } while (nCount < 1);
            CClientInputRecord* pEvent = new CClientInputRecord;
            pEvent->m_nTime = nTime - nBaseTime;
            pEvent->m_cType = cType;
            if (cType == 'K' || cType == 'C') {
                unsigned short nKey;
                sscanf(sLine, "%d", &nKey);
                pEvent->m_nKey = nKey;
            }
            if (cType == 'I' || cType == 'Z') {

                sscanf(sLine, "%d %d", &nParam1, &nParam2);
                pEvent->m_nParam1 = nParam1;
                pEvent->m_nParam2 = nParam2;
            }
            m_lstPlayBackEvents.AddTail(pEvent);
        }

        if (cType == 'M') {
            int nMessage;
            int nMessageParam;
            unsigned long nDataSize;

            if (!bFirstMessage) {
                bFirstMessage = 1;
                CClientInputRecord* pEvent = new CClientInputRecord;
                pEvent->m_nTime = nTime - nBaseTime;
                pEvent->m_cType = 'L';
                m_lstPlayBackEvents.AddTail(pEvent);
            }
            int nNewLines = 0;
            i = 0;
            int nSpaces = 0;
            do {
                if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[i], 1, 1)) {
                    return;
                }
                if (sLine[i] == ' ') {
                    nSpaces++;
                }
                if (sLine[i] == '\n') {
                    nNewLines++;
                }
                i++;
                nPos++;
            } while (nSpaces < 3);
            sscanf(sLine, "%d %d %d ", &nMessage, &nMessageParam, &nDataSize);
            char* pData = new char[nDataSize];
            if (g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(pData, nDataSize, 1) != nDataSize) {
                return;
            }
            nPos += nDataSize;
            while (nNewLines < 1) {
                if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[i], 1, 1)) {
                    return;
                }
                if (sLine[i] == '\n') {
                    nNewLines++;
                }
                i++;
                nPos++;
            }
            CClientInputRecord* pEvent = new CClientInputRecord;
            pEvent->m_nTime = nTime - nBaseTime;
            pEvent->m_cType = cType;
            pEvent->m_nDataSize = nDataSize;
            pEvent->m_nMessageParam = nMessageParam;
            pEvent->m_nMessage = nMessage;
            pEvent->m_pData = pData;
            m_lstPlayBackEvents.AddTail(pEvent);
        }

        if (cType == 'V') {
            nCount = 0;
            i = 0;
            do {
                if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[i], 1, 1)) {
                    return;
                }
                if (sLine[i] == '\n') {
                    nCount++;
                }
                i++;
                nPos++;
            } while (nCount < 1);
            CClientInputRecord* pEvent = new CClientInputRecord;
            pEvent->m_nTime = nTime - nBaseTime;
            pEvent->m_cType = cType;
            sscanf(sLine, "%f %f %f", &fX, &fY, &fZ);
            pEvent->m_fX = fX;
            pEvent->m_fY = fY;
            pEvent->m_fZ = fZ;
            m_lstPlayBackEvents.AddTail(pEvent);
        }

        if (cType == 'Y') {
            unsigned long nKeyState;

            nCount = 0;
            i = 0;
            do {
                if (!g_pAppManager->GetClientExoApp()->GetCaptureFile()->Read(&sLine[i], 1, 1)) {
                    return;
                }
                if (sLine[i] == '\n') {
                    nCount++;
                }
                i++;
                nPos++;
            } while (nCount < 1);
            CClientInputRecord* pEvent = new CClientInputRecord;
            pEvent->m_nTime = nTime - nBaseTime;
            pEvent->m_cType = cType;
            sscanf(sLine, "%x %f %f %f", &nKeyState, &fX, &fY, &fZ);
            pEvent->m_fX = fX;
            pEvent->m_fY = fY;
            pEvent->m_fZ = fZ;
            pEvent->m_nKeyState = nKeyState;
            m_lstPlayBackEvents.AddTail(pEvent);
        }
    }
}
