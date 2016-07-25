/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: shutdown_manager.cc,v 1.7 1995/10/13 10:32:55 nmcl Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
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

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDLIB_H_
#   include <System/stdlib.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef RAJDOOT_RPC_USERS_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif


extern long managerAddr;
extern void checkEnvironment();


int main (int argc, char** argv)
{
    struct mc_msghdr req_buf, result_buf;
    struct RPC_SN request_sn, rq_sn;
  	struct mc_sg_filter ack;
    struct MID multithem, actid;
    struct MID unithem, client;
    struct UNIQUEID uid;
    host_address Host;
    long   manager_port, serverport;
    long   request_code = htonl(MANAGER_DIE);
    int    opn_code = htonl(NOARG);
    char*  host = (char*) 0;

    checkEnvironment();
    manager_port = managerAddr;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    host = argv[i+1];
	if (::strcmp(argv[i], "-port") == 0)
	    manager_port = atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: shutdown_manager [-host <host_name>] [-port <manager_port>]" << endl;
	    return 1;
	}
    }
    
    if (host == (char*) 0)
    {
	host = ::new char[MAXHOSTNAMELEN+1];
	::memset(host, '\0', MAXHOSTNAMELEN+1);
	::gethostname(host, MAXHOSTNAMELEN);
    }
    
    Host.real_host = ::new char[::strlen(host)+1];
    ::memset(Host.real_host, '\0', ::strlen(host)+1);
    ::strcpy(Host.real_host, host);
    
    if (getloghost(&Host) != SUCCEEDED)
    {
	cerr << "Could not look up host " << Host.real_host << endl;
	return -1;
    }


    /* Get a Sequence Number */
    getsn(&rq_sn);    
    request_sn.sec = htonl(rq_sn.sec);    /* Byte Swap */
    request_sn.msec = htonl(rq_sn.msec);

    getnullmid(&actid);
    getnullmid(&multithem);

    req_buf.mc_msg[0].iov_base = (char *) &request_sn;
    req_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
    req_buf.mc_msg[1].iov_base = (char *) &request_code;
    req_buf.mc_msg[1].iov_len = sizeof(request_code);
    req_buf.mc_msg[2].iov_base = (char *) &multithem;
    req_buf.mc_msg[2].iov_len = sizeof(struct MID);
    req_buf.mc_msg[3].iov_base = (char *) &actid;
    req_buf.mc_msg[3].iov_len = sizeof(struct MID);
    req_buf.mc_msg[4].iov_base = (char *) &uid;
    req_buf.mc_msg[4].iov_len = sizeof(struct UNIQUEID);
    req_buf.mc_msg[5].iov_base = (char *) &opn_code;
    req_buf.mc_msg[5].iov_len = sizeof(opn_code);  
    req_buf.mc_msg[6].iov_base = (char *) &serverport;
    req_buf.mc_msg[6].iov_len = sizeof(serverport);
    req_buf.mc_msg[7].iov_base = 0;
    req_buf.mc_msg[7].iov_len = 0;
    req_buf.mc_msg[8].iov_base = 0;
    req_buf.mc_msg[8].iov_len = 0;
    req_buf.msg_len = 9;

    result_buf.mc_msg[0].iov_len  = sizeof(struct RPC_SN);
    result_buf.mc_msg[0].iov_base = 0;
    result_buf.mc_msg[1].iov_len  = sizeof(long);
    result_buf.mc_msg[1].iov_base = 0;
    result_buf.mc_msg[2].iov_len  = 0;
    result_buf.mc_msg[2].iov_base = 0;
    result_buf.mc_msg[3].iov_len  = 0;
    result_buf.mc_msg[3].iov_base = 0;
    result_buf.msg_len = 4;
    ack.next_fs = NULL;
    ack.mc_msg = &result_buf;

    unithem.type = htonl(UID_type);
    unithem.lo_part = htonl(manager_port);
    unithem.hi_part = Host.log_host;

    multicast_get_mid(&client);

#ifndef GCC_ARRAY_DELETE_BUG
    ::delete [] Host.real_host;
    ::delete [] host;
#else
    ::delete Host.real_host;
    ::delete host;
#endif

    if (multicast_long_send(&unithem, &req_buf) != MC_OK)
    {
	cerr << "Send to manager failed" << endl;
	return -2;
    }
    
    cout << "Send to manager successful" << endl;

    if (multicast_long_receive(&client, &ack, 0, GWTIMEOUT) > 0)
    {
	cout << "Received acknowledgment from manager." << endl;
	return 0;
    }

    cout << "Did not receive acknowledgment from manager." << endl;

    return -1;
}

    
    
