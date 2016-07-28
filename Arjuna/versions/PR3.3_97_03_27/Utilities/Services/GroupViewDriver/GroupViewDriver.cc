/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupViewDriver.cc,v 1.13 1995/10/13 14:38:51 nmcl Exp $
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

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState_stub.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
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

const char* defaultRoot = ".";

NSInterface* GV = (NSInterface*) 0;
Uid *group = (Uid*) 0, *groupUid = (Uid*) 0;
char uidString[uidSize], hostName[hostSize], groupString[uidSize];
char tn[pathSize], Name[hostSize];
char rootOfStore[pathSize];
ObjectState* nameserverState = (ObjectState*) 0;

OutCome result = NOTDONE;
Boolean res;


void reset ()
{
    ::memset(uidString, '\0', uidSize);
    ::memset(hostName, '\0', hostSize);
    ::memset(groupString, '\0', uidSize);
    ::memset(tn, '\0', pathSize);
    ::memset(Name, '\0', hostSize);
    ::memset(rootOfStore, '\0', pathSize);
    if (groupUid)
    {
        delete groupUid;
	groupUid = 0;
    }
    if (group)
    {
        delete group;
	group = 0;
    }
}

void getUid (ReplicaDescriptor& rd)
{
    char* u_id = ::new char[uidSize];
    ::memset(u_id, '\0', uidSize);

    cin >> u_id;
    Uid u(u_id);
    rd.setUid(u);
}

void complex1 (char opt)
{
    Buffer packInto;
    GroupData *toUse = (GroupData*) 0, *ptr = (GroupData*) 0;
    char* templateType = (char*) 0;
    AtomicAction A;
    int i = 0;
    char opt2;
	    
    reset();
    
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
	if (ptr == (GroupData*) 0)
	{
	    toUse = new GroupData;
	    ptr = toUse;
	}
	else
	{
	    ptr->next = new GroupData;
	    ptr = ptr->next;
	}
		
	cout << "\nWhat is host name " << j+1 << "?" << endl;
	::memset(hostName, '\0', hostSize);
	cin >> hostName;
	ptr->setHost(hostName);
	ptr->setObjectName(templateType);
    }
	    
    res = toUse->pack(packInto);
    delete toUse;

    if (!res)
    {
	cerr << "\n**An error occurred while packing template**" << endl;
	return;
    }
    
    A.Begin();
	    
    if ((opt == 'K') || (opt == 'k'))
	result = GV->addTemplate(*groupUid, packInto);
    else
	result = GV->replaceTemplate(*groupUid, packInto);
	    
    if (result == DONE)
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
    }
    else
    {
	cerr << "\n**An error occurred while attempting to add template**" << endl;
	A.Abort();
    }
}

void complex2 ()
{
    AtomicAction A;
	    
    reset();
	    
    cout << "\nWhat is the group Uid?" << endl;
    cin >> uidString;
    groupUid = new Uid(uidString);
	    
    A.Begin();
	    
    result = GV->deleteTemplate(*groupUid);
    if (result == DONE)
    {
	if (A.End() != COMMITTED)
	    cerr << "\n**An error occurred while committing action**" << endl;
	else
	    cout << "\nTemplate deleted successfully" << endl;
    }
    else
    {
	cerr << "\n**An error occurred while attempting to delete template**" << endl;
	A.Abort();
    }
}

void complex3 ()
{
    AtomicAction A;
    Buffer toUse;
    GroupData theObject;
	    
    reset();
	    
    cout << "\nWhat is the Uid of the object to inspect?" << endl;
    cin >> uidString;
    groupUid = new Uid(uidString);
	    
    A.Begin();
	    
    res = GV->getTemplate(*groupUid, toUse);
    if (res)
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
    }
    else
    {
	cerr << "\n**An error occurred while attempting to add template**" << endl;
	A.Abort();
    }
}

