// Small CSWGuiMessageBox controls; field and callee offsets are confirmed by
// the original code, symbols.csv, and the neighbouring worker-L layout.
class CResRef {
public:
    int IsValid() const;                              // 0x00406030
};

class CSWGuiBorderParams {
public:
    char pad_0x0[0x40];
    void SetFillImage(const CResRef& cImage, int bForce); // 0x00414C00
};

class CSWGuiMessageBox {
public:
    char pad_0x0[0x60];
    unsigned char m_nCancelState;                     // +0x60
    char pad_0x61[3];
    union {
        unsigned long m_nFlags;                       // +0x64
        struct {
            unsigned long m_bAccepted : 1;
            unsigned long m_bAllowCancel : 1;
            unsigned long m_nOtherFlags : 30;
        } m_bits;
    };
    char pad_0x68[0x1BC];
    CSWGuiBorderParams m_cIconParams;                 // +0x224

    void UpdateControls();                            // 0x00625200
    void SetAllowCancel(int bCancel);
    // Mac has a weaker 1/2 SetIcon hint; the COFF symbol and header both say
    // SetImage(const CResRef&), which matches the original call sequence.
    void SetImage(const CResRef& cImage);
};

// FUNCTION: SWKOTOR 0x00627130
void CSWGuiMessageBox::SetAllowCancel(int bCancel)
{
    m_bits.m_bAllowCancel = bCancel;
    m_nCancelState = (unsigned char)((((bCancel == 0) - 1) & 0x0D) + 2);
    UpdateControls();
}

// FUNCTION: SWKOTOR 0x00627160
void CSWGuiMessageBox::SetImage(const CResRef& cImage)
{
    if (cImage.IsValid()) {
        m_nFlags |= 0x10;
        m_cIconParams.SetFillImage(cImage, 0);
        UpdateControls();
    }
}
