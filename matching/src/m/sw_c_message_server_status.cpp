class CExoString { public:
 char* m_sString; unsigned long m_nBufferLength; CExoString(); ~CExoString();
 void __cdecl Format(const char*,...); int GetLength() const; char* CStr() const;
};
class CResRef { public: char m_ref[16]; CResRef(const CExoString&); };
class CNetLayer { public: int SendMessageToPlayer(unsigned long,unsigned char*,unsigned long,unsigned long); };
class CGuiInGame;
class CClientExoApp { public:
 virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C(); virtual CNetLayer* GetNetLayer();
 CGuiInGame* GetGuiInGame(); void ServerModeChange(unsigned char);
 int GetGameOver(); void SetGameOver(int); int GetPlayerCharacterSet(); CExoString* GetPlayerCharacterName();
};
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
extern unsigned long PLAYERID_SERVER;
class CSWCMessage { public:
 int MessageReadOverflow(int); int MessageReadUnderflow(int);
 int SendPlayerToServerLogin_LoginLocalCharacter(const CResRef&,int);
 int HandleServerToPlayerServerStatus(unsigned char);
};
// FUNCTION: SWKOTOR 0x0064F280
int CSWCMessage::HandleServerToPlayerServerStatus(unsigned char minor)
{
 CExoString text;
 if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
 g_pAppManager->m_pClientExoApp->GetGuiInGame();
 switch(minor) {
 case 3:
  g_pAppManager->m_pClientExoApp->ServerModeChange(2);
  if (g_pAppManager->m_pClientExoApp->GetGameOver()) g_pAppManager->m_pClientExoApp->SetGameOver(0);
  else if (g_pAppManager->m_pClientExoApp->GetPlayerCharacterSet()) {
   CResRef ref(*g_pAppManager->m_pClientExoApp->GetPlayerCharacterName());
   SendPlayerToServerLogin_LoginLocalCharacter(ref,0);
  }
success:
  return 1;
 case 2: g_pAppManager->m_pClientExoApp->ServerModeChange(0); goto success;
 case 1: {
  g_pAppManager->m_pClientExoApp->ServerModeChange(1);
  text.Format("%c%s.%s",'s',"Module","Run");
  CClientExoApp* client=g_pAppManager->m_pClientExoApp;
  client->GetNetLayer()->SendMessageToPlayer(PLAYERID_SERVER,(unsigned char*)text.CStr(),text.GetLength(),0);
  goto success;
 }
 default: return 0;
 }
 return 1;
}
