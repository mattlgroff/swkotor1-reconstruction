// CSWGuiInGameCharacter: attribute modifier text, the outlined label, the
// rotating 3D preview and the level-up / auto level-up entry points.
// Member destructors of the panel inline without vptr restores here.
#define L_CHARACTER_VIEW_TU
#include "l/sw_gui_in_game_character.h"

// FUNCTION: SWKOTOR 0x006AE990
void CSWGuiInGameCharacter::SetModText(CSWGuiLabel* pLabel, int nMod)
{
    pLabel->m_cText.SetText(nMod > 0 ? CExoString("+") + nMod : CExoString(nMod));
}

// FUNCTION: SWKOTOR 0x006AEAC0
void CSWGuiBlackenedLabel::Draw(float fDelta)
{
    CSWGuiExtent extent = m_cTextBox.m_extent;
    Vector vColor = m_cText.m_vColor;
    m_cText.SetColor(g_vBlack);
    extent.left--;
    extent.top++;
    m_cTextBox.SetExtent(extent);
    m_cTextBox.Draw(fDelta);
    extent.left += 2;
    m_cTextBox.SetExtent(extent);
    m_cTextBox.Draw(fDelta);
    extent.top -= 2;
    m_cTextBox.SetExtent(extent);
    m_cTextBox.Draw(fDelta);
    extent.left -= 2;
    m_cTextBox.SetExtent(extent);
    m_cTextBox.Draw(fDelta);
    m_cText.SetColor(vColor);
    extent.top++;
    extent.left++;
    m_cTextBox.SetExtent(extent);
    CSWGuiLabel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006AEBF0
void CSWGuiInGameCharacter::AttachModel()
{
    if (g_bDo3DGui) {
        GetPreviewV()->SetGuiScene(m_view3DChar.m_cScene.m_pScene);
        CAurCameraHook* pHook = m_view3DChar.m_cScene.m_pCamera->GetHook(0x3EA);
        if (pHook) {
            pHook->m_bEnabled = 1;
            pHook->m_vPosition = Vector(0.0f, 0.0f, GetPreviewV()->GetModelHeight());
        }
    }
}

// FUNCTION: SWKOTOR 0x006AED20
void CSWGuiInGameCharacter::RotateCharacter(int bLeft)
{
    float fYaw = bLeft ? -10.0f : 10.0f;
    ((CSWCCreatureVirtuals*)m_pPreview)->SetOrientation(YawPitchRoll(fYaw, 0.0f, 0.0f) * m_pPreview->m_vOrientation);
}

// FUNCTION: SWKOTOR 0x006AED90
void CSWGuiInGameCharacter::RotateCharacterLeft(CSWGuiControl*)
{
    ((CSWCCreatureVirtuals*)m_pPreview)->SetOrientation(YawPitchRoll(-10.0f, 0.0f, 0.0f) * m_pPreview->m_vOrientation);
}

// FUNCTION: SWKOTOR 0x006AEDE0
void CSWGuiInGameCharacter::RotateCharacterRight(CSWGuiControl*)
{
    ((CSWCCreatureVirtuals*)m_pPreview)->SetOrientation(YawPitchRoll(10.0f, 0.0f, 0.0f) * m_pPreview->m_vOrientation);
}

// FUNCTION: SWKOTOR 0x006B0BB0
int CSWGuiInGameCharacter::ShowLevelUpGUI(int bLevelUp)
{
    int bAvailable = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bLevelUpAvailable;
    if (bAvailable) {
        if (!g_pAppManager->m_pClientExoApp->GetCharacterChangeInProgress()) {
            SetStats();
            CSWGuiLevelUpCharGen* pLevelUp = new CSWGuiLevelUpCharGen(
                GetManager(), g_pAppManager->m_pClientExoApp->GetPlayerCreature()->GetId(), m_pPreview);
            pLevelUp->m_pCharacter = this;
            pLevelUp->m_nInputClass = bLevelUp ? 0 : 2;
            m_pManager->AddPanel(pLevelUp, 3, 1);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006B0CB0
void CSWGuiInGameCharacter::StartAutoLevelup()
{
    CSWCCreature* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
    unsigned long nExperience = pPC->m_pStats->m_nExperience;
    unsigned char nLevel = pPC->m_pStats->GetTotalLevel();
    unsigned char nNext = nLevel + 1;
    CExoString sMessage;
    while (nExperience >= g_pRules->m_anExperienceTable[nNext]) {
        nNext++;
    }
    {
        CExoString sLevel(nLevel);
        g_pTlkTable->SetCustomToken(0, sLevel);
    }
    {
        CExoString sNext(nNext);
        g_pTlkTable->SetCustomToken(1, sNext);
    }
    sMessage = g_pTlkTable->GetSimpleString(0x8FCB);
    g_pTlkTable->ParseTokens(&sMessage);
    CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(1);
    pBox->SetMessage(sMessage);
    pBox->SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiInGameCharacter::DoAutoLevelup, 0);
    m_pManager->AddPanel(pBox, 1, 1);
}

// FUNCTION: SWKOTOR 0x006B2250
void CSWGuiInGameCharacter::HandleInputEvent(int nEvent, int bDown)
{
    g_pAppManager->m_pClientExoApp->GetParty()->GetMember(0);
    if (bDown) {
        switch (nEvent) {
        case 0x27:
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bLevelUpAvailable &&
                !g_pAppManager->m_pClientExoApp->GetCharacterChangeInProgress()) {
                m_pManager->PlayGuiSound(0);
                ShowLevelUpGUI(0);
            }
            break;
        case 0x29: {
            m_pManager->PlayGuiSound(0);
            CSWCObject* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetMember(0);
            if (pPC) {
                m_pScriptSelect->SetCreatureID(
                    g_pAppManager->m_pServerExoApp->ClientToServerObjectID(pPC->GetId()));
                m_pManager->AddPanel(m_pScriptSelect, 3, 1);
            }
            break;
        }
        case 0x2a:
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bLevelUpAvailable &&
                !g_pAppManager->m_pClientExoApp->GetCharacterChangeInProgress()) {
                m_pManager->PlayGuiSound(0);
                StartAutoLevelup();
            }
            break;
        case 0xce:
            m_pManager->PlayGuiSound(1);
            if (g_pAppManager->m_pServerExoApp->GetServerInfo()->m_bPartyMode) {
                int bFound = 0;
                for (int nNPC = m_nPartyMember + 1; nNPC < 9 && !bFound; nNPC++) {
                    if (g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(nNPC)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        if (pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                                g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(nNPC, bFound, 1)))) {
                            m_nPartyMember = (char)nNPC;
                            bFound = 1;
                        }
                    }
                }
                if (!bFound) {
                    m_nPartyMember = -1;
                }
            } else {
                g_pAppManager->m_pClientExoApp->ChangeCharacterToNextLivingPartyMember(0, 0, 1);
            }
            UpdatePortraits();
            break;
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0xdf:
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->HideSWInGameGui(0)) {
                g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
            }
            break;
        case 0x3b:
            RotateCharacter(1);
            break;
        case 0x3c:
            RotateCharacter(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006AF970
CSWGuiInGameCharacter::~CSWGuiInGameCharacter()
{
    if (m_pScriptSelect) {
        delete m_pScriptSelect;
    }
    if (m_pPreview) {
        delete (CSWCCreatureDeletable*)m_pPreview;
    }
}
