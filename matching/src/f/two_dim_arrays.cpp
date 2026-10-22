// CTwoDimArrays 2DA loaders (b005c_f2_2da_loaders).
// Load2DArrays_DifficultyOptions (0x005C2680) is matched in pilot1.

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char *szName);
    CResRef(const CResRef &cResRef);
};

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    char *CStr() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    virtual ~C2DA();
    char pad_0x4[0x20];
    int m_nNumRows;               // 0x24
    char pad_0x28[0x2c];
    C2DA(CResRef cResRef, int bPrivate);
    int Load2DArray();
    void Unload2DArray();
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);
    int GetColumnIndex(const CExoString &sColumn);
    int GetColumn(const CExoString &sColumn) { return GetColumnIndex(sColumn); }
    int GetNumRows() { return m_nNumRows; }
};

// Column indices cached by the column-aware loaders.
extern int g_nFootstepPitchOffsetColumn;     // 0x007A21A0
extern int g_nFootstepRollingColumn;         // 0x007A219C
extern int g_nWeaponSoundPitchOffsetColumn;  // 0x007A21A4
extern int g_nRegenForceRegenColumn;         // 0x007A21AC
extern int g_nRegenHealthRegenColumn;        // 0x007A21A8
extern int g_nAnimAttackColumn;              // 0x007A217C
extern int g_nAnimDamageColumn;              // 0x007A2168
extern int g_nAnimDialogColumn;              // 0x007A2170
extern int g_nAnimDodgeColumn;               // 0x007A2178
extern int g_nAnimFireForgetColumn;          // 0x007A214C
extern int g_nAnimHideEquippedItemsColumn;   // 0x007A2180
extern int g_nAnimLoopingColumn;             // 0x007A2154
extern int g_nAnimNameColumn;                // 0x007A2174
extern int g_nAnimOverlayColumn;             // 0x007A2150
extern int g_nAnimParryColumn;               // 0x007A216C
extern int g_nAnimPauseColumn;               // 0x007A215C
extern int g_nAnimRunningColumn;             // 0x007A2164
extern int g_nAnimStationaryColumn;          // 0x007A2158
extern int g_nAnimWalkingColumn;             // 0x007A2160
extern int g_nForceAdjustEvilCostColumn;     // 0x007A2188
extern int g_nForceAdjustGoodCostColumn;     // 0x007A2184
extern int g_nDialogAnimDialogColumn;        // 0x007A2190
extern int g_nDialogAnimFireForgetColumn;    // 0x007A218C
extern int g_nDialogAnimLoopingColumn;       // 0x007A2194
extern int g_nDialogAnimOverlayColumn;       // 0x007A2198
extern int g_nAppearanceCol_ABORTONPARRY;  // 0x7A20C0
extern int g_nAppearanceCol_BACKUPHEAD;  // 0x7A20C4
extern int g_nAppearanceCol_CameraHeightOffset;  // 0x7A20C8
extern int g_nAppearanceCol_CAMERASPACE;  // 0x7A20CC
extern int g_nAppearanceCol_CREPERSPACE;  // 0x7A20D0
extern int g_nAppearanceCol_DisableInjuredAnim;  // 0x7A20D4
extern int g_nAppearanceCol_DRIVEACCl;  // 0x7A20D8
extern int g_nAppearanceCol_DriveAnimRun;  // 0x7A20E4
extern int g_nAppearanceCol_DriveAnimWalk;  // 0x7A20E0
extern int g_nAppearanceCol_DriveMaxSpeed;  // 0x7A20DC
extern int g_nAppearanceCol_ENVMAP;  // 0x7A20E8
extern int g_nAppearanceCol_FootstepType;  // 0x7A20EC
extern int g_nAppearanceCol_HEAD_ARC_H;  // 0x7A20F4
extern int g_nAppearanceCol_HEAD_ARC_V;  // 0x7A20F8
extern int g_nAppearanceCol_HEADBONE;  // 0x7A20FC
extern int g_nAppearanceCol_HEADTRACK;  // 0x7A2100
extern int g_nAppearanceCol_HEIGHT;  // 0x7A2104
extern int g_nAppearanceCol_HITRADIUS;  // 0x7A210C
extern int g_nAppearanceCol_hitdist;  // 0x7A2108
extern int g_nAppearanceCol_GROUNDTILT;  // 0x7A20F0
extern int g_nAppearanceCol_MODELTYPE;  // 0x7A2110
extern int g_nAppearanceCol_NORMALHEAD;  // 0x7A2114
extern int g_nAppearanceCol_PERSPACE;  // 0x7A2118
extern int g_nAppearanceCol_RACE;  // 0x7A211C
extern int g_nAppearanceCol_RUNDIST;  // 0x7A2120
extern int g_nAppearanceCol_SIZECATEGORY;  // 0x7A2124
extern int g_nAppearanceCol_SoundAppType;  // 0x7A2128
extern int g_nAppearanceCol_WALKDIST;  // 0x7A212C
extern int g_nAppearanceSoundsCol_ArmorType;  // 0x7A2144
extern int g_nAppearanceSoundsCol_FallDirt;  // 0x7A2130
extern int g_nAppearanceSoundsCol_FallHard;  // 0x7A2138
extern int g_nAppearanceSoundsCol_FallMetal;  // 0x7A2134
extern int g_nAppearanceSoundsCol_FallWater;  // 0x7A213C
extern int g_nAppearanceSoundsCol_Weapon;  // 0x7A2140
extern int g_nGenderCol_GENDER;  // 0x7A2148
extern int g_nSurfaceMaterialCol_Name;  // 0x7A21B0
extern int g_nVisualEffectCol_Imp_HeadCon_Node;  // 0x7A21B4
extern int g_nVisualEffectCol_Imp_Impact_Node;  // 0x7A21B8
extern int g_nVisualEffectCol_Imp_Root_H_Node;  // 0x7A21C8
extern int g_nVisualEffectCol_Imp_Root_L_Node;  // 0x7A21C4
extern int g_nVisualEffectCol_Imp_Root_M_Node;  // 0x7A21BC
extern int g_nVisualEffectCol_Imp_Root_S_Node;  // 0x7A21C0
extern int g_nVisualEffectCol_LowQuality;  // 0x7A21CC
extern int g_nVisualEffectCol_OrientWithGround;  // 0x7A21D0
extern int g_nVisualEffectCol_ShakeDelay;  // 0x7A21DC
extern int g_nVisualEffectCol_ShakeDuration;  // 0x7A21D8
extern int g_nVisualEffectCol_ShakeType;  // 0x7A21D4
extern int g_nVisualEffectCol_SoundDuration;  // 0x7A21E4
extern int g_nVisualEffectCol_SoundImpact;  // 0x7A21E0
extern int g_nPersistentVisualEffectCol_DurationVFX;  // 0x7A21E8
extern int g_nPersistentVisualEffectCol_OrientWithGround;  // 0x7A21EC
extern int g_nPersistentVisualEffectCol_SHAPE;  // 0x7A21F0
extern int g_nPersistentVisualEffectCol_RADIUS;  // 0x7A21F4
extern int g_nPersistentVisualEffectCol_WIDTH;  // 0x7A21F8
extern int g_nPersistentVisualEffectCol_LENGTH;  // 0x7A21FC
extern int g_nPersistentVisualEffectCol_MODELMIN01;  // 0x7A2200
extern int g_nPersistentVisualEffectCol_MODEL01;  // 0x7A2204
extern int g_nPersistentVisualEffectCol_MODELMIN02;  // 0x7A2208
extern int g_nPersistentVisualEffectCol_MODEL02;  // 0x7A220C
extern int g_nPersistentVisualEffectCol_MODELMIN03;  // 0x7A2210
extern int g_nPersistentVisualEffectCol_MODEL03;  // 0x7A2214
extern int g_nPersistentVisualEffectCol_NUMACT01;  // 0x7A2218
extern int g_nPersistentVisualEffectCol_NUMACT02;  // 0x7A221C
extern int g_nPersistentVisualEffectCol_NUMACT03;  // 0x7A2220
extern int g_nPersistentVisualEffectCol_DURATION01;  // 0x7A2224
extern int g_nPersistentVisualEffectCol_DURATION02;  // 0x7A2228
extern int g_nPersistentVisualEffectCol_DURATION03;  // 0x7A222C
extern int g_nPersistentVisualEffectCol_EDGEWGHT01;  // 0x7A2230
extern int g_nPersistentVisualEffectCol_EDGEWGHT02;  // 0x7A2234
extern int g_nPersistentVisualEffectCol_EDGEWGHT03;  // 0x7A2238
extern int g_nPersistentVisualEffectCol_SoundOneShot;  // 0x7A223C
extern int g_nPersistentVisualEffectCol_SoundOneShotPercentage;  // 0x7A2240
extern int g_nCreatureSpeedCol_RUNRATE;  // 0x7A2244
extern int g_nCreatureSpeedCol_WALKRATE;  // 0x7A2248
extern int g_nDoorTypesCol_SoundAppType;  // 0x7A224C
extern int g_nGenericDoorsCol_SoundAppType;  // 0x7A2250
extern int g_nPlaceablesCol_SoundAppType;  // 0x7A2254
extern int g_nPlaceablesCol_StrRef;  // 0x7A2258
extern int g_nPlaceablesCol_IgnoreStaticHitcheck;  // 0x7A225C
extern int g_nPlaceableSoundsCol_ArmorType;  // 0x7A2260
extern int g_nPlaceableSoundsCol_Locked;  // 0x7A2264
extern int g_nPlaceableSoundsCol_Opened;  // 0x7A2268
extern int g_nPlaceableSoundsCol_Closed;  // 0x7A226C
extern int g_nPlaceableSoundsCol_Destroyed;  // 0x7A2270

