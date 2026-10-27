// CSWGuiMapHider destructor (map control, 0x248 bytes, vtable 0x00754738),
// declared with sized member classes.

typedef void* CExoLinkedListPosition;

class CExoLinkedListNode {
public:
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    ~CExoLinkedList() { delete m_pList; }
    CExoLinkedListInternal* m_pList;
};

template <class T>
class CExoArrayList {
public:
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    T& operator[](int i) { return element[i]; }
    T* element;
    int num;
    int array_size;
};

class CSWGuiControl {
public:
    virtual ~CSWGuiControl();                       // 0x00419A20
    char pad_0x4[0x58];
};

// 0x1c4 bytes.
class CSWGuiButton : public CSWGuiControl {
public:
    virtual ~CSWGuiButton();                        // 0x00419DA0
    char pad_0x5c[0x1c4 - 0x5c];
};

// Selection marker object; its deleting destructor is the second slot.
class CSWGuiMapMarker {
public:
    virtual void v00();
    virtual ~CSWGuiMapMarker();
};

class CSWGuiMapImage {
public:
    virtual ~CSWGuiMapImage();
};

struct CSWGuiMapNote {
    unsigned long m_oidNote;
};

class CSWGuiMapHider : public CSWGuiControl {
public:
    ~CSWGuiMapHider();
    void ClearMapNotes();                           // 0x00692E30

    CSWGuiMapMarker* m_pMarker;                     // 0x5c (descriptive)
    CSWGuiMapImage* m_pMapImage;                    // 0x60 (descriptive)
    CSWGuiMapImage* m_pHiderImage;                  // 0x64 (descriptive)
    CSWGuiButton m_cMarker;                         // 0x68 (descriptive)
    CExoArrayList<CSWGuiControl*> m_aControls;      // 0x22c
    CExoLinkedList<CSWGuiMapNote> m_lMapNotes;      // 0x238
    CExoLinkedListPosition m_posSelectedNote;       // 0x23c
    char pad_0x240[0x244 - 0x240];
    int m_bMapNoteFlag;                             // 0x244 (descriptive)
};

// FUNCTION: SWKOTOR 0x006942D0
CSWGuiMapHider::~CSWGuiMapHider()
{
    ClearMapNotes();
    if (m_pMarker) {
        delete m_pMarker;
        m_pMarker = 0;
    }
    if (m_pMapImage) {
        delete m_pMapImage;
        m_pMapImage = 0;
    }
    if (m_pHiderImage) {
        delete m_pHiderImage;
        m_pHiderImage = 0;
    }
    for (int i = 0; i < m_aControls.num; i++) {
        if (m_aControls[i]) {
            delete m_aControls[i];
        }
    }
}
