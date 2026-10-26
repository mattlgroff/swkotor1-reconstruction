// small CSWSCreature leaves in 0x005B4880-0x005B8050 (debilitated check,
// three-way range state at +0x558, appearance test, attacker list iteration), two
// empty stubs (0x005B5E90, 0x005C5630) and the record lookup at 0x005BE6D0.

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    float x, y, z;
};

float Magnitude(const Vector &v);

class CSWSCombatAttackData {
public:
    char pad_0x0[0x8];
    unsigned short m_nLength;        // +0x8 (placeholder name)
    char pad_0xa[0x6];
    unsigned short m_nTravelTime;    // +0x10
    unsigned short m_nReactAnimation; // +0x12
    char pad_0x14[0x48];
    unsigned char m_nAttackResult;   // +0x5c
    char pad_0x5d[0xb];
    Vector m_vTarget;                // +0x68
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData *GetAttack(int nAttack);
    int GetActionValid(unsigned long oidAttacker, int nLength, int bFlag);
    void DecrementRoundLength(int nLength, int bFlag);
    char pad_0x0[0x948];
    int m_bTargetDebilitated;        // +0x948
    char pad_0x94c[0x96c - 0x94c];
    unsigned char m_nCurrentAttack;  // +0x96c
    char pad_0x96d[0x9b8 - 0x96d];
    int m_bReactions;                // +0x9b8 (placeholder name)
};

class CSWSCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation); // +0x7c
    unsigned long m_idSelf;          // +0x4
};

class CSWSObject : public CGameObject {
public:
    char pad_0x8[0x88];
    Vector m_vPosition;              // +0x90
};

class CSWSCreature : public CSWSObject {
public:
    int IsDebilitated();
    int GetDeadTemp();
    int GetIsRangeState0(int nValue);
    int GetIsRangeState1(int nValue);
    int GetIsRangeState2(int nValue);
    void SetRangeState(int nValue);
    int GetIsSpecialAppearance(unsigned short nAppearance);
    void Stub_005B5E90(int a, int b);
    unsigned long GetNextAttacker();
    void ClearAttackerList();
    Vector GetRangedMissLocation(CSWSObject *pTarget);
    void ResolveRangedHit(CSWSObject *pTarget);
    void ResolveRangedMiss(CSWSObject *pTarget);
    void SetCombatReactionTime(int nAttack, short nLength, short nFlag);
    int IsLightsaberEquipped();
    void ResolveRangedAnimations(CSWSObject *pTarget, unsigned short nLength, int bNoReaction);
    void ResolveMeleeAnimations(int nUnused1, int nUnused2, CSWSObject *pTarget, int nLength,
                                int bForceReaction);

    char pad_0x9c[0xd4 - 0x9c];
    unsigned short m_nAppearance;    // +0xd4
    char pad_0xd6[0x558 - 0xd6];
    int m_nRangeState;               // +0x558
    char pad_0x55c[0x8ed - 0x55c];
    unsigned char m_bStunned;        // +0x8ed
    char pad_0x8ee[0x9c8 - 0x8ee];
    CSWSCombatRound *m_pcCombatRound; // +0x9c8
    char pad_0x9cc[0xa80 - 0x9cc];
    CExoArrayList<unsigned long> *m_pAttackerList; // +0xa80
    int m_nAttackerListIndex;        // +0xa84
};

// FUNCTION: SWKOTOR 0x005B4880
int CSWSCreature::IsDebilitated()
{
    int bDebilitated = 0;
    if (m_bStunned || GetDeadTemp())
        bDebilitated = 1;
    return bDebilitated;
}

