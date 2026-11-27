// CExoResMan::CacheResource: the by-value overloads (exoresman.o in the Mac
// symbol tables). The three-argument form only copies the name in this build.

#include "shared/exo_string.h"

class CExoResMan {
public:
    CExoString CacheResource(const CExoString& sName, unsigned short nType);
    CExoString CacheResource(const CExoString& sName, unsigned short nType, int& nResult);
};

// FUNCTION: SWKOTOR 0x00406C70 SYMBOL: ?CacheResource@CExoResMan@@QAE?AVCExoString@@ABV2@GAAH@Z
CExoString CExoResMan::CacheResource(const CExoString& sName, unsigned short nType, int& nResult)
{
    CExoString sResult(sName);
    return sResult;
}

// FUNCTION: SWKOTOR 0x004085F0 SYMBOL: ?CacheResource@CExoResMan@@QAE?AVCExoString@@ABV2@G@Z
CExoString CExoResMan::CacheResource(const CExoString& sName, unsigned short nType)
{
    int nResult;
    return CacheResource(sName, nType, nResult);
}
