// CSWSObject debug overflow report (0x004CE6E0).
#include "i/sws_object.h"

class CSWSPlayer {
public:
    CGameObject* GetGameObject();                   // 0x00561080
};

class CServerAIMaster {
public:
    char pad_0x0[0x60];
    CExoString m_sLastScript;                       // 0x60 (placeholder name)
    int m_nEventCount0;                             // 0x68 (placeholder name)
    int m_nEventCount1;                             // 0x6c (placeholder name)
    int m_nEventCount2;                             // 0x70 (placeholder name)
    int m_nEventCount3;                             // 0x74 (placeholder name)
    int m_nEventCount4;                             // 0x78 (placeholder name)
    int m_nUpdateCount;                             // 0x7c (placeholder name)
};

// FUNCTION: SWKOTOR 0x004CE6E0
void CSWSObject::ReportOverflow(unsigned long nMessage, int nValue, int nLimit, int nUnused)
{
    if (GetServerExoApp()->GetDebugMode() != 1) {
        return;
    }
    CSWSMessage* pMessage = GetServerExoApp()->GetNWSMessage();
    CExoLinkedList<CSWSPlayer>* pPlayers = GetServerExoApp()->GetPlayerList();
    if (pMessage == NULL || pPlayers == NULL) {
        return;
    }
    CExoLinkedListPosition pos = pPlayers->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = pPlayers->GetAtPos(pos);
        if (pPlayer && pPlayer->GetGameObject()) {
            CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
            if (pCreature) {
                CSWCCMessageData* pData = new CSWCCMessageData();
                pData->SetInteger(0, nValue);
                pData->SetInteger(1, nLimit);
                pData->SetObjectID(0, 0x7f000000);
                pData->SetObjectID(1, m_idSelf);
                pData->SetString(1, m_sTag);
                if (nMessage == 0xa3) {
                    CServerAIMaster* pAI = GetServerExoApp()->GetServerAIMaster();
                    pData->SetString(2, pAI->m_sLastScript);
                    pData->SetInteger(2, pAI->m_nUpdateCount);
                    pData->SetInteger(3, pAI->m_nEventCount0 + pAI->m_nEventCount1 * 2 +
                                             pAI->m_nEventCount2 * 4 + pAI->m_nEventCount3 * 8 +
                                             pAI->m_nEventCount4 * 16);
                }
                pCreature->SendFeedbackMessage((unsigned short)nMessage, pData);
            }
        }
        pPlayers->GetNext(pos);
    }
}
