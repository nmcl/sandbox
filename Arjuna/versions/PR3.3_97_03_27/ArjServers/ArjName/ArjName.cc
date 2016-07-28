/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ArjName.cc,v 1.13 1995/07/11 15:21:35 nmcl Exp $
 */

static const char RCSid[] = "$Id: ArjName.cc,v 1.13 1995/07/11 15:21:35 nmcl Exp $";

/*
 * Arjuna Name Implementation
 *
 */

/*
 * the following definition is required for the Register operation
 */

#define STATEMANAGER_SERVER

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef DEBUG_H_
#  ifdef DEBUG
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef SERVERRPC_H_
#  include <StubGen/ServerRpc.h>
#endif

#ifndef ARJSERVERS_ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

/*
 * Public constructors and destructor
 */

ArjunaName::ArjunaName ()
                       : objectName(0),
			 objectUid (NIL_UID),
			 serviceName(0),
			 hostName(0),
			 OSRoot(0),
			 NShandle(0),
			 _valid(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::ArjunaName()" << "\n" << flush; 
#endif
}

ArjunaName::ArjunaName ( const char* const name )
                       : objectName(0),
			 objectUid (NIL_UID),
			 serviceName(0),
			 hostName(0),
			 OSRoot(0),
			 NShandle(0),
			 _valid(FALSE)			 
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::ArjunaName(const char* Name)\n" << flush;
#endif

    setObjectName(name);
    (void) lookUp();
}

ArjunaName::~ArjunaName ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::~ArjunaName()" << "\n" << flush;
#endif
    
   if (objectName != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] objectName;
#else
        delete objectName;
#endif

    if (serviceName != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] serviceName;
#else
        delete serviceName;
#endif

    if (hostName != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] hostName;
#else
        delete hostName;
#endif

    if (OSRoot != 0)
#ifndef GCC_ARRAY_DELETE_BUG
        delete [] OSRoot;
#else
        delete OSRoot;
#endif
    
    if (NShandle)
	delete NShandle;
}

/* Name Server Operations */

Boolean ArjunaName::valid () const
{
    return _valid;
}

Boolean ArjunaName::lookUp ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::lookUp ()" << "\n" << flush;
#endif
    
    Boolean done;
    int res = -1;

    _valid = FALSE;
    
    if (!NShandle)
	NShandle = new NSInterface(res);
    else
	res = 0;

    done = (res == 0 ? TRUE : FALSE);

    if (done)
    {
	Buffer key, data;

	done = pack(key);

	if (done)
	{
	    done = FALSE;
	    NShandle->fetchName(done, key, data);
	    
	    if (done)
	    {
		done = unpack(data);
		_valid = TRUE;
	    }
	    else
		error_stream << "Name server lookUp failed\n";
	}
    }

    return done;
}

Boolean ArjunaName::registerName ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::registerName()" << "\n" << flush;
#endif

    Boolean done;
    int res = -1;

    _valid = FALSE;
    
    /* This code is like this because g++2.1 core dumps if its not! */

    if (!NShandle)
	NShandle = new NSInterface(res);
    else
	res = 0;

    done = (res == 0 ? TRUE : FALSE);

    if (done)
    {
	Buffer key, data;
	ArjunaName RN;

	RN.setObjectName(getObjectName());

	RN.pack(key);
	pack(data);

	done = FALSE;
	NShandle->storeName(done, key, data);
	if (! done)
		error_stream << "Name server \"registerName\" Failed\n";
	else
	    _valid = TRUE;
    }

    return done;
}


Boolean ArjunaName::registerName ( const char* const name,
				   const StateManager& obj )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::registerName(const char* const, const StateManager&)" << "\n" << flush;
#endif

    setObjectName(name);
    setObjectUid(obj.get_uid());
    setServiceName(ServerRpc::getServerName());

    char thisHostName[MAXHOSTNAMELEN+1];
    ::memset(thisHostName, '\0', MAXHOSTNAMELEN+1);
    gethostname(thisHostName, MAXHOSTNAMELEN);
    setHostName(thisHostName);

    _valid = registerName();
    
    return _valid;
}

/* set Operations */

void ArjunaName::setObjectName (const char* const name )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::setObjectName(const char* const)" << "\n" << flush;
#endif
    
    if (objectName != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] objectName;
#else
        delete objectName;
#endif

    objectName = clone(name);
}

void ArjunaName::setObjectUid (const Uid& U )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::setObjectUid(const Uid& U)" << "\n" << flush;
#endif
    
    objectUid = U;
}

