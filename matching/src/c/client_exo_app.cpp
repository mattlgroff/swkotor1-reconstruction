// CClientExoApp facade accessors and forwarders (b005e_client_exo_app_accessors).
// 0x005ED480 GetCaptureEventPlayBack is matched in pilot1/tiny.cpp.

#include "c/client_exo_app.h"

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char* szResRef);                  // 0x00406D80
};

// FUNCTION: SWKOTOR 0x005ED320
CSWCQuestJournal* CClientExoApp::GetQuestJournal()
{
    return m_pcInternal->m_pQuestJournal;
}

// FUNCTION: SWKOTOR 0x005ED330
CSWCWorldJournal* CClientExoApp::GetWorldJournal()
{
    return m_pcInternal->m_pWorldJournal;
}

// FUNCTION: SWKOTOR 0x005ED470
int CClientExoApp::GetCaptureEvents()
{
    return m_pcInternal->m_bCaptureEvents;
}

// FUNCTION: SWKOTOR 0x005ED4D0
int CClientExoApp::IsClientImeUI()
{
    return m_pcInternal->m_bClientImeUI;
}

// FUNCTION: SWKOTOR 0x005ED4E0
CImeUI* CClientExoApp::GetImeUI()
{
    return m_pcInternal->m_pImeUI;
}

// FUNCTION: SWKOTOR 0x005ED500
CWorldTimer* CClientExoApp::GetPauseTimer()
{
    return m_pcInternal->m_pPauseTimer;
}

// FUNCTION: SWKOTOR 0x005ED510
CWorldTimer* CClientExoApp::GetAnimationTimer()
{
    return m_pcInternal->m_pAnimationTimer;
}

// FUNCTION: SWKOTOR 0x005ED6A0
CSWGuiPanel* CClientExoApp::GetPlayModulePanel()
{
    return m_pcInternal->m_pPlayModulePanel;
}

// FUNCTION: SWKOTOR 0x005ED6C0
CExoLinkedList<CClientPlayer>* CClientExoApp::GetPlayerList()
{
    return &m_pcInternal->m_lstPlayerList;
}

// FUNCTION: SWKOTOR 0x005ED7C0
int CClientExoApp::IsClientInitialized()
{
    return m_pcInternal->m_bClientInitialized;
}

// FUNCTION: SWKOTOR 0x005ED8C0
unsigned long CClientExoApp::GetHitCheckMaterialMask()
{
    return m_pcInternal->m_nHitCheckMaterialMask;
}

// FUNCTION: SWKOTOR 0x005ED910
CSoundSetList* CClientExoApp::GetSoundSetList()
{
    return m_pcInternal->m_pSoundSetList;
}

// FUNCTION: SWKOTOR 0x005ED970
int CClientExoApp::GetLoadBarState()
{
    return m_pcInternal->m_nLoadBarState;
}

// FUNCTION: SWKOTOR 0x005EDA70
CClientInput* CClientExoApp::GetInputClass()
{
    return m_pcInternal->m_pInputClass;
}

// FUNCTION: SWKOTOR 0x005EDAC0
int CClientExoApp::GetPlayerCharacterSet()
{
    return m_pcInternal->m_nPlayerCharacterSet;
}

// FUNCTION: SWKOTOR 0x005EDAD0
int CClientExoApp::GetAreaNotReadyToDisplay()
{
    return m_pcInternal->m_bAreaNotReadyToDisplay;
}

// FUNCTION: SWKOTOR 0x005EDB60
CExoArrayList<CResRef>* CClientExoApp::GetMoveToModuleMovies()
{
    return &m_pcInternal->m_lstMoveToModuleMovies;
}

// FUNCTION: SWKOTOR 0x005EDC60
int CClientExoApp::GetGameOver()
{
    return m_pcInternal->m_bGameOver;
}

// FUNCTION: SWKOTOR 0x005EDD60
int CClientExoApp::GetLoadFromIFO()
{
    return m_pcInternal->m_bLoadFromIFO;
}

// FUNCTION: SWKOTOR 0x005EDD80
unsigned long CClientExoApp::GetLastTarget()
{
    return m_pcInternal->m_oidLastTarget;
}

// FUNCTION: SWKOTOR 0x005EDE70
int CClientExoApp::GetCombatMode()
{
    return m_pcInternal->m_bCombatMode;
}

// FUNCTION: SWKOTOR 0x005EDEF0
int CClientExoApp::GetAutoPaused()
{
    return m_pcInternal->m_bAutoPaused;
}

