// CSWSExpression node/lifetime leaves (b0059_f2_scriptvar_dialog).

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void SetSize(int s)
    {
        if (s == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
            num = 0;
        }
    }
};

class CSWSExpressionNode {
public:
    CSWSExpressionNode() { m_pNext = 0; m_pLeft = 0; }
    ~CSWSExpressionNode();
    void Destroy();

    CSWSExpressionNode *m_pLeft;     // +0x0
    int m_nOp;                       // +0x4
    char pad_0x8[0x4];
    CSWSExpressionNode *m_pNext;     // +0xc
    CExoString m_sValue;             // +0x10
    CSWSExpressionNode *m_pAlternate;   // +0x18
};

class CSWSExpressionList {
public:
    ~CSWSExpressionList() {}
    void DeleteAlternate(CSWSExpressionNode *pNode);
    void AddNode(CSWSExpressionNode *pNode);
    void DeleteList();
    CSWSExpressionNode *m_pHead;     // +0x0
    CSWSExpressionNode *m_pCurrent;  // +0x4
    CSWSExpressionNode *m_pMarker;   // +0x8
};

class CSWSExpression {
public:
    CSWSExpression();
    ~CSWSExpression();
    CSWSExpressionNode *NewNode(int nOp);
    void ClearAlternate(CSWSExpressionNode *pNode);
    void ClearGraph();

    int m_n0;                                // +0x0
    CExoArrayList<CExoString *> m_aStrings;  // +0x4
    int m_n10;                               // +0x10
    CSWSExpressionList *m_pList;             // +0x14
    CExoString m_sText;                      // +0x18
};

// FUNCTION: SWKOTOR 0x0059D7C0
CSWSExpressionNode::~CSWSExpressionNode()
{
}

// FUNCTION: SWKOTOR 0x0059D7D0
void CSWSExpressionList::DeleteAlternate(CSWSExpressionNode *pNode)
{
    while (pNode) {
        if (pNode->m_nOp == 3)
            return;
        CSWSExpressionNode *pNext = pNode->m_pNext;
        if (pNode->m_nOp == 2)
            DeleteAlternate(pNode->m_pAlternate);
        delete pNode;
        pNode = pNext;
    }
}

// FUNCTION: SWKOTOR 0x0059D820
void CSWSExpressionList::AddNode(CSWSExpressionNode *pNode)
{
    if (m_pHead == 0 && m_pMarker == 0) {
        m_pHead = pNode;
        m_pMarker = pNode;
        m_pCurrent = pNode;
        return;
    }
    if (m_pCurrent == m_pMarker)
        m_pMarker = pNode;
    pNode->m_pNext = m_pCurrent->m_pNext;
    m_pCurrent->m_pNext = pNode;
    m_pCurrent = pNode;
}

// FUNCTION: SWKOTOR 0x0059D860
CSWSExpressionNode *CSWSExpression::NewNode(int nOp)
{
    CSWSExpressionNode *pNode = new CSWSExpressionNode;
    pNode->m_nOp = nOp;
    return pNode;
}

// FUNCTION: SWKOTOR 0x0059D8E0
void CSWSExpression::ClearAlternate(CSWSExpressionNode *pNode)
{
    while (pNode) {
        if (pNode->m_nOp == 3)
            return;
        if (pNode->m_nOp == 2)
            ClearAlternate(pNode->m_pAlternate);
        pNode->m_pLeft = 0;
        pNode = pNode->m_pNext;
    }
}

// FUNCTION: SWKOTOR 0x0059D920
void CSWSExpressionList::DeleteList()
{
    CSWSExpressionNode *pNode = m_pHead;
    while (pNode) {
        CSWSExpressionNode *pNext = pNode->m_pNext;
        if (pNode->m_nOp == 2)
            DeleteAlternate(pNode->m_pAlternate);
        delete pNode;
        pNode = pNext;
    }
}

// FUNCTION: SWKOTOR 0x0059D960
void CSWSExpression::ClearGraph()
{
    CSWSExpressionNode *pNode = m_pList->m_pHead;
    while (pNode) {
        if (pNode->m_nOp == 2)
            ClearAlternate(pNode->m_pAlternate);
        pNode->m_pLeft = 0;
        pNode = pNode->m_pNext;
    }
}

// FUNCTION: SWKOTOR 0x0059D990
CSWSExpression::CSWSExpression()
{
    m_pList = 0;
    m_n10 = 0;
    m_n0 = 0;
}

// FUNCTION: SWKOTOR 0x0059D9E0
CSWSExpression::~CSWSExpression()
{
    if (m_pList) {
        m_pList->DeleteList();
        delete m_pList;
    }
    for (int i = 0; i < m_aStrings.num; i++) {
        if (m_aStrings.element[i])
            delete m_aStrings.element[i];
    }
    m_aStrings.SetSize(0);
}
