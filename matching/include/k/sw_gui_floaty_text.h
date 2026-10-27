// Worker K: floating text labels over objects (CSWGuiFloatyText, vtable
// 0x00753EA8, 0x154 bytes), owned by the main interface list at +0x5cb4.
// Minimal self-contained copies of the GUI label, client object and camera
// classes (see k/sw_gui_main_interface.h, n/sw_c_object.h).
#ifndef K_SW_GUI_FLOATY_TEXT_H
#define K_SW_GUI_FLOATY_TEXT_H

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-() const { return Vector(-x, -y, -z); }
    float operator*(const Vector& v) const { return x * v.x + y * v.y + z * v.z; }
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
    Vector ToDirection() const;                     // 0x0043E250
};

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

class CSWGuiTextParams {
public:
    void SetBaseFont(const CResRef& cFont);         // 0x00415DD0
    void SetText(const CExoString& sText);          // 0x00415E00
    void SetAlignment(int nAlign);                  // 0x00415F20
    void SetColor(const Vector& vColor);            // 0x00414E10
    CExoString m_sText;                             // 0x00
    char pad_0x8[0x20];
    float m_fAlpha;                                 // 0x28
    char pad_0x2c[0x2c];
};

class CSWGuiLabelText {
public:
    virtual void v00();
    int GetLineHeight();                            // 0x00414EE0
    char pad_0x4[0x14];
};

class CSWGuiControl {
public:
    virtual ~CSWGuiControl();                       // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    CSWGuiExtent m_extent;                          // 0x04
    char pad_0x14[0x30];
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_bActive : 1;            //      bit 0
            unsigned long m_bVisible : 1;           //      bit 1
            unsigned long m_nFlagsRest : 30;
        };
    };
    char pad_0x48[0x14];
};

// 0x140 bytes.
class CSWGuiLabel : public CSWGuiControl {
public:
    char pad_0x5c[0x74];
    CSWGuiLabelText m_cTextBox;                     // 0xd0
    CSWGuiTextParams m_cText;                       // 0xe8
};

class CSWGuiFloatyText : public CSWGuiLabel {
public:
    void Init(unsigned long oid, const CExoString& sText, const Vector& vColor,
              float fDuration);                     // 0x00688E10
    void Update(float fDelta);                      // 0x00688FF0
    float m_fTimeLeft;                              // 0x140
    float m_fDuration;                              // 0x144
    unsigned long m_oidObject;                      // 0x148
    unsigned char m_nStack;                         // 0x14c
    short m_nScreenX;                               // 0x14e
    short m_nScreenY;                               // 0x150
};

// Render object (Gob slot +0x64, see m/sw_c_module_camera.h).
class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual Vector GetPosition();                   // +0x64
};

class CSWCCreature;
class CSWCObject;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void SetId(unsigned long oid);          // 0x04
    virtual void ResetUpdateTimes(unsigned long nDay, unsigned long nTime);  // 0x08
    virtual CSWCObject* AsSWCObject();              // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
};

class CSWCObject : public CGameObject {
public:
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CAurObject* GetPartModel(unsigned char nPart, int bVisible);    // 0x98 (placeholder name)
    virtual void v9C(); virtual void vA0(); virtual void vA4(); virtual void vA8();
    virtual void vAC(); virtual void vB0(); virtual void vB4(); virtual void vB8();
    virtual void vBC(); virtual void vC0(); virtual void vC4(); virtual void vC8();
    virtual void vCC(); virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void vDC(); virtual void vE0(); virtual void vE4(); virtual void vE8();
    virtual void vEC(); virtual void vF0(); virtual void vF4(); virtual void vF8();
    virtual void vFC(); virtual void v100(); virtual void v104(); virtual void v108();
    virtual void v10C(); virtual void v110(); virtual void v114(); virtual void v118();
    virtual void v11C(); virtual void v120(); virtual void v124(); virtual void v128();
    virtual void v12C(); virtual void v130(); virtual void v134(); virtual void v138();
    virtual void GetTextAnchor(Vector* pAnchor);   // 0x13c (placeholder name)
};

class CSWCModuleCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                   // +0x20 (placeholder name)
    virtual Quaternion GetOrientation();            // +0x24 (placeholder name)
    virtual Vector WorldToScreen(Vector v, int b);  // +0x28 (placeholder name)
};

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWCModuleCamera* GetModuleCamera();            // 0x005ED560
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiMainInterface {
public:
    void RemoveControl(CSWGuiControl* pControl);    // 0x00688060
};

extern CSWGuiMainInterface* g_pMainInterface;       // 0x00833BB4 (descriptive)

#endif