// FUNCTION: SWKOTOR 0x005EDFA0
int CClientExoApp::GetAttackMashTutorialTimer()
{
    return m_pcInternal->m_nAttackMashTutorialTimer;
}

// FUNCTION: SWKOTOR 0x005EDFB0
int CClientExoApp::GetLoadingSaveGame()
{
    return m_pcInternal->m_bLoadingSaveGame;
}

// FUNCTION: SWKOTOR 0x005EE010
void CClientExoApp::ClearEnemySighted()
{
    m_pcInternal->m_bEnemySighted = 0;
}

// FUNCTION: SWKOTOR 0x005EE050
unsigned char CClientExoApp::GetGamePlayHint()
{
    return m_pcInternal->m_bGamePlayHint;
}

// FUNCTION: SWKOTOR 0x005EE070
unsigned char CClientExoApp::GetStoryHint()
{
    return m_pcInternal->m_bStoryHint;
}

// FUNCTION: SWKOTOR 0x005EE0C0
void CClientExoApp::ResetModuleResourcesLoaded()
{
    m_pcInternal->m_bModuleResourcesLoaded = 0;
}

// FUNCTION: SWKOTOR 0x005EE140
int CClientExoApp::GetPazaakIsRunning()
{
    return m_pcInternal->m_bPazaakIsRunning;
}

// FUNCTION: SWKOTOR 0x005EE190
int CClientExoApp::GetCharacterChangeInProgress()
{
    return m_pcInternal->m_bCharacterChangeInProgress;
}

// FUNCTION: SWKOTOR 0x005EE1E0
int CClientExoApp::GetCreditSequenceInProgress()
{
    return m_pcInternal->m_bCreditSequenceInProgress;
}

// FUNCTION: SWKOTOR 0x005EE2C0
int CClientExoApp::GetAllowHighMonitorFrequency()
{
    return m_pcInternal->m_bAllowHighMonitorFrequency;
}

// FUNCTION: SWKOTOR 0x005EE2D0
int CClientExoApp::GetSoundPausedByCombat()
{
    return m_pcInternal->m_bSoundPausedByCombat;
}

// FUNCTION: SWKOTOR 0x005EE380
void CClientExoApp::RemoveMoveToModuleMovies()
{
    m_pcInternal->m_lstMoveToModuleMovies.num = 0;
}

// FUNCTION: SWKOTOR 0x005EDD90
void CClientExoApp::SetRunScript(const CExoString& sScript)
{
    CClientExoAppInternal* pInternal = m_pcInternal;
    pInternal->m_bRunScript = 1;
    pInternal->m_sRunScript = sScript;
}

// FUNCTION: SWKOTOR 0x005EE2F0
CClientExoApp::CClientExoApp()
{
    m_pcInternal = new CClientExoAppInternal();
}

// FUNCTION: SWKOTOR 0x005EE360
CClientExoApp::~CClientExoApp()
{
    if (m_pcInternal) {
        delete m_pcInternal;
    }
}
// FUNCTION: SWKOTOR 0x005ED300
void CClientExoApp::BeginIntro()
{
    m_pcInternal->BeginIntro();
}

// FUNCTION: SWKOTOR 0x005ED340
void CClientExoApp::JournalUpdated(int a, int b, int c)
{
    m_pcInternal->JournalUpdated(a, b, c);
}

// FUNCTION: SWKOTOR 0x005ED350
int CClientExoApp::GetObjectName(unsigned long oid, CExoString* pName)
{
    return m_pcInternal->GetObjectName(oid, pName);
}

// FUNCTION: SWKOTOR 0x005ED410
void CClientExoApp::SetCharacterFile(unsigned long nSize, const char* pData, unsigned long nValue)
{
    m_pcInternal->SetCharacterFile(nSize, pData, nValue);
}

// FUNCTION: SWKOTOR 0x005ED420
void CClientExoApp::DisplayMainMenu(int n)
{
    m_pcInternal->ResetGuis(n);
}

// FUNCTION: SWKOTOR 0x005ED430
void CClientExoApp::UnloadModule()
{
    m_pcInternal->UnloadModule();
}

// FUNCTION: SWKOTOR 0x005ED440
CSWCModule* CClientExoApp::CreateModule()
{
    return m_pcInternal->CreateModule();
}

// FUNCTION: SWKOTOR 0x005ED450
void CClientExoApp::ExitProgram()
{
    m_pcInternal->ExitProgram();
}

