// CSWSObject AI action handlers and action-queue helpers (0x0057B290-0x0057CF00).
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/faction_manager.h"
#include "b/sws_player.h"

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);       // 0x0062EEB0
};

class CVirtualMachine {
public:
    int RunScriptSituation(void* pScript, unsigned long oidSelf, int bArg); // 0x005D0FD0
    void DeleteScriptSituation(void* pScript);      // 0x005D1110
};

extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern const unsigned char GENDER_MALE;             // 0x0073ECD0

// .rdata action results.
extern const unsigned long ACTION_IN_PROGRESS;      // 0x00745DBC (1)
extern const unsigned long ACTION_COMPLETE;         // 0x00745DC0 (2)
extern const unsigned long ACTION_FAILED;           // 0x00745DC4 (3)
extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_AREA_OF_EFFECT; // 0x0074663A

// Hearing range of AI sounds (.rdata 0x00747444, 1000.0f).
extern const float AI_SOUND_RANGE;

// Reputation override (-1 = off).
extern int g_nReputationOverride;                   // 0x007A2068

class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x248 - 0xf4];
    unsigned long m_oidCreator;                     // 0x248
};

// FUNCTION: SWKOTOR 0x0057A240
int CSWSObject::GetAcceptableAction(unsigned long nActionId)
{
    switch (nActionId) {
    case 1:
    case 7:
    case 9:
    case 12:
    case 15:
    case 20:
    case 21:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 46:
    case 50:
    case 54:
    case 55:
    case 56:
    case 61:
    case 63:
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0057A2B0
unsigned short CSWSObject::GetAQActionIDByID(unsigned short nActionId, int bAlternate)
{
    if (bAlternate) {
        switch (nActionId) {
        case 0xb:
            return 0x1b;
        case 0x1e:
            return 0x18;
        }
        return 0xffff;
    }
    switch (nActionId) {
    case 1:
        return 0x0;
    case 7:
        return 0x1;
    case 9:
        return 0x2;
    case 12:
        return 0x3;
    case 63:
        return 0x27;
    case 15:
        return 0x4;
    case 20:
        return 0x5;
    case 21:
        return 0x6;
    case 24:
        return 0x7;
    case 25:
        return 0x8;
    case 26:
        return 0x9;
    case 27:
        return 0xa;
    case 28:
        return 0xb;
    case 29:
        return 0xc;
    case 38:
        return 0xd;
    case 39:
        return 0xe;
    case 40:
        return 0xf;
    case 41:
        return 0x10;
    case 42:
        return 0x11;
    case 43:
        return 0x12;
    case 46:
        return 0x13;
    case 50:
        return 0x1f;
    case 56:
        return 0x21;
    case 54:
        return 0x22;
    case 55:
        return 0x23;
    case 61:
        return 0x26;
    case 30:
        return 0x24;
    }
    return 0xffff;
}

// FUNCTION: SWKOTOR 0x0057B290
unsigned long CSWSObject::AIActionPauseDialog(CSWSObjectActionNode* pNode)
{
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetObjectState94() != 1 && GetIsPaused() != 1 && bDead != 1) {
        m_bDialogPaused = 1;
        if (AsSWSCreature()) {
            AsSWSCreature()->SetAIStateFlag(4, 1);
        }
        return ACTION_COMPLETE;
    }
    SetDialogOwner(OBJECT_INVALID_ID);
    return ACTION_FAILED;
}

// FUNCTION: SWKOTOR 0x0057B320
unsigned long CSWSObject::AIActionResumeDialog(CSWSObjectActionNode* pNode)
{
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetObjectState94() != 1 && GetIsPaused() != 1 && bDead != 1) {
        m_bDialogPaused = 0;
        if (AsSWSCreature()) {
            AsSWSCreature()->ActivityManager(2);
            AsSWSCreature()->SetAIStateFlag(4, 0);
            AsSWSCreature()->SetActivity(1, 0, 0);
        }
        return ACTION_COMPLETE;
    }
    SetDialogOwner(OBJECT_INVALID_ID);
    return ACTION_FAILED;
}

