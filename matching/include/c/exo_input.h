// Worker C matching headers: CExoInputInternal and its input types.
#ifndef C_EXO_INPUT_H
#define C_EXO_INPUT_H

typedef void* HCURSOR;
typedef void* HINSTANCE;

extern "C" {
__declspec(dllimport) int __stdcall ShowCursor(int bShow);                   // IAT 0x0073D3D0
__declspec(dllimport) HCURSOR __stdcall LoadCursorA(HINSTANCE h, const char* name);  // IAT 0x0073D420
__declspec(dllimport) HCURSOR __stdcall SetCursor(HCURSOR h);                // IAT 0x0073D3D4
__declspec(dllimport) int __stdcall DestroyCursor(HCURSOR h);                // IAT 0x0073D3D8
}

struct tagPOINT { long x; long y; };
struct tagCURSORINFO {
    unsigned long cbSize;
    unsigned long flags;
    HCURSOR hCursor;
    tagPOINT ptScreenPos;
};
extern "C" __declspec(dllimport) int __stdcall GetCursorInfo(tagCURSORINFO* pci);

extern HINSTANCE g_hInstance;                       // 0x007A39C0

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    void* RemoveHead();                             // 0x005E9C80
    CExoLinkedListPosition AddHead(void* p);        // 0x005E9D70
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    CExoLinkedListPosition AddHead(T* p) { return m_pcExoLinkedListInternal->AddHead(p); }
    CExoLinkedListPosition AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    CExoArrayList(const CExoArrayList<T>& o);
    T& operator[](int i) { return element[i]; }
    void Allocate(int n);
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = t;
        num++;
    }
};

struct CExoInputEvent {
    unsigned long m_nEvent;                         // 0x00
    unsigned long m_nValue1;                        // 0x04
    unsigned long m_nValue2;                        // 0x08
    unsigned long m_nValue3;                        // 0x0c
};

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoFile {
public:
    int Write(const CExoString& s);                 // 0x005E6A70
};

class CExoInputEventDesc {
public:
    virtual int IsAxis();                           // vtable +0x00
    virtual int HasAlternate();                     // vtable +0x04
    unsigned long m_nValue;                         // 0x04
    unsigned long m_nRefCount;                      // 0x08
    unsigned long m_bCoolDown;                      // 0x0c
    unsigned long m_nId;                            // 0x10
    int m_nType;                                    // 0x14
    int m_nSlot;                                    // 0x18
    unsigned long m_nControl;                       // 0x1c
    int m_bScaled;                                  // 0x20
    unsigned long m_nValue24;                       // 0x24
    float m_fScale28;                               // 0x28
    float m_fScale2c;                               // 0x2c
    unsigned long m_nAltControl;                    // 0x30
    unsigned long m_nValue34;                       // 0x34
};

// Event description subclasses built by CreateNewEvent (folder j range).
class CExoInputEventDescType3 {
public:
    CExoInputEventDescType3() throw();              // 0x005DFCC0
    CExoInputEventDescType3(CExoInputEventDesc* pFrom) throw();   // 0x005DFCF0
    char pad_0x0[0x30];
};

class CExoInputEventDescType4 {
public:
    CExoInputEventDescType4() throw();              // 0x005DFD50
    char pad_0x0[0x38];
};

extern const int g_nInputAnySlot;                   // 0x0074D3C4 (-1)
extern const unsigned long g_nInputMouseAxisX;      // 0x0074D3EC (3)
extern const unsigned long g_nInputMouseAxisY;      // 0x0074D3F0 (4)
extern float g_fMouseDeltaScale;                    // 0x007A2278 (100.0f)
extern int g_nScreenHeight;                         // 0x0078D1D8
extern void* g_hWnd;                                // 0x007A39D8

extern "C" {
__declspec(dllimport) int __stdcall ClientToScreen(void* hWnd, struct tagPOINT* p);  // IAT 0x0073D3C4
__declspec(dllimport) int __stdcall SetCursorPos(int x, int y);                     // IAT 0x0073D3C8
}

struct CExoMouseState {
    long lX;                                        // 0x00
    long lY;                                        // 0x04
    long lZ;                                        // 0x08
    unsigned char rgbButtons[4];                    // 0x0c
};

