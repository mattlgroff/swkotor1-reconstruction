// CSWCCreature::RequestModel (nwccreature.o, worker C s7) with a
// translation-unit-local layout. The start 0x0060C1E0 is missing from
// functions.csv (it lies inside the 0x0060BF60 extent there).

#include <stdio.h>
#include "c/exo_base.h"

class CResRef {
public:
    void CopyToString(CExoString& s) const;         // 0x00405F70
    char* GetResRefStr() const;                     // 0x00405FE0
    char m_resRef[16];
};

class RenderInterface {
public:
    static int IsResident(char* sModel);            // 0x0070D980
};

class CAuroraInterface {
public:
    static void RequestModel(char* sModel);         // 0x0070D8D0
};

struct Vector {
    float x, y, z;
};

struct Quaternion {
    float w, x, y, z;

    Quaternion(float fW, float fX, float fY, float fZ)
    {
        z = fZ;
        y = fY;
        x = fX;
        w = fW;
    }
};

extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634

class CSWCCreature;

class CAurPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void SetClientObject(CSWCCreature* pObject);    // +0x74 (descriptive)
    virtual void v78();
    virtual void SetObjectType(int nType);                  // +0x7c (descriptive)
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual int GetNodeTransform(char* sNode, Vector* pPosition, Quaternion* pOrientation); // +0xa0 (descriptive)
};

class CAnimBase {
public:
    virtual ~CAnimBase();
    virtual void v04();
    virtual CAurPart* GetModel(unsigned char nPart);        // +0x08
    virtual int LoadModel(const CResRef& resModel, unsigned char nPart); // +0x0c
    int SetValue(int nIndex, float fValue);         // 0x00698E30

    char pad_0x4[0xc4 - 0x4];
    unsigned char m_nAnimType;                      // 0xc4 (descriptive)
};

class CSWCAnimBase : public CAnimBase {
public:
    CSWCAnimBase();                                 // 0x0069DFB0

    char pad_0xc5[0xf0 - 0xc5 - 4];
};

class CSWCAnimBaseTW : public CSWCAnimBase {
public:
    CSWCAnimBaseTW();                               // 0x0069CBD0

    char pad_0xf0[0x174 - 0xf0];
};

class CSWCAnimBaseHead : virtual public CSWCAnimBase {
public:
    CSWCAnimBaseHead();                             // 0x0069BB80

    char pad_0x4[0x1c4 - 0xf0 - 0x4];
};

class CSWCAnimBaseWield : virtual public CSWCAnimBase {
public:
    CSWCAnimBaseWield();                            // 0x00699DD0

    char pad_0x4[0x1d0 - 0xf0 - 0x4];
};

class CSWCAnimBaseHeadWield : virtual public CSWCAnimBase {
public:
    CSWCAnimBaseHeadWield();                        // 0x00698EC0

    char pad_0x4[0x220 - 0xf0 - 0x4];
};

class CSWCCreature {
public:
    int RequestModel(const CResRef& resModel, const CResRef& resSuperModel, unsigned char nAnimType);
    int LoadModel(const CResRef& resModel, unsigned char nPart, unsigned char nAnimType);
    void RegisterAnimationCallbacks();              // 0x0061AB40 (descriptive)

    void* m_vtable;                                 // 0x00
    char pad_0x4[0x68 - 0x4];
    CSWCAnimBase* m_pModel;                         // 0x68
    char pad_0x6c[0xa4 - 0x6c];
    float m_fHeadConjureHeight;                     // 0xa4 (descriptive)
    char pad_0xa8[0x1f8 - 0xa8];
    int m_n1F8;                                     // 0x1f8
    char pad_0x1fc[0x358 - 0x1fc];
    CSWCAnimBase* m_pPendingModel;                  // 0x358 (descriptive)
    int m_n35C;                                     // 0x35c
};