// FUNCTION: SWKOTOR 0x0057B3D0
unsigned long CSWSObject::AIActionSpeakStrRef(CSWSObjectActionNode* pNode)
{
    if (AsSWSCreature()) {
        AsSWSCreature()->ActivityManager(2);
        AsSWSCreature()->SetActivity(1, 0, 0);
    }
    unsigned long nStrRef = (unsigned long)pNode->m_pParameter[0];
    unsigned long nVolume = (unsigned long)pNode->m_pParameter[1];
    g_pAppManager->GetServerExoApp()->GetSWSMessage()->ChatStrRefMessage(
        nVolume, m_idSelf, nStrRef);
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x0057B430
unsigned long CSWSObject::AIActionSpeak(CSWSObjectActionNode* pNode)
{
    CExoString sText;
    if (AsSWSCreature()) {
        AsSWSCreature()->ActivityManager(2);
        AsSWSCreature()->SetActivity(1, 0, 0);
    }
    sText = *(CExoString*)pNode->m_pParameter[0];
    unsigned long nVolume = (unsigned long)pNode->m_pParameter[1];
    {
        CExoString sName("");
        g_pAppManager->GetServerExoApp()->GetSWSMessage()->ChatMessage(
            nVolume, m_idSelf, sText, 0xffffffff, sName);
    }
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x0057B530
unsigned long CSWSObject::AIActionDoCommand(CSWSObjectActionNode* pNode)
{
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetObjectState94() != 1 && GetIsPaused() != 1 && bDead != 1) {
        g_pVirtualMachine->RunScriptSituation(pNode->m_pParameter[0], m_idSelf, 1);
        pNode->m_pParameter[0] = 0;
        return ACTION_COMPLETE;
    }
    return ACTION_FAILED;
}

// FUNCTION: SWKOTOR 0x0057C7B0
unsigned long CSWSObject::AIActionSetCommandable(CSWSObjectActionNode* pNode)
{
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetObjectState94() || GetIsPaused() || bDead) {
        return ACTION_FAILED;
    }
    m_bCommandable = (int)pNode->m_pParameter[0];
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x0057C810
int CSWSObject::AddUseObjectAction(unsigned long oidObject)
{
    if (!m_bCommandable) {
        return 0;
    }
    AddAction(0x28, 0xffff, 3, &oidObject);
    return 1;
}

// FUNCTION: SWKOTOR 0x0057CB10
int CSWSObject::AddDoCommandAction(void* pScript)
{
    if (!m_bCommandable) {
        g_pVirtualMachine->DeleteScriptSituation(pScript);
        return 0;
    }
    AddAction(0x25, 0xffff, 5, pScript);
    return 1;
}

// FUNCTION: SWKOTOR 0x0057CD90
unsigned char CSWSObject::GetAIStateReputation(unsigned long oidTarget)
{
    GetArea();
    CGameObject* pObject;
    g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject);
    if (pObject && pObject->AsSWSObject()) {
        unsigned long oidSelf = m_idSelf;
        int nReputation = pObject->AsSWSObject()->GetReputation(oidSelf, 0);
        if (nReputation >= 90) {
            return 1;
        }
        if (nReputation <= 10) {
            return 2;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0057C870
int CSWSObject::AddGiveItemActions(unsigned long oidItem, unsigned long oidGiveTo,
                                   int nStackSize, int bArg)
{
    CGameObject* pTarget = g_pAppManager->GetServerExoApp()->GetGameObject(oidGiveTo);
    if (!pTarget || !pTarget->AsSWSObject()) {
        return 0;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        CSWSCreature* pCreature = AsSWSCreature();
        CSWSCreature* pPossessor =
            g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(pItem->GetPossessor());
        if ((pPossessor && pCreature && pPossessor->m_bInParty && pCreature->m_bInParty) ||
            pItem->GetPossessor() == m_idSelf) {
            if (m_bCommandable) {
                if (nStackSize > pItem->m_nStackCount) {
                    nStackSize = pItem->m_nStackCount;
                }
                AddAction(0x22, 0xfffe, 3, &oidItem, 3, &oidGiveTo, 1, &nStackSize, 1, &bArg);
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0057CE00
unsigned long CSWSObject::AIActionBarkString(CSWSObjectActionNode* pNode)
{
    unsigned long nStrRef = (unsigned long)pNode->m_pParameter[0];
    STR_RES strRes;
    g_pTlkTable->Fetch(nStrRef, strRes, GENDER_MALE);
    CResRef cSound(strRes.cSoundResRef);
    CExoString sText(strRes.sString);
    unsigned long oidSelf = m_idSelf;
    unsigned long oidClient = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidSelf);
    g_pAppManager->GetClientExoApp()->BarkString(oidClient, sText, cSound, 0);
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x0057BAE0
unsigned long CSWSObject::AIActionTakeItem(CSWSObjectActionNode* pNode)
{
    if (pNode->m_nNumParams < 1) {
        return ACTION_FAILED;
    }
    unsigned long oidItem = (unsigned long)pNode->m_pParameter[0];
    unsigned long oidFrom = (unsigned long)pNode->m_pParameter[1];
    int bFeedback = (int)pNode->m_pParameter[2];
    CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CGameObject* pObject;
    CSWSItem* pItem = NULL;
    if (pArray->GetGameObject(oidItem, &pObject) == GAME_OBJECT_VALID && pObject) {
        pItem = pObject->AsSWSItem();
    }
    if (!pItem) {
        return ACTION_FAILED;
    }
    if (m_nObjectType == 5) {
        if (AsSWSCreature()->AcquireItem(&pItem, oidFrom, INVALID_OBJECT_ID, bFeedback) &&
            bFeedback) {
            CSWSCreature* pFrom =
                g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFrom);
            if (pFrom && pFrom->m_bInParty) {
                g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(8, 0);
            }
        }
    }
    else if (m_nObjectType == 9) {
        AsSWSPlaceable()->AcquireItem(&pItem, oidFrom, 1);
    }
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x0057C980
int CSWSObject::AddTakeItemActions(unsigned long oidItem, unsigned long oidTakeFrom, int bArg)
{
    int bRun = 0;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTakeFrom);
    if (!pGameObject) {
        return 0;
    }
    CSWSObject* pFrom = pGameObject->AsSWSObject();
    if (pFrom) {
        CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
        if (pItem && pItem->m_oidPossessor != m_idSelf && m_bCommandable) {
            if (AsSWSCreature()) {
                {
                    Vector vDiff = m_vPosition - pFrom->m_vPosition;
                    if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z > 25.0f) {
                        bRun = 1;
                    }
                }
                AsSWSCreature()->AddMoveToPointAction(
                    0xffff, pFrom->m_vPosition, pFrom->GetArea()->GetId(),
                    oidTakeFrom, bRun, 1.0f, 0, 0, 0, 0, 0, 0, 0, Vector(0.0f, 0.0f, 0.0f),
                    0, 0, 0);
            }
            AddAction(0x23, 0xffff, 3, &oidItem, 3, &oidTakeFrom, 1, &bArg);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0057CB80
int CSWSObject::GetReputation(unsigned long oidTarget, int bArg)
{
    if (g_nReputationOverride != -1) {
        return g_nReputationOverride;
    }
    int nReputation = 0;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) ==
        GAME_OBJECT_VALID) {
        CFactionManager* pFactionManager = g_pAppManager->GetServerExoApp()->GetFactionManager();
        if (pFactionManager) {
            if (pObject->m_nObjectType == OBJECT_TYPE_AREA_OF_EFFECT && pObject->AsSWSTrigger()) {
                oidTarget = pObject->AsSWSTrigger()->m_oidCreator;
            }
            if (AsSWSTrigger()) {
                unsigned long oidCreator = AsSWSTrigger()->m_oidCreator;
                CGameObject* pCreator;
                if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
                        oidCreator, &pCreator) != GAME_OBJECT_VALID) {
                    return 50;
                }
                return pCreator->AsSWSObject()->GetReputation(oidTarget, bArg);
            }
            int nFaction1;
            int nFaction2;
            if (!g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidTarget, &nFaction1)) {
                return 50;
            }
            unsigned long oidSelf = m_idSelf;
            if (!g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidSelf, &nFaction2)) {
                return 50;
            }
            if (oidTarget == m_idSelf) {
                return 100;
            }
            if (!pFactionManager->IsValidFactionId(nFaction1) &&
                pFactionManager->IsValidFactionId(nFaction2) == 1) {
                unsigned long oidSelf2 = m_idSelf;
                return pObject->AsSWSObject()->GetReputation(oidSelf2, 0);
            }
            if (AsSWSCreature()) {
                nReputation = AsSWSCreature()->GetCreatureReputation(oidTarget, nFaction1);
            }
            else if (AsSWSPlaceable() || AsSWS5C() || AsSWSDoor() || AsSWSAreaOfEffectObject()) {
                nReputation = pFactionManager->GetNPCReputation(nFaction1, nFaction2);
            }
            if (nReputation < 0) {
                nReputation = 0;
            }
            else if (nReputation > 100) {
                nReputation = 100;
            }
            return nReputation;
        }
    }
    return 50;
}

// FUNCTION: SWKOTOR 0x0057CF00
unsigned long CSWSObject::AIActionPlaySound(CSWSObjectActionNode* pNode)
{
    CExoString sSound;
    sSound = *(CExoString*)pNode->m_pParameter[0];
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
    void* pPos = pPlayers->GetHeadPos();
    while (pPos) {
        CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayers->GetAtPos(pPos);
        if (pPlayer->GetGameObject()) {
            CSWSObject* pObject = (CSWSObject*)pPlayer->GetGameObject();
            if (pObject) {
                CSWSArea* pArea = pObject->GetArea();
                Vector vPosition = pObject->m_vPosition;
                if (pArea == GetArea()) {
                    float fZ = vPosition.z - m_vPosition.z;
                    float fY = vPosition.y - m_vPosition.y;
                    float fX = vPosition.x - m_vPosition.x;
                    if (fX * fX + fY * fY + fZ * fZ <=
                            AI_SOUND_RANGE * AI_SOUND_RANGE &&
                        pMessage) {
                        pMessage->SendServerToPlayerAIActionPlaySound(pPlayer->GetPlayerId(),
                                                                     m_idSelf, sSound);
                    }
                }
            }
        }
        pPlayers->GetNext(pPos);
    }
    return ACTION_COMPLETE;
}
