// CGuiInGame::AppendToDialogBuffer(const CExoString&, unsigned long)
// (guiingame.o): resolves the speaker's name and forwards to the
// (text, name) overload. Standalone layout.
#include "shared/exo_string.h"
class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual CExoString GetName(); // +0x70
};
#include "shared/game_object.h"
class CClientExoApp { public: CGameObject* GetGameObject(unsigned long oid); /* 0x005ED580 */ };
#include "shared/app_manager.h"
#include "shared/globals.h"
class CGuiInGame {
public:
    void AppendToDialogBuffer(const CExoString& sText,const CExoString& sName); // 0x0062B680
    void AppendToDialogBuffer(const CExoString& sText,unsigned long oidSpeaker);
};

// FUNCTION: SWKOTOR 0x00630560
void CGuiInGame::AppendToDialogBuffer(const CExoString& sText,unsigned long oidSpeaker)
{
    if (sText != "") {
        CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidSpeaker);
        if (pObject) {
            CExoString sName;
            sName = pObject->AsSWCObject()->GetName();
            AppendToDialogBuffer(sText,sName);
        }
    }
}
