#include <os/iostream.h>

#ifndef COMMON_KUBERA_H_
#  include <Common/Kubera.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPIDFACTORY_H_
#  include <Implementation/Arjuna_BasicPIDFactory.h>
#endif

#ifndef IMPLEMENTATION_PIDINVENTORY_H_
#  include <Implementation/PIDInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPID_H_
#  include <Implementation/Arjuna_BasicPID.h>
#endif

Boolean Arjuna_BasicPIDFactory_Init::added = FALSE;

Arjuna_BasicPIDFactory_Init::Arjuna_BasicPIDFactory_Init ()
{
    if (!added)
    {
	PIDInventory::addCreateVoid(Arjuna_BasicPID::create, Arjuna_BasicPID::pidName());
	PIDInventory::addCreateString(Arjuna_BasicPID::create, Arjuna_BasicPID::pidName());
	added = TRUE;
    }
}

Arjuna_BasicPIDFactory_Init::~Arjuna_BasicPIDFactory_Init ()
{
}


Arjuna_BasicPIDFactory::Arjuna_BasicPIDFactory ()
{
#ifdef DEBUG
    cout << "Arjuna_BasicPIDFactory::Arjuna_BasicPIDFactory ()" << endl;
#endif    
}

Arjuna_BasicPIDFactory::Arjuna_BasicPIDFactory (const char* name)
					       : UidPIDFactoryBOAImpl(name)
{
#ifdef DEBUG    
    cout << "Arjuna_BasicPIDFactory::Arjuna_BasicPIDFactory ( " << name << " )" << endl;
#endif    
}

Arjuna_BasicPIDFactory::~Arjuna_BasicPIDFactory ()
{
#ifdef DEBUG    
    cout << "Arjuna_BasicPIDFactory::~Arjuna_BasicPIDFactory ()" << endl;
#endif
}

CosPersistencePID::PID_ptr Arjuna_BasicPIDFactory::create_PID_from_key (const char* key, CORBA_NAMESPACE(Environment& env))
{
    if (::strcmp(key, Arjuna_BasicPID::pidName()) == 0)
	return new Arjuna_BasicPID();
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_BasicPIDFactory::create_PID_from_string (const char* pid_string, CORBA_NAMESPACE(Environment& env))
{
    CosPersistencePID::PID_ptr toReturn = CosPersistencePID::PID::_nil();
    char* marker = ::strstr(pid_string, _separator);
    char* pidType = ::new char[marker - pid_string + 1];

    ::memcpy(pidType, pid_string, marker - pid_string);
    ::memset(pidType+(marker-pid_string), '\0', 1);

    if (::strcmp(pidType, Arjuna_BasicPID::pidName()) != 0)
	throw PIDFactory::CreateFailure();
    else
	toReturn = new Arjuna_BasicPID(marker+1);

    ::delete [] pidType;

    return toReturn;
}

CosPersistencePID::PID_ptr Arjuna_BasicPIDFactory::create_PID_from_key_and_string (const char* key, const char* pid_string,
										   CORBA_NAMESPACE(Environment& env))
{
    if (::strcmp(key, Arjuna_BasicPID::pidName()) == 0)
	return new Arjuna_BasicPID(pid_string);
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_BasicPIDFactory::create_PID_from_key_and_store (const char* key, const char* store_name,
										  CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = Inventory::find(key);

    if (pidCreator)
    {
	Arjuna_PID* toReturn = pidCreator->create();

	toReturn->datastore_type(store_name, env);

	return toReturn;
    }
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_BasicPIDFactory::create_PID_from_key_and_store_and_type (const char* key, const char* store_name,
											   const char* type,
											   CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = Inventory::find(key);
    
    if (pidCreator)
    {
	Arjuna_PID* toReturn = pidCreator->create();

	if (store_name)
	    toReturn->datastore_type(store_name, env);
	
	if (type)
	    toReturn->typeName(type, env);

	return toReturn;
    }
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_BasicPIDFactory::create_PID_from_key_and_store_and_type_and_uid (const char* key, const char* store_name,
												   const char* type, const char* uid,
												   CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = Inventory::find(key);
    
    if (pidCreator)
	return pidCreator->create(store_name, type, uid);
    else
	throw PIDFactory::CreateFailure();
}
