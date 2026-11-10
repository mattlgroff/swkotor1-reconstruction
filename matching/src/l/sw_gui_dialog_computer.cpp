// CSWGuiDialogComputer (swguidialog.o): the computer terminal's skill and
// item counters and its background type (comptypes.2da). Names from the
// Mac debug map.
#define L_GUI_LABEL_SET_STYLE_SLOT
#define L_GUI_TEXT_PARAMS_EXACT
#include "l/sw_gui_tutorial_box.h"

extern Vector g_vDialogTopColor;                    // 0x007A239C (descriptive)

class CSWSItem {
public:
    char pad_0x0[0x28];
    CExoString m_sTag;                              // 0x28
    char pad_0x30[0x28c - 0x30];
    int m_nStackSize;                               // 0x28c
};

class CItemRepository {
public:
    CSWSItem* GetItemAt(int nIndex);                // 0x005560B0
    char pad_0x0[0x10];
    unsigned long m_nItems;                         // 0x10
};

class CSWSCreature {
public:
    CItemRepository* GetItemRepository(int bCreate);    // 0x004EF770
};

class CSWCCreatureStats {
public:
    char GetSkillRank(unsigned short nSkill);       // 0x006477B0
};

// FUNCTION: SWKOTOR 0x006A8240
void CSWGuiDialogComputer::UpdateSkills()
{
    CSWCCreature* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetPartyMember(0);
    CItemRepository* pItems = pPC->AsServerCreature()->GetItemRepository(1);
    int nSpikes = 0;
    int nParts = 0;
    for (unsigned long i = 0; i < pItems->m_nItems; i++) {
        CSWSItem* pItem = pItems->GetItemAt(i);
        if (pItem->m_sTag == "k_computer_spike") {
            nSpikes += pItem->m_nStackSize;
        } else if (pItem->m_sTag == "k_repair_part") {
            nParts += pItem->m_nStackSize;
        }
    }
    m_aEntries[0].m_lblC.m_cText.SetText(pPC->m_pStats->GetSkillRank(0));
    m_aEntries[1].m_lblC.m_cText.SetText(pPC->m_pStats->GetSkillRank(5));
    m_aEntries[2].m_lblC.m_cText.SetText(nSpikes);
    m_aEntries[3].m_lblC.m_cText.SetText(nParts);
}

// FUNCTION: SWKOTOR 0x006A8400
void CSWGuiDialogComputer::SetType(unsigned char nType)
{
    C2DA* pTypes = new C2DA("comptypes", 0);
    if (!pTypes->Load2DArray()) {
        return;
    }
    if (nType >= pTypes->m_nNumRows) {
        if (!pTypes->m_nNumRows) {
            return;
        }
        nType = 0;
    }
    m_nResolution = nType;
    CExoString sBackground;
    pTypes->GetCExoStringEntry(nType, "ComputerBackground", &sBackground);
    SetBackground(sBackground);
    delete pTypes;
}

// FUNCTION: SWKOTOR 0x006A95F0
CSWGuiDialogComputerCamera::CSWGuiDialogComputerCamera(CSWGuiManager* pManager)
    : CSWGuiDialog(pManager)
{
    m_pCamera = 0;
    StartLoadFromLayout("computercamera");
    CSWGuiExtent extent = m_extent;
    extent.height = g_nScreenHeight;
    extent.width = g_nScreenWidth;
    SetExtent(extent);
    InitControl(&m_lblReturn, "LBL_RETURN", 1);
    extent = m_lblReturn.m_extent;
    extent.left = (g_nScreenWidth - extent.width) / 2;
    extent.top = g_nScreenHeight - 75;
    m_lblReturn.SetExtent(extent);
    UnloadResource();
}

// FUNCTION: SWKOTOR 0x006A78C0
CSWGuiDialogTop::CSWGuiDialogTop(CSWGuiManager* pManager, const CResRef& cFont, const Vector& vColor)
    : CSWGuiPanel(pManager)
{
    CSWGuiLabel* pLabel = &m_lblReply;
    int nMarginX;
    int nMarginY;
    m_pManager->GetSafeMargins(&nMarginX, &nMarginY);
    m_extent.left = nMarginX;
    m_extent.top = nMarginY;
    CSWGuiManager* pGuiManager = m_pManager;
    m_extent.width = pGuiManager->m_nWidth - 2 * nMarginX;
    m_extent.height =
        (pGuiManager->m_nHeight - (int)(pGuiManager->m_nWidth * 0.4285715f)) / 2 - nMarginY - 5;
    CSWGuiExtent extent = m_lblReply.m_extent;
    extent.width = m_extent.width;
    pLabel->SetExtent(extent);
    m_vColor = g_vDialogTopColor;
    {
        CExoString sEmpty("");
        CSWGuiTextParams cText(sEmpty, cFont, vColor, 0x22, 1.0f);
        CSWGuiExtent cStyle = {0, 0, m_extent.width, m_extent.height};
        pLabel->SetStyle(cStyle, &cText,
                         &CSWGuiBorderParams(CResRef(), CResRef(), CResRef(), 0, 0, 1.0f,
                                             Vector(1.0f, 1.0f, 1.0f), 0),
                         1.0f);
    }
    AddControl(pLabel);
    m_nFlags |= 1;
}
