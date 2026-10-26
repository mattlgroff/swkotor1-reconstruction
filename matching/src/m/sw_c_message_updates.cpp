#include "m/sw_c_message_updates.h"

// FUNCTION: SWKOTOR 0x0064CC90
int CSWCMessage::HandleServerToPlayerCreatureUpdate_Delete()
{
 unsigned long id = ReadOBJECTIDClient();
 int instant = ReadBOOL();
 int snap = ReadBOOL();
 unsigned long fade = ReadDWORD(32);
 int dead = ReadBOOL();
 if (MessageReadOverflow(1)) return 0;
 int time = instant ? 0 : 45000;
 CSWCCreature* creature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(id);
 if (!creature) return 1;
 CSWSCreature* server = creature->GetServerCreature();
 if (!dead) {
  if (snap) {
   creature->SetFadeState(0);
   creature->SetDesiredFadeState(0,1,time,0);
  } else {
   creature->SetFadeState(1);
   creature->SetDesiredFadeState(0,1,time,fade);
  }
 } else if (creature->m_pArea) {
  int bodyBag;
  int found;
  { CExoString column("Body_Bag");
   unsigned short row = creature->m_pAppearance->m_nAppearanceType;
   C2DA* table = g_pRules->Get2DArrays()->GetAppearance();
   found = table->GetINTEntry(row,column,&bodyBag);
  }
  int corpse;
  if (found) {
   { CExoString column("Corpse");
    C2DA* table = g_pRules->Get2DArrays()->GetBodyBag();
    found = table->GetINTEntry(bodyBag,column,&corpse);
   }
   if (found && corpse) goto no_corpse;
  }
  CSWCArea* area = creature->m_pArea;
  area->AddCorpse(creature->m_idSelf);
 }
no_corpse:
 creature->ClearActions();
 creature->SetCombatState(0);
 creature->m_nState1B0 = 0;
 if (server || !creature->m_pArea) creature->m_bFading = 1;
 return 1;
}

// FUNCTION: SWKOTOR 0x0064F960
int CSWCMessage::HandleServerToPlayerTriggerUpdate_Delete()
{
 unsigned long id = ReadOBJECTIDClient();
 unsigned char kind = ReadBYTE(8);
 if (MessageReadOverflow(1)) return 0;
 CSWCTrigger* trigger = g_pAppManager->m_pClientExoApp->GetTriggerByGameObjectID(id);
 if (!trigger) return 0;
 trigger->RemoveFromRenderList();
 if (kind == 1) {
  CExoString sound;
  unsigned char row = trigger->m_nTrapType;
  if (row != 255) {
   int found;
   { CExoString column("ExplosionSound");
    C2DA* table = g_pRules->Get2DArrays()->GetTraps();
    found = table->GetCExoStringEntry(row,column,&sound);
   }
   if (found) {
    CResRef ref(sound);
    g_pExoSound->PlayOneShotSoundAt(ref,trigger->m_vPosition,0.0f,0x16,0,0,0,0);
   }
  }
  trigger->PlayAnimation(0x15c,0,0);
 } else if (kind == 2) { trigger->PlayAnimation(0x15d,0,0); }
 else {
  CGameObject* object;
  CClientExoApp* client = g_pAppManager->m_pClientExoApp;
  CGameObjectArray* array = client->GetGameObjectArray();
  if (array->Delete(id,&object) != GAME_OBJECT_VALID || !object) return 0;
  CSWCTrigger* removed = object->AsSWCTrigger();
  if (removed) delete removed;
 }
 return 1;
}

// FUNCTION: SWKOTOR 0x00654B10
int CSWCMessage::HandleServerToPlayerItemScript_Add()
{
 unsigned long id = ReadOBJECTIDClient();
 if (MessageReadOverflow(1)) return 0;
 int base;
 unsigned char variation, a, b;
 HandleServerToPlayerItemAppearance(&base,&variation,&a,&b);
 CSWCArea* area = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
 CGameObjectArray* array = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 CGameObject* object;
 CSWCItem* item;
 if (array->GetGameObject(id,&object) != GAME_OBJECT_VALID) {
  item = CreateItem(base,variation,0x4d);
  if (!item) return 0;
  item->SetObjectID(id);
  item->SetArea(area);
  if (array->AddExternalObject(&id,item,1) != GAME_OBJECT_VALID) return 0;
 } else item = object->AsSWCItem();
 item->m_nState168 = 0;
 item->SetVisibilityOnSetArea(area);
 item->SetEquipped(area->m_nAreaState);
 CClientAIMaster* ai = g_pAppManager->m_pClientExoApp->GetClientAIMaster();
 ai->AddObject(item);
 HandleServerToPlayerActiveItemProperties(id);
 if (!item->LoadModel(item->m_resModel,255,0)) return 0;
 int baseItem = item->m_nBaseItem;
 if (g_pRules->GetBaseItemArray()->GetBaseItem(baseItem)->m_pModelHook) {
  CResRef hook("default");
  item->AttachHook(255,1,hook);
 }
 return 1;
}
