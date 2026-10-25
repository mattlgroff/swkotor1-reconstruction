// CGuiInGame dialogue/camera helpers. The Mac symbol hints and playable
// reconstruction agree on names and the relevant pointer/flag offsets.
class CSWGuiDialogOutput {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void ShowDialogMessage();             // +0x80
};

class CGuiInGame {
public:
    char pad_0x0[0x3c];
    CSWGuiDialogOutput* m_pDialogOutput;            // +0x3c
    char pad_0x40[0x24];
    int m_bDialogAnimationActive;                   // +0x64
    char pad_0x68[0x4c];
    int m_bDialogScrollFinished;                    // +0xB4
    char pad_0xB8[0xAFC];
    int m_bDialogScrollDone;                        // +0xBB4

    int IsLoopingDialogStuntAnimation(unsigned short nAnimation);
    void DialogGuiDoneScroll();
    int IsCameraDialog();
};

// FUNCTION: SWKOTOR 0x0062AB70
int CGuiInGame::IsLoopingDialogStuntAnimation(unsigned short nAnimation)
{
    return nAnimation >= 0x0578 && nAnimation <= 0x06BF;
}

// FUNCTION: SWKOTOR 0x0062AB90
void CGuiInGame::DialogGuiDoneScroll()
{
    if (m_bDialogAnimationActive == 0)
        return;
    m_bDialogScrollDone = 1;
    m_pDialogOutput->ShowDialogMessage();
}

// FUNCTION: SWKOTOR 0x0062ABD0
int CGuiInGame::IsCameraDialog()
{
    return m_pDialogOutput != 0 &&
        (char*)m_pDialogOutput == *(char**)((char*)this + 0x40);
}
