/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRaj.cc,v 1.22 1996/10/09 13:19:53 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClRaj.cc,v 1.22 1996/10/09 13:19:53 nmcl Exp $";

/*
 * This class provides C++ interface to the client side of the
 * Rajdoot rpc protocol.
 *
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif 

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef RAJDOOT_CLRAJ_H_
#  include <RPC/Rajdoot/ClRaj.h>
#endif

#define UNKNOWNSERVICE          "TheUnknownService"

//
//	Fundermental generic operstions provided by
//	the RPC interface.
//

ClientRajdoot_rpc::ClientRajdoot_rpc ( const char* serviceName )
				     : status(UNINITIATED),
				       numOfServers(DEFAULT_NUMBEROFSERVERS),
				       retry(DEFAULT_RETRY),
				       timeout(DEFAULT_TIMEOUT),
				       burstSize(0),
				       burstTime(0),
				       objects(0),
				       defaultServiceName(0),
				       callGroup(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::ClientRajdoot_rpc( "
		 << serviceName << " )\n" << flush;
    debug_stream.unlock();
    
#endif

    if (serviceName == NULL)
    {
	defaultServiceName = new char[strlen(UNKNOWNSERVICE) + 1];
	strcpy(defaultServiceName, UNKNOWNSERVICE);
    }
    else
    {
	defaultServiceName = new char[strlen(serviceName) + 1];
	strcpy(defaultServiceName, serviceName);
    }

    setNumberOfServers();		// Will cause initialisation of "objects".


    actionId.type      		= 0;
    actionId.hi_part   		= 0;
    actionId.lo_part   		= 0;

    callFilter.filt_name     	= NULL;
    callFilter.no_successes  	= numOfServers;   /* ??? */
    callFilter.compare       	= compare_NONE;

    actionGroupFilter.filt_name     = NULL;
    actionGroupFilter.no_successes  = numOfServers;   /* ??? */
    actionGroupFilter.compare       = compare_NONE;
}

ClientRajdoot_rpc::~ClientRajdoot_rpc ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::~ClientRajdoot_rpc()\n" << flush;
    debug_stream.unlock();
    
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	error_stream << WARNING 
		     << " ClientRajdoot_rpc::~ClientRajdoot_rpc : Call on active servers (Will terminate)\n";
	terminate();
    }

    if (status != SOFTDEAD)
    {
	if (objects != NULL)
	    delete [] objects;

	if (defaultServiceName != NULL)
	    delete [] defaultServiceName;

	if (callGroup != NULL)
	    delete callGroup;
    }
}

RPC_Status ClientRajdoot_rpc::initiate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::initiate()\n" << flush;
    debug_stream.unlock();
