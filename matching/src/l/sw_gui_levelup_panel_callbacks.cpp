// Callback leaves for the inferred CSWGuiLevelUpPanel (vtable 0x00759568).
// The parent at +0x68 and current step at +0x1CCC are documented in the
// original GUI reconstruction.
#include "l/sw_gui_dialog.h"
#include "l/sw_gui_options_in_game.h"

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
    char pad_0x6c[0x18d8];
    CSWGuiButton m_btnBack;                        // 0x1944
    CSWGuiButton m_btnCancel;                      // 0x1B08
    int m_nState;                                  // 0x1CCC
    int m_nLastState;                              // 0x1CD0
    int m_aStepEnabled[5];                         // 0x1CD4
    volatile int m_bCanCancel;                     // 0x1CE8

    void NewAbilityScores(unsigned int strength, unsigned int charisma,
                          unsigned int intelligence, unsigned int wisdom,
                          unsigned int constitution, unsigned int dexterity,
                          unsigned int points);
    void ChangeState(int bBack);
    void CancelledByChild();
    void AbilitySelectionCompleted();
    void PowersSelectionCompleted();
    void SetCanCancel(int bCanCancel);
    virtual void OnPanelAdded();
    virtual void HandleInputEvent(int nEvent, int bDown);
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

// FUNCTION: SWKOTOR 0x006EE640
void CSWGuiLevelUpPanel::SetCanCancel(int bCanCancel)
{
    int bAtLastState = m_nState == m_nLastState;
    m_bCanCancel = bCanCancel;
    int bEnableBack = 1;
    if (bAtLastState && bCanCancel == 0) {
        bEnableBack = 0;
    }
    m_btnBack.SetEnabled(bEnableBack);
    if (bEnableBack) {
        m_btnBack.m_cTextParams.SetColor(*(Vector*)0x007A23B4);
    } else {
        m_btnBack.m_cTextParams.SetColor(*(Vector*)0x007A2408);
    }
    unsigned long nCancelFlags = m_btnCancel.m_nFlags;
    m_btnCancel.m_nFlags = nCancelFlags ^ (((m_bCanCancel << 1) ^ nCancelFlags) & 2);
}

// FUNCTION: SWKOTOR 0x006EE6C0
void CSWGuiLevelUpPanel::OnPanelAdded()
{
    m_nState = -1;
    ChangeState(0);
    m_nLastState = m_nState;
    if (m_bCanCancel == 0) {
        m_btnBack.SetEnabled(0);
        m_btnBack.m_cTextParams.SetColor(*(Vector*)0x007A2408);
        m_btnCancel.m_nFlags &= ~2ul;
    }
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006EE720
void CSWGuiLevelUpPanel::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        int nEventOffset = nEvent - 0x28;
        if (nEventOffset == 0) {
            m_pManager->PlayGuiSound(0);
            ChangeState(1);
            CSWGuiPanel::HandleInputEvent(nEvent, bDown);
            return;
        } else {
            nEventOffset -= 5;
            if (nEventOffset == 0) {
                m_pManager->PlayGuiSound(0);
            } else {
                --nEventOffset;
                if (nEventOffset == 0) {
                    m_pManager->PlayGuiSound(0);
                    ChangeState(1);
                    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
                    return;
                }
            }
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}
