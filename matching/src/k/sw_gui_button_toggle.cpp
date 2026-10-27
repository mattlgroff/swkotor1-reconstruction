// CSWGuiButtonToggle::SetSelected, emitted out of line in the main
// interface object (swguimaininterfacepc2.o).

class CSWGuiButtonToggle {
public:
    void SetSelected(int bSelected);

    char pad_0x0[0x1c8];
    unsigned long m_bSelected : 1;                  // 0x1c8 bit 0
};

// FUNCTION: SWKOTOR 0x00685540
void CSWGuiButtonToggle::SetSelected(int bSelected)
{
    m_bSelected = bSelected;
}
