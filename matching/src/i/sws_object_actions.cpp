// CSWSObject action-queue members (0x004CAC00, 0x004CC3D0-0x004CCD79,
// 0x004CFAC0-0x004CFB7A).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CAC00
CSWSObjectActionNode::CSWSObjectActionNode()
{
    for (int i = 0; i < 13; i++) {
        m_nParamType[i] = 0;
        m_pParam[i] = NULL;
    }
    m_nNumParams = 0;
}

// FUNCTION: SWKOTOR 0x004CC3D0 SYMBOL: ??0?$CExoLinkedList@VCSWSObjectActionNode@@@@QAE@XZ
template CExoLinkedList<CSWSObjectActionNode>::CExoLinkedList();

// FUNCTION: SWKOTOR 0x004CC530
unsigned short CSWSObject::GetActionByGroupId(unsigned short nGroupId,
                                              CSWSObjectActionNode** ppNode)
{
    *ppNode = NULL;
    unsigned short nActionId = 0xffff;
    if (m_lQueuedActions.IsEmpty()) {
        return nActionId;
    }
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
    int bFound = 0;
    while (pNode) {
        if (pNode->m_nGroupActionId == nGroupId) {
            bFound = 1;
            if (FUN_0057a240(pNode->m_nActionId) == 1) {
                nActionId = (unsigned short)pNode->m_nActionId;
                *ppNode = pNode;
            }
        } else if (bFound == 1) {
            break;
        }
        pNode = m_lQueuedActions.GetNext(pos);
    }
    return nActionId;
}

