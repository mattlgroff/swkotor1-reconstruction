// Worker K matching headers: CSWCAnimBaseWield (0x00699DD0-0x0069B020),
// the wielded-item layer ("rhand" / "lhand" items, "dummyright" /
// "dummyleft" placeholder models, the lightsaber hum sound) over a virtual
// CSWCAnimBaseTW base. Layout: vbptr at 0x00, own members 0x04-0x57,
// vtordisp at 0x58, CSWCAnimBaseTW at 0x5c.
#ifndef K_SW_C_ANIM_BASE_WIELD_H
#define K_SW_C_ANIM_BASE_WIELD_H

#include "k/sw_c_anim_base_head.h"

extern const unsigned long g_oidInvalid;            // 0x00746644 (0x7f000000)

class CExoSoundSource {
public:
    CExoSoundSource(const CResRef& cResRef);        // 0x005D60E0
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char n);         // 0x005D5900
    void Set3D(int b);                              // 0x005D5910
    void Play();                                    // 0x005D5930
    void SetVolume(unsigned char n);                // 0x005D5950
    void SetLooping(int b);                         // 0x005D59D0
    void SetPosition(const Vector& v);              // 0x005D59E0
    void Stop();                                    // 0x005D5A20

    int m_n4;
};

class CSWCAnimBaseWield : public virtual CSWCAnimBaseTW {
public:
    CSWCAnimBaseWield();                            // 0x00699DD0
    virtual ~CSWCAnimBaseWield();                   // 0x00699ED0
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c
    virtual int SetWieldedItem(unsigned long oid, const CExoString& sNode, unsigned char nHand, float fAlpha);  // 0x7c
    virtual void SetWieldedModels(const CResRef& cRight, const CResRef& cLeft);  // 0x80
    virtual void ShowWieldedItems();                // 0x84
    virtual void SetCheckItems(int b);              // 0x90
    virtual void ModelVC0();                        // 0xb8
    virtual void ModelVBC();                        // 0xbc
    virtual void UpdateSound(int bOn);              // 0xc4
    virtual void ModelVF8();                        // 0xe4
    virtual void ModelVFC();                        // 0xe8
    virtual int PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b);  // 0xf0
    virtual int PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b);  // 0xf4

    CAurObject* GetItemModel(unsigned long oid, unsigned char nPart);  // 0x0069A650
    // Inlined copy of GetItemModel(oid, 0xff).
    CAurObject* GetItemModelInline(unsigned long oid)
    {
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
        if (!pItem) {
            return 0;
        }
        return pItem->GetPartModel(0xff, 1);
    }
    int IsItemValid(unsigned long oid);             // 0x0069A680
    void AttachWieldedItems();                      // 0x0069AF10

    CResRef m_cRightModel;                          // 0x04
    CResRef m_cLeftModel;                           // 0x14
    CExoString m_sRightNode;                        // 0x24
    CExoString m_sLeftNode;                         // 0x2c
    CAurObject* m_pRightDummy;                      // 0x34
    CAurObject* m_pLeftDummy;                       // 0x38
    unsigned long m_oidRight;                       // 0x3c
    unsigned long m_oidLeft;                        // 0x40
    int m_bCheckItems;                              // 0x44
    int m_bDummiesShown;                            // 0x48
    int m_n4C;                                      // 0x4c
    int m_bSaberIgnited;                            // 0x50
    CExoSoundSource* m_pSound;                      // 0x54
};

#endif
