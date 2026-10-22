// Pilot group 3: CExoArrayList instances.
// Build flags: /O2 /Gy /GX  (EH frames / eh vector destructor iterator need /GX).
// matchcmp naming: the "<class X>" spelling below is only so the tool can find
// the template symbol in UNDNAME output.

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    ~CExoArrayList();
    void Allocate(int s);
    void AddAndDispose(T t);
};

template <class T>
CExoArrayList<T>::~CExoArrayList()
{
    if (element)
        delete[] element;
    element = 0;
}

template <class T>
void CExoArrayList<T>::AddAndDispose(T t)
{
    if (num == array_size)
        Allocate(array_size ? array_size * 2 : 16);
    element[num++] = t;
}

class CSWGuiInterfaceAction {
public:
    ~CSWGuiInterfaceAction();
    char pad_0x0[0x38];
};

class SJournalEntry {
public:
    SJournalEntry &operator=(const SJournalEntry &);
    ~SJournalEntry();
    char pad_0x0[0x3c];
};

// FUNCTION: SWKOTOR 0x005FBFB0
template CExoArrayList<class CSWGuiInterfaceAction>::~CExoArrayList<class CSWGuiInterfaceAction>();

// FUNCTION: SWKOTOR 0x005C5920
template void CExoArrayList<class SJournalEntry>::AddAndDispose(SJournalEntry t);
