#include "k/sw_c_placeable_door.h"
// FUNCTION: SWKOTOR 0x006823F0
int CSWCPlaceable::LoadModel(const CResRef& res,unsigned char part,int)
{
 if(!m_pModel) { m_pModel=new CSWCAnimBasePlaceable; if(!m_pModel) return 0; }
 if(!m_pModel->LoadModel(res,part)) return 0;
 CAurObject* model=m_pModel->GetModel(part);
 if(model) { model->SetClientObject(this); model->SetObjectType(g_nObjectTypePlaceable); }
 res.CopyToString(m_sLight);
 m_sLight=m_sLight.SubString(4,-1);
 Quaternion rotation; Vector position;
 CExoString node;
 node=m_sLight+"_head_hit";
 if(model->GetNodeTransform(node.CStr(),&position,&rotation)) m_fCanSeeHeight=position.z;
 else m_fCanSeeHeight=1.5f;
 return 1;
}