// FUNCTION: SWKOTOR 0x0060C1E0
int CSWCCreature::RequestModel(const CResRef& resModel, const CResRef& resSuperModel, unsigned char nAnimType)
{
    if (!m_pModel || m_pModel->m_nAnimType != nAnimType) {
        switch (nAnimType) {
        case 0:
            m_pPendingModel = new CSWCAnimBase;
            break;
        case 11:
            m_pPendingModel = new CSWCAnimBaseTW;
            break;
        case 1:
            m_pPendingModel = new CSWCAnimBaseHead;
            break;
        case 2:
            m_pPendingModel = new CSWCAnimBaseWield;
            break;
        case 3:
            m_pPendingModel = new CSWCAnimBaseHeadWield;
            break;
        default:
            return 0;
        }
        if (!m_pPendingModel) {
            return 0;
        }
        m_pPendingModel->SetValue(0, 1850.0f);
        m_pPendingModel->SetValue(1, 6600.0f);
        m_pPendingModel->SetValue(2, 0.925f);
        m_pPendingModel->SetValue(3, 3.3f);
        m_n35C = 0;
        if (!m_pModel) {
            m_pModel = m_pPendingModel;
            m_n1F8 = 0;
            m_pPendingModel = 0;
        }
    }
    CExoString sModel;
    resModel.CopyToString(sModel);
    if (!RenderInterface::IsResident(sModel.CStr())) {
        CAuroraInterface::RequestModel(sModel.CStr());
        resSuperModel.CopyToString(sModel);
        sModel.GetLength();
        return 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0061B380
int CSWCCreature::LoadModel(const CResRef& resModel, unsigned char nPart, unsigned char nAnimType)
{
    if (m_pPendingModel) {
        if (m_pModel) {
            delete m_pModel;
            m_pModel = 0;
        }
        m_pModel = m_pPendingModel;
        m_pPendingModel = 0;
        m_n1F8 = m_n35C;
        m_n35C = 0;
    }
    if (!m_pModel || m_pModel->m_nAnimType != nAnimType) {
        if (m_pModel) {
            delete m_pModel;
            m_pModel = 0;
        }
        switch (nAnimType) {
        case 0:
            m_pModel = new CSWCAnimBase;
            break;
        case 11:
            m_pModel = new CSWCAnimBaseTW;
            break;
        case 1:
            m_pModel = new CSWCAnimBaseHead;
            break;
        case 2:
            m_pModel = new CSWCAnimBaseWield;
            break;
        case 3:
            m_pModel = new CSWCAnimBaseHeadWield;
            break;
        default:
            return 0;
        }
        if (!m_pModel) {
            return 0;
        }
        m_pModel->SetValue(0, 1850.0f);
        m_pModel->SetValue(1, 6600.0f);
        m_pModel->SetValue(2, 0.925f);
        m_pModel->SetValue(3, 3.3f);
        m_n1F8 = 0;
    }
    if (!m_pModel->LoadModel(resModel, nPart)) {
        char sError[256];
        sprintf(sError, "CSWCCreature::LoadModel(): Failed to load creature model '%s'.", resModel.GetResRefStr());
        return 0;
    }
    if (nPart == 0xff || nPart == 0xfe || nPart == 0xfd || nPart == 0xfc) {
        CAurPart* pPart = m_pModel->GetModel(nPart);
        pPart->SetClientObject(this);
        pPart->SetObjectType(OBJECT_TYPE_CREATURE);
        if (nPart == 0xff) {
            Quaternion qOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            Vector vPosition;
            vPosition.x = 0.0f;
            vPosition.y = 0.0f;
            vPosition.z = 0.0f;
            RegisterAnimationCallbacks();
            if (pPart->GetNodeTransform("headconjure", &vPosition, &qOrientation)) {
                m_fHeadConjureHeight = vPosition.z - vPosition.z * 0.125f;
                return 1;
            }
            m_fHeadConjureHeight = 2.1f;
        }
    }
    return 1;
}
