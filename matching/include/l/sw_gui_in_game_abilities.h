// Worker L matching headers: the character abilities screen
// CSWGuiInGameAbilities (vtable 0x00755E50) and its list entry
// CSWGuiInGameSkillEntry (vtable 0x00755ED0), 0x006AB8B0-0x006ACCE0.
#ifndef L_SW_GUI_IN_GAME_ABILITIES_H
#define L_SW_GUI_IN_GAME_ABILITIES_H

#include "l/sw_gui_options_in_game.h"

// vtable 0x0073E338; 0x74 bytes.
class CSWGuiBorder {
public:
    CSWGuiBorder();                                 // 0x004167F0
    ~CSWGuiBorder();                                // 0x004151C0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    CSWGuiExtent GetInnerExtent();                  // 0x00415240

    CSWGuiExtent m_extent;                          // 0x04
    CSWGuiBorderParams m_cParams;                   // 0x14
};

// The original's CSWGuiExtent has a zeroing ctor (returned through a hidden pointer).
CSWGuiZeroExtent IntersectExtents(const CSWGuiExtent& a, const CSWGuiExtent& b);  // 0x0040A4A0

// vtable 0x0073EA28 family; ctor 0x0041B5C0, dtor 0x0041B610.
class CSWGuiLabelHilight : public CSWGuiLabel {
public:
    CSWGuiLabelHilight();                           // 0x0041B5C0
    ~CSWGuiLabelHilight();                          // 0x0041B610
    CSWGuiBorder m_cHilight;                        // 0x140
};

// vtable 0x00755ED0; ctor 0x006ACC50, deleting dtor 0x006ACCC0, implicit
// dtor 0x006ABA80. A skill/feat/power row: icon frame + label + highlight.
class CSWGuiInGameSkillEntry : public CSWGuiLabelHilight {
public:
    CSWGuiInGameSkillEntry();                       // 0x006ACC50
    void SetExtent(const CSWGuiExtent& extent);     // 0x04 (0x006AB8E0)
    void Draw(float fDelta);                        // 0x38 (0x006AB9F0)
    void SetSkill(unsigned short nId, unsigned char nType);    // 0x006ACB40

    CSWGuiBorder m_cIconBorder;                     // 0x1b4
    CSWGuiBorder m_cIconHilight;                    // 0x228
    CSWGuiBorder m_cIcon;                           // 0x29c
};

class CSWSkill {
public:
    CExoString GetNameText();                       // 0x005CD8D0
    int m_nNameStrRef;                              // 0x00
    char pad_0x4[0xc];
    CExoString m_sIcon;                             // 0x10
    char pad_0x18[0x8];
};

class CSWFeat {
public:
    CExoString GetNameText();                       // 0x005CD760
    CExoString GetDescriptionText();                // 0x005CD800
    char pad_0x0[0x8];
    int m_nNameStrRef;                              // 0x08
    char pad_0xc[0xc];
    CResRef m_cIcon;                                // 0x18
    char pad_0x28[0x10];
    unsigned short m_nPrereq1;                      // 0x38 (0xffff: chain root)
    unsigned short m_nPrereq2;                      // 0x3a

    // 0 for a chain root, 1/2 for the first/second upgrade.
    int GetChainLevel()
    {
        if (m_nPrereq1 == 0xffff) {
            return 0;
        }
        return (m_nPrereq2 != 0xffff) + 1;
    }
};

class CSWSpell {
public:
    char pad_0x0[0x8];
    int m_nNameStrRef;                              // 0x08
    char pad_0xc[0x4];
    CResRef m_cIcon;                                // 0x10
    char pad_0x20[0x120];
    unsigned char m_bUsable;                        // 0x140 (1: listed in the powers screen)
    char pad_0x141[0x1f];
    int m_anPrereqs[1];                             // 0x160 (-1 terminated)

    CExoString GetSpellNameText();                  // 0x0059B940

