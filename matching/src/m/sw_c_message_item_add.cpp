#include "m/sw_c_message_updates.h"
// FUNCTION: SWKOTOR 0x0064E7B0
int CSWCMessage::HandleServerToPlayerItemUpdate_Add(int flag,CSWCItem** out)
{
 char gender='M';
 unsigned long id=ReadOBJECTIDClient();
 if (MessageReadOverflow(1)) return 0;
 int base;
 unsigned char variation,a,b;
 HandleServerToPlayerItemAppearance(&base,&variation,&a,&b);
 CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
 CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 CGameObject* found;
 if (array->GetGameObject(id,&found)==GAME_OBJECT_VALID) {
  CSWCItem* item=found->AsSWCItem();
  item->m_nState168=0;
  int baseIndex=item->m_nBaseItem;
  CSWBaseItem* baseInfo=g_pRules->GetBaseItemArray()->GetBaseItem(baseIndex);
  if (baseInfo && baseInfo->m_nModelType==1) g_pAppManager->m_pClientExoApp->GetPlayerCreature();
  if (!flag) {
   if (item->m_oidPossessor==0x7f000000) {
    item->SetVisibilityOnSetArea(area);
    item->SetEquipped(area->m_nAreaState);
    CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
    ai->AddObject(item);
    HandleServerToPlayerActiveItemProperties(id);
    return 1;
   }
   CSWCCreature* holder=item->GetEquippedBy();
   if (holder) {
    if (holder->GetEquippedItemID(0x10)==id) holder->EquipItem(0x10,0);
    if (holder->GetEquippedItemID(0x20)==id) holder->EquipItem(0x20,0);
    if (holder->GetEquippedItemID(1)==id) holder->EquipItem(1,0);
   } else {
    CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
    ai->AddObject(item);
   }
   item->SetEquippedBy(0);
   CGameObject* previous;
   if (array->GetGameObject(item->m_oidPossessor,&previous)==GAME_OBJECT_VALID && previous->AsSWCCreature())
    SwitchWeaponEvent(0,0,previous->AsSWCCreature());
   item->SetArea(0);
   item->SetEquipped(area->m_nAreaState);
   HandleServerToPlayerActiveItemProperties(id);
   return 1;
  }
  if (flag==1) {
   item->SetArea(0);
   *out=item;
   HandleServerToPlayerActiveItemProperties(id);
   return 1;
  }
 }
 CSWCCreature* player=g_pAppManager->m_pClientExoApp->GetPlayerCreature();
 if (player && player->GetAppearance()) gender=player->GetAppearance()->GetGender();
 CSWCItem* item=CreateItem(base,variation,gender);
 if (!item) return 0;
 item->SetObjectID(id);
 if (flag==1) { item->SetEquipped(0); item->SetArea(0); *out=item; }
 else {
  item->SetEquipped(area->m_nAreaState);
  item->SetArea(area);
  CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
  ai->AddObject(item);
 }
 if (array->AddExternalObject(&id,item,1)!=GAME_OBJECT_VALID) return 0;
 HandleServerToPlayerActiveItemProperties(id);
 return 1;
}
