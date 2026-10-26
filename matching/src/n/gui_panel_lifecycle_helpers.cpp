// CSWGuiPanel lifecycle fields and the tooltip's embedded text-parameter
// object, laid out from binary member offsets and the existing symbol map.
class CSWGuiControl {
};

class CExoString {
};

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);            // 0x00415E00
};

class CSWGuiPanel {
public:
    char pad_0x0[0x44];
    union {
        unsigned long m_nFlags;                       // +0x44
        struct {
            unsigned long m_nFlagsLow : 8;
            unsigned long m_bToDestroy : 1;           // bit 8
            unsigned long m_nFlagsHigh : 23;
        } m_bits;
    };
    char pad_0x48[0x20];
    void (CSWGuiPanel::*m_pfnCloseCallback)(CSWGuiControl*); // +0x68
    CSWGuiPanel* m_pCloseObject;                      // +0x6C
    int m_nCloseArgument;                             // +0x70

    void SetToDestroy(int bDestroy);
    void SetCloseCallback(CSWGuiPanel* pObject,
                          void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*),
                          int nArgument);
};

class CSWGuiToolTip {
public:
    char pad_0x0[0x14C];
    CSWGuiTextParams m_cTextParams;                   // +0x14C

    void SetText(const CExoString& sText);
};

// FUNCTION: SWKOTOR 0x00624A00
void CSWGuiPanel::SetToDestroy(int bDestroy)
{
    m_bits.m_bToDestroy = bDestroy;
    if ((m_nFlags & 0x600ul) != 0x400ul)
        m_nFlags = (m_nFlags & 0xFFFFFBFFul) | 0x200ul;
}

// FUNCTION: SWKOTOR 0x00624A40
void CSWGuiPanel::SetCloseCallback(CSWGuiPanel* pObject,
                                   void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*),
                                   int nArgument)
{
    m_pCloseObject = pObject;
    m_pfnCloseCallback = pfnCallback;
    m_nCloseArgument = nArgument;
}

// FUNCTION: SWKOTOR 0x00624AE0
void CSWGuiToolTip::SetText(const CExoString& sText)
{
    m_cTextParams.SetText(sText);
}
