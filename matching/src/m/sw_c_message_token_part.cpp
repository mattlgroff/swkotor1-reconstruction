#include "shared/exo_string.h"
class CSWSObject;
class CTlkTable { public: void SetCustomToken(int,const CExoString&); };
class CSWTlkTable : public CTlkTable { public: CExoString Fetch(unsigned long,CSWSObject*,int,int); };
#include "shared/globals.h"
extern CSWTlkTable* g_pTlkTableAlt; // 0x007A3A0C
class CSWCMessage { public: int GetSingleCustomTokenStringPart(int,unsigned long,CExoString*); };
// FUNCTION: SWKOTOR 0x006500B0
int CSWCMessage::GetSingleCustomTokenStringPart(int value,unsigned long strref,CExoString* out)
{
 int result=0;
 CExoString part;
 if (value) {
  { CExoString number(value); g_pTlkTable->SetCustomToken(0,number); }
  part=g_pTlkTableAlt->Fetch(strref,0,0,0);
  *out=*out+part;
  result=1;
 }
 return result;
}
