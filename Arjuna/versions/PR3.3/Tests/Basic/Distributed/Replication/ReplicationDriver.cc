/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicationDriver.cc,v 1.6 1995/09/18 11:36:33 nmcl Exp $
 */

#include <Config/Configure.h>

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef REPLICA_H_
#  include "Replica.h"
#endif

#ifndef ELAPSEDTIME_H_
#  include "ElapsedTime.h"
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif

int main(int argc, char** argv)
{
    Replica *objectGroup = 0;
    AtomicAction A, B;
    int res = -1;
    ElapsedTime stime, ftime;
    Boolean doTime = FALSE;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-timer") == 0)
	    doTime = TRUE;
    }
    
    objectGroup = new Replica(res);

    if (res != 0)
	cout << "Error while creating new Replica object\n" << flush;
    else
    {
	stime.get();
	
	A.Begin();

	if (!objectGroup->change(2))
	{
	    cout << "Could not change value for new object\n" << flush;
	    A.Abort();
	}
	else
	{
	    cout << "Value for single object changed to 2 successfully\n" << flush;

	    if (A.End() != COMMITTED)
		cout << "A.End() : Not Done\n" << flush;
	    else
	    {
		ftime.get();
		
		
		cout << "A.End() : Done\n" << flush;

		if (doTime)
		{
		    cout << "Time for write operation: " << (ftime - stime) << endl;
		}

		B.Begin();
		
		if (objectGroup->value() == 2)
		{
		    cout << "Value returned is 2. This is correct." << endl;
		    if (B.End() != COMMITTED)
			cout << "B.End() : Not Done" << endl;
		    else
			cout << "B.End() : Done" << endl;
		}
		else
		{
		    cout << "Value returned is incorrect." << endl;
		    B.Abort();
		}
	    }
	}
    }

    if (objectGroup)
	delete objectGroup;

    return 0;
}
