// Pilot 4 arm A: CSWCCreature::GetMeleeAnimation.
// NONMATCH (171 diff insns, same size): layout, jump tables and tail merging match
// except one shared case-1 block; registers swapped throughout (original keeps nAnim
// in esi and nWeapon/nBase in edi, ours the reverse).
#include "pilot4/a/p4a_c_creature.h"

static inline int IsGroupA(int n) { return n == 0x51 || n == 0x13 || n == 8; }
static inline int IsGroupB(int n) { return n == 0x35 || n == 0x5b || n == 0xb; }
static inline int IsGroupC(int n) { return n == 0x53 || n == 0x11 || n == 0x1c; }
static inline int IsGroupD(int n) { return n == 0x67 || n == 0x66 || n == 0x65; }

// FUNCTION: SWKOTOR 0x00613DA0
int CSWCCreature::GetMeleeAnimation(unsigned short nCurrent, int nWeapon, int bDual, int nCategory)
{
    int nAnim = 0;
    if (nWeapon && !GetIsSmallOrLargeModel()) {
        if (IsGroupA(nWeapon)) {
            switch (nCategory) {
            case 1: nAnim = 0x57; break;
            case 2: nAnim = 0x71; break;
            case 3: nAnim = 0x9a; break;
            case 4: nAnim = 0xc3; break;
            }
            return nAnim;
        }
        if (IsGroupB(nWeapon)) {
            switch (nCategory) {
            case 1: nAnim = 0x58; break;
            case 2: nAnim = 0x72; break;
            case 3: nAnim = 0x9b; break;
            case 4: nAnim = 0xc4; break;
            }
            return nAnim;
        }
        if (IsGroupC(nWeapon)) {
            switch (nCategory) {
            case 1: nAnim = 0x57; break;
            case 2: nAnim = 0x73; break;
            case 3: nAnim = 0x9c; break;
            case 4: nAnim = 0xc5; break;
            }
            return nAnim;
        }
        if (IsGroupD(nWeapon)) {
            switch (nCategory) {
            case 2: nAnim = 0x182; break;
            case 3: nAnim = 0x183; break;
            case 4: nAnim = 0x184; break;
            }
            return nAnim;
        }
    } else {
        int nBase = m_nAnimationBase;
        if (nCurrent == 0x277d) {
            if (bDual) {
                if (GetIsSmallOrLargeModel()) {
                    if (GetAnimationBase()->HasAnimation(0x11a)) {
                        nAnim = nBase + 0x11a;
                    } else {
                        nAnim = nBase + 0x114;
                    }
                } else {
                    switch (nCategory) {
                    case 1: nAnim = nBase + 0x57; break;
                    case 2: nAnim = nBase + 0x7d; break;
                    case 3: nAnim = nBase + 0xa6; break;
                    case 4: nAnim = nBase + 0xcf; break;
                    case 8: nAnim = nBase + 0xf7; break;
                    }
                }
            } else {
                switch (nCategory) {
                case 2: nAnim = nBase + 0x5e; break;
                case 3: nAnim = nBase + 0x87; break;
                case 4: nAnim = nBase + 0xb0; break;
                case 1: nAnim = nBase % 2 + 0x57; break;
                case 8: nAnim = nBase % 2 + 0xf7; break;
                }
            }
        } else {
            if (GetIsSmallOrLargeModel()) {
                nAnim = nBase + 0x114;
            } else {
                switch (nCategory) {
                case 1: nAnim = nBase + 0x57; break;
                case 2: nAnim = nBase + 0x7a; break;
                case 3: nAnim = nBase + 0xa3; break;
                case 4: nAnim = nBase + 0xcc; break;
                case 8: nAnim = nBase + 0xf7; break;
                }
            }
        }
    }
    return nAnim;
}
