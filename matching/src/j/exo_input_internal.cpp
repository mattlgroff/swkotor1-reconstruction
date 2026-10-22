// CExoInputInternal leaves in 0x005DFxxx and the axis event
// descriptor constructors. Kept apart from exo_input.cpp so the CExoInput
// forwarders do not inline these.
#include "j/exo_input.h"

// FUNCTION: SWKOTOR 0x005DFCC0 SYMBOL: ??0CExoInputAxisEventDesc@@QAE@XZ
CExoInputAxisEventDesc::CExoInputAxisEventDesc()
{
    m_fScaleX = 1.0f;
    m_fScaleY = 1.0f;
    m_n20 = 0;
    m_n24 = 0;
}

// FUNCTION: SWKOTOR 0x005DFCF0 SYMBOL: ??0CExoInputAxisEventDesc@@QAE@ABVCExoInputEventDescBase@@@Z
CExoInputAxisEventDesc::CExoInputAxisEventDesc(const CExoInputEventDescBase& o)
    : CExoInputEventDescBase(o)
{
    m_fScaleX = 1.0f;
    m_fScaleY = 1.0f;
    m_n20 = 0;
    m_n24 = 0;
}

// FUNCTION: SWKOTOR 0x005DFD50
CExoInputAxisEventDescEx::CExoInputAxisEventDescEx()
{
    m_n34 = 0;
}

// FUNCTION: SWKOTOR 0x005DFF40
int CExoInputInternal::GetActive()
{
    if (m_pRawInput)
        return m_pRawInput->GetActive();
    return 0;
}

// FUNCTION: SWKOTOR 0x005DFFB0
void CExoInputInternal::SetActive(int bActive)
{
    m_bActive = bActive;
    if (m_pRawInput)
        m_pRawInput->SetActive(bActive);
}

// FUNCTION: SWKOTOR 0x005DFFD0
int CExoInputInternal::GetMainDeviceLost()
{
    return m_pRawInput->GetMainDeviceLost();
}

// FUNCTION: SWKOTOR 0x005DFFE0
void CExoInputInternal::PauseRumble()
{
    m_pRawInput->m_bRumblePaused = 1;
}

// FUNCTION: SWKOTOR 0x005DFFF0
void CExoInputInternal::UnpauseRumble()
{
    m_pRawInput->m_bRumblePaused = 0;
}
