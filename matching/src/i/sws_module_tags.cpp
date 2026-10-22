// CSWSModule name helpers, module-info message writer and the tag
// lookup table queries (0x004C3060-0x004C61AC).
#include <string.h>

#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C3060
void CSWSModule::SetLowerCaseName(const CExoString& sName)
{
    m_sLowerCaseName = sName.LowerCase();
}

// FUNCTION: SWKOTOR 0x004C3170
void CSWSModule::WriteModuleInfo(void* pUnused)
{
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    {
        CExoString sName(strchr(m_sModuleName.CStr(), ':') + 1);
        pMessage->WriteCExoString(sName, 0x20);
    }
    pMessage->WriteCExoLocStringServer(m_cName, 0);
    pMessage->WriteBYTE(m_n1a0, 8);
    pMessage->WriteCResRef(m_cResRef6c, 0x10);
    pMessage->WriteCResRef(m_cResRef7c, 0x10);
}

// FUNCTION: SWKOTOR 0x004C4B80
CExoString CSWSModule::GetModuleShortName()
{
    CExoString sName;
    sName = m_sModuleName;
    int nPos;
    while ((nPos = sName.Find(':', 0)) != -1) {
        sName = sName.Right(sName.GetLength() - nPos - 1);
    }
    return sName;
}

// DelIndex stays a call to the out-of-line copy at 0x004C4E90.
#pragma inline_depth(0)
// FUNCTION: SWKOTOR 0x004C5EF0
int CSWSModule::RemoveObjectFromLookupTable(const CExoString& sTag, unsigned long oid)
{
    CExoString sKey = sTag;
    int i = FindTagNode(sKey.CStr());
    for (; i < m_aTagNodes.num; i++) {
        if (m_aTagNodes.element[i].m_n24 == oid) {
            break;
        }
    }
    if (i < m_aTagNodes.num && sKey == m_aTagNodes.element[i].m_szTag) {
        m_aTagNodes.DelIndex(i);
        return 1;
    }
    return 0;
}
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x004C5FE0
unsigned long CSWSModule::GetObjectByTag(const CExoString& sTag, int nNth)
{
    CExoString sKey = sTag;
    int i = FindTagNode(sKey.CStr()) + nNth;
    if (i < m_aTagNodes.num && sKey == m_aTagNodes.element[i].m_szTag) {
        return m_aTagNodes.element[i].m_n24;
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004C60B0
unsigned long CSWSModule::GetObjectByTagAndType(const CExoString& sTag, int nType, int nNth)
{
    CExoString sKey = sTag.LowerCase();
    CGameObjectArray* pArray = NULL;
    CGameObject* pObject;
    int i = FindTagNode(sKey.CStr());
    int nCount = nNth;
    for (; i < m_aTagNodes.num; i++) {
        if (!(sKey == m_aTagNodes.element[i].m_szTag)) {
            break;
        }
        if (pArray == NULL) {
            pArray = GetServerExoApp()->GetObjectArray();
        }
        if (pArray->GetGameObject(m_aTagNodes.element[i].m_n24, &pObject) == GAME_OBJECT_VALID &&
            nType == pObject->m_nObjectType) {
            if (nCount == 0) {
                return m_aTagNodes.element[i].m_n24;
            }
            nCount--;
        }
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004C7DE0
int CSWSModule::AddObjectToLookupTable(const CExoString& sTag, unsigned long oid)
{
    CExoString sKey = sTag;
    int i = FindTagNode(sKey.CStr());
    CSWSTagNode cNode;
    cNode.m_n24 = oid;
    strcpy(cNode.m_szTag, sKey.CStr());
    for (; i < m_aTagNodes.num; i++) {
        if (!(sTag == m_aTagNodes.element[i].m_szTag)) {
            break;
        }
    }
    m_aTagNodes.Insert(cNode, i);
    return 1;
}

// FUNCTION: SWKOTOR 0x004C7ED0
void CSWSModule::AddWorldJournalEntry(unsigned long nID, const CExoString& sName,
                                      const CExoString& sText, unsigned long n10,
                                      unsigned long n14)
{
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    CExoLinkedListPosition pos;
    CWorldJournalEntry cEntry;
    cEntry.m_nID = nID;
    cEntry.m_sName = sName;
    cEntry.m_sText = sText;
    cEntry.m_n10 = n10;
    cEntry.m_n14 = n14;
    cEntry.m_bStrref = 0;
    m_aWorldJournal.Add(cEntry);
    if (pMessage) {
        CExoLinkedList<CSWSPlayer>* pList = GetServerExoApp()->GetPlayerList();
        if (pList) {
            pos = pList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pPlayer = pList->GetAtPos(pos);
                if (pPlayer && pPlayer->GetGameObject()) {
                    pMessage->SendServerToPlayerJournalAddWorld(pPlayer, nID, sName, sText, n10,
                                                                n14);
                }
                pList->GetNext(pos);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004C8030
void CSWSModule::AddWorldJournalEntryStrref(unsigned long nStrref, unsigned long n18,
                                            unsigned long n10, unsigned long n14)
{
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    CExoLinkedListPosition pos;
    CWorldJournalEntry cEntry;
    cEntry.m_nID = nStrref;
    cEntry.m_n18 = n18;
    cEntry.m_n10 = n10;
    cEntry.m_n14 = n14;
    cEntry.m_bStrref = 1;
    m_aWorldJournal.Add(cEntry);
    if (pMessage) {
        CExoLinkedList<CSWSPlayer>* pList = GetServerExoApp()->GetPlayerList();
        if (pList) {
            pos = pList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pPlayer = pList->GetAtPos(pos);
                if (pPlayer && pPlayer->GetGameObject()) {
                    pMessage->SendServerToPlayerJournalAddWorldStrref(pPlayer, nStrref, n18, n10,
                                                                      n14);
                }
                pList->GetNext(pos);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004C3B10
int CSWSModule::LoadTemporaryArea()
{
    CExoString sGitName;
    CExoString sAreaName;
    CExoString sTemp;
    if (m_pTempAreaData == NULL) {
        return 0;
    }
    sAreaName = "tmparea";
    sTemp = "tmpgit";
    CSWSArea* pArea = GetServerExoApp()->GetAreaByGameObjectID(m_oidArea);
    sGitName = sTemp;
    pArea->LoadAreaFromData(m_pTempAreaData, sGitName, m_nTempAreaSize);
    g_pAppManager->m_p14->m_n8 = 1;
    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerLoadBar_UpdateStallEvent(
        1, g_pAppManager->m_p14->m_nC);
    return 1;
}
