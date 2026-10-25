// CExoArrayList<CSWCJournalPendingUpdate> storage and string ownership.
#ifndef M_SW_C_JOURNAL_PENDING_UPDATE_H
#define M_SW_C_JOURNAL_PENDING_UPDATE_H

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString();                                  // 0x005B3190
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                 // 0x005E5C20
    CExoString& operator=(const CExoString& s);    // 0x005E5C50
};

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
