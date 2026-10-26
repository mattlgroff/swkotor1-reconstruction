// CServerExoApp facade members (0x004AE6B0-0x004AF0E0).
#include "i/server_exo_app.h"

// FUNCTION: SWKOTOR 0x004AEC00
CServerExoApp::CServerExoApp()
{
    m_pcExoAppInternal = new CServerExoAppInternal;
}

// FUNCTION: SWKOTOR 0x004AEC70
CServerExoApp::~CServerExoApp()
{
    if (m_pcExoAppInternal) {
        delete m_pcExoAppInternal;
    }
}

// FUNCTION: SWKOTOR 0x004AE700
int CServerExoApp::LoadCharacterStart(unsigned char nType, CSWSPlayer* pPlayer, CResRef cResRef,
                                      void* pData, unsigned long nSize)
{
    return m_pcExoAppInternal->LoadCharacterStart(nType, pPlayer, cResRef, pData, nSize);
}

// FUNCTION: SWKOTOR 0x004AEA50
float CServerExoApp::ComputeHeight(unsigned long oidArea, float fX, float fY)
{
    if (m_pcExoAppInternal->GetModule()) {
        CSWSArea* pArea = m_pcExoAppInternal->GetModule()->GetArea();
        if (pArea) {
            return pArea->ComputeHeight(fX, fY, 0);
        }
    }
    return g_fZero;
}

// FUNCTION: SWKOTOR 0x004AEAA0
void CServerExoApp::UpdatePlayerPosition(unsigned long oid, const Vector& vPosition, const Vector& vOrientation)
{
    unsigned long oidServer = m_pcExoAppInternal->ClientToServerObjectID(oid);
    m_pcExoAppInternal->UpdatePlayerPosition(oidServer, vPosition, vOrientation);
}

// FUNCTION: SWKOTOR 0x004AEAF0
int CServerExoApp::GetRoomIndex(unsigned long oidArea, float fX, float fY)
{
    int nRoom = -1;
    if (m_pcExoAppInternal->GetModule()) {
        CSWSArea* pArea = m_pcExoAppInternal->GetModule()->GetArea();
        if (pArea) {
            nRoom = pArea->GetRoomIndex(fX, fY);
        }
    }
    return nRoom;
}

// FUNCTION: SWKOTOR 0x004AEB70
Vector CServerExoApp::GetNormalOfPoint(const Vector& vPoint)
{
    return m_pcExoAppInternal->GetNormalOfPoint(vPoint);
}

