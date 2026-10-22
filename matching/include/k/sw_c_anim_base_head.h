// Worker K matching headers: CSWCAnimBaseHead (0x0069B030-0x0069BC80), a
// head model ("head" attached at "HEADHOOK") over a virtual
// CSWCAnimBaseTW base. Layout: vbptr at 0x00, own members 0x04-0x4b,
// vtordisp at 0x4c, CSWCAnimBaseTW at 0x50. Members overriding the base's
// virtuals receive `this` pointing at the virtual base (fields at negative
// offsets in the disassembly).
#ifndef K_SW_C_ANIM_BASE_HEAD_H
#define K_SW_C_ANIM_BASE_HEAD_H

#include "k/sw_c_anim_base.h"

// Client item (CClientExoApp::GetItemByGameObjectID 0x005ED5A0). The
// interface at +0x10 hands out the item's part models.
class CSWCItemObjectBase {
public:
    virtual ~CSWCItemObjectBase();
    char pad_0x4[0xc];
};

class CSWCItemModelIf {
public:
    virtual void s00();
    virtual void s04();
    virtual void s08();
    virtual void s0C();
    virtual void s10();
    virtual void s14();
    virtual void s18();
    virtual void s1C();
    virtual void s20();
    virtual void s24();
    virtual void s28();
    virtual void s2C();
    virtual void s30();
    virtual void s34();
    virtual void s38();
    virtual void s3C();
    virtual void s40();
    virtual void s44();
    virtual void s48();
    virtual void s4C();
    virtual void s50();
    virtual void s54();
    virtual void s58();
    virtual void s5C();
    virtual void s60();
    virtual void s64();
    virtual void s68();
    virtual void s6C();
    virtual void s70();
    virtual void s74();
    virtual void s78();
    virtual void s7C();
    virtual void s80();
    virtual void s84();
    virtual void s88();
    virtual void s8C();
    virtual void s90();
    virtual void s94();
    virtual CAurObject* GetPartModel(unsigned char nPart, int bCreate);  // 0x98
};

struct CSWBaseItemData {
    char pad_0x0[0xac];
    unsigned char m_nWeaponWield;                   // 0xac (0x1d: lightsaber)
};

class CSWCItem : public CSWCItemObjectBase, public CSWCItemModelIf {
public:
    CSWBaseItemData* GetBaseItem();                 // 0x005B4790
    int GetKeepAttached();                          // 0x00646630
};

class CClientExoApp {
public:
    CSWCItem* GetItemByGameObjectID(unsigned long nId);   // 0x005ED5A0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCAnimBaseHead : public virtual CSWCAnimBaseTW {
public:
    CSWCAnimBaseHead();                             // 0x0069BB80
    virtual ~CSWCAnimBaseHead();                    // 0x0069B030
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c
    virtual int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore);  // 0x1c
    virtual int ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore);  // 0x18
    virtual int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart);  // 0x14
    virtual int ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart);  // 0x10
    virtual int RestoreTexturePart(unsigned char nPart);  // 0x20
    virtual int RestoreTexture();                   // 0x24
    virtual int SetSkin(const CResRef& cSkin, int n);  // 0x28
    virtual int ClearSkin();                        // 0x2c
    virtual int SetFlag144(int b);                  // 0x30
    virtual void UnloadModelPart(unsigned char nPart);  // 0x74
    virtual void UnloadModel();                     // 0x78
    virtual int SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap);  // 0x98
    virtual int SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap);  // 0x9c
    virtual int RestoreEnvMap();                    // 0xa0
    virtual void ModelVC0();                        // 0xb8
    virtual void ModelVBC();                        // 0xbc
    virtual void ModelVF8();                        // 0xe4
    virtual void ModelVFC();                        // 0xe8
    virtual void SetHeadItem(unsigned long oid);    // 0xec

    unsigned short m_aTextureIndicesHead[10];       // 0x04
    unsigned char m_nTextureCountHead;              // 0x18
    unsigned char m_nTextureStartHead;              // 0x19
    char pad_0x1a[2];
    CResRef m_cReplaceTextureHead;                  // 0x1c
    int m_bReplaceTextureIndexedHead;               // 0x2c
    CResRef m_cEnvMapHead;                          // 0x30
    int m_bEnvMapHead;                              // 0x40
    CAurObject* m_pModelHead;                       // 0x44
    unsigned long m_oidHeadItem;                    // 0x48
};

#endif
