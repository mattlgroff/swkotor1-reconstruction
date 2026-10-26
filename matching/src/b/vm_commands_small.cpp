// CSWVirtualMachineCommands: small leaf handlers left over from rounds
// s3-s5 (creature size and rating, music tracks, party removal, vectors,
// attackers), 0x00536C60-0x00548BE0.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/party_table.h"
#include "b/global_variable_table.h"
#include "b/game_effect.h"
#include "b/sws_player.h"
#include "b/sws_area_map.h"
#include <stdlib.h>

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const int EFFECT_TYPE_LINK;                          // 0x0074671C

// FUNCTION: SWKOTOR 0x00536C60
int CSWVirtualMachineCommands::ExecuteCommandGetChallengeRating(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    float fRating = 0.0f;
    if (pCreature && pCreature->m_pStats) {
        fRating = pCreature->m_pStats->m_fChallengeRating;
    }
    return g_pVirtualMachine->StackPushFloat(fRating) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536E40
int CSWVirtualMachineCommands::ExecuteCommandGetCommandable(int nCommandId, int nParameters)
{
    unsigned long oidTarget = m_oidObjectRunScript;
    if (nParameters >= 1 && !g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    int bCommandable;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID) {
        bCommandable = pObject->m_bCommandable;
    }
    return g_pVirtualMachine->StackPushInteger(bCommandable) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536F80
int CSWVirtualMachineCommands::ExecuteCommandGetCreatureSize(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nSize = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        nSize = pCreature->m_nCreatureSize;
    }
    return g_pVirtualMachine->StackPushInteger(nSize) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00537300
int CSWVirtualMachineCommands::ExecuteCommandMusicBackgroundGetDayTrack(int nCommandId, int nParameters)
{
    unsigned long oidArea;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        nResult = pArea->m_pAmbientSound->m_nMusicDay;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005404F0
int CSWVirtualMachineCommands::ExecuteCommandActionPickUpItem(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->AddPickUpItemActions(oidItem, 0x7f000000, 0xff);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00541640
int CSWVirtualMachineCommands::ExecuteCommandPrintInteger(int nCommandId, int nParameters)
{
    int nInteger;
    int nArg;
    if (!g_pVirtualMachine->StackPopInteger(&nInteger)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&nArg)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00541B40
int CSWVirtualMachineCommands::ExecuteCommandRemoveAvailableNPC(int nCommandId, int nParameters)
{
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bResult = g_pAppManager->GetServerExoApp()->GetPartyTable()->RemoveAvailableNPC(nNPC);
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00541BA0
int CSWVirtualMachineCommands::ExecuteCommandRemoveFromParty(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature && pCreature->m_bIsPC) {
        CSWSFaction* pFaction = pCreature->GetFaction();
        if (pFaction) {
            pFaction->RemoveMember(oidCreature);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00546190
int CSWVirtualMachineCommands::ExecuteCommandGetFirstAttacker(int nCommandId, int nParameters)
{
    unsigned long oidCreature = m_oidObjectRunScript;
    unsigned long oidResult = OBJECT_INVALID_ID;
    if (nParameters >= 1 && !g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        unsigned long oidAttacker;
        if (nCommandId == 0x2d7) {
            oidAttacker = pCreature->GetFirstAttacker();
        } else if (nCommandId == 0x2d8) {
            oidAttacker = pCreature->GetNextAttacker();
        } else {
            goto done;
        }
        if (oidAttacker == 0x7f000000) {
            oidResult = OBJECT_INVALID_ID;
        }
    }
done:
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Encounter object: only the fields the encounter queries read.
class CSWSEncounter {
public:
    char pad_0x0[0x238];
    int m_bActive;                                          // 0x238
    char pad_0x23c[0x248 - 0x23c];
    int m_nDifficulty;                                      // 0x248
    char pad_0x24c[0x2c4 - 0x24c];
    int m_nSpawnsMax;                                       // 0x2c4
    int m_nSpawnsCurrent;                                   // 0x2c8
};

extern const int DAMAGE_TYPE_MAX;                           // 0x00746878 (name guessed)

float Magnitude(const Vector& v);                           // 0x004AB100

// FUNCTION: SWKOTOR 0x00537250
int CSWVirtualMachineCommands::ExecuteCommandGetDamageDealtByType(int nCommandId, int nParameters)
{
    int nResult = 0;
    CSWSObject* pObject;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    int nDamageType;
    if (!g_pVirtualMachine->StackPopInteger(&nDamageType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pObject && nDamageType > 0 && nDamageType < DAMAGE_TYPE_MAX) {
        unsigned char nType = pObject->m_nObjectType;
        if (nType == 5 || nType == 9 || nType == 10) {
            nResult = pObject->GetEffectCount(nDamageType);
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00537E30
int CSWVirtualMachineCommands::ExecuteCommandGetEncounterActive(int nCommandId, int nParameters)
{
    unsigned long oidEncounter = 0x7f000000;
    if (nParameters >= 1 && !g_pVirtualMachine->StackPopObject(&oidEncounter)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult;
    CSWSEncounter* pEncounter = g_pAppManager->GetServerExoApp()->GetEncounterByGameObjectID(oidEncounter);
    if (pEncounter) {
        if (nCommandId == 0x114) {
            nResult = pEncounter->m_bActive;
        } else if (nCommandId == 0x129) {
            nResult = pEncounter->m_nDifficulty;
        } else if (nCommandId == 0x116) {
            nResult = pEncounter->m_nSpawnsMax;
        } else if (nCommandId == 0x118) {
            nResult = pEncounter->m_nSpawnsCurrent;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005400C0
int CSWVirtualMachineCommands::ExecuteCommandMusicBattlePlay(int nCommandId, int nParameters)
{
    unsigned long oidArea;
    int nTrack = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1 && !g_pVirtualMachine->StackPopInteger(&nTrack)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        switch (nCommandId) {
        case 0x1ae:
            pArea->m_pAmbientSound->PlayBattleMusic(1);
            break;
        case 0x1af:
            pArea->m_pAmbientSound->PlayBattleMusic(0);
            break;
        case 0x1b0:
            pArea->m_pAmbientSound->SetBattleMusicTrack(nTrack);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00540260
int CSWVirtualMachineCommands::ExecuteCommandActionOpenDoor(int nCommandId, int nParameters)
{
    unsigned long oidDoor;
    if (!g_pVirtualMachine->StackPopObject(&oidDoor)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        ((CSWSObject*)pObject)->GetArea() && pObject->AsSWSObject()->m_bCommandable == 1) {
        pObject->AsSWSObject()->AddOpenDoorAction(oidDoor, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005435C0
int CSWVirtualMachineCommands::ExecuteCommandMagicalEffect(int nCommandId, int nParameters)
{
    CGameEffect* pEffect;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    switch (nCommandId) {
    case 0x70:
        pEffect->m_nSubType = (pEffect->m_nSubType & 0xffef) | 0x8;
        break;
    case 0x71:
        pEffect->m_nSubType = (pEffect->m_nSubType & 0xfff7) | 0x10;
        break;
    case 0x72:
        pEffect->m_nSubType |= 0x18;
        break;
    }
    if (pEffect->m_nType == EFFECT_TYPE_LINK) {
        pEffect->UpdateLinked();
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    operator delete(pEffect);
    return 0;
}

// FUNCTION: SWKOTOR 0x00543ED0
int CSWVirtualMachineCommands::ExecuteCommandSpawnAvailableNPC(int nCommandId, int nParameters)
{
    int nNPC;
    CScriptLocation* pLocation;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidNPC = g_pAppManager->GetServerExoApp()->GetPartyTable()->SpawnNPC(
        nNPC, 1, (Vector*)pLocation->m_vPosition, (Vector*)pLocation->m_vOrientation, 1);
    if (!g_pVirtualMachine->StackPushObject(oidNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pLocation;
    return 0;
}

// FUNCTION: SWKOTOR 0x00545120
int CSWVirtualMachineCommands::ExecuteCommandVector(int nCommandId, int nParameters)
{
    Vector v(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopFloat(&v.x) ||
        !g_pVirtualMachine->StackPopFloat(&v.y) ||
        !g_pVirtualMachine->StackPopFloat(&v.z)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return g_pVirtualMachine->StackPushVector(v) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00545320
int CSWVirtualMachineCommands::ExecuteCommandVectorMagnitude(int nCommandId, int nParameters)
{
    Vector v(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopVector(&v)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    float fMagnitude = Magnitude(v);
    return g_pVirtualMachine->StackPushFloat(fMagnitude) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

class CScriptTalent {
public:
    ~CScriptTalent();                                       // 0x004015A0 (shared empty function)

    int m_nType;                                            // 0x00
    int m_nId;                                              // 0x04
};

class CSWCArea {
public:
    void SetFogColor(const Vector& vColor);                 // 0x00605090 (name guessed)
};

class CSWSEncounterView : public CSWSEncounter {
public:
    void SetActive(int bActive);                            // 0x00593C50 (name guessed)
    void SetDifficulty(int nDifficulty);                    // 0x00590A60 (name guessed)
};

// FUNCTION: SWKOTOR 0x00536EC0
int CSWVirtualMachineCommands::ExecuteCommandGetCreatureHasTalent(int nCommandId, int nParameters)
{
    unsigned long oidCreature = 0x7f000000;
    int bResult = 0;
    CScriptTalent* pTalent;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_TALENT, (void**)&pTalent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1 && !g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        bResult = pStats->GetHasTalent(pTalent->m_nType, pTalent->m_nId);
    }
    if (!g_pVirtualMachine->StackPushInteger(bResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pTalent;
    return 0;
}

// FUNCTION: SWKOTOR 0x00541C00
int CSWVirtualMachineCommands::ExecuteCommandRemovePartyMember(int nCommandId, int nParameters)
{
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWPartyTable* pParty = pServer->GetPartyTable();
    unsigned long oidNPC = pParty->GetNPCObject(nNPC, 0, 1);
    int bResult = pParty->RemoveMember(nNPC);
    if (bResult) {
        CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
        CSWCCreature* pClientCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(oidNPC));
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oidNPC);
        if (pClientCreature) {
            pClientCreature->SetPartyMember(0);
        }
        if (pCreature) {
            pCreature->SetInParty(0, 1);
        }
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00542410
int CSWVirtualMachineCommands::ExecuteCommandSetAreaTransitionBMP(int nCommandId, int nParameters)
{
    int nPredefined;
    CExoString sCustom;
    if (!g_pVirtualMachine->StackPopInteger(&nPredefined)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sCustom)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(m_oidObjectRunScript);
    if (pPlayer) {
        pPlayer->SetAreaTransitionBMP(nPredefined, sCustom);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00542760
int CSWVirtualMachineCommands::ExecuteCommandSetEncounterActive(int nCommandId, int nParameters)
{
    int nValue;
    unsigned long oidEncounter = 0x7f000000;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1 && !g_pVirtualMachine->StackPopObject(&oidEncounter)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSEncounterView* pEncounter =
        (CSWSEncounterView*)g_pAppManager->GetServerExoApp()->GetEncounterByGameObjectID(oidEncounter);
    if (pEncounter) {
        if (nCommandId == 0x115) {
            pEncounter->SetActive(nValue);
            return 0;
        }
        if (nCommandId == 0x128) {
            pEncounter->SetDifficulty(nValue);
            return 0;
        }
        if (nCommandId == 0x117) {
            pEncounter->m_nSpawnsMax = nValue;
            return 0;
        }
        if (nCommandId == 0x119) {
            pEncounter->m_nSpawnsCurrent = nValue;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00547240
int CSWVirtualMachineCommands::ExecuteCommandRevealMap(int nCommandId, int nParameters)
{
    Vector vPoint(0.0f, 0.0f, 0.0f);
    int nRadius;
    if (!g_pVirtualMachine->StackPopVector(&vPoint)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nRadius)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule && pModule->m_pAreaMap) {
        if (nRadius != -1) {
            pModule->m_pAreaMap->SetWorldPointExplored(vPoint, nRadius);
        } else {
            pModule->m_pAreaMap->SetAllExplored();
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005478B0
int CSWVirtualMachineCommands::ExecuteCommandSetAreaFogColor(int nCommandId, int nParameters)
{
    unsigned long oidArea;
    Vector vColor(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&vColor.x)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&vColor.y)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&vColor.z)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        pArea->SetSunFogColor(vColor);
        CSWCArea* pClientArea = pArea->GetClientArea();
        if (pClientArea) {
            pClientArea->SetFogColor(vColor);
        }
    }
    return 0;
}

class CSWGuiInGame {
public:
    void ShowPartySelection(const CExoString& sExitScript, int bArg,
                            int nForceNPC1, int nForceNPC2); // 0x0062DD20 (name guessed)
};

// FUNCTION: SWKOTOR 0x00543670
int CSWVirtualMachineCommands::ExecuteCommandSetTime(int nCommandId, int nParameters)
{
    int nHour;
    int nMinute;
    int nSecond;
    int nMillisecond;
    if (!g_pVirtualMachine->StackPopInteger(&nHour)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nMinute)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nSecond)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nMillisecond)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->AdvanceToTime(nHour, nMinute, nSecond, nMillisecond);
    unsigned long nCalendarDay;
    unsigned long nTimeOfDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    g_pAppManager->GetServerExoApp()->GetModule()->UpdateTime(nCalendarDay, nTimeOfDay, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x005438B0
int CSWVirtualMachineCommands::ExecuteCommandShowPartySelectionGUI(int nCommandId, int nParameters)
{
    int nForceNPC2;
    int nForceNPC1;
    CExoString sExitScript;
    if (!g_pVirtualMachine->StackPopString(&sExitScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters == 3) {
        if (!g_pVirtualMachine->StackPopInteger(&nForceNPC1)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (!g_pVirtualMachine->StackPopInteger(&nForceNPC2)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowPartySelection(sExitScript, 1, nForceNPC1, nForceNPC2);
    return 0;
}

// FUNCTION: SWKOTOR 0x00544830
int CSWVirtualMachineCommands::ExecuteCommandActionSurrenderToEnemies(int nCommandId, int nParameters)
{
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        pObjectArray->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature && !pCreature->m_pStats->m_bIsPC) {
            if (nCommandId == 0x17b) {
                pCreature->AddAction(0x41, 0xffff, 0, NULL);
            } else if (nCommandId == 0x1dc) {
                pCreature->Surrender(0);
            } else if (nCommandId == 0x2fa) {
                pCreature->Surrender(1);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00545390
int CSWVirtualMachineCommands::ExecuteCommandVectorNormalize(int nCommandId, int nParameters)
{
    Vector v(0.0f, 0.0f, 0.0f);
    Vector vResult(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopVector(&v)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!(v == Vector(0.0f, 0.0f, 0.0f))) {
        vResult = Normalize(v);
    }
    return g_pVirtualMachine->StackPushVector(vResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005458D0
int CSWVirtualMachineCommands::ExecuteCommandGetIsLinkImmune(int nCommandId, int nParameters)
{
    int bImmune = 0;
    unsigned long oidTarget;
    CGameEffect* pEffect;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        bImmune = pStats->GetIsLinkImmune(pEffect);
    }
    if (!g_pVirtualMachine->StackPushInteger(bImmune)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

extern const char g_sEmpty[];                               // 0x0073D71C

// FUNCTION: SWKOTOR 0x00538ED0
int CSWVirtualMachineCommands::ExecuteCommandGetGlobalString(int nCommandId, int nParameters)
{
    CExoString sName;
    CExoString sValue;
    if (!g_pVirtualMachine->StackPopString(&sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->GetValueString(sName, &sValue);
    if (!g_pVirtualMachine->StackPushString(sValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00540170
int CSWVirtualMachineCommands::ExecuteCommandObjectToString(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sResult;
    sResult.Format("%x", oidObject);
    bool bFailed;
    {
        CExoString sValue(sResult.CStr());
        bFailed = !g_pVirtualMachine->StackPushString(sValue);
    }
    if (bFailed) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00545E20
int CSWVirtualMachineCommands::ExecuteCommandGivePlotXP(int nCommandId, int nParameters)
{
    int nPercentage;
    CExoString sPlotName(g_sEmpty);
    if (!g_pVirtualMachine->StackPopString(&sPlotName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nPercentage)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->GivePlotXP(sPlotName, nPercentage);
    return 0;
}

class CExoAliasList {
public:
    CExoString* GetAliasPath(const CExoString& sAlias, int nArg); // 0x005E6890
};

class CExoBase {
public:
    void* m_p0;                                             // 0x00
    void* m_pcExoTimers;                                    // 0x04
    void* m_pcExoDebug;                                     // 0x08
    CExoAliasList* m_pcExoAliasList;                        // 0x0c
};

extern CExoBase* g_pExoBase;                                // 0x007A39E0

// FUNCTION: SWKOTOR 0x00542C60
int CSWVirtualMachineCommands::ExecuteCommandSetGlobalString(int nCommandId, int nParameters)
{
    CExoString sName;
    CExoString sValue;
    if (!g_pVirtualMachine->StackPopString(&sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetGlobalVariableTable()->SetValueString(sName, &sValue);
    return 0;
}

// FUNCTION: SWKOTOR 0x0053FFB0
int CSWVirtualMachineCommands::ExecuteCommandMusicBackgroundPlay(int nCommandId, int nParameters)
{
    unsigned long oidArea;
    int nValue = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1 && !g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        switch (nCommandId) {
        case 0x1a9:
            pArea->m_pAmbientSound->PlayMusic(1);
            break;
        case 0x1aa:
            pArea->m_pAmbientSound->PlayMusic(0);
            break;
        case 0x1ab:
            pArea->m_pAmbientSound->SetMusicDelay(nValue);
            break;
        case 0x1ac:
            pArea->m_pAmbientSound->SetMusicDayTrack(nValue);
            break;
        case 0x1ad:
            pArea->m_pAmbientSound->SetMusicNightTrack(nValue);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00547AB0
int CSWVirtualMachineCommands::ExecuteCommandGetIsLiveContentAvailable(int nCommandId, int nParameters)
{
    int bAvailable = 0;
    CExoString sAlias;
    int nPackage;
    if (!g_pVirtualMachine->StackPopInteger(&nPackage)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nPackage == 1) {
        bAvailable = 1;
    } else if (nPackage >= 2 && nPackage <= 6) {
        sAlias.Format("LIVE%d", nPackage);
        bAvailable = g_pExoBase->m_pcExoAliasList->GetAliasPath(sAlias, 0)->GetLength() != 0;
    }
    if (!g_pVirtualMachine->StackPushInteger(bAvailable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// Applied effects of a server object (CSWSObject keeps its members out of
// the shared header).
struct CSWSObjectEffectsView {
    char pad_0x0[0x124];
    CExoArrayList<CGameEffect*> m_appliedEffects;           // 0x124
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long nCaller,
                          unsigned long nObject, unsigned long nEventId, void* pEventData); // 0x004B08D0
};

extern const unsigned long EVENT_REMOVE_EFFECT;             // 0x007441EC (name guessed)

// FUNCTION: SWKOTOR 0x00542530
int CSWVirtualMachineCommands::ExecuteCommandSetCameraFacing(int nCommandId, int nParameters)
{
    float fDirection;
    if (!g_pVirtualMachine->StackPopFloat(&fDirection)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    fDirection = fDirection * 57.29578f;
    int nCameraMode = 1;
    float fDistance = 0.0f;
    float fPitch = 0.0f;
    int nTransitionType = 0;
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_bCommandable) {
        pObject->AsSWSCreature()->AddAction(0x16, 0xffff, 1, &nCameraMode, 2, &fDirection, 2, &fDistance,
                                            2, &fPitch, 1, &nTransitionType);
    }
    return 0;
}

// NONMATCH: the original materialises the lookup result as an int
// (sub/neg/sbb/inc) and compares it with 1; cl folds `bValid == 1` here.
// FUNCTION: SWKOTOR 0x00544270
int CSWVirtualMachineCommands::ExecuteCommandActionSpeakStringByStrRef(int nCommandId, int nParameters)
{
    int nStrRef;
    int nTalkVolume;
    if (!g_pVirtualMachine->StackPopInteger(&nStrRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nTalkVolume)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (m_bValidObjectRunScript == 1) {
        CGameObject* pObject;
        int bValid = g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID;
        if (bValid == 1) {
            int nVolume = 8;
            if (nTalkVolume == 1) {
                nVolume = 0xa;
            } else if (nTalkVolume == 2) {
                nVolume = 9;
            }
            if (pObject->AsSWSObject() && pObject->AsSWSObject()->m_bCommandable) {
                pObject->AsSWSObject()->AddAction(0x21, 0xffff, 1, &nStrRef, 1, &nVolume);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00544710
int CSWVirtualMachineCommands::ExecuteCommandStringToInt(int nCommandId, int nParameters)
{
    CExoString sString;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nCommandId == 0xe8) {
        if (!g_pVirtualMachine->StackPushInteger(atoi(sString.CStr()))) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else {
        if (!g_pVirtualMachine->StackPushFloat((float)atof(sString.CStr()))) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0054AE40
int CSWVirtualMachineCommands::ExecuteCommandRemoveEffect(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidCreature);
    if (pObject && pObject->AsSWSObject()) {
        CExoArrayList<CGameEffect*>* pList = &((CSWSObjectEffectsView*)pObject->AsSWSObject())->m_appliedEffects;
        CGameEffect* pMatch = NULL;
        if (pList && pList->num > 0) {
            for (unsigned long i = 0; i < (unsigned long)pList->num; i++) {
                CGameEffect* pApplied = pList->element[i];
                if (pApplied->m_nID == pEffect->m_nID) {
                    pApplied->m_n20 = 0;
                    pMatch = pApplied;
                }
            }
            if (pMatch) {
                g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                    0, 0, m_oidObjectRunScript, oidCreature, EVENT_REMOVE_EFFECT, pMatch);
            }
        }
    }
    delete pEffect;
    return 0;
}

// GetClassByPosition, GetLevelByPosition and GetLevelByClass share this
// handler.
// NONMATCH: the original places the shared pop-failure block after the
// first pop (the second pop jumps back to it); the third GetClass call loads
// the stats pointer before its argument.
// FUNCTION: SWKOTOR 0x00536CE0
int CSWVirtualMachineCommands::ExecuteCommandGetClassByPosition(int nCommandId, int nParameters)
{
    int nResult = 0;
    int nValue;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        if (nCommandId == 0x155) {
            nResult = 0xff;
            if (nValue >= 1 && nValue <= pCreature->m_pStats->m_nNumMultiClasses) {
                pCreature->m_pStats->GetClass((unsigned char)(nValue - 1));
                if (pCreature->m_pStats->GetClass((unsigned char)(nValue - 1)) <= 0x20) {
                    nResult = pCreature->m_pStats->GetClass((unsigned char)(nValue - 1));
                }
            }
        } else if (nCommandId == 0x156) {
            if (nValue >= 1 && nValue <= pCreature->m_pStats->m_nNumMultiClasses) {
                nResult = pCreature->m_pStats->GetClassLevel((unsigned char)(nValue - 1), 0);
            }
        } else if (nCommandId == 0x157) {
            for (int i = 0; i < 2; i++) {
                if (pCreature->m_pStats->GetClass((unsigned char)i) == nValue) {
                    nResult = pCreature->m_pStats->GetClassLevel((unsigned char)i, 0);
                }
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// NONMATCH: register choice; the original loads the 2DA pointer into eax
// and re-takes the column temporary's address (lea) for the push.
// FUNCTION: SWKOTOR 0x00546F90
int CSWVirtualMachineCommands::ExecuteCommandDuplicateHeadAppearance(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    unsigned long oidSource;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidSource)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    CSWSCreature* pSource = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSource);
    if (pTarget && pSource) {
        int nHead;
        if (pSource->m_cAppearance.m_n1C) {
            g_pRules->m_p2DArrays->GetAppearanceTable()->GetINTEntry(pSource->m_cAppearance.m_nAppearance,
                                                                     CExoString("BACKUPHEAD"), &nHead);
        } else {
            g_pRules->m_p2DArrays->GetAppearanceTable()->GetINTEntry(pSource->m_cAppearance.m_nAppearance,
                                                                     CExoString("NORMALHEAD"), &nHead);
        }
        pTarget->m_cAppearance.m_n20 = (unsigned char)nHead;
    }
    return 0;
}
