/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: dump_clist.cc,v 1.10 1995/09/30 10:28:32 nmcl Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef RAJDOOT_RPC_USERS_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif


int main (int, char**)
{
    struct C_LIST_EL* tmpclist = NULL;
    struct C_LIST_EL clist;
    
    int retry = FILESRETRY, rstat = FAILED;

    do
    {
	rstat = clistopen(tmpclist);
    } while ((rstat != SUCCEEDED) && (retry-- > 0));

    if (rstat != SUCCEEDED)
    {
	cout << "Error. Could not open clist." << endl;
	return -1;
    }

    ::memcpy((char*) &clist, tmpclist, sizeof(C_LIST_EL));
    
    if (clistclose() != SUCCEEDED)
    {
	cout << "Error. Could not close clist." << endl;
	return -2;
    }

    cout << "Processing client-side information." << endl;

    for (int i = 0; i < NSTATION; i++)
    {
	if (clist.client[i].node != 0)
	{
	    cout << "\nEntry " << i+1 << endl;
	    
	    struct hostent* hp = ::gethostbyaddr((char*) &clist.client[i].node, sizeof(struct in_addr), AF_INET);	
	    cout << "\thost name : " << hp->h_name << endl;
	    cout << "\tcrash count value : " << clist.client[i].c_count << endl;
	}
    }
    
    cout << "\nProcessing server-side information." << endl;

    for (int j = 0; j < MAXSERVERS; j++)
    {
	if (clist.s_desc[j].process_id != -1)
	{
	    cout << "\nEntry " << j+1 << endl;
	    cout << "\tprocess id : " << clist.s_desc[j].process_id << endl;
	    cout << "\tpotential orphan : " << ((clist.s_desc[j].poflag) ? "TRUE" : "FALSE") << endl;
	    
	    struct hostent* hp = ::gethostbyaddr((char*) &clist.s_desc[j].sender.hi_part, sizeof(struct in_addr), AF_INET);
	
	    cout << "\taddress : < " << hp->h_name << ", " << clist.s_desc[j].sender.lo_part << " >" << endl;

	
	    cout << "\tunique id : < " << clist.s_desc[j].u_id.host << ", " << clist.s_desc[j].u_id.sec
		 << ", " << clist.s_desc[j].u_id.msec << " >" << endl;
	    cout << "\tnumber of links : " << clist.s_desc[j].no_links << endl;

	    cout << "\n\tClients for this server are (full rpc path) :" << endl;

	    int clientCount = 0;
	    
	    for (int k = 0; k < MAXCLIENTS; k++)
	    {
		Boolean incremented = FALSE;
		int counter = 0;
		
		for (int l = 0; l < NSTATION; l++)
		{
		    if (clist.s_desc[j].linkspath[k].rpcpath[l].node != 0)
		    {
			/*
			 * Only increment client number if we have client data.
			 */
			
			if (!incremented)
			{
			    clientCount++;
			    incremented = TRUE;
			    cout << "\n\t\tClient : " << clientCount << endl;
			}

			counter++;
			
			cout << "\n\t\t\tDepth : " << counter << endl;
		    
			struct hostent* hp = ::gethostbyaddr((char*) &clist.s_desc[j].linkspath[k].rpcpath[l].node,
							     sizeof(struct in_addr),
							     AF_INET);
		
			cout << "\t\t\thost name : " << hp->h_name << endl;
			cout << "\t\t\tport : " << ntohl(clist.s_desc[j].linkspath[k].groupid.lo_part) << endl;
			cout << "\t\t\tcrash count : " << clist.s_desc[j].linkspath[k].rpcpath[l].c_count << endl;			
		    }
		}
	    }
	}
    }

    return 0;
}
