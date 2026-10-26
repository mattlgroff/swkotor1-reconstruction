// Pazaak rules objects (swpazaak.o, 0x006E4360-0x006E4C60): CPazaakPlayer
// totals and card handling, CSWPazaak deck, side-deck and AI turn logic.
// Names follow symbols.csv / the Mac maps; ShuffleDeck, DetermineWinner and
// FindPotentialSidecardTotal are descriptive.

#include "l/sw_pazaak.h"

// FUNCTION: SWKOTOR 0x006E4360
int CPazaakPlayer::GetTotal()
{
    int nTotal = 0;
    for (int i = 0; i < 9; i++) {
        int nValue = m_aBoard[i].m_nValue;
        if (nValue == -1) {
            break;
        }
        int nCard;
        if (nValue < 0) {
            nCard = 0;
        } else if (nValue >= 12 && nValue < 18 && m_aBoard[i].m_bFlag) {
            nCard = -g_nPazaakCardValues[nValue];
        } else {
            nCard = g_nPazaakCardValues[nValue];
        }
        nTotal += nCard;
    }
    return nTotal;
}

// FUNCTION: SWKOTOR 0x006E4580
void CPazaakPlayer::ChooseSidedeck(CPazaakCard* pCards)
{
    int nLeft = 10;
    for (int i = 0; i < 4; i++) {
        int nPick = rand() % nLeft;
        m_aHand[i] = pCards[nPick];
        pCards[nPick] = pCards[nLeft - 1];
        pCards[nLeft - 1] = CPazaakCard(-1, 0);
        nLeft--;
    }
}

// FUNCTION: SWKOTOR 0x006E45F0
void CPazaakPlayer::AddGameCard(CPazaakCard cCard)
{
    int i;
    for (i = 0; i < 9; i++) {
        if (m_aBoard[i].m_nValue == -1) {
            m_aBoard[i] = cCard;
            break;
        }
    }
    if (i >= 8 || GetTotal() == 20) {
        m_bStanding = 1;
    }
}

// FUNCTION: SWKOTOR 0x006E4640
void CSWPazaak::ClearGameBoard()
{
    for (int i = 0; i < 9; i++) {
        m_cPlayer.m_aBoard[i] = CPazaakCard(-1, 0);
        m_cAI.m_aBoard[i] = CPazaakCard(-1, 0);
    }
    m_cPlayer.m_bStanding = 0;
    m_cAI.m_bStanding = 0;
}

// FUNCTION: SWKOTOR 0x006E4730
void CSWPazaak::ShuffleDeck()
{
    CPazaakCard aCards[40];
    int i;
    for (i = 0; i < 40; i++) {
        aCards[i] = CPazaakCard(i % 10 + 18, 0);
    }
    int nLeft = 40;
    for (i = 0; i < 40; i++) {
        int nPick = rand() % nLeft;
        m_aDeck[i] = aCards[nPick];
        aCards[nPick] = aCards[nLeft - 1];
        aCards[nLeft - 1] = CPazaakCard(-1, 0);
        nLeft--;
    }
    m_nDeckIndex = 39;
}

// FUNCTION: SWKOTOR 0x006E4800
CPazaakCard CSWPazaak::DrawCard()
{
    if (m_nDeckIndex < 0) {
        ShuffleDeck();
    }
    return m_aDeck[m_nDeckIndex--];
}

// FUNCTION: SWKOTOR 0x006E4840
int CSWPazaak::CanUsePlayerSidedeckCard(int nIndex)
{
    return m_cPlayer.m_aHand[nIndex].m_nValue >= 0;
}

