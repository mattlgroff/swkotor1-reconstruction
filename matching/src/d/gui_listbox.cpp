// CSWGuiListBox layout passes.
#include "d/gui_control.h"

void operator delete(void* p);
void* operator new(unsigned int n);



// FUNCTION: SWKOTOR 0x0041a2d0
void CSWGuiListBox::OrganizeOversized()
{
    int nPadding = m_n2c0;
    CSWGuiExtent cExtent;
    cExtent.left = nPadding;
    cExtent.width = m_cArea.width - (nPadding + nPadding);
    cExtent.height = m_nItemSize;
    int nPages;
    if (m_cArea.height) {
        nPages = (int)(m_nItemSize * m_fScale / m_cArea.height + 0.9999f) - (int)m_fScale + 1;
    } else {
        nPages = 1;
    }
    if (m_nPages == nPages) {
        cExtent.top = m_cArea.height - m_nItemSize - nPadding;
        m_cScrollBar.SetCurValue(m_nItemSize);
    } else {
        cExtent.top = nPadding - (int)((m_nPages - 1) * m_cArea.height / m_fScale);
        if (m_nPages == 1) {
            m_cScrollBar.SetCurValue(0);
        } else {
            m_cScrollBar.SetCurValue((int)((float)(m_nPages - 1) / (nPages - 1) * (m_nItemSize - m_cArea.height)));
        }
    }
    m_lstControls.element[0]->SetExtent(cExtent);
}

// FUNCTION: SWKOTOR 0x0041b140
void CSWGuiListBox::OrganizeControls()
{
    int nCount = m_lstControls.num;
    if (nCount == 0) {
        if (m_bDirty) {
            m_cScrollBar.SetMaxValue(1, 0);
            m_cScrollBar.SetCurValue(1);
            m_cScrollBar.SetVisibleValue(1);
            m_bDirty = 0;
        }
        return;
    }
    if (m_bUnequal) {
        OrganizeUnequal();
        return;
    }
    int nStep = m_n2c0 + m_nItemSize;
    int i;
    if (m_bDirty) {
        m_nItemSize = 0;
        for (i = 0; i < nCount; i++) {
            m_lstControls.element[i]->SetExtent(m_lstExtents.element[i]);
            int nHeight = m_lstControls.element[i]->GetHeight();
            if (nHeight > m_nItemSize) {
                m_nItemSize = nHeight;
            }
        }
        m_nFirstVisible = 0;
        if (m_nItemSize == 0) {
            m_cScrollBar.SetMaxValue(1, 0);
            m_cScrollBar.SetCurValue(1);
            m_cScrollBar.SetVisibleValue(1);
            return;
        }
        int nArea = m_cArea.height;
        nStep = m_n2c0 + m_nItemSize;
        m_nVisible = nArea / nStep;
        if (m_nVisible == 0) {
            m_cScrollBar.SetMaxValue(m_nItemSize - nArea + 1, m_bFlag9);
            m_cScrollBar.SetVisibleValue(m_cArea.height);
        } else if (m_bFlag9) {
            int nMax = nCount - m_nVisible + 1;
            if (nMax <= 1) {
                nMax = 1;
            }
            m_cScrollBar.SetMaxValue(nMax, m_bFlag9);
            m_cScrollBar.SetVisibleValue(1);
        } else {
            int nMax = nCount - m_nVisible + 1;
            if (nMax <= 1) {
                nMax = 1;
            }
            m_cScrollBar.SetMaxValue(nMax, m_bFlag9);
            int nVisible = m_nVisible;
            if (nVisible >= nCount) {
                nVisible = nCount;
            }
            m_cScrollBar.SetVisibleValue(nVisible);
        }
        if (m_n2c0 + m_nItemSize > m_cArea.height && m_lstControls.num > 0 && !m_bUnequal) {
            CSWGuiLabel* pLabel = m_lstControls.element[0]->GetLabel();
            if (pLabel) {
                int nFont = pLabel->m_cText.GetFontHeight();
                if (nFont) {
                    m_fScale = (float)m_cArea.height / nFont;
                }
            }
        }
        m_bDirty = 0;
    }
    if (m_n2c0 + m_nItemSize > m_cArea.height && m_lstControls.num > 0 && !m_bUnequal) {
        OrganizeOversized();
        return;
    }
    if (m_bKeepSelectionVisible && m_nSelected != -1) {
        if (m_nSelected < m_nFirstVisible || m_nSelected >= m_nVisible + m_nFirstVisible) {
            if (m_nSelected < m_nFirstVisible) {
                m_nFirstVisible = m_nSelected;
            } else {
                int nFirst = m_nSelected - m_nVisible + 1;
                m_nFirstVisible = nFirst < 0 ? 0 : nFirst;
            }
        }
    }
    m_cScrollBar.SetCurValue(m_nFirstVisible);
    int nVisible = m_nVisible;
    int nPadding = m_n2c0;
    int nRemainder = m_cArea.height - nVisible * nStep - nPadding;
    int nExtra = nRemainder / nVisible;
    CSWGuiExtent cExtent;
    cExtent.left = nPadding;
    cExtent.top = 0;
    cExtent.height = m_nItemSize;
    nRemainder -= nVisible * nExtra;
    cExtent.width = m_cArea.width - (nPadding + nPadding);
    int nY = nPadding - m_nFirstVisible * nStep;
    for (i = 0; i < m_nFirstVisible; i++) {
        cExtent.top = nY;
        m_lstControls.element[i]->SetExtent(cExtent);
        nY += nStep;
    }
    int nEnd = m_nFirstVisible + m_nVisible;
    if (nEnd >= nCount) {
        nEnd = nCount;
    }
    int nGrown = m_nItemSize + nExtra;
    for (i = m_nFirstVisible; i < nEnd; i++) {
        cExtent.height = nGrown;
        cExtent.top = nY;
        if (nRemainder) {
            cExtent.height = nGrown + 1;
            nY++;
            nRemainder--;
        }
        m_lstControls.element[i]->SetExtent(cExtent);
        nY += m_n2c0 + nGrown;
    }
    cExtent.height = m_nItemSize;
    for (i = m_nFirstVisible + m_nVisible; i < nCount; i++) {
        cExtent.top = nY;
        m_lstControls.element[i]->SetExtent(cExtent);
        nY += nStep;
    }
}

