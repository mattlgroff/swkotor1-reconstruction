// exo_input.h -- CExoInput
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/exo_input.h); the other copies' members fall on its members.
#ifndef SHARED_EXO_INPUT_H
#define SHARED_EXO_INPUT_H

#include "shared/exo_string.h"

class CExoFile;
struct CExoInputEvent;
class CExoInputEventList;
class CExoInputInternal;

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

    // declared by other copies
    void UpdateJoysticks();
    int GetKeyboardMode();
    void StartTextInput();
    void StopTextInput();
    void SetMousePosition(int x, int y);
};

#endif
