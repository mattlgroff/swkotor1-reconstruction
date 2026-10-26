// clientinput.o leaves (0x0061F760-0x0061FB50): mouse position setters,
// the action-to-cursor table, the door-open animation test and the input
// event list removal. Names without a project record are descriptive.

#include "c/exo_base.h"

extern int g_nScreenWidth;                          // 0x0078D1D4
extern int g_nScreenHeight;                         // 0x0078D1D8

// Unnamed 0x2C-byte record zeroed by its constructor (descriptive name).
struct CClientInputRecord {
    unsigned long m_n00;                            // 0x00
    unsigned char m_b04;                            // 0x04
    char pad_0x5[0x1];
    unsigned short m_w06;                           // 0x06
    unsigned long m_n08;                            // 0x08
    unsigned long m_n0c;                            // 0x0c
    unsigned long m_n10;                            // 0x10
    unsigned long m_n14;                            // 0x14
    unsigned long m_n18;                            // 0x18
    unsigned long m_n1c;                            // 0x1c
    unsigned long m_n20;                            // 0x20
    unsigned long m_n24;                            // 0x24
    unsigned long m_n28;                            // 0x28

    CClientInputRecord();
};

// FUNCTION: SWKOTOR 0x0061F760
CClientInputRecord::CClientInputRecord()
{
    m_n00 = 0;
    m_b04 = 0;
    m_w06 = 0;
    m_n08 = 0;
    m_n0c = 0;
    m_n10 = 0;
    m_n14 = 0;
    m_n18 = 0;
    m_n1c = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_n28 = 0;
}

class CSWCObject {
public:
    unsigned short GetCurrentAnimation();           // 0x0063C060
};

class CSWCDoor : public CSWCObject {
public:
    int GetIsOpen();
};

// FUNCTION: SWKOTOR 0x0061F790
int CSWCDoor::GetIsOpen()
{
    if (GetCurrentAnimation() == 10050 || GetCurrentAnimation() == 10051) {
        return 1;
    }
    return 0;
}

class CClientExoAppInternal {
public:
    char pad_0x0[0x3c8];
    int m_nMouseX;                                  // 0x3c8
    int m_nMouseY;                                  // 0x3cc

    int SetMouseX(int x);
    int SetMouseY(int y);
    int GetCursorForAction(unsigned long nAction, int nUnused);
};

// FUNCTION: SWKOTOR 0x0061F7E0
int CClientExoAppInternal::SetMouseX(int x)
{
    if (m_nMouseX == x) {
        return 0;
    }
    m_nMouseX = x;
    m_nMouseX = m_nMouseX < 0 ? 0 : m_nMouseX;
    m_nMouseX = m_nMouseX < g_nScreenWidth - 1 ? m_nMouseX : g_nScreenWidth - 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x0061F830
int CClientExoAppInternal::SetMouseY(int y)
{
    y = g_nScreenHeight - y - 1;
    if (m_nMouseY == y) {
        return 0;
    }
    m_nMouseY = y;
    m_nMouseY = m_nMouseY < 0 ? 0 : m_nMouseY;
    m_nMouseY = m_nMouseY < g_nScreenHeight - 1 ? m_nMouseY : g_nScreenHeight - 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x0061FAA0
int CClientExoAppInternal::GetCursorForAction(unsigned long nAction, int nUnused)
{
    switch ((int)(nAction & 0x0fffffff)) {
    case -1:
        return 5;
    case 0x3e9:
        return 0x19;
    case 0x3ea:
        return 0xb;
    case 0x3eb:
        return 0x33;
    case 0x3f2:
        return 0x17;
    case 0x3f5:
        return 7;
    case 0x3f4:
        return 0x21;
    case 0x3f7:
        return 0x19;
    case 0x402:
        return 0x25;
    }
    return 5;
}

struct CExoInputEvent {
    unsigned long m_n00;
    unsigned long m_n04;
    unsigned long m_n08;
    unsigned long m_n0c;
};

// FUNCTION: SWKOTOR 0x0061FB50 SYMBOL: ?DelIndex@?$CExoArrayList@UCExoInputEvent@@@@QAEXH@Z
template void CExoArrayList<CExoInputEvent>::DelIndex(int);
