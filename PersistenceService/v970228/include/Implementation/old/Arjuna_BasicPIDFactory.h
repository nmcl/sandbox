#ifndef IMPLEMENTATION_ARJUNA_BASICPIDFACTORY_H_
#define IMPLEMENTATION_ARJUNA_BASICPIDFACTORY_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/NameSpace.h>
#endif

#include <Interface/UidPIDFactory.h>

class Arjuna_BasicPIDFactory : public UidPIDFactoryBOAImpl
{
public:
    Arjuna_BasicPIDFactory ();
    Arjuna_BasicPIDFactory (const char* name);
    virtual ~Arjuna_BasicPIDFactory ();

    CosPersistencePID::PID_ptr create_PID_from_key (const char* key, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_string (const char* pid_string, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_key_and_string (const char* key, const char* pid_string,
							       CORBA_NAMESPACE(Environment& env));

    CosPersistencePID::PID_ptr create_PID_from_key_and_store (const char* key, const char* store_name,
							      CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_key_and_store_and_type (const char* key, const char* store_name,
								       const char* type, CORBA_NAMESPACE(Environment& env));
    CosPersistencePID::PID_ptr create_PID_from_key_and_store_and_type_and_uid (const char* key, const char* store_name,
									       const char* type, const char* uid,
									       CORBA_NAMESPACE(Environment& env));
};

class Arjuna_BasicPIDFactory_Init
{
public:
    Arjuna_BasicPIDFactory_Init ();
    virtual ~Arjuna_BasicPIDFactory_Init ();

private:
    static Boolean added;
};

static Arjuna_BasicPIDFactory_Init initBasicPIDFactory;

#endif