// FUNCTION: SWKOTOR 0x005ED530
CSWCModule* CClientExoApp::GetModule()
{
    return m_pcInternal->GetModule();
}

// FUNCTION: SWKOTOR 0x005ED540
CSWCCreature* CClientExoApp::GetPlayerCreature()
{
    return m_pcInternal->GetPlayerCreature();
}

// FUNCTION: SWKOTOR 0x005ED560
void* CClientExoApp::GetModuleCamera()
{
    return m_pcInternal->GetModuleCamera();
}

// FUNCTION: SWKOTOR 0x005ED580
CGameObject* CClientExoApp::GetGameObject(unsigned long oid)
{
    return m_pcInternal->GetGameObject(oid);
}

// FUNCTION: SWKOTOR 0x005ED590
CClientPlayer* CClientExoApp::GetPlayerByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetPlayerByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED5A0
CSWCItem* CClientExoApp::GetItemByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetItemByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED5B0
CSWCCreature* CClientExoApp::GetCreatureByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetCreatureByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED5C0
CSWCArea* CClientExoApp::GetAreaByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetAreaByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED5D0
CSWCTrigger* CClientExoApp::GetTriggerByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetTriggerByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED5E0
CSWCProjectile* CClientExoApp::GetProjectileByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetProjectileByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED5F0
CSWCPlaceable* CClientExoApp::GetPlaceableByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetPlaceableByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED600
CSWCDoor* CClientExoApp::GetDoorByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetDoorByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED610
CSWCAreaOfEffectObject* CClientExoApp::GetAreaOfEffectByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetAreaOfEffectByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED620
CSWCSoundObject* CClientExoApp::GetSoundObjectByGameObjectID(unsigned long oid)
{
    return m_pcInternal->GetSoundObjectByGameObjectID(oid);
}

// FUNCTION: SWKOTOR 0x005ED640
int CClientExoApp::GetPauseState(unsigned char nType)
{
    return m_pcInternal->GetPauseState(nType);
}

// FUNCTION: SWKOTOR 0x005ED650
void CClientExoApp::SetPauseState(int nType, int bPause, int bForce)
{
    m_pcInternal->SetPauseState(nType, bPause, bForce);
}

// FUNCTION: SWKOTOR 0x005ED670
int CClientExoApp::SendSaveGameRequest(int a, int b, int c)
{
    return m_pcInternal->SendSaveGameRequest(a, b, c);
}

// FUNCTION: SWKOTOR 0x005ED680
int CClientExoApp::SendLoadGameRequest(int a, int b, int c)
{
    return m_pcInternal->SendLoadGameRequest(a, b, c);
}

// FUNCTION: SWKOTOR 0x005ED6D0
CClientPlayer* CClientExoApp::GetPlayerByPlayerID(unsigned long nPlayerId)
{
    return m_pcInternal->GetPlayerByPlayerID(nPlayerId);
}

// FUNCTION: SWKOTOR 0x005ED720
void CClientExoApp::AttachPlayerGameObjectToArea()
{
    m_pcInternal->AttachPlayerGameObjectToArea();
}

// FUNCTION: SWKOTOR 0x005ED730
void CClientExoApp::InitializeInGameGUI()
{
    m_pcInternal->InitializeInGameGUI();
}

// FUNCTION: SWKOTOR 0x005ED740
void CClientExoApp::ServerModeChange(unsigned char nMode)
{
    m_pcInternal->ServerModeChange(nMode);
}

// FUNCTION: SWKOTOR 0x005ED750
void CClientExoApp::AddObjectToHitcheckIgnoreList(unsigned long oid)
{
    m_pcInternal->AddObjectToHitcheckIgnoreList(oid);
}

// FUNCTION: SWKOTOR 0x005ED760
void CClientExoApp::RemoveObjectFromHitcheckIgnoreList(unsigned long oid)
{
    m_pcInternal->RemoveObjectFromHitcheckIgnoreList(oid);
}

// FUNCTION: SWKOTOR 0x005ED770
void CClientExoApp::EmptyHitcheckIgnoreList()
{
    m_pcInternal->EmptyHitcheckIgnoreList();
}

// FUNCTION: SWKOTOR 0x005ED780
void CClientExoApp::SetPlayerCreature(unsigned long oid)
{
    m_pcInternal->SetPlayerCreature(oid);
}

