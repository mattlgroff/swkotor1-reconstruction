#include "c/exo_base.h"

class C2DA {
public:
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue); // 0x00414110
};
class C2DAs { public: char pad_0x0[0x78]; C2DA* m_pReactionTable; };
class CSWRules { public: char pad_0x0[0xb8]; C2DAs* m_p2DArrays; };
extern CSWRules* g_pRules;

class CSWCCreature {
public:
    int InCombat(); // 0x0060d320, preserved verified symbol (MODELTYPE predicate).
    int ResolveRangedAnimation(int nAttackType, int nStance);
    unsigned short ResolveReaction(int nAnimation, unsigned short nReaction, int nStance);
};

// FUNCTION: SWKOTOR 0x00614110
// NONMATCH: compiler lays the four selector groups out in reverse order.
int CSWCCreature::ResolveRangedAnimation(int nAttackType, int nStance)
{
    int nAnimation = 0;
    int bNormal = !InCombat();
    if (nAttackType == 0x52 || nAttackType == 0x12 || nAttackType == 0x1d) {
        if (bNormal) {
            switch (nStance) {
            case 5: nAnimation = 0x16a; break;
            case 6: nAnimation = 0x16b; break;
            case 7: nAnimation = 0x16c; break;
            case 9: nAnimation = 0x16d; break;
            }
        } else nAnimation = 0x169;
        return nAnimation;
    }
    if (nAttackType == 0x1a || nAttackType == 0x5c || nAttackType == 0x1e) {
        if (bNormal) {
            switch (nStance) {
            case 5: nAnimation = 0xda; break;
            case 6: nAnimation = 0xe8; break;
            case 7: nAnimation = 0xf0; break;
            case 9: nAnimation = 0x161; break;
            }
        } else nAnimation = 0x121;
        return nAnimation;
    }
    if (nAttackType == 0x4d || nAttackType == 0x14 || nAttackType == 0x1f) {
        if (bNormal) {
            switch (nStance) {
            case 5: nAnimation = 0xdb; break;
            case 6: nAnimation = 0xe9; break;
            case 7: nAnimation = 0xf1; break;
            case 9: nAnimation = 0x162; break;
            }
        } else nAnimation = 0x15f;
        return nAnimation;
    }
    {
        if (bNormal) {
            switch (nStance) {
            case 5: nAnimation = 0xd9; break;
            case 6: nAnimation = 0xe7; break;
            case 7: nAnimation = 0xef; break;
            case 9: nAnimation = 0x160; break;
            }
        } else nAnimation = 0x120;
    }
    return nAnimation;
}

// FUNCTION: SWKOTOR 0x0060D140
// NONMATCH: selector lifetime and constant-reaction/query block placement (29 insns).
unsigned short CSWCCreature::ResolveReaction(int nAnimation, unsigned short nReaction, int nStance)
{
    CExoString sRow((int)(unsigned short)nAnimation);
    CExoString sColumn(nStance);
    int nResult = 0;
    switch (nReaction) {
    case 0x2711:
        nResult = 0x2711;
        break;
    case 0x271b:
        sColumn = CExoString("Dodge") + sColumn;
        break;
    case 0x271c:
        sColumn = CExoString("Parry") + sColumn;
        break;
    case 0x271e:
        sColumn = CExoString("Damage") + sColumn;
        break;
    }
    if (nReaction != 0x2711) {
        g_pRules->m_p2DArrays->m_pReactionTable->GetINTEntry(sRow, sColumn, &nResult);
    }
    return (unsigned short)nResult;
}
