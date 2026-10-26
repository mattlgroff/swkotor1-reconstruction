// CAurTriangleListElem and its CExoArrayList (aurgrass.o,
// 0x004A7220-0x004A8230). Lists here grow by doubling from 8.
#ifndef I_TRIANGLE_LIST_H
#define I_TRIANGLE_LIST_H

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    CExoArrayList(const CExoArrayList<T>& o)
    {
        num = 0;
        array_size = 0;
        element = 0;
        for (int i = 0; i < o.num; i++)
            Add(o.element[i]);
    }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);
    void Allocate(int n);
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        } else {
            Allocate(n);
        }
        num = n;
    }
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
};

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

class MaxFace;
class PartTriMesh;

class CAurTriangleListElem {
public:
    CAurTriangleListElem()                              // 0x004A7220
    {
        m_pFace = 0;
        m_pPart = 0;
        m_nCount = 0;
    }
    CAurTriangleListElem(CExoArrayList<float> lCorners, MaxFace* pFace, PartTriMesh* pPart,
                         int nCount);                   // 0x004A7240
    ~CAurTriangleListElem();                            // 0x004A72D0

    CExoArrayList<float> m_lCorners;                    // +0x00
    MaxFace* m_pFace;                                   // +0x0c
    PartTriMesh* m_pPart;                               // +0x10
    int m_nCount;                                       // +0x14
};

#endif
