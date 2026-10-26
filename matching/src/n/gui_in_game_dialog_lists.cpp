// CGuiInGame dialog participant lists, conversation panels and dialog
// camera reset (guiingame.o).
#include "n/gui_in_game_dialog.h"

// FUNCTION: SWKOTOR 0x00628670
void CGuiInGame::SetConversationType(unsigned long nType, unsigned char nComputerType)
{
    if (nType == 1) {
        if (m_pDialogComputer == 0)
            m_pDialogComputer = new CSWGuiDialogComputer(m_pGuiManager);
        m_pDialog = m_pDialogComputer;
        m_pDialogComputer->SetType(nComputerType);
    }
    else {
        m_pDialog = m_pDialogNormal;
    }
}

// FUNCTION: SWKOTOR 0x0062A970
void CGuiInGame::ShutDownGlobal()
{
    if (m_pGlobalPanel98) {
        delete m_pGlobalPanel98;
        m_pGlobalPanel98 = 0;
    }
    if (m_pGlobalPanel9C) {
        delete m_pGlobalPanel9C;
        m_pGlobalPanel9C = 0;
    }
    if (m_pGlobalPanelA0) {
        delete m_pGlobalPanelA0;
        m_pGlobalPanelA0 = 0;
    }
    if (m_pGlobalPanelA4) {
        delete m_pGlobalPanelA4;
        m_pGlobalPanelA4 = 0;
    }
    m_pGuiManager = 0;
}

// FUNCTION: SWKOTOR 0x0062C2A0
void CGuiInGame::EndDialogCamera()
{
    g_pAppManager->m_pClientExoApp->GetModule()->RestoreCamera();
    m_bAnimatedCamera = 0;
    m_fAnimatedCameraTime = 0.0f;
    m_bShotCameraInitialized = 0;
    m_bCameraModelSet = 0;
    m_nB34 = 0;
    m_nAnimatedCamera = 10098;
    m_oid184 = 0x7F000000;
    m_oidDialogOwner = 0x7F000000;
    m_oidDialogSpeaker = 0x7F000000;
    m_oid178 = 0x7F000000;
    m_oid17C = 0x7F000000;
}

