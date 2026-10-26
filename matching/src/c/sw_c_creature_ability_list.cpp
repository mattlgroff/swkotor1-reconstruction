#include "c/exo_base.h"

#include "c/sw_c_creature_ability_list.h"

// FUNCTION: SWKOTOR 0x00610BC0 SYMBOL: ?Allocate@?$CExoArrayList@VCSWSStats_SpellLikeAbility@@@@QAEXH@Z
template <>
void CExoArrayList<CSWSStats_SpellLikeAbility>::Allocate(int n)
{
    CSWSStats_SpellLikeAbility* pOld = element;
    array_size = n;
    element = new CSWSStats_SpellLikeAbility[n];
    for (int i = 0; i < num; ++i) {
        element[i] = pOld[i];
    }
    if (pOld) delete[] pOld;
}
