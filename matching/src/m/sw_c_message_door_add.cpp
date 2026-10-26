#include "m/sw_c_message_object_adds.h"
// FUNCTION: SWKOTOR 0x0064CF30
int CSWCMessage::HandleServerToPlayerDoorUpdate_Add()
{
 CExoString model,name,description;
 int visible=1;
 CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
 unsigned long id=ReadOBJECTIDClient();
 int ignore=ReadBOOL();
 unsigned char generic=ReadBYTE(8),doorType;
 if (!generic) doorType=ReadBYTE(8);
 name=ReadCExoLocStringClient(CResRef(),0);
 ReadBOOL();
 unsigned short animation=ReadWORD(16);
 int a=ReadBOOL(),b=ReadBOOL(),c=ReadBOOL(),transition=ReadBOOL();
 description=ReadCExoLocStringClient(CResRef(),0);
 if (MessageReadOverflow(1)) return 0;
 if (generic) {
  { CExoString column("Model");
   C2DA* table=g_pRules->Get2DArrays()->GetGenericDoors();
   table->GetCExoStringEntry(generic,column,&model);
  }
  { CExoString column("VisibleModel");
   C2DA* table=g_pRules->Get2DArrays()->GetGenericDoors();
   table->GetINTEntry(generic,column,&visible);
  }
 } else {
  CExoString column("ModelName");
  C2DA* table=g_pRules->Get2DArrays()->GetDoors();
  table->GetCExoStringEntry(doorType,column,&model);
 }
 CSWCDoor* door=g_pAppManager->m_pClientExoApp->GetDoorByGameObjectID(id);
 if (!door) {
  door=new CSWCDoor;
  CResRef ref(model);
  door->LoadModel(ref,255,5);
  door->SetObjectID(id);
  door->SetArea(area);
  if (array->AddExternalObject(&id,door,1)!=GAME_OBJECT_VALID) return 0;
  CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
  ai->AddObject(door);
 } else door->SetVisibilityOnSetArea(area);
 door->AttachToScene(area->m_pScene);
 door->m_sName=name;
 if (animation==0x2726) door->SetState(0);
 else if (animation==0x2742) door->SetState(1);
 else if (animation==0x2743) door->SetState(2);
 else if (animation==0x2758) door->SetState(3);
 door->SetAnimationWithoutTrigger(animation);
 door->m_n108=a; door->m_n10C=b; door->m_n104=c;
 door->SetIsAreaTransition(transition);
 door->m_nGenericType=generic; door->m_nDoorType=doorType;
 if (!visible) { door->m_n104=1; door->SetState(3); }
 door->m_bIgnoreHitCheck=ignore;
 if (ignore) g_pAppManager->m_pClientExoApp->AddObjectToHitcheckIgnoreList(door->m_idSelf);
 door->m_sDescription=description;
 return 1;
}
