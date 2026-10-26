// Panel removal uses the journal's viewed-entry cleanup.
class CSWGuiInGameJournal {
public:
    void MarkNewQuestsObserved();  // 0x00645610
    void OnPanelRemoved();
};

// FUNCTION: SWKOTOR 0x00646070
void CSWGuiInGameJournal::OnPanelRemoved()
{
    MarkNewQuestsObserved();
}
