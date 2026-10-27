// CSWSObject::AIActionGiveItem (round s10).
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/vector.h"

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);               // 0x0062EEB0
};

// .rdata action results.
extern const unsigned long ACTION_COMPLETE;         // 0x00745DC0 (2)
extern const unsigned long ACTION_FAILED;           // 0x00745DC4 (3)
// Squared distance beyond which the giver runs (.rdata 0x00745E5C, 25.0f).
extern const float AI_GIVE_ITEM_RUN_DISTANCE_SQ;

static __forceinline CSWBaseItem* GetItemBaseItem(CSWSItem* pItem)
{
    int nBaseItem = pItem->m_nBaseItem;
    return g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
}

// Matches with the inline item accessors (GetStackSize, GetPossessor) and
// m_oidArea read into a local just before AddMoveToPointActionToFront.
// FUNCTION: SWKOTOR 0x0057B6A0
unsigned long CSWSObject::AIActionGiveItem(CSWSObjectActionNode* pNode)
{
    CSWSItem* pItem = NULL;
    if (pNode->m_nNumParams < 2) {
        return ACTION_FAILED;
    }
    unsigned long oidItem = (unsigned long)pNode->m_pParameter[0];
    int nSplit = (int)pNode->m_pParameter[2];
    unsigned long oidReceiver = (unsigned long)pNode->m_pParameter[1];
    int bStatus = (int)pNode->m_pParameter[3];
    CGameObject* pReceiver = g_pAppManager->GetServerExoApp()->GetGameObject(oidReceiver);
    if (!pReceiver) {
        return ACTION_FAILED;
    }
    if (AsSWSCreature() && !AsSWSCreature()->GetIsInUseRange(oidReceiver, 0.0f, 0)) {
        Vector vPosition(0.0f, 0.0f, 0.0f);
        int bRun = 0;
        float fRange;
        int bOne = 1;
        AsSWSCreature()->GetUseRange(oidReceiver, vPosition, fRange, 0);
        float fX = m_vPosition.x - vPosition.x;
        float fY = m_vPosition.y - vPosition.y;
        float fZ = m_vPosition.z - vPosition.z;
        if (fZ * fZ + fY * fY + fX * fX > AI_GIVE_ITEM_RUN_DISTANCE_SQ) {
            bRun = 1;
        }
        AddActionToFront(0x22, pNode->m_n6C, 3, &oidItem, 3, &oidReceiver, 1, &nSplit, 1, &bStatus);
        AddActionToFront(0x31, pNode->m_n6C, 2, &vPosition.x, 2, &vPosition.y, 2, &vPosition.z);
        AddActionToFront(0x11, pNode->m_n6C, 3, &oidReceiver, 1, &bRun, 2, &fRange, 2, &fRange, 1, &bOne);
        Vector vFace(0.0f, 0.0f, 0.0f);
        unsigned long oidArea = m_oidArea;
        AsSWSCreature()->AddMoveToPointActionToFront(pNode->m_n6C, vPosition, oidArea, oidReceiver, bRun,
                                                     fRange, 0.0f, 0, 0, 0, 0, 0, 0, vFace, 0, 0, 0);
        return ACTION_COMPLETE;
    }
    pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (!pItem) {
        return ACTION_FAILED;
    }
    if (pItem->GetBaseItem()->m_nBaseType == 0x17) {
        if (AsSWSCreature()) {
            AsSWSCreature()->RemoveGold(pItem->GetStackSize(), 1);
        }
    } else if (pItem->GetBaseItem()->m_nBaseType == 0x2a) {
        if (AsSWSCreature()) {
            AsSWSCreature()->RemovePazaakCard(pItem);
        }
    }
    int bFeedback = 1;
    if (pItem->GetBaseItem()->m_nBaseType == 0x17) {
        bFeedback = 0;
    }
    if (nSplit != -1) {
        CSWSItem* pSplit = pItem->SplitItem(nSplit);
        if (pSplit) {
            pItem = pSplit;
        }
    }
    unsigned char nType = pReceiver->m_nObjectType;
    if (nType == 5) {
        pReceiver->AsSWSCreature()->AcquireItem(&pItem, pItem->GetPossessor(), INVALID_OBJECT_ID, bFeedback);
        if (bStatus && pReceiver->AsSWSCreature()->m_bInParty) {
            g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(7, 0);
        }
        return ACTION_COMPLETE;
    }
    if (nType == 9) {
        pReceiver->AsSWSPlaceable()->AcquireItem(&pItem, pItem->GetPossessor(), bFeedback);
        return ACTION_COMPLETE;
    }
    if (nType == 6 &&
        GetItemBaseItem(pReceiver->AsSWSItem())->m_bContainer == 1) {
        pReceiver->AsSWSItem()->AcquireItem(&pItem, pItem->GetPossessor(), bFeedback);
    }
    return ACTION_COMPLETE;
}
