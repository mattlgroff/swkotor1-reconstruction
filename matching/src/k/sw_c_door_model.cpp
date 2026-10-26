#include "k/sw_c_placeable_door.h"
// FUNCTION: SWKOTOR 0x006842E0
int CSWCDoor::LoadModel(const CResRef& res,unsigned char part,int)
{
 if(!m_pModel) {
  CSWCAnimBaseDoor* animation=new CSWCAnimBaseDoor;
  m_pModel=animation;
  animation->m_nD0=m_n118;
  if(!m_pModel) return 0;
 }
 if(!m_pModel->LoadModel(res,part)) return 0;
 CAurObject* model=m_pModel->GetModel(part);
 if(model) {
  model->SetClientObject(this);
  model->SetObjectType(g_nObjectTypeDoor);
  model->SetAnimationEvent(DoorAnimationEvent,g_sEmpty,this,9999.0f,0);
 }
 res.CopyToString(m_sTransition);
 return 1;
}
