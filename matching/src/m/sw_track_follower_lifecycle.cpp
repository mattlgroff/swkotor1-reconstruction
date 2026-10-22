// CSWTrackFollower constructor / destructor. No auto_inline(off) here: the
// member list constructors are only inlined without that pragma.

#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x0066DEE0
// NONMATCH (7 diff insns): store scheduling in the body; the original
// writes m_pMiniGame / m_pModel / m_nFlags first, ours sinks them (source
// order and bitfield flags have no effect).
CSWTrackFollower::CSWTrackFollower(CSWCMiniGame* pGame, int nType)
    : CSWMiniGameObject(nType)
{
    m_pMiniGame = pGame;
    m_pModel = 0;
    m_bTrackActive = 0;
    m_bDead = 0;
    m_f84 = 0.0f;
    m_n94 = 0;
    m_fA0 = 0.0f;
    m_f9C = 0.0f;
    m_n19C = 0;
    m_fSpeed = 100.0f;
    m_nAxisMode = 1;
    m_n8C = 100;
    m_nTarget170 = 0xff;
    m_apSources[0] = 0;
}

// FUNCTION: SWKOTOR 0x0066E000
// NONMATCH (8 diff insns): the original stores m_lstHooked.num = 0 before
// the m_bDead test and enters the gun-bank loop with `lea ebp, [eax-1];
// cmp ebp, 0; jl` (ours: `dec ebp; js`); for and while forms tried.
CSWTrackFollower::~CSWTrackFollower()
{
    int i;
    int nCount = m_lstHooked.num;
    for (i = 0; i < nCount; i++) {
        if (m_lstHooked.element[i].m_pModel) {
            delete m_lstHooked.element[i].m_pModel;
        }
    }
    if (m_lstHooked.element) {
        delete[] m_lstHooked.element;
        m_lstHooked.element = 0;
        m_lstHooked.array_size = 0;
    }
    m_lstHooked.num = 0;
    if (!(m_nFlags & 2)) {
        for (i = m_lstGunBanks.num - 1; i >= 0; i--) {
            CSWMGGunBank* pBank = m_lstGunBanks.element[i];
            if (pBank) {
                delete pBank;
            }
        }
    }
    if (m_apSources[0]) {
        m_apSources[0]->Stop();
        if (m_apSources[0]) {
            delete m_apSources[0];
        }
    }
    if (m_pModel) {
        delete m_pModel;
    }
}


// FUNCTION: SWKOTOR 0x0066EB50
CSWMiniPlayer::CSWMiniPlayer(CSWCMiniGame* pGame)
    : CSWTrackFollower(pGame, 1)
{
    m_nBank = 0;
    m_f1D8 = 100.0f;
    m_f1DC = 100.0f;
    m_f1E0 = 0.0f;
    m_b1D4 = 0;
    m_pHook = 0;
    m_lstEvents1A0.SetSize(4);
    m_lstEvents1AC.SetSize(4);
    m_lst1B8.SetSize(8);
    m_lstEvents1A0.num = 0;
    m_lstEvents1AC.num = 0;
}

// FUNCTION: SWKOTOR 0x0066ECF0
CSWMiniPlayer::~CSWMiniPlayer()
{
    if (m_pHook) {
        delete m_pHook;
    }
}

// FUNCTION: SWKOTOR 0x0066F7C0 SYMBOL: ??_GCSWTrackFollower@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0066F870 SYMBOL: ??_GCSWMiniPlayer@@UAEPAXI@Z
