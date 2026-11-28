// swc_object_server_link.h -- CSWCObjectServerLink
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_c_placeable_door.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_OBJECT_SERVER_LINK_H
#define SHARED_SWC_OBJECT_SERVER_LINK_H


class CSWSObject;

class CSWCObjectServerLink { public: virtual ~CSWCObjectServerLink(); CSWSObject* GetServerObject(); };

#endif