    unsigned long GetChainCode(int nArg);           // 0x0059B6A0 (group * 10 + rank)
    int GetSortKey(int nArg);                       // 0x00647510
    int GetUpgradeSlot(int nArg);                   // 0x006474D0
};

class CSWSpellArray {
public:
    int m_nNumSpells;                               // 0x00
    CSWSpell* GetSpell(unsigned short nSpell);                 // 0x0059B6D0
    CSWSpell* GetSpellI(int nSpell);                // 0x0059B6D0 (GetSpell, int argument)
};

class CSWCCreatureStats {
public:
    unsigned char GetClass(unsigned char nIndex);   // 0x00647730
    int HasFeat(unsigned short nFeat);              // 0x00649340
    void ClearFeats();                              // 0x00648320
    unsigned short GetFeat(unsigned short nIndex);  // 0x006486E0
    void ClearSpells();                             // 0x00649520
    unsigned short GetNumberKnownSpells(unsigned char nClass, unsigned char nLevel); // 0x006487D0
    unsigned long GetKnownSpell(unsigned char nClass, unsigned char nLevel,
                                unsigned char nIndex); // 0x006495A0
    void AddKnownSpell(unsigned char nClass, unsigned long nSpell); // 0x00649F90
    char pad_0x0[0x32];
    unsigned char m_nNumClasses;                    // 0x32
    char pad_0x33[0x7d];
    unsigned short* m_pFeats;                       // 0xB0
    unsigned short m_nNumFeats;                     // 0xB4
    char pad_0xb6[2];
    int m_nFeatCapacity;                            // 0xB8

    unsigned char GetLastClass() { return GetClass(m_nNumClasses - 1); }
};

class CSWSObject;

class CSWSCreatureStats {
public:
    char GetSkillRank(unsigned char nSkill, CSWSObject* pVersus, int bBaseOnly);  // 0x005AA570
    unsigned char GetClass(unsigned char nMultiClass);  // 0x005A4E90
    int HasFeat(unsigned short nFeat);              // 0x005A6630
    int HasSpell(unsigned char nList, unsigned long nSpell, int bUpgrade);  // 0x005A6E70
    void AddKnownSpell(unsigned char nClass, unsigned long nSpell);     // 0x005AA9B0
    void AddFeat(unsigned short nFeat);             // 0x005AA810
    void SetSkillRank(unsigned char nSkill, char nRank);    // 0x005A54C0
    char GetSkillRankBase(unsigned char nSkill);    // 0x005A5FD0 (placeholder name)
    char pad_0x0[0x89];
    unsigned char m_nNumClasses;                    // 0x89
};

// 0x1a0 bytes, g_pRules->m_pClasses.
class CSWClass {
public:
    int IsBonusFeat(unsigned short nFeat);          // 0x005BE270
    int IsNormalFeat(unsigned short nFeat);         // 0x005BE2C0
    int IsGrantedFeat(unsigned short nFeat, unsigned char& nLevel);  // 0x005BE310
    int IsFeatUseable(unsigned short nFeat);        // 0x005BE370
    char pad_0x0[0x1a0];
};

class CSWSCreature {
public:
    char pad_0x0[0xa74];
    CSWSCreatureStats* m_pStats;                    // 0xa74
};

// Virtual interface of the client creature (vtable slot 0x148 only); the
// shared CSWCCreature declaration carries no vtable.
class CSWCCreatureVirtuals {
public:
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00C();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01C();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02C();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03C();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04C();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05C();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06C();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07C();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08C();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09C();
    virtual void v0A0();
    virtual void v0A4();
    virtual void v0A8();
    virtual void v0AC();
    virtual void v0B0();
    virtual void v0B4();
    virtual void v0B8();
    virtual void v0BC();
    virtual void v0C0();
    virtual void v0C4();
    virtual void v0C8();
    virtual void v0CC();
    virtual void v0D0();
    virtual void v0D4();
    virtual void v0D8();
    virtual void v0DC();
    virtual void v0E0();
    virtual void v0E4();
    virtual void v0E8();
    virtual void v0EC();
    virtual void v0F0();
    virtual void v0F4();
    virtual void v0F8();
    virtual void v0FC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12C();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13C();
    virtual void v140();
    virtual void v144();
    virtual CResRef GetPortrait(int bLarge);        // 0x148
};

