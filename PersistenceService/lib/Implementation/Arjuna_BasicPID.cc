#include <os/string.h>
#include <os/strstream.h>
#include <os/sys/types.h>
#include <os/iostream.h>

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePOM.ch>
#else
#  include <Interface/CosPersistencePOM.h>
#endif

#ifndef COMMON_KUBERA_H_
#  include <Common/Kubera.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJECTSTORE_OBJECTSTORENAMES_H_
#  include <ObjectStore/ObjectStoreNames.h>
#endif

#ifndef IMPLEMENTATION_PIDINVENTORY_H_
#  include <Implementation/PIDInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPDS_H_
#  include <Implementation/Arjuna_BasicPDS.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPID_H_
#  include <Implementation/Arjuna_BasicPID.h>
#endif

#ifndef COMMON_ORBINTERFACE_H_
#  include <Common/ORBInterface.h>
#endif

#ifndef COMMON_COSPERSISNAMES_H_
#  include <Common/CosPersisNames.h>
#endif

Boolean Arjuna_BasicPID_Init::added = FALSE;

Arjuna_BasicPID_Init::Arjuna_BasicPID_Init ()
{
    if (!added)
    {
	PIDInventory::addCreateVoid(Arjuna_BasicPID::create, Arjuna_BasicPDS::pdsName());
	PIDInventory::addCreateString(Arjuna_BasicPID::create, Arjuna_BasicPDS::pdsName());
	added = TRUE;
    }
}

Arjuna_BasicPID_Init::~Arjuna_BasicPID_Init ()
{
}

Arjuna_BasicPID::Arjuna_BasicPID ()
				 : _name(0),
				   _objectStoreType(0)
{
#ifdef DEBUG    
    cout << "Arjuna_BasicPID::Arjuna_BasicPID ()" << endl;
#endif    
    
    setDefault();
}

/*
 * Put error checking in.
 */

Arjuna_BasicPID::Arjuna_BasicPID (const char* s)
				 : _uid(NIL_UID),
				   _name(0),
				   _objectStoreType(0)
{
#ifdef DEBUG    
    cout << "Arjuna_BasicPID::Arjuna_BasicPID ( " << s << " )" << endl;
#endif    

    CORBA_(Environment env);

    setDefault();
    
    /*
     * Assume key has already been stripped from the string.
     */
    
    char* marker = ::strstr(s, _separator);

    if (marker)
    {
	char* storeType = ::new char[marker-s+1];
	::memcpy(storeType, s, marker-s);
	::memset(storeType+(marker-s), '\0', 1);

	objectStore(storeType, env);
	::delete [] storeType;
    
	marker++;
	char* marker2 = ::strstr(marker, _separator);

	if (marker2)
	{
	    char* type = ::new char[marker2-marker+1];
	    ::memcpy(type, marker, marker2-marker);
	    ::memset(type+(marker2-marker), '\0', 1);
	    
	    typeName(type, env);
	    
	    ::delete [] type;
	    
	    marker2++;

	    if ((marker2 - s) < ::strlen(s))
	    {
		char* id = ::new char[::strlen(marker2)+1];
		::strcpy(id, marker2);

		Uid u(id);
		_uid = u;

		::delete [] id;
	    }
	}
    }
}

Arjuna_BasicPID::Arjuna_BasicPID (const char* storeType, const char* type, const Uid& u)
				 : _uid(u),
				   _name(0),
				   _objectStoreType(0)
			 
{
#ifdef DEBUG    
    cout << "Arjuna_BasicPID::Arjuna_BasicPID ( " << storeType << ", " << type << ", " << id << " )" << endl;
#endif    

    CORBA_(Environment env);
    
    setDefault();
    
    if (storeType)
	objectStore(storeType, env);

    if (type)
	typeName(type, env);
}

Arjuna_BasicPID::~Arjuna_BasicPID ()
{
#ifdef DEBUG    
    cout << "Arjuna_BasicPID::~Arjuna_BasicPID ()" << endl;
#endif    
    
    if (_name)
	::delete [] _name;

    if (_objectStoreType)
	::delete [] _objectStoreType;
}

