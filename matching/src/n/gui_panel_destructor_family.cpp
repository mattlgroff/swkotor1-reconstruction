// Compiler-generated GUI lifetimes. Minimal declarations deliberately omit
// unrelated virtual overrides: the original destructors do not install vptrs.
class CSWGuiPanel { public: virtual ~CSWGuiPanel(); char pad_0x4[0x60]; };
class CSWGuiLabel { public: virtual ~CSWGuiLabel(); char pad_0x4[0x13c]; };
class CSWGuiLabelHilight { public: virtual ~CSWGuiLabelHilight(); char pad_0x4[0x1b0]; };
class CSWGuiButton { public: virtual ~CSWGuiButton(); char pad_0x4[0x1c0]; };
class CSWGuiListBox { public: virtual ~CSWGuiListBox(); char pad_0x4[0x2dc]; };
class CSWGuiMessageBox : public CSWGuiPanel {
public: CSWGuiMessageBox();
    char pad_0x64[0x10]; CSWGuiLabel m_message,m_icon; CSWGuiButton m_ok,m_cancel; CSWGuiListBox m_list;
};
CSWGuiMessageBox::CSWGuiMessageBox() {}
// FUNCTION: SWKOTOR 0x006270A0 SYMBOL: ??1CSWGuiMessageBox@@UAE@XZ
// Implicit CSWGuiMessageBox destructor (emitted with the vtable).
// FUNCTION: SWKOTOR 0x00627080 SYMBOL: ??_GCSWGuiMessageBox@@UAEPAXI@Z
class CSWGuiToolTipPanel : public CSWGuiPanel {
public: CSWGuiToolTipPanel();  CSWGuiLabel m_label;
};
CSWGuiToolTipPanel::CSWGuiToolTipPanel() {}
// FUNCTION: SWKOTOR 0x00627930 SYMBOL: ??1CSWGuiToolTipPanel@@UAE@XZ
// Implicit CSWGuiToolTipPanel destructor (emitted with the vtable).
// 0x00624920 is the fade's label-owning lifetime, independently confirmed by ctor 0x00624810.
class CSWGuiFade : public CSWGuiPanel {
public: CSWGuiFade();  char pad_0x64[0x10]; CSWGuiLabel m_label;
};
CSWGuiFade::CSWGuiFade() {}
// FUNCTION: SWKOTOR 0x00624920 SYMBOL: ??1CSWGuiFade@@UAE@XZ
// Implicit CSWGuiFade destructor (emitted with the vtable).
// FUNCTION: SWKOTOR 0x00624900 SYMBOL: ??_GCSWGuiFade@@UAEPAXI@Z
class CSWGuiStatusSummary : public CSWGuiPanel {
public: CSWGuiStatusSummary();
    char pad_0x64[0x1c]; CSWGuiLabel m_heading,m_total;
    CSWGuiLabel m_icons[9],m_descriptions[9]; CSWGuiButton m_ok;
};
CSWGuiStatusSummary::CSWGuiStatusSummary() {}
// FUNCTION: SWKOTOR 0x00628500 SYMBOL: ??1CSWGuiStatusSummary@@UAE@XZ
// Implicit CSWGuiStatusSummary destructor (emitted with the vtable).
// FUNCTION: SWKOTOR 0x006284E0 SYMBOL: ??_GCSWGuiStatusSummary@@UAEPAXI@Z
class CSWGuiInGameMenu : public CSWGuiPanel {
public: CSWGuiInGameMenu();
    CSWGuiLabelHilight m_equipment,m_inventory,m_character,m_map,m_abilities,m_journal,m_options,m_messages;
    CSWGuiButton m_equipmentButton,m_inventoryButton,m_characterButton,m_mapButton,m_abilitiesButton,m_journalButton,m_optionsButton,m_messagesButton;
};
CSWGuiInGameMenu::CSWGuiInGameMenu() {}
// FUNCTION: SWKOTOR 0x00628100 SYMBOL: ??1CSWGuiInGameMenu@@UAE@XZ
// Implicit CSWGuiInGameMenu destructor (emitted with the vtable).
// FUNCTION: SWKOTOR 0x006280E0 SYMBOL: ??_GCSWGuiInGameMenu@@UAEPAXI@Z
class CExoString { public: char* m_pString; unsigned long m_length; ~CExoString(); };
class CGuiInGameDialogMessage { public: CExoString m_sSpeaker,m_sText; ~CGuiInGameDialogMessage(); };
// FUNCTION: SWKOTOR 0x00628600
CGuiInGameDialogMessage::~CGuiInGameDialogMessage() {}
