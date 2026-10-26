#include "m/sw_c_message_updates.h"
// FUNCTION: SWKOTOR 0x0064DEE0
int CSWCMessage::HandleServerToPlayerInventory(unsigned char minor)
{
 CGameObject* object=0;
 unsigned long id=ReadOBJECTIDClient();
 if (MessageReadOverflow(1)) return 0;
 CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 if (!array) return 0;
 switch(minor) {
 case 3: case 4: {
  if (MessageReadUnderflow(1)) return 0;
  if (array->GetGameObject(id,&object)==GAME_OBJECT_VALID) {
   CSWCItem* item=object->AsSWCItem();
   if (item) {
    if (minor!=4) {
     item->PlayDropSound();
     CSWCCreature* owner=item->GetEquippedBy();
     if (owner) SwitchWeaponEvent(0,0,owner);
    }
    item->m_nInventoryState=0;
    if (minor==3 && g_pAppManager->m_pClientExoApp->GetPlayerCreature()->GetEquippedItemID(item->m_nInventorySlot)==item->m_idItem && item->m_nInventorySlot!=1)
     g_pAppManager->m_pClientExoApp->GetPlayerCreature()->EquipItem(item->m_nInventorySlot,0);
   }
  }
  return 1;
 }
 case 1: case 2: {
  unsigned long slot=ReadDWORD(32);
  int instant;
  if (minor==1) instant=ReadBOOL();
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (minor==1 && g_pAppManager->m_pClientExoApp->GetPlayerCreature()) {
   g_pAppManager->m_pClientExoApp->GetPlayerCreature()->SetLastInstantEquip(slot,instant);
   if (array->GetGameObject(id,&object)==GAME_OBJECT_VALID) {
    CSWCItem* item=object->AsSWCItem();
    if (item && g_pAppManager->m_pClientExoApp->GetPlayerCreature()->GetEquippedItemID(item->m_nInventorySlot)!=item->m_idItem && slot!=1)
     g_pAppManager->m_pClientExoApp->GetPlayerCreature()->EquipItem(slot,item);
   }
  }
  return 1;
 }
 case 5: case 6: {
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (array->GetGameObject(id,&object)==GAME_OBJECT_VALID && object) {
   CSWCItem* item=object->AsSWCItem();
   if (item) {
    if (minor==6) item->m_nInventoryState=0;
    else {
     item->PlayDropSound();
     item->SetEquipped(0);
     if (item->m_nPendingSlot!=0xffffffff) {
      CSWCMessage* message=g_pAppManager->m_pClientExoApp->GetSWCMessage();
      if (message) message->SendPlayerToServer_0C_01(item->m_idItem,item->m_nPendingSlot);
     } else item->m_nInventoryState=0;
     item->m_nPendingSlot=0xffffffff;
    }
   }
  }
  return 1;
 }
 case 15:
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (array->GetGameObject(id,&object)==GAME_OBJECT_VALID && object) {
   CSWCItem* item=object->AsSWCItem();
   if (item) item->PlayDropSound();
  }
  return 1;
 case 7: case 8: {
  int instant;
  if (minor==7) instant=ReadBOOL();
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (array->GetGameObject(id,&object)==GAME_OBJECT_VALID && object) {
   CSWCItem* item=object->AsSWCItem();
   if (item) {
    if (minor!=8 && !instant) item->PlayDropSound();
    item->m_nInventoryState=0;
    if (minor==7 && g_pAppManager->m_pClientExoApp->GetPlayerCreature()->GetEquippedItemID(item->m_nInventorySlot)==item->m_idItem) {
     if (item->m_nInventorySlot!=1) {
      g_pAppManager->m_pClientExoApp->GetPlayerCreature()->SetLastInstantEquip(item->m_nInventorySlot,instant);
      g_pAppManager->m_pClientExoApp->GetPlayerCreature()->EquipItem(item->m_nInventorySlot,0);
     }
     if (item->m_nInventorySlot!=0xffffffff) item->m_nInventorySlot=0xffffffff;
    }
   }
  }
  return 1;
 }
 case 14:
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  object=g_pAppManager->m_pClientExoApp->GetGameObject(id);
  if (object) { CSWCItem* item=object->AsSWCItem(); if (item) item->m_nInventoryState=0; }
  return 1;
 default: return 0;
 }
 return 1;
}
