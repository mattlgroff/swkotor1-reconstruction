// globals.h -- one extern declaration for each game global (CODE_RULES.md, R2)
//
// Collected from the declarations the units carried, sorted by address. A
// global whose units disagreed on its type takes the type most units used;
// research/r2-globals.md lists those choices and the merged aliases.
#ifndef SHARED_GLOBALS_H
#define SHARED_GLOBALS_H

#include "shared/vector.h"

template <class T> class CExoArrayList;
template <class T> class SafePointer;
struct AurColor;
struct AurConsoleVar;
class AurRes;
struct AurResFileList;
class BSPNode;
class Bin;
class CApp;
class CAppManager;
class CAppManagerT;
struct CAurColor;
class CAurFont;
class CAurFontRender;
class CAurGUIImageData;
struct CAurGrassInfo;
struct CAurGrassRecord;
class CAurInternalGL;
class CAurPostString;
class CAurProgram;
class CAurTexture;
class CAurTextureBasic;
class CAuroraInterface;
class CDeletable;
class CEncounterPointsTable;
class CExoBase;
class CExoFileThread;
class CExoInput;
class CExoLinkedListInternal;
class CExoMoviePlayerInternal;
class CExoResMan;
class CExoSound;
class CExoString;
struct CFrameInfoLine;
class CHitInfo;
class CNetLayer;
class CRender;
class CRenderWindow;
struct CSWEquipSlotInfo;
struct CSWGuiClassChoice;
class CSWGuiMainInterface;
class CSWGuiManager;
class CSWRules;
struct CSWSAreaValue8;
struct CSWUpgradeSlotInfo;
class CScriptCompiler;
class CServerExoAppInternal;
class CTlkTable;
struct CToolTipGlobals;
class CVirtualMachine;
class Camera;
class CapPart;
struct CombinerCaps;
class ConsoleFunc;
struct DIDATAFORMAT;
struct EnvMapSource;
class Gob;
class ImposterManager;
class Material;
class MaxTree;
class MdlNodeTriMesh;
class Model;
class NvFaceInfo;
class Part;
class RequestPoolManager;
class Scene;
struct SceneTileSlot;
struct ShadowEdge;
class VertexProgram;
struct VertexProgramArray;
struct VertexProgramData;
struct _GUID;
struct _iobuf;

