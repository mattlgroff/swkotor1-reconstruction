// CAurGUIStringInternal wrapped text height (0x0045B7A0). Hidden native
// entry; the method name is descriptive. TU-local minimal classes.

template <class T>
class GUIFloatList {
public:
    T* element;
    int num;
    int array_size;

    GUIFloatList(const GUIFloatList<T>& o);                 // 0x0046D250
    ~GUIFloatList() { if (element) delete[] element; }
    GUIFloatList<T>& operator=(const GUIFloatList<T>& o);   // 0x00438BB0
};

struct CAurFontInfo {
    char pad_0x0[4];
    float m_fAscent;                            // +0x04, descriptive
    char pad_0x8[0xc];
    float m_fDescent;                           // +0x14, descriptive
};

class CAurFont {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual CAurFontInfo* GetInfo();            // +0x38
};

class CAurGUIStringInternal {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void Layout(int nWidth);            // +0x4c, descriptive

    char pad_0x4[0x14];
    CAurFont* m_pFont;                          // +0x18
    char pad_0x1c[0xc];
    GUIFloatList<float> m_lA;                   // +0x28
    GUIFloatList<float> m_lB;                   // +0x34
    float m_fScale;                             // +0x40

    int GetTextHeight(int nWidth);
};

// NONMATCH: the original packs the two branches' list copies into the same
// two stack slots (sub esp,0x18, slots swapped per branch) and multiplies
// the line count in the other operand order; cl keeps four slots here.
// FUNCTION: SWKOTOR 0x0045B7A0
int CAurGUIStringInternal::GetTextHeight(int nWidth)
{
    int nResult;
    if (nWidth == -1) {
        if (m_lB.num) {
            CAurFontInfo* pInfo = m_pFont->GetInfo();
            return (int)(m_lB.num * ((pInfo->m_fDescent + pInfo->m_fAscent) * m_fScale * 100.0f) + 0.5f);
        }
        {
            GUIFloatList<float> lA(m_lA);
            GUIFloatList<float> lB(m_lB);
            Layout(-1);
            short nLines = (short)m_lB.num;
            m_lB = lB;
            m_lA = lA;
            CAurFontInfo* pInfo = m_pFont->GetInfo();
            float fLine = (pInfo->m_fDescent + pInfo->m_fAscent) * m_fScale * 100.0f;
            nResult = (int)(nLines * fLine + 0.5f);
        }
    } else {
        {
            GUIFloatList<float> lB(m_lA);
            GUIFloatList<float> lA(m_lB);
            Layout(nWidth);
            short nLines = (short)m_lB.num;
            m_lB = lA;
            m_lA = lB;
            CAurFontInfo* pInfo = m_pFont->GetInfo();
            int nLine = (int)((pInfo->m_fDescent + pInfo->m_fAscent) * 100.0f + 0.5f);
            nResult = nLines * (int)(nLine * m_fScale + 0.5f);
        }
    }
    return nResult;
}
