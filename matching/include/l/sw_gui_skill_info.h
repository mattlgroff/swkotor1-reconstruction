// Worker L matching headers: the skill info popup CSWGuiSkillInfo (layout
// "skillinfo", vtable 0x00757940, 0x006CD320-0x006CE9C0) and the examine
// message box CSWGuiExamineBox (vtable 0x007579C8, 0x006CE9C0-0x006CEA90).
#ifndef L_SW_GUI_SKILL_INFO_H
#define L_SW_GUI_SKILL_INFO_H

#include "l/sw_gui_in_game_abilities.h"

// Skill entry styling from the list box prototype (slot 0xa8, 0x006AC930).
class CSWGuiInGameSkillEntryV : public CSWGuiInGameSkillEntry {
public:
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void SetStyle(CSWGuiLabelText* pText, CSWGuiBorder* pBorder,
                          CSWGuiBorder* pHilight, int nWidth);  // 0xa8 (0x006AC930)
    void SetSkillI(int nId, int nType);             // 0x006ACB40 (SetSkill, int args)
};

// vtable 0x00757940; ctor 0x006CE7C0, dtor 0x006CD320, ??_G 0x006CDFA0.
class CSWGuiSkillInfo : public CSWGuiPanel {
public:
    CSWGuiSkillInfo(CSWGuiManager* pManager);       // 0x006CE7C0
    ~CSWGuiSkillInfo();                             // 0x006CD320
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006CD3C0)
    void SetSkills(const CExoArrayListL<int>& lstSkills, int nType);    // 0x006CDFC0

    void CenterPanel();                             // 0x0040A600

    CSWGuiListBox m_lbSkills;                       // 0x64
    CSWGuiLabel m_lblMessage;                       // 0x344
    CSWGuiButton m_btnOk;                           // 0x484
    CSWGuiInGameSkillEntry m_aEntries[10];          // 0x648
};

class CGuiInGameL : public CGuiInGame {
public:
    void HideExamineBox();                          // 0x0062D440
};

// vtable 0x007579C8 (??_G folded into 0x006AA830); 0x988 bytes.
class CSWGuiExamineBox : public CSWGuiMessageBox {
public:
    CSWGuiExamineBox(CSWGuiManager* pManager);      // 0x006CE9C0
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006CEA30)
    CSWGuiButton& GetOkButton() { return *(CSWGuiButton*)(pad_0x1b4 + 0x140); }   // 0x2f4

    unsigned long m_n984;                           // 0x984
};

#endif
