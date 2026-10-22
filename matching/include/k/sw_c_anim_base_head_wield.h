// Worker K matching headers: CSWCAnimBaseHeadWield (0x00698EC0-0x00699D70,
// vftable 0x00754AE8), a creature with a separate head model and wielded
// items. No own members: vbptr at 0x00, vtordisp at 0x04, the virtual
// bases CSWCAnimBaseTW at 0x08, CSWCAnimBaseHead at 0x17c and
// CSWCAnimBaseWield at 0x1c8. Every TW slot that Head or Wield overrides
// is overridden again here and forwards to one or both of them.
#ifndef K_SW_C_ANIM_BASE_HEAD_WIELD_H
#define K_SW_C_ANIM_BASE_HEAD_WIELD_H

#include "k/sw_c_anim_base_wield.h"

class CSWCAnimBaseHeadWield : public virtual CSWCAnimBaseHead, public virtual CSWCAnimBaseWield {
public:
    CSWCAnimBaseHeadWield();                        // 0x00698EC0
    virtual ~CSWCAnimBaseHeadWield();
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
    virtual int PlayAnimSet(int a, int b);          // 0x44
    virtual int PlayAnim(int nAnim, float fSpeed, int a, int b);  // 0x54
    virtual int PlayAnimPart(unsigned char nPart, int nAnim, float fSpeed, int a, int b);  // 0x58
    virtual int SetTalk(int a, int b, int c, int d);  // 0x5c
    virtual void SetFlag130(int b);                 // 0x6c
    virtual void SetFlag138(int b);                 // 0x70
    virtual void UnloadModelPart(unsigned char nPart);  // 0x74
    virtual void UnloadModel();                     // 0x78
    virtual int SetWieldedItem(unsigned long oid, const CExoString& sNode, unsigned char nHand, float fAlpha);  // 0x7c
    virtual void SetWieldedModels(const CResRef& cRight, const CResRef& cLeft);  // 0x80
    virtual void ShowWieldedItems();                // 0x84
    virtual void DetachHeadItem();                  // 0x88
    virtual void AttachHeadItem();                  // 0x8c
    virtual void SetCheckItems(int b);              // 0x90
    virtual int SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap);  // 0x98
    virtual int SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap);  // 0x9c
    virtual int RestoreEnvMap();                    // 0xa0
    virtual void ModelVC0();                        // 0xb8
    virtual void ModelVBC();                        // 0xbc
    virtual void UpdateSound(int bOn);              // 0xc4
    virtual int SetModelV128Self(int n);            // 0xc8
    virtual int SetModelV12CSelf(int n);            // 0xcc
    virtual int GetBoundingExtent(CSWGuiExtent* pExtent);  // 0xd8
    virtual void ModelVF8();                        // 0xe4
    virtual void ModelVFC();                        // 0xe8
    virtual void SetHeadItem(unsigned long oid);    // 0xec
    virtual int PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b);  // 0xf0
    virtual int PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b);  // 0xf4
};

#endif