#endif

    int successes = numOfServers;
    int rcode;

    if (status == UNINITIATED)
    {
	callGroup = new GROUPS;
	callGroup->grp_srvr.group.head_gd = 0;
	callGroup->groups_type = 0;

	setActionID(actionId);
	actionId.type = actionId.hi_part = actionId.lo_part = 0;
	rcode = ::initiate(objects, callGroup, &successes);
	
	if (rcode == SUCCEEDED)
	{
	    status = OPERATIONAL;
	    
	    return OPER_DONE;
	}
	else
	{
	    char* hostName = (char*) 0;
	    char* serviceName = (char*) 0;

	    getHost(hostName);
	    getServiceName(serviceName);

	    if ((rcode == RPC_UNABLE) || (rcode == OPENFAILED) || (rcode == PATH_TOO_LONG))
	    {
		if (rcode == RPC_UNABLE)
		{
			error_stream << WARNING << " ClientRajdoot_rpc::Initiate() : virtual failure for "
				     << serviceName << " on " << hostName << endl;
		
			status = INIT_UNABLE;
	
			if (hostName)
			    ::delete [] hostName;

			if (serviceName)
			    ::delete [] serviceName;
		
			return OPER_UNKNOWN;
		}
		else
		{
		    error_stream << WARNING
				 << "ClientRajdoot_rpc::initiate () : parameter errors. Opcode: "
				 << rcode << endl;
		
			status = INIT_UNABLE;
	
			if (hostName)
			    ::delete [] hostName;

			if (serviceName)
			    ::delete [] serviceName;
		
			return OPER_UNKNOWN;
		}
	    }
	    else
	    {
		error_stream << WARNING << " ClientRajdoot_rpc::Initiate() : Failed for "
			     << serviceName << " on " << hostName << endl;
		
		delete callGroup;
		callGroup = NULL;

		if (hostName)
		    ::delete [] hostName;

		if (serviceName)
		    ::delete [] serviceName;
		
		return OPER_NOTDONE;
	    }
	}
    }

    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::call(Int32  opcode,  Buffer& callBuff,
				   Int32& errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::call(Int32  opcode,  Buffer& callBuff, Int32& errcode, Buffer& result)\n" << flush;
    debug_stream.unlock();
    
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL) || (status == UNABLE))
    {
	RPC in, out;
	int rcode;

	in.rpc_code    = opcode;
	in.rpc_length1 = callBuff.length();
	in.rpc_buff1   = (char *)callBuff.buffer();
	in.rpc_length2 = 0;
	in.rpc_buff2   = NULL;

	setBurstSize(burstSize);
	setBurstTime(burstTime);

	rcode = multi_call(callGroup, &in, &out, timeout, retry, &callFilter, objects);
	
	if (rcode == SUCCEEDED)
	{
	    errcode = out.rpc_code;
	    result.setBuffer((size_t) out.rpc_length1, out.rpc_buff1);
	    status = OPERATIONAL;

	    return OPER_DONE;
	}
	else
	{
	    if (rcode == RPC_UNABLE)
	    {
		status = UNABLE;
		return OPER_UNKNOWN;
	    }
	    else
	    {
		status = INOPERATIONAL;
		return OPER_NOTDONE;
	    }
	}
    }

    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::terminate()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::terminate()\n" << flush;
    debug_stream.unlock();
    
#endif

    	if ((status == OPERATIONAL) || (status == INOPERATIONAL) ||
	    (status == UNABLE) || (status == INIT_UNABLE))
    	{
		status = DEAD;
         	::terminate(callGroup, (Boolean) FALSE, objects);

         	return OPER_DONE;
    	}
    	else if (status == UNINITIATED)
    	{
		error_stream << WARNING << " ClientRajdoot_rpc::terminate() : Call on inactive servers.\n";

         	return OPER_NOTDONE;
    	}
    	else if (status == DEAD)
	{
	    error_stream << WARNING << " ClientRajdoot_rpc::terminate() : Call on DEAD servers.\n";
	    return OPER_NOTDONE;
	}
	else
    	{
		error_stream << WARNING << " ClientRajdoot_rpc::terminate() : Call on (soft) dead servers.\n";

         	return OPER_NOTDONE;
    	}
}

//
//	Additional operations provided by the RPC interface.
//

ServerStatus ClientRajdoot_rpc::getServerStatus()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::getServerStatus()\n" << flush;
    debug_stream << "ClientRajdoot_rpc::server status is " << status << "\n" << flush;
    debug_stream.unlock();
#endif
  	return status;
}

