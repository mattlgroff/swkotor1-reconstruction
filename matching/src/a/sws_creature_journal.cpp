// Worker A round s7: CSWSCreature::LoadJournal (nwscreature.cpp).
#include "a/sws_creature.h"

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CSWSJournal {
public:
    void SetState(const CExoString& sPlotID, int nState, int bArg);         // 0x005C5A40
    void SetDate(const CExoString& sPlotID, unsigned long nDate, int bArg); // 0x005C63F0
    void SetTime(const CExoString& sPlotID, unsigned long nTime, int bArg); // 0x005C6650
    char pad_0x0[0x4];
    int m_bLoaded;                                  // 0x04 (placeholder name)
};

extern int g_nJournalSortOrder;                     // 0x00833A90

// FUNCTION: SWKOTOR 0x004F17D0
void CSWSCreature::LoadJournal(CResGFF* pRes, CResStruct* pStruct)
{
    CSWSJournal* pJournal = GetServerExoApp()->GetPartyTable()->GetJournal();
    if (!pJournal || pJournal->m_bLoaded) {
        return;
    }
    CResList lEntries;
    if (!pRes->GetList(&lEntries, pStruct, "JNL_Entries")) {
        return;
    }
    int bSuccess;
    g_nJournalSortOrder = pRes->ReadFieldINT(pStruct, "JNL_SortOrder", bSuccess, g_nJournalSortOrder);
    CExoString sPlotID;
    int nCount = pRes->GetListCount(&lEntries);
    for (int i = 0; i < nCount; i++) {
        CResStruct sEntry;
        pRes->GetListElement(&sEntry, &lEntries, i);
        {
            CExoString sDefault("");
            sPlotID = pRes->ReadFieldCExoString(&sEntry, "JNL_PlotID", bSuccess, sDefault);
        }
        int nState = pRes->ReadFieldINT(&sEntry, "JNL_State", bSuccess, 0);
        GetServerExoApp()->GetPartyTable()->GetJournal()->SetState(sPlotID, nState, 1);
        unsigned long nDate = pRes->ReadFieldDWORD(&sEntry, "JNL_Date", bSuccess, 0);
        GetServerExoApp()->GetPartyTable()->GetJournal()->SetDate(sPlotID, nDate, 1);
        unsigned long nTime = pRes->ReadFieldDWORD(&sEntry, "JNL_Time", bSuccess, 0);
        GetServerExoApp()->GetPartyTable()->GetJournal()->SetTime(sPlotID, nTime, 1);
    }
}
