// Compact CSWCTrigger display state and cursor leaves (binary offsets corroborated
// by the client trigger update and display call sites).

#include "k/sw_gui_in_game_credits.h"

class CSWCTrigger {
public:
    char pad_0x0[0x108];
    unsigned long m_bAreaTransition;               // 0x108
    unsigned long m_bHostile;                      // 0x10c
    unsigned long m_bDisplayed;                     // 0x110
    unsigned long m_bTransitionMode;               // 0x114
    float m_fHighlightWidth;                       // 0x118
    char pad_0x11c[0x10];
    unsigned short m_nCursor;                      // 0x12c
    char pad_0x12e[0x0a];
    void* m_pAurPartTrigger;                       // 0x138

    void UpdateTriggerColor(int nUnused);          // 0x006913B0
    float GetHighlightWidth() { return m_fHighlightWidth; }
    void DisplayTrigger(int bDisplay);             // 0x00691470
    void SetCursor(unsigned char nRow);            // 0x006914D0
};

void AurPartTriggerSetColor(void* pPart, float* pColor, float fAlpha,
                            unsigned char bC4, float fC8, unsigned char bCC); // 0x0049D9B0
void AurPartTriggerSetHighlight(void* pPart, unsigned char bHighlight,
                                float fWidth, unsigned char bFlag); // 0x0049E610

// FUNCTION: SWKOTOR 0x006913B0
void CSWCTrigger::UpdateTriggerColor(int nUnused)
{
    if (m_bAreaTransition != 0) {
        return;
    }
    float color[3];
    if (m_bHostile != 0) {
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 1.0f;
        if (m_bDisplayed != 0)
            AurPartTriggerSetColor(m_pAurPartTrigger, color, 0.25f, 0, 0.0f, 0);
        else
            AurPartTriggerSetColor(m_pAurPartTrigger, color, 0.0f, 0, 0.0f, 0);
    } else {
        color[2] = 1.0f;
        color[1] = 1.0f;
        color[0] = 0.3f;
        if (m_bDisplayed != 0)
            AurPartTriggerSetColor(m_pAurPartTrigger, color, 0.25f, 0, 0.0f, 0);
        else
            AurPartTriggerSetColor(m_pAurPartTrigger, color, 0.0f, 0, 0.0f, 0);
    }
    AurPartTriggerSetHighlight(m_pAurPartTrigger, 1, GetHighlightWidth(), 1);
}

// FUNCTION: SWKOTOR 0x00691470
void CSWCTrigger::DisplayTrigger(int bDisplay)
{
    if ((unsigned long)bDisplay == m_bDisplayed) {
        return;
    }
    m_bDisplayed = (unsigned long)bDisplay;
    if (m_bAreaTransition != 0) {
        if (m_bTransitionMode != 0) {
            UpdateTriggerColor(0);
            return;
        }
        UpdateTriggerColor(1);
        return;
    }
    if (m_bHostile != 0) {
        UpdateTriggerColor(2);
        return;
    }
    UpdateTriggerColor(1);
}

class C2DAs {
public:
    char pad_0x0[0x4c];
    C2DA* m_pCursors;                              // +0x4C
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DAs* m_p2DArrays;                            // +0xB8
};

extern CSWRules* g_pRules;                         // 0x007A3A28

// FUNCTION: SWKOTOR 0x006914D0
void CSWCTrigger::SetCursor(unsigned char nRow)
{
    int value;
    int found;
    {
        CExoString column("CursorID");
        found = g_pRules->m_p2DArrays->m_pCursors->GetINTEntry(nRow, column, &value);
    }
    if (found)
        m_nCursor = (unsigned short)value;
}