// FUNCTION: SWKOTOR 0x005ED7B0
int CClientExoApp::Initialize()
{
    return m_pcInternal->Initialize();
}

// FUNCTION: SWKOTOR 0x005ED7D0
int CClientExoApp::MainLoop()
{
    return m_pcInternal->MainLoop();
}

// FUNCTION: SWKOTOR 0x005ED810
void CClientExoApp::Uninitialize()
{
    m_pcInternal->Uninitialize();
}

// FUNCTION: SWKOTOR 0x005ED820
int CClientExoApp::CreateCaptureEventFile(const CExoString& sFile)
{
    return m_pcInternal->CreateCaptureEventFile(sFile);
}

// FUNCTION: SWKOTOR 0x005ED830
void CClientExoApp::setUpEventPlayBack(const CExoString& sFile)
{
    m_pcInternal->setUpEventPlayBack(sFile);
}

// FUNCTION: SWKOTOR 0x005ED840
void CClientExoApp::setCaptureEventRate(int nRate)
{
    m_pcInternal->setCaptureEventRate(nRate);
}

// FUNCTION: SWKOTOR 0x005ED850
void CClientExoApp::SetDebugMode(int nMode)
{
    m_pcInternal->SetDebugMode(nMode);
}

// FUNCTION: SWKOTOR 0x005ED860
int CClientExoApp::StartServices(int a, int b, int c, int d, int e)
{
    return m_pcInternal->StartServices(a, b, c, d, e);
}

// FUNCTION: SWKOTOR 0x005ED870
void CClientExoApp::SetEventDescriptions()
{
    m_pcInternal->SetEventDescriptions();
}

// FUNCTION: SWKOTOR 0x005ED880
void CClientExoApp::StopServices()
{
    m_pcInternal->StopServices();
}

// FUNCTION: SWKOTOR 0x005ED980
void CClientExoApp::RestartMoviesIfNecessary()
{
    m_pcInternal->RestartMoviesIfNecessary();
}

// FUNCTION: SWKOTOR 0x005ED9A0
void CClientExoApp::AddObjectModelDirectlyToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList)
{
    m_pcInternal->AddObjectToIgnoreList(oid, pList);
}

// FUNCTION: SWKOTOR 0x005ED9B0
void CClientExoApp::StartMenuMusic(int a, int b, int c)
{
    m_pcInternal->SetMusic(a, b, c);
}

// FUNCTION: SWKOTOR 0x005ED9C0
void CClientExoApp::StopMenuMusic()
{
    m_pcInternal->StopMenuMusic();
}

// FUNCTION: SWKOTOR 0x005ED9D0
void CClientExoApp::StopLoadMusic()
{
    m_pcInternal->StopLoadMusic();
}

// FUNCTION: SWKOTOR 0x005ED9F0
unsigned char CClientExoApp::GetActivePauseState()
{
    return m_pcInternal->GetActivePauseState();
}

// FUNCTION: SWKOTOR 0x005EDA00
void CClientExoApp::QueryExitProgram()
{
    m_pcInternal->QueryExitProgram();
}

// FUNCTION: SWKOTOR 0x005EDA10
void CClientExoApp::SetInputActive(int bActive, int bClear)
{
    m_pcInternal->SetInputActive(bActive, bClear);
}

// FUNCTION: SWKOTOR 0x005EDA20
void CClientExoApp::TurnOffKeyboardModal()
{
    m_pcInternal->ResetInput();
}

// FUNCTION: SWKOTOR 0x005EDA30
int CClientExoApp::IsPlayerCreature(CGameObject* pObject)
{
    return m_pcInternal->IsPlayerCreature(pObject);
}

// FUNCTION: SWKOTOR 0x005EDA50
unsigned long CClientExoApp::ServerToClientObjectId(unsigned long oid)
{
    return m_pcInternal->ServerToClientObjectId(oid);
}

// FUNCTION: SWKOTOR 0x005EDA60
int CClientExoApp::SetInputClass(int nClass, int bForce)
{
    return m_pcInternal->SetInputClass(nClass, bForce);
}

// FUNCTION: SWKOTOR 0x005EDA80
void CClientExoApp::ShowLoadScreen(int a, int b, int c)
{
    m_pcInternal->ShowLoadScreen(a, b, c);
}

// FUNCTION: SWKOTOR 0x005EDA90
void CClientExoApp::HideLoadScreen(int a, int b)
{
    m_pcInternal->HideLoadScreen(a, b);
}

