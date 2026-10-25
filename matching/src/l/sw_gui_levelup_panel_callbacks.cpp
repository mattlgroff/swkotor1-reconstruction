// Callback leaves for the inferred CSWGuiLevelUpPanel (vtable 0x00759568).
// The parent at +0x68 and current step at +0x1CCC are documented in the
// original GUI reconstruction.
#include "l/sw_gui_dialog.h"

class CSWGuiLevelUpCharGen : public CSWGuiPanel {
public:
    void NewAbilityScores(unsigned int strength, unsigned int charisma,
                          unsigned int intelligence, unsigned int wisdom,
                          unsigned int constitution, unsigned int dexterity,
                          unsigned int points);
    void AbilitiesChanged();
};

class CSWGuiLevelUpPanel : public CSWGuiPanel {
public:
    char pad_0x64[4];
    CSWGuiLevelUpCharGen* m_pParent;               // 0x68
    char pad_0x6c[0x1c60];
    int m_nState;                                  // 0x1CCC

    void NewAbilityScores(unsigned int strength, unsigned int charisma,
                          unsigned int intelligence, unsigned int wisdom,
                          unsigned int constitution, unsigned int dexterity,
                          unsigned int points);
    void ChangeState(int bBack);
    void CancelledByChild();
    void AbilitySelectionCompleted();
    void PowersSelectionCompleted();
};

// FUNCTION: SWKOTOR 0x006EE0D0
void CSWGuiLevelUpPanel::NewAbilityScores(unsigned int strength, unsigned int charisma,
                                          unsigned int intelligence, unsigned int wisdom,
                                          unsigned int constitution, unsigned int dexterity,
                                          unsigned int points)
{
    m_pParent->NewAbilityScores(strength, charisma, intelligence, wisdom,
                                constitution, dexterity, points);
}

// FUNCTION: SWKOTOR 0x006EE580
void CSWGuiLevelUpPanel::CancelledByChild()
{
    --m_nState;
    ChangeState(0);
    m_pParent->SetVisible(1);
}

// FUNCTION: SWKOTOR 0x006EE5B0
void CSWGuiLevelUpPanel::AbilitySelectionCompleted()
{
    m_pParent->SetVisible(1);
    m_pParent->AbilitiesChanged();
    ChangeState(0);
}

// FUNCTION: SWKOTOR 0x006EE5D0
void CSWGuiLevelUpPanel::PowersSelectionCompleted()
{
    m_pParent->SetVisible(1);
    ChangeState(0);
}