// 0x10-byte entry of CExoInputInternal +0x144.
struct CExoInputActiveEvent {
    unsigned long m_n0;                             // 0x00
    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    CExoInputEventDesc* m_pDesc;                    // 0x0c
};

// Payload of a buffered event (vtable 0x0074D5FC).
class CExoInputBufferedPayload {
public:
    CExoInputBufferedPayload() { m_n4 = 0; m_n8 = 0; m_nC = 0; }
    virtual void Unknown00();

    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    unsigned long m_nC;                             // 0x0c
    unsigned long m_n10;                            // 0x10
    unsigned long m_n14;                            // 0x14
    unsigned long m_nValue1;                        // 0x18
    unsigned long m_nValue2;                        // 0x1c
};

struct CExoInputBufferedEvent {
    unsigned long m_nEvent;                         // 0x00
    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    CExoInputBufferedPayload* m_pPayload;           // 0x0c
};

// One per input class at CExoInputInternal +0x08 + class*0x30.
struct CExoInputClass {
    char pad_0x0[0x8];
    CExoLinkedList<unsigned long> m_lstAnySlot;     // 0x08
    CExoLinkedList<unsigned long> m_lstSlots[6];    // 0x0c
    CExoArrayList<unsigned long> m_aBits;           // 0x24
};

struct CExoInputRepeatEvent {
    unsigned long m_nEvent;                         // 0x00
    unsigned long m_nInitialDelay;                  // 0x04
    unsigned long m_nRepeatDelay;                   // 0x08
    unsigned long m_nUnknown0c;                     // 0x0c
    unsigned long m_nUnknown10;                     // 0x10
    unsigned long m_nUnknown14;                     // 0x14
    unsigned long m_nUnknown18;                     // 0x18
};

typedef void* HWND;
typedef long HRESULT;
struct _GUID;
struct DIDATAFORMAT;
struct DIPROPHEADER {
    unsigned long dwSize;                           // 0x00
    unsigned long dwHeaderSize;                     // 0x04
    unsigned long dwObj;                            // 0x08
    unsigned long dwHow;                            // 0x0c
};
struct DIPROPDWORD {
    DIPROPHEADER diph;                              // 0x00
    unsigned long dwData;                           // 0x10
};
struct DIDEVICEOBJECTDATA {
    unsigned long dwOfs;                            // 0x00
    unsigned long dwData;                           // 0x04
    unsigned long dwTimeStamp;                      // 0x08
    unsigned long dwSequence;                       // 0x0c
    unsigned long uAppData;                         // 0x10
};

class IDirectInputDevice8A {
public:
    virtual HRESULT __stdcall QueryInterface(const _GUID& riid, void** ppv);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual HRESULT __stdcall GetCapabilities(void* p);
    virtual HRESULT __stdcall EnumObjects(void* p, void* q, unsigned long f);
    virtual HRESULT __stdcall GetProperty(const _GUID& g, DIPROPHEADER* p);
    virtual HRESULT __stdcall SetProperty(const _GUID& g, const DIPROPHEADER* p);   // +0x18
    virtual HRESULT __stdcall Acquire();                                            // +0x1c
    virtual HRESULT __stdcall Unacquire();                                          // +0x20
    virtual HRESULT __stdcall GetDeviceState(unsigned long cb, void* p);            // +0x24
    virtual HRESULT __stdcall GetDeviceData(unsigned long cb, DIDEVICEOBJECTDATA* p,
                                            unsigned long* pdwInOut, unsigned long f);  // +0x28
    virtual HRESULT __stdcall SetDataFormat(const DIDATAFORMAT* p);                 // +0x2c
    virtual HRESULT __stdcall SetEventNotification(void* h);                        // +0x30
    virtual HRESULT __stdcall SetCooperativeLevel(HWND hWnd, unsigned long f);      // +0x34
};

class IDirectInput8A {
public:
    virtual HRESULT __stdcall QueryInterface(const _GUID& riid, void** ppv);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual HRESULT __stdcall CreateDevice(const _GUID& g, IDirectInputDevice8A** pp, void* pOuter);  // +0x0c
};

