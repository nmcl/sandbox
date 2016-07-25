/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRaj.cc,v 1.1 1993/11/03 14:40:28 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClRaj.cc,v 1.1 1993/11/03 14:40:28 nmcl Exp $";

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

ClientRajdoot_rpc::ClientRajdoot_rpc(const char* serviceName)
{
#ifdef DEBUG
        debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::ClientRajdoot_rpc( "
	    << serviceName << " )\n" << flush;
#endif

    	status 				= UNINITIATED;

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

	objects = NULL;

    	SetNumberOfServers();		// Will cause initialisation of "objects".

    	SetRetry();
  	SetTimeout();

	callGroup 			= NULL;

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

ClientRajdoot_rpc::~ClientRajdoot_rpc()
{
#ifdef DEBUG
        debug_stream << DESTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::~ClientRajdoot_rpc()\n" << flush;
#endif

    	if ((status == OPERATIONAL) || (status == INOPERATIONAL))
	{
	  	error_stream << WARNING << " ClientRajdoot_rpc::~ClientRajdoot_rpc : Call on active servers (Will terminate)\n";
		Terminate();
	}

	if (status != SOFTDEAD)
        {
	    	if (objects != NULL)
        		delete objects;

    		if (defaultServiceName != NULL)
        		delete defaultServiceName;

    		if (callGroup != NULL)
        		delete callGroup;
     	}
}

RPC_Status ClientRajdoot_rpc::Initiate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::Initiate()\n" << flush;
#endif

  	int successes = numOfServers;

  	if (status == UNINITIATED)
        {
	  	callGroup = new GROUPS;

		if (initiate(objects, callGroup, &successes) == SUCCEEDED)
		{
		  	status = OPERATIONAL;

        		return OPER_DONE;
		}
    		else
		{
        		error_stream << WARNING << " ClientRajdoot_rpc::Initiate() : Failed\n";

    			delete callGroup;
    			callGroup = NULL;

    			return OPER_UNKNOWN;
		}
	}

    	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::Call(long  opcode,  Buffer& call,
				   long& errcode, Buffer& result)
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::Call(long  opcode,  Buffer& call, long& errcode, Buffer& result)\n" << flush;
#endif

    	if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    	{
        	RPC in, out;

		in.rpc_code    = opcode;
		in.rpc_length1 = call.length();
		in.rpc_buff1   = call.buffer();
		in.rpc_length2 = 0;
		in.rpc_buff2   = NULL;

        	if (multi_call(callGroup, &in, &out, timeout, retry, &callFilter, objects) == SUCCEEDED)
		{
	    		errcode = out.rpc_code;
			result.setBuffer((size_t) out.rpc_length1, out.rpc_buff1);
			status = OPERATIONAL;

			return OPER_DONE;
		}
        	else
		{
			status = INOPERATIONAL;

	    		return OPER_UNKNOWN;
		}
    	}

    	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::Terminate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::Terminate()\n" << flush;
#endif

    	if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    	{
		status = DEAD;
         	terminate(callGroup, (Boolean) FALSE, objects);

         	return OPER_DONE;
    	}
    	else if (status == UNINITIATED)
    	{
		error_stream << WARNING << " ClientRajdoot_rpc::Terminate() : Call on inactive servers.\n";

         	return OPER_NOTDONE;
    	}
    	else
    	{
		error_stream << WARNING << " ClientRajdoot_rpc::Terminate() : Call on (soft) dead servers.\n";

         	return OPER_NOTDONE;
    	}
}

//
//	Additional operations provided by the RPC interface.
//

ServerStatus ClientRajdoot_rpc::GetServerStatus()
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::GetServerStatus()\n" << flush;
	debug_stream << "ClientRajdoot_rpc::server status is " << status << "\n" << flush;
#endif
  	return status;
}

