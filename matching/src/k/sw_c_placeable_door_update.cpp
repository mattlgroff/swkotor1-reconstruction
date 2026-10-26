#include "k/sw_c_placeable_door.h"
// FUNCTION: SWKOTOR 0x00682270
CSWCPlaceable::CSWCPlaceable() : CSWCObject(g_nObjectTypePlaceable)
{
 m_nAnimation=0x275c;
 unsigned long day,tick;
 CClientExoApp* app=g_pAppManager->m_pClientExoApp;
 app->GetWorldTimer()->GetWorldTime(&day,&tick);
 m_nDay=day; m_nTick=tick;
 m_n11c=0; m_n124=0; m_n110=0; m_n114=0; m_n118=0; m_n128=0;
 m_n12c=1; m_n130=1; m_n13c=0; m_n120=0; m_nAppearance=0xff;
 m_n134=-1; m_n138=0;
 m_sName=g_sEmpty; m_sModel=g_sEmpty; m_sLight=g_sEmpty;
 m_pLight=0; m_n140=0;
}
// FUNCTION: SWKOTOR 0x006823C0
CExoString CSWCPlaceable::GetName()
{
 return m_sName;
}
// FUNCTION: SWKOTOR 0x00682DD0
void CSWCPlaceable::ActionMenuLock(int,CSWCCreature*)
{
 CSWCMessage* message=g_pAppManager->m_pClientExoApp->GetMessage();
 if(message) message->SendPlayerToServer_06_0C(m_oid,0x7f000000);
}
// Descriptive name for the model's cdecl animation-event callback.
// FUNCTION: SWKOTOR 0x00683F00
void __cdecl DoorAnimationEvent(void*,const char* event,CSWCDoor* self)
{
 if(_stricmp(event,"door")) {
  if(_strnicmp(event,"done",4)) return;
  event+=4;
  if(_strnicmp(event,"opening",7) && _strnicmp(event,"closing",7)) return;
 }
 CSWSDoor* door=0;
 if(self->GetServerObject()) door=self->GetServerObject()->AsSWCDoor();
 if(door) door->MoveToNextOpenState();
}
#define UPDATE_AI_BODY(extra) \
 unsigned long day,tick,elapsedDay,elapsedTick; \
 CClientExoApp* app=g_pAppManager->m_pClientExoApp; \
 app->GetWorldTimer()->GetWorldTime(&day,&tick); \
 app=g_pAppManager->m_pClientExoApp; \
 app->GetWorldTimer()->SubtractWorldTimes(day,tick,m_nDay,m_nTick,&elapsedDay,&elapsedTick); \
 m_nElapsed=elapsedTick; \
 if(m_nCountdown) { if(m_nCountdown>elapsedTick) m_nCountdown-=elapsedTick; else m_nCountdown=0; } \
 UpdatePopUpBubble(); UpdateAnimation(); UpdateVisualEffectsList(); EmptyUpdate(); \
 extra \
 if(FadeObject()) { \
  CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster(); \
  ai->GetAIList()->RemoveObject((CClientObject*)this); \
  CGameObject* removed; \
  unsigned long oid=m_oid; \
  app=g_pAppManager->m_pClientExoApp; \
  if(app->GetGameObjectArray()->Delete(oid,&removed)==g_nObjectArraySuccess && removed) { delete this; return; } \
 } \
 m_nDay=day; m_nTick=tick;
// FUNCTION: SWKOTOR 0x006836C0
void CSWCPlaceable::UpdateAI() { UPDATE_AI_BODY(;) }
// FUNCTION: SWKOTOR 0x00684CD0
void CSWCDoor::UpdateAI() { UPDATE_AI_BODY(UpdateInteraction();) }
