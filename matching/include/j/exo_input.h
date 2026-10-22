// Worker J matching headers: CExoInput (the public facade over
// CExoInputInternal) and the input classes it forwards to.
#ifndef J_EXO_INPUT_H
#define J_EXO_INPUT_H

typedef void* HINSTANCE;
typedef void* HWND;

extern HINSTANCE g_hInstance;                       // 0x007A39C0
extern HWND g_hWnd;                                 // 0x007A39D8

#include "j/exo_string.h"

// Returned by value from GetEvents (has a destructor).
class CExoInputEventList {
public:
    CExoInputEventList(const CExoInputEventList& o);
    ~CExoInputEventList();
    void* element;
    int num;
    int array_size;
};

class CExoInputEvent;

// Event descriptor base (vftable 0x0074D5FC).
class CExoInputEventDescBase {
public:
    CExoInputEventDescBase() { m_n4 = 0; m_n8 = 0; m_nC = 0; }
    CExoInputEventDescBase(const CExoInputEventDescBase& o)
    {
        m_n4 = o.m_n4;
        m_n10 = o.m_n10;
        m_n14 = o.m_n14;
        m_n18 = o.m_n18;
        m_n1C = o.m_n1C;
        m_n8 = o.m_n8;
        m_nC = o.m_nC;
    }
    virtual int IsValid();

    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    unsigned long m_nC;                             // 0x0c
    unsigned long m_n10;                            // 0x10
    unsigned long m_n14;                            // 0x14
    unsigned long m_n18;                            // 0x18
    unsigned long m_n1C;                            // 0x1c
};

// Analog (axis) event descriptor, size 0x30 (vftable 0x0074D604).
class CExoInputAxisEventDesc : public CExoInputEventDescBase {
public:
    CExoInputAxisEventDesc();                                       // 0x005DFCC0
    CExoInputAxisEventDesc(const CExoInputEventDescBase& o);        // 0x005DFCF0
    virtual int IsValid();

    unsigned long m_n20;                            // 0x20
    unsigned long m_n24;                            // 0x24
    float m_fScaleX;                                // 0x28
    float m_fScaleY;                                // 0x2c
};

// Axis descriptor with an extra pair, size 0x38 (vftable 0x0074D60C).
class CExoInputAxisEventDescEx : public CExoInputAxisEventDesc {
public:
    CExoInputAxisEventDescEx();                                     // 0x005DFD50
    virtual int IsValid();

    unsigned long m_n30;                            // 0x30
    unsigned long m_n34;                            // 0x34
};
class CExoFile;

// Raw-input device at CExoInputInternal +0x140.
class CExoRawInputInternal {
public:
    char pad_0x0[0x4];
    int m_bActive;                                  // 0x04
    char pad_0x8[0x8];
    int m_bRumblePaused;                            // 0x10

    int GetActive() { return m_bActive; }
    void KeyboardModeOn();                          // 0x005E3BE0
    void KeyboardModeOff();                         // 0x005E3C00
    void SetActive(int bActive);                    // 0x005E3C20
    int GetMainDeviceLost();                        // 0x0063E7F0
};

class CExoInputInternal {
public:
    CExoInputInternal(HINSTANCE hInstance, HWND hWnd);   // 0x005E1880
    virtual ~CExoInputInternal();

    int CreateNewEvent(unsigned long a, int b, int c, unsigned long d, unsigned long e);
    void AddEvent(unsigned long a, unsigned long b);
    int ScaleEvent(unsigned long nEvent, float a, float b);
    int SetEventRepeatable(unsigned long a, unsigned long b, unsigned long c);
    void ClearEventRepeat(unsigned long nEvent);
    void CoolDownEvent(unsigned long a, unsigned long b);
    CExoInputEvent* GetEvent(unsigned long nEvent, int nMode);
    float PollInput(unsigned long nDevice, int nFlags);
    void ClearEvents();
    void WriteEventToFile(unsigned long a, int b, CExoFile* pFile);
    int GetActive();
    void KeyboardModeOn();
    void KeyboardModeOff();
    void SetActive(int bActive);
    int SetRumble(int a, float b, float c, unsigned long d);
    int GetMainDeviceLost();
    void PauseRumble();
    void UnpauseRumble();
    void BufferEvent(CExoInputEvent* pEvent);
    int SetMouseMode(unsigned char nMode);
    void RefreshMouseCursor(int b);
    int SetMouseCursor(int a, int b, int c);
    void SetMousePos(int x, int y);
    void HideMouse();
    void ShowMouse();
    void ClearAllInputEventDescriptions();
    void GetMouseDelta(float* pX, float* pY);
    void UpdateMouseDelta(float f);
    int GetLastCapturedKeyboardKey();
    CExoString GetDisplayCharacterForKey(int nKey);
    CExoInputEventList GetEvents(int a, int b);

    int m_bKeyboardMode;                            // 0x04
    char pad_0x8[0x120];
    void** m_pEventDescs;                           // 0x128
    int m_nEventDescs;                              // 0x12c
    char pad_0x130[0x10];
    CExoRawInputInternal* m_pRawInput;              // 0x140
    char pad_0x144[0x10];
    int m_bActive;                                  // 0x154
    char pad_0x158[0x220];
    unsigned char m_nMouseMode;                     // 0x378
    char pad_0x379[0x2f];
    int m_bCaptureKeyboardKeys;                     // 0x3a8
    int m_bLastSetMouseDownState;                   // 0x3ac
};

class CExoInput {
public:
    CExoInput();
    virtual ~CExoInput();

    int CreateNewEvent(unsigned long a, int b, int c, unsigned long d, unsigned long e);
    void AddEvent(unsigned long a, unsigned long b);
    int ScaleEvent(unsigned long nEvent, float a, float b);
    int SetEventRepeatable(unsigned long a, unsigned long b, unsigned long c);
    void ClearEventRepeat(unsigned long nEvent);
    void CoolDownEvent(unsigned long a, unsigned long b);
    CExoInputEvent* GetEvent(unsigned long nEvent, int nMode);
    float PollInput(unsigned long nDevice, int nFlags);
    void ClearEvents();
    void WriteEventToFile(unsigned long a, int b, CExoFile* pFile);
    int GetActive();
    void KeyboardModeOn();
    void KeyboardModeOff();
    void SetActive(int bActive);
    int SetRumble(int a, float b, float c, unsigned long d);
    int GetMainDeviceLost();
    void PauseRumble();
    void UnpauseRumble();
    void BufferEvent(CExoInputEvent* pEvent);
    int SetMouseMode(unsigned char nMode);
    void RefreshMouseCursor(int b);
    int SetMouseCursor(int a, int b, int c);
    void SetMousePos(int x, int y);
    void HideMouse();
    void ShowMouse();
    void ClearAllInputEventDescriptions();
    void GetMouseDelta(float* pX, float* pY);
    void UpdateMouseDelta(float f);
    int GetLastCapturedKeyboardKey();
    CExoString GetDisplayCharacterForKey(int nKey);
    CExoInputEventList GetEvents(int a, int b);
    unsigned char GetMouseMode();
    int HasEventDesc(int nIndex);
    void SetCaptureKeyboardKeys(int b);
    int GetLastSetMouseDownState();

    CExoInputInternal* m_pInternal;                 // 0x04
};

#endif
