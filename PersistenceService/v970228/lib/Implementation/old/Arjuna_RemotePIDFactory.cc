#include <os/iostream.h>

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPIDFACTORY_H_
#  include <Implementation/Arjuna_RemotePIDFactory.h>
#endif

#ifndef IMPLEMENTATION_PIDIMPLELIST_H_
#  include <Implementation/PIDImpleList.h>
#endif

#ifndef IMPLEMENTATION_PID_INIT_H_
#  include <Implementation/PID_Init.h>
#endif

Arjuna_PIDFactory::Arjuna_PIDFactory ()
{
#ifdef DEBUG
    cout << "Arjuna_PIDFactory::Arjuna_PIDFactory ()" << endl;
#endif    
}

Arjuna_PIDFactory::Arjuna_PIDFactory (const char* name)
				     : UidPIDFactoryBOAImpl(name)
{
#ifdef DEBUG    
    cout << "Arjuna_PIDFactory::Arjuna_PIDFactory ( " << name << " )" << endl;
#endif    
}

Arjuna_PIDFactory::~Arjuna_PIDFactory ()
{
#ifdef DEBUG    
    cout << "Arjuna_PIDFactory::~Arjuna_PIDFactory ()" << endl;
#endif
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key (const char* key, CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = ImpleList::find(key);

    if (pidCreator)
	return pidCreator->create();
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_string (const char* pid_string, CORBA_NAMESPACE(Environment& env))
{
    CosPersistencePID::PID_ptr toReturn = CosPersistencePID::PID::_nil();
    char* marker = ::strstr(pid_string, _separator);
    char* pidType = ::new char[marker - pid_string + 1];

    ::memcpy(pidType, pid_string, marker - pid_string);
    ::memset(pidType+(marker-pid_string), '\0', 1);

#ifdef DEBUG    
    cout << "comparing " << pidType << " and " << Arjuna_pidName << endl;
#endif    

    PID_Init* pidCreator = ImpleList::find(pidType);

    if (pidCreator)
	toReturn = pidCreator->create(marker+1);
    else
    {
	::delete [] pidType;
	throw PIDFactory::CreateFailure();
    }

    ::delete [] pidType;

    return toReturn;
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key_and_string (const char* key, const char* pid_string,
									      CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = ImpleList::find(key);

    if (pidCreator)
	return pidCreator->create(pid_string);
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key_and_store (const char* key, const char* store_name,
									     CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = ImpleList::find(key);

    if (pidCreator)
    {
	Arjuna_PID* toReturn = pidCreator->create();

	toReturn->datastore_type(store_name, env);

	return toReturn;
    }
    else
	throw PIDFactory::CreateFailure();
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key_and_store_and_type (const char* key, const char* store_name,
										 const char* type, CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = ImpleList::find(key);
    
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

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key_and_store_and_type_and_uid (const char* key, const char* store_name,
											      const char* type, const char* uid,
											      CORBA_NAMESPACE(Environment& env))
{
    PID_Init* pidCreator = ImpleList::find(key);
    
    if (pidCreator)
	return pidCreator->create(store_name, type, uid);
    else
	throw PIDFactory::CreateFailure();
}
