// CSWSMessage server-admin packet helpers.
#include "b/sws_message.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x00579890
int CSWSMessage::SendServerToServerAdminMessage(unsigned long nPlayerId, const CExoString& sMessage)
{
    unsigned long nSize = sMessage.GetLength() + 1;
    unsigned char* pMessage = new unsigned char[nSize];
    if (!pMessage) {
        return 0;
    }
    pMessage[0] = 'S';
    unsigned long nTextLength = sMessage.GetLength();
    memcpy(pMessage + 1, sMessage.CStr(), nTextLength);
    CNetLayer* pNetLayer = g_pAppManager->GetServerExoApp()->GetNetLayer();
    int bResult = pNetLayer->SendMessageToPlayer(nPlayerId, pMessage, nSize, 0);
    delete[] pMessage;
    return bResult;
}

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x00579A80
void CSWSMessage::SendServerToServerAdminPlayerList(unsigned long nPlayerId)
{
    CExoString sMessage;
    sMessage.Format("%s.%s ", g_sServerStatusKey, g_sPlayerListKey);
    sMessage = sMessage + g_pAppManager->GetServerExoApp()->GetPlayerListString();
    SendServerToServerAdminMessage(nPlayerId, sMessage);
}


// FUNCTION: SWKOTOR 0x00579920
void CSWSMessage::SendServerToServerAdminServerStatus(unsigned long nPlayerId)
{
    CExoString sMessage;
    CExoString sModule;
    unsigned short nState = g_pAppManager->GetServerExoApp()->GetServerMode();
    if (nState == 0) {
        sMessage.Format("%s.%s", g_sServerStatusKey, g_sIdleKey);
        SendServerToServerAdminMessage(nPlayerId, sMessage);
    } else {
        sModule = g_pAppManager->GetServerExoApp()->GetModuleName();
        if (nState == 1) {
            sMessage.Format("%s.%s %s", g_sServerStatusKey, g_sModuleLoadedKey, sModule.CStr());
            SendServerToServerAdminMessage(nPlayerId, sMessage);
        } else if (nState == 2) {
            sMessage.Format("%s.%s %s", g_sServerStatusKey, g_sModuleRunningKey, sModule.CStr());
            SendServerToServerAdminMessage(nPlayerId, sMessage);
        }
    }
}