void complex4 ()
{
    AtomicAction A;
    ReplicaDescriptor  rd;
    ReplicaDescriptor* ptr = (ReplicaDescriptor*) 0;
    Uid *toUse = (Uid*) 0, *replicaGUid = (Uid*) 0;
    int i;
    char* root = (char*) 0;

    reset();
	    
    cout << "\nWhat is the Uid of the template group to add to?" << endl;
    cin >> groupString;
    group = new Uid(groupString);
	    
    cout << "\nWhat is the type of the object(s)?" << endl;
    cin >> tn;
	    
    cout << "\nWhat is the Uid of the replica's group?" << endl;
    ::memset(groupString, '\0', uidSize);
    cin >> groupString;
    replicaGUid = new Uid(groupString);

    cout << "\nWhat is the relative root of the object store ('.' for default)?" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	root = rootOfStore;
		
    cout << "\nHow many replicas are there to add?" << endl;
    cin >> i;
	    
    for (int j = 0; j < i; j++)
    {
	if (ptr == (ReplicaDescriptor*) 0)
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
	ptr->setStoreRoot(root);
    }
	    
    A.Begin();
	    
    result = GV->addReplicas(rd, *group);
	    
    if (result == DONE)
    {
	if (A.End() != COMMITTED)
	    cerr << "\n**An error occurred while committing action**" << endl;
	else
	    cout << "\nAdd replicas successfully" << endl;
    }
    else
    {
	cerr << "\n**An error occurred while performing opertion**" << endl;
	A.Abort();
    }
}

void complex5 ()
{
    AtomicAction A;
    ReplicaDescriptor  rd;
    ReplicaDescriptor* ptr = (ReplicaDescriptor*) 0;
    Uid *toUse = (Uid*) 0, *replicaGUid = (Uid*) 0;
    int i;
    char* root = (char*) 0;

    reset();
	    
    cout << "\nWhat is the Uid of the template group to delete from?" << endl;
    cin >> groupString;
    group = new Uid(groupString);
	    
    cout << "\nWhat is the type of the object(s) to removed?" << endl;
    cin >> tn;
	    
    cout << "\nWhat is the Uid of the replica's group?" << endl;
    ::memset(groupString, '\0', uidSize);
    cin >> groupString;
    replicaGUid = new Uid(groupString);

    cout << "\nWhat is the relative root of the object store ('.' for default)?" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	root = rootOfStore;
	    
    cout << "\nHow many replicas are there to delete?" << endl;
    cin >> i;
	    
    for (int j = 0; j < i; j++)
    {
	if (ptr == (ReplicaDescriptor*) 0)
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
	ptr->setStoreRoot(root);
	ptr->setHost(templateName);
    }

    A.Begin();
	    
    result = GV->deleteReplicas(rd, *group);
	    
    if (result == DONE)
    {
	if (A.End() != COMMITTED)
	    cerr << "\n**An error occurred while committing action**" << endl;
	else
	    cout << "\nDeleted replicas successfully" << endl;
    }
    else
    {
	cerr << "\n**An error occurred during operation**" << endl;
	A.Abort();
    }
}

