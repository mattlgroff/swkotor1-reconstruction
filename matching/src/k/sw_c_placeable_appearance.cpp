#include "k/sw_c_placeable_door.h"
// NONMATCH: 2DA argument/global-load scheduling (three source shapes).
// FUNCTION: SWKOTOR 0x00682C60
int CSWCPlaceable::SetShadow()
{
 CExoString shadow;
 if(!m_pModel) return 0;
 int size;
 if(!g_pRules->m_p2DArrays->m_pPlaceables->GetINTEntry(m_nAppearance,CExoString("ShadowSize"),&size)) return 1;
 switch(size) {
 case 0: shadow="Fx_Shadow_S"; break;
 case 1: shadow="Fx_Shadow_M"; break;
 case 2: shadow="Fx_Shadow_L"; break;
 default: shadow="Fx_Shadow_M"; break;
 }
 if(m_bShadow) m_pModel->ModelVBC();
 int result=m_pModel->CreateShadow(shadow);
 if(!m_bShadow) RefreshHilite();
 return result;
}
// NONMATCH: 2DA argument/global-load scheduling (three source shapes).
// FUNCTION: SWKOTOR 0x00682E20
void CSWCPlaceable::SetAppearance(unsigned char appearance)
{
 m_nAppearance=appearance;
 int staticModel=0;
 g_pRules->m_p2DArrays->GetPlaceables()->GetINTEntry(appearance,"HitCheck",&staticModel);
 CAurObject* model=GetModel(0xff,1);
 if(model) model->SetIgnoreHitCheck(staticModel==0);
 if(m_pModel) {
  int ignored;
  g_pRules->m_p2DArrays->GetPlaceables()->GetINTEntry(appearance,"NonCull",&ignored);
 }
}
// NONMATCH: receiver register and 2DA argument scheduling (three source shapes).
// FUNCTION: SWKOTOR 0x00682F20
float CSWCPlaceable::GetCanSeeHeight()
{
 C2DA* placeables=g_pRules->m_p2DArrays->m_pPlaceables;
 float height=1.5f;
 CSWSPlaceable* server=ServerInline();
 if(server) {
  if(!placeables->GetFLOATEntry(server->m_nAppearance,"CanSeeHeight",&height)) return 1.5f;
 }
 return height;
}
