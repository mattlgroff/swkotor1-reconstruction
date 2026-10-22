// CExoLinkedList<T> constructors (b005f2_exo_linked_list_ctors, b005e4_exo_pimpl_ctors).

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    CExoLinkedListInternal();                       // 0x005E9AA0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedList();
};

template <class T>
CExoLinkedList<T>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal();
}

class CSWCPlayer;
class CSWBehaviorAttachSound;
class CExoString;
struct CExoInputEventDesc;
class CFileSection;
struct ExoLocString_st;

template class CExoLinkedList<CSWCPlayer>;
template class CExoLinkedList<CSWBehaviorAttachSound>;
template class CExoLinkedList<CExoString>;
template class CExoLinkedList<CExoInputEventDesc*>;
template class CExoLinkedList<CFileSection>;
template class CExoLinkedList<ExoLocString_st>;

// FUNCTION: SWKOTOR 0x005F4F40 SYMBOL: ??0?$CExoLinkedList@VCSWCPlayer@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x00605800 SYMBOL: ??0?$CExoLinkedList@VCSWBehaviorAttachSound@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x0062B510 SYMBOL: ??0?$CExoLinkedList@VCExoString@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x005E0350 SYMBOL: ??0?$CExoLinkedList@PAUCExoInputEventDesc@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x005EA4D0 SYMBOL: ??0?$CExoLinkedList@VCFileSection@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x005EC690 SYMBOL: ??0?$CExoLinkedList@UExoLocString_st@@@@QAE@XZ
