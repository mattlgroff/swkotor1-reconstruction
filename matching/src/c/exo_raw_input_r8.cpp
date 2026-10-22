// CExoRawInputInternal (DirectInput 8 wrapper, b005e6 cluster), worker C r8.

#include <string.h>
#include "c/exo_input.h"

// FUNCTION: SWKOTOR 0x005E3010
void CExoRawInputInternal::FlushKeyboardData()
{
    if (m_bKeyboardMode || m_pKeyboard == 0 || m_bActive == 0) {
        return;
    }
    do {
        unsigned long dwItems = 0xffffffff;
        HRESULT hr = m_pKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 0, &dwItems, 0);
        if (hr == 0) {
            return;
        }
        if (hr != (HRESULT)0x8007000c) {
            if (hr == (HRESULT)0x8007001e) {
                m_pKeyboard->Acquire();
            }
            return;
        }
    } while (m_pKeyboard->Acquire() == (HRESULT)0x8007001e);
}

// FUNCTION: SWKOTOR 0x005E3080
void CExoRawInputInternal::FlushMouseData()
{
    if (m_pMouse == 0 || m_bActive == 0) {
        return;
    }
    do {
        unsigned long dwItems = 0xffffffff;
        HRESULT hr = m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 0, &dwItems, 0);
        if (hr == 0) {
            return;
        }
        if (hr != (HRESULT)0x8007000c) {
            if (hr == (HRESULT)0x8007001e) {
                m_pMouse->Acquire();
            }
            return;
        }
    } while (m_pMouse->Acquire() == (HRESULT)0x8007001e);
}

