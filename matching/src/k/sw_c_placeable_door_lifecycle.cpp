#include "k/sw_c_placeable_door.h"

// FUNCTION: SWKOTOR 0x00682BE0
void CSWCPlaceable::UnloadLight()
{
 if(g_pAppManager->m_pClientExoApp->GetModule()) {
  CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->GetArea();
  if(area) area->RemovePlaceableObjectLight(m_oid,0);
 }
 if(m_pLight) {
  m_pLight->AttachToObject(0,0,0); m_pLight->SetVisible(0);
  delete m_pLight; m_pLight=0;
 }
}
// FUNCTION: SWKOTOR 0x006831B0
int CSWCPlaceable::GetIsHostile()
{
 if(ServerInline()) return ServerInline()->m_bHostile;
 return 0;
}
// FUNCTION: SWKOTOR 0x00683200
CSWCPlaceable::~CSWCPlaceable()
{
 UnloadLight();
 if(m_pModel) { m_pModel->UnloadModel(); delete m_pModel; m_pModel=0; }
}
// FUNCTION: SWKOTOR 0x00683630
int CSWCPlaceable::PlayAnimation(unsigned short anim,float speed,int a)
{
 CSWSPlaceable* server=ServerInline();
 if(server && server->m_pCreature) return 1;
 if(anim==0x132 || anim==0x2758) UnloadLight();
 return CSWCObject::PlayAnimation(anim,speed,a);
}
// FUNCTION: SWKOTOR 0x00683BB0
CSWCDoor::CSWCDoor() : CSWCObject(g_nObjectTypeDoor)
{
 m_nAnimation=0x2726;
 unsigned long day,tick;
 CClientExoApp* app=g_pAppManager->m_pClientExoApp;
 app->GetWorldTimer()->GetWorldTime(&day,&tick);
 m_nDay=day; m_nTick=tick;
 m_n108=0; m_n10c=0; m_n110=0; m_n114=0; m_n104=0;
 m_n120=0; m_bTransition=0; m_n118=1;
 m_n124=0xff; m_n125=0xff;
 m_sTransition=g_sEmpty;
 m_n128=0; m_n138=0;
}
// FUNCTION: SWKOTOR 0x00683F90
CSWCDoor::~CSWCDoor()
{
 if(m_pModel) { m_pModel->UnloadModel(); delete m_pModel; m_pModel=0; }
}
