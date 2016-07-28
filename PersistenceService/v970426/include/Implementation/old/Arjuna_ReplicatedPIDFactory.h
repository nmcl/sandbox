#ifndef IMPLEMENTATION_ARJUNA_REPLICATEDPIDFACTORY_H_
#define IMPLEMENTATION_ARJUNA_REPLICATEDPIDFACTORY_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/NameSpace.h>
#endif

#ifndef IMPLEMENTATION_PIDFACTORY_INIT_H_
#  include <Implementation/PIDFactory_Init.h>
#endif

#include <Interface/UidPIDFactory.h>

class Arjuna_ReplicatedPIDFactory : public UidPIDFactoryBOAImpl
{
public:
    Arjuna_ReplicatedPIDFactory ();
    Arjuna_ReplicatedPIDFactory (const char* name);
    virtual ~Arjuna_ReplicatedPIDFactory ();

    CosPersistencePID::PID_ptr create_PID_from_key (const char* key, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_string (const char* pid_string, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_key_and_string (const char* key, const char* pid_string,
							       CORBA_NAMESPACE(Environment& env));

    static const char* factoryName ();
};

class Arjuna_ReplicatedPIDFactory_Init : public PIDFactory_Init
{
public:
    Arjuna_ReplicatedPIDFactory_Init ();
    virtual ~Arjuna_ReplicatedPIDFactory_Init ();

    const char* factoryName () const;

    Arjuna_ReplicatedPID* create ();    
    Arjuna_ReplicatedPID* create (const char* string);
    Arjuna_ReplicatedPID* create (const char* storeType, const char* type, const char* id);

private:
    static Boolean added;
};

static Arjuna_ReplicatedPIDFactory_Init initReplicatedPIDFactory;

#endif