void complex6 ()
{
    char opt2;
    AtomicAction A;
	    
    reset();
	    
    cout << "A/a: Assign Group Uid" << endl;
    cout << "B/b: Assign New Group\n" << endl;
    cin >> opt2;
	    
    switch (opt2)
    {
    case 'A':
    case 'a':
	{
	    Uid* assignTo = (Uid*) 0;
		
	    cout << "What is the Uid of the group to assign to?" << endl;
	    cin >> uidString;
	    groupUid = new Uid(uidString);
		
	    cout << "What is the Uid of the group to assign to " << *groupUid << "?" << endl;
	    ::memset(uidString, '\0', uidSize);
	    cin >> uidString;
	    assignTo = new Uid(uidString);
		
	    A.Begin();
		
	    result = GV->assignCompositeObject(*groupUid, *assignTo);
	    if (result == DONE)
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
	    ReplicaDescriptor rd, *ptr = (ReplicaDescriptor *) 0;
	    Uid *assignTo = (Uid *) 0, *tempUid = (Uid *) 0;
	    int i;

	    reset();

	    cout << "\nWhat group do you want to assign to?" << endl;
	    cin >> uidString;
	    assignTo = new Uid(uidString);
		
	    cout << "\nHow many replicas are in the group to add?" << endl;
	    cin >> i;

	    cout << "\nWhat is the Uid of the group?" << endl;
	    ::memset(uidString, '\0', uidSize);
	    cin >> uidString;
	    groupUid = new Uid(uidString);
		
	    for (int j = 0; j < i; j++)
	    {
		if (ptr == (ReplicaDescriptor *) 0)
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
		cin >> hostName;
		    
		ptr->setHost(hostName);
		
		cout << "\nWhat is the replica type?" << endl;
		::memset(hostName, '\0', hostSize);
		cin >> hostName;
		    
		ptr->setObjectName(hostName);
		
		cout << "\nWhat is the relative root of the object store ('.' for default)?" << endl;
		::memset(rootOfStore, '\0', pathSize);
		cin >> rootOfStore;

		if (::strcmp(rootOfStore, defaultRoot) != 0)
		    ptr->setStoreRoot(rootOfStore);
	    }
		
	    A.Begin();
		
	    result = GV->assignCompositeObject(*assignTo, rd);
		
	    if (result == DONE)
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
	break;
    }
}

void complex7 ()
{
    AtomicAction A;
    char opt2;

    reset ();

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
	    cin >> uidString;
	    groupUid = new Uid(uidString);
		
	    cout << "\nWhat is the Uid of the group to remove?" << endl;
	    ::memset(uidString, '\0', uidSize);
	    cin >> uidString;
	    toRemove = new Uid(uidString);
		
	    A.Begin();
		
	    result = GV->removeCompositeObject(*groupUid, *toRemove);
		
	    if (result == DONE)
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
	    ReplicaDescriptor rd, *ptr = (ReplicaDescriptor *) 0;
	    Uid *toRemove = (Uid *) 0, *tempUid = (Uid *) 0;
	    int i;
		
	    cout << "\nWhat is the Uid of the group to remove from?" << endl;
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
		if (ptr == (ReplicaDescriptor*) 0)
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
		cin >> hostName;
		    
		ptr->setHost(hostName);

		cout << "\nWhat is the replica type?" << endl;
		::memset(hostName, '\0', hostSize);
		cin >> hostName;

		ptr->setObjectName(hostName);

		cout << "\nWhat is the relative root of the object store?" << endl;
		::memset(rootOfStore, '\0', pathSize);
		cin >> rootOfStore;

		if (::strcmp(rootOfStore, defaultRoot) != 0)
		    ptr->setStoreRoot(rootOfStore);
	    }

	    A.Begin();
		
	    result = GV->removeCompositeObject(*toRemove, rd);
		
	    if (result == DONE)
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
	break;
    }
}

void complexObject (char opt)
{
    switch (opt)
    {
        case 'K':
        case 'k':
        case 'M':
        case 'm':
	    complex1(opt);
	    break;
	case 'L':
	case 'l':
	    complex2();
	    break;
	case 'N':
	case 'n':
	    complex3();
	    break;
	case 'O':
	case 'o':
	    complex4();
	    break;
	case 'P':
	case 'p':
	    complex5();
	    break;
	case 'Q':
	case 'q':
	    complex6();
	    break;
	case 'R':
	case 'r':
	    complex7();
	    break;
    }
}

