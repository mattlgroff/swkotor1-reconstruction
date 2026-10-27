// CSWSJournal::SetDate / SetTime (b005c_f2_journal_store): record the date or
// time of a quest entry and queue the matching player journal update.
// Own copy of the SJournalEntry layout (journal.cpp keeps the out-of-line
// members; here the empty destructor is inline, as in the original callers).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    int CompareNoCase(const CExoString &s) const;
    CExoString LowerCase() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    CExoLocString(const CExoLocString &s);
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    void *m_pPad;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    void Add(T t);
    void AddAndDispose(T t);
    void DelIndex(int nIndex);
};

struct SJournalEntry {
    CExoLocString m_sName;           // 0x00
    CExoLocString m_sText;           // 0x08
    unsigned long m_nCalendarDay;    // 0x10
    unsigned long m_nTimeOfDay;      // 0x14
    CExoString m_sPlotID;            // 0x18
    int m_nState;                    // 0x20
    int m_nPriority;                 // 0x24
    unsigned short m_nPictureIndex;  // 0x28
    unsigned long m_bQuestCompleted : 1; // 0x2C bit 0
    unsigned long m_bQuestDisplayed : 1; // 0x2C bit 1
    unsigned long m_bUpdated : 1;        // 0x2C bit 2
    unsigned long m_nPlanetID;       // 0x30
    int m_n34;                       // 0x34
    unsigned long m_nQuestXP;        // 0x38

    SJournalEntry();
    SJournalEntry(const SJournalEntry &e);
    ~SJournalEntry() {}
};

struct CSWSPlayerJournalQuestUpdates {
    unsigned short m_nUpdateType;    // 0x00
    CExoString m_sPlotID;            // 0x04
};

class CSWSPlayerJournalInfo {
public:
    int m_b0;                        // 0x00 (placeholder name)
    CExoArrayList<CSWSPlayerJournalQuestUpdates> m_lstUpdates; // 0x04
    int m_b10;                       // 0x10 (placeholder name)
};

class CSWSPlayer {
public:
    char pad_0x0[0x4c];
    CSWSPlayerJournalInfo *m_pJournalInfo; // 0x4c
};

class CSWSMessage {
public:
    int SendServerToPlayerJournalRemoveQuest(CSWSPlayer *pPlayer, const CExoString &sPlotID);
    int SendServerToPlayerJournalUpdated(CSWSPlayer *pPlayer, int bFlag, int bCompleted,
                                         const CExoLocString &sName);
};

class CServerExoApp {
public:
    CSWSMessage *GetNWSMessage();
    unsigned long GetPlayerCharacterID();
    CSWSPlayer *GetClientObjectByObjectID(unsigned long oid);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // 0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSJournal {
public:
    CExoArrayList<SJournalEntry> m_lstEntries;   // 0x00

    void SetDate(const CExoString &sPlotID, unsigned long nDate, int bFlag);
    void SetTime(const CExoString &sPlotID, unsigned long nTime, int bFlag);
    void Destroy(const CExoString &sPlotID);
};