// FUNCTION: SWKOTOR 0x005EDAA0
void CClientExoApp::SetPlayerCharacterName(const CExoString& sName)
{
    m_pcInternal->SetPlayerCharacterName(sName);
}

// FUNCTION: SWKOTOR 0x005EDAB0
CExoString* CClientExoApp::GetPlayerCharacterName()
{
    return m_pcInternal->GetPlayerCharacterName();
}

// FUNCTION: SWKOTOR 0x005EDB20
void CClientExoApp::PlayLoadModuleMovies()
{
    m_pcInternal->PlayLoadModuleMovies();
}

// FUNCTION: SWKOTOR 0x005EDB30
int CClientExoApp::PlayMovie(int a, int b, int c, int d)
{
    return m_pcInternal->PlayMovie(a, b, c, d);
}

// FUNCTION: SWKOTOR 0x005EDB40
int CClientExoApp::IsMoviePlaying()
{
    return m_pcInternal->IsMoviePlaying();
}

// FUNCTION: SWKOTOR 0x005EDB50
void CClientExoApp::AddMoveToModuleMovie(const CResRef& cMovie)
{
    m_pcInternal->AddMoveToModuleMovie(cMovie);
}

// FUNCTION: SWKOTOR 0x005EDB70
void CClientExoApp::IndicateObject(CGameObject* pObject)
{
    m_pcInternal->IndicateObject(pObject);
}

// FUNCTION: SWKOTOR 0x005EDBA0
void CClientExoApp::ResetDriveAcceleration()
{
    m_pcInternal->ResetDriveAcceleration();
}

// FUNCTION: SWKOTOR 0x005EDBB0
void CClientExoApp::BarkString(unsigned long oid, const CExoString& text, const CResRef& sound, int b)
{
    m_pcInternal->BarkString(oid, text, sound, b);
}

// FUNCTION: SWKOTOR 0x005EDBC0
int CClientExoApp::IsSoundPlayingInDialog()
{
    return m_pcInternal->IsSoundPlayingInDialog();
}

// FUNCTION: SWKOTOR 0x005EDBD0
void CClientExoApp::StopSoundPlayingInDialog()
{
    m_pcInternal->StopSoundPlayingInDialog();
}

// FUNCTION: SWKOTOR 0x005EDBE0
void CClientExoApp::DisableInput()
{
    m_pcInternal->DisableInput();
}

// FUNCTION: SWKOTOR 0x005EDBF0
void CClientExoApp::EnableInput()
{
    m_pcInternal->EnableInput();
}

// FUNCTION: SWKOTOR 0x005EDC00
void CClientExoApp::DismissInGameGUI()
{
    m_pcInternal->DismissInGameGUI();
}

// FUNCTION: SWKOTOR 0x005EDC10
int CClientExoApp::GetPausedByCombat()
{
    return m_pcInternal->GetPausedByCombat();
}

// FUNCTION: SWKOTOR 0x005EDC30
void CClientExoApp::ChangeCharacter(int a, int b)
{
    m_pcInternal->ChangeCharacter(a, b);
}

// FUNCTION: SWKOTOR 0x005EDC40
void CClientExoApp::RunDeathSequence()
{
    m_pcInternal->RunDeathSequence();
}

// FUNCTION: SWKOTOR 0x005EDC50
void CClientExoApp::SetGameOver(int b)
{
    m_pcInternal->SetGameOver(b);
}

// FUNCTION: SWKOTOR 0x005EDC70
void CClientExoApp::SetLastPartyMemberTempKilled(int b)
{
    m_pcInternal->SetLastPartyMemberTempKilled(b);
}

// FUNCTION: SWKOTOR 0x005EDC80
void CClientExoApp::SetCanSendDialog(int b)
{
    m_pcInternal->SetCanSendDialog(b);
}

// FUNCTION: SWKOTOR 0x005EDC90
int CClientExoApp::GetCanSendDialog(unsigned long oidSpeaker, unsigned long oidListener, CSWSDialog* pDialog)
{
    return m_pcInternal->GetCanSendDialog(oidSpeaker, oidListener, pDialog);
}

// FUNCTION: SWKOTOR 0x005EDCE0
int CClientExoApp::GetClientMeleeAnimation(unsigned long a, unsigned long b, int c, int d, int e, int f)
{
    return m_pcInternal->GetClientMeleeAnimation(a, b, c, d, e, f);
}

