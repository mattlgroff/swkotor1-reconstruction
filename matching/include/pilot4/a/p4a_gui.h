// Pilot 4 arm A: GUI panel subset.
#ifndef P4A_GUI_H
#define P4A_GUI_H

class CSWGuiManager {
public:
    void PlayGuiSound(char nSound);                     // 0x0040A140
};

class CGuiInGame {
public:
    void FlashStatus(int nStatus);                      // 0x0062B0B0
};

class CClientExoApp {
public:
    CGuiInGame* GetGuiInGame();                         // 0x005ED690
    void SetInputClass(int nClass, int bForce);         // 0x005EDA60
    int SetPausedByCombat(int bPaused, unsigned char nFlag, int bForce);   // 0x005EDC20
};

class CAppManager {
public:
    void* m_pUnknown00;                                 // 0x00
    CClientExoApp* m_pClientExoApp;                     // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                      // 0x007A39FC

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x00409E60

    void SetToDestroy(int bDestroy);                    // 0x00624A00

    char pad_0x4[0x18 - 0x4];
    CSWGuiManager* m_pManager;                          // 0x18
};

class CSWGuiStatusSummary : public CSWGuiPanel {
public:
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x00625AC0

    void UpdateStatusFlashes();                         // 0x00625B50

    char pad_0x1c[0x64 - 0x1c];
    unsigned long m_bFlash0 : 1;                        // 0x64 bit 0
    unsigned long m_bFlash1 : 1;                        // 0x64 bit 1
    unsigned long m_bFlash7 : 1;                        // 0x64 bit 2
    unsigned long m_bFlash8 : 1;                        // 0x64 bit 3
    unsigned long m_bFlash0b : 1;                       // 0x64 bit 4
    unsigned long m_bSound0C : 1;                       // 0x64 bit 5
    unsigned long m_bSound0D : 1;                       // 0x64 bit 6
    unsigned long m_bSound0E : 1;                       // 0x64 bit 7
    unsigned long m_bBit8 : 1;                          // 0x64 bit 8
    unsigned char m_nOld;                               // 0x68
    unsigned char m_nNew;                               // 0x69
    char pad_0x6a[2];
    int m_n6C;                                          // 0x6c
    int m_n70;                                          // 0x70
    int m_n74;                                          // 0x74
    char pad_0x78[0x7c - 0x78];
    unsigned char m_nFlags;                             // 0x7c
};

#endif