class CTwoDimArrays {
public:
    C2DA **m_paIPRPCostTables;               // 0x0
    C2DA **m_paIPRPParamTables;              // 0x4
    unsigned char m_nNumIPRPCostTables;      // 0x8
    unsigned char m_nNumIPRPParamTables;     // 0x9
    char pad_0xa[0x2];
    C2DA *m_pAppearance;  // 0xc
    C2DA *m_pGender;  // 0x10
    C2DA *m_pSurfaceMaterial;  // 0x14
    C2DA *m_pAreaTransition;  // 0x18
    C2DA *m_pPersistentVisualEffect;  // 0x1c
    C2DA *m_pCreatureSpeed;  // 0x20
    C2DA *m_pDoorTypes;  // 0x24
    C2DA *m_pGenericDoors;  // 0x28
    C2DA *m_pPlaceables;  // 0x2c
    C2DA *m_pIPRPMelee;  // 0x30
    C2DA *m_pItemPropDef;  // 0x34
    C2DA *m_pPortrait;  // 0x38
    C2DA *m_pAnimations;  // 0x3c
    C2DA *m_pDialogAnimations;  // 0x40
    C2DA *m_pHeads;  // 0x44
    C2DA *m_pLightColor;  // 0x48
    C2DA *m_pCursor;  // 0x4c
    C2DA *m_pAmbientMusic;  // 0x50
    C2DA *m_pAmbientSound;  // 0x54
    C2DA *m_pFootstepSounds;  // 0x58
    C2DA *m_pAppearanceSounds;  // 0x5c
    C2DA *m_pWeaponSounds;  // 0x60
    C2DA *m_pAmmunitionTypes;  // 0x64
    C2DA *m_pKeymap;  // 0x68
    C2DA *m_pBindableKey;  // 0x6c
    C2DA *m_pPlaceableSounds;  // 0x70
    C2DA *m_pCameraStyle;  // 0x74
    C2DA *m_pCombatAnimations;  // 0x78
    C2DA *m_pDifficultyOptions;  // 0x7c
    C2DA *m_pGamma;  // 0x80
    C2DA *m_pStateScripts;  // 0x84
    C2DA *m_pVisualEffect;  // 0x88
    C2DA *m_pTraps;  // 0x8c
    C2DA *m_pPoison;  // 0x90
    C2DA *m_pDisease;  // 0x94
    C2DA *m_pRepAdjustments;  // 0x98
    C2DA *m_pFractionalCR;  // 0x9c
    C2DA *m_pExcitedDuration;  // 0xa0
    C2DA *m_pRegeneration;  // 0xa4
    C2DA *m_pEncounterDifficulty;  // 0xa8
    C2DA *m_pIPRPMonsterDamage;  // 0xac
    C2DA *m_pIPRPDamage;  // 0xb0
    C2DA *m_pIPRPBonusCost;  // 0xb4
    C2DA *m_pIPRPSRCost;  // 0xb8
    C2DA *m_pIPRPNeg5Cost;  // 0xbc
    C2DA *m_pXpBase;  // 0xc0
    C2DA *m_pRanges;  // 0xc4
    C2DA *m_pOnHit;  // 0xc8
    C2DA *m_pOnHitDuration;  // 0xcc
    C2DA *m_pDamageHitVisual;  // 0xd0
    C2DA *m_pBodyBag;  // 0xd4
    C2DA *m_pForceAdjust;  // 0xd8
    C2DA *m_pGameEffects;  // 0xdc
    C2DA *m_pRepute;  // 0xe0
    C2DA *m_pWeaponDischarge;  // 0xe4
    C2DA *m_pDroidDischarge;  // 0xe8
    C2DA *m_pPlotXP;  // 0xec
    C2DA *m_pNPC;  // 0xf0
    C2DA *m_pSpells;  // 0xf4
    C2DA *m_pFeat;  // 0xf8
    C2DA *m_pFormations;  // 0xfc
    C2DA *m_pGrenadeSound;  // 0x100
    C2DA *m_pPlanetary;  // 0x104
    C2DA *m_pFeedBackText;  // 0x108
    C2DA *m_pForceShields;  // 0x10c
    C2DA *m_pVideoEffects;  // 0x110
    C2DA *m_pItemValues;  // 0x114
    C2DA *m_pTutorial;  // 0x118
    C2DA *m_pMovies;  // 0x11c
    C2DA *m_pEffectIcon;  // 0x120
    C2DA *m_pRemoveFXOnDeath;  // 0x124
    C2DA *m_pTexPacks;  // 0x128