void ArjunaName::setServiceName( const char* const sName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::setServiceName(const char* sName)" << "\n" << flush;
#endif
    if (serviceName != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] serviceName;
#else
        delete serviceName;
#endif

    serviceName = clone(sName);
}

void ArjunaName::setHostName ( const char* const hName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::setHostName(const char* hName)" << "\n" << flush;
#endif
    if (hostName != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] hostName;
#else
        delete hostName;
#endif

    hostName = clone(hName);
}

void ArjunaName::setObjectStoreRoot ( const char* const root )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ArjunaName::setObjectStoreRoot ( " << root << " )" << endl;
#endif

    if (OSRoot)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] OSRoot;
#else
        ::delete OSRoot;
#endif    

    OSRoot = clone(root);
}
/* get Operations */

Uid ArjunaName::getObjectUid() const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::getObjectUid()" << "\n" << flush;
#endif
    
    return objectUid;
}

const char* const ArjunaName::getObjectName () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::getObjectName()" << "\n" << flush;
#endif
    
    return objectName;
}

const char* const ArjunaName::getServiceName () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::getServiceName()" << "\n" << flush;
#endif
    
    return serviceName;
}

const char* const ArjunaName::getHostName () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::getHostName()" << "\n" << flush;
#endif
    
    return hostName;
}

const char* const ArjunaName::getObjStoreRoot () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "char* ArjunaName::getObjStoreRoot ()" << endl;
#endif
    
    return OSRoot;
}

/* Pack & Unpack Operations */

Boolean ArjunaName::pack (Buffer& buff) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::pack()" << "\n" << flush;
#endif
    Boolean done = FALSE;
    
    if (objectName != 0)
	done = (buff.pack(FALSE) && buff.packMappedString(objectName));
    else
	done = buff.pack(TRUE);

    if (! done)
	return FALSE;
    
    done = objectUid.pack(buff);
    
    if (! done)
	return FALSE;
    
    if (serviceName != 0)
	done = (buff.pack(FALSE) && buff.packMappedString(serviceName));
    else
	done = buff.pack(TRUE);
    
    if (! done)
	return FALSE;
    
    if (hostName != 0)
	done = (buff.pack(FALSE) && buff.packMappedString(hostName));
    else
	done = buff.pack(TRUE);

    if (OSRoot != 0)
	done = buff.pack(FALSE) && buff.packMappedString(OSRoot);
    else
	done = buff.pack(TRUE);

    return done;
}

Boolean ArjunaName::unpack (Buffer& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName::unpack()" << "\n" << flush;
#endif
    Boolean isNull = FALSE;

    _valid = FALSE;
    
    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (objectName != 0)
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		delete [] objectName;
#else
		delete objectName;
#endif
		objectName = 0;
	    }
	}
	else
	    if (! buff.unpack(objectName))
		return FALSE;
    }
    else
	return FALSE;

    if (!objectUid.unpack(buff))
	return FALSE;
    
    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (serviceName != 0)
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		delete [] serviceName;
#else
		delete serviceName;
#endif
		serviceName = 0;
	    }
	}
	else
	    if (! buff.unpack(serviceName))
		return FALSE;
    }
    else
	return FALSE;

    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (hostName != 0)
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		delete [] hostName;
#else
		delete hostName;
#endif
		hostName = 0;
	    }
	}
	else
	    if (! buff.unpack(hostName))
		return FALSE;
    }
    else
	return FALSE;

    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (OSRoot != 0)
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		::delete [] OSRoot;
#else
		::delete OSRoot;
#endif
		OSRoot = 0;
	    }
	}
	else
	    if (!buff.unpack(OSRoot))
		return FALSE;
    }
    else
	return FALSE;
	    
    _valid = TRUE;
    
    return TRUE;
}

ostream& ArjunaName::print ( ostream& strm ) const
{
    if (objectName == 0)
	strm << "{NULL";
    else 
	strm << "{" << objectName;
    
    strm << ", " << (objectUid);
    
    if (serviceName == 0)
	strm << ", NULL";
    else 
	strm << ", " << serviceName;
    
    if (hostName == 0)
	strm << ", NULL";
    else 
	strm << ", " << hostName;

    if (OSRoot == 0)
	strm << ", NULL}";
    else
	strm << ", " << OSRoot << "}";
    
    return strm;
}

ostream& operator<< ( ostream& strm, const ArjunaName& an )
{
    return an.print (strm);
}

/*
 * Pack this Arjuna name into an RpcBuffer.
 */

void ArjunaName::marshall ( RpcBuffer& buff ) const
{
    pack(buff);
}

/*
 * Unpack this from the arg buffer.
 */

void ArjunaName::unmarshall ( RpcBuffer& buff )
{
    unpack(buff);
}


