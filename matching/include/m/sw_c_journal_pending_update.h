// CExoArrayList<CSWCJournalPendingUpdate> storage and string ownership.
#ifndef M_SW_C_JOURNAL_PENDING_UPDATE_H
#define M_SW_C_JOURNAL_PENDING_UPDATE_H

#include "shared/exo_string.h"

struct CSWCJournalPendingUpdate {
    CExoString m_sText;
    unsigned long m_nCompleted;
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);
    void Add(T t);
};

#endif
