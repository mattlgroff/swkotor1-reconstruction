class CSWSDialogNode {
public:
    char pad_0x00[0xa8];
    CSWSDialogNode();
};

class CSWSDialogEntry : public CSWSDialogNode {
public:
    unsigned long m_nReplies;
    void* m_pReplies;

    CSWSDialogEntry();
};

// FUNCTION: SWKOTOR 0x005A0E60
CSWSDialogEntry::CSWSDialogEntry()
{
    m_nReplies = 0;
    m_pReplies = 0;
}
