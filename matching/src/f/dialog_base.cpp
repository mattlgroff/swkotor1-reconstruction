// CSWSDialog node classes and small dialog / journal records
// (0x0059E860-0x0059F5B0).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    unsigned long m_nStrRef;
};

class CResRef {
public:
    CResRef();
    CResRef operator=(const CResRef &r);
    char m_resRef[16];
};

class Vector {
public:
    float x, y, z;
};

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();
    ~CExoLinkedListInternal();
    void *m_pHead;
    void *m_pTail;
    int m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList() { m_pcExoLinkedListInternal = new CExoLinkedListInternal(); }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

class CSWSDialogPlayer;

// FUNCTION: SWKOTOR 0x0059F470 SYMBOL: ??0?$CExoLinkedList@VCSWSDialogPlayer@@@@QAE@XZ
template class CExoLinkedList<CSWSDialogPlayer>;

class CSWSDialogBase {
public:
    CSWSDialogBase();
    ~CSWSDialogBase();
    CSWSDialogBase &operator=(const CSWSDialogBase &o);

    CExoString m_sSpeaker;           // +0x0
    CExoLocString m_sText;           // +0x8
    CResRef m_cScript;               // +0x10
    unsigned long m_n20;             // +0x20
    unsigned long m_n24;             // +0x24
    unsigned long m_n28;             // +0x28
    unsigned long m_n2C;             // +0x2c
    unsigned long m_n30;             // +0x30
    CResRef m_cSound;                // +0x34
    CExoString m_sQuest;             // +0x44
    CExoString m_sListener;          // +0x4c
    unsigned long m_n54;             // +0x54
    unsigned long m_n58;             // +0x58
    unsigned long m_n5C;             // +0x5c
    unsigned long m_n60;             // +0x60
    unsigned long m_n64;             // +0x64
    unsigned char m_n68;             // +0x68
    char pad_0x69[0x3];
    unsigned long m_n6C;             // +0x6c
    unsigned long m_n70;             // +0x70
    unsigned long m_n74;             // +0x74
    unsigned long m_n78;             // +0x78
    unsigned long m_n7C;             // +0x7c
    unsigned char m_n80;             // +0x80
    char pad_0x81[0x3];
};

// FUNCTION: SWKOTOR 0x0059E860
CSWSDialogBase::CSWSDialogBase()
    : m_n20(0), m_n24(0), m_n2C(0), m_n30(0), m_n54(0), m_n60(0), m_n64(0), m_n6C(0),
      m_n70(0), m_n74(0), m_n80(0)
{
}

// FUNCTION: SWKOTOR 0x0059F140
CSWSDialogBase &CSWSDialogBase::operator=(const CSWSDialogBase &o)
{
    m_sSpeaker = o.m_sSpeaker;
    m_sText = o.m_sText;
    m_cScript = o.m_cScript;
    m_n20 = o.m_n20;
    m_n24 = o.m_n24;
    m_n28 = o.m_n28;
    m_n2C = o.m_n2C;
    m_n30 = o.m_n30;
    m_cSound = o.m_cSound;
    m_sQuest = o.m_sQuest;
    m_sListener = o.m_sListener;
    m_n54 = o.m_n54;
    m_n58 = o.m_n58;
    m_n5C = o.m_n5C;
    m_n60 = o.m_n60;
    m_n64 = o.m_n64;
    m_n68 = o.m_n68;
    *(Vector *)&m_n6C = *(const Vector *)&o.m_n6C;
    m_n78 = o.m_n78;
    m_n7C = o.m_n7C;
    m_n80 = o.m_n80;
    return *this;
}

class CSWSDialogLinkEntry {
public:
    CSWSDialogLinkEntry();
    CResRef m_cActive;               // +0x0
    unsigned long m_nIndex;          // +0x10
    unsigned long m_n14;             // +0x14
};

// FUNCTION: SWKOTOR 0x0059E900
CSWSDialogLinkEntry::CSWSDialogLinkEntry()
{
    m_nIndex = 0;
    m_n14 = 0;
}

// FUNCTION: SWKOTOR 0x0059E920
CSWSDialogBase::~CSWSDialogBase()
{
}

class CSWSDialogSpeaker {
public:
    CSWSDialogSpeaker();
    unsigned long m_oidSpeaker;      // +0x0
    CExoString m_sTag;               // +0x4
};

// FUNCTION: SWKOTOR 0x0059E990
CSWSDialogSpeaker::CSWSDialogSpeaker() : m_oidSpeaker(0x7f000000)
{
}

class CSWSPlayerJournalQuestUpdates {
public:
    ~CSWSPlayerJournalQuestUpdates();
    unsigned long m_n0;              // +0x0
    CExoString m_sTag;               // +0x4
};

// FUNCTION: SWKOTOR 0x0059EA20
CSWSPlayerJournalQuestUpdates::~CSWSPlayerJournalQuestUpdates()
{
}

class CSWSDialogStuntModel {
public:
    CSWSDialogStuntModel();
    CExoString m_sParticipant;       // +0x0
    CResRef m_cModel;                // +0x8
};

// FUNCTION: SWKOTOR 0x0059EA40
CSWSDialogStuntModel::CSWSDialogStuntModel()
{
}

class CLiveContentStruct {
public:
    CLiveContentStruct();
    CExoString m_sName;              // +0x0
};

// FUNCTION: SWKOTOR 0x0059EA90
CLiveContentStruct::CLiveContentStruct()
{
}

class CSWSDialogNodeData {
public:
    CSWSDialogNodeData()
    {
        m_n8 = 0;
        m_nC = 0;
        m_n10 = 0;
        m_n14 = 0;
        m_n18 = 0x7fff;
        m_f1C = -1.0f;
        m_n20 = -1;
    }
    CExoString m_sComment;           // +0x0
    unsigned long m_n8;              // +0x8
    unsigned long m_nC;              // +0xc
    unsigned long m_n10;             // +0x10
    unsigned long m_n14;             // +0x14
    unsigned short m_n18;            // +0x18
    char pad_0x1a[0x2];
    float m_f1C;                     // +0x1c
    int m_n20;                       // +0x20
};

class CSWSDialogNode : public CSWSDialogBase, public CSWSDialogNodeData {
public:
    CSWSDialogNode();
    ~CSWSDialogNode();
};

// FUNCTION: SWKOTOR 0x0059F4E0
CSWSDialogNode::CSWSDialogNode()
{
}

// FUNCTION: SWKOTOR 0x0059F550
CSWSDialogNode::~CSWSDialogNode()
{
}

class CSWSDialogReply : public CSWSDialogNode {
public:
    CSWSDialogReply();
    unsigned long m_nEntries;        // +0xa8
    CSWSDialogLinkEntry *m_pEntries; // +0xac
    int m_nB0;                       // +0xb0
};

// FUNCTION: SWKOTOR 0x0059F5B0
CSWSDialogReply::CSWSDialogReply()
{
    m_nEntries = 0;
    m_pEntries = 0;
    m_nB0 = -1;
}
