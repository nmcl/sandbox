/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerGroupEdit.cc,v 1.8 1995/09/12 14:12:09 nmcl Exp $
 */

#include <Config/Configure.h>

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
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
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

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif


#define MaxObjNameSize 128
#define UidSize 64

const int groupSize = 20;

int main ()
{
    Boolean finished = FALSE, done;
    int res = -1;

    NSInterface NS(res);

    if (res != 0)
    {
	error_stream << WARNING << "Could not open/create name server.\n" ;
	return 1;
    }

    while (!finished)
    {
	char opt;

	cout << "\n1: Create\n";
	cout << "2: Delete\n";
	cout << "3: Modify\n";
	cout << "4: List\n";
	cout << "5: Fetch\n";
	cout << "6: Help\n";
	cout << "7: QUIT\n";

	cin >> opt;

	switch (opt)
	{
	case '1':
	{
	    ArjunaName ANKey, ANData[groupSize];
	    Buffer Key, Data;
	    int number = 0;
	    Boolean isPrimary = FALSE;
	    char ObjName[MaxObjNameSize+1], ServiceName[MaxObjNameSize+1], HostName[MAXHOSTNAMELEN];
	    char ans, uidString[UidSize+1];
	    Uid* keyUid = 0;

	    cout << "How many members in the group are there?\n" << flush;
	    cin >> number;

	    if ((number < 1) || (number > groupSize))
	    {
		cout << "Error. There must be at least one member and no more than " << groupSize << endl;
		break;
	    }
	    
	    ::memset(ObjName, '\0', MaxObjNameSize+1);
	    cout << "Object Name : ";
	    cin >> ObjName;
	    ANKey.setObjectName(ObjName);
	    
	    cout << "Generate Uid for Key?\n" << flush;
	    cin >> ans;

	    if ((ans == 'Y') || (ans == 'y'))
	    {
		keyUid = new Uid;
	    }
	    else
	    {
		cout << "What is Key Uid?\n" << flush;
		::memset(uidString, '\0', UidSize+1);
		cin >> uidString;
		keyUid = new Uid(uidString);
	    }

	    ANKey.setObjectUid(*keyUid);

	    cout << "Key is : " << ANKey << "\n" << flush;

	    done = TRUE;

	    cout << "****" << endl;
	    for (int i = 0; ((i < number) && (done)); i++)
	    {
		cout << "\nEntry number " << i+1 << endl;

		ANData[i].setObjectName(ObjName);

		::memset(ServiceName, '\0', MaxObjNameSize+1);
		cout << "Service Name : ";
		cin >> ServiceName;
		ANData[i].setServiceName(ServiceName);

		::memset(HostName, '\0', MAXHOSTNAMELEN);
		cout << "Host Name : ";
		cin >> HostName;
		ANData[i].setHostName(HostName);

		done = ANData[i].pack(Data);
		if (done)
		{
		    done = Data.pack(isPrimary);

		    if ((i == 0) && (done))
			done = Data.pack(number);
		}
	    }

	    if (!done)
		error_stream << WARNING << "Packing ArjunaNames failed.\n" << flush;
	    else
	    {
		done = ANKey.pack(Key);
		if (!done)
		    error_stream << WARNING << "Packing ArjunaNames failed.\n" << flush;
		else
		{
		    AtomicAction A;

		    done = FALSE;
		    A.Begin();

		    NS.storeName(done, Key, Data);
		    if (!done)
		    {
			error_stream << WARNING << "NameServer::storeName() : Failed.\n";
			A.Abort();
		    }
		    else
		    {
			if (A.End() == COMMITTED)
			    cout << "NameServer::storeName() : Succeeded.\n" << flush;
			else
			    cout << "NameServer::storeName() : action failed.\n" << flush;
		    }
		}
	    }
	}
	break;
        case '2': 
        {
	    AtomicAction A;
	    ArjunaName AN;
	    char ObjName[MaxObjNameSize+1], uidString[65];
	    Buffer Key;

	    ::memset(ObjName, '\0', MaxObjNameSize+1);
	    ::memset(uidString, '\0', UidSize+1);
	    
	    cout << "Object Name : ";
	    cin >> ObjName;

	    cout << "Uid : ";
	    cin >> uidString;

	    Uid u(uidString);
	    
	    AN.setObjectName(ObjName);
	    AN.setObjectUid(u);
	    AN.pack(Key);
		  
	    done = FALSE;
	    A.Begin();

	    NS.removeName(done, Key);

	    if (!done)
	    {
		error_stream << WARNING << "NameServer::removeName() : Failed.\n";
		A.Abort();
	    }
	    else
	    {
		if (A.End() == COMMITTED)
		    cout << "NameServer::removeName() : Succeeded.\n" << flush;
		else
		    cout << "NameServer::removeName() : action failed\n" << flush;
	    }
	}
	break;
        case '3' :
        {
	    ArjunaName ANKey, ANData[groupSize];
	    Buffer Key, Data;
	    int number, choice = 0;
	    Boolean isPrimary = FALSE;
	    char ObjName[MaxObjNameSize+1], uidString[UidSize+1];
	    AtomicAction A;
	
	    ::memset(ObjName, '\0', MaxObjNameSize+1);
	    ::memset(uidString, '\0', UidSize+1);
	    
	    cout << "Object Name : ";
	    cin >> ObjName;

	    cout << "Uid : ";
	    cin >> uidString;

	    Uid u(uidString);
	    
	    ANKey.setObjectName(ObjName);
	    ANKey.setObjectUid(u);
	    ANKey.pack(Key);
	    done = FALSE;

	    A.Begin();

	    NS.fetchName(done, Key, Data);
	    if (!done)
	    {
		error_stream << WARNING << "NameServer::fetchName() : Failed.\n";
		A.Abort();
		break;
	    }

	    if (A.End() != COMMITTED)
	    {
		error_stream << WARNING << "NameServer::fetchName() : action failed.\n" << flush;
		break;
	    }

	    ArjunaName ANtemp;
	    Boolean headFlag;

	    Data.reread();
	    done = ANtemp.unpack(Data) && Data.unpack(headFlag) && Data.unpack(number);
	    if (!done)
	    {
		error_stream << WARNING << "ArjunaName unpack Failed.\n";
		break;
	    }

	    cout << "1: Add to Group\n" << flush;
	    cout << "2: Delete from Group\n" << flush;
	    cout << "3: Return to main menu\n" << flush;
	    
	    cin >> choice;
	    
	    if (choice == 1)
	    {
		ArjunaName ANX;
		char ServiceName[MaxObjNameSize+1], HostName[MAXHOSTNAMELEN];
		AtomicAction B;
		Buffer NewData;
		int X;
	    
		if (number == groupSize)
		{
		    cout << "Group is maximum allowed size. Cannot add new member.\n" << flush;
		    break;
		}

		number++;
		::memset(ServiceName, '\0', MaxObjNameSize+1);
		cout << "Service Name : ";
		cin >> ServiceName;
		ANX.setServiceName(ServiceName);

		::memset(HostName, '\0', MAXHOSTNAMELEN);
		cout << "Host Name : ";
		cin >> HostName;
		ANX.setHostName(HostName);
		ANX.setObjectName(ObjName);

		ANtemp.pack(NewData);
		NewData.pack(headFlag);
		NewData.pack(number);

		for (int k = 1; k < number -1; k++)
		{
		    ANtemp.unpack(Data);
		    Data.unpack(X);
		    ANtemp.pack(NewData);
		    NewData.pack(X);
		}

		ANX.pack(NewData);
		NewData.pack(isPrimary);

		done = FALSE;
		B.Begin();

		NS.replaceName(done, Key, NewData);
		if (!done)
		{
		    error_stream << WARNING << "NameServer::replaceName() : Failed.\n";
		    B.Abort();
		}
		else
		{
		    if (B.End() == COMMITTED)
			cout << "NameServer::replaceName() : Succeeded.\n" << flush;
		    else
			cout << "NameServer::replaceName() : action failed.\n" << flush;
		}
	    }
	    else if (choice == 2)
	    {
		int primary = -1;
		AtomicAction B;
		Buffer NewData;
		Boolean haveDeleted = FALSE;

		if (number == 1)
		{
		    cout << "Group only has one member. Use Delete Group option.\n" << flush;
		    break;
		}

		for (int i = 0; i < number; i++)
		{
		    if (i == 0)
		    {
			ANData[0] = ANtemp;
			isPrimary = headFlag;
		    }
		    else
		    {
			ANData[i].unpack(Data);
			Data.unpack(isPrimary);
		    }
		    
		    if (isPrimary)
		    {
			primary = i;
			cout << "Warning. This member is flagged as being the primary.";
			cout << "Check it is not in use.\n" << flush;
			cout << ": " << ANData[i] << "\n" << flush;
		    }
		}

		number--;
		for (int j = 0; j < number+1; j++)
		{
		    char YN = 'n';
		
		    if (!haveDeleted)
		    {
			cout << "Data : " << ANData[j] << "\n" << flush;
			cout << "Delete? (Y/N)\n" << flush;
			cin >> YN;
		    }

		    if ((YN != 'Y') && (YN != 'y'))
		    {
			ArjunaName TempA = ANData[j];
			TempA.pack(NewData);

			if (j == primary)
			    NewData.pack(TRUE);
			else
			    NewData.pack(FALSE);
		    
			if (j == 0)
			    NewData.pack(number);
		    }
		    else
			haveDeleted = TRUE;
		}

		if (haveDeleted)
		{
		    done = FALSE;
		    B.Begin();
		    
		    NS.replaceName(done, Key, NewData);
		    if (!done)
		    {
			error_stream << WARNING << "NameServer::replaceName() : Failed.\n";
			B.Abort();
		    }
		    else
		    {
			if (B.End() == COMMITTED)
			    cout << "NameServer::replaceName() : Succeeded.\n" << flush;
			else
			    cout << "NameServer::replaceName() : action failed.\n" << flush;
		    }
		}
	    }
	}
	break;
        case '4' : 
        {
	    Boolean eod, theEnd = FALSE;
	    AtomicAction A;

	    A.Begin();

	    done = FALSE;
	    NS.restart(done);

	    while (!theEnd)
	    {
		cout << "****\n" << flush;

		Buffer Key;
		done = FALSE;
		eod = TRUE;
		NS.getNextName(done, Key, eod);

		if (!done)
		{
		    error_stream << WARNING << "\nNameServer::getNextName() : Failed.\n";
		    A.End();
		    theEnd = TRUE;
		    break;
		}

		if (!eod)
		{
		    Buffer Data;
			      
		    done = FALSE;
		    NS.fetchName(done, Key, Data);

		    if (!done)
		    {
			error_stream << WARNING << "\nNameServer::fetchName() : Failed.\n";
			A.Abort();
			theEnd = TRUE;
			break;
		    }
		    else
		    {
			ArjunaName ANKey, ANData[groupSize];
			int numb, indx = 0;
			Boolean flag;

			ANKey.unpack(Key);
			cout << "Key : " << ANKey << "\n" << flush;

			do
			{
			    done = ANData[indx].unpack(Data) && Data.unpack(flag);
			    if (done)
			    {
				if (indx == 0)
				    done = Data.unpack(numb);

				if (done)
				{
				    cout << "Data : " << ANData[indx];
				    if (flag)
					cout << " <Primary>";
				    cout << endl;
				    indx++;
				}
			    }
			} while ((indx < numb) && (done));
		    }
		}
	    }
	}
	break;
        case '5' :
        {
	    ArjunaName ANKey, ANData[groupSize];
	    Buffer Key, Data;
	    char ObjName[MaxObjNameSize+1], uidString[UidSize+1];
	    AtomicAction A;
	    int number = 1;
	    Boolean flag = FALSE;

	    ::memset(ObjName, '\0', MaxObjNameSize+1);
	    ::memset(uidString, '\0', UidSize+1);
	    
	    cout << "Object Name : ";
	    cin >> ObjName;

	    cout << "Uid : ";
	    cin >> uidString;

	    Uid u(uidString);
	    
	    ANKey.setObjectName(ObjName);
	    ANKey.setObjectUid(u);

	    done = FALSE;

	    A.Begin();

	    cout << "Key : " << ANKey << "\n" << flush;
	    ANKey.pack(Key);
	    NS.fetchName(done, Key, Data);
	    if (!done)
	    {
		error_stream << WARNING << "NameServer::fetchName() : Failed.\n";
		A.Abort();
		break;
	    }
	    else
	    {
		if (A.End() != COMMITTED)
		{
		    error_stream << WARNING << "NameServer::action failed.\n" << flush;
		    break;
		}
	    }

	    done = ANData[0].unpack(Data);
	
	    if (!done)
	    {
		error_stream << WARNING << "ArjunaName unpack Failed.\n";
		break;
	    }

	    if (Data.unpack(flag))
		Data.unpack(number);

	    for (int i = 0; i < number; i++)
	    {
		if (i != 0)
		{
		    ANData[i].unpack(Data);
		    Data.unpack(flag);
		}
		
		cout << ANData[i];
		if (flag)
		    cout << " <Primary>";
		cout << endl;
	    }
	}
	break;
        case '6' :
	break;
        case '7' : 
        {
	    finished = TRUE;
	    break;
	}
	default :
	break;
    }
    }
}
