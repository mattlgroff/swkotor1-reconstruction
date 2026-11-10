// CSWCCreature::SaveCharGenCreature (nwccreature.o, Mac name), worker C s11:
// writes the character-generation creature to TEMP:temp.bic as a GFF "BIC "
// V2.0 file. The CSWCCreatureStats members below are the ones the fields
// written here name; the GFF labels give each member's meaning.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const; // 0x005E5D10
    char* CStr() const;                             // 0x005E5670
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoLocString {
public:
    CExoLocString();                                // 0x005E9E10
    ~CExoLocString();                               // 0x005EA000
    int AddString(int nLanguage, const CExoString& sString, unsigned char nGender); // 0x005EA340

    void* m_pList;                                  // 0x00
    unsigned long m_dwStringRef;                    // 0x04
};

class CResRef {
public:
    CResRef(const CExoString& sName);               // 0x00406D60

    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    CResGFF();                                      // 0x004105A0
    virtual ~CResGFF();

    int CreateGFFFile(CResStruct* pTopLevelStruct, const CExoString& sFileType,
                      const CExoString& sVersion);  // 0x00411260
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel); // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType); // 0x004124E0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel); // 0x00412620
    int WriteFieldCHAR(CResStruct* pStruct, char nValue, char* szLabel); // 0x00412670
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel); // 0x004126C0
    int WriteFieldSHORT(CResStruct* pStruct, short nValue, char* szLabel); // 0x00412710
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel); // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel); // 0x004127B0
    int WriteFieldCResRef(CResStruct* pStruct, const CResRef& cValue, char* szLabel); // 0x004128C0
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
    int WriteFieldCExoLocString(CResStruct* pStruct, CExoLocString& lsValue, char* szLabel); // 0x00412A10
    int WriteGFFFile(const CExoString& sFileName, unsigned short nType); // 0x00413030

    char pad_0x4[0xa0 - 0x4];
};

struct CSWRace {
    unsigned char GetSTRAdjust() { return m_nSTRAdjust; }
    unsigned char GetDEXAdjust() { return m_nDEXAdjust; }
    unsigned char GetINTAdjust() { return m_nINTAdjust; }
    unsigned char GetCHAAdjust() { return m_nCHAAdjust; }
    unsigned char GetWISAdjust() { return m_nWISAdjust; }
    unsigned char GetCONAdjust() { return m_nCONAdjust; }
    char pad_0x0[0x18];
    unsigned char m_nSTRAdjust;                     // 0x18
    unsigned char m_nDEXAdjust;                     // 0x19
    unsigned char m_nINTAdjust;                     // 0x1a
    unsigned char m_nCHAAdjust;                     // 0x1b
    unsigned char m_nWISAdjust;                     // 0x1c
    unsigned char m_nCONAdjust;                     // 0x1d
    char pad_0x1e[0x34 - 0x1e];
};

