// CSWGuiCharButton (b0062 message leaves).

#include "c/exo_base.h"

class CSWGuiButton {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();

    char pad_0x4[0x1c4];
};

class CSWGuiCharButton : public CSWGuiButton {
public:
    CSWGuiCharButton();
    virtual ~CSWGuiCharButton();

    CExoString m_sName;                             // 0x1c8
};

// FUNCTION: SWKOTOR 0x00686AB0
CSWGuiCharButton::CSWGuiCharButton()
{
}
