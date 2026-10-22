// CGuiInGameDialogMessage (b0062 message leaves).

#include "c/exo_base.h"

class CGuiInGameDialogMessage {
public:
    CGuiInGameDialogMessage();

    CExoString m_sSpeaker;                          // 0x00
    CExoString m_sText;                             // 0x08
};

// FUNCTION: SWKOTOR 0x006285B0
CGuiInGameDialogMessage::CGuiInGameDialogMessage()
{
}
