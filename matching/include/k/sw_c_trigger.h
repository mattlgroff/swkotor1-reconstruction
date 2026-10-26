// CSWCTrigger (client trigger, vtable 0x00754440).
#ifndef K_SW_C_TRIGGER_H
#define K_SW_C_TRIGGER_H

#include "k/sw_c_client_object.h"

class CSWCAnimBaseTrigger : public CSWCAnimBase {
public:
    CSWCAnimBaseTrigger();                                           // 0x006E5F40
    char pad_0x4[0xf0 - 0x4];
};

class CSWCTrigger : public CSWCObject {
public:
    CSWCTrigger();
    virtual ~CSWCTrigger();

    int m_nFC;                                                       // +0xFC
    CExoString m_sName;                                              // +0x100
    unsigned long m_bAreaTransition;                                 // +0x108
    unsigned long m_bHostile;                                        // +0x10C
    unsigned long m_bDisplayed;                                      // +0x110
    unsigned long m_bTransitionMode;                                 // +0x114
    float m_fHighlightWidth;                                         // +0x118
    char pad_0x11c[0x12c - 0x11c];
    unsigned short m_nCursor;                                        // +0x12C
    char pad_0x12e[2];
    int m_nVertices;                                                 // +0x130
    Vector* m_pVertices;                                             // +0x134
    PartTrigger* m_pAurPartTrigger;                                  // +0x138
    int m_n13C;                                                      // +0x13C
    CExoString m_sTransitionTag;                                     // +0x140
    unsigned char m_n148;                                            // +0x148
    char pad_0x149[3];

    int LoadModel(const CResRef&, unsigned char, int);               // 0x006910D0
    CExoString GetName();                                            // 0x006912B0
    unsigned long GetTransitionMode();                               // 0x006912E0, descriptive
    void PlayAnimation(unsigned long, void*, int);                   // 0x006911B0
    void CalculatePosition();                                        // 0x00691560, descriptive
    int GetScreenPosition(float*);                                   // 0x006916B0, descriptive
    void UpdateAnimation();                                          // 0x00691970, descriptive
    int IntersectSegment(const Vector&, const Vector&, Vector*);     // 0x00691A30, descriptive
    void AddToRenderList(int);                                       // 0x00691CC0, descriptive
    void RemoveFromRenderList();                                     // 0x00691D80
    void UpdateTriggerColor(int);                                    // 0x006913B0
    void RefreshTriggerColor(int);                                   // 0x00691E00, descriptive
    float GetHighlightWidth() { return m_fHighlightWidth; }
};

typedef char KTriggerSize[(sizeof(CSWCTrigger) == 0x14c) ? 1 : -1];

#endif
