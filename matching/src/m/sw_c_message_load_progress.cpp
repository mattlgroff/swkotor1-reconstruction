class CExoString { public: char* m_sString; unsigned long m_nBufferLength; CExoString(); ~CExoString(); };
class CResRef { public: char m_ref[16]; void CopyToString(CExoString&) const; };
class CSWCModule { public: char pad_000[0x28]; CResRef m_resArea; };
class CClientExoAppInternal { public: char pad_000[0x3e4]; unsigned char m_nLoadSteps[5]; };
class CClientExoApp { public: void* m_pVTable; CClientExoAppInternal* m_pcInternal;
 CSWCModule* GetModule(); void SetLoadBarProgress(int,int);
 unsigned char GetTutorialFlag(int); // established 0x005EDFF0 symbol
};
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
void RunFrame(float,int,int);
class CSWCMessage { public: unsigned char ReadBYTE(int); int MessageReadOverflow(int);
 int SendPlayerToServer_04_03(); int HandleServerToPlayerUpdate_LoadProgress(); };
// FUNCTION: SWKOTOR 0x0064DCF0
int CSWCMessage::HandleServerToPlayerUpdate_LoadProgress()
{
 unsigned char stage = ReadBYTE(8);
 unsigned char readTotal = ReadBYTE(8);
 unsigned char total = readTotal < 1 ? 1 : readTotal;
 if (MessageReadOverflow(1)) return 0;
 CSWCModule* module = g_pAppManager->m_pClientExoApp->GetModule();
 CClientExoApp* client = g_pAppManager->m_pClientExoApp;
 client->SetLoadBarProgress((int)(client->GetTutorialFlag(3) +
  (float)(client->GetTutorialFlag(2) +
   (float)(client->GetTutorialFlag(1) +
    (float)(client->GetTutorialFlag(0) +
     (float)(client->GetTutorialFlag(4) * ((float)stage / (float)total)))))),1);
 RunFrame(1.0f/30.0f,0,0);
 if (stage == total) {
  CExoString area;
  CResRef ref = module->m_resArea;
  ref.CopyToString(area);
  SendPlayerToServer_04_03();
 }
 return 1;
}