inline CResRef GetCreaturePortrait(CSWCCreature* pCreature, int bLarge)
{
    return ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(bLarge);
}

int IsJediClass(int nClass);                        // 0x005BE4A0

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    void Allocate(int nSize);                       // 0x004186E0 (12-byte T), 0x005E03C0 (int)
    void Insert(T t, int nPos);                     // <int>: 0x00616940
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 0x10);
        }
        element[num] = t;
        num++;
    }
    int AddUnique(T t);                           // <unsigned short>: 0x005A7670
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

// A CExoArrayList with the inline constructor / growth the panels use for
// their temporary control lists.
template <class T>
class CExoArrayListL {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayListL()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayListL()
    {
        delete[] element;
    }
    void Allocate(int nSize);                       // <int>: 0x005E03C0
    void Insert(T t, int nPos);                     // <int>: 0x00616940, <unsigned short>: 0x006169A0
    T& operator[](int i) { return element[i]; }
    void SetSize(int nSize)
    {
        if (nSize) {
            Allocate(nSize);
        }
        num = nSize;
    }
    // Growth through the out-of-line Allocate.
    void AddAlloc(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 0x10);
        }
        element[num] = t;
        num++;
    }
    __forceinline void Add(T t)
    {
        if (num == array_size) {
            int nSize = array_size ? array_size * 2 : 0x10;
            T* pOld = element;
            array_size = nSize;
            element = new T[nSize];
            for (int i = 0; i < num; i++) {
                element[i] = pOld[i];
            }
            if (pOld) {
                delete[] pOld;
            }
        }
        element[num] = t;
        num++;
    }
};

// vtable 0x0073E390; 0x54 bytes (the d folder's CSWGuiImage).
class CSWGuiImageParams {
public:
    void SetImage(const CResRef& cResRef, int bForce);  // 0x00415900
    void SetAlignment(int n);                       // 0x00415990
    void SetScaled(int b);                          // 0x004159C0
    char pad_0x0[0x14];
    float m_fAlpha;                                 // 0x14
    char pad_0x18[0x14];
};

class CSWGuiImage {
public:
    CSWGuiImage();                                  // 0x00415A30
    ~CSWGuiImage();                                 // 0x00415AD0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    char pad_0x4[0x24];
    CSWGuiImageParams m_cParams;                    // 0x28
};

// One node of an ability chain (a feat or power and its upgrades); 0x128
// bytes, ctor 0x006CCB80, dtor 0x006CCBE0.
class CSWGuiAbilityNode {
public:
    CSWGuiAbilityNode();                            // 0x006CCB80
    ~CSWGuiAbilityNode();                           // 0x006CCBE0
    CSWGuiBorder m_cBorder;                         // 0x00
    CSWGuiImage m_cIcon;                            // 0x74
    CSWGuiImage m_cIndent;                          // 0xc8
    int m_nId;                                      // 0x11c
    unsigned char m_nStatus;                        // 0x120
    unsigned long m_bSelected : 1;                  // 0x124
};

extern Vector g_vAbilityColorGranted;               // 0x007A23C0
extern Vector g_vAbilityColorAvailable;             // 0x007A23D8
extern Vector g_vAbilityColorLocked;                // 0x007A23E4

