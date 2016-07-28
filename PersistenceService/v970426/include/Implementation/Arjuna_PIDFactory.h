#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#define IMPLEMENTATION_ARJUNA_PIDFACTORY_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePID.ch>
#  include <Interface/PIDFactory.ch>
#else
#  include <Interface/CosPersistencePID.h>
#  include <Interface/PIDFactory.h>
#endif

class Arjuna_PIDFactory : public BOA_(PIDFactory)
{
public:
    Arjuna_PIDFactory ();
    virtual ~Arjuna_PIDFactory ();

    CosPersistencePID::PID_ptr create_PID_from_key (const char* key, CORBA_(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_string (const char* pid_string, CORBA_(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_key_and_string (const char* key, const char* pid_string,
							       CORBA_(Environment& env));
    
    static CosPersistencePID::PID_ptr createFromKey (const char* string);
    static CosPersistencePID::PID_ptr createFromString (const char* string);    
    static CosPersistencePID::PID_ptr createFromKeyAndString (const char* key, const char* pid);
};

#endif
