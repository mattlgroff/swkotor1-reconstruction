// CExoInputInternal::BufferEvent(MSG*), worker C s9 (exoinputinternal.o).

#include "c/exo_input.h"

struct tagMSG {
    void* hwnd;                                     // 0x00
    unsigned int message;                           // 0x04
    unsigned int wParam;                            // 0x08
    long lParam;                                    // 0x0c
};

#define LOWORD(l) ((unsigned short)((unsigned long)(l) & 0xffff))
#define HIWORD(l) ((unsigned short)((unsigned long)(l) >> 16))
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#define GET_WHEEL_DELTA_WPARAM(wp) ((short)HIWORD(wp))

#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_RBUTTONDBLCLK 0x0206
#define WM_MBUTTONDOWN 0x0207
#define WM_MBUTTONUP 0x0208
#define WM_MOUSEWHEEL 0x020A

extern const unsigned long g_nInputMouseLeft;       // 0x0074D3E0
extern const unsigned long g_nInputMouseRight;      // 0x0074D3E4
extern const unsigned long g_nInputMouseMiddle;     // 0x0074D3E8
extern const unsigned long g_nInputMouseWheel;      // 0x0074D3F4

// Buffers the cursor position of a button message unless a SetMousePos
// warp is pending (descriptive helpers; expanded inline in each case).
static __forceinline void BufferMousePosition(CExoInputInternal* pInput, tagMSG* pMsg)
{
    if (!pInput->m_bMousePosSet) {
        int x = GET_X_LPARAM(pMsg->lParam);
        int y = GET_Y_LPARAM(pMsg->lParam);
        pInput->BufferEvent(g_nInputDeviceMouse, g_nInputMouseAxisX, x);
        pInput->BufferEvent(g_nInputDeviceMouse, g_nInputMouseAxisY, y);
        pInput->m_nMouseX = -1;
        pInput->m_nMouseY = -1;
    } else {
        pInput->m_bMousePosSet = 0;
    }
}

static __forceinline void BufferMousePositionUnsigned(CExoInputInternal* pInput, tagMSG* pMsg)
{
    if (!pInput->m_bMousePosSet) {
        int x = LOWORD(pMsg->lParam);
        int y = HIWORD(pMsg->lParam);
        pInput->BufferEvent(g_nInputDeviceMouse, g_nInputMouseAxisX, x);
        pInput->BufferEvent(g_nInputDeviceMouse, g_nInputMouseAxisY, y);
        pInput->m_nMouseX = -1;
        pInput->m_nMouseY = -1;
    } else {
        pInput->m_bMousePosSet = 0;
    }
}

// Returns 1 for a handled mouse message, 0 otherwise (symbols.csv has a
// void return from a call site; the original sets eax on both paths).
// NONMATCH (~88 insns): every case body matches, but the shared tail call
// gets the control id in eax and the value in ebx (the original uses edx and
// ecx, and hoists the -1 of the double-click cases into ebx before the test).
// FUNCTION: SWKOTOR 0x005E12A0
int CExoInputInternal::BufferEvent(tagMSG* pMsg)
{
    int nValue;
    unsigned long nControl;
    switch (pMsg->message) {
    case WM_MOUSEMOVE:
        if (m_bMousePosSet) {
            m_bMousePosSet = 0;
            return 1;
        }
        m_nMouseX = GET_X_LPARAM(pMsg->lParam);
        m_nMouseY = GET_Y_LPARAM(pMsg->lParam);
        return 1;
    case WM_LBUTTONDOWN:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseLeft;
        nValue = 1;
        break;
    case WM_LBUTTONDBLCLK:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseLeft;
        nValue = -1;
        break;
    case WM_LBUTTONUP:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseLeft;
        nValue = 0;
        break;
    case WM_RBUTTONDOWN:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseRight;
        nValue = 1;
        break;
    case WM_RBUTTONDBLCLK:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseRight;
        nValue = -1;
        break;
    case WM_RBUTTONUP:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseRight;
        nValue = 0;
        break;
    case WM_MOUSEWHEEL:
        BufferMousePosition(this, pMsg);
        nControl = g_nInputMouseWheel;
        nValue = GET_WHEEL_DELTA_WPARAM(pMsg->wParam);
        break;
    case WM_MBUTTONDOWN:
        BufferMousePositionUnsigned(this, pMsg);
        nControl = g_nInputMouseMiddle;
        nValue = 1;
        break;
    case WM_MBUTTONUP:
        BufferMousePositionUnsigned(this, pMsg);
        nControl = g_nInputMouseMiddle;
        nValue = 0;
        break;
    default:
        return 0;
    }
    BufferEvent(g_nInputDeviceMouse, nControl, nValue);
    return 1;
}