RPC_Status ClientRajdoot_rpc::call(Int32  opcode,  Buffer& call1,   Buffer& call2,
		                   Int32& errcode, Buffer& result1, Buffer& result2)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::call(Int32  opcode, Buffer& call1, Buffer& call2, Int32& errcode, Buffer& result1, Buffer& result2))\n" << flush;
    debug_stream.unlock();
    
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL) || (status == UNABLE))
    {
	RPC in, out;
	int rcode;

	in.rpc_code    = opcode;
	in.rpc_length1 = call1.length();
	in.rpc_buff1   = (char *)call1.buffer();
	in.rpc_length2 = call2.length();
	in.rpc_buff2   = (char *)call2.buffer();

	setBurstSize(burstSize);
	setBurstTime(burstTime);

#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::call\ncall buffer1 = " << call1  << "\ncall buffer2 " << call2 << "\n" << flush;
	debug_stream.unlock();
	
#endif

	rcode = multi_call(callGroup, &in, &out, timeout, retry, &callFilter, objects);
	
	if (rcode == SUCCEEDED)
	{
	    errcode = out.rpc_code;
	    result1.setBuffer((size_t) out.rpc_length1, out.rpc_buff1);
	    result2.setBuffer((size_t) out.rpc_length2, out.rpc_buff2);
	    status = OPERATIONAL;

	    return OPER_DONE;
	}
	else
	{
	    if (rcode == RPC_UNABLE)
	    {
		status = UNABLE;
		return OPER_UNKNOWN;
	    }
	    else
	    {
		status = INOPERATIONAL;
		return OPER_NOTDONE;
	    }
	}
    }

    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setAllToDefault()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setAllToDefault()\n" << flush;
    debug_stream.unlock();
    
