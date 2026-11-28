// gob_attachment.h -- GobAttachment
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/g/gob.h); the other copies' members fall on its members.
#ifndef SHARED_GOB_ATTACHMENT_H
#define SHARED_GOB_ATTACHMENT_H


class CAurBehaviorAttach;

class GobAttachment {
public:
    virtual ~GobAttachment();
    virtual void v04();
    virtual void Render(int nFlags);                    // +0x08
    virtual void SetLight(int n);                       // +0x0c
    virtual int GetId();                                // +0x10
    virtual CAurBehaviorAttach* GetAttachBehavior();             // +0x14
};

#endif
