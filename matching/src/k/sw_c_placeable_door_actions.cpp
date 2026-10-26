#include "k/sw_c_placeable_door.h"

// FUNCTION: SWKOTOR 0x006825F0
void CSWCPlaceable::UnloadModel()
{
 if (m_pModel) { m_pModel->UnloadModel(); delete m_pModel; m_pModel = 0; }
}
// FUNCTION: SWKOTOR 0x00682620
void CSWCPlaceable::PlayAnimationAction(unsigned long anim,int a,int b)
{
 float speed=m_fSpeed;
 int duration=(int)(GetAnimationLength(anim)/speed);
 PlayAnimationTimed(anim,duration,speed,a,0);
}
// FUNCTION: SWKOTOR 0x00682660
void CSWCPlaceable::ActionMenuUse(int,CSWCCreature*)
{
 CSWCMessage* message=g_pAppManager->GetClientExoApp()->GetMessage();
 if (g_pAppManager->GetClientExoApp()->GetInGameGui()->CanPerformAction())
  message->SendPlayerToServer_06_0B(m_oid);
}
// NONMATCH: tutorial-call register allocation / global-load scheduling (three shapes tried).
// FUNCTION: SWKOTOR 0x006826A0
void CSWCPlaceable::ActionMenuOpen(int,CSWCCreature* creature)
{
 if (creature && !g_pAppManager->m_pClientExoApp->ShowTutorialWindow(0x21,creature->m_oid,m_oid,0))
  creature->PerformAttack(m_oid);
}
// FUNCTION: SWKOTOR 0x00683D00
void CSWCDoor::SetAnimation(int anim)
{
 m_nAnimation=anim;
 if(IsAnimationLooping(anim)) SetLoopingAnimation(anim);
 m_pModel->PlayAnim(anim,0.0f,0,0);
}
// FUNCTION: SWKOTOR 0x00683D40
void CSWCDoor::SetOpenState(unsigned char state)
{
 CAurObject* model=GetModel(0xff,1);
 m_nOpenState=state;
 if(model) {
  if(state!=0) { if(state>0 && state<=3) model->SetIgnoreHitCheck(1); }
  else model->SetIgnoreHitCheck(0);
 }
}
// FUNCTION: SWKOTOR 0x00683D90
void CSWCDoor::ActionMenuUse(int,CSWCCreature* creature)
{
 if(creature) creature->ClearAllActions();
 CSWCMessage* message=g_pAppManager->GetClientExoApp()->GetMessage();
 if(m_nAnimation==0x2726) {
  message->SendPlayerToServer_06_03(m_oid,0x2725);
  g_pAppManager->GetClientExoApp()->DelayNextEnemyAutoPause();
 } else if(m_nAnimation==0x2742 || m_nAnimation==0x2743)
  message->SendPlayerToServer_06_03(m_oid,0x2726);
}
// FUNCTION: SWKOTOR 0x00683E00
void CSWCDoor::PlayAnimationAction(unsigned long anim,int a,int b)
{
 int duration;
 if(m_fSpeed!=0.0f) duration=(int)(GetAnimationLength(anim)/fabs(m_fSpeed));
 else duration=(int)GetAnimationLength(anim);
 PlayAnimationTimed(anim,duration,m_fSpeed,a,0);
}
// FUNCTION: SWKOTOR 0x00683E50
void CSWCDoor::ActionMenuLock(int,CSWCCreature* creature)
{
 if(creature) creature->ClearAllActions();
 CSWCMessage* message=g_pAppManager->GetClientExoApp()->GetMessage();
 message->SendPlayerToServer_06_0C(m_oid,0x7f000000);
 g_pAppManager->GetClientExoApp()->DelayNextEnemyAutoPause();
}
// NONMATCH: tutorial-call register allocation / global-load scheduling (three shapes tried).
// FUNCTION: SWKOTOR 0x00683E90
void CSWCDoor::ActionMenuOpen(int,CSWCCreature* creature)
{
 if(creature) {
  unsigned long target=m_oid, actor=creature->m_oid;
  if(!g_pAppManager->m_pClientExoApp->ShowTutorialWindow(0x21,actor,target,1)) {
  creature->ClearAllActions(); creature->PerformAttack(m_oid);
  g_pAppManager->m_pClientExoApp->DelayNextEnemyAutoPause();
  }
 }
}
