// pazaak_card.h -- CPazaakCard
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_gui_pazaak.h); the other copies' members fall on its members.
#ifndef SHARED_PAZAAK_CARD_H
#define SHARED_PAZAAK_CARD_H


struct CPazaakCard {
    CPazaakCard() { m_bFlag = 0; }
    CPazaakCard(int nValue, int bFlag)
    {
        m_nValue = nValue;
        m_bFlag = bFlag;
    }
    int operator!=(CPazaakCard cCard) { return m_nValue != cCard.m_nValue || m_bFlag != cCard.m_bFlag; }

    int m_nValue;
    int m_bFlag;
};

#endif
