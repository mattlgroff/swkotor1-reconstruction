// the ability chains of the abilities / level-up screens
// (CSWGuiAbilityChain, vtable 0x007578A8, a row of up to three chained
// feats or powers) and the CSWGuiAbilityList that holds them,
// 0x006CCB80-0x006CDF80.
#include "l/sw_gui_in_game_abilities.h"

// FUNCTION: SWKOTOR 0x006CCB80
CSWGuiAbilityNode::CSWGuiAbilityNode()
{
}

// FUNCTION: SWKOTOR 0x006CCBE0
CSWGuiAbilityNode::~CSWGuiAbilityNode()
{
}

// FUNCTION: SWKOTOR 0x006CCC40
void CSWGuiAbilityChain::Draw(float fDelta)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (m_aNodes[i].m_nId != -1) {
            m_aNodes[i].m_cIndent.Draw(fDelta);
            m_aNodes[i].m_cIcon.Draw(fDelta);
            m_aNodes[i].m_cBorder.Draw(fDelta);
        }
    }
    for (i = 0; i < 2; i++) {
        if (m_aNodes[i + 1].m_nId != -1) {
            m_aArrows[i].Draw(fDelta);
        }
    }
}

// FUNCTION: SWKOTOR 0x006CCCC0
CSWGuiAbilityChain::CSWGuiAbilityChain()
{
    int i;
    for (i = 0; i < 3; i++) {
        m_aNodes[i].m_cBorder.m_cParams.SetCornerImage("border2d", 0);
        m_aNodes[i].m_cBorder.m_cParams.SetEdgeImage("border1d", 0);
        m_aNodes[i].m_cIcon.m_cParams.SetScaled(1);
        m_aNodes[i].m_cIcon.m_cParams.SetAlignment(0x12);
        m_aNodes[i].m_cIndent.m_cParams.SetImage("lbl_indent", 0);
    }
    for (i = 0; i < 2; i++) {
        m_aArrows[i].m_cParams.SetScaled(1);
        m_aArrows[i].m_cParams.SetAlignment(0x12);
        m_aArrows[i].m_cParams.SetImage("lbl_skarr", 0);
    }
    for (i = 0; i < 3; i++) {
        m_aNodes[i].m_bSelected = 0;
        m_aNodes[i].m_nId = -1;
        m_aNodes[i].m_nStatus = 0;
    }
}

// FUNCTION: SWKOTOR 0x006CCE30
void CSWGuiAbilityChain::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent cNode;
    cNode.left = extent.left;
    cNode.top = extent.top;
    cNode.width = extent.height;
    cNode.height = extent.height;
    CSWGuiExtent cArrow;
    cArrow.left = 0;
    cArrow.top = 0;
    cArrow.width = 32;
    cArrow.height = 32;
    m_aNodes[0].m_cBorder.SetExtent(cNode);
    m_aNodes[0].m_cIndent.SetExtent(cNode);
    cNode.left = (extent.width - extent.height) / 2 + extent.left;
    m_aNodes[1].m_cBorder.SetExtent(cNode);
    m_aNodes[1].m_cIndent.SetExtent(cNode);
    cNode.left = extent.left - extent.height + extent.width;
    m_aNodes[2].m_cBorder.SetExtent(cNode);
    m_aNodes[2].m_cIndent.SetExtent(cNode);
    cArrow.top = (extent.height - 32) / 2 + extent.top;
    cArrow.left = (extent.width + extent.height) / 4 + extent.left - 16;
    m_aArrows[0].SetExtent(cArrow);
    cArrow.left = (extent.width * 3 - extent.height) / 4 + extent.left - 16;
    m_aArrows[1].SetExtent(cArrow);
    for (int i = 0; i < 3; i++) {
        m_aNodes[i].m_cIcon.SetExtent(m_aNodes[i].m_cBorder.GetInnerExtent());
    }
    m_extent = extent;
}