// FUNCTION: SWKOTOR 0x004AECA0
void* CServerExoApp::GetModuleDescription()
{
    if (m_pcExoAppInternal->GetModule()) {
        return m_pcExoAppInternal->GetModule()->m_cDescription;
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004AEE40
int CServerExoApp::IsInjured(unsigned long oid)
{
    return m_pcExoAppInternal->IsInjured(m_pcExoAppInternal->ClientToServerObjectID(oid));
}

// NONMATCH (15): the original reloads oidCreature from its stack slot for the
// else branch and the ClientToServerObjectID call; cl caches it in ebx (push
// ebx) and schedules the six float pushes differently. Tried: ternary, inline
// wrappers, address-taken param, Vector-by-value params.
// FUNCTION: SWKOTOR 0x004AEE80
int CServerExoApp::TestDirectLine(unsigned long oidCreature, unsigned long oidArea,
                                  float fX1, float fY1, float fX2, float fY2, float fZ1, float fZ2)
{
    CSWSArea* pArea;
    if (m_pcExoAppInternal->GetModule()) {
        pArea = m_pcExoAppInternal->GetModule()->GetArea();
    }
    else {
        pArea = (CSWSArea*)oidCreature;
    }
    CSWSCreature* pCreature = m_pcExoAppInternal->GetCreatureByGameObjectID(
        m_pcExoAppInternal->ClientToServerObjectID(oidCreature));
    if (pCreature && pArea) {
        pArea->m_pPathfindInformation = pCreature->m_pPathfindInformation;
        return pArea->TestDirectLine(fX1, fY1, fX2, fY2, fZ1, fZ2);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004AEF10
CPathfindInformation* CServerExoApp::GetCreaturePathfindInformation(unsigned long oid)
{
    CSWSCreature* pCreature = m_pcExoAppInternal->GetCreatureByGameObjectID(
        m_pcExoAppInternal->ClientToServerObjectID(oid));
    if (pCreature) {
        return pCreature->m_pPathfindInformation;
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004AEF40
void CServerExoApp::ClearAllActions(unsigned long oid)
{
    CSWSCreature* pCreature = m_pcExoAppInternal->GetCreatureByGameObjectID(oid);
    if (pCreature) {
        pCreature->ClearAllActions(1);
        pCreature->m_pcCombatRound->RemoveAllActions();
        pCreature->m_pcCombatRound->m_oidNewAttackTarget = 0x7f000000;
        pCreature->m_oid50c = 0x7f000000;
        pCreature->m_oid504 = 0x7f000000;
        pCreature->m_oid524 = 0x7f000000;
        pCreature->m_oid528 = 0x7f000000;
    }
}

// FUNCTION: SWKOTOR 0x004AEFA0
void CServerExoApp::ClearAllQueuedCombatActions(unsigned long oid)
{
    CSWSCreature* pCreature = m_pcExoAppInternal->GetCreatureByGameObjectID(oid);
    if (pCreature) {
        pCreature->m_pcCombatRound->RemoveAllActions();
    }
}

// FUNCTION: SWKOTOR 0x004AE6B0
CSWSModule* CServerExoApp::GetModule()
{
    return m_pcExoAppInternal->GetModule();
}

// FUNCTION: SWKOTOR 0x004AE6E0
int CServerExoApp::SaveGame(unsigned long nSlot, CExoString& sName, CExoString& sDesc)
{
    return m_pcExoAppInternal->SaveGame(nSlot, sName, sDesc);
}

// FUNCTION: SWKOTOR 0x004AE6F0
int CServerExoApp::LoadGame(unsigned long nSlot, CExoString& sName)
{
    return m_pcExoAppInternal->LoadGame(nSlot, sName);
}

// FUNCTION: SWKOTOR 0x004AE740
int CServerExoApp::LoadCharacterFinish(CSWSPlayer* pPlayer, int bArg)
{
    return m_pcExoAppInternal->LoadCharacterFinish(pPlayer, bArg);
}

// FUNCTION: SWKOTOR 0x004AE750
CGameObject* CServerExoApp::GetGameObject(unsigned long oid)
{
    return m_pcExoAppInternal->GetGameObject(oid);
}

// FUNCTION: SWKOTOR 0x004AE760
CSWSItem* CServerExoApp::GetItemByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetItemByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE770
CSWSCreature* CServerExoApp::GetCreatureByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetCreatureByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE780
CSWSArea* CServerExoApp::GetAreaByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetAreaByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE790
CSWSTrigger* CServerExoApp::GetTriggerByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetTriggerByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE7A0
CSWSPlaceable* CServerExoApp::GetPlaceableByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetPlaceableByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE7B0
CSWSStore* CServerExoApp::GetStoreByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetStoreByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE7C0
CSWSDoor* CServerExoApp::GetDoorByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetDoorByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE7D0
CSWSAreaOfEffectObject* CServerExoApp::GetAreaOfEffectByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetAreaOfEffectByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE7E0
CSWSWaypoint* CServerExoApp::GetWaypointByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetWaypointByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE7F0
CSWSEncounter* CServerExoApp::GetEncounterByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetEncounterByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE800
CSWSSoundObject* CServerExoApp::GetSoundObjectByGameObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetSoundObjectByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE810
int CServerExoApp::GetPlayerLanguage(unsigned long nPlayerId)
{
    return m_pcExoAppInternal->GetPlayerLanguage(nPlayerId);
}

// FUNCTION: SWKOTOR 0x004AE820
int CServerExoApp::GetModuleLanguage()
{
    return m_pcExoAppInternal->GetModuleLanguage();
}

// FUNCTION: SWKOTOR 0x004AE830
CWorldTimer* CServerExoApp::GetActiveTimer(unsigned long oid)
{
    return m_pcExoAppInternal->GetActiveTimer(oid);
}

// FUNCTION: SWKOTOR 0x004AE850
int CServerExoApp::Initialize()
{
    return m_pcExoAppInternal->Initialize();
}

// FUNCTION: SWKOTOR 0x004AE860
int CServerExoApp::MainLoop()
{
    return m_pcExoAppInternal->MainLoop();
}

// FUNCTION: SWKOTOR 0x004AE890
void CServerExoApp::Shutdown(int bExit, int bShutdownNetwork)
{
    m_pcExoAppInternal->Shutdown(bExit, bShutdownNetwork);
}

// FUNCTION: SWKOTOR 0x004AE8A0
int CServerExoApp::UnloadModule()
{
    return m_pcExoAppInternal->UnloadModule();
}

// FUNCTION: SWKOTOR 0x004AE8B0
int CServerExoApp::LoadModule(CExoString& sModule, int bLoadFromSave)
{
    return m_pcExoAppInternal->LoadModule(sModule, bLoadFromSave);
}

// FUNCTION: SWKOTOR 0x004AE8C0
int CServerExoApp::RunModule()
{
    return m_pcExoAppInternal->RunModule();
}

// FUNCTION: SWKOTOR 0x004AE8F0
int CServerExoApp::StartServices(CExoString& sModule, int a, int b)
{
    return m_pcExoAppInternal->StartServices(sModule, a, b);
}

// FUNCTION: SWKOTOR 0x004AE900
void CServerExoApp::StopServices()
{
    m_pcExoAppInternal->StopServices();
}

// FUNCTION: SWKOTOR 0x004AE910
CSWSClient* CServerExoApp::GetClientObjectByObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->GetClientObjectByObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AE920
CSWSClient* CServerExoApp::GetClientObjectByPlayerID(unsigned long nPlayerId, unsigned char b)
{
    return m_pcExoAppInternal->GetClientObjectByPlayerID(nPlayerId, b);
}

// FUNCTION: SWKOTOR 0x004AE930
void CServerExoApp::InitiateModuleForPlayer(void* pPlayer)
{
    m_pcExoAppInternal->InitiateModuleForPlayer(pPlayer);
}

// FUNCTION: SWKOTOR 0x004AE940
void CServerExoApp::MovePlayerToArea(void* pPlayer)
{
    m_pcExoAppInternal->MovePlayerToArea(pPlayer);
}

// FUNCTION: SWKOTOR 0x004AE950
int CServerExoApp::GetFactionOfObject(unsigned long oid, int* pFaction)
{
    return m_pcExoAppInternal->GetFactionOfObject(oid, pFaction);
}

// FUNCTION: SWKOTOR 0x004AE960
void CServerExoApp::TogglePauseState(unsigned char nState)
{
    m_pcExoAppInternal->TogglePauseState(nState);
}

// FUNCTION: SWKOTOR 0x004AE970
void CServerExoApp::SkipPastDialog(unsigned long oid)
{
    m_pcExoAppInternal->SkipPastDialog(oid);
}

// FUNCTION: SWKOTOR 0x004AE980
int CServerExoApp::GetPauseState(unsigned char nMask)
{
    return m_pcExoAppInternal->GetPauseState(nMask);
}

// FUNCTION: SWKOTOR 0x004AE990
unsigned char CServerExoApp::GetActivePauseState()
{
    return m_pcExoAppInternal->GetActivePauseState();
}

// FUNCTION: SWKOTOR 0x004AE9A0
void CServerExoApp::SetPauseState(unsigned char nState, int bPause)
{
    m_pcExoAppInternal->SetPauseState(nState, bPause);
}

// FUNCTION: SWKOTOR 0x004AE9B0
void CServerExoApp::AddToExclusionList(unsigned long oid, unsigned char nList)
{
    return m_pcExoAppInternal->AddToExclusionList(oid, nList);
}

// FUNCTION: SWKOTOR 0x004AE9C0
void CServerExoApp::RemoveFromExclusionList(unsigned long oid, unsigned char nList)
{
    return m_pcExoAppInternal->RemoveFromExclusionList(oid, nList);
}

// FUNCTION: SWKOTOR 0x004AE9D0
CExoLinkedList<unsigned long>* CServerExoApp::GetActiveExclusionList()
{
    return m_pcExoAppInternal->GetActiveExclusionList();
}

// FUNCTION: SWKOTOR 0x004AE9E0
unsigned long CServerExoApp::ResolvePlayerByFirstName(const CExoString& sName)
{
    return m_pcExoAppInternal->ResolvePlayerByFirstName(sName);
}

// FUNCTION: SWKOTOR 0x004AE9F0
int CServerExoApp::GetDifficultyOption(int nOption)
{
    return m_pcExoAppInternal->GetDifficultyOption(nOption);
}

// FUNCTION: SWKOTOR 0x004AEA00
int CServerExoApp::AddCharListRequest(unsigned long nPlayerId)
{
    return m_pcExoAppInternal->AddCharListRequest(nPlayerId);
}

// FUNCTION: SWKOTOR 0x004AEA10
unsigned long CServerExoApp::GetFirstPCObject()
{
    return m_pcExoAppInternal->GetFirstPCObject();
}

// FUNCTION: SWKOTOR 0x004AEA20
unsigned long CServerExoApp::GetNextPCObject()
{
    return m_pcExoAppInternal->GetNextPCObject();
}

// FUNCTION: SWKOTOR 0x004AEA30
unsigned long CServerExoApp::ClientToServerObjectID(unsigned long oid)
{
    return m_pcExoAppInternal->ClientToServerObjectID(oid);
}

// FUNCTION: SWKOTOR 0x004AEA40
unsigned long CServerExoApp::GetPlayerCreatureID()
{
    return m_pcExoAppInternal->GetPlayerCreatureID();
}

// FUNCTION: SWKOTOR 0x004AEAD0
void CServerExoApp::UpdateSubareasOnMoveTo(unsigned long oid, const Vector& a, const Vector& b)
{
    m_pcExoAppInternal->UpdateSubareasOnMoveTo(oid, a, b);
}

// FUNCTION: SWKOTOR 0x004AEAE0
int CServerExoApp::IsActionsPending(unsigned long oid)
{
    return m_pcExoAppInternal->IsActionsPending(oid);
}

// FUNCTION: SWKOTOR 0x004AEB40
int CServerExoApp::CreatureAcquireItem(unsigned long oidCreature, const CResRef& cTemplate)
{
    return m_pcExoAppInternal->CreatureAcquireItem(oidCreature, cTemplate);
}

// FUNCTION: SWKOTOR 0x004AEB50
int CServerExoApp::TestSafeLocation(unsigned long oidCreature, const Vector& v)
{
    return m_pcExoAppInternal->TestSafeLocation(oidCreature, v);
}

// FUNCTION: SWKOTOR 0x004AEB60
int CServerExoApp::ComputeSafeLocation(const Vector& v, float f, unsigned long oidCreature, void* p, Vector* out)
{
    return m_pcExoAppInternal->ComputeSafeLocation(v, f, oidCreature, p, out);
}

// FUNCTION: SWKOTOR 0x004AEB90
void CServerExoApp::ClearLastModuleFound()
{
    m_pcExoAppInternal->ClearLastModuleFound();
}

// FUNCTION: SWKOTOR 0x004AEBA0
void CServerExoApp::ClearAreaCache()
{
    m_pcExoAppInternal->ClearAreaCache();
}

// FUNCTION: SWKOTOR 0x004AEBB0
int CServerExoApp::ShrinkLineByCreatures(Vector* a, Vector* b, unsigned long c)
{
    return m_pcExoAppInternal->ShrinkLineByCreatures(a, b, c);
}

// FUNCTION: SWKOTOR 0x004AEBC0
void CServerExoApp::SetMoveToWorldTime(unsigned long n0, unsigned char n1, unsigned char n2, unsigned char n3, unsigned short n4, unsigned short n5, unsigned short n6)
{
    m_pcExoAppInternal->SetMoveToWorldTime(n0, n1, n2, n3, n4, n5, n6);
}

// FUNCTION: SWKOTOR 0x004AEBD0
void CServerExoApp::GetMoveToWorldTime(unsigned long* p0, unsigned char* p1, unsigned char* p2, unsigned char* p3, unsigned short* p4, unsigned short* p5, unsigned short* p6)
{
    m_pcExoAppInternal->GetMoveToWorldTime(p0, p1, p2, p3, p4, p5, p6);
}

// FUNCTION: SWKOTOR 0x004AEBE0
int CServerExoApp::GetModulePreviouslyVisited(CExoString& sModule)
{
    return m_pcExoAppInternal->GetModulePreviouslyVisited(sModule);
}

// FUNCTION: SWKOTOR 0x004AEBF0
int CServerExoApp::ExecuteLoadModule(CExoString& sModule)
{
    return m_pcExoAppInternal->ExecuteLoadModule(sModule);
}

// FUNCTION: SWKOTOR 0x004AE6C0
CExoString CServerExoApp::GetModuleName()
{
    return m_pcExoAppInternal->GetModuleName();
}

// FUNCTION: SWKOTOR 0x004AE8D0
CExoString CServerExoApp::GetPlayerListString()
{
    return m_pcExoAppInternal->GetPlayerListString();
}

// FUNCTION: SWKOTOR 0x004AEC90
CServerInfo* CServerExoApp::FUN_004aec90()
{
    return m_pcExoAppInternal->m_pServerInfo;
}

// FUNCTION: SWKOTOR 0x004AED70
CGameObjectArray* CServerExoApp::FUN_004aed70()
{
    return m_pcExoAppInternal->m_pGameObjArray;
}

// FUNCTION: SWKOTOR 0x004AED80
CServerAIMaster* CServerExoApp::FUN_004aed80()
{
    return m_pcExoAppInternal->m_pServerAIMaster;
}

// FUNCTION: SWKOTOR 0x004AED90
CExoLinkedList<CSWSPlayer>* CServerExoApp::FUN_004aed90()
{
    return m_pcExoAppInternal->m_pPlayerList;
}

// FUNCTION: SWKOTOR 0x004AEDB0
CSWSMessage* CServerExoApp::FUN_004aedb0()
{
    return m_pcExoAppInternal->m_pServerMessage;
}

// FUNCTION: SWKOTOR 0x004AEDC0
CExoLinkedList<CSWSPlayer>* CServerExoApp::GetSysAdminList()
{
    return m_pcExoAppInternal->m_pSysAdminList;
}

// FUNCTION: SWKOTOR 0x004AEDD0
unsigned short CServerExoApp::GetServerMode()
{
    return m_pcExoAppInternal->m_nServerMode;
}

// FUNCTION: SWKOTOR 0x004AEDE0
unsigned long CServerExoApp::FUN_004aede0()
{
    return (unsigned long)m_pcExoAppInternal->m_pWorldTimer;
}

// FUNCTION: SWKOTOR 0x004AEDF0
CWorldTimer* CServerExoApp::GetTimestopTimer()
{
    return m_pcExoAppInternal->m_pTimestopTimer;
}

// FUNCTION: SWKOTOR 0x004AEE00
CWorldTimer* CServerExoApp::GetPauseTimer()
{
    return m_pcExoAppInternal->m_pPauseTimer;
}

// FUNCTION: SWKOTOR 0x004AEE20
int CServerExoApp::GetDebugMode()
{
    return m_pcExoAppInternal->m_bDebugMode;
}

// FUNCTION: SWKOTOR 0x004AECF0
int CServerExoApp::GetLoadingModule()
{
    return m_pcExoAppInternal->m_bLoadingModule;
}

// FUNCTION: SWKOTOR 0x004AED20
int CServerExoApp::GetAutoSavePending()
{
    return m_pcExoAppInternal->m_bAutoSavePending;
}

// FUNCTION: SWKOTOR 0x004AEFC0
CFactionManager* CServerExoApp::GetFactionManager()
{
    return m_pcExoAppInternal->m_pFactionManager;
}

// FUNCTION: SWKOTOR 0x004AF020
unsigned long CServerExoApp::GetActionConversation()
{
    return m_pcExoAppInternal->m_oidActionConversation;
}

// FUNCTION: SWKOTOR 0x004AF040
int CServerExoApp::GetIgnoreConversationStartRange()
{
    return m_pcExoAppInternal->m_bIgnoreConversationStartRange;
}

// FUNCTION: SWKOTOR 0x004AF070
int CServerExoApp::GetButtonMashCheck()
{
    return m_pcExoAppInternal->m_bButtonMashCheck;
}

// FUNCTION: SWKOTOR 0x004AF0B0
unsigned long CServerExoApp::GetMoveToPausedCalendarDay()
{
    return m_pcExoAppInternal->m_nMoveToPausedCalendarDay;
}

// FUNCTION: SWKOTOR 0x004AF0D0
unsigned long CServerExoApp::GetMoveToPausedTimeOfDay()
{
    return m_pcExoAppInternal->m_nMoveToPausedTimeOfDay;
}

// FUNCTION: SWKOTOR 0x004AEE60
CSWGlobalVariableTable* CServerExoApp::GetGlobalVariableTable()
{
    return &m_pcExoAppInternal->m_cGlobalVariableTable;
}

// FUNCTION: SWKOTOR 0x004AEE70
CServerExoAppInternal1b770* CServerExoApp::FUN_004aee70()
{
    return &m_pcExoAppInternal->m_c1b770;
}

// FUNCTION: SWKOTOR 0x004AECC0
void CServerExoApp::SetMoveToModulePending(int b)
{
    m_pcExoAppInternal->m_bMoveToModulePending = b;
}

// FUNCTION: SWKOTOR 0x004AECE0
void CServerExoApp::SetExportAllCharactersPending(int b)
{
    m_pcExoAppInternal->m_bExportAllCharactersPending = b;
}

// FUNCTION: SWKOTOR 0x004AED00
void CServerExoApp::SetLoadingModule(int b)
{
    m_pcExoAppInternal->m_bLoadingModule = b;
}

// FUNCTION: SWKOTOR 0x004AED10
void CServerExoApp::SetAutoSavePending(int b)
{
    m_pcExoAppInternal->m_bAutoSavePending = b;
}

// FUNCTION: SWKOTOR 0x004AEE10
void CServerExoApp::SetDebugMode(int b)
{
    m_pcExoAppInternal->m_bDebugMode = b;
}

// FUNCTION: SWKOTOR 0x004AF010
void CServerExoApp::SetActionConversation(unsigned long oid)
{
    m_pcExoAppInternal->m_oidActionConversation = oid;
}

// FUNCTION: SWKOTOR 0x004AF030
void CServerExoApp::SetIgnoreConversationStartRange(int b)
{
    m_pcExoAppInternal->m_bIgnoreConversationStartRange = b;
}

// FUNCTION: SWKOTOR 0x004AF060
void CServerExoApp::SetLoadFromSaveGame(int b)
{
    m_pcExoAppInternal->m_bLoadFromSaveGame = b;
}

// FUNCTION: SWKOTOR 0x004AF080
void CServerExoApp::SetButtonMashCheck(int b)
{
    m_pcExoAppInternal->m_bButtonMashCheck = b;
}

// FUNCTION: SWKOTOR 0x004AF090
void CServerExoApp::SetDoingStartNewModule(int b)
{
    m_pcExoAppInternal->m_bDoingStartNewModule = b;
}

// FUNCTION: SWKOTOR 0x004AF0A0
void CServerExoApp::SetMoveToPausedCalendarDay(unsigned long n)
{
    m_pcExoAppInternal->m_nMoveToPausedCalendarDay = n;
}

// FUNCTION: SWKOTOR 0x004AF0C0
void CServerExoApp::SetMoveToPausedTimeOfDay(unsigned long n)
{
    m_pcExoAppInternal->m_nMoveToPausedTimeOfDay = n;
}

// FUNCTION: SWKOTOR 0x004AECD0
void CServerExoApp::SetMoveToModuleString(const CExoString& s)
{
    m_pcExoAppInternal->m_sMoveToModuleString = s;
}

// FUNCTION: SWKOTOR 0x004AED30
void CServerExoApp::SetMoveToModuleStartWaypoint(const CExoString& s)
{
    m_pcExoAppInternal->m_sMoveToModuleStartWaypoint = s;
}

// FUNCTION: SWKOTOR 0x004AEFD0
void CServerExoApp::SetLastConversation(const CExoString& s)
{
    m_pcExoAppInternal->m_sLastConversation = s;
}

// FUNCTION: SWKOTOR 0x004AF0E0
void CServerExoApp::SetSaveGameName(const CExoString& s)
{
    m_pcExoAppInternal->m_sSaveGameName = s;
}

// FUNCTION: SWKOTOR 0x004AED40
CExoString CServerExoApp::GetMoveToModuleStartWaypoint()
{
    return m_pcExoAppInternal->m_sMoveToModuleStartWaypoint;
}

// FUNCTION: SWKOTOR 0x004AEFE0
CExoString CServerExoApp::GetLastConversation()
{
    return m_pcExoAppInternal->m_sLastConversation;
}
