#include <os/string.h>
#include <os/strstream.h>
#include <os/sys/types.h>
#include <os/iostream.h>

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePOM.ch>
#else
#  include <Interface/CosPersistencePOM.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_KUBERA_H_
#  include <Common/Kubera.h>
#endif

#ifndef OBJECTSTORE_OBJECTSTORENAMES_H_
#  include <ObjectStore/ObjectStoreNames.h>
#endif

#ifndef IMPLEMENTATION_PIDINVENTORY_H_
#  include <Implementation/PIDInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPID_H_
#  include <Implementation/Arjuna_RemotePID.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPDS_H_
#  include <Implementation/Arjuna_RemotePDS.h>
#endif

#ifndef IMPLEMENTATION_PDSINVENTORY_H_
#  include <Implementation/PDSInventory.h>
#endif

#ifndef COMMON_ORBINTERFACE_H_
#  include <Common/ORBInterface.h>
#endif

#ifndef COMMON_COSPERSISNAMES_H_
#  include <Common/CosPersisNames.h>
#endif

#ifndef COMMON_EXCEPT_H_
#  include <Common/except.h>
#endif

Boolean Arjuna_RemotePID_Init::added = FALSE;

Arjuna_RemotePID_Init::Arjuna_RemotePID_Init ()
{
    if (!added)
    {
	PIDInventory::addCreateVoid(Arjuna_RemotePID::create, Arjuna_RemotePDS::pdsName());
	PIDInventory::addCreateString(Arjuna_RemotePID::create, Arjuna_RemotePDS::pdsName());
	added = TRUE;
    }
}

Arjuna_RemotePID_Init::~Arjuna_RemotePID_Init ()
{
}


Arjuna_RemotePID::Arjuna_RemotePID ()
				   : _hostName(0),
				     _pidString(0)
{
#ifdef DEBUG    
    cout << "Arjuna_RemotePID::Arjuna_RemotePID ()" << endl;
#endif    
    
    setDefault();
}

/*
 * Put error checking in, e.g., going past end of string.
 */

Arjuna_RemotePID::Arjuna_RemotePID (const char* s)
				   : _hostName(0),
				     _pidString(0)
{
#ifdef DEBUG    
    cout << "Arjuna_RemotePID::Arjuna_RemotePID ( " << s << " )" << endl;
#endif    

    CORBA_(Environment env);

    /*
     * Assume already removed PID key to get here.
     */

    char* marker = ::strstr(s, _separator);
    
    char* host = ::new char[marker-s+1];
    ::memcpy(host, s, marker-s);
    ::memset(host+(marker-s), '\0', 1);

    hostName(host, env);
    ::delete [] host;
    
    marker++;

    pidString(marker, env);
}

Arjuna_RemotePID::Arjuna_RemotePID (const char* host, const char* pid)
				   : _hostName(0),
				     _pidString(0)
			 
{
#ifdef DEBUG    
    cout << "Arjuna_RemotePID::Arjuna_RemotePID ( " << host << ", " << pid << " )" << endl;
#endif    

    CORBA_(Environment env);
    
    setDefault();
    
    if (host)
	hostName(host, env);

    if (pid)
	pidString(pid, env);
}

Arjuna_RemotePID::~Arjuna_RemotePID ()
{
#ifdef DEBUG    
    cout << "Arjuna_RemotePID::~Arjuna_RemotePID ()" << endl;
#endif    
    
    if (_hostName)
	::delete [] _hostName;

    if (_pidString)
	::delete [] _pidString;
}

CosPersistencePID::PID_ptr Arjuna_RemotePID::create ()
{
#ifdef PTR_AS_POINTER    
    return new Arjuna_RemotePID();
#else    
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
    {
	cerr << "Error - orb/boa not initialised!" << endl;
	return PDS::_nil();
    }

    Arjuna_RemotePID* p = new Arjuna_RemotePID();    
    Environment env;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(PID_RemoteService)(p), 0, 0, env);

    return ptr;
#endif
}

CosPersistencePID::PID_ptr Arjuna_RemotePID::create (const char* string)
{
#ifdef PTR_AS_POINTER
    return new Arjuna_RemotePID(string);
#else    
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
    {
	cerr << "Error - orb/boa not initialised!" << endl;
	return PDS::_nil();
    }

    Arjuna_RemotePID* p = new Arjuna_RemotePID(string);
    Environment env;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(PID_RemoteService)(p), 0, 0, env);

    return ptr;
#endif    
}

void Arjuna_RemotePID::setDefault ()
{
    _hostName = ::new char[::strlen("NO_HOSTNAME")+1];
    ::strcpy(_hostName, "NO_HOSTNAME");
    _pidString = ::new char[::strlen("NULL")+1];
    ::strcpy(_pidString, "NULL");
}

char* Arjuna_RemotePID::hostName (CORBA_(Environment& env))
{
    char* toReturn = ::new char[::strlen(_hostName)+1];
    ::strcpy(toReturn, _hostName);

    return toReturn;
}

void Arjuna_RemotePID::hostName (const char* host, CORBA_(Environment& env))
{
    if (_hostName)
	::delete [] _hostName;

    _hostName = ::new char[::strlen(host)+1];
    ::strcpy(_hostName, host);
}

void Arjuna_RemotePID::pidString (const char* pid, CORBA_(Environment& env))
{
    if (_pidString)
	::delete [] _pidString;

    _pidString = ::new char[::strlen(pid)+1];
    ::strcpy(_pidString, pid);
}

char* Arjuna_RemotePID::pidString (CORBA_(Environment& env))
{
    char* toReturn = ::new char[::strlen(_pidString)+1];
    ::strcpy(toReturn, _pidString);

    return toReturn;
}

void Arjuna_RemotePID::datastore_type (const char* type, CORBA_(Environment& env))
{
    // should throw an exception, since we cannot allow a change once created.
}

char* Arjuna_RemotePID::datastore_type (CORBA_(Environment& env))
{
    char* toReturn = ::new char[::strlen(Arjuna_RemotePDS::pdsName())+1];
    ::strcpy(toReturn, Arjuna_RemotePDS::pdsName());

    return toReturn;
}

char* Arjuna_RemotePID::get_PIDString (CORBA_(Environment& env))
{
    /*
     * Build string.
     * <Arjuna_RemotePID>:<host name>:<pid string>
     */

    char* toReturn = ::new char[::strlen(Arjuna_RemotePDS::pdsName())+::strlen(_hostName)+::strlen(_pidString)+3];
    ::memset(toReturn, '\0', ::strlen(Arjuna_RemotePDS::pdsName())+::strlen(_hostName)+::strlen(_pidString)+3);

    ::strcpy(toReturn, Arjuna_RemotePDS::pdsName());
    ::strcat(toReturn, _separator);
    ::strcat(toReturn, _hostName);
    ::strcat(toReturn, _separator);
    ::strcat(toReturn, _pidString);
    
    return toReturn;
}