// FUNCTION: SWKOTOR 0x00419ee0
void CSWGuiListBox::OrganizeUnequal()
{
    int nCount = m_lstControls.num;
    CExoString sUnused;
    int i;
    if (m_bDirty) {
        if (nCount == 1) {
            m_nItemSize = m_lstControls[0]->GetHeight();
        } else {
            m_nItemSize = 0;
            for (i = 0; i < nCount - 1; i++) {
                int nHeight = m_lstControls[i]->GetHeight();
                if (nHeight > m_nItemSize) {
                    m_nItemSize = nHeight;
                }
            }
        }
        m_nFirstVisible = 0;
        m_nLastVisible = -1;
        m_bScrolledToTop = 1;
        if (m_nItemSize == 0) {
            return;
        }
        m_bDirty = 0;
    }
    if (m_bKeepSelectionVisible && m_nSelected != -1) {
        if (m_nFirstVisible != -1 && m_nSelected < m_nFirstVisible) {
            m_nFirstVisible = m_nSelected;
            m_nLastVisible = -1;
            m_bScrolledToTop = 1;
        } else if (m_nLastVisible != -1 && m_nSelected > m_nLastVisible) {
            m_nFirstVisible = -1;
            m_nLastVisible = m_nSelected;
            m_bScrolledToTop = 0;
        }
    }
    int nPadding = m_n2c0;
    CSWGuiExtent cExtent;
    cExtent.width = m_cArea.width - 2 * nPadding;
    cExtent.left = nPadding;
    cExtent.top = 0;
    cExtent.height = 0;
    int nAnchor;
    int nY;
    if (m_bScrolledToTop) {
        nAnchor = m_nFirstVisible;
        nY = nPadding;
    } else {
        nAnchor = m_nLastVisible;
        nY = m_cArea.height - m_lstControls[nAnchor]->m_extent.height - nPadding;
    }
    cExtent.top = nY;
    cExtent.height = m_lstControls[nAnchor]->m_extent.height;
    m_lstControls[nAnchor]->SetExtent(cExtent);
    for (i = nAnchor - 1; i >= 0; i--) {
        int nHeight = m_lstControls[i]->m_extent.height;
        cExtent.height = nHeight;
        nY -= m_n2c0 + nHeight;
        cExtent.top = nY;
        m_lstControls[i]->SetExtent(cExtent);
    }
    const CSWGuiExtent& cAnchor = m_lstControls[nAnchor]->m_extent;
    nY = cAnchor.height + cAnchor.top + m_n2c0;
    for (i = nAnchor + 1; i < nCount; i++) {
        int nHeight = m_lstControls[i]->m_extent.height;
        cExtent.height = nHeight;
        cExtent.top = nY;
        nY += m_n2c0 + nHeight;
        m_lstControls[i]->SetExtent(cExtent);
    }
    if (m_bScrolledToTop) {
        int bAllFit = 1;
        for (i = m_nFirstVisible + 1; i < nCount; i++) {
            const CSWGuiExtent& cItem = m_lstControls[i]->m_extent;
            if (cItem.height + cItem.top > m_cArea.height) {
                m_nLastVisible = i - 1;
                bAllFit = 0;
                break;
            }
        }
        if (m_nLastVisible == -1 || bAllFit) {
            m_nLastVisible = nCount - 1;
        }
    } else {
        for (i = m_nLastVisible - 1; i >= 0; i--) {
            if (m_lstControls[i]->m_extent.top < 0) {
                m_nFirstVisible = i + 1;
                break;
            }
        }
        if (m_nFirstVisible == -1) {
            m_nFirstVisible = 0;
        }
        m_bScrolledToTop = 1;
    }
    m_nVisible = m_nLastVisible - m_nFirstVisible + 1;
    int nMax = nCount - m_nVisible + 1;
    if (nMax <= 1) {
        nMax = 1;
    }
    m_cScrollBar.SetMaxValue(nMax, m_bFlag9);
    m_cScrollBar.SetVisibleValue(m_nLastVisible - m_nFirstVisible + 1);
    m_cScrollBar.SetCurValue(m_nFirstVisible);
}

