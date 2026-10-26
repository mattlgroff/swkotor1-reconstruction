// Worker L matching headers: the bark bubble panel (CSWGuiBarkBubble,
// vtable 0x00755C60, 0x006A9770-0x006AA020) and the tutorial message box
// (CSWGuiTutorialBox, vtable 0x00755CD0, 0x006AA040-0x006AA8A0).
#ifndef L_SW_GUI_BARK_BUBBLE_H
#define L_SW_GUI_BARK_BUBBLE_H

#include "l/sw_gui_dialog.h"

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef();                                      // 0x00405ED0
    CResRef(const CResRef& cResRef);
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    CResRef operator=(const CExoString& sResRef);   // 0x004061F0
    CResRef operator=(const char* sResRef);         // 0x00406290
    char m_resRef[16];
};

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200

    int IsPlaying();                                // 0x005D5AF0
    int GetPlayFailed();                            // 0x005D5B00
    int Play(unsigned long a, unsigned long b, unsigned long c,
             unsigned long d, unsigned long e);     // 0x005D5B10
    void Pause();                                   // 0x005D5B20
    void Resume();                                  // 0x005D5B30
    void SetVolume(unsigned char nVolume);          // 0x005D5B50
    void Stop();                                    // 0x005D5B80
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5BA0
    void SetReverbEnabled(int bEnabled);            // 0x005D5BB0
    void SetProperty0x58(unsigned long n);          // 0x005D6290
    void SetProperty0x5C(const Vector& v);          // 0x005D62C0

    void* m_pInternal;
};

struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender);  // 0x0041E550
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
    void ParseTokens(CExoString* pString);          // 0x0041DD30
    unsigned char GetGender();                      // 0x00674B40

    char pad_0x0[0x3c];
    unsigned char m_nGender;                        // 0x3c
    char pad_0x3d[0x1b];
    void* m_pTokenObject;                           // 0x58
    unsigned long m_bTokenFlag : 1;                 // 0x5c bit 0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern CTlkTable* g_pTlkTableAlt;                   // 0x007A3A0C
extern unsigned char g_nGender;                     // 0x0073ECD0
extern unsigned char g_nBarkVolume;                 // 0x0074C5E8

class CWorldTimer {
public:
    unsigned __int64 GetSnapshotTime();             // 0x004ADC70
};

class CClientExoAppInternal {
public:
    char pad_0x0[0x2c];
    CWorldTimer* m_pWorldTimer;                     // 0x2c
};

inline CWorldTimer* CClientExoApp::GetWorldTimer() { return m_pInternal->m_pWorldTimer; }

class CSWCObject {
public:
    char pad_0x0[0x4];
    unsigned long m_oid;                            // 0x04
    char pad_0x8[0x1c];
    Vector m_vPosition;                             // 0x24
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
};

class CSWCCreature;
class CSWCDoor;
class CSWCPlaceable;

class CSWParty {
public:
    CSWCObject* GetMember(int n);                   // 0x006346C0
    CSWCCreature* GetPlayerCharacter();             // 0x00635460
};

class CSWSClient {
public:
    char pad_0x0[0x38];
    unsigned long m_oidServer;                      // 0x38
};

Vector operator-(const Vector& a, const Vector& b);  // 0x00423BC0

// vtable 0x00755C60; deleting dtor 0x006AA020.
class CSWGuiBarkBubble : public CSWGuiPanel {
public:
    CSWGuiBarkBubble(CSWGuiManager* pManager);      // 0x006A9770
    ~CSWGuiBarkBubble();                            // 0x006A9890
    void SetExtent(const CSWGuiExtent& extent);     // 0x04 (0x006A9FD0)
    void Draw(float fDelta);                        // 0x34 (0x006A9CE0)
    void OnPanelAdded();                            // 0x48 (0x006A9F90)

    void SetBark(unsigned long oidSpeaker, const CExoString& sText,
                 unsigned long nSound, int bIgnoreRange);   // 0x006A9920
    void Pause();                                   // 0x006A9BB0
    void Resume();                                  // 0x006A9C10
    void Hide(int bStopSound);                      // 0x006A9C60

    void StopSound()
    {
        if (m_pSound) {
            m_pSound->Stop();
            delete m_pSound;
            m_pSound = 0;
        }
    }
    void HidePanel()
    {
        m_nFlags &= ~0x100;
        unsigned long nFlags = m_nFlags;
        if ((nFlags & 0x600) != 0x400) {
            m_nFlags = (nFlags & ~0x400) | 0x200;
        }
    }

    CSWGuiLabel m_lblText;                          // 0x64
    CSWGuiZeroExtent m_baseExtent;                  // 0x1a4
    float m_fTime;                                  // 0x1b4
    CExoStreamingSoundSource* m_pSound;             // 0x1b8
    int m_nStartTime;                               // 0x1bc
    unsigned long m_oidSpeaker;                     // 0x1c0
    int m_bPlaying;                                 // 0x1c4
    unsigned long m_bIgnoreRange : 1;               // 0x1c8 bit 0
};

#endif
