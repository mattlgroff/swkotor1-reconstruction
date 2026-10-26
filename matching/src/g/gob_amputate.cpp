// Gob::Amputate (0x0044b670, vtable +0xb0): cuts a part sub-tree off the
// instance tree and releases the models appended under it. Own minimal
// layouts (gob.h's list has an out-of-line Add).
typedef unsigned int size_t;

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int n)
    {
        num = 0;
        array_size = 0;
        element = 0;
        if (n)
            Allocate(n);
    }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
    void Allocate(int n);                       // 0x005e03c0
    __forceinline void Add(T t)
    {
        if (num == array_size) {
            array_size = array_size ? array_size * 2 : 8;
            T* pOld = element;
            element = new T[array_size];
            for (int i = 0; i < num; i++)
                element[i] = pOld[i];
            if (pOld)
                delete[] pOld;
        }
        element[num++] = t;
    }
    int Contains(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                n++;
        return n;
    }
    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                return i;
        return -1;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (int i = nIndex; i < num; i++)
            element[i] = element[i + 1];
    }
    void Remove(T t)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == t)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
};

class MaxTree {
public:
    char pad00[0x54];
    int m_nRefCount;                            // +0x54

    ~MaxTree() {}
    static void operator delete(void* p);       // 0x0044aec0
};

class Model : public MaxTree {
};

class Part {
public:
    virtual ~Part();

    char pad04[0x30 - 0x04];
    CExoArrayList<Part*> m_lChildren;           // +0x30
    Part* m_pParent;                            // +0x3c
};

Part* __cdecl FindPart(Part* pRoot, char* szName);  // 0x0043fe00

extern int g_bNoModelDelete;                    // 0x007a8a04

class Gob {
public:
    virtual ~Gob();

    char pad04[0x6c - 0x04];
    Part* m_pRoot;                              // +0x6c
    char pad70[0xec - 0x70];
    CExoArrayList<Model*> m_lAttachedModels;    // +0xec
    CExoArrayList<Part*> m_lAttachedParts;      // +0xf8
    char pad104[0x167 - 0x104];
    unsigned char m_bDirty;                     // +0x167

    int Amputate(char* szPart);
};

// FUNCTION: SWKOTOR 0x0044b670
// NONMATCH (122): flow, calls and the inlined list helpers agree; the
// original keeps the stack's element/num in ebp/edi across the loop and
// spills `this` and the popped part (frame 0x18 vs 0x10).
int Gob::Amputate(char* szPart)
{
    Part* pPart = FindPart(m_pRoot, szPart);
    if (!pPart)
        return 0;
    CExoArrayList<Part*> lStack(8);
    lStack.element[lStack.num++] = pPart;
    while (lStack.num) {
        Part* pCurrent = lStack.element[--lStack.num];
        for (int i = 0; i < pCurrent->m_lChildren.num; i++)
            lStack.Add(pCurrent->m_lChildren.element[i]);
        if (m_lAttachedParts.Contains(pCurrent)) {
            int nIndex = m_lAttachedParts.IndexOf(pCurrent);
            Model* pModel = m_lAttachedModels.element[nIndex];
            if (!g_bNoModelDelete) {
                if (--pModel->m_nRefCount == 0)
                    delete pModel;
            }
            m_lAttachedParts.DelIndex(nIndex);
            m_lAttachedModels.DelIndex(nIndex);
        }
    }
    pPart->m_pParent->m_lChildren.Remove(pPart);
    pPart->m_pParent = 0;
    delete pPart;
    m_bDirty = 1;
    return 1;
}