void op1 (const char* myHostName, pid_t pid, long UseCount)
{
    ReplicaDescriptor* rd = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* includeList = (ReplicaDescriptor*) 0;
    AtomicAction A;
    int position = 0;
    Uid groupIdentifier;

    result = DONE;

    A.Begin();
	    
    while (result == DONE)
    {
	if (rd)
	    delete rd;
		
	rd = new ReplicaDescriptor;

	result = ((GV->traverseDatabase(*rd, groupIdentifier, myHostName, position, pid) == TRUE) ? DONE : NOTDONE);
	if (result == DONE)
	    result = GV->releaseGroup(groupIdentifier, myHostName, pid);
	if (result == DONE)
	    UseCount = GV->getUseCount(groupIdentifier);
	
	if (result != DONE)
	{
	    cerr << "\n**An error has occurred while traversing the database**\n" << flush;
	    A.Abort();
	    return;
	}

	if (position == -1)
	{
	    cout << "\nEnd of database reached\n" << flush;
	    if (A.End() != COMMITTED)
		cerr << "\n**An error has occurred while committing action**\n" << flush;
	    return;
	}

	if (includeList)
	{
	    delete includeList;
	    includeList = (ReplicaDescriptor*) 0;
	}
	rd->getGroupList(groupIdentifier, includeList);
	
	cout << "\n****\nEntry " << position << "\n" << flush;
	cout << "Replica group UID " << groupIdentifier << " with the following members:\n" << endl;

	cout << *rd;
	
	cout << "\n****\nUse count is : " << UseCount << endl;

	if (includeList)
	{
	    delete includeList;
	    includeList = (ReplicaDescriptor*) 0;
	}
    }

    if (rd)
	delete rd;
}

void op2 (const char* myHostName, pid_t pid, long UseCount)
{
    Uid old;
    ReplicaDescriptor  rd;
    ReplicaDescriptor* excludeList = (ReplicaDescriptor*) 0;

    cout << "\nGive UID of group view to inspect\n" << flush;
    getUid(rd);
    old = rd.getUid();

    cout << "\nScanning for " << old << "\n" << endl;

    AtomicAction A;

    A.Begin();
	    
    result = ((GV->getView(rd, old, myHostName, pid) == TRUE) ? DONE : NOTDONE);

    result = GV->releaseGroup(old, myHostName, pid);
    UseCount = GV->getUseCount(old);
	    
    if (result != DONE)
    {
	cerr << "\n**An error has occurred during getView**\n" << flush;
	A.Abort();
	return;
    }
    else
    {
	if (A.End() != COMMITTED)
	{
	    cerr << "\n**An error has occurred while committing the action**\n" << flush;
	    return;
	}

	if (result == DONE)
	{
	    ReplicaDescriptor* objects = (ReplicaDescriptor*) 0;
	    
	    rd.getGroupList(old, objects);

	    cout << "\nThe replica group with UID " << old << " has the following active members:\n" << endl;
	    if (objects == (ReplicaDescriptor*) 0)
		cout << rd;
	    else
		cout << *objects;

	    if (excludeList)
	    {
		delete excludeList;
		excludeList = (ReplicaDescriptor*) 0;
	    }
	    rd.getExcludedList(excludeList);
		
	    cout << "\n****\nExclude List is:";
	    if (excludeList == (ReplicaDescriptor*) 0)
		cout << " Empty" << endl;
	    else
		cout << "\n\n" << *excludeList << endl;

	    cout << "\n****\n" << endl;

	    cout << "Use count is : " << UseCount << endl;
	    delete objects;
	}
	else
	    cout << "\nEntry not found in database\n" << flush;
    }
}

void op3 ()
{
    AtomicAction A;
    ReplicaDescriptor rd;

    reset();
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

    cout << "\nGive the relative root of the object store ('.' for default)" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	rd.setStoreRoot(rootOfStore);
	    
    A.Begin();

    result = GV->addReplicas(rd, *group);
	    
    if (result == DONE)
    {
	cout << "\nAdded group view successfully\n" << flush;
	
	if (A.End() == COMMITTED)
	    return;
	else
	{
	    cerr << "\n**An error has occurred while committing the action**\n" << flush;
	    return;
	}
    }
    else
    {
	cerr << "\n**An error occurred while attempting to add group view**\n" << flush;
	A.Abort();
    }
}

void op4 ()
{
    AtomicAction A;
    ReplicaDescriptor rd;
    Uid *uidDelete = (Uid*) 0, u(NIL_UID);

    reset();

    cout << "\nGive Uid of Replica Group to delete\n" << flush;
    cin >> uidString;

    uidDelete = new Uid(uidString);
    rd.setUid(u);

    A.Begin();

    result = GV->deleteReplicas(rd, *uidDelete, TRUE);

    delete uidDelete;

    if (result == DONE)
    {
	cout << "\nDeleted group successfully\n" << flush;
	if (A.End() == COMMITTED)
	    return;
	else
	{
	    cerr << "\n**An error has occurred while committing the action**\n" << flush;
	    return;
	}
    }
    else
    {
	cerr << "\n**An error has occurred while attempting to delete group\n" << flush;
	A.Abort();
    }
}