RPC_Status ClientRajdoot_rpc::Call(long  opcode,  Buffer& call1,   Buffer& call2,
		                   long& errcode, Buffer& result1, Buffer& result2)
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::Call(long  opcode, Buffer& call1, Buffer& call2, long& errcode, Buffer& result1, Buffer& result2))\n" << flush;
#endif

    	if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    	{
        	RPC in, out;

		in.rpc_code    = opcode;
		in.rpc_length1 = call1.length();
		in.rpc_buff1   = call1.buffer();
		in.rpc_length2 = call2.length();
		in.rpc_buff2   = call2.buffer();

#ifdef DEBUG
                debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
                debug_stream << "ClientRajdoot_rpc::Call\ncall buffer1 = " << call1  << "\ncall buffer2 " << call2 << "\n" << flush;
#endif

        	if (multi_call(callGroup, &in, &out, timeout, retry, &callFilter, objects) == SUCCEEDED)
		{
	    		errcode = out.rpc_code;
	    		result1.setBuffer((size_t) out.rpc_length1, out.rpc_buff1);
	    		result2.setBuffer((size_t) out.rpc_length2, out.rpc_buff2);
			status = OPERATIONAL;

            		return OPER_DONE;
		}
        	else
		{
			status = INOPERATIONAL;

	    		return OPER_UNKNOWN;
		}
	}

    	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::SetAllToDefault()
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetAllToDefault()\n" << flush;
#endif

   	if (status == UNINITIATED)
    	{
		if (objects != NULL)
		  	delete objects;

    		objects = new OBJECT_DESC[numOfServers + 1];

    		for (int numSer = 0; numSer < numOfServers; numSer++)
	  		if (SetToDefault(numSer) != OPER_DONE)
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

RPC_Status ClientRajdoot_rpc::SetToDefault(int serverNum)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetToDefault(int serverNum)\n" << flush;
#endif

   	if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
    	{
		if (SetHost(NULL, serverNum) != OPER_DONE)
		  	return OPER_UNKNOWN;

		if (SetServiceName(NULL, serverNum) != OPER_DONE)
		  	return OPER_UNKNOWN;

       		objects[serverNum].u_id.host      = 0;
		objects[serverNum].u_id.sec       = 0;
		objects[serverNum].u_id.msec      = 0;

		return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::SetObjectUid (UNIQUEID rUid)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::SetObjectUid (UNIQUEID rUid)" << endl;
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

RPC_Status ClientRajdoot_rpc::SetBurstSize (unsigned long setTo)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::SetBurstSize ( " << setTo << " )" << endl;
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	setBurstSize(setTo);
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::GetBurstSize (unsigned long& setTo) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::GetBurstSize (unsigned long& setTo) const" << endl;
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	getBurstSize(setTo);
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::SetBurstTime (unsigned long setTo)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::SetBurstTime ( " << setTo << " )" << endl;
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	setBurstTime(setTo);
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::GetBurstTime (unsigned long& setTo) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "RPC_Status ClientRajdoot_rpc::GetBurstTime (unsigned long& setTo) const" << endl;
#endif

    if ((status != SOFTDEAD) && (status != DEAD))
    {
	getBurstTime(setTo);
	return OPER_DONE;
    }
    
    return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::SetNumberOfServers(int numberOfServers)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetNumberOfServers(int numberOfServers)\n" << flush;
#endif

  	if ((status == UNINITIATED) && (numberOfServers > 0))
	{
		numOfServers 			= numberOfServers;
    		callFilter.no_successes 	= numberOfServers;   /* ??? */
    		actionGroupFilter.no_successes 	= numberOfServers;   /* ??? */
		
		return SetAllToDefault();
        }

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::GetNumberOfServers(int& numberOfServers)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::GetNumberOfServers(int& numberOfServers)\n" << flush;
#endif

  	numberOfServers = numOfServers;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::SetServiceName(char* serviceName, int serverNum)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetServiceName(char* serviceName, int serverNum)\n" << flush;
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
	     			error_stream << WARNING << " ClientRajdoot_rpc::SetServiceName() : Service name to long.\n";
				return OPER_NOTDONE;
		        }
			else
				strcpy(objects[serverNum].object_name, serviceName);
    		}

		return OPER_DONE;
  	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::GetServiceName(char*& serviceName, int serverNum)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::GetServiceName(char*& serviceName, int serverNum)\n" << flush;
