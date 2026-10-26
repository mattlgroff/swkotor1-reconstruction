// Journal button forwards to the panel virtual action.
class CSWGuiControl;
class CSWGuiInGameJournal {
public:
    virtual ~CSWGuiInGameJournal();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07();
    virtual void Slot08();
    virtual void Slot09();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void Slot22();
    virtual void Slot23();
    virtual void HandleAction60(CSWGuiControl* control); // vtable +0x60
    void OnBlackButtonPressed(CSWGuiControl* control);
};

// NONMATCH (11): executable instructions match; original padding is int3, emitted padding is nop.
// FUNCTION: SWKOTOR 0x00644730
void CSWGuiInGameJournal::OnBlackButtonPressed(CSWGuiControl* control)
{
    HandleAction60(control);
}
