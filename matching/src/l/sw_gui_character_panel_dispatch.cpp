// Character-panel slot dispatch and its compiler-generated deleting wrapper.
#include "l/sw_gui_dialog.h"

class CSWGuiInGameCharacter : public CSWGuiPanel {
public:
    CSWGuiInGameCharacter();
    __declspec(noinline) virtual ~CSWGuiInGameCharacter();
    virtual void Draw(float fDelta);
    virtual void OnPanelAdded();
    virtual void OnPanelRemoved();
    void SetStats();                                    // 0x006AFDA0 (Mac name)
    void UpdatePortraits();                             // 0x006AEE30 (reference name)

    char pad_0x64[0x4b84 - 0x64];
    unsigned long m_n4B84;
    char pad_0x4b88[0x4d48 - 0x4b88];
    unsigned long m_n4D48;
    char pad_0x4d4c[0x59e8 - 0x4d4c];
    unsigned long m_n59E8;
    char pad_0x59ec[4];
    unsigned char m_n59F0;
    char pad_0x59f1[7];
};

// FUNCTION: SWKOTOR 0x006AE980
void CSWGuiInGameCharacter::OnPanelRemoved()
{
    m_n59E8 = 0;
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006B0B90
void CSWGuiInGameCharacter::Draw(float fDelta)
{
    SetStats();
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006B2220
void CSWGuiInGameCharacter::OnPanelAdded()
{
    m_n59F0 = 0xff;
    m_n4B84 = 0xfffffffe;
    m_n4D48 = 0xfffffffe;
    SetStats();
    UpdatePortraits();
    CSWGuiPanel::OnPanelAdded();
}

// This unannotated constructor emits the class vtable and its deleting
// wrapper.  The regular destructor at 0x006AF970 remains an external
// declaration; its body is not reconstructed by this translation unit.
// FUNCTION: SWKOTOR 0x006B2200 SYMBOL: ??_GCSWGuiInGameCharacter@@UAEPAXI@Z
CSWGuiInGameCharacter::CSWGuiInGameCharacter() {}
