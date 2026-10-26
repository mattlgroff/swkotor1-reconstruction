#include "m/sw_c_message_object_adds.h"
// FUNCTION: SWKOTOR 0x0064F4B0
int CSWCMessage::HandleServerToPlayerTriggerUpdate_Add()
{
 CExoString name,text;
 int flag=0;
 unsigned long id=ReadOBJECTIDClient();
 { CResRef sound; name=ReadCExoLocStringClient(sound,0); }
 int transition=ReadBOOL();
 int hostile=ReadBOOL();
 unsigned char cursor=ReadBYTE(8),row;
 if (transition) { flag=ReadBOOL(); row=ReadBYTE(8); }
 float width=ReadFLOAT(1.0f,32);
 { CResRef sound; text=ReadCExoLocStringClient(sound,0); }
 unsigned char count=ReadBYTE(8);
 if (MessageReadOverflow(1)) return 0;
 CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
 CSWCTrigger* trigger=new CSWCTrigger;
 if (transition) {
  CExoString model;
  int found;
  { CExoString column("Model");
   C2DA* table=g_pRules->Get2DArrays()->GetTraps();
   found=table->GetCExoStringEntry(row,column,&model);
  }
  if (!found) model="v_mnfrag";
  CResRef ref(model);
  trigger->LoadModel(ref,255,9);
 } else { CResRef ref("EMPTY"); trigger->LoadModel(ref,255,9); }
 trigger->SetObjectID(id);
 trigger->SetArea(area);
 if (array->AddExternalObject(&id,trigger,1)!=GAME_OBJECT_VALID) return 0;
 CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
 ai->AddObject(trigger);
 trigger->m_sName=name;
 if (transition) { trigger->m_bAreaTransition=1; trigger->m_nTrapType=row; }
 else if (hostile) trigger->m_bHostile=1;
 trigger->m_n114=flag;
 trigger->SetCursor(cursor);
 trigger->m_fHighlightWidth=width;
 if (trigger->m_nPointCapacity < count) {
  trigger->m_pPoints=new Vector[count];
  trigger->m_nPointCapacity=count;
 }
 for (unsigned long i=0; i<count; ++i) {
  float x=ReadFLOAT(1.0f,32),y=ReadFLOAT(1.0f,32),z=ReadFLOAT(1.0f,32)+0.05f;
  if (MessageReadOverflow(1)) return 0;
  trigger->m_pPoints[i].x=x; trigger->m_pPoints[i].y=y; trigger->m_pPoints[i].z=z;
 }
 if (transition) trigger->AttachToScene(area->m_pScene);
 else if (hostile) { trigger->AddToRenderList(2); trigger->DisplayTrigger(1); }
 else trigger->AddToRenderList(1);
 trigger->SpecialDisplay(g_bShowAllTriggers);
 trigger->m_sText=text;
 return 1;
}
