// CAurFontRender teardown and the global font list (0x004A21D0-0x004A22E0).
#ifndef I_FONT_RENDER_H
#define I_FONT_RENDER_H

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Remove(T value);                               // 0x00445080
    __forceinline void RemoveInline(T value)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == value)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    __forceinline void DelIndexInline(int nIndex)
    {
        num--;
        for (int i = nIndex; i < num; i++)
            element[i] = element[i + 1];
    }
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
            m_p->m_lSafePointers.RemoveInline(this);
    }
    void Reset()
    {
        if (m_p)
            m_p->m_lSafePointers.Remove(this);
        m_p = 0;
    }

    T* m_p;
};

void AurTextureRelease(CAurTexture* pTexture);          // 0x00420D70

class CAurFontRender {
public:
    ~CAurFontRender();                                  // 0x004A21D0

    int m_nIndex;                                       // +0x00
    char pad_0x04[0x1c - 0x04];
    SafePointer<CAurTexture> m_pTexture;                // +0x1c
};

extern CExoArrayList<CAurFontRender*> g_aFonts;         // 0x0083052C

void AurDeleteAllFonts();                               // 0x004A22E0

#endif