extern const float g_fSpeedThousand;  // 0x0073D6FC
extern const float g_fThousand;  // 0x0073D6FC (1000.0f)
extern const float AREA_WIND_SCALE_NONE;  // 0x0073D700
extern const float g_fMessageZero;  // 0x0073D700 (0.0f)
extern const float g_fZero;  // 0x0073D700 (0.0f)
extern float g_fZeroEpsilon;  // 0x0073D700
extern const float g_fSpeedMilli;  // 0x0073D708
extern const char g_sEmpty[];  // 0x0073D71C
extern const float AREA_WIND_SCALE_LIGHT;  // 0x0073D764
extern const float g_fOne;  // 0x0073D764 (1.0f)
extern const float g_fSpeedOne;  // 0x0073D764
extern const float g_fSpeedHundredth;  // 0x0073DFEC
extern const float g_fSpeedTenth;  // 0x0073E798
extern const int MIN_CUSTOM_TOKEN;  // 0x0073ECCC
extern const unsigned char GENDER_MALE;  // 0x0073ECD0
extern unsigned char g_nDefaultGender;  // 0x0073ECD0
extern unsigned char g_nDefaultObjectGender;  // 0x0073ECD0
extern unsigned char g_nGender;  // 0x0073ecd0
extern unsigned char g_nLiveFolderCount;  // 0x0073ECD2 (descriptive)
extern unsigned char g_nNumLiveDirectories;  // 0x0073ECD2
extern unsigned int g_nTrackModelViewProjection;  // 0x0073F218
extern unsigned int g_nTrackModelView;  // 0x0073F21C
extern unsigned int g_nTrackIdentity;  // 0x0073F224
extern const unsigned int g_nGLTex2DConst;  // 0x0073f23c
extern int g_aFogModes[3];  // 0x0073F25C
extern unsigned int g_eMaxTextureUnits;  // 0x0073F26C
extern unsigned int g_aBlendFactors[];  // 0x0073F270
extern unsigned int g_aGLBlendFactors[];  // 0x0073f270
extern const unsigned int g_nBlendSrcAlpha;  // 0x0073F270
extern unsigned int g_nGLSrcAlpha;  // 0x0073F270
extern const unsigned int g_nBlendOneMinusSrcAlpha;  // 0x0073F274
extern unsigned int g_nGLOneMinusSrcAlpha;  // 0x0073F274
extern const unsigned int g_nBlendOne;  // 0x0073F278
extern unsigned int g_aGLMatrixModes[];  // 0x0073f290
extern unsigned int g_nGLProjection;  // 0x0073F290
extern unsigned int g_eModelviewMatrix;  // 0x0073F298
extern unsigned int g_nGLModelView;  // 0x0073F298
extern unsigned int g_eTextureMatrix;  // 0x0073F29C
extern unsigned int g_eGLLequal;  // 0x0073F2AC
extern unsigned int g_eAlphaFunc;  // 0x0073F2B0
extern unsigned int g_nGLAlways;  // 0x0073F2BC
extern unsigned int g_eGLCW;  // 0x0073F2C0
extern unsigned int g_eGLCCW;  // 0x0073F2C4
extern unsigned int g_nGLKeep;  // 0x0073F2C8
extern unsigned int g_nGLReplace;  // 0x0073F2D0
extern unsigned int g_eGLIncr;  // 0x0073F2D4
extern unsigned int g_eGLDecr;  // 0x0073F2D8
extern unsigned int g_aPolygonModes[2];  // 0x0073F2E4
extern unsigned int g_aMaterialFaces[];  // 0x0073f2ec
extern unsigned int g_eGLFront;  // 0x0073F2EC
extern unsigned int g_eMaterialFace;  // 0x0073F2EC
extern unsigned int g_aMaterialParams[];  // 0x0073f2f8
extern unsigned int g_eGLAmbient;  // 0x0073F2FC
extern unsigned int g_eGLDiffuse;  // 0x0073F300
extern unsigned int g_eMaterialDiffuse;  // 0x0073F300
extern unsigned int g_eGLEmission;  // 0x0073F304
extern unsigned int g_eGLEmissionMode;  // 0x0073F30C
extern const unsigned int g_aGLLightParams[];  // 0x0073F320 (placeholder name)
extern unsigned int g_nGLAmbient;  // 0x0073F320 (GL_AMBIENT)
extern unsigned int g_nGLDiffuse;  // 0x0073F324 (GL_DIFFUSE)
extern unsigned int g_nGLSpecular;  // 0x0073F328 (GL_SPECULAR)
extern unsigned int g_nGLPosition;  // 0x0073F32C (GL_POSITION)
extern unsigned int g_nGLQuadraticAttenuation;  // 0x0073F330
extern unsigned int g_aPrimitiveModes[];  // 0x0073f334
extern unsigned int g_nGLLineStrip;  // 0x0073F358
extern const unsigned int g_nGLMagFilterConst;  // 0x0073f364
extern const unsigned int g_nGLMinFilterConst;  // 0x0073f368
extern unsigned int g_aGLCompressedFormats[];  // 0x0073f36c
extern const unsigned int g_nSnapshotFormat;  // 0x0073f370 (GL_RGB)
extern unsigned int g_eTexCoordArray;  // 0x0073F398
extern const unsigned int g_nTexCoordArray;  // 0x0073F398
extern unsigned int g_eNormalArray;  // 0x0073F39C
extern const unsigned int g_nNormalArray;  // 0x0073F39C
extern unsigned int g_eColorArray;  // 0x0073F3A0
extern const unsigned int g_nColorArray;  // 0x0073F3A0
extern unsigned int g_eVertexArray;  // 0x0073F3A4
extern const unsigned int g_nVertexArray;  // 0x0073F3A4
extern unsigned int g_eAttribArray5;  // 0x0073F3B4, descriptive
extern unsigned int g_eTangentArray;  // 0x0073F3B8
extern unsigned int g_eBinormalArray;  // 0x0073F3BC
extern unsigned int g_eTexNormalArray;  // 0x0073F3C0
extern const float g_fRate125;  // 0x0073F400 (0.125f)
extern const float g_fRate15;  // 0x0073F414 (1.5f)
extern const float g_fDefaultHeadTurnH;  // 0x00740A0C
extern const unsigned char g_nAnimationTreeType;  // 0x00740A12
extern const unsigned short NODE_TYPE_DUMMY;  // 0x00740a18 (0x001)
extern const unsigned short NODE_TYPE_LIGHT;  // 0x00740a1c (0x003)
extern const unsigned short NODE_TYPE_EMITTER;  // 0x00740a20 (0x005)
extern const unsigned short NODE_TYPE_CAMERA;  // 0x00740a24 (0x009)
extern const unsigned short NODE_TYPE_REFERENCE;  // 0x00740a28 (0x011)
extern const unsigned short NODE_TYPE_TRIMESH;  // 0x00740a2c (0x021)
extern const unsigned short NODE_TYPE_SKIN;  // 0x00740a30 (0x061)
extern const unsigned short NODE_TYPE_ANIM;  // 0x00740a34 (0x0a1)
extern const unsigned short NODE_TYPE_DANGLY;  // 0x00740a38 (0x121)
extern const unsigned short NODE_TYPE_AABB;  // 0x00740a3c (0x221)
extern const unsigned short NODE_TYPE_PATCH;  // 0x00740a40 (0x401)
extern const unsigned short g_nTriggerNodeType;  // 0x00740A40
extern const unsigned short NODE_TYPE_SABER;  // 0x00740a44 (0x821)
extern const short SABER_NUM_SEGMENTS;  // 0x00740a4c
extern const short SABER_NUM_VERTS;  // 0x00740a50
extern const short SABER_NUM_STRIPS;  // 0x00740a54
extern const int g_nMeshLightmapReset;  // 0x00740A70, descriptive
extern const float g_fDarkThreshold;  // 0x00740C84 (3.0f)
extern const float g_fDegToRad;  // 0x007415F8
extern const float g_fHundred;  // 0x00741838 (100.0f)
extern float g_fAreaTransitionDistance;  // 0x00741870
extern const float AREA_WIND_SCALE_STRONG;  // 0x00741BE0
extern float g_fRadToDeg;  // 0x00742EF8
extern float g_fDialogCameraOffset;  // 0x00743264
extern const float g_afCosTable[32];  // 0x00743FE8
extern const float g_afSinTable[32];  // 0x00744070
extern const float g_fBoxEpsilon;  // 0x00744104 (-0.02f)
extern const unsigned long g_nModuleDefaultA4;  // 0x0074410C
extern const unsigned long g_nMillisecondsPerSecond;  // 0x00744174 (placeholder name)
extern const unsigned long g_nMillisecondsPerMinute;  // 0x00744178 (60000)
extern const unsigned long EVENT_TIMED_EVENT;  // 0x007441B8
extern const int EVENT_AI_SOON;  // 0x007441C4 (placeholder name)
extern const unsigned long g_nEvent4;  // 0x007441C4 (4)
extern const unsigned long EVENT_APPLY_EFFECT;  // 0x007441C8
extern const unsigned long g_nEvent7441C8;  // 0x007441C8
extern unsigned long g_nEventProjectileMiss;  // 0x007441C8
extern const unsigned long EVENT_CLOSE_OBJECT;  // 0x007441CC (name guessed)
extern const unsigned long EVENT_OPEN_OBJECT;  // 0x007441D0 (name guessed)
extern const unsigned long EVENT_SPELL_IMPACT;  // 0x007441D4
extern const unsigned long EVENT_0x7441DC;  // 0x007441DC (placeholder name)
extern const unsigned long EVENT_DELAY_INVENTORY;  // 0x007441DC (10)
extern const unsigned long EVENT_SIGNAL_EVENT;  // 0x007441DC (name guessed)
extern const int EVENT_AI_LATER;  // 0x007441E0 (placeholder name)
extern const unsigned long EVENT_DESTROY_OBJECT;  // 0x007441E0 (11)
extern const unsigned long g_nEventRemoveAoE;  // 0x007441E0 (0x0b)
extern const unsigned long EVENT_REMOVE_EFFECT;  // 0x007441EC (name guessed)
extern const unsigned long g_nEventRemoveEffect;  // 0x007441EC (placeholder name)
extern const unsigned long EVENT_ATTACK;  // 0x007441F0
extern const unsigned long EVENT_ON_MELEE_ATTACKED;  // 0x007441F0
extern const unsigned long EVENT_0x7441F4;  // 0x007441F4 (placeholder name)
extern const unsigned long EVENT_SELF;  // 0x00744200
extern const unsigned long EVENT_FEEDBACK_MESSAGE;  // 0x0074420C (placeholder name)
extern const unsigned long EVENT_ON_HIT;  // 0x0074420C
extern const unsigned long EVENT_SUMMON_CREATURE;  // 0x00744214 (0x18, placeholder name)
extern const unsigned long EVENT_ACQUIRE_ITEM;  // 0x00744218
extern const float BROADCAST_SPELL_RANGE_SQUARED;  // 0x007455BC (placeholder name)
extern const float g_fDefaultHeadTurnV;  // 0x007455DC
extern const unsigned long ACTION_IN_PROGRESS;  // 0x00745DBC
extern const int AI_ACTION_IN_PROGRESS;  // 0x00745DBC
extern const int PLOT_PATH_OK;  // 0x00745DBC
extern const unsigned long ACTION_COMPLETE;  // 0x00745DC0
extern const int AI_ACTION_RESULT_005140E0;  // 0x00745DC0 (placeholder name)
extern const int PLOT_PATH_DONE;  // 0x00745DC0
extern const unsigned long ACTION_FAILED;  // 0x00745DC4 (3)
extern const int AI_ACTION_RESULT_005140D0;  // 0x00745DC4 (placeholder name)
extern const int PLOT_PATH_FAIL;  // 0x00745DC4
extern const float g_fInvLog10Of2;  // 0x00745FF8
extern const unsigned char OBJECT_TYPE_MODULE;  // 0x00746632 (3)
extern const unsigned char OBJECT_TYPE_0746633;  // 0x00746633 (placeholder name)
extern const unsigned char OBJECT_TYPE_AREA;  // 0x00746633
extern const unsigned char OBJECT_TYPE_LAST_NON_GAME;  // 0x00746633 (4)
extern const unsigned char g_nObjectTypeArea;  // 0x00746633 (placeholder name)
extern const unsigned char g_nObjectTypeA;  // 0x00746634
extern const unsigned char g_nObjectTypeCreature;  // 0x00746634 (placeholder name)
extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;  // 0x00746635
extern const unsigned char OBJECT_TYPE_TRIGGER;  // 0x00746636
extern const unsigned char g_nObjectTypeTrigger;  // 0x00746636 (placeholder name)
extern const unsigned char OBJECT_TYPE_PROJECTILE;  // 0x00746637
extern const unsigned char g_nObjectTypeB;  // 0x00746638
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
extern const unsigned char g_nObjectTypeC;  // 0x00746639  // 0x007473B8
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern const unsigned char OBJECT_TYPE_AREAOFEFFECT;  // 0x0074663A
extern const unsigned char OBJECT_TYPE_AREA_OF_EFFECT;  // 0x0074663A
extern unsigned char g_nObjectTypeAreaOfEffect;  // 0x0074663A
extern const unsigned char OBJECT_TYPE_WAYPOINT;  // 0x0074663B
extern const unsigned char g_nObjectTypeWaypoint;  // 0x0074663B
extern const unsigned char OBJECT_TYPE_ENCOUNTER;  // 0x0074663C
extern const unsigned char OBJECT_TYPE_STORE;  // 0x0074663D
extern const unsigned char g_nObjectTypeStore;  // 0x0074663D
extern const unsigned char OBJECT_TYPE_SOUND;  // 0x0074663E
extern const unsigned char g_nObjectTypeSound;  // 0x0074663E
extern const unsigned long OBJECT_INVALID_ID;  // 0x00746644
extern const unsigned long g_oidInvalid;  // 0x00746644 (0x7f000000)
extern const unsigned long OBJECT_ALL_POSSESSOR;  // 0x0074664C (0x7FFFFFFF)
extern const unsigned long CLIENT_OBJECT_MASK;  // 0x0074665C
extern const unsigned char GAME_OBJECT_ADD_OK;  // 0x0074666C
extern const unsigned char GAME_OBJECT_ARRAY_SUCCESS;  // 0x0074666C
extern const unsigned char GAME_OBJECT_VALID;  // 0x0074666C
extern char g_cObjectArraySuccess;  // 0x0074666C
extern const unsigned char g_nGameObjectFound;  // 0x0074666C
extern const unsigned char g_nObjectArraySuccess;  // 0x0074666C
extern const int g_nTRUE;  // 0x00746678
extern const unsigned short EFFECT_TYPE_INVALIDEFFECT;  // 0x0074667C (0)
extern const int EFFECT_TYPE_INVALIDEFFECT_DWORD;  // 0x0074667C (0, compared as a dword)
extern const int g_nEffectTrueType74667C;  // 0x0074667C
extern const int EFFECT_TYPE_0746680;  // 0x00746680 (placeholder name)
extern const unsigned short EFFECT_TYPE_HASTE;  // 0x00746680 (1)
extern const int g_nEffectTrueType746680;  // 0x00746680
extern const int EFFECT_TYPE_0746684;  // 0x00746684 (placeholder name)
extern const int EFFECT_TYPE_DAMAGE_RESISTANCE;  // 0x00746684 (2)
extern const int g_nEffectTrueType746684;  // 0x00746684
extern int EFFECT_TYPE_SLOW;  // 0x00746688 (3; NWN true type 3 = slow)
extern const int g_nEffectTrueType746688;  // 0x00746688
extern const unsigned short EFFECT_TYPE_RESURRECTION;  // 0x0074668C (4)
extern const int g_nEffectTrueType746690;  // 0x00746690
extern const int g_nEffectSummonCreature;  // 0x00746694
extern int EFFECT_TYPE_REGENERATE;  // 0x00746698 (7)
extern const int g_nEffectTrueType746698;  // 0x00746698
extern int EFFECT_TYPE_SETSTATE;  // 0x0074669C (8)
extern const int g_nEffectTrueType74669C;  // 0x0074669C
extern const int g_nEffectType7466A0;  // 0x007466A0
extern const int EFFECT_TYPE_07466A4;  // 0x007466A4 (placeholder name)
extern const int EFFECT_TYPE_ATTACK_INCREASE;  // 0x007466A4 (10)
extern const int g_nEffectTrueType7466A4;  // 0x007466A4
extern const int EFFECT_TYPE_ATTACK_DECREASE;  // 0x007466A8 (11)
extern const int g_nEffectTrueType7466A8;  // 0x007466A8
extern const int EFFECT_TYPE_07466AC;  // 0x007466AC (placeholder name)
extern const int EFFECT_TYPE_DAMAGE_REDUCTION;  // 0x007466AC (12)
extern const int g_nEffectTrueType7466AC;  // 0x007466AC
extern const int EFFECT_TYPE_07466B0;  // 0x007466B0 (placeholder name)
extern const int EFFECT_TYPE_DAMAGE_INCREASE;  // 0x007466B0 (13)
extern const int g_nEffectTrueType7466B0;  // 0x007466B0
extern const int EFFECT_TYPE_DAMAGE_DECREASE;  // 0x007466B4 (14)
extern const int g_nEffectTrueType7466B4;  // 0x007466B4
extern const int EFFECT_TYPE_TEMPORARY_HITPOINTS;  // 0x007466B8 (descriptive name)
extern const int g_nEffectTrueType7466B8;  // 0x007466B8
extern const int EFFECT_TYPE_DAMAGE_IMMUNITY;  // 0x007466BC (16)
extern const int g_nEffectTrueType7466BC;  // 0x007466BC
extern const int EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE;  // 0x007466C0 (17)
extern int EFFECT_TYPE_DAMAGE_VULNERABILITY;  // 0x007466C0 (0x11)
extern const int g_nEffectTrueType7466C0;  // 0x007466C0
extern const unsigned short EFFECT_TYPE_ENTANGLE;  // 0x007466C4 (18)
extern const int g_nEffectTrueType7466C4;  // 0x007466C4
extern int EFFECT_TYPE_DEATH;  // 0x007466C8
extern const unsigned short EFFECT_TYPE_KNOCKDOWN;  // 0x007466CC (20)
extern const int g_nEffectTrueType7466D0;  // 0x007466D0
extern const int EFFECT_TYPE_07466D4;  // 0x007466D4 (placeholder name)
extern const int EFFECT_TYPE_IMMUNITY;  // 0x007466D4 (22)
extern const int g_nEffectTrueType7466D4;  // 0x007466D4
extern const int EFFECT_TYPE_07466D8;  // 0x007466D8 (placeholder name)
extern const int g_nEffectTrueType7466DC;  // 0x007466DC
extern const int g_nEffectTrueType7466E0;  // 0x007466E0
extern const int EFFECT_TYPE_07466E4;  // 0x007466E4 (placeholder name)
extern const int EFFECT_TYPE_SAVING_THROW_INCREASE;  // 0x007466E4 (26)
extern const int g_nEffectTrueType7466E4;  // 0x007466E4
extern const int EFFECT_TYPE_SAVING_THROW_DECREASE;  // 0x007466E8 (27)
extern const int g_nEffectTrueType7466E8;  // 0x007466E8
extern const unsigned short EFFECT_TYPE_MOVEMENT_SPEED_INCREASE;  // 0x007466EC
extern const int g_nEffectTrueType7466EC;  // 0x007466EC
extern const int EFFECT_TYPE_07466F0;  // 0x007466F0 (29, placeholder name)
extern const unsigned short EFFECT_TYPE_MOVEMENT_SPEED_DECREASE;  // 0x007466F0
extern const int g_nEffectTrueType7466F0;  // 0x007466F0
extern int EFFECT_TYPE_VISUALEFFECT;  // 0x007466F4 (0x1e)
extern unsigned short EFFECT_TYPE_VISUAL_EFFECT;  // 0x007466F4
extern const int g_nEffectTrueType7466F4;  // 0x007466F4
extern const int g_nEffectType7466F4;  // 0x007466F4
extern const int g_nEffectVisualEffect;  // 0x007466F4
extern const unsigned short EFFECT_TYPE_AREA_OF_EFFECT;  // 0x007466F8
extern const int g_nEffectAreaOfEffect;  // 0x007466F8
extern const int g_nEffectTrueType7466F8;  // 0x007466F8
extern const unsigned short EFFECT_TYPE_BEAM;  // 0x007466FC (0x20)
extern const int g_nEffectTrueType7466FC;  // 0x007466FC
extern const int EFFECT_TYPE_0746700;  // 0x00746700 (placeholder name)
extern const int EFFECT_TYPE_FORCE_RESISTANCE_INCREASE;  // 0x00746700 (33)
extern const int g_nEffectTrueType746700;  // 0x00746700
extern const int EFFECT_TYPE_FORCE_RESISTANCE_DECREASE;  // 0x00746704 (34)
extern const int g_nEffectTrueType746704;  // 0x00746704
extern int EFFECT_TYPE_POISON;  // 0x00746708 (0x23, placeholder)
extern const int g_nEffectTrueType746708;  // 0x00746708
extern const int EFFECT_TYPE_074670C;  // 0x0074670C (placeholder name)
extern int EFFECT_TYPE_ABILITY_INCREASE;  // 0x0074670C (36)
extern const int g_nEffectTrueType74670C;  // 0x0074670C
extern int EFFECT_TYPE_ABILITY_DECREASE;  // 0x00746710 (37)
extern const int g_nEffectTrueType746710;  // 0x00746710
extern int EFFECT_TYPE_DAMAGE;  // 0x00746714
extern const int g_nEffectTrueType746714;  // 0x00746714
extern const unsigned short EFFECT_TYPE_HEAL;  // 0x00746718
extern const int EFFECT_TYPE_LINK;  // 0x0074671C
extern const int g_nEffectLink;  // 0x0074671C
extern const int g_nEffectType74671C;  // 0x0074671C
extern const unsigned short EFFECT_TYPE_MODIFYNUMATTACKS;  // 0x0074672C
extern const int g_nEffectTrueType746730;  // 0x00746730
extern const int g_nEffectTrueType746734;  // 0x00746734
extern const int EFFECT_TYPE_0746738;  // 0x00746738 (47, placeholder name)
extern const unsigned short EFFECT_TYPE_2F;  // 0x00746738 (0x2F)
extern const int g_nEffectTrueType746738;  // 0x00746738
extern const int EFFECT_TYPE_AC_INCREASE;  // 0x0074673C
extern const int g_nEffectTrueType74673C;  // 0x0074673C
extern const int EFFECT_TYPE_AC_DECREASE;  // 0x00746740
extern const int g_nEffectTrueType746740;  // 0x00746740
extern const int EFFECT_TYPE_0746744;  // 0x00746744 (placeholder name)
extern const int EFFECT_TYPE_SPELL_IMMUNITY;  // 0x00746744 (placeholder name)
extern const int g_nEffectTrueType746744;  // 0x00746744
extern const unsigned short EFFECT_TYPE_DISPELMAGICALL;  // 0x00746748 (51)
extern const int g_nEffectTrueType746748;  // 0x00746748
extern const unsigned short EFFECT_TYPE_DISPELMAGICBEST;  // 0x0074674C (52)
extern const int g_nEffectTrueType74674C;  // 0x0074674C
extern int EFFECT_TYPE_PROPERTY29;  // 0x00746754 (item property 29 effect)
extern const int EFFECT_TYPE_0746758;  // 0x00746758 (placeholder name)
extern int EFFECT_TYPE_PROPERTY36;  // 0x00746758
extern const int EFFECT_TYPE_SKILL_INCREASE;  // 0x00746758 (55)
extern const int g_nEffectTrueType746758;  // 0x00746758
extern const int EFFECT_TYPE_SKILL_DECREASE;  // 0x0074675C (56)
extern const int g_nEffectTrueType74675C;  // 0x0074675C
extern const unsigned short EFFECT_TYPE_HITPOINT_CHANGE_WHEN_DYING;  // 0x00746760
extern const int EFFECT_TYPE_0746764;  // 0x00746764 (placeholder name)
extern int EFFECT_TYPE_SPECIALWALK;  // 0x00746764 (0x3a)
extern const int EFFECT_TYPE_0746768;  // 0x00746768 (59, placeholder name)
extern int EFFECT_TYPE_SPECIALWALK_END;  // 0x00746768 (0x3b)
extern const unsigned short EFFECT_TYPE_FORCE_PUSHED;  // 0x0074676C (0x3C)
extern const int EFFECT_TYPE_0746770;  // 0x00746770 (placeholder name)
extern int EFFECT_TYPE_DAMAGE_SHIELD;  // 0x00746770
extern const int g_nEffectTrueType746770;  // 0x00746770
extern const int EFFECT_TYPE_DISGUISE;  // 0x00746774 (0x3E)
extern int EFFECT_TYPE_PROPERTY59;  // 0x00746774
extern const int g_nEffectTrueType746774;  // 0x00746774
extern const int EFFECT_TYPE_0746778;  // 0x00746778 (63, placeholder name)
extern const int g_nEffectTrueType746778;  // 0x00746778
extern const int EFFECT_TYPE_TIMESTOP;  // 0x0074677C
extern const int g_nEffectTrueType74677C;  // 0x0074677C
extern const int EFFECT_TYPE_0746780;  // 0x00746780 (placeholder name)
extern const unsigned short EFFECT_TYPE_SPELLLEVELABSORPTION;  // 0x00746780
extern const int g_nEffectTrueType746780;  // 0x00746780
extern const int g_nEffectType746780;  // 0x00746780
extern const int EFFECT_TYPE_0746788;  // 0x00746788 (placeholder name)
extern int EFFECT_TYPE_746788;  // 0x00746788 (0x43)
extern const int EFFECT_TYPE_ICON;  // 0x00746788 (read as a dword here)
extern const int g_nEffectType746788;  // 0x00746788
extern const unsigned short EFFECT_TYPE_074678C;  // 0x0074678C (68, placeholder name)
extern const int EFFECT_TYPE_0746794;  // 0x00746794 (placeholder name)
extern const unsigned short EFFECT_TYPE_SEEINVISIBLE;  // 0x00746794
extern const int g_nEffectTrueType746794;  // 0x00746794
extern const int EFFECT_TYPE_0746798;  // 0x00746798 (placeholder name)
extern const int g_nEffectTrueType746798;  // 0x00746798
extern int EFFECT_TYPE_TRUESEEING;  // 0x0074679C (0x48)
extern const int g_nEffectTrueType74679C;  // 0x0074679C
extern const int EFFECT_TYPE_07467A0;  // 0x007467A0 (placeholder name)
extern const int g_nEffectTrueType7467A0;  // 0x007467A0
extern const int g_nEffectTrueType7467A4;  // 0x007467A4
extern const int EFFECT_TYPE_07467A8;  // 0x007467A8 (placeholder name)
extern const int g_nEffectTrueType7467A8;  // 0x007467A8
extern const int EFFECT_TYPE_07467AC;  // 0x007467AC (placeholder name)
extern const int EFFECT_TYPE_CONCEALMENT;  // 0x007467AC (76)
extern const int g_nEffectTrueType7467AC;  // 0x007467AC
extern const unsigned short EFFECT_TYPE_APPEAR;  // 0x007467C0 (name guessed)
extern const int g_nEffectTrueType7467C4;  // 0x007467C4
extern int EFFECT_TYPE_BONUS_FEAT;  // 0x007467C8 (0x53)
extern const unsigned short EFFECT_TYPE_FORCE_DRAIN;  // 0x007467E0
extern const unsigned short EFFECT_TYPE_5B;  // 0x007467E4 (0x5B)
extern const unsigned short EFFECT_TYPE_5C;  // 0x007467E8 (0x5C)
extern int EFFECT_TYPE_PROPERTY55;  // 0x007467E8
extern const unsigned short EFFECT_TYPE_5D;  // 0x007467EC (0x5D)
extern int EFFECT_TYPE_PROPERTY56;  // 0x007467EC
extern const unsigned short EFFECT_TYPE_5F;  // 0x007467F4 (0x5F)
extern const unsigned short EFFECT_TYPE_60;  // 0x007467F8 (0x60)
extern const unsigned short EFFECT_TYPE_BODY_FUEL;  // 0x00746800 (0x62)
extern const unsigned short EFFECT_TYPE_PSYCHIC_STATIC;  // 0x00746804 (0x63)
extern const unsigned short EFFECT_TYPE_LIGHTSABERTHROW;  // 0x00746808 (0x64)
extern const int g_nEffectTrueType746808;  // 0x00746808
extern const unsigned short EFFECT_TYPE_ASSUREDHIT;  // 0x0074680C (100)
extern const int g_nEffectTrueType74680C;  // 0x0074680C
extern int EFFECT_TYPE_FORCE_JUMP;  // 0x00746810
extern const int g_nEffectTrueType746810;  // 0x00746810
extern const unsigned short EFFECT_TYPE_ASSUREDDEFLECTION;  // 0x00746818 (103)
extern const int g_nEffectTrueType746818;  // 0x00746818
extern const unsigned short EFFECT_TYPE_FORCE_RESISTED;  // 0x0074681C (104)
extern const unsigned short EFFECT_TYPE_FORCE_FIZZLE;  // 0x00746820 (105)
extern const int EFFECT_FORCE_SHIELD;  // 0x00746824
extern const unsigned short EFFECT_TYPE_FORCE_SHIELD;  // 0x00746824
extern const int EFFECT_TYPE_0746828;  // 0x00746828 (108, placeholder name)
extern const int EFFECT_TYPE_074682C;  // 0x0074682C (109, placeholder name)
extern const int EFFECT_TYPE_0746830;  // 0x00746830 (list-end sentinel, placeholder name)
extern const int ABILITY_STRENGTH;  // 0x00746838 (0)
extern const int ABILITY_CHARISMA;  // 0x0074684C (5)
extern const int DAMAGE_FLAG_00;  // 0x00746850
extern int DAMAGE_FLAG_01;  // 0x00746854
extern int DAMAGE_FLAG_02;  // 0x00746858
extern int DAMAGE_FLAG_03;  // 0x0074685C
extern const int DAMAGE_TYPE_DEFAULT;  // 0x0074685C (8)
extern int DAMAGE_FLAG_04;  // 0x00746860
extern int DAMAGE_FLAG_05;  // 0x00746864
extern int DAMAGE_FLAG_06;  // 0x00746868
extern int DAMAGE_FLAG_07;  // 0x0074686C
extern int DAMAGE_FLAG_08;  // 0x00746870
extern int DAMAGE_FLAG_09;  // 0x00746874
extern int DAMAGE_FLAG_10;  // 0x00746878
extern int DAMAGE_FLAG_11;  // 0x0074687C
extern int DAMAGE_FLAG_12;  // 0x00746880
extern int DAMAGE_FLAG_13;  // 0x00746884
extern const int DAMAGE_TYPE_MAX;  // 0x00746884 (0x2000)
extern int AC_DEFLECTION_BONUS_VS_DAMAGE;  // 0x0074688C (16391): AC modifier damage type
extern const int AC_VS_DAMAGE_TYPE_ALL;  // 0x0074688C (0x4007)
extern const int DAMAGE_POWER_NORMAL;  // 0x00746894 (0)
extern const int DAMAGE_POWER_MAX;  // 0x007468AC (6, name guessed)
extern int AC_TYPE_DODGE;  // 0x007468B0 (0, placeholder name)
extern const int AC_DEFLECTION_BONUS;  // 0x007468C0 (4)
extern const int ATTACK_BONUS_MISC;  // 0x007468C4 (0)
extern int ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON;  // 0x007468C8 (1)
extern int ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON;  // 0x007468CC (2)
extern int ITEM_SLOT_ATTACK_TYPE_CLAW1;  // 0x007468D0 (3)
extern int ITEM_SLOT_ATTACK_TYPE_CLAW2;  // 0x007468D4 (4)
extern int ITEM_SLOT_ATTACK_TYPE_CLAW3;  // 0x007468D8 (5)
extern const int ATTACK_BONUS_LAST;  // 0x007468DC (7, name guessed)
extern int ITEM_SLOT_ATTACK_TYPE_ONHAND;  // 0x007468DC (7)
extern int ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON;  // 0x007468E4 (1)
extern int ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON;  // 0x007468E8 (2)
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW1;  // 0x007468EC
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW2;  // 0x007468F0
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW3;  // 0x007468F4
extern int ITEM_SLOT_DAMAGE_TYPE_ONHAND;  // 0x007468F8
extern const int IMMUNITY_TYPE_NONE;  // 0x007468FC
extern int IMMUNITY_TYPE_01;  // 0x00746900
extern int IMMUNITY_TYPE_02;  // 0x00746904
extern int IMMUNITY_TYPE_03;  // 0x00746908
extern int IMMUNITY_TYPE_04;  // 0x0074690C
extern const unsigned char g_nImmunityTypeTrap;  // 0x00746910 (placeholder name)
extern unsigned char IMMUNITY_TYPE_06;  // 0x00746914
extern unsigned char IMMUNITY_TYPE_12;  // 0x0074692C
extern unsigned char IMMUNITY_TYPE_13;  // 0x00746930
extern unsigned char IMMUNITY_TYPE_16;  // 0x00746934
extern int IMMUNITY_TYPE_19;  // 0x00746940
extern int IMMUNITY_TYPE_MOVEMENT_SPEED_DECREASE;  // 0x00746954 (0x18)
extern int IMMUNITY_TYPE_28;  // 0x00746964
extern int IMMUNITY_TYPE_29;  // 0x00746968
extern int IMMUNITY_TYPE_30;  // 0x0074696C
extern unsigned char IMMUNITY_TYPE_SNEAK_ATTACK;  // 0x0074696C (0x1e, placeholder)
extern int IMMUNITY_TYPE_31;  // 0x00746970
extern unsigned char IMMUNITY_TYPE_CRITICAL_HIT;  // 0x00746970 (0x1f, placeholder)
extern int IMMUNITY_TYPE_32;  // 0x00746974
extern const int IMMUNITY_TYPE_MAX;  // 0x00746978 (0x21)
extern const int MISS_CHANCE_TYPE_NORMAL;  // 0x00746990 (0, name guessed)
extern const float MOVE_AWAY_DEFAULT_RANGE;  // 0x00746B10 (40.0f, name guessed)
extern const char g_szCreditsTemplate[];  // 0x007471C0 (credits item template)
extern const unsigned short DURATION_TYPE_7473B8;  // 0x007473B8
extern const unsigned short DURATION_TYPE_INSTANT;  // 0x007473B8
extern unsigned short SUBTYPE_MAGICAL;  // 0x007473B8
extern const unsigned short DURATION_TYPE_7473BC;  // 0x007473BC
extern unsigned short SUBTYPE_EQUIPPED;  // 0x007473BC (3)
extern const unsigned short g_nDurationInnate;  // 0x007473BC (placeholder name)
extern const unsigned short DURATION_TYPE_7473C0;  // 0x007473C0
extern unsigned short DURATION_TYPE_TEMPORARY;  // 0x007473C0 (1)
extern const unsigned short g_nDurationTemporary;  // 0x007473C0
extern const unsigned short DURATION_TYPE_7473C4;  // 0x007473C4
extern unsigned short DURATION_TYPE_PERMANENT;  // 0x007473C4
extern const unsigned short g_nDurationPermanent;  // 0x007473C4 (placeholder name)
extern const unsigned short DURATION_TYPE_7473C8;  // 0x007473C8
extern const unsigned short g_nDurationEquipped;  // 0x007473C8 (placeholder name)
extern const float PROJECTILE_BROADCAST_RANGE;  // 0x00747440
extern const float g_fVisualEffectBroadcastRange;  // 0x00747440 (250.0f)
extern const float AI_SOUND_RANGE;  // 0x00747444, 1000.0f
extern const float AI_WHISPER_RANGE;  // 0x00747448 (name guessed)
extern float g_fDialogSpeakerRange;  // 0x0074744C (placeholder name, 1000.0)
extern const float g_fFacingEpsilon;  // 0x00747504
extern const float g_fSpeedScale;  // 0x00747C40
extern const unsigned char CLASS_TYPE_SCOUT;  // 0x0074B319
extern const unsigned char g_nClassScout;  // 0x0074B31A (1, descriptive)
extern const unsigned char g_nClassScoundrel;  // 0x0074B31B (2, descriptive)
extern const unsigned char CLASS_TYPE_JEDI_GUARDIAN;  // 0x0074B31C
extern const unsigned char CLASS_TYPE_JEDI_CONSULAR;  // 0x0074B31D
extern const unsigned char CLASS_TYPE_JEDI_SENTINEL;  // 0x0074B31E
extern const unsigned short RACE_TYPE_0074BF10;  // 0x0074BF10 (5, placeholder name)
extern const unsigned short RACIAL_TYPE_DROID;  // 0x0074BF10
extern const unsigned short RACE_TYPE_0074BF14;  // 0x0074BF14 (6, placeholder name)
extern const unsigned short RACIAL_TYPE_HUMAN;  // 0x0074BF14
extern const unsigned short RACIAL_TYPE_COUNT;  // 0x0074BF18 (name guessed)
extern const unsigned short g_nForceTargetHuman;  // 0x0074BF34 (descriptive)
extern const unsigned short g_nForceTargetDroid;  // 0x0074BF38 (descriptive)
extern const unsigned short g_nForceTargetOther;  // 0x0074BF3C (descriptive)
extern const int g_nSmoothStateStart;  // 0x0074C444 (placeholder name)
extern const int g_nSmoothStatePass1;  // 0x0074C448 (placeholder name)
extern const int g_nSmoothStatePass2;  // 0x0074C44C (placeholder name)
extern const int g_nSmoothStatePass3;  // 0x0074C450 (placeholder name)
extern const unsigned long PLAYER_ID_INVALID;  // 0x0074C524 (placeholder name)
extern unsigned long PLAYERID_SERVER;  // 0x0074C528 (0xFFFFFFFD)
extern const unsigned long PLAYER_ID_SERVER_ADMIN;  // 0x0074C534 (placeholder name)
extern int g_nNetStatusTime;  // 0x0074c538
extern unsigned char g_nConnectionType;  // 0x0074C540 (descriptive)
extern unsigned char g_nBarkVolume;  // 0x0074C5E8
extern unsigned char g_nDefaultSourceVolume;  // 0x0074C5E8
extern unsigned char g_nMiniGameAccelVolume;  // 0x0074C5E8
extern const int MUSIC_STATE_NORMAL;  // 0x0074C5EC (0)
extern const int SOUND_MODE_NORMAL;  // 0x0074C5EC
extern const int g_nAreaSoundMode;  // 0x0074C5EC
extern int g_nMusicStateNone;  // 0x0074C5EC
extern int g_nSoundModeBack;  // 0x0074C5EC
extern int g_nSoundModeZero;  // 0x0074C5EC
extern int g_nMusicStateMenu;  // 0x0074C5F0
extern int g_nSoundModeMuted;  // 0x0074C5F0
extern const int MUSIC_STATE_COMBAT_PAUSED;  // 0x0074C5F4 (2)
extern int g_n74C5F4;  // 0x0074C5F4
extern int g_nSoundModeQuiet1;  // 0x0074C5F4
extern const int SOUND_MODE_MOVIE;  // 0x0074C5F8 (3)
extern const int g_nSoundModeMovie;  // 0x0074C5F8
extern int g_nSoundModeQuiet2;  // 0x0074C5F8
extern int g_n74C5FC;  // 0x0074C5FC
extern int g_nGuiSoundMode;  // 0x0074C5FC
extern const int g_nSoundModeMenu;  // 0x0074C5FC
extern int g_nSoundModeQuiet3;  // 0x0074C5FC
extern int g_nSoundModeInactive;  // 0x0074c600
extern int g_nSoundModePauseAll;  // 0x0074C600
extern int g_nSoundModeActive;  // 0x0074c604
extern int g_nSoundModeResumeAll;  // 0x0074C604
extern const int g_nInputAnySlot;  // 0x0074D3C4 (-1)
extern const int g_nInputDeviceKeyboard;  // 0x0074D3C8 (0)
extern const int g_nInputDeviceMouse;  // 0x0074D3CC (1)
extern const int g_nInputDeviceJoystick;  // 0x0074D3D0 (2)
extern const int KEY_DISPLAY_FIRST;  // 0x0074D3D8
extern const int g_nFirstKeyIndex;  // 0x0074D3D8
extern const unsigned long g_nKeyMapKeyBase;  // 0x0074D3D8
extern const int KEY_DISPLAY_LAST;  // 0x0074D3DC
extern const int g_nLastKeyIndex;  // 0x0074D3DC
extern const unsigned long g_nInputMouseLeft;  // 0x0074D3E0
extern const unsigned long g_nKeyMapIndex_74D3E0;  // 0x0074D3E0 (0)
extern const unsigned long g_nInputMouseRight;  // 0x0074D3E4
extern const unsigned long g_nKeyMapIndex_74D3E4;  // 0x0074D3E4 (1)
extern const unsigned long g_nInputMouseMiddle;  // 0x0074D3E8
extern const unsigned long g_nKeyMapIndex_74D3E8;  // 0x0074D3E8 (2)
extern const unsigned long g_nInputMouseAxisX;  // 0x0074D3EC (3)
extern const unsigned long g_nInputMouseAxisY;  // 0x0074D3F0 (4)
extern const unsigned long g_nInputMouseWheel;  // 0x0074D3F4
extern const unsigned long g_nKeyMapIndex_74D3F4;  // 0x0074D3F4 (5)
extern const unsigned long g_nKeyMapIndex_74D3F8;  // 0x0074D3F8 (6)
extern const unsigned long g_nKeyMapIndex_74D3FC;  // 0x0074D3FC (7)
extern const unsigned long g_nKeyMapIndex_74D400;  // 0x0074D400 (8)
extern const unsigned long g_nKeyMapIndex_74D404;  // 0x0074D404 (9)
extern const unsigned long g_nKeyMapIndex_74D408;  // 0x0074D408 (10)
extern const unsigned long g_nKeyMapIndex_74D40C;  // 0x0074D40C (11)
extern const unsigned long g_nKeyMapIndex_74D410;  // 0x0074D410 (12)
extern const unsigned long g_nKeyMapIndex_74D414;  // 0x0074D414 (13)
extern const unsigned long g_nKeyMapIndex_74D418;  // 0x0074D418 (14)
extern const unsigned long g_nKeyMapIndex_74D41C;  // 0x0074D41C (15)
extern const unsigned long g_nKeyMapIndex_74D420;  // 0x0074D420 (16)
extern const unsigned long g_nKeyMapIndex_74D424;  // 0x0074D424 (17)
extern const unsigned long g_nKeyMapIndex_74D428;  // 0x0074D428 (18)
extern const unsigned long g_nKeyMapIndex_74D42C;  // 0x0074D42C (19)
extern const unsigned long g_nKeyMapIndex_74D430;  // 0x0074D430 (20)
extern const unsigned long g_nKeyMapIndex_74D434;  // 0x0074D434 (21)
extern const unsigned long g_nKeyMapIndex_74D438;  // 0x0074D438 (22)
extern const unsigned long g_nKeyMapIndex_74D43C;  // 0x0074D43C (23)
extern const unsigned long g_nKeyMapIndex_74D440;  // 0x0074D440 (24)
extern const unsigned long g_nKeyMapIndex_74D444;  // 0x0074D444 (25)
extern const unsigned long g_nKeyMapIndex_74D448;  // 0x0074D448 (26)
extern const unsigned long g_nKeyMapIndex_74D44C;  // 0x0074D44C (27)
extern const unsigned long g_nKeyMapIndex_74D450;  // 0x0074D450 (28)
extern const unsigned long g_nKeyMapIndex_74D454;  // 0x0074D454 (29)
extern const unsigned long g_nKeyMapIndex_74D458;  // 0x0074D458 (30)
extern const unsigned long g_nKeyMapEscapeKey;  // 0x0074D45C
extern const unsigned long g_nKeyMapIndex_74D460;  // 0x0074D460 (32)
extern const unsigned long g_nKeyMapIndex_74D464;  // 0x0074D464 (33)
extern const unsigned long g_nKeyMapIndex_74D468;  // 0x0074D468 (34)
extern const unsigned long g_nKeyMapIndex_74D46C;  // 0x0074D46C (35)
extern const unsigned long g_nKeyMapIndex_74D470;  // 0x0074D470 (36)
extern const unsigned long g_nKeyMapIndex_74D474;  // 0x0074D474 (37)
extern const unsigned long g_nKeyMapIndex_74D478;  // 0x0074D478 (38)
extern const unsigned long g_nKeyMapIndex_74D47C;  // 0x0074D47C (39)
extern const unsigned long g_nKeyMapIndex_74D480;  // 0x0074D480 (40)
extern const unsigned long g_nKeyMapIndex_74D484;  // 0x0074D484 (41)
extern const unsigned long g_nKeyMapIndex_74D488;  // 0x0074D488 (42)
extern const unsigned long g_nKeyMapIndex_74D48C;  // 0x0074D48C (43)
extern const unsigned long g_nKeyMapIndex_74D490;  // 0x0074D490 (44)
extern const unsigned long g_nKeyMapIndex_74D494;  // 0x0074D494 (45)
extern const unsigned long g_nKeyMapIndex_74D498;  // 0x0074D498 (46)
extern const unsigned long g_nKeyMapIndex_74D49C;  // 0x0074D49C (47)
extern const unsigned long g_nKeyMapIndex_74D4A0;  // 0x0074D4A0 (48)
extern const unsigned long g_nKeyMapIndex_74D4A4;  // 0x0074D4A4 (49)
extern const unsigned long g_nKeyMapIndex_74D4A8;  // 0x0074D4A8 (50)
extern const unsigned long g_nKeyMapIndex_74D4AC;  // 0x0074D4AC (51)
extern const unsigned long g_nKeyMapIndex_74D4B0;  // 0x0074D4B0 (52)
extern const unsigned long g_nKeyMapIndex_74D4B4;  // 0x0074D4B4 (53)
extern const unsigned long g_nKeyMapIndex_74D4B8;  // 0x0074D4B8 (54)
extern const unsigned long g_nKeyMapIndex_74D4BC;  // 0x0074D4BC (55)
extern const unsigned long g_nKeyMapIndex_74D4C0;  // 0x0074D4C0 (56)
extern const unsigned long g_nKeyMapIndex_74D4C4;  // 0x0074D4C4 (57)
extern const unsigned long g_nKeyMapIndex_74D4C8;  // 0x0074D4C8 (58)
extern const unsigned long g_nKeyMapIndex_74D4CC;  // 0x0074D4CC (59)
extern const unsigned long g_nKeyMapIndex_74D4D0;  // 0x0074D4D0 (60)
extern const unsigned long g_nKeyMapIndex_74D4D4;  // 0x0074D4D4 (61)
extern const unsigned long g_nKeyMapIndex_74D4D8;  // 0x0074D4D8 (62)
extern const unsigned long g_nKeyMapIndex_74D4DC;  // 0x0074D4DC (63)
extern const unsigned long g_nKeyMapIndex_74D4E0;  // 0x0074D4E0 (64)
extern const unsigned long g_nKeyMapIndex_74D4E4;  // 0x0074D4E4 (65)
extern const unsigned long g_nKeyMapIndex_74D4E8;  // 0x0074D4E8 (66)
extern const unsigned long g_nKeyMapIndex_74D4EC;  // 0x0074D4EC (67)
extern const unsigned long g_nKeyMapIndex_74D4F0;  // 0x0074D4F0 (68)
extern const unsigned long g_nKeyMapIndex_74D4F4;  // 0x0074D4F4 (69)
extern const unsigned long g_nKeyMapIndex_74D4F8;  // 0x0074D4F8 (70)
extern const unsigned long g_nKeyMapIndex_74D4FC;  // 0x0074D4FC (71)
extern const unsigned long g_nKeyMapIndex_74D500;  // 0x0074D500 (72)
extern const unsigned long g_nKeyMapIndex_74D504;  // 0x0074D504 (73)
extern const unsigned long g_nKeyMapIndex_74D508;  // 0x0074D508 (74)
extern const unsigned long g_nKeyMapIndex_74D50C;  // 0x0074D50C (75)
extern const unsigned long g_nKeyMapIndex_74D510;  // 0x0074D510 (76)
extern const unsigned long g_nKeyMapIndex_74D514;  // 0x0074D514 (77)
extern const unsigned long g_nKeyMapIndex_74D518;  // 0x0074D518 (78)
extern const unsigned long g_nKeyMapIndex_74D51C;  // 0x0074D51C (79)
extern const unsigned long g_nKeyMapIndex_74D520;  // 0x0074D520 (80)
extern const unsigned long g_nKeyMapIndex_74D524;  // 0x0074D524 (81)
extern const unsigned long g_nKeyMapIndex_74D528;  // 0x0074D528 (82)
extern const unsigned long g_nKeyMapIndex_74D52C;  // 0x0074D52C (83)
extern const unsigned long g_nKeyMapIndex_74D530;  // 0x0074D530 (84)
extern const unsigned long g_nKeyMapIndex_74D534;  // 0x0074D534 (85)
extern const unsigned long g_nKeyMapIndex_74D538;  // 0x0074D538 (86)
extern const unsigned long g_nKeyMapIndex_74D53C;  // 0x0074D53C (87)
extern const unsigned long g_nKeyMapIndex_74D540;  // 0x0074D540 (88)
extern const unsigned long g_nKeyMapIndex_74D544;  // 0x0074D544 (89)
extern const unsigned long g_nKeyMapIndex_74D548;  // 0x0074D548 (90)
extern const unsigned long g_nKeyMapIndex_74D54C;  // 0x0074D54C (91)
extern const unsigned long g_nKeyMapIndex_74D550;  // 0x0074D550 (92)
extern const unsigned long g_nKeyMapIndex_74D554;  // 0x0074D554 (93)
extern const unsigned long g_nKeyMapIndex_74D558;  // 0x0074D558 (94)
extern const unsigned long g_nKeyMapIndex_74D55C;  // 0x0074D55C (95)
extern const unsigned long g_nKeyMapIndex_74D560;  // 0x0074D560 (96)
extern const unsigned long g_nKeyMapIndex_74D564;  // 0x0074D564 (97)
extern const unsigned long g_nKeyMapIndex_74D568;  // 0x0074D568 (98)
extern const unsigned long g_nKeyMapIndex_74D56C;  // 0x0074D56C (99)
extern const unsigned long g_nKeyMapIndex_74D570;  // 0x0074D570 (100)
extern const unsigned long g_nKeyMapIndex_74D574;  // 0x0074D574 (101)
extern const unsigned long g_nKeyMapIndex_74D578;  // 0x0074D578 (102)
extern const unsigned long g_nKeyMapIndex_74D57C;  // 0x0074D57C (103)
extern const unsigned long g_nKeyMapIndex_74D580;  // 0x0074D580 (104)
extern const unsigned long g_nKeyMapIndex_74D584;  // 0x0074D584 (105)
extern const unsigned long g_nKeyMapIndex_74D588;  // 0x0074D588 (106)
extern const unsigned long g_nKeyMapIndex_74D58C;  // 0x0074D58C (107)
extern const unsigned long g_nKeyMapIndex_74D590;  // 0x0074D590 (108)
extern const unsigned long g_nKeyMapIndex_74D594;  // 0x0074D594 (110)
extern const unsigned long g_nKeyMapIndex_74D598;  // 0x0074D598 (111)
extern const unsigned long g_nKeyMapIndex_74D59C;  // 0x0074D59C (112)
extern const unsigned long g_nKeyMapIndex_74D5A0;  // 0x0074D5A0 (113)
extern const unsigned long g_nKeyMapIndex_74D5A4;  // 0x0074D5A4 (114)
extern const unsigned long g_nKeyMapIndex_74D5A8;  // 0x0074D5A8 (115)
extern const unsigned long g_nKeyMapIndex_74D5AC;  // 0x0074D5AC (116)
extern const unsigned long g_nKeyMapIndex_74D5B0;  // 0x0074D5B0 (117)
extern const unsigned long g_nKeyMapIndex_74D5B4;  // 0x0074D5B4 (118)
extern const unsigned long g_nKeyMapIndex_74D5B8;  // 0x0074D5B8 (119)
extern const unsigned long g_nKeyMapIndex_74D5BC;  // 0x0074D5BC (120)
extern const unsigned long g_nKeyMapIndex_74D5C0;  // 0x0074D5C0 (121)
extern const unsigned long g_nKeyMapIndex_74D5C4;  // 0x0074D5C4 (122)
extern const unsigned long g_nKeyMapIndex_74D5C8;  // 0x0074D5C8 (123)
extern const unsigned long g_nKeyMapIndex_74D5CC;  // 0x0074D5CC (124)
extern const unsigned long g_nKeyMapIndex_74D5D0;  // 0x0074D5D0 (125)
extern const unsigned long g_nKeyMapIndex_74D5D4;  // 0x0074D5D4 (126)
extern const unsigned long g_nKeyMapIndex_74D5D8;  // 0x0074D5D8 (127)
extern const unsigned long g_nKeyMapIndex_74D5DC;  // 0x0074D5DC (128)
extern const unsigned long g_nKeyMapIndex_74D5E0;  // 0x0074D5E0 (129)
extern const unsigned long g_nKeyMapIndex_74D5E4;  // 0x0074D5E4 (130)
extern const _GUID GUID_SysKeyboard;  // 0x0074D694
extern const _GUID GUID_SysMouse;  // 0x0074D6A4
extern const _GUID IID_IDirectInput8A;  // 0x0074D6F4
extern const DIDATAFORMAT c_dfDIKeyboard;  // 0x0074D8FC
extern const DIDATAFORMAT c_dfDIMouse;  // 0x0074DB04
extern const int COMBAT_MODE_OFF;  // 0x0074F220 (0)
extern const int COMBAT_MODE_ON;  // 0x0074F224 (1)
extern int g_nDefaultCombatMode;  // 0x0074F224
extern char g_sLightsaberContactNode[];  // 0x0074F368
extern char g_sFootstepPrefix[];  // 0x0074F378
extern const int g_nCameraSettingsTypeOnAStick;  // 0x0075182C
extern unsigned long g_aInventoryFilterNames[6];  // 0x00756444 (descriptive)
extern CSWEquipSlotInfo g_aEquipSlots[9];  // 0x00756560 (descriptive)
extern CSWUpgradeSlotInfo g_aUpgradeSlots[4][4];  // 0x00756FB4
extern int g_nScreenWidth;  // 0x0078d1d4
extern int g_nWindowWidth;  // 0x0078D1D4
extern int g_nScreenHeight;  // 0x0078D1D8
extern int g_nWindowHeight;  // 0x0078D1D8
extern unsigned long g_nScreenRefreshRate;  // 0x0078D1DC
extern unsigned char g_nScreenDepth;  // 0x0078D1E0
extern int g_bDo3DGui;  // 0x0078D1E4 (descriptive)
extern unsigned long g_nSavedScreenWidth;  // 0x0078d1f0
extern unsigned long g_nSavedScreenHeight;  // 0x0078d1f4
extern unsigned char g_nSavedScreenDepth;  // 0x0078d1f8
extern int g_bRereadVideoMode;  // 0x0078d1fc
extern char g_szWindowClassName[];  // 0x0078d200
extern char* g_aCursorNames[];  // 0x0078d240
extern Vector g_vDisabledColor;  // 0x0078d3b4
extern Vector g_vHilightColor;  // 0x0078d3c0
extern char* g_sGFFVersion;  // 0x0078d3cc -> "V3.2"
extern float g_fGuiPulse;  // 0x0078d3d4
extern Vector g_vGuiSceneAmbient;  // 0x0078d3d8
extern Vector g_vGuiViewportColor;  // 0x0078D3D8
extern float g_fGuiPulseAlpha;  // 0x0078d3e4
extern const char* g_sTlkFileVersion;  // 0x0078d3ec -> "V3.0"
extern float g_fAurTextureDynamicMemoryRatio;  // 0x0078d3f0
extern float g_fTextureMemScale;  // 0x0078d3f0
extern int g_nTextureMem;  // 0x0078d3f4
extern float g_fLightmapMemScale;  // 0x0078d3f8
extern float g_fTexMemMaxOverTime;  // 0x0078d3fc
extern unsigned char g_bTextureModeDynamic;  // 0x0078d400
extern int g_bProceduralTextures;  // 0x0078d430
extern char g_bVSync;  // 0x0078d438
extern float g_fSpeedBlurRatio;  // 0x0078d43c
extern int g_nPrevAntiAliasingMode;  // 0x0078d440
extern unsigned int g_aGLTextureTargets[];  // 0x0078d448
extern unsigned int g_eGLAlphaTest;  // 0x0078D448
extern unsigned int g_nCapBlend;  // 0x0078d448
extern unsigned int g_nGLBlend;  // 0x0078D448
extern unsigned int g_nGLCap4;  // 0x0078d448
extern unsigned int g_eGLTexture2D;  // 0x0078D44C
extern unsigned int g_nCapAlphaTest;  // 0x0078d44c
extern unsigned int g_nGLAlphaTest;  // 0x0078D44C
extern unsigned int g_nGLCap5;  // 0x0078d44c
extern unsigned int g_eGLFog;  // 0x0078d450
extern unsigned int g_nGLFog;  // 0x0078D450
extern unsigned int g_nGLCap0;  // 0x0078d454
extern unsigned int g_nGLCullFace;  // 0x0078D454
extern unsigned int g_eGLTexture2DB;  // 0x0078D458
extern unsigned int g_nCapDepthTest;  // 0x0078D458
extern unsigned int g_nGLCap3;  // 0x0078d458
extern unsigned int g_nGLTexture2D;  // 0x0078d458
extern unsigned int g_eGLLighting;  // 0x0078D45C
extern unsigned int g_nCapLighting;  // 0x0078d45c
extern unsigned int g_nGLCap2;  // 0x0078d45c
extern unsigned int g_nGLDepthTest;  // 0x0078D45C
extern unsigned int g_eGLTexGenS;  // 0x0078D460
extern unsigned int g_eGLTexGenT;  // 0x0078D464
extern unsigned int g_eGLTexGenR;  // 0x0078D468
extern unsigned int g_eGLTexture3D;  // 0x0078D46C
extern unsigned int g_eVertexProgramTarget;  // 0x0078d46c
extern unsigned int g_nGLStencilTest;  // 0x0078d470
extern unsigned int g_eGLDepthTest;  // 0x0078d474
extern unsigned int g_nGLCap1;  // 0x0078d474
extern unsigned int g_nGLLighting;  // 0x0078D474
extern unsigned int g_eGLTexCubeMap;  // 0x0078D478
extern unsigned int g_nGLTextureCubeMap;  // 0x0078d478
extern unsigned int g_nGLTextureCubeMapEnable;  // 0x0078d47c
extern unsigned int g_eGLFogStart;  // 0x0078D484
extern unsigned int g_eGLFogEnd;  // 0x0078D488
extern unsigned int g_eGLFogDensity;  // 0x0078D48C
extern unsigned int g_eGLFogColor;  // 0x0078D490
extern int g_bUseViewMatrix;  // 0x0078d494
extern int g_nTransformMode;  // 0x0078D494
extern unsigned char g_aFontBitmaps[];  // 0x0078d498
extern int g_bConvolutionAvailable;  // 0x0078d974
extern int g_bScanNoiseAvailable;  // 0x0078d978
extern int g_bSaturationAvailable;  // 0x0078d97c
extern int g_bDistortionAvailable;  // 0x0078d980
extern int g_bSpeedBlurAvailable;  // 0x0078d984
extern int g_bFilmNoiseAvailable;  // 0x0078d988
extern int g_bFrameBufferEffects;  // 0x0078d98c
extern char g_bBloomRendered;  // 0x0078d990
extern int g_bShadowClampFar;  // 0x0078d994
extern unsigned char g_bSoftShadowsAllowed;  // 0x0078dae9
extern char g_bFrameBufferEnabled;  // 0x0078daf4
extern float g_fBloomAlpha;  // 0x0078daf8
extern float g_fBloomOffset;  // 0x0078db00
extern float g_fBloomScale;  // 0x0078db04
extern int g_nBloomLevel;  // 0x0078db08
extern float g_fBloomSize;  // 0x0078db0c
extern int g_bConvolutionKeep;  // 0x0078db20
extern float g_vFogColor[3];  // 0x0078db24
extern Vector g_vSaturationTint;  // 0x0078db24
extern ImposterManager g_ImposterManager;  // 0x0078db30
extern int g_aCubeFaceRotation[6];  // 0x0078db38
extern int g_aCubeFaceOrder[6];  // 0x0078db50
extern float g_afBloomKernelATI[25];  // 0x0078e230
extern float g_afBloomKernel[25];  // 0x0078e2a0
extern char g_szWindowClass[];  // 0x0078e310 "Test GL Window"
extern int g_nNextTriMeshId;  // 0x0078e328
extern unsigned char g_bTextureStateDirty;  // 0x0078E330, descriptive
extern Vector g_vDefaultDiffuse;  // 0x0078e33c
extern Vector g_vDefaultAmbient;  // 0x0078e348
extern int g_nCurrentFont;  // 0x0078e358
extern char g_bFontFlag;  // 0x0078e35c
extern int g_bConsoleOverlay;  // 0x0078E360
extern unsigned char g_bAurPrintfRight;  // 0x0078e364
extern AurColor g_cPostStart;  // 0x0078e368
extern AurColor g_cPostEnd;  // 0x0078e378
extern short g_bHeadLight;  // 0x0078e388
extern short g_nHeadLight;  // 0x0078E388 (placeholder name)
extern float g_fMaterialAlpha;  // 0x0078E390
extern unsigned long g_nClearColor;  // 0x0078e394
extern unsigned char g_bVBOWriteOnly;  // 0x0078e398
extern int g_nGeomFadeMode;  // 0x0078e3a0
extern int g_bShadows;  // 0x0078e3a8
extern int g_bStencilDynamic;  // 0x0078E3A8, descriptive
extern int g_bGrass;  // 0x0078e3ac
extern int g_bRenderBins;  // 0x0078E3AC, descriptive
extern float g_fBinAlphaRef;  // 0x0078E3B0, descriptive
extern int g_bOrderByPriority;  // 0x0078E3B4, descriptive
extern int g_bAnimCulling;  // 0x0078E3B8
extern float g_fVisibilityInterval;  // 0x0078E3BC, descriptive
extern float g_fVisibilityRatio;  // 0x0078E3C0, descriptive
extern int g_bRenderGobs;  // 0x0078E3C4, descriptive
extern float g_fShadowDistance;  // 0x0078E3C8, descriptive
extern int g_bShadowDistanceCheck;  // 0x0078E3CC, descriptive
extern int g_bStaticMeshBeams;  // 0x0078E3D0, descriptive
extern float g_fMinProjectionRadius;  // 0x0078E3D4, descriptive
extern int g_bRenderEmitters;  // 0x0078E3D8, descriptive
extern int g_bRenderScenes;  // 0x0078E3F4, descriptive
extern int g_nRenderFrame3;  // 0x0078E3F8, descriptive
extern int g_bRenderDynamic;  // 0x0078E3FC, descriptive
extern int g_bRenderStatic;  // 0x0078E400, descriptive
extern float g_fAmbientScale;  // 0x0078E408
extern int g_bSoftShadows;  // 0x0078e420
extern float g_fGrassBinSize;  // 0x0078E428
extern float g_fGUIFlushAlpha;  // 0x0078E43C
extern float g_aLowpassKernel[3][3];  // 0x0078E450
extern unsigned char g_aLowpassShift[3][3];  // 0x0078E474
extern unsigned int g_nCap_GL_EXT_bgra;  // 0x0078e480
extern unsigned int g_nCap_GL_ARB_multitexture;  // 0x0078e484
extern unsigned int g_nCap_GL_EXT_texture_env_combine;  // 0x0078e488
extern unsigned int g_nCap_GL_NV_register_combiners;  // 0x0078e48c
extern unsigned int g_nCap_GL_EXT_texture_cube_map;  // 0x0078e490
extern unsigned int g_nCap_GL_EXT_fog_coord;  // 0x0078e494
extern unsigned int g_nCap_GL_EXT_secondary_color;  // 0x0078e498
extern unsigned int g_nCap_GL_NV_vertex_program;  // 0x0078e49c
extern unsigned int g_nCap_GL_EXT_texture_filter_anisotropic;  // 0x0078e4a0
extern unsigned int g_nCap_GL_NV_texture_shader;  // 0x0078e4a8
extern unsigned int g_nCap_GL_NV_vertex_array_range;  // 0x0078e4ac
extern unsigned int g_nCap_GL_NV_vertex_array_range2;  // 0x0078e4b0
extern unsigned int g_nCap_GL_NV_fence;  // 0x0078e4b4
extern unsigned int g_nCap_GL_ARB_texture_compression;  // 0x0078e4b8
extern unsigned int g_nCap_GL_EXT_texture_compression_s3tc;  // 0x0078e4bc
extern unsigned int g_nCap_GL_ATI_pn_triangles;  // 0x0078e4c0
extern unsigned int g_nCap_GL_EXT_compiled_vertex_array;  // 0x0078e4c4
extern unsigned int g_nCap_WGL_ARB_render_texture;  // 0x0078e4c8
extern unsigned int g_nCap_GL_SGIS_generate_mipmap;  // 0x0078e4cc
extern unsigned int g_nCap_GL_ATI_fragment_shader;  // 0x0078e4d0
extern unsigned int g_nCap_GL_ARB_vertex_program;  // 0x0078e4d4
extern unsigned int g_nCap_GL_ARB_vertex_buffer_object;  // 0x0078e4d8
extern unsigned int g_nCap_GL_NV_register_combiners2;  // 0x0078e4dc
extern unsigned int g_nCap_GL_EXT_stencil_two_side;  // 0x0078e4e0
extern unsigned int g_nCap_WGL_NV_render_texture_rectangle;  // 0x0078e4e4
extern unsigned int g_nCap_WGL_ARB_buffer_region;  // 0x0078e4e8
extern int g_nExt4EC;  // 0x0078E4EC
extern unsigned int g_nMultisampleCapability;  // 0x0078e4ec
extern unsigned int g_nCap_WGL_EXT_swap_control;  // 0x0078e4f0
extern unsigned int g_nCap_GL_ARB_depth_texture;  // 0x0078e4f4
extern int g_bGL4FC;  // 0x0078E4FC
extern int g_nShaderLevel;  // 0x0078E500
extern float g_fRingAmplitude;  // 0x0078E560, descriptive
extern float g_fRingFrequency;  // 0x0078E564, descriptive
extern float g_fRingPhaseScale;  // 0x0078E568, descriptive
extern float g_fRingPhaseSpeed;  // 0x0078E56C, descriptive
extern float g_fAnimationTime;  // 0x0078E574
extern float g_fAurFrameDelta;  // 0x0078E574
extern float g_fFrameTime;  // 0x0078e574
extern int g_nShaderFrame;  // 0x0078E574
extern int g_bCompressQuaternions;  // 0x0078E588
extern float g_fKeyEpsilon;  // 0x0078E58C
extern int g_bCullKeys;  // 0x0078E590
extern float g_fGobFadeTime;  // 0x0078E5A0, descriptive
extern int g_bHitWalkmesh;  // 0x0078E5A8
extern int g_bUseVisibility;  // 0x0078E5C8
extern int g_n5D8;  // 0x0078E5D8
extern int g_n5DC;  // 0x0078E5DC
extern int g_nPartDrawDepth;  // 0x0078E5E0
extern int g_bARBVertexProgram;  // 0x0078E5EC
extern int g_bUseARBVertexProgram;  // 0x0078e5ec
extern int g_bUseHardwareSkin;  // 0x0078E5EC
extern int g_bVertexPrograms;  // 0x0078e5ec
extern int g_bPartTransform;  // 0x0078E5F0
extern int g_bTransformParts;  // 0x0078E5F0
extern int g_bBumpSpecularMagnitude;  // 0x0078E5F4, descriptive
extern int g_bUseLightMapTexture;  // 0x0078E5FC
extern int g_bEMBMLightmaps;  // 0x0078E608
extern int g_bShadowsOnly;  // 0x0078E60C
extern int g_bHighlightTriggers;  // 0x0078E614, descriptive
extern int g_bRenderFlat;  // 0x0078E614
extern int g_bRenderFlatKeepState;  // 0x0078E61C
extern int g_bRenderLightMapped;  // 0x0078E620
extern int g_bRenderLightMappedNoShaders;  // 0x0078E628
extern float g_fBumpSpecularScale;  // 0x0078E630, descriptive
extern int g_nBumpSpecularMode;  // 0x0078E634, descriptive
extern int g_bOnePassEnvMap;  // 0x0078E638
extern int g_bEnvMapTexturesOnly;  // 0x0078E63C
extern int g_bRenderEnvMapped;  // 0x0078E640
extern int g_bRenderEMLM;  // 0x0078E644
extern float g_fBumpLightScale;  // 0x0078E648, descriptive
extern int g_bEMBMBasePass;  // 0x0078E650
extern int g_bEMBMBumpPass;  // 0x0078E658
extern int g_bRenderEMBM;  // 0x0078E65C
extern int g_bEMBMEnabled;  // 0x0078E66C
extern int g_bDrawAllParts;  // 0x0078E68C
extern int g_bBeamStencil;  // 0x0078E69C
extern float g_fBeamAlpha;  // 0x0078E6A0
extern float g_fFogDensity;  // 0x0078E6A0
extern Vector g_aWalkMeshColors[32];  // 0x0078E6A8
extern int g_bDrawPreparedParts;  // 0x0078E844
extern float g_fGobFadeMin;  // 0x0078E858, descriptive
extern float g_fHitMinAlpha;  // 0x0078e858
extern int g_bRenderHiddenModels;  // 0x0078E85C, descriptive
extern int g_bRenderGobs2;  // 0x0078E860, descriptive
extern Vector g_vWhite;  // 0x0078E894
extern Vector g_vBeamColor;  // 0x0078E8A0
extern Vector g_vPartDraw;  // 0x0078E8AC
extern Vector g_vPartLightColor;  // 0x0078E8AC; descriptive
extern float g_fLastMatAlpha;  // 0x0078E8B8, descriptive
extern unsigned char g_bLastMatDynamic;  // 0x0078E8BC, descriptive
extern int g_bNVVertexProgram;  // 0x0078E8C0
extern int g_bUseVertexPrograms;  // 0x0078E8C0
extern char g_szVertexProgram0[];  // 0x0078f600
extern char g_szProgramSuffixNV[];  // 0x0078fd78
extern char g_szProgramSuffixATI[];  // 0x0078fe18
extern char g_szFlatVP[];  // 0x0078FEB8
extern char g_szVertexProgramD[];  // 0x0078feb8
extern char g_szEnvMappedVP[];  // 0x00790300
extern char g_szEMLMVP[];  // 0x007908B8
extern char g_szVertexProgram1[];  // 0x00791240
extern char g_szSkinVPHw[];  // 0x00792658
extern char g_szVertexProgramB[];  // 0x00792658
extern char g_szSkinCubeMapVPHw[];  // 0x007938C8
extern int g_bAlphaTest;  // 0x00798A8C
extern float g_fAlphaRef;  // 0x00798A90
extern int g_nNvStripNumSamples;  // 0x00798A94
extern int g_bEventTimeFilter;  // 0x00798B14
extern float g_fControlWeight;  // 0x00798b38
extern float g_fPositionScale;  // 0x00798B38
extern int g_bRenderSkins;  // 0x00798B68
extern int g_nRenderNormal;  // 0x00798B6C
extern int g_nRenderBillboardToLocalZ;  // 0x00798B70
extern int g_nRenderBillboardToWorldZ;  // 0x00798B74
extern int g_nRenderLinked;  // 0x00798B78
extern int g_nRenderAlignedToWorldZ;  // 0x00798B7C
extern short g_nLightningRenderFlags;  // 0x00798B80
extern short g_nEmitterRenderFlags;  // 0x00798B84
extern int g_nRenderAlignedToParticleDir;  // 0x00798B88
extern int g_nRenderMotionBlur;  // 0x00798B8C
extern AurColor g_cCurrentColor;  // 0x00798bb4
extern CAurColor g_cDefaultStringColor;  // 0x00798BB4 (placeholder name)
extern char g_szVertexProgramENV[];  // 0x00798cc8
extern char g_szVertexProgramA[];  // 0x007990f0
extern char g_szBumpSpecularVP[];  // 0x0079A218, descriptive
extern char g_szBumpDiffuseVP[];  // 0x0079A600, descriptive
extern char g_szNoVertexProgram[];  // 0x0079AB28
extern char g_szVertexProgramDNV[];  // 0x0079ab28
extern char g_szSkinVPSw[];  // 0x0079B5A8
extern char g_szVertexProgramBNV[];  // 0x0079b5a8
extern char g_szVertexProgramC[];  // 0x0079c310
extern char g_szSkinSphereMapVP[];  // 0x0079E958
extern char g_szSkinCubeMapVPSw[];  // 0x0079F128
extern int g_nMaxLights;  // 0x007A1934 (placeholder name)
extern int g_nMaxShadowLights;  // 0x007A1938 (placeholder name)
extern int g_bVertexProgramRefresh;  // 0x007A193C
extern float g_fLightFadeRate;  // 0x007A1940 (placeholder name)
extern float g_fShadowFadeRate;  // 0x007A1944 (placeholder name)
extern float g_fShadowLightFadeTime;  // 0x007A1948 (placeholder name)
extern Vector g_vHeadLightDiffuse;  // 0x007A194C (placeholder name)
extern Vector g_vHeadLightAmbient;  // 0x007A1958 (placeholder name)
extern int g_bQuaternionIdentityTest;  // 0x007A1980
extern Vector g_vXAxis;  // 0x007A1998
extern Vector g_vYAxis;  // 0x007A19A4
extern Vector g_vUp;  // 0x007A19B0
extern Vector g_vZAxis;  // 0x007A19B0
extern int g_nDamageMultiplier;  // 0x007A1B30 (descriptive name)
extern int g_b7A1B34;  // 0x007A1B34 (placeholder name)
extern int g_bCollisionDisabled;  // 0x007A1B34 (descriptive)
extern unsigned __int64 g_nNextEffectID;  // 0x007A1B40 (placeholder name)
extern CSWSAreaValue8 g_cAreaValue8;  // 0x007A1B48 (placeholder name)
extern int g_bSubAreaTriggerOverride;  // 0x007A1B54 (descriptive name)
extern const char* g_sServerStatusKey;  // 0x007A1F14 ("ServerStatus")
extern const char* g_sModuleLoadedKey;  // 0x007A1F1C
extern const char* g_sIdleKey;  // 0x007A1F20
extern const char* g_sModuleRunningKey;  // 0x007A1F24
extern const char* g_sPlayerListKey;  // 0x007A1F3C ("PlayerList")
extern int g_nReputationOverride;  // 0x007A2068
extern int g_nAppearanceCol_ABORTONPARRY;  // 0x7A20C0
extern int g_nAppearanceColUnknown20C4;  // 0x007A20C4
extern int g_nAppearanceCol_BACKUPHEAD;  // 0x7A20C4
extern int g_nAppearanceColCameraOffset;  // 0x007A20C8
extern int g_nAppearanceCol_CameraHeightOffset;  // 0x7A20C8
extern int g_nAppearanceColCrePerSpace;  // 0x007A20CC
extern int g_nAppearanceCol_CAMERASPACE;  // 0x7A20CC
extern int g_nAppearanceColPerSpace;  // 0x007A20D0
extern int g_nAppearanceCol_CREPERSPACE;  // 0x7A20D0
extern int g_nHeadBoneColumn;  // 0x007A20D0
extern int g_nAppearanceCol_DisableInjuredAnim;  // 0x007A20D4
extern int g_nAppearanceColUnknown20D8;  // 0x007A20D8
extern int g_nAppearanceCol_DRIVEACCl;  // 0x7A20D8
extern int g_nAppearanceColUnknown20DC;  // 0x007A20DC
extern int g_nAppearanceCol_DriveMaxSpeed;  // 0x7A20DC
extern int g_nAppearanceColUnknown20E0;  // 0x007A20E0
extern int g_nAppearanceCol_DriveAnimWalk;  // 0x7A20E0
extern int g_nAppearanceColUnknown20E4;  // 0x007A20E4
extern int g_nAppearanceCol_DriveAnimRun;  // 0x7A20E4
extern int g_nAppearanceColEnvMap;  // 0x007A20E8
extern int g_nAppearanceCol_ENVMAP;  // 0x7A20E8
extern int g_nAppearanceCol_FootstepType;  // 0x007A20EC
extern int g_nAppearanceColUnknown20F0;  // 0x007A20F0
extern int g_nAppearanceCol_GROUNDTILT;  // 0x7A20F0
extern int g_nAppearanceCol_HEAD_ARC_H;  // 0x7A20F4
extern int g_nHeadTurnHColumn;  // 0x007A20F4
extern int g_nAppearanceCol_HEAD_ARC_V;  // 0x7A20F8
extern int g_nHeadTurnVColumn;  // 0x007A20F8
extern int g_nAppearanceCol_HEADBONE;  // 0x007A20FC
extern int g_nAppearanceCol_HEADTRACK;  // 0x007A2100
extern int g_nAppearanceColHeight;  // 0x007A2104
extern int g_nAppearanceCol_HEIGHT;  // 0x7A2104
extern int g_nAppearanceColWalkDist;  // 0x007A2108
extern int g_nAppearanceCol_hitdist;  // 0x7A2108
extern int g_nAppearanceColPrefAttackDist;  // 0x007A210C (placeholder name)                          // 0x007A3A28
extern int g_nAppearanceCol_HITRADIUS;  // 0x7A210C
extern int g_nAppearanceColModelType;  // 0x007A2110
extern int g_nAppearanceCol_MODELTYPE;  // 0x007A2110
extern int g_nAppearanceColUnknown2114;  // 0x007A2114
extern int g_nAppearanceCol_NORMALHEAD;  // 0x7A2114
extern int g_nAppearanceColHitRadius;  // 0x007A2118 (cached 2DA column, placeholder names)
extern int g_nAppearanceCol_PERSPACE;  // 0x7A2118
extern int g_nHeadTurnRateColumn;  // 0x007A2118
extern int g_nAppearanceColRace;  // 0x007A211C
extern int g_nAppearanceCol_RACE;  // 0x7A211C
extern int g_nAppearanceColRunRate;  // 0x007A2120
extern int g_nAppearanceCol_RUNDIST;  // 0x007A2120
extern int g_nAppearanceCol_SIZECATEGORY;  // 0x007A2124
extern int g_nRacialTypeColumn;  // 0x007A2124
extern int g_nAppearanceCol_SoundAppType;  // 0x007A2128
extern int g_nAppearanceCol_WALKDIST;  // 0x007A212C
extern int g_nAppearanceSoundsCol_FallDirt;  // 0x7A2130
extern int g_nAppearanceSoundsCol_FallMetal;  // 0x7A2134
extern int g_nAppearanceSoundsCol_FallHard;  // 0x7A2138
extern int g_nAppearanceSoundsCol_FallWater;  // 0x7A213C
extern int g_nAppearanceSoundsCol_Weapon;  // 0x007A2140
extern int g_nAppearanceSoundsCol_ArmorType;  // 0x007A2144
extern int g_nGenderColGender;  // 0x007A2148
extern int g_nGenderCol_GENDER;  // 0x7A2148
extern int g_nAnimFireForgetColumn;  // 0x007A214C
extern int g_nAnimLoopingColumn;  // 0x007A2154
extern int g_nAnimDamageColumn;  // 0x007A2168
extern int g_nAnimDialogColumn;  // 0x007A2170
extern int g_nAnimNameColumn;  // 0x007A2174
extern int g_nAnimDodgeColumn;  // 0x007A2178
extern int g_nForceAdjustGoodCostColumn;  // 0x007A2184
extern int g_nForceAdjustEvilCostColumn;  // 0x007A2188
extern int g_nDialogAnimFireForgetColumn;  // 0x007A218C
extern int g_nDialogAnimDialogColumn;  // 0x007A2190
extern int g_nDialogAnimLoopingColumn;  // 0x007A2194
extern int g_nDialogAnimOverlayColumn;  // 0x007A2198
extern int g_nFootstepRollingColumn;  // 0x007A219C
extern int g_nFootstepPitchOffsetColumn;  // 0x007A21A0
extern int g_nWeaponSoundPitchColumn;  // 0x007A21A4 (descriptive)
extern int g_nWeaponSoundPitchOffsetColumn;  // 0x007A21A4
extern int g_nRegenHealthRegenColumn;  // 0x007A21A8
extern int g_nRegenForceRegenColumn;  // 0x007A21AC
extern int g_nSurfaceMaterialCol_Name;  // 0x007A21B0
extern int g_nVisualEffectCol_Imp_HeadCon_Node;  // 0x7A21B4
extern int g_nVisualEffectCol_Imp_Impact_Node;  // 0x7A21B8
extern int g_nVisualEffectCol_Imp_Root_M_Node;  // 0x7A21BC
extern int g_nVisualEffectCol_Imp_Root_S_Node;  // 0x7A21C0
extern int g_nVisualEffectCol_Imp_Root_L_Node;  // 0x7A21C4
extern int g_nVisualEffectCol_Imp_Root_H_Node;  // 0x7A21C8
extern int g_nVisualEffectCol_LowQuality;  // 0x7A21CC
extern int g_nVisualEffectCol_OrientWithGround;  // 0x7A21D0
extern int g_nVisualEffectCol_ShakeType;  // 0x007A21D4
extern int g_nVisualEffectCol_ShakeDuration;  // 0x007A21D8
extern int g_nVisualEffectCol_ShakeDelay;  // 0x007A21DC
extern int g_nVisualEffectCol_SoundImpact;  // 0x007A21E0
extern int g_nVisualEffectCol_SoundDuration;  // 0x007A21E4
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
extern int g_nCreatureSpeedRunColumn;  // 0x007A2244 (placeholder name)
extern int g_nCreatureSpeedCol_WALKRATE;  // 0x7A2248
extern int g_nCreatureSpeedWalkColumn;  // 0x007A2248 (placeholder name)
extern int g_nDoorTypesCol_SoundAppType;  // 0x007A224C
extern int g_nGenericDoorsCol_SoundAppType;  // 0x007A2250
extern int g_nPlaceablesCol_SoundAppType;  // 0x007A2254
extern int g_nPlaceablesCol_StrRef;  // 0x007A2258
extern int g_nPlaceablesCol_IgnoreStaticHitcheck;  // 0x7A225C
extern int g_nPlaceableSoundsCol_ArmorType;  // 0x007A2260
extern int g_nPlaceableSoundsCol_Locked;  // 0x7A2264
extern int g_nPlaceableSoundsCol_Opened;  // 0x7A2268
extern int g_nPlaceableSoundsCol_Closed;  // 0x7A226C
extern int g_nPlaceableSoundsCol_Destroyed;  // 0x7A2270
extern float g_fMouseDeltaScale;  // 0x007A2278 (100.0f)
extern unsigned long g_nRandMask;  // 0x007A2280
extern int g_bCameraResetPending;  // 0x007A2288
extern int g_bDrawGui;  // 0x007A2288
extern float g_fListenerOffsetBlend;  // 0x007A22A0 (descriptive)
extern float g_fAmbientBright;  // 0x007A22AC
extern float g_fAmbientDark;  // 0x007A22B0
extern float g_fFPS;  // 0x007A22B4
extern int g_bLockCamera;  // 0x007A22B8
extern const float g_fRunRatio;  // 0x007A22F4 (descriptive)
extern int g_nForcedGuiAlignment;  // 0x007A22F8 (descriptive, -1 = unset)
extern float g_fFarClipOverride;  // 0x007A2320
extern float g_fFarClipOverride2;  // 0x007A2324
extern float g_fFarClipOverride3;  // 0x007A2328
extern int g_bGuiFlag;  // 0x007A2370
extern Vector g_vUpgradeAvailableColor;  // 0x007A2378 (descriptive)
extern Vector g_vGuiDisabledBorderColor;  // 0x007A2384
extern Vector g_vKeyMapLockedColor;  // 0x007A2384
extern Vector g_vKeyMapNormalColor;  // 0x007A2390
extern Vector g_vDialogTopColor;  // 0x007A239C (descriptive)
extern Vector g_vFloatyTextColorWhite;  // 0x007A23A8 (descriptive)
extern Vector g_vDifficultyExitColor;  // 0x007A23B4
extern Vector g_vFeedbackColor;  // 0x007A23B4, descriptive
extern Vector g_vGuiColor23B4;  // 0x007A23B4 (descriptive)
extern Vector g_vPazaakButtonHilight;  // 0x007A23B4
extern Vector g_vAbilityColorGranted;  // 0x007A23C0
extern Vector g_vDefaultReplyColor;  // 0x007A23C0
extern Vector g_vPazaakFlashColor;  // 0x007A23CC
extern Vector g_vAbilityColorAvailable;  // 0x007A23D8
extern Vector g_vFeedbackAltColor;  // 0x007A23D8, descriptive
extern Vector g_vGuiColor23D8;  // 0x007A23D8 (descriptive)
extern Vector g_vAbilityColorLocked;  // 0x007A23E4
extern Vector g_vGuiColor23E4;  // 0x007A23E4 (descriptive)
extern Vector g_vFloatyTextColor23F0;  // 0x007A23F0 (descriptive)
extern Vector g_vGuiColor23F0;  // 0x007A23F0 (descriptive)
extern Vector g_vGuiColor23FC;  // 0x007A23FC (descriptive)
extern Vector g_vKeyMapChangedColor;  // 0x007A23FC
extern Vector g_vGuiDisabledTextColor;  // 0x007A2408
extern Vector g_vKeyMapLockedTextColor;  // 0x007A2408
extern Vector g_vPazaakButtonColor;  // 0x007A2408
extern int g_nFollowStateOverride;  // 0x007A2414, descriptive
extern float g_fCameraTiltUpLimit;  // 0x007A2420
extern float g_fCameraTiltDownLimit;  // 0x007A2424
extern float g_fCameraRotation;  // 0x007A2428
extern int g_nPendingCameraStyle;  // 0x007A242C
extern float g_fCameraFollowA;  // 0x007A2450
extern float g_fCameraFollowB;  // 0x007A2454
extern float g_fCameraFollowC;  // 0x007A2458
extern float g_fCameraFollowPitch;  // 0x007A245C
extern float g_fCameraTurnThreshold;  // 0x007A2460
extern float g_fCameraTurnScale;  // 0x007A2464
extern float g_fCameraShakePeriod;  // 0x007A246C
extern float g_fCameraShakeDegrees;  // 0x007A2470
extern unsigned long g_anJournalSortStrRefs[4];  // 0x007A2474 (descriptive)
extern int g_bCameraFollowFlag;  // 0x007A2484
extern float g_fMiniInertiaScale;  // 0x007A2488 (descriptive)
extern float g_fPlayerControlFastRate;  // 0x007A25F8 (15.0)
extern const char* g_szPazaakCardText[28];  // 0x007A2600
extern float g_fCreditsScrollRate;  // 0x007A2670 (305.0)
extern float g_fVisualEffectAnimSpeed40;  // 0x007A2674 (0.8)
extern int g_bCheatToggle7A267C;  // 0x007A267C (descriptive)
extern float g_fGroundTiltScale;  // 0x007A2680
extern CSWGuiClassChoice g_aClassChoices[6];  // 0x007A2684 (descriptive)
extern int g_nPazaakCardValues[28];  // 0x007A26B8
extern int g_nMaxTextureMemory;  // 0x007A399C
extern int g_bQuitRequested;  // 0x007a39b8
extern void* g_hWindow;  // 0x007a39d4
extern void* g_hMainWindow;  // 0x007a39d8
extern void* g_hRenderWindow;  // 0x007a39d8
extern unsigned char g_nAppMode;  // 0x007A39DC (1: client)
extern CExoBase* g_pExoBase;                        // 0x007A39E0
extern CExoInput* g_pExoInput;  // 0x007a39e4
extern CExoResMan* g_pExoResMan;  // 0x007A39E8
extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern CAuroraInterface* g_pAuroraInterface;  // 0x007A39F0 (descriptive)
extern CSWGuiManager* g_pGuiManager;  // 0x007A39F4
extern CToolTipGlobals* g_pToolTipGlobals;  // 0x007A39F4
extern CRender* g_pRender;  // 0x007A39F8
extern CRenderWindow* g_pRenderWindow;  // 0x007a39f8
extern CAppManager* g_pAppManager;  // 0x007A39FC
extern CAppManagerT* g_pAppManagerT;  // 0x007A39FC (g_pAppManager)
extern CVirtualMachine* g_pVirtualMachine;  // 0x007A3A00
extern CScriptCompiler* g_pScriptCompiler;  // 0x007A3A04 (placeholder name)
extern CTlkTable* g_pTlkTable;  // 0x007a3a08
extern unsigned __int64 g_nSessionStartTime;  // 0x007A3A20
extern CSWRules* g_pRules;  // 0x007A3A28
extern CDeletable* g_pUnknown7A3A28;  // 0x007A3A28
extern unsigned long g_bReinitializeVideoMode;  // 0x007A3A2C
extern int g_nActivateState;  // 0x007A3A2C
extern int g_bWindowed;  // 0x007A3A30
extern int g_bAppActive;  // 0x007A3A38
extern int g_bShowFrameInfo;  // 0x007a3a40
extern CFrameInfoLine g_cFrameInfoLine;  // 0x007a3a40
extern char g_szFrameInfo[];  // 0x007a3a4c
extern int g_bClientHeadLight;  // 0x007a3c50 (app-side flag; not the Aurora 0x0078e388 one)
extern int g_bMovieVideoMode;  // 0x007A3C54 (descriptive)
extern float g_fFrameLock;  // 0x007A3C64 (placeholder name)
extern int g_bMovieWaitForFrame;  // 0x007a3c9c (descriptive)
extern int g_bFrameInfoTextInput;  // 0x007a3cb4
extern int g_nMovieResolutionNesting;  // 0x007a3cb8
extern int g_bMovieResolutionChanged;  // 0x007a3cbc
extern int g_bMovieResolutionLocked;  // 0x007a3cc0
extern CExoString g_sBuildNumber;  // 0x007A3CC4
extern CExoString g_sPrivateBuild;  // 0x007A3CC4
extern CExoString g_sInternalName;  // 0x007A3CCC
extern CExoString g_sWindowSubTitle;  // 0x007a3ccc
extern CExoString g_sProductVersion;  // 0x007A3CD4
extern CExoString g_sLegalCopyright;  // 0x007A3CDC
extern CExoString g_sWindowTitle;  // 0x007a3cdc
extern int g_nCompletionOffset;  // 0x007a3cf0
extern CExoMoviePlayerInternal* g_pMoviePlayer;  // 0x007a3cf4
extern int g_bDrawSafeRegion;  // 0x007a3d4c
extern CExoString g_aResolutionSuffixes[];  // 0x007a3d50
extern CExoString g_sComputerBGPrefix;  // 0x007A3D70
extern unsigned long g_aCRCTable[256];  // 0x007a3d78
extern unsigned long g_aGffCRCTable[256];  // 0x007a4178
extern int g_aMemLightmaps[5];  // 0x007a469c
extern int g_nBindCountPeak;  // 0x007a46b0
extern int g_nAurTextureMaxTexID;  // 0x007a46bc
extern unsigned int g_nMaxGLName;  // 0x007a46bc
extern int g_aMemTextures[5];  // 0x007a46c0
extern unsigned char g_bBumpMaps;  // 0x007a46d4
extern unsigned char g_b7a46d5;  // 0x007a46d5
extern int g_nMemTotal;  // 0x007a46d8
extern int g_nMemLightmaps;  // 0x007a46dc
extern int g_nMemTextures;  // 0x007a46e0
extern int g_aMemByKind[4];  // 0x007a46e4
extern int g_nAurFrameCounter;  // 0x007A46F4
extern int g_nMemFrame;  // 0x007a46f4
extern int g_bAurTextureDisableDDS;  // 0x007a4700
extern unsigned char* g_pLayerTable;  // 0x007a4704
extern unsigned char* g_pLayerAlpha;  // 0x007a4708
extern int g_nBindLimit;  // 0x007a470c
extern int g_nBindCount;  // 0x007a4710
extern int g_bTexturesLocked;  // 0x007a4714
extern int g_nTextureCount;  // 0x007a4718
extern int g_nStat724;  // 0x007a4724
extern int g_nStat728;  // 0x007a4728
extern int g_nStat72c;  // 0x007a472c
extern int g_nStat730;  // 0x007a4730
extern int g_nHashedCount;  // 0x007a4748
extern float g_fTexMemOverTime;  // 0x007a474c
extern float g_fFrameTimePeak;  // 0x007a4750
extern int g_nStatPeak754;  // 0x007a4754
extern int g_nStatPeak758;  // 0x007a4758
extern int g_nStatPeak75c;  // 0x007a475c
extern int g_nStatPeak760;  // 0x007a4760
extern unsigned char g_aBumpTable[];  // 0x007a4764
extern CExoArrayList<CAurTextureBasic*> g_lTextures;  // 0x007a4770
extern SafePointer<CAurTexture> g_pNullTexture;  // 0x007a477c
extern CExoArrayList<CAurTextureBasic*> g_lOrderedA;  // 0x007a4780
extern CExoArrayList<int> g_lGLNameOrdering;  // 0x007a478c
extern CExoArrayList<CAurTextureBasic*> g_lHashed;  // 0x007a4798
extern CExoArrayList<CAurTextureBasic*> g_lOrderedB;  // 0x007a47a4
extern int g_nScreenShotCount;  // 0x007a47b0
extern int g_nSnapshotCount;  // 0x007a47b4
extern unsigned int g_nLastBoundGLName;  // 0x007a47b8
extern unsigned int g_nFontListBase;  // 0x007a47c0
extern unsigned int g_anBloomTexture[6];  // 0x007a47c4
extern void* g_hGLDC;  // 0x007a47e4
extern void* g_pShadowVertices;  // 0x007a47e8
extern void* g_ahBloomDC[6];  // 0x007a47ec
extern void* g_ahBloomContext[6];  // 0x007a4804
extern unsigned short g_aShadowVertexMarks[];  // 0x007a4820
extern void* g_ahBloomPbuffer[6];  // 0x007a6824
extern unsigned int g_aBoundTextures[];  // 0x007a683c
extern int g_aCurrentStageTID[6];  // 0x007a683c
extern void* g_hGLContext;  // 0x007a6854
extern float g_fMaxAnisotropy;  // 0x007a6858
extern int g_nAnisotropyRequested;  // 0x007a685c
extern char g_bFrameBufferActive;  // 0x007a6860
extern unsigned int g_nBloomRamp0;                   // 0x007a6864
extern unsigned int g_nBloomRamp1;                   // 0x007a6868
extern unsigned int g_nBloomRamp2;                   // 0x007a686c
extern unsigned int g_nColorRamp0;  // 0x007a6870
extern unsigned int g_nColorRamp1;  // 0x007a6874
extern unsigned int g_nColorRamp2;  // 0x007a6878
extern unsigned int g_nRootTexture;  // 0x007a687c
extern char g_bSpeedBlur;  // 0x007a6880
extern char g_bSaturation;  // 0x007a6881
extern char g_bScanNoise;  // 0x007a6882
extern char g_bFilmNoise;  // 0x007a6883
extern float g_fFogDistance;  // 0x007a6884
extern float g_fSaturation;  // 0x007a6884
extern int g_nAntiAliasingMode;  // 0x007a6888
extern const char* g_szWGLExtensions;  // 0x007a688c
extern unsigned int g_nPowerWidth;          // 0x007a6890
extern unsigned int g_nPowerHeight;         // 0x007a6894
extern int g_nActiveTextureStage;  // 0x007a6898
extern unsigned int g_nActiveTextureUnit;  // 0x007A6898
extern int g_nCurrentStage;  // 0x007a6898
extern int g_nGUIImageFlag;  // 0x007A689C
extern RequestPoolManager* g_pRequestPoolManager;  // 0x007a68a4
extern int g_nPoolBytes;  // 0x007a68a8
extern int g_nPoolPages;  // 0x007a68ac
extern int g_bOverbrighten;  // 0x007a68bc
extern unsigned int g_nBloomTexture;  // 0x007a68c0
extern void* g_hBloomPbuffer;  // 0x007a68c4
extern void* g_hBloomDC;  // 0x007a68c8
extern void* g_hBloomContext;  // 0x007a68cc
extern int g_nFilmNoiseTexture;  // 0x007a68d0
extern CAurTexture* g_pFilmNoiseTexture;  // 0x007a68d0
extern unsigned int g_nDistortionProgram;  // 0x007a68d4
extern CAurTexture* g_pDistortionTexture;   // 0x007a68d8
extern int g_bShadowTwoSidedStencil;  // 0x007a68dc
extern int g_bShadowNoClip;  // 0x007a68e0
extern unsigned int g_nSaturationProgram;  // 0x007a68e4
extern void* g_hSoftShadowPbuffer1;  // 0x007a68e8
extern void* g_hSoftShadowDC1;  // 0x007a68ec
extern void* g_hSoftShadowContext1;  // 0x007a68f0
extern unsigned int g_nSoftShadowTexture1;  // 0x007a68f4
extern unsigned int g_nSoftShadowTexture;  // 0x007a68f8
extern void* g_hSoftShadowPbuffer2;  // 0x007a68fc
extern void* g_hSoftShadowDC2;  // 0x007a6900
extern void* g_hSoftShadowContext2;  // 0x007a6904
extern unsigned int g_nSoftShadowTexture2;  // 0x007a6908
extern unsigned int g_nColorRampAlphaShaderATI;  // 0x007a690c
extern unsigned int g_nCopyShaderATI;  // 0x007a6910
extern unsigned int g_nConvolutionShaderATI;  // 0x007a6914
extern unsigned int g_nColorMatrixShaderATI;  // 0x007a6918
extern unsigned int g_nAccumBlurShaderATI;  // 0x007a691c
extern unsigned int g_nFilmNoiseShaderATI;  // 0x007a6920
extern unsigned int g_nDistortionShaderATI;  // 0x007a6924
extern unsigned int g_nBloomShaderATI;  // 0x007a6928
extern unsigned int g_nColorRampShaderATI;  // 0x007a692c
extern int g_nBloomSource;  // 0x007a6938
extern CExoArrayList<unsigned int> g_aRenderSquares;  // 0x007a693c
extern char g_bAccumFrameRendered;  // 0x007a698c
extern char g_bFrameRendered;  // 0x007a698d
extern int g_nModelMemory;  // 0x007a6990
extern MdlNodeTriMesh* g_pHitAABBMesh;  // 0x007a69b0
extern CHitInfo* g_pHitAABBInfo;  // 0x007a69b4
extern ShadowEdge g_aShadowEdges[];  // 0x007a69c0
extern int g_nTriFaceCount;  // 0x007a8a00
extern int g_bKeepModels;  // 0x007a8a04
extern int g_bModelNoDelete;  // 0x007a8a04
extern int g_bNoModelDelete;  // 0x007a8a04
extern int g_nMdlNodesAlive;  // 0x007a8a08
extern int g_nMdlNodesTotal;  // 0x007a8a0c
extern int g_nPartCount;  // 0x007a8a10
extern int g_nPartTriMeshCount;  // 0x007a8a14
extern int g_nGobCount;  // 0x007a8a18
extern int g_nGobsCreated;  // 0x007a8a1c
extern int g_nHitAABBDirection;  // 0x007a8a24
extern _iobuf* g_pTextureLog;  // 0x007a8a3c
extern CExoArrayList<Gob*> g_lGobs;  // 0x007a8a58
extern CExoArrayList<Model*> g_lModels;  // 0x007a8a64
extern CExoArrayList<int> g_lSkinScratch0;  // 0x007a8a70
extern CExoArrayList<int> g_lSkinScratch1;  // 0x007a8a7c
extern CExoArrayList<int> g_lSkinScratch2;  // 0x007a8a88
extern ConsoleFunc* g_apConsoleFuncs[0x4000];  // 0x007a8ab8
extern char g_szConsoleResult[0x500];  // 0x007b8ab8
extern int g_nConsoleFuncs;  // 0x007b8fb8
extern int g_nCommandHistoryIndex;  // 0x007b8fbc
extern char g_bConsoleEcho;  // 0x007b8fc0
extern unsigned char g_bModelGate;  // 0x007b8fc0
extern char** g_ppCommandHistory;  // 0x007b8fcc
extern int g_nCommandHistoryCount;  // 0x007b8fd0
extern AurResFileList g_AurResOpenFiles;  // 0x007B8FD8
extern CExoArrayList<AurRes*> g_lAurResStack;  // 0x007b8fd8
extern char g_szAurResDoFileMessage[];  // 0x007B8FE8
extern int g_nAurPrintfRow;  // 0x007b90e8
extern CAurFont* g_pFont;  // 0x007b90ec
extern int g_bAuroraInitialised;  // 0x007b90f0
extern int g_bFontInitialised;  // 0x007b90f0
extern int g_nAurPrintfColumn;  // 0x007b90f4
extern int g_bAurPrintf;  // 0x007b90f8
extern CExoArrayList<CAurPostString*> g_aPostStrings;  // 0x007b90fc
extern CExoArrayList<AurConsoleVar*> g_aConsoleVars;  // 0x007b9108
extern char g_szUntraceBuffer[];  // 0x007b9118
extern int g_nAuroraRefs;  // 0x007b9218
extern CAurInternalGL* g_pAurora;  // 0x007b921c
extern short g_nColorBits;  // 0x007b9220
extern short g_nColourDepth;  // 0x007b9220
extern short g_nScreenWidthUnknown;  // 0x007b9220
extern short g_nDepthBits;  // 0x007b9224
extern char g_bEditorProgramsBuilt;  // 0x007b9226
extern char g_szVersion[];  // 0x007b9228
extern float g_afGUIAlpha[];  // 0x007B9248
extern float g_fStartupDelay;  // 0x007b9270
extern float g_fAvailPhysKB;  // 0x007b9274
extern int g_nAnisotropy;  // 0x007b9278
extern unsigned char g_bBinaryModel;  // 0x007B927C
extern char g_bDisableVBO;  // 0x007b927d
extern unsigned char g_bNoVBO;  // 0x007B927D
extern __int64 g_nAnimEndTime;  // 0x007B9288
extern __int64 g_nAnimStartTime;  // 0x007B9298
extern int g_bRegisterCombiners;  // 0x007B92A0
extern int g_nChunkVisible;  // 0x007B92A0
extern Scene* g_pActiveScene;  // 0x007B92A0 (placeholder name)
extern Scene* g_pAnimatingScene;  // 0x007B92A0
extern CApp* g_pApp;  // 0x007B92A0
extern CombinerCaps* g_pCombinerCaps;  // 0x007B92A0
extern Scene* g_pScene;                         // 0x007B92A0
extern int g_bPauseFlag;  // 0x007B92AC
extern float g_fMeshBucketSeconds;  // 0x007B92B0, descriptive
extern float g_fBinBucketSeconds;  // 0x007B92B4, descriptive
extern float g_fTriggerSeconds;  // 0x007B92B8, descriptive
extern float g_fShadowSeconds;  // 0x007B92BC, descriptive
extern float g_fPathSeconds;  // 0x007B92C0, descriptive
extern float g_fDynamicSeconds;  // 0x007B92C4, descriptive
extern float g_fEmitterSeconds;  // 0x007B92C8, descriptive
extern float g_fPostSeconds;  // 0x007B92CC, descriptive
extern float g_fAnimSeconds;  // 0x007B92D0
extern float g_fRenderSeconds;  // 0x007B92D4, descriptive
extern int g_nFocusGobCounter;  // 0x007B92D8, descriptive
extern int g_bDynamicLights;  // 0x007B92DC
extern int g_n7B92E0;  // 0x007B92E0
extern int g_bStencilTriggers;  // 0x007B92E4, descriptive
extern float g_fSceneAlphaRef;  // 0x007B92E8 (bss), descriptive; not g_fAlphaRef 0x00798A90
extern int g_nBlurBucketCount;  // 0x007B92EC, descriptive
extern int g_bGobDepthToggle;  // 0x007B92F0, descriptive
extern unsigned char g_bAnimVisibleOnly;  // 0x007B92F4
extern char g_bInterleavedAnimations;  // 0x007b92f4
extern int g_bVisibilityTest;  // 0x007B92F8, descriptive
extern int g_bGobBucketFlag;  // 0x007B92FC, descriptive
extern int g_bFocusLights;  // 0x007B9300, descriptive
extern int g_bForceFog;  // 0x007B930C
extern int g_n7B9310;  // 0x007B9310
extern int g_bShowAllPersonalSpace;  // 0x007B9314
extern int g_bShowPaths;  // 0x007B9318
extern int g_nModelsRead;  // 0x007B932C
extern int g_nImageDecodeCalls;  // 0x007B9334
extern int g_nStatAPeak;  // 0x007B9338, descriptive
extern int g_nStatBPeak;  // 0x007B933C, descriptive
extern int g_nStatA;  // 0x007B9340, descriptive
extern int g_nStatB;  // 0x007B9344, descriptive
extern int g_nStatTotalPeak;  // 0x007B9348, descriptive
extern int g_bRenderingScreenShot;  // 0x007b9350
extern Scene* g_pRenderScene;  // 0x007B935C, descriptive
extern int g_bProjectWorldSpace;  // 0x007B9374, descriptive
extern CExoArrayList<Scene*> g_aScenes;  // 0x007b9398
extern CExoArrayList<Scene*> g_lScenes;  // 0x007B9398
extern CHitInfo g_DefaultHitInfo;  // 0x007b93a8
extern CHitInfo g_cHitInfo;  // 0x007B93A8
extern Vector g_vMiniHitPoint;  // 0x007B93A8
extern Vector g_vMiniHitPos;  // 0x007B93C4
extern Vector g_vMiniHitNormal;  // 0x007B93E8
extern char* g_szMiniHitPart;  // 0x007B93F4
extern short g_nGUILayer;  // 0x007B9460
extern short g_aGUIRects[][5];  // 0x007B9468 {x, y, w, h, bClear}
extern char g_aGUIVertexBuffer[];  // 0x007B94D0
extern int g_bAurNoGUIDraw;  // 0x007BB4D0
extern int g_nGUIBufferCount;  // 0x007BB4D4
extern int g_nGUIBatchTexture;  // 0x007BB4D8
extern CAurGUIImageData* g_pGUIBatchData;  // 0x007BB4DC
extern unsigned char g_bGUIBatchTriangles;  // 0x007BB4E0
extern EnvMapSource* g_pEnvMapSource;  // 0x007BB4E8
extern Camera* g_pRenderCamera;                   // 0x007BB4E8
extern int g_bImageDecodeDisabled;  // 0x007BB510
extern void* glGetVariantPointervEXT;  // 0x007bb524
extern void* glGenProgramsARB;  // 0x007bb528
extern void* glVertexAttrib4NsvARB;  // 0x007bb52c
extern void* glBindVertexShaderEXT;  // 0x007bb534
extern void* wglSwapIntervalEXT;  // 0x007bb538
extern void* glGetCombinerInputParameterfvNV;  // 0x007bb53c
extern void* glVertexAttrib4ubvARB;  // 0x007bb540
extern void* glVertexAttrib4svARB;  // 0x007bb548
extern void* glProgramEnvParameter4dARB;  // 0x007bb54c
extern void* glGetVariantIntegervEXT;  // 0x007bb558
extern void* glGetProgramParameterfvNV;  // 0x007bb55c
extern void* glSetFenceNV;  // 0x007bb560
extern void* glVertexAttrib4ivARB;  // 0x007bb564
extern void* glMultiTexCoord3fARB;  // 0x007bb568
extern void* glGetProgramEnvParameterfvARB;  // 0x007bb56c
extern void* wglGetSwapIntervalEXT;  // 0x007bb570
extern void* glVertexAttrib2fvARB;  // 0x007bb574
extern void* glGenFencesNV;  // 0x007bb57c
extern void* glProgramStringARB;  // 0x007bb580
extern void* glFinishFenceNV;  // 0x007bb584
extern void* glProgramLocalParameter4dvARB;  // 0x007bb588
extern void* glVertexAttrib3sARB;  // 0x007bb58c
extern void* glVariantsvEXT;  // 0x007bb590
extern void* wglSaveBufferRegionARB;  // 0x007bb594
extern void* glCombinerOutputNV;  // 0x007bb59c
extern void* glVertexAttrib1dARB;  // 0x007bb5a8
extern void* glIsProgramNV;  // 0x007bb5ac
extern void* glGetInvariantFloatvEXT;  // 0x007bb5b0
extern void* glIsProgramARB;  // 0x007bb5b8
extern void* glUnlockArraysEXT;  // 0x007bb5bc
extern void* glGetLocalConstantFloatvEXT;  // 0x007bb5c0
extern void* glVertexAttrib4dvARB;  // 0x007bb5c4
extern void* glVariantusEXT;  // 0x007bb5c8
extern void* glVertexAttrib3fARB;  // 0x007bb5cc
extern void* glVariantfvEXT;  // 0x007bb5d0
extern void* glVertexAttrib3fvARB;  // 0x007bb5d4
extern void* glBindTextureUnitParameterEXT;  // 0x007bb5dc
extern void* wglFreeMemoryNV;  // 0x007bb5e0
extern void* glLoadProgramNV;  // 0x007bb5e4
extern void* glTestFenceNV;  // 0x007bb5ec
extern void* glGetCompressedTexImageARB;  // 0x007bb5f0
extern void* glGetInvariantIntegervEXT;  // 0x007bb5f4
extern void* glBindMaterialParameterEXT;  // 0x007bb5f8
extern void* glVertexAttrib4fvNV;  // 0x007bb5fc
extern void* glVertexAttrib4sARB;  // 0x007bb600
extern void* glMultiTexCoord2iARB;  // 0x007bb604
extern void* glGetVariantFloatvEXT;  // 0x007bb608
extern void* glVertexAttrib2dARB;  // 0x007bb60c
extern void* glSwizzleEXT;  // 0x007bb610
extern void* glPNTrianglesfATI;  // 0x007bb614
extern void* glGetProgramLocalParameterdvARB;  // 0x007bb618
extern void* glSetInvariantEXT;  // 0x007bb61c
extern void* glShaderOp1EXT;  // 0x007bb628
extern void* glGetVariantBooleanvEXT;  // 0x007bb62c
extern void* glWriteMaskEXT;  // 0x007bb630
extern void* glDeleteFencesNV;  // 0x007bb638
extern void* wglGetPixelFormatAttribivARB;  // 0x007bb63c
extern void* glGetFinalCombinerInputParameterfvNV;  // 0x007bb640
extern void* glPNTrianglesiATI;  // 0x007bb644
extern void* glFogCoordPointerEXT;  // 0x007bb648
extern void* glBindTexGenParameterEXT;  // 0x007bb64c
extern void* glVertexAttrib4NbvARB;  // 0x007bb650
extern void* glFinalCombinerInputNV;  // 0x007bb654
extern void* wglAllocateMemoryNV;  // 0x007bb658
extern void* glVertexAttrib1sARB;  // 0x007bb65c
extern void* glIsFenceNV;  // 0x007bb660
extern void* glCombinerParameteriNV;  // 0x007bb664
extern void* glVertexAttrib4fARB;  // 0x007bb668
extern void* glVertexAttrib4usvARB;  // 0x007bb66c
extern void* glGetProgramivARB;  // 0x007bb670
extern void* glVariantubEXT;  // 0x007bb674
extern void* glCombinerParameterfNV;  // 0x007bb678
extern void* glExtractComponentEXT;  // 0x007bb67c
extern void* glGetVertexAttribivARB;  // 0x007bb684
extern void* wglCreateBufferRegionARB;  // 0x007bb688
extern void* glVertexAttrib4NubvARB;  // 0x007bb690
extern void* glDeleteVertexShadersEXT;  // 0x007bb694
extern void* glShaderOp2EXT;  // 0x007bb698
extern void* glVertexAttrib4NusvARB;  // 0x007bb69c
extern void* glVariantbvEXT;  // 0x007bb6a0
extern void* glGetProgramEnvParameterdvARB;  // 0x007bb6a4
extern void* glGetCombinerOutputParameterfvNV;  // 0x007bb6a8
extern void* glMultiTexCoord3fvARB;  // 0x007bb6ac
extern void* glLockArraysEXT;  // 0x007bb6b4
extern void* glProgramEnvParameter4fvARB;  // 0x007bb6bc
extern void* glVariantuiEXT;  // 0x007bb6c0
extern void* glBindParameterEXT;  // 0x007bb6c4
extern void* glVertexAttrib2dvARB;  // 0x007bb6c8
extern void* glGetFinalCombinerInputParameterivNV;  // 0x007bb6d8
extern void* glGenSymbolsEXT;  // 0x007bb6dc
extern void* glGetProgramLocalParameterfvARB;  // 0x007bb6e4
extern void* glDisableVariantClientStateEXT;  // 0x007bb6ec
extern void* glVertexAttrib2sARB;  // 0x007bb6f0
extern void* glDeleteProgramsNV;  // 0x007bb6f4
extern void* glBindLightParameterEXT;  // 0x007bb6fc
extern void* glGetLocalConstantBooleanvEXT;  // 0x007bb700
extern void* glGetInvariantBooleanvEXT;  // 0x007bb704
extern void* glVertexAttrib4uivARB;  // 0x007bb708
extern void* glVariantPointerEXT;  // 0x007bb70c
extern void* glSecondaryColor3fvEXT;  // 0x007bb714
extern void* glProgramEnvParameter4dvARB;  // 0x007bb718
extern void* glGetVertexAttribfvARB;  // 0x007bb71c
extern void* glInsertComponentEXT;  // 0x007bb720
extern void* glIsBufferARB;  // 0x007bb728
extern void* glVertexAttrib1fARB;  // 0x007bb72c
extern void* glVertexAttrib1fvNV;  // 0x007bb734
extern void* glCombinerInputNV;  // 0x007bb738
extern void* glProgramLocalParameter4fARB;  // 0x007bb73c
extern void* glVertexAttrib1fvARB;  // 0x007bb740
extern void* glTrackMatrixNV;  // 0x007bb744
extern void* glVertexAttrib4NuivARB;  // 0x007bb748
extern void* glGetLocalConstantIntegervEXT;  // 0x007bb74c
extern void* glVertexAttrib2svARB;  // 0x007bb758
extern void* glVertexAttrib4NubARB;  // 0x007bb760
extern void* glGetVertexAttribPointervARB;  // 0x007bb764
extern void* glVertexAttrib3fvNV;  // 0x007bb768
extern void* glProgramLocalParameter4dARB;  // 0x007bb76c
extern void* glDisableVertexAttribArrayARB;  // 0x007bb770
extern void* glCompressedTexImage2DARB;  // 0x007bb774
extern void* glGetBufferPointervARB;  // 0x007bb778
extern void* glGetVertexAttribdvARB;  // 0x007bb77c
extern void* glVertexAttrib3dvARB;  // 0x007bb780
extern void* glProgramLocalParameter4fvARB;  // 0x007bb784
extern void* glBindProgramARB;  // 0x007bb788
extern void* wglDeleteBufferRegionARB;  // 0x007bb78c
extern void* glShaderOp3EXT;  // 0x007bb790
extern void* glVertexAttrib4fvARB;  // 0x007bb794
extern void* glVertexAttrib2fARB;  // 0x007bb798
extern void* glEnableVertexAttribArrayARB;  // 0x007bb79c
extern void* wglQueryPbufferARB;  // 0x007bb7a0
extern void* glVertexAttrib3dARB;  // 0x007bb7a4
extern void* glProgramParameter4fNV;  // 0x007bb7ac
extern void* glGenProgramsNV;  // 0x007bb7b0
extern void* glVertexArrayRangeNV;  // 0x007bb7c0
extern void* glGenVertexShadersEXT;  // 0x007bb7c4
extern void* glBeginVertexShaderEXT;  // 0x007bb7c8
extern void* glVertexAttrib3svARB;  // 0x007bb7cc
extern void* glVertexAttrib1svARB;  // 0x007bb7d0
extern void* glVertexAttrib4dARB;  // 0x007bb7d4
extern void* glVertexAttribPointerARB;  // 0x007bb7d8
extern void* glVariantdvEXT;  // 0x007bb7dc
extern void* glFogCoordfEXT;  // 0x007bb7e0
extern void* glSetLocalConstantEXT;  // 0x007bb7e4
extern void* glMultiTexCoord4fvARB;  // 0x007bb7e8
extern void* glExecuteProgramNV;  // 0x007bb7f0
extern void* glMultiTexCoord2fvARB;  // 0x007bb7f8
extern void* glGetFenceivNV;  // 0x007bb800
extern void* glGetProgramStringARB;  // 0x007bb808
extern void* glGetCombinerInputParameterivNV;  // 0x007bb810
extern void* glVariantivEXT;  // 0x007bb814
extern void* glCombinerParameterivNV;  // 0x007bb81c
extern void* glVertexAttrib2fvNV;  // 0x007bb820
extern void* wglRestoreBufferRegionARB;  // 0x007bb824
extern void* glGetBufferSubDataARB;  // 0x007bb828
extern void* glVertexAttrib4bvARB;  // 0x007bb82c
extern void* glEndVertexShaderEXT;  // 0x007bb830
extern void* glBindProgramNV;  // 0x007bb834
extern void* glEnableVariantClientStateEXT;  // 0x007bb838
extern void* glIsVariantEnabledEXT;  // 0x007bb83c
extern void* glVertexAttrib1dvARB;  // 0x007bb844
extern void* glVertexAttrib4NivARB;  // 0x007bb848
extern void* glGetCombinerOutputParameterivNV;  // 0x007bb84c
extern void* glProgramEnvParameter4fARB;  // 0x007bb850
extern void* glVertexAttribPointerNV;  // 0x007bb854
extern unsigned int g_nCapabilities;  // 0x007bb85c
extern unsigned int g_nGLCaps;  // 0x007bb85c
extern int g_nGLExtensions;  // 0x007BB85C
extern int g_bATISoftShadowsUnlocked;  // 0x007bb860
extern int g_bGLExtension860;  // 0x007BB860
extern unsigned int g_nLookUpTexture;  // 0x007bb868
extern unsigned char g_aDistortImage[];  // 0x007BB870
extern MaxTree* g_pCurrentTree;  // 0x007FBAE4
extern Model* g_pLoadingModel;   // 0x007FBAE4
extern int g_nControllers;  // 0x007FBAE8
extern int g_bNoGobCulling;  // 0x007FBF20
extern int g_nHitMask;  // 0x007FBF48
extern CHitInfo* g_pBSPHitInfo;  // 0x007FBF48, descriptive
extern int g_bHitStatic;  // 0x007FBF4C
extern int g_bHitGround;  // 0x007FBF50
extern int g_bHitGroundPlane;  // 0x007FBF54
extern int g_nHitTests;  // 0x007FBF58
extern int g_bShowWalkMesh;  // 0x007FBF5C, descriptive
extern int g_nLastObjectRoom;  // 0x007FBF68
extern CExoArrayList<void*> g_lSceneLights;  // 0x007FBF78
extern CExoArrayList<CapPart*> g_lStaticMeshes;  // 0x007FBF84
extern CExoArrayList<Bin*> g_lActiveBins;       // 0x007FBF9C
extern CExoArrayList<Part*> g_lAllMeshParts;  // 0x007FBFB4
extern CExoArrayList<void*> g_lSceneObjects;  // 0x007FBFCC
extern CExoArrayList<BSPNode*> g_lLightTiles;  // 0x007FBFE4
extern CExoArrayList<Bin*> g_lPrevActiveBins;  // 0x007FBFFC
extern CExoArrayList<Part*> g_aBucketsA[];  // 0x007FC008
extern CExoArrayList<CapPart*> g_aCapBuckets[];  // 0x007FC008
extern CExoArrayList<void*> g_lSceneEmitters;  // 0x0080AA74
extern CExoArrayList<Part*> g_aBucketsC[];  // 0x0080AA80
extern CExoArrayList<Part*> g_aFadeBuckets[];  // 0x0080AA80
extern CExoArrayList<Part*> g_aMeshBuckets[];  // 0x008194E0
extern SceneTileSlot g_aSceneTileSlots[5000];  // 0x008194E4
extern CExoArrayList<Part*> g_lTransparentParts;  // 0x00827F40
extern int g_nPartPlane;  // 0x00827F4C
extern int g_nLightPlane;  // 0x00827F50
extern int g_nEmitterPlane;  // 0x00827F54
extern int g_bFSEnvMapOn;  // 0x00827FA0, descriptive
extern int g_n827fa0;  // 0x00827fa0
extern int g_bFSTexture1On;  // 0x00827FA4, descriptive
extern int g_n827fa4;  // 0x00827fa4
extern int g_nPartsProjected;  // 0x00827FA8, descriptive
extern int g_nTransformCount;  // 0x00827FA8
extern int g_nGobsRendered;  // 0x00827FAC, descriptive
extern int g_nFlatTriangles;  // 0x00827FB0
extern int g_nShadowTriangles;  // 0x00827fb0
extern int g_nFlatParts;  // 0x00827FB4
extern int g_bSkipA4;  // 0x00827FCC
extern int g_bEnvMapBlendAdd;  // 0x00827FD4
extern int g_bEMBMAdditive;  // 0x00827FE8
extern CAurProgram* g_pProgram827fec;  // 0x00827fec
extern VertexProgram* g_pProgramfec;  // 0x00827fec
extern VertexProgram* g_pVPLightmapped;  // 0x00827FEC
extern CAurProgram* g_pProgram827ff0;  // 0x00827ff0
extern VertexProgram* g_pProgramff0;  // 0x00827ff0
extern VertexProgram* g_pVPFog;  // 0x00827FF0
extern CAurProgram* g_pProgram827ff4;  // 0x00827ff4
extern VertexProgram* g_pProgramff4;  // 0x00827ff4
extern VertexProgram* g_pVPBase;  // 0x00827FF4
extern CAurProgram* g_pProgram827ff8;  // 0x00827ff8
extern VertexProgram* g_pProgramff8;  // 0x00827ff8
extern VertexProgram* g_pVPBump;  // 0x00827FF8
extern CAurProgram* g_pProgram827ffc;  // 0x00827ffc
extern VertexProgram* g_pProgramffc;  // 0x00827ffc
extern VertexProgram* g_pVPBumpLightmapped;  // 0x00827FFC
extern CAurProgram* g_pProgram828014;  // 0x00828014
extern VertexProgram* g_pVPSelfIllum;  // 0x00828014
extern int g_bDrawPartAxes;  // 0x00828024
extern int g_bMatDisabled;  // 0x00828040, descriptive
extern int g_bMatForce;  // 0x00828048, descriptive
extern int g_nShadowMemoryPeak;  // 0x00828050
extern int g_bDrawGobBounds;  // 0x0082805C, descriptive
extern int g_nDrawGobSpheres;  // 0x00828060, descriptive
extern int g_bModelVariants;  // 0x00828068
extern int g_bGobFading;  // 0x0082806C, descriptive
extern Vector g_vEnvMapPosition;  // 0x00828074
extern float g_aTransformMatrix[16];  // 0x008280A0
extern int g_nNodeVertexBytes;  // 0x00830238
extern char* g_szProgramText0;  // 0x0083028c
extern char* g_szProgramText83028c;  // 0x0083028c
extern char* g_szProgramText1;  // 0x00830290
extern char* g_szProgramText830290;  // 0x00830290
extern CExoArrayList<Material*> g_lMaterials;  // 0x00830298
extern bool g_bNvStripBadEdge;  // 0x008302A4
extern CExoArrayList<NvFaceInfo*> g_lNvStripFaces;  // 0x008302A8
extern unsigned char g_bNoKeyReports;  // 0x008302C8
extern int g_bReportKeys;  // 0x008302CC
extern float g_fAnimationLength;  // 0x008302D0
extern VertexProgram* g_pProgram37c;  // 0x0083037c
extern CAurProgram* g_pProgram83037c;  // 0x0083037c
extern VertexProgram* g_pProgram380;  // 0x00830380
extern CAurProgram* g_pProgram830380;  // 0x00830380
extern VertexProgram* g_pProgram384;  // 0x00830384
extern CAurProgram* g_pProgram830384;  // 0x00830384
extern CAurProgram* g_pProgram830388;  // 0x00830388
extern CAurProgram* g_pProgram83038c;  // 0x0083038c
extern int g_nParticleCount;  // 0x0083046C
extern int g_nEmitterCount;  // 0x00830470
extern float g_fLastEmitterF58;  // 0x00830494
extern int g_bDrawEmitters;  // 0x00830498
extern float g_fTriggerAlpha;  // 0x008304B8
extern CExoArrayList<CAurFontRender*> g_aFonts;  // 0x0083052c
extern unsigned int g_vertex_program_current_generation;  // 0x00830540
extern int g_nProgramParams544;  // 0x00830544
extern int g_nProgramParams548;  // 0x00830548
extern int g_nProgramParams54c;  // 0x0083054c
extern int g_nBumpSpecularVPContext;  // 0x00830554, descriptive
extern int g_nBumpDiffuseVPContext;  // 0x00830558, descriptive
extern int g_nProgramParams560;  // 0x00830560
extern int g_nVertexProgramContext;  // 0x00830560
extern int g_nProgramParams564;  // 0x00830564
extern int g_nSkinVPType;  // 0x00830578
extern int g_nProgramParams580;  // 0x00830580
extern int g_nSkinSphereMapVPType;  // 0x0083058C
extern int g_nSkinCubeMapVPType;  // 0x00830590
extern unsigned char g_nCurrentVertexProgramLighting;  // 0x008305D8 (placeholder name)
extern char* g_pCurrentVertexProgramSource;  // 0x008305DC (placeholder name)
extern int g_bBlackFog;  // 0x008305e0
extern int g_bDisableTexCoordArray;  // 0x008305E0
extern int g_nEnabledLights;  // 0x008305E4 (placeholder name)
extern int g_nActiveLights;  // 0x008305E8 (placeholder name)
extern int g_nDynamicActiveLights;  // 0x008305EC (placeholder name)
extern int g_bNegativeLight;  // 0x008305F0 (placeholder name)
extern int g_bLightsFadeLocked;  // 0x008305F4 (placeholder name)
extern char g_szLightText2[0x400];  // 0x00830608 (placeholder name)
extern char g_szLightText1[0x400];  // 0x00830A08 (placeholder name)
extern char g_szLightText0[0x400];  // 0x00830E08 (placeholder name)
extern CExoArrayList<CAurGrassRecord> g_aAurGrassInfo;  // 0x00831E44 (placeholder name)
extern int g_nGrassMaterials;  // 0x00831E48, descriptive
extern int g_bLogEvents;  // 0x00831FD8 (unnamed)
extern CServerExoAppInternal* g_pServerExoAppInternal;  // 0x00831FDC
extern int g_bDebugDirectLine;  // 0x008327C4 (placeholder name)
extern int g_bDebugDirectLineHeights;  // 0x008327C8 (placeholder name)
extern int g_nDamageIndex;  // 0x008327E0
extern int g_bDebugEventStrings;  // 0x008327E4 (placeholder name)
extern int g_bRenderQA;  // 0x008327EC
extern int g_bDebugActions;  // 0x0083281C (name guessed)
extern int g_bLogActions;  // 0x0083281C (placeholder name)
extern int g_nScriptVar;  // 0x00832828 (.data)
extern int g_bExoDebugDisabled;  // 0x0083282C (.data)
extern int g_bShipBuild;  // 0x00832834 (name guessed)
extern int g_bDebugOverlap;  // 0x00832838 (placeholder name)
extern int g_bWalkDebug;  // 0x00832838
extern int g_bShowAllTriggers;  // 0x0083285C
extern int g_nDebugAttackRoll;  // 0x00832874
extern int g_bDebugNoDamage;  // 0x00832878 (placeholder name)
extern int g_bPathfindDebug;  // 0x00832880 (placeholder name)
extern CNetLayer* g_pNetLayerList;  // 0x0083288C
extern int g_bSoundPaused;  // 0x00832890
extern int g_bSoundDisabled;  // 0x00832894
extern int g_nRawInputUnknown;  // 0x008328A0
extern char g_sExoEmptyString[];  // 0x008328A4 "" (.data)
extern char* g_pExoStringFormatBuffer;  // 0x008328A8
extern unsigned long g_nExoStringFormatBufferSize;  // 0x008328AC
extern CExoFileThread g_cExoFileThread;  // 0x008328B0
extern int g_bAsyncReadComplete;  // 0x008328BC
extern int g_bModuleLoadEnabled;  // 0x008328FC
extern int g_nModuleFrameLock;  // 0x00832904 (placeholder name)
extern unsigned __int64 g_nClientAIEndTime;  // 0x00832C58
extern unsigned __int64 g_nClientAIStartTime;  // 0x00832C60
extern float g_fClientModuleUpdateTime;  // 0x00832C68
extern float g_fClientAIUpdateTime;  // 0x00832C6C
extern float g_fLightningAmbient;  // 0x00832C70
extern int g_nMeshFaces;  // 0x00832c80
extern int g_nMeshVertBytes;  // 0x00832c88
extern int g_nDebugAttackAnimation;  // 0x008338BC
extern CExoString g_sLookAtBoneOverride;  // 0x008338D0 (descriptive)
extern CExoString g_sDefaultTexturePack;  // 0x008338D8
extern int g_bMessageCheatShown;  // 0x008338E8, descriptive
extern CDeletable* g_pUnknown8338EC;  // 0x008338EC
extern int g_bSwapMouseButtons;  // 0x008338F0
extern int g_nDriveForward;  // 0x008338F0 (placeholder name)
extern int g_bMouseButtonDown;  // 0x008338F4
extern int g_nDriveTurn;  // 0x008338F4 (placeholder name)
extern int g_nMouseDownState;  // 0x008338F4
extern int g_bClearTargetOnAction;  // 0x008338F8
extern int g_bUnknown8338F8;  // 0x008338F8
extern int g_nDriveStrafe;  // 0x008338F8 (placeholder name)
extern CDeletable* g_pUnknown8338FC;  // 0x008338FC
extern int g_bLetterbox;  // 0x0083390C
extern Vector g_vBlack;  // 0x00833910
extern int g_bUpdateCreatureOnMove;  // 0x0083395C
extern int g_nJournalSortOrder;  // 0x00833A90
extern int g_bJournalUpdated;  // 0x00833A94
extern int g_bItemGroundAlign;  // 0x00833A98 (descriptive)
extern int g_bLogConversations;  // 0x00833A9C (placeholder name)
extern int g_bPlayerControlFastMove;  // 0x00833BAC
extern int g_bPazaakShowOpponentHand;  // 0x00833BB0
extern CSWGuiMainInterface* g_pMainInterface;  // 0x00833BB4 (descriptive)
extern int g_nCreditsSavedScreenHeight;  // 0x00833BB8
extern int g_nCreditsSavedGuiHeight;  // 0x00833BBC
extern int g_nCreditsSavedGuiWidth;  // 0x00833BC0
extern int g_nCreditsSavedScreenWidth;  // 0x00833BC4
extern CExoLinkedListInternal* g_lCachedVFX;  // 0x00833BC8
extern short g_nSpeedBlurCount;  // 0x00833BCC
extern CExoString g_asTexturePacks[4];  // 0x00834098
extern const int ABILITY_CONSTITUTION;
extern const int ABILITY_DEXTERITY;
extern const int ABILITY_INTELLIGENCE;
extern const int ABILITY_WISDOM;
extern const float AI_GIVE_ITEM_RUN_DISTANCE_SQ;
extern const int EFFECT_TRUETYPE_DAMAGE_IMMUNITY;
extern const int EFFECT_TRUETYPE_IMMUNITY;
extern unsigned long EVENT_PLACEABLE_CLOSED;
extern const unsigned long EVENT_PLAY_ANIMATION;
extern const unsigned long EVENT_REMOVE_FROM_AREA;
extern const int GUI_PANEL_POPUP_A;
extern const int GUI_PANEL_POPUP_B;
extern const int IMMUNITY_TYPE_ABILITY_DECREASE;
extern const int IMMUNITY_TYPE_BLINDNESS;
extern const int IMMUNITY_TYPE_CHARM;
extern const int IMMUNITY_TYPE_CONFUSED;
extern const int IMMUNITY_TYPE_COUNT;
extern const int IMMUNITY_TYPE_CURSED;
extern const int IMMUNITY_TYPE_DAZED;
extern const int IMMUNITY_TYPE_DEAFNESS;
extern const int IMMUNITY_TYPE_DISEASE;
extern const int IMMUNITY_TYPE_DOMINATE;
extern const int IMMUNITY_TYPE_ENTANGLE;
extern const int IMMUNITY_TYPE_FEAR;
extern const int IMMUNITY_TYPE_MIND_SPELLS;
extern const int IMMUNITY_TYPE_PARALYSIS;
extern const int IMMUNITY_TYPE_POISON;
extern const int IMMUNITY_TYPE_SILENCE;
extern const int IMMUNITY_TYPE_SLEEP;
extern const int IMMUNITY_TYPE_SLOW;
extern const int IMMUNITY_TYPE_STUN;
extern const int IMMUNITY_TYPE_TRAP;
extern const unsigned long PLAYER_ID_ALL_PLAYERS;
extern const int TLK_CUSTOM_TOKEN_FIRST;
extern unsigned long g_action_complete_00745dc0;
extern unsigned long g_action_failed_00745dc4;
extern unsigned long g_action_in_progress_00745dbc;
extern unsigned int g_aur_grass_info_count;
extern CAurGrassInfo* g_aur_grass_info_data;
extern int g_bShowPlaceableBoxes;
extern float g_fSafeProjectileRange;
extern float g_fTriggerRadius;
extern CExoArrayList<Vector> g_lTexIndices1;
extern CExoArrayList<Vector> g_lTexIndices2;
extern CExoArrayList<Vector> g_lTexIndices3;
extern CEncounterPointsTable g_lstEncounterCRPoints;
extern unsigned int g_nATIShader_00830244;
extern unsigned int g_nATIShader_00830248;
extern unsigned int g_nATIShader_0083024C;
extern unsigned int g_nATIShader_00830250;
extern unsigned int g_nATIShader_00830254;
extern unsigned int g_nATIShader_00830258;
extern unsigned int g_nATIShader_0083025C;
extern unsigned int g_nATIShader_00830260;
extern unsigned int g_nATIShader_00830264;
extern int g_nAnimAttackColumn;
extern int g_nAnimHideEquippedItemsColumn;
extern int g_nAnimOverlayColumn;
extern int g_nAnimParryColumn;
extern int g_nAnimPauseColumn;
extern int g_nAnimRunningColumn;
extern int g_nAnimStationaryColumn;
extern int g_nAnimWalkingColumn;
extern int g_nCombatAnimationTime;
extern int g_nDoorDebugMode;
extern int g_nDoorStateClosed;
extern int g_nDoorStateFirst;
extern int g_nDoorStateLast;
extern int g_nDoorStateOpen1;
extern int g_nDoorStateOpen2;
extern const unsigned short g_nDurationInstant;
extern unsigned short g_nEffectSubTypeMagical;
extern unsigned short g_nEffectTypeVisual;
extern int g_nEffectVisualParam;
extern unsigned char g_nFeatMines;
extern unsigned char g_nObjectTypeDoor;  // 00746639
extern unsigned char g_nObjectTypePlaceable;  // 00746638
extern int g_programs_arb_0078e5ec;
extern CExoString g_sExoFileRetryLog;
extern int g_vertex_program_capacity_008305d4;
extern VertexProgramData** g_vertex_program_data_008305cc;
extern int g_vertex_program_size_008305d0;
extern VertexProgramArray g_vertex_programs_008305cc;

#endif
