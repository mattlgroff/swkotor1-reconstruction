// CSWCObject attachment state passed from the server object.

class CSWSObject;

class CSWCObject {
public:
    void AttachmentFromServerObject(CSWSObject* pServerObject);
    void DetachFromServerObject();

    char pad_0x0[0xf8];
    CSWSObject* m_pServerObject;                 // +0xf8
};

// FUNCTION: SWKOTOR 0x0063D520
void CSWCObject::AttachmentFromServerObject(CSWSObject* pServerObject)
{
    m_pServerObject = pServerObject;
}

// FUNCTION: SWKOTOR 0x0063D530
void CSWCObject::DetachFromServerObject()
{
    m_pServerObject = 0;
}
