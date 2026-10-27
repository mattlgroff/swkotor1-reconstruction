// CTwoDimArrays constructor, destructor and the Load2DArrays / UnLoad2DArrays
// drivers (0x005BFB60, 0x005BFCC0, 0x005C4C20, 0x005C5070).
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

extern unsigned char g_nAppMode;  // 0x007A39DC (1: client)

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

    CTwoDimArrays();
    ~CTwoDimArrays();
    int Load2DArrays();
    int UnLoad2DArrays();
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
    int Load2DArrays_DifficultyOptions();
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

// FUNCTION: SWKOTOR 0x005BFB60
CTwoDimArrays::CTwoDimArrays()
{
    m_pAppearance = 0;
    m_pGender = 0;
    m_pSurfaceMaterial = 0;
    m_pStateScripts = 0;
    m_pVisualEffect = 0;
    m_pAreaTransition = 0;
    m_pTraps = 0;
    m_pPersistentVisualEffect = 0;
    m_pCreatureSpeed = 0;
    m_pPoison = 0;
    m_pDisease = 0;
    m_pLightColor = 0;
    m_pRepAdjustments = 0;
    m_pFractionalCR = 0;
    m_pEncounterDifficulty = 0;
    m_pDoorTypes = 0;
    m_pGenericDoors = 0;
    m_pPlaceables = 0;
    m_pExcitedDuration = 0;
    m_pRegeneration = 0;
    m_pCursor = 0;
    m_pIPRPMelee = 0;
    m_pIPRPMonsterDamage = 0;
    m_pIPRPDamage = 0;
    m_pIPRPBonusCost = 0;
    m_pIPRPSRCost = 0;
    m_pIPRPNeg5Cost = 0;
    m_pXpBase = 0;
    m_pRanges = 0;
    m_pItemPropDef = 0;
    m_paIPRPCostTables = 0;
    m_nNumIPRPCostTables = 0;
    m_paIPRPParamTables = 0;
    m_nNumIPRPParamTables = 0;
    m_pAmbientMusic = 0;
    m_pAmbientSound = 0;
    m_pFootstepSounds = 0;
    m_pAppearanceSounds = 0;
    m_pWeaponSounds = 0;
    m_pAmmunitionTypes = 0;
    m_pKeymap = 0;
    m_pOnHit = 0;
    m_pOnHitDuration = 0;
    m_pDamageHitVisual = 0;
    m_pPlaceableSounds = 0;
    m_pBodyBag = 0;
    m_pPortrait = 0;
    m_pCameraStyle = 0;
    m_pRepute = 0;
    m_pWeaponDischarge = 0;
    m_pDroidDischarge = 0;
    m_pPlotXP = 0;
    m_pAnimations = 0;
    m_pCombatAnimations = 0;
    m_pDialogAnimations = 0;
    m_pSpells = 0;
    m_pFeat = 0;
    m_pFormations = 0;
    m_pGrenadeSound = 0;
    m_pPlanetary = 0;
    m_pFeedBackText = 0;
    m_pForceShields = 0;
    m_pHeads = 0;
    m_pItemValues = 0;
    m_pVideoEffects = 0;
    m_pDifficultyOptions = 0;
    m_pGamma = 0;
    m_pTutorial = 0;
    m_pMovies = 0;
    m_pEffectIcon = 0;
    m_pRemoveFXOnDeath = 0;
    m_pTexPacks = 0;
}