// FUNCTION: SWKOTOR 0x005EDCF0
void CClientExoApp::SetLoadScreenByModuleName(const CExoString& sModule)
{
    m_pcInternal->SetLoadScreenByModuleName(sModule);
}

// FUNCTION: SWKOTOR 0x005EDD40
void CClientExoApp::SetLoadBarProgress(int a, int b)
{
    m_pcInternal->SetLoadBarProgress(a, b);
}

// FUNCTION: SWKOTOR 0x005EDD70
void CClientExoApp::SetLastTarget(unsigned long oid, int b)
{
    m_pcInternal->SetLastTarget(oid, b);
}

// FUNCTION: SWKOTOR 0x005EDDB0
void CClientExoApp::UpdateRoom(int n)
{
    m_pcInternal->UpdateRoom(n);
}

// FUNCTION: SWKOTOR 0x005EDDC0
void CClientExoApp::StartPazaakGame(int a, int b, int c, int d, int e)
{
    m_pcInternal->StartPazaakGame(a, b, c, d, e);
}

// FUNCTION: SWKOTOR 0x005EDDD0
void CClientExoApp::EndPazaakGame(int a, int b, int c, int d)
{
    m_pcInternal->EndPazaakGame(a, b, c, d);
}

// FUNCTION: SWKOTOR 0x005EDE40
int CClientExoApp::PlayRumblePattern(int nPattern)
{
    return m_pcInternal->PlayRumblePattern(nPattern);
}

// FUNCTION: SWKOTOR 0x005EDE50
void CClientExoApp::StopRumblePattern(int nPattern)
{
    m_pcInternal->StopRumblePattern(nPattern);
}

// FUNCTION: SWKOTOR 0x005EDE60
void CClientExoApp::TogglePartyFollow()
{
    m_pcInternal->TogglePartyFollow();
}

// FUNCTION: SWKOTOR 0x005EDE80
void CClientExoApp::SetCombatMode(int bCombat)
{
    m_pcInternal->SetCombatMode(bCombat);
}

// FUNCTION: SWKOTOR 0x005EDE90
void CClientExoApp::PlayerFlourishWeapons()
{
    m_pcInternal->PlayerFlourishWeapons();
}

// FUNCTION: SWKOTOR 0x005EDEA0
void CClientExoApp::AddFloatyText(unsigned long oid, unsigned char nType, int nValue)
{
    m_pcInternal->AddFloatyText(oid, nType, nValue);
}

// FUNCTION: SWKOTOR 0x005EDEE0
void CClientExoApp::SetAutoPaused(int a, int b)
{
    m_pcInternal->SetAutoPaused(a, b);
}

// FUNCTION: SWKOTOR 0x005EDF00
void CClientExoApp::SetLoadScreenHint(int nHint)
{
    m_pcInternal->SetLoadScreenHint(nHint);
}

// FUNCTION: SWKOTOR 0x005EDF10
void CClientExoApp::EnableVideoEffect(int nEffect)
{
    m_pcInternal->EnableVideoEffect(nEffect);
}

// FUNCTION: SWKOTOR 0x005EDF20
void CClientExoApp::DisableVideoEffect()
{
    m_pcInternal->DisableVideoEffect();
}

// FUNCTION: SWKOTOR 0x005EDF30
void CClientExoApp::DelayNextEnemyAutoPause()
{
    m_pcInternal->DelayNextEnemyAutoPause();
}

// FUNCTION: SWKOTOR 0x005EDF40
void CClientExoApp::ShowTutorialWindow(int a, int b, int c, int d)
{
    m_pcInternal->ShowTutorialWindow(a, b, c, d);
}

// FUNCTION: SWKOTOR 0x005EDF50
void CClientExoApp::ClearCachedCreatures(int b)
{
    m_pcInternal->ClearCachedCreatures(b);
}

// FUNCTION: SWKOTOR 0x005EDF60
void CClientExoApp::LookUpAndPerformRumbleWithCutOff(int a, int b, int c)
{
    m_pcInternal->LookUpAndPerformRumbleWithCutOff(a, b, c);
}

// FUNCTION: SWKOTOR 0x005EDF70
int CClientExoApp::SaveOptions()
{
    return m_pcInternal->ForwardUnknown46f0();
}

// FUNCTION: SWKOTOR 0x005EDF80
void CClientExoApp::ChangeCharacterToNextLivingPartyMember(int a, int b, int c)
{
    m_pcInternal->ChangeCharacterToNextLivingPartyMember(a, b, c);
}

