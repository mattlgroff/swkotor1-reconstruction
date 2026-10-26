// GUI panel and camera storage/forwarding family. Larger layout is local,
// leaving the compact shared CGuiInGame header's size unchanged.
class CExoString;
struct Vector { float x,y,z; };
struct CSWPlaceableCamera { char m_data[0x30]; };
struct CSWGuiExtent { int left,top,width,height; };
class CSWGuiPanel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void UpdateFonts();
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7c(); virtual void ShowDialogMessage();
    CSWGuiExtent m_extent;
};
class CSWGuiManager {
public:
    int PanelExists(CSWGuiPanel* pPanel); // 0x0040BD70
};
class CSWGuiMainInterface : public CSWGuiPanel {
public:
    char pad_0x14[0x5d04-0x14];
    unsigned long m_nMapFlags;
    char pad_0x5d08[0xa458-0x5d08];
    unsigned char m_nCombatBarHeight;
    void AddFloatyText(unsigned long oid, const CExoString& text, const Vector& position, float duration); // 0x0068B7C0
    void SetCombatMessage(unsigned long message); // 0x00687700, descriptive inherited name
};
class CSWGuiEquipment : public CSWGuiPanel {
public:
    char pad_0x14[0x4274-0x14];
    signed char m_nNPC;
};
class CSWGuiFade : public CSWGuiPanel {
public:
    char pad_0x14[0x4c-0x14]; float m_fAlpha;
};
class CGuiInGame {
public:
    char pad_0x0[0xc];
    union { CSWGuiEquipment* m_pEquipment; CSWGuiPanel* m_pFontPanels[8]; };
    char pad_0x2c[0x38-0x2c]; CSWGuiManager* m_pGuiManager;
    CSWGuiPanel* m_pDialogOutput; CSWGuiPanel* m_pDialogPanel;
    char pad_0x44[8]; CSWGuiPanel* m_pBarkBubble;
    char pad_0x50[0x6c-0x50]; CSWGuiFade* m_pGlobalFade;
    char pad_0x70[0x20]; CSWGuiMainInterface* m_pMainInterface;
    char pad_0x94[0x170-0x94]; unsigned long m_oidDialogOwner,m_oidDialogSpeaker;
    char pad_0x178[0x198-0x178]; unsigned long m_nDialogAnimations;
    char pad_0x19c[4]; unsigned long* m_pDialogObjects;
    unsigned short* m_pDialogAnimations;
    char pad_0x1a8[8]; int m_nPlaceableCameras;
    CSWPlaceableCamera m_cameras[51];
    char pad_0xb44[0xbfc-0xb44]; int m_bDeferredFOV; float m_fDeferredFOV;
    char pad_0xc04[8]; Vector m_vAreaTransition; unsigned long m_oidAreaTransition;
    int SetPlaceableCameraCount(int count); // descriptive name, no prior symbol
    CSWPlaceableCamera* GetPlaceableCamera(int index);
    void AddFloatyText(unsigned long oid,const CExoString& text,const Vector& position,float duration);
    void SetCombatMessage(unsigned long message);
    void ShowDialogMessage();
    void SetDeferredFOVCallForAnimatedCutscenes(float fov);
    void SetAreaTransition(unsigned long oid,const Vector& position);
    int IsInDialog(unsigned long oid);
    void SetDialogAnimation(unsigned long index,unsigned long oid,unsigned short animation); // descriptive
    int GetBarkBubbleVisible(int* bottom);
    unsigned char GetCombatBarHeight();
    int GetCurrentEquipScreenNPC();
    void ResetInterfaceFont();
    int IsGlobalFadeObscuring();
};
// FUNCTION: SWKOTOR 0x0062AC80
int CGuiInGame::SetPlaceableCameraCount(int count) { if(count>50) return 0; m_nPlaceableCameras=count; return 1; }
// FUNCTION: SWKOTOR 0x0062AD30
CSWPlaceableCamera* CGuiInGame::GetPlaceableCamera(int index) { if(index>m_nPlaceableCameras) return 0; return &m_cameras[index]; }
// FUNCTION: SWKOTOR 0x0062B080
void CGuiInGame::AddFloatyText(unsigned long oid,const CExoString& text,const Vector& position,float duration) { if(m_pMainInterface) m_pMainInterface->AddFloatyText(oid,text,position,duration); }
// FUNCTION: SWKOTOR 0x0062B110
void CGuiInGame::SetCombatMessage(unsigned long message) { if(m_pMainInterface) m_pMainInterface->SetCombatMessage(message); }
// FUNCTION: SWKOTOR 0x0062B130
void CGuiInGame::ShowDialogMessage() { if(m_pDialogOutput==m_pDialogPanel) m_pDialogOutput->ShowDialogMessage(); }
// FUNCTION: SWKOTOR 0x0062B470
void CGuiInGame::SetDeferredFOVCallForAnimatedCutscenes(float fov) { m_bDeferredFOV=1; m_fDeferredFOV=fov; }
// FUNCTION: SWKOTOR 0x0062B020
void CGuiInGame::SetAreaTransition(unsigned long oid,const Vector& position) { m_vAreaTransition=position; m_oidAreaTransition=oid; }
// FUNCTION: SWKOTOR 0x0062DBD0
int CGuiInGame::IsInDialog(unsigned long oid) { return m_oidDialogOwner==oid || m_oidDialogSpeaker==oid; }
// FUNCTION: SWKOTOR 0x00628710
void CGuiInGame::SetDialogAnimation(unsigned long index,unsigned long oid,unsigned short animation) { if(index<m_nDialogAnimations && m_pDialogObjects && m_pDialogAnimations) { m_pDialogObjects[index]=oid; m_pDialogAnimations[index]=animation; } }
// FUNCTION: SWKOTOR 0x0062EBE0
int CGuiInGame::GetBarkBubbleVisible(int* bottom) { if(m_pBarkBubble && m_pGuiManager->PanelExists(m_pBarkBubble)) { const CSWGuiExtent& extent=m_pBarkBubble->m_extent; *bottom=extent.top+extent.height; return 1; } return 0; }
// FUNCTION: SWKOTOR 0x0062F8F0
unsigned char CGuiInGame::GetCombatBarHeight() { if(m_pMainInterface) return m_pMainInterface->m_nCombatBarHeight; return 0; }
// FUNCTION: SWKOTOR 0x0062FEA0
int CGuiInGame::GetCurrentEquipScreenNPC() { int npc=-1; if(m_pGuiManager && m_pGuiManager->PanelExists(m_pEquipment)) npc=m_pEquipment->m_nNPC; return npc; }
// FUNCTION: SWKOTOR 0x0062B490
void CGuiInGame::ResetInterfaceFont() { if(m_pMainInterface) m_pMainInterface->UpdateFonts(); CSWGuiPanel** p=m_pFontPanels; for(int i=8;i;--i,++p) { if(*p) (*p)->UpdateFonts(); } }
// FUNCTION: SWKOTOR 0x0062DED0
int CGuiInGame::IsGlobalFadeObscuring() { return m_pGlobalFade->m_fAlpha>0.001f; }
