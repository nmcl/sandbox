/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupViewDriver.cc,v
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NSINTERFACE_H_
#  include <RPC/ArjServers/NSInterface.h>
#endif

#ifndef GROUPDATA_H_
#  include <RPC/ArjServers/GroupData.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


// various magic numbers (remove later!)

const int uidSize = 80;
const int hostSize = 400;
const int pathSize = 256;

extern const char* templateName;
extern const char* wildCardName;
extern const char* globalWildCardName;


void getUid (ReplicaDescriptor& rd)
{
    char* u_id = new char[uidSize];
    ::memset(u_id, '\0', uidSize);

    cin >> u_id;
    Uid u(u_id);
    rd.setUid(u_id);
}

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    Boolean finished = FALSE;
    OutCome result = NotDone;
    Boolean done = FALSE;
    int res = -1;
    pid_t pid;
    Uid* group = 0;
    NSInterface* GV = 0;
    char myHostName[MAXHOSTNAMELEN];
    long UseCount;
    
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

    cout << "\nGroupView Database Editor\n" << endl;

    if (res != 0)
    {
	cerr << "**Error while creating GroupView Database**\n" << flush;
	if (GV)
	    delete GV;
	return -1;
    }
    else
    {
	cout << "Created GroupView Database\n" << endl;
	::memset(myHostName, '\0', MAXHOSTNAMELEN);
	(void) ::gethostname(myHostName, MAXHOSTNAMELEN);
	pid = getpid();
    }

    while (!finished)
    {
	char opt;
	cout << "\n****" << endl;
	cout << "\nA/a: Inspect Database\n" << flush;
	cout << "B/b: Get Replica Group View\n" << endl;
	cout << "C/c: Create Replica Group View\n" << flush;
	cout << "D/d: Delete Replica Group View\n" << endl;
	cout << "E/e: Add Replica to Group\n" << flush;
	cout << "F/f: Delete Replica from Group\n" << endl;
	cout << "G/g: Exclude Replica\n" << flush;
	cout << "H/h: Include Replica\n" << endl;
	cout << "I/i: Recover Node" << endl;
	cout << "J/j: Get Node Exclude List\n" << endl;
	cout << "K/k: Add Template Object" << endl;
	cout << "L/l: Delete Template Object" << endl;
	cout << "M/m: Replace Template Object" << endl;
	cout << "N/n: Inspect Template Object" << endl;
	cout << "O/o: Assign Replica to Template Group" << endl;
	cout << "P/p: Remove Replica from Template Group" << endl;
	cout << "\nQ/q: Assign Composite Object" << endl;
	cout << "R/r: Remove Composite Object\n" << endl;
	cout << "\nX/x: Quit\n" << endl;
	cout << "****\n" << endl;

	cin >> opt;

	switch (opt)
	{
	case 'a':
	case 'A':
	{
	    ReplicaDescriptor* rd = 0;
	    AtomicAction A;
	    int position = 0;
	    Uid groupUID;

	    result = Done;

	    A.Begin();
	    
	    while (result == Done)
	    {
		if (rd)
		    delete rd;
		
		rd = new ReplicaDescriptor;
		result = ((GV->traverseDatabase(*rd, groupUID, myHostName, position, pid) == TRUE) ? Done : NotDone);
		if (result == Done)
		    result = GV->releaseGroup(groupUID, myHostName, pid);
		UseCount = GV->getUseCount(groupUID);

		if (position == -1)
		{
		    cout << "\nEnd of database reached\n" << flush;
		    if (A.End() != COMMITTED)
			cerr << "\n**An error has occurred while committing action**\n" << flush;
		    break;
		}

		if (result != Done)
		{
		    cerr << "\n**An error has occurred while traversing the database**\n" << flush;
		    A.Abort();
		    break;
		}

		cout << "\n****\nEntry " << position << "\n" << flush;
		cout << "Replica group UID " << groupUID << " with the following members:\n" << endl;

		cout << *rd;

		cout << "Use count is : " << UseCount << endl;
	    }

	    if (rd)
		delete rd;
	}
	break;
	case 'b':
	case 'B':
	{
	    Uid old;
	    ReplicaDescriptor rd;

	    cout << "\nGive UID of group view to inspect\n" << flush;
	    getUid(rd);
	    old = rd.getUid();

	    cout << "\nScanning for " << old << "\n" << endl;

	    AtomicAction A;

	    A.Begin();
	    
	    result = ((GV->getView(rd, old, myHostName, pid) == TRUE) ? Done : NotDone);
	    result = GV->releaseGroup(old, myHostName, pid);
	    UseCount = GV->getUseCount(old);
	    
	    if (result != Done)
	    {
		cerr << "\n**An error has occurred during getView**\n" << flush;
		A.Abort();
		break;
	    }
	    else
	    {
		if (A.End() != COMMITTED)
		{
		    cerr << "\n**An error has occurred while committing the action**\n" << flush;
		    break;
		}

		if (result == Done)
		{
		    ReplicaDescriptor* objects = 0;

		    rd.getGroupList(old, objects);

		    cout << "\nThe replica group with UID " << old << " has the following members:\n" << endl;
		    cout << *objects;
		    cout << "Use count is : " << UseCount << endl;
		    delete objects;
		}
		else
		    cout << "\nEntry not found in database\n" << flush;
	    }
	}
	break;
	case 'c':
        case 'C':
        {
	    AtomicAction A;
	    ReplicaDescriptor rd;
	    char Name[hostSize];
	    char tn[pathSize];
	    ::memset(Name, '\0', hostSize);
	    ::memset(tn, '\0', pathSize);

	    if (group)
		delete group;

	    group = new Uid;

	    cout << "\nGroup Uid will be " << *group << "\n" << flush;
	    rd.setGroupUid(*group);

	    cout << "\nGive the object's Uid\n" << flush;
	    getUid(rd);

	    cout << "\nGive the location of the object\n" << flush;
	    cin >> Name;

	    rd.setHost(Name);

	    cout << "\nGive the object type" << endl;
	    cin >> tn;
	    rd.setObjectName(tn);
	    
	    A.Begin();

	    result = GV->addReplicas(rd, *group);
	    
	    if (result == Done)
	    {
		cout << "\nAdded group view successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "\n**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "\n**An error occurred while attempting to add group view**\n" << flush;
		A.Abort();
	    }
	}
	break;
	case 'd':
        case 'D':
        {
	    AtomicAction A;
	    ReplicaDescriptor rd;
	    Uid *uidDelete = 0, u(NIL_UID);
	    char* uidString = new char[uidSize];
	    ::memset(uidString, '\0', uidSize);

	    cout << "\nGive Uid of Replica Group to delete\n" << flush;
	    cin >> uidString;

	    uidDelete = new Uid(uidString);
	    rd.setUid(u);

	    A.Begin();

	    result = GV->deleteReplicas(rd, *uidDelete, TRUE);

	    delete uidDelete;

	    if (result == Done)
	    {
		cout << "\nDeleted group successfully\n" << flush;
		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "\n**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "\n**An error has occurred while attempting to delete group\n" << flush;
		A.Abort();
	    }
	}
	break;
	case 'e':
        case 'E': 
        {
	    AtomicAction A;
	    ReplicaDescriptor rd;
	    char* group_uid = ::new char[uidSize];
	    char Name[hostSize];
	    char tn[pathSize];

	    ::memset(group_uid, '\0', uidSize);
	    ::memset(Name, '\0', hostSize);
	    ::memset(tn, '\0', pathSize);

	    if (group)
		delete group;

	    cout << "\nGive Uid of Replica Group\n" << flush;
	    cin >> group_uid;
	    group = new Uid(group_uid);
	    rd.setGroupUid(*group);

	    cout << "\nGive the object's Uid\n" << flush;
	    getUid(rd);

	    cout << "\nGive the location of the object\n" << flush;
	    cin >> Name;

	    rd.setHost(Name);

	    cout << "\nGive the object's type" << endl;
	    cin >> tn;
	    rd.setObjectName(tn);
	    
	    A.Begin();

	    result = GV->addReplicas(rd, *group);

	    if (result == Done)
	    {
		cout << "\nAdded successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "\n**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "\n**An error occurred while attempting to add entry**\n" << flush;
		A.Abort();
	    }
	}
	break;
	case 'f':
        case 'F':
        {
	    ReplicaDescriptor rd;
	    char *group_uid = new char[uidSize];
	    char Name[hostSize];
	    char tn[pathSize];
	    
	    ::memset(group_uid, '\0', uidSize);
	    ::memset(Name, '\0', hostSize);
	    ::memset(tn, '\0', pathSize);
	    
	    cout << "\nGive UID of group view to delete from\n" << flush;
	    cin >> group_uid;
	    group = new Uid(group_uid);
	    rd.setGroupUid(*group);

	    cout << "\nGive the Uid of the replica to be deleted\n" << flush;
	    getUid(rd);

	    cout << "\nGive the location of the object to delete\n" << flush;
	    cin >> Name;

	    rd.setHost(Name);

	    cout << "\nGive the name of the object" << endl;
	    cin >> tn;
	    rd.setObjectName(tn);
	    
	    AtomicAction A;

	    A.Begin();

	    result = GV->deleteReplicas(rd, *group);

	    if (result == Done)
	    {
		cout << "\nDeleted entry successfully\n" << flush;
		    
		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "\n**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "\n**An error occurred while deleting entry**\n" << flush;
		A.Abort();
	    }
	}
	break;
	case 'g':
        case 'G':
        {
	    char Name[hostSize];
	    char tn[pathSize];
	    Uid *ud;
	    ReplicaDescriptor rd;

	    ::memset(Name, '\0', hostSize);
	    ::memset(tn, '\0', pathSize);

	    cout << "\nGive UID of group to exclude replica from\n" << flush;
	    getUid(rd);
	    ud = new Uid(NIL_UID);
	    *ud = rd.getUid();
	    rd.setGroupUid(*ud);

	    cout << "\nGive the UID of the object to exclude\n" << flush;
	    getUid(rd);

	    cout << "\nGive the location of the object to exclude\n" << flush;
	    cin >> Name;
	    rd.setHost(Name);
	    
	    cout << "\nGive the type of the object" << endl;
	    cin >> tn;
	    rd.setObjectName(tn);
	    
	    AtomicAction A;

	    A.Begin();

	    rd.setActive(FALSE);
	    result = GV->exclude(rd, *ud);

	    if (result == Done)
	    {
		cout << "\nExcluded entry successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "**\nAn error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "\n**An error occurred while excluding**\n" << flush;
		A.Abort();
	    }
	}
	break;
	case 'h':
        case 'H':
        {
	    char Name[hostSize];
	    char tn[pathSize];
	    Uid *ud;
	    ReplicaDescriptor rd;

	    ::memset(Name, '\0', hostSize);
	    ::memset(tn, '\0', pathSize);
	    
	    cout << "\nGive UID of group to include replica in\n" << flush;
	    getUid(rd);
	    ud = new Uid(NIL_UID);
	    *ud = rd.getUid();
	    rd.setGroupUid(*ud);

	    cout << "\nGive the UID of the object to include\n" << flush;
	    getUid(rd);

	    cout << "\nGive the location of the object to include\n" << flush;
	    cin >> Name;
	    rd.setHost(Name);
	    
	    cout << "\nGive the type of the object" << endl;
	    cin >> tn;
	    rd.setObjectName(tn);

	    AtomicAction A;
	    
	    A.Begin();

	    rd.setActive(TRUE);
	    result = GV->include(rd, *ud);

	    if (result == Done)
	    {
		cout << "\nIncluded entry successfully\n" << flush;

		if (A.End() == COMMITTED)
		    break;
		else
		{
		    cerr << "\n**An error has occurred while committing the action**\n" << flush;
		    break;
		}
	    }
	    else
	    {
		cerr << "\n**An error occurred while including**\n" << flush;
		A.Abort();
	    }
	}
	break;
	case 'i':
	case 'I':
        {
	    OutCome result;
	    AtomicAction A;
	    char Name[hostSize];
	    ::memset(Name, '\0', hostSize);

	    cout << "\nWhat is the name of the recovering node?" << endl;
	    cin >> Name;
	    
	    A.Begin();
	    
	    result = GV->recover(Name);
	    
	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while trying to commit action**" << endl;
		else
		    cout << "\nOperation performed successfully" << endl;
		
		break;
	    }
	    else
	    {
		A.Abort();
		cerr << "\n**An error occurred during the action**" << endl;
	    }
	}
	break;
	case 'j':
	case 'J':
        {
	    OutCome result;
	    AtomicAction A;
	    char Name[MAXHOSTNAMELEN+1];
	    ::memset(Name, '\0', MAXHOSTNAMELEN+1);
	    ReplicaDescriptor unknownList, rd;
	    
	    cout << "\nWhat is the name of the node?" << endl;
	    cin >> Name;
	    
	    A.Begin();
	    
	    result = GV->getNodeUpdateList(rd, unknownList, Name);

	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while trying to commit action**" << endl;
		else
		{
		    cout << "\n****" << endl;
		    cout << "Needs updating :" << endl;
		    if (rd != NIL_RD)
			cout << rd << "\n" << endl;
		    else
			cout << "Null list." << endl;
		    cout << "May need updating :" << endl;
		    if (unknownList != NIL_RD)
			cout << unknownList << endl;
		    else
			cout << "Null list." << endl;
		    
		    break;
		}
	    }
	    else
	    {
		A.Abort();
		cerr << "\n**An error occurred during the action**" << endl;
	    }
	}
	break;
	case 'K':
	case 'k':
	case 'M':
	case 'm':
        {
	    Buffer packInto;
	    GroupData *toUse = 0, *ptr = 0;
	    Uid* groupUid = 0;
	    char uidString[uidSize];
	    char hostName[hostSize];
	    char tn[pathSize];
	    char* templateType = 0;
	    AtomicAction A;
	    int i = 0;
	    Boolean res;
	    char opt2;
	    
	    ::memset(uidString, '\0', uidSize);

	    cout << "\nWhat is the Uid of the group to add the template to?" << endl;
	    cin >> uidString;
	    groupUid = new Uid(uidString);
	    
	    cout << "\nHow many elements in the template?" << endl;
	    cin >> i;
	    
	    cout << "\nIs this a wild card template (y/n)?" << endl;
	    cin >> opt2;
	    if (opt2 == 'y')
	    {
		cout << "\nIs this a global wild card (y/n)?" << endl;
		cin >> opt2;
		if (opt2 == 'y')
		    templateType = (char*) globalWildCardName;
		else
		    templateType = (char*) wildCardName;
	    }
	    else
		templateType = (char*) templateName;
	    
	    for (int j = 0; j < i; j++)
	    {
		if (ptr == 0)
		{
		    toUse = new GroupData;
		    ptr = toUse;
		}
		else
		{
		    ptr->next = new GroupData;
		    ptr = ptr->next;
		}
		
		::memset(hostName, '\0', hostSize);
		cout << "\nWhat is host name " << j+1 << "?" << endl;
		cin >> hostName;
		ptr->setHost(hostName);
		ptr->setObjectName(templateType);
	    }
	    
	    res = toUse->pack(packInto);
	    delete toUse;

	    if (!res)
	    {
		cerr << "\n**An error occurred while packing template**" << endl;
		break;
	    }
	    
	    A.Begin();
	    
	    if ((opt == 'K') || (opt == 'k'))
		result = GV->addTemplate(*groupUid, packInto);
	    else
		result = GV->replaceTemplate(*groupUid, packInto);
	    
	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while committing action**" << endl;
		else
		{
		    if ((opt == 'K') || (opt == 'k'))
			cout << "\nTemplate added successfully" << endl;
		    else
			cout << "\nReplaced template successfully" << endl;
		}
		break;
	    }
	    else
	    {
		cerr << "\n**An error occurred while attempting to add template**" << endl;
		A.Abort();
	    }
	}
	break;
	case 'L':
	case 'l':
        {
	    AtomicAction A;
	    char uidString[uidSize];
	    Uid* groupUid = 0;
	    
	    ::memset(uidString, '\0', uidSize);
	    
	    cout << "\nWhat is the group Uid?" << endl;
	    cin >> uidString;
	    groupUid = new Uid(uidString);
	    
	    A.Begin();
	    
	    result = GV->deleteTemplate(*groupUid);
	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while committing action**" << endl;
		else
		    cout << "\nTemplate deleted successfully" << endl;
		break;
	    }
	    else
	    {
		cerr << "\n**An error occurred while attempting to delete template**" << endl;
		A.Abort();
	    }
	}
	break;
	case 'N':
	case 'n':
        {
	    AtomicAction A;
	    Boolean result;
	    char uidString[uidSize];
	    Uid* groupUid = 0;
	    Buffer toUse;
	    GroupData theObject;
	    Boolean res;
	    
	    ::memset(uidString, '\0', uidSize);
	    
	    cout << "\nWhat is the Uid of the object to inspect?" << endl;
	    cin >> uidString;
	    groupUid = new Uid(uidString);
	    
	    A.Begin();
	    
	    result = GV->getTemplate(*groupUid, toUse);
	    if (result)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while committing action**" << endl;
		else
		{
		    res = theObject.unpack(toUse);
		    if (res)
			cout << "Group " << *groupUid << "\n" << theObject;
		    else
			cerr << "**An error occurred while unpacking template**" << endl;
		}
		
		break;
	    }
	    else
	    {
		cerr << "\n**An error occurred while attempting to add template**" << endl;
		A.Abort();
	    }
	}
	break;
	case 'O':
	case 'o':
        {
	    AtomicAction A;
	    ReplicaDescriptor rd, *ptr = 0;
	    char groupString[uidSize];
	    Uid *toUse = 0, *replicaGUid = 0;
	    char Name[hostSize];
	    char tn[pathSize];
	    int i;
	    
	    if (group)
		delete group;
	    
	    cout << "\nWhat is the Uid of the template group to add to?" << endl;
	    ::memset(groupString, '\0', uidSize);
	    cin >> groupString;
	    group = new Uid(groupString);
	    
	    cout << "\nWhat is the type of the object(s)?" << endl;
	    ::memset(tn, '\0', pathSize);
	    cin >> tn;
	    
	    cout << "\nWhat is the Uid of the replica's group?" << endl;
	    ::memset(groupString, '\0', uidSize);
	    cin >> groupString;
	    replicaGUid = new Uid(groupString);
	    
	    cout << "\nHow many replicas are there to add?" << endl;
	    cin >> i;
	    
	    for (int j = 0; j < i; j++)
	    {
		if (ptr == 0)
		    ptr = &rd;
		else
		{
		    ptr->next = new ReplicaDescriptor;
		    ptr = ptr->next;
		}
		
		cout << "\nWhat is the Uid for object " << j+1 << "?" << endl;
		::memset(groupString, '\0', uidSize);
		cin >> groupString;
		if (toUse)
		    delete toUse;
		toUse = new Uid(groupString);
		ptr->setUid(*toUse);
		ptr->setGroupUid(*replicaGUid);
		ptr->setObjectName(tn);
		ptr->setHost((char*) templateName);
	    }
	    
	    A.Begin();
	    
	    result = GV->addReplicas(rd, *group);
	    
	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while committing action**" << endl;
		else
		    cout << "\nAdd replicas successfully" << endl;
		break;
	    }
	    else
	    {
		cerr << "\n**An error occurred while performing opertion**" << endl;
		A.Abort();
	    }
	}
	break;
	case 'P':
	case 'p':
        {
	    AtomicAction A;
	    ReplicaDescriptor rd, *ptr = 0;
	    char groupString[uidSize];
	    Uid *toUse = 0, *replicaGUid = 0;
	    char Name[hostSize];
	    char tn[pathSize];
	    int i;
	    
	    if (group)
		delete group;
	    
	    cout << "\nWhat is the Uid of the template group to delete from?" << endl;
	    ::memset(groupString, '\0', uidSize);
	    cin >> groupString;
	    group = new Uid(groupString);
	    
	    cout << "\nWhat is the type of the object(s) to removed?" << endl;
	    ::memset(tn, '\0', pathSize);
	    cin >> tn;
	    
	    cout << "\nWhat is the Uid of the replica's group?" << endl;
	    ::memset(groupString, '\0', uidSize);
	    cin >> groupString;
	    replicaGUid = new Uid(groupString);
	    
	    cout << "\nHow many replicas are there to delete?" << endl;
	    cin >> i;
	    
	    for (int j = 0; j < i; j++)
	    {
		if (ptr == 0)
		    ptr = &rd;
		else
		{
		    ptr->next = new ReplicaDescriptor;
		    ptr = ptr->next;
		}
		
		cout << "\nWhat is the Uid for object " << j+1 << "?" << endl;
		::memset(groupString, '\0', uidSize);
		cin >> groupString;
		if (toUse)
		    delete toUse;
		toUse = new Uid(groupString);
		ptr->setUid(*toUse);
		ptr->setGroupUid(*replicaGUid);
		ptr->setObjectName(tn);
		ptr->setHost((char*) templateName);
	    }

	    A.Begin();
	    
	    result = GV->deleteReplicas(rd, *group);
	    
	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while committing action**" << endl;
		else
		    cout << "\nDeleted replicas successfully" << endl;
		break;
	    }
	    else
	    {
		cerr << "\n**An error occurred during operation**" << endl;
		A.Abort();
	    }
	}
	break;
	case 'Q':
	case 'q':
        {
	    char opt2;
	    char uidString[uidSize];
	    Uid* groupUid = 0;
	    AtomicAction A;
	    
	    ::memset(uidString, '\0', uidSize);
	    
	    cout << "A/a: Assign Group Uid" << endl;
	    cout << "B/b: Assign New Group\n" << endl;
	    cin >> opt2;
	    
	    switch (opt2)
	    {
	    case 'A':
	    case 'a':
	    {
		Uid* assignTo = 0;
		
		cout << "What is the Uid of the group to assign to?" << endl;
		cin >> uidString;
		groupUid = new Uid(uidString);
		
		::memset(uidString, '\0', uidSize);
		cout << "What is the Uid of the group to assign to " << *groupUid << "?" << endl;
		cin >> uidString;
		assignTo = new Uid(uidString);
		
		A.Begin();
		
		result = GV->assignCompositeObject(*groupUid, *assignTo);
		if (result == Done)
		{
		    if (A.End() != COMMITTED)
			cerr << "\n**An error occurred while committing action**" << endl;
		    else
			cout << "\nAssigned groups successfully" << endl;
		    break;
		}
		else
		{
		    cerr << "\n**An error occurred while attempting to assign**" << endl;
		    A.Abort();
		}
	    }
	    break;
	    case 'B':
	    case'b':
	    {
		ReplicaDescriptor rd, *ptr = 0;
		char hostName[hostSize];
		Uid *assignTo = 0, *tempUid = 0;
		int i;

		cout << "\nWhat group do you want to assign to?" << endl;
		::memset(uidString, '\0', uidSize);
		cin >> uidString;
		assignTo = new Uid(uidString);
		
		cout << "\nHow many replicas are in the group to add?" << endl;
		cin >> i;

		cout << "\nWhat is the Uid of the group?" << endl;
		::memset(uidString, '\0', uidSize);
		cin >> uidString;
		if (groupUid)
		    delete groupUid;
		groupUid = new Uid(uidString);
		
		for (int j = 0; j < i; j++)
		{
		    if (ptr == 0)
			ptr = &rd;
		    else
		    {
			ptr->next = new ReplicaDescriptor;
			ptr = ptr->next;
		    }
		    
		    ptr->setGroupUid(*groupUid);
		    
		    cout << "\nWhat is the Uid for replica " << j << "?" << endl;
		    ::memset(uidString, '\0', uidSize);
		    cin >> uidString;
		    if (tempUid)
			delete tempUid;
		    tempUid = new Uid(uidString);
		    
		    ptr->setUid(*tempUid);
		    
		    cout << "\nWhat is the location of the replica?" << endl;
		    ::memset(hostName, '\0', hostSize);
		    cin >> hostName;
		    
		    ptr->setHost(hostName);
		    
		    cout << "\nWhat is the replica type?" << endl;
		    ::memset(hostName, '\0', hostSize);
		    cin >> hostName;
		    
		    ptr->setObjectName(hostName);
		}
		
		A.Begin();
		
		result = GV->assignCompositeObject(*assignTo, rd);
		
		if (result == Done)
		{
		    if (A.End() != COMMITTED)
			cerr << "\n**An error occurred while committing action**" << endl;
		    else
			cout << "\nAssigned groups successfully" << endl;
		    break;
		}
		else
		{
		    cerr << "\n**An error occurred while attempting to assign**" << endl;
		    A.Abort();
		}
	    }
	    break;
	    default:
	    break; }
	}
	break;
	case 'R':
	case 'r':
        {
	    AtomicAction A;
	    char opt2;
	    char uidString[uidSize];
	    Uid* groupUid = 0;

	    cout << "\nA/a: Remove Group Uid" << endl;
	    cout << "B/b: Remove Group\n" << endl;
	    cin >> opt2;
	 
	    switch (opt2)
	    {
	    case 'A':
	    case 'a':
	    {
		Uid* toRemove;
		
		cout << "\nWhat is the Uid of the group to remove from?" << endl;
		::memset(uidString, '\0', uidSize);
		cin >> uidString;
		groupUid = new Uid(uidString);
		
		cout << "\nWhat is the Uid of the group to remove?" << endl;
		::memset(uidString, '\0', uidSize);
		cin >> uidString;
		toRemove = new Uid(uidString);
		
		A.Begin();
		
		result = GV->removeCompositeObject(*groupUid, *toRemove);
		
		if (result == Done)
		{
		    if (A.End() != COMMITTED)
			cerr << "\n**An error occurred while committing action**" << endl;
		    else
			cout << "\nRemoved group successfully" << endl;
		    break;
		}
		else
		{
		    cerr << "\n**An error occurred while trying to remove group" << endl;
		    A.Abort();
		}
	    }
	    break;
	    case 'B':
	    case 'b':
	    {
		ReplicaDescriptor rd, *ptr = 0;
		char hostName[hostSize];
		Uid *toRemove = 0, *tempUid = 0;
		int i;
		
		cout << "\nWhat is the Uid of the group to remove from?" << endl;
		::memset(uidString, '\0', uidSize);
		cin >> uidString;
		groupUid = new Uid(uidString);
		
		cout << "\nHow many replicas are in the group to remove?" << endl;
		cin >> i;

		cout << "\nWhat is the Uid of the group to remove?" << endl;
		::memset(uidString, '\0', uidSize);
		cin >> uidString;
		toRemove = new Uid(uidString);
		
		for (int j = 0; j < i; j++)
		{
		    if (ptr == 0)
			ptr = &rd;
		    else
		    {
			ptr->next = new ReplicaDescriptor;
			ptr = ptr->next;
		    }
		    
		    ptr->setGroupUid(*groupUid);
		    
		    cout << "\nWhat is the Uid for replica " << j << "?" << endl;
		    ::memset(uidString, '\0', uidSize);
		    cin >> uidString;
		    if (tempUid)
			delete tempUid;
		    tempUid = new Uid(uidString);
		    
		    ptr->setUid(*tempUid);
		    
		    cout << "\nWhat is the location of the replica?" << endl;
		    ::memset(hostName, '\0', hostSize);
		    cin >> hostName;
		    
		    ptr->setHost(hostName);

		    cout << "\nWhat is the replica type?" << endl;
		    ::memset(hostName, '\0', hostSize);
		    cin >> hostName;

		    ptr->setObjectName(hostName);
		}

		A.Begin();
		
		result = GV->removeCompositeObject(*toRemove, rd);
		
		if (result == Done)
		{
		    if (A.End() != COMMITTED)
			cerr << "\n**An error occurred while committing action**" << endl;
		    else
			cout << "\nRemoved group successfully" << endl;
		    break;
		}
		else
		{
		    cerr << "\n**An error occurred while attempting to remove**" << endl;
		    A.Abort();
		}   
	    }
	    break;
	    default:
	    break; }
	}
        break;
        case 'X':
	case 'x':
	finished = TRUE;
	break;
        default:
	break; }
    }

    if (GV)
	delete GV;

    return 0;
}
