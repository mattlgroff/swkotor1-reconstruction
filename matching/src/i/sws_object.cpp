// CSWSObject leaves (0x004CA9D0-0x004CFABE).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CA9D0
int CLoopingVisualEffect::GetIsBeam()
{
    return FUN_004ca960(m_nVisualEffect);
}

// FUNCTION: SWKOTOR 0x004CAFE0
unsigned long CSWSObject::FUN_004cafe0(unsigned long nMask)
{
    for (int i = 0; i < 15; i++) {
        if ((unsigned long)(1 << i) == nMask) {
            return m_p168[i];
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004CB120
CSWSArea* CSWSObject::GetArea()
{
    CGameObject* pObject = NULL;
    CGameObjectArray* pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
    pArray->GetGameObject(m_oidArea, &pObject);
    if (pObject) {
        return pObject->AsSWSArea();
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004CD1A0
void CSWSObject::SetPosition(const Vector& vPosition, int bUpdateArea, int bUnused,
                             int bNoUpdateFlag)
{
    if (m_vPosition == vPosition) {
        return;
    }
    if (bNoUpdateFlag == 0) {
        m_nUpdateFlags[0] |= 2;
    }
    CSWSArea* pArea = GetArea();
    m_vPosition = vPosition;
    if (bUpdateArea) {
        if (AsSWSCreature()) {
            AsSWSCreature();
        }
        if (pArea) {
            pArea->FUN_00506fe0(this);
        }
    }
}

// FUNCTION: SWKOTOR 0x004CB160
void CSWSObject::SetArea(CSWSArea* pArea)
{
    if (pArea) {
        m_oidArea = pArea->m_idSelf;
    } else {
        m_oidArea = OBJECT_INVALID_ID;
    }
}

// FUNCTION: SWKOTOR 0x004CB480
int CSWSObject::ReplyDialog(unsigned long nReply, int a, int b, int c, int d)
{
    if (m_pDialog == NULL) {
        return 0;
    }
    if (!m_pDialog->FUN_005a03c0(nReply, this, a, b, c, d) && m_pDialog) {
        m_pDialog->RunEndConversationScript(this, 0);
        m_n38 = 0;
        m_n3c = 0;
        m_n48 = 0;
        m_n214 = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004CB4F0
int CSWSObject::StopDialog()
{
    if (AsSWSCreature()) {
        AsSWSCreature()->FUN_004f34a0(OBJECT_INVALID_ID, 0);
    }
    if (m_oidDialogOwner == 0x7f000000) {
        return 0;
    }
    if (m_oidDialogOwner != m_idSelf) {
        CSWSObject* pOwner;
        if (GetServerExoApp()->GetObjectArray()->GetGameObject(
                m_oidDialogOwner, (CGameObject**)&pOwner) == GAME_OBJECT_VALID) {
            pOwner->RemoveObjectFromDialog(m_idSelf);
            m_oidDialogOwner = 0x7f000000;
        }
        return 1;
    }
    if (m_pDialog == NULL) {
        return 0;
    }
    m_pDialog->RunEndConversationScript(this, 0);
    m_n38 = 0;
    m_n3c = 0;
    m_n48 = 0;
    m_n50 = 0;
    delete m_pDialog;
    m_pDialog = NULL;
    return 1;
}

// FUNCTION: SWKOTOR 0x004CB5A0
void CSWSObject::StopSoundPlayingInDialog()
{
    m_bStopSoundPlaying = 1;
    g_pAppManager->m_pClientExoApp->StopSoundPlayingInDialog();
}

// FUNCTION: SWKOTOR 0x004CB7A0
void CSWSObject::SetDialogOwner(unsigned long oid)
{
    m_oidDialogOwner = oid;
}

// FUNCTION: SWKOTOR 0x004CB7B0
CScriptLocation CSWSObject::GetScriptLocation()
{
    CScriptLocation cLocation(m_vPosition, m_vOrientation);
    return cLocation;
}

// FUNCTION: SWKOTOR 0x004CB810
int CSWSObject::GetDead()
{
    if (AsSWSCreature() && AsSWSCreature()->m_b9d4) {
        return AsSWSCreature()->GetCurrentHitPoints(0) <= -10;
    }
    if (AsSWSCreature()) {
        return AsSWSCreature()->GetCurrentHitPoints(0) <= 0;
    }
    return GetCurrentHitPoints(0) <= 0;
}

// FUNCTION: SWKOTOR 0x004CC1C0
int CSWSObject::FUN_004cc1c0(unsigned short nId)
{
    if (nId == 0x2712 || nId == 0x276d || nId == 0x2795) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004CC1F0
void CSWSObject::SetUpdateFlag(unsigned long nBit)
{
    unsigned long nIndex = nBit / 32;
    m_nUpdateFlags[nIndex] |= 1 << (nBit - nIndex * 32);
}

// FUNCTION: SWKOTOR 0x004CC220
int CSWSObject::GetUpdateFlag(unsigned long nBit)
{
    unsigned long nIndex = nBit / 32;
    return (m_nUpdateFlags[nIndex] & (1 << (nBit - nIndex * 32))) != 0;
}

// FUNCTION: SWKOTOR 0x004CC250
void CSWSObject::ClearUpdateFlag(unsigned long nBit)
{
    unsigned long nIndex = nBit / 32;
    m_nUpdateFlags[nIndex] &= 0xffffffff - (1 << (nBit - nIndex * 32));
}

// FUNCTION: SWKOTOR 0x004CC280
void CSWSObject::SetCurrentHitPoints(int nHP)
{
    m_nCurrentHitPoints = nHP;
    if (m_bPlotObject && nHP < 1) {
        m_nCurrentHitPoints = 1;
    }
}

// FUNCTION: SWKOTOR 0x004CC2B0
CSWCObject* CSWSObject::GetClientObject()
{
    if (m_pClientObject == NULL) {
        CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
            g_pAppManager->m_pClientExoApp->ServerToClientObjectId(m_idSelf));
        if (pObject) {
            m_pClientObject = pObject->AsSWCObject();
            if (m_pClientObject) {
                m_pClientObject->FUN_0063d520(this);
            }
        }
    }
    return m_pClientObject;
}

// FUNCTION: SWKOTOR 0x004CC310
void CSWSObject::AttachmentFromClientObject(CSWCObject* p)
{
    m_pClientObject = p;
}

// FUNCTION: SWKOTOR 0x004CC320
void CSWSObject::DetachFromClientObject()
{
    m_pClientObject = NULL;
}

// FUNCTION: SWKOTOR 0x004CC330
void CSWSObject::ClearDialog()
{
    if (m_pDialog) {
        delete m_pDialog;
        m_pDialog = NULL;
    }
    m_n214 = 0;
}

// FUNCTION: SWKOTOR 0x004CC360
void CSWSObject::TimeStampCameraAnimation()
{
    m_n70 = 0;
    m_nCameraAnimationTime =
        (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
}

// FUNCTION: SWKOTOR 0x004CC440
void CSWSObject::SetLookAtTarget(unsigned long oid)
{
    m_oidLookAtTarget = oid;
    if (oid == 0x7f000000) {
        m_vLookAt = Vector(0.0f, 0.0f, 0.0f);
    }
}

// FUNCTION: SWKOTOR 0x004CC490
void CSWSDialog::FUN_004cc490(void* p)
{
    if (p) {
        m_lstPlayers.AddTail(p);
    }
}

// FUNCTION: SWKOTOR 0x004CC4B0
unsigned long CSWSDialog::FUN_004cc4b0()
{
    if (m_lstPlayers.m_pList->m_pHead) {
        return m_oidOwner;
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004CCF60
void CSWSObject::SetAnimation(int nAnimation)
{
    m_nUpdateFlags[0] |= 1;
    m_nAnimation = nAnimation;
}

// FUNCTION: SWKOTOR 0x004CCF80
void CSWSObject::DamageHitPoints(int nDamage)
{
    m_nCurrentHitPoints = GetCurrentHitPoints(1) - nDamage;
    if (m_bPlotObject && m_nCurrentHitPoints < 1) {
        m_nCurrentHitPoints = 1;
    }
}

// FUNCTION: SWKOTOR 0x004CD170
void CSWSObject::SetOrientation(const Vector& v)
{
    m_vOrientation = v;
    m_nUpdateFlags[0] |= 4;
}

// FUNCTION: SWKOTOR 0x004CD240
void CSWSObject::RemoveLoopingVisualEffect(unsigned short nVisualEffect)
{
    for (int i = m_lstLoopingVisualEffects.num - 1; i >= 0; i--) {
        if (m_lstLoopingVisualEffects[i]->m_nVisualEffect == nVisualEffect) {
            delete m_lstLoopingVisualEffects[i];
            m_lstLoopingVisualEffects[i] = NULL;
            m_lstLoopingVisualEffects.DelIndex(i);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004CD660
int CSWSObject::RemoveObjectFromDialog(unsigned long oid)
{
    if (m_pDialog == NULL || m_idSelf == oid) {
        return 0;
    }
    if ((m_pDialog->m_lstPlayers.IsEmpty() && m_pDialog->GetOwnerInline() == oid) ||
        (!m_pDialog->m_lstPlayers.IsEmpty() && !m_pDialog->RemovePlayer(oid))) {
        return StopDialog();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004CD6C0
CSWSExpression* CSWSObject::GetListenExpressionById(int nId)
{
    for (int i = 0; i < m_lstListenExpressions.num; i++) {
        if (m_lstListenExpressions[i]->m_nId == nId) {
            return m_lstListenExpressions[i];
        }
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004CD700
int CSWSObject::TestListenExpression(CExoString& sText)
{
    int nResult = -1;
    for (int i = 0; i < m_lstListenExpressions.num; i++) {
        if (m_lstListenExpressions[i] && m_lstListenExpressions[i]->TestExpression(sText, 1)) {
            nResult = m_lstListenExpressions[i]->m_nId;
            break;
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x004CDAC0
int CSWSObject::GetHasSpellEffectApplied(int nSpellId)
{
    if (nSpellId != -1) {
        for (int i = 0; i < m_appliedEffects.num; i++) {
            if (m_appliedEffects[i]->m_nSpellId == nSpellId) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004CE680
int CSWSObject::GetHasFeatEffectApplied(unsigned short nFeat)
{
    CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
    if (pFeat == NULL) {
        return 0;
    }
    int nSpellId = pFeat->m_nSpellId;
    if (nSpellId != -1) {
        for (int i = 0; i < m_appliedEffects.num; i++) {
            if (m_appliedEffects[i]->m_nSpellId == nSpellId) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004CE8A0
void CSWSObject::SetFadeOutTime(int nTime)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSClient* pClient = pServer->GetClientObjectByObjectID(pServer->GetPlayerCreatureID());
    if (pClient) {
        CExoLinkedListPosition pos = pClient->m_pEntries->GetHeadPos();
        while (pos) {
            CSWCCreatureEntry* pEntry = pClient->m_pEntries->GetAtPos(pos);
            if (pEntry->m_oidObject == m_idSelf) {
                pEntry->m_nFadeOutTime = nTime;
                return;
            }
            pClient->m_pEntries->GetNext(pos);
        }
    }
    m_nFadeOutTime = nTime;
}

// FUNCTION: SWKOTOR 0x004CE920
void CSWSObject::SetDeleteNoFade(int bValue)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSClient* pClient = pServer->GetClientObjectByObjectID(pServer->GetPlayerCreatureID());
    if (pClient) {
        CExoLinkedListPosition pos = pClient->m_pEntries->GetHeadPos();
        while (pos) {
            CSWCCreatureEntry* pEntry = pClient->m_pEntries->GetAtPos(pos);
            if (pEntry->m_oidObject == m_idSelf) {
                pEntry->m_bDeleteNoFade = bValue;
                return;
            }
            pClient->m_pEntries->GetNext(pos);
        }
    }
    m_bDeleteNoFade = bValue;
}

// FUNCTION: SWKOTOR 0x004CE9A0
void CSWSObject::SetKeepCorpse(int bValue)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSClient* pClient = pServer->GetClientObjectByObjectID(pServer->GetPlayerCreatureID());
    if (pClient) {
        CExoLinkedListPosition pos = pClient->m_pEntries->GetHeadPos();
        while (pos) {
            CSWCCreatureEntry* pEntry = pClient->m_pEntries->GetAtPos(pos);
            if (pEntry->m_oidObject == m_idSelf) {
                pEntry->m_bKeepCorpse = bValue;
                return;
            }
            pClient->m_pEntries->GetNext(pos);
        }
    }
    m_bKeepCorpse = bValue;
}

// FUNCTION: SWKOTOR 0x004CEC50
void CSWSObject::SaveObjectState(CResGFF* pRes, CResStruct* pStruct)
{
    SaveEffectList(pRes, pStruct);
    m_ScriptVars.SaveVarTable(pRes, pStruct);
    m_VarTable.SaveVarTable(pRes, pStruct);
    SaveActionQueue(pRes, pStruct);
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bCommandable, "Commandable");
}

// FUNCTION: SWKOTOR 0x004CF390
void CSWSObject::AddLoopingVisualEffect(unsigned short nVisualEffect, unsigned long oidTarget,
                                        unsigned char nBodyPart)
{
    CLoopingVisualEffect* pEffect = new CLoopingVisualEffect;
    pEffect->m_nVisualEffect = nVisualEffect;
    pEffect->m_oidTarget = oidTarget;
    pEffect->m_nBodyPart = nBodyPart;
    m_lstLoopingVisualEffects.Add(pEffect);
}

// FUNCTION: SWKOTOR 0x004CFA50
void CSWSObject::ClearMatchedExpressionStrings()
{
    for (int i = 0; i < m_lstMatchedExpressionStrings.num; i++) {
        if (m_lstMatchedExpressionStrings[i]) {
            delete m_lstMatchedExpressionStrings[i];
            m_lstMatchedExpressionStrings[i] = NULL;
        }
    }
    m_lstMatchedExpressionStrings.SetSize(0);
}

// FUNCTION: SWKOTOR 0x004CD460
int CSWSObject::SendDialogEntry(int nEntry)
{
    if (m_pDialog == NULL) {
        return 0;
    }
    if (nEntry != -1 && (m_n50 || IsDialogDelay())) {
        m_n4c = nEntry;
        m_n48 = 1;
        return 1;
    }
    m_n1f4 = 0;
    int nStatus = 0;
    int bResult = m_pDialog->SendDialogEntry(this, -1, nEntry, &nStatus);
    if (nStatus == 10) {
        m_n4c = nEntry;
        m_n48 = 1;
        return 1;
    }
    if (bResult == 0) {
        if (m_pDialog) {
            m_pDialog->RunEndConversationScript(this, 0);
            m_n38 = 0;
            m_n3c = 0;
            m_n48 = 0;
            m_n214 = 1;
        }
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004CD500
int CSWSObject::SendDialogReplies()
{
    if (m_pDialog == NULL) {
        return 0;
    }
    if (m_n50 || IsDialogDelay()) {
        m_n48 = 1;
        m_n4c = -1;
        return 1;
    }
    m_n1f4 = 0;
    if (!m_pDialog->SendDialogReplies(this, -1)) {
        m_pDialog->RunEndConversationScript(this, 0);
        m_n38 = 0;
        m_n3c = 0;
        m_n48 = 0;
        m_n214 = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004CF3F0
void CSWSObject::RemoveEffectTarget(unsigned long oid)
{
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oid);
    if (pObject == NULL || pObject->AsSWSObject() == NULL) {
        return;
    }
    if (m_lstEffectTargets.num <= 0) {
        return;
    }
    CExoArrayList<CGameEffect*>* pEffects = &pObject->AsSWSObject()->m_appliedEffects;
    int bFound = 0;
    for (unsigned long i = 0; i < (unsigned long)pEffects->num; i++) {
        if (bFound) {
            return;
        }
        if ((*pEffects)[i]->m_oidCreator == m_idSelf) {
            bFound = 1;
        }
    }
    if (bFound) {
        return;
    }
    if (m_lstEffectTargets.Count(oid)) {
        m_lstEffectTargets.Remove(oid);
    }
}