// FUNCTION: SWKOTOR 0x004CC5C0
CExoLinkedListPosition CSWSObject::GetPositionByGroupIndex(int nIndex)
{
    int nGroup = 0;
    if (m_lQueuedActions.IsEmpty()) {
        return NULL;
    }
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
    unsigned short nGroupId = pNode->m_nGroupActionId;
    if (nGroup == nIndex) {
        return pos;
    }
    while (pNode) {
        if (pNode->m_nGroupActionId != nGroupId) {
            nGroupId = pNode->m_nGroupActionId;
            nGroup++;
        }
        if (nGroup == nIndex) {
            return pos;
        }
        pNode = m_lQueuedActions.GetNext(pos);
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004CC630
CSWSObjectActionNode* CSWSObject::GetNodeById(unsigned short nId)
{
    if (m_lQueuedActions.IsEmpty()) {
        return NULL;
    }
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
    while (pos) {
        if (pNode->m_nActionId == nId) {
            return pNode;
        }
        pNode = m_lQueuedActions.GetNext(pos);
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004CC690
int CSWSObject::GetNumActionGroups()
{
    if (m_lQueuedActions.IsEmpty()) {
        return 0;
    }
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
    int nGroups = pNode != NULL;
    unsigned short nGroupId = pNode->m_nGroupActionId;
    while (pos) {
        if (pNode->m_nGroupActionId != nGroupId) {
            nGroupId = pNode->m_nGroupActionId;
            nGroups++;
        }
        pNode = m_lQueuedActions.GetNext(pos);
    }
    return nGroups;
}

// FUNCTION: SWKOTOR 0x004CC6F0
void CSWSObject::RemoveGroup(unsigned short nGroupId)
{
    if (m_lQueuedActions.IsEmpty()) {
        return;
    }
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
    while (pos) {
        if (pNode->m_nGroupActionId == nGroupId) {
            CExoLinkedListPosition posRemove = pos;
            CSWSObjectActionNode* pRemove = pNode;
            pNode = m_lQueuedActions.GetNext(pos);
            m_lQueuedActions.Remove(posRemove);
            delete pRemove;
        } else {
            pNode = m_lQueuedActions.GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x004CC790
void CSWSObject::SetGroupInterruptable(unsigned short nGroupId, int bInterruptable)
{
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    CSWSObjectActionNode* pNode = m_lQueuedActions.GetHead();
    while (pNode) {
        if (pNode->m_nGroupActionId == nGroupId) {
            pNode->m_bInterruptable = bInterruptable;
        }
        pNode = m_lQueuedActions.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x004CCD30
void CSWSObject::DeleteCurrentAIAction()
{
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    if (pos) {
        if (RunActionNode(m_lQueuedActions.GetHead(), 1) == 1) {
            CSWSObjectActionNode* pNode = m_lQueuedActions.RemoveHead();
            delete pNode;
        }
    }
}

// FUNCTION: SWKOTOR 0x004CFAC0
int CSWSObject::AddOpenDoorAction(unsigned long oidDoor, int bFlag)
{
    if (!m_bCommandable) {
        return 0;
    }
    AddAction(0x14, 0xffff, 3, &oidDoor, 1, &bFlag);
    return 1;
}

// FUNCTION: SWKOTOR 0x004CFB20
int CSWSObject::AddCloseDoorAction(unsigned long oidDoor, int bFlag)
{
    if (!m_bCommandable) {
        return 0;
    }
    AddAction(0x15, 0xffff, 3, &oidDoor, 1, &bFlag);
    return 1;
}

// FUNCTION: SWKOTOR 0x004CFB80
int CSWSObject::AddUnlockObjectAction(unsigned long oidObject, unsigned long oidKey, int bFlag)
{
    if (AsSWSCreature() && !AsSWSCreature()->GetCreatureStats()->GetCanUseSkill(6)) {
        AsSWSCreature()->SendFeedbackMessage(0, NULL);
        return 0;
    }
    if (!m_bCommandable) {
        return 0;
    }
    AddAction(0x26, 0xffff, 3, &oidObject, 3, &oidKey, 1, &bFlag);
    return 1;
}

// FUNCTION: SWKOTOR 0x004CFC20
int CSWSObject::AddLockObjectAction(unsigned long oidObject)
{
    if ((AsSWSCreature() && !AsSWSCreature()->GetCreatureStats()->GetCanUseSkill(6)) || !m_bCommandable) {
        return 0;
    }
    AddAction(0x27, 0xffff, 3, &oidObject);
    return 1;
}

// FUNCTION: SWKOTOR 0x004CCB00
void CSWSObject::AddActionToFront(int nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2, void* p2, int nType3, void* p3,
                   int nType4, void* p4, int nType5, void* p5, int nType6, void* p6,
                   int nType7, void* p7, int nType8, void* p8, int nType9, void* p9,
                   int nType10, void* p10, int nType11, void* p11, int nType12, void* p12,
                   int nType13, void* p13)
{
    CSWSObjectActionNode* pNode = new CSWSObjectActionNode();
    pNode->m_nActionId = nActionId;
    pNode->m_bInterruptable = 1;
    if (nGroupId == 0xffff) {
        unsigned short nId = m_nNextGroupActionId++;
        if (m_nNextGroupActionId == 0xffff) {
            m_nNextGroupActionId = 0;
        }
        m_nLastGroupActionId = nId;
        pNode->m_nGroupActionId = nId;
    }
    else if (nGroupId == 0xfffe) {
        pNode->m_nGroupActionId = m_nLastGroupActionId;
    }
    else {
        pNode->m_nGroupActionId = nGroupId;
    }
    if (nType1) {
        AddActionNodeParameter(pNode, 0, nType1, p1);
    if (nType2) {
        AddActionNodeParameter(pNode, 1, nType2, p2);
    if (nType3) {
        AddActionNodeParameter(pNode, 2, nType3, p3);
    if (nType4) {
        AddActionNodeParameter(pNode, 3, nType4, p4);
    if (nType5) {
        AddActionNodeParameter(pNode, 4, nType5, p5);
    if (nType6) {
        AddActionNodeParameter(pNode, 5, nType6, p6);
    if (nType7) {
        AddActionNodeParameter(pNode, 6, nType7, p7);
    if (nType8) {
        AddActionNodeParameter(pNode, 7, nType8, p8);
    if (nType9) {
        AddActionNodeParameter(pNode, 8, nType9, p9);
    if (nType10) {
        AddActionNodeParameter(pNode, 9, nType10, p10);
    if (nType11) {
        AddActionNodeParameter(pNode, 10, nType11, p11);
    if (nType12) {
        AddActionNodeParameter(pNode, 11, nType12, p12);
    if (nType13) {
        AddActionNodeParameter(pNode, 12, nType13, p13);
    }}}}}}}}}}}}}
    m_lQueuedActions.AddHead(pNode);
    {
        CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
        CSWSObjectActionNode* pCur = m_lQueuedActions.GetHead();
        while (pCur) {
            pCur = m_lQueuedActions.GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x004CEA20
void CSWSObject::AddAction(int nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2, void* p2, int nType3, void* p3,
                   int nType4, void* p4, int nType5, void* p5, int nType6, void* p6,
                   int nType7, void* p7, int nType8, void* p8, int nType9, void* p9,
                   int nType10, void* p10, int nType11, void* p11, int nType12, void* p12,
                   int nType13, void* p13)
{
    CSWSObjectActionNode* pNode = new CSWSObjectActionNode();
    pNode->m_nActionId = nActionId;
    pNode->m_bInterruptable = 1;
    if (nGroupId == 0xffff) {
        unsigned short nId = m_nNextGroupActionId++;
        if (m_nNextGroupActionId == 0xffff) {
            m_nNextGroupActionId = 0;
        }
        m_nLastGroupActionId = nId;
        pNode->m_nGroupActionId = nId;
    }
    else if (nGroupId == 0xfffe) {
        pNode->m_nGroupActionId = m_nLastGroupActionId;
    }
    else {
        pNode->m_nGroupActionId = nGroupId;
    }
    if (nType1) {
        AddActionNodeParameter(pNode, 0, nType1, p1);
    if (nType2) {
        AddActionNodeParameter(pNode, 1, nType2, p2);
    if (nType3) {
        AddActionNodeParameter(pNode, 2, nType3, p3);
    if (nType4) {
        AddActionNodeParameter(pNode, 3, nType4, p4);
    if (nType5) {
        AddActionNodeParameter(pNode, 4, nType5, p5);
    if (nType6) {
        AddActionNodeParameter(pNode, 5, nType6, p6);
    if (nType7) {
        AddActionNodeParameter(pNode, 6, nType7, p7);
    if (nType8) {
        AddActionNodeParameter(pNode, 7, nType8, p8);
    if (nType9) {
        AddActionNodeParameter(pNode, 8, nType9, p9);
    if (nType10) {
        AddActionNodeParameter(pNode, 9, nType10, p10);
    if (nType11) {
        AddActionNodeParameter(pNode, 10, nType11, p11);
    if (nType12) {
        AddActionNodeParameter(pNode, 11, nType12, p12);
    if (nType13) {
        AddActionNodeParameter(pNode, 12, nType13, p13);
    }}}}}}}}}}}}}
    m_lQueuedActions.AddTail(pNode);
    {
        CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
        CSWSObjectActionNode* pCur = m_lQueuedActions.GetHead();
        while (pCur) {
            pCur = m_lQueuedActions.GetNext(pos);
        }
    }
}

extern int g_bLogActions;                           // 0x0083281C (placeholder name)

// FUNCTION: SWKOTOR 0x004CCD80
void CSWSObject::ClearAllActions(int bClearCombat)
{
    if (g_bLogActions && AsSWSCreature() && AsSWSCreature()->m_bPlayerCharacter) {
        m_sTag.CStr();
    }
    if (m_bCommandable != 1) {
        return;
    }
    CExoLinkedListPosition pos = m_lQueuedActions.GetHeadPos();
    while (pos) {
        CSWSObjectActionNode* pNode = m_lQueuedActions.GetAtPos(pos);
        CExoLinkedListPosition posCur = pos;
        m_lQueuedActions.GetNext(pos);
        if (RunActionNode(pNode, pNode == m_lQueuedActions.GetHeadInline()) == 1) {
            CSWSObjectActionNode* pRemoved = m_lQueuedActions.Remove(posCur);
            if (pRemoved) {
                delete pRemoved;
            }
        }
    }
    if (AsSWSCreature()) {
        AsSWSCreature()->m_pcPathfindInformation->ResetAtEndOfPath();
        AsSWSCreature()->m_nAIState = 1;
        AsSWSCreature()->m_oid508 = 0x7f000000;
        CPathfindInformation* pInfo = AsSWSCreature()->m_pcPathfindInformation;
        pInfo->m_oidTarget = 0x7f000000;
        pInfo->m_vTarget = Vector(0.0f, 0.0f, 0.0f);
        if (bClearCombat) {
            AsSWSCreature()->m_pcCombatRound->ClearAllAttacks();
        }
        AsSWSCreature()->m_sTag.CStr();
        if (AsSWSCreature()->m_bPlayerCharacter) {
            unsigned long oidSelf = AsSWSCreature()->m_idSelf;
            int nIndex = g_pAppManager->m_pClientExoApp->GetParty()->IsPartyMember(
                g_pAppManager->m_pClientExoApp->ServerToClientObjectId(oidSelf));
            if (nIndex != -1) {
                g_pAppManager->m_pClientExoApp->GetParty()->m_aMembers[nIndex].m_n28 = -1;
            }
        }
    }
}
