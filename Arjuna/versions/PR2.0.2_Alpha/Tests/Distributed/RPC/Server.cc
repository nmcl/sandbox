/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Server.cc,v 
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

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif


const char* reply = "This is a test";

int main (int argc, char** argv)
{
    int res = init_server (argc, argv);
    RPC work, result;
    GROUPS clientGroup;
    DEADLINE deadlines;
    
    if (res != SUCCEEDED)
	exit(-1);

    deadlines.timeout = 10000;

    for (;;)
    {
	if (getwork(&clientGroup, &work, &deadlines, 0) == SUCCEEDED)
	{
	    cout << "got " << work.rpc_buff1 << endl;
	    
	    result.rpc_code = 32786;
	    result.rpc_length1 = ::strlen(reply);
	    result.rpc_buff1 = (char*) reply;
	    result.rpc_length2 = 0;
	    result.rpc_buff2 = 0;
	
	    if (sendresult(&clientGroup, &result) != SUCCEEDED)
		cerr << "sendresult failed" << endl;
	    else
		cout << "sent result ok" << endl;
	}
    }
}

	
    

    
    
