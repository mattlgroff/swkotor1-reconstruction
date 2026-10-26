// CExoArrayList<SafePointer<CAurTexture> >::Add (0x004A8230). Separate
// view so Allocate (0x0043DE00) and operator= (0x00447FA0) stay calls.
#ifndef I_SAFE_TEXTURE_LIST_H
#define I_SAFE_TEXTURE_LIST_H

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);
    void Add(T t);
    void Remove(void* p);                               // 0x00445080
    void RemoveNoThrow(void* p) throw();                // 0x00445080 (same body)
};

class CAurTexture {
public:
    virtual ~CAurTexture();

    CExoArrayList<void*> m_lSafePointers;               // +0x04
};

template <class T>
class SafePointer {
public:
    ~SafePointer()
    {
        if (m_p)
            m_p->m_lSafePointers.RemoveNoThrow(this);
    }
    SafePointer<T>& operator=(T* p);                    // 0x00447FA0
    SafePointer<T>& operator=(const SafePointer<T>& o) { return *this = o.m_p; }

    T* m_p;
};

#endif