// FUNCTION: SWKOTOR 0x0041d3e0
void CSWGuiListBox::LoadProtoItem(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cProto;
    if (pGFF->GetStructFromStruct(&cProto, pStruct, "PROTOITEM")) {
        int bSuccess;
        m_nProtoType = pGFF->ReadFieldINT(&cProto, "CONTROLTYPE", bSuccess, 0);
        switch (m_nProtoType) {
        case 4:
            m_pProtoItem = new CSWGuiLabel;
            break;
        case 5:
            m_pProtoItem = new CSWGuiLabelHilight;
            break;
        case 6:
            m_pProtoItem = new CSWGuiButton;
            break;
        case 7:
            m_pProtoItem = new CSWGuiButtonToggle;
            break;
        case 8:
            m_pProtoItem = new CSWGuiSlider;
            break;
        }
        if (m_pProtoItem) {
            CExoString sName("PROTOITEM");
            m_pProtoItem->Load(this, pGFF, pStruct, sName);
        }
    }
}

// FUNCTION: SWKOTOR 0x0041d5b0
void CSWGuiListBox::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    {
        CExoString sBorder("BORDER");
        m_cBorder.Load(pGFF, pStruct, sBorder);
    }
    {
        CExoString sScrollBar("SCROLLBAR");
        m_cScrollBar.CSWGuiControl::Load(this, pGFF, pStruct, sScrollBar);
    }
    SetScrollBarOnLeft(pGFF->ReadFieldBYTE(pStruct, "LEFTSCROLLBAR", bSuccess, 1));
    SetPadding(pGFF->ReadFieldINT(pStruct, "PADDING", bSuccess, 0));
    m_cScroll = pGFF->ReadFieldVector(pStruct, "COLOR", bSuccess, Vector(0.0f, 0.0f, 0.0f));
    m_bFlag6 = pGFF->ReadFieldBYTE(pStruct, "LOOPING", bSuccess, 0);
    CSWGuiNavigable::Load(pGFF, pStruct);
    LoadProtoItem(pGFF, pStruct);
}
