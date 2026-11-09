// CSWCAnimBase: bounding extent wrapper and the server -> client
// animation id map.
#include "k/sw_c_anim_base.h"

// FUNCTION: SWKOTOR 0x0069F630
int CSWCAnimBase::GetBoundingExtent(CSWGuiExtent* pExtent)
{
    return GetBoundingRect(m_pModel, pExtent);
}

// The id map itself. The original evaluates the animation set, the id and
// the creature type into by-value temporaries before the first test, as an
// inlined helper does (helper name descriptive; a plain `inline` is not
// expanded by cl at this size, hence __forceinline).
__forceinline unsigned short CSWCAnimBase::MapAnimation(unsigned char nSet, unsigned short nAnim, unsigned char nType)
{
    if (nSet == 1) {
        switch (nAnim) {
        case 0x2712:
        case 0x2713:
            if (nType == 2) {
                if ((unsigned char)m_nDC > 0) {
                    nAnim = 0x152;
                    break;
                }
            }
            else if (nType == 4) {
                if ((unsigned char)m_nDC > 0) {
                    nAnim = 0x153;
                    break;
                }
            }
            else if (nType == 3) {
                if ((unsigned char)m_nDC > 0) {
                    nAnim = 0x155;
                    break;
                }
            }
            else if (nType == 7) {
                nAnim = 0x154;
                break;
            }
            else if (nType == 9) {
                nAnim = 0x154;
                break;
            }
            nAnim = 0;
            break;
        case 0x2714:
            if (m_nCC) {
                nAnim = 0;
                break;
            }
            if (nType == 2) {
                if ((unsigned char)m_nDC > 0) {
                    nAnim = 0x159;
                    break;
                }
            }
            else if (nType == 4) {
                if ((unsigned char)m_nDC > 0) {
                    nAnim = 3;
                    break;
                }
            }
            else if (nType == 3) {
                if ((unsigned char)m_nDC > 0) {
                    nAnim = 0x157;
                    break;
                }
            }
            else if (nType == 7) {
                nAnim = 0x156;
                break;
            }
            else if (nType == 9) {
                nAnim = 0x156;
                break;
            }
            nAnim = 2;
            break;
        case 0x271f:
            nAnim = 0x3e;
            break;
        case 0x2720:
            nAnim = 0x40;
            break;
        case 0x2716:
            nAnim = 0x51;
            break;
        case 0x2718:
            nAnim = 0x53;
            break;
        case 0x2721:
            nAnim = 0x3f;
            break;
        case 0x2722:
            nAnim = 0x41;
            break;
        case 0x2723:
            nAnim = 0x43;
            break;
        case 0x271c:
            nAnim = 0x12d;
            break;
        case 0x271b:
            nAnim = 0x12e;
            break;
        case 0x2727:
            nAnim = 0x4d;
            break;
        case 0x2744:
            nAnim = 0x7;
            break;
        case 0x2747:
            nAnim = 0xc;
            break;
        case 0x2748:
            nAnim = 0xd;
            break;
        case 0x2749:
            nAnim = 0xe;
            break;
        case 0x274a:
            nAnim = 0xf;
            break;
        case 0x2746:
            nAnim = 0xa;
            break;
        case 0x2745:
            nAnim = 0xb;
            break;
        case 0x272c:
            nAnim = 0x21;
            break;
        case 0x273c:
            nAnim = 0x11;
            break;
        case 0x2730:
        case 0x2731:
            nAnim = 0x18;
            break;
        case 0x2733:
            nAnim = 0x13;
            break;
        case 0x274b:
            nAnim = 0x28;
            break;
        case 0x274c:
            nAnim = 0x29;
            break;
        case 0x274d:
            nAnim = 0x46;
            break;
        case 0x2737:
            nAnim = 0x1b;
            break;
        case 0x2738:
            nAnim = 0x1a;
            break;
        case 0x2739:
            nAnim = 0x1d;
            break;
        case 0x273a:
            nAnim = 0x1c;
            break;
        case 0x2736:
            nAnim = 0x19;
            break;
        case 0x2756:
            nAnim = 0x25;
            break;
        case 0x272d:
            nAnim = 0x1f;
            break;
        case 0x272e:
            nAnim = 0x12;
            break;
        case 0x2732:
            nAnim = 0x10;
            break;
        case 0x271e:
        case 0x275d:
            nAnim = 0x12f;
            break;
        case 0x276d:
            nAnim = 0x1;
            break;
        case 0x276e:
            nAnim = m_nCC ? 1 : 4;
            break;
        case 0x2795:
            nAnim = 0x5;
            break;
        case 0x276c:
            nAnim = 0x8;
            break;
        case 0x2791:
            nAnim = 0x39;
            break;
        case 0x2792:
            nAnim = 0x3a;
            break;
        case 0x2785:
            nAnim = 0x4b;
            break;
        case 0x2788:
            nAnim = 0x20;
            break;
        case 0x2789:
            nAnim = 0x2c;
            break;
        case 0x278a:
            nAnim = 0x35;
            break;
        case 0x278b:
            nAnim = 0x36;
            break;
        case 0x278d:
            nAnim = 0x2b;
            break;
        case 0x278e:
            nAnim = 0x44;
            break;
        case 0x278f:
        case 0x2798:
            nAnim = 0x26;
            break;
        case 0x2786:
            nAnim = 0x10e;
            break;
        case 0x278c:
            nAnim = 0x4a;
            break;
        case 0x2790:
        case 0x2796:
            nAnim = 0x2f;
            break;
        case 0x2793:
        case 0x2797:
            nAnim = 0x30;
            break;
        case 0x2794:
        case 0x279d:
            nAnim = 0x33;
            break;
        case 0x279c:
            nAnim = 0x34;
            break;
        case 0x2799:
            nAnim = 0x4c;
            break;
        case 0x279a:
            nAnim = 0x4e;
            break;
        case 0x279b:
            nAnim = 0x4f;
            break;
        case 0x2711:
        case 0x27a4:
            switch (nType) {
            case 1:
                nAnim = 0x5c;
                break;
            case 2:
                nAnim = 0x85;
                break;
            case 3:
                nAnim = 0xae;
                break;
            case 4:
                nAnim = 0xd7;
                break;
            case 5:
                nAnim = 0xdf;
                break;
            case 6:
                nAnim = 0xed;
                break;
            case 8:
                nAnim = 0xf9;
                break;
            case 7:
            case 9:
                nAnim = 0xf5;
                break;
            }
            break;
        case 0x2719:
        case 0x277d:
            nAnim = 0x12c;
            break;
        case 0x279e:
            nAnim = 0x15a;
            break;
        case 0x27a1:
            nAnim = 0x163;
            break;
        case 0x27a3:
            nAnim = 0x165;
            break;
        case 0x2710:
        case 0x27a5:
            nAnim = 0x6;
            break;
        case 0x27a6:
            nAnim = 0x48;
            break;
        case 0x27b2:
            nAnim = 0x45;
            break;
        case 0x27a7:
            nAnim = 0x167;
            break;
        case 0x27a8:
            nAnim = 0x168;
            break;
        case 0x27ac:
            nAnim = 0x177;
            break;
        case 0x27aa:
            nAnim = 0x172;
            break;
        case 0x27ab:
            nAnim = 0x173;
            break;
        case 0x27ad:
            switch (nType) {
            case 4:
                nAnim = 0x178;
                break;
            case 3:
                nAnim = 0x17a;
                break;
            case 2:
                nAnim = 0x179;
                break;
            }
            break;
        case 0x27ae:
            switch (nType) {
            case 4:
                nAnim = 0xd6;
                break;
            case 2:
                nAnim = 0x84;
                break;
            case 3:
                nAnim = 0xad;
                break;
            }
            break;
        case 0x27af:
            nAnim = 0x22;
            break;
        case 0x27b0:
            nAnim = 0x23;
            break;
        case 0x27b1:
            nAnim = 0x47;
            break;
        case 0x27b3:
            nAnim = 0x180;
            break;
        case 0x27b4:
            nAnim = 0x181;
            break;
        case 0x27eb:
        case 0x27ec:
            nAnim = 0x55;
            break;
        case 0x27ed:
            nAnim = 0x50;
            break;
        case 0x27ee:
            nAnim = 0x52;
            break;
        case 0x27ef:
            nAnim = 0x17d;
            break;
        case 0x27f0:
            nAnim = 0x17e;
            break;
        case 0x2806:
            switch (nType) {
            case 1:
                nAnim = 0x5b;
                break;
            case 2:
                nAnim = 0x84;
                break;
            case 3:
                nAnim = 0xad;
                break;
            case 4:
                nAnim = 0xd6;
                break;
            case 5:
                nAnim = 0xde;
                break;
            case 6:
                nAnim = 0xec;
                break;
            case 7:
            case 9:
                nAnim = 0xf4;
                break;
            case 8:
                nAnim = 0x175;
                break;
            }
            break;
        case 0x283c:
            switch (nType) {
            case 4:
                nAnim = 0xc0;
                break;
            case 3:
                nAnim = 0x97;
                break;
            case 2:
                nAnim = 0x6d;
                break;
            }
            break;
        case 0x27fd:
        case 0x27fe:
            nAnim = 0x17;
            break;
        case 0x281f:
            nAnim = 0x17f;
            break;
        case 0x283d:
            switch (nType) {
            case 2:
                nAnim = 0x6e;
                break;
            case 3:
                nAnim = 0x97;
                break;
            case 4:
                nAnim = 0xc0;
                break;
            }
            break;
        case 0x28a1:
            nAnim = 0x73;
            break;
        case 0x28a0:
            nAnim = 0x54;
            break;
        case 0x283e:
            switch (nType) {
            case 2:
                nAnim = 0x7c;
                break;
            case 4:
                nAnim = 0xa5;
                break;
            case 3:
                nAnim = 0xce;
                break;
            case 5:
                nAnim = 0xdc;
                break;
            case 6:
                nAnim = 0xea;
                break;
            case 8:
                nAnim = 0x118;
                break;
            case 7:
            case 9:
                nAnim = 0xf2;
                break;
            }
            break;
        case 0x28a2:
            nAnim = 0x54;
            break;
        case 0x2af9:
            nAnim = 0x188;
            break;
        case 0x2af8:
            nAnim = 0x42;
            break;
        case 0x2afa:
            nAnim = 0x189;
            break;
        }
    }
    else {
        switch (nAnim) {
        case 0x272c:
            nAnim = 0x107;
            break;
        case 0x2727:
            nAnim = 0x10c;
            break;
        case 0x2736:
        case 0x2737:
        case 0x2738:
        case 0x2739:
        case 0x273a:
            nAnim = 0x105;
            break;
        case 0x271f:
        case 0x2720:
            nAnim = 0x17b;
            break;
        case 0x273c:
            nAnim = 0x104;
            break;
        case 0x2714:
        case 0x276e:
            nAnim = m_nCC ? 0xfd : 0xff;
            break;
        case 0x276d:
            nAnim = 0xfe;
            break;
        case 0x2744:
            nAnim = 0x101;
            break;
        case 0x2746:
            nAnim = 0x103;
            break;
        case 0x2745:
            nAnim = 0x102;
            break;
        case 0x2785:
            nAnim = 0x10a;
            break;
        case 0x2712:
        case 0x2713:
        case 0x2764:
        case 0x2795:
            nAnim = 0xfd;
            break;
        case 0x2799:
            nAnim = 0x10b;
            break;
        case 0x279a:
            nAnim = 0x10d;
            break;
        case 0x275a:
            nAnim = 0x12b;
            break;
        case 0x27a3:
            nAnim = 0x166;
            break;
        case 0x2710:
        case 0x2747:
        case 0x2748:
        case 0x2749:
        case 0x274a:
        case 0x276c:
        case 0x27a5:
            nAnim = 0x100;
            break;
        case 0x27a6:
            nAnim = 0x108;
            break;
        case 0x2711:
        case 0x27a4:
            nAnim = 0x116;
            break;
        case 0x2716:
        case 0x2718:
        case 0x27ac:
            nAnim = 0x113;
            break;
        case 0x27aa:
            nAnim = 0x174;
            break;
        case 0x27a8:
            nAnim = 0x171;
            break;
        case 0x27eb:
        case 0x27ec:
            nAnim = 0x110;
            break;
        case 0x2806:
            switch (nType) {
            case 1:
                nAnim = 0x5b;
                break;
            case 2:
                nAnim = 0x84;
                break;
            case 3:
                nAnim = 0xad;
                break;
            case 4:
                nAnim = 0xd6;
                break;
            case 5:
                nAnim = 0xde;
                break;
            case 6:
                nAnim = 0xec;
                break;
            case 7:
                nAnim = 0xf4;
                break;
            case 9:
                nAnim = 0xf4;
                break;
            }
            break;
        case 0x27ed:
        case 0x27ee:
            nAnim = 0x112;
            break;
        case 0x27ef:
        case 0x27f0:
            nAnim = 0x111;
            break;
        case 0x2af8:
            nAnim = 0x17c;
            break;
        case 0x28a0:
        case 0x28a2:
            nAnim = 0x10f;
            break;
        case 0x2afa:
            nAnim = 0x189;
            break;
        case 0x271c:
            nAnim = 0x12d;
            break;
        case 0x271b:
            nAnim = 0x12e;
            break;
        case 0x272e:
        case 0x27ab:
            nAnim = 0x12;
            break;
        case 0x278d:
            nAnim = 0x2b;
            break;
        case 0x2786:
            nAnim = 0x10e;
            break;
        case 0x278c:
            nAnim = 0x4a;
            break;
        case 0x2719:
        case 0x277d:
            nAnim = 0x12c;
            break;
        case 0x279e:
            nAnim = 0x15a;
            break;
        case 0x28a1:
            nAnim = 0x73;
            break;
        case 0x271e:
        case 0x275d:
        case 0x283e:
            nAnim = 0x118;
            break;
        case 0x2af9:
            nAnim = 0x188;
            break;
        }
    }
    return nAnim;
}

// Vtable slot 0xe0. Not in the function table (inside 0x0069F630's span).
// FUNCTION: SWKOTOR 0x0069F650
unsigned short CSWCAnimBase::ServerToClientAnimation(unsigned short nAnim)
{
    return MapAnimation(m_nC6, nAnim, m_nC5);
}
