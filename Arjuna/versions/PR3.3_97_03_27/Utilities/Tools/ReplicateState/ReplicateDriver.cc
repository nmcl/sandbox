/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicateDriver.cc,v 1.8 1995/02/22 14:56:21 ngdp Exp $
 */

#include <Config/Configure.h>

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef REPLICACREATE_H_
#  include <Replication/ReplicaCreate.h>
#endif

#ifndef REGISTERGROUP_H_
#  include <Replication/RegisterGroup.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif


const int uidSize = 80;

int main(int, char**)
{
    ReplicaDescriptor *rd = (ReplicaDescriptor*) 0, *marker = (ReplicaDescriptor*) 0;
    AtomicAction A;
    ReplicaCreate repC;
    Uid* u_id = 0;
    char tn[MAXPATHLEN], host[MAXHOSTNAMELEN+1], uid[uidSize];
    int number = 0;
    StoreState result;
    char copyOpt;

    ::memset(tn, '\0', MAXPATHLEN);
    ::memset(host, '\0', MAXHOSTNAMELEN+1);
    ::memset(uid, '\0', uidSize);

    cout << "\nHow many replicas do you require (excluding the original)?\n" << flush;
    cin >> number;

    cout << "\nWhat is the Uid of the object state to be copied?\n";
    cin >> uid;
    u_id = new Uid(uid);
    rd = new ReplicaDescriptor;
    rd->setUid(*u_id);

    cout << "\nWhat is the type name of the object state?\n";
    cin >> tn;
    rd->setObjectName(tn);

    cout << "\nWhat is the origin host name?\n";
    cin >> host;
    rd->setHost(host);

    cout << "\nDo you want to copy the state or replicated (c/r)?\n";
    cin >> copyOpt;
    
    // create group view list outside ReplicaCreate instance.

    marker = rd;
    for (int j = 0; j < number; j++)
    {
	Uid u;
	
        // head of list represents state to be replicated.

	marker->next = new ReplicaDescriptor;
	marker = marker->next;

	cout << "\nWhat is the destination node called?\n";
	::memset(host, '\0', MAXHOSTNAMELEN+1);
	cin >> host;
	marker->setHost(host);
	marker->setObjectName(tn);

	if (copyOpt == 'c')
	    marker->setUid(*u_id);
	else
	    marker->setUid(u);
    }

    repC.setObjectInfo(*u_id, tn);
    result = repC.createReplicas(*rd);

    if (result == OK)
    {
        // print information on new group view.

        char opt;
	ReplicaDescriptor* tmpRD = rd;

	cout << "The following replicas were created:" << endl;
	while (tmpRD)
	{
	    if (tmpRD->getActive())
	        cout << "Replica on " << tmpRD->getHost() <<endl;
	    tmpRD = tmpRD->next;
	}

	cout << "The following replicas were not created:" << endl;
	while (tmpRD)
	{
	    if (!tmpRD->getActive())
	        cout << "Replica on " << tmpRD->getHost() <<endl;
	    tmpRD = tmpRD->next;
	}

	// register operation will only register available replicas.

        cout << "Replicas created. Do you wish to register them (y/n)?" << endl;
	cin >> opt;

	if ((opt == 'y') || (opt == 'Y'))
	{
	    delete u_id;
	    ::memset(uid, '\0', uidSize);

	    cout << "What is the group Uid to use?" << endl;
	    cin >> uid;
	    u_id = new Uid(uid);

	    A.Begin();

	    RegisterGroup reg(*u_id, rd);

	    result = reg.recordGroup() ? OK : UNKNOWN;

	    if (result == OK)
	    {
	        if (A.End() == COMMITTED)
		    cout << "Registered group successfully" << endl;
		else
		    cout << "Action commit error" << endl;
	    }
	    else
	    {
	        A.Abort();
		cout << "An error occurred during registering" << endl;
	    }
	}
    }
    else
	cout << "Error number " << result << " occurred during copy." << endl;

    delete u_id;
    return 0;
}

    