void op5 ()
{
    AtomicAction A;
    ReplicaDescriptor rd;

    reset();

    cout << "\nGive Uid of Replica Group\n" << flush;
    cin >> groupString;
    group = new Uid(groupString);
    rd.setGroupUid(*group);

    cout << "\nGive the object's Uid\n" << flush;
    getUid(rd);

    cout << "\nGive the location of the object\n" << flush;
    cin >> Name;

    rd.setHost(Name);

    cout << "\nGive the object's type" << endl;
    cin >> tn;
    rd.setObjectName(tn);

    cout << "\nGive the relative root of the object store ('.' for default)" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	rd.setStoreRoot(rootOfStore);
	    
    A.Begin();

    result = GV->addReplicas(rd, *group);

    if (result == DONE)
    {
	cout << "\nAdded successfully\n" << flush;

	if (A.End() == COMMITTED)
	    return;
	else
	{
	    cerr << "\n**An error has occurred while committing the action**\n" << flush;
	    return;
	}
    }
    else
    {
	cerr << "\n**An error occurred while attempting to add entry**\n" << flush;
	A.Abort();
    }
}

void op6 ()
{
    ReplicaDescriptor rd;

    reset();

    cout << "\nGive UID of group view to delete from\n" << flush;
    cin >> groupString;
    group = new Uid(groupString);
    rd.setGroupUid(*group);

    cout << "\nGive the Uid of the replica to be deleted\n" << flush;
    getUid(rd);

    cout << "\nGive the location of the object to delete\n" << flush;
    cin >> Name;

    rd.setHost(Name);

    cout << "\nGive the name of the object" << endl;
    cin >> tn;
    rd.setObjectName(tn);

    cout << "\nGive the relative root of the object store ('.' for default)" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	rd.setStoreRoot(rootOfStore);
	    
    AtomicAction A;
    
    A.Begin();

    result = GV->deleteReplicas(rd, *group);

    if (result == DONE)
    {
	cout << "\nDeleted entry successfully\n" << flush;
		    
	if (A.End() == COMMITTED)
	    return;
	else
	{
	    cerr << "\n**An error has occurred while committing the action**\n" << flush;
	    return;
	}
    }
    else
    {
	cerr << "\n**An error occurred while deleting entry**\n" << flush;
	A.Abort();
    }
}

void op7 ()
{
    AtomicAction A;
    Uid *ud = new Uid(NIL_UID);
    ReplicaDescriptor rd;

    reset();
	    
    cout << "\nGive UID of group to exclude replica from\n" << flush;
    getUid(rd);
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

    cout << "\nGive the relative root of the object store ('.' for default)" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	rd.setStoreRoot(rootOfStore);
	    
    A.Begin();
    
    rd.setActive(FALSE);
    result = GV->exclude(rd, *ud);

    if (result == DONE)
    {
	cout << "\nExcluded entry successfully\n" << flush;

	if (A.End() == COMMITTED)
	    return;
	else
	{
	    cerr << "**\nAn error has occurred while committing the action**\n" << flush;
	    return;
	}
    }
    else
    {
	cerr << "\n**An error occurred while excluding**\n" << flush;
	A.Abort();
    }
}

void op8 ()
{
    Uid *ud = new Uid(NIL_UID);
    ReplicaDescriptor rd;
    AtomicAction A;
	    
    reset();
    
    cout << "\nGive UID of group to include replica in\n" << flush;
    getUid(rd);
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

    cout << "\nGive the relative root of the object store ('.' for default)" << endl;
    cin >> rootOfStore;

    if (::strcmp(rootOfStore, defaultRoot) != 0)
	rd.setStoreRoot(rootOfStore);

    A.Begin();

    rd.setActive(TRUE);
    result = GV->include(rd, *ud);

    if (result == DONE)
    {
	cout << "\nIncluded entry successfully\n" << flush;
	
	if (A.End() == COMMITTED)
	    return;
	else
	{
	    cerr << "\n**An error has occurred while committing the action**\n" << flush;
	    return;
	}
    }
    else
    {
	cerr << "\n**An error occurred while including**\n" << flush;
	A.Abort();
    }
}


