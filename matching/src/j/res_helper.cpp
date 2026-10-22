// CResHelper<T, nType> instantiations in 0x005D0000-0x005DFFFF.
// The member templates live in j/res_helper.h.
#include "j/res_helper.h"

// FUNCTION: SWKOTOR 0x005DE590 SYMBOL: ??1?$CResHelper@VCResLYT@@$0LLI@@@UAE@XZ
// FUNCTION: SWKOTOR 0x005D1A60 SYMBOL: ??1?$CResHelper@VCResNCS@@$0HNK@@@UAE@XZ
// FUNCTION: SWKOTOR 0x005DAED0 SYMBOL: ??1?$CResHelper@VCResWave@@$03@@UAE@XZ
// FUNCTION: SWKOTOR 0x005D11E0 SYMBOL: ??1?$CResHelper@VCResNSS@@$0HNJ@@@UAE@XZ
// CResHelper<T, nType>::~CResHelper (j/res_helper.h)

// FUNCTION: SWKOTOR 0x005D5E90 SYMBOL: ?SetResRef@?$CResHelper@VCResWave@@$03@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x005DE5F0 SYMBOL: ?SetResRef@?$CResHelper@VCResLYT@@$0LLI@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x005D1AC0 SYMBOL: ?SetResRef@?$CResHelper@VCResNCS@@$0HNK@@@QAEXABVCResRef@@H@Z
// CResHelper<T, nType>::SetResRef (j/res_helper.h)

// FUNCTION: SWKOTOR 0x005D1C20 SYMBOL: ??_G?$CResHelper@VCResNCS@@$0HNK@@@UAEPAXI@Z
CResHelper<CResNCS, 2010>* NewResHelperNCS() { return new CResHelper<CResNCS, 2010>; }

// FUNCTION: SWKOTOR 0x005DAF30 SYMBOL: ??_G?$CResHelper@VCResWave@@$03@@UAEPAXI@Z
CResHelper<CResWave, 4>* NewResHelperWave() { return new CResHelper<CResWave, 4>; }

// FUNCTION: SWKOTOR 0x005D1240 SYMBOL: ??_G?$CResHelper@VCResNSS@@$0HNJ@@@UAEPAXI@Z
CResHelper<CResNSS, 2009>* NewResHelperNSS() { return new CResHelper<CResNSS, 2009>; }

template class CResHelper<CResWave, 4>;
template class CResHelper<CResLYT, 3000>;
template class CResHelper<CResNCS, 2010>;
template class CResHelper<CResNSS, 2009>;
