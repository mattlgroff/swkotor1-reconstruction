// swc_camera.h -- CSWCCamera
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_s9.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWC_CAMERA_H
#define SHARED_SWC_CAMERA_H


class Quaternion;
class Vector;

class CSWCCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                   // 0x20
    virtual Quaternion GetOrientation();            // 0x24
    virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void* GetCameraOfType(unsigned long nType);  // 0x80 (descriptive)

    // declared by other copies
    void SetTargetFollow();
};

#endif
