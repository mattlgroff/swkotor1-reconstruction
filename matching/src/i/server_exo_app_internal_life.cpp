// CServerExoAppInternal construction and destruction.
#include "i/server_exo_app.h"

class CObjectTableManager {
public:
    void ClearAll();                                                // 0x004AE5F0
};

class CAppManager {
public:
    CObjectTableManager* GetObjectTableManager(int nTable);         // 0x00401060
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC

// The live server internals (placeholder name).
extern CServerExoAppInternal* g_pServerExoAppInternal;              // 0x00831FDC
extern const unsigned long PLAYER_ID_INVALID;                       // 0x0074C524 (placeholder name)

// FUNCTION: SWKOTOR 0x004B7230
CServerExoAppInternal::~CServerExoAppInternal()
{
    if (m_pPlayerList) {
        while (!m_pPlayerList->IsEmpty()) {
            CSWSPlayer* pPlayer = m_pPlayerList->RemoveHead();
            if (pPlayer) {
                delete pPlayer;
            }
        }
        delete m_pPlayerList;
    }
    if (m_pSysAdminList) {
        while (!m_pSysAdminList->IsEmpty()) {
            CSWSPlayer* pPlayer = m_pSysAdminList->RemoveHead();
            if (pPlayer) {
                delete pPlayer;
            }
        }
        delete m_pSysAdminList;
    }
    if (m_p10018) {
        delete m_p10018;
    }
    delete m_pExclusionList2;
    m_pExclusionList2 = NULL;
    delete m_pExclusionList1;
    m_pExclusionList1 = NULL;
    g_pServerExoAppInternal = NULL;
    g_pAppManager->GetObjectTableManager(1)->ClearAll();
}

// FUNCTION: SWKOTOR 0x004B55E0
CServerExoAppInternal::CServerExoAppInternal()
{
    m_nShutdownRemaining = 0;
    m_bDebugMode = 0;
    m_pServerInfo = NULL;
    m_nServerMode = 0;
    m_oidModule = OBJECT_INVALID_ID;
    m_pPlayerList = new CExoLinkedList<CSWSPlayer>;
    m_pSysAdminList = new CExoLinkedList<CSWSPlayer>;
    m_pad10014 = 0;
    m_pServerMessage = NULL;
    g_pServerExoAppInternal = this;
    m_p10018 = new unsigned long[16];
    for (unsigned int i = 0; i < 16; i++) {
        m_p10018[i] = PLAYER_ID_INVALID;
    }
    m_nPauseState = 0;
    m_bDoingStartNewModule = 0;
    m_bMoveToModulePending = 0;
    m_sMoveToModuleString = "";
    m_bLoadingModule = 0;
    m_bAutoSavePending = 0;
    m_bExportAllCharactersPending = 0;
    m_pad100c0 = 0;
    m_pExclusionList2 = new CExoLinkedList<unsigned long>;
    m_pExclusionList1 = new CExoLinkedList<unsigned long>;
    m_nEstimatedSaveSize = 0;
    m_posPC = NULL;
    m_sLastConversation = "";
    m_oidActionConversation = 0;
    m_bIgnoreConversationStartRange = 0;
    m_bLoadFromSaveGame = 0;
    m_bButtonMashCheck = 0;
    m_oidLastModule = 0x7f000000;
    m_pLastModule = NULL;
    m_oidLastArea = 0x7f000000;
    m_pLastArea = NULL;
    m_nMoveToPausedCalendarDay = 0;
    m_nMoveToPausedTimeOfDay = 0;
    m_nAutoSaveElapsed = 0;
    m_pad1b928[0] = 0;
    m_pad1b928[1] = 0;
    m_bAutoSaveDue = 0;
    m_nAutoSaveLastTime = 0;
    m_sSaveGameName = "";
}
