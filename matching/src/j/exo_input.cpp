// CExoInput, the public facade that forwards every call to its
// CExoInputInternal (+0x04). 0x005DF3F0-0x005DF730.
#include "j/exo_input.h"

// FUNCTION: SWKOTOR 0x005DF3F0
CExoInput::CExoInput()
{
    m_pInternal = new CExoInputInternal(g_hInstance, g_hWnd);
}

// FUNCTION: SWKOTOR 0x005DF460
int CExoInput::CreateNewEvent(unsigned long a, int b, int c, unsigned long d, unsigned long e)
{
    return m_pInternal->CreateNewEvent(a, b, c, d, e);
}

// FUNCTION: SWKOTOR 0x005DF470
void CExoInput::AddEvent(unsigned long a, unsigned long b)
{
    m_pInternal->AddEvent(a, b);
}

// FUNCTION: SWKOTOR 0x005DF480
int CExoInput::ScaleEvent(unsigned long nEvent, float a, float b)
{
    return m_pInternal->ScaleEvent(nEvent, a, b);
}

// FUNCTION: SWKOTOR 0x005DF490
int CExoInput::SetEventRepeatable(unsigned long a, unsigned long b, unsigned long c)
{
    return m_pInternal->SetEventRepeatable(a, b, c);
}

// FUNCTION: SWKOTOR 0x005DF4A0
void CExoInput::ClearEventRepeat(unsigned long nEvent)
{
    m_pInternal->ClearEventRepeat(nEvent);
}

// FUNCTION: SWKOTOR 0x005DF4B0
void CExoInput::CoolDownEvent(unsigned long a, unsigned long b)
{
    m_pInternal->CoolDownEvent(a, b);
}

// FUNCTION: SWKOTOR 0x005DF4C0
CExoInputEvent* CExoInput::GetEvent(unsigned long nEvent, int nMode)
{
    return m_pInternal->GetEvent(nEvent, nMode);
}

// FUNCTION: SWKOTOR 0x005DF4D0
float CExoInput::PollInput(unsigned long nDevice, int nFlags)
{
    return m_pInternal->PollInput(nDevice, nFlags);
}

// FUNCTION: SWKOTOR 0x005DF4E0
void CExoInput::ClearEvents()
{
    m_pInternal->ClearEvents();
}

// FUNCTION: SWKOTOR 0x005DF4F0
void CExoInput::WriteEventToFile(unsigned long a, int b, CExoFile* pFile)
{
    m_pInternal->WriteEventToFile(a, b, pFile);
}

// FUNCTION: SWKOTOR 0x005DF510
int CExoInput::GetActive()
{
    return m_pInternal->GetActive();
}

// FUNCTION: SWKOTOR 0x005DF520
void CExoInput::KeyboardModeOn()
{
    m_pInternal->KeyboardModeOn();
}

// FUNCTION: SWKOTOR 0x005DF530
void CExoInput::KeyboardModeOff()
{
    m_pInternal->KeyboardModeOff();
}

// FUNCTION: SWKOTOR 0x005DF540
void CExoInput::SetActive(int bActive)
{
    m_pInternal->SetActive(bActive);
}

// FUNCTION: SWKOTOR 0x005DF550
int CExoInput::SetRumble(int a, float b, float c, unsigned long d)
{
    return m_pInternal->SetRumble(a, b, c, d);
}

// FUNCTION: SWKOTOR 0x005DF560
int CExoInput::GetMainDeviceLost()
{
    return m_pInternal->GetMainDeviceLost();
}

// FUNCTION: SWKOTOR 0x005DF570
void CExoInput::PauseRumble()
{
    m_pInternal->PauseRumble();
}

// FUNCTION: SWKOTOR 0x005DF580
void CExoInput::UnpauseRumble()
{
    m_pInternal->UnpauseRumble();
}

// FUNCTION: SWKOTOR 0x005DF590
void CExoInput::BufferEvent(CExoInputEvent* pEvent)
{
    m_pInternal->BufferEvent(pEvent);
}

// FUNCTION: SWKOTOR 0x005DF5A0
int CExoInput::SetMouseMode(unsigned char nMode)
{
    return m_pInternal->SetMouseMode(nMode);
}

// FUNCTION: SWKOTOR 0x005DF5B0
void CExoInput::RefreshMouseCursor(int b)
{
    m_pInternal->RefreshMouseCursor(b);
}

// FUNCTION: SWKOTOR 0x005DF5C0
int CExoInput::SetMouseCursor(int a, int b, int c)
{
    return m_pInternal->SetMouseCursor(a, b, c);
}

// FUNCTION: SWKOTOR 0x005DF5D0
void CExoInput::SetMousePos(int x, int y)
{
    m_pInternal->SetMousePos(x, y);
}

// FUNCTION: SWKOTOR 0x005DF5E0
void CExoInput::HideMouse()
{
    m_pInternal->HideMouse();
}

// FUNCTION: SWKOTOR 0x005DF5F0
void CExoInput::ShowMouse()
{
    m_pInternal->ShowMouse();
}

// FUNCTION: SWKOTOR 0x005DF600
void CExoInput::ClearAllInputEventDescriptions()
{
    m_pInternal->ClearAllInputEventDescriptions();
}

// FUNCTION: SWKOTOR 0x005DF610
void CExoInput::GetMouseDelta(float* pX, float* pY)
{
    m_pInternal->GetMouseDelta(pX, pY);
}

// FUNCTION: SWKOTOR 0x005DF620
void CExoInput::UpdateMouseDelta(float f)
{
    m_pInternal->UpdateMouseDelta(f);
}

// FUNCTION: SWKOTOR 0x005DF630
int CExoInput::GetLastCapturedKeyboardKey()
{
    return m_pInternal->GetLastCapturedKeyboardKey();
}

// FUNCTION: SWKOTOR 0x005DF640
CExoString CExoInput::GetDisplayCharacterForKey(int nKey)
{
    return m_pInternal->GetDisplayCharacterForKey(nKey);
}

// FUNCTION: SWKOTOR 0x005DF670 SYMBOL: ??_GCExoInput@@UAEPAXI@Z
CExoInput::~CExoInput()
{
    if (m_pInternal) {
        delete m_pInternal;
        m_pInternal = 0;
    }
}

// FUNCTION: SWKOTOR 0x005DF6B0
CExoInputEventList CExoInput::GetEvents(int a, int b)
{
    return m_pInternal->GetEvents(a, b);
}

// FUNCTION: SWKOTOR 0x005DF6E0
unsigned char CExoInput::GetMouseMode()
{
    return m_pInternal->m_nMouseMode;
}

// FUNCTION: SWKOTOR 0x005DF6F0
int CExoInput::HasEventDesc(int nIndex)
{
    if (m_pInternal->m_nEventDescs > nIndex && m_pInternal->m_pEventDescs[nIndex])
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005DF720
void CExoInput::SetCaptureKeyboardKeys(int b)
{
    m_pInternal->m_bCaptureKeyboardKeys = b;
}

// FUNCTION: SWKOTOR 0x005DF730
int CExoInput::GetLastSetMouseDownState()
{
    return m_pInternal->m_bLastSetMouseDownState;
}