// FUNCTION: SWKOTOR 0x005C63F0
void CSWSJournal::SetDate(const CExoString &sPlotID, unsigned long nDate, int bFlag)
{
    SJournalEntry cEntry;
    CSWSPlayerJournalQuestUpdates cUpdate;
    int bFound = 0;
    int nEntries = m_lstEntries.num;
    for (int i = 0; i < nEntries; i++) {
        if (m_lstEntries.element[i].m_sPlotID.CompareNoCase(sPlotID)) {
            m_lstEntries.element[i].m_nCalendarDay = nDate;
            bFound = 1;
            break;
        }
    }
    if (!bFound) {
        cEntry.m_sPlotID = sPlotID.LowerCase();
        cEntry.m_nCalendarDay = nDate;
        m_lstEntries.AddAndDispose(cEntry);
    }
    CServerExoApp *pServer = g_pAppManager->GetServerExoApp();
    CSWSPlayer *pPlayer = pServer->GetClientObjectByObjectID(pServer->GetPlayerCharacterID());
    if (pPlayer && !pPlayer->m_pJournalInfo->m_b10) {
        CExoArrayList<CSWSPlayerJournalQuestUpdates> *pUpdates =
            &pPlayer->m_pJournalInfo->m_lstUpdates;
        if (pUpdates) {
            int nUpdates = pUpdates->num;
            int bUpdateFound = 0;
            for (int k = 0; k < nUpdates; k++) {
                if (pUpdates->element[k].m_sPlotID.CompareNoCase(sPlotID)) {
                    if (!bFound) {
                        pUpdates->element[k].m_nUpdateType = 0x8000;
                    } else {
                        pUpdates->element[k].m_nUpdateType &= ~0x4000;
                        pUpdates->element[k].m_nUpdateType |= 0x4;
                    }
                    if (bFlag)
                        pUpdates->element[k].m_nUpdateType |= 0x100;
                    bUpdateFound = 1;
                    break;
                }
            }
            if (!bUpdateFound) {
                cUpdate.m_sPlotID = sPlotID.LowerCase();
                cUpdate.m_nUpdateType = bFound ? 0x4 : 0x8000;
                if (bFlag)
                    cUpdate.m_nUpdateType |= 0x100;
                pUpdates->Add(cUpdate);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005C6650
void CSWSJournal::SetTime(const CExoString &sPlotID, unsigned long nTime, int bFlag)
{
    SJournalEntry cEntry;
    CSWSPlayerJournalQuestUpdates cUpdate;
    int bFound = 0;
    int nEntries = m_lstEntries.num;
    for (int i = 0; i < nEntries; i++) {
        if (m_lstEntries.element[i].m_sPlotID.CompareNoCase(sPlotID)) {
            m_lstEntries.element[i].m_nTimeOfDay = nTime;
            bFound = 1;
            break;
        }
    }
    if (!bFound) {
        cEntry.m_sPlotID = sPlotID.LowerCase();
        cEntry.m_nTimeOfDay = nTime;
        m_lstEntries.AddAndDispose(cEntry);
    }
    CServerExoApp *pServer = g_pAppManager->GetServerExoApp();
    CSWSPlayer *pPlayer = pServer->GetClientObjectByObjectID(pServer->GetPlayerCharacterID());
    if (pPlayer && !pPlayer->m_pJournalInfo->m_b10) {
        CExoArrayList<CSWSPlayerJournalQuestUpdates> *pUpdates =
            &pPlayer->m_pJournalInfo->m_lstUpdates;
        if (pUpdates) {
            int nUpdates = pUpdates->num;
            int bUpdateFound = 0;
            for (int k = 0; k < nUpdates; k++) {
                if (pUpdates->element[k].m_sPlotID.CompareNoCase(sPlotID)) {
                    if (!bFound) {
                        pUpdates->element[k].m_nUpdateType = 0x8000;
                    } else {
                        pUpdates->element[k].m_nUpdateType &= ~0x4000;
                        pUpdates->element[k].m_nUpdateType |= 0x8;
                    }
                    if (bFlag)
                        pUpdates->element[k].m_nUpdateType |= 0x100;
                    bUpdateFound = 1;
                    break;
                }
            }
            if (!bUpdateFound) {
                cUpdate.m_sPlotID = sPlotID.LowerCase();
                cUpdate.m_nUpdateType = bFound ? 0x8 : 0x8000;
                if (bFlag)
                    cUpdate.m_nUpdateType |= 0x100;
                pUpdates->Add(cUpdate);
            }
        }
    }
}

// NONMATCH (8): the frame is 4 bytes larger - the two LowerCase()
// temporaries and the update count do not share one slot as in the original;
// loop-shape and branch variants tried.
// FUNCTION: SWKOTOR 0x005C68B0
void CSWSJournal::Destroy(const CExoString &sPlotID)
{
    int nEntries = m_lstEntries.num;
    CSWSMessage *pMessage = g_pAppManager->GetServerExoApp()->GetNWSMessage();
    CSWSPlayerJournalQuestUpdates cUpdate;
    CExoLocString sName;
    int i;
    int bFound = 0;
    for (i = nEntries - 1; i >= 0; i--) {
        if (m_lstEntries.element[i].m_sPlotID.CompareNoCase(sPlotID)) {
            bFound = 1;
            break;
        }
    }
    if (!bFound)
        return;
    int bCompleted = m_lstEntries.element[i].m_bQuestCompleted;
    sName = m_lstEntries.element[i].m_sName;
    m_lstEntries.DelIndex(i);
    CServerExoApp *pServer = g_pAppManager->m_pServerExoApp;
    CSWSPlayer *pPlayer = pServer->GetClientObjectByObjectID(pServer->GetPlayerCharacterID());
    if (!pPlayer)
        return;
    if (pPlayer->m_pJournalInfo->m_b0) {
        pMessage->SendServerToPlayerJournalRemoveQuest(pPlayer, sPlotID.LowerCase());
        return;
    }
    if (!pPlayer->m_pJournalInfo->m_b10) {
        CExoArrayList<CSWSPlayerJournalQuestUpdates> *pUpdates =
            &pPlayer->m_pJournalInfo->m_lstUpdates;
        if (pUpdates) {
            int nUpdates = pUpdates->num;
            int bUpdateFound = 0;
            for (int k = 0; k < nUpdates; k++) {
                if (pUpdates->element[k].m_sPlotID.CompareNoCase(sPlotID)) {
                    pUpdates->element[k].m_nUpdateType = 0x4000;
                    bUpdateFound = 1;
                    break;
                }
            }
            if (!bUpdateFound) {
                cUpdate.m_nUpdateType = 0x4000;
                cUpdate.m_sPlotID = sPlotID.LowerCase();
                pUpdates->Add(cUpdate);
            }
        }
    }
    pMessage->SendServerToPlayerJournalUpdated(pPlayer, 1, bCompleted, sName);
}
