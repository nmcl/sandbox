#include <os/string.h>
#include <os/strstream.h>
#include <os/sys/types.h>
#include <os/iostream.h>

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJECTSTORE_OBJECTSTORENAMES_H_
#  include <ObjectStore/ObjectStoreNames.h>
#endif

#ifndef IMPLEMEMTATION_PIDINVENTORY_H_
#  include <Implementation/PIDInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REPLICATEDPID_H_
#  include <Implementation/Arjuna_ReplicatedPID.h>
#endif

static const char* _pidSeparator = "|";

Boolean Arjuna_ReplicatedPID_Init::added = FALSE;

Arjuna_ReplicatedPID_Init::Arjuna_ReplicatedPID_Init ()
{
    if (!added)
    {
	PIDInventory::addCreateVoid(Arjuna_ReplicatedPID::create, Arjuna_ReplicatedPID::pidName());
	PIDInventory::addCreateString(Arjuna_ReplicatedPID::create, Arjuna_ReplicatedPID::pidName());
	added = TRUE;
    }
}

Arjuna_ReplicatedPID_Init::~Arjuna_ReplicatedPID_Init ()
{
}


Arjuna_ReplicatedPID::Arjuna_ReplicatedPID ()
					   : _numberOfReplicats(0),
					     _thePIDs(0)
{
#ifdef DEBUG    
    cout << "Arjuna_ReplicatedPID::Arjuna_ReplicatedPID ()" << endl;
#endif    
    
    setDefault();
}

/*
 * Put error checking in.
 */

Arjuna_ReplicatedPID::Arjuna_ReplicatedPID (const char* s)
					   : _numberOfReplicats(0),
					     _thePIDs(0)
{
#ifdef DEBUG    
    cout << "Arjuna_ReplicatedPID::Arjuna_ReplicatedPID ( " << s << " )" << endl;
#endif    

    CORBA_NAMESPACE(Environment env);

    /*
     * Assume already removed PID key to get here.
     */

    char* marker = ::strstr(s, _pidSeparator);
    
    char* storeType = ::new char[marker-s+1];
    ::memcpy(storeType, s, marker-s);
    ::memset(storeType+(marker-s), '\0', 1);

    datastore_type(storeType, env);
    ::delete [] storeType;
    
    marker++;
    char* marker2 = ::strstr(marker, _separator);

    _name = ::new char[marker2-marker+1];
    ::memcpy(_name, marker, marker2-marker);
    ::memset(_name+(marker2-marker), '\0', 1);

    marker2++;
    char* id = ::new char[::strlen(marker2)+1];
    ::strcpy(id, marker2);

    Uid u(id);
    _uid = u;

    ::delete [] id;
}

Arjuna_ReplicatedPID::Arjuna_ReplicatedPID (const char* storeType, const char* type, const char* id)
				 : _uid(NIL_UID),
				   _name(0),
				   _datastoreType(0)
			 
{
#ifdef DEBUG    
    cout << "Arjuna_ReplicatedPID::Arjuna_ReplicatedPID ( " << storeType << ", " << type << ", " << id << " )" << endl;
#endif    

    CORBA_NAMESPACE(Environment env);
    
    setDefault();
    
    if (storeType)
	datastore_type(storeType, env);

    if (type)
	typeName(type, env);

    if (id)
    {
	Uid u(id);
	_uid = u;
    }
}

Arjuna_ReplicatedPID::~Arjuna_ReplicatedPID ()
{
#ifdef DEBUG    
    cout << "Arjuna_ReplicatedPID::~Arjuna_ReplicatedPID ()" << endl;
#endif    
    
    if (_name)
	::delete [] _name;

    if (_datastoreType)
	::delete [] _datastoreType;
}

const char* Arjuna_ReplicatedPID::pidName ()
{
    return "ArjunaReplicatedPersistenceService";
}

CosPersistencePID::PID_ptr Arjuna_ReplicatedPID::create ()
{
    return new Arjuna_ReplicatedPID();
}

CosPersistencePID::PID_ptr Arjuna_ReplicatedPID::create (const char* string)
{
    return new Arjuna_ReplicatedPID(string);
}

void Arjuna_ReplicatedPID::setDefault ()
{
    _datastoreType = ::strdup(FragmentedStoreType);
    _name = ::strdup("/NoTypeName");
}

char* Arjuna_ReplicatedPID::uid (CORBA_NAMESPACE(Environment& env))
{
    char* toReturn = ::new char[UidSize];
    ::memset(toReturn, '\0', UidSize);

    ostrstream output(toReturn, UidSize);

    output << _uid;

    return toReturn;
}

void Arjuna_ReplicatedPID::uid (const char* uid, CORBA_NAMESPACE(Environment& env))
{
    Uid u(uid);
    
    _uid = u;
}

char* Arjuna_ReplicatedPID::typeName (CORBA_NAMESPACE(Environment& env))
{
    char* toReturn = ::strdup(_name);
    
    return toReturn;
}

void Arjuna_ReplicatedPID::typeName (const char* type, CORBA_NAMESPACE(Environment& env))
{
    if (_name)
	::delete [] _name;

    _name = ::strdup(type);
}

void Arjuna_ReplicatedPID::datastore_type (const char* type, CORBA_NAMESPACE(Environment& env))
{
    if (_datastoreType)
	::delete [] _datastoreType;

    _datastoreType = ::strdup(type);
}

char* Arjuna_ReplicatedPID::datastore_type (CORBA_NAMESPACE(Environment& env))
{
    char* toReturn = ::strdup(_datastoreType);

    return toReturn;
}

char* Arjuna_ReplicatedPID::get_ReplicatedPIDString (CORBA_NAMESPACE(Environment& env))
{
    /*
     * Build string.
     * <Arjuna_ReplicatedPID>:<Store type>:<TypeName>:<Uid>
     */

    char* toReturn = ::new char[::strlen(Arjuna_ReplicatedPID::pidName())+::strlen(_datastoreType)+::strlen(_name)+UidSize+4];
    ::memset(toReturn, '\0', ::strlen(Arjuna_ReplicatedPID::pidName())+::strlen(_datastoreType)+::strlen(_name)+UidSize+4);

    ::strcpy(toReturn, Arjuna_ReplicatedPID::pidName());
    ::strcat(toReturn, _separator);
    ::strcat(toReturn, _datastoreType);
    ::strcat(toReturn, _separator);
    ::strcat(toReturn, _name);
    ::strcat(toReturn, _separator);

    char* ptr = uid(env);

    ::strcat(toReturn, ptr);
    
    ::delete [] ptr;

    return toReturn;
}
