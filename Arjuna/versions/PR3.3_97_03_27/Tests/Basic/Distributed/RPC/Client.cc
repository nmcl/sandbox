/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Client.cc,v 
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#include "ElapsedTime.h"

int main (int argc, char** argv)
{
    GROUPS newgroup;
    OBJECT_DESC objects[2];
    RPC in, out;
    FILTER_INFO filter;
    host_address Host;
    char *hostToUse = 0, *message = 0;
    int rcode, successes = 1;
    ElapsedTime startTime, finishTime, initTime, termTime, st, et;
    Boolean canDelete = FALSE;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostToUse = argv[i+1];
	if (::strcmp(argv[i], "-msg") == 0)
	    message = argv[i+1];
    }
    
    if (hostToUse == 0)
    {
	hostToUse = ::new char[MAXHOSTNAMELEN+1];
	::memset(hostToUse, '\0', MAXHOSTNAMELEN+1);
	::gethostname(hostToUse, MAXHOSTNAMELEN);
	canDelete = TRUE;
    }
    
    if (message == 0)
	message = "Hello World";
    
    Host.real_host = hostToUse;
    if (getloghost(&Host) != SUCCEEDED)
    {
	cerr << "Host error" << endl;
	exit(-1);
    }
    
    objects[0].destn_host = Host.log_host;
    ::memset(objects[0].object_name, '\0', OBJECT_NAME_SIZE);
    ::strcpy(objects[0].object_name, "Server");
    objects[0].u_id.host = 2222;
    objects[0].u_id.sec = 2222;
    objects[0].u_id.msec = 2222;
    objects[1].destn_host = 0;

    startTime.get();
    st = startTime;

    rcode = initiate(&objects[0], &newgroup, &successes);
	
    et.get();
    initTime = initTime + (et - st);
    
    if (rcode != SUCCEEDED)
    {
	cerr << "An error occurred during initiate." << endl;
	if (canDelete)
	    ::delete [] hostToUse;
	
	return -1;
    }
    else
	cout << "Initiate succeeded." << endl;
    
    in.rpc_code = 32786;
    in.rpc_length1 = ::strlen(message)+1;
    in.rpc_buff1 = (char*) message;
    in.rpc_length2 = 0;
    in.rpc_buff2 = 0;
    
    filter.filt_name = 0;
    filter.no_successes = 1;
    filter.compare = compare_NONE;

    if (multi_call(&newgroup, &in, &out, 1000, 2, &filter, &objects[0]) == SUCCEEDED)
	cout << "rpc returned with: " << out.rpc_buff1 << endl;
    else
	cerr << "rpc error" << endl;

    st.get(); 
    
    (void) terminate(&newgroup, TRUE, &objects[0]);

    et.get();
    termTime = termTime + (et - st);

    finishTime.get();

    cout <<"-------------------------------------------\n";
    cout << "Overall Time : " <<  finishTime - startTime << "\n";
    cout << "Initiate Elapsed Time: " << initTime << "\n";
    cout << "Terminate Elapsed Time: " << termTime << "\n";
    cout  << "-------------------------------------------\n" << flush;

    if (canDelete)
	::delete [] hostToUse;

    ::delete [] out.rpc_buff1;
    
    return 0;
}
