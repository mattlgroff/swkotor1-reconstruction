#include "c/client_exo_app.h"

class CSWCMessage;
class CExoString;

extern "C" CSWCMessage* __fastcall CClientExoApp_GetMessage(void*, void*);   // 0x005ED6F0
extern "C" int __fastcall CSWCCreatureStats_GetClass(void*, void*, unsigned char); // 0x00647730
extern "C" int __fastcall C2DA_GetInt(void*, void*, int, CExoString*, int*); // 0x00413660
extern "C" int __fastcall CClientExoAppInternal_ShowTutorialWindow(
    void*, void*, int, unsigned long, int, unsigned long);                    // 0x005EDF40
extern "C" int __fastcall CSWCMessage_SendForceAction(
    void*, void*, unsigned long, int, int, unsigned long, const float*);      // 0x00677BD0
extern "C" int __fastcall CSWCMessage_SendForcePowerAction(
    void*, void*, unsigned long, unsigned long, int, int, int, const float*, unsigned long); // 0x006776A0

extern void* g_pRules;                                                        // 0x007A3A28

// FUNCTION: SWKOTOR 0x0060F330
void CSWCCreature::ForceMenuAction(unsigned long nAction, CSWCCreature* pCreature)
{
    const unsigned long nMaskedAction = nAction & 0xDFFFFFFF;
    CAppManager* pManager = *(CAppManager**)0x007A39FC;
    CClientExoApp* pClient = pManager->m_pClientExoApp;
    CSWCMessage* pMessage = CClientExoApp_GetMessage(pClient, 0);
    if (pMessage == 0) {
        return;
    }

    char* pStats = *(char**)((char*)pCreature + 0x2F8);
    unsigned char nSlot = 0;
    unsigned long nWorkingAction = nAction;
    for (;;) {
        nWorkingAction = (nWorkingAction & 0xFFFFFF00) | nSlot;
        if (CSWCCreatureStats_GetClass(pStats, 0, nSlot) == 3 ||
            CSWCCreatureStats_GetClass(pStats, 0, nSlot) == 4 ||
            CSWCCreatureStats_GetClass(pStats, 0, nSlot) == 5) {
            break;
        }
        ++nSlot;
        nWorkingAction = (nWorkingAction & 0xFFFFFF00) | nSlot;
        if (nSlot >= 2) {
            if (*(unsigned short*)(pStats + 0x24) == 5) {
                (void)CSWCMessage_SendForceAction(pMessage, 0, nMaskedAction, 0, 0,
                    *(unsigned long*)((char*)this + 4), (float*)((char*)this + 0x24));
            }
            return;
        }
    }

    CExoString sColumn("FORCEHOSTILE");
    char* pRules = *(char**)0x007A3A28;
    void* pTable = *(void**)(*(char**)(pRules + 0xB8) + 0xF4);
    int nUnused = 0;
    int bFound = C2DA_GetInt(pTable, 0, (int)nMaskedAction, &sColumn, &nUnused);
    pManager = *(CAppManager**)0x007A39FC;
    pClient = pManager->m_pClientExoApp;
    int nResult = CClientExoAppInternal_ShowTutorialWindow(pClient->m_pcInternal, 0,
        bFound != 0 ? 4 : 3, *(unsigned long*)((char*)this + 4), nSlot, nMaskedAction);
    if (nResult != 0) {
        return;
    }

    const float vPosition[3] = { (float)nResult, (float)nResult, (float)nResult };
    pManager = *(CAppManager**)0x007A39FC;
    pClient = pManager->m_pClientExoApp;
    pMessage = CClientExoApp_GetMessage(pClient, 0);
    if (pMessage != 0) {
        (void)CSWCMessage_SendForcePowerAction(pMessage, 0, nMaskedAction,
            nWorkingAction, 0, 0, 0, vPosition,
            *(unsigned long*)((char*)this + 4));
    }
}
