#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x006637C0
int CSWCMessage::HandleServerToPlayerGuiQuickbar(unsigned char minor)
{
    switch (minor) {
    case 1: return HandleServerToPlayerGuiQuickbar_SetButton(0, 1);
    case 2:
        minor = ReadBYTE(8);
        return HandleServerToPlayerGuiQuickbar_SetButton(minor, 0);
    default: return 0;
    }
}

// FUNCTION: SWKOTOR 0x006631E0
int CSWCMessage::HandleServerToPlayerGuiInfoPopupMessage(unsigned char minor)
{
    switch (minor) {
    case 2: return 1;
    case 3: return 0;
    case 4: return 0;
    case 5: return 0;
    case 6: return 0;
    default: return 0;
    }
}

// FUNCTION: SWKOTOR 0x0064DEA0
int CSWCMessage::HandleServerToPlayerGuiInventory(unsigned char minor)
{
    if (minor == 1) {
        ReadBYTE(8);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006514C0
int CSWCMessage::HandleServerToPlayerCombatRound(unsigned char minor)
{
    CExoString unused;
    if (MessageReadOverflow(1)) return 0;
    if (minor == 1) ReadBYTE(8);
    return 1;
}

// FUNCTION: SWKOTOR 0x00654230
int CSWCMessage::HandleServerToPlayerPopUpGUIPanel(unsigned char minor)
{
    switch (minor) {
    case 1: {
        CExoString text;
        if (MessageReadOverflow(1)) return 0;
        ReadBOOL();
        ReadBOOL();
        ReadDWORD(0x20);
        text = ReadCExoString(0x20);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
        break;
    }
    case 2:
        g_pAppManager->m_pClientExoApp->GetGuiInGame()->ClosePopUpPanel();
        return 1;
    case 3:
        ReadCExoString(0x20);
        return 1;
    }
    return 1;
}

// NONMATCH: EH state/stack slot difference around unused resource local; two shapes tried.
// FUNCTION: SWKOTOR 0x006529F0
int CSWCMessage::HandleServerToPlayerModule_Type13()
{
    CExoString first;
    CResRef resource;
    CExoString second;
    ReadBYTE(8);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    return 1;
}
