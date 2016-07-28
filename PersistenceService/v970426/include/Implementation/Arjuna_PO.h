#ifndef IMPLEMENTATION_ARJUNA_PO_H_
#define IMPLEMENTATION_ARJUNA_PO_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePOM.ch>
#  include <Interface/CosPersistencePDS.ch>
#  include <Interface/PackablePO.ch>
#else
#  include <Interface/CosPersistencePOM.h>
#  include <Interface/CosPersistencePDS.h>
#  include <Interface/PackablePO.h>
#endif

class Arjuna_PO : public BOA_(PackablePO)
{
public:
    CosPersistencePID::PID_ptr p (CORBA_(Environment& env));
    void p (CosPersistencePID::PID_ptr, CORBA_(Environment& env));
    
    CosPersistencePDS::PDS_ptr connect (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    
    void disconnect  (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void store       (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void restore     (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void deleteState (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));

    virtual void unpackState (ObjectBuffer_ptr os, CORBA_(Environment& env));
    virtual void packState   (ObjectBuffer_ptr os, CORBA_(Environment& env));

    virtual char* type (CORBA_(Environment& env));
    
protected:
    Arjuna_PO ();
    Arjuna_PO (CosPersistencePID::PID_ptr pid);
    virtual ~Arjuna_PO ();
    
private:
    void init ();
    void createNewPid ();
    
    CosPersistencePOM::POM_ptr _protocolManager;
    CosPersistencePID::PID_ptr _myPID;
    CosPersistencePDS::PDS_ptr _theProtocol;
    CORBA::Boolean             _isNew;
};

#endif
