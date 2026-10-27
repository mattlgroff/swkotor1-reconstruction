// CSWMGBehaviorTrack destructor: the original body does not restore the
// derived vptr, so the class is declared novtable in this isolated TU. The
// track-and-fire destructor and both deleting destructors fold into these.
#define L_MG_BEHAVIOR_DTORS
#include "l/sw_mg_behavior.h"

// FUNCTION: SWKOTOR 0x006DB630
CSWMGBehaviorTrack::~CSWMGBehaviorTrack()
{
}
