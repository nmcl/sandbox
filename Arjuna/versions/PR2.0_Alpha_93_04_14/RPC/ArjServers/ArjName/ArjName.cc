/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ArjName.cc,v 1.16 1993/05/12 11:29:50 nmcl Exp $
 */

static const char RCSid[] = "$Id: ArjName.cc,v 1.16 1993/05/12 11:29:50 nmcl Exp $";

/*
 * Arjuna Name Implementation
 *
 */

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
#  include <Configure.h>
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

#ifndef ARJSERVERS_ARJNAME_H_
#  include <RPC/ArjServers/ArjName.h>
#endif

#ifndef NSINTERFACE_H_
#  include <RPC/ArjServers/NSInterface.h>
#endif

#ifndef RAJDOOT_SVRAJ_H_
#  include <RPC/Rajdoot/SvRaj.h>
#endif

/*
 * Public constructors and destructor
 */

ArjunaName::ArjunaName ()
                       : ObjName(0),
			 ObjUid (NIL_UID),
			 ServiceName(0),
			 HostName(0),
			 NS(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::ArjunaName()" << "\n" << flush; 
#endif
}

ArjunaName::ArjunaName (const char* Name)
                       : ObjName(0),
			 ObjUid (NIL_UID),
			 ServiceName(0),
			 HostName(0),
			 NS(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::ArjunaName(const char* Name)\n" << flush;
#endif
    SetObjName(Name);
    LookUp();
}

ArjunaName::~ArjunaName ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::~ArjunaName()" << "\n" << flush;
#endif
   if (ObjName != 0)
	delete ObjName;

    if (ServiceName != 0)
	delete ServiceName;

    if (HostName != 0)
	delete HostName;
    
    if (NS)
	delete NS;
}

/* Name Server Operations */

void ArjunaName::LookUp()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::LookUp()" << "\n" << flush;
#endif
    Boolean       done;
    int res = -1;

    if (!NS)
	NS = new NSInterface(res);

    done = (Boolean) (res == 0);

    if (done)
    {
	Buffer key, data;

	done = pack(key);
	if (done)
	{
	    done = FALSE;
	    NS->FetchName(done, key, data);
	    if (done)
		done = unpack(data);
	    else
		error_stream << "Name server \"LookUp\" Failed\n";
	}
    }
}

void ArjunaName::Register()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::Register()" << "\n" << flush;
#endif
    Boolean done;
    int res = -1;

    /* This code is like this because g++2.1 core dumps if its not! */

    if (!NS)
	NS = new NSInterface(res);

    done = (Boolean) (res == 0);

    if (done)
    {
	Buffer key, data;
	ArjunaName RN;

	RN.SetObjName(GetRefObjName());

	RN.pack(key);
	pack(data);

	done = FALSE;
	NS->StoreName(done, key, data);
	if (! done)
		error_stream << "Name server \"Register\" Failed\n";
    }
}


void ArjunaName::Register(const char* name, StateManager* obj)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::Register(const char*, StateManager*)" << "\n" << flush;
#endif
    SetObjName(name);
    SetObjUid(obj->get_uid());
    SetServiceName(ServerRpc::ServerName);

    char hostName[64];
    gethostname(hostName, 64);
    SetHostName(hostName);

    Register();
}

/* Set Operations */

void ArjunaName::SetObjName(const char* Name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::SetObjName(const char* Name)" << "\n" << flush;
#endif
    if (ObjName != 0)
	delete ObjName;

    if (Name == 0)
	ObjName = 0;
    else
    {
	ObjName  = new char[strlen(Name) + 1];
	strcpy(ObjName, Name);
    }
}

void ArjunaName::SetObjUid(const Uid& U)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::SetObjUid(const Uid& U)" << "\n" << flush;
#endif
    ObjUid = U;
}

void ArjunaName::SetServiceName(const char* SN)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::SetServiceName(const char* SN)" << "\n" << flush;
#endif
    if (ServiceName != 0)
	delete ServiceName;

    if (SN == 0)
	ServiceName = 0;
    else
    {
	ServiceName  = new char[strlen(SN) + 1];
	strcpy(ServiceName, SN);
    }
}

void ArjunaName::SetHostName(const char* HN)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::SetHostName(const char* HN)" << "\n" << flush;
#endif
    if (HostName != 0)
	delete HostName;

    if (HN == 0)
	HostName = 0;
    else
    {
	HostName  = new char[strlen(HN) + 1];
	strcpy(HostName, HN);
    }
}

/* Get Operations */

