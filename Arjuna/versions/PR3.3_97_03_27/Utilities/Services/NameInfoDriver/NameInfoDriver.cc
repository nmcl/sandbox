/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameInfoDriver.cc,v 1.12 1995/08/10 13:02:43 nmcl Exp $
 */

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif

#ifndef NAMEINFO_H_
#  include <ArjServers/NameInfo.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif


const TypeName storeName = DEFAULT_OBJECTSTORE_Q;

const int uid_Size = 80;
const int host_Size = MAXHOSTNAMELEN;


unsigned long hostInetAddr ()
{
    static unsigned long myAddr;
    static char hostname[MAXHOSTNAMELEN];

    struct hostent *h;
    
    ::gethostname(hostname, MAXHOSTNAMELEN);
    h = ::gethostbyname(hostname);
    ::memcpy(&myAddr, h->h_addr, h->h_length);

    return myAddr;
}

int main (int argc, char** argv)
{
    Boolean finished = FALSE, done = FALSE, newDatabase = FALSE;
    NameInfo* X = (NameInfo*) 0;
    unsigned long hostAddr = hostInetAddr();
    char* s_uid = ::new char[80];
    Boolean doReset = FALSE;

    ::memset(s_uid, '\0', 80);
    sprintf(s_uid, "%lx:0:0:0", hostAddr);
    Uid Y(s_uid);

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-new") == 0)
	{
	    X = new NameInfo(done);
	    newDatabase = TRUE;
	}
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: NameInforDriver [-new] [-reset]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-reset") == 0)
	    doReset = TRUE;
    }

    if (X == (NameInfo*) 0)
	X = new NameInfo(Y, done);

    if (!done)
    {
	cout << "Error while creating NameInfo object\n" << flush;
	cout << "If this is a new database setup then use the -new option." << endl;
	if (X)
	    delete X;
	return -1;
    }

    if (doReset)
    {
	if (X->reset() == FALSE)
	{
	    delete X;
	    cout << "Reset error." << endl;
	    return -1;
	}
	else
	    cout << "Reset ok." << endl;
    }

    while (!finished)
    {
	char opt;

	cout << "\n****" << endl;
	cout << "1: Get Replication Information\n" << flush;
	cout << "2: Set (overwrite) Replication Information\n" << flush;
	cout << "3: Add Entry" << endl;
	cout << "4: Remove Entry" << endl;
	cout << "5: Quit\n" << flush;

	cin >> opt;

	switch (opt)
	{
	case '1': {
	    AtomicAction B;
	    Boolean result = FALSE;
	    NameData *marker, *ptr;
	    int indx = 1, level = 0;

	    B.Begin();

	    marker = X->getReplicationInfo(result, level);

	    if (!result)
	    {
		cout << "\nError while attempting to get information\n" << flush;
		B.Abort();
		break;
	    }

	    if (level == 0)
		cout << "Database is empty." << endl;
	    else
	    {
		cout << endl;
		ptr = marker;
		while (ptr != 0)
		{
		    cout << "Entry " << indx << " has <" << ptr->hostname << ", "
			 << *ptr->myUID << ">\n" << flush;
		    ptr = ptr->next;
		    indx++;
		}
	    }

	    if (B.End() != COMMITTED)
		cout << "\nError while committing action\n" << flush;

	}
	break;
	case '2': {
	    AtomicAction B;
	    Boolean result = FALSE;
	    NameData *marker, *ptr;
	    int indx = 0;

	    cout << "\nHow many data entries?\n" << flush;
	    cin >> indx;
	    if (indx < 1)
	    {
		cout << "Number must be positive\n" << flush;
		break;
	    }

	    marker = new NameData;
	    ptr = marker;
	    for (int i = 1; i <= indx; i++)
	    {
		cout << "\nWhat is the hostname?\n" << flush;
		ptr->hostname = ::new char[host_Size];
		::memset(ptr->hostname, '\0', host_Size);
		cin >> ptr->hostname;

		cout << "\nWhat is the UID?\n" << flush;
		char u_id[uid_Size];
		::memset(u_id, '\0', uid_Size);
		cin >> u_id;
		ptr->myUID = new Uid(u_id);
		
		if (i+1 <= indx)
		{
		    ptr->next = new NameData;
		    ptr = ptr->next;
		}
	    }

	    B.Begin();

	    result = X->setReplicationInfo(marker);

	    if (!result)
	    {
		cout << "\nError while attempting to set information\n" << flush;
		B.Abort();
		break;
	    }

	    cout << "\nInformation set ok\n" << flush;
	    if (B.End() != COMMITTED)
		cout << "\nError while committing action\n" << flush;
	}
	break;
	case '3':
	{
	    AtomicAction A;
	    Boolean result = FALSE;
	    NameData toAdd;
	    
	    cout << "\nWhat is the hostname?" << endl;
	    toAdd.hostname = ::new char[host_Size];
	    ::memset(toAdd.hostname, '\0', host_Size);
	    cin >> toAdd.hostname;
	    
	    cout << "\nWhat is the UID?" << endl;
	    char u_id[uid_Size];
	    ::memset(u_id, '\0', uid_Size);
	    cin >> u_id;
	    toAdd.myUID = new Uid(u_id);
	    
	    A.Begin();

	    result = X->add(&toAdd);
	    
	    if (!result)
	    {
		cout << "\nError while attempting to add entry." << endl;
		A.Abort();
		break;
	    }
	    
	    cout << "\nEntry added ok" << endl;
	    if (A.End() != COMMITTED)
		cout << "\nError while committing action" << endl;
	}
	    break;
	case '4':
	{
	    AtomicAction A;
	    Boolean result = FALSE;
	    NameData toRemove;
	    
	    cout << "\nWhat is the hostname?" << endl;
	    toRemove.hostname = ::new char[host_Size];
	    ::memset(toRemove.hostname, '\0', host_Size);
	    cin >> toRemove.hostname;
	    
	    cout << "\nWhat is the UID?" << endl;
	    char u_id[uid_Size];
	    ::memset(u_id, '\0', uid_Size);
	    cin >> u_id;
	    toRemove.myUID = new Uid(u_id);
	    
	    A.Begin();
	    
	    result = X->remove(&toRemove);
	    if (!result)
	    {
		cout << "\nError while attempting to remove entry." << endl;
		A.Abort();
		break;
	    }
	    
	    cout << "\nEntry removed ok" << endl;
	    if (A.End() != COMMITTED)
		cout << "\nError while committing action" << endl;
	}
	    break;
	case '5':
	    finished = TRUE;
	    break;
	default:
	    break;
        }		   
    }

    // since this is a new database we must rename the state Uid.
    
    if (newDatabase)
    {
	const char* dbName = X->type();

	ObjectStore* toUse = ObjectStore::create(storeName);
	ObjectState* state = toUse->read_committed(X->get_uid(), X->type());

	if (state)
	{
	    if (!toUse->write_committed(Y, X->type(), *state))
		cout << "\nError while attempting to write NameInfo state."
		     << endl;
	    else
		toUse->remove_committed(X->get_uid(), X->type());
	}
	else
	    cout << "\nError - could not read NameInfo state." << endl;

	ObjectStore::destroy(toUse);
    }

    if (X)
	delete X;
}