// FUNCTION: SWKOTOR 0x005EE090
int CClientExoApp::GetNextLoadScreenHintSTRREF(int a, int b)
{
    return m_pcInternal->GetNextLoadScreenHintSTRREF(a, b);
}

// FUNCTION: SWKOTOR 0x005EE120
void CClientExoApp::BusyWaitMenuMusic()
{
    m_pcInternal->WaitSoundFade750();
}

// FUNCTION: SWKOTOR 0x005EE130
void CClientExoApp::BusyWaitMuteSound()
{
    m_pcInternal->WaitSound500();
}

// FUNCTION: SWKOTOR 0x005EE150
void CClientExoApp::SetMouseMode(unsigned char nMode)
{
    m_pcInternal->SetMouseMode(nMode);
}

// FUNCTION: SWKOTOR 0x005EE160
void CClientExoApp::CancelMovie()
{
    m_pcInternal->CancelMovie();
}

// FUNCTION: SWKOTOR 0x005EE170
void CClientExoApp::HideMouse(int nMask)
{
    m_pcInternal->SetInputState(nMask);
}

// FUNCTION: SWKOTOR 0x005EE180
void CClientExoApp::ShowMouse(int nMask)
{
    m_pcInternal->ClearInputState(nMask);
}

// FUNCTION: SWKOTOR 0x005EE1B0
void CClientExoApp::FadeAndStopStreamingSoundSource(CExoStreamingSoundSource* p, unsigned long nMs)
{
    m_pcInternal->FadeAndStopStreamingSoundSource(p, nMs);
}

// FUNCTION: SWKOTOR 0x005EE1C0
void CClientExoApp::StartCreditSequence(int a, int b)
{
    m_pcInternal->StartCreditSequence(a, b);
}

// FUNCTION: SWKOTOR 0x005EE1D0
void CClientExoApp::StopCreditSequence()
{
    m_pcInternal->StopCreditSequence();
}

// FUNCTION: SWKOTOR 0x005EE1F0
void CClientExoApp::EnableMouseAtEndOfLoadModuleMovie()
{
    m_pcInternal->EnableMouseAtEndOfLoadModuleMovie();
}

// FUNCTION: SWKOTOR 0x005EE210
void CClientExoApp::AddMovieToMovieQueue(int a, int b)
{
    m_pcInternal->AddMovieToMovieQueue(a, b);
}

// FUNCTION: SWKOTOR 0x005EE220
void CClientExoApp::PlayMovieQueue(int b)
{
    m_pcInternal->PlayMovieQueue(b);
}

// FUNCTION: SWKOTOR 0x005EE2B0
int CClientExoApp::IsValidResolution(int a, int b)
{
    return m_pcInternal->IsValidResolution(a, b);
}

// FUNCTION: SWKOTOR 0x005EE2E0
void CClientExoApp::DontUseThisSetPlayerCreature(unsigned long oid)
{
    m_pcInternal->DontUseThisSetPlayerCreature(oid);
}

// FUNCTION: SWKOTOR 0x005ED490
CExoString CClientExoApp::GetClientName()
{
    return m_pcInternal->m_sClientName;
}

// FUNCTION: SWKOTOR 0x005ED630
void CClientExoApp::SetInStartNewModule(int b)
{
    m_pcInternal->m_bInStartNewModule = b;
}

// FUNCTION: SWKOTOR 0x005ED7E0
int CClientExoApp::CreatureAcquireItem(unsigned long oidCreature, unsigned long oidItem)
{
    unsigned long oid = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidCreature);
    return g_pAppManager->GetServerExoApp()->CreatureAcquireItem(oid, oidItem);
}

// FUNCTION: SWKOTOR 0x005ED8D0
void CClientExoApp::SetVideoMode(int nMode)
{
    m_pcInternal->SetVideoMode(nMode, 0);
}

// FUNCTION: SWKOTOR 0x005ED8F0
void CClientExoApp::SetTexturePack(int nPack)
{
    m_pcInternal->SetTexturePack((unsigned char)nPack, 1);
}

inline CResRef CClientExoAppInternal::GetItemDropSound(unsigned char nType)
{
    if (m_pItemDropSounds) {
        return m_pItemDropSounds[nType];
    }
    return CResRef("");
}

// FUNCTION: SWKOTOR 0x005ED920
CResRef CClientExoApp::GetItemDropSound(unsigned char nType)
{
    return m_pcInternal->GetItemDropSound(nType);
}

