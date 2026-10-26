// CGuiInGame::~CGuiInGame (guiingame.o). Standalone layout: only the
// members with destructors (+0xB18 .. +0xB58) and the owned pointers.
class CExoString {
public:
    char* m_sString; unsigned long m_nLength;
    ~CExoString(); // 0x005E5C20
};
class CExoLinkedListInternal {
public:
    void* m_pcExoLinkedListNode;
    ~CExoLinkedListInternal(); // 0x005E9AB0
    void* RemoveHead(); // 0x005E9C80
};
template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedList;
    ~CExoLinkedList() { delete m_pcExoLinkedList; }
    void* GetHead() { return m_pcExoLinkedList->m_pcExoLinkedListNode; }
    T* RemoveHead() { return (T*)m_pcExoLinkedList->RemoveHead(); }
};
class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource(); // 0x005D6200
    void Stop(); // 0x005D5B80
};
struct CGuiDialogMessage { char m_data[0x10]; ~CGuiDialogMessage(); /* 0x00628600 */ };
struct CDialogLiveContent { char m_data[0x10]; ~CDialogLiveContent(); /* 0x0059EA30 */ };
class CGuiInGame {
public:
    char pad_0x0[0xf8];
    CDialogLiveContent* m_pLiveContent; // +0xF8
    CGuiDialogMessage* m_pDialogMessages; // +0xFC
    char pad_0x100[0xa18];
    CExoString m_sB18;
    char pad_0xb20[0x20];
    CExoStreamingSoundSource* m_pDialogSound; // +0xB40
    CExoLinkedList<unsigned long> m_lstLockedOrientation; // +0xB44
    CExoLinkedList<unsigned long> m_lstLockedHeadFollow; // +0xB48
    CExoLinkedList<CExoString> m_lstDialogNonDependant; // +0xB4C
    char pad_0xb50[8];
    CExoString m_sB58[8];
    char pad_0xb98[0x24];
    CExoStreamingSoundSource* m_pSoundBBC; // +0xBBC
    ~CGuiInGame();
    void ShutDown(); // 0x0062C310
    void ShutDownGlobal(); // 0x0062A970
};

// FUNCTION: SWKOTOR 0x006303B0
CGuiInGame::~CGuiInGame()
{
    ShutDown();
    ShutDownGlobal();
    delete[] m_pDialogMessages;
    delete[] m_pLiveContent;
    while (m_lstLockedOrientation.GetHead())
        delete m_lstLockedOrientation.RemoveHead();
    if (m_pSoundBBC) {
        delete m_pSoundBBC;
        m_pSoundBBC = 0;
    }
    if (m_pDialogSound) {
        m_pDialogSound->Stop();
        delete m_pDialogSound;
        m_pDialogSound = 0;
    }
}
