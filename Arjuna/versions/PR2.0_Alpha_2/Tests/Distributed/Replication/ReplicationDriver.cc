/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicationDriver.cc,v 1.1 1993/11/03 14:59:09 nmcl Exp $
 */

#include <Configure.h>

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#include <System/protos.h>
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef REPLICA_H_
#include "Replica.h"
#endif

#ifndef REGISTER_H_
#include <Replication/Register.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int, char**)
#else
int main(int, char**)
#endif
{
    const int numberReplicas = 2;
    Replica *loneObject = 0, *objectGroup = 0;
    Uid u_id;
    Register* reg = 0;
    AtomicAction A, B, C;
    ListData *head = 0, *ptr = 0;
    Boolean result = FALSE;
    int res = -1, value;

    loneObject = new Replica(res);
    if (res != 0)
	cout << "Error while creating new Replica object\n" << flush;
    else
    {
	A.Begin();

	if (!loneObject->change(2))
	    cout << "Could not change value for new object\n" << flush;
	else
	{
	    cout << "Value for single object changed to 2 successfully\n" << flush;

	    if (A.End() != COMMITTED)
		cout << "A.End() : Not Done\n" << flush;
	    else
	    {
		cout << "A.End() : Done\n" << flush;
		
		Uid u_id = loneObject->get_uid();
		char hostName[MAXHOSTNAMELEN+1];
		head = new ListData;

		for (int i = 0; i < numberReplicas; i++)
		{
		    if (!ptr)
			ptr = head;
		    else
		    {
			ptr->next = new ListData;
			ptr = ptr->next;
		    }

		    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
		    (void)gethostname(hostName, MAXHOSTNAMELEN);
		    *ptr = hostName;
		}
	    
		reg = new Register;
		
		B.Begin();

		reg->registerObject(u_id, loneObject->type());
		if (!reg->replicateState(head))
		{
		    cout << "Error while attempting to replicate state\n" << flush;
		    B.Abort();
		}
		else
		{
		    cout << "Replicated state ok\n" << flush;

		    if (B.End() != COMMITTED)
			cout << "B.End() : Not Done\n" << flush;
		    else
		    {
			cout << "B.End() : Done\n" << flush;

			if (reg)
			{
			    delete reg;
			    reg = 0;
			}

			res = -1;
			objectGroup = new Replica(u_id, res);
			if (res != 0)
			    cout << "Error creating replica group\n" << flush;
			else
			{
			    objectGroup->makeReplicated(TRUE);
			    C.Begin();

			    value = objectGroup->value();

			    if (value == 2)
				cout << "Value returned from replica group is 2\n" << flush;
			    else
				cout << "Value returned from replica group is wrong\n" << flush;
		
			    if (C.End() == COMMITTED)
				cout << "C.End() : Done\n" << flush;
			    else
				cout << "C.End() : Not Done\n" << flush;
			}
		    }
		}
	    }
	}
    }

    if (head)
	delete head;
    
    if (loneObject)
	delete loneObject;

    if (objectGroup)
	delete objectGroup;

    if (reg)
	delete reg;

    return 0;
}