// FUNCTION: SWKOTOR 0x005ED9E0
void CClientExoApp::SetStartMenuMusicOnMovieEnd(int b)
{
    m_pcInternal->m_bStartMenuMusicOnMovieEnd = b;
}

// FUNCTION: SWKOTOR 0x005EDAE0
void CClientExoApp::SetAreaNotReadyToDisplay(int b)
{
    m_pcInternal->m_bAreaNotReadyToDisplay = b;
}

// FUNCTION: SWKOTOR 0x005EDB00
CSWCMiniGame* CClientExoApp::GetMiniGame()
{
    CSWCModule* pModule = m_pcInternal->GetModule();
    if (pModule) {
        return pModule->GetMiniGame();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EDCA0
void CClientExoApp::SetDialogSent(int b)
{
    m_pcInternal->m_bDialogSent = b;
}

// FUNCTION: SWKOTOR 0x005EDCB0
void CClientExoApp::SetDialogModelsSent(int b)
{
    m_pcInternal->m_bDialogModelsSent = b;
}

// FUNCTION: SWKOTOR 0x005EDCC0
int CClientExoApp::GetClientRangedAnimation(unsigned long a, unsigned long b, int c, int d)
{
    return m_pcInternal->GetClientRangedAnimation(a, b, 0, 0);
}

// FUNCTION: SWKOTOR 0x005EDD00
CExoString CClientExoApp::GetLoadMusicByModuleName(const CExoString& sModule)
{
    return m_pcInternal->GetLoadMusicByModuleName(sModule);
}

// FUNCTION: SWKOTOR 0x005EDD30
void CClientExoApp::SetLoadMusic(const CExoString& sMusic)
{
    m_pcInternal->m_sLoadMusic = sMusic;
}

// FUNCTION: SWKOTOR 0x005EDD50
void CClientExoApp::SetLoadFromIFO(int b)
{
    m_pcInternal->m_bLoadFromIFO = b;
}

// FUNCTION: SWKOTOR 0x005EDE00
void CClientExoApp::SetFutureLeader(unsigned long oid)
{
    m_pcInternal->m_oidFutureLeader = oid;
}

// FUNCTION: SWKOTOR 0x005EDEB0
CExoString CClientExoApp::GetGUIString(int nId)
{
    return m_pcInternal->GetGUIString(nId);
}

// FUNCTION: SWKOTOR 0x005EDF90
void CClientExoApp::SetAttackMashTutorialTimer(int n)
{
    m_pcInternal->m_nAttackMashTutorialTimer = n;
}

// FUNCTION: SWKOTOR 0x005EDFC0
void CClientExoApp::SetLoadingSaveGame(int b)
{
    m_pcInternal->m_bLoadingSaveGame = b;
}

// FUNCTION: SWKOTOR 0x005EE040
void CClientExoApp::SetLoadScreenHintUpdateTimer(int n)
{
    m_pcInternal->m_nLoadScreenHintUpdateTimer = n;
}

// FUNCTION: SWKOTOR 0x005EE060
void CClientExoApp::SetGamePlayHint(unsigned char b)
{
    m_pcInternal->m_bGamePlayHint = b;
}

// FUNCTION: SWKOTOR 0x005EE080
void CClientExoApp::SetStoryHint(unsigned char b)
{
    m_pcInternal->m_bStoryHint = b;
}

// FUNCTION: SWKOTOR 0x005EE1A0
void CClientExoApp::SetCharacterChangeInProgress(int b)
{
    m_pcInternal->m_bCharacterChangeInProgress = b;
}

// FUNCTION: SWKOTOR 0x005EE200
void CClientExoApp::SetLastClickedOnTarget(unsigned long oid)
{
    m_pcInternal->m_oidLastClickedOnTarget = oid;
}

// FUNCTION: SWKOTOR 0x005EE230
int CClientExoApp::GetInFreeLook()
{
    if (m_pcInternal->GetModule() && m_pcInternal->GetModule()->m_pScene) {
        return m_pcInternal->GetModule()->GetScene()->TestCameraFlag(0x106e) != 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EE280
CExoString CClientExoApp::GetDisplayCharacterForKey(int nKey)
{
    return m_pcInternal->GetDisplayCharacterForKey(nKey);
}

// FUNCTION: SWKOTOR 0x005ED660
CWorldTimer* CClientExoApp::GetActiveTimer(unsigned long oid)
{
    return m_pcInternal->GetActiveTimer(oid);
}