#endif

   	if (status == UNINITIATED)
    	{
	    if (objects != NULL)
		delete [] objects;

	    objects = new OBJECT_DESC[numOfServers + 1];
	    
	    for (int numSer = 0; numSer < numOfServers; numSer++)
		if (setToDefault(numSer) != OPER_DONE)
		    return OPER_UNKNOWN;

	    objects[numOfServers].destn_host     = 0;
	    objects[numOfServers].object_name[0] = '\0';
	    objects[numOfServers].u_id.host      = 0;
	    objects[numOfServers].u_id.sec       = 0;
	    objects[numOfServers].u_id.msec      = 0;
	    
	    return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setToDefault(int serverNum)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setToDefault(int serverNum)\n" << flush;
    debug_stream.unlock();
    
#endif

   	if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
    	{
	    if (setHost(NULL, serverNum) != OPER_DONE)
		return OPER_UNKNOWN;

	    if (setServiceName(NULL, serverNum) != OPER_DONE)
		return OPER_UNKNOWN;

	    objects[serverNum].u_id.host      = 0;
	    objects[serverNum].u_id.sec       = 0;
	    objects[serverNum].u_id.msec      = 0;

	    return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setObjectUid (UNIQUEID rUid)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::setObjectUid (UNIQUEID rUid)" << endl;
    debug_stream.unlock();
    
#endif

    if (status == UNINITIATED)
    {
	for (int i = 0; i < numOfServers; i++)
	{
	    objects[i].u_id.host = rUid.host;
	    objects[i].u_id.sec = rUid.sec;
	    objects[i].u_id.msec = rUid.msec;
	}
	
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setActionID (const MID& actID)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Statuc ClientRajdoot_rpc::setActionID (const MID& actID)" << endl;
    debug_stream.unlock();
    
#endif

    if (status == UNINITIATED)
    {
	actionId = actID;
	return OPER_DONE;
    }

    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setBurstSize (Uint32 setTo)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::setBurstSize ( " << setTo << " )" << endl;
    debug_stream.unlock();
    
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	burstSize = setTo;
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::getBurstSize (Uint32& setTo) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::getBurstSize (Uint32& setTo) const" << endl;
    debug_stream.unlock();
    
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	setTo = burstSize;
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setBurstTime (Uint32 setTo)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::setBurstTime ( " << setTo << " )" << endl;
    debug_stream.unlock();
    
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	burstSize = setTo;
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::getBurstTime (Uint32& setTo) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::getBurstTime (Uint32& setTo) const" << endl;
    debug_stream.unlock();
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	setTo = burstTime;
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setNumberOfServers(int numberOfServers)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setNumberOfServers(int numberOfServers)\n" << flush;
    debug_stream.unlock();
#endif

  	if ((status == UNINITIATED) && (numberOfServers > 0))
	{
		numOfServers 			= numberOfServers;
    		callFilter.no_successes 	= numberOfServers;   /* ??? */
    		actionGroupFilter.no_successes 	= numberOfServers;   /* ??? */
		
		return setAllToDefault();
        }

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::getNumberOfServers(int& numberOfServers)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::getNumberOfServers(int& numberOfServers)\n" << flush;
    debug_stream.unlock();
    
#endif

  	numberOfServers = numOfServers;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::setServiceName ( const char* const serviceName, int serverNum )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setServiceName(char* serviceName, int serverNum)\n" << flush;
    debug_stream.unlock();
    
#endif

   	if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
	{
    		if (serviceName == NULL)
		{
		    if (defaultServiceName == NULL)
			objects[serverNum].object_name[0] = '\0';
		    else
			strcpy(objects[serverNum].object_name, defaultServiceName);
    		}
		else
    		{
		    if (strlen(serviceName) >= OBJECT_NAME_SIZE)
		    {
			error_stream << WARNING << " ClientRajdoot_rpc::setServiceName() : Service name too long." << endl;
			return OPER_NOTDONE;
		    }
		    else
			strcpy(objects[serverNum].object_name, serviceName);
    		}

		return OPER_DONE;
  	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::getServiceName(char*& serviceName, int serverNum)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::getServiceName(char*& serviceName, int serverNum)\n" << flush;
    debug_stream.unlock();
    
#endif

   	if ((serverNum >= 0) && (serverNum < numOfServers))
	{
    		if (serviceName != NULL)
			delete [] serviceName;

		serviceName = new char[strlen(objects[serverNum].object_name) + 1];
		strcpy(serviceName, objects[serverNum].object_name);

		return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setHost ( const char* const hostName, int serverNum)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setHost(char* hostName, int serverNum)\n" << flush;
    debug_stream.unlock();
    
#endif

   	if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
	{
    		host_address Host;

    		if (hostName == NULL)
    		{
		    Host.real_host = new char[MAXHOSTNAMELEN];

		    if (gethostname(Host.real_host, MAXHOSTNAMELEN) == -1)
		    {
			::delete [] Host.real_host;	    
			error_stream << WARNING << " ClientRajdoot_rpc::setHost() : Unknown local host name?\n";
			return OPER_NOTDONE;
		    }
    		}
    		else
    		{
		    Host.real_host = new char[strlen(hostName)+1];
		    strcpy(Host.real_host, hostName);
    		}

    		if (getloghost(&Host) == SUCCEEDED)
		{
		    objects[serverNum].destn_host = Host.log_host;
		    ::delete [] Host.real_host;
		}
		else
		{
		    ::delete [] Host.real_host;		    
		    error_stream << WARNING << " ClientRajdoot_rpc::setHost() : Unknown host name " << Host.real_host << endl;
		    return OPER_NOTDONE;
        	}

		return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::getHost(char*& hostName, int serverNum)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::getHost(char*& hostName, int serverNum)\n" << flush;
    debug_stream.unlock();
    
#endif

   	if ((serverNum >= 0) && (serverNum < numOfServers))
	{
    		host_address Host;

		if (hostName != NULL)
		  	delete [] hostName;

		Host.log_host = objects[serverNum].destn_host;

		if (getrealhost(&Host) == SUCCEEDED)
		{
			hostName = new char[strlen(Host.real_host) + 1];
			strcpy(hostName, Host.real_host);
			::delete [] Host.real_host;
		}

		return OPER_UNKNOWN;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::setTimeout(Int32 tout)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setTimeout(Int32 tout)\n" << flush;
    debug_stream.unlock();
    
#endif

  	if (tout < MINIMUM_TIMEOUT)
        {
	  	error_stream << WARNING << " ClientRajdoot_rpc::setTimeout : Timeout below minimum (no thing done)" << endl;
		return OPER_NOTDONE;
        }
	

  	if (tout > MAXIMUM_TIMEOUT)
        {
	  	error_stream << WARNING << " ClientRajdoot_rpc::setTimeout : Timeout above maximum (nothing done)" << endl;
		return OPER_NOTDONE;
        }
	

	timeout = tout;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::getTimeout(Int32& tout)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::getTimeout(Int32& tout)\n" << flush;
    debug_stream.unlock();
    
#endif

	tout = timeout;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::setRetry(Int32  rty)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::setRetry(Int32  rty)\n" << flush;
    debug_stream.unlock();
    
#endif

  	if (rty < MINIMUM_RETRY)
		return OPER_NOTDONE;

        retry = rty;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::getRetry(Int32& rty)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::getRetry(Int32& rty)\n" << flush;
    debug_stream.unlock();
    
#endif

	rty = retry;

	return OPER_DONE;
}

/*
 * This version pings the machine, rather than the server. In
 * later versions this will be corrected.
 */

Boolean ClientRajdoot_rpc::ping (int serverNum)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::ping ()\n" << flush;
    debug_stream.unlock();
    
#endif

    Uint32 timeTaken = 0;
    
    if ((serverNum >= 0) && (serverNum < numOfServers))
	return ::ping(objects[serverNum].destn_host, timeTaken);
    else
    {
	error_stream << WARNING << "ClientRajdoot_rpc::ping - invalid server number " << serverNum << endl;
	return FALSE;
    }
}

//
//	Additional operations provided by the RPC interface,
//	to allow atomic actions.
//

ClientRajdoot_rpc::ClientRajdoot_rpc(ClientRajdoot_rpc* doner)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpc::ClientRajdoot_rpc(ClientRajdoot_rpc* doner)\n" << flush;
    debug_stream.unlock();
    
#endif

	if (doner == NULL)
	{
	   	error_stream << WARNING << "ClientRajdoot_rpc::ClientRajdoot_rpc() : NULL doner\n";
		return;
	}

	if (doner->getServerStatus() != SOFTDEAD)
	{
	   	error_stream << WARNING << "ClientRajdoot_rpc::ClientRajdoot_rpc() : doner is not SOFTDEAD\n";
		return;
	}

    	status 			= OPERATIONAL;

	numOfServers		= doner->numOfServers;
  	defaultServiceName      = doner->defaultServiceName;
	objects                 = doner->objects;

    	retry                   = doner->retry;
  	timeout                 = doner->timeout;
	burstSize               = doner->burstSize;
	burstTime               = doner->burstTime;

	callGroup 		= doner->callGroup;
    	actionId 		= doner->actionId;
    	callFilter 		= doner->callFilter;
    	actionGroupFilter 	= doner->actionGroupFilter;
}

RPC_Status ClientRajdoot_rpc::softTerminate()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PROTECTED;
    debug_stream << "ClientRajdoot_rpc::softTerminate()\n" << flush;
    debug_stream.unlock();
    
#endif
    
    char* hostName = (char*) 0;
    char* serviceName = (char*) 0;

    getHost(hostName);
    getServiceName(serviceName);
	    
    error_stream << WARNING << "ClientRajdoot_rpc::softTerminate called for " << serviceName
		 << " on machine " << hostName << endl;

    if (hostName)
	::delete [] hostName;

    if (serviceName)
	::delete [] serviceName;
    
    if ((status == OPERATIONAL) || (status == INOPERATIONAL) ||
	(status == INIT_UNABLE) || (status == UNABLE))
    {
	status = SOFTDEAD;
	
	return OPER_DONE;
    }
    else if (status == UNINITIATED)
    {
	error_stream << WARNING << " ClientRajdoot_rpc::softTerminate() : Call on inactive servers.\n";
	
	return OPER_NOTDONE;
    }
    else if (status == DEAD)
    {
	error_stream << WARNING << " ClientRajdoot_rpc::softTerminate() : Call on DEAD servers.\n";
	
	return OPER_NOTDONE;
    }
    else
    {
	error_stream << WARNING << " ClientRajdoot_rpc::softTerminate() : Call on (soft) dead servers.\n";
	
	return OPER_NOTDONE;
    }
}
