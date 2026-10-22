// CSWSObject listen expressions (own file so /Ob2 does not inline
// GetListenExpressionById into SetListenExpression).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CF8F0
void CSWSObject::SetListenExpression(const CExoString& sExpression, int nId)
{
    CSWSExpression* pExpression = GetListenExpressionById(nId);
    if (pExpression == NULL) {
        pExpression = new CSWSExpression;
        pExpression->m_nId = nId;
        m_lstListenExpressions.Add(pExpression);
    }
    pExpression->SetExpression(sExpression);
}

// FUNCTION: SWKOTOR 0x004CF9B0
void CSWSObject::AddMatchedExpressionString(const CExoString& sString)
{
    m_lstMatchedExpressionStrings.Add(new CExoString(sString));
}
