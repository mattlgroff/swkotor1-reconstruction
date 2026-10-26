class CExoString { public: char* m_sString; unsigned long m_nBufferLength; CExoString(const char*); ~CExoString(); };
class CUnknown004b13e0 { public:
 int m_bActive; unsigned long m_nKind,m_nStep,m_nProgress;
 int m_n10,m_bDone;
 void FUN_004b13e0(unsigned long);
};
class CClientExoAppInternal { public:
 char pad_000[0x188]; int m_bLoadInProgress;
 char pad_18c[0x240-0x18c]; int m_bSuppressLoad;
 char pad_244[0x264-0x244]; unsigned long m_nLoadError;
};
class CClientExoApp { public:
 void* m_pVtable; CClientExoAppInternal* m_pcInternal;
 void CallUnknown5FCD10(int,int,const CExoString&);
 int GetLoadInProgress() { return m_pcInternal->m_bLoadInProgress; }
 int GetSuppressLoad() { return m_pcInternal->m_bSuppressLoad; }
 void SetSuppressLoad(int n) { m_pcInternal->m_bSuppressLoad=n; }
 void SetLoadError(unsigned long n) { m_pcInternal->m_nLoadError=n; }
};
class CServerExoApp;
class CAppManager { public:
 void* m_pUnknown00; CClientExoApp* m_pClientExoApp; CServerExoApp* m_pServerExoApp;
 char pad_00c[8]; CUnknown004b13e0* m_pLoading;

};
extern CAppManager* g_pAppManager;
class CSWCMessage { public:
 int MessageReadOverflow(int); int MessageReadUnderflow(int); unsigned long ReadDWORD(int);
 int HandleServerToPlayerLoadBar(unsigned char);
};
// FUNCTION: SWKOTOR 0x0064EDD0
int CSWCMessage::HandleServerToPlayerLoadBar(unsigned char minor)
{
 if (MessageReadOverflow(1)) return 0;
 switch(minor) {
 case 1: {
  unsigned long event=ReadDWORD(32);
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (g_pAppManager->m_pClientExoApp->GetLoadInProgress() && event==2) {
   g_pAppManager->m_pClientExoApp->SetSuppressLoad(1);
   return 1;
  }
  if (g_pAppManager->m_pServerExoApp) return 1;
  if (event==2) { CExoString text(""); g_pAppManager->m_pClientExoApp->CallUnknown5FCD10(0xe1,0,text); }
  g_pAppManager->m_pLoading->FUN_004b13e0(event);
  g_pAppManager->m_pLoading->m_nStep=0;
  g_pAppManager->m_pLoading->m_nProgress=0;
  return 1;
 }
 case 2: {
  unsigned long event=ReadDWORD(32);
  unsigned long progress=ReadDWORD(32);
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (g_pAppManager->m_pClientExoApp->GetSuppressLoad()==1 || g_pAppManager->m_pServerExoApp) return 1;
  g_pAppManager->m_pLoading->m_nStep=event;
  if (progress!=0xffffffff) g_pAppManager->m_pLoading->m_nProgress=progress;
  return 1;
 }
 case 3: {
  unsigned long event=ReadDWORD(32);
  unsigned long error=ReadDWORD(3);
  if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
  if (g_pAppManager->m_pClientExoApp->GetSuppressLoad()==1) {
   g_pAppManager->m_pClientExoApp->SetSuppressLoad(0);
   return 1;
  }
  g_pAppManager->m_pClientExoApp->SetLoadError(error);
  if (g_pAppManager->m_pServerExoApp) return 1;
  if (event==2) { CExoString text(""); g_pAppManager->m_pClientExoApp->CallUnknown5FCD10(0xe2,0,text); }
  g_pAppManager->m_pLoading->m_bDone=1;
  return 1;
 }
 default: return 0;
 }
 return 1;
}
