// ~CExoInputInternal (b005e6 cluster), worker C r8. Self-contained: the
// member containers need destructors that exo_input.h's users must not see.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* RemoveHead();                             // 0x005E9C80
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    T& operator[](int i) { return element[i]; }
    void Allocate(int n);
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
};

class CExoCriticalSection {
public:
    void* m_pcExoCriticalSectionInternal;           // 0x00

    ~CExoCriticalSection();                         // 0x005E9320
};

class CExoInputClass {
public:
    ~CExoInputClass();                              // 0x005E0AF0
    char pad_0x0[0x30];
};

struct CExoInputEvent {
    unsigned long m_nEvent;                         // 0x00
    unsigned long m_nValue1;                        // 0x04
    unsigned long m_nValue2;                        // 0x08
    unsigned long m_nValue3;                        // 0x0c
};

class CExoInputEventDesc {
public:
    virtual int IsAxis();
    char pad_0x4[0x34];
};

struct CExoInputRepeatEvent {
    unsigned long m_nEvent;                         // 0x00
    char pad_0x4[0x18];
};

struct CExoInputBufferedPayload {
    char pad_0x0[0x20];
};

struct CExoInputBufferedEvent {
    unsigned long m_nEvent;                         // 0x00
    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    CExoInputBufferedPayload* m_pPayload;           // 0x0c
};

class CExoRawInputInternal {
public:
    ~CExoRawInputInternal();                        // 0x005E3F90
};

class CExoInputInternal {
public:
    virtual ~CExoInputInternal();

    char pad_0x4[0x4];
    CExoInputClass m_aClasses[6];                   // 0x08
    CExoArrayList<CExoInputEventDesc*> m_aEventDescs;   // 0x128
    CExoLinkedList<CExoInputEventDesc*> m_lstCoolDown;  // 0x134
    CExoLinkedList<CExoInputRepeatEvent> m_lstRepeatEvents; // 0x138
    char pad_0x13c[0x4];
    CExoRawInputInternal* m_pRawInput;              // 0x140
    CExoArrayList<CExoInputEvent> m_aActiveEvents;  // 0x144
    char pad_0x150[0xc];
    CExoCriticalSection m_csLock;                   // 0x15c
    char pad_0x160[0x4];
    unsigned long m_aKeyMap[0x84];                  // 0x164
    CExoLinkedList<CExoInputBufferedEvent> m_lstBufferedEvents;   // 0x374
    char pad_0x378[0x34];
};

// FUNCTION: SWKOTOR 0x005E0B80
CExoInputInternal::~CExoInputInternal()
{
    if (m_pRawInput) {
        delete m_pRawInput;
        m_pRawInput = 0;
    }
    int nCount = m_aEventDescs.num;
    for (int i = 0; i < nCount; i++) {
        if (m_aEventDescs[i]) {
            delete m_aEventDescs[i];
        }
    }
    m_aEventDescs.SetSize(0);
    CExoLinkedListPosition pos = m_lstRepeatEvents.GetHeadPos();
    while (pos) {
        delete m_lstRepeatEvents.GetAtPos(pos);
        m_lstRepeatEvents.GetNext(pos);
    }
    while (!m_lstBufferedEvents.IsEmpty()) {
        CExoInputBufferedEvent* pEvent = m_lstBufferedEvents.RemoveHead();
        if (pEvent) {
            if (pEvent->m_pPayload) {
                delete pEvent->m_pPayload;
                pEvent->m_pPayload = 0;
            }
            delete pEvent;
        }
    }
}