struct CSWClass {
    unsigned char GetHitDie() { return m_nHitDie; }
    char pad_0x0[0x54];
    unsigned char m_nHitDie;                        // 0x54
    char pad_0x55[0x1a0 - 0x55];
};

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;                     // 0xab
    CSWClass* m_lstClasses;                         // 0xac
    CSWRace* m_lstRaces;                            // 0xb0

    CSWClass* GetClassObject(unsigned char nClass) { return &m_lstClasses[nClass]; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CSWCCreatureStats {
public:
    short GetArmorClass(int bIncludeDex);           // 0x00647720
    unsigned char GetClass(unsigned char nClassIndex); // 0x00647730
    unsigned char GetClassLevel(unsigned char nClassIndex); // 0x00647750
    short GetMaxHitPoints();                        // 0x00647A80
    CResRef GetPortrait(int bCheckCustom);          // 0x00647E80
    unsigned short GetFeat(unsigned short nIndex);  // 0x006486E0
    unsigned short GetNumberKnownSpells(unsigned char nClassIndex, unsigned char nLevel); // 0x006487D0
    unsigned long GetKnownSpell(unsigned char nClassIndex, unsigned char nLevel,
                                unsigned char nIndex); // 0x006495A0

    char GetSkillRank(unsigned short nSkill)
    {
        if (nSkill >= g_pRules->m_nNumSkills) {
            return 0;
        }
        return m_pSkillRanks[nSkill];
    }
    CResRef GetConversation() { return m_cConversation; }
    int GetNumFeats() { return m_lstFeats.num; }
    int GetUseBackupHead() { return m_bUseBackupHead; }
    unsigned short GetAppearanceType() { return m_nAppearanceType; }
    short GetBaseHitPoints() { return m_nBaseHitPoints; }
    unsigned char GetCHA() { return m_nCHA; }
    unsigned char GetCON() { return m_nCON; }
    short GetCurrentHitPoints() { return m_nCurrentHitPoints; }
    unsigned char GetDEX() { return m_nDEX; }
    unsigned long GetExperience() { return m_nExperience; }
    unsigned short GetForceSum() { return m_nForce124 + m_nForce122; }
    short GetForcePoints() { return m_nForcePoints; }
    short GetGoodEvil() { return m_nGoodEvil; }
    unsigned char GetINT() { return m_nINT; }
    short GetMaxForcePoints() { return m_nMaxForcePoints; }
    unsigned char GetNumInventoryRes() { return m_nNumInventoryRes; }
    unsigned char GetNumMultiClasses() { return m_nNumMultiClasses; }
    unsigned short GetPortraitId() { return m_nPortraitId; }
    unsigned short GetRace() { return m_nRace; }
    unsigned char GetSTR() { return m_nSTR; }
    unsigned short GetSkillPoints() { return m_nSkillPoints; }
    unsigned char GetWIS() { return m_nWIS; }
    unsigned char GetGender() { return m_nGender; }
    unsigned char GetSubRaceIndex() { return m_nSubRaceIndex; }
    unsigned char GetStartingPackage() { return m_nStartingPackage; }
    char GetReflexSave() { return m_nReflexSave; }
    char GetWillSave() { return m_nWillSave; }
    char GetFortSave() { return m_nFortSave; }
    unsigned char GetColorSkin() { return m_nColorSkin; }
    unsigned char GetColorHair() { return m_nColorHair; }
    unsigned char GetColorTattoo1() { return m_nColorTattoo1; }
    unsigned char GetColorTattoo2() { return m_nColorTattoo2; }
    unsigned char GetPhenotype() { return m_nPhenotype; }
    unsigned char GetAppearanceHead() { return m_nAppearanceHead; }
    unsigned char GetDuplicatingHead() { return m_nDuplicatingHead; }
    unsigned char GetTail() { return m_nTail; }
    unsigned char GetWings() { return m_nWings; }
    int GetAge() { return m_nAge; }

    char pad_0x0[0x4];
    CResRef m_cConversation;                        // 0x04
    CExoString m_sFirstName;                        // 0x14
    CExoString m_sLastName;                         // 0x1c
    unsigned short m_nRace;                         // 0x24
    char pad_0x26[0x2];
    CExoString m_sSubRace;                          // 0x28
    unsigned char m_nSubRaceIndex;                  // 0x30
    unsigned char m_nGender;                        // 0x31
    unsigned char m_nNumMultiClasses;               // 0x32
    unsigned char m_nStartingPackage;               // 0x33
    unsigned char m_nSTR;                           // 0x34
    unsigned char m_nDEX;                           // 0x35
    unsigned char m_nCON;                           // 0x36
    unsigned char m_nINT;                           // 0x37
    unsigned char m_nWIS;                           // 0x38
    unsigned char m_nCHA;                           // 0x39
    char pad_0x3a[0xe];
    short m_nBaseHitPoints;                         // 0x48
    char pad_0x4a[0x2];
    short m_nCurrentHitPoints;                      // 0x4c
    char pad_0x4e[0xa];
    unsigned long m_nExperience;                    // 0x58
    char m_nFortSave;                               // 0x5c
    char m_nWillSave;                               // 0x5d
    char m_nReflexSave;                             // 0x5e
    char pad_0x5f[0x21];
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0x6];
    char* m_pSkillRanks;                            // 0x88
    unsigned short m_nAppearanceType;               // 0x8c
    unsigned char m_nColorSkin;                     // 0x8e
    unsigned char m_nColorHair;                     // 0x8f
    unsigned char m_nColorTattoo1;                  // 0x90
    unsigned char m_nColorTattoo2;                  // 0x91
    unsigned char m_nPhenotype;                     // 0x92
    unsigned char m_nAppearanceHead;                // 0x93
    unsigned char m_nDuplicatingHead;               // 0x94
    char pad_0x95[0x3];
    int m_bUseBackupHead;                           // 0x98
    unsigned char m_nTail;                          // 0x9c
    unsigned char m_nWings;                         // 0x9d
    char pad_0x9e[0x2];
    CExoString m_sDescription;                      // 0xa0
    int m_nAge;                                     // 0xa8
    char pad_0xac[0x2];
    unsigned short m_nSkillPoints;                  // 0xae
    CExoArrayList<unsigned short> m_lstFeats;       // 0xb0
    char pad_0xbc[0x114 - 0xbc];
    CExoString m_sDeity;                            // 0x114
    unsigned short m_nPortraitId;                   // 0x11c
    short m_nMaxForcePoints;                        // 0x11e
    short m_nForcePoints;                           // 0x120
    unsigned short m_nForce122;                     // 0x122
    unsigned short m_nForce124;                     // 0x124
    char pad_0x126[0x2];
    CResRef* m_pInventoryRes;                       // 0x128
    unsigned char m_nNumInventoryRes;               // 0x12c
};