// FUNCTION: SWKOTOR 0x005BFCC0
CTwoDimArrays::~CTwoDimArrays()
{
    if (m_pRemoveFXOnDeath)
        delete m_pRemoveFXOnDeath;
    m_pRemoveFXOnDeath = 0;
    if (m_pEffectIcon)
        delete m_pEffectIcon;
    m_pEffectIcon = 0;
    if (m_pAppearance)
        delete m_pAppearance;
    m_pAppearance = 0;
    if (m_pGender)
        delete m_pGender;
    m_pGender = 0;
    if (m_pSurfaceMaterial)
        delete m_pSurfaceMaterial;
    m_pSurfaceMaterial = 0;
    if (m_pStateScripts)
        delete m_pStateScripts;
    m_pStateScripts = 0;
    if (m_pVisualEffect)
        delete m_pVisualEffect;
    m_pVisualEffect = 0;
    if (m_pAreaTransition)
        delete m_pAreaTransition;
    m_pAreaTransition = 0;
    if (m_pTraps)
        delete m_pTraps;
    m_pTraps = 0;
    if (m_pPersistentVisualEffect)
        delete m_pPersistentVisualEffect;
    m_pPersistentVisualEffect = 0;
    if (m_pCreatureSpeed)
        delete m_pCreatureSpeed;
    m_pCreatureSpeed = 0;
    if (m_pDoorTypes)
        delete m_pDoorTypes;
    m_pDoorTypes = 0;
    if (m_pPlaceables)
        delete m_pPlaceables;
    m_pPlaceables = 0;
    if (m_pGenericDoors)
        delete m_pGenericDoors;
    m_pGenericDoors = 0;
    if (m_pPoison)
        delete m_pPoison;
    m_pPoison = 0;
    if (m_pDisease)
        delete m_pDisease;
    m_pDisease = 0;
    if (m_pRepAdjustments)
        delete m_pRepAdjustments;
    m_pRepAdjustments = 0;
    if (m_pFractionalCR)
        delete m_pFractionalCR;
    m_pFractionalCR = 0;
    if (m_pEncounterDifficulty)
        delete m_pEncounterDifficulty;
    m_pEncounterDifficulty = 0;
    if (m_pExcitedDuration)
        delete m_pExcitedDuration;
    m_pExcitedDuration = 0;
    if (m_pRegeneration)
        delete m_pRegeneration;
    m_pRegeneration = 0;
    if (m_pLightColor)
        delete m_pLightColor;
    m_pLightColor = 0;
    if (m_pIPRPMelee)
        delete m_pIPRPMelee;
    m_pIPRPMelee = 0;
    if (m_pXpBase)
        delete m_pXpBase;
    m_pXpBase = 0;
    if (m_pCursor)
        delete m_pCursor;
    m_pCursor = 0;
    if (m_pIPRPMonsterDamage)
        delete m_pIPRPMonsterDamage;
    m_pIPRPMonsterDamage = 0;
    if (m_pIPRPDamage)
        delete m_pIPRPDamage;
    m_pIPRPDamage = 0;
    if (m_pIPRPBonusCost)
        delete m_pIPRPBonusCost;
    m_pIPRPBonusCost = 0;
    if (m_pIPRPSRCost)
        delete m_pIPRPSRCost;
    m_pIPRPSRCost = 0;
    if (m_pIPRPNeg5Cost)
        delete m_pIPRPNeg5Cost;
    m_pIPRPNeg5Cost = 0;
    if (m_pRanges)
        delete m_pRanges;
    m_pRanges = 0;
    if (m_pItemPropDef)
        delete m_pItemPropDef;
    m_pItemPropDef = 0;
    delete[] m_paIPRPCostTables;
    m_paIPRPCostTables = 0;
    m_nNumIPRPCostTables = 0;
    delete[] m_paIPRPParamTables;
    m_paIPRPParamTables = 0;
    m_nNumIPRPParamTables = 0;
    if (m_pAmbientMusic)
        delete m_pAmbientMusic;
    m_pAmbientMusic = 0;
    if (m_pAmbientSound)
        delete m_pAmbientSound;
    m_pAmbientSound = 0;
    if (m_pFootstepSounds)
        delete m_pFootstepSounds;
    m_pFootstepSounds = 0;
    if (m_pAppearanceSounds)
        delete m_pAppearanceSounds;
    m_pAppearanceSounds = 0;
    if (m_pWeaponSounds)
        delete m_pWeaponSounds;
    m_pWeaponSounds = 0;
    if (m_pAmmunitionTypes)
        delete m_pAmmunitionTypes;
    m_pAmmunitionTypes = 0;
    if (m_pKeymap)
        delete m_pKeymap;
    m_pKeymap = 0;
    if (m_pBindableKey)
        delete m_pBindableKey;
    m_pBindableKey = 0;
    if (m_pOnHit)
        delete m_pOnHit;
    m_pOnHit = 0;
    if (m_pOnHitDuration)
        delete m_pOnHitDuration;
    m_pOnHitDuration = 0;
    if (m_pDamageHitVisual)
        delete m_pDamageHitVisual;
    m_pDamageHitVisual = 0;
    if (m_pPlaceableSounds)
        delete m_pPlaceableSounds;
    m_pPlaceableSounds = 0;
    if (m_pBodyBag)
        delete m_pBodyBag;
    m_pBodyBag = 0;
    if (m_pPortrait)
        delete m_pPortrait;
    m_pPortrait = 0;
    if (m_pForceAdjust)
        delete m_pForceAdjust;
    m_pForceAdjust = 0;
    if (m_pGameEffects)
        delete m_pGameEffects;
    m_pGameEffects = 0;
    if (m_pCameraStyle)
        delete m_pCameraStyle;
    m_pCameraStyle = 0;
    if (m_pRepute)
        delete m_pRepute;
    m_pRepute = 0;
    if (m_pWeaponDischarge)
        delete m_pWeaponDischarge;
    m_pWeaponDischarge = 0;
    if (m_pDroidDischarge)
        delete m_pDroidDischarge;
    m_pDroidDischarge = 0;
    if (m_pPlotXP)
        delete m_pPlotXP;
    m_pPlotXP = 0;
    if (m_pAnimations)
        delete m_pAnimations;
    m_pAnimations = 0;
    if (m_pCombatAnimations)
        delete m_pCombatAnimations;
    m_pCombatAnimations = 0;
    if (m_pDialogAnimations)
        delete m_pDialogAnimations;
    m_pDialogAnimations = 0;
    if (m_pNPC)
        delete m_pNPC;
    m_pNPC = 0;
    if (m_pSpells)
        delete m_pSpells;
    m_pSpells = 0;
    if (m_pFeat)
        delete m_pFeat;
    m_pFeat = 0;
    if (m_pFormations)
        delete m_pFormations;
    m_pFormations = 0;
    if (m_pGrenadeSound)
        delete m_pGrenadeSound;
    m_pGrenadeSound = 0;
    if (m_pPlanetary)
        delete m_pPlanetary;
    m_pPlanetary = 0;
    if (m_pFeedBackText)
        delete m_pFeedBackText;
    m_pFeedBackText = 0;
    if (m_pForceShields)
        delete m_pForceShields;
    m_pForceShields = 0;
    if (m_pHeads)
        delete m_pHeads;
    m_pHeads = 0;
    if (m_pItemValues)
        delete m_pItemValues;
    m_pItemValues = 0;
    if (m_pVideoEffects)
        delete m_pVideoEffects;
    m_pVideoEffects = 0;
    if (m_pTutorial)
        delete m_pTutorial;
    m_pTutorial = 0;
    if (m_pDifficultyOptions)
        delete m_pDifficultyOptions;
    m_pDifficultyOptions = 0;
    if (m_pGamma)
        delete m_pGamma;
    m_pGamma = 0;
    if (m_pMovies)
        delete m_pMovies;
    m_pMovies = 0;
    if (m_pTexPacks)
        delete m_pTexPacks;
    m_pTexPacks = 0;
}

