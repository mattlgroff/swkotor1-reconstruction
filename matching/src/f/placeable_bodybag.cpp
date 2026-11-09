// CSWSPlaceable::LoadBodyBag (round s10, 0x005864B0): builds a body-bag
// placeable from the placeables 2DA row instead of a GFF. Own minimal layout;
// member names follow placeable_ctor.cpp / placeable_save.cpp where known.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void ClearLocString();
    void SetStrRef(unsigned long nStrRef) { m_nStrRef = nStrRef; }
    void *m_pList;
    unsigned long m_nStrRef;
};

class CResRef {
public:
    CResRef(const char *s);
    CResRef operator=(const CResRef &r);
    char m_resRef[16];
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pnValue);
};

class C2DArrays {
public:
    char pad_0x0[0x2c];
    C2DA *m_pPlaceablesTable;        // +0x2c
    C2DA *GetPlaceablesTable() { return m_pPlaceablesTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;

class CSWSModule {
public:
    int AddObjectToLookupTable(const CExoString &sTag, unsigned long oid);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSPlaceable {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void SetAnimation(int nAnimation);     // +0x7c
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual void vc8();
    virtual void SetPortrait(const CResRef &cPortrait);  // +0xcc

    int LoadBodyBag(unsigned char nAppearance);
    void SetTag(const CExoString &sTag);
    void SetCurrentHitPoints(int nHitPoints);
    void PostProcess();
    CExoString &GetTag() { return m_sTag; }
    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0xc0];
    int m_nHitPoints;                // +0xe0
    char pad_0xe4[0x14];
    int m_bPlot;                     // +0xf8
    char pad_0xfc[0x12c];
    CExoLocString m_sLocName;        // +0x228
    unsigned char m_nAppearance;     // +0x230
    char pad_0x231[0x3];
    CExoLocString m_sDescription;    // +0x234
    int m_n23C;                      // +0x23c (placeholder names below)
    CResRef m_cConversation;         // +0x240
    char pad_0x250[0x8];
    unsigned char m_n258;            // +0x258
    char pad_0x259[0x7];
    int m_n260;                      // +0x260
    CExoString m_sPortrait;          // +0x264
    int m_n26C;                      // +0x26c
    int m_n270;                      // +0x270
    unsigned char m_n274;            // +0x274
    unsigned char m_n275;            // +0x275
    unsigned char m_n276;            // +0x276
    char pad_0x277;
    int m_n278;                      // +0x278
    unsigned char m_n27C;            // +0x27c
    char pad_0x27d[0x3];
    int m_n280;                      // +0x280
    int m_n284;                      // +0x284
    int m_n288;                      // +0x288
    char pad_0x28c[0x4];
    unsigned char m_n290;            // +0x290
    char pad_0x291[0x3];
    CExoString m_sScripts[16];       // +0x294
    unsigned char m_n314;            // +0x314
    unsigned char m_n315;            // +0x315
    unsigned char m_n316;            // +0x316
    char pad_0x317[0xd];
    int m_n324;                      // +0x324
    int m_n328;                      // +0x328
    char pad_0x32c[0x4];
    int m_n330;                      // +0x330
    char pad_0x334[0x44];
    CResRef m_cResRef378;            // +0x378
    char pad_0x388[0x10];
    int m_n398;                      // +0x398
};

// FUNCTION: SWKOTOR 0x005864B0
int CSWSPlaceable::LoadBodyBag(unsigned char nAppearance)
{
    CExoLocString sLocName;
    CExoString sName;
    int nStrRef;
    unsigned char bMissing =
        !g_pRules->Get2DArrays()->GetPlaceablesTable()->GetINTEntry(nAppearance, "StrRef", &nStrRef);
    if (bMissing)
        nStrRef = 0;
    sLocName.SetStrRef(nStrRef);
    m_cResRef378 = "";
    SetTag("Body Bag");
    m_sLocName = sLocName;
    m_n270 = 0;
    m_n23C = 1;
    m_n280 = 1;
    m_n284 = 1;
    m_n27C = 15;
    m_n276 = 0;
    m_n278 = 0;
    m_n288 = 1;
    m_n290 = 0;
    m_bPlot = 0;
    m_n274 = 18;
    m_sPortrait = "";
    m_nAppearance = nAppearance;
    m_nHitPoints = 15;
    SetCurrentHitPoints(15);
    m_n314 = 16;
    m_n315 = 0;
    m_n316 = 0;
    m_n330 = 0;
    m_n258 = nAppearance != 0xd9 ? 5 : 1;
    m_n260 = 0;
    m_n328 = 1;
    m_n324 = 1;
    m_n26C = 0;
    m_n275 = 0;
    SetPortrait("PO_PLC_B04_");
    m_cConversation = "";
    sLocName.ClearLocString();
    sLocName.SetStrRef(38612);
    m_sDescription = sLocName;
    m_sScripts[10] = "";
    m_sScripts[0] = "";
    m_sScripts[1] = "";
    m_sScripts[2] = "";
    m_sScripts[3] = "";
    m_sScripts[4] = "";
    m_sScripts[5] = "";
    m_sScripts[6] = "";
    m_sScripts[7] = "";
    m_sScripts[8] = "";
    m_sScripts[9] = "";
    m_sScripts[11] = "";
    m_sScripts[12] = "";
    m_sScripts[13] = "";
    m_sScripts[14] = "";
    m_sScripts[15] = "";
    m_n398 = 0;
    SetAnimation(10000);
    g_pAppManager->GetServerExoApp()->GetModule()->AddObjectToLookupTable(GetTag(), GetId());
    PostProcess();
    return 1;
}
