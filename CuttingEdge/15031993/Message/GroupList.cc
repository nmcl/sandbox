#ifndef GROUPLIST_H_
#  include "GroupList.h"
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif

GroupList::GroupList (int number)
{
    newgroup = 0;
    objects = new OBJECT_DESC[number +1];
    
    for (int i = 0; i < number; i++)
    {
	objects[i].destn_host = 0;
	objects[i].object_name[0] = '\0';
	objects[i].u_id.host = 0;
	objects[i].u_id.sec = 0;
	objects[i].u_id.msec = 0;
    }

    numberOfServers = 0;
    total = number;
    status = UNINITIATED;
}

GroupList::~GroupList ()
{
    if (objects)
	delete objects;

    if (newgroup)
	delete newgroup;

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	error_stream << WARNING << " ClientRajdoot_rpc::~ClientRajdoot_rpc : Call on active servers (Will terminate)\n";
	Client C;
	(void)C.Terminate(*this);
    }
}

Boolean GroupList::addName (const char* serviceName, const char* hostName, int location)
{
    Boolean result = FALSE, overWrite = FALSE;
    host_address Host;
    int toSet = 0;
    
    if (location != -1)
    {
	overWrite = TRUE;
	toSet = location;
    }
    else
	toSet = numberOfServers;
    
    if ((toSet <= total) && (toSet >= 0) && (serviceName))
    {
	if (::strlen(serviceName) >= OBJECT_NAME_SIZE)
	    error_stream << WARNING << " ClientRajdoot_rpc::SetServiceName() : Service name to long.\n";
	else
	{
	    ::strcpy(objects[toSet].object_name, serviceName);
	    
	    if (!hostName)
	    {
		Host.real_host = new char[MAXHOSTNAMELEN];
		::memset(Host.real_host, '\0', MAXHOSTNAMELEN);
		
		if (::gethostname(Host.real_host, MAXHOSTNAMELEN) == -1)
		{
		    error_stream << WARNING << " ClientRajdoot_rpc::SetHost() : Unknown local host name?\n";
		    return FALSE;
		}
	    }
	    else
	    {
		Host.real_host = new char[strlen(hostName)+1];
		::strcpy(Host.real_host, hostName);
	    }
	    
	    if (getloghost(&Host) == SUCCEEDED)
	    {
		objects[toSet].destn_host = Host.log_host;
		delete Host.real_host;
		result = TRUE;

		if (!overWrite)
		    numberOfServers++;
	    }
	    else
		error_stream << WARNING << " ClientRajdoot_rpc::SetHost() : Unknown host name\n";
	}
    }
    
    return result;
}

Boolean GroupList::addName (const char* serviceName, const char* hostName)
{
    return addName(serviceName, hostName, -1);
}

Boolean GroupList::modifyName (const char* serviceName, int location, const char* hostName)
{
    return addName(serviceName, hostName, location);
}

OBJECT_DESC* GroupList::getServerList () const
{
    return objects;
}

GROUPS* GroupList::getGroup ()
{
    if (!newgroup)
	newgroup = new GROUPS;
    
    return newgroup;
}

ServerStatus GroupList::getServerStatus () const
{
    return status;
}

void GroupList::setServerStatus (ServerStatus newStatus)
{
    status = newStatus;
}

Boolean GroupList::pack (Buffer& buff) const
{
    Boolean result = TRUE;

    if (status != OPERATIONAL)
	return FALSE;
    
    for (int i = 0; ((i < numberOfServers) && result); i++)
    {
	result = buff.pack(numberOfServers);
	result = buff.pack(objects[i].destn_host);
	result = result && buff.pack(objects[i].object_name);
	result = result && buff.pack(objects[i].u_id.host);
	result = result && buff.pack(objects[i].u_id.sec);
	result = result && buff.pack(objects[i].u_id.msec);
	result = result && buff.pack(objects[i].grps_id.groups_type);
	result = result && buff.pack(objects[i].grps_id.grp_srvr.srvr.sr_idtype);
	result = result && buff.pack(objects[i].grps_id.grp_srvr.srvr.sr_id.type);
	result = result && buff.pack(objects[i].grps_id.grp_srvr.srvr.sr_id.hi_part);
	result = result && buff.pack(objects[i].grps_id.grp_srvr.srvr.sr_id.lo_part);
    }
    
    return result;
}

Boolean GroupList::unpack (Buffer buff)
{
    Boolean result = TRUE;
    int temp;
    
    if (status == OPERATIONAL)
	return FALSE;

    result = buff.unpack(temp);
    
    if (result)
    {
	if (objects)
	    delete objects;
	
	numberOfServers = temp;
	objects = new OBJECT_DESC[numberOfServers+1];
	
	for (int i = 0; ((i < numberOfServers) && result); i++)
	{
	    char* name = 0;
	    
	    result = buff.unpack(objects[i].destn_host);

	    result = result && buff.unpack(name);
	    ::memset(objects[i].object_name, '\0', MAXHOSTNAMELEN);
	    ::memcpy(objects[i].object_name, name, strlen(name));
	    delete name;

	    result = result && buff.unpack(objects[i].u_id.host);
	    result = result && buff.unpack(objects[i].u_id.sec);
	    result = result && buff.unpack(objects[i].u_id.msec);
	    result = result && buff.unpack(objects[i].grps_id.groups_type);
	    result = result && buff.unpack(objects[i].grps_id.grp_srvr.srvr.sr_idtype);
	    result = result && buff.unpack(objects[i].grps_id.grp_srvr.srvr.sr_id.type);
	    result = result && buff.unpack(objects[i].grps_id.grp_srvr.srvr.sr_id.hi_part);
	    result = result && buff.unpack(objects[i].grps_id.grp_srvr.srvr.sr_id.lo_part);
	}
    }
    
    return result;
}
