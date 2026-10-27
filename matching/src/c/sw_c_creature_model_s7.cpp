// CSWCCreature::RequestModel (nwccreature.o, worker C s7) with a
// translation-unit-local layout. The start 0x0060C1E0 is missing from
// functions.csv (it lies inside the 0x0060BF60 extent there).

#include "c/exo_base.h"

class CResRef {
public:
    void CopyToString(CExoString& s) const;         // 0x00405F70
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

class CAnimBase {
public:
    virtual ~CAnimBase();
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

    void* m_vtable;                                 // 0x00
    char pad_0x4[0x68 - 0x4];
    CSWCAnimBase* m_pModel;                         // 0x68
    char pad_0x6c[0x1f8 - 0x6c];
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