// FUNCTION: SWKOTOR 0x005E3930
int CExoRawInputInternal::GetJoystickAxis(int nDevice, unsigned long nControl)
{
    if ((unsigned int)nDevice >= 6) {
        return 0;
    }
    if (nDevice == g_nInputDeviceKeyboard) {
        return 1;
    }
    if (nDevice == g_nInputDeviceMouse) {
        return 1;
    }
    if (nDevice < g_nInputDeviceJoystick) {
        return 0;
    }
    if (nControl >= 0x30 && nControl <= 0x4f) {
        return 0;
    }
    if (nControl == 0x20) {
        return 0;
    }
    if (nControl != 0 && nControl != 4 && nControl != 0x18 && nControl != 0x1c) {
        return 0;
    }
    CExoJoystickState* pState = &m_pJoystickPrevStates[nDevice - 2];
    switch (nControl) {
    case 0:
        return pState->lX;
    case 4:
        return pState->lY;
    case 0x18:
        return pState->rglSlider[0];
    case 0x1c:
        return pState->lRz;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E39F0
int CExoRawInputInternal::IsDigitalControl(int nDevice, unsigned long nControl)
{
    if (nDevice == g_nInputDeviceKeyboard) {
        return 1;
    }
    if (nDevice == g_nInputDeviceMouse) {
        if (nControl >= 0xc && nControl <= 0xf) {
            return 1;
        }
        return nControl <= 8;
    }
    if (nDevice < g_nInputDeviceJoystick) {
        return 0;
    }
    if ((nControl >= 0x30 && nControl <= 0x4f) || (nControl >= 0x384 && nControl <= 0x393) || nControl == 0x20) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E3A60
// NONMATCH (6 diff insns): block order only; the original lays the
// "return 0.0f" block right after the keyboard test.
float CExoRawInputInternal::GetAxisRange(int nDevice, unsigned long nControl)
{
    if (nDevice != g_nInputDeviceKeyboard && nDevice != g_nInputDeviceMouse
        && (nControl == 0 || nControl == 4 || nControl == 0x18 || nControl == 0x1c)) {
        return 32767.0f;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x005E3AA0
// NONMATCH (6 diff insns): same block order as GetAxisRange.
float CExoRawInputInternal::GetAxisDeadZone(int nDevice, unsigned long nControl)
{
    if (nDevice != g_nInputDeviceKeyboard && nDevice != g_nInputDeviceMouse
        && (nControl == 0 || nControl == 4 || nControl == 0x18 || nControl == 0x1c)) {
        return 8191.0f;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x005E3AE0
// NONMATCH (26 diff insns): the original keeps the first "return 0"
// before the esi/edi pushes and does not tail-merge its return-0 blocks.
int CExoRawInputInternal::GetKeyboardBuffer(CExoKeyboardBuffer* pBuffer)
{
    if (m_pKeyboard == 0 || m_bActive == 0) {
        return 0;
    }
    if (pBuffer->m_pData) {
        delete pBuffer->m_pData;
        pBuffer->m_pData = 0;
    }
    pBuffer->m_nCount = 0;
    if (m_bKeyboardMode) {
        return 1;
    }
    pBuffer->m_pData = new DIDEVICEOBJECTDATA[0x100];
    if (pBuffer->m_pData == 0) {
        return 0;
    }
    memset(pBuffer->m_pData, 0, 0x100);
    unsigned long dwItems = 0x100;
    HRESULT hr = m_pKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), pBuffer->m_pData, &dwItems, 0);
    if (hr == 0) {
        pBuffer->m_nCount = dwItems;
        return 1;
    }
    switch (hr) {
    case (HRESULT)0x8007001e:
        if (m_pKeyboard->Acquire() == 0) {
            return 1;
        }
        break;
    case (HRESULT)0x8007000c:
        m_pKeyboard->Acquire();
        return 1;
    case (HRESULT)0x80070015:
        return 0;
    case (HRESULT)0x80040207:
    case (HRESULT)0x80070057:
        break;
    case 1:
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E3BE0
void CExoRawInputInternal::KeyboardModeOn()
{
    m_bKeyboardMode = 1;
    if (m_pKeyboard) {
        m_pKeyboard->Unacquire();
    }
}

// FUNCTION: SWKOTOR 0x005E3C00
void CExoRawInputInternal::KeyboardModeOff()
{
    m_bKeyboardMode = 0;
    if (m_pKeyboard) {
        m_pKeyboard->Acquire();
    }
}

// FUNCTION: SWKOTOR 0x005E3C20
void CExoRawInputInternal::SetActive(int bActive)
{
    if (bActive == m_bActive) {
        return;
    }
    if (m_pKeyboard) {
        if (bActive == 1) {
            if (!m_bKeyboardMode) {
                m_pKeyboard->Acquire();
            }
        } else {
            m_pKeyboard->Unacquire();
        }
    }
    if (m_pMouse) {
        if (bActive == 1) {
            m_pMouse->Acquire();
        } else {
            m_pMouse->Unacquire();
        }
    }
    for (int i = 0; i < m_nJoysticks; i++) {
        if (m_ppJoysticks && m_ppJoysticks[i]) {
            if (bActive == 1) {
                m_ppJoysticks[i]->Acquire();
            } else {
                m_ppJoysticks[i]->Unacquire();
            }
        }
    }
    m_bActive = bActive;
    if (bActive) {
        FlushKeyboardData();
        FlushMouseData();
        for (int j = 0; j < m_nJoysticks; j++) {
            memset(&m_pJoystickStates[j], 0, 0x50);
            m_pJoystickStates[j].m_nTime =
                (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
        }
    }
}

// FUNCTION: SWKOTOR 0x005E3E80
// NONMATCH (1 insn): the original tests the SetProperty result with
// "cmp eax, ebx" (zero register) instead of "test eax, eax".
CExoRawInputInternal::CExoRawInputInternal(HINSTANCE hInstance, HWND hWnd)
{
    m_bInitialized = 0;
    m_pDirectInput = 0;
    m_pKeyboard = 0;
    m_pMouse = 0;
    m_bActive = 0;
    m_bKeyboardMode = 0;
    m_nUnknown0c = 1;
    m_nUnknown10 = 0;
    m_nUnknown14 = 0;
    if (DirectInput8Create(hInstance, 0x800, IID_IDirectInput8A, (void**)&m_pDirectInput, 0) != 0
        && DirectInput8Create(hInstance, 0x300, IID_IDirectInput8A, (void**)&m_pDirectInput, 0) != 0) {
        ShutDownDirectInput();
        return;
    }
    if (m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, 0) != 0) {
        ShutDownDirectInput();
        return;
    }
    if (m_pKeyboard->SetDataFormat(&c_dfDIKeyboard) != 0) {
        ShutDownDirectInput();
        return;
    }
    if (m_pKeyboard->SetCooperativeLevel(hWnd, 6) != 0) {
        ShutDownDirectInput();
        return;
    }
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = 0;
    dipdw.dwData = 0x100;
    if (m_pKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph) != 0) {
        ShutDownDirectInput();
        return;
    }
    m_ppJoysticks = 0;
    m_pJoystickStates = 0;
    m_pJoystickPrevStates = 0;
    m_nJoysticks = 0;
    m_bInitialized = 1;
    g_nRawInputUnknown = 0;
}

// FUNCTION: SWKOTOR 0x005E3FA0
int CExoRawInputInternal::InitializeDirectInputMouse()
{
    if (m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, 0) != 0) {
        ShutDownDirectInput();
        return 0;
    }
    if (m_pMouse->SetDataFormat(&c_dfDIMouse) != 0) {
        ShutDownDirectInput();
        return 0;
    }
    if (m_pMouse->SetCooperativeLevel(g_hWnd, 0xa) != 0) {
        ShutDownDirectInput();
        return 0;
    }
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = 0;
    dipdw.dwData = 0x100;
    if (m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph) != 0) {
        ShutDownDirectInput();
        return 0;
    }
    if (m_pMouse->Acquire() != 0) {
        ShutDownDirectInput();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005E3D10
CExoString CExoRawInputInternal::GetKeyName(unsigned long nKey)
{
    char sName[0x50];
    DIPROPSTRING dips;
    dips.diph.dwSize = sizeof(DIPROPSTRING);
    dips.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dips.diph.dwObj = nKey;
    dips.diph.dwHow = 1;
    if (m_pKeyboard->GetProperty(DIPROP_KEYNAME, &dips.diph) >= 0) {
        memset(sName, 0, sizeof(sName));
        g_pExoBase->WcsToMbs(sName, dips.wsz, 0x4f);
        return CExoString(sName);
    }
    return CExoString("");
}

// FUNCTION: SWKOTOR 0x005E3DC0
void CExoRawInputInternal::ShutDownDirectInput()
{
    if (m_pDirectInput == 0) {
        return;
    }
    if (m_pKeyboard) {
        m_pKeyboard->Unacquire();
        m_pKeyboard->Release();
        m_pKeyboard = 0;
    }
    if (m_pMouse) {
        m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = 0;
    }
    for (int i = 0; i < m_nJoysticks; i++) {
        if (m_ppJoysticks[i]) {
            m_ppJoysticks[i]->Unacquire();
            m_ppJoysticks[i]->Release();
            m_ppJoysticks[i] = 0;
        }
    }
    if (m_pJoystickStates) {
        delete[] m_pJoystickStates;
        m_pJoystickStates = 0;
    }
    if (m_pJoystickPrevStates) {
        delete[] m_pJoystickPrevStates;
    }
    if (m_ppJoysticks) {
        delete[] m_ppJoysticks;
        m_ppJoysticks = 0;
    }
    m_pDirectInput->Release();
    m_pDirectInput = 0;
}
