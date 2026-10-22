// Pilot 4 arm A: CClientExoAppInternal::CreateCaptureEventFile.
#include "pilot4/a/p4a_client_internal.h"

// FUNCTION: SWKOTOR 0x005F5720
void CClientExoAppInternal::CreateCaptureEventFile(const CExoString& sName)
{
    m_bCapturing = 1;
    if (m_n90 == 1) {
        m_bCapturing = 0;
        return;
    }
    if (sName == "") {
        m_bCapturing = 0;
        return;
    }
    if (m_pCaptureFile) {
        delete m_pCaptureFile;
        m_pCaptureFile = 0;
    }
    CExoString sFile;
    sFile.Format("HD0:%s", sName.CStr());
    m_pCaptureFile = new CExoFile(sFile, 10, "wb");
    if (m_pCaptureFile) {
        if (m_pCaptureFile->IsOpen()) {
            unsigned long nStart = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
            CExoString sLine;
            sLine.Format("Start: %d\n", nStart);
            m_pCaptureFile->Write(sLine);
        } else {
            delete m_pCaptureFile;
            m_pCaptureFile = 0;
        }
    }
}
