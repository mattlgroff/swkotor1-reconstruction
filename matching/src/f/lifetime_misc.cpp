// small lifetime leaves (b0058_f_lifetime_leaves): strings,
// item arrays, inventories, pathfinding, VM, sound, input.

class CExoString {
public:
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

// A class whose only destructible member is a CExoString at +0: its
// destructor is a single tail jump into ~CExoString.
class CExoStringHolder {
public:
    ~CExoStringHolder();
    CExoString m_sValue;
};

// FUNCTION: SWKOTOR 0x0059EA30
CExoStringHolder::~CExoStringHolder()
{
}

// ---------------------------------------------------------------------------

class CSWBaseItem {
public:
    ~CSWBaseItem();
    char pad_0x0[0xc8];
};

class CSWBaseItemArray {
public:
    ~CSWBaseItemArray();
    int m_nNumBaseItems;           // +0x0
    CSWBaseItem *m_pBaseItems;     // +0x4
};

// FUNCTION: SWKOTOR 0x005B4730
CSWBaseItemArray::~CSWBaseItemArray()
{
    if (m_pBaseItems) {
        delete[] m_pBaseItems;
        m_pBaseItems = 0;
        m_nNumBaseItems = 0;
    }
}

// ---------------------------------------------------------------------------

struct CExoLinkedListNode;

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();
    void *RemoveHead();
    CExoLinkedListNode *pHead;
    CExoLinkedListNode *pTail;
    unsigned long nCount;
};

template <class T>
class CExoLinkedList {
public:
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
    T *RemoveHead() { return (T *)m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

struct CSWSPlayerLUOItem {
    unsigned long m_oidItem;
};

class CSWSPlayerLUOInventory {
public:
    void ClearRepository();
    void ClearContainer();

    char pad_0x0[0x90];
    CExoLinkedList<CSWSPlayerLUOItem> m_lRepository;   // +0x90
    char pad_0x94[0x4];
    CExoLinkedList<CSWSPlayerLUOItem> m_lContainer;    // +0x98
};

// FUNCTION: SWKOTOR 0x005CA470
void CSWSPlayerLUOInventory::ClearRepository()
{
    while (!m_lRepository.IsEmpty())
        delete m_lRepository.RemoveHead();
}

// FUNCTION: SWKOTOR 0x005CA4A0
void CSWSPlayerLUOInventory::ClearContainer()
{
    while (!m_lContainer.IsEmpty())
        delete m_lContainer.RemoveHead();
}

struct CExoSoundSourceEntry;

// UnwindLinkedList: the out-of-line CExoLinkedList<T> destructor.
// FUNCTION: SWKOTOR 0x005D86C0 SYMBOL: ??1?$CExoLinkedList@UCExoSoundSourceEntry@@@@QAE@XZ
template class CExoLinkedList<CExoSoundSourceEntry>;

// ---------------------------------------------------------------------------

class CPathfindInformation {
public:
    ~CPathfindInformation();
    void Shutdown();
};

// FUNCTION: SWKOTOR 0x005CFD90
CPathfindInformation::~CPathfindInformation()
{
    Shutdown();
}

// ---------------------------------------------------------------------------
// Scalar deleting destructors whose destructor lives elsewhere: MSVC emits
// the vftable (and ??_G) in the TU that instantiates the constructor.

class CVirtualMachineInternal {
public:
    CVirtualMachineInternal() {}
    virtual ~CVirtualMachineInternal();
};
CVirtualMachineInternal *NewVirtualMachineInternal() { return new CVirtualMachineInternal; }

// FUNCTION: SWKOTOR 0x005D4C10 SYMBOL: ??_GCVirtualMachineInternal@@UAEPAXI@Z

class CRes {
public:
    CRes();
    virtual ~CRes();
    char pad_0x4[0x24];
};

class CResNCS : public CRes {
public:
    CResNCS() {}
    virtual ~CResNCS();
};
CResNCS *NewResNCS() { return new CResNCS; }

// FUNCTION: SWKOTOR 0x005D4C90 SYMBOL: ??_GCResNCS@@UAEPAXI@Z

class CResLYT;

template <class T, int nType>
class CResHelper {
public:
    CResHelper() {}
    virtual ~CResHelper();
};
CResHelper<CResLYT, 3000> *NewResHelperLYT() { return new CResHelper<CResLYT, 3000>; }

// FUNCTION: SWKOTOR 0x005DE750 SYMBOL: ??_G?$CResHelper@VCResLYT@@$0LLI@@@UAEPAXI@Z

// ---------------------------------------------------------------------------

class CExoStreamingSoundSourceInternal {
public:
    ~CExoStreamingSoundSourceInternal();
};

class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource();
    CExoStreamingSoundSourceInternal *m_pcExoStreamingSoundSourceInternal;
};

// FUNCTION: SWKOTOR 0x005D6200
CExoStreamingSoundSource::~CExoStreamingSoundSource()
{
    if (m_pcExoStreamingSoundSourceInternal) {
        delete m_pcExoStreamingSoundSourceInternal;
        m_pcExoStreamingSoundSourceInternal = 0;
    }
}

class CExoSoundInternal {
public:
    ~CExoSoundInternal();
};

class CExoSound {
public:
    ~CExoSound();
    CExoSoundInternal *m_pcExoSoundInternal;
};

// FUNCTION: SWKOTOR 0x005D62F0
CExoSound::~CExoSound()
{
    if (m_pcExoSoundInternal) {
        delete m_pcExoSoundInternal;
        m_pcExoSoundInternal = 0;
    }
}

// ---------------------------------------------------------------------------

class CExoInputDevice {
public:
    void KeyboardModeOn();
    void KeyboardModeOff();
};

class CExoInputInternal {
public:
    void KeyboardModeOn();
    void KeyboardModeOff();

    char pad_0x0[0x4];
    int m_bKeyboardMode;             // +0x4
    char pad_0x8[0x138];
    CExoInputDevice *m_pKeyboard;    // +0x140
};

// FUNCTION: SWKOTOR 0x005DFF60
void CExoInputInternal::KeyboardModeOn()
{
    if (m_pKeyboard)
        m_pKeyboard->KeyboardModeOn();
    m_bKeyboardMode = 1;
}

// FUNCTION: SWKOTOR 0x005DFF80
void CExoInputInternal::KeyboardModeOff()
{
    if (m_pKeyboard) {
        m_pKeyboard->KeyboardModeOff();
        m_bKeyboardMode = 0;
    } else {
        m_bKeyboardMode = 1;
    }
}
