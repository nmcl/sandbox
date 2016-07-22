/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupViewDriver.cc
 */

#ifndef CONFIGURE_H_
#include <Configure.h>
#endif

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#include <RPC/ArjServers/NameServ_stub.h>

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#include <RPC/ArjServers/NSInterface.h>

const int uidSize = 80; // ??
const int hostSize = 400; // ??

void GetUid (ReplicaDescriptor& rd)
{
    char* u_id = new char[uidSize];
    ::memset(u_id, '\0', uidSize);

    cin >> u_id;
    Uid u(u_id);
    rd.SetUid(u_id);
}

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    _arjuna_debug->set_debuglevel(FULL_DEBUGGING);
    
    Boolean finished = FALSE;
    Boolean opcode;
    Boolean done = FALSE;
    int res = -1;
    Uid* group = 0;
    NSInterface* GV = 0;

    if (argc == 2)
    {
	if (strcmp(argv[1], "-new") == 0)
	{
	    GV = new NSInterface(done); // create new nameserver
	    
	    if (done)
		res = 0;
	}
    }
    else
    {
	GV = new NSInterface(res); // use old nameserver
    }

    cout << "GroupView Database Editor\n" << flush;

    if (res != 0)
    {
	cerr << "**Error while creating GroupView Database**\n" << flush;
	if (GV)
	    delete GV;
	return 1;
    }
    else
	cout << "**Created GroupView Database**\n" << flush;

    while (!finished)
    {
	char opt;
	cout << "1: Traverse Database\n" << flush;
	cout << "2: Get Replica Group View\n" << flush;
	cout << "3: Create Replica Group View\n" << flush;
	cout << "4: Delete Replica Group View\n" << flush;
	cout << "5: Add Member to Group\n" << flush;
	cout << "6: Delete Member from Group\n" << flush;
	cout << "7: Deactivate Replica\n" << flush;
	cout << "8: Reactivate Replica\n" << flush;
	cout << "9: Quit\n" << flush;

	cin >> opt;

	switch (opt)
	{
	case '1':
	{
	    ReplicaDescriptor rd(0), *ptr;
	    AtomicAction A;
	    int position = 0;
	    Uid group, u_id(NIL_UID);
		
	    rd.SetUid(u_id);
	    opcode = TRUE;

	    A.Begin();
	    
	    while (opcode)
	    {
		opcode = GV->Traverse_Database(rd, group, position);
	       
		if (position == -1)
		{
		    cout << "End of database reached\n" << flush;
		    if (A.End() != COMMITTED)
			cerr << "**An error has occurred while committing action**\n" << flush;
		    break;
		}

		if (!opcode)
		{
		    cerr << "**An error has occurred while traversing the database**\n" << flush;
		    A.Abort();
		    break;
		}

		ptr = &rd;
		cout << "\n****\nEntry " << position << "\n" << flush;
		cout << "Replica group UID " << group << " with the following members\n" << flush;

		while (ptr)
		{
		    cout << ptr->GetHost() << "\n" << flush;
		    cout << "with UID " << *ptr->GetUid() << "\n" << flush;
		    if (ptr->GetActive())
			cout << "it is active\n" << flush;
		    else
			cout << "it is not active\n" << flush;

		    ptr = ptr->next;
		}
	    }
	}
	break;
	case '2':
	{
	    cout << "Give UID of group view to inspect\n" << flush;

	    Uid old;
	    ReplicaDescriptor rd(0), *ptr;
	    GetUid(rd);
	    old = *rd.GetUid();

	    cout << "Scanning for " << old << "\n" << flush;

	    AtomicAction A;

	    A.Begin();
	    
	    opcode = GV->Get_View(rd, old);

	    if (!opcode)
	    {
		cerr << "**An error has occurred during Get_View**\n" << flush;
		A.Abort();
		break;
	    }
	    else
	    {
		if (A.End() != COMMITTED)
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}

		if (opcode)
		{
		    ptr = &rd;
		    cout << "The replica group with UID " << old << " has the following members:\n" << flush;

		    while (ptr)
		    {
			cout << ptr->GetHost() << "\n" << flush;
			cout << "with UID " << *ptr->GetUid() << "\n" << flush;
			if (ptr->GetActive())
			    cout << "it is active\n" << flush;
			else
			    cout << "it is not active\n" << flush;

			ptr = ptr->next;
		    }
		}
		else
		    cout << "Entry not found in database\n" << flush;
	    }
	}
	break;
        case '3':
        {
	    AtomicAction A;
	    char Name[hostSize];
	    ::memset(Name, '\0', hostSize);

	    if (group)
		delete group;

	    group = new Uid;

	    cout << "Group Uid will be " << *group << "\n" << flush;

	    cout << "Give the location of the object\n" << flush;
	    cin >> Name;
	    ReplicaDescriptor rd(Name);

	    cout << "Give the object's Uid\n" << flush;
	    GetUid(rd);

	    A.Begin();

	    opcode = GV->Add_Replicas(rd, *group);
	    
	    if (opcode)
	    {
		cout << "Added group view successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "**An error occurred while attempting to add group view**\n" << flush;
		A.Abort();
	    }
	}
	break;
        case '4':
        {
	    cout << "Give Uid of Replica Group to delete\n" << flush;

	    AtomicAction A;
	    ReplicaDescriptor rd(0);
	    Uid *uidDelete = 0, u(NIL_UID);
	    char* uidString = new char[uidSize];
	    ::memset(uidString, '\0', uidSize);

	    cin >> uidString;

	    uidDelete = new Uid(uidString);
	    rd.SetUid(u);

	    A.Begin();

	    opcode = GV->Delete_Replicas(rd, *uidDelete, TRUE);

	    delete uidDelete;

	    if (opcode)
	    {
		cout << "Deleted group successfully\n" << flush;
		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "**An error has occurred while attempting to delete group\n" << flush;
		A.Abort();
	    }
	}
	break;
        case '5': 
        {
	    AtomicAction A;
	    char *group_uid = new char[uidSize];
	    char Name[hostSize];

	    ::memset(group_uid, '\0', uidSize);
	    ::memset(Name, '\0', hostSize);

	    if (group)
		delete group;

	    cout << "Give Uid of Replica Group\n" << flush;
	    cin >> group_uid;

	    group = new Uid(group_uid);

	    cout << "Give the location of the object\n" << flush;
	    cin >> Name;
	    ReplicaDescriptor rd(Name);

	    cout << "Give the object's Uid\n" << flush;
	    GetUid(rd);

	    A.Begin();

	    opcode = GV->Add_Replicas(rd, *group);

	    if (opcode)
	    {
		cout << "Added successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "**An error occurred while attempting to add entry**\n" << flush;
		A.Abort();
	    }
	}
	break;
        case '6':
        {
	    char *group_uid = new char[uidSize];
	    char Name[hostSize];
	    
	    ::memset(group_uid, '\0', uidSize);
	    ::memset(Name, '\0', hostSize);
	    
	    cout << "Give UID of group view to delete from\n" << flush;
	    cin >> group_uid;
	    group = new Uid(group_uid);

	    cout << "Give the location of the object to delete\n" << flush;
	    cin >> Name;
	    ReplicaDescriptor rd(Name);

	    cout << "Give the Uid of the replica to be deleted\n" << flush;
	    GetUid(rd);

	    AtomicAction A;

	    A.Begin();

	    opcode = GV->Delete_Replicas(rd, *group);

	    if (opcode)
	    {
		cout << "Deleted entry successfully\n" << flush;
		    
		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "**An error occurred while deleting entry**\n" << flush;
		A.Abort();
	    }
	}
	break;
        case '7':
        {
	    cout << "Give UID of group to deactive replica from\n" << flush;

	    char Name[hostSize];
	    ::memset(Name, '\0', hostSize);
	    Uid *ud;
	    ReplicaDescriptor rd(0);
	    GetUid(rd);

	    ud = new Uid(NIL_UID);
	    *ud = *rd.GetUid();

	    cout << "Give the UID of the object to deactivate\n" << flush;
	    GetUid(rd);

	    cout << "Give the location of the object to deactivate\n" << flush;
	    cin >> Name;
	    rd.SetHost(Name);
	    
	    AtomicAction A;

	    A.Begin();

	    rd.SetActive(FALSE);
	    opcode = GV->ChangeReplicaStatus(rd, *ud, FALSE);

	    if (opcode)
	    {
		cout << "Deactivated entry successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "**An error occurred while deactivating**\n" << flush;
		A.Abort();
	    }
	}
	break;
        case '8':
        {
	    cout << "Give UID of group to reactive replica in\n" << flush;

	    char Name[hostSize];
	    ::memset(Name, '\0', hostSize);
	    Uid *ud;
	    ReplicaDescriptor rd(0);
	    GetUid(rd);
	    
	    ud = new Uid(NIL_UID);
	    *ud = *rd.GetUid();

	    cout << "Give the UID of the object to reactivate\n" << flush;
	    GetUid(rd);

	    cout << "Give the location of the object to reactivate\n" << flush;
	    cin >> Name;
	    rd.SetHost(Name);
	    
	    AtomicAction A;
	    
	    A.Begin();

	    rd.SetActive(TRUE);
	    opcode = GV->ChangeReplicaStatus(rd, *ud, TRUE);

	    if (opcode)
	    {
		cout << "Reactivated entry successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "**An error occurred while reactivating**\n" << flush;
		A.Abort();
	    }
	}
	break;
        case '9':
	finished = TRUE;
	break;
        default:
	break; }
    }

    if (GV)
	delete GV;

    return 0;
}