// FUNCTION: SWKOTOR 0x0062D820
int CGuiInGame::IsOverlayDialogAnimation(unsigned short nAnimation)
{
    if (nAnimation >= 10000 && nAnimation < g_pRules->m_p2DArrays->m_pDialogAnimations->m_nNumRows + 10000) {
        C2DA* pTable = g_pRules->m_p2DArrays->m_pDialogAnimations;
        int nRow = (unsigned short)(nAnimation - 10000);
        int bOverlay;
        int bDialog;
        if (pTable->GetINTEntry(nRow, g_nDialogAnimOverlayColumn, &bOverlay)) {
            pTable = g_pRules->m_p2DArrays->m_pDialogAnimations;
            if (pTable->GetINTEntry(nRow, g_nDialogAnimDialogColumn, &bDialog) && bOverlay && bDialog)
                return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0062F160
void CGuiInGame::SetLockObjectOrientationInDialog(unsigned long oid, int bLock)
{
    if (bLock == 1) {
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        CGameObject* pObject = pClient->GetGameObject(oid);
        if (pObject == 0 || pObject->AsSWCObject() == 0)
            return;
        CExoLinkedListPosition pos = m_lstLockedOrientation.GetHeadPos();
        unsigned long* pId = m_lstLockedOrientation.GetHead();
        while (pos) {
            if (*pId == oid)
                return;
            pId = m_lstLockedOrientation.GetNext(pos);
        }
        unsigned long* pNew = new unsigned long;
        if (pNew) {
            *pNew = oid;
            m_lstLockedOrientation.AddHead(pNew);
        }
    }
    else {
        CExoLinkedListPosition pos = m_lstLockedOrientation.GetHeadPos();
        unsigned long* pId = m_lstLockedOrientation.GetHead();
        while (pos) {
            if (*pId == oid) {
                m_lstLockedOrientation.Remove(pos);
                return;
            }
            pId = m_lstLockedOrientation.GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062F250
int CGuiInGame::GetCanOrientInDialog(unsigned long oid)
{
    if (oid == 0x7F000000)
        return 0;
    CExoLinkedListPosition pos = m_lstLockedOrientation.GetHeadPos();
    unsigned long* pId = m_lstLockedOrientation.GetHead();
    while (pos) {
        if (*pId == oid)
            return 0;
        pId = m_lstLockedOrientation.GetNext(pos);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0062F2B0
void CGuiInGame::SetLockHeadFollowInDialog(unsigned long oid, int bLock)
{
    if (bLock == 1) {
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        CGameObject* pObject = pClient->GetGameObject(oid);
        if (pObject == 0 || pObject->AsSWCObject() == 0)
            return;
        CExoLinkedListPosition pos = m_lstLockedHeadFollow.GetHeadPos();
        unsigned long* pId = m_lstLockedHeadFollow.GetHead();
        while (pos) {
            if (*pId == oid)
                return;
            pId = m_lstLockedHeadFollow.GetNext(pos);
        }
        unsigned long* pNew = new unsigned long;
        if (pNew) {
            *pNew = oid;
            m_lstLockedHeadFollow.AddHead(pNew);
            if (pObject->AsSWCCreature())
                pObject->AsSWCCreature()->LookAt(0x7F000000, 0.0f);
        }
    }
    else {
        CExoLinkedListPosition pos = m_lstLockedHeadFollow.GetHeadPos();
        unsigned long* pId = m_lstLockedHeadFollow.GetHead();
        while (pos) {
            if (*pId == oid) {
                m_lstLockedHeadFollow.Remove(pos);
                return;
            }
            pId = m_lstLockedHeadFollow.GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062F3C0
int CGuiInGame::GetCanHeadFollowInDialog(unsigned long oid)
{
    if (oid == 0x7F000000)
        return 0;
    CExoLinkedListPosition pos = m_lstLockedHeadFollow.GetHeadPos();
    unsigned long* pId = m_lstLockedHeadFollow.GetHead();
    while (pos) {
        if (*pId == oid)
            return 0;
        pId = m_lstLockedHeadFollow.GetNext(pos);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0062F9A0
int CGuiInGame::CanPerformAction()
{
    unsigned long nTimeDiff = 0;
    unsigned long nTime;
    unsigned long nDay;
    unsigned long nDays;
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    pClient->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    pClient = g_pAppManager->m_pClientExoApp;
    pClient->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nActionDay, m_nActionTime, &nDays, &nTimeDiff);
    if (nDays > 0)
        return 1;
    return nTimeDiff >= m_nActionDelay;
}

// FUNCTION: SWKOTOR 0x00632300
void CGuiInGame::AddToDialogNonDependantList(const CExoString& sName)
{
    if (sName.CompareNoCase(""))
        return;
    if (sName.IsEmpty())
        return;
    if (IsOnDialogNonDependantList(sName))
        return;
    CExoString* pName = new CExoString;
    *pName = sName;
    m_lstDialogNonDependant.AddHead(pName);
}

// FUNCTION: SWKOTOR 0x0062D5E0
int CGuiInGame::IsLoopingDialogAnimation(unsigned short nAnimation)
{
    int bLooping;
    int bDialog;
    if (nAnimation >= 10000 && nAnimation < g_pRules->m_p2DArrays->m_pDialogAnimations->m_nNumRows + 10000) {
        C2DA* pTable = g_pRules->m_p2DArrays->m_pDialogAnimations;
        int nRow = (unsigned short)(nAnimation - 10000);
        if (!pTable->GetINTEntry(nRow, g_nDialogAnimLoopingColumn, &bLooping))
            return 0;
        pTable = g_pRules->m_p2DArrays->m_pDialogAnimations;
        if (pTable->GetINTEntry(nRow, g_nDialogAnimDialogColumn, &bDialog) && bLooping && bDialog)
            return 1;
        return 0;
    }
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nAnimation, g_nAnimLoopingColumn, &bLooping)) {
        pTable = g_pRules->m_p2DArrays->m_pAnimations;
        if (pTable->GetINTEntry(nAnimation, g_nAnimDialogColumn, &bDialog) && bLooping && bDialog)
            return 1;
    }
    if (nAnimation >= 1400 && nAnimation <= 1727)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0062D700
int CGuiInGame::IsFireForgetDialogAnimation(unsigned short nAnimation)
{
    int bFireForget;
    int bDialog;
    if (nAnimation >= 10000 && nAnimation < g_pRules->m_p2DArrays->m_pDialogAnimations->m_nNumRows + 10000) {
        C2DA* pTable = g_pRules->m_p2DArrays->m_pDialogAnimations;
        int nRow = (unsigned short)(nAnimation - 10000);
        if (!pTable->GetINTEntry(nRow, g_nDialogAnimFireForgetColumn, &bFireForget))
            return 0;
        pTable = g_pRules->m_p2DArrays->m_pDialogAnimations;
        if (pTable->GetINTEntry(nRow, g_nDialogAnimDialogColumn, &bDialog) && bFireForget && bDialog)
            return 1;
        return 0;
    }
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nAnimation, g_nAnimFireForgetColumn, &bFireForget)) {
        pTable = g_pRules->m_p2DArrays->m_pAnimations;
        if (pTable->GetINTEntry(nAnimation, g_nAnimDialogColumn, &bDialog) && bFireForget && bDialog)
            return 1;
    }
    if (nAnimation >= 1000 && nAnimation <= 1327)
        return 1;
    return 0;
}

// Descriptive name: the area-transition panel's stored position, or zero.
// FUNCTION: SWKOTOR 0x0062EC20
Vector CGuiInGame::GetAreaTransitionPosition()
{
    if (m_pAreaTransition == 0)
        return Vector(0.0f, 0.0f, 0.0f);
    return m_pAreaTransition->m_vPosition;
}

// FUNCTION: SWKOTOR 0x0062CCD0
void CGuiInGame::NextSWInGameGui()
{
    if (!m_bInitialized)
        return;
    CSWGuiManager* pManager = g_pAppManager->m_pClientExoApp->GetGuiManager();
    if (g_bGuiFlag && m_bShowPanels && pManager)
        pManager->RemovePanel(m_apPanels[m_nCurrentPanel]);
    int nOldPanel = m_nCurrentPanel;
    if (++m_nCurrentPanel >= 8)
        m_nCurrentPanel = 0;
    UpdateCreatedInGameGUI(nOldPanel, (m_bShowPanels && pManager) ? m_nCurrentPanel : -1);
    if (m_bShowPanels && pManager) {
        pManager->AddPanel(m_apPanels[m_nCurrentPanel], 2, 1);
        pManager->SendPanelToBack(m_apPanels[m_nCurrentPanel]);
        m_pInGameMenu->SetActiveControlID(m_nCurrentPanel, 1);
        if (g_pVirtualMachine) {
            CExoString sScript("k_pend_screenchg");
            g_pVirtualMachine->RunScript(&sScript, 0x7F000000, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062CDF0
void CGuiInGame::PrevSWInGameGui()
{
    if (!m_bInitialized)
        return;
    CSWGuiManager* pManager = g_pAppManager->m_pClientExoApp->GetGuiManager();
    if (g_bGuiFlag && m_bShowPanels && pManager)
        pManager->RemovePanel(m_apPanels[m_nCurrentPanel]);
    int nOldPanel = m_nCurrentPanel;
    if (--m_nCurrentPanel < 0)
        m_nCurrentPanel = 7;
    UpdateCreatedInGameGUI(nOldPanel, (m_bShowPanels && pManager) ? m_nCurrentPanel : -1);
    if (m_bShowPanels && pManager) {
        pManager->AddPanel(m_apPanels[m_nCurrentPanel], 2, 1);
        pManager->SendPanelToBack(m_apPanels[m_nCurrentPanel]);
        m_pInGameMenu->SetActiveControlID(m_nCurrentPanel, 1);
        if (g_pVirtualMachine) {
            CExoString sScript("k_pend_screenchg");
            g_pVirtualMachine->RunScript(&sScript, 0x7F000000, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062CF10
void CGuiInGame::SwitchToSWInGameGui(int nPanel)
{
    if (nPanel < 0 || nPanel >= 8 || nPanel == m_nCurrentPanel || !m_bInitialized)
        return;
    CSWGuiManager* pManager = g_pAppManager->m_pClientExoApp->GetGuiManager();
    if (g_bGuiFlag && m_bShowPanels && pManager)
        pManager->RemovePanel(m_apPanels[m_nCurrentPanel]);
    UpdateCreatedInGameGUI(m_nCurrentPanel, (m_bShowPanels && pManager) ? nPanel : -1);
    m_nCurrentPanel = nPanel;
    if (m_bShowPanels && pManager) {
        pManager->AddPanel(m_apPanels[nPanel], 2, 1);
        pManager->SendPanelToBack(m_apPanels[m_nCurrentPanel]);
        m_pInGameMenu->SetActiveControlID(m_nCurrentPanel, 1);
        if (g_pVirtualMachine) {
            CExoString sScript("k_pend_screenchg");
            g_pVirtualMachine->RunScript(&sScript, 0x7F000000, 0);
        }
    }
}