// FUNCTION: SWKOTOR 0x005C5070
int CTwoDimArrays::UnLoad2DArrays()
{
    UnLoadIPRPCostTables();
    UnLoadIPRPParamTables();
    if (m_pRemoveFXOnDeath)
        m_pRemoveFXOnDeath->Unload2DArray();
    if (m_pEffectIcon)
        m_pEffectIcon->Unload2DArray();
    if (m_pAppearance)
        m_pAppearance->Unload2DArray();
    if (m_pGender)
        m_pGender->Unload2DArray();
    if (m_pSurfaceMaterial)
        m_pSurfaceMaterial->Unload2DArray();
    if (m_pStateScripts)
        m_pStateScripts->Unload2DArray();
    if (m_pVisualEffect)
        m_pVisualEffect->Unload2DArray();
    if (m_pAreaTransition)
        m_pAreaTransition->Unload2DArray();
    if (m_pTraps)
        m_pTraps->Unload2DArray();
    if (m_pPersistentVisualEffect)
        m_pPersistentVisualEffect->Unload2DArray();
    if (m_pCreatureSpeed)
        m_pCreatureSpeed->Unload2DArray();
    if (m_pDoorTypes)
        m_pDoorTypes->Unload2DArray();
    if (m_pPlaceables)
        m_pPlaceables->Unload2DArray();
    if (m_pGenericDoors)
        m_pGenericDoors->Unload2DArray();
    if (m_pPoison)
        m_pPoison->Unload2DArray();
    if (m_pDisease)
        m_pDisease->Unload2DArray();
    if (m_pRepAdjustments)
        m_pRepAdjustments->Unload2DArray();
    if (m_pFractionalCR)
        m_pFractionalCR->Unload2DArray();
    if (m_pEncounterDifficulty)
        m_pEncounterDifficulty->Unload2DArray();
    if (m_pExcitedDuration)
        m_pExcitedDuration->Unload2DArray();
    if (m_pRegeneration)
        m_pRegeneration->Unload2DArray();
    if (m_pLightColor)
        m_pLightColor->Unload2DArray();
    if (m_pIPRPMelee)
        m_pIPRPMelee->Unload2DArray();
    if (m_pItemPropDef)
        m_pItemPropDef->Unload2DArray();
    if (m_pXpBase)
        m_pXpBase->Unload2DArray();
    if (m_pCursor)
        m_pCursor->Unload2DArray();
    if (m_pIPRPMonsterDamage)
        m_pIPRPMonsterDamage->Unload2DArray();
    if (m_pIPRPDamage)
        m_pIPRPDamage->Unload2DArray();
    if (m_pIPRPBonusCost)
        m_pIPRPBonusCost->Unload2DArray();
    if (m_pIPRPSRCost)
        m_pIPRPSRCost->Unload2DArray();
    if (m_pIPRPNeg5Cost)
        m_pIPRPNeg5Cost->Unload2DArray();
    if (m_pAmbientMusic)
        m_pAmbientMusic->Unload2DArray();
    if (m_pAmbientSound)
        m_pAmbientMusic->Unload2DArray();
    if (m_pFootstepSounds)
        m_pFootstepSounds->Unload2DArray();
    if (m_pAppearanceSounds)
        m_pAppearanceSounds->Unload2DArray();
    if (m_pWeaponSounds)
        m_pWeaponSounds->Unload2DArray();
    if (m_pAmmunitionTypes)
        m_pAmmunitionTypes->Unload2DArray();
    if (m_pKeymap)
        m_pKeymap->Unload2DArray();
    if (m_pBindableKey)
        m_pBindableKey->Unload2DArray();
    if (m_pOnHit)
        m_pOnHit->Unload2DArray();
    if (m_pOnHitDuration)
        m_pOnHitDuration->Unload2DArray();
    if (m_pDamageHitVisual)
        m_pDamageHitVisual->Unload2DArray();
    if (m_pPlaceableSounds)
        m_pPlaceableSounds->Unload2DArray();
    if (m_pBodyBag)
        m_pBodyBag->Unload2DArray();
    if (m_pPortrait)
        m_pPortrait->Unload2DArray();
    if (m_pCameraStyle)
        m_pCameraStyle->Unload2DArray();
    if (m_pRepute)
        m_pRepute->Unload2DArray();
    if (m_pPlotXP)
        m_pPlotXP->Unload2DArray();
    if (m_pNPC)
        m_pNPC->Unload2DArray();
    if (m_pSpells)
        m_pSpells->Unload2DArray();
    if (m_pFeat)
        m_pFeat->Unload2DArray();
    if (m_pFormations)
        m_pFormations->Unload2DArray();
    if (m_pGrenadeSound)
        m_pGrenadeSound->Unload2DArray();
    if (m_pPlanetary)
        m_pPlanetary->Unload2DArray();
    if (m_pFeedBackText)
        m_pFeedBackText->Unload2DArray();
    if (m_pItemValues)
        m_pItemValues->Unload2DArray();
    if (m_pVideoEffects)
        m_pVideoEffects->Unload2DArray();
    if (m_pTutorial)
        m_pTutorial->Unload2DArray();
    if (m_pDifficultyOptions)
        m_pDifficultyOptions->Unload2DArray();
    if (m_pGamma)
        m_pGamma->Unload2DArray();
    if (m_pTexPacks)
        m_pTexPacks->Unload2DArray();
    return 1;
}