class CSWCCreature {
public:
    void SaveCharGenCreature();
    CSWCCreatureStats* GetCreatureStats() { return m_pStats; }
    unsigned long GetGold() { return m_nGold; }
    unsigned short GetSoundSet() { return m_nSoundSet; }

    char pad_0x0[0x144];
    unsigned long m_nGold;                          // 0x144
    char pad_0x148[0x2f8 - 0x148];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    unsigned short m_nSoundSet;                     // 0x2fc
};

// FUNCTION: SWKOTOR 0x006123E0
void CSWCCreature::SaveCharGenCreature()
{
    CExoLocString cLocString;
    CResGFF* pGFF = new CResGFF;
    CResStruct cTopLevel;
    pGFF->CreateGFFFile(&cTopLevel, "BIC ", "V2.0");

    cLocString.AddString(0, GetCreatureStats()->m_sFirstName, 0);
    pGFF->WriteFieldCExoLocString(&cTopLevel, cLocString, "FirstName");
    cLocString.AddString(0, GetCreatureStats()->m_sLastName, 0);
    pGFF->WriteFieldCExoLocString(&cTopLevel, cLocString, "LastName");
    cLocString.AddString(0, GetCreatureStats()->m_sDescription, 0);
    pGFF->WriteFieldCExoLocString(&cTopLevel, cLocString, "Description");
    pGFF->WriteFieldDWORD(&cTopLevel, GetGold(), "Gold");
    pGFF->WriteFieldCResRef(&cTopLevel, GetCreatureStats()->GetConversation(), "Conversation");
    pGFF->WriteFieldINT(&cTopLevel, GetCreatureStats()->GetAge(), "Age");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetGender(), "Gender");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetRace(), "Race");
    pGFF->WriteFieldCExoString(&cTopLevel, GetCreatureStats()->m_sSubRace, "Subrace");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetSubRaceIndex(), "SubraceIndex");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetStartingPackage(), "StartingPackage");
    pGFF->WriteFieldCExoString(&cTopLevel, GetCreatureStats()->m_sDeity, "Deity");
    pGFF->WriteFieldSHORT(&cTopLevel, GetCreatureStats()->GetArmorClass(1), "ArmorClass");
    pGFF->WriteFieldCHAR(&cTopLevel, GetCreatureStats()->GetReflexSave(), "RefSaveThrow");
    pGFF->WriteFieldCHAR(&cTopLevel, GetCreatureStats()->GetWillSave(), "WillSaveThrow");
    pGFF->WriteFieldCHAR(&cTopLevel, GetCreatureStats()->GetFortSave(), "FortSaveThrow");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetNumMultiClasses() - 1, "MClassLevUpIn");
    pGFF->WriteFieldWORD(&cTopLevel, GetSoundSet(), "SoundSetFile");

    CResList cClassList;
    pGFF->AddList(&cClassList, &cTopLevel, "ClassList");
    CResStruct cClass;
    pGFF->AddListElement(&cClass, &cClassList, 0);
    pGFF->WriteFieldSHORT(&cClass, GetCreatureStats()->GetClassLevel(0), "ClassLevel");
    pGFF->WriteFieldINT(&cClass, GetCreatureStats()->GetClass(0), "Class");

    CResList cKnownList;
    if (GetCreatureStats()->GetNumberKnownSpells(0, 0)) {
        CExoString sKnownList;
        sKnownList.Format("%s%d", "KnownList", 0);
        pGFF->AddList(&cKnownList, &cClass, sKnownList.CStr());
    }
    for (unsigned char nSpell = 0; nSpell < GetCreatureStats()->GetNumberKnownSpells(0, 0); nSpell++) {
        CResStruct cSpell;
        pGFF->AddListElement(&cSpell, &cKnownList, 0);
        pGFF->WriteFieldWORD(&cSpell, GetCreatureStats()->GetKnownSpell(0, 0, nSpell), "Spell");
    }
    pGFF->WriteFieldWORD(&cTopLevel, GetCreatureStats()->GetSkillPoints(), "SkillPoints");

    CResList cSkillList;
    CResStruct cSkill;
    pGFF->AddList(&cSkillList, &cTopLevel, "SkillList");
    for (unsigned short nSkill = 0; nSkill < g_pRules->m_nNumSkills; nSkill++) {
        pGFF->AddListElement(&cSkill, &cSkillList, 0);
        pGFF->WriteFieldBYTE(&cSkill, GetCreatureStats()->GetSkillRank(nSkill), "Rank");
    }

    CResList cFeatList;
    CResStruct cFeat;
    pGFF->AddList(&cFeatList, &cTopLevel, "FeatList");
    for (unsigned short nFeat = 0; nFeat < (unsigned short)GetCreatureStats()->GetNumFeats(); nFeat++) {
        unsigned short nFeatId = GetCreatureStats()->GetFeat(nFeat);
        pGFF->AddListElement(&cFeat, &cFeatList, 0);
        pGFF->WriteFieldWORD(&cFeat, nFeatId, "Feat");
    }

    CSWRace* pRace = &g_pRules->m_lstRaces[GetCreatureStats()->GetRace()];
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetSTR() - pRace->GetSTRAdjust(), "Str");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetDEX() - pRace->GetDEXAdjust(), "Dex");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetCON() - pRace->GetCONAdjust(), "Con");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetINT() - pRace->GetINTAdjust(), "Int");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetWIS() - pRace->GetWISAdjust(), "Wis");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetCHA() - pRace->GetCHAAdjust(), "Cha");

    short nCurrentHitPoints = GetCreatureStats()->GetBaseHitPoints() + GetCreatureStats()->GetCurrentHitPoints() -
                              GetCreatureStats()->GetMaxHitPoints();
    short nCurrentForce = GetCreatureStats()->GetForceSum() -
                          (unsigned short)GetCreatureStats()->GetMaxForcePoints() +
                          (unsigned short)GetCreatureStats()->GetForcePoints();
    pGFF->WriteFieldSHORT(&cTopLevel, nCurrentHitPoints, "CurrentHitPoints");
    pGFF->WriteFieldSHORT(&cTopLevel, GetCreatureStats()->GetBaseHitPoints(), "HitPoints");
    pGFF->WriteFieldSHORT(&cTopLevel, GetCreatureStats()->GetMaxHitPoints(), "MaxHitPoints");
    pGFF->WriteFieldSHORT(&cTopLevel, GetCreatureStats()->GetCurrentHitPoints(), "PregameCurrent");
    pGFF->WriteFieldSHORT(&cTopLevel, nCurrentForce, "CurrentForce");
    pGFF->WriteFieldSHORT(&cTopLevel, GetCreatureStats()->GetForcePoints(), "ForcePoints");
    pGFF->WriteFieldSHORT(&cTopLevel, GetCreatureStats()->GetMaxForcePoints(), "MaxForcePoints");

    CResList cLevelList;
    pGFF->AddList(&cLevelList, &cTopLevel, "LvlStatList");
    CResStruct cLevel;
    pGFF->AddListElement(&cLevel, &cLevelList, 0);
    pGFF->WriteFieldBYTE(&cLevel, g_pRules->GetClassObject(GetCreatureStats()->GetClass(0))->GetHitDie(),
                         "LvlStatHitDie");
    pGFF->WriteFieldBYTE(&cLevel, GetCreatureStats()->GetClass(0), "LvlStatClass");

    if (GetCreatureStats()->GetNumberKnownSpells(0, 0)) {
        CExoString sKnownList;
        sKnownList.Format("%s%d", "KnownList", 0);
        pGFF->AddList(&cKnownList, &cLevel, sKnownList.CStr());
    }
    for (unsigned char nSpell = 0; nSpell < GetCreatureStats()->GetNumberKnownSpells(0, 0); nSpell++) {
        CResStruct cSpell;
        pGFF->AddListElement(&cSpell, &cKnownList, 0);
        pGFF->WriteFieldWORD(&cSpell, GetCreatureStats()->GetKnownSpell(0, 0, nSpell), "Spell");
    }
    pGFF->WriteFieldWORD(&cLevel, GetCreatureStats()->GetSkillPoints(), "SkillPoints");

    pGFF->AddList(&cSkillList, &cLevel, "SkillList");
    for (unsigned short nSkill = 0; nSkill < g_pRules->m_nNumSkills; nSkill++) {
        pGFF->AddListElement(&cSkill, &cSkillList, 0);
        pGFF->WriteFieldBYTE(&cSkill, GetCreatureStats()->GetSkillRank(nSkill), "Rank");
    }

    pGFF->AddList(&cFeatList, &cLevel, "FeatList");
    for (unsigned short nFeat = 0; nFeat < (unsigned short)GetCreatureStats()->GetNumFeats(); nFeat++) {
        unsigned short nFeatId = GetCreatureStats()->GetFeat(nFeat);
        pGFF->AddListElement(&cFeat, &cFeatList, 0);
        pGFF->WriteFieldWORD(&cFeat, nFeatId, "Feat");
    }

    pGFF->WriteFieldDWORD(&cTopLevel, GetCreatureStats()->GetExperience(), "Experience");
    pGFF->WriteFieldWORD(&cTopLevel, GetCreatureStats()->GetPortraitId(), "PortraitId");
    pGFF->WriteFieldCResRef(&cTopLevel, GetCreatureStats()->GetPortrait(0), "Portrait");
    pGFF->WriteFieldBYTE(&cTopLevel, (unsigned char)GetCreatureStats()->GetGoodEvil(), "GoodEvil");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetColorSkin(), "Color_Skin");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetColorHair(), "Color_Hair");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetColorTattoo1(), "Color_Tattoo1");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetColorTattoo2(), "Color_Tattoo2");
    pGFF->WriteFieldINT(&cTopLevel, GetCreatureStats()->GetPhenotype(), "Phenotype");
    pGFF->WriteFieldWORD(&cTopLevel, GetCreatureStats()->GetAppearanceType(), "Appearance_Type");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetAppearanceHead(), "Appearance_Head");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetDuplicatingHead(), "DuplicatingHead");
    pGFF->WriteFieldBYTE(&cTopLevel, (unsigned char)GetCreatureStats()->GetUseBackupHead(), "UseBackupHead");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetTail(), "Tail");
    pGFF->WriteFieldBYTE(&cTopLevel, GetCreatureStats()->GetWings(), "Wings");

    CResList* pItemList = new CResList;
    CResStruct cItem;
    pGFF->AddList(pItemList, &cTopLevel, "ItemList");
    for (unsigned char nItem = 0; nItem < GetCreatureStats()->GetNumInventoryRes(); nItem++) {
        pGFF->AddListElement(&cItem, pItemList, 0);
        CResRef cItemRes = GetCreatureStats()->m_pInventoryRes[nItem];
        pGFF->WriteFieldCResRef(&cItem, cItemRes, "InventoryRes");
    }
    delete pItemList;

    CExoString sScript;
    sScript = "k_hen_heartbt01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptHeartbeat");
    sScript = "k_hen_percept01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptOnNotice");
    sScript = "k_def_spellat01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptSpellAt");
    sScript = "k_hen_attacked01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptAttacked");
    sScript = "k_def_damage01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptDamaged");
    sScript = "";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptDisturbed");
    sScript = "k_hen_combend01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptEndRound");
    sScript = "k_hen_dialogue01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptDialogue");
    sScript = "k_hen_spawn01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptSpawn");
    sScript = "";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptRested");
    sScript = "";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptDeath");
    sScript = "k_def_userdef01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptUserDefine");
    sScript = "k_def_blocked01";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptOnBlocked");
    sScript = "";
    pGFF->WriteFieldCResRef(&cTopLevel, sScript, "ScriptEndDialogue");

    CExoString sResName;
    CExoString sFileName;
    sFileName = "temp";
    sResName = sFileName;
    sFileName = CExoString("TEMP:") + sFileName;
    pGFF->WriteGFFFile(sFileName.CStr(), 0x7df);
    delete pGFF;
}
