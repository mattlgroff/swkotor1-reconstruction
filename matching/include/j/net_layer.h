// Worker J matching headers: CNetLayer (in-process loopback message layer,
// 0x005D4CB0-0x005D57D0) and CNetLayerPlayerInfo.
#ifndef J_NET_LAYER_H
#define J_NET_LAYER_H

#include "j/exo_string.h"

extern "C" int __cdecl isalnum(int c);              // 0x006FCA8F

// Receiver of the frames (the CExoNet side); only the slots used here.
class CNetLayerWindow {
public:
    virtual int PlayerConnected(unsigned long nPlayer, int bConnected, int nUnused); // slot 0
    virtual int ProcessReceivedMessage(unsigned long nPlayer, unsigned char* pData,
                                       unsigned long nSize, int bNonWindow); // slot 1
};

class CNetLayerPlayerInfo {
public:
    CNetLayerPlayerInfo();
    void Reset()
    {
        m_bPlayerInUse = 0;
        m_sPlayerName = "";
        m_bGameMasterPrivileges = 0;
        m_bPlayerPrivileges = 0;
        m_bServerAdminPrivileges = 0;
    }

    int m_bPlayerInUse;                             // 0x00
    CExoString m_sPlayerName;                       // 0x04
    char pad_0xc[0x10];
    int m_bGameMasterPrivileges;                    // 0x1c
    int m_bPlayerPrivileges;                        // 0x20
    int m_bServerAdminPrivileges;                   // 0x24
};

class CNetLayer {
public:
    CNetLayer();
    ~CNetLayer();
    int Initialize(CNetLayerWindow* pWindow);
    int ShutDown();
    int SendMessageToPlayer(unsigned long nPlayer, unsigned char* pData,
                            unsigned long nSize, unsigned long nFlags);
    int StartServerMode(const CExoString& sSessionName, int nUnused);
    int EndServerMode();
    int StartConnectToSession(int nUnused, const CExoString& sPassword,
                              const CExoString& sSessionName, int nUnused2,
                              int nType);
    int EndConnectToSession();
    CNetLayerPlayerInfo* GetPlayerInfo(unsigned long nPlayer);
    int HandleBNCSMessage(unsigned long nPlayer, unsigned char* pData, unsigned long nSize);
    int HandleBNCRMessage(unsigned long nPlayer, unsigned char* pData, unsigned long nSize);
    int UpdateStatusLoop(unsigned long nMessage);
    int NonWindowMessages(unsigned long nPlayer, unsigned char* pData, unsigned long nSize);
    void ProcessReceivedFrames(int bProcess);

    unsigned char m_pFrameBuffer[0x20000];          // 0x00000
    unsigned long m_nReadPosition;                  // 0x20000
    unsigned long m_nWritePosition;                 // 0x20004
    CNetLayerWindow* m_pWindow;                     // 0x20008
    CNetLayer* m_pPeer;                             // 0x2000c
    int m_bInitialized;                             // 0x20010
    CNetLayerPlayerInfo m_cPlayerInfo;              // 0x20014
    int m_bServerMode;                              // 0x2003c
    CExoString m_sSessionName;                      // 0x20040
    CExoString m_sPlayerPassword;                   // 0x20048
    CExoString m_sGameMasterPassword;               // 0x20050
    CExoString m_sServerAdminPassword;              // 0x20058
    int m_bConnecting;                              // 0x20060
    CExoString m_sConnectSessionName;               // 0x20064
    int m_nConnectType;                             // 0x2006c
    int m_bConnectReplied;                          // 0x20070
    int m_nConnectStatus;                           // 0x20074
    int m_bConnectAccepted;                         // 0x20078
};

extern CNetLayer* g_pNetLayerList;                  // 0x0083288C

#endif