// FUNCTION: SWKOTOR 0x006CCFD0
void CSWGuiAbilityChain::SetNodeStatus(unsigned char nNode, unsigned char nStatus)
{
    m_aNodes[nNode].m_nStatus = nStatus;
    m_aNodes[nNode].m_cBorder.m_cParams.m_fAlpha = 1.0f;
    m_aNodes[nNode].m_cIcon.m_cParams.m_fAlpha = 1.0f;
    m_aNodes[nNode].m_cIndent.m_cParams.m_fAlpha = 1.0f;
    if (nNode > 0) {
        m_aArrows[nNode - 1].m_cParams.m_fAlpha = 1.0f;
    }
    switch (nStatus) {
    case 1:
    case 2:
        m_aNodes[nNode].m_cBorder.m_cParams.m_vColor = g_vAbilityColorLocked;
        if (!m_aNodes[nNode].m_bSelected) {
            m_aNodes[nNode].m_cBorder.m_cParams.m_fAlpha = 0.5f;
        }
        break;
    case 4:
        m_aNodes[nNode].m_cBorder.m_cParams.m_vColor = g_vAbilityColorLocked;
        break;
    case 3:
        m_aNodes[nNode].m_cIcon.m_cParams.m_fAlpha = 0.25f;
        m_aNodes[nNode].m_cIndent.m_cParams.m_fAlpha = 0.25f;
        if (m_aNodes[nNode].m_bSelected) {
            m_aNodes[nNode].m_cBorder.m_cParams.m_vColor = g_vAbilityColorAvailable;
        } else {
            m_aNodes[nNode].m_cBorder.m_cParams.m_fAlpha = 0.0f;
        }
        if (nNode > 0) {
            m_aArrows[nNode - 1].m_cParams.m_fAlpha = 0.0f;
        }
        break;
    case 5:
        if (m_aNodes[nNode].m_bSelected) {
            m_aNodes[nNode].m_cBorder.m_cParams.m_vColor = g_vAbilityColorGranted;
        } else {
            m_aNodes[nNode].m_cBorder.m_cParams.m_fAlpha = 0.0f;
        }
        if (nNode > 0) {
            m_aArrows[nNode - 1].m_cParams.m_fAlpha = 0.25f;
        }
        break;
    case 0:
        m_aNodes[nNode].m_cIcon.m_cParams.m_fAlpha = 0.25f;
    case 6:
        if (m_aNodes[nNode].m_bSelected) {
            m_aNodes[nNode].m_cBorder.m_cParams.m_vColor = g_vAbilityColorGranted;
        } else {
            m_aNodes[nNode].m_cBorder.m_cParams.m_fAlpha = 0.0f;
        }
        break;
    }
    if (m_aNodes[nNode].m_bSelected) {
        m_aNodes[nNode].m_cBorder.m_cParams.SetFlags(1, 1, 0);
    } else {
        m_aNodes[nNode].m_cBorder.m_cParams.SetFlags(0, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x006CD1C0
int CSWGuiAbilityChain::HitTest(CSWGuiPoint pt)
{
    int nIndex = 0;
    for (int i = 0; i < 3; i++) {
        if (m_aNodes[i].m_nId == -1) {
            break;
        }
        CSWGuiExtent ext = m_aNodes[i].m_cBorder.m_extent;
        if (pt.x >= ext.left && pt.x <= ext.left + ext.width &&
            pt.y >= ext.top && pt.y <= ext.top + ext.height) {
            nIndex = i;
            break;
        }
    }
    return m_aNodes[nIndex].m_nId;
}

// FUNCTION: SWKOTOR 0x006CD240
void CSWGuiAbilityList::Reset()
{
    for (int i = 0; i < m_lstChains.num; i++) {
        delete m_lstChains.element[i];
        m_lstChains.element[i] = 0;
    }
    m_lstChains.num = 0;
}

// FUNCTION: SWKOTOR 0x006CD280
void CSWGuiAbilityList::SetStatus(int nId, unsigned char nStatus)
{
    for (int i = 0; i < m_lstChains.num; i++) {
        int nNode = m_lstChains.element[i]->GetNodeIndex(nId);
        if (nNode != -1) {
            m_lstChains.element[i]->SetNodeStatus(nNode, nStatus);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x006CD2E0
void CSWGuiAbilityList::SetMode(unsigned char nStatus)
{
    for (int i = 0; i < m_lstChains.num; i++) {
        for (int j = 0; j < 3; j++) {
            m_lstChains.element[i]->SetNodeStatus(j, nStatus);
        }
    }
}

// FUNCTION: SWKOTOR 0x006CD420 SYMBOL: ??_GCSWGuiAbilityChain@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006CD440 SYMBOL: ??1CSWGuiAbilityChain@@UAE@XZ

// FUNCTION: SWKOTOR 0x006CD4C0
void CSWGuiAbilityChain::SetNodeSelected(unsigned char nNode, int bSelected)
{
    if (m_aNodes[nNode].m_bSelected != bSelected) {
        m_aNodes[nNode].m_bSelected = bSelected;
        SetNodeStatus(nNode, m_aNodes[nNode].m_nStatus);
    }
}

// FUNCTION: SWKOTOR 0x006CD510
CSWGuiAbilityList::CSWGuiAbilityList()
{
    m_lstChains.num = 0;
    m_lstChains.array_size = 0;
    m_lstChains.element = 0;
    m_nRow = 0;
    m_nPage = 0;
}

// FUNCTION: SWKOTOR 0x006CD530
CSWGuiAbilityList::~CSWGuiAbilityList()
{
    Reset();
}

// FUNCTION: SWKOTOR 0x006CD7E0
void CSWGuiAbilityList::AddSpellChain(int nSpell1, int nSpell2, int nSpell3)
{
    CResRef cIcon;
    CSWGuiAbilityChain* pChain = new CSWGuiAbilityChain;
    {
    int aSpells[3];
    aSpells[0] = nSpell1;
    aSpells[1] = nSpell2;
    aSpells[2] = nSpell3;
    for (int i = 0; i < 3; i++) {
        int nSpell = aSpells[i];
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
        if (pSpell) {
            cIcon = pSpell->m_cIcon;
        } else {
            cIcon = "";
        }
        pChain->SetNode(i, nSpell, cIcon);
    }
    }
    {
    CSWGuiExtent cExtent;
    cExtent.left = 0;
    cExtent.top = 0;
    cExtent.width = 0xf2;
    cExtent.height = 0x28;
    pChain->SetExtent(cExtent);
    }
    Add(pChain);
}

// FUNCTION: SWKOTOR 0x006CDA80
void CSWGuiAbilityList::AddFeatChain(int nFeat1, int nFeat2, int nFeat3)
{
    CResRef cIcon;
    CSWGuiAbilityChain* pChain = new CSWGuiAbilityChain;
    {
    int aFeats[3];
    aFeats[0] = nFeat1;
    aFeats[1] = nFeat2;
    aFeats[2] = nFeat3;
    for (int i = 0; i < 3; i++) {
        CSWFeat* pFeat = g_pRules->GetFeat(aFeats[i]);
        if (pFeat) {
            cIcon = pFeat->m_cIcon;
        } else {
            cIcon = "";
        }
        pChain->SetNode(i, aFeats[i], cIcon);
    }
    }
    {
    CSWGuiExtent cExtent;
    cExtent.left = 0;
    cExtent.top = 0;
    cExtent.width = 0xf2;
    cExtent.height = 0x28;
    pChain->SetExtent(cExtent);
    }
    Add(pChain);
}

// FUNCTION: SWKOTOR 0x006CDC00
void CSWGuiAbilityList::SetSelected(int nId)
{
    for (int i = 0; i < m_lstChains.num; i++) {
        int nNode = m_lstChains.element[i]->GetNodeIndex(nId);
        if (nNode != -1) {
            if (m_nRow != nNode || m_nPage != i) {
                m_lstChains.element[m_nPage]->Deselect(m_nRow);
                m_nRow = nNode;
                m_nPage = i;
            }
            m_lstChains.element[m_nPage]->Select(m_nRow);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x006CDD10
int CSWGuiAbilityList::Refresh()
{
    if (m_nRow || m_nPage) {
        if (m_nPage < m_lstChains.num) {
            m_lstChains.element[m_nPage]->SetNodeSelected(m_nRow, 0);
        }
        m_nPage = 0;
        m_nRow = 0;
    }
    if (m_lstChains.num > 0) {
        m_lstChains.element[0]->Select(0);
        return m_lstChains.element[0]->m_aNodes[0].m_nId;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x006CDD80
int CSWGuiAbilityList::HandleInputEvent(int nEvent)
{
    int nOldPage = m_nPage;
    int nOldRow = m_nRow;
    switch (nEvent) {
    case 0x32:
    case 0x3e:
        if (m_nPage == m_lstChains.num - 1) {
            m_nPage = 0;
        } else {
            m_nPage++;
        }
        while (m_lstChains.element[m_nPage]->m_aNodes[m_nRow].m_nId == -1) {
            m_nRow--;
        }
        break;
    case 0x31:
    case 0x3d:
        if (m_nPage > 0) {
            m_nPage--;
        } else {
            m_nPage = m_lstChains.num - 1;
        }
        while (m_lstChains.element[m_nPage]->m_aNodes[m_nRow].m_nId == -1) {
            m_nRow--;
        }
        break;
    case 0x30:
    case 0x40:
        if (m_nRow < 2 && m_lstChains.element[m_nPage]->m_aNodes[(unsigned char)(m_nRow + 1)].m_nId != -1) {
            g_pAppManager->m_pClientExoApp->GetGuiManager()->PlayGuiSound(1);
            m_nRow++;
        }
        break;
    case 0x2f:
    case 0x3f:
        if (m_nRow > 0) {
            g_pAppManager->m_pClientExoApp->GetGuiManager()->PlayGuiSound(1);
            m_nRow--;
        }
        break;
    }
    if (nOldPage != m_nPage || nOldRow != m_nRow) {
        m_lstChains.element[nOldPage]->Deselect(nOldRow);
        m_lstChains.element[m_nPage]->Select(m_nRow);
    }
    return m_lstChains.element[m_nPage]->m_aNodes[m_nRow].m_nId;
}

// FUNCTION: SWKOTOR 0x006CD590
void CSWGuiAbilityList::GetSpellList(CExoArrayListL<int>* pList)
{
    for (int i = 0; i < g_pRules->m_pSpellArray->m_nNumSpells; i++) {
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(i);
        if (pSpell && pSpell->m_bUsable == 1 && pSpell->m_nNameStrRef) {
            int bInserted = 0;
            for (int j = 0; j < pList->num; j++) {
                int nId = pList->element[j];
                CSWSpell* pOther = g_pRules->m_pSpellArray->GetSpellI(nId);
                if (!pOther) {
                    continue;
                }
                int nRank = pSpell->GetChainCode(0) - pSpell->GetChainCode(0) / 10 * 10;
                if ((int)(pOther->GetChainCode(0) - pOther->GetChainCode(0) / 10 * 10) > nRank) {
                    goto insert;
                }
                nRank = pSpell->GetChainCode(0) - pSpell->GetChainCode(0) / 10 * 10;
                if ((int)(pOther->GetChainCode(0) - pOther->GetChainCode(0) / 10 * 10) != nRank) {
                    continue;
                }
                {
                    int nGroup = (int)pSpell->GetChainCode(0) / 1000;
                    if ((int)pOther->GetChainCode(0) / 1000 < nGroup) {
                        goto insert;
                    }
                    nGroup = (int)pSpell->GetChainCode(0) / 1000;
                    if ((int)pOther->GetChainCode(0) / 1000 != nGroup) {
                        continue;
                    }
                }
                if (pOther->GetSortKey(0) <= pSpell->GetSortKey(0)) {
                    continue;
                }
insert:
                pList->Insert(i, j);
                bInserted = 1;
                break;
            }
            if (!bInserted) {
                pList->AddAlloc(i);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006CD960
void CSWGuiAbilityList::GetFeatList(CExoArrayListL<unsigned short>* pList)
{
    for (int i = 0; i < g_pRules->m_nNumFeats; i++) {
        CSWFeat* pFeat = g_pRules->GetFeat(i);
        if (pFeat && pFeat->m_nNameStrRef) {
            int nLevel = pFeat->GetChainLevel();
            int bInserted = 0;
            for (int j = 0; j < pList->num; j++) {
                CSWFeat* pOther = g_pRules->GetFeat(pList->element[j]);
                if (pOther && pOther->GetChainLevel() > nLevel) {
                    pList->Insert(i, j);
                    bInserted = 1;
                    break;
                }
            }
            if (!bInserted) {
                pList->Add(i);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006CE0F0
// NONMATCH: the original keeps the element pointer as an induction pointer
// and re-reads it from the list; register allocation differs (59 insns).
void CSWGuiAbilityList::SetCreature(CSWSCreatureStats* pStats, int bAll)
{
    CExoArrayListL<int> lstSpells;
    Reset();
    GetSpellList(&lstSpells);
    int* pSpells = lstSpells.element;
    int bContinue = 1;
    for (int j = 0; bContinue && j < lstSpells.num; j++) {
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(pSpells[j]);
        if (pSpell) {
            if ((int)(pSpell->GetChainCode(0) - pSpell->GetChainCode(0) / 10 * 10) > 0) {
                bContinue = 0;
            } else if (bAll || pStats->HasSpell(0, pSpells[j], 0)) {
                int nUp1 = -1;
                int nUp2 = -1;
                for (int k = j + 1; k < lstSpells.num; k++) {
                    if (nUp1 != -1 && nUp2 != -1) {
                        break;
                    }
                    if (bAll || pStats->HasSpell(0, pSpells[k], 1)) {
                        CSWSpell* pOther = g_pRules->m_pSpellArray->GetSpellI(pSpells[k]);
                        if (pOther &&
                            (int)pSpell->GetChainCode(0) / 1000 == (int)pOther->GetChainCode(0) / 1000 &&
                            pOther->GetSortKey(0) == pSpell->GetSortKey(0)) {
                            if ((int)(pOther->GetChainCode(0) - pOther->GetChainCode(0) / 10 * 10) == 1) {
                                nUp1 = pSpells[k];
                            } else if (pOther->GetUpgradeSlot(0) == 2) {
                                nUp2 = pSpells[k];
                            }
                        }
                    }
                }
                AddSpellChain(pSpells[j], nUp1, nUp2);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006CE370
// NONMATCH: register allocation (the original keeps the element pointer in
// ebx and reloads pStats; ours keeps pStats in ebx).
void CSWGuiAbilityList::SetCreatureB(CSWSCreatureStats* pStats)
{
    CExoArrayListL<unsigned short> lstFeats;
    CSWClass* pClass = &g_pRules->m_pClasses[pStats->GetClass(pStats->m_nNumClasses - 1)];
    Reset();
    GetFeatList(&lstFeats);
    unsigned short* pFeats = lstFeats.element;
    int bContinue = 1;
    for (int j = 0; bContinue && j < lstFeats.num; j++) {
        CSWFeat* pFeat = g_pRules->GetFeat(pFeats[j]);
        if (pFeat) {
            if (pFeat->m_nPrereq1 != 0xffff) {
                bContinue = 0;
            } else if (pStats->HasFeat(pFeats[j])) {
                int nUp1 = -1;
                int nUp2 = -1;
                for (int k = j + 1; k < lstFeats.num; k++) {
                    if (nUp1 != -1 && nUp2 != -1) {
                        break;
                    }
                    if (pStats->HasFeat(pFeats[k])) {
                        CSWFeat* pOther = g_pRules->GetFeat(pFeats[k]);
                        if (pOther && pOther->m_nPrereq1 == pFeats[j] &&
                            (pClass->IsFeatUseable(pFeats[k]) || pStats->HasFeat(pFeats[k]))) {
                            if (pOther->m_nPrereq2 == 0xffff) {
                                nUp1 = pFeats[k];
                            } else {
                                nUp2 = pFeats[k];
                            }
                        }
                    }
                }
                AddFeatChain(pFeats[j], nUp1, nUp2);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006CE570
// NONMATCH: same register allocation / element caching differences as
// SetCreatureB.
void CSWGuiAbilityList::SetLevelUpFeats(CSWCCreatureStats* pStats, int bLevelUp)
{
    CExoArrayListL<unsigned short> lstFeats;
    CSWClass* pClass = &g_pRules->m_pClasses[pStats->GetLastClass()];
    Reset();
    GetFeatList(&lstFeats);
    unsigned short* pFeats = lstFeats.element;
    int bContinue = 1;
    for (int j = 0; bContinue && j < lstFeats.num; j++) {
        CSWFeat* pFeat = g_pRules->GetFeat(pFeats[j]);
        if (pFeat) {
            if (pFeat->m_nPrereq1 != 0xffff) {
                bContinue = 0;
            } else if (pStats->HasFeat(pFeats[j]) || pClass->IsFeatUseable(pFeats[j])) {
                unsigned char nLevel;
                if ((pClass->IsGrantedFeat(pFeats[j], nLevel) && pStats->HasFeat(pFeats[j])) ||
                    (bLevelUp ? pClass->IsBonusFeat(pFeats[j]) : pClass->IsNormalFeat(pFeats[j]))) {
                    int nUp1 = -1;
                    int nUp2 = -1;
                    for (int k = j + 1; k < lstFeats.num; k++) {
                        if (nUp1 != -1 && nUp2 != -1) {
                            break;
                        }
                        CSWFeat* pOther = g_pRules->GetFeat(pFeats[k]);
                        if (pOther && pOther->m_nPrereq1 == pFeats[j] &&
                            (pStats->HasFeat(pFeats[k]) || pClass->IsFeatUseable(pFeats[k]))) {
                            if (pOther->m_nPrereq2 == 0xffff) {
                                nUp1 = pFeats[k];
                            } else {
                                nUp2 = pFeats[k];
                            }
                        }
                    }
                    AddFeatChain(pFeats[j], nUp1, nUp2);
                }
            }
        }
    }
}
