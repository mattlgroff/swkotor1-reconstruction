// Array growth for per-player journal update entries.
#include "b/exo_array_list.h"
#include "b/exo_string.h"

struct CSWSPlayerJournalQuestUpdates {
    ~CSWSPlayerJournalQuestUpdates();                       // 0x0059EA20
    unsigned short m_nUpdateType;
    CExoString m_sPlotID;
};

// FUNCTION: SWKOTOR 0x00527490 SYMBOL: ?Allocate@?$CExoArrayList@UCSWSPlayerJournalQuestUpdates@@@@QAEXH@Z
template void CExoArrayList<CSWSPlayerJournalQuestUpdates>::Allocate(int nSize);