// A row of up to three chained abilities with arrows between them;
// vtable 0x007578A8, 0x47c bytes, ctor 0x006CCCC0, ??_G 0x006CD420,
// implicit dtor 0x006CD440.
class CSWGuiAbilityChain : public CSWGuiControl {
public:
    CSWGuiAbilityChain();                           // 0x006CCCC0
    void SetExtent(const CSWGuiExtent& extent);     // 0x04 (0x006CCE30)
    void Draw(float fDelta);                        // 0x38 (0x006CCC40)
    void SetNodeStatus(unsigned char nNode, unsigned char nStatus);    // 0x006CCFD0
    int HitTest(CSWGuiPoint pt);                    // 0x006CD1C0
    void SetNodeSelected(unsigned char nNode, int bSelected);   // 0x006CD4C0
    void SetNode(unsigned char nNode, int nId, const CResRef& cIcon)
    {
        m_aNodes[nNode].m_nId = nId;
        m_aNodes[nNode].m_cIcon.m_cParams.SetImage(cIcon, 0);
    }
    void Select(unsigned char nNode)
    {
        if (!m_aNodes[nNode].m_bSelected) {
            m_aNodes[nNode].m_bSelected = 1;
            SetNodeStatus(nNode, m_aNodes[nNode].m_nStatus);
        }
    }
    void Deselect(unsigned char nNode)
    {
        if (m_aNodes[nNode].m_bSelected) {
            m_aNodes[nNode].m_bSelected = 0;
            SetNodeStatus(nNode, m_aNodes[nNode].m_nStatus);
        }
    }
    int GetNodeIndex(int nId)
    {
        for (int i = 0; i < 3; i++) {
            if (m_aNodes[i].m_nId == nId) {
                return i;
            }
        }
        return -1;
    }

    CSWGuiAbilityNode m_aNodes[3];                  // 0x5c
    CSWGuiImage m_aArrows[2];                       // 0x3d4
};

// The abilities lists (0x10 bytes): a CExoArrayList of chains plus the
// selected node / chain.
class CSWGuiAbilityList {
public:
    CSWGuiAbilityList();                            // 0x006CD510
    ~CSWGuiAbilityList();                           // 0x006CD530
    void Reset();                                   // 0x006CD240
    void SetCreature(CSWSCreatureStats* pStats, int bAll);  // 0x006CE0F0 (power chains)
    void SetCreatureB(CSWSCreatureStats* pStats);   // 0x006CE370 (feat chains)
    void SetLevelUpFeats(CSWCCreatureStats* pStats, int bLevelUp);  // 0x006CE570
    void GetSpellList(CExoArrayListL<int>* pList);    // 0x006CD590 (sorted powers)
    void GetFeatList(CExoArrayListL<unsigned short>* pList);    // 0x006CD960 (feats by chain level)
    void SetStatus(int nId, unsigned char nStatus); // 0x006CD280
    void SetSelected(int nId);                      // 0x006CDC00
    void SetMode(unsigned char nStatus);            // 0x006CD2E0
    int Refresh();                                  // 0x006CDD10
    int HandleInputEvent(int nEvent);               // 0x006CDD80
    void AddSpellChain(int nSpell1, int nSpell2, int nSpell3);  // 0x006CD7E0
    void AddFeatChain(int nFeat1, int nFeat2, int nFeat3);      // 0x006CDA80
    int GetSelectedId();                            // 0x006ABA50

    __forceinline void Add(CSWGuiAbilityChain* pChain)
    {
        if (m_lstChains.num == m_lstChains.array_size) {
            int nSize = m_lstChains.array_size ? m_lstChains.array_size * 2 : 0x10;
            CSWGuiAbilityChain** pOld = m_lstChains.element;
            m_lstChains.array_size = nSize;
            m_lstChains.element = new CSWGuiAbilityChain*[nSize];
            for (int i = 0; i < m_lstChains.num; i++) {
                m_lstChains.element[i] = pOld[i];
            }
            if (pOld) {
                delete[] pOld;
            }
        }
        m_lstChains.element[m_lstChains.num] = pChain;
        m_lstChains.num++;
    }
    CSWGuiAbilityNode& GetSelectedNode() { return m_lstChains.element[m_nPage]->m_aNodes[m_nRow]; }

    CExoArrayList<CSWGuiAbilityChain*> m_lstChains; // 0x00
    unsigned char m_nRow;                           // 0x0c (node)
    unsigned char m_nPage;                          // 0x0d (chain)
};

