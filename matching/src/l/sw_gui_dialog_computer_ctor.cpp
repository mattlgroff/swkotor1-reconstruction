// CSWGuiDialogComputer constructor (swguidialog.o, vtable 0x00755888): the
// computer terminal panel with its looping hum, reply list and the skill /
// spike / repair-unit counters.
#define L_GUI_TEXT_PARAMS_EXACT
#include "l/sw_gui_tutorial_box.h"

class CSWSkill {
public:
    int m_nNameStrRef;                              // 0x00
    char pad_0x4[0xc];
    CExoString m_sIcon;                             // 0x10
    char pad_0x18[0x8];
};

// FUNCTION: SWKOTOR 0x006A8EB0
// NONMATCH: eax/ecx/edx rotations from the LB_REPLIES InitControl on and in
// the reply-label loop (allocator ties; layout and calls agree).
CSWGuiDialogComputer::CSWGuiDialogComputer(CSWGuiManager* pManager)
    : CSWGuiDialog(pManager)
{
    CExoString sName;
    m_vReplyColor = Vector(0.67f, 0.56f, 0.87f);
    m_pSound = new CExoSoundSource("computer_lp_04");
    if (m_pSound) {
        m_pSound->SetLooping(1);
        m_pSound->SetVolume(30);
    }
    StartLoadFromLayout("computer");
    InitControl(&m_lbEntries, "LB_MESSAGE", 1);
    InitControl(&m_lbReplies, "LB_REPLIES", 1);
    InitControl(&m_lblTitle, "LBL_OBSCURE", 1);
    for (int i = 0; i < 4; i++) {
        sName.Format("LBL_STATIC%d", i + 1);
        InitControl(&m_aLabels[i], sName, 1);
        m_aLabels[i].m_cBorder.m_fAlpha = 0.4f;
    }
    InitControl(&m_aEntries[0].m_lblB, "LBL_COMP_SKILL", 1);
    InitControl(&m_aEntries[0].m_lblA, "LBL_COMP_SKILL_ICON", 1);
    InitControl(&m_aEntries[0].m_lblC, "LBL_COMP_SKILL_VAL", 1);
    InitControl(&m_aEntries[1].m_lblB, "LBL_REP_SKILL", 1);
    InitControl(&m_aEntries[1].m_lblA, "LBL_REP_SKILL_ICON", 1);
    InitControl(&m_aEntries[1].m_lblC, "LBL_REP_SKILL_VAL", 1);
    InitControl(&m_aEntries[2].m_lblB, "LBL_COMP_SPIKES", 1);
    InitControl(&m_aEntries[2].m_lblA, "LBL_COMP_SPIKES_ICON", 1);
    InitControl(&m_aEntries[2].m_lblC, "LBL_COMP_SPIKES_VAL", 1);
    InitControl(&m_aEntries[3].m_lblB, "LBL_REP_UNITS", 1);
    InitControl(&m_aEntries[3].m_lblA, "LBL_REP_UNITS_ICON", 1);
    InitControl(&m_aEntries[3].m_lblC, "LBL_REP_UNITS_VAL", 1);
    UnloadResource();
    CSWSkill* pSkill = &g_pRules->m_pSkills[0];
    m_aEntries[0].m_lblB.m_cText.SetStrRef(pSkill->m_nNameStrRef);
    m_aEntries[0].m_lblA.m_cBorder.SetFillImage(pSkill->m_sIcon, 0);
    pSkill = &g_pRules->m_pSkills[5];
    m_aEntries[1].m_lblB.m_cText.SetStrRef(pSkill->m_nNameStrRef);
    m_aEntries[1].m_lblA.m_cBorder.SetFillImage(pSkill->m_sIcon, 0);
    CSWGuiControl* pProto = m_lbReplies.m_pProtoItem;
    for (int j = 0; j < 20; j++) {
        CSWGuiExtent cReply = {0, 0, m_lbReplies.m_nAreaWidth - 2 * m_lbReplies.m_nPadding, 20};
        m_aReplies[j].CopyStyle(cReply, pProto, 1.0f);
        m_aReplies[j].AddEvent(0, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiDialog::ResetReplyColor);
        m_aReplies[j].AddEvent(1, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiDialog::SetReplyColor);
    }
    CSWGuiControl* pMessageProto = m_lbEntries.m_pProtoItem;
    m_lblMessage.CopyStyle(pMessageProto->m_extent, pMessageProto, 1.0f);
    m_lblMessage.m_cText.SetColor(m_vReplyColor);
}
