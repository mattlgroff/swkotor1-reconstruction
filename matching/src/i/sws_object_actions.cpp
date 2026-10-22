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
