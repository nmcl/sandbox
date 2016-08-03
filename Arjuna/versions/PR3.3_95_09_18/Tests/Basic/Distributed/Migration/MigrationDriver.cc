/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MigrationDriver.cc,v 1.3 1995/09/30 10:24:19 nmcl Exp $
 */

#include <Config/Configure.h>

#include <System/protos.h>
#include <System/string.h>
#include <System/iostream.h>

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef GROUPINFORMATION_H_
#  include <Replication/GroupInformation.h>
#endif

#ifndef MIGRATOR_H_
#  include <Replication/Migrator.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef REPLICA_H_
#  include "Replica.h"
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif


int main (int argc, char** argv)
{
    AtomicAction A;
    int res = -1;
    char* hostName = (char*) 0;
    char fromHost[MAXHOSTNAMELEN+1];

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -host <host name> [-help]"
		 << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-host") == 0)
	    hostName = argv[i+1];
    }

    if (!hostName)
    {
	cout << "Error - no migration host destination name given." << endl;
	return -1;
    }

    ::memset(fromHost, '\0', MAXHOSTNAMELEN+1);
    ::gethostname(fromHost, MAXHOSTNAMELEN);
    
    Replica objectGroup(res);
		     
    if (res != 0)
	cout << "Error while creating new Replica object\n" << flush;
    else
    {
	A.Begin();

	if (!objectGroup.change(2))
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
		cout << "A.End() : Done\n" << flush;

		cout << "\nNow migrating to " << hostName << endl;

		ReplicaDescriptor* groupView = 0;
		GroupInformation myGroup(objectGroup);

		/*
		 * Since we know this cannot be a composite replica group
		 * there is no need to further manipulate the returned
		 * group view. However, in certain cases this may become an
		 * issue.
		 */
		
		if (myGroup.currentGroup(groupView))
		{
		    /*
		     * For simplicity, we shall migrate the first replica.
		     */

		    res = -1;

		    Migrator m(res);

		    if (res == 0)
		    {
			m.typeIs(objectGroup.type());

			if (m.moveFromTo(fromHost, hostName, objectGroup.get_uid(), groupView->getUid()) == MovedOk)
			{
			    cout << "Migrated successfully." << endl;

			    AtomicAction B;

			    B.Begin();
			    
			    if (objectGroup.change(4))
			    {
				if (objectGroup.value() == 4)
				{
				    cout << "Value changed to 4." << endl;
				    B.End();  // should check this, but ...
				}
				else
				{
				    cout << "Value change error at get." << endl;
				    B.Abort();
				}
			    }
			    else
			    {
				cout << "Value change error at write." << endl;
				B.Abort();
			    }
			}
			else
			    cout << "Migration failure." << endl;
		    }
		    else
			cout << "Migrator create error." << endl;

		    delete groupView;
		}
		else
		    cout << "Get group view error." << endl;
	    }
	}
    }

    return 0;
}

		    
