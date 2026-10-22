// CAurGUIStringInternal teardown and factory
// (0x0045BC70 .. 0x0045BE80).

#include "h/scene.h"

void AurTextureRelease(CAurObject* pTexture);          // 0x00420D70

template <class T>
class GUIList : public CExoArrayList<T> {
public:
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
        }
        array_size = 0;
        num = 0;
    }
};

class CAurGUIImage {
public:
    virtual ~CAurGUIImage() {}
};

class CAurGUIStringInternal : public CAurGUIImage {
public:
    int m_n4;                                   // +0x04
    int m_n8;                                   // +0x08
    int m_nC;                                   // +0x0c
    int m_n10;                                  // +0x10
    char* m_szString;                           // +0x14
    SafePointerBase m_pFont;                    // +0x18
    char pad_0x1c[0xc];
    GUIList<float> m_lA;                        // +0x28
    GUIList<float> m_lB;                        // +0x34
    char pad_0x40[8];

    CAurGUIStringInternal(char* szText, char* szFont, Vector vColor, int n);  // 0x0045B990
    virtual ~CAurGUIStringInternal();           // 0x0045BC70
};

// FUNCTION: SWKOTOR 0x0045BC70
CAurGUIStringInternal::~CAurGUIStringInternal()
{
    CAurObject* pFont = m_pFont.m_p;
    m_pFont.Reset();
    AurTextureRelease(pFont);
    m_pFont.Reset();
    delete m_szString;
    m_szString = 0;
    m_lB.Clear();
    m_lA.Clear();
    m_n4 = 0;
    m_n8 = 0;
    m_nC = 0;
    m_n10 = 0;
}

// FUNCTION: SWKOTOR 0x0045BDF0
CAurGUIImage* NewCAurGUIString(char* szText, char* szFont, Vector& vColor, int n)
{
    return new CAurGUIStringInternal(szText, szFont, vColor, n);
}

// FUNCTION: SWKOTOR 0x0045BE80 SYMBOL: ??_GCAurGUIStringInternal@@UAEPAXI@Z
