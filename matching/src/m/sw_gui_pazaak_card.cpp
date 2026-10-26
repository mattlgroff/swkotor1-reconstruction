// Pazaak GUI controls: CSWGuiActivatedButton (vtable 0x00753120) and
// CSWGuiPazaakCard (vtable 0x007531C0), 0x0067CA50-0x0067EFC0.

#include "m/sw_gui_pazaak.h"

extern "C" double __cdecl sin(double x);
#pragma intrinsic(sin)

// FUNCTION: SWKOTOR 0x0067CA50
void CSWGuiActivatedButton::SetColor(const Vector& vColor)
{
    m_fTimer = 0.8f;
    m_vColor = vColor;
}

// FUNCTION: SWKOTOR 0x0067CB70
void CSWGuiActivatedButton::Draw(float fDelta)
{
    if (m_fTimer >= 0.0f) {
        Vector vColor = g_vPazaakButtonColor;
        m_fTimer -= fDelta;
        if (m_fTimer < 0.0f) {
            if (m_bSelectable) {
                vColor = g_vPazaakButtonHilight;
            }
        }
        else {
            float fPulse = ((float)sin(m_fTimer * 23.5619449f) + 1.0f) * 0.5f;
            vColor = m_vColor * fPulse + vColor * (1.0f - fPulse);
        }
        m_cText.m_cParams.SetColor(vColor);
    }
    CSWGuiButton::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x0067CC90
void CSWGuiActivatedButton::SetEnabled(int b)
{
    CSWGuiButton::SetEnabled(b);
    if (m_fTimer < 0.0f) {
        m_cText.m_cParams.SetColor(m_bSelectable ? g_vPazaakButtonHilight : g_vPazaakButtonColor);
        SetActive(m_bSelectable && this == g_pGuiManager->m_pMouseOverControl ? 1 : 0);
    }
}

// FUNCTION: SWKOTOR 0x0067CA80
void CSWGuiPazaakCard::Draw(float fDelta)
{
    if (m_bFlag0) {
        CSWGuiBorder* pBorder = &m_cBorder;
        pBorder->Draw(fDelta);
        CSWGuiBorder* pHilight = &m_cHilight;
        pHilight->Draw(fDelta);
        return;
    }
    CSWGuiButton::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x0067CAC0
void CSWGuiPazaakCard::HandleInputEvent(int nEvent, int bDown)
{
    if (nEvent == 0x27) {
        CSWGuiNavigable::HandleInputEvent(nEvent, bDown);
        return;
    }
    CSWGuiButton::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0067CD00
void CSWGuiPazaakCard::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    CSWGuiButton::Load(pGFF, pStruct);
    m_cHilight.m_cParams.SetFlags(1, 0, 1);
}

// FUNCTION: SWKOTOR 0x0067CD30
void CSWGuiPazaakCard::SetCard(CPazaakCard cCard)
{
    m_cCard = cCard;
    if (cCard.m_nValue >= 0 && cCard.m_nValue < 28) {
        if (cCard.m_nValue >= 12 && cCard.m_nValue < 18) {
            if (cCard.m_bFlag) {
                CExoString sText(g_szPazaakCardText[cCard.m_nValue - 6]);
                m_lblFace.m_cText.m_cParams.SetText(sText);
            }
            else {
                CExoString sText(g_szPazaakCardText[cCard.m_nValue - 12]);
                m_lblFace.m_cText.m_cParams.SetText(sText);
            }
        }
        else {
            CExoString sText(g_szPazaakCardText[cCard.m_nValue]);
            m_lblFace.m_cText.m_cParams.SetText(sText);
        }
    }
    else {
        CExoString sText("");
        m_lblFace.m_cText.m_cParams.SetText(sText);
    }

    CResRef cFill;
    switch (cCard.m_nValue) {
    case 0: case 1: case 2: case 3: case 4: case 5:
        cFill = "lbl_cardmpos";
        break;
    case 6: case 7: case 8: case 9: case 10: case 11:
        cFill = "lbl_cardmneg";
        break;
    case 12: case 13: case 14: case 15: case 16: case 17:
        if (cCard.m_bFlag) {
            cFill = "lbl_cardraref";
        }
        else {
            cFill = "lbl_cardrarem";
        }
        break;
    case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27:
        cFill = "lbl_cardstand";
        break;
    case -1:
        cFill = "";
        break;
    default:
        cFill = "lbl_cardback";
        break;
    }
    m_cBorder.m_cParams.SetFillImage(cFill, 0);
}

// FUNCTION: SWKOTOR 0x0067CF00
void CSWGuiPazaakCard::DrawDragDrop(int x, int y)
{
    if (g_pGuiManager->m_pCaptureControl == this && g_pGuiManager->m_nCaptureType == 1) {
        CSWGuiExtent cSaved = m_cBorder.m_extent;
        CSWGuiExtent cExtent = m_cBorder.m_extent;
        cExtent.left = x - cExtent.width / 2;
        cExtent.top = y - cExtent.height / 2;
        CSWGuiBorder* pBorder = &m_cBorder;
        pBorder->SetExtent(cExtent);
        pBorder->Draw(0.0f);
        CSWGuiExtent cLabelSaved = m_lblFace.m_extent;
        CSWGuiExtent cLabel = cLabelSaved;
        cLabel.left += cExtent.left - cSaved.left;
        cLabel.top += cExtent.top - cSaved.top;
        CSWGuiLabel* pLabel = &m_lblFace;
        pLabel->SetExtent(cLabel);
        pLabel->Draw(0.0f);
        pLabel->SetExtent(cLabelSaved);
        pBorder->SetExtent(cSaved);
        return;
    }
    g_pGuiManager->m_pDragControl = 0;
}

// FUNCTION: SWKOTOR 0x0067D040
void CSWGuiPazaakCard::HandleLMouseDown()
{
    m_nDownX = g_pGuiManager->m_nMouseX;
    m_nDownY = g_pGuiManager->m_nMouseY;
    CSWGuiControl::HandleLMouseDown();
}

extern "C" int __cdecl abs(int n);
#pragma intrinsic(abs)

// FUNCTION: SWKOTOR 0x0067D060
int CSWGuiPazaakCard::HandleMouseCapturedMovement(int x, int y)
{
    if (m_bDraggable && g_pGuiManager->m_nCaptureType == 1 && g_pGuiManager->m_pDragControl != this) {
        if (*m_cBorder.m_cParams.m_cFill.GetResRef()) {
            if (abs(x - m_nDownX) > 12 || abs(y - m_nDownY) > 12) {
                g_pGuiManager->m_pDragControl = this;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0067D0D0
void CSWGuiPazaakCard::HandleLMouseUp()
{
    if (g_pGuiManager->m_pDragControl == this) {
        g_pGuiManager->m_pDragControl = 0;
        HandleInputEvent(0x17feb, 1);
        g_pGuiManager->ReleaseCapture(this);
        OnCaptureLost();
        return;
    }
    CSWGuiControl::HandleLMouseUp();
}

// FUNCTION: SWKOTOR 0x0067DDD0
CSWGuiPazaakCard::CSWGuiPazaakCard()
{
    m_bDraggable = 1;
    m_bCanSelect = 1;
    CPazaakCard cEmpty;
    cEmpty.m_nValue = -1;
    cEmpty.m_bFlag = 0;
    m_cCard = cEmpty;
    m_lblFace.SetFlag5(1);
}

// FUNCTION: SWKOTOR 0x0067DE60
int CSWGuiPazaakCard::GetIsSelectable()
{
    return m_bSelectable & m_bCanSelect;
}

// FUNCTION: SWKOTOR 0x0067DE80
CSWGuiPazaakCard::~CSWGuiPazaakCard()
{
    if (g_pGuiManager->m_pDragControl == this) {
        g_pGuiManager->m_pDragControl = 0;
    }
}

// FUNCTION: SWKOTOR 0x0067EFC0 SYMBOL: ??_GCSWGuiPazaakCard@@UAEPAXI@Z
