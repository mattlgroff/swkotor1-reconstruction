// CExoArrayList<CSWCJournalPendingUpdate>::Add.
#include "m/sw_c_journal_pending_update.h"

// FUNCTION: SWKOTOR 0x006764A0 SYMBOL: ?Add@?$CExoArrayList@UCSWCJournalPendingUpdate@@@@QAEXUCSWCJournalPendingUpdate@@@Z
template <class T>
void CExoArrayList<T>::Add(T t)
{
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 16);
    }
    element[num++] = t;
}

template void CExoArrayList<CSWCJournalPendingUpdate>::Add(CSWCJournalPendingUpdate t);
