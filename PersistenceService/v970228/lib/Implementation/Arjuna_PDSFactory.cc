#include <os/iostream.h>

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PDSFACTORY_H_
#  include <Implementation/Arjuna_PDSFactory.h>
#endif

#ifndef IMPLEMENTATION_PDSINVENTORY_H_
#  include <Implementation/PDSInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPDS_H_
#  include <Implementation/Arjuna_BasicPDS.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPDS_H_
#  include <Implementation/Arjuna_RemotePDS.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
#endif

Arjuna_PDSFactory::Arjuna_PDSFactory ()
{
}

#ifdef ORBIX
Arjuna_PDSFactory::Arjuna_PDSFactory (const char* name)
				     : BOA_(PDSFactory)(name)
{
#ifdef DEBUG    
    cout << "Arjuna_PDSFactory::Arjuna_PDSFactory ( " << name << " )" << endl;
#endif
}
#endif

Arjuna_PDSFactory::~Arjuna_PDSFactory ()
{
#ifdef DEBUG    
    cout << "Arjuna_PDSFactory::~Arjuna_PDSFactory ()" << endl;
#endif    
}

CosPersistencePDS::PDS_ptr Arjuna_PDSFactory::create_from_string (const char* string, CORBA_(Environment& env))
{
    return Arjuna_PDSFactory::createFromString(string);
}

CosPersistencePDS::PDS_ptr Arjuna_PDSFactory::create_from_pidstring (const char* string, CORBA_(Environment& env))
{
    return Arjuna_PDSFactory::createFromPIDString(string);
}

CosPersistencePDS::PDS_ptr Arjuna_PDSFactory::create_from_PID (CosPersistencePID::PID_ptr pid,CORBA_(Environment& env))
{
    return Arjuna_PDSFactory::createFromPID(pid);
}

/*
 * Should raise an exception!
 */

CosPersistencePDS::PDS_ptr Arjuna_PDSFactory::createFromString (const char* string)
{
    return CosPersistencePDS::PDS::_nil();
}

CosPersistencePDS::PDS_ptr Arjuna_PDSFactory::createFromPIDString (const char* string)
{
    if (string)
    {
	CosPersistencePID::PID_ptr pid = Arjuna_PIDFactory::createFromString(string);

	CosPersistencePDS::PDS_ptr ptr = Arjuna_PDSFactory::createFromPID(pid);

	CORBA::release(pid);

	return ptr;
    }
    else
	return CosPersistencePDS::PDS::_nil();
}

CosPersistencePDS::PDS_ptr Arjuna_PDSFactory::createFromPID (CosPersistencePID::PID_ptr pid)
{
    if (CORBA_(is_nil)(pid))
    {
	CORBA_(Environment) env;
	
	char* pdsName = pid->datastore_type(env);

	CosPersistencePDS::PDS_ptr ptr = CosPersistencePDS::PDS::_duplicate(PDSInventory::createVoid(pdsName));
	
	::delete [] pdsName;

	return ptr;
    }
    else
	return CosPersistencePDS::PDS::_nil();
}
