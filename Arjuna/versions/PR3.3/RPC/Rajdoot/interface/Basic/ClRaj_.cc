/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRaj_.cc,v 1.4 1995/06/26 10:34:07 ngdp Exp $
 */

static const char RCSid[] = "$Id: ClRaj_.cc,v 1.4 1995/06/26 10:34:07 ngdp Exp $";

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

ClientRajdoot_rpc::ClientRajdoot_rpc ( ArjunaName* ArjName )
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
    debug_stream << "ClientRajdoot_rpc::ClientRajdoot_rpc(ArjunaName* ArjNam)\n" << flush;
    debug_stream.unlock();
    
#endif

	defaultServiceName = new char[strlen(UNKNOWNSERVICE) + 1];
	strcpy(defaultServiceName, UNKNOWNSERVICE);

    	setNumberOfServers();		// Will cause initialisation of "objects".

	if (ArjName != NULL)
	{
	    setHost(ArjName->getHostName());
	    setServiceName(ArjName->getServiceName());
	}

    	setRetry();
  	setTimeout();

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