char* ArjunaName::GetObjName()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetObjName()" << "\n" << flush;
#endif
    char* res;

    if (ObjName == 0)
	res = 0;
    else
    {
	res  = new char[strlen(ObjName) + 1];
	strcpy(res, ObjName);
    }

    return res;
}

Uid ArjunaName::GetObjUid()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetObjUid()" << "\n" << flush;
#endif
    return ObjUid;
}

char* ArjunaName::GetServiceName()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetServiceName()" << "\n" << flush;
#endif
    char* res;

    if (ServiceName == 0)
	res = 0;
    else
    {
	res  = new char[strlen(ServiceName) + 1];
	strcpy(res, ServiceName);
    }

    return res;
}

char* ArjunaName::GetHostName()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetHostName()" << "\n" << flush;
#endif
    char* res;

    if (HostName == 0)
	res = 0;
    else
    {
	res  = new char[strlen(HostName) + 1];
	strcpy(res, HostName);
    }

    return res;
}

/* Get Operations */

char* ArjunaName::GetRefObjName()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetRefObjName()" << "\n" << flush;
#endif
    return ObjName;
}

Uid ArjunaName::GetRefObjUid()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetRefObjUid()" << "\n" << flush;
#endif
    return ObjUid ;
}

char* ArjunaName::GetRefServiceName()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetRefServiceName()" << "\n" << flush;
#endif
    return ServiceName;
}

char* ArjunaName::GetRefHostName()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::GetRefHostName()" << "\n" << flush;
#endif
    return HostName;
}

/* Pack & Unpack Operations */

Boolean ArjunaName::pack (Buffer& buff) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::pack()" << "\n" << flush;
#endif
    Boolean done = FALSE;
    
    if (ObjName != 0)
	done = (buff.pack((int) FALSE) && buff.pack(ObjName));
    else
	done = buff.pack((int) TRUE);

    if (! done)
	return FALSE;
    
    done = ObjUid.pack(buff);
    
    if (! done)
	return FALSE;
    
    if (ServiceName != 0)
	done = (buff.pack((int) FALSE) && buff.pack(ServiceName));
    else
	done = buff.pack((int) TRUE);
    
    if (! done)
	return FALSE;
    
    if (HostName != 0)
	done = (buff.pack((int) FALSE) && buff.pack(HostName));
    else
	done =buff.pack((int) TRUE);

    return done;
}

Boolean ArjunaName::unpack (Buffer& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ArjunaName::unpack()" << "\n" << flush;
#endif
    int isNull = (int) FALSE;
    
    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (ObjName != 0)
	    {
		delete ObjName;
		ObjName = 0;
	    }
	}
	else
	    if (! buff.unpack(ObjName))
		return FALSE;
    }
    else
	return FALSE;

    if (!ObjUid.unpack(buff))
	return FALSE;

    
    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (ServiceName != 0)
	    {
		delete ServiceName;
		ServiceName = 0;
	    }
	}
	else
	    if (! buff.unpack(ServiceName))
		return FALSE;
    }
    else
	return FALSE;

    if (buff.unpack(isNull))
    {
	if (isNull)
	{
	    if (HostName != 0)
	    {
		delete HostName;
		HostName = 0;
	    }
	}
	else
	    if (! buff.unpack(HostName))
		return FALSE;
    }
    else
	return FALSE;

    return TRUE;
}

ostream& ArjunaName::print ( ostream& strm ) const
{
    if (ObjName == 0)
	strm << "{NULL";
    else 
	strm << "{" << ObjName;
    
    strm << ", " << (ObjUid);
    
    if (ServiceName == 0)
	strm << ", NULL";
    else 
	strm << ", " << ServiceName;
    
    if (HostName == 0)
	strm << ", NULL}";
    else 
	strm << ", " << HostName << "}";
    
    return strm;
}

ostream& operator<< ( ostream& strm, const ArjunaName& an )
{
    return an.print (strm);
}

Buffer& operator<<  ( Buffer& rpcbuff, const ArjunaName& topack )
{
    topack.pack (rpcbuff);
    return rpcbuff;
}

Buffer& operator>>  ( Buffer& rpcbuff, ArjunaName& tounpack )
{
    tounpack.unpack(rpcbuff);
    return rpcbuff;
}

Buffer& operator<<  ( Buffer& rpcbuff, ArjunaName* topack )
{
    topack->pack (rpcbuff);
    return rpcbuff;
}

Buffer& operator>>  ( Buffer& rpcbuff, ArjunaName*& tounpack )
{
    if (tounpack == 0)
	tounpack = new ArjunaName();
    tounpack->unpack(rpcbuff);
    return rpcbuff;
}

