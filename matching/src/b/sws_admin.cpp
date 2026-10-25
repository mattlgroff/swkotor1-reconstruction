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
