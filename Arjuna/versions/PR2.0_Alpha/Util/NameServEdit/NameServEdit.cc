/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServEdit.cc,v 1.8 1993/03/30 13:13:56 nmcl Exp $
 *
 * NameServerEdit source code.
 *
 */

#include <Configure.h>

#ifdef DEBUG
#include <Common/Debug.h>
#endif

#include <System/sys/param.h>

#include <System/string.h>

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#include <Configure.h>

#include <Common/Error.h>
#include <Common/Boolean.h>

#include <RPC/ArjServers/ArjName.h>
#include <RPC/ArjServers/NSInterface.h>

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
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
		NS.StoreName(done, Key, Data);
		if (! done)
		    error_stream << "NameServer::StoreName() : Failed.\n" << flush;
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
	    NS.FetchName(done, Key, Data);
	    if (! done)
	        error_stream << "NameServer::FetchName() : Failed.\n" << flush;
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

	    NS.Restart(done);

	    cout<<"****\n" << flush;
	    while (! finish)
	    {
		Buffer Key;
	        NS.GetNextName(done, Key, eod);

		if (! eod)
		{
		    if (! done)
			error_stream << "\nNameServer::GetNextName() : Failed.\n" << flush;
		    else
		    {
			Buffer Data;

			done = FALSE;
			NS.FetchName(done, Key, Data);

			if (! done)
			    error_stream << "\nNameServer::FetchName() : Failed.\n" << flush;
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

	    NS.RemoveName(done, Key);
	    if (! done)
	        error_stream << "NameServer::RemoveName() : Failed.\n" << flush;
	}
	else
	    cout<<"\nInvalid command ?\n" << flush;

	cout<<"\nCommand : " << flush;
	cin>>ch;
    }

    return 0;
}
