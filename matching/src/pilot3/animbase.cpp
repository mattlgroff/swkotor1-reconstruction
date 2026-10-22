// Pilot group 3: CSWCAnimBaseHeadWield constructor (virtual inheritance).
// Build flags: /O2 /Gy /GX (EH state for constructed virtual bases).

class CSWCAnimBase {
public:
    CSWCAnimBase();
    virtual ~CSWCAnimBase();
    virtual void Update();
    char pad_0x4[0xc0];
    unsigned char m_nAnimBaseType; // +0xc4
    char pad_0xc5[0x174 - 0xc5];
};

class CSWCAnimBaseHead : public virtual CSWCAnimBase {
public:
    CSWCAnimBaseHead();
    ~CSWCAnimBaseHead();
    char pad_0x4[0x48];
};

class CSWCAnimBaseWield : public virtual CSWCAnimBase {
public:
    CSWCAnimBaseWield();
    ~CSWCAnimBaseWield();
    char pad_0x4[0x10];
};

class CSWCAnimBaseHeadWield : public virtual CSWCAnimBaseHead, public virtual CSWCAnimBaseWield {
public:
    CSWCAnimBaseHeadWield();
    virtual ~CSWCAnimBaseHeadWield();
    virtual void Update();
};

// FUNCTION: SWKOTOR 0x00698EC0
CSWCAnimBaseHeadWield::CSWCAnimBaseHeadWield()
{
    m_nAnimBaseType = 3;
}