    int Load2DArrays_AreaTransition();
    int Load2DArrays_IPRPMelee();
    int Load2DArrays_ItemPropDef();
    int Load2DArrays_Portrait();
    int Load2DArrays_IPRPDamage();
    int Load2DArrays_Traps();
    int Load2DArrays_LightColor();
    int Load2DArrays_Cursor();
    int Load2DArrays_AmbientMusic();
    int Load2DArrays_AmbientSound();
    int Load2DArrays_AmmunitionTypes();
    int Load2DArrays_Keymap();
    int Load2DArrays_BindableKey();
    int Load2DArrays_CameraStyle();
    int Load2DArrays_Tutorial();
    int Load2DArrays_Gamma();
    int Load2DArrays_StateScripts();
    int Load2DArrays_Poison();
    int Load2DArrays_Disease();
    int Load2DArrays_RepAdjustments();
    int Load2DArrays_FractionalCR();
    int Load2DArrays_EncounterDifficulty();
    int Load2DArrays_ExcitedDuration();
    int Load2DArrays_XpBase();
    int Load2DArrays_IPRPMonsterDamage();
    int Load2DArrays_IPRPBonusCost();
    int Load2DArrays_IPRPSRCost();
    int Load2DArrays_IPRPNeg5Cost();
    int Load2DArrays_Ranges();
    int Load2DArrays_OnHit();
    int Load2DArrays_OnHitDuration();
    int Load2DArrays_DamageHitVisual();
    int Load2DArrays_BodyBag();
    int Load2DArrays_GameEffects();
    int Load2DArrays_Repute();
    int Load2DArrays_WeaponDischarge();
    int Load2DArrays_DroidDischarge();
    int Load2DArrays_PlotXP();
    int Load2DArrays_CombatAnimations();
    int Load2DArrays_NPC();
    int Load2DArrays_Spells();
    int Load2DArrays_Feat();
    int Load2DArrays_Formations();
    int Load2DArrays_GrenadeSound();
    int Load2DArrays_Planetary();
    int Load2DArrays_FeedBackText();
    int Load2DArrays_ForceShields();
    int Load2DArrays_Heads();
    int Load2DArrays_ItemValues();
    int Load2DArrays_VideoEffects();
    int Load2DArrays_Movies();
    int Load2DArrays_EffectIcon();
    int Load2DArrays_RemoveFXOnDeath();
    int Load2DArrays_TexPacks();
    int Load2DArrays_FootstepSounds();
    int Load2DArrays_WeaponSounds();
    int Load2DArrays_Regeneration();
    int Load2DArrays_Animations();
    int Load2DArrays_ForceAdjust();
    int Load2DArrays_DialogAnimations();
    int LoadIPRPCostTables();
    int UnLoadIPRPCostTables();
    C2DA *GetIPRPCostTable(unsigned char nTable);
    int LoadIPRPParamTables();
    int UnLoadIPRPParamTables();
    C2DA *GetIPRPParamTable(unsigned char nTable);
    int Load2DArrays_Appearance();
    int Load2DArrays_AppearanceSounds();
    int Load2DArrays_Gender();
    int Load2DArrays_SurfaceMaterial();
    int Load2DArrays_VisualEffect();
    int Load2DArrays_PersistentVisualEffect();
    int Load2DArrays_CreatureSpeed();
    int Load2DArrays_DoorTypes();
    int Load2DArrays_GenericDoors();
    int Load2DArrays_Placeables();
    int Load2DArrays_PlaceableSounds();
};

