// CGuiInGame reply-array allocation (guiingame.o).

#include "n/gui_in_game_dialog.h"

// FUNCTION: SWKOTOR 0x0062BC70
int CGuiInGame::InitializeRepliesData(unsigned long nCount)
{
    int bResult = 0;
    ClearRepliesData();
    m_nReplies = nCount;
    m_pReplies = new CExoString[nCount];
    m_pRepliesData = new int[m_nReplies];
    m_pReplySpeakers = new unsigned long[m_nReplies];
    m_pShot = new int[m_nReplies];
    m_pShotCameraId = new int[m_nReplies];
    m_pShot12C = new float[m_nReplies];
    m_pShot130 = new float[m_nReplies];
    m_pShot134 = new int[m_nReplies];
    m_pReply138 = new unsigned short[m_nReplies];
    m_pReply13C = new float[m_nReplies];
    m_pShotVideoEffect = new int[m_nReplies];
    m_pReply144 = new unsigned char[m_nReplies];
    m_pReply148 = new Vector[m_nReplies];
    m_pReply14C = new float[m_nReplies];
    m_pReply150 = new float[m_nReplies];
    m_pParticipantCounts = new int[m_nReplies];
    m_ppParticipantIds = new unsigned long*[m_nReplies];
    m_ppParticipantAnims = new unsigned short*[m_nReplies];
    if (m_pReplies && m_pRepliesData && m_pReplySpeakers && m_pShot && m_pShotCameraId
        && m_pShot12C && m_pShot130 && m_pShot134 && m_pReply138 && m_pReply13C
        && m_pShotVideoEffect && m_pReply144 && m_pReply148 && m_pReply14C && m_pReply150
        && m_pParticipantCounts && *m_ppParticipantIds && *m_ppParticipantAnims)
        bResult = 1;
    return bResult;
}
