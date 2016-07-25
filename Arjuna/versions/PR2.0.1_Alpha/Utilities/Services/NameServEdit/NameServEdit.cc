/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServEdit.cc,v 1.1 1993/11/03 15:03:46 nmcl Exp $
 *
 * NameServerEdit source code.
 *
 */

#include <Configure.h>

#ifdef DEBUG
#  include <Common/Debug.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int, char**)
#else
int main(int, char**)
#endif
{
    Boolean done = FALSE;
    int res = -1;

    NSInterface NS(res);

    if (res == 0)
	done = TRUE;

    if (! done)
    {
	error_stream << "Could not open name server.\n" ;
	return 1;
    }

    char ch = 'H';

    while ((ch != 'Q') && (ch != 'q'))
    {
	if ((ch == 'H') || (ch == 'h'))
	{
	    cout<<"\nHelp:\n\n" << flush;
	    cout<<"S) Store\n" << flush;
	    cout<<"F) Fetch\n" << flush;
	    cout<<"L) List\n" << flush;
	    cout<<"R) Remove\n" << flush;
	    cout<<"H) Help\n\n" << flush;
	    cout<<"Q) QUIT\n" << flush;
	}
	else if ((ch == 'S') || (ch == 's'))
	{
	    cout<<"\nStore:\n" << flush;

	    ArjunaName ANKey, ANData;

	    char ObjName[32];
	    cout<<"Object Name  : " << flush;
	    cin>>ObjName;
	    ANKey.SetObjName(ObjName);
	    ANData.SetObjName(ObjName);

	    char ObjUidStr[32];
	    Uid*  ObjUid;
	    cout<<"Object Uid   : " << flush;
	    cin>>ObjUidStr;
	    ObjUid = new Uid (ObjUidStr);
	    ANData.SetObjUid(*ObjUid);
	    delete ObjUid;

	    char ServiceName[32];
	    cout<<"Service Name : " << flush;
	    cin>>ServiceName;
	    ANData.SetServiceName(ServiceName);

	    char HostName[MAXHOSTNAMELEN];
	    cout<<"Host Name    : " << flush;
	    cin>>HostName;
	    ANData.SetHostName(HostName);

	    Buffer Key, Data;
	    cout << "Key  : " << ANKey << "\n" << flush;
	    cout << "Data : " << ANData << "\n" << "****\n" << flush;

	    done = (ANKey.pack(Key) && ANData.pack(Data));
	    if (! done)
		error_stream << "Packing ArjunaNames Failed.\n" << flush;
	    else
	    {
		NS.storeName(done, Key, Data);
		if (! done)
		    error_stream << "NameServer::storeName() : Failed.\n" << flush;
	    }
	}
	else if ((ch == 'F') || (ch == 'f'))
	{
	    cout<<"\nFetch:\n" << flush;

	    ArjunaName ANKey, ANData;

	    char ObjName[32];
	    cout<<"Object Name  : " << flush;
	    cin>>ObjName;
	    ANKey.SetObjName(ObjName);

	    Buffer Key, Data;
	    ANKey.pack(Key);
	    done = FALSE;

	    cout << "Key  : " << ANKey << "\n" << flush;
	    NS.fetchName(done, Key, Data);
	    if (! done)
	        error_stream << "NameServer::fetchName() : Failed.\n" << flush;
	    else
	    {
		done = ANData.unpack(Data);
		if (! done)
		    error_stream << "ArjunaName unpack Failed.\n" << flush;
		else
		    cout << ANData << "\n" << flush;
	    }
	}
	else if ((ch == 'L') || (ch == 'l'))
	{
	    cout<<"\nList:\n" << flush;

	    Boolean finish = (Boolean) FALSE, done, eod;

	    NS.restart(done);

	    cout<<"****\n" << flush;
	    while (! finish)
	    {
		Buffer Key;
	        NS.getNextName(done, Key, eod);

		if (! eod)
		{
		    if (! done)
			error_stream << "\nNameServer::getNextName() : Failed.\n" << flush;
		    else
		    {
			Buffer Data;

			done = FALSE;
			NS.fetchName(done, Key, Data);

			if (! done)
			    error_stream << "\nNameServer::fetchName() : Failed.\n" << flush;
			else
			{
			    ArjunaName ANKey, ANData;
			    done = (ANKey.unpack(Key) && ANData.unpack(Data));

			    if (done)
			    {
				cout << "Key  : " << ANKey << "\n" << flush;
                                cout << "Data : " << ANData << "\n" << "****\n" << flush;
			    }
			    else
				cout << "Unpack of ArjunaName failed.\n" << flush;
			}
		    }
		}

		finish = (Boolean) ((! done) || eod);
	    }
	}
	else if ((ch == 'R') || (ch == 'r'))
	{
	    cout<<"\nRemove:\n" << flush;

	    ArjunaName AN;

	    char ObjName[32];
	    cout<<"Object Name  : " << flush;
	    cin>>ObjName;

	    AN.SetObjName(ObjName);	    

	    Buffer Key;

	    AN.pack(Key);
	    cout << "Key  : " << AN << "\n" << flush;

	    NS.removeName(done, Key);
	    if (! done)
	        error_stream << "NameServer::removeName() : Failed.\n" << flush;
	}
	else
	    cout<<"\nInvalid command ?\n" << flush;

	cout<<"\nCommand : " << flush;
	cin>>ch;
    }

    return 0;
}
