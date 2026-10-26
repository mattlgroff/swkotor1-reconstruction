// Highest node id in a model node tree (0x00438400). The original inlines
// the recursion ten levels deep; the deepest level calls the out-of-line
// CExoArrayList::operator[].

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

class MdlNode {
public:
    unsigned short m_nType;                 // +0x00
    short m_nId;                            // +0x02
    char pad04[0x2c - 0x04];
    CExoArrayList<MdlNode*> m_lChildren;    // +0x2c
};

#pragma inline_recursion(on)
#pragma inline_depth(10)

// FUNCTION: SWKOTOR 0x00438400
int __cdecl MdlNodeCountNodes(MdlNode* pNode, int nMax)
{
    if (pNode->m_nId > nMax)
        nMax = pNode->m_nId;
    for (int i = 0; i < pNode->m_lChildren.num; i++)
        nMax = MdlNodeCountNodes(pNode->m_lChildren[i], nMax);
    return nMax;
}
