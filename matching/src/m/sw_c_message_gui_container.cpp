// Container protocol records retain the native item coordinates and state.
class CSWCItem { public:
 char pad_000[0x15c]; unsigned char m_nContainerX, m_nContainerY;
 char pad_15e[0x168-0x15e]; int m_nState168;
 char pad_16c[0x180-0x16c]; unsigned long m_nRepositoryValue;
 void SetStackSize(int);
};
class CClientExoApp { public: CSWCItem* GetItemByGameObjectID(unsigned long); };
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
class CSWCMessage { public:
 unsigned char ReadBYTE(int); unsigned long ReadDWORD(int);
 unsigned long ReadOBJECTIDClient(); int MessageReadOverflow(int);
 int HandleServerToPlayerItemUpdate_Add(int,CSWCItem**);
 int HandleServerToPlayerUpdate_GuiContainer();
};
// FUNCTION: SWKOTOR 0x0064FEF0
int CSWCMessage::HandleServerToPlayerUpdate_GuiContainer()
{
 unsigned char kind = ReadBYTE(8);
 if (MessageReadOverflow(1)) return 0;
 if (kind == 'P') { ReadBYTE(8); }
 else if (kind == 'A') {
  unsigned char x = ReadBYTE(8);
  unsigned char y = ReadBYTE(8);
  if (MessageReadOverflow(1)) return 0;
  CSWCItem* item;
  HandleServerToPlayerItemUpdate_Add(1,&item);
  CSWCItem* added = item;
  added->m_nContainerX = x;
  added->m_nContainerY = y;
  added->m_nState168 = 1;
  return 1;
 }
 else if (kind == 'U') {
  unsigned long id = ReadOBJECTIDClient();
  unsigned long stack = ReadDWORD(32);
  unsigned long value = ReadDWORD(32);
  if (MessageReadOverflow(1)) return 0;
  CSWCItem* item = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(id);
  if (item) {
   item->SetStackSize(stack);
   item->m_nRepositoryValue = value;
  }
  return 1;
 }
 else if (kind == 'D') {
  unsigned long id = ReadOBJECTIDClient();
  if (MessageReadOverflow(1)) return 0;
  g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(id);
  return 1;
 }
 else if (kind == 'M') {
  unsigned char x = ReadBYTE(8);
  unsigned char y = ReadBYTE(8);
  unsigned long id = ReadOBJECTIDClient();
  if (MessageReadOverflow(1)) return 0;
  CSWCItem* item = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(id);
  if (!item) return 0;
  item->m_nContainerX = x;
  item->m_nContainerY = y;
  return 1;
 }
 return 0;
}
