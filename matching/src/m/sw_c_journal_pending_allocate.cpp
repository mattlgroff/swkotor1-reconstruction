// CExoArrayList allocation for the journal's pending-update records.
#include "m/sw_c_journal_pending_update.h"

// FUNCTION: SWKOTOR 0x00675DE0 SYMBOL: ?Allocate@?$CExoArrayList@UCSWCJournalPendingUpdate@@@@QAEXH@Z
template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* old = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; ++i) {
        element[i] = old[i];
    }
    if (old) {
        delete[] old;
    }
}

template void CExoArrayList<CSWCJournalPendingUpdate>::Allocate(int n);
