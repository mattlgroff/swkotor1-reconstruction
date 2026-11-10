// Worker O matching headers: CResHelper<T, nType> member templates (copied
// from bucket j) for the Aurora wrappers, CLIP and CSWNameGen.
#ifndef O_AURORA_RES_HELPER_H
#define O_AURORA_RES_HELPER_H

#include "o/aurora_res.h"

template <class T, int nType>
CResHelper<T, nType>::~CResHelper()
{
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
            m_bAutoRequest = 0;
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
}

template <class T, int nType>
void CResHelper<T, nType>::SetResRef(const CResRef& cResRef, int bAutoRequest)
{
    if (cResRef == m_cResRef) {
        return;
    }
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
    if (cResRef == "") {
        m_pRes = 0;
        m_cResRef = "";
        return;
    }
    T* pRes = (T*)g_pExoResMan->GetResObject(cResRef, nType);
    if (!pRes) {
        pRes = new T();
        g_pExoResMan->SetResObject(cResRef, nType, pRes);
        if (!pRes) {
            return;
        }
    }
    m_pRes = pRes;
    if (bAutoRequest) {
        m_bAutoRequest = 1;
        m_pRes->Request();
    }
    m_cResRef = cResRef;
}

#endif