extern "C" HRESULT __stdcall DirectInput8Create(HINSTANCE h, unsigned long dwVersion,
                                                const _GUID& riid, void** ppv, void* pOuter);  // 0x005E50C0
extern const _GUID IID_IDirectInput8A;              // 0x0074D6F4
extern const _GUID GUID_SysKeyboard;                // 0x0074D694
extern const _GUID GUID_SysMouse;                   // 0x0074D6A4
extern const DIDATAFORMAT c_dfDIKeyboard;           // 0x0074D8FC
extern const DIDATAFORMAT c_dfDIMouse;              // 0x0074DB04
extern const int g_nInputDeviceKeyboard;            // 0x0074D3C8 (0)
extern const int g_nInputDeviceMouse;               // 0x0074D3CC (1)
extern const int g_nInputDeviceJoystick;            // 0x0074D3D0 (2)
extern int g_nRawInputUnknown;                      // 0x008328A0

#define DIPROP_BUFFERSIZE (*(const _GUID*)1)

// 0x74-byte joystick state at CExoRawInputInternal +0x2c / +0x30.
struct CExoJoystickState {
    long lX;                                        // 0x00
    long lY;                                        // 0x04
    long lZ;                                        // 0x08
    long lRx;                                       // 0x0c
    long lRy;                                       // 0x10
    long lRz;                                       // 0x14
    long rglSlider[2];                              // 0x18
    unsigned long rgdwPOV[4];                       // 0x20
    unsigned char rgbButtons[32];                   // 0x30
    unsigned long m_nTime;                          // 0x50
    char pad_0x54[0x20];
};

// Keyboard buffer filled by GetKeyboardBuffer.
struct CExoKeyboardBuffer {
    CExoKeyboardBuffer();                           // 0x005C5A30 (folded)
    ~CExoKeyboardBuffer();                          // 0x005E2FF0

    DIDEVICEOBJECTDATA* m_pData;                    // 0x00
    unsigned long m_nCount;                         // 0x04
    unsigned long m_nUnknown08;                     // 0x08
};

extern const int g_nFirstKeyIndex;                  // 0x0074D3D8
extern const int g_nLastKeyIndex;                   // 0x0074D3DC

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBaseTimers {
public:
    void* m_p0;                                     // 0x00
    CExoTimers* m_pcExoTimers;                      // 0x04

    void WcsToMbs(char* sDest, const unsigned short* sSource, int nMax);   // 0x005E6690
};

struct DIPROPSTRING {
    DIPROPHEADER diph;                              // 0x00
    unsigned short wsz[260];                        // 0x10
};

#define DIPROP_KEYNAME (*(const _GUID*)20)

extern CExoBaseTimers* g_pExoBase;                  // 0x007A39E0

class CExoRawInputInternal {
public:
    CExoRawInputInternal(HINSTANCE hInstance, HWND hWnd);
    ~CExoRawInputInternal();
    void FlushKeyboardData();                       // 0x005E3010
    void FlushMouseData();                          // 0x005E3080
    CExoString GetKeyName(unsigned long nKey);      // 0x005E3D10
    void ShutDownDirectInput();                     // 0x005E3DC0
    int GetJoystickAxis(int nDevice, unsigned long nControl);   // 0x005E3930
    int IsDigitalControl(int nDevice, unsigned long nControl);  // 0x005E39F0
    float GetAxisRange(int nDevice, unsigned long nControl);    // 0x005E3A60
    float GetAxisDeadZone(int nDevice, unsigned long nControl); // 0x005E3AA0
    int GetKeyboardBuffer(CExoKeyboardBuffer* pBuffer);         // 0x005E3AE0
    void KeyboardModeOn();                          // 0x005E3BE0
    void KeyboardModeOff();                         // 0x005E3C00
    void SetActive(int bActive);                    // 0x005E3C20
    int InitializeDirectInputMouse();               // 0x005E3FA0
    int GetMouseState(CExoMouseState* pState);      // 0x005E4050