#endif

   	if ((serverNum >= 0) && (serverNum < numOfServers))
	{
    		if (serviceName != NULL)
			delete serviceName;

		serviceName = new char[strlen(objects[serverNum].object_name) + 1];
		strcpy(serviceName, objects[serverNum].object_name);

		return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::SetHost(char* hostName, int serverNum)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetHost(char* hostName, int serverNum)\n" << flush;
#endif

   	if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
	{
    		host_address Host;

    		if (hostName == NULL)
    		{
        		Host.real_host = new char[MAXHOSTNAMELEN];

        		if (gethostname(Host.real_host, MAXHOSTNAMELEN) == -1)
			{
	     			error_stream << WARNING << " ClientRajdoot_rpc::SetHost() : Unknown local host name?\n";
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
			delete Host.real_host;
		}
		else
		{
	  		error_stream << WARNING << " ClientRajdoot_rpc::SetHost() : Unknown host name " << Host.real_host << endl;
			return OPER_NOTDONE;
        	}

		return OPER_DONE;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::GetHost(char*& hostName, int serverNum)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::GetHost(char*& hostName, int serverNum)\n" << flush;
#endif

   	if ((serverNum >= 0) && (serverNum < numOfServers))
	{
    		host_address Host;

		if (hostName != NULL)
		  	delete hostName;

		Host.log_host = objects[serverNum].destn_host;

		if (getrealhost(&Host) == SUCCEEDED)
		{
			hostName = new char[strlen(Host.real_host) + 1];
			strcpy(hostName, Host.real_host);
		}

		return OPER_UNKNOWN;
	}

	return OPER_NOTDONE;
}

RPC_Status ClientRajdoot_rpc::SetTimeout(long tout)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetTimeout(long tout)\n" << flush;
#endif

  	if (tout < MINIMUM_TIMEOUT)
		return OPER_NOTDONE;

	timeout = tout;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::GetTimeout(long& tout)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::GetTimeout(long& tout)\n" << flush;
#endif

	tout = timeout;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::SetRetry(long  rty)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::SetRetry(long  rty)\n" << flush;
#endif

  	if (rty < MINIMUM_RETRY)
		return OPER_NOTDONE;

        retry = rty;

	return OPER_DONE;
}

RPC_Status ClientRajdoot_rpc::GetRetry(long& rty)
{
#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::GetRetry(long& rty)\n" << flush;
#endif

	rty = retry;

	return OPER_DONE;
}

Boolean ClientRajdoot_rpc::Ping()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::Ping())\n" << flush;
#endif

	error_stream << "ClientRajdoot_rpc::Bind() : Not implemented.\n";

	return TRUE;
}

//
//	Additional operations provided by the RPC interface,
//	to allow atomic actions.
//

ClientRajdoot_rpc::ClientRajdoot_rpc(ClientRajdoot_rpc* doner)
{
#ifdef DEBUG
        debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::ClientRajdoot_rpc(ClientRajdoot_rpc* doner)\n" << flush;
#endif

	if (doner == NULL)
	{
	   	error_stream << WARNING << "ClientRajdoot_rpc::ClientRajdoot_rpc() : NULL doner\n";
		return;
	}

	if (doner->GetServerStatus() != SOFTDEAD)
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

	callGroup 		= doner->callGroup;
    	actionId 		= doner->actionId;
    	callFilter 		= doner->callFilter;
    	actionGroupFilter 	= doner->actionGroupFilter;
}

RPC_Status ClientRajdoot_rpc::SoftTerminate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PROTECTED;
	debug_stream << "ClientRajdoot_rpc::SoftTerminate()\n" << flush;
#endif

    	if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    	{
		status = SOFTDEAD;

         	return OPER_DONE;
    	}
    	else if (status == UNINITIATED)
    	{
		error_stream << WARNING << " ClientRajdoot_rpc::SoftTerminate() : Call on inactive servers.\n";

         	return OPER_NOTDONE;
    	}
    	else
    	{
		error_stream << WARNING << " ClientRajdoot_rpc::SoftTerminate() : Call on (soft) dead servers.\n";

         	return OPER_NOTDONE;
    	}
}