// FUNCTION: SWKOTOR 0x005B5510
int CSWSCreature::GetIsRangeState0(int nValue)
{
    int bResult = 0;
    if (nValue == -1) {
        if (m_nRangeState == 0)
            return 1;
    } else if (nValue > 0 && nValue <= 3) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B5540
int CSWSCreature::GetIsRangeState1(int nValue)
{
    int bResult = 0;
    if (nValue == -1) {
        if (m_nRangeState == 1)
            return 1;
    } else if (nValue >= 4 && nValue <= 6) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B5570
int CSWSCreature::GetIsRangeState2(int nValue)
{
    int bResult = 0;
    if (nValue == -1) {
        if (m_nRangeState == 2)
            return 1;
    } else if (nValue >= 8 && nValue <= 10) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B55A0
int CSWSCreature::GetIsSpecialAppearance(unsigned short nAppearance)
{
    if (nAppearance == -1)
        nAppearance = m_nAppearance;
    int bResult = 0;
    switch (nAppearance) {
    case 10015:
    case 10016:
    case 10017:
    case 10018:
    case 10019:
    case 10020:
    case 10061:
    case 10070:
    case 10071:
    case 10401:
    case 11000:
        bResult = 1;
        break;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B5E90
void CSWSCreature::Stub_005B5E90(int a, int b)
{
}

// NONMATCH (22): register roles; the original keeps pTarget in ebp for the
// whole function and spills pAttack to the pTarget argument slot.
// FUNCTION: SWKOTOR 0x005B6C40
void CSWSCreature::ResolveRangedAnimations(CSWSObject *pTarget, unsigned short nLength, int bNoReaction)
{
    if (pTarget == 0)
        return;
    int nAttack = m_pcCombatRound->m_nCurrentAttack;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(nAttack);
    pAttack->m_nLength = nLength;
    pAttack->m_nTravelTime = 0;
    m_pcCombatRound->DecrementRoundLength(nLength, 0);
    CSWSCreature *pCreature = pTarget->AsSWSCreature();
    if (!pCreature)
        return;
    m_pcCombatRound->m_bTargetDebilitated = pCreature->IsDebilitated();
    if (!bNoReaction && !pCreature->IsLightsaberEquipped() && !pCreature->GetIsSpecialAppearance(0xffff) &&
        pCreature->m_pcCombatRound->m_bReactions)
        pAttack->m_nReactAnimation = 0x271b;
    else
        pAttack->m_nReactAnimation = 0x2711;
    if (pCreature->IsDebilitated()) {
        pAttack->m_nReactAnimation = 0x2711;
        return;
    }
    unsigned long oidSelf = m_idSelf;
    CSWSCombatRound *pRound = pTarget->AsSWSCreature()->m_pcCombatRound;
    if (!pRound->GetActionValid(oidSelf, nLength, 0))
        return;
    if (!m_pcCombatRound->m_bReactions)
        return;
    SetCombatReactionTime(nAttack, nLength, 0);
    pCreature->SetAnimation(pAttack->m_nReactAnimation);
    pCreature->m_pcCombatRound->DecrementRoundLength(nLength, 0);
}

// Names are descriptive: the reaction animation / projectile travel time of
// the current ranged attack (0x271e hit, 0x271c deflect).
// FUNCTION: SWKOTOR 0x005B6DB0
void CSWSCreature::ResolveRangedHit(CSWSObject *pTarget)
{
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    if (GetIsRangeState0(pAttack->m_nAttackResult))
        pAttack->m_nReactAnimation = 0x271e;
    else if (GetIsRangeState2(pAttack->m_nAttackResult))
        pAttack->m_nReactAnimation = 0x271c;
    pAttack->m_vTarget = pTarget->m_vPosition;
    pAttack->m_nTravelTime = (unsigned short)(Magnitude(pTarget->m_vPosition - m_vPosition) * 23.8095f);
}

// FUNCTION: SWKOTOR 0x005B6E90
void CSWSCreature::ResolveRangedMiss(CSWSObject *pTarget)
{
    if (!pTarget)
        return;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
    pAttack->m_vTarget = GetRangedMissLocation(pTarget);
    pAttack->m_nTravelTime = (unsigned short)(Magnitude(pAttack->m_vTarget - m_vPosition) * 23.8095f);
}

// NONMATCH (9, 4 real): after the inlined IsDebilitated test cl reuses the
// known-zero eax for the `push 0` of GetActionValid; the original pushes an
// immediate and keeps eax free.
// FUNCTION: SWKOTOR 0x005B7470
void CSWSCreature::ResolveMeleeAnimations(int nUnused1, int nUnused2, CSWSObject *pTarget,
                                          int nLength, int bForceReaction)
{
    if (!pTarget)
        return;
    int nAttack = m_pcCombatRound->m_nCurrentAttack;
    CSWSCombatAttackData *pAttack = m_pcCombatRound->GetAttack(nAttack);
    pAttack->m_nLength = (unsigned short)nLength;
    m_pcCombatRound->DecrementRoundLength(nLength, 0);
    CSWSCreature *pCreature = pTarget->AsSWSCreature();
    if (!pCreature)
        return;
    if (GetIsRangeState0(pAttack->m_nAttackResult))
        pAttack->m_nReactAnimation = 0x271e;
    else if (GetIsRangeState2(pAttack->m_nAttackResult))
        pAttack->m_nReactAnimation = 0x271c;
    else if (GetIsRangeState1(pAttack->m_nAttackResult))
        pAttack->m_nReactAnimation = 0x271b;
    m_pcCombatRound->m_bTargetDebilitated = pCreature->IsDebilitated();
    if (!pCreature->IsDebilitated() &&
        (pCreature->m_pcCombatRound->GetActionValid(m_idSelf, nLength, 0) || bForceReaction) &&
        m_pcCombatRound->m_bReactions) {
        SetCombatReactionTime(nAttack, nLength, 0);
        pCreature->SetAnimation(pAttack->m_nReactAnimation);
        pCreature->m_pcCombatRound->DecrementRoundLength(nLength, 0);
    } else {
        pAttack->m_nReactAnimation = 0x2711;
    }
}

// FUNCTION: SWKOTOR 0x005B7400
void CSWSCreature::SetRangeState(int nValue)
{
    if ((nValue == -1 && m_nRangeState == 0) || (nValue > 0 && nValue <= 3))
        m_nRangeState = 0;
    else if ((nValue == -1 && m_nRangeState == 1) || (nValue >= 4 && nValue <= 6))
        m_nRangeState = 1;
    else if (nValue == -1) {
        if (m_nRangeState == 2)
            m_nRangeState = 2;
    } else if (nValue >= 8 && nValue <= 10)
        m_nRangeState = 2;
}

// FUNCTION: SWKOTOR 0x005B7FE0
unsigned long CSWSCreature::GetNextAttacker()
{
    unsigned long oidAttacker = 0x7f000000;
    if (m_pAttackerList && m_nAttackerListIndex < m_pAttackerList->num) {
        int nIndex = m_nAttackerListIndex;
        m_nAttackerListIndex = nIndex + 1;
        oidAttacker = m_pAttackerList->element[nIndex];
    }
    return oidAttacker;
}

// FUNCTION: SWKOTOR 0x005B8010
void CSWSCreature::ClearAttackerList()
{
    if (m_pAttackerList) {
        delete m_pAttackerList;
        m_pAttackerList = 0;
        m_nAttackerListIndex = 0;
    }
}

class CSWSJournalStub {
public:
    void Stub_005C5630(int a, int b, int c);
};

// FUNCTION: SWKOTOR 0x005C5630
void CSWSJournalStub::Stub_005C5630(int a, int b, int c)
{
}

struct SRecordEntry {
    unsigned short m_nId;            // +0x0
    char pad_0x2[0x2];
    int m_nPrimaryKey;               // +0x4
    int m_nSecondaryKey;             // +0x8
    unsigned char m_nState;          // +0xc
    char pad_0xd[0x3];
};

class CRecordTable {
public:
    int FindRecord(int bPrimary, int nKey);

    char pad_0x0[0x174];
    SRecordEntry *m_pRecords;        // +0x174
    unsigned short m_nRecords;       // +0x178
};

// NONMATCH (6): only the scratch register of the two `return m_pRecords[i].m_nId` paths
// differs (original edx / eax, ours eax / ecx); the separate primary-key `continue` block
// reproduces the rest.
// FUNCTION: SWKOTOR 0x005BE6D0
int CRecordTable::FindRecord(int bPrimary, int nKey)
{
    unsigned short nCount = m_nRecords;
    int nResult = -1;
    int nFree = 0;
    for (unsigned short i = 0; i < nCount; i++) {
        SRecordEntry *pEntry = &m_pRecords[i];
        if (pEntry->m_nState == 1 || pEntry->m_nState == 5) {
            int nValue;
            if (bPrimary) {
                nValue = pEntry->m_nPrimaryKey;
                if (nValue == nKey)
                    return m_pRecords[i].m_nId;
                if (nValue == 0 && nFree != nKey) {
                    nResult = pEntry->m_nId;
                    nFree++;
                }
                continue;
            } else {
                nValue = pEntry->m_nSecondaryKey;
                if (nValue == nKey)
                    return m_pRecords[i].m_nId;
            }
            if (nValue == 0 && nFree != nKey) {
                nResult = pEntry->m_nId;
                nFree++;
            }
        }
    }
    return nResult;
}
