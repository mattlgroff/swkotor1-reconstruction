// Pilot group 2: small leaf functions. MSVC 7.0 /O2 matching.

void* operator new(unsigned int n);
void operator delete(void* p);

class CAurTextureBasic {
public:
    CAurTextureBasic() {}
    virtual ~CAurTextureBasic();
};

// MSVC emits the vftable (and with it the scalar deleting destructor) in
// the TU that instantiates the constructor; ~CAurTextureBasic itself lives
// elsewhere and is called, not inlined.
CAurTextureBasic* NewAurTextureBasic() { return new CAurTextureBasic; }

// FUNCTION: SWKOTOR 0x004233C0
/* CAurTextureBasic::`scalar deleting destructor'( */

class CGameEffect {
public:
    char pad_0x00[0x78];
    unsigned long m_oidObjects[4];

    unsigned long GetObjectID(int nIndex);
};

// FUNCTION: SWKOTOR 0x005036E0
unsigned long CGameEffect::GetObjectID(int nIndex)
{
    return m_oidObjects[nIndex];
}

class CSWSFaction {
public:
    void* vtable_dummy;
    int m_nMembers;

    unsigned long GetLeader();
    void RemoveMember(unsigned long oidMember);
    int AttemptRemove(unsigned long oidMember, unsigned long oidRequester);
};

// FUNCTION: SWKOTOR 0x005BFB20
int CSWSFaction::AttemptRemove(unsigned long oidMember, unsigned long oidRequester)
{
    if (m_nMembers > 1) {
        if (oidMember == oidRequester || GetLeader() == oidRequester) {
            RemoveMember(oidMember);
            return 1;
        }
    }
    return 0;
}

class CAurTexture;
CAurTexture* AurTextureFind(char* name, CAurTexture* tex, int a, int b, int c);

// FUNCTION: SWKOTOR 0x00423A60
CAurTexture* AurTextureGetReference(char* name, int a)
{
    CAurTexture* tex = AurTextureFind(name, 0, a, 0, 0);
    return AurTextureFind(0, tex, 0, 0, 0);
}
