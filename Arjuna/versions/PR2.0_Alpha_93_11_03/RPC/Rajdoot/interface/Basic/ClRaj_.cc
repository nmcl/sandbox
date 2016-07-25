/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRaj_.cc,v 1.1 1993/11/03 14:40:29 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClRaj_.cc,v 1.1 1993/11/03 14:40:29 nmcl Exp $";

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef RAJDOOT_CLRAJ_H_
#  include <RPC/Rajdoot/ClRaj.h>
#endif

#ifndef RAJDOOT_ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#define UNKNOWNSERVICE          "TheUnknownService"

ClientRajdoot_rpc::ClientRajdoot_rpc(ArjunaName* ArjName)
{
#ifdef DEBUG
        debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ClientRajdoot_rpc::ClientRajdoot_rpc(ArjunaName* ArjNam)\n" << flush;
#endif

    	status 				= UNINITIATED;

	objects = NULL;

	defaultServiceName = new char[strlen(UNKNOWNSERVICE) + 1];
	strcpy(defaultServiceName, UNKNOWNSERVICE);

    	SetNumberOfServers();		// Will cause initialisation of "objects".

	if (ArjName != NULL)
	{
	    SetHost(ArjName->GetRefHostName());
	    SetServiceName(ArjName->GetRefServiceName());
	}

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

