// Gob::CollectLights (breadth-first walk of the part tree).
typedef unsigned int size_t;

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++)
            element[i] = pOld[i];
        if (pOld)
            delete[] pOld;
    }
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
};

class PartLight;

class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20();
    virtual PartLight* AsLight();           // +0x24

    char pad04[0x30 - 0x04];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

class Gob {
public:
    char pad00[0x6c];
    Part* m_pRoot;                          // +0x6c

    void CollectLights(CExoArrayList<PartLight*>* pList);
};

// FUNCTION: SWKOTOR 0x00447180
void Gob::CollectLights(CExoArrayList<PartLight*>* pList)
{
    CExoArrayList<Part*> lStack;
    pList->num = 0;
    lStack.Add(m_pRoot);
    for (int i = 0; i < lStack.num; i++) {
        Part* pPart = lStack.element[i];
        PartLight* pLight = pPart->AsLight();
        if (pLight)
            pList->Add(pLight);
        for (int j = 0; j < pPart->m_lChildren.num; j++)
            lStack.Add(pPart->m_lChildren.element[j]);
    }
}