CosPersistencePID::PID_ptr Arjuna_BasicPID::create ()
{
#ifdef PTR_AS_POINTER    
    return new Arjuna_BasicPID();
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
    {
	cerr << "Error - orb/boa not initialised!" << endl;
	return PID::_nil();
    }

    Arjuna_BasicPID* p = new Arjuna_BasicPID();
    Environment env;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(PID_Uid)(p), 0, 0, env);

    return ptr;
#endif
}

CosPersistencePID::PID_ptr Arjuna_BasicPID::create (const char* string)
{
#ifdef PTR_AS_POINTER
    return new Arjuna_BasicPID(string);
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
    {
	cerr << "Error - orb/boa not initialised!" << endl;
	return PID::_nil();
    }

    Arjuna_BasicPID* p = new Arjuna_BasicPID(string);
    Environment env;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(PID_Uid)(p), 0, 0, env);

    return ptr;
#endif
}

void Arjuna_BasicPID::setDefault ()
{
    _objectStoreType = ::new char[::strlen(FragmentedStoreType)+1];
    ::strcpy(_objectStoreType, FragmentedStoreType);

    _name = ::new char[::strlen("/NoTypeName")+1];
    ::strcpy(_name, "/NoTypeName");
}

char* Arjuna_BasicPID::uid (CORBA_(Environment& env))
{
    char* toReturn = ::new char[UidSize];
    ::memset(toReturn, '\0', UidSize);

    ostrstream output(toReturn, UidSize);

    output << _uid;

    return toReturn;
}

void Arjuna_BasicPID::uid (const char* uid, CORBA_(Environment& env))
{
    Uid u(uid);
    
    _uid = u;
}

char* Arjuna_BasicPID::typeName (CORBA_(Environment& env))
{
    char* toReturn = ::new char[::strlen(_name)+1];
    ::strcpy(toReturn, _name);
    
    return toReturn;
}

void Arjuna_BasicPID::typeName (const char* type, CORBA_(Environment& env))
{
    if (_name)
	::delete [] _name;

    _name = ::new char[::strlen(type)+1];
    ::strcpy(_name, type);
}

void Arjuna_BasicPID::datastore_type (const char* type, CORBA_(Environment& env))
{
    // should throw an exception, since we should not allow a change once created.
}

char* Arjuna_BasicPID::datastore_type (CORBA_(Environment& env))
{
    char* toReturn = ::new char[::strlen(Arjuna_BasicPDS::pdsName())+1];
    ::strcpy(toReturn, Arjuna_BasicPDS::pdsName());

    return toReturn;
}

void Arjuna_BasicPID::objectStore (const char* type, CORBA_(Environment& env))
{
    if (_objectStoreType)
	::delete [] _objectStoreType;

    _objectStoreType = ::new char[::strlen(type)+1];
    ::strcpy(_objectStoreType, type);
}

char* Arjuna_BasicPID::objectStore (CORBA_(Environment& env))
{
    char* toReturn = ::new char[::strlen(_objectStoreType)+1];
    ::strcpy(toReturn, _objectStoreType);

    return toReturn;
}

char* Arjuna_BasicPID::get_PIDString (CORBA_(Environment& env))
{
    /*
     * Build string.
     * <Arjuna_BasicPDS name>:<Store type>:<TypeName>:<Uid>
     */

    char* toReturn = ::new char[::strlen(Arjuna_BasicPDS::pdsName())+::strlen(_objectStoreType)+::strlen(_name)+UidSize+4];
    ::memset(toReturn, '\0', ::strlen(Arjuna_BasicPDS::pdsName())+::strlen(_objectStoreType)+::strlen(_name)+UidSize+4);

    ::strcpy(toReturn, Arjuna_BasicPDS::pdsName());
    ::strcat(toReturn, _separator);
    ::strcat(toReturn, _objectStoreType);
    ::strcat(toReturn, _separator);
    ::strcat(toReturn, _name);
    ::strcat(toReturn, _separator);

    char* ptr = uid(env);

    ::strcat(toReturn, ptr);
    
    ::delete [] ptr;

    return toReturn;
}
