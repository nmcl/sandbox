/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: ClANSA.cc,v 1.4 1993/03/18 15:05:54 nmcl Exp $
 *
 * This class provides C++ interface to the client side of the
 * ANSA rpc protocol.
 *
 */

#ifdef DEBUG
#   ifndef DEBUG_H_
#      include <Common/Debug.h>
#   endif
#endif

#ifndef BUFFER_H_
#   include <Common/Buffer.h>
#endif

#ifndef PROTOS_H_
#   include <System/protos.h>
#endif

#ifndef STRING_H_
#   include <System/string.h>
#endif

#ifndef UNISTD_H_
#   include <System/unistd.h>
#endif

#ifndef ERROR_H_
#   include <Common/Error.h>
#endif

#ifndef ANSA_CLANSA_H_
#   include <RPC/ANSA/ClANSA.h>
#endif

extern "C"
{
#include "../../src/tRPC.h"
extern ansa_InterfaceRef nullRef;
extern rpc_initiate(OBJECT_DESC *, int *);
extern rpc_call(RPCBuff *, RPCBuff *, OBJECT_DESC *);
extern rpc_terminate(OBJECT_DESC *);
}

struct ObjectInfo
{
    OBJECT_DESC *desc;
};

static const char RCSid[] = "$Id: ClANSA.cc,v 1.4 1993/03/18 15:05:54 nmcl Exp $";

// Fundamental RPC operations

ClientANSA_rpc::ClientANSA_rpc( const char* serviceName )
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::ClientANSA_rpc(const char* serviceName)\n";
    if (serviceName == NULL)
    {
	debug_stream << "   serviceName = NULL\n" << flush;
    }
    else
    {
	debug_stream << "   serviceName = " << serviceName << "\n" << flush;
    }
#endif

    status = UNINITIATED;

    if (serviceName == NULL)
    {
	defaultServiceName = NULL;
    }
    else
    {
	defaultServiceName = new char[strlen(serviceName)+1];
	strcpy(defaultServiceName,serviceName);
    }

    objects = new ObjectInfo;
    objects->desc = NULL;
    SetNumberOfServers();				    /* initialises 'objects' */
}


ClientANSA_rpc::~ClientANSA_rpc()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::~ClientANSA_rpc()\n" << flush;
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	error_stream << WARNING << " ClientANSA_rpc::~ClientANSA_rpc : Call on active servers (will terminate)\n";
	Terminate();
    }

    if (status != SOFTDEAD)
    {
	if (objects->desc != NULL)
	    free(objects->desc);
	if (objects != NULL)
	    delete objects;
	if (defaultServiceName != NULL)
	    delete defaultServiceName;
    }
}


