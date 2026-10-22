// CSWGuiMainInterface (b0060_client_dtors_forwarders, b0062 message leaves).

class CSWGuiControl;

extern int g_bUnknown8338F8;                        // 0x008338F8

class CSWGuiPanel {
public:
    CSWGuiControl* HitCheckMouse(int nX, int nY);   // 0x0040B690
};

class CSWGuiMainInterfaceBC {
public:
    CSWGuiControl* HitCheckMouse(int nX, int nY);   // 0x00685F90
    void DoAction(int nIndex, int b);               // 0x00689610
};

class CSWGuiMainInterface : public CSWGuiPanel {
public:
    CSWGuiControl* HitCheckMouse(int nX, int nY);
    void DoTargetAction(int nIndex, int nUnused);
    void RefreshTargetActions();                    // 0x006887D0

    char pad_0x0[0xbc];
    CSWGuiMainInterfaceBC m_cUnknownBC;             // 0xbc
};

// FUNCTION: SWKOTOR 0x00686B70
CSWGuiControl* CSWGuiMainInterface::HitCheckMouse(int nX, int nY)
{
    CSWGuiControl* pControl = m_cUnknownBC.HitCheckMouse(nX, nY);
    if (pControl) {
        return pControl;
    }
    return CSWGuiPanel::HitCheckMouse(nX, nY);
}

// FUNCTION: SWKOTOR 0x0068AD20
void CSWGuiMainInterface::DoTargetAction(int nIndex, int nUnused)
{
    if (nIndex >= 0 && nIndex < 3) {
        if (g_bUnknown8338F8) {
            RefreshTargetActions();
        }
        m_cUnknownBC.DoAction(nIndex, 1);
    }
}
