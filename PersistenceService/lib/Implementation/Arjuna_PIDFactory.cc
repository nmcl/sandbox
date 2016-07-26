#include <os/iostream.h>

#ifndef COMMON_KUBERA_H_
#  include <Common/Kubera.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
#endif

#ifndef IMPLEMENTATION_PIDINVENTORY_H_
#  include <Implementation/PIDInventory.h>
#endif


Arjuna_PIDFactory::Arjuna_PIDFactory ()
{
#ifdef DEBUG
    cout << "Arjuna_PIDFactory::Arjuna_PIDFactory ()" << endl;
#endif    
}

Arjuna_PIDFactory::~Arjuna_PIDFactory ()
{
#ifdef DEBUG    
    cout << "Arjuna_PIDFactory::~Arjuna_PIDFactory ()" << endl;
#endif
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key (const char* key, CORBA_(Environment& env))
{
    return Arjuna_PIDFactory::createFromKey(key);
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_string (const char* pid_string, CORBA_(Environment& env))
{
    return Arjuna_PIDFactory::createFromString(pid_string);
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::create_PID_from_key_and_string (const char* key, const char* pid_string,
									      CORBA_(Environment& env))
{
    return Arjuna_PIDFactory::createFromKeyAndString(key, pid_string);
}

CosPersistencePID::PID_ptr Arjuna_PIDFactory::createFromKey (const char* key)
{
    return PIDInventory::createVoid(key);
}
    
CosPersistencePID::PID_ptr Arjuna_PIDFactory::createFromString (const char* string)
{
    CosPersistencePID::PID_ptr thePid = CosPersistencePID::PID::_nil();    
    char* marker = ::strstr(string, _separator);

    if (marker)
    {
	char* pidImple = ::new char[marker-string+1];
	::memcpy(pidImple, string, marker-string);
	::memset(pidImple+(marker-string), '\0', 1);

	marker++;
	
	char* pid = ::new char[::strlen(marker)+1];
	::strcpy(pid, marker);

	if (pid)
	    thePid = PIDInventory::createString(pidImple, pid);
	else
	    thePid = PIDInventory::createVoid(pidImple);

	::delete [] pid;
	::delete [] pidImple;
    }

    return thePid;
}
    
CosPersistencePID::PID_ptr Arjuna_PIDFactory::createFromKeyAndString (const char* key, const char* pid)
{
    return PIDInventory::createString(key, pid);
}
