// CClientExoAppInternal leftovers, worker C r9: GetModuleScene,
// HandleMessage (capture-file logging + 'P'/'S' dispatch),
// GetActivePauseState.

#include "c/client_exo_app.h"

class CExoFile {
public:
    unsigned long Write(const void* pBuffer, unsigned long nSize, unsigned long nCount);   // 0x005E69A0
    int Write(const CExoString& s);                 // 0x005E6A70
};

// FUNCTION: SWKOTOR 0x005EE6F0
CSWCModuleScene* CClientExoAppInternal::GetModuleScene()
{
    if (m_pModule) {
        return m_pModule->GetScene();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EE700
int CClientExoAppInternal::HandleMessage(unsigned long nPlayerId, unsigned char* pData, unsigned long nSize, int bRaw)
{
    if (g_pAppManager->GetClientExoApp()->GetCaptureEvents() && g_pAppManager->GetClientExoApp()->GetCaptureFile()) {
        CExoString s;
        s.Format("%d %c %d %d %d ",
                 (unsigned long)(g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetSnapshotTime() / 1000),
                 'M', nPlayerId, bRaw, nSize);
        g_pAppManager->GetClientExoApp()->GetCaptureFile()->Write(s);
        g_pAppManager->GetClientExoApp()->GetCaptureFile()->Write(pData, nSize, 1);
        s.Format("\n");
        g_pAppManager->GetClientExoApp()->GetCaptureFile()->Write(s);
    }
    if (bRaw == 0) {
        if (pData[0] == 'P') {
            m_pSaveLoad->HandleMessage(pData, nSize);
        }
        if (pData[0] == 'S') {
            m_pMessageHandler1c0->HandleMessage(pData, nSize);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005EE910
unsigned char CClientExoAppInternal::GetActivePauseState()
{
    if (m_nPauseFlags & 2) {
        return 2;
    }
    unsigned char n = m_nPauseFlags;
    n &= 1;
    return n;
}