struct CSWSkillListEntry {
    CSWSkill* pSkill;
    int nBase;
    int nBonus;
};

// vtable 0x00755E50; deleting dtor 0x006ACC30.
class CSWGuiInGameAbilities : public CSWGuiPanel {
public:
    ~CSWGuiInGameAbilities();                       // 0x006ABAF0
    void OnPanelRemoved();                          // 0x4c (0x006AB8B0)
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006AE5F0)
    void OnPanelAdded();                            // 0x48 (0x006ADC20)

    void RefreshList();                             // 0x006ABCE0
    void UpdateDescriptionListBox(int bHasText);    // 0x006ABD20
    void SetDescription(unsigned long nStrRef);     // 0x006ABDC0
    static void SetPortrait(CSWGuiLabel* pLabel, const CResRef& cPortrait)
    {
        pLabel->m_cBorder.SetFillImage(cPortrait, 0);
    }
    void SetDescriptionText(const CExoString& sText)
    {
        m_lblDesc.m_cText.SetText(sText);
        UpdateDescriptionListBox(sText.GetLength() > 0);
    }
    int CanShowPowers();                            // 0x006ABE70
    void ShowSkill(int nSkill);                     // 0x006ACCE0
    void ShowFeat(unsigned short nFeat);            // 0x006AD410
    void OnAbilitySelectionChanged(CSWGuiControl* pControl);    // 0x006AD4B0
    void UpdateList();                              // 0x006AD560
    void CreateSkillList();                         // 0x006AD960
    void OnShowFeats(CSWGuiControl* pControl);      // 0x006ADA70
    void OnShowPowers(CSWGuiControl* pControl);     // 0x006ADAA0
    void OnShowSkills(CSWGuiControl* pControl);     // 0x006ADAD0
    void SetCharacter(CSWCCreature* pCreature);     // 0x006ADB00
    void OnChangeCharacter(CSWGuiControl* pControl);    // 0x006ADC80
    void UpdatePortraits();                         // 0x006ABEC0

    CExoArrayList<CSWSkillListEntry> m_lstSkills;   // 0x64
    CSWGuiInGameSkillEntry m_aEntries[10];          // 0x70
    CSWGuiLabel m_lbl1F10;                          // 0x1f10
    CSWGuiLabel m_lblDesc;                          // 0x2050
    CSWGuiLabel m_lbl2190;                          // 0x2190
    CSWGuiLabel m_lbl22D0;                          // 0x22d0
    CSWGuiLabel m_lbl2410;                          // 0x2410
    CSWGuiLabel m_lbl2550;                          // 0x2550
    CSWGuiLabel m_lbl2690;                          // 0x2690
    CSWGuiLabel m_lbl27D0;                          // 0x27d0
    CSWGuiLabel m_lbl2910;                          // 0x2910
    CSWGuiLabel m_lbl2A50;                          // 0x2a50
    CSWGuiButton m_btn2B90;                         // 0x2b90
    CSWGuiButton m_btn2D54;                         // 0x2d54
    CSWGuiButton m_btn2F18;                         // 0x2f18
    CSWGuiListBox m_lbSkills;                       // 0x30dc
    CSWGuiListBox m_lbDesc;                         // 0x33bc
    CSWGuiButton m_btn369C;                         // 0x369c
    CSWGuiButton m_btn3860;                         // 0x3860
    CSWGuiButton m_btn3A24;                         // 0x3a24
    CSWGuiButton m_btn3BE8;                         // 0x3be8
    CSWGuiButton m_btn3DAC;                         // 0x3dac
    unsigned char m_n3F70;                          // 0x3f70
    char m_nPartyMember;                            // 0x3f71
    CSWCCreature* m_pCreature;                      // 0x3f74
    CSWGuiAbilityList m_lstAbilities;               // 0x3f78
    CSWGuiAbilityList m_lstSecond;                  // 0x3f88
};

#endif
