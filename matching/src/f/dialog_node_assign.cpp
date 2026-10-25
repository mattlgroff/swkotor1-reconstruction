// CSWSDialogNode assignment isolated from the constructor/destructor TU.

class CExoString {
public:
    CExoString& operator=(const CExoString& s) throw();
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CSWSDialogBase {
public:
    CSWSDialogBase& operator=(const CSWSDialogBase& o) throw();
    char pad_0x0[0x84];
};

class CSWSDialogNodeData {
public:
    CExoString m_sComment;
    unsigned long m_n8;
    unsigned long m_nC;
    unsigned long m_n10;
    unsigned long m_n14;
    unsigned short m_n18;
    char pad_0x1a[2];
    float m_f1C;
    int m_n20;
};

class CSWSDialogNode : public CSWSDialogBase, public CSWSDialogNodeData {
public:
    CSWSDialogNode& operator=(const CSWSDialogNode& o);
};

// FUNCTION: SWKOTOR 0x005A0360
CSWSDialogNode& CSWSDialogNode::operator=(const CSWSDialogNode& o) throw()
{
    CSWSDialogBase::operator=(o);
    m_sComment = o.m_sComment;
    m_n8 = o.m_n8;
    m_nC = o.m_nC;
    m_n10 = o.m_n10;
    m_n14 = o.m_n14;
    m_n18 = o.m_n18;
    m_f1C = o.m_f1C;
    m_n20 = o.m_n20;
    return *this;
}
