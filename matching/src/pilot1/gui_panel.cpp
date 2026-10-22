// Pilot group 1: CSWGuiPanel.

class CSWGuiControl;

class CSWGuiNavigation {
public:
    int GetNavigation(int nDirection);
    void SetNavigation(int nDirection, CSWGuiControl* pControl);
};

class CSWGuiControl {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual CSWGuiNavigation* GetNavigation();    // 0x4c
};

template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CSWGuiPanel {
public:
    char pad_0x0[0x20];
    CExoArrayList<CSWGuiControl*> m_lstControls;  // 0x20
    void SetupNavigation();
};

// FUNCTION: SWKOTOR 0x0040A890
void CSWGuiPanel::SetupNavigation()
{
    int nControls = m_lstControls.num;
    for (int i = 0; i < nControls; i++) {
        CSWGuiControl* pControl = m_lstControls.element[i];
        if (pControl) {
            CSWGuiNavigation* pNav = pControl->GetNavigation();
            if (pNav) {
                for (int nDir = 0; nDir < 4; nDir++) {
                    int nIndex = pNav->GetNavigation(nDir);
                    if (nIndex >= 0 && nIndex < nControls) {
                        pNav->SetNavigation(nDir, m_lstControls.element[nIndex]);
                    }
                    else {
                        pNav->SetNavigation(nDir, 0);
                    }
                }
            }
        }
    }
}