RPC_Status ClientANSA_rpc::Initiate()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::Initiate()\n" << flush;
#endif

    int successes = numOfServers;

    if (status == UNINITIATED)
    {
	if (rpc_initiate(objects->desc,&successes) == 1)
	{
	    status = OPERATIONAL;
	    return OPER_DONE;
	}
	else
	{
	    error_stream << WARNING << " ClientANSA_rpc::Initiate() : Failed\n";
	    return OPER_UNKNOWN;
	}
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::Call(long opcode, Buffer& call, long& errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::Call(long opcode, Buffer& call, long& errcode, Buffer& result)\n" << flush;
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	RPCBuff in, out;

	in.Opcode = opcode;
	in.Buffer1.length = call.length();
	in.Buffer1.data   = (ansa_Octet *) call.buffer();
	in.Buffer2.length = 0;
	in.Buffer2.data   = NULL;
	out.Opcode = 0;
	out.Buffer1.length = 0;
	out.Buffer1.data = NULL;
	out.Buffer2.length = 0;
	out.Buffer2.data = NULL;
	if (rpc_call(&in,&out,objects->desc) > 0)
	{
	    errcode = out.Opcode;
	    result.setBuffer( (size_t) out.Buffer1.length, (char *) out.Buffer1.data );
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


RPC_Status ClientANSA_rpc::Call(long opcode, Buffer& call1, Buffer& call2,
				long& errcode, Buffer& result1, Buffer& result2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::Call(long opcode, Buffer& call1, Buffer& call2, long& errcode, Buffer& result1, Buffer& result2)\n" << flush;
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	RPCBuff in, out;

	in.Opcode = opcode;
	in.Buffer1.length = call1.length();
	in.Buffer1.data   = (ansa_Octet *) call1.buffer();
	in.Buffer2.length = call2.length();
	in.Buffer2.data   = (ansa_Octet *) call2.buffer();
	out.Opcode = 0;
	out.Buffer1.length = 0;
	out.Buffer1.data = NULL;
	out.Buffer2.length = 0;
	out.Buffer2.data = NULL;
	if (rpc_call(&in,&out,objects->desc) > 0)
	{
	    errcode = out.Opcode;
	    result1.setBuffer( (size_t) out.Buffer1.length, (char *) out.Buffer1.data );
	    result2.setBuffer( (size_t) out.Buffer2.length, (char *) out.Buffer2.data );
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


RPC_Status ClientANSA_rpc::Terminate()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::Terminate()\n" << flush;
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	status = DEAD;
	rpc_terminate(objects->desc);
	return OPER_DONE;
    }
    else if (status == UNINITIATED)
    {
	error_stream << WARNING << " ClientANSA_rpc::Terminate() : Call on inactive servers.\n";
	return OPER_NOTDONE;
    }
    else
    {
	error_stream << WARNING << " ClientANSA_rpc::Terminate() : Call on (soft) dead servers.\n";
	return OPER_NOTDONE;
    }
}


// Additional RPC operations


ServerStatus ClientANSA_rpc::GetServerStatus()
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::GetServerStatus()\n";
#endif

    return status;
}


RPC_Status ClientANSA_rpc::SetAllToDefault()
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::SetAllToDefault()\n" << flush;
#endif
    int numSer;

    if (status == UNINITIATED)
    {
	if (objects->desc != NULL)
	    free(objects->desc);

	objects->desc = (OBJECT_DESC*) malloc(sizeof(OBJECT_DESC) * (numOfServers + 1));
	
	/* initialise all fields */
	for(numSer = 0; numSer <= numOfServers; numSer++)
	{
	    objects->desc[numSer].hostName = NULL;
	    objects->desc[numSer].objectName = NULL;
	    objects->desc[numSer].factory = nullRef;
	    objects->desc[numSer].capsule = nullRef;
	    objects->desc[numSer].id      = nullRef;
	    objects->desc[numSer].server  = nullRef;
	}

	for(numSer = 0; numSer < numOfServers; numSer++)
	{
	    if (SetToDefault(numSer) != OPER_DONE)
		return OPER_UNKNOWN;
	}
	objects->desc[numOfServers].hostName   = NULL;	    /* dummy last object - end marker */
	objects->desc[numOfServers].objectName = NULL;
	return OPER_DONE;
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::SetToDefault( int serverNum )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::SetToDefault(int serverNum)\n";
    debug_stream << "   serverNum = " << serverNum << "\n" << flush;
#endif

    if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
    {
	if (SetHost(NULL, serverNum) != OPER_DONE)
	    return OPER_UNKNOWN;
	if (SetServiceName(NULL, serverNum) != OPER_DONE)
	    return OPER_UNKNOWN;
	return OPER_DONE;
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::SetNumberOfServers( int numberOfServers )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::SetNumberOfServers(int numberOfServers)\n";
    debug_stream << "   numberOfServers = " << numberOfServers << "\n" << flush;
#endif

    if ((status == UNINITIATED) && (numberOfServers > 0))
    {
	numOfServers = numberOfServers;
	return SetAllToDefault();
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::GetNumberOfServers( int& numberOfServers )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::GetNumberOfServers(int& numberOfServers)\n" << flush;
#endif

    numberOfServers = numOfServers;
    return OPER_DONE;
}


RPC_Status ClientANSA_rpc::SetServiceName( char* service, int serverNum )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::SetServiceName( char* service, int serverNum)\n";
    if (service == NULL)
	debug_stream << "   service = NULL; serverNum = " << serverNum << "\n" << flush;
    else
	debug_stream << "   service = " << service << "; serverNum =  " << serverNum << "\n" << flush;
#endif

    if ((status == UNINITIATED) && (serverNum >= 0) && (serverNum < numOfServers))
    {
	char *object;

	if (service == NULL)
	{
	    if (defaultServiceName == NULL)
	    {
		object = new char[2];
		strcpy(object,"");
	    }
	    else
	    {
		object = new char[ strlen(defaultServiceName) + 1];
		strcpy(object,defaultServiceName);
	    }
	}
	else
	{
	    if (strlen(service) >= MAX_SERVICENAME_SIZE)
	    {
		error_stream << WARNING << " ClientANSA_rpc::SetServiceName() : Service name too long.\n";
		return OPER_NOTDONE;
	    }
	    object = new char[ strlen(service) + 1 ];
	    strcpy(object,service);
	}

	if (objects->desc[serverNum].objectName != NULL)
	    delete objects->desc[serverNum].objectName;

	objects->desc[serverNum].objectName = object;
	return OPER_DONE;
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::GetServiceName( char*& service, int serverNum )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::GetServiceName(char*& service, int serverNum )\n";
#endif

    if ((serverNum >= 0) && (serverNum < numOfServers))
    {
	if (service != NULL)
	    delete service;
	
	service = new char[ strlen(objects->desc[serverNum].objectName) + 1 ];
	strcpy( service, objects->desc[serverNum].objectName );
	return OPER_DONE;
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::SetHost( char* hostName, int serverNum )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::SetHost(char* hostName, int serverNum )\n";
    if (hostName == NULL)
	debug_stream << "   hostName = NULL; serverNum = " << serverNum << "\n" << flush;
    else
	debug_stream << "   hostName = " << hostName << "; serverNum = " << serverNum << "\n" << flush;
#endif

    if ((status == UNINITIATED) && (serverNum >=0) && (serverNum < numOfServers))
    {
	char *host;

	if (hostName == NULL)
	{
	    host = new char[MAX_HOSTNAME_SIZE+1];
	    if (gethostname(host,MAX_HOSTNAME_SIZE) == -1)
	    {
		error_stream << WARNING << " ClientANSA_rpc::SetHost() : Unknown local host?\n";
		return OPER_NOTDONE;
	    }
	}
	else
	{
	    if (strlen(hostName) >= MAX_HOSTNAME_SIZE)
	    {
		error_stream << WARNING << " ClientANSA_rpc::SetHost() : Host name too long.\n";
		return OPER_NOTDONE;
	    }
	    host = new char[strlen(hostName)+1];
	    strcpy(host,hostName);
	}

	if (objects->desc[serverNum].hostName != NULL)
	    delete objects->desc[serverNum].hostName;

	objects->desc[serverNum].hostName = host;
	return OPER_DONE;
    }
    return OPER_NOTDONE;
}


RPC_Status ClientANSA_rpc::GetHost( char*& hostName, int serverNum )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::GetHost( char*& hostName, int serverNum )\n" << flush;
#endif

    if ((serverNum >= 0) && (serverNum < numOfServers))
    {
	if (hostName != NULL)
	    delete hostName;
	hostName = new char[strlen(objects->desc[serverNum].hostName) + 1];
	strcpy(hostName,objects->desc[serverNum].hostName);
	return OPER_DONE;
    }
    return OPER_NOTDONE;
}


ClientANSA_rpc::ClientANSA_rpc( ClientANSA_rpc *doner )
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::ClientANSA_rpc(ClientANSA_rpc *doner)\n" << flush;
#endif

    if (doner == NULL)
    {
	error_stream << WARNING << "ClientANSA_rpc::ClientANSA_rpc() : NULL doner\n";
	return;
    }

    if (doner->GetServerStatus() != SOFTDEAD)
    {
	error_stream << WARNING << "ClientANSA_rpc::ClientANSA_rpc() : NULL is not SOFTDEAD\n";
	return;
    }

    status             = OPERATIONAL;
    numOfServers       = doner->numOfServers;
    defaultServiceName = doner->defaultServiceName;
    objects            = doner->objects;
}


RPC_Status ClientANSA_rpc::SoftTerminate()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PROTECTED;
    debug_stream << "ClientANSA_rpc::SoftTerminate()\n" << flush;
#endif

    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
    {
	status = SOFTDEAD;
	return OPER_DONE;
    }
    else if (status == UNINITIATED)
    {
	error_stream << WARNING << "ClientANSA_rpc::SoftTerminate() : Call on inactive server.\n";
	return OPER_NOTDONE;
    }
    else
    {
	error_stream << WARNING << "ClientANSA_rpc::SoftTerminate() : Call on (soft) dead server.\n";
	return OPER_NOTDONE;
    }
}