// FUNCTION: SWKOTOR 0x006E4860
int CSWPazaak::UsePlayerSidedeckCard(int nIndex)
{
    if (m_cPlayer.m_aHand[nIndex].m_nValue < 0) {
        return 0;
    }
    m_cPlayer.AddGameCard(m_cPlayer.m_aHand[nIndex]);
    m_cPlayer.m_aHand[nIndex] = CPazaakCard(-1, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x006E48A0
int CSWPazaak::DetermineWinner()
{
    int nPlayer = m_cPlayer.GetTotal();
    int nAI = m_cAI.GetTotal();
    if (nPlayer > 20 || nAI > 20) {
        m_cPlayer.m_bStanding = 1;
        m_cAI.m_bStanding = 1;
    }
    if (!m_cPlayer.m_bStanding || !m_cAI.m_bStanding) {
        return 0;
    }
    if (nPlayer > 20) {
        nPlayer = -1000000;
    }
    if (nAI > 20) {
        nAI = -1000000;
    }
    if (nPlayer > nAI) {
        return 1;
    }
    if (nPlayer < nAI) {
        return 2;
    }
    return 3;
}

// FUNCTION: SWKOTOR 0x006E4C00
CSWPazaak::CSWPazaak()
{
    m_nWager = 0;
    m_oidOpponent = 0x7f000000;
    m_nDeckIndex = -1;
    m_nAIState = 0;
}

// FUNCTION: SWKOTOR 0x006E4B50
CPazaakPlayer::CPazaakPlayer()
{
    m_bStanding = 0;
    m_nSetsWon = 0;
    int i;
    for (i = 0; i < 4; i++) {
        m_aHand[i] = CPazaakCard(-1, 0);
    }
    for (i = 0; i < 9; i++) {
        m_aBoard[i] = CPazaakCard(-1, 0);
    }
}

static inline int GetCardValue(const CPazaakCard& cCard)
{
    if (cCard.m_nValue < 0) {
        return 0;
    }
    if (cCard.m_nValue >= 12 && cCard.m_nValue < 18 && cCard.m_bFlag) {
        return -g_nPazaakCardValues[cCard.m_nValue];
    }
    return g_nPazaakCardValues[cCard.m_nValue];
}

// FUNCTION: SWKOTOR 0x006E4910
int CSWPazaak::FindPotentialSidecardTotal(CPazaakPlayer* pPlayer, int* pIndex, int bLower)
{
    int nBase = pPlayer->GetTotal();
    int nBest = nBase;
    if (nBest > 20) {
        nBest = -1000000;
    }
    int nBestIndex = -1;
    int bBestFlag = 0;
    int bBestFlip = 0;
    for (int i = 0; i < 4; i++) {
        int nValue = pPlayer->m_aHand[i].m_nValue;
        int nPasses = 2;
        do {
            int bFlag = 0;
            if (nValue >= 12 && nValue < 18) {
                bFlag = nPasses == 2;
                pPlayer->m_aHand[i].m_bFlag = bFlag;
            } else {
                nPasses = 1;
            }
            int nTotal = GetCardValue(pPlayer->m_aHand[i]) + nBase;
            if (nTotal > 20) {
                nTotal = -1000000;
            }
            if ((!bLower && nTotal > nBest) || (bLower && nTotal < nBest && nTotal > -1000000)
                || (nTotal == nBest && bBestFlip)) {
                nBest = nTotal;
                nBestIndex = i;
                bBestFlag = bFlag;
                if (nValue >= 12 && nValue < 18) {
                    bBestFlip = 1;
                } else {
                    bBestFlip = 0;
                }
            }
        } while (--nPasses);
    }
    if (nBestIndex >= 0) {
        *pIndex = nBestIndex;
        pPlayer->m_aHand[nBestIndex].m_bFlag = bBestFlag;
    }
    return nBest;
}

// FUNCTION: SWKOTOR 0x006E4A40
void CSWPazaak::LoadInitialSideDeck(int nDeck, CPazaakCard* pCards)
{
    C2DA c2da(CResRef("PazaakDecks"), 0);
    if (!c2da.Load2DArray()) {
        return;
    }
    CExoString sCard;
    if (nDeck < 0) {
        int nRows = c2da.m_nNumRows;
        nDeck = rand() % nRows;
    }
    for (int i = 0; i < 10; i++) {
        c2da.GetCExoStringEntry(nDeck, i, &sCard);
        int nValue = sCard[1] - '1';
        switch (sCard[0]) {
        case '*':
            nValue += 12;
            break;
        case '-':
            nValue += 6;
            break;
        }
        pCards[i] = CPazaakCard(nValue, 0);
    }
    c2da.Unload2DArray();
}

// FUNCTION: SWKOTOR 0x006E4C60
int CSWPazaak::PlayAITurn(CPazaakPlayer* pAI, CPazaakPlayer* pPlayer, int nState)
{
    int nIndex;
    int nPlayerTotal = pPlayer->GetTotal();
    if (nPlayerTotal > 20) {
        nPlayerTotal = -1000000;
        pPlayer->m_bStanding = 1;
    }
    switch (nState) {
    case 3:
    case 4:
        return 0;
    case 0:
        if (pAI->m_bStanding) {
            return 0;
        }
        pAI->AddGameCard(DrawCard());
        return 1;
    case 1: {
        int nAITotal = pAI->GetTotal();
        int bMayWait = 1;
        if (pPlayer->m_nSetsWon + 1 >= 3) {
            bMayWait = 0;
        }
        if (pPlayer->m_bStanding && nAITotal > nPlayerTotal && nAITotal <= 20) {
            pAI->m_bStanding = 1;
            return 3;
        }
        int nLowest = FindPotentialSidecardTotal(pAI, &nIndex, 1);
        nIndex = -1;
        int nHighest = FindPotentialSidecardTotal(pAI, &nIndex, 0);
        if (nHighest >= 18 && nHighest >= nPlayerTotal
            && (nHighest >= 20 || (nHighest >= 19 && nLowest > 11) || nLowest > 13) && nIndex >= 0) {
            pAI->AddGameCard(pAI->m_aHand[nIndex]);
            pAI->m_aHand[nIndex] = CPazaakCard(-1, 0);
            return 2;
        }
        if (nAITotal > 20) {
            if (bMayWait && nPlayerTotal >= 18 && nHighest < nPlayerTotal) {
                return 4;
            }
            if (nIndex >= 0) {
                pAI->AddGameCard(pAI->m_aHand[nIndex]);
                pAI->m_aHand[nIndex] = CPazaakCard(-1, 0);
                return 2;
            }
        }
    }
    case 2: {
        int nTotal = pAI->GetTotal();
        if (nTotal > 20) {
            nTotal = -1000000;
        }
        if (pPlayer->m_bStanding && nTotal < nPlayerTotal) {
            return 4;
        }
        if (nTotal >= 18 || FindPotentialSidecardTotal(pAI, &nIndex, 1) >= 16) {
            pAI->m_bStanding = 1;
            return 3;
        }
        return 4;
    }
    }
    return 4;
}