// FUNCTION: SWKOTOR 0x005C1B40
int CTwoDimArrays::Load2DArrays_AreaTransition()
{
    m_pAreaTransition = new C2DA(CResRef("Loadscreens"), 0);
    if (m_pAreaTransition && m_pAreaTransition->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1BE0
int CTwoDimArrays::Load2DArrays_IPRPMelee()
{
    m_pIPRPMelee = new C2DA(CResRef("IPRP_MeleeCost"), 0);
    if (m_pIPRPMelee && m_pIPRPMelee->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1C80
int CTwoDimArrays::Load2DArrays_ItemPropDef()
{
    m_pItemPropDef = new C2DA(CResRef("ItemPropDef"), 0);
    if (m_pItemPropDef && m_pItemPropDef->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1D20
int CTwoDimArrays::Load2DArrays_Portrait()
{
    m_pPortrait = new C2DA(CResRef("Portraits"), 0);
    if (m_pPortrait && m_pPortrait->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1DC0
int CTwoDimArrays::Load2DArrays_IPRPDamage()
{
    m_pIPRPDamage = new C2DA(CResRef("IPRP_DAMAGECOST"), 0);
    if (m_pIPRPDamage && m_pIPRPDamage->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1E60
int CTwoDimArrays::Load2DArrays_Traps()
{
    m_pTraps = new C2DA(CResRef("Traps"), 0);
    if (m_pTraps && m_pTraps->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1F00
int CTwoDimArrays::Load2DArrays_LightColor()
{
    m_pLightColor = new C2DA(CResRef("LightColor"), 0);
    if (m_pLightColor && m_pLightColor->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1FA0
int CTwoDimArrays::Load2DArrays_Cursor()
{
    m_pCursor = new C2DA(CResRef("cursors"), 0);
    if (m_pCursor && m_pCursor->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2040
int CTwoDimArrays::Load2DArrays_AmbientMusic()
{
    m_pAmbientMusic = new C2DA(CResRef("AmbientMusic"), 0);
    if (m_pAmbientMusic && m_pAmbientMusic->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C20E0
int CTwoDimArrays::Load2DArrays_AmbientSound()
{
    m_pAmbientSound = new C2DA(CResRef("AmbientSound"), 0);
    if (m_pAmbientSound && m_pAmbientSound->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2360
int CTwoDimArrays::Load2DArrays_AmmunitionTypes()
{
    m_pAmmunitionTypes = new C2DA(CResRef("AmmunitionTypes"), 0);
    if (m_pAmmunitionTypes && m_pAmmunitionTypes->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2400
int CTwoDimArrays::Load2DArrays_Keymap()
{
    m_pKeymap = new C2DA(CResRef("keymap"), 0);
    if (m_pKeymap && m_pKeymap->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C24A0
int CTwoDimArrays::Load2DArrays_BindableKey()
{
    m_pBindableKey = new C2DA(CResRef("BindableKeys"), 0);
    if (m_pBindableKey && m_pBindableKey->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2540
int CTwoDimArrays::Load2DArrays_CameraStyle()
{
    m_pCameraStyle = new C2DA(CResRef("CameraStyle"), 0);
    if (m_pCameraStyle && m_pCameraStyle->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C25E0
int CTwoDimArrays::Load2DArrays_Tutorial()
{
    m_pTutorial = new C2DA(CResRef("tutorial"), 0);
    if (m_pTutorial && m_pTutorial->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2720
int CTwoDimArrays::Load2DArrays_Gamma()
{
    m_pGamma = new C2DA(CResRef("gamma"), 0);
    if (m_pGamma && m_pGamma->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C27C0
int CTwoDimArrays::Load2DArrays_StateScripts()
{
    m_pStateScripts = new C2DA(CResRef("StateScripts"), 0);
    if (m_pStateScripts && m_pStateScripts->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2860
int CTwoDimArrays::Load2DArrays_Poison()
{
    m_pPoison = new C2DA(CResRef("poison"), 0);
    if (m_pPoison && m_pPoison->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2900
int CTwoDimArrays::Load2DArrays_Disease()
{
    m_pDisease = new C2DA(CResRef("disease"), 0);
    if (m_pDisease && m_pDisease->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C29A0
int CTwoDimArrays::Load2DArrays_RepAdjustments()
{
    m_pRepAdjustments = new C2DA(CResRef("RepAdjust"), 0);
    if (m_pRepAdjustments && m_pRepAdjustments->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2A40
int CTwoDimArrays::Load2DArrays_FractionalCR()
{
    m_pFractionalCR = new C2DA(CResRef("FractionalCR"), 0);
    if (m_pFractionalCR && m_pFractionalCR->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2AE0
int CTwoDimArrays::Load2DArrays_EncounterDifficulty()
{
    m_pEncounterDifficulty = new C2DA(CResRef("EncDifficulty"), 0);
    if (m_pEncounterDifficulty && m_pEncounterDifficulty->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2B80
int CTwoDimArrays::Load2DArrays_ExcitedDuration()
{
    m_pExcitedDuration = new C2DA(CResRef("ExcitedDuration"), 0);
    if (m_pExcitedDuration && m_pExcitedDuration->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2D40
int CTwoDimArrays::Load2DArrays_XpBase()
{
    m_pXpBase = new C2DA(CResRef("XPTable"), 0);
    if (m_pXpBase && m_pXpBase->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2DE0
int CTwoDimArrays::Load2DArrays_IPRPMonsterDamage()
{
    m_pIPRPMonsterDamage = new C2DA(CResRef("IPRP_MONSTCOST"), 0);
    if (m_pIPRPMonsterDamage && m_pIPRPMonsterDamage->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2E80
int CTwoDimArrays::Load2DArrays_IPRPBonusCost()
{
    m_pIPRPBonusCost = new C2DA(CResRef("IPRP_BONUSCOST"), 0);
    if (m_pIPRPBonusCost && m_pIPRPBonusCost->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2F20
int CTwoDimArrays::Load2DArrays_IPRPSRCost()
{
    m_pIPRPSRCost = new C2DA(CResRef("IPRP_SRCOST"), 0);
    if (m_pIPRPSRCost && m_pIPRPSRCost->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2FC0
int CTwoDimArrays::Load2DArrays_IPRPNeg5Cost()
{
    m_pIPRPNeg5Cost = new C2DA(CResRef("IPRP_NEG5COST"), 0);
    if (m_pIPRPNeg5Cost && m_pIPRPNeg5Cost->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3060
int CTwoDimArrays::Load2DArrays_Ranges()
{
    m_pRanges = new C2DA(CResRef("Ranges"), 0);
    if (m_pRanges && m_pRanges->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3100
int CTwoDimArrays::Load2DArrays_OnHit()
{
    m_pOnHit = new C2DA(CResRef("IPRP_ONHIT"), 0);
    if (m_pOnHit && m_pOnHit->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C31A0
int CTwoDimArrays::Load2DArrays_OnHitDuration()
{
    m_pOnHitDuration = new C2DA(CResRef("IPRP_ONHITDUR"), 0);
    if (m_pOnHitDuration && m_pOnHitDuration->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3240
int CTwoDimArrays::Load2DArrays_DamageHitVisual()
{
    m_pDamageHitVisual = new C2DA(CResRef("DamageHitVisual"), 0);
    if (m_pDamageHitVisual && m_pDamageHitVisual->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3670
int CTwoDimArrays::Load2DArrays_BodyBag()
{
    m_pBodyBag = new C2DA(CResRef("bodybag"), 0);
    if (m_pBodyBag && m_pBodyBag->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3830
int CTwoDimArrays::Load2DArrays_GameEffects()
{
    m_pGameEffects = new C2DA(CResRef("GameEffects"), 0);
    if (m_pGameEffects && m_pGameEffects->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C38D0
int CTwoDimArrays::Load2DArrays_Repute()
{
    m_pRepute = new C2DA(CResRef("Repute"), 0);
    if (m_pRepute && m_pRepute->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3970
int CTwoDimArrays::Load2DArrays_WeaponDischarge()
{
    m_pWeaponDischarge = new C2DA(CResRef("WeaponDischarge"), 0);
    if (m_pWeaponDischarge && m_pWeaponDischarge->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3A10
int CTwoDimArrays::Load2DArrays_DroidDischarge()
{
    m_pDroidDischarge = new C2DA(CResRef("DroidDischarge"), 0);
    if (m_pDroidDischarge && m_pDroidDischarge->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3AB0
int CTwoDimArrays::Load2DArrays_PlotXP()
{
    m_pPlotXP = new C2DA(CResRef("Plot"), 0);
    if (m_pPlotXP && m_pPlotXP->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3B50
int CTwoDimArrays::Load2DArrays_CombatAnimations()
{
    m_pCombatAnimations = new C2DA(CResRef("CombatAnimations"), 0);
    if (m_pCombatAnimations && m_pCombatAnimations->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3D70
int CTwoDimArrays::Load2DArrays_NPC()
{
    m_pNPC = new C2DA(CResRef("NPC"), 0);
    if (m_pNPC && m_pNPC->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3E10
int CTwoDimArrays::Load2DArrays_Spells()
{
    m_pSpells = new C2DA(CResRef("Spells"), 0);
    if (m_pSpells && m_pSpells->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3EB0
int CTwoDimArrays::Load2DArrays_Feat()
{
    m_pFeat = new C2DA(CResRef("Feat"), 0);
    if (m_pFeat && m_pFeat->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3F50
int CTwoDimArrays::Load2DArrays_Formations()
{
    m_pFormations = new C2DA(CResRef("Formations"), 0);
    if (m_pFormations && m_pFormations->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3FF0
int CTwoDimArrays::Load2DArrays_GrenadeSound()
{
    m_pGrenadeSound = new C2DA(CResRef("GrenadeSND"), 0);
    if (m_pGrenadeSound && m_pGrenadeSound->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C4090
int CTwoDimArrays::Load2DArrays_Planetary()
{
    m_pPlanetary = new C2DA(CResRef("Planetary"), 0);
    if (m_pPlanetary && m_pPlanetary->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C4130
int CTwoDimArrays::Load2DArrays_FeedBackText()
{
    m_pFeedBackText = new C2DA(CResRef("FeedBackText"), 0);
    if (m_pFeedBackText && m_pFeedBackText->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C41D0
int CTwoDimArrays::Load2DArrays_ForceShields()
{
    m_pForceShields = new C2DA(CResRef("ForceShields"), 0);
    if (m_pForceShields && m_pForceShields->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C4270
int CTwoDimArrays::Load2DArrays_Heads()
{
    m_pHeads = new C2DA(CResRef("Heads"), 0);
    if (m_pHeads && m_pHeads->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C4310
int CTwoDimArrays::Load2DArrays_ItemValues()
{
    m_pItemValues = new C2DA(CResRef("ItemValue"), 0);
    if (m_pItemValues && m_pItemValues->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C43B0
int CTwoDimArrays::Load2DArrays_VideoEffects()
{
    m_pVideoEffects = new C2DA(CResRef("VideoEffects"), 0);
    if (m_pVideoEffects && m_pVideoEffects->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C4450
int CTwoDimArrays::Load2DArrays_Movies()
{
    m_pMovies = new C2DA(CResRef("Movies"), 0);
    if (m_pMovies && m_pMovies->Load2DArray()) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C44F0
int CTwoDimArrays::Load2DArrays_EffectIcon()
{
    m_pEffectIcon = new C2DA(CResRef("EffectIcon"), 0);
    if (!m_pEffectIcon)
        return 0;
    if (!m_pEffectIcon->Load2DArray()) {
        delete m_pEffectIcon;
        m_pEffectIcon = 0;
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005C45B0
int CTwoDimArrays::Load2DArrays_RemoveFXOnDeath()
{
    m_pRemoveFXOnDeath = new C2DA(CResRef("removefxondeath"), 0);
    if (!m_pRemoveFXOnDeath)
        return 0;
    if (!m_pRemoveFXOnDeath->Load2DArray()) {
        delete m_pRemoveFXOnDeath;
        m_pRemoveFXOnDeath = 0;
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005C4670
int CTwoDimArrays::Load2DArrays_TexPacks()
{
    m_pTexPacks = new C2DA(CResRef("TexPacks"), 0);
    if (!m_pTexPacks)
        return 0;
    if (!m_pTexPacks->Load2DArray()) {
        delete m_pTexPacks;
        m_pTexPacks = 0;
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005C2180
int CTwoDimArrays::Load2DArrays_FootstepSounds()
{
    m_pFootstepSounds = new C2DA(CResRef("FootstepSounds"), 0);
    if (m_pFootstepSounds && m_pFootstepSounds->Load2DArray()) {
        {
            CExoString sColumn("PitchOffset");
            g_nFootstepPitchOffsetColumn = m_pFootstepSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Rolling");
            g_nFootstepRollingColumn = m_pFootstepSounds->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2290
int CTwoDimArrays::Load2DArrays_WeaponSounds()
{
    m_pWeaponSounds = new C2DA(CResRef("WeaponSounds"), 0);
    if (m_pWeaponSounds && m_pWeaponSounds->Load2DArray()) {
        {
            CExoString sColumn("PitchOffset");
            g_nWeaponSoundPitchOffsetColumn = m_pWeaponSounds->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C2C20
int CTwoDimArrays::Load2DArrays_Regeneration()
{
    m_pRegeneration = new C2DA(CResRef("Regeneration"), 0);
    if (m_pRegeneration && m_pRegeneration->Load2DArray()) {
        {
            CExoString sColumn("ForceRegen");
            g_nRegenForceRegenColumn = m_pRegeneration->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HealthRegen");
            g_nRegenHealthRegenColumn = m_pRegeneration->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C32E0
int CTwoDimArrays::Load2DArrays_Animations()
{
    m_pAnimations = new C2DA(CResRef("Animations"), 0);
    if (m_pAnimations && m_pAnimations->Load2DArray()) {
        {
            CExoString sColumn("Attack");
            g_nAnimAttackColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Damage");
            g_nAnimDamageColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Dialog");
            g_nAnimDialogColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Dodge");
            g_nAnimDodgeColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FireForget");
            g_nAnimFireForgetColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HideEquippedItems");
            g_nAnimHideEquippedItemsColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Looping");
            g_nAnimLoopingColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Name");
            g_nAnimNameColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Overlay");
            g_nAnimOverlayColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Parry");
            g_nAnimParryColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Pause");
            g_nAnimPauseColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Running");
            g_nAnimRunningColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Stationary");
            g_nAnimStationaryColumn = m_pAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Walking");
            g_nAnimWalkingColumn = m_pAnimations->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3710
int CTwoDimArrays::Load2DArrays_ForceAdjust()
{
    m_pForceAdjust = new C2DA(CResRef("ForceAdjust"), 0);
    if (m_pForceAdjust && m_pForceAdjust->Load2DArray()) {
        {
            CExoString sColumn("EvilCost");
            g_nForceAdjustEvilCostColumn = m_pForceAdjust->GetColumn(sColumn);
        }
        {
            CExoString sColumn("GoodCost");
            g_nForceAdjustGoodCostColumn = m_pForceAdjust->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C3BF0
int CTwoDimArrays::Load2DArrays_DialogAnimations()
{
    m_pDialogAnimations = new C2DA(CResRef("DialogAnimations"), 0);
    if (m_pDialogAnimations && m_pDialogAnimations->Load2DArray()) {
        {
            CExoString sColumn("Dialog");
            g_nDialogAnimDialogColumn = m_pDialogAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FireForget");
            g_nDialogAnimFireForgetColumn = m_pDialogAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Looping");
            g_nDialogAnimLoopingColumn = m_pDialogAnimations->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Overlay");
            g_nDialogAnimOverlayColumn = m_pDialogAnimations->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C0270
int CTwoDimArrays::Load2DArrays_Appearance()
{
    m_pAppearance = new C2DA(CResRef("Appearance"), 0);
    if (m_pAppearance && m_pAppearance->Load2DArray()) {
        {
            CExoString sColumn("ABORTONPARRY");
            g_nAppearanceCol_ABORTONPARRY = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("BACKUPHEAD");
            g_nAppearanceCol_BACKUPHEAD = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("CameraHeightOffset");
            g_nAppearanceCol_CameraHeightOffset = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("CAMERASPACE");
            g_nAppearanceCol_CAMERASPACE = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("CREPERSPACE");
            g_nAppearanceCol_CREPERSPACE = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DisableInjuredAnim");
            g_nAppearanceCol_DisableInjuredAnim = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DRIVEACCl");
            g_nAppearanceCol_DRIVEACCl = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DriveAnimRun");
            g_nAppearanceCol_DriveAnimRun = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DriveAnimWalk");
            g_nAppearanceCol_DriveAnimWalk = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DriveMaxSpeed");
            g_nAppearanceCol_DriveMaxSpeed = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("ENVMAP");
            g_nAppearanceCol_ENVMAP = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FootstepType");
            g_nAppearanceCol_FootstepType = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HEAD_ARC_H");
            g_nAppearanceCol_HEAD_ARC_H = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HEAD_ARC_V");
            g_nAppearanceCol_HEAD_ARC_V = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HEADBONE");
            g_nAppearanceCol_HEADBONE = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HEADTRACK");
            g_nAppearanceCol_HEADTRACK = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HEIGHT");
            g_nAppearanceCol_HEIGHT = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("HITRADIUS");
            g_nAppearanceCol_HITRADIUS = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("hitdist");
            g_nAppearanceCol_hitdist = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("GROUNDTILT");
            g_nAppearanceCol_GROUNDTILT = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODELTYPE");
            g_nAppearanceCol_MODELTYPE = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("NORMALHEAD");
            g_nAppearanceCol_NORMALHEAD = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("PERSPACE");
            g_nAppearanceCol_PERSPACE = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("RACE");
            g_nAppearanceCol_RACE = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("RUNDIST");
            g_nAppearanceCol_RUNDIST = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SIZECATEGORY");
            g_nAppearanceCol_SIZECATEGORY = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SoundAppType");
            g_nAppearanceCol_SoundAppType = m_pAppearance->GetColumn(sColumn);
        }
        {
            CExoString sColumn("WALKDIST");
            g_nAppearanceCol_WALKDIST = m_pAppearance->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C08F0
int CTwoDimArrays::Load2DArrays_AppearanceSounds()
{
    m_pAppearanceSounds = new C2DA(CResRef("AppearanceSndSet"), 0);
    if (m_pAppearanceSounds && m_pAppearanceSounds->Load2DArray()) {
        {
            CExoString sColumn("ArmorType");
            g_nAppearanceSoundsCol_ArmorType = m_pAppearanceSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FallDirt");
            g_nAppearanceSoundsCol_FallDirt = m_pAppearanceSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FallHard");
            g_nAppearanceSoundsCol_FallHard = m_pAppearanceSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FallMetal");
            g_nAppearanceSoundsCol_FallMetal = m_pAppearanceSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("FallWater");
            g_nAppearanceSoundsCol_FallWater = m_pAppearanceSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Weapon");
            g_nAppearanceSoundsCol_Weapon = m_pAppearanceSounds->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C0AE0
int CTwoDimArrays::Load2DArrays_Gender()
{
    m_pGender = new C2DA(CResRef("GENDER"), 0);
    if (m_pGender && m_pGender->Load2DArray()) {
        {
            CExoString sColumn("GENDER");
            g_nGenderCol_GENDER = m_pGender->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C0BB0
int CTwoDimArrays::Load2DArrays_SurfaceMaterial()
{
    m_pSurfaceMaterial = new C2DA(CResRef("SURFACEMAT"), 0);
    if (m_pSurfaceMaterial && m_pSurfaceMaterial->Load2DArray()) {
        {
            CExoString sColumn("Name");
            g_nSurfaceMaterialCol_Name = m_pSurfaceMaterial->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C0C80
int CTwoDimArrays::Load2DArrays_VisualEffect()
{
    m_pVisualEffect = new C2DA(CResRef("visualeffects"), 0);
    if (m_pVisualEffect && m_pVisualEffect->Load2DArray()) {
        {
            CExoString sColumn("Imp_HeadCon_Node");
            g_nVisualEffectCol_Imp_HeadCon_Node = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Imp_Impact_Node");
            g_nVisualEffectCol_Imp_Impact_Node = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Imp_Root_H_Node");
            g_nVisualEffectCol_Imp_Root_H_Node = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Imp_Root_L_Node");
            g_nVisualEffectCol_Imp_Root_L_Node = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Imp_Root_M_Node");
            g_nVisualEffectCol_Imp_Root_M_Node = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Imp_Root_S_Node");
            g_nVisualEffectCol_Imp_Root_S_Node = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("LowQuality");
            g_nVisualEffectCol_LowQuality = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("OrientWithGround");
            g_nVisualEffectCol_OrientWithGround = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("ShakeDelay");
            g_nVisualEffectCol_ShakeDelay = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("ShakeDuration");
            g_nVisualEffectCol_ShakeDuration = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("ShakeType");
            g_nVisualEffectCol_ShakeType = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SoundDuration");
            g_nVisualEffectCol_SoundDuration = m_pVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SoundImpact");
            g_nVisualEffectCol_SoundImpact = m_pVisualEffect->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1010
int CTwoDimArrays::Load2DArrays_PersistentVisualEffect()
{
    m_pPersistentVisualEffect = new C2DA(CResRef("VFX_Persistent"), 0);
    if (m_pPersistentVisualEffect && m_pPersistentVisualEffect->Load2DArray()) {
        {
            CExoString sColumn("DurationVFX");
            g_nPersistentVisualEffectCol_DurationVFX = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("OrientWithGround");
            g_nPersistentVisualEffectCol_OrientWithGround = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SHAPE");
            g_nPersistentVisualEffectCol_SHAPE = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("RADIUS");
            g_nPersistentVisualEffectCol_RADIUS = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("WIDTH");
            g_nPersistentVisualEffectCol_WIDTH = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("LENGTH");
            g_nPersistentVisualEffectCol_LENGTH = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODELMIN01");
            g_nPersistentVisualEffectCol_MODELMIN01 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODEL01");
            g_nPersistentVisualEffectCol_MODEL01 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODELMIN02");
            g_nPersistentVisualEffectCol_MODELMIN02 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODEL02");
            g_nPersistentVisualEffectCol_MODEL02 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODELMIN03");
            g_nPersistentVisualEffectCol_MODELMIN03 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("MODEL03");
            g_nPersistentVisualEffectCol_MODEL03 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("NUMACT01");
            g_nPersistentVisualEffectCol_NUMACT01 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("NUMACT02");
            g_nPersistentVisualEffectCol_NUMACT02 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("NUMACT03");
            g_nPersistentVisualEffectCol_NUMACT03 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DURATION01");
            g_nPersistentVisualEffectCol_DURATION01 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DURATION02");
            g_nPersistentVisualEffectCol_DURATION02 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("DURATION03");
            g_nPersistentVisualEffectCol_DURATION03 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("EDGEWGHT01");
            g_nPersistentVisualEffectCol_EDGEWGHT01 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("EDGEWGHT02");
            g_nPersistentVisualEffectCol_EDGEWGHT02 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("EDGEWGHT03");
            g_nPersistentVisualEffectCol_EDGEWGHT03 = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SoundOneShot");
            g_nPersistentVisualEffectCol_SoundOneShot = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        {
            CExoString sColumn("SoundOneShotPercentage");
            g_nPersistentVisualEffectCol_SoundOneShotPercentage = m_pPersistentVisualEffect->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1580
int CTwoDimArrays::Load2DArrays_CreatureSpeed()
{
    m_pCreatureSpeed = new C2DA(CResRef("CreatureSpeed"), 0);
    if (m_pCreatureSpeed && m_pCreatureSpeed->Load2DArray()) {
        {
            CExoString sColumn("RUNRATE");
            g_nCreatureSpeedCol_RUNRATE = m_pCreatureSpeed->GetColumn(sColumn);
        }
        {
            CExoString sColumn("WALKRATE");
            g_nCreatureSpeedCol_WALKRATE = m_pCreatureSpeed->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1690
int CTwoDimArrays::Load2DArrays_DoorTypes()
{
    m_pDoorTypes = new C2DA(CResRef("DoorTypes"), 0);
    if (m_pDoorTypes && m_pDoorTypes->Load2DArray()) {
        {
            CExoString sColumn("SoundAppType");
            g_nDoorTypesCol_SoundAppType = m_pDoorTypes->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1760
int CTwoDimArrays::Load2DArrays_GenericDoors()
{
    m_pGenericDoors = new C2DA(CResRef("GenericDoors"), 0);
    if (m_pGenericDoors && m_pGenericDoors->Load2DArray()) {
        {
            CExoString sColumn("SoundAppType");
            g_nGenericDoorsCol_SoundAppType = m_pGenericDoors->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1830
int CTwoDimArrays::Load2DArrays_Placeables()
{
    m_pPlaceables = new C2DA(CResRef("Placeables"), 0);
    if (m_pPlaceables && m_pPlaceables->Load2DArray()) {
        {
            CExoString sColumn("SoundAppType");
            g_nPlaceablesCol_SoundAppType = m_pPlaceables->GetColumn(sColumn);
        }
        {
            CExoString sColumn("StrRef");
            g_nPlaceablesCol_StrRef = m_pPlaceables->GetColumn(sColumn);
        }
        {
            CExoString sColumn("IgnoreStaticHitcheck");
            g_nPlaceablesCol_IgnoreStaticHitcheck = m_pPlaceables->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C1980
int CTwoDimArrays::Load2DArrays_PlaceableSounds()
{
    m_pPlaceableSounds = new C2DA(CResRef("placeableobjsnds"), 0);
    if (m_pPlaceableSounds && m_pPlaceableSounds->Load2DArray()) {
        {
            CExoString sColumn("ArmorType");
            g_nPlaceableSoundsCol_ArmorType = m_pPlaceableSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Locked");
            g_nPlaceableSoundsCol_Locked = m_pPlaceableSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Opened");
            g_nPlaceableSoundsCol_Opened = m_pPlaceableSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Closed");
            g_nPlaceableSoundsCol_Closed = m_pPlaceableSounds->GetColumn(sColumn);
        }
        {
            CExoString sColumn("Destroyed");
            g_nPlaceableSoundsCol_Destroyed = m_pPlaceableSounds->GetColumn(sColumn);
        }
        return 1;
    }
    return 0;
}

extern unsigned char g_nAppMode;  // 0x007A39DC (1: client)

// FUNCTION: SWKOTOR 0x005C4730
int CTwoDimArrays::LoadIPRPCostTables()
{
    int bLoad;
    C2DA *pTable = new C2DA(CResRef("IPRP_COSTTABLE"), 0);
    if (!pTable || !pTable->Load2DArray())
        return 0;
    m_nNumIPRPCostTables = pTable->GetNumRows();
    CExoString sName;
    m_paIPRPCostTables = new C2DA *[m_nNumIPRPCostTables];
    for (int i = 0; i < m_nNumIPRPCostTables; i++) {
        m_paIPRPCostTables[i] = 0;
        bLoad = 1;
        if (g_nAppMode == 1) {
            pTable->GetINTEntry(i, "ClientLoad", &bLoad);
            if (!bLoad)
                continue;
        }
        pTable->GetCExoStringEntry(i, "Name", &sName);
        m_paIPRPCostTables[i] = new C2DA(CResRef(sName.CStr()), 0);
        if (!m_paIPRPCostTables[i]->Load2DArray()) {
            delete pTable;
            return 0;
        }
    }
    delete pTable;
    return 1;
}

// FUNCTION: SWKOTOR 0x005C4940
int CTwoDimArrays::UnLoadIPRPCostTables()
{
    if (m_paIPRPCostTables) {
        for (int i = 0; i < m_nNumIPRPCostTables; i++) {
            if (m_paIPRPCostTables[i]) {
                m_paIPRPCostTables[i]->Unload2DArray();
                delete m_paIPRPCostTables[i];
                m_paIPRPCostTables[i] = 0;
            }
        }
        delete[] m_paIPRPCostTables;
        m_paIPRPCostTables = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005C49A0
C2DA *CTwoDimArrays::GetIPRPCostTable(unsigned char nTable)
{
    C2DA *pTable = 0;
    if (nTable < m_nNumIPRPCostTables && m_paIPRPCostTables)
        pTable = m_paIPRPCostTables[nTable];
    return pTable;
}

// FUNCTION: SWKOTOR 0x005C49C0
int CTwoDimArrays::LoadIPRPParamTables()
{
    C2DA *pTable = new C2DA(CResRef("IPRP_PARAMTABLE"), 0);
    if (!pTable || !pTable->Load2DArray())
        return 0;
    m_nNumIPRPParamTables = pTable->GetNumRows();
    CExoString sTableResRef;
    m_paIPRPParamTables = new C2DA *[m_nNumIPRPParamTables];
    for (int i = 0; i < m_nNumIPRPParamTables; i++) {
        m_paIPRPParamTables[i] = 0;
        pTable->GetCExoStringEntry(i, "TableResRef", &sTableResRef);
        m_paIPRPParamTables[i] = new C2DA(CResRef(sTableResRef.CStr()), 0);
        if (!m_paIPRPParamTables[i]->Load2DArray()) {
            delete pTable;
            return 0;
        }
    }
    delete pTable;
    return 1;
}

// FUNCTION: SWKOTOR 0x005C4B90
int CTwoDimArrays::UnLoadIPRPParamTables()
{
    if (m_paIPRPParamTables) {
        for (int i = 0; i < m_nNumIPRPParamTables; i++) {
            if (m_paIPRPParamTables[i]) {
                m_paIPRPParamTables[i]->Unload2DArray();
                delete m_paIPRPParamTables[i];
                m_paIPRPParamTables[i] = 0;
            }
        }
        delete[] m_paIPRPParamTables;
        m_paIPRPParamTables = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005C4C00
C2DA *CTwoDimArrays::GetIPRPParamTable(unsigned char nTable)
{
    C2DA *pTable = 0;
    if (nTable < m_nNumIPRPParamTables && m_paIPRPParamTables)
        pTable = m_paIPRPParamTables[nTable];
    return pTable;
}
