// CNetLayer, the in-process loopback frame layer (0x005D4CB0-0x005D57D0).
// Callers are defined before their callees so the small callees stay calls.
#include <string.h>

#include "j/net_layer.h"

#define NET_BYTE_VALUE(n) (nSize >= (n) + 1 ? pData[n] : 0)
#define NET_BYTE_CHAR(n) ((nSize >= (n) + 1 && isalnum(pData[n])) ? pData[n] : '_')

// FUNCTION: SWKOTOR 0x005D4CB0
int CNetLayer::SendMessageToPlayer(unsigned long nPlayer, unsigned char* pData,
                                   unsigned long nSize, unsigned long nFlags)
{
    if (!m_pPeer) {
        return 0;
    }
    unsigned long nOffset = m_pPeer->m_nWritePosition;
    if (nOffset >= 0x10000) {
        nOffset = 0;
    }
    unsigned long nEnd = nOffset - (nSize & 3) + nSize + 8;
    if (nEnd > m_pPeer->m_nReadPosition + 0x10000) {
        CExoString sError;
        sError.Format("CNetLayer::SendMessageToPlayer - Message Buffer Overflow! Message Header Follows:\n"
                      "Size = %d! <%c %x, %c %x, %c %x, %c %x, %c %x, %c %x, %c %x, %c %x>\n",
                      nSize,
                      NET_BYTE_CHAR(0), NET_BYTE_VALUE(0),
                      NET_BYTE_CHAR(1), NET_BYTE_VALUE(1),
                      NET_BYTE_CHAR(2), NET_BYTE_VALUE(2),
                      NET_BYTE_CHAR(3), NET_BYTE_VALUE(3),
                      NET_BYTE_CHAR(4), NET_BYTE_VALUE(4),
                      NET_BYTE_CHAR(5), NET_BYTE_VALUE(5),
                      NET_BYTE_CHAR(6), NET_BYTE_VALUE(6),
                      NET_BYTE_CHAR(7), NET_BYTE_VALUE(7));
    }
    *(unsigned long*)(m_pPeer->m_pFrameBuffer + nOffset) = nSize;
    memcpy(m_pPeer->m_pFrameBuffer + nOffset + 4, pData, nSize);
    m_pPeer->m_nWritePosition = nEnd;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D4FB0
int CNetLayer::StartServerMode(const CExoString& sSessionName, int nUnused)
{
    if (m_bServerMode == 1) {
        return 0;
    }
    m_bServerMode = 1;
    m_sSessionName = sSessionName;
    m_sPlayerPassword = "";
    m_sGameMasterPassword = "";
    m_sServerAdminPassword = "";
    return 1;
}

// FUNCTION: SWKOTOR 0x005D5020
int CNetLayer::EndServerMode()
{
    if (!m_bServerMode) {
        return 0;
    }
    m_bServerMode = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D5040
int CNetLayer::StartConnectToSession(int nUnused, const CExoString& sPassword,
                                     const CExoString& sSessionName, int nUnused2,
                                     int nType)
{
    if (m_bConnecting == 1) {
        return 0;
    }
    m_bConnecting = 0;
    unsigned long nSize = sPassword.GetLength() + 9;
    unsigned char* pMessage = new unsigned char[nSize];
    pMessage[0] = 'B';
    pMessage[1] = 'N';
    pMessage[2] = 'C';
    pMessage[3] = 'S';
    pMessage[4] = 'U';
    pMessage[5] = 0;
    pMessage[6] = 0;
    pMessage[7] = (unsigned char)nType;
    pMessage[8] = (unsigned char)sPassword.GetLength();
    memcpy(pMessage + 9, sPassword.CStr(), sPassword.GetLength());
    SendMessageToPlayer(0, pMessage, nSize, 0);
    m_bConnecting = 1;
    m_sConnectSessionName = sSessionName;
    m_nConnectType = nType;
    m_bConnectReplied = 0;
    m_nConnectStatus = 0;
    m_bConnectAccepted = 0;
    delete[] pMessage;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D5650
int CNetLayer::UpdateStatusLoop(unsigned long nMessage)
{
    if (nMessage == 0x400000 && m_bConnecting == 1 && m_bConnectReplied == 1 &&
        m_nConnectStatus != 2) {
        EndConnectToSession();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D5140
int CNetLayer::EndConnectToSession()
{
    if (!m_bConnecting) {
        return 0;
    }
    m_bConnecting = 0;
    m_bConnectReplied = 0;
    m_nConnectStatus = 0;
    m_bConnectAccepted = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D5170
CNetLayerPlayerInfo* CNetLayer::GetPlayerInfo(unsigned long nPlayer)
{
    return &m_cPlayerInfo;
}

// FUNCTION: SWKOTOR 0x005D57D0
void CNetLayer::ProcessReceivedFrames(int bProcess)
{
    if (!bProcess) {
        return;
    }
    while (m_nReadPosition != m_nWritePosition) {
        if (m_nReadPosition >= 0x10000) {
            m_nReadPosition = 0;
        }
        unsigned long nSize = *(unsigned long*)(m_pFrameBuffer + m_nReadPosition);
        unsigned long nData = m_nReadPosition + 4;
        unsigned long nNext = nData - (nSize & 3) + nSize + 4;
        unsigned char* pData = m_pFrameBuffer + nData;
        m_nReadPosition = nData;
        if (pData[0] == 'B' && m_pFrameBuffer[nData + 1] == 'N') {
            NonWindowMessages(0, pData, nSize);
        }
        else {
            m_pWindow->ProcessReceivedMessage(0, pData, nSize, 0);
        }
        m_nReadPosition = nNext;
    }
}

// FUNCTION: SWKOTOR 0x005D5690
int CNetLayer::NonWindowMessages(unsigned long nPlayer, unsigned char* pData, unsigned long nSize)
{
    if (pData[0] == 'B' && pData[1] == 'N') {
        if (m_bServerMode == 1 && pData[2] == 'C' && pData[3] == 'S' && nSize >= 7) {
            HandleBNCSMessage(nPlayer, pData, nSize);
        }
        if (m_bConnecting == 1 && pData[2] == 'C' && pData[3] == 'R') {
            HandleBNCRMessage(nPlayer, pData, nSize);
        }
        return 0;
    }
    m_pWindow->ProcessReceivedMessage(nPlayer, pData, nSize, 1);
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5180
int CNetLayer::HandleBNCSMessage(unsigned long nPlayer, unsigned char* pData, unsigned long nSize)
{
    CExoString sPlayerName;
    CExoString sPassword;
    if (nSize < 9) {
        return 1;
    }
    unsigned long nNameLength = pData[8];
    unsigned long nType = pData[7];
    if (nSize != nNameLength + 9) {
        return 1;
    }
    sPlayerName = CExoString((char*)pData + 9, nNameLength);
    unsigned char* pReply = new unsigned char[10];
    pReply[0] = 'B';
    pReply[1] = 'N';
    pReply[2] = 'C';
    pReply[3] = 'R';
    m_cPlayerInfo.m_sPlayerName = sPlayerName;
    m_cPlayerInfo.m_bPlayerInUse = 1;
    if (nType == 0x40) {
        if (m_sServerAdminPassword != "") {
            m_cPlayerInfo.m_bPlayerPrivileges = 0;
            m_cPlayerInfo.m_bGameMasterPrivileges = 0;
            m_cPlayerInfo.m_bServerAdminPrivileges = 1;
        }
    }
    else if (nType == 0x20) {
        if (m_sGameMasterPassword != "") {
            m_cPlayerInfo.m_bServerAdminPrivileges = 0;
            m_cPlayerInfo.m_bPlayerPrivileges = 1;
            m_cPlayerInfo.m_bGameMasterPrivileges = 0;
        }
    }
    else if (m_sPlayerPassword != "") {
        m_cPlayerInfo.m_bPlayerPrivileges = 0;
        m_cPlayerInfo.m_bGameMasterPrivileges = 1;
        m_cPlayerInfo.m_bServerAdminPrivileges = 1;
    }
    pReply[4] = 'A';
    if ((nType == 0x10 && m_sPlayerPassword != "") ||
        (nType == 0x20 && m_sGameMasterPassword != "") ||
        (nType == 0x40 && m_sServerAdminPassword != "")) {
        pReply[5] = 'C';
        m_pWindow->PlayerConnected(0, 1, 0);
    }
    else {
        pReply[5] = 'P';
    }
    pReply[6] = (unsigned char)nType;
    pReply[7] = 'U';
    pReply[8] = 0;
    pReply[9] = 0;
    SendMessageToPlayer(0, pReply, 10, 0);
    delete[] pReply;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D53F0
int CNetLayer::HandleBNCRMessage(unsigned long nPlayer, unsigned char* pData, unsigned long nSize)
{
    if (nSize != 10) {
        return 0;
    }
    if (pData[4] != 'A') {
        m_bConnectAccepted = 0;
        return 1;
    }
    if (pData[5] == 'C') {
        m_bConnectReplied = 1;
        m_nConnectStatus = 1;
        m_bConnectAccepted = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D54A0
CNetLayer::CNetLayer()
{
    m_pPeer = 0;
    m_bInitialized = 0;
}

// FUNCTION: SWKOTOR 0x005D5440
CNetLayerPlayerInfo::CNetLayerPlayerInfo()
{
    m_bPlayerInUse = 0;
    m_sPlayerName = "";
    m_bGameMasterPrivileges = 0;
    m_bPlayerPrivileges = 0;
    m_bServerAdminPrivileges = 0;
}

// FUNCTION: SWKOTOR 0x005D5550
int CNetLayer::Initialize(CNetLayerWindow* pWindow)
{
    if (!g_pNetLayerList) {
        g_pNetLayerList = this;
    }
    else {
        m_pPeer = g_pNetLayerList;
        g_pNetLayerList->m_pPeer = this;
    }
    m_nReadPosition = 0;
    m_nWritePosition = 0;
    if (m_bInitialized == 1) {
        return 0;
    }
    m_pWindow = pWindow;
    m_bInitialized = 1;
    m_bServerMode = 0;
    m_cPlayerInfo.Reset();
    m_bConnecting = 0;
    m_nConnectType = 0;
    m_bConnectReplied = 0;
    m_nConnectStatus = 0;
    m_bConnectAccepted = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D5720
CNetLayer::~CNetLayer()
{
    if (m_bInitialized == 1) {
        ShutDown();
    }
}

// FUNCTION: SWKOTOR 0x005D5600
int CNetLayer::ShutDown()
{
    if (!m_bInitialized) {
        return 0;
    }
    if (m_bServerMode == 1) {
        m_bServerMode = 0;
    }
    if (m_pPeer) {
        m_pPeer->m_pPeer = 0;
    }
    if (g_pNetLayerList == this) {
        g_pNetLayerList = m_pPeer;
    }
    m_bInitialized = 0;
    return 1;
}