// The failure exit is a shared label: every early-out jumps forward to one
// "return 0" placed before the final "return 1".
// FUNCTION: SWKOTOR 0x005C4C20
int CTwoDimArrays::Load2DArrays()
{
    LoadIPRPCostTables();
    LoadIPRPParamTables();
    if (!Load2DArrays_Appearance())
        goto failed;
    if (!Load2DArrays_Gender())
        goto failed;
    if (!Load2DArrays_SurfaceMaterial())
        goto failed;
    if (!Load2DArrays_VisualEffect())
        goto failed;
    if (!Load2DArrays_PersistentVisualEffect())
        goto failed;
    if (!Load2DArrays_CreatureSpeed())
        goto failed;
    if (!Load2DArrays_DoorTypes())
        goto failed;
    if (!Load2DArrays_GenericDoors())
        goto failed;
    if (!Load2DArrays_Placeables())
        goto failed;
    if (!Load2DArrays_AreaTransition())
        goto failed;
    if (!Load2DArrays_IPRPMelee())
        goto failed;
    if (!Load2DArrays_ItemPropDef())
        goto failed;
    if (!Load2DArrays_Portrait())
        goto failed;
    if (!Load2DArrays_IPRPDamage())
        goto failed;
    if (!Load2DArrays_Traps())
        goto failed;
    if (!Load2DArrays_LightColor())
        goto failed;
    if (g_nAppMode != 2) {
        if (!Load2DArrays_Cursor())
            goto failed;
        if (!Load2DArrays_AmbientMusic())
            goto failed;
        if (!Load2DArrays_AmbientSound())
            goto failed;
        if (!Load2DArrays_FootstepSounds())
            goto failed;
        if (!Load2DArrays_AppearanceSounds())
            goto failed;
        if (!Load2DArrays_WeaponSounds())
            goto failed;
        if (!Load2DArrays_AmmunitionTypes())
            goto failed;
        if (!Load2DArrays_Keymap())
            goto failed;
        if (!Load2DArrays_BindableKey())
            goto failed;
        if (!Load2DArrays_PlaceableSounds())
            goto failed;
        if (!Load2DArrays_CameraStyle())
            goto failed;
        if (!Load2DArrays_Tutorial())
            goto failed;
        if (!Load2DArrays_DifficultyOptions())
            goto failed;
        if (!Load2DArrays_Gamma())
            goto failed;
    }
    if (g_nAppMode != 1) {
        if (!Load2DArrays_StateScripts())
            goto failed;
        if (!Load2DArrays_Poison())
            goto failed;
        if (!Load2DArrays_Disease())
            goto failed;
        if (!Load2DArrays_RepAdjustments())
            goto failed;
        if (!Load2DArrays_FractionalCR())
            goto failed;
        if (!Load2DArrays_EncounterDifficulty())
            goto failed;
        if (!Load2DArrays_ExcitedDuration())
            goto failed;
        if (!Load2DArrays_Regeneration())
            goto failed;
        if (!Load2DArrays_XpBase())
            goto failed;
        if (!Load2DArrays_IPRPMonsterDamage())
            goto failed;
        if (!Load2DArrays_IPRPBonusCost())
            goto failed;
        if (!Load2DArrays_IPRPSRCost())
            goto failed;
        if (!Load2DArrays_IPRPNeg5Cost())
            goto failed;
        if (!Load2DArrays_Ranges())
            goto failed;
        if (!Load2DArrays_OnHit())
            goto failed;
        if (!Load2DArrays_OnHitDuration())
            goto failed;
        if (!Load2DArrays_DamageHitVisual())
            goto failed;
        if (!Load2DArrays_Animations())
            goto failed;
        if (!Load2DArrays_BodyBag())
            goto failed;
        if (!Load2DArrays_ForceAdjust())
            goto failed;
        if (!Load2DArrays_GameEffects())
            goto failed;
        if (!Load2DArrays_GameEffects())
            goto failed;
        if (!Load2DArrays_Repute())
            goto failed;
        if (!Load2DArrays_WeaponDischarge())
            goto failed;
        if (!Load2DArrays_DroidDischarge())
            goto failed;
        if (!Load2DArrays_PlotXP())
            goto failed;
        if (!Load2DArrays_CombatAnimations())
            goto failed;
        if (!Load2DArrays_DialogAnimations())
            goto failed;
        if (!Load2DArrays_NPC())
            goto failed;
        if (!Load2DArrays_Spells())
            goto failed;
        if (!Load2DArrays_Feat())
            goto failed;
        if (!Load2DArrays_Formations())
            goto failed;
        if (!Load2DArrays_GrenadeSound())
            goto failed;
        if (!Load2DArrays_Planetary())
            goto failed;
        if (!Load2DArrays_FeedBackText())
            goto failed;
        if (!Load2DArrays_ForceShields())
            goto failed;
        if (!Load2DArrays_Heads())
            goto failed;
        if (!Load2DArrays_ItemValues())
            goto failed;
        if (!Load2DArrays_VideoEffects())
            goto failed;
        if (!Load2DArrays_Movies())
            goto failed;
        if (!Load2DArrays_EffectIcon())
            goto failed;
        if (!Load2DArrays_RemoveFXOnDeath())
            goto failed;
        if (!Load2DArrays_TexPacks())
            goto failed;
    }
    return 1;
failed:
    return 0;
}
