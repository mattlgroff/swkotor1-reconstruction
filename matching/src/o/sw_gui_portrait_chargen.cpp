// CSWGuiPortraitCharGen (swguiportraitchargen.o, layout "PORTCUST"): the
// portrait picker opened by the quick and custom character panels, with a
// 3D head preview, 0x006F89D0-0x006F9430.
#include "o/sw_gui_main_chargen.h"

#include <stdlib.h>

// FUNCTION: SWKOTOR 0x006F89D0
void CSWGuiPortraitCharGen::OnPanelAdded()
{
    m_fIdleTimer = (rand() % 300) * 0.01f + 1.0f;
}

// FUNCTION: SWKOTOR 0x006F8A00
void CSWGuiPortraitCharGen::SetParentPanel(CSWGuiPanel* pParent, unsigned char nType)
{
    m_pParent = pParent;
    m_nParentType = nType;
}

// FUNCTION: SWKOTOR 0x006F8A20
void CSWGuiPortraitCharGen::OnSelectDisplayButton()
{
    CResRef cEmpty("");
    GetCreatureV()->SetPortrait(cEmpty);
    GetCreatureV()->SetPortraitId(m_lstPortraits[m_nCurrent]);
    GetStats()->m_nAppearance = m_lstAppearances[m_nCurrent];
    m_pManager->PopModalPanel();
    m_nFlags = (m_nFlags & ~0x300) | 0x400;
    if (m_nParentType == 1) {
        ((CSWGuiQuickPanel*)m_pParent)->PortraitCreationCompleted();
    } else if (m_nParentType == 2) {
        ((CSWGuiCustomPanel*)m_pParent)->PortraitCreationCompleted();
    }
}

// Applies the selected portraits.2da row (portrait id and head appearance),
// refreshes the portrait label and re-attaches the head to the scene.
// FUNCTION: SWKOTOR 0x006F8AD0
void CSWGuiPortraitCharGen::UpdatePortraitButton()
{
    int& nPortraitId = m_lstPortraits[m_nCurrent];
    if (GetStats()->m_nPortraitId != nPortraitId) {
        GetCreatureV()->SetPortraitId(nPortraitId);
        CSWCCreatureAppearance cAppearance = *m_pCreature->m_pAppearance;
        cAppearance.m_nAppearance = m_lstAppearances[m_nCurrent];
        m_pCreature->SetAppearance(&cAppearance, 2, 1);
        m_pCreature->m_pAppearance->m_nAppearance = m_lstAppearances[m_nCurrent];
        if (g_bDo3DGui) {
            CAurObject* pModel = m_pCreature->GetAnimBase()->GetModel(0xff);
            if (pModel) {
                pModel->PlayAnimation("pause1", 1.0f, 2, 0);
            }
            pModel = m_pCreature->GetAnimBase()->GetModel(0xfe);
            if (pModel) {
                pModel->PlayAnimation("pause1", 1.0f, 2, 0);
            }
        }
    }
    m_lblPortrait.SetFillImage(GetCreatureV()->GetPortrait(0));
    if (g_bDo3DGui) {
        GetCreatureV()->SetGuiScene(m_cHead.m_cScene.m_pScene);
        CAurCameraHook* pHook = m_cHead.m_cScene.m_pCamera->GetHook(0x3EA);
        if (pHook) {
            pHook->m_bEnabled = 0;
            pHook->m_vPosition = Vector(0.0f, GetCreatureV()->GetModelHeight(), 0.0f);
        }
    }
}

