#ifndef IMPLEMENTATION_ARJUNAPDSFACTORY_H_
#define IMPLEMENTATION_ARJUNAPDSFACTORY_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/PDSFactory.ch>
#else
#  include <Interface/PDSFactory.h>
#endif

class Arjuna_PDSFactory : public virtual BOA_(PDSFactory)
{
public:
    Arjuna_PDSFactory ();
#ifdef ORBIX    
    Arjuna_PDSFactory (const char* name);
#endif    
    ~Arjuna_PDSFactory ();

    CosPersistencePDS::PDS_ptr create_from_string (const char* string, CORBA_(Environment& env));
    CosPersistencePDS::PDS_ptr create_from_pidstring (const char* pidstring, CORBA_(Environment& env));    
    CosPersistencePDS::PDS_ptr create_from_PID (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));

    static CosPersistencePDS::PDS_ptr createFromString (const char* string);
    static CosPersistencePDS::PDS_ptr createFromPIDString (const char* string);    
    static CosPersistencePDS::PDS_ptr createFromPID (CosPersistencePID::PID_ptr pid);    
};

#endif
