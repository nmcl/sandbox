/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerGroupEdit.cc,
 */

#include <Configure.h>

#ifdef DEBUG
#include <Common/Debug.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef ERROR_H_
#include <Common/Error.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#include <RPC/ArjServers/ArjName.h>
#include <RPC/ArjServers/NSInterface.h>
#include <Arjuna/AtomicA.h>

const int groupSize = 10;

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    Boolean finished = FALSE, done;
    int res = -1;

    NSInterface NS(res);

    if (res != 0)
    {
	error_stream << "Could not open name server.\n" ;
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
	    char ObjName[32], ServiceName[32], HostName[MAXHOSTNAMELEN];
	    char ans, uidString[64];
	    Uid* keyUid = 0;

	    cout << "How many members in the group are there?\n" << flush;
	    cin >> number;

	    if (number < 1)
	    {
		cout << "Error. There must be at least one member!\n" << flush;
		break;
	    }
	    
	    ::memset(ObjName, '\0', 32);
	    cout << "Object Name : ";
	    cin >> ObjName;
	    ANKey.SetObjName(ObjName);
	    
	    cout << "Generate Uid for Key?\n" << flush;
	    cin >> ans;

	    if ((ans == 'Y') || (ans == 'y'))
	    {
		keyUid = new Uid;
	    }
	    else
	    {
		cout << "What is Key Uid?\n" << flush;
		::memset(uidString, '\0', 64);
		cin >> uidString;
		keyUid = new Uid(uidString);
	    }

	    ANKey.SetObjUid(*keyUid);

	    cout << "Key is : " << ANKey << "\n" << flush;

	    done = TRUE;

	    for (int i = 0; ((i < number) && (done)); i++)
	    {
		ANData[i].SetObjName(ObjName);

		::memset(ServiceName, '\0', 32);
		cout << "Service Name : ";
		cin >> ServiceName;
		ANData[i].SetServiceName(ServiceName);

		::memset(HostName, '\0', MAXHOSTNAMELEN);
		cout << "Host Name : ";
		cin >> HostName;
		ANData[i].SetHostName(HostName);

		ANData[i].pack(Data);
		Data.pack(isPrimary);

		if (i == 0)
		    Data.pack(number);
	    }

	    if (!done)
		error_stream << "Packing ArjunaNames failed.\n" << flush;
	    else
	    {
		done = ANKey.pack(Key);
		if (!done)
		    error_stream << "Packing ArjunaNames failed.\n" << flush;
		else
		{
		    AtomicAction A;

		    done = FALSE;
		    A.Begin();

		    NS.StoreName(done, Key, Data);
		    if (!done)
		    {
			error_stream << "NameServer::StoreName() : Failed.\n";
			A.Abort();
		    }
		    else
		    {
			if (A.End() == COMMITTED)
			    cout << "NameServer::StoreName() : Succeeded.\n" << flush;
			else
			    cout << "NameServer::StoreName() : action failed.\n" << flush;
		    }
		}
	    }
	}
	break;
        case '2': 
        {
	    AtomicAction A;
	    ArjunaName AN;
	    char ObjName[32];
	    Buffer Key;

	    ::memset(ObjName, '\0', 32);
	    cout << "Object Name : ";
	    cin >> ObjName;
	
	    AN.SetObjName(ObjName);
	    AN.pack(Key);
		  
	    done = FALSE;
	    A.Begin();

	    NS.RemoveName(done, Key);

	    if (!done)
	    {
		error_stream << "NameServer::RemoveName() : Failed.\n";
		A.Abort();
	    }
	    else
	    {
		if (A.End() == COMMITTED)
		    cout << "NameServer::RemoveName() : Succeeded.\n" << flush;
		else
		    cout << "NameServer::RemoveName() : action failed\n" << flush;
	    }
	}
	break;
        case '3' :
        {
	    ArjunaName ANKey, ANData[groupSize];
	    Buffer Key, Data;
	    int number, choice = 0;
	    Boolean isPrimary = FALSE, isModified = FALSE;
	    char ObjName[32];
	    AtomicAction A;
	
	    ::memset(ObjName, '\0', 32);
	    cout << "Object Name : ";
	    cin >> ObjName;

	    ANKey.SetObjName(ObjName);
	    ANKey.pack(Key);
	    done = FALSE;

	    A.Begin();

	    NS.FetchName(done, Key, Data);
	    if (!done)
	    {
		error_stream << "NameServer::FetchName() : Failed.\n";
		A.Abort();
		break;
	    }

	    if (A.End() != COMMITTED)
	    {
		error_stream << "NameServer::FetchName() : action failed.\n" << flush;
		break;
	    }

	    ArjunaName ANtemp;
	    Boolean headFlag;

	    Data.reread();
	    done = ANtemp.unpack(Data) && Data.unpack(headFlag) && Data.unpack(number);
	    if (!done)
	    {
		error_stream << "ArjunaName unpack Failed.\n";
		break;
	    }

	    cout << "1: Add to Group\n" << flush;
	    cout << "2: Delete from Group\n" << flush;
	    cout << "3: Return to main menu\n" << flush;
	    
	    cin >> choice;
	    
	    if (choice == 1)
	    {
		ArjunaName ANX;
		char ServiceName[32], HostName[MAXHOSTNAMELEN];
		AtomicAction B;
		Buffer NewData;
		int X;
	    
		if (number == groupSize)
		{
		    cout << "Group is maximum allowed size. Cannot add new member.\n" << flush;
		    break;
		}

		number++;
		::memset(ServiceName, '\0', 32);
		cout << "Service Name : ";
		cin >> ServiceName;
		ANX.SetServiceName(ServiceName);

		::memset(HostName, '\0', MAXHOSTNAMELEN);
		cout << "Host Name : ";
		cin >> HostName;
		ANX.SetHostName(HostName);
		ANX.SetObjName(ObjName);

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

		NS.ReplaceName(done, Key, NewData);
		if (!done)
		{
		    error_stream << "NameServer::ReplaceName() : Failed.\n";
		    B.Abort();
		}
		else
		{
		    if (B.End() == COMMITTED)
			cout << "NameServer::ReplaceName() : Succeeded.\n" << flush;
		    else
			cout << "NameServer::ReplaceName() : action failed.\n" << flush;
		}
		
		break;
	    }
	    else if (choice == 2)
	    {
		int primary = -1;
		AtomicAction B;
		Buffer NewData;
		Boolean haveDeleted = FALSE;
		ArjunaName Temp;

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
		    
		    NS.ReplaceName(done, Key, NewData);
		    if (!done)
		    {
			error_stream << "NameServer::ReplaceName() : Failed.\n";
			B.Abort();
		    }
		    else
		    {
			if (B.End() == COMMITTED)
			    cout << "NameServer::ReplaceName() : Succeeded.\n" << flush;
			else
			    cout << "NameServer::ReplaceName() : action failed.\n" << flush;
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
	    NS.Restart(done);

	    while (!theEnd)
	    {
		cout << "****\n" << flush;

		Buffer Key;
		done = FALSE;
		eod = TRUE;
		NS.GetNextName(done, Key, eod);

		if (!done)
		{
		    error_stream << "\nNameServer::GetNextName() : Failed.\n";
		    A.Abort();
		    theEnd = TRUE;
		    break;
		}

		if (!eod)
		{
		    Buffer Data;
			      
		    done = FALSE;
		    NS.FetchName(done, Key, Data);

		    if (!done)
		    {
			error_stream << "\nNameServer::FetchName() : Failed.\n";
			A.Abort();
			theEnd = TRUE;
			break;
		    }
		    else
		    {
			ArjunaName ANKey, ANData[groupSize];
			int numb, indx = 1;
			Boolean flag;
		    
			ANData[0].unpack(Data);
			Data.unpack(flag);
			Data.unpack(numb);

			ANKey.unpack(Key);
			cout << "Key : " << ANKey << "\n" << flush;
			cout << "Data : " << ANData[0] << "\n" << flush;

			done = TRUE;
			do
			{
			    done = ANData[indx].unpack(Data) && Data.unpack(flag);
			    if (done)
				cout << "Data : " << ANData[indx] << "\n" << flush;
			    indx++;
			} while ((indx < numb) && (done));
		       
			A.End();
		    }
		}
	    }
	}
	break;
        case '5' :
        {
	    ArjunaName ANKey, ANData[groupSize];
	    Buffer Key, Data;
	    char ObjName[32];
	    AtomicAction A;
	    int number = 1;
	    Boolean flag = FALSE;

	    ::memset(ObjName, '\0', 32);
	    cout << "Object Name : ";
	    cin >> ObjName;
	    ANKey.SetObjName(ObjName);

	    done = FALSE;

	    A.Begin();

	    cout << "Key : " << ANKey << "\n" << flush;
	    ANKey.pack(Key);
	    NS.FetchName(done, Key, Data);
	    if (!done)
	    {
		error_stream << "NameServer::FetchName() : Failed.\n";
		A.Abort();
		break;
	    }
	    else
	    {
		if (A.End() != COMMITTED)
		{
		    error_stream << "NameServer::action failed.\n" << flush;
		    break;
		}
	    }

	    done = ANData[0].unpack(Data);
	
	    if (!done)
	    {
		error_stream << "ArjunaName unpack Failed.\n";
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
		
		cout << ANData[i] << "\n" << flush;
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
