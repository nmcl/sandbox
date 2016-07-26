#ifndef IMPLEMENTATION_ARJUNA_POM_H_
#define IMPLEMENTATION_ARJUNA_POM_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePOM.ch>
#  include <Interface/CosPersistencePDS.ch>
#else
#  include <Interface/CosPersistencePOM.h>
#  include <Interface/CosPersistencePDS.h>
#endif

class Arjuna_POM : public BOA_(CosPersistencePOM::POM)
{
public:
    Arjuna_POM ();
    virtual ~Arjuna_POM ();
    
    CosPersistencePDS::PDS_ptr connect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    
    void disconnect  (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void store       (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void restore     (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void deleteState (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));

private:
    CosPersistencePDS::PDS_ptr _protocolService;
};

#endif