    int m_bInitialized;                             // 0x00
    int m_bActive;                                  // 0x04
    int m_bKeyboardMode;                            // 0x08
    int m_nUnknown0c;                               // 0x0c
    int m_nUnknown10;                               // 0x10
    int m_nUnknown14;                               // 0x14
    int m_nJoysticks;                               // 0x18
    IDirectInput8A* m_pDirectInput;                 // 0x1c
    IDirectInputDevice8A* m_pKeyboard;              // 0x20
    IDirectInputDevice8A* m_pMouse;                 // 0x24
    IDirectInputDevice8A** m_ppJoysticks;           // 0x28
    CExoJoystickState* m_pJoystickStates;           // 0x2c
    CExoJoystickState* m_pJoystickPrevStates;       // 0x30
};

class CExoInputInternal {
public:
    CExoInputInternal();
    virtual ~CExoInputInternal();                   // 0x005E0B80

    int SetEventRepeatable(unsigned long nEvent, unsigned long nInitialDelay, unsigned long nRepeatDelay);
    int SetMouseMode(unsigned char nMode);
    CExoString GetDisplayCharacterForKey(unsigned long nKey);
    CExoInputEventDesc* IsControlUsed(unsigned long nControl, int nSlot, int nClass);
    void ClearEventRepeat(unsigned long nEvent);
    void CoolDownEvent(unsigned long nEvent, int bCoolDown);
    void BufferEvent(unsigned long nValue1, unsigned long nValue2, unsigned long nEvent);
    int BufferEvent(struct tagMSG* pMsg);           // 0x005E12A0 (window mouse message)
    int IsEventInClass(int nEvent, int nClass);
    CExoInputEventDesc* GetEvent(int nEvent, int nClass);
    void ClearAllInputEventDescriptions();
    void HideMouse();
    void ShowMouse();

    // r8 (matching/src/c/exo_input_internal_r8.cpp)
    int SetMouseCursor(int nCursor, int nOption, int bAlternate);
    void GetMouseDelta(float* pX, float* pY);
    void UpdateMouseDelta(float f);
    void ClearEvents();
    void RefreshMouseCursor(int b);
    void SetMousePos(int x, int y);
    int CreateNewEvent(unsigned long nEvent, int nType, int nSlot, unsigned long nControl, unsigned long nAltControl);
    int AddEvent(int nEvent, unsigned long nClass);
    int RemoveEvent(int nEvent);
    int ScaleEvent(int nEvent, float fScaleA, float fScaleB);
    void WriteEventToFile(int nValue, int nIndex, CExoFile* pFile);
    int GetLastCapturedKeyboardKey();
    float PollInput(int nEvent, int nClass);
    float GetControlValue(CExoInputEventDesc* pDesc, unsigned long nControl, unsigned long nValue);   // 0x005DFD80

    char pad_0x4[0x4];
    CExoInputClass m_aClasses[6];                   // 0x08
    CExoArrayList<CExoInputEventDesc*> m_aEventDescs;   // 0x128
    CExoLinkedList<CExoInputEventDesc*> m_lstCoolDown;  // 0x134
    CExoLinkedList<CExoInputRepeatEvent> m_lstRepeatEvents; // 0x138
    char pad_0x13c[0x4];
    CExoRawInputInternal* m_pRawInput;              // 0x140
    CExoArrayList<CExoInputActiveEvent> m_aActiveEvents;  // 0x144
    int m_nActiveEventLimit;                        // 0x150
    char pad_0x154[0x4];
    int m_nSlots;                                   // 0x158
    char pad_0x15c[0x8];
    unsigned long m_aKeyMap[0x84];                  // 0x164
    CExoLinkedList<CExoInputBufferedEvent> m_lstBufferedEvents;   // 0x374
    unsigned char m_nMouseMode;                     // 0x378
    char pad_0x379[0x37c - 0x379];
    int m_nMouseX;                                  // 0x37c (descriptive; -1 after a button event)
    int m_nMouseY;                                  // 0x380 (descriptive)
    int m_bMousePosSet;                             // 0x384
    char pad_0x388[0x10];
    int m_nCursor;                                  // 0x398
    int m_nCursorOption;                            // 0x39c
    float m_fMouseDeltaX;                           // 0x3a0
    float m_fMouseDeltaY;                           // 0x3a4
    int m_bCaptureKeys;                             // 0x3a8
    int m_nUnknown3ac;                              // 0x3ac
};

#endif