// Expires the control timers, keeps the head in the panel scene and plays
// an idle animation when the idle timer runs out.
// FUNCTION: SWKOTOR 0x006F8C60
void CSWGuiPortraitCharGen::Draw(float fDelta)
{
    int nCount = m_lstTimers.num;
    for (int i = nCount - 1; i >= 0; i--) {
        m_lstTimers[i].m_fTime -= fDelta;
        if (m_lstTimers[i].m_fTime <= 0.0f) {
            m_lstTimers[i].m_pControl->SetFlag0(0);
            m_lstTimers.DelIndex(i);
        }
    }
    if (m_pCreature && g_bDo3DGui) {
        CAurObject* pBody = m_pCreature->GetAnimBase()->GetModel(0xff);
        CAurScene* pScene = m_cHead.m_cScene.m_pScene;
        if (pBody->GetScene() != pScene) {
            pScene = m_cHead.m_cScene.m_pScene;
            GetCreatureV()->SetGuiScene(pScene);
            CAurCameraHook* pHook = m_cHead.m_cScene.m_pCamera->GetHook(0x3EA);
            if (pHook) {
                pHook->m_bEnabled = 0;
                pHook->m_vPosition = Vector(0.0f, GetCreatureV()->GetModelHeight(), 0.0f);
            }
        }
        m_fIdleTimer -= fDelta;
        if (m_fIdleTimer <= 0.0f) {
            CExoString sIdle;
            CExoString sPause("pause1");
            CAurObject* pModel = m_pCreature->GetAnimBase()->GetModel(0xff);
            switch (rand() % 2) {
            case 0:
                sIdle = "pause2";
                break;
            case 1:
                sIdle = "listen";
                break;
            }
            if (pModel) {
                pModel->SetAnimationBlend(1);
                pModel->PlayAnimation(sIdle.CStr(), 1.0f, 0x21, 0);
                pModel->PlayAnimation(sPause.CStr(), 1.0f, 0x60, 0);
            }
            pModel = m_pCreature->GetAnimBase()->GetModel(0xfe);
            if (pModel) {
                pModel->SetAnimationBlend(1);
                pModel->PlayAnimation(sIdle.CStr(), 1.0f, 0x21, 0);
                pModel->PlayAnimation(sPause.CStr(), 1.0f, 0x60, 0);
            }
            m_fIdleTimer = (rand() % 300) * 0.01f + 1.0f;
        }
    }
    CSWGuiPanel::Draw(fDelta);
}


// Restores the creature's original portrait row.
// FUNCTION: SWKOTOR 0x006F8F40
void CSWGuiPortraitCharGen::HandleCancelButton()
{
    if (m_pParent) {
        m_nCurrent = m_lstPortraits.IndexOf(m_nOriginalPortraitId);
        UpdatePortraitButton();
        m_pManager->PopModalPanel();
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        if (m_nParentType == 1) {
            ((CSWGuiQuickPanel*)m_pParent)->CancelledByChild();
        } else if (m_nParentType == 2) {
            ((CSWGuiCustomPanel*)m_pParent)->CancelledByChild();
        }
    }
}

// FUNCTION: SWKOTOR 0x006F8FC0
void CSWGuiPortraitCharGen::HandleRightButton()
{
    if (++m_nCurrent >= m_lstPortraits.num) {
        m_nCurrent = 0;
    }
    UpdatePortraitButton();
}

// FUNCTION: SWKOTOR 0x006F8FF0
void CSWGuiPortraitCharGen::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            OnSelectDisplayButton();
            break;
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            HandleCancelButton();
            break;
        case 0x2F:
        case 0x35:
        case 0x3F:
            m_pManager->PlayGuiSound(1);
            if (--m_nCurrent < 0) {
                m_nCurrent = m_lstPortraits.num - 1;
            }
            UpdatePortraitButton();
            break;
        case 0x30:
        case 0x36:
        case 0x40:
            m_pManager->PlayGuiSound(1);
            HandleRightButton();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// Collects the portraits.2da rows for the gender (ForPC rows only) and the
// head appearance column for the creature's class. ForPC is read into an
// uninitialised local, as in the original.
// NONMATCH: register picks (this / the table / the second Add argument).
// FUNCTION: SWKOTOR 0x006F90F0
void CSWGuiPortraitCharGen::GetPortraitsList(int nGender)
{
    C2DA* pTable = new C2DA(CResRef("PORTRAITS"), 0);
    pTable->Load2DArray();
    int nRows = pTable->m_nNumRows;
    CExoString sBaseResRef;
    CResRef cResRef;
    m_lstAppearances.num = 0;
    m_lstPortraits.num = 0;
    for (int nRow = 0; nRow < nRows; nRow++) {
        int nSex = -1;
        int nAppearance = -1;
        int bForPC;
        pTable->GetINTEntry(nRow, "Sex", &nSex);
        if (nSex != nGender) {
            continue;
        }
        pTable->GetINTEntry(nRow, "ForPC", &bForPC);
        if (bForPC != 1) {
            continue;
        }
        switch (GetStats()->GetClass(0)) {
        case 2:
            pTable->GetINTEntry(nRow, "Appearance_S", &nAppearance);
            break;
        case 1:
            pTable->GetINTEntry(nRow, "AppearanceNumber", &nAppearance);
            break;
        case 0:
            pTable->GetINTEntry(nRow, "Appearance_L", &nAppearance);
            break;
        default:
            pTable->GetINTEntry(nRow, "AppearanceNumber", &nAppearance);
            break;
        }
        pTable->GetCExoStringEntry(nRow, "BaseResRef", &sBaseResRef);
        if (GetStats()->m_nPortraitId == nRow) {
            m_nCurrent = m_lstPortraits.num;
        }
        m_lstPortraits.Add(nRow);
        m_lstAppearances.Add(nAppearance);
    }
    delete pTable;
}
