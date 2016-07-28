#ifndef IMPLEMENTATION_ARJUNA_REMOTEPIDFACTORY_H_
#define IMPLEMENTATION_ARJUNA_REMOTEPIDFACTORY_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/NameSpace.h>
#endif

#ifndef IMPLEMENTATION_PIDFACTORY_INIT_H_
#  include <Implementation/PIDFactory_Init.h>
#endif

#include <Interface/PIDFactory.h>

class Arjuna_RemotePIDFactory : public UidPIDFactoryBOAImpl
{
public:
    Arjuna_RemotePIDFactory ();
    Arjuna_RemotePIDFactory (const char* name);
    virtual ~Arjuna_RemotePIDFactory ();

    CosPersistencePID::PID_ptr create_PID_from_key (const char* key, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_string (const char* pid_string, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_key_and_string (const char* key, const char* pid_string,
							       CORBA_NAMESPACE(Environment& env));
};

class Arjuna_RemotePIDFactory_Init : public PIDFactory_Init
{
public:
    Arjuna_RemotePIDFactory_Init ();
    virtual ~Arjuna_RemotePIDFactory_Init ();

    const char* pidName () const;

    Arjuna_RemotePID* create ();    
    Arjuna_RemotePID* create (const char* string);
    Arjuna_RemotePID* create (const char* storeType, const char* type, const char* id);

private:
    static Boolean added;
};

static Arjuna_RemotePIDFactory_Init initRemotePIDFactory;

#endif