int main (int, char**)
{
    Boolean finished = FALSE;
    int res = -1;
    pid_t pid;
    char myHostName[MAXHOSTNAMELEN];
    long UseCount = -1;
    
    GV = new NSInterface(res); // use old nameserver or create new one.

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
#ifdef DEBUG
	cout << "S/s: Get State"  << endl;
	cout << "T/t: Impose State" << endl;
#endif	
	cout << "\nZ/z: Quit\n" << endl;
	cout << "****\n" << endl;

	cin >> opt;

	switch (opt)
	{
	case 'a':
	case 'A':
	    op1(myHostName, pid, UseCount);
	    break;
	case 'b':
	case 'B':
	    op2(myHostName, pid, UseCount);
	    break;
	case 'c':
        case 'C':
	    op3();
	    break;
	case 'd':
        case 'D':
	    op4();
	    break;
	case 'e':
        case 'E':
	    op5();
	    break;
	case 'f':
        case 'F':
	    op6();
	    break;
	case 'g':
        case 'G':
	    op7();
	    break;
	case 'h':
        case 'H':
	    op8();
	    break;
	case 'i':
	case 'I':
        {
	    AtomicAction A;

	    cout << "\nWhat is the name of the recovering node?" << endl;
	    cin >> Name;
	    
	    A.Begin();
	    
	    result = GV->recover(Name);
	    
	    if (result == DONE)
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
	    AtomicAction A;
	    ReplicaDescriptor unknownList, rd;

	    reset();

	    cout << "\nWhat is the name of the node?" << endl;
	    cin >> Name;
	    
	    A.Begin();
	    
	    result = GV->getNodeUpdateList(rd, unknownList, Name);

	    if ((result == DONE) || (result == INUSE))
	    {
		if (A.End() != COMMITTED)
		    cerr << "\n**An error occurred while trying to commit action**" << endl;
		else
		{
		    cout << "\n****" << endl;
		    cout << "Needs updating :\n" << endl;
		    if (rd != NIL_RD)
			cout << rd << "\n" << endl;
		    else
			cout << "Null list." << endl;
		    cout << "\nMay need updating :\n" << endl;
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
	case 'L':
	case 'l':
	case 'N':
	case 'n':
	case 'O':
	case 'o':
	case 'P':
	case 'p':
	case 'Q':
	case 'q':
	case 'R':
	case 'r':
	complexObject(opt);
        break;
#ifdef DEBUG
	case 'S':
	case 's':
	    {
		AtomicAction A;

		if (nameserverState)
		    delete nameserverState;
		
		nameserverState = new ObjectState;
		
		A.Begin();

		if (GV->getGroupViewState(*nameserverState))
		{
		    if (A.End() == COMMITTED)
			cout << "Got state successfully." << endl;
		    else
			cout << "An error occurred during commit." << endl;
		}
		else
		{
		    cout << "An error occurred while getting state." << endl;
		    A.Abort();
		}
	    }
	break;
	case 'T':
	case 't':
	    {
		AtomicAction A;

		if (!nameserverState)
		    cout << "Error. No state to impose." << endl;
		else
		{
		    A.Begin();

		    if (GV->imposeGroupViewState(*nameserverState))
		    {
			if (A.End() == COMMITTED)
			    cout << "Imposed state successfully." << endl;
			else
			    cout << "An error occurred during commit." << endl;
		    }
		    else
		    {
			cout << "An error occurred while imposing state." << endl;
			A.Abort();
		    }
		}
	    }
	break;
#endif	
        case 'Z':
	case 'z':
	finished = TRUE;
	break;
        default:
	break; }
    }

    if (GV)
	delete GV;

    reset();

    if (nameserverState)
	delete nameserverState;
    
    return 0;
}
