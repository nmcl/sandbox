#ifndef IMPLEMENTATION_ARJUNA_OBJBUFFER_H_
#define IMPLEMENTATION_ARJUNA_OBJBUFFER_H_

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef COMMON_COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/ObjectBuffer.ch>
#else
#  include <Interface/ObjectBuffer.h>
#endif

class ObjectState;
class ostream;

class Arjuna_ObjectState : public BOA_(ObjectBuffer)
{
public:
    /* Constructors & destructor */
    
    Arjuna_ObjectState ();
    Arjuna_ObjectState (ObjectState*);    
    virtual ~Arjuna_ObjectState ();

    /* non-virtual member functions and operators */
    
    char* contents (CORBA_(Environment& env));
    long length (CORBA_(Environment& env));
    
    ObjectBuffer::Status readState (CORBA_(Environment& env));
    
    /* pack operations for standard types */

    void packChar            (char, CORBA_(Environment& env));
    void packBoolean         (CORBA::Boolean, CORBA_(Environment& env));
    void packShort           (short, CORBA_(Environment& env));
    void packUShort          (unsigned short, CORBA_(Environment& env));
    void packLong            (long, CORBA_(Environment& env));
    void packULong           (unsigned long, CORBA_(Environment& env));
    void packFloat           (float, CORBA_(Environment& env));
    void packDouble          (double, CORBA_(Environment& env));
    void packMappedString    (const char *, CORBA_(Environment& env));
    void packNonMappedString (const char *, CORBA_(Environment& env));

    void unpackChar          (char&, CORBA_(Environment& env));
    void unpackBoolean       (CORBA::Boolean&, CORBA_(Environment& env));
    void unpackLong          (long&, CORBA_(Environment& env));
    void unpackULong         (unsigned long&, CORBA_(Environment& env));
    void unpackShort         (short&, CORBA_(Environment& env));
    void unpackUShort        (unsigned short&, CORBA_(Environment& env));
    void unpackFloat         (float&, CORBA_(Environment& env));
    void unpackDouble        (double&, CORBA_(Environment& env));
    void unpackString        (char *&, CORBA_(Environment& env));

    /*
     * Implementation specific methods.
     */

    ObjectState* buffer () const;
    void buffer (ObjectState*);

private:
    ObjectState* _theBuffer;
};

#endif
