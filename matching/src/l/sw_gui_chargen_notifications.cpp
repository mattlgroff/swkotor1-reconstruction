// Notifications from the character creation subpanels.
class CExoString {
public:
    char* m_sString;
    unsigned long m_nLength;
    CExoString(const CExoString& value); // 0x005E5B00
    ~CExoString();
};
class CSWGuiMainCharGen {
public:
    void PortraitChanged();  // 0x006EA9F0
    void NameChanged();      // 0x006EEF70
    void AbilitiesChanged(); // 0x006EAC30
    void ClearAbilities();   // 0x006EB010
    void SetVisiblePanel(int n); // 0x006EA760
    char pad_0x00[0x22d4];
    CExoString m_sName;
};
class CSWCLevelUpStats { public: char pad_0x00[0xac]; unsigned char m_nAbilityPoints; };
class CSWCCreature { public: char pad_0x00[0x2f8]; CSWCLevelUpStats* m_pStats; };
class CSWGuiCustomPanel {
public:
    char pad_0x00[0x64];
    CSWCCreature* m_pCreature;
    CSWGuiMainCharGen* m_pParent;
    void SetParentPanel(CSWGuiMainCharGen* parent);
    void ClearAbilities();
    void CancelPanel();
    void SetState(int state); // 0x006EEFD0
    void PortraitCreationCompleted();
    void NameCreationCompleted();
    void AbilityCreationCompleted();
    void SkillCreationCompleted();
    void FeatCreationCompleted();
};
class CSWGuiQuickPanel {
public:
    char pad_0x00[0x68];
    CSWGuiMainCharGen* m_pParent;
    void SetState(int state); // 0x006EFC10
    void PortraitCreationCompleted();
    void NameCreationCompleted();
    CExoString GetCurrentName();
};

// FUNCTION: SWKOTOR 0x006EF4C0
void CSWGuiCustomPanel::PortraitCreationCompleted() { m_pParent->PortraitChanged(); SetState(1); }
// FUNCTION: SWKOTOR 0x006EF4E0
void CSWGuiCustomPanel::NameCreationCompleted() { m_pParent->NameChanged(); SetState(5); }
// FUNCTION: SWKOTOR 0x006EF500
void CSWGuiCustomPanel::AbilityCreationCompleted() { m_pParent->AbilitiesChanged(); SetState(2); }
// FUNCTION: SWKOTOR 0x006EF520
void CSWGuiCustomPanel::SkillCreationCompleted() { SetState(3); }
// FUNCTION: SWKOTOR 0x006EF530
void CSWGuiCustomPanel::FeatCreationCompleted() { SetState(4); }
// FUNCTION: SWKOTOR 0x006EFF60
void CSWGuiQuickPanel::PortraitCreationCompleted() { m_pParent->PortraitChanged(); SetState(1); }
// FUNCTION: SWKOTOR 0x006EFF80
void CSWGuiQuickPanel::NameCreationCompleted() { m_pParent->NameChanged(); SetState(2); }

// FUNCTION: SWKOTOR 0x006EEF60
void CSWGuiCustomPanel::SetParentPanel(CSWGuiMainCharGen* parent) { m_pParent = parent; }

// FUNCTION: SWKOTOR 0x006EF540
void CSWGuiCustomPanel::ClearAbilities()
{
    if (m_pParent) m_pParent->ClearAbilities();
    m_pCreature->m_pStats->m_nAbilityPoints = 30;
}

// FUNCTION: SWKOTOR 0x006EF570
void CSWGuiCustomPanel::CancelPanel()
{
    if (m_pParent) {
        m_pParent->SetVisiblePanel(1);
        if (m_pParent) m_pParent->ClearAbilities();
        m_pCreature->m_pStats->m_nAbilityPoints = 30;
    }
    SetState(0);
}

// FUNCTION: SWKOTOR 0x006EFFA0
CExoString CSWGuiQuickPanel::GetCurrentName()
{
    return m_pParent->m_sName;
}
